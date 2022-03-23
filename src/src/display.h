#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <Arduino.h>

#define WELCOME_SCREEN_TIMEOUT 500u
#define START_CHARGING_SCREEN_TIMEOUT 300u
#define CELL_READOUTS_DISPLAY_UPDATE_TIMEOUT 100u

typedef enum {
    POWER_ON_SCREEN = 0,
    MAIN_SCREEN = 1,
    CHANNEL_SETTINGS_SCREEN = 2,
    CHARGING_CONFIRMATION_SCREEN = 3,
    CHARGING_STARTED_SCREEN = 4,
    UNINITIALISED_SCREEN = 255
} MENU_INDEX;

typedef enum {
    MODE_CHANNEL_SETTING = 0,
    CHARGING_CURRENT_CHANNEL_SETTING = 1,
    CHARGING_STATE_CHANNEL_SETTING = 2
} CHANNEL_SETTING;

typedef struct {
    MENU_INDEX menuIndex;
    uint8_t editedChannel;
    uint8_t editedChannelSetting;
    bool startChargingChoice;
    bool updateCellsReadouts;
    uint16_t welcomeScreenTimeout;
    uint16_t startChargingScreenTimeout;
    uint16_t updateCellsReadoutsTimeout;
} DISPLAY_DATA;

extern DISPLAY_DATA displayData;

void displayInit(void);
void displayMenuHandler(void);
void displayMenuTimeouts(void);

#endif  // _DISPLAY_H_