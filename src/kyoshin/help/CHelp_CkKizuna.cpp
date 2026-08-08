#include "kyoshin/help/CHelp_CkKizuna.hpp"

namespace cf {

// Cast-only SI iface: CHelp's manual interface-table pointer sits at this+8 (not a
// C++ vptr at +0). Declaring a non-polymorphic Shift base (mOwner@0, mParam@4) makes
// MWCC place the C++ vptr at +8, and virtual dispatch then emits retail's
// lwz r12,8(this) / lwz r12,28(r12) / bcctrl chain. With -RTTI on, the first two
// hidden vtable slots are omitted so _v01C lands at vtable offset 0x1C (manual slot 7).
// Never constructed.
struct CHelpCkKizunaIfShift {
    void* mOwner; // +0
    u32   mParam; // +4
};
struct CHelpCkKizunaIf : CHelpCkKizunaIfShift {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C(u32 arg); // manual table slot 7 (vtable +0x1C)
};

void CHelp_CkKizuna::func_802B7DEC() {
    // Inline the comparison so MWCC keeps func_801BCF38's return in r3 for the
    // cntlzw/srwi boolean conversion (matching retail's scheduling exactly).
    reinterpret_cast<CHelpCkKizunaIf*>(this)->_v01C(func_801BCF38() == 0);
}

} // namespace cf
