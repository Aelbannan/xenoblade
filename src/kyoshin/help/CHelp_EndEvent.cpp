#include "kyoshin/help/CHelp_EndEvent.hpp"
#include <cstring>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

extern char lbl_eu_80571628[0x24];

namespace cf {

void CHelp_EndEvent::checkEndEventState() {
    u32 v;

    if (unk10 != NULL) {
        v = 0;
        if (lbl_eu_80663E24 & 0x00400000) {
            bool eq = strcmp(lbl_eu_80571628, unk10) == 0;
            if (eq) {
                v = 1;
            }
        }
    } else {
        v = (lbl_eu_80663E24 >> 22) & 1;
    }

    this->func_802B7CBC(v == 0);
}

} // namespace cf
