#pragma once

class MotorControl {
public:
  static void init();
  static void stop();
  static void forward(int speed = 200);
  static void backward(int speed = 200);
  static void turnRight(int speed = 180);
  static void turnLeft(int speed = 180);
};
