#include <analogInputs.h>
#include "processControl.h"

void analogInputsRead(void) {
    for (int i = 0; i < CHANNELS_QUANTITY; i++) {
        controlData.cellVoltage[i] = 0.0049 * analogRead(CELL_1_VOLTAGE_INPUT + 1);
        controlData.cellCurrent[i] = 0.0049 * analogRead(CELL_1_CURRENT_INPUT + 1);
    }
}
