#include <M5Atom.h>

#define USE_NIMBLE
#include "./src/ESP32-BLE-Keyboard/BleKeyboard.h"

// SETTINGS
#define DEVICE_NAME "M5Keyboard"

BleKeyboard bleKeyboard;

const CRGB disconnectedLED = CRGB(128, 0, 0);
const CRGB connectedLED = CRGB(0, 128, 0);

void setup()
{
  M5.begin(true, false, true);
  M5.dis.clear();

  Serial.begin(115200);
  Serial.println("started");
  bleKeyboard.setName(DEVICE_NAME);
  bleKeyboard.begin();

  M5.dis.drawpix(0, disconnectedLED);
}

bool connected = false;

void loop()
{
  if (bleKeyboard.isConnected())
  {
    if (!connected)
    {
      connected = true;
      Serial.println("connected");
      M5.dis.drawpix(0, connectedLED);
    }

    if (M5.Btn.wasPressed())
    {
      Serial.println("Sending key...");
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    }
  }
  else if (connected)
  {
    connected = false;
    Serial.println("disconnected");
    M5.dis.drawpix(0, disconnectedLED);
  }

  delay(100);
  M5.update();
}
