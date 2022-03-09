#include <display.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

CONTROL_DATA controlData;
DISPLAY_DATA displayData = {.menuIndex = MENU_INDEX(POWER_ON_SCREEN)};

MENU_INDEX lastMenuIndex = UNINITIALISED_SCREEN;
CHARGER_MODE lastChargerMode = CHARGE_MODE;
uint8_t lastEditedChannel = 0;
uint8_t lastActiveChannels = 0;
CHANNEL_SETTING lastEditedChannelSetting = MODE_CHANNEL_SETTING;
CHANNEL_CHARGING_MODE lastChannelChargingMode;
CHANNEL_CHARGING_CURRENT lastChannelChargingCurrent;
CHANNEL_CHARGING_STATE lastChannelChargingState[CHANNELS_QUANTITY];
bool lastStartChargingChoice;

LiquidCrystal_I2C lcd(0x27, 20,
                      4);  //że podłączony jest wyswietlacz LCD o adresie 0x27 o wymiarach 20 znaków w 4 liniach

void displayInit(void) {
    lcd.begin(20, 4);  // zaczynami z menu inicjalizajca wyswietlacza
    lcd.init();        // initialize the lcd
    lcd.backlight();
    lcd.clear();
}

bool displayCheckChannelsChanges(void) {
    bool statesChanged = false;
    for (uint8_t i = 0; i < CHANNELS_QUANTITY; i++) {
        if (controlData.channelChargingState[i] != lastChannelChargingState[i]) {
            lastChannelChargingState[i] = controlData.channelChargingState[i];
            statesChanged = true;
        }
    }
    return statesChanged;
}

void displayPrintActiveChannels(void) {
    for (uint8_t i = 0; i < CHANNELS_QUANTITY; i++) {
        switch (controlData.channelChargingState[i]) {
            case OFF_CHARGING_STATE:
                lcd.print("0");
                break;
            case ON_CHARGING_STATE:
                lcd.print("1");
                break;
            case ALERT_CHARGING_STATE:
                lcd.print("A");
                break;

            default:
                break;
        }
    }
}

void displayPrintEditedChannelIndicator(void) {
    for (uint8_t i = 0; i < CHANNELS_QUANTITY; i++) {
        if (i == displayData.editedChannel) {
            lcd.print("*");
        } else {
            lcd.print(" ");
        }
    }
}

void displayUpdateSelectionIndicator(bool show) {
    if (show) {
        lcd.print(">");
    } else {
        lcd.print(" ");
    }
}

void displayPrintChargingMode(void) {
    if (controlData.channelChargingMode[displayData.editedChannel] == LIPO_MODE) {
        lcd.print("LiPo");
    } else {
        lcd.print("NiMH");
    }
}

void displayPrintChargingState(void) {
    switch (controlData.channelChargingState[displayData.editedChannel]) {
        case OFF_CHARGING_STATE:
            lcd.print("OFF  ");
            break;
        case ON_CHARGING_STATE:
            lcd.print("ON   ");
            break;
        case ALERT_CHARGING_STATE:
            lcd.print("ALERT");
            break;

        default:
            break;
    }
}

void displayPrintChargingCurrent(void) {
    switch (controlData.channelChargingCurrent[displayData.editedChannel]) {
        case A_0_5_CHARGING_CURRENT:
            lcd.print("0.5A");
            break;
        case A_1_0_CHARGING_CURRENT:
            lcd.print("1.0A");
            break;
        case A_1_5_CHARGING_CURRENT:
            lcd.print("1.5A");
            break;
        case A_2_0_CHARGING_CURRENT:
            lcd.print("2.0A");
            break;
        default:
            break;
    }
}

void displayPrintFloatValue(float value) {
    if (value < 10) {
        lcd.print(" ");
    }
    lcd.print(value, 1);
}

