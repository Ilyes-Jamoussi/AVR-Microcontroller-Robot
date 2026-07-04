# AVR Microcontroller Robot

[![CI](https://github.com/Ilyes-Jamoussi/AVR-Microcontroller-Robot/actions/workflows/ci.yml/badge.svg)](https://github.com/Ilyes-Jamoussi/AVR-Microcontroller-Robot/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

An autonomous line-following robot running on a bare **ATmega324PA** — no Arduino, no HAL.
The drivers (PWM motor control, IR line sensing, analog distance ranging, I2C EEPROM,
UART) are written in C++ at the register level from the AVR datasheet.

The robot navigates a taped course, figures out **where it started and which way it was
facing** by matching its recorded moves against known route signatures, detects posts
with an infrared distance sensor and stops 10 cm away, then replays a journey report
persisted in EEPROM over UART.

## Operating Modes

A mode is selected at boot by pressing the reset button while the status LED cycles
green → red → amber (2 s each):

| Mode | LED | Behavior |
|------|-----|----------|
| **1 — Search endpoint** | 🟢 | Placed at point A or B in any of four orientations, the robot follows the tape to an endpoint (C, D, or E), deduces its start point and initial orientation from the moves it made, then signals the endpoint with an LED color and a sound. |
| **2 — Cross course** | 🔴 | The robot drives toward a post detected by IR ranging and stops exactly 10 cm away. Each time the post is moved, a button press resumes the chase, until the robot finishes at endpoint E. |
| **3 — Report** | 🟠 | Replays the journey stored in EEPROM over UART: start point, initial orientation, endpoint reached, and the posts detected along the way. |

## How self-localization works

The robot does not know its starting position. While following the line it records every
decision it makes (move forward, turn left/right, junction probes) into a move sequence.
Because the course is a fixed graph, each *(start point, orientation)* pair produces a
unique signature — so the recorded sequence is matched against the eight known route
signatures (`lib/path`) to identify the start point, the initial orientation, and the
endpoint. The result is persisted to the I2C EEPROM for the report mode.

## Modules (`lib/`)

| Module | Role |
|--------|------|
| `motor` | wheel speed and direction via PWM |
| `linemarker` | line-following IR sensor array |
| `sharp` | Sharp IR distance sensor readings (via ADC), calibrated for the 10 cm stop |
| `led` | bicolor status LED (green / red / amber) |
| `soundplayer` | piezo tones for mode and event feedback |
| `button` | push-button input |
| `rs232` | UART transmission, including printf-style formatted output |
| `path` | move-sequence recording and route-signature matching |
| `robot` | mode orchestration and the EEPROM journey report |

Course-provided support code keeps its original authorship headers and (French) comments:
`lib/can` (ADC access, Khouzam & Collin), `lib/memoire_24` (I2C EEPROM, Lauzier), and the
Makefiles (Barrette & Collin, Polytechnique Montréal).

## Build

Requires `avr-gcc` and `avr-libc` (plus `avrdude` with a USBasp programmer to flash):

```sh
make -C lib          # static library (librobot.a)
make -C app          # firmware (robot.hex)
make -C app install  # flash to the microcontroller
```

CI builds the firmware with `avr-gcc` on every push.

## Hardware

- ATmega324PA @ 8 MHz
- IR line-follower sensors and a Sharp IR distance sensor
- Two DC motors driven by PWM
- Bicolor LED and piezo buzzer
- 24Cxxx-series I2C EEPROM
- USBasp programmer

## License

[MIT](LICENSE) — course-provided support files noted above retain their original headers.
