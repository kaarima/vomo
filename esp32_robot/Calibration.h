#pragma once

class Calibration {
public:
  static void startDistanceCal();
  static void startRotationCal();
  static bool submit(const String& type, float value);
  static float distancePerSecond();
  static float rotationPerSecond();
private:
  static float _distancePerSec;
  static float _rotationPerSec;
};
