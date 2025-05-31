#include "State.h"

Track State::getCurrentTrack() {
  if (queue.empty()) return Track();
  return queue[currentIndex];
}

void State::setBatteryLevel(int level) {
  batteryLevel = level;
}
