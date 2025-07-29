#include <SPI.h>
// #include <I2S.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <Adafruit_TLV320DAC3100.h>
#include <Adafruit_TLV320DAC3100_typedefs.h>
#include <Audio.h>
#include "cutepixel.h"
#include "monogram.h"
#include "constants.h"
#include "ScreenManager.h"
#include "FileManager.h"
#include "InputManager.h"

#include "screens/ConfirmationScreen.h"
#include "screens/PlayerScreen.h"
#include "screens/LibraryScreen.h"
#include "screens/QueueScreen.h"

Adafruit_SSD1351 display = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_CS, OLED_DC, OLED_RST);
Adafruit_TLV320DAC3100 dac;
Audio audio;

GFXcanvas16 currentFrame(SCREEN_WIDTH, SCREEN_HEIGHT);
GFXcanvas16 lastFrame(SCREEN_WIDTH, SCREEN_HEIGHT);

ScreenManager screenManager;
FileManager fileManager;
State state;
InputManager inputManager;

ConfirmationScreen confirmationScreen(&screenManager, &currentFrame, &state, &inputManager);
PlayerScreen playerScreen(&screenManager, &currentFrame, &state, &inputManager);
LibraryScreen libraryScreen(&screenManager, &currentFrame, &state, &inputManager);
QueueScreen queueScreen(&screenManager, &currentFrame, &state, &inputManager);

volatile int last_state_a = HIGH;
volatile int rotary_value = 0;

void IRAM_ATTR readEncoder() {
  int current_state_a = digitalRead(ROTARY_A);

  if (current_state_a != last_state_a) {
    if (digitalRead(ROTARY_B) != current_state_a) {
      rotary_value++;
    } else {
      rotary_value--;
    }

    // Serial.print("value - ");
    // Serial.println(rotary_value);
  }

  last_state_a = current_state_a;
}

void audioTask(void *parameter) {
  while (true) {
    audio.loop();
    vTaskDelay(1);
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();

  Wire.begin(22, 20);
  if (!dac.begin()) {
    Serial.println("dac initialization failed!");
    display.println("dac initialization failed!");
    return;
  }

  dac.setCodecInterface(TLV320DAC3100_FORMAT_I2S, TLV320DAC3100_DATA_LEN_16);
  dac.setCodecClockInput(TLV320DAC3100_CODEC_CLKIN_PLL);
  dac.setPLLValues(1, 1, 32, 0);
  dac.powerPLL(true);

  // Set dividers
  dac.setNDAC(true, 4);
  dac.setMDAC(true, 4);

  dac.setDACDataPath(true, true,                    // Power up both DACs
    TLV320_DAC_PATH_NORMAL,        // Normal left path
    TLV320_DAC_PATH_NORMAL,        // Normal right path
    TLV320_VOLUME_STEP_1SAMPLE);

  // Configure analog routing
  dac.configureAnalogInputs(TLV320_DAC_ROUTE_MIXER, // Left DAC to mixer
            TLV320_DAC_ROUTE_MIXER,   // Right DAC to mixer
            false, false, false,      // No AIN routing
            false);                   // No HPL->HPR
  dac.configureHeadphoneDriver(true, true, TLV320_HP_COMMON_1_35V, false);
  // dac.enableBeep(true);

  // Configure headphone PGAs
  dac.configureHPL_PGA(0, true); // Set HPL gain, unmute
  dac.configureHPR_PGA(0, true); // Set HPR gain, unmute

  // Set headphone volumes
  dac.setHPLVolume(true, 6);     // Enable and set HPL volume
  dac.setHPRVolume(true, 6);     // Enable and set HPR volume

  dac.setDACVolumeControl(false, false, TLV320_VOL_INDEPENDENT); // Unmute both channels
  dac.setChannelVolume(false, -4);  // Left DAC 0dB
  dac.setChannelVolume(true, -4);   // Right DAC 0dB

  // input
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(ROTARY_A, INPUT_PULLUP);
  pinMode(ROTARY_B, INPUT_PULLUP);

  // wrapper for isr
  attachInterrupt(digitalPinToInterrupt(ROTARY_A), readEncoder, CHANGE);

  // display initialization
  display.begin();
  display.fillScreen(BG);
  display.setSPISpeed(8000000);
  display.setTextColor(FG);

  // boot screen
  Serial.println("booting...");
  display.println("booting...");

  // card reader initializion
  if (!SD.begin(SD_CS)) {
    Serial.println("sd card initialization failed!");
    display.println("sd card initialization failed!");
    return;
  }

  // audio initialization
  // audio.setBufsize(1024, 1024);
  audio.setPinout(DAC_BCLK, DAC_LRC, DAC_DATA);
  audio.setVolume(5);
  if (!audio.connecttoFS(SD, "/Specialist.mp3")) {
    Serial.println("audio connect failed");
  }

  xTaskCreatePinnedToCore(
    audioTask,
    "audioplay",
    12288,                  // Increased stack size
    NULL,
    5,                     // Higher priority
    NULL,
    1                      // Core 1
  );

  fileManager.indexSongs("/.yume", state.queue);

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

  Serial.print("value: ");
  Serial.println(rotary_value);

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
  float voltageLevel = (rawValue / 4095.0) * 2 * 1.1 * 3.3;
  float batteryFraction = voltageLevel / MAX_BATTERY_VOLTAGE;
  state.setBatteryLevel((int)(batteryFraction * 100.0));

  delay(1000 / 60);
  vTaskDelay(2);
}
