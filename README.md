# RTC-Based Smart Traffic Signal Scheduler

A menu-driven embedded traffic signal controller built around the LPC21xx ARM7 family, an on-chip RTC, a 4×4 keypad, a character LCD and 12 traffic LEDs for four directions.

## Project files
- `Source_Code/main.c` — RTC setup, traffic schedule configuration, custom schedules and Auto/Manual mode menu.
- `Source_Code/traffic.c/.h` — four-direction traffic sequence, schedule selection, green/yellow timing and manual control.
- `Source_Code/rtc.c/.h` — RTC initialization and time/date access.
- `Source_Code/LCD.c/.h` and `LCD_defines.h` — character LCD driver.
- `Source_Code/KPM.c/.h` and `KPM_defines.h` — 4×4 keypad driver.
- `Source_Code/delay.c/.h` — software delay functions.
- `Proteus/RTC_Traffic_Signal_System.DSN` — Proteus design file supplied in the archive.
- `Documentation/Project_Report.docx` — project documentation.
- `Images/system_architecture.png` — illustrative architecture diagram.

## Features visible in the supplied source
1. Four traffic directions, each with red, yellow and green outputs.
2. RTC-based main operating window with configurable start and stop time.
3. Automatic modes selected by time:
   - NORMAL: 06:00–17:00
   - PEAK: 17:00–22:00
   - NIGHT: 22:00–06:00
4. Two optional custom time windows with configurable green/yellow durations.
5. Keypad menu for RTC time, traffic schedule and custom modes.
6. Manual mode: keys 1–4 select a direction; key 5 sets all-red; C exits.
7. LCD display of time, active mode, direction and signal phase.

## Default timing constants in `traffic.h`
| Mode | Green | Yellow |
|---|---:|---:|
| Normal | 10 s | 3 s |
| Peak | 20 s | 3 s |
| Night | 5 s | 2 s |

The custom mode durations are entered through the keypad menu.

## Hardware mapping in supplied source
- Direction 1 LEDs: P0.14, P0.15, P0.16
- Direction 2 LEDs: P0.17, P0.18, P0.19
- Direction 3 LEDs: P0.20, P0.21, P0.22
- Direction 4 LEDs: P0.23, P0.24, P0.25
- Keypad rows: P1.16–P1.19
- Keypad columns: P1.20–P1.23
- LCD data: P0.0–P0.7; LCD control definitions are in `LCD_defines.h`.

## Build notes
The source includes LPC21xx device headers and is intended for an ARM7 LPC21xx toolchain (for example, Keil/ARMCC) and Proteus simulation. The archive contains a Proteus `.DSN` file, but this package has not been rebuilt or simulated here. Check the selected MCU in Proteus and the device header/toolchain before compiling.

## Important source notes
- The project source uses both `LPC21xx.h` and `lpc21xx.h` include spellings; case sensitivity may matter on some toolchains.
- The RTC source/header naming has been normalized to `RTC.h` in this package to match the include used by the C files; source contents are otherwise retained.
- The main file and traffic module are supplied source, not a newly rewritten or independently validated implementation.
- This is an educational simulation/prototype, not a certified real-road traffic controller. Do not connect LEDs/logic outputs directly to real traffic infrastructure.
