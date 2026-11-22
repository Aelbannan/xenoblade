#include "monolib/work.hpp"

CWorkFlowWiiPowerOff* CWorkFlowWiiPowerOff::spInstance;

/**
 * Returns the singleton instance of the Wii Power Off workflow thread.
 */
CWorkFlowWiiPowerOff* CWorkFlowWiiPowerOff::getInstance(){
    return spInstance;
}
