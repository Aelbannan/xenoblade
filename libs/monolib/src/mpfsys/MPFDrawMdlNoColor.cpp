#include "monolib/mpfsys/MPFDrawMdlNoColor.hpp"

extern "C" {
    extern s8 lbl_eu_80665890;
    extern mpfsys::MPFDrawMdlNoColor* lbl_eu_80665894;
    // Prototype instance storage ships from the retail data unit (monolibdata2).
    // Array-typed extern so MWCC emits the retail lis/addi (HA/LO) addressing
    // instead of sda21 for this .data object.
    extern u8 lbl_eu_8056DBE0[0x10];
}

namespace mpfsys {

MPFDrawMdlNoColor* MPFDrawMdlNoColor::getInstance(){
    if(!lbl_eu_80665890){
        lbl_eu_80665894 = (mpfsys::MPFDrawMdlNoColor*)lbl_eu_8056DBE0;
        lbl_eu_80665890 = 1;
    }
    return (mpfsys::MPFDrawMdlNoColor*)&lbl_eu_80665894;
}

} // namespace mpfsys
