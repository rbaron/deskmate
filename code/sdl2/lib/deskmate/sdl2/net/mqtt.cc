#include "deskmate/sdl2/net/mqtt.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>

#include "MQTTClient.h"

namespace deskmate {
namespace sdl2 {
namespace net {
namespace {
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTMessageBuffer;
using deskmate::mqtt::MQTTMessageQueue;

constexpr bool kCleanSession = false;
constexpr int kKeepAliveIntervalSecs = 20;
// Quality of service 0 just so we match the Arduino implementation.
constexpr int kQoS = 0;

void OnConnLost(void* context, char* cause) {
  std::cout << "Connection lost!\n";
}

}  // namespace

PahoMQTTManager::PahoMQTTManager(const char* server, int port,
                                 const char* username, const char* password,
                                 const char* client_id) {
  mqtt_client_ =
      std::unique_ptr<MQTTClient, PahoMQTTClientDeleter>(new MQTTClient);
  if (!mqtt_client_) {
    std::cerr << "Unable to initialize MQTTClient" << std::endl;
    std::exit(-1);
  }

  int ret;
  if ((ret = MQTTClient_create(mqtt_client_.get(), server, client_id,
                               MQTTCLIENT_PERSISTENCE_NONE, NULL)) !=
      MQTTCLIENT_SUCCESS) {
    std::cerr << "Unable to create client: " << ret << std::endl;
    std::exit(-1);
  }

  if ((ret = MQTTClient_setCallbacks(*mqtt_client_, this, &OnConnLost,
                                     &PahoMQTTManager::OnMessageReceived,
                                     NULL)) != MQTTCLIENT_SUCCESS) {
    std::cerr << "Unable to set callbacks: " << ret << std::endl;
    std::exit(-1);
  }

  conn_opts_ = MQTTClient_connectOptions_initializer;
  conn_opts_.keepAliveInterval = kKeepAliveIntervalSecs;
  conn_opts_.cleansession = kCleanSession;
  conn_opts_.username = username;
  conn_opts_.password = password;
}

bool PahoMQTTManager::Connect() {
  int ret;
  if ((ret = MQTTClient_connect(*mqtt_client_, &conn_opts_)) !=
      MQTTCLIENT_SUCCESS) {
    std::cerr << "Failed to connect: " << ret << std::endl;
    return false;
  }

  return true;
}

bool PahoMQTTManager::Subscribe(const std::string& topic) {
  int ret = MQTTClient_subscribe(*mqtt_client_, topic.c_str(), kQoS);
  if (ret != MQTTCLIENT_SUCCESS) {
    std::cerr << "Failed to subscribe: " << ret << std::endl;
    return false;
  }
  subscribed_topics_.push_back(topic);
  return true;
}

// TODO: move this function to the base class. Create a virtual private
// Publish(const &MQTTMessage) to be implemented by derived classes.
bool PahoMQTTManager::Process() {
  if (!MQTTClient_isConnected(*mqtt_client_)) {
    std::cerr << "Client is not connected. Reconnecting and re-subscribing\n";
    if (!Connect()) {
      return false;
    }
    // TODO: better error handling when re-subscribing.
    // Re-subscribe to topics.
    std::for_each(subscribed_topics_.cbegin(), subscribed_topics_.cend(),
                  [this](const std::string& topic) { Subscribe(topic); });
  }

  // Send messages.
  while (!out_queue_.empty()) {
    const MQTTMessage& msg = out_queue_.front();
    std::cout << "Will send message: " << msg.topic << " -> " << msg.payload
              << std::endl;
    if (MQTTClient_publish(*mqtt_client_, msg.topic.c_str(),
                           msg.payload.length(), msg.payload.c_str(), kQoS,
                           /*retained=*/false,
                           /*dt=*/nullptr) == MQTTCLIENT_SUCCESS) {
      std::cout << "Sent!\n";
    } else {
      std::cout << "Failed!\n";
      return false;
    }
    out_queue_.pop();
  }
  return true;
}

int PahoMQTTManager::OnMessageReceived(void* context, char* topic,
                                       int topic_len,
                                       MQTTClient_message* message) {
  if (context) {
    PahoMQTTManager* instance = static_cast<PahoMQTTManager*>(context);
    instance->in_queue_.push(
        // topic_len is only set if there is a \0 in topic.
        {std::string(topic, topic_len > 0 ? topic_len : std::strlen(topic)),
         std::string(static_cast<char*>(message->payload),
                     message->payloadlen)});
    std::cout << "Received message: " << instance->in_queue_.back().topic
              << " -> " << instance->in_queue_.back().payload << std::endl;
  }
  MQTTClient_free(topic);
  MQTTClient_freeMessage(&message);
  return true;
}

}  // namespace net
}  // namespace sdl2
}  // namespace deskmate