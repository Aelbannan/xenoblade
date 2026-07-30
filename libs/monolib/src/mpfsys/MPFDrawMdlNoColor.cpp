#include "monolib/mpfsys/MPFDrawMdlNoColor.hpp"

extern "C" {
    s8 lbl_eu_80665890;
    mpfsys::MPFDrawMdlNoColor* lbl_eu_80665894;
}

struct MPFDrawMdlNoColor_Prototype {
    mpfsys::MPFDrawMdlNoColor obj;
    u32 padding[4];
};

static MPFDrawMdlNoColor_Prototype lbl_eu_8056DBE0;

namespace mpfsys {

MPFDrawMdlNoColor* MPFDrawMdlNoColor::getInstance(){
    if(!lbl_eu_80665890){
        lbl_eu_80665894 = &lbl_eu_8056DBE0.obj;
        lbl_eu_80665890 = 1;
    }
    return (mpfsys::MPFDrawMdlNoColor*)&lbl_eu_80665894;
}

} // namespace mpfsys
