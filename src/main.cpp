#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include "cutepixel.h"
#include "monogram.h"
#include "constants.h"
#include "ScreenManager.h"
#include "FileManager.h"
#include "InputManager.h"

#include "screens/ConfirmationScreen.h"
#include "screens/PlayerScreen.h"
#include "screens/LibraryScreen.h"

Adafruit_SSD1351 display = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_CS, OLED_DC, OLED_RST);

GFXcanvas16 currentFrame(SCREEN_WIDTH, SCREEN_HEIGHT);
GFXcanvas16 lastFrame(SCREEN_WIDTH, SCREEN_HEIGHT);

ScreenManager screenManager;
FileManager fileManager;
State state;
InputManager inputManager;

ConfirmationScreen confirmationScreen(&screenManager, &currentFrame, &state, &inputManager);
PlayerScreen playerScreen(&screenManager, &currentFrame, &state, &inputManager);
LibraryScreen libraryScreen(&screenManager, &currentFrame, &state, &inputManager);

void setup() {
  Serial.begin(9600);
  SPI.begin();

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

  Serial.println("booting...");

  display.begin();
  display.fillScreen(BG);
  display.setSPISpeed(8000000);
  display.setTextColor(FG);
  display.println("booting...");

  if (!SD.begin(SD_CS)) {
    Serial.println("sd card initialization failed!");
    return;
  }

  fileManager.indexSongs("/.bumpi", state.queue);

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
  // inputManager.updateInput();

  screenManager.update();
  screenManager.draw();

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
}
