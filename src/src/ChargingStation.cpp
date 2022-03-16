#include <FastX9CXXX.h>  //DO OBSŁUGI POTENCJOMETRU  ELEKTRONICZNEGO
#include "display.h"
#include "timer.h"
#include "buttons.h"
#include "menuControl.h"
#include "analogInputs.h"

// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
unsigned short batTestVolt_1 = 0, batTestVolt_2 = 0, batTestVolt_3 = 0, batTestVolt_4 = 0, batTestVolt_5 = 0,
               batTestVolt_6 = 0;  // zmienne do przechowywania warosci napiec podlaczoego aku zakres od 0 do 1024
unsigned short batSetCurr_1 = 0, batSetCurr_2 = 0, batSetCurr_3 = 0, batSetCurr_4 = 0, batSetCurr_5 = 0,
               batSetCurr_6 = 0;  // zmienne do ustawiania pradu ladowania NiMH zakres od 0 do 1024
bool niMh_1 = 0, niMh_2 = 0, niMh_3 = 0, niMh_4 = 0, niMh_5 = 0,
     niMh_6 = 0;  // zmienna 0 lub 1 przelaczanie przekaznika ladowania niMh
bool LiPo_1 = 0, LiPo_2 = 0, LiPo_3 = 0, LiPo_4 = 0, LiPo_5 = 0,
     LiPo_6 = 0;  // zmienna 0 lub 1 przelaczanie przekaznika ladowania liPo
float batTestVolt_V1 = 0, batTestVolt_V2 = 0, batTestVolt_V3 = 0, batTestVolt_V4 = 0, batTestVolt_V5 = 0,
      batTestVolt_V6 =
          0;  // zmienna przechowująca przeliczone na [V] napięcia z pinów analogowych do wyswietlenia na interface
unsigned short loadCurr1 = 0, loadCurr2 = 0, loadCurr3 = 0, loadCurr4 = 0, loadCurr5 = 0,
               loadCurr6 = 0;  // zmienna do przechowywania pradu zmierzonego ładującego aku (do regulacji przetwornicy)
unsigned long aktualnyCzas = 0;         // licznik czasu mills()
unsigned long zapamietanyCzasLED1 = 0;  // licznik czasu mills()
unsigned short currentTemp = 0;         // sprawadzanie temperatury urzadzenia
float currentTemp_1 = 0;                // przeliczona na stopnie temperatura otoczenia na potrzeby wyswietlania
int workingPath = 0, testVolt = 0;      // workingpatch to zminenna zawierająca informacje o wybranym torze

bool niMh = 1, LiPo = 0;  // zmienne przechowujące wybrany przez użytkonika tor ładowania
// zdefniwac przy motnazu!
unsigned short R1 = 0;  // rezystancja do resystora do pomiaru temeperatury w obudowie (devTemp) Zdefiniować po
                        // dobraniu!

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
int AlarmLED_1_RED = 53;
int workingLED_1_GREEN = 52;
int AlarmLED_2_RED = 51;
int workingLED_2_GREEN = 50;
int AlarmLED_3_RED = 49;
int workingLED_3_GREEN = 48;
int AlarmLED_4_RED = 47;
int workingLED_4_GREEN = 46;
int AlarmLED_5_RED = 45;
int workingLED_5_GREEN = 44;
int AlarmLED_6_RED = 43;
int workingLED_6_GREEN = 42;

// Piny przerwań
// int Inter_1 =21
// int Inter_2 =20
// int Inter_3 =19
// int Inter_4 =18

// Zmienne pamiętające nastawy prądów/napięć/ itp

void timerProcess(void) {
    buttonsReadPinsAndFilter();
    displayMenuTimeouts();
}

