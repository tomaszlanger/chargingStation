#include <FastX9CXXX.h>  //DO OBSŁUGI POTENCJOMETRU  ELEKTRONICZNEGO
#include "display.h"
#include "timer.h"
#include "buttons.h"
#include "menuControl.h"
#include "analogInputs.h"
#include "processControl.h"
#include "outputs.h"

// MENU

// Ekran1
#define Ekran1_1 = "Ładowarka"
#define Ekran1_2 = "Akumulatorow"
#define Ekran1_3 = "WIEA UZ 2022"

// Ekran2
#define Ekran2_1 = "Wybierz opcje:"
#define Ekran2_2 = "Ladowanie AKU"
#define Ekran2_3 = "TESTY"

// Ekran3 - wybór powoduje zapis zmiennej workingPath stosownej wartości (od 1 do 6) żeby można było zdefiniować tor
// ładowania
#define Ekran3_1 = "Ladowanie AKU:"
#define Ekran3_2 = "WYBIERZ TOR:"
#define Ekran3_3 = "1"
#define Ekran3_4 = "2"
#define Ekran3_5 = "3"
#define Ekran3_6 = "4"
#define Ekran3_7 = "5"
#define Ekran3_8 = "6"

// Ekran4
#define Ekran4_1 = "Jaki Akumulator?"
#define Ekran4_2 \
    = "Tor" workingPath    // tu wyświetlamy wartość zmiennej workingPath zeby było wiadomo co się
                           // konfiguruje
#define Ekran4_3 = "LiPo"  // jak wybiream lipo to do zmiennej lipo zapisuję 1
#define Ekran4_4 = "NiMH"  // jak wybiream nimh to do zmiennej nimh zapisuję 1

// Ekran5 - jak NiMH
#define Ekran5_1 = "Jaki prad ladowania?"
#define Ekran5_2 = "0,5 A"
#define Ekran5_3 = "1,0 A"
#define Ekran5_4 = "1,5 A"
#define Ekran5_5 = "2,0 A"

// Ekran6 - jak NiMH po wyborze prądu
#define Ekran6_1 = "Czy zaczynac ladowanie?"
#define Ekran6_2 = "TAK"
#define Ekran6_3 = "NIE"
#define Ekran6_4 = "UWAGA! PODLACZ AKU!"

// Ekran7 - jak LiPO
#define Ekran7_1 = "Czy zaczynac ladowanie?"
#define Ekran7_2 = "TAK"
#define Ekran7_3 = "NIE"
#define Ekran7_4 = "UWAGA! PODALACZ AKU!"

// Ekran8 KONIEC ŁADOWANIA W TORZE: workingPath
#define Ekran8_1 = "Koniec Ladowania:"
#define Ekran8_2 = "w torze:"
#define Ekran8_3 = workingPath
#define Ekran8_4 = "UWAGA! ODLACZ AKU!"

// Ekran9 TESTY
#define Ekran9_1 = "JAKI TEST? - TOR 1!"
#define Ekran9_2 = "Napięcie AKU"
#define Ekran9_3 = "Temp komory AKU"
#define Ekran9_4 = currentTemp

// definicja pinow analogowych - pomiary napiecia baterii zapis do zmiennej batTestVolt_x
int testVolt_1 = A0;
int testVolt_2 = A1;
int testVolt_3 = A2;
int testVolt_4 = A3;
int testVolt_5 = A4;
int testVolt_6 = A5;
// definicnaj pinow analogowych pomiary pradu (zapis do zmiennej loadCurrX)
int testCurr_1 = A6;
int testCurr_2 = A7;
int testCurr_3 = A8;
int testCurr_4 = A9;
int testCurr_5 = A10;
int testCurr_6 = A11;

int devTemp = A12;  // do podłącznenia termistora

// definicja pinwów cyfrowych tor ładowania NiMH_x
int niMh_path_1 = 22;
int niMh_path_2 = 23;
int niMh_path_3 = 24;
int niMh_path_4 = 25;
int niMh_path_5 = 26;
int niMh_path_6 = 27;

