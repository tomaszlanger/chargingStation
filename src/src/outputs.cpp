#include "outputs.h"
#include "processControl.h"

#define ALERT_LED_BLINK_TIME_MS 200u

#define ALERT_LED_1_OUTPUT 53
#define ALERT_LED_2_OUTPUT 51
#define ALERT_LED_3_OUTPUT 49
#define ALERT_LED_4_OUTPUT 47
#define ALERT_LED_5_OUTPUT 45
#define ALERT_LED_6_OUTPUT 43

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

//#define X9_1_CS_PIN 3
#define X9_1_UD_PIN 40
#define X9_1_INC_PIN 41

//#define X9_2_CS_PIN 3
#define X9_2_UD_PIN 42
#define X9_2_INC_PIN 43

//#define X9_3_CS_PIN 3
#define X9_3_UD_PIN 44
#define X9_3_INC_PIN 45

//#define X9_4_CS_PIN 3
#define X9_4_UD_PIN 46
#define X9_4_INC_PIN 47

//#define X9_5_CS_PIN 3
#define X9_5_UD_PIN 48
#define X9_5_INC_PIN 49

//#define X9_6_CS_PIN 3
#define X9_6_UD_PIN 50
#define X9_6_INC_PIN 51

// From https://www.electroschematics.com/dc-module/
// Vref = 2.5V
// RUser = 5[kohm] - przykładowa rezystancja (dowolnie nastawiaony ograncznik pradu)
// RvarTotal = 10 [kochm] - rezystancja całkowita potencjometru regulujacego ograncznik pradu
// RcurrentSense = 0.05 [ohm] - rezystancja mierzaca prad ladowania (fizyzcnie taki jest rezystor mierzacy prad)
// Napiecie 2.5V * (Ruser/(71,5 + RvarTotal)/ RcurrentSense
// 3.26 kochm = 2,04A limiter
// 2,45 kochm = 1.51A limiter
// 1,63 kochm = 1,001A limiter
// 0,82 kochm = 0.501A limiter

#define CURRENT_LIMITER_05A_POT_STEPS 8u
#define CURRENT_LIMITER_10A_POT_STEPS 16u
#define CURRENT_LIMITER_15A_POT_STEPS 25u
#define CURRENT_LIMITER_20A_POT_STEPS 33u

static FastX9C104 Potentiometer[CHANNELS_QUANTITY];

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

    pinMode(ALERT_LED_1_OUTPUT, OUTPUT);
    pinMode(ALERT_LED_2_OUTPUT, OUTPUT);
    pinMode(ALERT_LED_3_OUTPUT, OUTPUT);
    pinMode(ALERT_LED_4_OUTPUT, OUTPUT);
    pinMode(ALERT_LED_5_OUTPUT, OUTPUT);
    pinMode(ALERT_LED_6_OUTPUT, OUTPUT);

    digitalWrite(CELL_1_NIMH_OUTPUT, true);
    digitalWrite(CELL_2_NIMH_OUTPUT, true);
    digitalWrite(CELL_3_NIMH_OUTPUT, true);
    digitalWrite(CELL_4_NIMH_OUTPUT, true);
    digitalWrite(CELL_5_NIMH_OUTPUT, true);
    digitalWrite(CELL_6_NIMH_OUTPUT, true);

    digitalWrite(CELL_1_LIPO_OUTPUT, true);
    digitalWrite(CELL_2_LIPO_OUTPUT, true);
    digitalWrite(CELL_3_LIPO_OUTPUT, true);
    digitalWrite(CELL_4_LIPO_OUTPUT, true);
    digitalWrite(CELL_5_LIPO_OUTPUT, true);
    digitalWrite(CELL_6_LIPO_OUTPUT, true);

    Potentiometer[0].Setup(X9_1_UD_PIN, X9_1_INC_PIN);
    Potentiometer[1].Setup(X9_2_UD_PIN, X9_2_INC_PIN);
    Potentiometer[2].Setup(X9_3_UD_PIN, X9_3_INC_PIN);
    Potentiometer[3].Setup(X9_4_UD_PIN, X9_4_INC_PIN);
    Potentiometer[4].Setup(X9_5_UD_PIN, X9_5_INC_PIN);
    Potentiometer[5].Setup(X9_6_UD_PIN, X9_6_INC_PIN);

    for (uint8_t i = 0; i < CHANNELS_QUANTITY; i++) {
        Potentiometer[i].Reset();
    }
}

void outputSetCurrentChannel(uint8_t channel, CHANNEL_CHARGING_CURRENT current) {
    uint8_t potentiometerSteps = 0;
    switch (current) {
        case A_0_5_CHARGING_CURRENT:
            potentiometerSteps = CURRENT_LIMITER_05A_POT_STEPS;
            break;
        case A_1_0_CHARGING_CURRENT:
            potentiometerSteps = CURRENT_LIMITER_10A_POT_STEPS;
            break;
        case A_1_5_CHARGING_CURRENT:
            potentiometerSteps = CURRENT_LIMITER_15A_POT_STEPS;
            break;
        case A_2_0_CHARGING_CURRENT:
            potentiometerSteps = CURRENT_LIMITER_20A_POT_STEPS;
            break;
        default:
            break;
    }
    Potentiometer[channel].Reset();
    Potentiometer[channel].JumpToStep(potentiometerSteps);
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

void outputSetChannelAlertLed(uint8_t channel, CHANNEL_CHARGING_STATE channelState) {
    uint32_t currentTime;
    static uint32_t lastTime = 0;
    static uint8_t ledBlinkState = 0;
    uint8_t ledLogicState = 0;
    uint8_t ledPin = ALERT_LED_1_OUTPUT;

    currentTime = millis();
    if ((currentTime - lastTime) >= ALERT_LED_BLINK_TIME_MS) {
        ledBlinkState = !ledBlinkState;
        lastTime = currentTime;
    }

    switch (channel) {
        case 0:
            ledPin = ALERT_LED_1_OUTPUT;
            break;
        case 1:
            ledPin = ALERT_LED_2_OUTPUT;
            break;
        case 2:
            ledPin = ALERT_LED_3_OUTPUT;
            break;
        case 3:
            ledPin = ALERT_LED_4_OUTPUT;
            break;
        case 4:
            ledPin = ALERT_LED_5_OUTPUT;
            break;
        case 5:
            ledPin = ALERT_LED_6_OUTPUT;
            break;
        default:
            break;
    }
    switch (channelState) {
        case OFF_CHARGING_STATE:
        case FINISHED_CHARGING_STATE:
            ledLogicState = 0;
            break;
        case ON_CHARGING_STATE:
            ledLogicState = 1;
            break;
        case ALERT_CHARGING_STATE:
            ledLogicState = ledBlinkState;
            break;
        default:
            break;
    }
    digitalWrite(ledPin, ledLogicState);
}
