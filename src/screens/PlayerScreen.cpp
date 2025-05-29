#include <Adafruit_GFX.h>
#include "screens/PlayerScreen.h"
#include "ScreenManager.h"
#include "AppState.h"
#include "constants.h"
#include "monogram.h"
#include "cutepixel.h"
#include "cover.h"
#include "utils.h"

PlayerScreen::PlayerScreen(ScreenManager* screenManager, GFXcanvas16* canvas, AppState* appState) : screenManager(screenManager), canvas(canvas), state(appState) {
  this->last_frame_time = 0;
  this->dt = 0;
  this->title_scroll = 0;
  this->title_scroll_speed = 1;
  this->scroll_timer = 0;
  this->scroll_delay = 5;
};

void PlayerScreen::init() {
  canvas->setTextWrap(false);
  canvas->setTextSize(1);
}

void PlayerScreen::update() {
  unsigned long now = millis();
  dt = (now - last_frame_time) / 1000.0f;
  last_frame_time = now;

  if (scroll_timer < scroll_delay) {
    scroll_timer += dt;
    return;
  }

  title_scroll += title_scroll_speed;
  if (title_scroll > String(state->getCurrentTrack().title + " - " + state->getCurrentTrack().artist).length() * 6) {
    title_scroll = 0;
    scroll_timer = 0;
  }
}

void PlayerScreen::draw() {
  canvas->fillScreen(BG);

  // progress bar
  canvas->fillRect(0, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT, SCREEN_WIDTH / 3 * 2, PROGRESS_BAR_HEIGHT, state->getCurrentTrack().color);

  // song duration and current time
  canvas->setTextColor(FG);
  canvas->setFont(&monogram8pt7b);
  canvas->setCursor(0, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 6);
  canvas->print("2:43");

  canvas->setCursor(SCREEN_WIDTH - 4 * 7, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 6);
  canvas->print(String(state->getCurrentTrack().duration));

  // song name and artist
  canvas->setTextColor(state->getCurrentTrack().color);
  canvas->setCursor(0 - title_scroll, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 20);
  canvas->print(state->getCurrentTrack().title);
  canvas->setTextColor(FG);
  canvas->setCursor(state->getCurrentTrack().title.length() * 6 - title_scroll, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 20);
  canvas->print(" - " + state->getCurrentTrack().artist);

  // cover art
  canvas->drawRGBBitmap(SCREEN_WIDTH / 2 - 43, SCREEN_HEIGHT / 3 - 38, sprite_bitmap, SPRITE_WIDTH, SPRITE_HEIGHT);
  canvas->drawRect(SCREEN_WIDTH / 2 - 43, SCREEN_HEIGHT / 3 - 38, SPRITE_WIDTH, SPRITE_HEIGHT, state->getCurrentTrack().color);
  canvas->drawRect(SCREEN_WIDTH / 2 - 43 - 1, SCREEN_HEIGHT / 3 - 38 - 1, SPRITE_WIDTH + 2, SPRITE_HEIGHT + 2, state->getCurrentTrack().color);
}
