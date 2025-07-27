#include <SD.h>
#include "screens/QueueScreen.h"
#include "constants.h"
#include "cutepixel.h"
#include "monogram.h"

QueueScreen::QueueScreen(ScreenManager* screenManager, GFXcanvas16* canvas, State* state, InputManager* inputManager) : screenManager(screenManager), canvas(canvas), state(state), inputManager(inputManager) {
  this->select_index = 0;
};

void QueueScreen::init() {
  canvas->setTextColor(FG);
  canvas->setTextWrap(false);
  canvas->setTextSize(1);
}

void QueueScreen::update() {
  if (inputManager->isLeftButtonDown()) {
    select_index = (select_index + 1) % state->queue.size();
  }
}

void QueueScreen::draw() {
  canvas->fillScreen(BG);

  canvas->setTextColor(FG);
  canvas->setFont(&monogram8pt7b);
  canvas->setCursor(SCREEN_WIDTH / 2 - 8, 10);
  canvas->print("QUEUE");

  for (size_t i = 0; i < state->queue.size(); i++) {
    if (select_index == i) {
      canvas->fillRect(0, 18 + 18 * i, SCREEN_WIDTH, 18, ACCENT);
    }

    const Track& track = state->queue[i];
    canvas->setCursor(8, 30 + 18 * i);
    canvas->setTextColor(select_index == i ? BG : FG);
    canvas->print(track.title);
    canvas->print(" - ");
    canvas->print(track.artist);
  }
}
