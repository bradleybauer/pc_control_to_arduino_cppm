/*
 * EEPROM configuration storage utility
 * Copyright 2016 by Thomas Buck <xythobuz@xythobuz.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

#define CPPM_OUTPUT_PIN_DEFAULT 8
#define CHANNEL_MINIMUM_VALUE (1000-8)
#define CHANNEL_DEFAULT_VALUE (1500-6)
#define CHANNEL_MAXIMUM_VALUE (2000)
// #define CHANNELS_MAX 12
// bb edit
#define CHANNELS_MAX 4
#define DEFAULT_CHANNELS 4
#define DEFAULT_FRAME_LENGTH 22500
#define DEFAULT_PULSE_LENGTH 300
#define DEFAULT_INVERT_STATE 0

#endif // __CONFIG_H__

