#ifndef DESKMATE_APP_CONFIG_H
#define DESKMATE_APP_CONFIG_H

#include <string>

namespace deskmate {
namespace app {

// MQTT commands/state topics. For now these are the requirements:
// 1. command topics accept the strings "ON", "OFF"
// 2. state topics publish the strings "ON", "OFF"
// This is the default in Tasmota.
// TODO: make this more flexible. A good example is how home-assistant does the
// config, in which it allows arbitrary payloads, templating etc.
struct MQTTConfig {
  std::string display_name;
  std::string command_topic;
  std::string state_topic;
};

struct MQTTFloatingPointSensorConfig {
  std::string display_name;
  std::string value_topic;
  std::string availability_topic;
  std::string unit;
};

}  // namespace app
}  // namespace deskmate

#endif  // DESKMATE_APP_CONFIG_H