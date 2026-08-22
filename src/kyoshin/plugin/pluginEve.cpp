// Auto-scaffolded catalog TU for kyoshin/plugin/pluginEve
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
// The CAIAction.hpp chain declares the shared battle-manager singleton getter
// returning void*, conflicting with an earlier CBattleManagerView* declaration
// pulled in by harness_catalog.hpp. Rename it out of the way (same idiom as
// CBattleManager.cpp); this TU never calls the getter.
#define getInstance__Q22cf14CBattleManagerFv eveBmSingletonGetUnused
#include "kyoshin/cf/CBattleManager.hpp"
#undef getInstance__Q22cf14CBattleManagerFv
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"

// Event/presentation flag bitfields (.sbss)
extern u32 lbl_eu_80663E28; // secondary mode bitfield
extern u32 lbl_eu_80663E24; // primary event/presentation bitfield

extern "C" u32 func_8009CF8C(u32 addr); // global data/flag memory reader

extern "C" void func_8007FD00__Q22cf13CfGameManagerFv(int id);
extern "C" int func_8007FD8C__Q22cf13CfGameManagerFv(u32 mode);
extern "C" void func_8008566C__Q22cf13CfGameManagerFv(u32 mode, const float* color,
                                                       u32 param);
extern "C" void func_80140E00(int a, int b, int c);
extern "C" void func_80291A04();

extern "C" void* __dynamic_cast(void* obj, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);
extern u32 lbl_eu_806618D8; // RTTI typeinfo (src type) for actor dynamic_cast

int getFlag(VMThread* pThread) {
    // flags packs a base offset (high 16 bits) and a length (low 16 bits).
    // Push the flag value at base+idx, or throw a plugin exception if idx is
    // outside the packed range.
    u32 flags = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int idx = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    u32 low = flags & 0xffff;
    u32 high = flags >> 16;

    VMArg ret;
    if (idx < (int)low) {
        ret.type = VM_TYPE_INT;
        ret.value.uintVal = func_8009CF8C(high + (u32)idx);
    } else {
        ret.type = VM_TYPE_INT;
        ret.value.uintVal = 0;
        vmPluginExceptionThrow(pThread);
        return 0;
    }
    vmRetValSet(pThread, &ret);
    return 1;
}

int setFlag(VMThread* pThread) {
    // flags packs a base address (high 16 bits) and a valid range (low 16
    // bits). If idx is in range and the value fits the trust level of that
    // address bank, write value to the flag memory, then apply two special
    // side-effects for the hard-coded sequence flags.
    u32 flags = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int idx = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    int value = vmArgIntGet(4, vmArgPtrGet(pThread, 3));

    u32 low = flags & 0xffff;
    u32 high = flags >> 16;
    int valid = 1;
    if (idx < (int)low) {
        int addr = (int)high + idx;
        // Larger flag-bank addresses only permit a limited value magnitude.
        if (addr >= 0x20) {
            if (addr < 0x220) {
                if (value > 0xffff) valid = 0;
            } else if (addr < 0xa20) {
                if (value > 0xff) valid = 0;
            } else if (addr < 0x36d0) {
                if (value > 0x1) valid = 0;
            }
        }
        if (valid == 0) {
            vmPluginExceptionThrow(pThread);
            return 0;
        }

        func_8009D018(addr, value);
        // Special flags: 0x22:03E8 (sequence) expects a 16-bit value near
        // 0xFF, and 0x0A:212C needs a camera/event reset.
        if (flags == 0x022003e8 && (u32)(value - 0xfe) <= 1) {
            func_80140E00(4, idx, 0);
        }
        if (flags == 0x0a20012c) {
            func_80291A04();
        }
        return 0;
    }
    vmPluginExceptionThrow(pThread);
    return 0;
}

// Get the value of award/sequence flag F16 and push it back as the result.
extern "C" u32 func_80082694__Q22cf13CfGameManagerFv(u32 id);
extern "C" void func_8008269C__Q22cf13CfGameManagerFv(u32 id, u32 value);
int getAwardFlagF16(VMThread* pThread) {
    VMArg arg;
    arg.value.uintVal =
        func_80082694__Q22cf13CfGameManagerFv(vmArgIntGet(2, vmArgPtrGet(pThread, 1)));
    arg.type = VM_TYPE_INT;
    vmRetValSet(pThread, &arg);
    return 1;
}

