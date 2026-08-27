// Auto-scaffolded catalog TU for kyoshin/plugin/pluginUnit
// Plugin functions for the YGG VM "unit" subsystem (battle units, arts).
// These are called from script via the function table registered in
// pluginUnitRegist.

#include <math.h>
#include "kyoshin/cf/CfMapItemManager.hpp"

#include "monolib/vm/yvm2.h"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectState.hpp"

namespace cf {
class CfObjectActor;
class CfUnknownSub;
class CfCode800F42AC;
// Minimal battle-manager interface. The full cf::CBattleManager header chain
// conflicts with CfGameManager.hpp's C-linkage singleton declarations
// (differing return types for the same mangled symbol), so pluginUnit only
// sees the static accessor; the definition lives in CBattleManager.cpp.
class CBattleManager {
public:
    static CBattleManager* getInstance();
};
}

// C-linkage retail symbols referenced by learnArts / clearPcBtlState.
extern "C" {
    extern const char lbl_eu_80507FC0[];
    extern const char lbl_eu_80535460[];

    // bdat helpers (C-linkage in retail; ocBdat.hpp declares them as C++).
    void* func_8003AA34();
    // (getBdatStringColumnValue is declared by object/CBattleState.hpp.)

    // Actor param helpers (C-linkage in retail). Typed as cf::CfObjectActor*
    // to satisfy the no_void_ptr lint rule while keeping C linkage.
    cf::CfObjectActor* func_8009EC9C(unsigned short);
    void func_800A18A4(cf::CfObjectActor*, int);
    cf::CfObjectActor* func_800B8B94(int);
    cf::CfObjectActor* func_800B8C78(int);
    void func_800F3958(cf::CBattleManager*, cf::CfObjectActor*, int);
    void func_800EC8FC(cf::CBattleManager*, cf::CfObjectActor*,
                      cf::CBattleStateEntry*, int);
    void* func_800EA444(cf::CBattleManager*);
    bool isGlobalCamFlagSet__Fi(int mask);
    cf::CfUnknownSub* func_800F477C(cf::CfCode800F42AC* self);

    // 100.0f constant used by getPcHpRate / getEneHpRate (lives in .sdata2).
    extern const float lbl_eu_80668250;

    // Forward declarations: functions defined below inherit C linkage from
    // this block (matching the unmangled retail symbols).
    int getPcHpRate(VMThread* pThread);
    int getEneHpRate(VMThread* pThread);

    // Forward declarations for arts/battle-state plugin commands.
    int onPcArtsAttack(VMThread* pThread);
    int setEneBtlState(VMThread* pThread);
    int onEneArtsAttack(VMThread* pThread);
    int setPcBtlState(VMThread* pThread);
}

void pluginUnitRegist() {
    extern void vmPluginRegist(const char*, const char*);
    vmPluginRegist(lbl_eu_80507FC0 + 0xb, lbl_eu_80535460);
}

using namespace cf;

// The actor HP getters dispatch real CActorParam-chain virtuals on the
// resolved actor (cf::CfObjectActor, whose primary base at offset 0 is
// cf::CActorParam). Retail slot map (US lbl_eu_80529DA0 CfObjectPc primary
// vtable): +0x128 = CActorParam_UnkVirtualFunc37 (lfs f1, 0x17E8(r3); current
// HP) and +0x12C = CActorParam_UnkVirtualFunc38 (lfs f1, 0x17F4(r3); max HP).
// Both are declared with the correct float signature in object/CActorParam.hpp,
// so the calls go through the real type - no cast-only interface needed.
// (The earlier CfObjectHpIf pad guessed these were CfObject slots 54/55, whose
// base implementations return int r3, not float f1.)

/// Script command: return the current HP of a player character (PC) actor,
/// ceiled and converted to int. The actor is resolved by id (arg 2) via
/// func_800B8B94 (pc list lookup); the HP value comes from the CActorParam
/// virtual at vtable+0x128 (CActorParam_UnkVirtualFunc37, float in f1). On
/// miss, returns -1.
extern "C" int getPcHp(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    cf::CfObjectActor* actor = func_800B8B94(id);
    VMArg result;
    if (actor != nullptr) {
        // Dispatch the CActorParam vtable slot at 0x128
        // (CActorParam_UnkVirtualFunc37).
        CActorParam* obj = reinterpret_cast<CActorParam*>(actor);
        result.type = VM_TYPE_INT;
        // Force ceil(double) and explicit float round before fctiwz.
        float val = (float)ceil(obj->CActorParam_UnkVirtualFunc37());
        result.value.intVal = (int)val;
    } else {
        result.type = VM_TYPE_INT;
        result.value.intVal = -1;
    }
    vmRetValSet(pThread, &result);
    return 1;
}

