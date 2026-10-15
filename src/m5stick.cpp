// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#if defined(M5_STICK_C) || defined(M5_STICK_S3)
#include <M5Unified.h>

#include "../include/Display.hpp"
#include "../include/RemoteShutter.hpp"
#include "../include/Settings.hpp"

#if defined(M5_STICK_C)
constexpr gpio_num_t LED_PIN = GPIO_NUM_10;
#endif

m5util::Display display = m5util::Display(&M5.Display, DEVICE_NAME);

RemoteShutter rs;

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  Serial.begin(115200);
  Serial.println("started");

#if defined(M5_STICK_C)
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
#endif

  M5.Speaker.begin();
  M5.Speaker.setVolume(192);
  M5.Display.setBrightness(128);
  M5.Display.setRotation(3);

  display.begin();
  rs.setShutterMode(display.getMode());

  rs.setName(DEVICE_NAME);
  rs.setOnConnect([] {
    Serial.println("connected");

#if defined(M5_STICK_C)
    digitalWrite(LED_PIN, LOW);
#endif
    M5.Speaker.tone(880, 50);
    delay(150);
#if defined(M5_STICK_C)
    digitalWrite(LED_PIN, HIGH);
#endif
  });
  rs.setOnDisconnect([] { Serial.println("disconnected"); });
  rs.begin();
}

void loop() {
  if (rs.isConnected()) {
    if (M5.BtnA.wasReleased()) {
      Serial.println("Shutter button was released");
      rs.stopShutter(millis());
    }

    if (M5.BtnA.wasPressed()) {
      Serial.println("Shutter button was pressed");
      rs.releaseShutter(millis());

    } else if (!M5.BtnA.isPressed() && rs.lastSentDuration(millis()) > 5000) {
      // about 5 seconds after from last sent

      Serial.println("Keep display on");
      rs.keepDisplayOn(millis());

    } else if (M5.BtnB.wasPressed()) {
      auto shutter_mode = display.toggleMode();
      rs.setShutterMode(shutter_mode);
      Serial.printf("Shutter mode changed: %s\n",
                    display.getModeName().c_str());
    }

    display.connected();
  } else {
    display.disconnected();
  }

  if (M5.BtnPWR.wasClicked()) {
    Serial.println("Power off");
    M5.Power.powerOff();
  }

  float v = M5.Power.getBatteryVoltage() / 1000.0f;
  int32_t c = M5.Power.getBatteryCurrent();
  int32_t l = M5.Power.getBatteryLevel();
  display.battery(v, c, l);
  rs.setBatteryLevel(l);

  display.update();
  rs.update();
  M5.update();
  delay(100);
}

#endif  // M5_STICK_C || M5_STICK_S3
