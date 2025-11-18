#pragma once

#include <Adafruit_GFX.h>

#include "ScreenManager.h"
#include "Screen.h"
#include "State.h"
#include "InputManager.h"
#include "FileManager.h"

class PlaylistScreen : public Screen {
  private:
    int select_index;

  public:
    PlaylistScreen(AppContext* ctx) : Screen(ctx) {};

    void init() override;
    void update() override;
    void draw() override;
};
