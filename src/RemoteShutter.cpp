// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#include "RemoteShutter.hpp"

void RemoteShutter::releaseShutter(uint64_t ms) {
  BleKeyboard::write(KEY_MEDIA_VOLUME_DOWN);
  last_sent_ms = ms;
}

void RemoteShutter::keepDisplayOn(uint64_t ms) {
  // enter a harmless key on Android/iOS because keep display on from going to
  // sleep
  BleKeyboard::write(KEY_MEDIA_LOCAL_MACHINE_BROWSER);
  last_sent_ms = ms;
}

uint64_t RemoteShutter::lastSentDuration(uint64_t ms) {
  return ms - last_sent_ms;
}

bool RemoteShutter::wasConnected(void) {
  if (isConnected() && !conn_state) {
    conn_state = true;
    return true;
  }
  return false;
}

bool RemoteShutter::wasDisconnected(void) {
  if (!isConnected() && conn_state) {
    conn_state = false;
    return true;
  }
  return false;
}
