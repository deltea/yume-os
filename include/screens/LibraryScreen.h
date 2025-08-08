#pragma once

#include <Adafruit_GFX.h>

#include "ScreenManager.h"
#include "Screen.h"
#include "State.h"
#include "InputManager.h"
#include "FileManager.h"

class LibraryScreen : public Screen {
  private:
    GFXcanvas16* canvas;
    ScreenManager* screenManager;
    State* state;
    InputManager* inputManager;
    FileManager* fileManager;

    const vector<String> OPTIONS = {"playlists", "artists", "albums", "songs"};

    int select_index;

  public:
    LibraryScreen(ScreenManager* screenManager, GFXcanvas16* canvas, State* state, InputManager* inputManager, FileManager* fileManager);

    void init() override;
    void update() override;
    void draw() override;
};
