#pragma once

#include <IPAddress.h>

// WiFi credentials
constexpr char WIFI_SSID[] = "AndroidAPEEAF";
constexpr char WIFI_PASS[] = "Soussi00";

// Fixed IP configuration
const IPAddress local_IP(192, 168, 129, 221);
const IPAddress gateway(192, 168, 129, 1);
const IPAddress subnet(255, 255, 255, 0);
const IPAddress primaryDNS(8, 8, 8, 8);
const IPAddress secondaryDNS(8, 8, 4, 4);

// Motor driver pins
constexpr int IN1_PIN = 26;
constexpr int IN2_PIN = 27;
constexpr int ENA_PIN = 25;
constexpr int IN3_PIN = 14;
constexpr int IN4_PIN = 12;
constexpr int ENB_PIN = 13;

// LED pin
constexpr int LED_PIN = 2;
