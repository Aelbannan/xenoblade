#include "monolib/mpfsys/MPFDrawCross.hpp"

// Retail singleton state lives in the shared retail data (monolibdata2.s):
// lbl_eu_806658A8 = init flag (.sbss), lbl_eu_806658AC = instance pointer
// (.sbss).  Declaration only so this TU emits no .bss/.sbss.
extern "C" {
    extern s8 lbl_eu_806658A8;
    extern void* lbl_eu_806658AC;
}

// The retail prototype is the compiler-emitted vtable of the dispatcher.
extern "C" u32 __vt__Q26mpfsys12MPFDrawCross[4];

namespace mpfsys {

MPFDrawCross* MPFDrawCross::getInstance(){
    if(!lbl_eu_806658A8){
        lbl_eu_806658AC = (mpfsys::MPFDrawCross*)&__vt__Q26mpfsys12MPFDrawCross;
        lbl_eu_806658A8 = 1;
    }
    return (mpfsys::MPFDrawCross*)&lbl_eu_806658AC;
}

} // namespace mpfsys

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.sbss] 0x806658A8-0x806658B0 (8B) zero-fill
s8 lbl_eu_806658A8;             // sbss +0 (init flag; +1..+3 align pad)
void* lbl_eu_806658AC;           // sbss +4..+7 (instance pointer)
