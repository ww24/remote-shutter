// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#if defined(M5_ATOM)
#include <FastLED.h>
#include <M5Unified.h>

#include "../include/RemoteShutter.hpp"
#include "../include/Settings.hpp"

RemoteShutter rs;

constexpr uint8_t LED_PIN = 27;
CRGB leds[1];

const CRGB disconnectedLED = CRGB(128, 0, 0);
const CRGB connectedLED = CRGB(0, 128, 0);

void setLED(CRGB color) {
  leds[0] = color;
  FastLED.show();
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  Serial.begin(115200);
  Serial.println("started");

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, 1);
  setLED(disconnectedLED);

  rs.setName(DEVICE_NAME);
  rs.setOnConnect([] {
    Serial.println("connected");
    setLED(connectedLED);
  });
  rs.setOnDisconnect([] {
    Serial.println("disconnected");
    setLED(disconnectedLED);
  });
  rs.begin();
}

void loop() {
  if (rs.isConnected()) {
    if (M5.BtnA.wasPressed()) {
      Serial.println("Release the shutter");
      rs.releaseShutter(millis());
    } else if (rs.lastSentDuration(millis()) > 5000) {
      // about 5 seconds after from last sent

      Serial.println("Keep display on");
      rs.keepDisplayOn(millis());
    }
  }

  rs.update();
  M5.update();
  delay(100);
}

#endif  // M5_ATOM
