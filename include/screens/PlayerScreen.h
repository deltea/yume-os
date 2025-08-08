#pragma once

#include <Adafruit_GFX.h>

#include "ScreenManager.h"
#include "Screen.h"
#include "State.h"
#include "InputManager.h"
#include "FileManager.h"

class PlayerScreen : public Screen {
  private:
    GFXcanvas16* canvas;
    ScreenManager* screenManager;
    State* state;
    InputManager* inputManager;
    FileManager* fileManager;

    const uint16_t PROGRESS_BAR_HEIGHT = 2;

    unsigned long last_frame_time;
    float dt;
    int title_scroll;
    int title_scroll_speed;
    float scroll_timer;
    float scroll_wait_timer;
    float scroll_delay;
    Track current_track;
    uint16_t cover_buffer[86 * 86];

    void readCoverImage();
    void nextTrack();

  public:
    PlayerScreen(ScreenManager* screenManager, GFXcanvas16* canvas, State* state, InputManager* inputManager, FileManager* fileManager);

    void init() override;
    void update() override;
    void draw() override;
};
