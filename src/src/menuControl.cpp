#include "menuControl.h"
#include "processControl.h"
#include "buttons.h"
#include "display.h"

void menuControlHandler(void) {
    switch (displayData.menuIndex) {
        case POWER_ON_SCREEN:
            if (!displayData.welcomeScreenTimeout) {
                displayData.menuIndex = MAIN_SCREEN;
            }
            break;
        case MAIN_SCREEN:
            if (buttons.back) {
                if (displayData.editedChannel > 0)
                    displayData.editedChannel--;
                else
                    displayData.editedChannel = CHANNELS_QUANTITY - 1;
                displayData.updateCellsReadouts = true;
            }
            if (buttons.next) {
                if (displayData.editedChannel < CHANNELS_QUANTITY - 1)
                    displayData.editedChannel++;
                else
                    displayData.editedChannel = 0;
                displayData.updateCellsReadouts = true;
            }
            if (buttons.prev) {
            }
            if (buttons.set) {
                displayData.menuIndex = CHANNEL_SETTINGS_SCREEN;
                displayData.editedChannelSetting = MODE_CHANNEL_SETTING;
            }
            break;
        case CHANNEL_SETTINGS_SCREEN:
            if (buttons.back) {
                if (displayData.editedChannelSetting > 0) displayData.editedChannelSetting--;
            }
            if (buttons.next) {
                if (displayData.editedChannelSetting < CHARGING_STATE_CHANNEL_SETTING)
                    displayData.editedChannelSetting++;
            }
            if (buttons.prev) {
                displayData.menuIndex = MAIN_SCREEN;
            }
            if (buttons.set) {
                switch (displayData.editedChannelSetting) {
                    case MODE_CHANNEL_SETTING:
                        if (controlData.channelChargingState[displayData.editedChannel] != ON_CHARGING_STATE) {
                            if (controlData.channelChargingMode[displayData.editedChannel] < NIMH_MODE)
                                controlData.channelChargingMode[displayData.editedChannel]++;
                            else
                                controlData.channelChargingMode[displayData.editedChannel] = 0;
                        }
                        break;
                    case CHARGING_CURRENT_CHANNEL_SETTING:
                        if (controlData.channelChargingState[displayData.editedChannel] != ON_CHARGING_STATE) {
                            if (controlData.channelChargingCurrent[displayData.editedChannel] < A_2_0_CHARGING_CURRENT)
                                controlData.channelChargingCurrent[displayData.editedChannel]++;
                            else
                                controlData.channelChargingCurrent[displayData.editedChannel] = 0;
                        }
                        break;
                    case CHARGING_STATE_CHANNEL_SETTING:
                        if (controlData.channelChargingState[displayData.editedChannel] != ON_CHARGING_STATE) {
                            displayData.menuIndex = CHARGING_CONFIRMATION_SCREEN;
                            displayData.startChargingChoice = false;
                        } else {
                            controlData.channelChargingState[displayData.editedChannel] = OFF_CHARGING_STATE;
                        }
                        break;
                    default:
                        break;
                }
            }
            break;
        case CHARGING_CONFIRMATION_SCREEN:
            if (buttons.back || buttons.next) {
                displayData.startChargingChoice ? displayData.startChargingChoice = false
                                                : displayData.startChargingChoice = true;
            }
            if (buttons.prev) {
                displayData.menuIndex = CHANNEL_SETTINGS_SCREEN;
            }
            if (buttons.set) {
                if (displayData.startChargingChoice) {
                    displayData.menuIndex = CHARGING_STARTED_SCREEN;
                    displayData.startChargingScreenTimeout = START_CHARGING_SCREEN_TIMEOUT;
                    controlData.channelChargingState[displayData.editedChannel] = ON_CHARGING_STATE;
                } else {
                    displayData.menuIndex = CHANNEL_SETTINGS_SCREEN;
                }
            }
            break;
        case CHARGING_STARTED_SCREEN:
            if (!displayData.startChargingScreenTimeout) {
                displayData.menuIndex = MAIN_SCREEN;
            }
            break;

        default:
            break;
    }
}
