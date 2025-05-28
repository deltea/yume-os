#include "screens/gradient_screen.h"
#include "constants.h"

const uint8_t bayer2x2[2][2] = {
  { 0, 2 },
  { 3, 1 }
};

const uint8_t bayer4x4[4][4] = {
  {  0,  8,  2, 10 },
  { 12,  4, 14,  6 },
  {  3, 11,  1,  9 },
  { 15,  7, 13,  5 }
};

GradientScreen::GradientScreen(GFXcanvas16* c) {
  canvas = c;
}

void GradientScreen::init() {
  canvas->setTextColor(FG);
  canvas->setTextWrap(false);
  canvas->setTextSize(1);
  canvas->fillScreen(BG);

  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      // brightness from 0–4 (use x position to make a horizontal gradient)
      int brightness = map(x, 0, SCREEN_WIDTH - 1, 0, 16);

      // get threshold from Bayer matrix
      int bx = x % 4;
      int by = y % 4;
      int threshold = bayer4x4[by][bx];

      if (brightness > threshold) {
        canvas->drawPixel(x, y, ACCENT); // blue pixel
      } else {
        canvas->drawPixel(x, y, BG); // black pixel
      }
    }
  }
}

void GradientScreen::update() {

}

void GradientScreen::draw() {

}
