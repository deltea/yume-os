#include <Adafruit_GFX.h>
#include <ArduinoJson.h>
#include "screens/PlayerScreen.h"
#include "ScreenManager.h"
#include "State.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "FileManager.h"
#include "constants.h"
#include "monogram.h"
#include "cutepixel.h"
#include "utils.h"

PlayerScreen::~PlayerScreen() {
  if (this->cover_buffer) {
    free(this->cover_buffer);
    this->cover_buffer = nullptr;
  }
}

void PlayerScreen::readCoverImage() {
  File cover = SD.open(this->current_track.cover_path);
  if (!cover) {
    Serial.println("couldn't open cover art file");
    return;
  }

  if (cover.available() && this->cover_buffer) {
    cover.read((uint8_t*)this->cover_buffer, 86 * 86 * sizeof(uint16_t));
  }
  cover.close();
}

void PlayerScreen::updateTrack() {
  this->title_scroll = 0;
  this->scroll_timer = 0;
  this->scroll_state = 0;

  this->current_track = ctx->files->getTrack(ctx->state->getCurrentTrackName());
  Serial.println("current track: " + this->current_track.title);

  readCoverImage();
  Serial.println("read cover image");

  ctx->audio->playTrack(current_track);
  Serial.println("played track");
}

void PlayerScreen::nextTrack() {
  if (ctx->state->currentIndex + 1 >= ctx->state->queue.size()) {
    ctx->state->currentIndex = 0;
  } else {
    ctx->state->currentIndex++;
  }

  updateTrack();
}

void PlayerScreen::init() {
  this->last_frame_time = 0;
  this->dt = 0;
  this->title_scroll = 0;
  this->title_scroll_speed = 1;
  this->scroll_timer = 0;
  this->scroll_state = 0;
  this->start_scroll_delay = 5;
  this->end_scroll_delay = 2;
  this->current_track = Track();

  this->cover_buffer = (uint16_t*)malloc(86 * 86 * sizeof(uint16_t));
  if (!this->cover_buffer) {
    Serial.println("ERROR: Failed to allocate cover_buffer!");
  }

  ctx->canvas->setTextWrap(false);
  ctx->canvas->setTextSize(1);

  updateTrack();
}

void PlayerScreen::update() {
  unsigned long now = millis();
  dt = (now - last_frame_time) / 1000.0;
  last_frame_time = now;

  if (ctx->input->isLeftButtonDown()) {
    nextTrack();
  }

  // don't scroll if the title and artist fit on the screen
  String title_artist = this->current_track.title + " - " + this->current_track.artist;

  if (getTextWidth(title_artist) < SCREEN_WIDTH) {
    this->title_scroll = -(SCREEN_WIDTH - getTextWidth(title_artist)) / 2;
    return;
  }

  if (this->scroll_state == 0) {
    // waiting before scroll
    this->scroll_timer += dt;
    if (this->scroll_timer >= this->start_scroll_delay) {
      this->scroll_timer = 0;
      this->scroll_state = 1;
    }
  } else if (this->scroll_state == 1) {
    // scrolling
    this->title_scroll += this->title_scroll_speed;
    if (this->title_scroll >= getTextWidth(title_artist) - SCREEN_WIDTH) {
      this->scroll_state = 2;
    }
  } else {
    // wait after scroll
    this->scroll_timer += dt;
    if (this->scroll_timer >= this->end_scroll_delay) {
      this->scroll_timer = 0;
      this->title_scroll = 0;
      this->scroll_state = 0;
    }
  }
}

void PlayerScreen::draw() {
  ctx->canvas->fillScreen(BG);

  // progress bar
  ctx->canvas->fillRect(
    0,
    SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT,
    // (this->audioManager->current_time / this->audioManager->track_duration) * SCREEN_WIDTH,
    this->current_track.duration > 0 ? ((float)ctx->audio->getAudioCurrentTime() / this->current_track.duration) * SCREEN_WIDTH : 0,
    PROGRESS_BAR_HEIGHT,
    this->current_track.color
  );

  // song duration and current time
  ctx->canvas->setTextColor(FG);
  ctx->canvas->setFont(&monogram8pt7b);
  ctx->canvas->setCursor(0, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 6);

  // TODO: elapsed time is wrong (too much)
  ctx->canvas->print(getTimestamp(ctx->audio->getAudioCurrentTime()));

  ctx->canvas->setCursor(SCREEN_WIDTH - 5 * 7, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 6);
  ctx->canvas->print(getTimestamp(this->current_track.duration));

  // song name and artist
  // todo: make this less ugly
  ctx->canvas->setTextColor(FG);
  ctx->canvas->setCursor(-this->title_scroll, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 20);
  ctx->canvas->print(this->current_track.title);

  ctx->canvas->setTextColor(this->current_track.color);
  ctx->canvas->setCursor(this->current_track.title.length() * 6 - this->title_scroll, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 20);
  ctx->canvas->print(" - " + this->current_track.artist);

  // cover art (and outline)
  if (cover_buffer) {
    ctx->canvas->drawRGBBitmap(SCREEN_WIDTH / 2 - 43, SCREEN_HEIGHT / 3 - 38, cover_buffer, 86, 86);
  }
  ctx->canvas->drawRect(SCREEN_WIDTH / 2 - 43, SCREEN_HEIGHT / 3 - 38, 86, 86, this->current_track.color);
  ctx->canvas->drawRect(SCREEN_WIDTH / 2 - 43 - 1, SCREEN_HEIGHT / 3 - 38 - 1, 86 + 2, 86 + 2, this->current_track.color);
}
