#ifndef _PROCESS_CONTROL_H_
#define _PROCESS_CONTROL_H_
#include <Arduino.h>

#define CHANNELS_QUANTITY 6u

typedef enum {
    OFF_CHARGING_STATE = 0,
    ON_CHARGING_STATE = 1,
    ALERT_CHARGING_STATE = 2,
    FINISHED_CHARGING_STATE = 3
} CHANNEL_CHARGING_STATE;

typedef enum { LIPO_MODE = 0, NIMH_MODE = 1 } CHANNEL_CHARGING_MODE;

typedef enum {
    A_0_5_CHARGING_CURRENT = 0,
    A_1_0_CHARGING_CURRENT = 1,
    A_1_5_CHARGING_CURRENT = 2,
    A_2_0_CHARGING_CURRENT = 3
} CHANNEL_CHARGING_CURRENT;

typedef struct {
    uint8_t channelChargingMode[CHANNELS_QUANTITY];
    uint8_t channelChargingCurrent[CHANNELS_QUANTITY];
    uint8_t channelChargingState[CHANNELS_QUANTITY];
    float cellVoltage[CHANNELS_QUANTITY];
    float cellCurrent[CHANNELS_QUANTITY];
} CONTROL_DATA;

extern CONTROL_DATA controlData;

void processControlHandler(void);
void processTimersHandler(void);

#endif  // PROCESS_CONTROL_H