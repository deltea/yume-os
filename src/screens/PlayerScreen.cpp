#include <Adafruit_GFX.h>
#include <ArduinoJson.h>
#include "screens/PlayerScreen.h"
#include "ScreenManager.h"
#include "State.h"
#include "constants.h"
#include "monogram.h"
#include "cutepixel.h"
#include "utils.h"

PlayerScreen::PlayerScreen(
  ScreenManager* screenManager, GFXcanvas16* canvas, State* state, InputManager* inputManager, FileManager* fileManager, AudioManager* audioManager
) : screenManager(screenManager), canvas(canvas), state(state), inputManager(inputManager), fileManager(fileManager), audioManager(audioManager) {
  this->last_frame_time = 0;
  this->dt = 0;
  this->title_scroll = 0;
  this->title_scroll_speed = 1;
  this->scroll_timer = 0;
  this->scroll_state = 0;
  this->start_scroll_delay = 5;
  this->end_scroll_delay = 2;
  this->current_track = Track();
};

void PlayerScreen::readCoverImage() {
  File cover = SD.open(this->current_track.cover_path);
  if (cover && cover.available()) {
    cover.read((uint8_t*)cover_buffer, sizeof(cover_buffer));
    cover.close();
  } else {
    Serial.println("couldn't open cover art file");
  }
}

void PlayerScreen::updateTrack() {
  title_scroll = 0;
  scroll_timer = 0;
  scroll_state = 0;

  this->current_track = fileManager->getTrack(state->getCurrentTrackName());
  Serial.println("current track: " + this->current_track.title);

  readCoverImage();
  Serial.println("read cover image");

  audioManager->playTrack(current_track);
  Serial.println("played track");
}

void PlayerScreen::nextTrack() {
  if (state->currentIndex + 1 >= state->queue.size()) {
    state->currentIndex = 0;
  } else {
    state->currentIndex++;
  }

  updateTrack();
}

void PlayerScreen::init() {
  canvas->setTextWrap(false);
  canvas->setTextSize(1);

  updateTrack();
}

void PlayerScreen::update() {
  unsigned long now = millis();
  dt = (now - last_frame_time) / 1000.0;
  last_frame_time = now;

  if (inputManager->isLeftButtonDown()) {
    nextTrack();
  }

  // don't scroll if the title and artist fit on the screen
  String title_artist = this->current_track.title + " - " + this->current_track.artist;

  if (getTextWidth(title_artist) < SCREEN_WIDTH) {
    title_scroll = -(SCREEN_WIDTH - getTextWidth(title_artist)) / 2;
    return;
  }

  if (scroll_state == 0) {
    // waiting before scroll
    scroll_timer += dt;
    if (scroll_timer >= start_scroll_delay) {
      scroll_timer = 0;
      scroll_state = 1;
    }
  } else if (scroll_state == 1) {
    // scrolling
    title_scroll += title_scroll_speed;
    if (title_scroll >= getTextWidth(title_artist) - SCREEN_WIDTH) {
      scroll_state = 2;
    }
  } else {
    // wait after scroll
    scroll_timer += dt;
    if (scroll_timer >= end_scroll_delay) {
      scroll_timer = 0;
      title_scroll = 0;
      scroll_state = 0;
    }
  }
}

void PlayerScreen::draw() {
  canvas->fillScreen(BG);

  // progress bar
  canvas->fillRect(
    0,
    SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT,
    // (this->audioManager->current_time / this->audioManager->track_duration) * SCREEN_WIDTH,
    this->current_track.duration > 0 ? ((float)this->audioManager->audio->getAudioCurrentTime() / this->current_track.duration) * SCREEN_WIDTH : 0,
    PROGRESS_BAR_HEIGHT,
    this->current_track.color
  );

  // song duration and current time
  canvas->setTextColor(FG);
  canvas->setFont(&monogram8pt7b);
  canvas->setCursor(0, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 6);
  canvas->print(getTimestamp(this->audioManager->audio->getAudioCurrentTime()));

  canvas->setCursor(SCREEN_WIDTH - 5 * 7, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 6);
  canvas->print(getTimestamp(this->current_track.duration));

  // song name and artist
  // todo: make this less ugly
  canvas->setTextColor(FG);
  canvas->setCursor(-title_scroll, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 20);
  canvas->print(this->current_track.title);

  canvas->setTextColor(this->current_track.color);
  canvas->setCursor(this->current_track.title.length() * 6 - title_scroll, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 20);
  canvas->print(" - " + this->current_track.artist);

  // cover art (and outline)
  canvas->drawRGBBitmap(SCREEN_WIDTH / 2 - 43, SCREEN_HEIGHT / 3 - 38, cover_buffer, 86, 86);
  canvas->drawRect(SCREEN_WIDTH / 2 - 43, SCREEN_HEIGHT / 3 - 38, 86, 86, this->current_track.color);
  canvas->drawRect(SCREEN_WIDTH / 2 - 43 - 1, SCREEN_HEIGHT / 3 - 38 - 1, 86 + 2, 86 + 2, this->current_track.color);

  // battery indicator
  canvas->drawRect(0, 0, 12, 6, FG);
  canvas->fillRect(12, 2, 1, 2, FG);
  canvas->fillRect(1, 1, 11 * (state->batteryLevel / 100.0), 4, FG);
}
