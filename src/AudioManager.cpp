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
    // mute dac to prevent pop sound
    dac.setDACVolumeControl(true, true, TLV320_VOL_INDEPENDENT);
    delay(10);

    audio->stopSong();
    delay(20);
  }

  if (audio->connecttoFS(SD, track.audio_path.c_str())) {
    Serial.println("audio connected");
    delay(30);

    // unmute dac
    dac.setDACVolumeControl(false, false, TLV320_VOL_INDEPENDENT);
  } else {
    Serial.println("audio connect failed");
  }
}

float AudioManager::getAudioCurrentTime() {
  if (!audio) return 0.0f;
  // guard in case audio library isn't running
  return audio->getAudioCurrentTime();
}