/// Script command: return the current HP rate (%) of a player character (PC)
/// actor. Resolves the actor by id (arg 2) via func_800B8B94 (pc list
/// lookup), then reads the max-HP value (vtable+0x12C,
/// CActorParam_UnkVirtualFunc38) and current-HP value (vtable+0x128,
/// CActorParam_UnkVirtualFunc37) from the CActorParam vtable, computing
/// `ceil(100.0f * (cur / max))`. On miss, returns -1.
int getPcHpRate(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    CActorParam* actor = reinterpret_cast<CActorParam*>(func_800B8B94(id));
    VMArg result;
    if (actor != nullptr) {
        result.type = VM_TYPE_INT;
        // First virtual: vtable+0x12C returns max-HP (saved in f31 by retail).
        float maxHp = actor->CActorParam_UnkVirtualFunc38();
        // Second virtual: vtable+0x128 returns current-HP (result in f1, then
        // fdivs against f31 yields cur/max). Order is fixed by retail codegen.
        float curHp = actor->CActorParam_UnkVirtualFunc37();
        // (float)ceil(...) forces MWCC to emit fdivs+fmuls before ceil,
        // then frsp+fctiwz to round to int (matching retail).
        result.value.intVal = (int)(float)ceil(lbl_eu_80668250 * (curHp / maxHp));
    } else {
        result.type = VM_TYPE_INT;
        result.value.intVal = -1;
    }
    vmRetValSet(pThread, &result);
    return 1;
}

/// Script command: return the current HP of an enemy (ENE) actor, ceiled
/// and converted to int. Mirrors getPcHp but resolves the actor via
/// func_800B8C78 (ene list lookup).
extern "C" int getEneHp(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    cf::CfObjectActor* actor = func_800B8C78(id);
    VMArg result;
    if (actor != nullptr) {
        CActorParam* obj = reinterpret_cast<CActorParam*>(actor);
        result.type = VM_TYPE_INT;
        // Force ceil(double) and explicit float round before fctiwz.
        float val = (float)ceil(obj->CActorParam_UnkVirtualFunc37());
        result.value.intVal = (int)val;
    } else {
        result.type = VM_TYPE_INT;
        result.value.intVal = -1;
    }
    vmRetValSet(pThread, &result);
    return 1;
}

/// Script command: return the current HP rate (%) of an enemy (ENE) actor.
/// Mirrors getPcHpRate but resolves the actor via func_800B8C78 (ene list
/// lookup). On miss, returns -1.
int getEneHpRate(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    CActorParam* actor = reinterpret_cast<CActorParam*>(func_800B8C78(id));
    VMArg result;
    if (actor != nullptr) {
        result.type = VM_TYPE_INT;
        float maxHp = actor->CActorParam_UnkVirtualFunc38();
        float curHp = actor->CActorParam_UnkVirtualFunc37();
        result.value.intVal = (int)(float)ceil(lbl_eu_80668250 * (curHp / maxHp));
    } else {
        result.type = VM_TYPE_INT;
        result.value.intVal = -1;
    }
    vmRetValSet(pThread, &result);
    return 1;
}


