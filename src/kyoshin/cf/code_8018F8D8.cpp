// Auto-scaffolded catalog TU for kyoshin/cf/code_8018F8D8
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>

// Concurrent-edit shield: CfCam.hpp now carries tentative definitions of
// ml::tau while several headers declare it extern "C"; rename the
// identifier out of the way per include group (this TU never uses it).

#define create__8CScnNw4rFv create__8CScnNw4rFv_party_shadow
#include "kyoshin/cf/code_8018F8D8.hpp"
#undef create__8CScnNw4rFv

#include "kyoshin/CTaskGameImports.hpp"
// NOTE: CfObjectActor.hpp is deliberately NOT included here:
// CAIAction.hpp (its line-5 include) still carries a stale 2-arg declaration
// of getBdatStringColumnValue that collides with the canonical 3-arg decl in
// plugin/ocBdat.hpp (MWCC 10197 "illegal function overloading"). Instead this
// TU includes CfObjectMove.hpp directly for the CfObjectMove_setAnimModeArgs owner decl and
// the other CfObject* declarations CfObjectActor would have provided.
#include "kyoshin/cf/object/CfObjectMove.hpp"  // CfObjectMove_setAnimModeArgs (owner decl)
#include "kyoshin/cf/object/CActorParam.hpp"  // CActorParam real virtuals (0x158/0x18C/0x190/0x2BC/0x308)
// CfGameManager.hpp (via code_8018F8D8.hpp) internally mixes a void* and a
// CBattleManagerView* declaration of this getter; this TU calls none of them,
// so rename it out of the way for every include below.
// (dispatchItemLoad / testActorStatus now have single unified decls on the
// CfMapItemManager owner header; no pre-include renames needed for them.)
// NOTE: CBattleManager.hpp temporarily dropped - its include subtree currently
// mixes a CfCam.hpp tentative definition of ml::tau with extern "C"
// declarations of the same symbol (concurrent-edit conflict).
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "monolib/math/CVec3.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/CfEnumListHolder.hpp"

// Imports used only by CfCmd_PickChain.
extern float lbl_eu_80667A94;
extern u32 lbl_eu_80532A28[3];   // 12-byte member-function-pointer pool entry


