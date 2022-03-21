#ifndef _OUTPUTS_H_
#define _OUTPUTS_H_

#include <Arduino.h>
#include <FastX9CXXX.h>
#include "processControl.h"

void outputsInit(void);
void outputSetNiMhChannel(uint8_t channel, bool state);
void outputSetLiPoChannel(uint8_t channel, bool state);
void outputSetCurrentChannel(uint8_t channel, CHANNEL_CHARGING_CURRENT current);
void outputSetChannelAlertLed(uint8_t channel, CHANNEL_CHARGING_STATE channelState);

#endif  // OUTPUTS_H
