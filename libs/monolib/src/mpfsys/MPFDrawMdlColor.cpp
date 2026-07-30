#include "monolib/mpfsys/MPFDrawMdlColor.hpp"

extern "C" {
    s8 lbl_eu_80665898;
    mpfsys::MPFDrawMdlColor* lbl_eu_8066589C;
}

static mpfsys::MPFDrawMdlColor lbl_eu_8056DBC0;

namespace mpfsys {

MPFDrawMdlColor* MPFDrawMdlColor::getInstance(){
    if(!lbl_eu_80665898){
        lbl_eu_8066589C = &lbl_eu_8056DBC0;
        lbl_eu_80665898 = 1;
    }
    return lbl_eu_8066589C;
}

} // namespace mpfsys
