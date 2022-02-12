#if defined(M5_STICK_C)
#include <M5StickCPlus.h>

#include "Settings.hpp"
#include "Display.hpp"
#include "../lib/ESP32-BLE-Keyboard/BleKeyboard.h"

m5util::Display display = m5util::Display(&M5.Lcd, DEVICE_NAME, 10);

BleKeyboard bleKeyboard;

void setup()
{
    M5.begin();
    M5.Beep.setVolume(5);

    Serial.begin(115200);
    Serial.println("started");
    bleKeyboard.setName(DEVICE_NAME);
    bleKeyboard.begin();

    pinMode(GPIO_NUM_10, OUTPUT);
    digitalWrite(GPIO_NUM_10, HIGH);

    M5.Axp.ScreenBreath(9);
    M5.Lcd.setRotation(3);

    display.disconnected();
}

bool isConnected = false;
unsigned long last_sent_ms;

void loop()
{
    if (bleKeyboard.isConnected())
    {
        if (!isConnected)
        {
            isConnected = true;
            Serial.println("connected");

            digitalWrite(GPIO_NUM_10, LOW);
            delay(100);
            M5.Beep.tone(880, 50);
            digitalWrite(GPIO_NUM_10, HIGH);

            display.connected();
        }

        if (M5.BtnA.wasPressed())
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
    else if (isConnected)
    {
        isConnected = false;
        Serial.println("disconnected");
        digitalWrite(GPIO_NUM_10, HIGH);

        display.disconnected();
    }

    if (M5.Axp.GetBtnPress() == 0x02)
    {
        Serial.println("Power off");
        M5.Axp.PowerOff();
    }

    float v = M5.Axp.GetBatVoltage();
    float vp = BATTERY_PERCENTAGE(v);
    display.battery(v, vp);
    bleKeyboard.setBatteryLevel(vp);

    delay(100);
    M5.update();
}

#endif // M5_STICK_C
