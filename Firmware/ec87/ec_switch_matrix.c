/* Copyright 2022 Cipulot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ec_switch_matrix.h"

#include "quantum.h"
#include "analog.h"
#include "atomic_util.h"
#include "print.h"

#define WAIT_DISCHARGE()
#define WAIT_CHARGE()

// pin connections
const uint32_t row_pins[]     = MATRIX_ROW_PINS;
const uint8_t  col_channels[] = MATRIX_COL_CHANNELS;
const uint32_t mux_sel_pins[] = MUX_SEL_PINS;

//_Static_assert(sizeof(mux_sel_pins) == 3, "invalid MUX_SEL_PINS");

static ecsm_config_t config;
static uint16_t      ecsm_sw_value[MATRIX_ROWS][MATRIX_COLS];

static inline void discharge_capacitor(void) { setPinOutput(DISCHARGE_PIN); }
static inline void charge_capacitor(uint8_t row) {
    setPinInput(DISCHARGE_PIN);
    writePinHigh(row_pins[row]);
}

static inline void clear_all_row_pins(void) {
    for (int row = 0; row < 6; row++) {
        writePinLow(row_pins[row]);
    }
}

static inline void init_mux_sel(void) {
    for (int idx = 0; idx < 3; idx++) {
        setPinOutput(mux_sel_pins[idx]);
    }
}

static inline void select_mux(uint8_t col) {
    uint8_t ch = col_channels[col];
    writePin(mux_sel_pins[0], ch & 1);
    writePin(mux_sel_pins[1], ch & 2);
    writePin(mux_sel_pins[2], ch & 4);
}

static inline void init_row(void) {
    for (int idx = 0; idx < 6; idx++) {
        setPinOutput(row_pins[idx]);
        writePinLow(row_pins[idx]);
    }
}

// Initialize pins
int ecsm_init(ecsm_config_t const* const ecsm_config) {
    // save config
    config = *ecsm_config;

    // initialize discharge pin as discharge mode
    writePinLow(DISCHARGE_PIN);
    setPinOutput(DISCHARGE_PIN);

    // initialize drive lines
    init_row();

    // initialize multiplexer select pin
    init_mux_sel();

    // Enable AMUX
    setPinOutput(APLEX_EN_PIN_0);
    writePinLow(APLEX_EN_PIN_0);
    setPinOutput(APLEX_EN_PIN_1);
    writePinLow(APLEX_EN_PIN_1);

    return 0;
}

// Read key value of key (channel, row, col) where channel 0 or 1
static uint16_t ecsm_readkey_raw(uint8_t channel, uint8_t row, uint8_t col) {
    uint16_t sw_value = 0;

    discharge_capacitor();

    if (channel == 0) {
        writePinHigh(APLEX_EN_PIN_0);
        select_mux(col);
        writePinLow(APLEX_EN_PIN_0);
    } else {
        writePinHigh(APLEX_EN_PIN_1);
        select_mux(col);
        writePinLow(APLEX_EN_PIN_1);
    }

    clear_all_row_pins();

    WAIT_DISCHARGE();

    // chSysLock();
    ATOMIC_BLOCK_FORCEON {
        charge_capacitor(row);

        WAIT_CHARGE();
        sw_value = analogReadPin(ANALOG_PORT_0);
    }
    // chSysUnlock();

    return sw_value;
}

// Update press/release state of key at (row, col)
static bool ecsm_update_key(matrix_row_t* current_row, uint8_t row, uint8_t col, uint16_t sw_value) {
    bool current_state = (*current_row >> col) & 1;

    // press to release
    if (current_state && sw_value < config.low_threshold_matrix[row][col]) {
        *current_row &= ~(1 << col);
        return true;
    }

    // release to press
    if ((!current_state) && sw_value > config.high_threshold_matrix[row][col]) {
        *current_row |= (1 << col);
        return true;
    }

    return false;
}

// Scan key values and update matrix state
bool ecsm_matrix_scan(matrix_row_t current_matrix[]) {
    bool updated = false;

    // COL 0 to COL sizeof(col_channels)

    // Disable AMUX of channel 1
    writePinHigh(APLEX_EN_PIN_1);
    for (int col = 0; col < sizeof(col_channels); col++) {
        for (int row = 0; row < 6; row++) {
            ecsm_sw_value[row][col] = ecsm_readkey_raw(0, row, col);
            updated |= ecsm_update_key(&current_matrix[row], row, col, ecsm_sw_value[row][col]);
        }
    }

    // COL sizeof(col_channels) + 1 to COL (sizeof(col_channels) + 1) + sizeof(col_channels)
    // Since row are shared simply shift + 8. For loop is the same since it's shared code to select the columns, in the ecsm_readkey_raw function. Shift + 8 the reading in the reading matrix to get the correct column.

    // Disable AMUX of channel 1
    writePinHigh(APLEX_EN_PIN_0);
    for (int col = 0; col < sizeof(col_channels); col++) {
        for (int row = 0; row < 6; row++) {
            ecsm_sw_value[row][col + 8] = ecsm_readkey_raw(1, row, col);
            updated |= ecsm_update_key(&current_matrix[row], row, col + 8, ecsm_sw_value[row][col + 8]);
        }
    }
    return updated;
}

// Debug print key values
void ecsm_print_matrix(void) {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 16; col++) {
            uprintf("%4d", ecsm_sw_value[row][col]);
            if (col < 15) {
                print(",");
            }
        }
        print("\n");
    }
    print("\n");
}
