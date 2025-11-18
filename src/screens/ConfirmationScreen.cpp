#include "screens/ConfirmationScreen.h"
#include "constants.h"
#include "cutepixel.h"
#include "monogram.h"

void ConfirmationScreen::init() {
  ctx->canvas->setTextColor(FG);
  ctx->canvas->setTextWrap(false);
  ctx->canvas->setTextSize(1);

  // ctx->canvas->getTextBounds(danger_text, 0, 0, nullptr, nullptr, &text_width, nullptr);
  text_width = danger_text.length() * 7;
}

void ConfirmationScreen::update() {
  scroll += scroll_speed;
  if (scroll >= text_width) scroll = 0;
}

void ConfirmationScreen::draw() {
  ctx->canvas->fillScreen(BG);

  // danger bars
  ctx->canvas->fillRect(0, margin, SCREEN_WIDTH, 12, FG);
  ctx->canvas->fillRect(0, SCREEN_HEIGHT - 12 - margin, SCREEN_WIDTH, 12, FG);

  ctx->canvas->setFont(&cute_pixel8pt7b);
  ctx->canvas->setTextColor(BG);

  int x = -scroll;
  while (x < SCREEN_WIDTH + text_width) {
    ctx->canvas->setTextColor(BG);
    ctx->canvas->setCursor(x, 13);
    ctx->canvas->print(danger_text);
    ctx->canvas->setCursor(x - text_width / 2, SCREEN_HEIGHT - 7);
    ctx->canvas->print(danger_text);

    x += text_width;
  }

  // confirmation prompt
  ctx->canvas->setTextColor(FG);
  ctx->canvas->setCursor(SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 - 16);
  ctx->canvas->print("are you sure?");

  ctx->canvas->setFont(&monogram8pt7b);
  ctx->canvas->setCursor(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 10);
  ctx->canvas->print("yup");

  ctx->canvas->setCursor(SCREEN_WIDTH / 2 - 14, SCREEN_HEIGHT / 2 + 25);
  ctx->canvas->print("nope");
}
