#include "InputManager.h"
#include "constants.h"

InputManager::InputManager() {
  this->is_left_button_down = false;
  this->is_right_button_down = false;
}

bool InputManager::isLeftButtonDown() {
  bool is_down = (digitalRead(BUTTON_LEFT) == LOW);
  if (is_down && !is_left_button_down) {
    is_left_button_down = true;
    return true;
  }

  is_left_button_down = is_down;
  return false;
}

bool InputManager::isRightButtonDown() {
  bool is_down = (digitalRead(BUTTON_RIGHT) == LOW);
  if (is_down && !is_right_button_down) {
    is_right_button_down = true;
    return true;
  }

  is_right_button_down = is_down;
  return false;
}
