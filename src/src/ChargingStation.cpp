#include <FastX9CXXX.h>  //DO OBSŁUGI POTENCJOMETRU  ELEKTRONICZNEGO
#include "display.h"
#include "timer.h"
#include "buttons.h"
#include "menuControl.h"
#include "analogInputs.h"
#include "processControl.h"
#include "outputs.h"

void timerProcess(void) {
    buttonsReadPinsAndFilter();
    displayMenuTimeouts();
    processTimersHandler();
}

void setup() {
    Serial.begin(115200);  // Uruchomienie komunikacji
    Serial.println("Turn ON Charging Station...");
    outputsInit();
    buttonsInit();
    displayInit();
    initTimer1(timerProcess);
}

void loop() {
    readButtons();
    analogInputsRead();
    menuControlHandler();
    processControlHandler();
    displayMenuHandler();
}
