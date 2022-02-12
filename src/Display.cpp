#include <M5StickCPlus.h>
#include "Display.hpp"

m5util::Display::Display(M5Display *display, std::string deviceName, uint8_t offset, uint8_t width, uint8_t height)
    : sprite(TFT_eSprite(display)),
      deviceName(deviceName.substr(0, 15)),
      offset(offset),
      width(width),
      height(height)
{
    sprite.createSprite(TFT_HEIGHT, TFT_WIDTH);
    sprite.setSwapBytes(true);
}

void m5util::Display::disconnected(void)
{
    uint8_t hb = HEADER_BAR(offset);
    sprite.fillRect(0, 0, height, width, TFT_BLACK);
    sprite.fillRect(0, 0, height, hb, sprite.color565(180, 0, 0));
    sprite.setTextSize(2);
    sprite.setCursor(offset, offset);
    sprite.setTextColor(sprite.color565(180, 180, 180));
    sprite.print("BLE disconnect");
    sprite.setCursor(offset, hb + 10);
    sprite.setTextColor(sprite.color565(18, 150, 219));
    sprite.setTextSize(2);
    sprite.print(deviceName.c_str());
    sprite.pushSprite(0, 0);
}

void m5util::Display::connected(void)
{
    uint8_t hb = HEADER_BAR(offset);
    sprite.fillRect(0, 0, height, width, TFT_BLACK);
    sprite.fillRect(0, 0, height, hb, sprite.color565(0, 0, 180));
    sprite.setTextColor(sprite.color565(180, 180, 180));
    sprite.setTextSize(2);
    sprite.setCursor(offset, offset);
    sprite.print("BLE connect!");
    sprite.setTextSize(3);
    sprite.setCursor(offset, round(width / 2.0) - 8);
    sprite.print("<<- PRESS");
    sprite.pushSprite(0, 0);
}

void m5util::Display::battery(float volt, float percentage)
{
    sprite.fillRect(0, width - 30, height, 30, TFT_BLACK);
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
    sprite.drawFastHLine(height - offset - (1 + btBarW) * 4 - 3, width - offset - btBarH - 4, (1 + btBarW) * 4 + 3, TFT_WHITE);
    sprite.drawFastHLine(height - offset - (1 + btBarW) * 4 - 3, width - offset - 1, (1 + btBarW) * 4 + 3, TFT_WHITE);
    sprite.drawFastVLine(height - offset - (1 + btBarW) * 4 - 3, width - offset - btBarH - 4, btBarH + 4, TFT_WHITE);
    sprite.drawFastVLine(height - offset - 1, width - offset - btBarH - 4, btBarH + 4, TFT_WHITE);
    sprite.fillRect(height - offset - (1 + btBarW) * 4 - 3 - 3, width - offset - btBarH, 3, 6, TFT_WHITE);
    for (int i = ceil(percentage / 25.0); i > 0; i--)
        sprite.fillRect(height - offset - (1 + btBarW) * i - 1, width - offset - btBarH - 2, btBarW, btBarH, btColor);
    sprite.pushSprite(0, 0);
}
