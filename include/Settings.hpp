// SETTINGS
#define DEVICE_NAME "RemoteShutter"

#define BATTERY_PERCENTAGE(voltage) max(min((voltage < 3.2) ? 0 : (voltage - 3.2) * 100, 0.0), 100.0)
