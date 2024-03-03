# EC87

## **DEPRECATED**

This project has been deprecated in favor of the next TKL I'm working on.
It will not be updated nor maintained from now on, so do no base projects, them being PCB or cases, on it.
The project of reference for EC TKL will be named (provisional for now, in search of a better name still): ECip TKL
This new design will join the EC60 in the reference design family of PCBs that I'm working on. Think of these as the EC equivalent of the H87/88 and H60 for MX switches. An FRL will be developed too, to complete the general layouts. Both will feature the same key and cluster spacing as the H87/88 PCBs, so that future design of cases can have both MX and EC options without compromising compatibility or having to develop separate cases/parts of cases for it.

## Introduction

EC87 is an Electrostatic Capacitive (EC) TKL keyboard PCB.

This project is a continuation of my development of open source EC boards after publishing the [Corne EC Revival](https://github.com/Cipulot/CorneECRevival).

Below is the KLE of the supported layout:

![EC87 KLE](/Docs/images/EC87_KLE.png)

Multi layout support is planned for Rev2.

## Technical information

- Layout size: tenkeykess (TKL)
- Compatible switches: EC switches (Topre and NIZ)
- Microcontroller: STM32F411
- Connector: detachable USB Type C
- Firmware compatibility: QMK (with VIA/VIAL support)
- Protection hardware:
  - Fused
  - ESD protection

## Renders and Prototypes

### EC87 PCB

#### Render

![EC87 PCB Top Render](/Docs/images/top.jpg)

![EC87 PCB Bottom Render](/Docs/images/bottom.jpg)

#### Prototype

![EC87 PCB Top Proto](/Docs/images/top_PCB.jpg)

![EC87 PCB Bottom Proto](/Docs/images/bottom_PCB.jpg)

### Plate

#### Render

![EC87 Plate Render](/Docs/images/plate.jpg)

#### Prototype

![EC87 Plate Proto](/Docs/images/plate_PCB.jpg)

### Backplate

#### Render

![EC87 Backplate Render](/Docs/images/backplate.jpg)

#### Prototype

![EC87 Backplate Proto](/Docs/images/backplate_PCB.jpg)

## Revisions and relative features

### Rev1

First revision of the board used for validation purposes of the circuit and QMK software.

Features include:

- Extension of the analog sensing circuit used in Corne EC Revival

- STM32F411 MCU for better performance, both cycle wise and in ADC performance compared to 32U4

- APC (Actuation Point Changer) with predefined actuation points and full scale analog switch control (full board / per key)

### Rev2

Rev 2 is already in the work and will have some radical changes, to allow better compatibility, layout options and performance improvements.

Features that will be included:

- Addition of multi-layout option (ANSI, ISO and different bottom rows)

- Rework of the analog circuit, basing it off a new "standard" model that is in development by [Gondolindrim](https://github.com/Gondolindrim) (absolute chad), myself and others. This so that we achieve a reliable enough design to enable more designers and more EC boards to get into the wild.

- Dedicated PCBs for specific MX boards already being run.

## WIP

Implementation of APC and initial calibration is in development. This features will allow to choose from a set op predetermined actuation points by simply cycling through them using a key combination. The initial calibration is to ensure that the overall baseline reading is equal across all the keys. Later, in Rev2, this will be used to adjust hardware gain through dedicated control circuit.

See commits in `Firmware` for details about the state of development.

## License

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.
