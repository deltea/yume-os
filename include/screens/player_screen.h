#pragma once

#include <Adafruit_GFX.h>

#include "screen.h"

class PlayerScreen : public Screen {
  public:
    PlayerScreen(GFXcanvas16* c);

    void init() override;
    void update() override;
    void draw() override;

  private:
    GFXcanvas16* canvas;

    const uint16_t PROGRESS_BAR_HEIGHT = 2;
};
