#pragma once

#include <Adafruit_GFX.h>

#include "ScreenManager.h"
#include "Screen.h"
#include "State.h"

class PlayerScreen : public Screen {
  private:
    GFXcanvas16* canvas;
    ScreenManager* screenManager;
    State* state;

    const uint16_t PROGRESS_BAR_HEIGHT = 2;

    unsigned long last_frame_time;
    float dt;
    int title_scroll;
    int title_scroll_speed;
    float scroll_timer;
    float scroll_delay;
    uint16_t cover_buffer[86 * 86];

    void readCoverImage() {
      state->getCurrentTrack().cover_file.read((uint8_t*)cover_buffer, sizeof(cover_buffer));
      state->getCurrentTrack().cover_file.close();
    }

  public:
    PlayerScreen(ScreenManager* screenManager, GFXcanvas16* canvas, State* state);

    void init() override;
    void update() override;
    void draw() override;
};
