# AC IoT Framework — Smart Air Conditioning Control System

## Overview

**AC IoT Framework** is a fully integrated, scalable embedded system designed for advanced air conditioning control and environmental monitoring using the ESP32 microcontroller platform. This solution combines wireless connectivity (Wi-Fi & BLE), dual temperature sensing, infrared (IR) control protocols, remote API integration, persistent device configuration, and data logging — making it suitable for large-scale deployments in smart homes, commercial buildings, and industrial environments.

Engineered for reliability and flexibility, the system supports real-time remote monitoring, adaptive automated control, and manual overrides, providing seamless integration into existing IoT ecosystems or building management systems.

---

## Core Features & Capabilities

### Wireless Communication
- **Wi-Fi Connectivity**: Connects to local networks to access cloud APIs for telemetry and command exchange.
- **BLE Mode**: Enables secure initial configuration and fallback operation if Wi-Fi connectivity is unavailable or lost.

### Temperature Sensing
- **Dual DS18B20 Sensors**: Independent monitoring of internal device temperature and external ambient temperature to optimize AC operation.

### Infrared (IR) Control
- Emulates standard AC remote control signals, including power toggle, temperature setpoints (16–28°C), and operational modes via an IR LED transmitter.
- Supports capturing and decoding IR commands for device-specific customization.

### Remote API Integration
- Sends real-time telemetry data (temperature, AC status) to RESTful endpoints.
- Periodically fetches control instructions and scheduling parameters from remote servers.
- Detects and responds to external state changes to ensure synchronization between local device state and cloud directives.

### Time Synchronization & Scheduling
- Uses NTP (Network Time Protocol) to maintain accurate system time for event scheduling.
- Supports configurable on/off timing for automated AC control based on user preferences or environmental conditions.

### Persistent Configuration & Logging
- Utilizes **EEPROM** to store Wi-Fi credentials, user preferences, and operational parameters securely.
- Optional SD card logging capability for local data archiving, system diagnostics, and audit trails.

### User Feedback & Manual Control
- LED indicators provide visual status feedback with customizable blinking patterns.
- Push-button support for device reset and mode switching.
- BLE and serial interfaces enable remote or local manual configuration and overrides.

---

## Detailed System Architecture

### Initialization & Mode Selection
Upon boot, the system checks EEPROM flags to determine the operating mode:

- **Wi-Fi Mode**: Loads stored Wi-Fi credentials, initializes sensors and peripherals, attempts to connect to the configured Wi-Fi network, and starts NTP synchronization.
- **BLE Mode**: Activates BLE server to allow users to configure network credentials and device settings via a BLE client app or tool, especially useful for first-time setup or network failure recovery.

### Sensor Data Acquisition & Processing
- Periodically reads temperature values from both internal and external DS18B20 sensors.
- Applies thresholds and user-configured limits to decide AC operational state and trigger IR commands accordingly.

### Infrared Command Handling
- Utilizes the `IRremoteESP8266` library for sending IR signals on 38 kHz carrier frequency.
- Commands are mapped to predefined temperature setpoints and power states.
- Includes support for capturing unknown IR codes for compatibility with diverse AC models.

### API Communication Workflow
- At configurable intervals, the system pushes sensor data and device status to remote APIs.
- Fetches updated commands and schedules from the server.
- Detects any changes in API-provided state and adjusts local operation accordingly.

### Time Management & Event Scheduling
- Implements NTP client for precise timekeeping.
- Uses synchronized time to enforce user-defined ON/OFF schedules, optimizing energy efficiency and comfort.

### Fail-Safe & Recovery Mechanisms
- Switches automatically to BLE mode if Wi-Fi connection fails or no credentials are available.
- Allows manual reset and mode switching via hardware buttons.
- Logs errors and critical events to SD card for post-mortem analysis.

---

## Hardware Specifications

