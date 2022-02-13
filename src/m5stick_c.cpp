// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#if defined(M5_STICK_C)
#include <M5StickCPlus.h>

#include "Display.hpp"
#include "RemoteShutter.hpp"
#include "Settings.hpp"

m5util::Display display = m5util::Display(&M5.Lcd, DEVICE_NAME, 10);

RemoteShutter rs;

void setup() {
  M5.begin();

  Serial.begin(115200);
  Serial.println("started");

  pinMode(GPIO_NUM_10, OUTPUT);
  digitalWrite(GPIO_NUM_10, HIGH);

  M5.Beep.setVolume(5);
  M5.Axp.ScreenBreath(9);
  M5.Lcd.setRotation(3);

  display.disconnected();

  rs.setName(DEVICE_NAME);
  rs.setOnConnect([] {
    Serial.println("connected");

    digitalWrite(GPIO_NUM_10, LOW);
    delay(100);
    M5.Beep.tone(880);
    delay(50);
    M5.Beep.mute();
    digitalWrite(GPIO_NUM_10, HIGH);

    display.connected();
  });
  rs.setOnDisconnect([] {
    Serial.println("disconnected");
    digitalWrite(GPIO_NUM_10, HIGH);

    display.disconnected();
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

  if (M5.Axp.GetBtnPress() == 0x02) {
    Serial.println("Power off");
    M5.Axp.PowerOff();
  }

  float v = M5.Axp.GetBatVoltage();
  float vp = BATTERY_PERCENTAGE(v);
  display.battery(v, vp);
  rs.setBatteryLevel(vp);

  rs.update();
  M5.update();
  delay(100);
}

#endif  // M5_STICK_C
