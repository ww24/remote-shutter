// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#ifndef INCLUDE_DISPLAY_HPP_
#define INCLUDE_DISPLAY_HPP_

#include <M5Unified.h>
#include <Preferences.h>

#include <string>

#include "./RemoteShutter.hpp"

#define HEADER_BAR(offset) (offset * 2 + 15)
#define FOOTER_BAR 30

namespace m5util {

class Display {
 private:
  M5Canvas sprite;
  std::string device_name;
  uint16_t offset;
  uint16_t width;
  uint16_t height;
  ShutterMode mode;
  bool auto_power_off = true;
  Preferences prefs;
  const std::string pref_name = "remote-shutter";
  const std::string pref_mode_key = "mode";
  const std::string pref_auto_poff_key = "auto_poff";
  void drawAutoPowerOffIcon(void);

 public:
  Display(M5GFX* display, std::string device_name, uint16_t offset = 10,
          uint16_t width = 135, uint16_t height = 240);
  ~Display();
  void begin(void);
  void disconnected(void);
  void connected(void);
  void battery(float voltage, int32_t current, int32_t percentage);
  void update(void);
  ShutterMode toggleMode(void);
  ShutterMode getMode(void);
  std::string getModeName(void);
  bool toggleAutoPowerOff(void);
  bool getAutoPowerOff(void);
};
}  // namespace m5util

#endif  // INCLUDE_DISPLAY_HPP_
