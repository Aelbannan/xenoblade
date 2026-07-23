#include "kyoshin/help/CHelp_CloseSysMenu.hpp"

extern "C" UNKWORD func_80124B78();

namespace cf {
void CHelp_CloseSysMenu::func_802B7E7C() {
    UNKWORD result = func_80124B78();
    CHelpVtbl* vt = mVtbl;
    reinterpret_cast<void (*)(CHelp*, u32)>(vt->mSlots[7])(this, result == 0);
}
} // namespace cf
