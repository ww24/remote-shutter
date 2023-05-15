// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#include "../include/Display.hpp"

using m5util::Display;

Display::Display(M5Display *display, std::string device_name, uint8_t offset,
                 uint8_t width, uint8_t height)
    : sprite(TFT_eSprite(display)),
      device_name(device_name.substr(0, 15)),
      offset(offset),
      width(width),
      height(height) {
  sprite.createSprite(TFT_HEIGHT, TFT_WIDTH);
  sprite.setSwapBytes(true);
}

Display::~Display() { prefs.end(); }

void Display::begin(void) {
  bool initialized = prefs.begin(pref_name.c_str(), false);
  if (!initialized && prefs.clear()) {
    Serial.println("NVS cleared");
  }

  mode = static_cast<ShutterMode>(prefs.getUChar(pref_mode_key.c_str(), 0));
}

void Display::disconnected(void) {
  uint8_t hb = HEADER_BAR(offset);
  sprite.fillRect(0, 0, height, width - FOOTER_BAR, TFT_BLACK);
  sprite.fillRect(0, 0, height, hb, sprite.color565(180, 0, 0));
  sprite.setTextSize(2);
  sprite.setCursor(offset, offset);
  sprite.setTextColor(sprite.color565(180, 180, 180));
  sprite.print("BLE disconnect");
  sprite.setCursor(offset, hb + 10);
  sprite.setTextColor(sprite.color565(18, 150, 219));
  sprite.setTextSize(2);
  sprite.print(device_name.c_str());
}

void Display::connected(void) {
  uint8_t hb = HEADER_BAR(offset);
  sprite.fillRect(0, 0, height, width - FOOTER_BAR, TFT_BLACK);
  sprite.fillRect(0, 0, height, hb, sprite.color565(0, 0, 180));
  sprite.setTextColor(sprite.color565(180, 180, 180));
  sprite.setTextSize(2);
  sprite.setCursor(offset, offset);
  sprite.print("BLE connect!");
  sprite.setTextSize(3);
  sprite.setCursor(offset, round(width / 2.0) - 10);
  sprite.print("<<- PRESS");
  sprite.setTextSize(2);
  sprite.setCursor(offset, round(width / 2.0) + 16);
  sprite.printf("(%s mode)", getModeName().c_str());
}

void Display::battery(float volt, float percentage) {
  sprite.fillRect(0, width - FOOTER_BAR, height, FOOTER_BAR, TFT_BLACK);
  sprite.setTextSize(1);
  sprite.setTextColor(sprite.color565(180, 180, 180));
  float batVolt = M5.Axp.GetBatVoltage();
  sprite.setCursor(offset, width - offset - 8);
  sprite.printf("Bat: V/%.3fv I/%.3fma", batVolt, M5.Axp.GetBatCurrent());

  uint32_t btColor;
  if (percentage > 50)
    btColor = TFT_GREEN;
  else if (percentage > 25)
    btColor = TFT_YELLOW;
  else
    btColor = TFT_RED;
  int btBarW = 5;
  int btBarH = 10;
  sprite.drawFastHLine(height - offset - (1 + btBarW) * 4 - 3,
                       width - offset - btBarH - 4, (1 + btBarW) * 4 + 3,
                       TFT_WHITE);
  sprite.drawFastHLine(height - offset - (1 + btBarW) * 4 - 3,
                       width - offset - 1, (1 + btBarW) * 4 + 3, TFT_WHITE);
  sprite.drawFastVLine(height - offset - (1 + btBarW) * 4 - 3,
                       width - offset - btBarH - 4, btBarH + 4, TFT_WHITE);
  sprite.drawFastVLine(height - offset - 1, width - offset - btBarH - 4,
                       btBarH + 4, TFT_WHITE);
  sprite.fillRect(height - offset - (1 + btBarW) * 4 - 3 - 3,
                  width - offset - btBarH, 3, 6, TFT_WHITE);
  for (int i = ceil(percentage / 25.0); i > 0; i--)
    sprite.fillRect(height - offset - (1 + btBarW) * i - 1,
                    width - offset - btBarH - 2, btBarW, btBarH, btColor);
}

void Display::update(void) { sprite.pushSprite(0, 0); }

ShutterMode Display::toggleMode(void) {
  auto next = static_cast<uint8_t>(mode) + 1;
  if (next >= static_cast<uint8_t>(ShutterMode::MAX)) next = 0;
  mode = static_cast<ShutterMode>(next);

  prefs.putUChar("mode", next);

  return mode;
}

ShutterMode Display::getMode(void) { return mode; }

std::string Display::getModeName(void) {
  switch (mode) {
    case ShutterMode::Default:
      return "Oneshot";

    case ShutterMode::Burst:
      return "Burst";

    case ShutterMode::PhotoPro:
      return "PhotoPro";

    default:
      return "unexpected!";
  }
}
