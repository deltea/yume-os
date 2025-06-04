#include "InputManager.h"
#include "constants.h"

void InputManager::updateInput() {
  is_left_button_down = (digitalRead(BUTTON_LEFT) == LOW);
  is_right_button_down = (digitalRead(BUTTON_RIGHT) == LOW);
}
