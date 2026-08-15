#include "monolib/work.hpp"

// Retail sbss singleton slot (config symbols.txt lbl_eu_806655F8; 8 bytes:
// word 0 = singleton pointer, word 1 = pad). Defined here so this TU owns
// its data (blob monolibdata1d dissolve).
CWorkFlowWiiReset* lbl_eu_806655F8[2];

CWorkFlowWiiReset* CWorkFlowWiiReset::getInstance(){
    return lbl_eu_806655F8[0];
}
