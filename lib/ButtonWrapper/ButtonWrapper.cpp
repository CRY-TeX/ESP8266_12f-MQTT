#include "ButtonWrapper.h"

#include "Arduino.h"

ButtonWrapper::ButtonWrapper(int pin, bool start_toggle_state)
{
    this->_pin = pin;
    this->_state = false;
    this->_last_state = false;
    this->_toggle_state_on = start_toggle_state;
    pinMode(this->_pin, INPUT);
}

bool ButtonWrapper::read_state()
{
    this->_last_state = this->_state;
    this->_state = digitalRead(this->_pin) == HIGH;
    if (this->_state && this->_state != this->_last_state)
    {
        this->_toggle_state_on = !this->_toggle_state_on;
    }

    return this->_state;
}

bool ButtonWrapper::is_pressed()
{
    return this->_state;
}

bool ButtonWrapper::is_toggled()
{
    return this->_toggle_state_on;
}