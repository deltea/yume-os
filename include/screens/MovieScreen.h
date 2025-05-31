#pragma once

#include <Adafruit_GFX.h>

#include "ScreenManager.h"
#include "Screen.h"
#include "State.h"

class MovieScreen : public Screen {
  private:
    GFXcanvas16* canvas;
    ScreenManager* screenManager;
    State* state;

  public:
    MovieScreen(ScreenManager* screenManager, GFXcanvas16* canvas, State* state);

    void init() override;
    void update() override;
    void draw() override;
};
