// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#include "RemoteShutter.hpp"

void RemoteShutter::releaseShutter(uint64_t ms) {
  switch (shutter_mode) {
    case ShutterMode::Burst:
      BleKeyboard::press(KEY_MEDIA_VOLUME_DOWN);
      break;

    default:
      BleKeyboard::write(KEY_MEDIA_VOLUME_DOWN);
      break;
  }

  last_sent_ms = ms;
}

void RemoteShutter::stopShutter(uint64_t ms) {
  switch (shutter_mode) {
    case ShutterMode::Burst:
      BleKeyboard::release(KEY_MEDIA_VOLUME_DOWN);
      break;

    default:
      return;
  }

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

void RemoteShutter::setOnConnect(std::function<void()> handler) {
  on_connect = handler;
}

void RemoteShutter::setOnDisconnect(std::function<void()> handler) {
  on_disconnect = handler;
}

void RemoteShutter::update(void) {
  if (wasConnected()) {
    on_connect();
  } else if (wasDisconnected()) {
    on_disconnect();
  }
}

void RemoteShutter::setShutterMode(ShutterMode mode) { shutter_mode = mode; }
