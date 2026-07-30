#include "monolib/mpfsys/MPFDrawCross.hpp"

extern "C" {
    s8 lbl_eu_806658A8;
    mpfsys::MPFDrawCross* lbl_eu_806658AC;
}

struct MPFDrawCross_Prototype {
    mpfsys::MPFDrawCross obj;
    u32 padding[4];
};

static MPFDrawCross_Prototype lbl_eu_8056DC28;

namespace mpfsys {

MPFDrawCross* MPFDrawCross::getInstance(){
    if(!lbl_eu_806658A8){
        lbl_eu_806658AC = &lbl_eu_8056DC28.obj;
        lbl_eu_806658A8 = 1;
    }
    return (mpfsys::MPFDrawCross*)&lbl_eu_806658AC;
}

} // namespace mpfsys
