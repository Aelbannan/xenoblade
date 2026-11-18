// Auto-scaffolded catalog TU for kyoshin/plugin/pluginEve
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/CVisionImports.hpp"  // TU imports formerly reached via the fat CBattleManager/chain headers

// Event/presentation flag bitfields (.sbss)
extern u32 lbl_eu_80663E28; // secondary mode bitfield (fade/skip status bits)

extern "C" u32 CtrlRemote_TouchBitByArg(u32 addr); // global data/flag memory reader

extern "C" void checkAvailableValue__Q22cf13CfGameManagerFv(int id);
extern "C" int processBattleQueue__Q22cf13CfGameManagerFv(u32 mode);
extern "C" void func_8008566C__Q22cf13CfGameManagerFv(u32 mode, const float* color,
                                                       u32 param);
extern "C" void UIWin_BuildFlagBuf(int a, int b, int c);
extern "C" void awardQuestFlags();

extern "C" void* __dynamic_cast(void* obj, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);
extern u32 __RTTI__Q22cf12CfObjectMove; // RTTI typeinfo (src type) for actor dynamic_cast
// sdata2 head floats (defined at EOF in retail order 0.0, 1.0): the fade
// color stores below use these instead of literals so no compiler pool
// remains and the section lands byte-exact.
extern f32 eve_sdata2_zero;
extern f32 eve_sdata2_one;

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
        ret.value.uintVal = CtrlRemote_TouchBitByArg(high + (u32)idx);
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
    int addr;
    int valid = 1;
    u32 high = flags >> 16;
    if (idx < (int)low) {
        // Larger flag-bank addresses only permit a limited value magnitude.
        addr = high + idx;
        if (addr >= 0x20) {
            if (addr < 0x220) {
                if (value > 0xffff) valid = 0;
            } else if (addr < 0xa20) {
                if (value > 0xff) valid = 0;
            } else if (addr < 0x36d0) {
                if (value > 0x1) valid = 0;
            }
        }
        if (valid != 0) {
            CtrlRemote_SetSharedBit(addr, value);
            // Special flags: 0x22:03E8 (sequence) expects a 16-bit value near
            // 0xFF, and 0x0A:212C needs a camera/event reset. Compared via
            // split high/low halves to match retail codegen.
            if ((u32)(flags - 0x02200000) == 0x3e8) {
                if ((u32)(value - 0xfe) <= 1) {
                    UIWin_BuildFlagBuf(4, idx, 0);
                }
            }
            if ((u32)(flags - 0x0a200000) == 0x12c) {
                awardQuestFlags();
            }
        } else {
            vmPluginExceptionThrow(pThread);
            return 0;
        }
    } else {
        vmPluginExceptionThrow(pThread);
        return 0;
    }
    return 0;
}

// Get the value of award/sequence flag F16 and push it back as the result.
extern "C" u32 getEventValue40__Q22cf13CfGameManagerFv(u32 id);
extern "C" void setEventManagerValue__Q22cf13CfGameManagerFv(u32 id, u32 value);
int getAwardFlagF16(VMThread* pThread) {
    VMArg arg;
    arg.value.uintVal =
        getEventValue40__Q22cf13CfGameManagerFv(vmArgIntGet(2, vmArgPtrGet(pThread, 1)));
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
    u32 sum = add + getEventValue40__Q22cf13CfGameManagerFv(id);
    if (sum > 0xffff)
        sum = 0xffff;
    setEventManagerValue__Q22cf13CfGameManagerFv(id, sum);
    return 0;
}

extern "C" void queueEventId__Q22cf13CfGameManagerFv(u32 value);
extern "C" bool processEventQueueB__Q22cf13CfGameManagerFv();

int setAwardFlagF1(VMThread* pThread) {
    // Set the award flag for the event passed as arg 1.
    queueEventId__Q22cf13CfGameManagerFv(vmArgIntGet(2, vmArgPtrGet(pThread, 1)));
    return 0;
}

