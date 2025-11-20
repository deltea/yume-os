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
#include "icons.h"

#include "screens/ConfirmationScreen.h"
#include "screens/PlayerScreen.h"
#include "screens/LibraryScreen.h"
#include "screens/QueueScreen.h"
#include "screens/PlaylistScreen.h"

Adafruit_SSD1351 display = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_CS, OLED_DC, OLED_RST);
Adafruit_TLV320DAC3100 dac;

GFXcanvas16 currentFrame(SCREEN_WIDTH, SCREEN_HEIGHT);
GFXcanvas16 lastFrame(SCREEN_WIDTH, SCREEN_HEIGHT);

ScreenManager screenManager;
FileManager fileManager;
State state;
InputManager inputManager;
AudioManager audioManager;

AppContext ctx = {
  .state = &state,
  .audio = &audioManager,
  .files = &fileManager,
  .input = &inputManager,
  .screens = &screenManager,
  .canvas = &currentFrame
};

ConfirmationScreen confirmationScreen(&ctx);
PlayerScreen playerScreen(&ctx);
LibraryScreen libraryScreen(&ctx);
QueueScreen queueScreen(&ctx);
PlaylistScreen playlistScreen(&ctx);

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

  Wire.begin(22, 20);
  if (!dac.begin()) {
    Serial.println("dac initialization failed!");
    display.println("dac initialization failed!");
    return;
  }

  audioManager.setupDac();

  // input
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(ROTARY_ROT, INPUT_PULLUP);
  pinMode(ROTARY_BUTTON, INPUT_PULLUP);

  // wrapper for isr
  attachInterrupt(digitalPinToInterrupt(ROTARY_ROT), []() {
    inputManager.readEncoder();
  }, CHANGE);

  // boot screen
  Serial.println("booting...");
  display.setTextColor(FG);
  display.setFont(&cute_pixel8pt7b);
  display.setCursor(SCREEN_WIDTH / 2 - getTextWidth("YUME_OS") / 2, SCREEN_HEIGHT / 2 - 8);
  display.println("YUME_OS");
  display.setFont(&monogram8pt7b);
  display.setCursor(SCREEN_WIDTH / 2 - getTextWidth("booting up...") / 2, SCREEN_HEIGHT - 10);
  display.println("booting up...");
  display.drawRGBBitmap(SCREEN_WIDTH / 2 - getTextWidth("YUME_OS") / 2 - 18, SCREEN_HEIGHT / 2 - 15, ICON_MUSIC, 16, 9);
  display.setCursor(0, SCREEN_HEIGHT / 2 + 5);

  // card reader initializion
  if (!SD.begin(SD_CS)) {
    Serial.println("sd card initialization failed!");
    display.println("sd card initialization failed!");
    return;
  }

  audioManager.initAudio();
  xTaskCreatePinnedToCore(audioTask, "audioplay", 16384, NULL, 5, NULL, 1);

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

  dac.setChannelVolume(false, -6 + inputManager.getRotaryValue());
  dac.setChannelVolume(true, -6 + inputManager.getRotaryValue());

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

  vTaskDelay(4);
}
