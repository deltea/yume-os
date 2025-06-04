#pragma once

#include <Adafruit_GFX.h>

class InputManager {
  private:
    bool is_left_button_down;
    bool is_right_button_down;

  public:
    void updateInput();
    bool isLeftButtonDown() {
      return is_left_button_down;
    }
    bool isRightButtonDown() {
      return is_right_button_down;
    }
};
