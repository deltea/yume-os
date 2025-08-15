#pragma once

#include <Audio.h>
#include "Track.h"

class AudioManager {
  public:
    Audio* audio;
    float current_time;
    float track_duration;

    void initAudio();
    void playTrack(const Track& track);
};
