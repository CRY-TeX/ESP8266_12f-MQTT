#pragma once

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <memory>
#include <string>

#define DHTTYPE DHT22

class DHT22Monitor
{
private:
    uint8_t _dht_pin;
    std::shared_ptr<DHT_Unified> _dht;
    sensor_t _temperature_sensor_descr;
    sensor_t _humidity_sensor_descr;

    sensors_event_t _temp_event;
    sensors_event_t _hum_event;

public:
    DHT22Monitor(uint8_t pin);

    void read_data();

    const sensor_t *get_temp_sensor_descr() const;
    const sensor_t *get_hum_sensor_descr() const;

    std::string get_data();
};