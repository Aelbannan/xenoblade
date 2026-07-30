#include "monolib/mpfsys/code_8047CA88.hpp"
#include "monolib/mpfsys/code_8047BB54.hpp"

extern "C" {
    s8 lbl_eu_806658C0;
    u8 lbl_eu_80658518[sizeof(UnkClass_8047CA88) + 64];
}

UnkClass_8047CA88* UnkClass_8047CA88::getInstance(){
    if(!lbl_eu_806658C0){
        lbl_eu_806658C0 = 1;
    }
    return (UnkClass_8047CA88*)&lbl_eu_80658518;
}

void UnkClass_8047CA88::func_8047CAA8() {
    // psq-heavy spatial query — stalled, needs PS-aware matching approach
}

void UnkClass_8047CA88::func_8047CC4C() {
    // psq-heavy init + recursive call — stalled, needs PS-aware matching approach
}
