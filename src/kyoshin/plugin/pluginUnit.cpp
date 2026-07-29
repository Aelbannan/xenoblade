// Auto-scaffolded catalog TU for kyoshin/plugin/pluginUnit
// Plugin functions for the YGG VM "unit" subsystem (battle units, arts).
// These are called from script via the function table registered in
// pluginUnitRegist.

#include <math.h>

#include "monolib/vm/yvm2.h"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// C-linkage retail symbols referenced by learnArts / clearPcBtlState.
extern "C" {
    extern const char lbl_eu_80507FC0[];
    extern const char lbl_eu_80535460[];

    // bdat helpers (C-linkage in retail; ocBdat.hpp declares them as C++).
    void* func_8003AA34();
    u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index);

    // Actor param helpers (C-linkage in retail). Typed as cf::CfObjectActor*
    // to satisfy the no_void_ptr lint rule while keeping C linkage.
    cf::CfObjectActor* func_8009EC9C(unsigned short);
    void func_800A18A4(cf::CfObjectActor*, int);
    cf::CfObjectActor* func_800B8B94(int);
    cf::CfObjectActor* func_800B8C78(int);
    void func_800F3958(cf::CBattleManager*, cf::CfObjectActor*, int);
    void func_800EC8FC(cf::CBattleManager*, cf::CfObjectActor*,
                      cf::CBattleStateEntry*, int);
    int func_80174C98(cf::CfObjectActor*, u32*, int);

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

// Cast-only interface for CfObject vtable. MWCC (-RTTI on) places two hidden
// typeinfo slots before the first declared virtual, so we omit _v000/_v004
// and start at 0x8 to make the declared vtable layout match the retail
// CfObjectModel vtable. The slot at 0x128 (CfObject_UnkVirtualFunc54) is the
// one getPcHp/getEneHp call: the recovered header declares it `void`, but the
// retail caller treats the return as float (per PPC ABI f1), so we override
// its signature here. Never constructed — only used for reinterpret_cast.
struct CfObjectHpIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual void _v0AC();
    virtual void _v0B0();
    virtual void _v0B4();
    virtual void _v0B8();
    virtual void _v0BC();
    virtual void _v0C0();
    virtual void _v0C4();
    virtual void _v0C8();
    virtual void _v0CC();
    virtual void _v0D0();
    virtual void _v0D4();
    virtual void _v0D8();
    virtual void _v0DC();
    virtual void _v0E0();
    virtual void _v0E4();
    virtual void _v0E8();
    virtual void _v0EC();
    virtual void _v0F0();
    virtual void _v0F4();
    virtual void _v0F8();
    virtual void _v0FC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual void _v11C();
    virtual void _v120();
    virtual void _v124();
    virtual float vf128();
    virtual float vf12C();
};

