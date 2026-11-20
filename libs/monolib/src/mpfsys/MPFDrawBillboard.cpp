#include "monolib/mpfsys/MPFDrawBillboard.hpp"

extern "C" {
    s8 lbl_eu_806658A0;   // .sbss word 0 (monolibdata2) -- now owned by this TU
    mpfsys::MPFDrawBillboard* lbl_eu_806658A4;   // .sbss word 1
}

// The retail prototype is the compiler-emitted vtable of the dispatcher.
extern "C" u32 __vt__Q26mpfsys16MPFDrawBillboard[4];

namespace mpfsys {

MPFDrawBillboard* MPFDrawBillboard::getInstance(){
    if(!lbl_eu_806658A0){
        lbl_eu_806658A4 = (mpfsys::MPFDrawBillboard*)&__vt__Q26mpfsys16MPFDrawBillboard;
        lbl_eu_806658A0 = 1;
    }
    return (mpfsys::MPFDrawBillboard*)&lbl_eu_806658A4;
}

} // namespace mpfsys
