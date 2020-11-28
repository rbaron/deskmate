#include "deskmate/sdl2/net/mqtt.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <thread>

#include "MQTTClient.h"

namespace deskmate {
namespace sdl2 {
namespace net {
namespace {
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTMessageBuffer;
using deskmate::mqtt::MQTTSubscriber;

constexpr bool kCleanSession = true;
constexpr int kKeepAliveIntervalSecs = 20;
// Match the Arduino QoS, although Paho supports higher QoS.
constexpr int kSubscribeQoS = 1;
constexpr int kPublishQoS = 0;

void OnConnLost(void* context, char* cause) {
  std::cerr << "MQTT connection lost!\n";
}

void TraceCallback(enum MQTTCLIENT_TRACE_LEVELS level, char* message) {
  std::cerr << "[trace] " << message << std::endl;
}
}  // namespace

PahoMQTTManager::PahoMQTTManager(const char* server, int port,
                                 const char* username, const char* password,
                                 const char* client_id) {
  mqtt_client_ =
      std::unique_ptr<MQTTClient, PahoMQTTClientDeleter>(new MQTTClient);

  // Debugging traces.
  // MQTTClient_setTraceLevel(MQTTCLIENT_TRACE_MINIMUM);
  // MQTTClient_setTraceCallback(TraceCallback);

  if (!mqtt_client_) {
    std::cerr << "Unable to initialize MQTTClient" << std::endl;
    std::exit(-1);
  }

  int ret;
  if ((ret = MQTTClient_create(mqtt_client_.get(), server, client_id,
                               MQTTCLIENT_PERSISTENCE_NONE, nullptr)) !=
      MQTTCLIENT_SUCCESS) {
    std::cerr << "Unable to create client: " << ret << std::endl;
    std::exit(-1);
  }

  if ((ret = MQTTClient_setCallbacks(*mqtt_client_, this, nullptr,
                                     &PahoMQTTManager::OnMessageReceived,
                                     nullptr)) != MQTTCLIENT_SUCCESS) {
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

bool PahoMQTTManager::IsConnected() const {
  return MQTTClient_isConnected(*mqtt_client_);
}

bool PahoMQTTManager::Subscribe(MQTTSubscriber* subscriber) {
  for (const std::string& topic : subscriber->SubscriptionTopics()) {
    subscribers_by_topic_[topic].push_back(subscriber);

    if (MQTTClient_subscribe(*mqtt_client_, topic.c_str(), kSubscribeQoS) !=
        MQTTCLIENT_SUCCESS) {
      std::cerr << "Error subscribing to " << topic << "\n";
    }
  }
  return true;
}

bool PahoMQTTManager::Publish(const MQTTMessage& msg) {
  return MQTTClient_publish(*mqtt_client_, msg.topic.c_str(),
                            msg.payload.length(), msg.payload.c_str(),
                            kPublishQoS,
                            /*retained=*/false,
                            /*dt=*/nullptr) == MQTTCLIENT_SUCCESS;
}

bool PahoMQTTManager::Tick() {
  if (!IsConnected()) {
    std::cerr << "[mqtt] Connection lost. Reconnecting.\n";
    if (!Connect()) {
      std::cerr << "[mqtt] Unable to connect.\n";
      return false;
    }

    // Re-subscribe to previously subscribed topics.
    std::cerr << "[mqtt] Will resubscribe to topics.\n";
    std::for_each(
        subscribers_by_topic_.cbegin(), subscribers_by_topic_.cend(),
        [this](
            const std::pair<std::string, std::vector<MQTTSubscriber*>>& pair) {
          MQTTClient_subscribe(*mqtt_client_, pair.first.c_str(),
                               kSubscribeQoS);
        });
    std::cerr << "[mqtt] Done resubscribing.\n";
  }

  // Forwards the received messages to registered subscribers.
  {
    std::shared_lock lock(input_queue_mutex_);
    while (!in_queue_.empty()) {
      const MQTTMessage& msg = in_queue_.front();
      for (MQTTSubscriber* subs : subscribers_by_topic_[msg.topic]) {
        subs->HandleMessage(msg);
      }
      in_queue_.pop();
    }
  }

  return true;
}

// Static callback. `context` holds the address of an instance of this class.
int PahoMQTTManager::OnMessageReceived(void* context, char* topic,
                                       int topic_len,
                                       MQTTClient_message* message) {
  if (context) {
    PahoMQTTManager* instance = static_cast<PahoMQTTManager*>(context);
    const MQTTMessage msg{
        std::string(topic, topic_len > 0 ? topic_len : std::strlen(topic)),
        std::string(static_cast<char*>(message->payload), message->payloadlen)};
    std::unique_lock lock(instance->input_queue_mutex_);
    instance->in_queue_.push(msg);
  }
  MQTTClient_free(topic);
  MQTTClient_freeMessage(&message);
  return true;
}

}  // namespace net
}  // namespace sdl2
}  // namespace deskmate