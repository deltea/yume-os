#include <SD.h>
#include "screens/LibraryScreen.h"
#include "constants.h"
#include "cutepixel.h"
#include "monogram.h"

LibraryScreen::LibraryScreen(ScreenManager* screenManager, GFXcanvas16* canvas, State* state, InputManager* inputManager) : screenManager(screenManager), canvas(canvas), state(state), inputManager(inputManager) {
  this->select_index = 0;
};

void LibraryScreen::init() {
  canvas->setTextColor(FG);
  canvas->setTextWrap(false);
  canvas->setTextSize(1);
}

void LibraryScreen::update() {
  if (inputManager->isLeftButtonDown()) {
    select_index++;
    if (select_index >= 4) {
      select_index = 0;
    }
  }
}

void LibraryScreen::draw() {
  canvas->fillScreen(BG);

  int count = 0;
  for (uint8_t row = 0; row < 2; row++) {
    for (uint8_t col = 0; col < 2; col++) {
      canvas->setCursor(col * 64 + 2, row * 32 + 2);
      canvas->setTextColor(count == select_index ? FG : 0xFF00);
      canvas->print(OPTIONS[count]);
      count++;
    }
  }
}
