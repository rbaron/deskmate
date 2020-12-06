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
constexpr char kUnitDegreeCelcius[] = "°C";

// MQTT.
constexpr char kMQTTUser[] = "mqttuser";
constexpr char kMQTTPassword[] = "mqttpassword";
constexpr char kMQTTClientId[] = "deskmate-sdl2";
constexpr char kMQTTServer[] = "mqtt-broker-hostname-or-ip-address";
constexpr int kMQTTPort = 1883;

// SDL config.
// Filesystem path to a monospace font used for rendering text.
// This can be relative to the directory where the binary is executed or
// absolute.
constexpr char kFontPath[] = "../assets/fonts/DejaVu/DejaVuSansMono.ttf";
constexpr int kDisplayHeight = 240;
constexpr int kDisplayWidth = 400;

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
         "home/Monstera/availability", "%"},
        {"Ficus", "home/Ficus/soil_moisture_percent/retained",
         "home/Ficus/availability", "%"},
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