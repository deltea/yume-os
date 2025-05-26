#pragma once

#include <Adafruit_GFX.h>

#include "screen.h"

class BatteryScreen : public Screen {
  public:
    BatteryScreen(GFXcanvas16* c);

    void init() override;
    void update() override;
    void draw() override;

  private:
    GFXcanvas16* canvas;

    float angle = 0.0;
    int frame = 0;
};
