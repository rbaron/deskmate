#ifndef ROOT_DESKMATE_CREDENTIALS_H
#define ROOT_DESKMATE_CREDENTIALS_H

#include <string>
#include <vector>

#include "deskmate/app/config.h"

// TODO: Make these configs dynamic. Ideally configured from a web page, like
// Tasmota/ESPHome.
namespace deskmate {
namespace credentials {
namespace {
using deskmate::app::MQTTConfig;
}  // namespace

// WiFi.
constexpr char kWIFISSID[] = "wifi-ssid";
constexpr char kWIFIPassword[] = "wifi-password";

// MQTT.
constexpr char kMQTTUser[] = "mqttuser";
constexpr char kMQTTPassword[] = "mqttpassword";
constexpr char kMQTTClientId[] = "deskmate-arduino";
constexpr char kMQTTServer[] = "mqtt-broker-hostname-or-ip-address";
constexpr int kMQTTPort = 1883;

// Display pins.
constexpr uint8_t kSCKPin = 14;
constexpr uint8_t kMOSIPin = 13;
constexpr uint8_t kCSPin = 15;

// Display size.
constexpr unsigned int kDisplayHeight = 240;
constexpr unsigned int kDisplayWidth = 400;

// Input pins.
// Crank A & B pins are the rotary encoder's two phase-shifted input pins.
constexpr uint8_t kButtonAPin = 27;
constexpr uint8_t kButtonBPin = 26;
constexpr uint8_t kButtonCPin = 25;
constexpr uint8_t kCrankAPin = 33;
constexpr uint8_t kCrankBPin = 34;
constexpr uint8_t kCrankPushPin = 32;

// MQTT-powered switches.
const std::vector<MQTTConfig> kMQTTConfigs{
    {"Desk", "home/switch/cmnd/officedesk/POWER",
     "home/switch/stat/officedesk/POWER"},
    {"Office", "home/switch/cmnd/office/POWER",
     "home/switch/stat/office/POWER"},
};

// MQTT-powered floating point sensors.
const std::vector<deskmate::app::MQTTFloatingPointSensorConfig>
    kMQTTFloatingPointSensors{
        {"Monstera", "home/Monstera/soil_moisture_percent/retained"},
        {"Ficus", "home/Ficus/soil_moisture_percent/retained"},
        {"Calethea", "home/Calethea/soil_moisture_percent/retained"},
        {"CitrusTree", "home/CitrusTree/soil_moisture_percent/retained"},
    };

}  // namespace credentials
}  // namespace deskmate

#endif  // ROOT_DESKMATE_CREDENTIALS_H