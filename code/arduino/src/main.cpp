#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <queue>
#include <string>
#include <vector>

#include "credentials.h"
#include "deskmate/app/app.h"
#include "deskmate/arduino/gfx/sharp_mem_display.h"
#include "deskmate/arduino/input/buttons.h"
#include "deskmate/arduino/input/crank.h"
#include "deskmate/arduino/net/mqtt.h"
#include "deskmate/arduino/net/wifi.h"
#include "deskmate/gfx/display.h"  // testing only

using deskmate::app::App;
using deskmate::arduino::gfx::SharpMemDisplay;
using deskmate::arduino::input::SetupButtonsInterruptHandler;
using deskmate::arduino::input::SetupCrankInterruptHandler;
using deskmate::arduino::net::MQTTManager;
using deskmate::arduino::net::WiFiManager;

// Display.
using deskmate::credentials::kCSPin;
using deskmate::credentials::kDisplayHeight;
using deskmate::credentials::kDisplayWidth;
using deskmate::credentials::kMOSIPin;
using deskmate::credentials::kSCKPin;

// Wifi.
using deskmate::credentials::kWIFIPassword;
using deskmate::credentials::kWIFISSID;

// MQTT.
using deskmate::credentials::kMQTTClientId;
using deskmate::credentials::kMQTTConfigs;
using deskmate::credentials::kMQTTFloatingPointSensors;
using deskmate::credentials::kMQTTPassword;
using deskmate::credentials::kMQTTPort;
using deskmate::credentials::kMQTTServer;
using deskmate::credentials::kMQTTUser;

// Input pins.
using deskmate::credentials::kButtonAPin;
using deskmate::credentials::kButtonBPin;
using deskmate::credentials::kButtonCPin;
using deskmate::credentials::kCrankAPin;
using deskmate::credentials::kCrankBPin;
using deskmate::credentials::kCrankPushPin;

void setup() {
  Serial.begin(9600);

  SharpMemDisplay display(kDisplayHeight, kDisplayWidth, kSCKPin, kMOSIPin,
                          kCSPin);
  WiFiManager wifi_manager(kWIFISSID, kWIFIPassword);
  MQTTManager mqtt_manager(kMQTTServer, kMQTTPort, kMQTTUser, kMQTTPassword,
                           kMQTTClientId);
  if (!wifi_manager.Connect()) {
    Serial.println("Unable to connect to WiFi.");
    return;
  }

  if (!mqtt_manager.Connect()) {
    Serial.println("Unable to connect to the MQTT server.");
    return;
  }

  App app(&display, &mqtt_manager);
  app.Init(kMQTTConfigs, kMQTTFloatingPointSensors);

  SetupButtonsInterruptHandler(kCrankPushPin, kButtonAPin, kButtonBPin,
                               kButtonCPin, app.GetInputEventHandler());
  SetupCrankInterruptHandler(kCrankAPin, kCrankBPin,
                             app.GetInputEventHandler());

  while (true) {
    wifi_manager.MaybeReconnect();
    app.Tick();
  }
}

void loop() {}