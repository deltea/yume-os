#include "State.h"

String State::getCurrentTrackName() {
  if (queue.empty()) return "";
  return queue[currentIndex];
}

void State::setBatteryLevel(int level) {
  batteryLevel = level;
}
