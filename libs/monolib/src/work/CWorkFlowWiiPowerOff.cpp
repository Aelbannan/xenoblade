#include "monolib/work.hpp"

// Static singleton instance; zero-initialized (BSS)
CWorkFlowWiiPowerOff* CWorkFlowWiiPowerOff::spInstance;

/**
 * Returns the singleton instance of the Wii Power Off workflow thread.
 */
CWorkFlowWiiPowerOff* CWorkFlowWiiPowerOff::getInstance(){
    return spInstance;
}
