#include "kyoshin/help/CHelp_CloseItemMenu.hpp"

extern "C" u32 func_801B481C();

namespace cf {

void CHelp_CloseItemMenu::func_802B7E34() {
    u32 result = func_801B481C();
    reinterpret_cast<void (*)(CHelp*, u32)>(mVtbl->mSlots[7])(this, result == 0);
}

} // namespace cf
