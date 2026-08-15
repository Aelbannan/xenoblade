#include "monolib/work.hpp"

// Retail singleton slot (sbss lbl_eu_806655F8); owned by the retail data
// blob (monolibdata*.s), so this TU declares it extern and stays data-free.
extern "C" CWorkFlowWiiReset* lbl_eu_806655F8;

CWorkFlowWiiReset* CWorkFlowWiiReset::getInstance(){
    return lbl_eu_806655F8;
}
