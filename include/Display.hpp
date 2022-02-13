// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#ifndef INCLUDE_DISPLAY_HPP_
#define INCLUDE_DISPLAY_HPP_

#include <M5StickCPlus.h>

#include <string>

#define HEADER_BAR(offset) (offset * 2 + 15)

namespace m5util {
class Display {
 private:
  TFT_eSprite sprite;
  std::string deviceName;
  uint8_t offset;
  uint8_t width;
  uint8_t height;

 public:
  Display(M5Display *display, std::string deviceName, uint8_t offset = 10,
          uint8_t width = TFT_WIDTH, uint8_t height = TFT_HEIGHT);
  void disconnected(void);
  void connected(void);
  void battery(float volt, float percentage);
};
}  // namespace m5util

#endif  // INCLUDE_DISPLAY_HPP_
