#include "monolib/mpfsys/MPFDrawMdlNoColor.hpp"

extern "C" {
    s8 lbl_eu_80665890;
    mpfsys::MPFDrawMdlNoColor* lbl_eu_80665894;
}

namespace mpfsys {

static mpfsys::MPFDrawMdlNoColor lbl_eu_8056DBE0;

MPFDrawMdlNoColor* MPFDrawMdlNoColor::getInstance(){
    if(!lbl_eu_80665890){
        lbl_eu_80665894 = &lbl_eu_8056DBE0;
        lbl_eu_80665890 = 1;
    }
    return lbl_eu_80665894;
}

} // namespace mpfsys
