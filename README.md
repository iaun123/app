<div align="center">

# 🌤️ Smart Weather Lamp & IoT Environmental Monitor

[![ESP8266](https://img.shields.io/badge/Hardware-ESP8266-red?style=for-the-badge&logo=espressif&logoColor=white)](https://www.espressif.com/)
[![Arduino](https://img.shields.io/badge/Platform-Arduino_IDE-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![MQTT](https://img.shields.io/badge/Protocol-MQTT_%2F_WebSockets-660066?style=for-the-badge&logo=hivemq&logoColor=white)](https://www.hivemq.com/)
[![OpenWeather](https://img.shields.io/badge/API-OpenWeatherMap-orange?style=for-the-badge&logo=openweathermap&logoColor=white)](https://openweathermap.org/)
[![JavaScript](https://img.shields.io/badge/Frontend-Vanilla_JS_%26_CSS3-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)](https://developer.mozilla.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](./LICENSE)

<p align="center">
  <b>A connected IoT ambient lighting and environmental monitoring system with real-time telemetry, remote lamp control, and live weather integration.</b><br>
  <i>(Bachelor's Degree IoT & Embedded Systems Project)</i>
</p>

<!-- Web Application Preview Screenshot -->
<p align="center">
  <img src="./docs/images/weather_lamp_preview.png" width="400" alt="Weather Lamp Web Application Preview" style="border-radius: 12px; box-shadow: 0 8px 24px rgba(0,0,0,0.3);" />
  <br>
  <sub><i>Interactive Web Application UI & Telemetry Dashboard</i></sub>
</p>

</div>

---

## 📌 Overview

**Smart Weather Lamp** is an undergraduate (Bachelor's Degree) IoT engineering project that bridges real-time hardware telemetry with an interactive web dashboard. Built around the **ESP8266 (NodeMCU)** microcontroller and a **DHT11 sensor**, the system continuously streams indoor temperature and humidity data to a web client via **MQTT over WebSockets**.

Additionally, the dashboard fetches live local weather conditions using the **OpenWeatherMap API** and browser Geolocation, providing a dual perspective of indoor microclimates and outdoor weather conditions, complete with time-adaptive dynamic ambient themes.

---

## ✨ Key Features

- 🌡️ **Real-Time Indoor Telemetry:** Continuously samples temperature (°C) and relative humidity (%) using the DHT11 sensor and publishes telemetry via MQTT.
- 💡 **Bidirectional Lamp Actuation:** Remotely toggle smart ambient lighting / LED indicators on/off directly from the web dashboard with sub-second latency.
- 🛰️ **Live Weather & Geolocation:** Automatically detects user location and integrates live weather data (sky conditions, cloudiness, rainfall) from OpenWeatherMap.
- 🎨 **Adaptive Time-of-Day UI:** Dashboard background smoothly adapts its color gradient according to the local hour (Dawn, Morning, Noon, Sunset, Dusk, Night).
- ⚡ **Lightweight WebSockets Architecture:** Directly communicates with HiveMQ cloud broker via WebSockets (`wss://broker.hivemq.com:8884/mqtt`) without requiring a custom backend server.

---

## 🏗️ System Architecture

```mermaid
graph TD
    subgraph Hardware_Layer ["🔌 Hardware Layer (ESP8266)"]
        DHT["DHT11 Sensor (Pin D5)"] -->|Read Temp/Hum| ESP["ESP8266 Microcontroller"]
        ESP -->|Digital Write| LED["Smart Lamp / LED (Pin D2)"]
    end

    subgraph Cloud_Broker ["☁️ MQTT Broker (HiveMQ)"]
        TOPIC_DATA["dht_temp / dht_hum"]
        TOPIC_CMD["switch_lamp"]
    end

    subgraph Web_Dashboard ["💻 Web Client (HTML5 / CSS3 / JS)"]
        UI_METRICS["Live Sensor Telemetry Display"]
        UI_CONTROL["Lamp ON / OFF Switch"]
        UI_WEATHER["OpenWeatherMap API Display"]
    end

    ESP -->|Publish Metrics| TOPIC_DATA
    TOPIC_DATA -->|Subscribe / WebSockets| UI_METRICS
    UI_CONTROL -->|Publish Command (1/0)| TOPIC_CMD
    TOPIC_CMD -->|Subscribe Callback| ESP
    UI_WEATHER -.->|HTTP GET| OWM["OpenWeatherMap REST API"]
```

---

## 🔌 Hardware Setup & Pinout

| Component | ESP8266 Pin | Pin Type | Function |
| :--- | :---: | :---: | :--- |
| **DHT11 Sensor** | `D5` (GPIO 14) | Digital Input | Reads Temperature and Humidity |
| **LED / Relay Module** | `D2` (GPIO 4) | Digital Output | Controls Smart Lamp status |
| **VCC / GND** | `3.3V / GND` | Power | Powers microcontroller & sensors |

---

## 📡 MQTT Topics & Protocol Specification

| Topic | Publisher | Subscriber | Payload | Description |
| :--- | :---: | :---: | :---: | :--- |
| `dht_temp` | ESP8266 | Web App | `float` (e.g. `"28.5"`) | Ambient temperature in Celsius |
| `dht_hum` | ESP8266 | Web App | `float` (e.g. `"65.0"`) | Relative humidity percentage |
| `switch_lamp` | Web App | ESP8266 | `"1"` or `"0"` | Remote toggle for the lamp (`1` = ON, `0` = OFF) |

---

## 🚀 Getting Started

### 1. Hardware & Firmware Configuration
1. Open [`Project_Mqtt/Project_Mqtt.ino`](./Project_Mqtt/Project_Mqtt.ino) in **Arduino IDE**.
2. Install required libraries from the Library Manager:
   - `DHT sensor library` by Adafruit
   - `PubSubClient` by Nick O'Leary
   - `ESP8266WiFi`
3. Update Wi-Fi credentials in `Project_Mqtt.ino`:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
4. Select **NodeMCU 1.0 (ESP-12E Module)** as the target board and click **Upload**.

### 2. Running the Web Application
1. Navigate to the [`web_app`](./web_app) directory.
2. Open [`web_app/dht.html`](./web_app/dht.html) directly in any modern web browser or serve it locally:
   ```bash
   cd web_app
   python3 -m http.server 8000
   ```
3. Open `http://localhost:8000/dht.html` in your browser.
4. Allow Geolocation permissions to enable local weather data fetching.

---

## 📂 Repository Structure

```
.
├── Project_Mqtt/
│   └── Project_Mqtt.ino     # ESP8266 Arduino firmware (Sensors & MQTT logic)
├── web_app/
│   ├── dht.html             # Dashboard UI layout
│   ├── style.css            # Responsive styling & ambient gradients
│   ├── temp.js              # WebSockets MQTT client & OpenWeatherMap integration
│   ├── icons/               # SVG Weather condition icons
│   └── vercel.json          # Deployment configuration for Vercel
├── docs/
│   └── images/
│       └── weather_lamp_preview.png  # Web application UI screenshot
├── LICENSE                  # MIT License
└── README.md                # Project documentation
```

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.

---

## 👨‍💻 Author

**Chananya Meepayung (Aun)**
- **Role:** Robotics & Embedded Software Engineer
- **GitHub:** [@iaun123](https://github.com/iaun123)
- **LinkedIn:** [chananya-meepayung](https://www.linkedin.com/in/chananya-meepayung-b39335356/)
- **Email:** chananyaaun123@gmail.com
