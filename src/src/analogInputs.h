#ifndef _ANALOG_INPUTS_H_
#define _ANALOG_INPUTS_H_

#include <Arduino.h>

#define CELL_1_VOLTAGE_INPUT A0
#define CELL_2_VOLTAGE_INPUT A1
#define CELL_3_VOLTAGE_INPUT A2
#define CELL_4_VOLTAGE_INPUT A3
#define CELL_5_VOLTAGE_INPUT A4
#define CELL_6_VOLTAGE_INPUT A5

#define CELL_1_CURRENT_INPUT A6
#define CELL_2_CURRENT_INPUT A7
#define CELL_3_CURRENT_INPUT A8
#define CELL_4_CURRENT_INPUT A9
#define CELL_5_CURRENT_INPUT A10
#define CELL_6_CURRENT_INPUT A11

void analogInputsRead(void);

#endif  // ANALOG_INPUTS_H

jak napiece jest powyzej 4,7 to bateria uszkodzona , zwarta ()
jak 3,5 do 4,7 to normanie rozladowana bateia i mozemy ladowac
jak 0.3 do 3.5 to normalne ladowanie 
