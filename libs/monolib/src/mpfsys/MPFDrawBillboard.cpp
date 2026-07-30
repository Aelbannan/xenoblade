#include "monolib/mpfsys/MPFDrawBillboard.hpp"

extern "C" {
    s8 lbl_eu_806658A0;
    mpfsys::MPFDrawBillboard* lbl_eu_806658A4;
}

static mpfsys::MPFDrawBillboard lbl_eu_8056DC48;

namespace mpfsys {

MPFDrawBillboard* MPFDrawBillboard::getInstance(){
    if(!lbl_eu_806658A0){
        lbl_eu_806658A4 = &lbl_eu_8056DC48;
        lbl_eu_806658A0 = 1;
    }
    return (mpfsys::MPFDrawBillboard*)&lbl_eu_806658A4;
}

} // namespace mpfsys
