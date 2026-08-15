#include "kyoshin/help/CHelp.hpp"

cf::CHelp::CHelp(void* owner, u32 param) {
    this->mOwner = owner;
    this->mVtbl = &lbl_eu_8053B3A0;
    this->mParam = param;
}

namespace cf {

void CHelp::CHelp_UnkVirtualFunc2() {
    // Retail calls the interface-table slots as virtuals (vptr-shaped table at
    // +8); the polymorphic view makes MWCC emit the r12 virtual-call sequence.
    CHelpVtblView* view = reinterpret_cast<CHelpVtblView*>(this);
    UNKWORD a = view->f18(); // slot 6 first, result kept across the next call
    UNKWORD b = view->f14();
    func_80134D18(static_cast<u8>(mParam), b, a);
    func_8009D018((u32)mOwner, 1);
}

} // namespace cf

// Unmangled retail free function (symbols.txt `func_802B7C64`): a C-style
// thunk that tail-jumps to func_8029A658 (the party-change-notice gate).
// CHelpManager.cpp calls it as a bare free function, so C linkage is required
// to emit the unmangled symbol name.
extern "C" void func_802B7C64() {
    func_8029A658();
}

// Close/refresh the party menu: func_8013DB6C(mode 3) on this instance's
// parameter, then re-arms the interface via func_8009D018.
void cf::CHelp::func_802B7C68() {
    func_8013DB6C(3, this->mParam, 0, 0);
    func_8009D018((u32)this->mOwner, 1);
}

void cf::CHelpSwitch::func_802B7CB0() {
    mFlag = 1;
}

u32 cf::CHelpSwitch::func_802B7CBC(u32 flag) {
    u32 result = 0;
    u32 prev = this->mFlag;
    this->mFlag = static_cast<u8>(flag);
    if (prev == 0) {
        if (flag != 0) {
            result = 1;
        }
    }
    return result;
}

u32 cf::CHelpSwitch::func_802B7CE4(u8 flag) {
    u8 prev = this->mFlag;
    this->mFlag = flag;
    return prev != flag;
}