void setup() {
    // Przerwania wylaczone na chwile bo nie wiem czy dobrze
    // attachInterrupt(digitalPinToInterrupt(21), menu, RISING); //po wciśnnieciu pinu 21(Inter_1) idz do funkcji
    // obslugi menu attachInterrupt(digitalPinToInterrupt(20), overHeat, RISING); //stan alarmowy przegrzanie ladowarki

    Serial.begin(115200);  // Uruchomienie komunikacji
    Serial.println("Turn ON Charging Station...");

    buttonsInit();
    displayInit();
    initTimer1(timerProcess);
}
// funkcja realizowana w przerwaniu (po wcisnieciu przycisku menu na klawiatrurze - podlaczonego do pinu 22

/*
void menu() {
    if (isInLowerLevel) {
    lcd.print(menu[currentMenuPos].label);
    lcd.setCursor(0, 1);
    lcd.print(F("> "));

    if (menu[currentMenuPos].handler != NULL) {
      (*(menu[currentMenuPos].handler))();
    } else {
      lcd.print(tempVal);
    }
  } else {
    lcd.print(F("Menu glowne"));
    lcd.setCursor(0, 1);
    lcd.print(F("> "));

    lcd.print(menu[currentMenuPos].label);
  }
}
void overHeat() {
  //funkja obsługi przegrzania
}
*/

void LiPoLOAD(int workingPath) {
    workingPath = 1;  // na chwilę żeby coś zwracać
    Serial.println((String) "pracuje tor NiMh" + (workingPath));
    //Ładowanie akumulatora LiPo
}

void NiMhLOAd(int workingPath) {
    //ładowanie akumulatora NiMH
    workingPath = 1;  // na chwilę żeby coś tu jednak było - w założeniu to będzie parametr z menu, która wskaże który
                      // tor ładowania ma być uruchomiony

    // Ustawianie prądu ładowania
    Serial.println((String) "pracuje tor LiPo" + (workingPath));
}

void BatFailure(int workingPath) {
    // co zrobić jak błąd baterii
    workingPath = 1;  // na chwilę żeby coś zwracać
    Serial.println((String) "błąd Baterii na torze" + (workingPath));
}

void redLedBlink(int ledNr, int ledFreq) {
    int stanLed = 1;
    switch (ledFreq) {
        case 1:

            aktualnyCzas = millis();

            if (aktualnyCzas - zapamietanyCzasLED1 >= 1000UL)  // migamy diodą przy pomocy miills()
            {
                zapamietanyCzasLED1 = aktualnyCzas;  // Zapamietaj aktualny czas
                stanLed = !stanLed;                  // Zmieniamy stan diody na przeciwny
                digitalWrite(stanLed, ledNr);  // ustawiamy nowy stan na diodzie  ledNr podanej jako parametr funkcji
            }

        case 2:

            aktualnyCzas = millis();

            if (aktualnyCzas - zapamietanyCzasLED1 >= 500UL)  // migamy diodą przy pomocy miills()
            {
                zapamietanyCzasLED1 = aktualnyCzas;  // Zapamietaj aktualny czas
                stanLed = !stanLed;                  // Zmieniamy stan diody na przeciwny
                digitalWrite(stanLed, ledNr);  // ustawiamy nowy stan na diodzie  ledNr podanej jako parametr funkcji
            }
    }
}