// Start a realtime event. If the request yields while syncing with the UI,
// suspend the script; otherwise fire the event and battle-camera reset.
extern "C" void processFieldLoad__Q22cf13CfGameManagerFv();
extern "C" void CBattleMan_MarkList48Flag40(cf::CBattleManager* bm);
int realtimeEventStart(VMThread* pThread) {
    // Arg 1: skip-if-busy flag (default TRUE).
    int skipIfBusy;
    if (vmArgOmitChk(pThread, 1) != 0) {
        skipIfBusy = 1;
    } else {
        skipIfBusy = vmArgBoolGet(2, vmArgPtrGet(pThread, 1));
    }
    int proceeding = 1; // unchanged unless we fall back to wait mode
    if (skipIfBusy != 0 && (lbl_eu_80663E24 & 0xafa40000) != 0) {
        vmWaitModeSet(pThread);
        proceeding = 0;
    }
    if (proceeding != 0) {
        processFieldLoad__Q22cf13CfGameManagerFv();
        CBattleMan_MarkList48Flag40(cf::CBattleManager::getInstance());
    }
    return 0;
}

// Play a realtime event named in arg 2; if the realtime flag is already set,
// suspend the script instead.
extern "C" void handleAreaChange__Q22cf13CfGameManagerFv(const char* name);
int realtimeEventPlay(VMThread* pThread) {
    const char* name = vmArgStringGet(2, vmArgPtrGet(pThread, 1));
    if (lbl_eu_80663E28 & 0x1) {
        vmWaitModeSet(pThread);
    } else {
        handleAreaChange__Q22cf13CfGameManagerFv(name);
    }
    return 0;
}

extern "C" void clearBattleFlagsAndQueue__Q22cf13CfGameManagerFv();

int realtimeEventEnd(VMThread* pThread) {
    clearBattleFlagsAndQueue__Q22cf13CfGameManagerFv();
    return 0;
}

