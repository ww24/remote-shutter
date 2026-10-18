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

// interval of reading battery status via PMIC (I2C)
constexpr uint32_t BATTERY_UPDATE_INTERVAL_MS = 5000;
// retry interval while the PMIC returns invalid values (e.g. just after boot)
constexpr uint32_t BATTERY_RETRY_INTERVAL_MS = 500;
// max time to wait for a valid battery reading in setup()
constexpr uint32_t BATTERY_INIT_TIMEOUT_MS = 1000;
// readings below this voltage are treated as invalid (PMIC not ready)
constexpr int16_t BATTERY_MIN_VALID_MV = 2500;

// redraw the screen only when something has changed
bool screen_dirty = true;
bool last_connected = false;

uint32_t last_battery_read_ms = 0;
uint32_t battery_read_interval_ms = 0;  // 0: read on the next call
int16_t last_battery_mv = -1;
int32_t last_battery_ma = 0;
int32_t last_battery_level = -1;

// read battery status periodically and update the footer if changed.
// returns true if a valid reading was obtained.
bool updateBattery(uint32_t now, bool force = false) {
  if (!force && now - last_battery_read_ms < battery_read_interval_ms) {
    return false;
  }
  last_battery_read_ms = now;

  int16_t mv = M5.Power.getBatteryVoltage();
#if defined(M5_STICK_C)
  int32_t ma = M5.Power.getBatteryCurrent();
#else
  int32_t ma = 0;
#endif
  int32_t level = M5.Power.getBatteryLevel();

  if (mv < BATTERY_MIN_VALID_MV || level < 0) {
    // PMIC is not ready yet; keep the previous values and retry soon
    battery_read_interval_ms = BATTERY_RETRY_INTERVAL_MS;
    return false;
  }
  battery_read_interval_ms = BATTERY_UPDATE_INTERVAL_MS;

  bool changed = false;
  if (mv != last_battery_mv || ma != last_battery_ma) {
    last_battery_mv = mv;
    last_battery_ma = ma;
    changed = true;
  }

  if (level != last_battery_level) {
    last_battery_level = level;
    rs.setBatteryLevel(level);
    changed = true;
  }

  if (changed) {
    display.battery(mv / 1000.0f, ma, level);
    screen_dirty = true;
  }
  return true;
}

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

  // draw the initial screen immediately (battery footer follows once read)
  display.disconnected();
  display.update();

  // wait for a valid battery reading before starting BLE so that the central
  // is notified of the actual battery level (not 0%) from the beginning
  uint32_t battery_wait_start = millis();
  while (!updateBattery(millis(), true) &&
         millis() - battery_wait_start < BATTERY_INIT_TIMEOUT_MS) {
    delay(50);
  }

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
  bool connected = rs.isConnected();
  if (connected != last_connected) {
    last_connected = connected;
    screen_dirty = true;
  }

  if (connected) {
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
      screen_dirty = true;
    }
  }

  if (M5.BtnPWR.wasClicked()) {
    Serial.println("Power off");
    M5.Power.powerOff();
  }

  updateBattery(millis());

  if (screen_dirty) {
    if (connected) {
      display.connected();
    } else {
      display.disconnected();
    }
    display.update();
    screen_dirty = false;
  }

  rs.update();
  M5.update();
  delay(100);
}

#endif  // M5_STICK_C || M5_STICK_S3
