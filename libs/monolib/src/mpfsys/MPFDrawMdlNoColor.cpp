#include "monolib/mpfsys/MPFDrawMdlNoColor.hpp"

extern "C" {
    s8 lbl_eu_80665890;   // .sbss word 0 (monolibdata2) -- now owned by this TU
    mpfsys::MPFDrawMdlNoColor* lbl_eu_80665894;   // .sbss word 1
    // Prototype instance storage ships from the retail data unit (monolibdata2).
    // Array-typed extern so MWCC emits the retail lis/addi (HA/LO) addressing
    // instead of sda21 for this .data object.
    extern u32 __vt__Q26mpfsys17MPFDrawMdlNoColor[4];
}

namespace mpfsys {

MPFDrawMdlNoColor* MPFDrawMdlNoColor::getInstance(){
    if(!lbl_eu_80665890){
        lbl_eu_80665894 = (mpfsys::MPFDrawMdlNoColor*)&__vt__Q26mpfsys17MPFDrawMdlNoColor;
        lbl_eu_80665890 = 1;
    }
    return (mpfsys::MPFDrawMdlNoColor*)&lbl_eu_80665894;
}

} // namespace mpfsys
