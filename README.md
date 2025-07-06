# 🏠 Home Automation System using Arduino and IoT

This project implements a basic **Home Automation System** using Arduino. It enables users to control home appliances (like lights, fans, etc.) remotely via IoT technologies.

## 🔧 Features

- 💡 Control appliances via input commands (e.g., from serial or future IoT integrations)
- 📶 Easily extendable for Wi-Fi, Bluetooth, or Firebase integration
- 🔌 Relay-based switching for safe appliance control
- 🕹️ Simple command interface for testing and development

## 📁 Project File

- `Home Automation Project.ino`: The main Arduino sketch that controls multiple devices connected via digital pins.

## 🛠️ Hardware Requirements

- Arduino Uno / Nano / Mega
- 4-Channel Relay Module
- Home appliances (bulb, fan, etc.)
- Jumper Wires
- Power Supply

## ⚙️ Pin Configuration

| Appliance       | Arduino Pin |
|-----------------|-------------|
| Light           | D2          |
| Fan             | D3          |
| TV / Device 1   | D4          |
| AC / Device 2   | D5          |

> You can modify these pins as per your hardware setup.

## 🚦 How It Works

1. Each device is connected to a digital output pin via a relay module.
2. You send a command (e.g., from Serial Monitor or Bluetooth/WiFi module).
3. The Arduino receives the command and toggles the relevant relay.
4. Status can be extended to be monitored via sensors or cloud.

### 🧪 Example Commands (if used with Serial)

L1_ON → Turns Light ON
L1_OFF → Turns Light OFF
FAN_ON → Turns Fan ON
FAN_OFF → Turns Fan OFF

You can expand the command list depending on how many devices are added.

## 🌐 Extensions

This basic setup can be extended with:

- 📱 **ESP8266/ESP32** for Wi-Fi control via mobile apps or web dashboards
- ☁️ **Firebase** or **MQTT** integration for cloud-based control and status sync
- 🎙️ **Voice Assistant Integration** (Google Assistant / Alexa)
- 📊 **Sensor Monitoring** (temperature, motion, light sensors)

## 🧠 Future Plans

- Add mobile app UI for real-time control
- Integrate scheduling logic
- Add feedback mechanism (e.g., current sensor for status validation)
- Improve security via authentication