// definicja pinwów cyfrowych tor ładowania LiPo
int LiPo_path_1 = 28;
int LiPo_path_2 = 29;
int LiPo_path_3 = 30;
int LiPo_path_4 = 31;
int LiPo_path_5 = 32;
int LiPo_path_6 = 33;

// Sterowanie pinami nastawiania prądu (do regulacji przy pomocy potencjomentru elektornicznego)
int sterCurr_1_UP = 41;
int sterCurr_1_DOWN = 40;
int sterCurr_2_UP = 39;
int sterCurr_2_DOWN = 38;
int sterCurr_3_UP = 37;
int sterCurr_3_DOWN = 36;
int sterCurr_4_UP = 35;
int sterCurr_4_DOWN = 34;

// brakuje pinu do SS przy komunikacji SPI (komunikacja SlaveSelect) i trzeba chyba użyć pinu jednego ze stwerowania
// prądem ładowania) pewnie trezba będzie uciąć dwa tory ładownaia i tam zostawić ładowanie tylko prądem na sztywno
// ustawionym (czyli okoł 4A)) dla LiPo nie ma problemu - ładowanie NiMh takim prądem będzie po prostu "szybkie")
int sterCurr_5_UP = 13;
int sterCurr_5_DOWN = 12;
int sterCurr_6_UP = 11;
int sterCurr_6_DOWN = 10;

// piny led do wskazania pracy tory ładowania
int workingLED_1_GREEN = 52;
int workingLED_2_GREEN = 50;
int workingLED_3_GREEN = 48;
int workingLED_4_GREEN = 46;
int workingLED_5_GREEN = 44;
int workingLED_6_GREEN = 42;

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

// if (buttons.prev) {
//     // if (controlData.channelChargingCurrent[displayData.editedChannel] > 0)
//     //     controlData.channelChargingCurrent[displayData.editedChannel]--;
//     // if (controlData.channelChargingMode[displayData.editedChannel] > 0)
//     //     controlData.channelChargingMode[displayData.editedChannel]--;
//     // if (controlData.channelChargingState[displayData.editedChannel] > 0)
//     // controlData.channelChargingState[displayData.editedChannel]--;
//     // if (controlData.cellCurrent[displayData.editedChannel] >= 0.5) {
//     //     controlData.cellCurrent[displayData.editedChannel] -= 0.5;
//     //     displayData.updateCellsReadouts = true;
//     // }
//     // if (controlData.cellVoltage[displayData.editedChannel] >= 0.5) {
//     //     controlData.cellVoltage[displayData.editedChannel] -= 0.5;
//     //     displayData.updateCellsReadouts = true;
//     // }
// }
// if (buttons.set) {
//     displayData.menuIndex = CHANNEL_SETTINGS_SCREEN;
//     displayData.editedChannelSetting = MODE_CHANNEL_SETTING;
//     // if (controlData.channelChargingCurrent[displayData.editedChannel] < A_2_0_CHARGING_CURRENT)
//     //     controlData.channelChargingCurrent[displayData.editedChannel]++;
//     // if (controlData.channelChargingMode[displayData.editedChannel] < NIMH_MODE)
//     //     controlData.channelChargingMode[displayData.editedChannel]++;
//     // if (controlData.channelChargingState[displayData.editedChannel] < FINISHED_CHARGING_STATE)
//     //     controlData.channelChargingState[displayData.editedChannel]++;
//     // if (controlData.cellCurrent[displayData.editedChannel] < 15) {
//     //     controlData.cellCurrent[displayData.editedChannel] += 0.5;
//     //     displayData.updateCellsReadouts = true;
//     // }
//     // if (controlData.cellVoltage[displayData.editedChannel] < 15) {
//     //     controlData.cellVoltage[displayData.editedChannel] += 0.5;
//     //     displayData.updateCellsReadouts = true;
//     // }
// }
