# Sign-to-Speech Glove — STM32 Blue Pill

## Project brief for developers and AI coding assistants

Build a modular sign-to-speech prototype migrated from an existing ESP32 implementation to an **STM32F103C8T6 Blue Pill using PlatformIO and the Arduino framework**. Four flex sensors are read through an ADS1115. Gesture recognition selects a command, the OLED displays it, and a DFPlayer Mini plays a locally stored voice recording through a 4 Ω, 3 W speaker.

This README is the source of truth for the intended hardware mapping and original behavior. Do not silently change command mapping, thresholds, pin assignments, or state behavior. If a hardware detail is uncertain, flag it and ask rather than guessing.

## 1. Required hardware

- STM32F103C8T6 Blue Pill
- Four resistive flex/bend sensors
- ADS1115 16-bit, four-channel I²C ADC breakout
- 0.91-inch horizontal I²C OLED; commonly SSD1306, 128 × 32 pixels (verify the actual module)
- DFPlayer Mini and FAT32 microSD card
- 4 Ω, 3 W speaker connected to the DFPlayer's SPK1/SPK2 outputs
- Status LED and 330 Ω series resistor (external LED)
- Buzzer (identify active/passive type and current before driving)
- Mode push button and optional separate start/stop push button
- IP5306 power-bank module and two Li-ion cells in parallel
- Available capacitors: 1000 µF, 25 V electrolytic; 104 ceramic (100 nF); 10 µF, 50 V
- Regulated power distribution, common ground, suitable wiring, and ST-Link V2 for SWD programming

## 2. Pin assignment

### STM32 GPIO and peripherals

| Function | STM32 pin | Wiring / notes |
|---|---|---|
| I²C1 SCL | PB6 | Connect to ADS1115 SCL and OLED SCL |
| I²C1 SDA | PB7 | Connect to ADS1115 SDA and OLED SDA |
| DFPlayer UART TX from MCU | PA2 / USART2_TX | Connect to DFPlayer RX through approximately 1 kΩ series resistor |
| DFPlayer UART RX to MCU | PA3 / USART2_RX | Connect to DFPlayer TX |
| External status LED | PB12 | PB12 -> 330 Ω resistor -> LED anode; LED cathode -> GND |
| Buzzer control | PB13 | Drive only if compatible with GPIO; use transistor/driver if required |
| Mode push button | PA0 | Button between PA0 and GND; use `INPUT_PULLUP` |
| Optional start/stop button | PA1 | Button between PA1 and GND; use `INPUT_PULLUP` |
| SWDIO programming | PA13 | ST-Link SWDIO |
| SWCLK programming | PA14 | ST-Link SWCLK |
| Ground | GND | Common ground for all modules and supplies |

The Blue Pill onboard LED is commonly on PC13, but this prototype's external status LED is assigned to PB12. Do not treat PB12 as the onboard LED.

### ADS1115 and flex sensors

| Sensor | ADS1115 input | Command |
|---|---|---|
| Flex 1 | A0 | “I want Coffee” |
| Flex 2 | A1 | “I want Water” |
| Flex 3 | A2 | “I want Food” |
| Flex 4 | A3 | “Go to Washroom” |
| Any three or four sensors bent together | A0–A3 combination | Emergency Alert |

Each flex sensor must be used in a voltage divider. Initial suggested arrangement: 3.3 V -> flex sensor -> junction (ADS1115 Ax) -> fixed resistor -> GND. Start by testing a 10 kΩ fixed resistor, but tune it against measured straight/bent sensor resistance. With this divider orientation, bending often lowers the junction voltage, but verify this on the actual sensors. Keep ADS1115 inputs between GND and its 3.3 V supply; never apply 5 V to its analog inputs.

### ADS1115 and OLED I²C

- ADS1115 VDD -> 3.3 V; GND -> common GND; SDA -> PB7; SCL -> PB6.
- OLED VCC -> the voltage supported by the actual module (use 3.3 V if supported); GND -> common GND; SDA -> PB7; SCL -> PB6.
- Common default addresses: ADS1115 `0x48`, OLED `0x3C`. Verify the OLED address and resolution on the actual board.
- I²C pull-ups must not pull SDA/SCL above 3.3 V.

### DFPlayer Mini and speaker

