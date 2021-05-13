#pragma once

class LedWrapper
{
private:
    int _pin;
    bool _is_on;

public:
    LedWrapper(int pin);

    void on();
    void off();

    bool is_on() const;
};