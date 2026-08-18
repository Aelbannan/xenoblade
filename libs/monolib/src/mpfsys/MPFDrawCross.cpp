#include "monolib/mpfsys/MPFDrawCross.hpp"

// Retail singleton state lives in the shared retail data (monolibdata2.s):
// lbl_eu_806658A8 = init flag (.sbss), lbl_eu_806658AC = instance pointer
// (.sbss), lbl_eu_8056DC28 = prototype object (.data). Declaration only so
// this TU emits no .bss/.sbss.
extern "C" {
    extern u32 lbl_eu_806658A8;
    extern void* lbl_eu_806658AC;
}

struct MPFDrawCross_Prototype {
    mpfsys::MPFDrawCross obj;
    u32 padding[4];
};

extern "C" MPFDrawCross_Prototype lbl_eu_8056DC28;

namespace mpfsys {

MPFDrawCross* MPFDrawCross::getInstance(){
    if(!lbl_eu_806658A8){
        lbl_eu_806658AC = &lbl_eu_8056DC28.obj;
        lbl_eu_806658A8 = 1;
    }
    return (mpfsys::MPFDrawCross*)&lbl_eu_806658AC;
}

} // namespace mpfsys

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.sbss] 0x806658A8-0x806658B0 (8B) zero-fill
u32 lbl_eu_806658A8;            // sbss +0..+3 (init flag + pad)
void* lbl_eu_806658AC;           // sbss +4..+7 (instance pointer)
