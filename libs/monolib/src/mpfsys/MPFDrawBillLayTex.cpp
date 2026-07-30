#include "monolib/mpfsys/MPFDrawBillLayTex.hpp"

extern "C" {
    s8 lbl_eu_806658B0;
    mpfsys::MPFDrawBillLayTex* lbl_eu_806658B4;
}

static mpfsys::MPFDrawBillLayTex lbl_eu_8056DC00;

namespace mpfsys {

MPFDrawBillLayTex* MPFDrawBillLayTex::getInstance(){
    if(!lbl_eu_806658B0){
        lbl_eu_806658B4 = &lbl_eu_8056DC00;
        lbl_eu_806658B0 = 1;
    }
    return lbl_eu_806658B4;
}

} // namespace mpfsys