int addAwardFlagF16(VMThread* pThread) {
    // Add the value in arg 2 (or 1 if omitted) to the 16-bit award/sequence
    // flag selected by arg 1 (flag F16), clamping the result to 16 bits.
    u32 id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    u32 add;
    if (vmArgOmitChk(pThread, 2) != 0) {
        add = 1;
    } else {
        add = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    }
    u32 sum = add + func_80082694__Q22cf13CfGameManagerFv(id);
    if (sum > 0xffff)
        sum = 0xffff;
    func_8008269C__Q22cf13CfGameManagerFv(id, sum);
    return 0;
}

extern "C" void func_800826F0__Q22cf13CfGameManagerFv(u32 value);
extern "C" bool func_800865E8__Q22cf13CfGameManagerFv();

int setAwardFlagF1(VMThread* pThread) {
    // Set the award flag for the event passed as arg 1.
    func_800826F0__Q22cf13CfGameManagerFv(vmArgIntGet(2, vmArgPtrGet(pThread, 1)));
    return 0;
}

// Start a realtime event. If the request yields while syncing with the UI,
// suspend the script; otherwise fire the event and battle-camera reset.
extern "C" void func_800862D0__Q22cf13CfGameManagerFv();
extern "C" void func_800F4004(cf::CBattleManager* bm);
int realtimeEventStart(VMThread* pThread) {
    int v;
    if (vmArgOmitChk(pThread, 1) != 0) {
        v = 1;
    } else {
        v = vmArgBoolGet(2, vmArgPtrGet(pThread, 1));
    }
    int proceeding = 1; // unchanged unless we fall back to wait mode
    if (v != 0 && (lbl_eu_80663E24 & 0xafa40000) != 0) {
        vmWaitModeSet(pThread);
        proceeding = 0;
    }
    if (proceeding != 0) {
        func_800862D0__Q22cf13CfGameManagerFv();
        func_800F4004(cf::CBattleManager::getInstance());
    }
    return 0;
}

// Play a realtime event named in arg 2; if the realtime flag is already set,
// suspend the script instead.
extern "C" void func_800863F4__Q22cf13CfGameManagerFv(const char* name);
int realtimeEventPlay(VMThread* pThread) {
    const char* name = vmArgStringGet(2, vmArgPtrGet(pThread, 1));
    if (lbl_eu_80663E28 & 0x1) {
        vmWaitModeSet(pThread);
    } else {
        func_800863F4__Q22cf13CfGameManagerFv(name);
    }
    return 0;
}

void func_80086490__Q22cf13CfGameManagerFv();

int realtimeEventEnd(VMThread* pThread) {
    func_80086490__Q22cf13CfGameManagerFv();
    return 0;
}

