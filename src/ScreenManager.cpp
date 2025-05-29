#include "ScreenManager.h"
#include <list>

void ScreenManager::setScreen(Screen* screen) {
  currentScreen = screen;
}

void ScreenManager::init() {
  if (currentScreen) {
    currentScreen->init();
  }
}

void ScreenManager::update() {
  if (currentScreen) {
    currentScreen->update();
  }
}

void ScreenManager::draw() {
  if (currentScreen) {
    currentScreen->draw();
  }
}
