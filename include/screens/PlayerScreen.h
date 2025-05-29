#pragma once

#include <Adafruit_GFX.h>

#include "ScreenManager.h"
#include "Screen.h"
#include "AppState.h"

class PlayerScreen : public Screen {
  private:
    GFXcanvas16* canvas;
    ScreenManager* screenManager;
    AppState* state;

    const uint16_t PROGRESS_BAR_HEIGHT = 2;

    unsigned long last_frame_time;
    float dt;
    int title_scroll;
    int title_scroll_speed;
    float scroll_timer;
    float scroll_delay;

  public:
    PlayerScreen(ScreenManager* screenManager, GFXcanvas16* canvas, AppState* appState);

    void init() override;
    void update() override;
    void draw() override;
};
