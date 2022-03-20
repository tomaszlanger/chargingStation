#include "processControl.h"
#include "analogInputs.h"
#include "outputs.h"

#define NIMH_SHORTCUT_VOLTAGE 4.7f
#define NIMH_SHORTCUT_ADC_VALUE ((NIMH_SHORTCUT_VOLTAGE / 5.0f) * 1024 * ACD_INPUTS_VOLTAGE_MULTIPLIER)

#define NIMH_FULL_CHARGED_CURRENT 0.05f  // 10 bit ADC gives 50mA sensivity
#define NIMH_FULL_CHARGED_ADC_VALUE ((ACD_INPUTS_CURRENT_MULTIPLIER / (5.0f / 1024)) * NIMH_FULL_CHARGED_CURRENT)

#define ON_OUTPUT_DELAY 200u
#define ON_OUTPUT_READOUTS_DELAY 200u

// Vref = 2.5V
// RUser = 5[kohm] - przykładowa rezystancja (dowolnie nastawiaony ograncznik pradu)
// RvarTotal = 10 [kochm] - rezystancja całkowita potencjometru regulujacego ograncznik pradu
// RcurrentSense = 0.05 [ohm] - rezystancja mierzaca prad ladowania (fizyzcnie taki jest rezystor mierzacy prad)
// Napiecie 2.5V * (Ruser/(71,5 + RvarTotal)/ RcurrentSense

// 3.26 kochm = 2,04A limiter
// 2,45 kochm = 1.51A limiter
// 1,63 kochm = 1,001A limiter
// 0,82 kochm = 0.501A limiter

typedef enum {
    OFF_OUTPUT_STATE = 0,
    WAIT_FOR_ON_OUTPUT_STATE = 1,
    WAIT_FOR_READOUTS_IN_ON_OUTPUT_STATE = 2,
    ON_OUTPUT_ON_STATE = 3
} OUTPUT_STATE;

typedef enum { NO_OUTPUT_REQUEST = 0, ON_OUTPUT_REQUEST = 1, OFF_OUTPUT_REQUEST = 2 } OUTPUT_REQUEST;

CONTROL_DATA controlData;
static OUTPUT_STATE outputState[CHANNELS_QUANTITY];
static bool channelReadoutsReady[CHANNELS_QUANTITY];
static uint8_t lastChannelChargingMode[CHANNELS_QUANTITY];
static uint8_t channelOutputTimeout[CHANNELS_QUANTITY];

void processTimersHandler(void) {
    for (uint8_t i = 0; i < CHANNELS_QUANTITY; i++) {
        if (channelOutputTimeout[i] > 0) channelOutputTimeout[i]--;
    }
}

void processHandleOutputs(uint8_t channel, OUTPUT_REQUEST outputOnRequest) {
    bool outputEnabled = false;
    if (outputOnRequest == OFF_OUTPUT_REQUEST) {
        channelReadoutsReady[channel] = false;
        outputState[channel] = OFF_OUTPUT_STATE;
    }
    switch (outputState[channel]) {
        case OFF_OUTPUT_STATE:
            if (outputOnRequest == ON_OUTPUT_REQUEST) {
                channelOutputTimeout[channel] = ON_OUTPUT_DELAY;
                outputState[channel] = WAIT_FOR_ON_OUTPUT_STATE;
            }
            break;
        case WAIT_FOR_ON_OUTPUT_STATE:
            if (!channelOutputTimeout[channel]) {
                outputEnabled = true;
                channelOutputTimeout[channel] = ON_OUTPUT_READOUTS_DELAY;
                outputState[channel] = WAIT_FOR_READOUTS_IN_ON_OUTPUT_STATE;
            }
            break;
        case WAIT_FOR_READOUTS_IN_ON_OUTPUT_STATE:
            outputEnabled = true;
            if (!channelOutputTimeout[channel]) {
                channelReadoutsReady[channel] = true;
            }
            break;
        case ON_OUTPUT_ON_STATE:
            outputEnabled = true;
            break;
        default:
            break;
    }
    switch (controlData.channelChargingMode[channel]) {
        case LIPO_MODE:
            outputSetNiMhChannel(channel, outputEnabled);
            break;
        case NIMH_MODE:
            outputSetLiPoChannel(channel, outputEnabled);
            break;
        default:
            break;
    }
}

void processControlHandler(void) {
    OUTPUT_REQUEST outputOnRequest = NO_OUTPUT_REQUEST;
    for (uint8_t i = 0; i < CHANNELS_QUANTITY; i++) {
        if (controlData.channelChargingState[i] == ON_CHARGING_STATE) {
            switch (controlData.channelChargingMode[i]) {
                case LIPO_MODE:
                    if (channelReadoutsReady[i]) {
                        // do nothing here
                    }
                    break;
                case NIMH_MODE:
                    if (channelReadoutsReady[i]) {
                        if (controlData.cellVoltage[i] < NIMH_SHORTCUT_ADC_VALUE) {
                            controlData.channelChargingState[i] = ALERT_CHARGING_STATE;
                        }
                        if (controlData.cellCurrent[i] < NIMH_FULL_CHARGED_ADC_VALUE) {
                            controlData.cellCurrent[i] = FINISHED_CHARGING_STATE;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        outputOnRequest = NO_OUTPUT_REQUEST;
        if (lastChannelChargingMode[i] != controlData.channelChargingState[i]) {
            if (controlData.channelChargingState[i] == ON_CHARGING_STATE) {
                outputOnRequest = ON_OUTPUT_REQUEST;
            } else {
                outputOnRequest = OFF_OUTPUT_REQUEST;
            }
            lastChannelChargingMode[i] = controlData.channelChargingState[i];
        }
        processHandleOutputs(i, outputOnRequest);
    }
}
