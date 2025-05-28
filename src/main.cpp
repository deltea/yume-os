#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>

#include "cutepixel.h"
#include "monogram.h"
#include "constants.h"

#include "screen_manager.h"
#include "screens/battery_screen.h"
#include "screens/confirmation_screen.h"
#include "screens/gradient_screen.h"
#include "screens/player_screen.h"

Adafruit_SSD1351 display = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_CS, OLED_DC, OLED_RST);

GFXcanvas16 currentFrame(SCREEN_WIDTH, SCREEN_HEIGHT);
GFXcanvas16 lastFrame(SCREEN_WIDTH, SCREEN_HEIGHT);

ScreenManager screenManager;
BatteryScreen batteryScreen(&currentFrame);
ConfirmationScreen confirmationScreen(&currentFrame);
GradientScreen gradientScreen(&currentFrame);
PlayerScreen playerScreen(&currentFrame);

void setup() {
  display.begin();
  display.setSPISpeed(8000000);
  display.fillScreen(BG);

  Serial.begin(9600);

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

  delay(16);
}
