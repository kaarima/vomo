#include <WiFi.h>
#include "Config.h"
#include "MotorControl.h"
#include "Movement.h"
#include "Calibration.h"
#include "VoiceCommand.h"
#include "WebPage.h"
#include "WebServerHandlers.h"

WebServer server(80);

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Init pins & state
  MotorControl::init();
  digitalWrite(LED_PIN, LOW);
  MotorControl::stop();

  // WiFi
  Serial.println("\n=== ESP32 Advanced Robot Starting ===");
  WiFi.mode(WIFI_STA);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("❌ Failed to configure static IP");
  }
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000); Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected!");
    Serial.print("📍 IP: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi failed, continuing offline.");
  }

  // Web server routes
  if (WiFi.status() == WL_CONNECTED) {
    WebServerHandlers::attachRoutes(server);
    server.begin();
    Serial.println("✅ Web server started at " + WiFi.localIP().toString());
  }

  Serial.println("=== Setup Complete ===\n");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  } else {
    static unsigned long lastTry = 0;
    if (millis() - lastTry > 30000) {
      lastTry = millis();
      Serial.println("Attempting WiFi reconnection...");
      WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
  }
}
