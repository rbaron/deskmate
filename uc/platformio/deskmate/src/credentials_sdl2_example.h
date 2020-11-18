#ifndef ROOT_DESKMATE_CREDENTIALS_H
#define ROOT_DESKMATE_CREDENTIALS_H

#include <string>
#include <vector>

// TODO: Make these configs dynamic. Ideally configured from a web page, like
// Tasmota/ESPHome.
namespace deskmate {
namespace credentials {

// WiFi.
constexpr char kWIFISSID[] = "your-wifi-ssid";
constexpr char kWIFIPass[] = "your-wifi-password";

// MQTT.
constexpr char kMQTTUser[] = "mqttuser";
constexpr char kMQTTPassword[] = "mqttpassword";
constexpr char kMQTTClientId[] = "deskmate-sdl2";
constexpr char kMQTTServer[] = "your-server-hostname-or-ip-addr";
constexpr int kMQTTPort = 1883;

// SDL config.
// Filesystem path to a monospace font used for rendering text.
// This can be relative to the directory where the binary is executed or
// absolute.
constexpr char kFontPath[] = "../assets/fonts/DejaVu/DejaVuSansMono.ttf";
constexpr unsigned int kDisplayHeight = 240;
constexpr unsigned int kDisplayWidth = 400;

// MQTT commands/state topics. For now these are the requirements:
// 1. command topics accept the strings "ON", "OFF"
// 2. state topics publish the strings "ON", "OFF"
// This is the default in Tasmota.
struct MQTTConfigType {
  std::string display_name;
  std::string command_topic;
  std::string state_topic;
};

const std::vector<MQTTConfigType> kMQTTConfigs{
    {"Office", "home/switch/cmnd/office/POWER",
     "home/switch/stat/office/POWER"},
    {"Living room", "home/switch/cmnd/livingroom2/POWER1",
     "home/switch/stat/livingroom2/POWER1"},
};

}  // namespace credentials
}  // namespace deskmate

#endif  // ROOT_DESKMATE_CREDENTIALS_H