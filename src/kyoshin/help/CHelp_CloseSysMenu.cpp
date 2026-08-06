#include "kyoshin/help/CHelp_CloseSysMenu.hpp"

extern "C" UNKWORD func_80124B78();

namespace cf {
void CHelp_CloseSysMenu::func_802B7E7C() {
    UNKWORD result = func_80124B78();
    typedef void (*Fn)(CHelp*, u32);
    Fn fn = reinterpret_cast<Fn>(mVtbl->mSlots[7]);
    fn(this, result == 0);
}
} // namespace cf
