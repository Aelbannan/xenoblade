#include "kyoshin/help/CHelp_CloseItemMenu.hpp"

// func_801B481C is a retail global helper ("any block condition active"),
// defined in kyoshin/menu/CMenuGetItemMulti.cpp. Its retail symbol is
// unmangled, so a C-linkage declaration is real ABI here, not scaffolding:
// removing the extern "C" would mangle the call and fail to link.
extern "C" u32 func_801B481C();

namespace cf {

// CHelp keeps its vtable pointer at object+8 (mVtbl, a manual interface
// table) rather than the PPC-standard +0. To dispatch a slot through it with
// MWCC virtual-call codegen (which reads the vptr into r12 then mtctr/bctrl),
// model a polymorphic interface whose 8-byte non-virtual prefix pushes the
// vptr to +8, mirroring CHelp::mOwner/mParam.
//
// With RTTI on MWCC lays the vtable out as [typeinfo @0, toffset @4,
// virtuals @8..]. The six virtuals below therefore map to the object offsets
// +0x08/+0x0C/+0x10/+0x14/+0x18/+0x1C; the last one is the slot this method
// dispatches to, i.e. CHelp::mVtbl->mSlots[7].
class CHelpCloseItemMenuDispatchBase {
public:
    char mOwnerAndParam[8]; // +0..+7; matches CHelp's mOwner/mParam and only shifts the vptr
};

class CHelpCloseItemMenuIf : public CHelpCloseItemMenuDispatchBase {
public:
    virtual void mSlot0();         // vtable offset +0x08
    virtual void mSlot1();         //                 +0x0C
    virtual void mSlot2();         //                 +0x10
    virtual void mSlot3();         //                 +0x14
    virtual void mSlot4();         //                 +0x18
    virtual void mSlot5(u32 flag); //                 +0x1C == mVtbl->mSlots[7]
};

void CHelp_CloseItemMenu::func_802B7E34() {
    reinterpret_cast<cf::CHelpCloseItemMenuIf*>(this)->mSlot5(func_801B481C() == 0);
}

} // namespace cf