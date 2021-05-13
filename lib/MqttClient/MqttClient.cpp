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
    this->_wifi_retries = 0;

    this->_connect_to_wifi(this->_wifi_ssid, this->_wifi_password);
    this->_connect_to_mqtt_broker(this->_wifi_client, this->_mqtt_broker_address, this->_mqtt_port, this->_mqtt_connect_id);
}

bool MqttClient::_connect_to_wifi(const std::string &ssid, const std::string &password, unsigned int retries)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        LOG_SERIAL("Succesfully connected to wifi");
        this->_wifi_retries = 0;
        return true;
    }
    else
    {
        LOG_SERIAL("Connecting to WIFI...");

        if (this->_wifi_retries > retries)
        {
            this->_wifi_retries = 0;
            WiFi.begin(ssid.c_str(), password.c_str());
        }

        this->_wifi_retries++;
        return WiFi.status() == WL_CONNECTED;
    }
}

void MqttClient::publish(const std::string &topic, const std::string &message)
{
    if (WiFi.status() == WL_CONNECTED && this->_pub_sub_client.connected())
    {
        this->_pub_sub_client.publish(topic.c_str(), message.c_str());
        LOG_SERIAL("Successfully published!");
    }
}

void MqttClient::publish(const std::string &message)
{
    this->publish(this->_default_topic, message);
}

bool MqttClient::_connect_to_mqtt_broker(const WiFiClient &wifi_client, const std::string &mqtt_broker_address, int mqtt_port, const std::string &mqtt_connect_id)
{

    if (this->_pub_sub_client.connected())
    {
        LOG_SERIAL("connected to mqtt broker!");
        return true;
    }
    else
    {
        LOG_SERIAL("Connecting to mqtt broker...");

        this->_pub_sub_client.setClient((WiFiClient &)wifi_client);
        this->_pub_sub_client.setServer(mqtt_broker_address.c_str(), mqtt_port);
        this->_pub_sub_client.connect(mqtt_connect_id.c_str());

        return this->_pub_sub_client.connected();
    }
}

void MqttClient::connect()
{
    if (WiFi.status() != WL_CONNECTED)
        this->_connect_to_wifi(this->_wifi_ssid, this->_wifi_password);

    if (!this->_pub_sub_client.connected())
        this->_connect_to_mqtt_broker(this->_wifi_client, this->_mqtt_broker_address, this->_mqtt_port, this->_mqtt_connect_id);
}

bool MqttClient::is_connected()
{
    return WiFi.status() == WL_CONNECTED && this->_pub_sub_client.connected();
}