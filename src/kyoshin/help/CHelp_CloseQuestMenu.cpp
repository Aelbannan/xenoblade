#include "kyoshin/help/CHelp_CloseQuestMenu.hpp"

extern "C" UNKWORD func_80122450();

namespace cf {

// Cast-only fake SI iface: the non-polymorphic 8-byte base pushes the vptr to
// object+8 (retail `mVtbl`), so MWCC virtual dispatch reads it into r12 (retail
// shape) instead of coloring a function-pointer temp as r4. Never constructed.
struct CHelpCloseQuestMenuShift {
    char pad[8];
};
struct CHelpCloseQuestMenuIf : cf::CHelpCloseQuestMenuShift {
    // vtable offsets (RTTI on: typeinfo@0 / toffset@4, first declared virtual @8).
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C(u32 flag); // vtable slot 0x1C == mVtbl->mSlots[7]
};

void CHelp_CloseQuestMenu::func_802B8604() {
    reinterpret_cast<cf::CHelpCloseQuestMenuIf*>(this)->_v01C(func_80122450() == 0);
}

} // namespace cf