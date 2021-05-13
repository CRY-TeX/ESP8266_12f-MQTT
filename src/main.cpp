#include <Arduino.h>

#include "consts.h"
#include "../lib/macros/macros.h"
#include "../lib/DHT22Monitor/DHT22Monitor.h"
#include "../lib/MqttClient/MqttClient.h"
#include "../lib/LedWrapper/LedWrapper.h"
#include "../lib/ButtonWrapper/ButtonWrapper.h"

// TIMER FOR MILLIS TO HAVE NON BLOCKING EXECUTION
// TODO: make interface handeler for this
unsigned long dht_timer_now = 0;
unsigned long button_timer_now = 0;
unsigned long led_timer_now = 0;
unsigned long mqtt_reconnect_timer_now = 0;

// STATE OF BUTTON
bool button_state = true;

// COMPONENT CLASSES
std::shared_ptr<LedWrapper> status_ok_led;
std::shared_ptr<LedWrapper> status_error_led;
std::shared_ptr<ButtonWrapper> led_switch_button;
std::shared_ptr<DHT22Monitor> dht_monitor;
std::shared_ptr<MqttClient> mqtt_client;

void setup()
{
  BEGIN_SERIAL();

  status_ok_led = std::make_shared<LedWrapper>(consts::LED_OK_PIN);
  status_error_led = std::make_shared<LedWrapper>(consts::LED_ERROR_PIN);
  // status_error_led->on();

  led_switch_button = std::make_shared<ButtonWrapper>(consts::BUTTON_SWICH_PIN, button_state);

  dht_monitor = std::make_shared<DHT22Monitor>(consts::DHT_SENSOR_PIN);
  mqtt_client = std::make_shared<MqttClient>(consts::SSID, consts::PASSWORD, consts::MQTT_BROKER_IP, consts::MQTT_PORT, "DHT22");
}

void loop()
{

  if (millis() - button_timer_now > consts::BUTTON_DELAY_INTERVAL)
  {
    button_timer_now = millis();
    led_switch_button->read_state();
    button_state = led_switch_button->is_toggled();
  }

  if (millis() - led_timer_now > consts::LED_DELAY_INTERVAL)
  {
    led_timer_now = millis();
    if (button_state)
    {
      if (dht_monitor->is_all_data_valid() && mqtt_client->is_connected())
      {
        status_error_led->off();
        status_ok_led->on();
      }
      else
      {
        status_ok_led->off();
        status_error_led->on();
      }
    }
    else
    {
      status_ok_led->off();
      status_error_led->off();
    }
  }

  if (millis() - mqtt_reconnect_timer_now > consts::MQTT_RECONNECT_INTERVAL)
  {
    mqtt_reconnect_timer_now = millis();
    mqtt_client->connect();
  }

  if (millis() - dht_timer_now > consts::DHT_DELAY_INTERVAL)
  {
    dht_timer_now = millis();
    dht_monitor->read_data();
    LOG_SERIAL(dht_monitor->get_data().c_str());
    mqtt_client->publish(dht_monitor->get_data());
  }
}