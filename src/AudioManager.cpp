#include "AudioManager.h"
#include "constants.h"

AudioManager::AudioManager() {
}

void AudioManager::initAudio() {
  audio = new Audio();

  audio->forceMono(false);
  audio->setI2SCommFMT_LSB(false);
  audio->setConnectionTimeout(500, 2700);
  audio->setPinout(DAC_BCLK, DAC_LRC, DAC_DATA);
  audio->setVolume(2);

  // xTaskCreatePinnedToCore(audioTask, "audioplay", 12288, NULL, 5, NULL, 1);
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

// void AudioManager::audioTask(void *parameter) {
//   while (true) {
//     if (audio->isRunning()) {
//       audio->loop();
//     }

//     vTaskDelay(1);
//   }
// }
