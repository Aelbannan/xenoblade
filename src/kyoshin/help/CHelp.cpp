#include "kyoshin/help/CHelp.hpp"

cf::CHelp::CHelp(u32 owner, u32 param) {
    this->mOwner = owner;
    this->vtbl() = &lbl_eu_8053B3A0;
    this->mParam = param;
}

namespace cf {

void CHelp::dispatchHelp() {
    UNKWORD a = this->getSkipResetFlag();
    UNKWORD b = this->getHelpSceneId();
    CUICfManager_queueTutorialMenu(static_cast<u8>(mParam), b, a);
    CtrlRemote_SetSharedBit(mOwner, 1);
}

} // namespace cf

// Unmangled retail free function (symbols.txt `CHelp_ForwardPartyNotice`): a C-style
// thunk that tail-jumps to MenuTutorialIsCreated (the party-change-notice gate).
// CHelpManager.cpp calls it as a bare free function, so C linkage is required
// to emit the unmangled symbol name.
extern "C" void CHelp_ForwardPartyNotice() {
    MenuTutorialIsCreated();
}

// Close/refresh the party menu: UIWin_CreateMenuUpdate(mode 3) on this instance's
// parameter, then re-arms the interface via CtrlRemote_SetSharedBit.
void cf::CHelp::refreshPartyMenu() {
    UIWin_CreateMenuUpdate(3, this->mParam, 0, 0);
    CtrlRemote_SetSharedBit(this->mOwner, 1);
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

// --- restored from git history (base:gone); do not expand beyond these functions ---
// from commit 365650b84230 needle=CHelp_UnkVirtualFunc2

void CHelp::CHelp_UnkVirtualFunc2() {
    // One-shot indirect calls (no named CHelpVtbl* temp) so MWCC colors the
    // iface base like a virtual dispatch (r12), matching retail.
    typedef UNKWORD (*SlotFn)(CHelp*);
    UNKWORD a = reinterpret_cast<SlotFn>(
        (*reinterpret_cast<void***>(reinterpret_cast<char*>(this) + 8))[6])(this);
    UNKWORD b = reinterpret_cast<SlotFn>(
        (*reinterpret_cast<void***>(reinterpret_cast<char*>(this) + 8))[5])(this);
    func_80134D18(static_cast<u8>(mParam), b, a);
    func_8009D018(mOwner, 1);
}

