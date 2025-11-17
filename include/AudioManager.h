#pragma once

#include <Audio.h>
#include <Adafruit_TLV320DAC3100.h>
#include "Track.h"

extern Adafruit_TLV320DAC3100 dac;

class AudioManager {
  public:
    Audio* audio;
    float current_time;
    float track_duration;

    void initAudio();
    void playTrack(const Track& track);
  // return current play time in seconds (0 if no audio)
  float getAudioCurrentTime();
};
