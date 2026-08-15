#include "monolib/work.hpp"

// Retail sbss singleton slot (config symbols.txt lbl_eu_806655E8; 8 bytes:
// word 0 = singleton pointer, word 1 = pad). Defined here so this TU owns
// its data (blob monolibdata1d dissolve).
CWorkFlowShutdownAll* lbl_eu_806655E8[2];

CWorkFlowShutdownAll* CWorkFlowShutdownAll::getInstance(){
    return lbl_eu_806655E8[0];
}
