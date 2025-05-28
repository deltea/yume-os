#include "screens/player_screen.h"
#include "constants.h"
#include "monogram.h"
#include "cutepixel.h"
#include "cover.h"

unsigned long lastFrameTime = 0;
float dt = 0; // in seconds

int title_scroll = 0;
int title_scroll_speed = 1;
float scroll_timer = 0;
float scroll_delay = 5;

PlayerScreen::PlayerScreen(GFXcanvas16* c) {
  canvas = c;
}

void PlayerScreen::init() {
  canvas->setTextWrap(false);
  canvas->setTextSize(1);
}

void PlayerScreen::update() {
  unsigned long now = millis();
  dt = (now - lastFrameTime) / 1000.0f;
  lastFrameTime = now;

  if (scroll_timer < scroll_delay) {
    scroll_timer += dt;
    return; // don't scroll yet
  }

  // title_scroll = (title_scroll + title_scroll_speed) % (String("last surprise - persona 5 ost").length() * 6);
  title_scroll += title_scroll_speed;
  if (title_scroll > String("last surprise - persona 5 ost").length() * 6) {
    title_scroll = 0;
    scroll_timer = 0;
  }
}

void PlayerScreen::draw() {
  canvas->fillScreen(BG);

  // progress bar
  canvas->fillRect(0, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT, SCREEN_WIDTH / 3 * 2, PROGRESS_BAR_HEIGHT, ACCENT);

  // song duration and current time
  canvas->setTextColor(FG);
  canvas->setFont(&monogram8pt7b);
  canvas->setCursor(0, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 6);
  canvas->print("2:43");

  canvas->setCursor(SCREEN_WIDTH - 4 * 7, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 6);
  canvas->print("4:28");

  // song name and artist
  canvas->setTextColor(ACCENT);
  // canvas->setFont(&cute_pixel8pt7b);
  canvas->setCursor(0 - title_scroll, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 20);
  canvas->print("last surprise");
  canvas->setTextColor(FG);
  canvas->setCursor(13 * 6 - title_scroll, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - 20);
  canvas->print(" - persona 5 ost");

  // cover art
  canvas->drawRGBBitmap(SCREEN_WIDTH / 2 - 43, SCREEN_HEIGHT / 3 - 38, sprite_bitmap, SPRITE_WIDTH, SPRITE_HEIGHT);
  canvas->drawRect(SCREEN_WIDTH / 2 - 43, SCREEN_HEIGHT / 3 - 38, SPRITE_WIDTH, SPRITE_HEIGHT, ACCENT);
}
