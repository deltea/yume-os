#pragma once

#include <Audio.h>
#include "Track.h"

class AudioManager {
  public:
    AudioManager();

    Audio* audio;
    void initAudio();
    void playTrack(const Track& track);
    // void audioTask(void *parameter);
    // void togglePause();

};
