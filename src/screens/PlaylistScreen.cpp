#include <SD.h>
#include "screens/PlaylistScreen.h"
#include "constants.h"
#include "cutepixel.h"
#include "monogram.h"
#include "utils.h"
#include "icons.h"

void PlaylistScreen::init() {
  select_index = 0;

  ctx->canvas->setTextColor(FG);
  ctx->canvas->setTextWrap(false);
  ctx->canvas->setTextSize(1);
  ctx->canvas->setFont(&monogram8pt7b);
}

void PlaylistScreen::update() {
  if (ctx->input->isLeftButtonDown()) {
    select_index = (select_index + 1) % ctx->state->queue.size();
  }
}

void PlaylistScreen::draw() {
  ctx->canvas->fillScreen(BG);

  ctx->canvas->drawRGBBitmap(1, 3, ICON_PLAYLIST, 9, 8);
  ctx->canvas->setCursor(14, 9);
  ctx->canvas->print("PLAYLISTS");
  ctx->canvas->fillRect(0, 16, SCREEN_WIDTH, 1, FG);
}
