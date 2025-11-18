#include <SD.h>
#include "screens/LibraryScreen.h"
#include "InputManager.h"
#include "constants.h"
#include "cutepixel.h"
#include "monogram.h"

void LibraryScreen::init() {
  this->select_index = 0;

  ctx->canvas->setTextColor(FG);
  ctx->canvas->setTextWrap(false);
  ctx->canvas->setTextSize(1);
}

void LibraryScreen::update() {
  if (ctx->input->isLeftButtonDown()) {
    select_index++;
    if (select_index >= 4) {
      select_index = 0;
    }
  }
}

void LibraryScreen::draw() {
  ctx->canvas->fillScreen(BG);

  int count = 0;
  for (uint8_t row = 0; row < 2; row++) {
    for (uint8_t col = 0; col < 2; col++) {
      ctx->canvas->setCursor(col * 64 + 2, row * 32 + 2);
      ctx->canvas->setTextColor(count == select_index ? FG : 0xFF00);
      ctx->canvas->print(OPTIONS[count]);
      count++;
    }
  }
}
