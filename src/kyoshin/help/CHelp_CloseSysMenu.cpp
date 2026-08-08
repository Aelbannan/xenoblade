#include "kyoshin/help/CHelp_CloseSysMenu.hpp"

namespace cf {

// Cast-only fake SI iface: the non-polymorphic 8-byte base pushes the vptr to
// object+8 (retail `mVtbl`), so MWCC virtual dispatch reads it into r12 (retail
// shape) instead of coloring a function-pointer temp as r4. Never constructed.
struct CHelpCloseSysMenuShift {
    char pad[8];
};
struct CHelpCloseSysMenuIf : cf::CHelpCloseSysMenuShift {
    // vtable offsets (RTTI on: typeinfo@0 / toffset@4, first declared virtual @8).
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C(u32 flag); // vtable slot 0x1C == mVtbl->mSlots[7]
};

void CHelp_CloseSysMenu::func_802B7E7C() {
    reinterpret_cast<cf::CHelpCloseSysMenuIf*>(this)->_v01C(func_80124B78() == 0);
}

} // namespace cf