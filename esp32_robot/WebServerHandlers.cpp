#include "WebServerHandlers.h"
#include "WebPage.h"
#include "MotorControl.h"
#include "Movement.h"
#include "Calibration.h"
#include "VoiceCommand.h"
#include "Config.h"

namespace WebServerHandlers {

  static void handleRoot()        { server.send(200, "text/html", getWebPage()); }
  static void handleForward()     { MotorControl::forward(); server.send(200, "text/plain", "Moving Forward"); }
  static void handleBackward()    { MotorControl::backward(); server.send(200, "text/plain", "Moving Backward"); }
  static void handleLeft()        { MotorControl::turnLeft(); server.send(200, "text/plain", "Turning Left"); }
  static void handleRight()       { MotorControl::turnRight(); server.send(200, "text/plain", "Turning Right"); }
  static void handleStop()        { MotorControl::stop(); server.send(200, "text/plain", "Stopped"); }
  static void handleLED()         { static bool state=false; state=!state; digitalWrite(LED_PIN, state); server.send(200, "text/plain", state?"LED ON":"LED OFF"); }
  static void handleVoice()       { VoiceCommand::process(); server.send(200, "text/plain", "Voice processed"); }
  static void handleSpecial()     { Movement::specialSequence(); server.send(200, "text/plain", "Special done"); }
  static void handleScan()        { Movement::scanArea(); server.send(200, "text/plain", "Scan done"); }
  static void handleCalDist()     { Calibration::startDistanceCal(); server.send(200,"text/plain","Measure distance and submit"); }
  static void handleCalRot()      { Calibration::startRotationCal(); server.send(200,"text/plain","Measure rotation and submit"); }
  static void handleCalSubmit()   {
    String type = server.arg("type");
    float v = server.arg("value").toFloat();
    if (Calibration::submit(type, v))
      server.send(200, "text/plain", String(type) + " calibrated");
    else
      server.send(400, "text/plain", "Invalid data");
  }
  static void handleGetCal()      {
    String j = "{\"distance\":" + String(Calibration::distancePerSecond()) +
               ",\"rotation\":" + String(Calibration::rotationPerSecond()) + "}";
    server.send(200, "application/json", j);
  }

  void attachRoutes(WebServer& server) {
    server.on("/",           handleRoot);
    server.on("/forward",    handleForward);
    server.on("/backward",   handleBackward);
    server.on("/left",       handleLeft);
    server.on("/right",      handleRight);
    server.on("/stop",       handleStop);
    server.on("/led",        handleLED);
    server.on("/voice",      handleVoice);
    server.on("/special",    handleSpecial);
    server.on("/scan",       handleScan);
    server.on("/calibrate_distance", handleCalDist);
    server.on("/calibrate_rotation", handleCalRot);
    server.on("/calibrate_submit",   handleCalSubmit);
    server.on("/get_calibration",    handleGetCal);
  }

}
