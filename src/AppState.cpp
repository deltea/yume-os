#include "AppState.h"

Track AppState::getCurrentTrack() {
  if (queue.empty()) return Track{"", "", 0};
  return queue[currentIndex];
}

void AppState::setBatteryLevel(int level) {
  batteryLevel = level;
}
