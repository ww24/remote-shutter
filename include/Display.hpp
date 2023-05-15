// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#ifndef INCLUDE_DISPLAY_HPP_
#define INCLUDE_DISPLAY_HPP_

#include <M5StickCPlus.h>
#include <Preferences.h>

#include <string>

#include "./RemoteShutter.hpp"

#define HEADER_BAR(offset) (offset * 2 + 15)
#define FOOTER_BAR 30

namespace m5util {

class Display {
 private:
  TFT_eSprite sprite;
  std::string device_name;
  uint8_t offset;
  uint8_t width;
  uint8_t height;
  ShutterMode mode;
  Preferences prefs;
  const std::string pref_name = "remote-shutter";
  const std::string pref_mode_key = "mode";

 public:
  Display(M5Display *display, std::string device_name, uint8_t offset = 10,
          uint8_t width = TFT_WIDTH, uint8_t height = TFT_HEIGHT);
  ~Display();
  void begin(void);
  void disconnected(void);
  void connected(void);
  void battery(float volt, float percentage);
  void update(void);
  ShutterMode toggleMode(void);
  ShutterMode getMode(void);
  std::string getModeName(void);
};
}  // namespace m5util

#endif  // INCLUDE_DISPLAY_HPP_
