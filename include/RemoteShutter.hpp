#ifndef REMOTE_SHUTTER_H
#define REMOTE_SHUTTER_H

#include "../lib/ESP32-BLE-Keyboard/BleKeyboard.h"

class RemoteShutter : public BleKeyboard
{
    using BleKeyboard::BleKeyboard;

private:
    unsigned long lastSentMs;
    bool connState; // true is connected

public:
    void releaseShutter(unsigned long ms);
    void keepDisplayOn(unsigned long ms);
    unsigned long lastSentDuration(unsigned long ms);
    bool wasConnected(void);
    bool wasDisconnected(void);
};

#endif // REMOTE_SHUTTER_H
