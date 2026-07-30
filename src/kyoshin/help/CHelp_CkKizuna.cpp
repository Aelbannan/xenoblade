#include "kyoshin/help/CHelp_CkKizuna.hpp"

extern "C" UNKWORD func_801BCF38();

namespace cf {
void CHelp_CkKizuna::func_802B7DEC() {
    UNKWORD result = func_801BCF38();
    reinterpret_cast<void (*)(CHelp*, u32)>(mVtbl->mSlots[7])(this, result == 0);
}
} // namespace cf
