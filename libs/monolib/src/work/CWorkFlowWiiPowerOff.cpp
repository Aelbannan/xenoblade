#include "monolib/work.hpp"

// Retail sbss singleton slot (config symbols.txt lbl_eu_80665600; the retail
// TU references it as a flat address label, so declare the typed extern with
// that name instead of defining spInstance locally — the definition lives in
// the retail data object).
extern "C" CWorkFlowWiiPowerOff* lbl_eu_80665600;

/**
 * Returns the singleton instance of the Wii Power Off workflow thread.
 */
CWorkFlowWiiPowerOff* CWorkFlowWiiPowerOff::getInstance(){
    return lbl_eu_80665600;
}
