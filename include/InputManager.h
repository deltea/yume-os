#pragma once

#include <Adafruit_GFX.h>

class InputManager {
  private:
    bool is_left_button_down;
    bool is_right_button_down;

    volatile int rotary_last_state;
    volatile int rotary_value = 0;

  public:
    InputManager();

    bool isLeftButtonDown();
    bool isRightButtonDown();
    void readEncoder();
    int getRotaryValue();
};
