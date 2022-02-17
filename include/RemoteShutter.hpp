// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#ifndef INCLUDE_REMOTESHUTTER_HPP_
#define INCLUDE_REMOTESHUTTER_HPP_

#include <functional>

#include "../lib/ESP32-BLE-Keyboard/BleKeyboard.h"

enum class ShutterMode { Default, Burst, MAX };

class RemoteShutter : public BleKeyboard {
  using BleKeyboard::BleKeyboard;

 private:
  uint64_t last_sent_ms;
  bool conn_state;  // it is true if connected
  std::function<void()> on_connect = [] {};
  std::function<void()> on_disconnect = [] {};
  ShutterMode shutter_mode;

 public:
  void releaseShutter(uint64_t ms);
  void stopShutter(uint64_t ms);
  void keepDisplayOn(uint64_t ms);
  uint64_t lastSentDuration(uint64_t ms);
  bool wasConnected(void);
  bool wasDisconnected(void);
  void setOnConnect(std::function<void()>);
  void setOnDisconnect(std::function<void()>);
  // should call in loop if use on_connect or on_disconnect handler
  void update(void);
  void setShutterMode(ShutterMode mode);
};

#endif  // INCLUDE_REMOTESHUTTER_HPP_
