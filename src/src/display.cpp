#include <display.h>  //DO OBSŁUGI POTENCJOMETRU  ELEKTRONICZNEGO
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CHANNELS_QUANTITY 8u

typedef enum {
    POWER_ON_SCREEN = 0,
    SELECT_MODE_SCREEN = 1,
    SELECT_CHANNEL_SCREEN = 2,
    CHANNEL_SETTINGS_SCREEN = 3,
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

typedef struct {
    CHARGER_MODE chargerMode;
    CHANNEL_CHARGING_MODE channelChargingMode[CHANNELS_QUANTITY];
    CHANNEL_CHARGING_CURRENT channelChargingCurrent[CHANNELS_QUANTITY];
    uint8_t activeChannels;
} CONTROL_DATA;

typedef enum { MODE_CHANNEL_SETTING = 0, CHARGING_CURRENT_CHANNEL_SETTING = 1 } CHANNEL_SETTING;

typedef struct {
    MENU_INDEX menuIndex;
    uint8_t editedChannel;
    CHANNEL_SETTING editedChannelSetting;
} DISPLAY_DATA;

CONTROL_DATA controlData = {.chargerMode = CHARGER_MODE(CHARGE_MODE)};
DISPLAY_DATA displayData = {.menuIndex = MENU_INDEX(POWER_ON_SCREEN)};

MENU_INDEX lastMenuIndex = UNINITIALISED_SCREEN;
CHARGER_MODE lastChargerMode = CHARGE_MODE;
uint8_t lastEditedChannel = 0;
uint8_t lastActiveChannels = 0;
CHANNEL_SETTING lastEditedChannelSetting = MODE_CHANNEL_SETTING;
CHANNEL_CHARGING_MODE lastChannelChargingMode;
CHANNEL_CHARGING_CURRENT lastChannelChargingCurrent;

LiquidCrystal_I2C lcd(0x27, 20,
                      4);  //że podłączony jest wyswietlacz LCD o adresie 0x27 o wymiarach 20 znaków w 4 liniach

void displayInit(void) {
    lcd.begin(20, 4);  // zaczynami z menu inicjalizajca wyswietlacza
    lcd.init();        // initialize the lcd
    lcd.backlight();
    lcd.clear();
}

void displayPrintActiveChannels(void) {
    for (uint8_t i = 0; i < CHANNELS_QUANTITY; i++) {
        ((controlData.activeChannels >> i) & 0x01) ? lcd.print("1") : lcd.print("0");
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

void displayPrintChargingCurrent(void) {
    switch (controlData.channelChargingCurrent[displayData.editedChannel]) {
        case A_0_5_CHARGING_CURRENT:
            lcd.print("0.5 A");
            break;
        case A_1_0_CHARGING_CURRENT:
            lcd.print("1.0 A");
            break;
        case A_1_5_CHARGING_CURRENT:
            lcd.print("1.5 A");
            break;
        case A_2_0_CHARGING_CURRENT:
            lcd.print("2.0 A");
            break;
        default:
            break;
    }
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

        case SELECT_MODE_SCREEN:
            if (menuChange) {
                lcd.setCursor(0, 0);
                lcd.print("Select Charger Mode ");
                lcd.setCursor(0, 1);
                displayUpdateSelectionIndicator(controlData.chargerMode == CHARGE_MODE);
                lcd.print("Charging           ");
                lcd.setCursor(0, 2);
                displayUpdateSelectionIndicator(controlData.chargerMode == TESTS_MODE);
                lcd.print("Tests              ");
                lcd.setCursor(0, 3);
                lcd.print("                    ");
            }
            if (controlData.chargerMode != lastChargerMode) {
                lastChargerMode = controlData.chargerMode;
                if (controlData.chargerMode == CHARGE_MODE) {
                    lcd.setCursor(0, 1);
                    displayUpdateSelectionIndicator(controlData.chargerMode == CHARGE_MODE);
                    lcd.setCursor(0, 2);
                    displayUpdateSelectionIndicator(controlData.chargerMode == TESTS_MODE);
                }
            }
            break;

        case SELECT_CHANNEL_SCREEN:
            if (menuChange) {
                lcd.setCursor(0, 0);
                lcd.print("    Charge Mode     ");
                lcd.setCursor(0, 1);
                lcd.print("   Select Channels  ");
                lcd.setCursor(0, 2);
                lcd.print("      ");
                displayPrintEditedChannelIndicator();
                lcd.print("      ");
                lcd.setCursor(0, 3);
                lcd.print("      ");
                displayPrintActiveChannels();
                lcd.print("      ");
            }
            if (displayData.editedChannel != lastEditedChannel) {
                lastEditedChannel = displayData.editedChannel;
                lcd.setCursor(5, 1);
                displayPrintEditedChannelIndicator();
                lcd.setCursor(5, 3);
                displayPrintEditedChannelIndicator();
            }
            if (controlData.activeChannels != lastActiveChannels) {
                lastActiveChannels = controlData.activeChannels;
                lcd.setCursor(5, 2);
                displayPrintActiveChannels();
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
                displayPrintChargingMode();
                lcd.print("      ");
                lastChannelChargingMode = controlData.channelChargingMode[displayData.editedChannel];
                lastChannelChargingCurrent = controlData.channelChargingCurrent[displayData.editedChannel];
            }
            if (displayData.editedChannelSetting != lastEditedChannelSetting) {
                lastEditedChannelSetting = displayData.editedChannelSetting;
                lcd.setCursor(0, 1);
                displayUpdateSelectionIndicator(displayData.editedChannelSetting == MODE_CHANNEL_SETTING);
                lcd.setCursor(0, 2);
                displayUpdateSelectionIndicator(displayData.editedChannelSetting == CHARGING_CURRENT_CHANNEL_SETTING);
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
            break;

        default:
            break;
    }
}