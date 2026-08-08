// Decompiled TU for kyoshin/help/CHelp_ClosePartyMenu.

#include "kyoshin/help/CHelp_ClosePartyMenu.hpp"
#include "kyoshin/help/CHelp_OpenPartyMenu.hpp"

namespace cf {

// Cast-only fake SI iface: the non-polymorphic 8-byte base pushes the vptr to
// object+8 (retail `mVtbl`), so MWCC virtual dispatch reads it into r12 (retail
// shape) instead of coloring a function-pointer temp as r4. Never constructed.
struct CHelpClosePartyMenuShift {
    char pad[8];
};
struct CHelpClosePartyMenuIf : cf::CHelpClosePartyMenuShift {
    // vtable offsets (RTTI on: typeinfo@0 / toffset@4, first declared virtual @8).
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C(u32 flag); // vtable slot 0x1C == mVtbl->mSlots[7]
};

// Target 2: us-802bafa4
// Calls vtbl slot 7 with 1 (true) only if both func_80293C10 and func_80192BD0
// return zero; otherwise 0.
void CHelp_ClosePartyMenu::func_802B8534() {
    u32 flag = (!func_80293C10() && !func_80192BD0()) ? 1 : 0;
    reinterpret_cast<cf::CHelpClosePartyMenuIf*>(this)->_v01C(flag);
}

} // namespace cf