int waitRealtimeEvent(VMThread* pThread) {
    // Suspend the script while realtime event playback is active.
    if (processEventQueueB__Q22cf13CfGameManagerFv()) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

// View of CfObjectActor's CfObjectMove subobject at absolute offset 0x3E9C.
// The retail "is talking?" dispatch is a virtual call on a CObjectState
// subobject at +0x3E9C (vtable slot +0x2C = CObjectState_setStateBitMask0,
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

// The retail symbol for this callback is the plain (C-style) name `onTalk`;
// define it as a normal C++ function here and let the symbol-recovery tooling
// map the emitted mangled name onto the retail symbol.
int onTalk(VMThread* pThread) {
    // Report (TRUE=1 / FALSE=2) whether the player actor's embedded move
    // object is currently talking. Query via setStateBitMask0(mask, flag)
    // (same (1|2, 1) shape as ocUnit::onEvent); packed void* 0x1000x was wrong.
    cf::CfObjectActor* actor = (cf::CfObjectActor*)__dynamic_cast(
        cf::CfGameManager::getPlayer(0), 0,
        &__RTTI__Q22cf13CfObjectActor, &__RTTI__Q22cf12CfObjectMove,
        0);
    CfActorTalkView* view = (CfActorTalkView*)actor;
    u8 ret;
    if (view->CObjectState_setStateBitMask0(1, 1) != 0 ||
        view->CObjectState_setStateBitMask0(2, 1) != 0) {
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
        cf::CfGameManager::getPlayer(0), 0,
        &__RTTI__Q22cf13CfObjectActor, &__RTTI__Q22cf12CfObjectMove,
        0);
    CfActorTalkView* view = (CfActorTalkView*)actor;
    u8 ret;
    if (view->CObjectState_setStateBitMask0(1, 0) != 0 ||
        view->CObjectState_setStateBitMask0(2, 0) != 0) {
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
    checkAvailableValue__Q22cf13CfGameManagerFv(0x10);
    float color[4];
    if (c == 0) {
        color[0] = eve_sdata2_one;
        color[1] = eve_sdata2_one;
        color[2] = eve_sdata2_one;
        color[3] = eve_sdata2_zero;
    } else {
        color[0] = eve_sdata2_zero;
        color[1] = eve_sdata2_zero;
        color[2] = eve_sdata2_zero;
        color[3] = eve_sdata2_zero;
    }
    func_8008566C__Q22cf13CfGameManagerFv(0, color, 1);
    color[3] = eve_sdata2_one;
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
    checkAvailableValue__Q22cf13CfGameManagerFv(0x10);
    int fadeActive = processBattleQueue__Q22cf13CfGameManagerFv(0x19);
    float color[4];
    if (c == 0) {
        color[0] = eve_sdata2_one;
        color[1] = eve_sdata2_one;
        color[2] = eve_sdata2_one;
        color[3] = eve_sdata2_zero;
    } else {
        color[0] = eve_sdata2_zero;
        color[1] = eve_sdata2_zero;
        color[2] = eve_sdata2_zero;
        color[3] = eve_sdata2_zero;
    }
    func_8008566C__Q22cf13CfGameManagerFv(mode & 0xffff, color, 1);
    if ((lbl_eu_80663E24 & 0x80) == 0 || fadeActive) {
        lbl_eu_80663E28 |= 0x2;
    }
    return 0;
}

// Query whether a fade/blank is active (/fadeWait command). If the game
// is not currently presenting, suspend the script until it finishes.
extern "C" int resetCameraManager__Q22cf13CfGameManagerFv();
int fadeWait(VMThread* pThread) {
    if (resetCameraManager__Q22cf13CfGameManagerFv() == 0) {
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

// Command-table name/data labels for the "eve" plugin (retail data symbols).
// .rodata entry names below (unsized, 4-packed) + "eve" plugin name.
extern char lbl_eu_804FACF0[];
extern PluginFuncData lbl_eu_80525EF8[];

// ----- Retail data sections defined by this TU (subset) -----
// .rodata long entry names (unsized, 4-packed) + "eve" plugin name + .data
// type-ref block (80525E98) + .sdata2 strings/floats + the PluginFuncData
// table (80525EF8), all landed byte-exact (wave DL-r4; the frozen UNIT_RULE
// strip/pool block is removed). The fade stores read the head float pair
// (eve_sdata2_zero/one, retail pool order) instead of literals so no
// compiler pool remains.
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FAC20[] = "getAwardFlagF16";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FAC30[] = "addAwardFlagF16";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FAC40[] = "setAwardFlagF1";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FAC50[] = "realtimeEventStart";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FAC64[] = "realtimeEventPlay";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FAC78[] = "realtimeEventEnd";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FAC8C[] = "waitRealtimeEvent";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FACA0[] = "checkEvent";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FACAC[] = "clearEventSkip";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FACBC[] = "isTalkEvent";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FACC8[] = "isVisionEvent";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FACD8[] = "onTalkEnd";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FACE4[] = "fadeWait";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FACF0[8] = "eve";
// Type-reference block (80525E98, frontier label): 10 external typeinfo
// pointers (ocUnit's .sdata, another unit defines them) + the CfObjectActor
// RTTI, each paired with a subobject offset (mostly 0x3E9C, the embedded
// CfObjectMove offset; a few differ), ending with a null pair. This is a
// dynamic-cast dispatch table (type -> subobject offset). RESIDUAL: exact
// source shape unknown; reproduced byte-faithfully.
extern u32 __RTTI__Q22cf11CActorParam[];
extern u32 __RTTI__Q22cf11CDebugState[];
extern u32 __RTTI__Q22cf11CActorState[];
extern u32 __RTTI__Q22cf12CBattleState[];
extern u32 __RTTI__Q22cf9CAIAction[];
extern u32 __RTTI__Q22cf12CObjectParam[];
extern u32 __RTTI__Q22cf12CObjectState[];
struct EveTypeRef { const void* type; u32 subobjOff; };
__declspec(section ".data") __attribute__((used)) EveTypeRef lbl_eu_80525E98[12] = {
    { __RTTI__Q22cf12CObjectState, 0x3E9C }, { __RTTI__Q22cf12CObjectParam, 0x3E9C },
    { &__RTTI__Q22cf8CfObject, 0x3E9C }, { &__RTTI__Q22cf13CfObjectModel, 0x3E9C },
    { &__RTTI__Q22cf12CfObjectMove, 0x3E9C }, { __RTTI__Q22cf9CAIAction, 0x3380 },
    { __RTTI__Q22cf11CDebugState, 0x15DC }, { __RTTI__Q22cf12CBattleState, 0x08 },
    { __RTTI__Q22cf11CActorState, 0 }, { __RTTI__Q22cf11CActorParam, 0 },
    { &__RTTI__Q22cf13CfObjectActor, 0 }, { 0, 0 },
};
// Note: the eve function table (80525EF8) uses the region-name string
// scRegionNameMEM1 (retail 8-byte string at sdata2+0x30, defined below)
// as its "fadeOut" entry name.

extern "C" void pluginEveRegist() {
    vmPluginRegist(lbl_eu_804FACF0, lbl_eu_80525EF8);
}

// [.sdata2] 0x80665DF8-0x80665E38 (0x40): head float pair (retail order 0.0,
// 1.0; the fade stores above read these, so no compiler pool remains),
// method-name strings, the MEM1 region-name string (retail symbol
// scRegionNameMEM1, "fadeOut"), and the two pluginCfs-read floats
// (4096.0 divider, 360.0). Emitted in declaration order after the code pool
// (pool-first, CMenuBattleChain precedent); offsets verified byte-exact.
#pragma section data_type ".sdata2" ".sdata2"
extern __declspec(section ".sdata2") f32 eve_sdata2_zero = 0.0f;
extern __declspec(section ".sdata2") f32 eve_sdata2_one = 1.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(8)))
char lbl_eu_80665E00[8] = "getFlag";
__declspec(section ".sdata2") __attribute__((used, aligned(8)))
char lbl_eu_80665E08[8] = "setFlag";
__declspec(section ".sdata2") __attribute__((used, aligned(8)))
char lbl_eu_80665E10[8] = "isEvent";
__declspec(section ".sdata2") __attribute__((used, aligned(1)))
char lbl_eu_80665E18[7] = "onTalk";
__declspec(section ".sdata2") __attribute__((used, aligned(2)))
char lbl_eu_80665E20[7] = "fadeIn";
__declspec(section ".sdata2") __attribute__((used, aligned(2)))
char scRegionNameMEM1__Q23mtl10MemManager[8] = "fadeOut";
#pragma section data_type ".data" ".data"

// [.data] PluginFuncData table (80525EF8, 20 entries): method-name /
// function pairs in retail order, null-terminated. clearEventSkip takes no
// args, so its slot carries an explicit PluginFunc cast (address only).
__declspec(section ".data") __attribute__((used, aligned(8)))
PluginFuncData lbl_eu_80525EF8[20] = {
    { lbl_eu_80665E00, getFlag },
    { lbl_eu_80665E08, setFlag },
    { lbl_eu_804FAC20, getAwardFlagF16 },
    { lbl_eu_804FAC30, addAwardFlagF16 },
    { lbl_eu_804FAC40, setAwardFlagF1 },
    { lbl_eu_804FAC50, realtimeEventStart },
    { lbl_eu_804FAC64, realtimeEventPlay },
    { lbl_eu_804FAC78, realtimeEventEnd },
    { lbl_eu_804FAC8C, waitRealtimeEvent },
    { lbl_eu_804FACA0, checkEvent },
    { lbl_eu_804FACAC, (PluginFunc)clearEventSkip },
    { lbl_eu_80665E10, isEvent },
    { lbl_eu_804FACBC, isTalkEvent },
    { lbl_eu_804FACC8, isVisionEvent },
    { lbl_eu_80665E18, onTalk },
    { lbl_eu_804FACD8, onTalkEnd },
    { lbl_eu_80665E20, fadeIn },
    { scRegionNameMEM1__Q23mtl10MemManager, fadeOut },
    { lbl_eu_804FACE4, fadeWait },
    { nullptr, nullptr },
};
#pragma section data_type ".data" ".data"
