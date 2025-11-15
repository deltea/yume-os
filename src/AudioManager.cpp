#include "AudioManager.h"
#include "constants.h"

void AudioManager::initAudio() {
  audio = new Audio();

  audio->forceMono(false);
  audio->setI2SCommFMT_LSB(false);
  audio->setConnectionTimeout(500, 2700);
  audio->setPinout(DAC_BCLK, DAC_LRC, DAC_DATA);
  audio->setVolume(2);
}

void AudioManager::playTrack(const Track& track) {
  if (audio->isRunning()) {
    audio->stopSong();
  }

  if (audio->connecttoFS(SD, track.audio_path.c_str())) {
    Serial.println("audio connected");
  } else {
    Serial.println("audio connect failed");
  }
}

float AudioManager::getAudioCurrentTime() {
  if (!audio) return 0.0f;
  // guard in case audio library isn't running
  return audio->getAudioCurrentTime();
}
