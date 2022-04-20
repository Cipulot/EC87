# EC87

![EC87](https://github.com/Cipulot/EC87/blob/main/Docs/images/top_PCB.jpg?raw=true)

EC version of the H87x TKL pcb.

* Keyboard Maintainer: [cipulot](https://github.com/cipulot)
* Hardware Supported: EC87
* Hardware Availability: TBD

Make example for this keyboard (after setting up your build environment):

    make ec87:default
    make ec87:via

Flashing example for this keyboard:

    make ec87:default:flash
    make ec87:via:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 2 main ways:

* **Physical reset pins**: Short circuit the pads marked as `BOOT0` on the top of the PCB and plug in the keyboard
* **Keycode in layout**: Press the key mapped to `RESET` if it is available.
