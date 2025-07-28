#include "InputManager.h"
#include "constants.h"

InputManager::InputManager() {
  this->is_left_button_down = false;
  this->is_right_button_down = false;
  this->last_state_a = digitalRead(ROTARY_A);
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

// void IRAM_ATTR InputManager::readEncoder() {
//   int current_state_a = digitalRead(ROTARY_A);

//   if (current_state_a != last_state_a) {
//     if (digitalRead(ROTARY_B) != current_state_a) {
//       rotary_value++;
//     } else {
//       rotary_value--;
//     }

//     Serial.print("value - ");
//     Serial.println(rotary_value);
//   }

//   last_state_a = current_state_a;
// }
