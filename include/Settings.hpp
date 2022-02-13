// Copyright (c) 2022 Takenori Nakagawa <ww24gm+oss@gmail.com>

#ifndef INCLUDE_SETTINGS_HPP_
#define INCLUDE_SETTINGS_HPP_

#include <algorithm>

// SETTINGS
#define DEVICE_NAME "RemoteShutter"

#define BATTERY_PERCENTAGE(voltage) \
  max(min((voltage < 3.2) ? 0 : (voltage - 3.2) * 100, 0.0), 100.0)

#endif  // INCLUDE_SETTINGS_HPP_
