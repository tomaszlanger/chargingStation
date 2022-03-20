#ifndef _OUTPUTS_H_
#define _OUTPUTS_H_

#include <Arduino.h>

void outputsInit(void);
void outputSetNiMhChannel(uint8_t channel, bool state);
void outputSetLiPoChannel(uint8_t channel, bool state);

#endif  // OUTPUTS_H
