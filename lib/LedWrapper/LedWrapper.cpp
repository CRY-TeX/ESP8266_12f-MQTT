#include "LedWrapper.h"

#include "Arduino.h"

LedWrapper::LedWrapper(int pin)
{
    this->_pin = pin;
    this->_is_on = false;
    pinMode(this->_pin, OUTPUT);
}

void LedWrapper::on()
{
    if (!this->_is_on)
    {
        digitalWrite(this->_pin, HIGH);
        this->_is_on = true;
    }
}

void LedWrapper::off()
{
    if (this->_is_on)
    {
        digitalWrite(this->_pin, LOW);
        this->_is_on = false;
    }
}

bool LedWrapper::is_on() const
{
    return this->_is_on;
}