void batTest() {
    // funkcja sprawdzająca stan akumulatora podłączonego do portu ładowania. testVolt - to wartość zmierzona w zmiennej
    // batTestVolt_?, NiMH, LiPo - stan toru ładowania 0/1 czy włączony czy wyłączony, AlarmLED_RED - zapalanie diody
    // alarmowej, workingLED_GREEN - zapalanie diody pracy
    switch (workingPath)  // sprawdzanie która ścieżka ładowania jest używana - generalnie zmienna workingPath powinna
                          // pochodzić z menu urządzenia
    {
        case 1:  // przypadek pierwszy - workingPath ma wartość 1 to sprawdzamy wszystko na torze 1
            testVolt =
                testVolt_1;  // zmienna testVolt równa jest wartości z przetwornika A/C zapisanej z pinu testVolt_1
                             // zadeklaerowanego na początku kodu nie przeliczam tego specjalnie na volty
            niMh = niMh_1;   // znowu w tym kejsie przypisanie do zmiennej niMh wartości zmiennje bool (0 albo 1) ze
                             // zmiennje niMh_1 - która też powinna pochodzić z menu
            LiPo = LiPo_1;   // uwaga jak wyżej
            switch (niMh)    // sprawdzamy stan zmiennej niMh
            {
                case 0:  // kiedy tor niMh jest zerem - czyli wyłączony

                    switch (LiPo)  // sprawdz stan zmiennje LiPo
                    {
                        case 0:  // jak ona też jest wyłączona
                            Serial.println(
                                (String) "Ostrzeżenie nie wybrano żadnego toru ładowania!");  // napisz na porcie
                                                                                              // szeregowym
                            redLedBlink(1, 1);  // wywołaj funkcję redLedBlink z parametrem 1

                        case 1:  // jeżeli jednak tor LiPo jest wybrany
                            Serial.println((String) "Ładowanie LiPo" + (workingPath) +
                                           "!");  // to napisz na porcie szeregowym
                            LiPoLOAD(1);          // wywołaj funkcję LiPoLOAD z parametrem 1
                    }
                case 1:                  // jeżeli wybrano tor NiMH
                    if (testVolt < 566)  // sprawdź jakie jest napięcie na akumulatorze i jeżeli jest mniejsze niż 566
                                         // (czyli około 4,7V)
                    {
                        redLedBlink(1, 2);  // to weź zamigaj diodą przy pomocy funkcji redLedBlink
                        BatFailure(1);      // a potem wykonaj funkcję obsługującą błąd baterii
                    }

                    else  // jak wartość testVolt jest inna niż 566 to
                    {
                        NiMhLOAd(1);  // uruchom funkcję ładowania baterii NiMH z parametrem 1
                    }
            }

        case 2:
            testVolt = testVolt_2;
            niMh = niMh_2;  // znowu w tym kejsie przypisanie do zmiennej niMh wartości zmiennje bool (0 albo 1) ze
                            // zmiennje niMh_1 - która też powinna pochodzić z menu
            LiPo = LiPo_2;  // uwaga jak wyżej
            switch (niMh)   // sprawdzamy stan zmiennej niMh
            {
                case 0:  // kiedy tor niMh jest zerem - czyli wyłączony

                    switch (LiPo)  // sprawdz stan zmiennje LiPo
                    {
                        case 0:  // jak ona też jest wyłączona
                            Serial.println(
                                (String) "Ostrzeżenie nie wybrano żadnego toru ładowania!");  // napisz na porcie
                                                                                              // szeregowym
                            redLedBlink(2, 1);  // wywołaj funkcję redLedBlink z parametrem linia 2, miganie wariant 1

                        case 1:  // jeżeli jednak tor LiPo jest wybrany
                            Serial.println((String) "Ładowanie LiPo" + (workingPath) +
                                           "!");  // to napisz na porcie szeregowym
                            LiPoLOAD(2);          // wywołaj funkcję LiPoLOAD z parametrem 1
                    }
                case 1:                  // jeżeli wybrano tor NiMH
                    if (testVolt < 566)  // sprawdź jakie jest napięcie na akumulatorze i jeżeli jest mniejsze niż 566
                                         // (czyli około 4,7V)
                    {
                        redLedBlink(2, 2);  // to weź zamigaj diodą przy pomocy funkcji redLedBlink
                        BatFailure(1);      // a potem wykonaj funkcję obsługującą błąd baterii
                    }

                    else  // jak wartość testVolt jest inna niż 566 to
                    {
                        NiMhLOAd(2);  // uruchom funkcję ładowania baterii NiMH z parametrem 1
                    }
            }

        case 3:
            niMh = niMh_3;  // znowu w tym kejsie przypisanie do zmiennej niMh wartości zmiennje bool (0 albo 1) ze
                            // zmiennje niMh_1 - która też powinna pochodzić z menu
            LiPo = LiPo_3;  // uwaga jak wyżej
            testVolt = testVolt_3;
            switch (niMh)  // sprawdzamy stan zmiennej niMh
            {
                case 0:  // kiedy tor niMh jest zerem - czyli wyłączony

                    switch (LiPo)  // sprawdz stan zmiennje LiPo
                    {
                        case 0:  // jak ona też jest wyłączona
                            Serial.println(
                                (String) "Ostrzeżenie nie wybrano żadnego toru ładowania!");  // napisz na porcie
                                                                                              // szeregowym
                            redLedBlink(3, 1);  // wywołaj funkcję redLedBlink z parametrem linia 2, miganie wariant 1

                        case 1:  // jeżeli jednak tor LiPo jest wybrany
                            Serial.println((String) "Ładowanie LiPo" + (workingPath) +
                                           "!");  // to napisz na porcie szeregowym
                            LiPoLOAD(3);          // wywołaj funkcję LiPoLOAD z parametrem 1
                    }
                case 1:                  // jeżeli wybrano tor NiMH
                    if (testVolt < 566)  // sprawdź jakie jest napięcie na akumulatorze i jeżeli jest mniejsze niż 566
                                         // (czyli około 4,7V)
                    {
                        redLedBlink(3, 2);  // to weź zamigaj diodą przy pomocy funkcji redLedBlink
                        BatFailure(1);      // a potem wykonaj funkcję obsługującą błąd baterii
                    }

                    else  // jak wartość testVolt jest inna niż 566 to
                    {
                        NiMhLOAd(3);  // uruchom funkcję ładowania baterii NiMH z parametrem 1
                    }
            }

        case 4:
            testVolt = testVolt_4;
            niMh = niMh_4;  // znowu w tym kejsie przypisanie do zmiennej niMh wartości zmiennje bool (0 albo 1) ze
                            // zmiennje niMh_1 - która też powinna pochodzić z menu
            LiPo = LiPo_4;  // uwaga jak wyżej
            switch (niMh) {
                case 0:

                    switch (LiPo)  // sprawdz stan zmiennje LiPo
                    {
                        case 0:  // jak ona też jest wyłączona
                            Serial.println(
                                (String) "Ostrzeżenie nie wybrano żadnego toru ładowania!");  // napisz na porcie
                                                                                              // szeregowym
                            redLedBlink(4, 1);  // wywołaj funkcję redLedBlink z parametrem linia 2, miganie wariant 1

                        case 1:  // jeżeli jednak tor LiPo jest wybrany
                            Serial.println((String) "Ładowanie LiPo" + (workingPath) +
                                           "!");  // to napisz na porcie szeregowym
                            LiPoLOAD(4);          // wywołaj funkcję LiPoLOAD z parametrem 1
                    }
                case 1:                  // jeżeli wybrano tor NiMH
                    if (testVolt < 566)  // sprawdź jakie jest napięcie na akumulatorze i jeżeli jest mniejsze niż 566
                                         // (czyli około 4,7V)
                    {
                        redLedBlink(4, 2);  // to weź zamigaj diodą przy pomocy funkcji redLedBlink
                        BatFailure(1);      // a potem wykonaj funkcję obsługującą błąd baterii
                    }

                    else  // jak wartość testVolt jest inna niż 566 to
                    {
                        NiMhLOAd(4);  // uruchom funkcję ładowania baterii NiMH z parametrem 1
                    }
            }
        case 5:
            testVolt = testVolt_5;
            niMh = niMh_5;  // znowu w tym kejsie przypisanie do zmiennej niMh wartości zmiennje bool (0 albo 1) ze
                            // zmiennje niMh_1 - która też powinna pochodzić z menu
            LiPo = LiPo_5;  // uwaga jak wyżej
            switch (niMh) {
                case 0:

                    switch (LiPo)  // sprawdz stan zmiennje LiPo
                    {
                        case 0:  // jak ona też jest wyłączona
                            Serial.println(
                                (String) "Ostrzeżenie nie wybrano żadnego toru ładowania!");  // napisz na porcie
                                                                                              // szeregowym
                            redLedBlink(5, 1);  // wywołaj funkcję redLedBlink z parametrem linia 2, miganie wariant 1

                        case 1:  // jeżeli jednak tor LiPo jest wybrany
                            Serial.println((String) "Ładowanie LiPo" + (workingPath) +
                                           "!");  // to napisz na porcie szeregowym
                            LiPoLOAD(5);          // wywołaj funkcję LiPoLOAD z parametrem 1
                    }
                case 1:                  // jeżeli wybrano tor NiMH
                    if (testVolt < 566)  // sprawdź jakie jest napięcie na akumulatorze i jeżeli jest mniejsze niż 566
                                         // (czyli około 4,7V)
                    {
                        redLedBlink(5, 2);  // to weź zamigaj diodą przy pomocy funkcji redLedBlink
                        BatFailure(1);      // a potem wykonaj funkcję obsługującą błąd baterii
                    }

                    else  // jak wartość testVolt jest inna niż 566 to
                    {
                        NiMhLOAd(5);  // uruchom funkcję ładowania baterii NiMH z parametrem 1
                    }
            }
        case 6:
            testVolt = testVolt_6;
            niMh = niMh_6;  // znowu w tym kejsie przypisanie do zmiennej niMh wartości zmiennje bool (0 albo 1) ze
                            // zmiennje niMh_1 - która też powinna pochodzić z menu
            LiPo = LiPo_6;  // uwaga jak wyżej
            switch (niMh) {
                case 0:

                    switch (LiPo)  // sprawdz stan zmiennje LiPo
                    {
                        case 0:  // jak ona też jest wyłączona
                            Serial.println(
                                (String) "Ostrzeżenie nie wybrano żadnego toru ładowania!");  // napisz na porcie
                                                                                              // szeregowym
                            redLedBlink(6, 1);  // wywołaj funkcję redLedBlink z parametrem linia 2, miganie wariant 1

                        case 1:  // jeżeli jednak tor LiPo jest wybrany
                            Serial.println((String) "Ładowanie LiPo" + (workingPath) +
                                           "!");  // to napisz na porcie szeregowym
                            LiPoLOAD(6);          // wywołaj funkcję LiPoLOAD z parametrem 1
                    }
                case 1:                  // jeżeli wybrano tor NiMH
                    if (testVolt < 566)  // sprawdź jakie jest napięcie na akumulatorze i jeżeli jest mniejsze niż 566
                                         // (czyli około 4,7V)
                    {
                        redLedBlink(6, 2);  // to weź zamigaj diodą przy pomocy funkcji redLedBlink
                        BatFailure(1);      // a potem wykonaj funkcję obsługującą błąd baterii
                    }

                    else  // jak wartość testVolt jest inna niż 566 to
                    {
                        NiMhLOAd(6);  // uruchom funkcję ładowania baterii NiMH z parametrem 1
                    }
            }
    }
}

