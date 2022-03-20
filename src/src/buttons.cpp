#include <buttons.h>

#define btnBackPin 5
#define btnNextPin 4
#define btnPrevPin 3
#define btnSetPin 2

BUTTONS buttons;
static uint8_t rawButtonsState, lastRawButtonsState, steadyButtons;
static uint8_t filteredButtonsState, lastfilteredButtonsState, risingButtonsState;

void buttonsInit(void) {
    pinMode(btnBackPin, INPUT_PULLUP);
    pinMode(btnNextPin, INPUT_PULLUP);
    pinMode(btnPrevPin, INPUT_PULLUP);
    pinMode(btnSetPin, INPUT_PULLUP);
}

void buttonsReadPinsAndFilter(void) {
    rawButtonsState = 0;
    if (!digitalRead(btnBackPin)) rawButtonsState |= 0x01;
    if (!digitalRead(btnNextPin)) rawButtonsState |= 0x02;
    if (!digitalRead(btnPrevPin)) rawButtonsState |= 0x04;
    if (!digitalRead(btnSetPin)) rawButtonsState |= 0x08;

    if (rawButtonsState == lastRawButtonsState) {
        if (steadyButtons < 3) steadyButtons++;
    } else {
        steadyButtons = 0;
    }
    lastRawButtonsState = rawButtonsState;

    if (steadyButtons == 3) {
        filteredButtonsState = rawButtonsState;
    }

    for (uint8_t i = 0; i < 4; i++) {
        if (((filteredButtonsState >> i) & 0x01) && !((lastfilteredButtonsState >> i) & 0x01)) {
            risingButtonsState |= (0x01 << i);
        }
    }
    lastfilteredButtonsState = filteredButtonsState;
}

void readButtons(void) {
    cli();
    buttons.back = ((risingButtonsState & 0x01) != 0);
    buttons.next = ((risingButtonsState & 0x02) != 0);
    buttons.prev = ((risingButtonsState & 0x04) != 0);
    buttons.set = ((risingButtonsState & 0x08) != 0);
    risingButtonsState = 0;
    sei();
}