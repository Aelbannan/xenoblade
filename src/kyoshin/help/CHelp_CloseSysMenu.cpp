#include "kyoshin/help/CHelp_CloseSysMenu.hpp"

extern "C" UNKWORD func_80124B78();

namespace cf {
void CHelp_CloseSysMenu::func_802B7E7C() {
    UNKWORD result = func_80124B78();
    reinterpret_cast<void (*)(CHelp*, u32)>(mVtbl->mSlots[7])(this, result == 0);
}
} // namespace cf
