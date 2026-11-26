# MineSeeker – Autonomous Landmine Detection Rover

MineSeeker is an Arduino-based robotic rover prototype designed to support safer landmine-search operations by combining metal detection with autonomous movement and ultrasonic obstacle avoidance. When metal is detected, the rover stops and activates a buzzer alert. An ESP32-based Bluetooth module can relay serial messages to a paired mobile device, providing a foundation for remote monitoring.

## Project Overview

Landmines can remain hidden for years and pose serious risks to military personnel and civilians. MineSeeker explores a robotic approach to preliminary detection: a mobile platform scans its surroundings, detects nearby metal using a sensor input, avoids obstacles, and alerts the operator when a detection event occurs.

> **Safety notice:** MineSeeker is an educational prototype, not a certified landmine detector or demining system. It cannot establish whether an object is an explosive device. Never use it to approach, handle, or investigate suspected mines. Real-world mine detection and clearance must be performed by qualified professionals using approved equipment and procedures.

## Features Implemented in the Supplied Code

- **Metal sensing:** Reads an analog metal-detector sensor input and compares it with a configurable threshold.
- **Audible alert:** Stops the motors and activates a buzzer when the sensor reading crosses the configured threshold.
- **Obstacle detection:** Uses an HC-SR04-style ultrasonic trigger/echo interface to measure distance.
- **Basic obstacle avoidance:** Attempts left and right turns, reverses if the path remains blocked, and checks the path again.
- **Motor control:** Drives two DC motor channels through an L298N-style H-bridge interface.
- **Bluetooth message relay:** The ESP32 reads serial messages from the Arduino and forwards them over Bluetooth to a paired device.

## Hardware

The supplied source code identifies the following interfaces and components:

| Component | Purpose |
|---|---|
| Arduino-compatible controller | Main rover control and sensor processing |
| L298N-compatible motor driver | Controls the two DC motor channels |
| DC motors and rover chassis | Provide movement |
| Analog metal-detection sensor / coil module | Supplies the metal-sensing reading |
| Ultrasonic sensor | Measures distance to nearby obstacles |
| Buzzer | Signals a metal-detection event |
| ESP32 development board | Relays serial messages over Bluetooth |
| Battery and wiring | Power and electrical connections |

Check your actual wiring, module variants, and power requirements before uploading or operating the code.

## Software Structure

```text
MineSeeker/
├── mineseeker/
│   └── mineseeker.ino       # Main rover: motors, metal sensor,
│                            # ultrasonic sensing and buzzer
├── mineseekeresp/
│   └── mineseekeresp.ino    # ESP32: UART-to-Bluetooth message relay
└── README.md
```

### 1. Arduino rover firmware

The main sketch:

1. Configures the motor-driver pins, ultrasonic sensor, analog metal-sensor input, and buzzer.
2. Starts the rover moving forward.
3. Continuously reads the metal-sensor value.
4. If the reading is below `metalThreshold`, stops the motors, sounds the buzzer for three seconds, and resumes movement.
5. Checks the ultrasonic distance and invokes a basic rerouting routine when an obstacle is detected within 15 cm.

The detection threshold is currently set to `100` and should be calibrated for the specific sensor and installation. The threshold condition in the sketch is `metalValue < metalThreshold`.

### 2. ESP32 Bluetooth firmware

The ESP32 sketch starts a Bluetooth serial service named `ESP32-BT`. It reads newline-terminated data arriving over its UART serial connection and forwards that data to a paired device using Bluetooth.

The supplied Arduino sketch currently prints sensor readings and status messages to its USB serial interface. Confirm the UART wiring and baud-rate configuration, and add the required serial output connection between the Arduino and ESP32 if you want those messages relayed to a phone.

## Getting Started

1. Assemble the rover and connect the motors through the motor driver.
2. Connect the ultrasonic sensor, analog metal-sensor output, and buzzer to the pins defined in `mineseeker.ino`.
3. Open `mineseeker/mineseeker.ino` in the Arduino IDE and select the correct board and port.
4. Verify the pin mapping and motor direction for your hardware before testing.
5. Upload the main rover sketch and test the motor, sensor, and buzzer functions in a controlled, safe environment.
6. To use the Bluetooth relay, upload `mineseekeresp/mineseekeresp.ino` to a compatible ESP32 board, connect its UART to the Arduino as required, and pair a phone or computer with `ESP32-BT`.

## Pin Configuration

### Arduino main sketch

| Function | Arduino pin |
|---|---:|
| Motor A enable (ENA) | 11 |
| Motor A inputs (IN1, IN2) | 9, 10 |
| Motor B enable (ENB) | 5 |
| Motor B inputs (IN3, IN4) | 6, 12 |
| Ultrasonic trigger | 7 |
| Ultrasonic echo | 8 |
| Metal sensor analog output | A0 |
| Buzzer | 2 |

The sketch uses a serial baud rate of `9600`.

## Current Scope and Future Work

The supplied firmware implements the functions listed above. The accompanying project presentation/documentation also describes possible or intended capabilities such as IR sensing, GPS coordinate logging, and LCD location display; those features are **not present in the supplied `.ino` files**.

Possible future development includes:

- Integrating and validating GPS-based position logging.
- Adding an LCD or other operator display.
- Integrating and testing an IR sensing subsystem, if required by the design.
- Improving navigation and coverage planning beyond the current timed-turn routine.
- Adding robust sensor calibration, fault handling, and data logging.
- Testing the complete system in controlled, non-hazardous environments.

## Repository Contents

- `mineseeker/mineseeker.ino` – Arduino rover control firmware.
- `mineseekeresp/mineseekeresp.ino` – ESP32 Bluetooth serial relay firmware.
- `README.md` – Project overview and setup notes.

## Disclaimer

This repository documents an educational robotics prototype. Metal detection alone cannot identify a landmine, determine whether an object is explosive, or guarantee detection of all hazardous devices. The rover is not suitable for operational demining, and this project must not be represented as a validated life-safety system.
