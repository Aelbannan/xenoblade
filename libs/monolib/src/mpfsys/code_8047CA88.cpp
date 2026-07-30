#include "monolib/mpfsys/code_8047CA88.hpp"

extern "C" {
    s8 lbl_eu_806658C0;
}

static char lbl_eu_80658518[4];

UnkClass_8047CA88* UnkClass_8047CA88::getInstance(){
    if(!lbl_eu_806658C0){
        lbl_eu_806658C0 = 1;
    }
    return (UnkClass_8047CA88*)&lbl_eu_80658518;
}
