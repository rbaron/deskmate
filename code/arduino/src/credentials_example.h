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

// Units.
// Our Arduino drawing library uses the CP437 character set.
constexpr char kUnitDegreeCelcius[] =
    "\xf8"
    "C";

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
constexpr int kDisplayHeight = 240;
constexpr int kDisplayWidth = 400;

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
        {"Monstera", "home/Monstera/soil_moisture_percent/retained",
         "home/Monstera/availability"},
        {"Ficus", "home/Ficus/soil_moisture_percent/retained",
         "home/Ficus/availability"},
    };

// MQTT-powered weather configs.
const std::vector<deskmate::app::MQTTFloatingPointSensorConfig>
    kMQTTWeatherConfigs{
        {"Zurich", "home/Zurich/temp/retained", "home/Zurich/availability"},
        {"Office", "home/AirSensor/air_temperature/retained",
         "home/AirSensor/availability"},
    };

// MQTT-powered weather configs.
const std::vector<deskmate::app::MQTTFloatingPointSensorConfig>
    kMQTTWeatherConfigs{
        {"Zurich", "home/Zurich/temp/retained", "home/Zurich/availability",
         kUnitDegreeCelcius},
        {"Office", "home/AirSensor/air_temperature/retained",
         "home/AirSensor/availability", kUnitDegreeCelcius},
    };

}  // namespace credentials
}  // namespace deskmate

#endif  // ROOT_DESKMATE_CREDENTIALS_H