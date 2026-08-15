#include "monolib/work.hpp"

// Retail sbss singleton slot (config symbols.txt lbl_eu_806655E8; the retail
// TU references it as a flat address label, so declare the typed extern with
// that name instead of defining spInstance locally — the definition lives in
// the retail data object).
extern "C" CWorkFlowShutdownAll* lbl_eu_806655E8;

CWorkFlowShutdownAll* CWorkFlowShutdownAll::getInstance(){
    return lbl_eu_806655E8;
}
