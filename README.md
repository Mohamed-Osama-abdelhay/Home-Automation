# Smart Irrigation System with AI Rain Prediction and Firebase Integration

This project implements a smart irrigation system using Arduino and ESP32. It utilizes AI-based rain prediction and Firebase Cloud to manage water resources efficiently, control a smart ceiling, and optimize irrigation based on real-time sensor data.

## 🔧 Features

- 🌧️ **AI Rain Prediction**: Receives AI-predicted rain data via Firebase.
- 🌱 **Soil Moisture Monitoring**: Monitors soil moisture using multiple sensors.
- 📡 **ESP32 to Arduino I2C Communication**: ESP32 sends rain prediction and sensor data to Arduino.
- ☁️ **Firebase Integration**: ESP32 reads from and writes to Firebase Realtime Database.
- 🚿 **Smart Irrigation Logic**:
  - If **rain is predicted** and **soil is moist** → Close ceiling.
  - If **rain is predicted** and **soil is dry** → Open ceiling.
  - If **no rain is predicted** and **soil is dry** → Activate water pump.
  - If **no rain is predicted** and **soil is moist** → Do nothing.
- 🌦️ **Fallback to Real Rain Sensor** in case of AI misprediction.
- 🛠️ **Motor Control** using L298N Driver for opening/closing smart ceiling.

## ⚙️ Hardware Components

- ESP32 Dev Module
- Arduino Uno
- 3 Soil Moisture Sensors
- Rain Sensor
- BMP180 Temperature Sensor
- L298N Motor Driver
- 2 DC Motors (for opening/closing smart ceiling)
- Water Pump
- Firebase Realtime Database

## 🔌 Wiring Overview

- ESP32 (I2C):
  - SDA: GPIO 21 (to Arduino)
  - SCL: GPIO 22 (to Arduino)
  - Secondary I2C (for BMP180): SDA (GPIO 18), SCL (GPIO 19)
- Arduino:
  - Reads sensor data and controls motors/pump
- Motor driver L298N:
  - IN1, IN2 → Motor A
  - IN3, IN4 → Motor B

## 🧠 Logic Flow

1. ESP32 reads AI rain prediction from Firebase.
2. Sends prediction and soil moisture data to Arduino via I2C.
3. Arduino uses logic to:
   - Open/close ceiling.
   - Start/stop water pump.
4. If a mismatch is detected between rain prediction and actual rain sensor → revert to real sensor logic.

## 📁 File Description

- `Home Automation Project`: Main Arduino sketch handling I2C data reception and controlling actuators based on AI and sensor input.

## 🛠️ Getting Started

### Prerequisites

- Arduino IDE
- Firebase Project (with Database rules configured)
- Required Libraries:
  - `Wire.h`
  - `Adafruit_BMP085.h` (or similar for BMP180)
  - `FirebaseESP32.h` (for ESP32 sketch)
  
### Uploading the Code

1. Upload the ESP32 code separately to your ESP32 board.
2. Upload `Home Automation Project` to the Arduino Uno.
3. Ensure both devices are powered and connected properly via I2C.

## 📊 Future Improvements

- Integrate a dashboard for real-time monitoring.
- Add alert notifications for hardware failures.
- Improve AI model confidence by blending real-time and predicted data.

## 📃 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

