#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include "cutepixel.h"
#include "monogram.h"
#include "constants.h"
#include "ScreenManager.h"
#include "FileManager.h"

#include "screens/ConfirmationScreen.h"
#include "screens/PlayerScreen.h"
#include "screens/MovieScreen.h"

Adafruit_SSD1351 display = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_CS, OLED_DC, OLED_RST);

GFXcanvas16 currentFrame(SCREEN_WIDTH, SCREEN_HEIGHT);
GFXcanvas16 lastFrame(SCREEN_WIDTH, SCREEN_HEIGHT);

ScreenManager screenManager;
FileManager fileManager;
State state;

ConfirmationScreen confirmationScreen(&screenManager, &currentFrame, &state);
PlayerScreen playerScreen(&screenManager, &currentFrame, &state);
MovieScreen movieScreen(&screenManager, &currentFrame, &state);

void setup() {
  Serial.begin(9600);
  SPI.begin();

  pinMode(BUTTON, INPUT_PULLUP);

  if (!SD.begin(SD_CS)) {
    Serial.println("sd card initialization failed!");
    return;
  }

  fileManager.indexSongs("/.bumpi", state.queue);

  display.begin();
  display.fillScreen(BG);
  // display.setSPISpeed(8000000);

  screenManager.setScreen(&playerScreen);
  screenManager.init();
}

void loop() {
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
