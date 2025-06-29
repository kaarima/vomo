# VoMo Robot Project - Complete Documentation

```
V   V  ooo   M   M  ooo    - N   N  RRRR   W   W
V   V o   o  MM MM o   o   - NN  N  R   R  W   W
V   V o   o  M M M o   o   - N N N  RRRR   W W W
 V V  o   o  M   M o   o   - N  NN  R  R   WW WW
  V    ooo   M   M  ooo    - N   N  R   R  W   W

             VoMo Team
```

## Project Overview

The VoMo Robot Project is a complete voice-controlled robot system consisting of two main components:

1. **ESP32 Robot Controller** - Hardware control system with web interface
2. **AI Voice Processing API** - Python FastAPI service for voice command interpretation

This system allows users to control a two-motor robot through voice commands, manual web controls, or automated movement sequences.


---

## System Architecture

```
┌─────────────────┐    HTTP Requests    ┌─────────────────┐
│   Voice Input   │ ──────────────────► │  AI API Server  │
│   (Microphone)  │                     │   (Python)      │
└─────────────────┘                     └─────────────────┘
                                                │
                                                │ JSON Commands
                                                ▼
┌─────────────────┐    WiFi Network     ┌─────────────────┐
│   Web Browser   │ ──────────────────► │  ESP32 Robot    │
│   (Dashboard)   │                     │   Controller    │
└─────────────────┘                     └─────────────────┘
                                                │
                                                │ Motor Signals
                                                ▼
                                       ┌─────────────────┐
                                       │   L298N Driver  │
                                       │   + Motors      │
                                       └─────────────────┘
```

---

## Component 1: ESP32 Robot Controller

### Hardware Requirements

- **ESP32 Development Board**
- **L298N Motor Driver**
- **2x DC Motors** (with wheels)
- **Robot Chassis**
- **Power Supply** (12V recommended)
- **Breadboard & Jumper Wires**

### Pin Configuration

| ESP32 Pin | Function | L298N Connection |
|-----------|----------|------------------|
| GPIO 26   | IN1      | Motor A Direction 1 |
| GPIO 27   | IN2      | Motor A Direction 2 |
| GPIO 25   | ENA      | Motor A Speed Control |
| GPIO 14   | IN3      | Motor B Direction 1 |
| GPIO 12   | IN4      | Motor B Direction 2 |
| GPIO 13   | ENB      | Motor B Speed Control |
| GPIO 2    | LED      | Onboard Status LED |

### Software Components

#### Core Files
- **`esp32_robot.ino`** - Main Arduino sketch
- **`config.h`** - WiFi and pin configuration
- **`MotorControl.h/.cpp`** - Low-level motor control
- **`Movement.h/.cpp`** - High-level movement routines
- **`Calibration.h/.cpp`** - Distance and rotation calibration
- **`VoiceCommand.h/.cpp`** - Voice command processing
- **`WebPage.h/.cpp`** - Web dashboard HTML/CSS/JS
- **`WebServerHandlers.h/.cpp`** - HTTP endpoint handlers

#### Key Features
- **Manual Control**: Forward, backward, left, right, stop
- **Automated Sequences**: Square pattern, circle pattern, 360° scan
- **Calibration System**: Distance and rotation calibration
- **Web Dashboard**: Mobile-friendly control interface
- **Voice Integration**: JSON command processing
- **LED Status Indicator**: Visual feedback

### Web API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/` | GET | Main dashboard interface |
| `/forward` | GET | Move robot forward |
| `/backward` | GET | Move robot backward |
| `/left` | GET | Turn robot left |
| `/right` | GET | Turn robot right |
| `/stop` | GET | Stop all motors |
| `/led` | GET | Toggle status LED |
| `/special` | GET | Execute special sequence |
| `/scan` | GET | Perform 360° scan |
| `/voice` | GET | Process voice command JSON |
| `/calibrate_distance` | GET | Start distance calibration |
| `/calibrate_rotation` | GET | Start rotation calibration |
| `/calibrate_submit` | GET | Submit calibration values |
| `/get_calibration` | GET | Retrieve calibration data |

---

## Component 2: AI Voice Processing API

### Technology Stack

- **FastAPI** - Web framework
- **Groq API** - AI transcription service
- **Whisper-large-v3** - Speech-to-text model
- **sounddevice** - Audio recording
- **numpy/scipy** - Audio processing

### Voice Command Processing

#### Supported Commands
- **Movement**: "forward 10", "backward 5", "left 45", "right 90"
- **Actions**: "stop", "scan", "special"
- **Units**: Distance in cm, rotation in degrees

#### Processing Flow
1. **Audio Recording**: 2-second audio capture at 16kHz
2. **Transcription**: Groq API converts speech to text
3. **Command Parsing**: Regex extraction of commands and values
4. **JSON Response**: Structured command data for ESP32

#### Example Voice Commands
```
"Move forward 15 centimeters" → {"distance": 15.0, "rotation": 0.0, "stop": false, "scan": false, "special": false}
"Turn left 90 degrees" → {"distance": 0.0, "rotation": 90.0, "stop": false, "scan": false, "special": false}
"Stop the robot" → {"distance": 0.0, "rotation": 0.0, "stop": true, "scan": false, "special": false}
```

---

## Installation & Setup