/// Script command: check whether the PC actor with the given id is currently
/// performing an arts attack matching the given arts type. Returns 1 if the
/// actor is attacking with the specified arts, 2 otherwise. The check queries
/// the actor's arts-state sub-object (at offset 4) via func_80174C98 with
/// selector 0xa, then compares the matched entry's field_0x77 byte against
/// the provided arts type.
int onPcArtsAttack(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int artsType = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    cf::CfObjectActor* actor = func_800B8B94(id);

    VMArg result;
    result.type = VM_TYPE_INT;
    result.value.intVal = 2;  // default: not matching

    // Gate on game-manager flag 0x400.
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet__Fi(0x400) != 0) {
        vmRetValSet(pThread, &result);
        return 1;
    }
    if (actor == nullptr) {
        vmRetValSet(pThread, &result);
        return 1;
    }

    // Arts-state sub-object at actor+4 is the CfObjectMove part (stored as
    // owner+0x3E9C by CActorParam ctor); its vtable is the CObjectState chain.
    cf::CObjectState* subObj =
        *reinterpret_cast<cf::CObjectState**>(reinterpret_cast<u8*>(actor) + 4);

    // Try selector 0xa against the sub-object's vtable slot +0x34 value.
    u32 val = *static_cast<u32*>(subObj->CObjectState_UnkVirtualFunc12());

    bool matched = false;
    if (func_80174C98(actor, (int*)&val, 0xa) != 0) {
        matched = true;
    } else {
        // Fall back to vtable slot +0x30 value.
        val = *static_cast<u32*>(subObj->CObjectState_UnkVirtualFunc11());
        if (func_80174C98(actor, (int*)&val, 0xa) != 0) {
            matched = true;
        }
    }

    if (matched) {
        // Retrieve the actor's CActorParam_UnkStruct1 via vtable[102] (0x298).
        CActorParam* actorParam = reinterpret_cast<CActorParam*>(actor);
        CActorParam_UnkStruct1* unk1 = actorParam->CActorParam_UnkVirtualFunc129();
        if (unk1 != nullptr && unk1->unk50 != nullptr) {
            // Compare byte at offset 0x77 of unk50 against the requested arts type.
            CActorParam_UnkStruct2* unk2 =
                reinterpret_cast<CActorParam_UnkStruct2*>(unk1->unk50);
            if (unk2->unk42[0x77 - 0x42] == static_cast<u8>(artsType)) {
                result.value.intVal = 1;
            }
        }
    }

    vmRetValSet(pThread, &result);
    return 1;
}

/// Script command: set a battle-state entry on an enemy actor. Resolves the
/// actor by id (arg 2) via func_800B8C78 (ENE list lookup), then populates a
/// CBattleStateEntry struct from the remaining optional arguments and
/// dispatches it to CBattleManager::func_800EC8FC.
int setEneBtlState(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int state = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    // Optional arguments with defaults of 0.
    int arg4 = 0;
    if (vmArgOmitChk(pThread, 3) == 0) {
        arg4 = vmArgIntGet(4, vmArgPtrGet(pThread, 3));
    }
    int arg5 = 0;
    int arg6 = 0;
    int arg7 = 0;
    int nextIdx = 4;
    if (vmArgOmitChk(pThread, nextIdx) == 0) {
        arg5 = vmArgIntGet(nextIdx + 1, vmArgPtrGet(pThread, nextIdx));
        nextIdx++;
    }
    if (vmArgOmitChk(pThread, nextIdx) == 0) {
        arg6 = vmArgIntGet(nextIdx + 1, vmArgPtrGet(pThread, nextIdx));
        nextIdx++;
    }
    if (vmArgOmitChk(pThread, nextIdx) == 0) {
        arg7 = vmArgIntGet(nextIdx + 1, vmArgPtrGet(pThread, nextIdx));
        nextIdx++;
    }

    cf::CfObjectActor* actor = func_800B8C78(id);
    if (actor == nullptr) {
        return 0;
    }

    // Build the CBattleStateEntry struct on the stack.
    CBattleStateEntry entry;
    memset(&entry, 0, sizeof(entry));
    entry.unk00 = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(actor) + 0x3f10);
    entry.unk14 = static_cast<s16>(state);
    entry.unk10 = arg5;
    entry.unk16 = static_cast<s16>(arg6);
    entry.unk1C = static_cast<f32>(arg4);
    entry.unk20 = static_cast<f32>(arg7);

    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    func_800EC8FC(bm, actor, &entry, 0);
    return 0;
}