int waitRealtimeEvent(VMThread* pThread) {
    // Suspend the script while realtime event playback is active.
    if (func_800865E8__Q22cf13CfGameManagerFv()) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

// View of CfObjectActor's CfObjectMove subobject at absolute offset 0x3E9C.
// The retail "is talking?" dispatch is a virtual call on a CObjectState
// subobject at +0x3E9C (vtable slot +0x2C = CObjectState_UnkVirtualFunc10,
// this = actor+0x3E9C). The C++ hierarchy layout in the headers places that
// subobject later than retail, so the call goes through this view class:
// CfActorTalkView's CObjectState base lands at +0x3E9C and a virtual call on
// it folds the subobject offset into the vtable load + this-adjustment
// exactly like the retail dispatch (same scheme as CfActorVt5C4Table).
class CfActorTalkBasePad {
public:
    u8 _pad[0x3E9C];
};
class CfActorTalkView : public CfActorTalkBasePad, public cf::CObjectState {
};

int onTalk(VMThread* pThread) {
    // Report (TRUE=1 / FALSE=2) whether the player actor's embedded move
    // object is currently talking.
    cf::CfObjectActor* actor = (cf::CfObjectActor*)__dynamic_cast(
        (void*)cf::CfGameManager::getPlayer(0), 0,
        (const void*)&__RTTI__Q22cf13CfObjectActor, (const void*)&lbl_eu_806618D8,
        0);
    CfActorTalkView* view = (CfActorTalkView*)actor;
    u8 ret;
    if (view->CObjectState_UnkVirtualFunc10((void*)1, 1) != 0 ||
        view->CObjectState_UnkVirtualFunc10((void*)2, 1) != 0) {
        ret = 1;
    } else {
        ret = 2;
    }
    vmRetValSet(pThread, (VMArg*)&ret);
    return 1;
}

int onTalkEnd(VMThread* pThread) {
    // Same talk-mode query as onTalk but without forcing the talk state on
    // (arg2 = 0 in both dispatch calls).
    cf::CfObjectActor* actor = (cf::CfObjectActor*)__dynamic_cast(
        (void*)cf::CfGameManager::getPlayer(0), 0,
        (const void*)&__RTTI__Q22cf13CfObjectActor, (const void*)&lbl_eu_806618D8,
        0);
    CfActorTalkView* view = (CfActorTalkView*)actor;
    u8 ret;
    if (view->CObjectState_UnkVirtualFunc10((void*)1, 0) != 0 ||
        view->CObjectState_UnkVirtualFunc10((void*)2, 0) != 0) {
        ret = 1;
    } else {
        ret = 2;
    }
    vmRetValSet(pThread, (VMArg*)&ret);
    return 1;
}

int fadeIn(VMThread* pThread) {
    // Fade command: arg 1 selects the fade mode, optional arg 2 picks the
    // color (default = white/opaque). Builds a 4-float RGBA fade color and
    // hands it to CfGameManager twice. When the secondary flag isn't set,
    // raise the "in narrative fade" bit.
    u32 mode = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    u32 c;
    if (vmArgOmitChk(pThread, 2)) {
        c = 0;
    } else {
        c = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    }
    func_8007FD00__Q22cf13CfGameManagerFv(0x10);
    float color[4];
    if (c == 0) {
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
        color[3] = 0.0f;
    } else {
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = 0.0f;
    }
    func_8008566C__Q22cf13CfGameManagerFv(0, color, 1);
    color[3] = 1.0f;
    func_8008566C__Q22cf13CfGameManagerFv(mode & 0xffff, color, 1);
    if ((lbl_eu_80663E24 & 0x80) == 0) {
        lbl_eu_80663E28 |= 0x2;
    }
    return 0;
}

int fadeOut(VMThread* pThread) {
    // Fade command: arg 1 selects the fade mode, optional arg 2 picks the
    // color (default = white/opaque). Builds a 4-float RGBA fade color and
    // hands it to CfGameManager, then raises the "in narrative fade" bit
    // unless the fade already ended cleanly while presenting.
    u32 mode = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    u32 c;
    if (vmArgOmitChk(pThread, 2)) {
        c = 0;
    } else {
        c = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    }
    func_8007FD00__Q22cf13CfGameManagerFv(0x10);
    int fadeActive = func_8007FD8C__Q22cf13CfGameManagerFv(0x19);
    float color[4];
    if (c == 0) {
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
        color[3] = 0.0f;
    } else {
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = 0.0f;
    }
    func_8008566C__Q22cf13CfGameManagerFv(mode & 0xffff, color, 1);
    if ((lbl_eu_80663E24 & 0x80) == 0 || fadeActive) {
        lbl_eu_80663E28 |= 0x2;
    }
    return 0;
}

// Query whether a fade/blank is active (/fadeWait command). If the game
// is not currently presenting, suspend the script until it finishes.
extern "C" int func_80085838__Q22cf13CfGameManagerFv();
int fadeWait(VMThread* pThread) {
    if (func_80085838__Q22cf13CfGameManagerFv() == 0) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

// Report whether an event is running. Normalizes the bool arg to 0/1.
extern "C" void func_80085978__Q22cf13CfGameManagerFv(int active);
int checkEvent(VMThread* pThread) {
    func_80085978__Q22cf13CfGameManagerFv(
        vmArgBoolGet(2, vmArgPtrGet(pThread, 1)) != 0);
    return 0;
}

int clearEventSkip() {
    lbl_eu_80663E28 &= ~0x100000;
    return 0;
}

// Pushes a VM boolean (TRUE/FALSE) reflecting the requested event flag bit.
static int eventFlagBool(VMThread* pThread, int shift) {
    u8 ret = (u8)((((lbl_eu_80663E24 >> shift) & 1) ^ 1) + 1);
    vmRetValSet(pThread, (VMArg*)&ret);
    return 1;
}

int isEvent(VMThread* pThread) {
    return eventFlagBool(pThread, 22);
}

int isTalkEvent(VMThread* pThread) {
    return eventFlagBool(pThread, 24);
}

int isVisionEvent(VMThread* pThread) {
    return eventFlagBool(pThread, 26);
}

extern "C" void pluginEveRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_804FACF0[];
    extern char lbl_eu_80525EF8[];
    vmPluginRegist((void*)lbl_eu_804FACF0, (void*)lbl_eu_80525EF8);
}
