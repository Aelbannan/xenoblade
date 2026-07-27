#include "kyoshin/help/CHelp.hpp"

extern "C" cf::CHelpVtbl lbl_eu_8053B3A0;

extern "C" void func_80134D18(u32 param0, UNKWORD param1, UNKWORD param2);
extern "C" void func_8009D018(void* owner, u32 flag);
extern "C" void func_8013DB6C(u32 mode, u32 param, u32 a, u32 b);
extern "C" void func_8029A658();

cf::CHelp::CHelp(cf::CHelp* self, void* owner, u32 param) {
    self->mOwner = owner;
    self->mVtbl = &lbl_eu_8053B3A0;
    self->mParam = param;
}

namespace cf {

void CHelp::CHelp_UnkVirtualFunc2() {
    typedef UNKWORD (*SlotFn)(CHelp*);
    UNKWORD a = reinterpret_cast<SlotFn>(this->mVtbl->mSlots[6])(this);
    UNKWORD b = reinterpret_cast<SlotFn>(this->mVtbl->mSlots[5])(this);
    func_80134D18(static_cast<u8>(mParam), b, a);
    func_8009D018(mOwner, 1);
}

void CHelpSwitch::func_802B7CB0() {
    mFlag = 1;
}

} // namespace cf

extern "C" void func_802B7C64() {
    func_8029A658();
}

extern "C" void func_802B7C68(cf::CHelp* self) {
    func_8013DB6C(3, self->mParam, 0, 0);
    func_8009D018(self->mOwner, 1);
}

u32 cf::CHelpSwitch::func_802B7CBC(cf::CHelpSwitch* self, u32 flag) {
    u32 result = 0;
    u32 prev = self->mFlag;
    self->mFlag = static_cast<u8>(flag);
    if (prev == 0) {
        if (flag != 0) {
            result = 1;
        }
    }
    return result;
}

u32 cf::CHelpSwitch::func_802B7CE4(cf::CHelpSwitch* self, u8 flag) {
    u8 prev = self->mFlag;
    self->mFlag = flag;
    return prev != flag;
}
