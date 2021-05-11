#include "MqttClient.h"
#include "../macros/macros.h"

MqttClient::MqttClient(const std::string &wifi_ssid, const std::string &wifi_password, const std::string &mqtt_broker_address, int mqtt_port, const std::string &default_topic, const std::string &mqtt_connect_id)
{
    this->_wifi_ssid = wifi_ssid;
    this->_wifi_password = wifi_password;
    this->_mqtt_broker_address = mqtt_broker_address;
    this->_mqtt_port = mqtt_port;
    this->_default_topic = default_topic;
    this->_mqtt_connect_id;

    this->_connect_to_wifi(this->_wifi_ssid, this->_wifi_password);
    this->_connect_to_mqtt_broker(this->_wifi_client, this->_mqtt_broker_address, this->_mqtt_port, this->_mqtt_connect_id);
}

bool MqttClient::_connect_to_wifi(const std::string &ssid, const std::string &password)
{
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        LOG_SERIAL("Connecting to WIFI...");
    }

    LOG_SERIAL("Succesfully connected to wifi");

    return true;
}

bool MqttClient::_reconnect_to_wifi(const std::string &ssid, const std::string &password)
{
    if (WiFi.status() == WL_CONNECTED)
        return true;

    LOG_SERIAL("Connection lost!");
    return this->_connect_to_wifi(ssid, password);
}

void MqttClient::publish(const std::string &topic, const std::string &message)
{
    this->_reconnect_to_wifi(this->_wifi_ssid, this->_wifi_password);

    // FIXME: if mqtt broker shuts down publish will continue with no connection - broker will not receive data on new startup
    if (this->_pub_sub_client.connected())
        this->_connect_to_mqtt_broker(this->_wifi_client, this->_mqtt_broker_address, this->_mqtt_port, this->_mqtt_connect_id);

    this->_pub_sub_client.publish(topic.c_str(), message.c_str());

    LOG_SERIAL("Successfully published!");
}

void MqttClient::publish(const std::string &message)
{
    this->publish(this->_default_topic, message);
}

bool MqttClient::_connect_to_mqtt_broker(const WiFiClient &wifi_client, const std::string &mqtt_broker_address, int mqtt_port, const std::string &mqtt_connect_id)
{

    this->_pub_sub_client.setClient((WiFiClient &)wifi_client);
    this->_pub_sub_client.setServer(mqtt_broker_address.c_str(), mqtt_port);
    while (!this->_pub_sub_client.connected())
    {
        this->_pub_sub_client.connect(mqtt_connect_id.c_str());
        delay(500);
        LOG_SERIAL("Connecting to mqtt broker...");
    }

    LOG_SERIAL("Successfully connected to mqtt broker!");

    return true;
}