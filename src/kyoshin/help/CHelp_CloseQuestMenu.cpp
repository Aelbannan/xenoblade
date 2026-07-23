#include "kyoshin/help/CHelp_CloseQuestMenu.hpp"

extern "C" UNKWORD func_80122450();

namespace cf {
void CHelp_CloseQuestMenu::func_802B8604() {
    UNKWORD result = func_80122450();
    CHelpVtbl* vt = mVtbl;
    reinterpret_cast<void (*)(CHelp*, u32)>(vt->mSlots[7])(this, result == 0);
}
} // namespace cf
