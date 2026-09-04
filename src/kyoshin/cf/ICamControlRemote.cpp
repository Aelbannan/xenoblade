// ICamControl base implementations (slots shared by the Remote and GC tables)
// plus the ICamControlRemote leaf overrides. Each getter calls the pad-status
// provider at vtable +0x8 (this->func_80274B28()) and extracts flag bits from
// the returned CPad's button-flag words.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/ICamControl.hpp"
#include "monolib/core/CPadManager.hpp"

// Remote pad "connected / usable" query. C-ABI symbol (defined in CfCam).
extern "C" int func_8006D700(int controllerId);

// Tail-call alias for CfGameManager::getCurrentPad().
CPad* cf::ICamControl::func_80274B28() {
    return cf::CfGameManager::getCurrentPad();
}

u32 cf::ICamControl::func_80274B2C(int controllerId) {
    CPad* st = this->func_80274B28();
    if (func_8006D700(controllerId) != 0) {
        u32 flags = st->mHeldButtonFlags;
        if ((flags >> 4) & 1) {
            if ((flags & (1 << 5)) != 0) {
                return 0; // both set -> 0
            }
        }
        return (flags >> 4) & 1; // bit4-clear or bit5-clear -> 0 or 1
    }
    return 0;
}

u32 cf::ICamControl::func_80274BA4(int controllerId) {
    CPad* st = this->func_80274B28();
    if (func_8006D700(controllerId) != 0) {
        u32 flags = st->mHeldButtonFlags;
        if ((flags & (1 << 4)) != 0 && (flags & (1 << 5)) != 0) {
            return false;
        }
        return (flags & (1 << 5)) != 0; // bit 26 (MSB)
    }
    return false;
}

u32 cf::ICamControl::func_80274C20() {
    CPad* st = this->func_80274B28();
    return ((st->mShortPressButtonFlags & (1 << 11)) & ~st->mLongHoldButtonFlags) != 0;
}

u32 cf::ICamControl::func_80274C68(int controllerId) {
    CPad* st = this->func_80274B28();
    if (func_8006D700(controllerId) != 0 && (st->mPressedButtonFlags & (1 << 12)) != 0) {
        return true;
    }
    return false;
}

u32 cf::ICamControlRemote::func_80274CD4(int arg) {
    CPad* st = this->func_80274B28();
    return ((st->mHeldButtonFlags >> 2) & 1) != 0;
}

u32 cf::ICamControlRemote::func_80274D08(int arg) {
    CPad* st = this->func_80274B28();
    return ((st->mHeldButtonFlags >> 3) & 1) != 0;
}

u32 cf::ICamControlRemote::func_80274D3C(int arg) {
    CPad* st = this->func_80274B28();
    return (st->mHeldButtonFlags & 1) != 0;
}

u32 cf::ICamControlRemote::func_80274D70(int arg) {
    CPad* st = this->func_80274B28();
    return ((st->mHeldButtonFlags >> 1) & 1) != 0;
}

u32 cf::ICamControl::func_80274DA4(int arg) {
    CPad* st = this->func_80274B28();
    return ((st->mLongHoldButtonFlags >> 11) & 1) != 0;
}