- DFPlayer VCC -> regulated 5 V; GND -> common GND.
- STM32 PA2 (USART2 TX) -> approximately 1 kΩ series resistor -> DFPlayer RX.
- DFPlayer TX -> STM32 PA3 (USART2 RX).
- Speaker connects between `SPK1` and `SPK2`; do not connect either speaker terminal to GND.
- The DFPlayer can drive a suitable small speaker through its speaker output. Do not connect the speaker to an STM32 GPIO.
- Use a stable 5 V supply and short, separate audio power/ground wiring.

## 3. Power architecture and decoupling

The prototype uses an IP5306 module powered by two Li-ion cells in parallel. The exact IP5306 board design, protection, current capability, and 5 V output rating must be checked before final design.

Recommended distribution:
1. IP5306 regulated 5 V output to a 5 V star/distribution point.
2. Separate positive and ground branches from that point to the DFPlayer and Blue Pill.
3. Power ADS1115, OLED (if supported), and flex dividers from 3.3 V; ensure common ground.
4. Keep speaker current loops away from sensor analog wiring.

Available capacitor placement:
- 1000 µF / 25 V electrolytic across DFPlayer VCC and GND, close to the module.
- 100 nF “104” ceramic in parallel with that capacitor, close to DFPlayer power pins.
- 10 µF / 50 V across the Blue Pill's 5 V input and GND if useful; check existing board decoupling.
- Observe electrolytic polarity: positive to supply, negative to GND.

Capacitors can reduce short transients but cannot fix an overloaded or poorly regulated boost converter. Verify the 5 V rail during audio playback with a multimeter and preferably an oscilloscope. Do not power the board simultaneously from USB and external 5 V unless the power arrangement safely supports it. Parallel Li-ion cells must be compatible, matched, and at nearly equal voltage before connection; use appropriate protection.

## 4. Original ESP32 behavior to preserve

The previous ESP32 prototype had the following command mapping:
- ADS1115 A0 / flex 1: “I want Coffee”
- ADS1115 A1 / flex 2: “I want Water”
- ADS1115 A2 / flex 3: “I want Food”
- ADS1115 A3 / flex 4: “Go to Washroom”
- Three or more bent sensors: “EMERGENCY ALERT”

Original starting voltage-drop thresholds (volts below the calibrated baseline):
- A0: `0.040`
- A1: `0.050`
- A2: `0.045`
- A3: `0.035`

Original ADC configuration:
- ADS1115 gain: `GAIN_ONE`
- Conversion factor in the original code: `raw * 0.000125` volts per count (125 µV/LSB at this gain).
- Baseline calibration: 20 readings, 10 ms delay between readings, while hand is open.
- Stable read: average 10 readings with 5 ms between reads.
- On potential bend: wait 800 ms, read again, then classify.
- Task display state lasts 5000 ms.
- Emergency LED/buzzer alarm toggles every 100 ms and times out after 4000 ms.
- Original button debounce used 200 ms.
- Original calibration feedback: two short beeps; status display prompts for an open hand.
- Original button starts calibration from IDLE; another press initiates stopping behavior and cancels emergency.
- The original code used a 128 × 64 SSD1306 layout. The new 0.91-inch horizontal OLED is commonly 128 × 32, so update display layout and confirm resolution.
- The original ESP32 used Wi-Fi and HTTPS MacroDroid webhooks with a retry/timeout path. STM32F103C8T6 has no built-in Wi-Fi. Do not attempt to compile ESP32 Wi-Fi/HTTP libraries for the Blue Pill. Webhooks are out of scope unless an external network module is deliberately added. The DFPlayer is the replacement local speech output.

### DFPlayer audio track mapping

Place audio files on a FAT32 microSD card. Suggested files:
- `/mp3/0001.mp3` — I want Coffee
- `/mp3/0002.mp3` — I want Water
- `/mp3/0003.mp3` — I want Food
- `/mp3/0004.mp3` — Go to Washroom
- `/mp3/0005.mp3` — Emergency Alert

Test actual DFPlayer file indexing with the installed library and card before integration.

## 5. Operating behavior and state machine

