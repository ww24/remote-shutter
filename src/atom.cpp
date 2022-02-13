// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#if defined(M5_ATOM)
#include <M5Atom.h>

#include "RemoteShutter.hpp"
#include "Settings.hpp"

RemoteShutter rs;

const CRGB disconnectedLED = CRGB(128, 0, 0);
const CRGB connectedLED = CRGB(0, 128, 0);

void setup() {
  M5.begin(true, false, true);

  Serial.begin(115200);
  Serial.println("started");

  M5.dis.clear();
  M5.dis.drawpix(0, disconnectedLED);

  rs.setName(DEVICE_NAME);
  rs.setOnConnect([] {
    Serial.println("connected");
    M5.dis.drawpix(0, connectedLED);
  });
  rs.setOnDisconnect([] {
    Serial.println("disconnected");
    M5.dis.drawpix(0, disconnectedLED);
  });
  rs.begin();
}

void loop() {
  if (rs.isConnected()) {
    if (M5.Btn.wasPressed()) {
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
