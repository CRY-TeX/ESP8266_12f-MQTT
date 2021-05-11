#pragma once

#include "Arduino.h"

#include "ESP8266WiFi.h"
#include "PubSubClient.h"
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#include <string>

class MqttClient
{
private:
    std::string _wifi_ssid;
    std::string _wifi_password;
    std::string _mqtt_broker_address;
    int _mqtt_port;
    std::string _default_topic;
    std::string _mqtt_connect_id;

    WiFiClient _wifi_client;
    PubSubClient _pub_sub_client;

    // FUNCTION DEFS
    bool _connect_to_wifi(const std::string &ssid, const std::string &password);
    bool _reconnect_to_wifi(const std::string &ssid, const std::string &password);

    bool _connect_to_mqtt_broker(const WiFiClient &wifi_client, const std::string &mqtt_broker_address, int mqtt_port, const std::string &mqtt_connect_id);

public:
    MqttClient(const std::string &wifi_ssid, const std::string &wifi_password, const std::string &mqtt_broker_address, int mqtt_port, const std::string &default_topic, const std::string &mqtt_connect_id = "ESP8266");

    void publish(const std::string &message);
    void publish(const std::string &topic, const std::string &message);
};