// C-linkage (unmangled) helpers referenced by the catalog functions below.
// These retail symbols are defined in other TUs / the linked library.
extern "C" {
    extern u32 lbl_eu_80663E28;
    extern float lbl_eu_80667A70;
    extern float lbl_eu_80667A74;
    extern u16 lbl_eu_80663E3C;
    extern u8  lbl_eu_8066476D;
    int*  CtrlObjectParam_GetSlotTableBase();
    void CfSoundMan_StopAllModes(int mode);
    void* NandPartySnap();
extern "C" void CfRes_tryRefreshSlot298(u32 a, u32 b);
    void func_8008064C__Q22cf13CfGameManagerFv(void* element0, int idx, float* stk);
    void stubEmptyC__Q22cf13CfGameManagerFv(int flag);
    bool isGlobalCamFlagSet__Fi(int mask);
    u32  CtrlRemote_TouchBitByArg(u32 resource);
    int  isBattleStatusActive(void* obj, int id);
    int  ChainTmr_StartUsable_E018(volatile s16* obj, FuncActorRef* arg);
    int pushBackwards(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
    int CfRes_pushRec(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
    void CfScript_ClearReq2();
    void setMonoFlag(int);
    void UIBattleMarkFlags82();
    void CUICfManager_createQueuedMenus();
    void CUICfManager_prepareMenus();
    s32 MiniMapHasGlobalData();
    void lookupResourceByKeys__Q22cf13CfGameManagerFv(void* this_, u32 second, u32 third);

    void func_801AAC78(int arg);
    int  scanBattleObjects();
    int  NandQueueBusy();
    int  UIBattleGetFlagE8();
    void UIBattleReleaseAll();
    int  CfRes_lookupLocalIndex(s32 arg);
    int  CCharVoiceMan_IsNodeActive(u32 value);
    void func_8009EB2C(int a, int b, u8* c);
extern "C" int  CfRes_tryResolveSlot1E4(u16 a, u16 b, int c);
    bool CfRes_markKey(UnkClass_80085334* obj, u32 mode);
    void func_8008566C__Q22cf13CfGameManagerFv(u32 mode, const UnkFloat4* value, u32 third);
    void cleanupMapEffects__Q22cf13CfGameManagerFv();
    int  isSceneReadyForInput__Q22cf13CfGameManagerFv();
    int  resetCameraManager__Q22cf13CfGameManagerFv();
    void recoverFieldState__Q22cf13CfGameManagerFv(bool alternate);

    long GetZealObjPtr();
    // getAnimModelId: canonical u32(void*) decl comes from CfObjectMove.hpp.
    int CBattleMan_FetchVisionObj();
    long __ptmf_cmpr(u32* slot, u32* pmf1, u32* pmf2);
    // get__16CfEnumListHolderFv: canonical void*(void*) decl comes from CPartyStateWin.hpp
    // (included via code_8018F8D8.hpp); call sites cast to CfEnumList90940*.
    u8* getObjectAt(CfEnumList90940* list, int idx);
}

namespace cf {}
using namespace cf;

int CfCmd_Gate8000Fwd(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    if (lbl_eu_80663E24 & 0x8000) {
        pushBackwards(p0, 0x23, p1, p2, p3, p4);
    }
    return 1;
}

int CfCmd_RecoverFwd24(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    if (scanBattleObjects() != 0) {
        lbl_eu_80663E28 &= ~0x20;
        recoverFieldState__Q22cf13CfGameManagerFv(true);
    } else {
        pushBackwards(p0, 0x24, p1, p2, p3, p4);
    }
    return 1;
}

int CfCmd_ClearOrFwd25(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    // Clear the event-pending flag when an event is already active/disabled
    // (busy manager, bit9 latching, or bit7 set); otherwise forward the request.
    if (isSceneReadyForInput__Q22cf13CfGameManagerFv() != 0 ||
        (lbl_eu_80663E24 & 0x400000) ||
        (lbl_eu_80663E28 & 0x1000000)) {
        lbl_eu_80663E24 &= ~0x2000;
    } else {
        pushBackwards(p0, 0x25, p1, p2, p3, p4);
    }
    return 1;
}

// View of the manager object behind CFuncHost::manager for CfCmd_PartyRebuild:
// only the +0xC data region and the +0x24 float are read.
struct FuncHostMgrView {
    u8 _0[0xc];
    u8 field_0xC[0x18];
    float field_0x24;
};

// Party-rebuild event: reposition/notify all players from the shared offset
// table, refresh the nine party slots, then raise the rebuild task events.
int CfCmd_PartyRebuild(CFuncHost* self, u32 p1, u32 p2, u32 p3, u32 p4) {
    // Selector byte defaults to 0 and is only loaded when bit20 is set.
    u32 selectId = 0;
    if (lbl_eu_80663E24 & 0x100000) {
        selectId = lbl_eu_8066476D;
    }
    FuncHostMgrView* mgr = reinterpret_cast<FuncHostMgrView*>(self->manager);
    cf::CfObjectMove* found = func_8007FF6C__Q22cf13CfGameManagerFv(
        lbl_eu_80663E40, mgr->field_0xC, selectId, mgr->field_0x24);
    if (lbl_eu_80663E24 & 0x100000) {
        // Fixed setup: force players into/out of "command" mode (bit 4 at
        // +0x68), then move each by an offset vector before the notify calls.
        float* base = reinterpret_cast<float*>(NandPartySnap());
        u8 tag = lbl_eu_8066476D;
        float ox = lbl_eu_80667A70;
        float oy = lbl_eu_80667A74;
        // Scratch buffers: component-wise sum of the fixed offset vector
        // {ox, oy, ox} and the per-player base entry (MWCC vectorizes the
        // summation with paired singles). Declared pos-first to match
        // retail's stack slot assignment (pos@8, offset@0x14, sum@0x20).
        float pos[3];
        float offset[3];
        float sum[3];
        const float* pOff = offset;
        float* pSum = sum;
        float* pPos = pos;
        for (int i = 0; i < 3; i++) {
            cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
            if (player != 0) {
                if (tag != 0) player->mFlags68 |= 0x10;
                else player->mFlags68 &= ~0x10;
                float* src = &base[i * 4];
                offset[0] = ox;
                offset[1] = oy;
                offset[2] = ox;
                for (int j = 0; j < 3; j++) {
                    pSum[j] = pOff[j] + src[j];
                }
                pPos[0] = pSum[0];
                pPos[1] = pSum[1];
                pPos[2] = pSum[2];
                static_cast<cf::CfObject*>(player)->CfObject_syncMoveTarget(reinterpret_cast<const ml::CVec3*>(pPos));
                player->CfObject_applyMoveYaw(src[3]);
            }
        }
    }

    // Refresh every occupied party slot: resolve its character data, reset
    // the two display sub-object modes (+0x17C slots 0xA4/0xA8) and sync.
    PartySlotList* slotList = reinterpret_cast<PartySlotList*>(CtrlObjectParam_GetSlotTableBase());
    for (int i = 0; i < 9; i++) {
        u32 entry = slotList->slots[i];
        if (entry == 0) continue;
        void* data = CtrlObjectParam_GetCharEntry((u16)entry);
        CtrlObjectParam_RecollectArtsRows(data);
        cf::CActorParam* cdParam = reinterpret_cast<cf::CActorParam*>(reinterpret_cast<u8*>(data) + 0x17c);
        cdParam->CActorParam_resetArtsStatus(NULL);
        cdParam->CActorParam_commitArtsStatus(1);
        CtrlObjectParam_SyncParamFromActor(data);
    }

    if (found != 0) {
        pushBackwards((u32)self, 0xe, 1, 1, 0, 0);
        assignCameraTarget__Q22cf13CfGameManagerFv(reinterpret_cast<u32>(found));
    }
    if (lbl_eu_80663E24 & 0x80000) {
        // Mark all players as needing a full status refresh (bit 28 at +0x68).
        for (int i = 0; i < 3; i++) {
            cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
            if (player == 0) continue;
            player->mFlags68 |= 0x10000000;
        }
    }
    return 0;
}

int CfCmd_LookupResKeys(void* p0, void* p1, cf::CfGameManager* gm, u32* p3) {
    lookupResourceByKeys__Q22cf13CfGameManagerFv(gm, *p3, 0);
    return 0;
}

int CfCmd_PartyMove(CFuncHost* self, u32 a, u32 b, u32 c, u32 d) {
    // The manager pointer is intentionally re-read from self (+0x408) at every
    // use site; retail reloads it each time rather than caching it in one
    // register, which is what fixes the callee-saved register allocation.
    if (self->manager->unk94[0] != 0) {
        CtrlObjectParam_GetSlotTableBase();
        if (lbl_eu_80663E24 & 0x100000) {
            // Fixed setup: force players 0..2 into-or-out-of "command" mode
            // (bit 4 at +0x68) and move them by an offset vector before the
            // per-player notify calls. Declaration order mirrors retail's
            // stack window: pos@0x8, offset@0x14, sum@0x20.
            float* base = reinterpret_cast<float*>(NandPartySnap());
            u8 tag = lbl_eu_8066476D;
            float ox = lbl_eu_80667A70;
            float oy = lbl_eu_80667A74;
            float pos[3];
            float offset[3];
            float sum[3];
            // Loop-invariant scratch pointers: these compile to retail's
            // pre-loop addi rD,sp,imm materialization (pos@0x8, offset@0x14,
            // sum@0x20).
            float* pPos = pos;
            float* pOff = offset;
            float* pSum = sum;
            int byteOfs = 0;
            for (int i = 0; i < 3; i++) {
                cf::CfObjectMove* p = cf::CfGameManager::getPlayer(i);
                if (p != 0) {
                    if (tag != 0) p->mFlags68 |= 0x10;
                    else p->mFlags68 &= ~0x10;
                    const float* src = &base[i * 4];
                    offset[0] = ox;
                    offset[1] = oy;
                    offset[2] = ox;
                    // Component-wise sum of the fixed offset vector and the
                    // per-player base row (retail vectorizes with paired singles).
                    pSum[0] = pOff[0] + src[0];
                    pSum[1] = pOff[1] + src[1];
                    pSum[2] = pOff[2] + src[2];
                    pPos[0] = pSum[0];
                    pPos[1] = pSum[1];
                    pPos[2] = pSum[2];
                    static_cast<cf::CfObject*>(p)->CfObject_syncMoveTarget(reinterpret_cast<const ml::CVec3*>(pPos));
                    p->CfObject_applyMoveYaw(src[3]);
                }
                byteOfs += 0x10;
            }
        } else {
            // Alternate path: only players 1..2, gated on the busy flag.
            // Scratch matches retail's sp+0x2c..0x38 window (vec3 + pad).
            float stk[4];
            for (int i = 1; i < 3; i++) {
                cf::CfObjectMove* p = self->manager->unk94[i];
                if (p != 0) {
                    func_8008064C__Q22cf13CfGameManagerFv(self->manager->unk94[0], i, stk);
                    if (!(lbl_eu_80663E28 & 0x100)) {
                        static_cast<cf::CfObject*>(p)->CfObject_syncMoveTarget(reinterpret_cast<const ml::CVec3*>(stk));
                        float ccRet90940 = self->manager->unk94[0]->CfObject_getMoveHeadAngle();
                        p->CfObject_setMoveYaw(ccRet90940);
                    }
                }
            }
        }
    }

    cf::CfObjectMove* e0 = self->manager->unk94[0];
    if (e0 != 0 && e0->CfObject_isMoveActiveNow() == 0) {
        pushBackwards((u32)self, 4, a, b, c, d);
        return 1;
    }

    if (lbl_eu_80663E24 & 0x200) {
        lbl_eu_80663E3C = 0;
        lbl_eu_80663E24 = (lbl_eu_80663E24 & ~0x200) | 0x8;
    }
    if (lbl_eu_80663E24 & 0x400000) {
        // rlwinm 19,17 clears BOTH bits 0x2000 and 0x1000.
        lbl_eu_80663E24 &= ~0x3000;
    } else {
        if (b == 0) CfRes_pushRec((u32)self, 0x25, 0, 0, 0, 0);
    }
    if (lbl_eu_80663E24 & 0x100000) {
        pushBackwards((u32)self, 0x24, 0, 0, 0, 0);
        pushBackwards((u32)self, 6, 2, 0, 0, 0);
        CfRes_pushRec((u32)self, 0x1f, 0, 0, 0, 0);
    }
    lbl_eu_80663E24 = (lbl_eu_80663E24 & 0xffefbfff) | 0x80;
    lbl_eu_80663E28 &= ~0x200;
    CfRes_tryRefreshSlot298(0x35300001, 5);
    CfRes_pushRec((u32)self, 6, 0xf, 0, 0, 0);
    pushBackwards((u32)self, 0xf, 0, 0, 0, 0);
    stubEmptyC__Q22cf13CfGameManagerFv(1);
    return 0;
}

int CfCmd_Call82C48(CFuncHost* self) {
    cf::CfGameManager* gm =
        reinterpret_cast<cf::CfGameManager*>(self->manager->unk94[0]);
    gm->func_80082C48();
    return 0;
}

int CfCmd_Stub0060(void* self) { return 0; }

int CfCmd_Stub0068(void* self) { return 0; }

int CfCmd_FwdCmd6(u32 p0, int p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (p1 != 0) {
        pushBackwards(p0, 6, (u16)(p1 - 1), p2, p3, p4);
        r = 1;
    }
    return r;
}

int CfCmd_RefreshUI(void* p0, int a, int b) {
    if (a != 0) {
        UIBattleMarkFlags82();
    }
    if (b != 0) {
        CUICfManager_createQueuedMenus();
        CUICfManager_prepareMenus();
    }
    return 0;
}

int CfCmd_Fwd1FOrFlush(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (UIBattleGetFlagE8() != 0) {
        UIBattleReleaseAll();
    } else {
        pushBackwards(p0, 0x1f, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

int CfCmd_Call86778(CFuncHost* self) {
    self->manager->func_80086778();
    return 0;
}

// Colour-unpack helper: convert the four bytes of `color` to normalized
// floats (byte * scale) and hand them to the scene vec4 setter when bit 7 of
// the mode word is clear. Retail leaves arg2 (r4) unset at the call site, so
// the local is intentionally uninitialized.
int CfCmd_FadeColor(u32 p0, u32 p1, u32 color) {
    const float scale = lbl_eu_80667A78;
    u8* c = reinterpret_cast<u8*>(&color);
    float cr = c[0] * scale;
    float cg = c[1] * scale;
    float cb = c[2] * scale;
    float ca = c[3] * scale;
    if (!(lbl_eu_80663E28 & 0x1000000)) {
        // Retail leaves arg2 (r4) unconsumed here; forwarding the untouched
        // p1 parameter keeps r4 untouched too.
        CTaskGame_setVec4_tmp v = { cr, cg, cb, ca };
        Scn_ReleaseUnk80(reinterpret_cast<CScnNw4r*>(lbl_eu_80663E14), (int)p1, &v);
    }
    return 0;
}

int CfCmd_CallC0094(int a, int b) { CfSoundMan_StopAllModes(b); return 0; }

int CfCmd_Stub027C(void* self) { return 0; }

unsigned long CfCmd_SetFlag8() {
    lbl_eu_80663E24 |= 8;
    return 0;
}

// Menu-open request dispatcher: forward cmd 0x12 to the task queue unless the
// manager is locked (sentinel id -1 at +0x88, or busy per resetCameraManager), in
// which case abort: clear the pending-event bit, reset the scene state and
// disable the menu input path.
int CfCmd_OpenMenu12(CFuncHost* self, u32 p1, u32 p2, u32 p3, u32 p4) {
    cf::CfGameManager* mgr = self->manager;
    if (mgr->field_0x88 != -1 && resetCameraManager__Q22cf13CfGameManagerFv() == 0) {
        pushBackwards((u32)self, 0x12, p1, p2, p3, p4);
        return 1;
    }
    lbl_eu_80663E24 &= ~0x100;
    // Retail invokes the reset hook with a null this pointer.
    ((cf::CfGameManager*)NULL)->func_80085FB8();
    CfScript_ClearReq2();
    setMonoFlag(0);
    return 0;
}

int CfCmd_Fwd13OrClear(u32 p0, u16 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (p1 != 0) {
        pushBackwards(p0, 0x13, (u16)(p1 - 1), p2, p3, p4);
        r = 1;
    } else {
        lbl_eu_80663E28 &= ~1;
    }
    return r;
}

int CfCmd_Fwd16IfItem(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if ((u32)(u16)p2 + 0x10000 != 0xffff && CCharVoiceMan_IsNodeActive((u16)p2)) {
        pushBackwards(p0, 0x16, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

int CfCmd_SetPAnimMode(void* p0, void* p1) {
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player != 0) {
        CfObjectMove_setAnimModeArgs((u8*)player, (int)p1, 0, 0, 1);
    }
    return 0;
}

int CfCmd_Fwd18IfCam(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    if (resetCameraManager__Q22cf13CfGameManagerFv() == 0) {
        pushBackwards(p0, 0x18, p1, p2, p3, p4);
        return 1;
    }
    return 0;
}

int CfCmd_FadeSetup(void* self) {
    // If the colour-fade is already active (bit30 of lbl28), just clear it.
    // Otherwise set up an all-zero colour mix and dispatch a full-colour
    // request event when the colour-change latch (bit12) is set.
    // Bit 1 (0x2): colour-fade-active latch.
    if (!(lbl_eu_80663E28 & 0x2)) {
        float f = lbl_eu_80667A70;
        UnkFloat4 v;
        v.field_0x0 = f;
        v.field_0x4 = f;
        v.field_0x8 = f;
        v.field_0xC = f;
        func_8008566C__Q22cf13CfGameManagerFv(0x1e, &v, 1);
        if (lbl_eu_80663E24 & 0x80000) {
            CfRes_pushRec((u32)self, 0x1c, 0x28, 0, 0, 0);
        }
    } else {
        // Volatile view forces retail's fresh reload of the flag word here
        // (MWCC would otherwise CSE it with the test above).
        lbl_eu_80663E28 = *(volatile u32*)&lbl_eu_80663E28 & ~0x2;
    }
    cleanupMapEffects__Q22cf13CfGameManagerFv();
    return 0;
}

// Party-menu event dispatcher: cmd 0 resets every player's tint flag and
// closes the party gauges; cmd 0x19 re-triggers the first-player colour flash
// via triggerPlayerEffects; any other cmd forwards (cmd-1) to the task queue.
// Retail keeps the (redundant-looking) per-player cmd==0 re-test inside the
// loop, so it is written out explicitly here.
int CfCmd_PartyMenuDisp(u32 self, u32 cmd, u32 a2, u32 a3, u32 a4) {
    int result = 0;
    if (cmd != 0) {
        if (cmd == 0x19) {
            triggerPlayerEffects__Q22cf13CfGameManagerFv(lbl_eu_80663E48[1],
                                                  lbl_eu_80663E4C[1],
                                                  lbl_eu_80667A88);
        }
        pushBackwards(self, 0x1c, (u16)(cmd - 1), a2, a3, a4);
        result = 1;
    } else {
        for (int i = 0; i < 3; i++) {
            cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
            if (player != 0 && cmd == 0) {
                // Clear the tint-request bit (bit 3 from MSB) then run the
                // +0x168 virtual tint setter and the gauge reset helper.
                player->mFlags68 &= ~0x10000000;
                player->CfObject_syncModelRate(lbl_eu_80667A8C);
                CfObjectMove_setMoveSpeedGated(player, lbl_eu_80667A88);
            }
        }
        CfRes_pushRec(self, 6, 0x1e, 0, 0, 0);
        CfRes_pushRec(self, 0x1d, 0, 0, 0, 0);
    }
    return result;
}

int CfCmd_Fwd1EIfBusy(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (NandQueueBusy() != 0) {
        pushBackwards(p0, 0x1e, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

extern "C" unsigned long CfCmd_Clear80000() {
    lbl_eu_80663E24 &= ~0x80000;
    return 0;
}

int CfCmd_FlushUI() {
    UIBattleMarkFlags82();
    CUICfManager_createQueuedMenus();
    CUICfManager_prepareMenus();
    return 0;
}

int CfCmd_Cond338C8() {
    if (MiniMapHasGlobalData() == 0) {
        CUICfManager_createQueuedMenus();
    }
    return 0;
}

int CfCmd_Fwd1BIfLocal(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (p2 != 0 && CfRes_lookupLocalIndex(p2) == 0) {
        pushBackwards(p0, 0x1b, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

int CfCmd_FadeClear() {
    if (!(lbl_eu_80663E24 & 0x400000)) {
        float f = lbl_eu_80667A70;
        UnkFloat4 v;
        v.field_0x0 = f;
        v.field_0x4 = f;
        v.field_0x8 = f;
        v.field_0xC = f;
        func_8008566C__Q22cf13CfGameManagerFv(0x1e, &v, 0);
        cleanupMapEffects__Q22cf13CfGameManagerFv();
    }
    func_801AAC78(0);
    return 0;
}

int CfCmd_ResolveSlot1E4(CFuncHost408* self, u32 arg1, u32 arg2) {
    // CtrlRemote_GetFileEventIds is declared in CfGameManager.hpp (CfFileEventIdsView*);
    // retail treats the result as this 4-byte pair.
    UnkR31_8019E88* p = reinterpret_cast<UnkR31_8019E88*>(CtrlRemote_GetFileEventIds());
    func_8009EB2C((arg2 >> 20) & 0x7f, (arg2 >> 10) & 0x3ff, &self->field_0x408[0x28]);
    if (CfRes_tryResolveSlot1E4(p->field_0x2, p->field_0x0, 3) == 0) {
        CfRes_markKey(reinterpret_cast<UnkClass_80085334*>(self), 0x28);
    }
    return 0;
}

int CfCmd_Fwd28IfClear(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (!(lbl_eu_80663E28 & 0x2000)) {
        pushBackwards(p0, 0x28, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

// Menu-command pump: pop an 8-byte record off the CfResBuffer ring and
// dispatch it through the member-function-pointer handler table indexed by
// the cmd word's top byte; bits 20-23 hold the popped data-word count and
// bit 19 suppresses dispatch for that record. A handler returning nonzero
// publishes the command byte and ends the pump.
int CfCmd_PumpRingBuf(MenuCmdRingView* buf, u32* outFlag) {
    *outFlag = 0;
    if (!(lbl_eu_80663E28 & 0x10000) && cf::CfGameManager::isSceneLoading()) {
        return buf->field_404 == 0;
    }
    MenuCmdHost* host = reinterpret_cast<MenuCmdHost*>(buf);
    u32 rec[2];
    u32 data[0x20];
    while (buf->field_404 != 0) {
        int result;
        CfResBuf_popRecord(buf, &rec[0], data);
        u32 cmd = rec[0];
        if (!((cmd >> 19) & 1)) {
            result = 1;
            u32 idx = cmd >> 24;
            if (idx < 0x29) {
                result = (host->*lbl_eu_80532838[idx])((u16)cmd, rec[1], data,
                                                       (cmd >> 20) & 0xF);
            }
            if (result != 0) {
                *outFlag = rec[0] >> 24;
                break;
            }
        }
    }
    return buf->field_404 == 0;
}

extern float lbl_eu_80667A90;
// Flat retail name: CtrlAct.cpp declares it as `extern "C" f32 CfCmd_GetThreshold()`.
extern "C" float CfCmd_GetThreshold() { return lbl_eu_80667A90; }

// State-gate probe: refresh the actor's current control word through the
// +0x4 sub-object's slot-0x30 getter, then test it against `gate` via
// testActorStatus. Inlined by MWCC at every retail call site.
static int probeGate90940(FuncActorRef* actor, u32 gate) {
    void* ret = reinterpret_cast<cf::CObjectState*>(actor->field_0004)
                      ->getStateData();
    u32 val = *(u32*)ret;
    return testActorStatus(actor, &val, gate);
}

// Battle-target search used by the party menu: validates the queried actor
// against a long chain of state gates, then sweeps the enum-list of nearby
// actors (tracking distance to each candidate), and finally runs a
// mode-specific selection pass over the closest candidates.
int CfCmd_PickChain(FuncResultRef* self, FuncActorRef* actor, int mode,
                  FuncActorRef* filter) {
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = NULL;

    if (probeGate90940(actor, 0x802) == 0) return 0;
    if (probeGate90940(actor, 1) == 0) return 0;
    cf::CfGameManager::getInstance();
    if (!isGlobalCamFlagSet__Fi(0x100)) return 0;
    if (CtrlRemote_TouchBitByArg(0x335f) != 0) return 0;
    if (GetZealObjPtr() != 0) return 0;
    if (!isGlobalCamFlagSet__Fi(0x400)) return 0;
    if (!isGlobalCamFlagSet__Fi(0x1000)) return 0;
    if (probeGate90940(actor, 0x1f) == 0) return 0;
    if (probeGate90940(actor, 0x1000) == 0) return 0;
    if (probeGate90940(actor, 0x806) == 0) return 0;

    BmView90940* bm = reinterpret_cast<BmView90940*>(getInstance__Q22cf14CBattleManagerFv());
    if (bm->field_0x1aa == 0) return 0;
    if (bm->field_0x20c8 == 0) return 0;

    // If a talk object is attached its +0x374 word must be set.
    if (actor->field_3f60 != NULL &&
        reinterpret_cast<Sub3F60View*>(actor->field_3f60)->field_0x374 == 0) {
        return 0;
    }
    if (reinterpret_cast<cf::CActorParam*>(actor)->CActorParam_isBattleLocked() != 0) {
        return 0;
    }

    u8* tags = actor->field_0008;
    // None of the busy/exclusion tags may be present; tag 0xB must be.
    if (isBattleStatusActive(tags, 0x9) != 0) return 0;
    if (isBattleStatusActive(tags, 0xf) != 0) return 0;
    if (isBattleStatusActive(tags, 0x10) != 0) return 0;
    if (isBattleStatusActive(tags, 0xa) != 0) return 0;
    if (isBattleStatusActive(tags, 0xb) == 0) return 0;
    // None of the event-lockout tags may be present either.
    if (isBattleStatusActive(tags, 0xeb) != 0) return 0;
    if (isBattleStatusActive(tags, 0xf0) != 0) return 0;
    if (isBattleStatusActive(tags, 0xf1) != 0) return 0;
    if (isBattleStatusActive(tags, 0xf8) != 0) return 0;
    if (probeGate90940(actor, 0xa) == 0) return 0;
    if (probeGate90940(actor, 0xb) == 0) return 0;
    if (probeGate90940(actor, 0x1a) == 0) return 0;
    if (getAnimModelId(actor->field_3f60) == 0x31) return 0;

    int gaugeMax = reinterpret_cast<cf::CActorParam*>(actor)->CActorParam_getSecondGaugeMax();
    int gauge = reinterpret_cast<cf::CActorParam*>(actor)->CActorParam_getSecondGauge();
    if (gaugeMax == gauge) return 0;

    // While in state 5 the actor's voice-height must stay below the cap.
    if (actor->field_3f28 == 5 &&
        reinterpret_cast<cf::CActorParam*>(actor)->CActorParam_getArtsGauge() >=
            lbl_eu_80667A94) {
        return 0;
    }

    self->field_0008 = 0;
    CfEnumListHolder holder(0x20, 0);

    float dists[2];       // sp+0x38
    FuncActorRef* cands[2]; // sp+0x40

    // Candidate sweep: collect actors whose position sub-object differs from
    // the queried one, recording their distance (mag of position delta minus
    // each candidate's bias float). The two-slot arrays intentionally overlap
    // later stack slots like retail when more than two entries appear.
    int count = 0;
    for (int i = 0; i < (int)((CfEnumList90940*)holder.get())->count; i++) {
        u8* basePos = actor ? reinterpret_cast<u8*>(&actor->field_3e9c)
                            : reinterpret_cast<u8*>(NULL);
        u8* data = getObjectAt((CfEnumList90940*)holder.get(), i);
        if (data == basePos) continue;
        data = getObjectAt((CfEnumList90940*)holder.get(), i);
        FuncActorRef* cand = (data != NULL)
            ? reinterpret_cast<FuncActorRef*>(data - 0x3e9c)
            : static_cast<FuncActorRef*>(NULL);
        cands[count] = cand;

        // Component-wise distance between the actor's and the candidate's
        // +0x3E9C positions (slot-0xAC Vec3 getters); MWCC pairs the XY/ZW
        // subtracts into ps_sub.
        u8* posSubA = reinterpret_cast<u8*>(&actor->field_3e9c);
        u8* posSubB = reinterpret_cast<u8*>(&cand->field_3e9c);
        const float* posA = reinterpret_cast<const float*>(
            reinterpret_cast<cf::CfObject*>(posSubA)->CfObject_getPosVector());
        const float* posB = reinterpret_cast<const float*>(
            reinterpret_cast<cf::CfObject*>(posSubB)->CfObject_getPosVector());
        Vec diff;
        diff.x = posB[0] - posA[0];
        diff.y = posB[1] - posA[1];
        diff.z = posB[2] - posA[2];
        float mag = PSVECMag(&diff);
        dists[count] = mag - cand->field_44d8;
        count++;
    }
    // Move the nearer of the first two candidates into slot 0.
    if (cands[1] != NULL && dists[0] > dists[1]) {
        float td = dists[0];
        FuncActorRef* tp = cands[0];
        dists[0] = dists[1];
        cands[0] = cands[1];
        dists[1] = td;
        cands[1] = tp;
    }

#define ACCEPT(v, subFlagSet)                                                  \
    do {                                                                       \
        self->field_000c = cand;                                               \
        if (subFlagSet) self->field_0008 = 1;                                  \
        self->field_0004 = (v);                                                \
        return (v);                                                            \
    } while (0)

    if (mode <= 1) {
        // Mode 0/1: nearest candidate engaged in battle with a free slot.
        const float thr = lbl_eu_80667A90;
        for (int k = 0; k < count; k++) {
            FuncActorRef* cand = cands[k];
            if (reinterpret_cast<cf::CActorParam*>(cand)->CActorParam_isBattleLocked() == 0)
                continue;
            u8* candPos = reinterpret_cast<u8*>(&cand->field_3e9c);
            if (reinterpret_cast<cf::CfObject*>(candPos)->CfObject_queryTargetState() == 0)
                continue;
            if (getAnimModelId(cand->field_3f60) != 5) continue;
            if (probeGate90940(cand, 0x1d) != 0) continue;
            if (bm->field_0x194 < 100) continue;
            if (dists[k] > thr) continue;
            if (filter != NULL && filter != cand) continue;
            ACCEPT(1, false);
        }
    }
    if (mode == 0 || mode == 2) {
        // Mode 0/2: nearest candidate carrying one of the support tags.
        const float thr = lbl_eu_80667A90;
        for (int k = 0; k < count; k++) {
            FuncActorRef* cand = cands[k];
            if (reinterpret_cast<cf::CActorParam*>(cand)->CActorParam_isBattleLocked() != 0)
                continue;
            u8* tags2 = cand->field_0008;
            if (!(isBattleStatusActive(tags2, 0xf) != 0 ||
                  isBattleStatusActive(tags2, 0x9) != 0 ||
                  isBattleStatusActive(tags2, 0xb) != 0))
                continue;
            if (dists[k] > thr) continue;
            if (filter != NULL && filter != cand) continue;
            ACCEPT(2, false);
        }
    }
    if (mode == 0 || mode == 3) {
        // Mode 0/3: nearest candidate with the chain-attack tag ready.
        const float thr = lbl_eu_80667A90;
        for (int k = 0; k < count; k++) {
            FuncActorRef* cand = cands[k];
            if (reinterpret_cast<cf::CActorParam*>(cand)->CActorParam_isBattleLocked() != 0)
                continue;
            if (isBattleStatusActive(cand->field_0008, 0x10) == 0) continue;
            if (probeGate90940(cand, 0x16) != 0) continue;
            if (dists[k] > thr) continue;
            if (filter != NULL && filter != cand) continue;
            ACCEPT(3, false);
        }
    }
    if (mode == 0 || mode == 4) {
        // Mode 0/4: revive-target scan gated on the battle manager's
        // revive-handler registration (member-function-pointer compare).
        int gate = 0;
        if (CBattleMan_FetchVisionObj() != 0 &&
            reinterpret_cast<BmByte261A4*>(bm)->field_0x261a4 == 0) {
            gate = 1;
        }
        int armed = 0;
        if (gate && bm->field_0x194 >= 100) {
            armed = 1;
        }
        if (armed) {
            u32 pmfLocal[3];
            pmfLocal[0] = lbl_eu_80532A28[0];
            pmfLocal[1] = lbl_eu_80532A28[1];
            pmfLocal[2] = lbl_eu_80532A28[2];
            if (__ptmf_cmpr(reinterpret_cast<BmPtmf28354*>(bm)->field_0x28354,
                            pmfLocal, lbl_eu_80532A28) != 0) {
                gate = 2;
            }
        }
        if (gate == 2) {
            if (count == 0) {
                self->field_0004 = 0;
                return 0;
            }
            const float thr = lbl_eu_80667A90;
            for (int k = 0; k < count; k++) {
                FuncActorRef* cand = cands[k];
                if (reinterpret_cast<cf::CActorParam*>(cand)->CActorParam_isBattleLocked() !=
                    0)
                    continue;
                if (dists[k] > thr) continue;
                if (cand->field_3388 & 0x10) continue;
                if (ChainTmr_StartUsable_E018(&bm->field_0x20c8, cand) == 0) continue;
                ACCEPT(4, true);
            }
        }
    }
    if (mode == 0 || mode == 7) {
        // Mode 0/7: nearest candidate available for a top-up action.
        const float thr = lbl_eu_80667A90;
        for (int k = 0; k < count; k++) {
            FuncActorRef* cand = cands[k];
            if (reinterpret_cast<cf::CActorParam*>(cand)->CActorParam_isBattleLocked() != 0)
                continue;
            if (probeGate90940(cand, 0x1d) != 0) continue;
            if (reinterpret_cast<cf::CActorParam*>(cand)->CActorParam_getStatusCount() >=
                2)
                continue;
            if (dists[k] > thr) continue;
            ACCEPT(7, false);
        }
    }
#undef ACCEPT

    {
        int res = self->field_0004;
        return res;
    }
}
