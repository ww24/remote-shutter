#if defined(M5_ATOM)
#include <M5Atom.h>

#include "Settings.hpp"
#include "RemoteShutter.hpp"

RemoteShutter rs;

const CRGB disconnectedLED = CRGB(128, 0, 0);
const CRGB connectedLED = CRGB(0, 128, 0);

void setup()
{
    M5.begin(true, false, true);

    Serial.begin(115200);
    Serial.println("started");

    rs.setName(DEVICE_NAME);
    rs.begin();

    M5.dis.clear();
    M5.dis.drawpix(0, disconnectedLED);
}

void loop()
{
    if (rs.isConnected())
    {
        if (rs.wasConnected())
        {
            Serial.println("connected");
            M5.dis.drawpix(0, connectedLED);
        }

        if (M5.Btn.wasPressed())
        {
            Serial.println("Release the shutter");
            rs.releaseShutter(millis());
        }
        else if (rs.lastSentDuration(millis()) > 5000) // about 5 seconds after from last sent
        {
            Serial.println("Keep display on");
            rs.keepDisplayOn(millis());
        }
    }
    else if (rs.wasDisconnected())
    {
        Serial.println("disconnected");
        M5.dis.drawpix(0, disconnectedLED);
    }

    delay(100);
    M5.update();
}

#endif // M5_ATOM
