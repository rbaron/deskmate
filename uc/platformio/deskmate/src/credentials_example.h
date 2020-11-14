#ifndef ROOT_DESKMATE_CREDENTIALS_H
#define ROOT_DESKMATE_CREDENTIALS_H

namespace deskmate {
namespace credentials {

// WiFi
constexpr char kWIFISSID[] = "";
constexpr char kWIFIPass[] = "";

// MQTT
constexpr char kMQTTUser[] = "";
constexpr char kMQTTPassword[] = "";
constexpr char kMQTTClientId[] = "deskmate";
constexpr char kMQTTServer[] = "";
constexpr int kMQTTPort = 1883;

}  // namespace credentials
}  // namespace deskmate

#endif  // ROOT_DESKMATE_CREDENTIALS_H