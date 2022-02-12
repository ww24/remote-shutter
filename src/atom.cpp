#if defined(M5_ATOM)
#include <M5Atom.h>

#include "Settings.hpp"
#include "../lib/ESP32-BLE-Keyboard/BleKeyboard.h"

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
unsigned long last_sent_ms;

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
            last_sent_ms = millis();
        }
        else if (millis() - last_sent_ms > 5000) // about 5 seconds after from last sent
        {
            Serial.println("Keep display on");
            // enter a harmless key on Android/iOS because keep display on from going to sleep
            bleKeyboard.write(KEY_MEDIA_LOCAL_MACHINE_BROWSER);
            last_sent_ms = millis();
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

#endif // M5_ATOM
