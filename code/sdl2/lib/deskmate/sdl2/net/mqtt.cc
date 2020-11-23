#include "deskmate/sdl2/net/mqtt.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>

#include "MQTTClient.h"

namespace deskmate {
namespace sdl2 {
namespace net {
namespace {
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTMessageBuffer;
using deskmate::mqtt::MQTTMessageQueue;

constexpr bool kCleanSession = true;
constexpr int kKeepAliveIntervalSecs = 20;
// Quality of service 0 just so we match the Arduino implementation.
constexpr int kQoS = 0;

void OnConnLost(void* context, char* cause) {
  std::cerr << "MQTT connection lost!\n";
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

bool PahoMQTTManager::SubscribeOnly(const std::string& topic) {
  int ret = MQTTClient_subscribe(*mqtt_client_, topic.c_str(), kQoS);
  if (ret != MQTTCLIENT_SUCCESS) {
    return false;
  }
  return true;
}

bool PahoMQTTManager::Publish(const MQTTMessage& msg) {
  return MQTTClient_publish(*mqtt_client_, msg.topic.c_str(),
                            msg.payload.length(), msg.payload.c_str(), kQoS,
                            /*retained=*/false,
                            /*dt=*/nullptr) == MQTTCLIENT_SUCCESS;
}

bool PahoMQTTManager::Process() {
  std::lock_guard<std::mutex> guard(lock_);
  return ProcessInner();
};

bool PahoMQTTManager::EnqueueForSending(const MQTTMessage& msg) {
  std::lock_guard<std::mutex> guard(lock_);
  out_queue_.push(msg);
  return true;
}


// Static callback. context is the address of a PahoMQTTManager instance.
int PahoMQTTManager::OnMessageReceived(void* context, char* topic,
                                       int topic_len,
                                       MQTTClient_message* message) {
  if (context) {
    PahoMQTTManager* instance = static_cast<PahoMQTTManager*>(context);
    const MQTTMessage msg{
        std::string(topic, topic_len > 0 ? topic_len : std::strlen(topic)),
        std::string(static_cast<char*>(message->payload), message->payloadlen)};
    {
      std::lock_guard<std::mutex> guard(instance->lock_);
      instance->Dispatch(msg);
    }
  }
  MQTTClient_free(topic);
  MQTTClient_freeMessage(&message);
  return true;
}

}  // namespace net
}  // namespace sdl2
}  // namespace deskmate