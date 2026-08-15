#include "monolib/work.hpp"

// Retail sbss singleton slot (config symbols.txt lbl_eu_80665600; 8 bytes:
// word 0 = singleton pointer, word 1 = pad). Defined here so this TU owns
// its data (blob monolibdata1d dissolve).
CWorkFlowWiiPowerOff* lbl_eu_80665600[2];

/**
 * Returns the singleton instance of the Wii Power Off workflow thread.
 */
CWorkFlowWiiPowerOff* CWorkFlowWiiPowerOff::getInstance(){
    return lbl_eu_80665600[0];
}
