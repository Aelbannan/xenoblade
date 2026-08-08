#include "kyoshin/help/CHelp_EndEvent.hpp"
#include <cstring>

extern u32 lbl_eu_80663E24;
extern char lbl_eu_80571628[0x24];

namespace cf {

// Fake polymorphic interface for vtable dispatch. CHelp keeps its vtable
// pointer at object+8 (mVtbl) rather than PPC-standard +0; with RTTI on MWCC
// lays out [typeinfo@0, toffset@4, virtuals@8..], so a non-polymorphic 8-byte
// prefix pushes the vptr to +8 and the six virtuals below map to vtable slot
// offsets +0x08..+0x1C. Dispatch then reads the vptr into r12 (retail shape)
// instead of a function-pointer temp. Never instantiated.
class CHelpEndEventDispatchBase {
public:
    char mOwnerAndParam[8]; // matches CHelp's mOwner/mParam, shifts the vptr
};

class CHelpEndEventIf : public CHelpEndEventDispatchBase {
public:
    virtual void mSlot0();         // vtable offset +0x08
    virtual void mSlot1();         //                 +0x0C
    virtual void mSlot2();         //                 +0x10
    virtual void mSlot3();         //                 +0x14
    virtual void mSlot4();         //                 +0x18
    virtual void mSlot5(u32 flag); //                 +0x1C == mVtbl->mSlots[7]
};

void CHelp_EndEvent::func_802B7EC4() {
    u32 v;

    if (unk10 != NULL) {
        v = 0;
        if (lbl_eu_80663E24 & 0x00400000) {
            bool eq = strcmp(lbl_eu_80571628, unk10) == 0;
            if (eq) {
                v = 1;
            }
        }
    } else {
        v = (lbl_eu_80663E24 >> 22) & 1;
    }

    reinterpret_cast<CHelpEndEventIf*>(this)->mSlot5(v == 0);
}

} // namespace cf