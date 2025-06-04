#pragma once

#include <Adafruit_GFX.h>

#include "ScreenManager.h"
#include "Screen.h"
#include "State.h"
#include "InputManager.h"

class LibraryScreen : public Screen {
  private:
    GFXcanvas16* canvas;
    ScreenManager* screenManager;
    State* state;
    InputManager* inputManager;

  public:
    LibraryScreen(ScreenManager* screenManager, GFXcanvas16* canvas, State* state, InputManager* inputManager);

    void init() override;
    void update() override;
    void draw() override;
};
