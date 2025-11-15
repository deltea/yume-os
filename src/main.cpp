#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <Adafruit_TLV320DAC3100.h>
#include <Adafruit_TLV320DAC3100_typedefs.h>
#include "cutepixel.h"
#include "monogram.h"
#include "utils.h"
#include "constants.h"
#include "ScreenManager.h"
#include "FileManager.h"
#include "InputManager.h"
#include "AudioManager.h"

#include "screens/ConfirmationScreen.h"
#include "screens/PlayerScreen.h"
#include "screens/LibraryScreen.h"
#include "screens/QueueScreen.h"

Adafruit_SSD1351 display = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_CS, OLED_DC, OLED_RST);
Adafruit_TLV320DAC3100 dac;

GFXcanvas16 currentFrame(SCREEN_WIDTH, SCREEN_HEIGHT);
GFXcanvas16 lastFrame(SCREEN_WIDTH, SCREEN_HEIGHT);

ScreenManager screenManager;
FileManager fileManager;
State state;
InputManager inputManager;
AudioManager audioManager;

ConfirmationScreen confirmationScreen(&screenManager, &currentFrame, &state, &inputManager, &fileManager);
PlayerScreen playerScreen(&screenManager, &currentFrame, &state, &inputManager, &fileManager, &audioManager);
LibraryScreen libraryScreen(&screenManager, &currentFrame, &state, &inputManager, &fileManager);
QueueScreen queueScreen(&screenManager, &currentFrame, &state, &inputManager, &fileManager);

volatile int last_state_a = HIGH;
volatile int rotary_value = 0;

void IRAM_ATTR readEncoder() {
  int current_state_a = digitalRead(ROTARY_A);

  if (current_state_a != last_state_a) {
    rotary_value += (digitalRead(ROTARY_B) != current_state_a) ? 1 : -1;
    Serial.println(rotary_value);
  }

  last_state_a = current_state_a;
}

void audioTask(void *parameter) {
  while (true) {
    if (audioManager.audio->isRunning()) {
      audioManager.audio->loop();
    }

    vTaskDelay(1);
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  SPI.setFrequency(40000000);

  // display initialization
  display.begin();
  display.fillScreen(BG);
  display.setSPISpeed(8000000);
  display.setTextColor(FG);

  delay(100);

  Wire.begin(22, 20);
  if (!dac.begin()) {
    Serial.println("dac initialization failed!");
    display.println("dac initialization failed!");
    return;
  }

  // dac configuration
  dac.setCodecInterface(TLV320DAC3100_FORMAT_I2S, TLV320DAC3100_DATA_LEN_32);
  dac.setCodecClockInput(TLV320DAC3100_CODEC_CLKIN_PLL);
  dac.setPLLClockInput(TLV320DAC3100_PLL_CLKIN_BCLK);
  dac.setPLLValues(1, 1, 32, 0);
  dac.powerPLL(true);

  dac.setNDAC(true, 2);
  dac.setMDAC(true, 2);

  dac.setDACDataPath(true, true, TLV320_DAC_PATH_NORMAL, TLV320_DAC_PATH_NORMAL, TLV320_VOLUME_STEP_1SAMPLE);
  dac.configureAnalogInputs(TLV320_DAC_ROUTE_MIXER, TLV320_DAC_ROUTE_MIXER, false, false, false, false);
  dac.configureHeadphoneDriver(true, true, TLV320_HP_COMMON_1_35V, false);

  dac.configureHPL_PGA(0, true);
  dac.configureHPR_PGA(0, true);
  dac.setHPLVolume(true, 12);
  dac.setHPRVolume(true, 12);
  dac.setChannelVolume(false, -6);
  dac.setChannelVolume(true, -6);
  dac.setDACVolumeControl(false, false, TLV320_VOL_INDEPENDENT);

  dac.configureHeadphonePop(true, TLV320_HP_TIME_304MS, TLV320_RAMP_4MS);
  dac.setInputCommonMode(true, true);

  delay(100);

  // input
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(ROTARY_A, INPUT_PULLUP);
  pinMode(ROTARY_B, INPUT_PULLUP);

  // wrapper for isr
  attachInterrupt(digitalPinToInterrupt(ROTARY_A), readEncoder, CHANGE);

  // boot screen
  Serial.println("booting...");
  display.setTextColor(FG);
  display.setFont(&cute_pixel8pt7b);
  display.setCursor(SCREEN_WIDTH / 2 - getTextWidth("YUME_OS") / 2, SCREEN_HEIGHT / 2 - 8);
  display.println("YUME_OS");
  display.setFont(&monogram8pt7b);
  display.setCursor(SCREEN_WIDTH / 2 - getTextWidth("booting up...") / 2, SCREEN_HEIGHT / 2 + 8);
  display.println("booting up...");

  // card reader initializion
  if (!SD.begin(SD_CS)) {
    Serial.println("sd card initialization failed!");
    display.println("sd card initialization failed!");
    return;
  }

  audioManager.initAudio();
  xTaskCreatePinnedToCore(audioTask, "audioplay", 12288, NULL, 5, NULL, 1);

  fileManager.indexSongs("/", state.queue);

  display.fillScreen(BG);

  // shuffle queue
  if (state.queue.size() > 1) {
    for (size_t i = 0; i < state.queue.size(); i++) {
      size_t j = random(i, state.queue.size());
      std::swap(state.queue[i], state.queue[j]);
    }
  }

  screenManager.setScreen(&playerScreen);
  screenManager.init();
}

void loop() {
  screenManager.update();
  screenManager.draw();

  dac.setChannelVolume(false, -6 + rotary_value);
  dac.setChannelVolume(true, -6 + rotary_value);

  // compare and update only changed pixels
  uint16_t* curr = currentFrame.getBuffer();
  uint16_t* prev = lastFrame.getBuffer();

  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      int index = y * SCREEN_WIDTH + x;
      if (curr[index] != prev[index]) {
        display.drawPixel(x, y, curr[index]);
        prev[index] = curr[index];
      }
    }
  }

  // battery calculations
  int rawValue = analogRead(A13);
  // float voltageLevel = (rawValue / 4095.0) * 2 * 1.1 * 3.3;
  float voltageLevel = rawValue * 2 / 1000;
  float batteryFraction = voltageLevel / MAX_BATTERY_VOLTAGE;
  // Serial.print("Battery voltage: ");
  // Serial.println(voltageLevel);

  state.setBatteryLevel((int)(batteryFraction * 100.0));

  // delay(1000 / 60);
  vTaskDelay(4);
}
