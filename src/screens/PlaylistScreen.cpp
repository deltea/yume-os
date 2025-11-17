#include <SD.h>
#include "screens/PlaylistScreen.h"
#include "constants.h"
#include "cutepixel.h"
#include "monogram.h"
#include "utils.h"
#include "icons.h"

PlaylistScreen::PlaylistScreen(
  ScreenManager* screenManager, GFXcanvas16* canvas, State* state, InputManager* inputManager, FileManager* fileManager
) : screenManager(screenManager), canvas(canvas), state(state), inputManager(inputManager), fileManager(fileManager) {};

void PlaylistScreen::init() {
  canvas->setTextColor(FG);
  canvas->setTextWrap(false);
  canvas->setTextSize(1);
  canvas->setFont(&monogram8pt7b);
}

void PlaylistScreen::update() {
  if (inputManager->isLeftButtonDown()) {
    select_index = (select_index + 1) % state->queue.size();
  }
}

void PlaylistScreen::draw() {
  canvas->fillScreen(BG);

  canvas->drawRGBBitmap(1, 3, ICON_PLAYLIST, 9, 8);
  canvas->setCursor(14, 9);
  canvas->print("PLAYLISTS");
  canvas->fillRect(0, 16, SCREEN_WIDTH, 1, FG);
}
