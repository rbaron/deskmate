#ifndef DESKMATE_ARDUINO_NET_WIFI_H
#define DESKMATE_ARDUINO_NET_WIFI_H

namespace deskmate {
namespace arduino {
namespace net {

class WiFiManager {
 public:

  WiFiManager(const char* ssid, const char* password)
      : ssid_(ssid), password_(password) {}
  bool Connect();
  bool MaybeReconnect();

 private:
  const char* ssid_;
  const char* password_;
};

}  // namespace net
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_NET_WIFI_H