Intended states:
- `IDLE`: waiting for the start/calibration action.
- `CALIBRATING`: prompt user to keep hand open; calculate four baselines.
- `ACTIVE`: read averaged sensor values and look for gestures.
- `COMMAND_DISPLAY`: show recognized command for approximately 5 seconds; play the corresponding audio.
- `STOPPING`: show stopped state for approximately 4 seconds, then return to IDLE.
- `ERROR`: optional error state for missing ADS1115, OLED or DFPlayer.

Mode button is on PA0. Optional start/stop button is on PA1. Use pull-ups and debounce both. Define the mode behavior explicitly in firmware; recommended initial modes are speech enabled and silent (display/indicators only). Do not confuse mode selection with start/stop.

Gesture detection should be protected against repeated triggering while the same gesture is held. Add a release-to-rearm or explicit cooldown after the original detection behavior has been preserved and tested. Emergency must take priority when three or four fingers are bent.

## 6. Code architecture

Use PlatformIO with `framework = arduino`. Arduino libraries and peripheral drivers should use `.cpp` implementations; headers use `.h`. `main.cpp` should only call `appInit()` and `appUpdate()`.

Recommended structure:
```text
SignToSpeech/
├── platformio.ini
├── README.md
├── include/
│   ├── config.h
│   └── app_types.h
├── src/
│   ├── main.cpp
│   ├── app/
│   │   ├── app.h
│   │   └── app.cpp
│   ├── sensors/
│   │   ├── flex_sensor.h
│   │   └── flex_sensor.cpp
│   ├── recognition/
│   │   ├── gesture.h
│   │   └── gesture.cpp
│   ├── audio/
│   │   ├── audio_player.h
│   │   └── audio_player.cpp
│   ├── display/
│   │   ├── display.h
│   │   └── display.cpp
│   ├── input/
│   │   ├── buttons.h
│   │   └── buttons.cpp
│   └── indicators/
│       ├── indicators.h
│       └── indicators.cpp
└── docs/
    ├── pinout.md
    ├── interfaces.md
    └── test_plan.md
```

## 7. Team workflow

Two people are developing simultaneously:
- Developer A: ADS1115, flex sensor acquisition, calibration and gesture recognition.
- Developer B: DFPlayer, OLED, buttons, status LED and buzzer.
- Integrate together: `app.cpp`, `main.cpp`, `config.h`, `app_types.h`.

Agree on public function interfaces before implementing modules. Use Git branches such as `feature/sensors` and `feature/peripherals`; merge only after compiling/testing. Avoid both developers editing shared configuration and integration files at the same time.

## 8. Development and validation sequence

1. Build and upload PC13 onboard LED blink to verify toolchain/programmer.
2. Test external LED PB12 and buzzer PB13 safely.
3. Scan I²C and verify ADS1115 (`0x48`) and OLED (`0x3C`).
4. Read raw ADS1115 counts/voltages for all four sensors; validate divider direction and resistor values.
5. Test DFPlayer UART and all five audio tracks without flex-sensor logic.
6. Implement calibration and gesture recognition using the original starting thresholds.
7. Integrate buttons, display, audio and emergency indication.
8. Test reset immunity and measure 5 V rail while speaker plays.
9. Validate each command repeatedly and document measured thresholds.

## Instructions for an AI coding assistant

Use this README as the hardware and behavior specification. Generate complete, compile-ready PlatformIO files for the STM32F103C8T6 Blue Pill using the Arduino framework. Do not use ESP32-only APIs. Keep each module in its own `.h` and `.cpp` pair. Do not invent different pin assignments. Clearly identify any assumption about the actual OLED resolution, DFPlayer library behavior, buzzer electrical type, or IP5306 board capability. Work in small stages and provide code for each requested file with its exact path. Start by ensuring the minimal LED test builds and uploads, then add one peripheral at a time.


## 9. Starter-project status

The accompanying source files are a modular starter scaffold, not yet the fully integrated application. Sensor reading/calibration, initial gesture classification, OLED output, DFPlayer track calls, buttons and indicators have starter implementations. The complete non-blocking application state machine, mode semantics, gesture release/re-arm, error handling, and full regression tests still need integration and hardware validation.

Build the project after extracting it. If the selected PlatformIO package or installed DFPlayer library version does not support a particular call, check that library's installed API and adjust without changing the documented pinout or command mapping.