void displayMenuHandler(void) {
    bool menuChange = (displayData.menuIndex != lastMenuIndex);
    lastMenuIndex = displayData.menuIndex;

    switch (displayData.menuIndex) {
        case POWER_ON_SCREEN:
            if (menuChange) {
                lcd.setCursor(0, 0);
                lcd.print("                    ");
                lcd.setCursor(0, 1);
                lcd.print("     LiPo/NiMH      ");
                lcd.setCursor(0, 2);
                lcd.print("  Charging Station  ");
                lcd.setCursor(0, 3);
                lcd.print("                    ");
            }
            break;

        case MAIN_SCREEN:
            if (menuChange) {
                lcd.setCursor(0, 0);
                lcd.print("          ");
                displayPrintEditedChannelIndicator();
                lcd.print("    ");
                lcd.setCursor(0, 1);
                lcd.print("Channels: ");
                displayPrintActiveChannels();
                lcd.print("    ");
                lcd.setCursor(0, 2);
                lcd.print("Sts: ");
                displayPrintFloatValue(controlData.cellVoltage[displayData.editedChannel]);
                lcd.print("V ");
                displayPrintFloatValue(controlData.cellCurrent[displayData.editedChannel]);
                lcd.print("A ");
                lcd.setCursor(0, 3);
                lcd.print("Set: ");
                displayPrintChargingMode();
                lcd.print(" ");
                displayPrintChargingCurrent();
                lcd.print("A     ");
                displayCheckChannelsChanges();
                lastChannelChargingCurrent = controlData.channelChargingCurrent[displayData.editedChannel];
                lastChannelChargingMode = controlData.channelChargingMode[displayData.editedChannel];
                lastEditedChannel = displayData.editedChannel;
                displayData.updateCellsReadouts = false;
            }
            if (displayData.editedChannel != lastEditedChannel) {
                lastEditedChannel = displayData.editedChannel;
                lcd.setCursor(5, 1);
                displayPrintEditedChannelIndicator();
                lcd.setCursor(5, 3);
                displayPrintEditedChannelIndicator();
            }
            if (displayCheckChannelsChanges()) {
                lcd.setCursor(10, 1);
                displayPrintActiveChannels();
            }
            if (displayData.updateCellsReadouts) {
                displayData.updateCellsReadouts = false;
                lcd.setCursor(5, 2);
                displayPrintFloatValue(controlData.cellVoltage[displayData.editedChannel]);
                lcd.setCursor(12, 2);
                displayPrintFloatValue(controlData.cellCurrent[displayData.editedChannel]);
            }
            if (controlData.channelChargingMode[displayData.editedChannel] != lastChannelChargingMode) {
                lastChannelChargingMode = controlData.channelChargingMode[displayData.editedChannel];
                lcd.setCursor(5, 3);
                displayPrintChargingMode();
            }
            if (controlData.channelChargingCurrent[displayData.editedChannel] != lastChannelChargingCurrent) {
                lastChannelChargingCurrent = controlData.channelChargingCurrent[displayData.editedChannel];
                lcd.setCursor(11, 2);
                displayPrintChargingCurrent();
            }
            break;

        case CHANNEL_SETTINGS_SCREEN:
            if (menuChange) {
                lcd.setCursor(0, 0);
                lcd.print(String("Channel ") + displayData.editedChannel + String(" settings  "));
                lcd.setCursor(0, 1);
                displayUpdateSelectionIndicator(displayData.editedChannelSetting == MODE_CHANNEL_SETTING);
                lcd.print("Mode: ");
                displayPrintChargingMode();
                lcd.print("         ");
                lcd.setCursor(0, 2);
                displayUpdateSelectionIndicator(displayData.editedChannelSetting == CHARGING_CURRENT_CHANNEL_SETTING);
                lcd.print("Current: ");
                displayPrintChargingCurrent();
                lcd.print("       ");
                lcd.setCursor(0, 3);
                displayUpdateSelectionIndicator(displayData.editedChannelSetting == CHARGING_STATE_CHANNEL_SETTING);
                lcd.print("Charging: ");
                displayPrintChargingState();
                lcd.print("     ");
                lastChannelChargingMode = controlData.channelChargingMode[displayData.editedChannel];
                lastChannelChargingCurrent = controlData.channelChargingCurrent[displayData.editedChannel];
                lastChannelChargingState[displayData.editedChannel] =
                    controlData.channelChargingState[displayData.editedChannel];
            }
            if (displayData.editedChannelSetting != lastEditedChannelSetting) {
                lastEditedChannelSetting = displayData.editedChannelSetting;
                lcd.setCursor(0, 1);
                displayUpdateSelectionIndicator(displayData.editedChannelSetting == MODE_CHANNEL_SETTING);
                lcd.setCursor(0, 2);
                displayUpdateSelectionIndicator(displayData.editedChannelSetting == CHARGING_CURRENT_CHANNEL_SETTING);
                lcd.setCursor(0, 3);
                displayUpdateSelectionIndicator(displayData.editedChannelSetting == CHARGING_STATE_CHANNEL_SETTING);
            }
            if (controlData.channelChargingMode[displayData.editedChannel] != lastChannelChargingMode) {
                lastChannelChargingMode = controlData.channelChargingMode[displayData.editedChannel];
                lcd.setCursor(8, 1);
                displayPrintChargingMode();
            }
            if (controlData.channelChargingCurrent[displayData.editedChannel] != lastChannelChargingCurrent) {
                lastChannelChargingCurrent = controlData.channelChargingCurrent[displayData.editedChannel];
                lcd.setCursor(11, 2);
                displayPrintChargingCurrent();
            }
            if (controlData.channelChargingState[displayData.editedChannel] !=
                lastChannelChargingState[displayData.editedChannel]) {
                lastChannelChargingState[displayData.editedChannel] =
                    controlData.channelChargingState[displayData.editedChannel];
                lcd.setCursor(12, 2);
                displayPrintChargingState();
            }
            break;

        case CHARGING_CONFIRMATION_SCREEN:
            if (menuChange) {
                lcd.setCursor(0, 0);
                lcd.print(String("Start Ch.") + displayData.editedChannel + String(" charging?"));
                lcd.setCursor(0, 1);
                displayUpdateSelectionIndicator(displayData.startChargingChoice == false);
                lcd.print("No                  ");
                lcd.setCursor(0, 2);
                displayUpdateSelectionIndicator(displayData.startChargingChoice == true);
                lcd.print("Yes                 ");
                lcd.setCursor(0, 3);
                lcd.print("WARN! CONNECT BATT! ");
                displayData.startChargingChoice = false;
                lastChannelChargingMode = controlData.channelChargingMode[displayData.editedChannel];
                lastChannelChargingCurrent = controlData.channelChargingCurrent[displayData.editedChannel];
                lastChannelChargingState[displayData.editedChannel] =
                    controlData.channelChargingState[displayData.editedChannel];
            }
            if (displayData.startChargingChoice != lastStartChargingChoice) {
                lastStartChargingChoice = displayData.startChargingChoice;
                lcd.setCursor(1, 1);
                displayUpdateSelectionIndicator(displayData.startChargingChoice == false);
                lcd.setCursor(1, 2);
                displayUpdateSelectionIndicator(displayData.startChargingChoice == true);
            }
            break;

        case CHARGING_STARTED_SCREEN:
            if (menuChange) {
                lcd.setCursor(0, 0);
                lcd.print("                    ");
                lcd.setCursor(0, 1);
                lcd.print(" Charging Started!  ");
                lcd.setCursor(0, 2);
                lcd.print("                    ");
                lcd.setCursor(0, 3);
                lcd.print("                    ");
            }
            break;

        default:
            break;
    }
}