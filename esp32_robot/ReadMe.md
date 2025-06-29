V   V  ooo   M   M  ooo    - N   N  RRRR   W   W
V   V o   o  MM MM o   o   - NN  N  R   R  W   W
V   V o   o  M M M o   o   - N N N  RRRR   W W W
 V V  o   o  M   M o   o   - N  NN  R  R   WW WW
  V    ooo   M   M  ooo    - N   N  R   R  W   W

             VoMo Team

# Advanced Robot Control Dashboard

This project runs on an ESP32 and provides both manual and voice-driven control of a two-motor robot using an L298N driver. A mobile-friendly web dashboard lets you issue commands from any browser.

---

## Features

- **Manual navigation**: Forward, backward, left, right, stop
- **Advanced routines**: Square, circle, scan
- **Calibration**: Auto-measure distance & rotation
- **Voice commands**: Speak JSON instructions to drive
- **LED indicator**: Toggle onboard LED

---

## Hardware Wiring

| ESP32 Pin  | Function         | Wire to L298N / Component |
| ---------- | ---------------- | ------------------------- |
| 26 (IN1)   | Motor A dir1     | IN1                       |
| 27 (IN2)   | Motor A dir2     | IN2                       |
| 25 (ENA)   | Motor A speed    | ENA                       |
| 14 (IN3)   | Motor B dir1     | IN3                       |
| 12 (IN4)   | Motor B dir2     | IN4                       |
| 13 (ENB)   | Motor B speed    | ENB                       |
| 2          | Onboard LED      | —                         |

---

## Software Setup

1. **Clone** this repo into your Arduino (or PlatformIO) projects folder.
2. **Open** `VoMoTeam.ino` in the Arduino IDE.
3. **Install** libraries via Library Manager:
   - WiFi
   - WebServer
   - ArduinoJson
4. **Edit** `Config.h` to set your WiFi SSID/password or static IP.
5. **Upload** to the ESP32 and open the Serial Monitor at 9600 baud.
6. **Browse** to the printed IP (e.g. `http://192.168.129.221`).

---

## API Endpoints

| Path                    | Method | Description                              |
| ----------------------- | ------ | ---------------------------------------- |
| `/`                     | GET    | Dashboard UI                             |
| `/forward`              | GET    | Move forward                             |
| `/backward`             | GET    | Move backward                            |
| `/left`                 | GET    | Turn left                                |
| `/right`                | GET    | Turn right                               |
| `/stop`                 | GET    | Stop motors                              |
| `/led`                  | GET    | Toggle LED                               |
| `/special`              | GET    | Special square & circle sequence         |
| `/scan`                 | GET    | 360° scan                                |
| `/voice`                | GET    | Fetch & process voice JSON command       |
| `/calibrate_distance`   | GET    | Drive 2 sec for distance calibration     |
| `/calibrate_rotation`   | GET    | Rotate 2 sec for rotation calibration    |
| `/calibrate_submit`     | GET    | Submit `type` & `value` for calibration  |
| `/get_calibration`      | GET    | Retrieve current calibration values      |

---

## Voice JSON Format

```json
{
  "distance":  30,     // cm to move
  "rotation":  45,     // degrees to turn
  "stop":     false,
  "scan":     false,
  "special":  false
}
