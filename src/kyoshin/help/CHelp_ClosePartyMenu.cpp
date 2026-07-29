// Decompiled TU for kyoshin/help/CHelp_ClosePartyMenu.

#include "kyoshin/help/CHelp_ClosePartyMenu.hpp"
#include "kyoshin/help/CHelp_OpenPartyMenu.hpp"

extern "C" u32 func_80293C10();
extern "C" u32 func_80192BD0();

namespace cf {

// Target 1: us-802baf5c
// Returns true if either func_80293C10 or func_80192BD0 returns non-zero.
bool CHelp_OpenPartyMenu::func_802B84EC() {
    return func_80293C10() || func_80192BD0();
}

// Target 2: us-802bafa4
// Calls vtbl slot 7 with true only if both func_80293C10 and func_80192BD0 return zero.
void CHelp_ClosePartyMenu::func_802B8534() {
    bool result = !func_80293C10() && !func_80192BD0();
    CHelpVtbl* vt = mVtbl;
    reinterpret_cast<void (*)(CHelp*, u32)>(vt->mSlots[7])(this, result);
}

} // namespace cf
