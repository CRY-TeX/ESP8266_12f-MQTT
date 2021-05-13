#pragma once

class ButtonWrapper
{
private:
    int _pin;
    bool _toggle_state_on;
    bool _state;
    bool _last_state;

public:
    ButtonWrapper(int pin, bool start_toggle_state = false);

    bool is_pressed();
    bool is_toggled();

    bool read_state();
};