#include <SD.h>
#include "screens/QueueScreen.h"
#include "constants.h"
#include "cutepixel.h"
#include "monogram.h"

void QueueScreen::init() {
  select_index = 0;

  ctx->canvas->setTextColor(FG);
  ctx->canvas->setTextWrap(false);
  ctx->canvas->setTextSize(1);
}

void QueueScreen::update() {
  if (ctx->input->isLeftButtonDown()) {
    select_index = (select_index + 1) % ctx->state->queue.size();
  }
}

void QueueScreen::draw() {
  ctx->canvas->fillScreen(BG);

  ctx->canvas->setTextColor(FG);
  ctx->canvas->setFont(&monogram8pt7b);
  ctx->canvas->setCursor(SCREEN_WIDTH / 2 - 8, 10);
  ctx->canvas->print("QUEUE");

  for (size_t i = 0; i < ctx->state->queue.size(); i++) {
    if (select_index == i) {
      ctx->canvas->fillRect(0, 18 + 18 * i, SCREEN_WIDTH, 18, ACCENT);
    }

    // const Track& track = ctx->state->queue[i];
    // ctx->canvas->setCursor(8, 30 + 18 * i);
    // ctx->canvas->setTextColor(select_index == i ? BG : FG);
    // ctx->canvas->print(track.title);
    // ctx->canvas->print(" - ");
    // ctx->canvas->print(track.artist);
  }
}