| Component            | Description                              |
|----------------------|----------------------------------------|
| ESP32 Dev Board      | Central microcontroller and Wi-Fi/BLE  |
| DS18B20 Temperature Sensors (x2) | For dual temperature sensing      |
| IR LED + Resistor    | Infrared transmitter for AC commands    |
| MicroSD Card Module  | Optional persistent logging storage     |
| LED Indicators       | Status indication (power, connectivity) |
| Push Button          | Manual reset/mode switch                 |
| Power Supply         | USB or regulated DC power                |

---

## Software & Library Dependencies

- `Arduino.h` — Core Arduino API  
- `IRremoteESP8266.h` & `IRsend.h` — Infrared transmission and decoding  
- `OneWire.h` & `DallasTemperature.h` — DS18B20 temperature sensors interface  
- `ArduinoJson.h` — JSON parsing for API data exchange  
- `EEPROM.h` — Persistent configuration storage  
- `SD.h` & `SPI.h` — SD card filesystem handling  
- NTP-related libraries (commented out in the provided code but integral for time sync)  
- BLE libraries (`BLEDevice.h`, `BLEServer.h`, etc.) for device provisioning and fallback

---

## Source Code Organization

| File                         | Description                                        |
|------------------------------|--------------------------------------------------|
| `ATM_AC_wifi_1.ino`          | Core firmware: initialization, main loop, mode management |
| `WiFi_Settings.ino`          | Wi-Fi connection, credential management          |
| `BLE.ino`                    | BLE server setup, communication callbacks        |
| `Send_API.ino`               | Sends telemetry data to cloud API                 |
| `Read_API.ino`               | Retrieves commands and configuration from cloud  |
| `Detect_Change_in_API.ino`   | Monitors API for state changes and syncs device   |
| `EEPROM_Settings.ino`        | Manages EEPROM read/write                          |
| `Get_NTP_Time.ino`           | Synchronizes system clock with NTP                |
| `Get_Temperature.ino`        | Interfaces with DS18B20 temperature sensors       |
| `LED_Blinking.ino`           | Provides visual LED status feedback                |
| `Send_Command.ino`           | Sends IR commands to AC units                       |
| `Remote_Configuration.ino`   | Remote setup and configuration over BLE or API    |
| `SD_CARD_Settings.ino`       | SD card initialization and file operations        |
| `Reset_Device.ino`           | System reset logic                                 |
| `Update_AC.ino`              | Updates AC states based on sensor data and API    |

---

## Installation & Deployment

1. **Clone the repository** to your local machine or development environment.  
2. Open the project in Arduino IDE or PlatformIO.  
3. Connect the ESP32 board and wire the peripherals: DS18B20 sensors (GPIO 25 & 27), IR LED (GPIO 32), SD card module, LEDs, and buttons as per hardware design.  
4. Install all required libraries listed above via Arduino Library Manager or PlatformIO.  
5. Compile and upload the firmware to the ESP32.  
6. For first-time setup or Wi-Fi credential changes, connect via BLE to provision network settings.  
7. Monitor serial output for debug logs and operation status.

---

## Operational Workflow Summary

- On startup, system decides between BLE and Wi-Fi modes based on EEPROM flags.
- Reads internal and external temperature sensors continuously.
- Executes IR commands to control AC unit power and temperature based on sensor readings and API instructions.
- Communicates with remote APIs for telemetry and command synchronization at periodic intervals.
- Maintains accurate system time via NTP for event scheduling.
- Logs operational data and critical events to SD card for traceability.
- Provides manual override via physical buttons and BLE commands.

---

## Potential Applications

- Intelligent climate control in smart homes and offices  
- HVAC automation and monitoring in commercial complexes  
- Embedded system prototyping for IoT environmental devices  
- Integration with enterprise building management systems (BMS)  
- Remote diagnostics and predictive maintenance of AC units

This system was originally designed and developed for a client in the HVAC industry, tailored to meet their commercial deployment requirements.



