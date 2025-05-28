#include "screens/battery_screen.h"
#include "constants.h"
#include "monogram.h"

BatteryScreen::BatteryScreen(GFXcanvas16* c) {
  canvas = c;
}

void BatteryScreen::init() {
  canvas->setTextColor(FG);
  canvas->setTextWrap(false);

  pinMode(A0, INPUT_PULLUP);
}

void BatteryScreen::update() {
  if (digitalRead(A0) != LOW) {
    angle += 4;
    if (angle >= 360) angle = 0;
  }

  frame++;
}

void BatteryScreen::draw() {
  canvas->fillScreen(BG);

  int rawValue = analogRead(A13);
  float voltageLevel = (rawValue / 4095.0) * 2 * 1.1 * 3.3;
  float batteryFraction = voltageLevel / MAX_BATTERY_VOLTAGE;
  String batteryText = "battery - " + (String)((batteryFraction * 100.0)) + "%";

  canvas->setCursor(SCREEN_WIDTH / 2 - batteryText.length() * 3, 12);
  canvas->setFont(&monogram8pt7b);
  canvas->print(batteryText);

  int cx = SCREEN_WIDTH / 2;
  int cy = SCREEN_HEIGHT / 2;
  int size = 30;

  float rad = angle * DEG_TO_RAD;

  // draw 4 sides of a rotating square
  for (int i = 0; i < 4; i++) {
    float theta1 = rad + i * PI / 2;
    float theta2 = rad + (i + 1) * PI / 2;

    int x1 = cx + size * cos(theta1);
    int y1 = cy + size * sin(theta1);
    int x2 = cx + size * cos(theta2);
    int y2 = cy + size * sin(theta2);

    canvas->drawLine(x1, y1, x2, y2, FG);
  }

  canvas->drawCircle(cx, cy, size, FG);
  canvas->drawCircle(cx, cy, 10 + std::sin(frame * 0.2) * 5, FG);
}
