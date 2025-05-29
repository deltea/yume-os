#include "screens/ConfirmationScreen.h"
#include "ScreenManager.h"
#include "AppState.h"
#include "constants.h"
#include "cutepixel.h"
#include "monogram.h"

ConfirmationScreen::ConfirmationScreen(ScreenManager* screenManager, GFXcanvas16* canvas, AppState* appState)
  : screenManager(screenManager), canvas(canvas), state(appState) {};

void ConfirmationScreen::init() {
  canvas->setTextColor(FG);
  canvas->setTextWrap(false);
  canvas->setTextSize(1);

  // canvas->getTextBounds(danger_text, 0, 0, nullptr, nullptr, &text_width, nullptr);
  text_width = danger_text.length() * 7;
}

void ConfirmationScreen::update() {
  scroll += scroll_speed;
  if (scroll >= text_width) scroll = 0;
}

void ConfirmationScreen::draw() {
  canvas->fillScreen(BG);

  // danger bars
  canvas->fillRect(0, margin, SCREEN_WIDTH, 12, FG);
  canvas->fillRect(0, SCREEN_HEIGHT - 12 - margin, SCREEN_WIDTH, 12, FG);

  canvas->setFont(&cute_pixel8pt7b);
  canvas->setTextColor(BG);

  int x = -scroll;
  while (x < SCREEN_WIDTH + text_width) {
    canvas->setTextColor(BG);
    canvas->setCursor(x, 13);
    canvas->print(danger_text);
    canvas->setCursor(x - text_width / 2, SCREEN_HEIGHT - 7);
    canvas->print(danger_text);

    x += text_width;
  }

  // confirmation prompt
  canvas->setTextColor(FG);
  canvas->setCursor(SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 - 16);
  canvas->print("are you sure?");

  canvas->setFont(&monogram8pt7b);
  canvas->setCursor(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 10);
  canvas->print("yup");

  canvas->setCursor(SCREEN_WIDTH / 2 - 14, SCREEN_HEIGHT / 2 + 25);
  canvas->print("nope");
}
