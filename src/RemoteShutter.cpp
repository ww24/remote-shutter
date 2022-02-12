#include "RemoteShutter.hpp"

void RemoteShutter::releaseShutter(unsigned long ms)
{
    BleKeyboard::write(KEY_MEDIA_VOLUME_DOWN);
    lastSentMs = ms;
}

void RemoteShutter::keepDisplayOn(unsigned long ms)
{
    // enter a harmless key on Android/iOS because keep display on from going to sleep
    BleKeyboard::write(KEY_MEDIA_LOCAL_MACHINE_BROWSER);
    lastSentMs = ms;
}

unsigned long RemoteShutter::lastSentDuration(unsigned long ms)
{
    return ms - lastSentMs;
}

bool RemoteShutter::wasConnected(void)
{
    if (isConnected() && !connState)
    {
        connState = true;
        return true;
    }
    return false;
}

bool RemoteShutter::wasDisconnected(void)
{
    if (!isConnected() && connState)
    {
        connState = false;
        return true;
    }
    return false;
}
