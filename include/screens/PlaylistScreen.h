#pragma once

#include <Adafruit_GFX.h>

#include "ScreenManager.h"
#include "Screen.h"
#include "State.h"
#include "InputManager.h"
#include "FileManager.h"

class PlaylistScreen : public Screen {
  private:
    GFXcanvas16* canvas;
    ScreenManager* screenManager;
    State* state;
    InputManager* inputManager;
    FileManager* fileManager;

    int select_index;

  public:
    PlaylistScreen(ScreenManager* screenManager, GFXcanvas16* canvas, State* state, InputManager* inputManager, FileManager* fileManager);

    void init() override;
    void update() override;
    void draw() override;
};
