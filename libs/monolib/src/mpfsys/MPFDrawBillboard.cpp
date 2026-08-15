#include "monolib/mpfsys/MPFDrawBillboard.hpp"

extern "C" {
    extern s8 lbl_eu_806658A0;
    extern mpfsys::MPFDrawBillboard* lbl_eu_806658A4;
}

struct MPFDrawBillboard_Prototype {
    mpfsys::MPFDrawBillboard obj;
    u32 padding[4];
};

// Retail-owned prototype storage (bss lbl_eu_8056DC48, in the retail data
// blob); declared extern so this TU defines no data.
extern MPFDrawBillboard_Prototype lbl_eu_8056DC48;

namespace mpfsys {

MPFDrawBillboard* MPFDrawBillboard::getInstance(){
    if(!lbl_eu_806658A0){
        lbl_eu_806658A4 = &lbl_eu_8056DC48.obj;
        lbl_eu_806658A0 = 1;
    }
    return (mpfsys::MPFDrawBillboard*)&lbl_eu_806658A4;
}

} // namespace mpfsys
