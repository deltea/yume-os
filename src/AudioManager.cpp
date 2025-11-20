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

void AudioManager::setupDac() {
  dac.setCodecInterface(TLV320DAC3100_FORMAT_I2S, TLV320DAC3100_DATA_LEN_32);
  dac.setCodecClockInput(TLV320DAC3100_CODEC_CLKIN_PLL);
  dac.setPLLClockInput(TLV320DAC3100_PLL_CLKIN_BCLK);
  dac.setPLLValues(1, 1, 64, 0);
  dac.powerPLL(true);

  dac.setNDAC(true, 1);
  dac.setMDAC(true, 1);

  dac.setDACDataPath(true, true, TLV320_DAC_PATH_NORMAL, TLV320_DAC_PATH_NORMAL, TLV320_VOLUME_STEP_1SAMPLE);
  dac.configureAnalogInputs(TLV320_DAC_ROUTE_MIXER, TLV320_DAC_ROUTE_MIXER, false, false, false, false);
  dac.configureHeadphoneDriver(true, true, TLV320_HP_COMMON_1_35V, false);

  dac.configureHPL_PGA(0, true);
  dac.configureHPR_PGA(0, true);
  dac.setHPLVolume(true, 15);
  dac.setHPRVolume(true, 15);
  dac.setChannelVolume(false, 0);
  dac.setChannelVolume(true, 0);
  dac.setDACVolumeControl(false, false, TLV320_VOL_INDEPENDENT);

  dac.configureHeadphonePop(true, TLV320_HP_TIME_304MS, TLV320_RAMP_4MS);
  dac.setInputCommonMode(true, true);
}

float AudioManager::getAudioCurrentTime() {
  if (!audio) return 0.0f;
  // guard in case audio library isn't running
  return audio->getAudioCurrentTime();
}
