#pragma once

#include <Adafruit_GFX.h>

#include "Screen.h"
#include "Track.h"

class PlayerScreen : public Screen {
  private:
    const uint16_t PROGRESS_BAR_HEIGHT = 2;

    unsigned long last_frame_time;
    float dt;
    int title_scroll;
    int title_scroll_speed;
    float scroll_timer;
    int scroll_state;
    float start_scroll_delay;
    float end_scroll_delay;
    uint16_t* cover_buffer;
    Track current_track;

    void readCoverImage();
    void nextTrack();
    void updateTrack();

  public:
    PlayerScreen(Context* ctx) : Screen(ctx) {};
    ~PlayerScreen();

    void init() override;
    void update() override;
    void draw() override;
};
