#include "DHT22Monitor.h"
#include "Arduino.h"
#include <sstream>

DHT22Monitor::DHT22Monitor(uint8_t pin)
{
    this->_dht_pin = pin;
    this->_dht = std::make_shared<DHT_Unified>(this->_dht_pin, DHTTYPE);
    this->_dht->begin();
    this->_dht->temperature().getSensor(&(this->_temperature_sensor_descr));
    this->_dht->humidity().getSensor(&(this->_humidity_sensor_descr));
}

void DHT22Monitor::read_data()
{
    this->_dht->temperature().getEvent(&this->_temp_event);
    this->_dht->humidity().getEvent(&this->_hum_event);
}

const sensor_t *DHT22Monitor::get_temp_sensor_descr() const
{
    return &(this->_temperature_sensor_descr);
}

const sensor_t *DHT22Monitor::get_hum_sensor_descr() const
{
    return &(this->_humidity_sensor_descr);
}

std::string DHT22Monitor::get_data()
{
    std::ostringstream ss_data;
    ss_data << "{";
    if (isnan(this->_temp_event.temperature))
        ss_data << "\"Temperature\": \"NaN\",";
    else
        ss_data << "\"Temperature\":\"" << this->_temp_event.temperature << "\",";

    if (isnan(this->_hum_event.relative_humidity))
        ss_data << "\"Humidity\":\"NaN\"";
    else
        ss_data << "\"Humidity\":\"" << this->_hum_event.relative_humidity << "\"";

    ss_data << "}";

    return ss_data.str();
}