/// Script command: check whether the enemy actor with the given id is
/// performing an arts attack matching the given arts type. Mirrors
/// onPcArtsAttack but resolves the actor via func_800B8C78 (ENE list lookup)
/// and includes extra mode-based logic that checks the battle manager's
/// current target.
int onEneArtsAttack(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int artsType = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    // Optional mode argument (default 0).
    int mode = 0;
    if (vmArgOmitChk(pThread, 3) == 0) {
        mode = vmArgIntGet(4, vmArgPtrGet(pThread, 3));
    }

    cf::CfObjectActor* actor = func_800B8C78(id);

    VMArg result;
    result.type = VM_TYPE_INT;
    result.value.intVal = 2;  // default: not matching

    // Gate on game-manager flag 0x400.
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet__Fi(0x400) != 0) {
        vmRetValSet(pThread, &result);
        return 1;
    }
    if (actor == nullptr) {
        vmRetValSet(pThread, &result);
        return 1;
    }

    // Arts-state sub-object at actor+4 is the CfObjectMove part (stored as
    // owner+0x3E9C by CActorParam ctor); its vtable is the CObjectState chain.
    cf::CObjectState* subObj =
        *reinterpret_cast<cf::CObjectState**>(reinterpret_cast<u8*>(actor) + 4);

    // Try selector 0xa against the sub-object's vtable slot +0x34 value.
    u32 val = *static_cast<u32*>(subObj->CObjectState_UnkVirtualFunc12());

    bool matched = false;
    if (func_80174C98(actor, (int*)&val, 0xa) != 0) {
        matched = true;
    } else {
        // Fall back to vtable slot +0x30 value.
        val = *static_cast<u32*>(subObj->CObjectState_UnkVirtualFunc11());
        if (func_80174C98(actor, (int*)&val, 0xa) != 0) {
            matched = true;
        }
    }

    if (matched) {
        CActorParam* actorParam = reinterpret_cast<CActorParam*>(actor);
        CActorParam_UnkStruct1* unk1 = actorParam->CActorParam_UnkVirtualFunc129();
        if (unk1 != nullptr && unk1->unk50 != nullptr) {
            CActorParam_UnkStruct2* unk2 =
                reinterpret_cast<CActorParam_UnkStruct2*>(unk1->unk50);
            if (unk2->unk42[0x77 - 0x42] == static_cast<u8>(artsType)) {
                // Mode-based target check.
                if (mode == 1) {
                    // Mode 1: check if battle manager's current target sub-object
                    // matches unk50.
                    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                    u8* bmTarget = reinterpret_cast<u8*>(func_800EA444(bm));
                    if (bmTarget != nullptr) {
                        CfCode800F42AC* bmTargetObj =
                            reinterpret_cast<CfCode800F42AC*>(bmTarget);
                        // func_800F477C returns CfUnknownSub*; compare as
                        // UnkStruct2* to match unk50's type.
                        if (reinterpret_cast<CActorParam_UnkStruct2*>(
                                func_800F477C(bmTargetObj)) == unk1->unk50) {
                            result.value.intVal = 1;
                        }
                    }
                } else if (mode == 2) {
                    // Mode 2: same as mode 1 but also requires bit 17 of
                    // bmTarget+0x824 to be set.
                    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                    u8* bmTarget = reinterpret_cast<u8*>(func_800EA444(bm));
                    if (bmTarget != nullptr) {
                        CfCode800F42AC* bmTargetObj =
                            reinterpret_cast<CfCode800F42AC*>(bmTarget);
                        if (reinterpret_cast<CActorParam_UnkStruct2*>(
                                func_800F477C(bmTargetObj)) == unk1->unk50) {
                            u32 bmFlags = *reinterpret_cast<u32*>(
                                bmTarget + 0x824);
                            if (bmFlags & (1u << 17)) {
                                result.value.intVal = 1;
                            }
                        }
                    }
                } else {
                    // Mode 0 (or any other): no target check needed.
                    result.value.intVal = 1;
                }
            }
        }
    }

    vmRetValSet(pThread, &result);
    return 1;
}