/// Script command: return the current HP of a player character (PC) actor,
/// ceiled and converted to int. The actor is resolved by id (arg 2) via
/// func_800B8B94 (pc list lookup); the HP value comes from the
/// CfObjectModel virtual at vtable+0x128 (the recovered header declares it
/// `void`, but the retail caller treats the result as float). On miss,
/// returns -1.
extern "C" int getPcHp(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    cf::CfObjectActor* actor = func_800B8B94(id);
    VMArg result;
    if (actor != nullptr) {
        // Dispatch the CfObjectModel vtable slot at 0x128 through the
        // cast-only interface so MWCC uses r12 for the vptr (matching
        // retail's r12 coloring on the virtual-call path).
        CfObjectHpIf* obj = reinterpret_cast<CfObjectHpIf*>(actor);
        result.type = VM_TYPE_INT;
        // Force ceil(double) and explicit float round before fctiwz.
        float val = (float)ceil(obj->vf128());
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
/// lookup), then reads the max-HP value (vtable+0x12C) and current-HP value
/// (vtable+0x128) from the CfObjectModel vtable, computing
/// `ceil(100.0f * (cur / max))`. On miss, returns -1.
int getPcHpRate(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    CfObjectHpIf* actor = reinterpret_cast<CfObjectHpIf*>(func_800B8B94(id));
    VMArg result;
    if (actor != nullptr) {
        result.type = VM_TYPE_INT;
        // First virtual: vtable+0x12C returns max-HP (saved in f31 by retail).
        float maxHp = actor->vf12C();
        // Second virtual: vtable+0x128 returns current-HP (result in f1, then
        // fdivs against f31 yields cur/max). Order is fixed by retail codegen.
        float curHp = actor->vf128();
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
        CfObjectHpIf* obj = reinterpret_cast<CfObjectHpIf*>(actor);
        result.type = VM_TYPE_INT;
        // Force ceil(double) and explicit float round before fctiwz.
        float val = (float)ceil(obj->vf128());
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
    CfObjectHpIf* actor = reinterpret_cast<CfObjectHpIf*>(func_800B8C78(id));
    VMArg result;
    if (actor != nullptr) {
        result.type = VM_TYPE_INT;
        float maxHp = actor->vf12C();
        float curHp = actor->vf128();
        result.value.intVal = (int)(float)ceil(lbl_eu_80668250 * (curHp / maxHp));
    } else {
        result.type = VM_TYPE_INT;
        result.value.intVal = -1;
    }
    vmRetValSet(pThread, &result);
    return 1;
}
/// Forward declaration for the arts-state sub-object pointer stored at
/// CfObjectActor+4. Cast-only: real layout is recovered in CActorParam.
/// The vtable slots at +0x30 and +0x34 return u32* (dereferenced by caller).
struct ArtsSubObjectIf {
    virtual ~ArtsSubObjectIf() {}
    virtual u32* subFunc0x30() = 0;
    virtual u32* subFunc0x34() = 0;
};

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
    if (func_8006EF04(0x400) != 0) {
        vmRetValSet(pThread, &result);
        return 1;
    }
    if (actor == nullptr) {
        vmRetValSet(pThread, &result);
        return 1;
    }

    // Access the arts-state sub-object at offset 4 of the actor.
    ArtsSubObjectIf* subObj =
        reinterpret_cast<ArtsSubObjectIf*>(*reinterpret_cast<void**>(
            reinterpret_cast<u8*>(actor) + 4));

    // Try selector 0xa against the sub-object's vtable[13] (offset 0x34) value.
    u32 val = *reinterpret_cast<u32*>(subObj->subFunc0x34());

    bool matched = false;
    if (func_80174C98(actor, &val, 0xa) != 0) {
        matched = true;
    } else {
        // Fall back to vtable[12] (offset 0x30) value.
        val = *reinterpret_cast<u32*>(subObj->subFunc0x30());
        if (func_80174C98(actor, &val, 0xa) != 0) {
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
    if (func_8006EF04(0x400) != 0) {
        vmRetValSet(pThread, &result);
        return 1;
    }
    if (actor == nullptr) {
        vmRetValSet(pThread, &result);
        return 1;
    }

    // Access the arts-state sub-object at offset 4 of the actor.
    ArtsSubObjectIf* subObj =
        reinterpret_cast<ArtsSubObjectIf*>(*reinterpret_cast<void**>(
            reinterpret_cast<u8*>(actor) + 4));

    // Try selector 0xa against the sub-object's vtable[13] (offset 0x34) value.
    u32 val = *subObj->subFunc0x34();

    bool matched = false;
    if (func_80174C98(actor, &val, 0xa) != 0) {
        matched = true;
    } else {
        // Fall back to vtable[12] (offset 0x30) value.
        val = *subObj->subFunc0x30();
        if (func_80174C98(actor, &val, 0xa) != 0) {
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
                    u8* bmTarget = reinterpret_cast<u8*>(bm->func_800EA444());
                    if (bmTarget != nullptr) {
                        CfCode800F42AC* bmTargetObj =
                            reinterpret_cast<CfCode800F42AC*>(bmTarget);
                        if (func_800F477C(bmTargetObj) == unk1->unk50) {
                            result.value.intVal = 1;
                        }
                    }
                } else if (mode == 2) {
                    // Mode 2: same as mode 1 but also requires bit 17 of
                    // bmTarget+0x824 to be set.
                    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                    u8* bmTarget = reinterpret_cast<u8*>(bm->func_800EA444());
                    if (bmTarget != nullptr) {
                        CfCode800F42AC* bmTargetObj =
                            reinterpret_cast<CfCode800F42AC*>(bmTarget);
                        if (func_800F477C(bmTargetObj) == unk1->unk50) {
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
    void* actor = func_800B8C78(id1);
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
    void* dest = func_800B8C78(id1);
    void* src = func_800B8C78(id2);
    if (dest != nullptr && src != nullptr) {
        u32 val = *(const u32*)((const u8*)src + 0x3F10);
        *(u32*)((u8*)dest + 0x45B8) = val;
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
    *(u32*)bytes = val;
    u8 byte = bytes[0];
    void* actor = func_8009EC9C(byte);
    func_800A18A4(actor, id);
    return 0;
}

// Script command: clear a battle-state flag on a player actor. Resolves the
// actor by id (arg 2) and, if the actor exists, dispatches the clear to the
// battle manager's virtual at +0x20 via func_800F3958 with a second id (arg 3).
extern "C" int clearPcBtlState(VMThread* pThread) {
    int id1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int id2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    void* actor = func_800B8B94(id1);
    if (actor != nullptr) {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        func_800F3958(bm, actor, id2);
    }
    return 0;
}
