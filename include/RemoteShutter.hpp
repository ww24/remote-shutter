// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#ifndef INCLUDE_REMOTESHUTTER_HPP_
#define INCLUDE_REMOTESHUTTER_HPP_

#include "../lib/ESP32-BLE-Keyboard/BleKeyboard.h"

class RemoteShutter : public BleKeyboard {
  using BleKeyboard::BleKeyboard;

 private:
  uint64_t last_sent_ms;
  bool conn_state;  // it is true if connected

 public:
  void releaseShutter(uint64_t ms);
  void keepDisplayOn(uint64_t ms);
  uint64_t lastSentDuration(uint64_t ms);
  bool wasConnected(void);
  bool wasDisconnected(void);
};

#endif  // INCLUDE_REMOTESHUTTER_HPP_
