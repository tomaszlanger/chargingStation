#include "outputs.h"
#include "processControl.h"

#define CELL_1_NIMH_OUTPUT 22
#define CELL_2_NIMH_OUTPUT 23
#define CELL_3_NIMH_OUTPUT 24
#define CELL_4_NIMH_OUTPUT 25
#define CELL_5_NIMH_OUTPUT 26
#define CELL_6_NIMH_OUTPUT 27

#define CELL_1_LIPO_OUTPUT 28
#define CELL_2_LIPO_OUTPUT 29
#define CELL_3_LIPO_OUTPUT 30
#define CELL_4_LIPO_OUTPUT 31
#define CELL_5_LIPO_OUTPUT 32
#define CELL_6_LIPO_OUTPUT 33

void outputsInit(void) {
    pinMode(CELL_1_NIMH_OUTPUT, OUTPUT);
    pinMode(CELL_2_NIMH_OUTPUT, OUTPUT);
    pinMode(CELL_3_NIMH_OUTPUT, OUTPUT);
    pinMode(CELL_4_NIMH_OUTPUT, OUTPUT);
    pinMode(CELL_5_NIMH_OUTPUT, OUTPUT);
    pinMode(CELL_6_NIMH_OUTPUT, OUTPUT);

    pinMode(CELL_1_LIPO_OUTPUT, OUTPUT);
    pinMode(CELL_2_LIPO_OUTPUT, OUTPUT);
    pinMode(CELL_3_LIPO_OUTPUT, OUTPUT);
    pinMode(CELL_4_LIPO_OUTPUT, OUTPUT);
    pinMode(CELL_5_LIPO_OUTPUT, OUTPUT);
    pinMode(CELL_6_LIPO_OUTPUT, OUTPUT);
}

void outputSetNiMhChannel(uint8_t channel, bool state) {
    switch (channel) {
        case 0:
            digitalWrite(CELL_1_NIMH_OUTPUT, state);
            break;
        case 1:
            digitalWrite(CELL_2_NIMH_OUTPUT, state);
            break;
        case 2:
            digitalWrite(CELL_3_NIMH_OUTPUT, state);
            break;
        case 3:
            digitalWrite(CELL_4_NIMH_OUTPUT, state);
            break;
        case 4:
            digitalWrite(CELL_5_NIMH_OUTPUT, state);
            break;
        case 5:
            digitalWrite(CELL_6_NIMH_OUTPUT, state);
            break;
        default:
            break;
    }
}

void outputSetLiPoChannel(uint8_t channel, bool state) {
    switch (channel) {
        case 0:
            digitalWrite(CELL_1_LIPO_OUTPUT, state);
            break;
        case 1:
            digitalWrite(CELL_2_LIPO_OUTPUT, state);
            break;
        case 2:
            digitalWrite(CELL_3_LIPO_OUTPUT, state);
            break;
        case 3:
            digitalWrite(CELL_4_LIPO_OUTPUT, state);
            break;
        case 4:
            digitalWrite(CELL_5_LIPO_OUTPUT, state);
            break;
        case 5:
            digitalWrite(CELL_6_LIPO_OUTPUT, state);
            break;
        default:
            break;
    }
}
