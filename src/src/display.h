#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <Arduino.h>

#define CHANNELS_QUANTITY 6u

typedef enum {
    POWER_ON_SCREEN = 0,
    MAIN_SCREEN = 1,
    CHANNEL_SETTINGS_SCREEN = 2,
    CHARGING_CONFIRMATION_SCREEN = 3,
    CHARGING_STARTED_SCREEN = 4,
    UNINITIALISED_SCREEN = 255
} MENU_INDEX;

typedef enum { CHARGE_MODE = 0, TESTS_MODE = 1 } CHARGER_MODE;

typedef enum { LIPO_MODE = 0, NIMH_MODE = 1 } CHANNEL_CHARGING_MODE;

typedef enum {
    A_0_5_CHARGING_CURRENT = 0,
    A_1_0_CHARGING_CURRENT = 1,
    A_1_5_CHARGING_CURRENT = 2,
    A_2_0_CHARGING_CURRENT = 3
} CHANNEL_CHARGING_CURRENT;

typedef enum { OFF_CHARGING_STATE = 0, ON_CHARGING_STATE = 1, ALERT_CHARGING_STATE = 2 } CHANNEL_CHARGING_STATE;

typedef enum {
    MODE_CHANNEL_SETTING = 0,
    CHARGING_CURRENT_CHANNEL_SETTING = 1,
    CHARGING_STATE_CHANNEL_SETTING = 2
} CHANNEL_SETTING;

typedef struct {
    CHANNEL_CHARGING_MODE channelChargingMode[CHANNELS_QUANTITY];
    CHANNEL_CHARGING_CURRENT channelChargingCurrent[CHANNELS_QUANTITY];
    CHANNEL_CHARGING_STATE channelChargingState[CHANNELS_QUANTITY];
    float cellVoltage[CHANNELS_QUANTITY];
    float cellCurrent[CHANNELS_QUANTITY];
    uint8_t activeChannels;
} CONTROL_DATA;

typedef struct {
    MENU_INDEX menuIndex;
    uint8_t editedChannel;
    CHANNEL_SETTING editedChannelSetting;
    bool startChargingChoice;
    bool updateCellsReadouts;
} DISPLAY_DATA;

extern CONTROL_DATA controlData;
extern DISPLAY_DATA displayData;

void displayInit(void);
void displayMenuHandler(void);

#endif  // _DISPLAY_H_