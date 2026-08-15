#include "monolib/work.hpp"

// Retail sbss singleton slot lbl_eu_806655F0 (8 bytes: word 0 = singleton
// pointer, word 1 = pad). Defined here so this TU owns its data (blob
// monolibdata1d dissolve). The retail references it as a flat address label,
// so the definition uses the retail name rather than the member name.
CWorkFlowWiiMenu* lbl_eu_806655F0[2];

CWorkFlowWiiMenu* CWorkFlowWiiMenu::getInstance(){
    return lbl_eu_806655F0[0];
}
