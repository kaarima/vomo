#include <Arduino.h>
#include "Movement.h"
#include "MotorControl.h"
#include "Calibration.h"

void Movement::moveDistance(float cm) {
  if (cm > 0) MotorControl::forward();
  else MotorControl::backward();
  delay((abs(cm) / Calibration::distancePerSecond()) * 1000);
  MotorControl::stop();
}

void Movement::rotateAngle(float degrees) {
  if (degrees > 0) MotorControl::turnRight();
  else MotorControl::turnLeft();
  delay((abs(degrees) / Calibration::rotationPerSecond()) * 1000);
  MotorControl::stop();
}

void Movement::specialSequence() {
  for (int i = 0; i < 4; i++) {
    moveDistance(20);
    delay(500);
    rotateAngle(90);
    delay(500);
  }
  for (int i = 0; i < 8; i++) {
    moveDistance(10);
    delay(300);
    rotateAngle(45);
    delay(300);
  }
  moveDistance(-15);
}

void Movement::scanArea() {
  for (int i = 0; i < 8; i++) {
    rotateAngle(45);
    delay(1000);
  }
}
