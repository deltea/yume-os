#pragma once

#include <Adafruit_GFX.h>

#include "screen.h"

class GradientScreen : public Screen {
  public:
    GradientScreen(GFXcanvas16* c);

    void init() override;
    void update() override;
    void draw() override;

  private:
    GFXcanvas16* canvas;
};
