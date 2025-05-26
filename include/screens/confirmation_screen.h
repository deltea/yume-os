#pragma once

#include <Adafruit_GFX.h>

#include "screen.h"

class ConfirmationScreen : public Screen {
  public:
    ConfirmationScreen(GFXcanvas16* c);

    void init() override;
    void update() override;
    void draw() override;

  private:
    GFXcanvas16* canvas;

    int select_index = 0;
    float margin = 4;
    int scroll = 0;
    int scroll_speed = 1.0;
    String danger_text = "DANGER!";
    uint16_t text_width = 0;
};
