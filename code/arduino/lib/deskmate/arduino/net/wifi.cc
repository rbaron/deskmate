#include "deskmate/arduino/net/wifi.h"

#include <WiFi.h>

namespace deskmate {
namespace arduino {
namespace net {

namespace {
constexpr int kMaxWaitForConnectionMS = 1000;
constexpr int kConnectionLoopDelay = 100;
constexpr int kConnectionTries = 3;

bool WiFiTryToConnectOnce(const char* ssid, const char* password) {
   long delayed = 0;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    // Serial.printf("[wifi] WiFi.status(): %d\n", WiFi.status());
    delay(kConnectionLoopDelay);
    delayed += kConnectionLoopDelay;
    if (delayed > kMaxWaitForConnectionMS) {
      return false;
    }
  }
  // Serial.printf("[wifi] Connected!\n");
  return true;
}

}  // namespace

// For some reason, my uc (esp32) fails to connect every other time. This
// re-begin() approach seems to work, although it's not very  elegant.
bool WiFiManager::Connect() {
  for (int i = 0; i < kConnectionTries; i++) {
    // Serial.printf("[wifi] Attempting to connect %d/%d...\n", i,
    //               kConnectionTries);
    if (WiFiTryToConnectOnce(ssid_, password_)) {
      return true;
    }
  }
  // Serial.printf("[wifi] Unable to connect.\n");
  return false;
}

// TODO: do we need the hacky retries here too?
bool WiFiManager::MaybeReconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    return Connect();
  }
  return true;
}

}  // namespace net
}  // namespace arduino
}  // namespace deskmate
