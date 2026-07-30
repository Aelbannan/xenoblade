#include "kyoshin/help/CHelp_EndEvent.hpp"
#include <cstring>

extern u32 lbl_eu_80663E24;
extern char lbl_eu_80571628[0x24];

namespace cf {
void CHelp_EndEvent::func_802B7EC4() {
    u32 r31;

    if (unk10 != NULL) {
        r31 = 0;
        if (lbl_eu_80663E24 & 0x00400000) {
            strcmp(lbl_eu_80571628, unk10);
        }
    } else {
        r31 = (lbl_eu_80663E24 >> 22) & 1;
    }

    reinterpret_cast<void (*)(CHelp*, u32)>(mVtbl->mSlots[7])(this, r31 == 0);
}
}
