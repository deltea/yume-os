#pragma once

#include <Adafruit_GFX.h>

class InputManager {
  private:
    bool is_left_button_down;
    bool is_right_button_down;

  public:
    InputManager();

    bool isLeftButtonDown();
    bool isRightButtonDown();
};