### Prerequisites
- Arduino IDE or PlatformIO
- Python 3.8+
- Microphone for voice input
- WiFi network access

### ESP32 Setup
1. **Install Libraries**:
   ```bash
   # Arduino Library Manager
   - WiFi
   - WebServer
   - ArduinoJson
   ```

2. **Configure WiFi**:
   Edit `config.h`:
   ```cpp
   constexpr char WIFI_SSID[] = "YourWiFiName";
   constexpr char WIFI_PASS[] = "YourWiFiPassword";
   ```

3. **Upload Code**:
   - Open `esp32_robot.ino` in Arduino IDE
   - Select ESP32 board
   - Upload to device

### AI API Setup
1. **Install Dependencies**:
   ```bash
   pip install fastapi uvicorn groq sounddevice numpy scipy
   ```

2. **Configure API Key**:
   Edit `AI_integration/api.py`:
   ```python
   api_key = 'your_groq_api_key_here'
   ```

3. **Start Server**:
   ```bash
   cd AI_integration
   uvicorn api:app --reload --host 0.0.0.0 --port 8000
   ```

---

## Usage Guide

### Manual Control
1. Connect to ESP32's IP address in web browser
2. Use dashboard buttons for direct control
3. Monitor robot status via LED indicator

### Voice Control
1. Start AI API server
2. Send GET request to `http://localhost:8000/`
3. Speak command clearly during 2-second recording
4. API returns JSON command for ESP32

### Calibration
1. Use `/calibrate_distance` to measure forward movement
2. Use `/calibrate_rotation` to measure turning accuracy
3. Submit actual measurements via `/calibrate_submit`
4. Retrieve calibration data with `/get_calibration`

### Automated Sequences
- **Square Pattern**: `/special` endpoint
- **360° Scan**: `/scan` endpoint
- **Custom Routines**: Extend `Movement.cpp`

---

## Configuration Options

### ESP32 Configuration (`config.h`)
```cpp
// WiFi Settings
constexpr char WIFI_SSID[] = "YourNetwork";
constexpr char WIFI_PASS[] = "YourPassword";

// Static IP (optional)
const IPAddress local_IP(192, 168, 1, 100);
const IPAddress gateway(192, 168, 1, 1);

// Motor Pins
constexpr int IN1_PIN = 26;  // Motor A Direction 1
constexpr int IN2_PIN = 27;  // Motor A Direction 2
constexpr int ENA_PIN = 25;  // Motor A Speed
constexpr int IN3_PIN = 14;  // Motor B Direction 1
constexpr int IN4_PIN = 12;  // Motor B Direction 2
constexpr int ENB_PIN = 13;  // Motor B Speed
```

### AI API Configuration (`api.py`)
```python
# Audio Settings
duration = 2      # Recording duration (seconds)
fs = 16000        # Sample rate (Hz)

# API Configuration
api_key = 'your_groq_api_key'
model = "whisper-large-v3"
```

---

## Troubleshooting

### Common Issues

#### ESP32 Connection Problems
- **WiFi not connecting**: Check SSID/password in `config.h`
- **Static IP issues**: Verify gateway and subnet settings
- **Motor not responding**: Check L298N wiring and power supply

#### AI API Issues
- **Audio not recording**: Check microphone permissions
- **Transcription errors**: Verify Groq API key and internet connection
- **Command parsing fails**: Ensure clear speech and proper command format

#### Integration Problems
- **Voice commands not reaching ESP32**: Check network connectivity
- **JSON parsing errors**: Verify API response format
- **Timing issues**: Adjust audio duration or add delays

### Debug Information
- ESP32 Serial Monitor: 9600 baud
- AI API logs: Check console output
- Web dashboard: Browser developer tools

---

## Extending the Project

### Adding New Commands
1. **ESP32**: Add new endpoints in `WebServerHandlers.cpp`
2. **AI API**: Extend command parsing in `api.py`
3. **Movement**: Implement new routines in `Movement.cpp`

### Hardware Enhancements
- **Sensors**: Add ultrasonic, IR, or camera modules
- **Actuators**: Implement servo motors or grippers
- **Displays**: Add LCD or OLED screens

### Software Improvements
- **Security**: Add authentication to web interface
- **Logging**: Implement comprehensive logging system
- **Mobile App**: Create dedicated mobile application
- **Cloud Integration**: Add remote monitoring capabilities

---

## Project Structure

```
esp32_robot/
├── esp32_robot.ino          # Main Arduino sketch
├── config.h                 # Configuration settings
├── MotorControl.h/.cpp      # Motor driver interface
├── Movement.h/.cpp          # Movement routines
├── Calibration.h/.cpp       # Calibration system
├── VoiceCommand.h/.cpp      # Voice command processing
├── WebPage.h/.cpp           # Web dashboard
├── WebServerHandlers.h/.cpp # HTTP endpoints
└── ReadMe.md               # Original documentation

AI_integration/
├── api.py                  # FastAPI voice processing
├── README.md              # AI component documentation
└── recorded.wav           # Temporary audio file
```

---

## License & Credits

**VoMo Team** - Advanced Robot Control Dashboard

This project demonstrates the integration of:
- ESP32 microcontroller programming
- Motor control and robotics
- Web development and APIs
- AI-powered voice recognition
- Real-time audio processing

For questions or contributions, please refer to the individual component documentation files.
