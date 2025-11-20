#include "InputManager.h"
#include "constants.h"

InputManager::InputManager() {
  this->is_left_button_down = false;
  this->is_right_button_down = false;

  this->rotary_last_state = HIGH;
  this->rotary_value = 0;
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

void IRAM_ATTR InputManager::readEncoder() {
  int rotary_current_state = digitalRead(ROTARY_ROT);

  if (rotary_current_state != rotary_last_state) {
    rotary_value += (digitalRead(ROTARY_BUTTON) != rotary_current_state) ? 1 : -1;
    Serial.println(rotary_value);
  }

  rotary_last_state = rotary_current_state;
}

int InputManager::getRotaryValue() {
  return rotary_value;
}
