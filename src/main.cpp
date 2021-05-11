#include <Arduino.h>

#include "consts.h"
#include "../lib/macros/macros.h"
#include "../lib/DHT22Monitor/DHT22Monitor.h"
#include "../lib/MqttClient/MqttClient.h"

std::shared_ptr<DHT22Monitor> dht_monitor;
std::shared_ptr<MqttClient> mqtt_client;

void setup()
{
  BEGIN_SERIAL();

  dht_monitor = std::make_shared<DHT22Monitor>(DHT_SENSOR_PIN);
  mqtt_client = std::make_shared<MqttClient>(ssid, password, mqtt_broker_IP, mqtt_port, "DHT22");
}

void loop()
{
  dht_monitor->read_data();
  LOG_SERIAL(dht_monitor->get_data().c_str());

  mqtt_client->publish(dht_monitor->get_data());

  delay(DELAY_MS);
}