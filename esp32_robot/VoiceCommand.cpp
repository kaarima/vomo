#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "VoiceCommand.h"
#include "Movement.h"
#include "MotorControl.h"

void VoiceCommand::process() {
  HTTPClient http;
  http.begin("http://192.168.129.32:8000");
  int code = http.GET();
  if (code > 0) {
    String resp = http.getString();
    DynamicJsonDocument doc(1024);
    if (!deserializeJson(doc, resp)) {
      int dist = doc["distance"];
      int rot  = doc["rotation"];
      bool stop = doc["stop"], scan = doc["scan"], special = doc["special"];
      if (stop) MotorControl::stop();
      else if (scan) Movement::scanArea();
      else if (special) Movement::specialSequence();
      else {
        if (dist) Movement::moveDistance(dist);
        if (rot)  Movement::rotateAngle(rot);
      }
    }
  }
  http.end();
}
