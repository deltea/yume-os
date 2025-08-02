#include <Adafruit_GFX.h>
#include <ArduinoJson.h>
#include "screens/PlayerScreen.h"
#include "ScreenManager.h"
#include "State.h"
#include "constants.h"
#include "monogram.h"
#include "cutepixel.h"
#include "utils.h"

PlayerScreen::PlayerScreen(ScreenManager* screenManager, GFXcanvas16* canvas, State* state, InputManager* inputManager) : screenManager(screenManager), canvas(canvas), state(state), inputManager(inputManager) {
  this->last_frame_time = 0;
  this->dt = 0;
  this->title_scroll = 0;
  this->title_scroll_speed = 1;
  this->scroll_timer = 0;
  this->scroll_wait_timer = 0;
  this->scroll_delay = 5;
};

void PlayerScreen::readCoverImage() {
  File cover = SD.open(state->getCurrentTrack().cover_path);
  if (cover && cover.available()) {
    cover.read((uint8_t*)cover_buffer, sizeof(cover_buffer));
    cover.close();
  } else {
    Serial.println("couldn't open cover art file");
  }
}

void PlayerScreen::nextTrack() {
  if (state->currentIndex + 1 >= state->queue.size()) {
    state->currentIndex = 0;
  } else {
    state->currentIndex++;
  }

  readCoverImage();

  title_scroll = 0;
  scroll_timer = 0;
}

void PlayerScreen::init() {
  canvas->setTextWrap(false);
  canvas->setTextSize(1);

  readCoverImage();
}

void PlayerScreen::update() {
  unsigned long now = millis();
  dt = (now - last_frame_time) / 1000.0;
  last_frame_time = now;

  if (inputManager->isLeftButtonDown()) {
    nextTrack();
  }

  // don't scroll if the title and artist fit on the screen
  String title_artist = state->getCurrentTrack().title + " - " + state->getCurrentTrack().artist;

  if (getTextWidth(title_artist) < SCREEN_WIDTH) {
    title_scroll = -(SCREEN_WIDTH - getTextWidth(title_artist)) / 2;
    return;
  }

  if (scroll_timer < scroll_delay) {
    scroll_timer += dt;
    return;
  }

  // if (scroll_wait_timer < scroll_delay) {
  //   scroll_wait_timer += dt;
  //   return;
  // } else {
  //   title_scroll = 0;
  // }

  title_scroll += title_scroll_speed;
  if (title_scroll > getTextWidth(title_artist) - SCREEN_WIDTH) {
    title_scroll = 0;
    scroll_wait_timer = 0;
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
  canvas->print("2:50");

  // song name and artist
  // todo: make this less ugly
  canvas->setTextColor(FG);
  canvas->setCursor(0 - title_scroll, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 20);
  canvas->print(state->getCurrentTrack().title);
  canvas->setTextColor(state->getCurrentTrack().color);
  canvas->setCursor(state->getCurrentTrack().title.length() * 6 - title_scroll, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 20);
  canvas->print(" - " + state->getCurrentTrack().artist);

  // cover art
  canvas->drawRGBBitmap(SCREEN_WIDTH / 2 - 43, SCREEN_HEIGHT / 3 - 38, cover_buffer, 86, 86);
  canvas->drawRect(SCREEN_WIDTH / 2 - 43, SCREEN_HEIGHT / 3 - 38, 86, 86, state->getCurrentTrack().color);
  canvas->drawRect(SCREEN_WIDTH / 2 - 43 - 1, SCREEN_HEIGHT / 3 - 38 - 1, 86 + 2, 86 + 2, state->getCurrentTrack().color);

  // battery indicator
  canvas->drawRect(0, 0, 12, 6, FG);
  canvas->fillRect(12, 2, 1, 2, FG);
  canvas->fillRect(1, 1, 11 * (state->batteryLevel / 100.0), 4, FG);
}
