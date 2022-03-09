#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include <Arduino.h>

typedef struct {
    uint8_t back : 1;
    uint8_t next : 1;
    uint8_t prev : 1;
    uint8_t set : 1;
} BUTTONS;

extern BUTTONS buttons;

void buttonsInit(void);
void buttonsReadPinsAndFilter(void);
void readButtons(void);

#endif  // _BUTTONS_H_