/// Script command: set a battle-state entry on a player character actor.
/// Resolves the actor by id (arg 2) via func_800B8B94 (PC list lookup),
/// then populates a CBattleStateEntry struct and dispatches it to
/// CBattleManager::func_800EC8FC. When state == 0xce, the unk18 field is
/// forced to 0xa.
int setPcBtlState(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int state = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    // Optional arguments with defaults of 0. Reading pattern matches
    // setEneBtlState: vmArgOmitChk(pThread, N) gates vmArgIntGet(N+1, ...).
    int arg4 = 0;
    if (vmArgOmitChk(pThread, 3) == 0) {
        arg4 = vmArgIntGet(4, vmArgPtrGet(pThread, 3));
    }
    int arg5 = 0;
    int arg6 = 0;
    int arg7 = 0;
    int nextIdx = 4;
    if (vmArgOmitChk(pThread, nextIdx) == 0) {
        arg5 = vmArgIntGet(nextIdx + 1, vmArgPtrGet(pThread, nextIdx));
        nextIdx++;
    }
    if (vmArgOmitChk(pThread, nextIdx) == 0) {
        arg6 = vmArgIntGet(nextIdx + 1, vmArgPtrGet(pThread, nextIdx));
        nextIdx++;
    }
    if (vmArgOmitChk(pThread, nextIdx) == 0) {
        arg7 = vmArgIntGet(nextIdx + 1, vmArgPtrGet(pThread, nextIdx));
        nextIdx++;
    }

    cf::CfObjectActor* actor = func_800B8B94(id);
    if (actor == nullptr) {
        return 0;
    }

    // Build the CBattleStateEntry struct on the stack.
    CBattleStateEntry entry;
    memset(&entry, 0, sizeof(entry));
    // +0x3f10 on the actor is a u32 handle/id used as the entry base.
    entry.unk00 = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(actor) + 0x3f10);
    entry.unk0C = static_cast<u16>(state);
    entry.unk14 = static_cast<s16>(arg6);
    // (float)(s32) forces MWCC to emit the xoris 0x8000 + fsubs int-via-double
    // conversion pattern instead of a single fctiwz.
    entry.unk20 = (float)(s32)arg4;
    entry.unk24 = (float)(s32)arg7;

    // Special case: when state == 0xce, force unk10 to 0xa.
    if (state == 0xce) {
        entry.unk10 = 0xa;
    } else {
        entry.unk10 = arg5;
    }

    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    func_800EC8FC(bm, actor, &entry, 0);
    return 0;
}

// Script command: clear a battle-state flag on an enemy actor. Resolves the
// actor by id (arg 2) and, if the actor exists, dispatches the clear to the
// battle manager's virtual at +0x20 via func_800F3958 with a second id (arg 3).
// Mirror of clearPcBtlState but uses func_800B8C78 (ene list lookup) instead
// of func_800B8B94 (pc list lookup).
extern "C" int clearEneBtlState(VMThread* pThread) {
    int id1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int id2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    cf::CfObjectActor* actor = func_800B8C78(id1);
    if (actor != nullptr) {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        func_800F3958(bm, actor, id2);
    }
    return 0;
}

// Script command: synchronize two enemy actors by copying a 4-byte field
// from one resolved ene to another. The resolved enes are looked up by id
// (arg 2 = destination, arg 3 = source) via func_800B8C78. The copy
// `*(u32*)((u8*)dest + 0x45B8) = *(u32*)((u8*)src + 0x3F10)` runs only
// when both lookups succeed. The (u8*)obj + literal pattern matches the
// established CBattleState access for deep fields past CfObjectModel /
// CfObjectMove.
extern "C" int synchro(VMThread* pThread) {
    int id1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int id2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    cf::CfObjectActor* dest = func_800B8C78(id1);
    cf::CfObjectActor* src = func_800B8C78(id2);
    if (dest != nullptr && src != nullptr) {
        u32 val = *reinterpret_cast<const u32*>(
            reinterpret_cast<const u8*>(src) + 0x3F10);
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(dest) + 0x45B8) = val;
    }
    return 0;
}

// Script command: teach a learnable art to the player. Looks up an art-id
// mapping in the bdat table ("ac_p" / "u" columns), resolves the actor slot
// via func_8009EC9C, then dispatches the learn to the actor-param handler
// func_800A18A4 with the original art id.
extern "C" int learnArts(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    (void)func_8003AA34();
    void* fp = getFP(lbl_eu_80507FC0);
    u32 val = getBdatStringColumnValue(fp, lbl_eu_80507FC0 + 8, id);
    // u8 spill/reload through a 4-byte array forces MWCC to emit the retail
    // `stw r3, 0x8(r1); lbz r3, 0x8(r1)` pattern; a plain `(u8)val` is
    // folded to `rlwinm r3, r3, 0, 24, 31` and the codegen regresses.
    u8 bytes[4];
    *reinterpret_cast<u32*>(bytes) = val;
    u8 byte = bytes[0];
    cf::CfObjectActor* actor = func_8009EC9C(byte);
    func_800A18A4(actor, id);
    return 0;
}

// Script command: clear a battle-state flag on a player actor. Resolves the
// actor by id (arg 2) and, if the actor exists, dispatches the clear to the
// battle manager's virtual at +0x20 via func_800F3958 with a second id (arg 3).
extern "C" int clearPcBtlState(VMThread* pThread) {
    int id1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int id2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    cf::CfObjectActor* actor = func_800B8B94(id1);
    if (actor != nullptr) {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        func_800F3958(bm, actor, id2);
    }
    return 0;
}
