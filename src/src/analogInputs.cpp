#include "analogInputs.h"
#include "processControl.h"

void analogInputsRead(void) {
    for (uint8_t i = 0; i < CHANNELS_QUANTITY; i++) {
        controlData.cellVoltage[i] = ACD_INPUTS_TO_VOLTS(analogRead(CELL_1_VOLTAGE_INPUT + i));
        controlData.cellCurrent[i] = ACD_INPUTS_TO_AMPERS(analogRead(CELL_1_CURRENT_INPUT + i));
        if (controlData.cellCurrent[i] < 0) controlData.cellCurrent[i] = 0;
    }
}
