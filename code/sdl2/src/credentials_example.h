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
constexpr unsigned int kDisplayHeight = 240;
constexpr unsigned int kDisplayWidth = 400;

const std::vector<MQTTConfig> kMQTTConfigs{
    {"Desk", "home/switch/cmnd/officedesk/POWER",
     "home/switch/stat/officedesk/POWER"},
    {"Office", "home/switch/cmnd/office/POWER",
     "home/switch/stat/office/POWER"},
};

}  // namespace credentials
}  // namespace deskmate

#endif  // ROOT_DESKMATE_CREDENTIALS_H