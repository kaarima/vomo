#include "Calibration.h"

float Calibration::_distancePerSec = 10.0;
float Calibration::_rotationPerSec = 90.0;

void Calibration::startDistanceCal() {
  Calibration::_distancePerSec = 0;
}

void Calibration::startRotationCal() {
  Calibration::_rotationPerSec = 0;
}

bool Calibration::submit(const String& type, float value) {
  if (type == "distance" && value > 0) {
    _distancePerSec = value / 2.0;
    return true;
  }
  if (type == "rotation" && value > 0) {
    _rotationPerSec = value / 2.0;
    return true;
  }
  return false;
}

float Calibration::distancePerSecond() {
  return _distancePerSec;
}

float Calibration::rotationPerSecond() {
  return _rotationPerSec;
}