void loop() {
    readButtons();
    analogInputsRead();
    menuControlHandler();
    // processControlHandler();
    displayMenuHandler();

    //     Serial.println((String) "editedChannelSetting " + (displayData.editedChannelSetting));

    // odczytujemy napięcia na akumulatorze podłączonym do złącza - i przepisujemy je do zmiennej batTestVolt_X
    // batTestVolt_1 =
    //     analogRead(testVolt_1) * 0.0083 * 2;  // mnożymy odczyt analogowy przez 0.0083 - żeby zamienić na Volty a
    //     potem
    //                                           // jeszcze razy 2 bo pomiar realizowany jest przez dzielnik
    //                                           dopasowujący
    //                                           // zakres napięcia mierzonego (  max 8,5V) do zakresu Arduino - czyli
    //                                           5V
    // batTestVolt_2 = analogRead(testVolt_2) * 0.0083 * 2;
    // batTestVolt_3 = analogRead(testVolt_3) * 0.0083 * 2;
    // batTestVolt_4 = analogRead(testVolt_4) * 0.0083 * 2;
    // batTestVolt_5 = analogRead(testVolt_5) * 0.0083 * 2;
    // batTestVolt_6 = analogRead(testVolt_6) * 0.0083 * 2;

    // // menu();
    // batTest();
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