// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectPc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/code_8025FB10.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
// (getCameraDataBlock__Q22cf13CfGameManagerFv: CfSoundMan.hpp now carries the
// single winning decl from CfGameManagerApi.hpp; only playActorSound is
// needed from it.)
#include "kyoshin/cf/CfSoundMan.hpp"   // cf::CfSoundMan::playActorSound (single decl on owner header)
#include "monolib/scn/CScn.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
// (CtrlObjectParam_GetCharEntry is declared by kyoshin/cf/object/CAIAction.hpp - u16 form;
// including CArtsInfo.hpp's u32 extern "C" here would be illegal overloading)
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "monolib/util/MemManager.hpp"  // canonical mtl::MemManager::allocate
#include "kyoshin/cf/CSuddenCommu.hpp"

namespace cf {
// CfObjectPoint: leaf over CfObject so slot 0x70 is CfObject_notifyEventDone
// (deleted local dummy pad list). novtable - never constructed in this TU.
class __declspec(novtable) CfObjectPoint : public CfObject {
public:
    int validatePointState();
};
}

// TU-local views over the secondary sub-object anchors written by the
// ctor and refreshed by setupActionTable (exact layout; names erased at
// codegen, same addi offsets as the raw byte expressions they replace).
struct PcAiSubView {
    u8 _p[0x3380];
    u8 ai;                  // 0x3380 - CAIAction sub-object anchor
};

// Allocates the 0x44-byte CfResPcImpl resource object and constructs it in
// place on the CfObjectMove subobject (the upcast carries the retail null
// guard), then runs the enable hook.
cf::CfObjectPc::CfObjectPc() {
    u8* res = (u8*)mtl::MemManager::allocate(0x44, CfRes_getAllocHandle());
    if (res != NULL) {
        res = (u8*)__ct__cf_CfResPcImpl(res, static_cast<cf::CfObjectMove*>(this));
    }
    ((CfObjectPcSubFields*)this)->mPtr3F4C = res;
    CfObject_UnkVirtualFunc3();
}

// Releases the move targets through the Pc override; the base destructors
// follow inline.
cf::CfObjectPc::~CfObjectPc() {
    CfObject_releaseMoveTargets();
}

// Per-frame PC setup: refreshes the CfResPcImpl resource object and runs
// slots 0x5EC/0x330/0x5F4; when this is the active player's move sub-object
// also resets the screen. For arts row 4, copies four bdat string bytes into
// +0x1629..+0x162C using the row from CtrlObjectParam_GetCurrentRowKey (the column getter
// returns a string pointer; only its first byte is consumed).
void CfObjectPc_setupFrame(cf::CfObjectPc* obj) {
    CfObjectPcSubFields* f = (CfObjectPcSubFields*)obj;
    CfResPcImpl_loadPcState(f->mPtr3F4C);
    obj->resetArtsState();
    obj->CActorParam_rearmAttackList();
    obj->dispatchPlayerBranch();
    u8* moveSub = (u8*)obj;
    if (obj != NULL) {
        moveSub = (u8*)obj + 0x3E9C;
    }
    if (moveSub == (u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
        UIBattleReleaseAll();
    }
    if (f->field_0x3F28 == 4) {
        // Declared in this order for MWCC register coloring
        // (names/file/data/col -> r31/r30/r29/r28); assigned in retail's
        // runtime order.
        // Declared here for register coloring; the first call references
        // the label directly so MWCC splits the @ha/@l address formation
        // around the call like retail.
        const char* names;
        void* file;
        CfObjectPcArtsData* data;
        int col;
        data = (CfObjectPcArtsData*)CtrlObjectParam_GetCharEntry(f->field_0x3F28);
        file = lbl_eu_80664090;
        col = 4;
        // Signed compare (cmpi) in retail.
        if ((int)CtrlObjectParam_GetCurrentRowKey(data) == 1) {
            col = 0xC;
        }
        // Union memory round-trip (same shape as CBattleState): MWCC homes
        // each union local to the stack, emitting stw/lbz pairs.
        union { u32 w; u8 b; } u0, u1, u2, u3;
        u0.w = getBdatStringColumnValue(file, (const char*)lbl_eu_804FC5EC, col);
        f->field_0x1629 = u0.b;
        names = (const char*)lbl_eu_804FC5EC;
        u1.w = getBdatStringColumnValue(file, names + 7, col);
        f->field_0x162A = u1.b;
        u2.w = getBdatStringColumnValue(file, names + 0xF, col);
        f->field_0x162B = u2.b;
        u3.w = getBdatStringColumnValue(file, names + 0x19, col);
        f->field_0x162C = u3.b;
        CtrlObjectParam_SyncParamFromActorEx((u8*)data, 1);
    }
}

// vf2 override: reset-style init (see CfObjectEne's initEnemyBattleState for the
// sibling shape). All accesses go through the local `self` so MWCC keeps it
// in the saved this-copy register and reassigns it to the +0x44A8 region
// (retail reuses the same callee-saved register for both).
// First call is a Class_-qualified direct bl to CfObjectModel's slot +0x58
// impl (alias: CfObject_initEventState). Keep Class_ form: retail uses bl,
// not virtual dispatch.
int cf::CfObjectPc::CfObject_UnkVirtualFunc2() {
    ((cf::CfObjectModel*)((u8*)this + 0x3E9C))->cf::CfObjectModel::CfObject_UnkVirtualFunc2();
    u8* self = (u8*)this;
    // Retail tests this with a record-form mask keeping only bit 0 (msb):
    // source shape (x & 0x80000000).
    if (!(((CfObjectPcSubFields*)self)->field_0x3F00 & 0x80000000)) {
        ((CfObjectPcSubFields*)self)->field_0x3F08 |= 1;
    }
    this->setPointEnabled(1);
    CfObjectMove_setModelDisplayFlag((cf::CfObjectMove*)((char*)self + 0x3E9C), 1);
    this->CfObjectModel_setModelVisible(1);
    dispatchItemLoad(self, 0x8000000);
    dispatchItemLoad(self, 0x10000000);
    CfObjectMove_setRegionAttached((u8*)((u32)self + 0x3E9C), 1);
    u8* region = self + 0x44A8;
    ColiSetMoveVec2(region, 0, lbl_eu_80666B0C, lbl_eu_80666B10);
    *(u16*)(region + 0xB2) = 100;
    return 1;
}


void cf::CfObjectPc::CfObject_releaseMoveTargets() {
    CActorParam_setupActorState();
    CActorParam_refreshBattleStatus();
    CActorParam_initStatusCounter();
    // Look up the arts data object for this PC's index, then write the
    // per-entry value at +0x17C from the CActorParam virtual 0x28C result.
    CfObjectPcArtsData* artsData = (CfObjectPcArtsData*)CtrlObjectParam_GetCharEntry(
        ((CfObjectPcSubFields*)this)->field_0x3F28);
    CActorParam_copyAll(reinterpret_cast<cf::CActorParam*>(&artsData->field_0x17C),
reinterpret_cast<cf::CActorParam*>(CActorParam_getCopyParam()));
}

void cf::CfObjectPc::CfObject_UnkVirtualFunc3() {
    ((CfObjectPcSubFields*)this)->field_0x4568 = 1;
    dispatchItemLoad((u8*)this, 0x00100000);
    ((CfObjectPcSubFields*)this)->field_0x45C0 = lbl_eu_80666B14;
}

// vf0x5E4: refresh the arts data for this PC's row and reset the sub-object
// pointer word at +0x15E0.
// NOTE: defined as the extern "C" wrapper (not as a member): the adjuster
// thunks reference the global identifier, and the wrapper also satisfies the
// vtable slot for the virtual member (same pattern as enablePcFlag).
void cf::CfObjectPc::CObjectParam_UnkVirtualFunc4() {
    CfObjectPcArtsData* data = (CfObjectPcArtsData*)CtrlObjectParam_GetCharEntry(
        ((CfObjectPcSubFields*)this)->field_0x3F28);
    // Recomputed lookup: retail keeps both results live (the first feeds
    // CtrlObjectParam_RefreshCharEntry across the call, the second forms the +0x3534 pointer).
    CfObjectPcArtsData* data2 = (CfObjectPcArtsData*)CtrlObjectParam_GetCharEntry(
        ((CfObjectPcSubFields*)this)->field_0x3F28);
    ((CfObjectPcSubFields*)this)->field_0x15E0 = (u32)data2 + 0x3534;
    CtrlObjectParam_RefreshCharEntry(data, 0);
    this->dispatchPlayerBranch();
    // NOTE: retail re-materializes addi r3, r30, 0x3E9C before EACH of the
    // three calls below, but MWCC value-numbers the identical receiver
    // conversions into one callee-saved temp regardless of spelling (see
    // MWCC_PATTERNS.md 7j negative result). Known unmatchable residual.
    ((cf::CfObject*)((u8*)this + 0x3E9C))->CfObject_setMoveValue(lbl_eu_80666B18);
    ((cf::CfObject*)((u8*)this + 0x3E9C))->CfObject_setMoveScale(lbl_eu_80666B1C);
    reinterpret_cast<cf::CfObjectMove*>((u8*)this + 0x3E9C)->CfObjectMove_recordMoveValue(lbl_eu_80666B20);
}


// (body defined below under its retail mangled name)

// Vtable slot 16 override (CfObjectMove vtable): the adjuster thunk passes the
// CfObjectMove sub-object, and the vtable caller supplies a second int in r4
// even though the retail mangled name is arg-less. Small args delegate to the
// base CfObjectMove implementation; larger ones forward a sub-object pointer
// to ObjPc_ForwardVf0C_A42C.
void cf::CfObjectPc::CfObjectMove_attachEffectSlot(u32 a, u32 b, u32 c, u32 d, u32 e) {
    if ((int)a < 0x2c) {
        // CfObjectMove sub-object at this+0x3E9C. Forward all five args so
        // r4-r8 stay live into the base Fv forced-name (retail arity Fv).
        CfObjectMove::CfObjectMove_attachEffectSlot(a, b, c, d, e);
    } else {
        u8* obj = ((CfObjectPcSubFields*)this)->mPtr3ED4;
        if (obj != NULL) {
            ObjPc_ForwardVf0C_A42C(reinterpret_cast<cf::CfObjectImplPc*>(obj), a, b, c, d, e);
        }
    }
}

// Scans the first six arts-data entries (indices 0..5) for one whose u16
// flag at +0x1A is non-zero ("occupied"); stops at the first hit.
void cf::CfObjectPc::scanArtsEntries() {
    CfObjectPcArtsData* data = (CfObjectPcArtsData*)CtrlObjectParam_GetCharEntry(
        ((CfObjectPcSubFields*)this)->field_0x3F28);
    for (int i = 0; i <= 5; i++) {
        if (CtrlObjectParam_GetArtsStatsRow(&data->mEntries, i)->field_0x1A != 0) break;
    }
}

// Applies arts entry itemId when it matches the row index at +0x3F28:
// constructs the payload (dest), fills the entry object embedded at
// artsData+0x17C through its vtable slot 0x20C, feeds the result to this
// object's slot 0x21C, then runs slot 0x28C and stores via CActorParam_copyAll.
void CfObjectPc_applyArtsEntry(cf::CfObjectPc* self, void* dest, s32 itemId) {
    if ((u32)itemId == (u32)((CfObjectPcSubFields*)self)->field_0x3F28) {
        __ct__8009ED08(dest, itemId);
        CfObjectPcArtsData* data =
            (CfObjectPcArtsData*)CtrlObjectParam_GetCharEntry(itemId & 0xFFFF);
        self->CActorParam_copyArtsBlock17E4(
            reinterpret_cast<cf::CActorParam*>(&data->field_0x17C)->CActorParam_getArtsDataBlock());
        // Copies the arts entry INTO this object's CActorParam (opposite
        // direction of syncArtsEntry).
        CActorParam_copyAll(reinterpret_cast<cf::CActorParam*>(self->CActorParam_getCopyParam()),
            reinterpret_cast<cf::CActorParam*>(&data->field_0x17C));
    }
}

// Looks up the data object for the index at +0x3F28, then runs its ctor.
void cf::CfObjectPc::CActorParam_UnkVirtualFunc166() {
    __ct__8009F8B8((u8*)CtrlObjectParam_GetCharEntry(((CfObjectPcSubFields*)this)->field_0x3F28));
}

void cf::CfObjectPc::CActorParam_UnkVirtualFunc167() {
    CtrlObjectParam_InitArtsSlotSet((u8*)CtrlObjectParam_GetCharEntry(((CfObjectPcSubFields*)this)->field_0x3F28));
}

int cf::CfObjectPc::resetArtsState() {
    return CtrlObjectParam_LearnArtsSlots((u8*)CtrlObjectParam_GetCharEntry(((CfObjectPcSubFields*)this)->field_0x3F28));
}

// Adjuster-style override (retail mangled name is arg-less Fv but the vtable
// caller supplies the parameter in r4): applies the arts data entry identified
// by the index at +0x3F28, passing the extra value through.
void cf::CfObjectPc::applyArtsParam(int param) {
    if (param == 0) return;
    CtrlObjectParam_ClaimArtsSlotRow((u8*)CtrlObjectParam_GetCharEntry(((CfObjectPcSubFields*)this)->field_0x3F28), param);
}

void cf::CfObjectPc::dispatchPlayerBranch() {
    // getPlayer(0) returns the player's CfObjectMove sub-object; compare it
    // against this object's CfObjectMove base (null-checked pointer adjust)
    // and dispatch to the matching per-frame handler.
    u8* target = (u8*)this;
    if (this != NULL) {
        target += 0x3E9C;
    }
    if (target == (u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
        this->resetActionTable();
    } else {
        this->setupActionTable();
    }
}

// vf0x5F8: reinstall the default action table on the +0x3380 CAIAction
// subobject: clear it, then install entries 0 and 1 (both 6-frame, kinds
// 7 and 0xE, gauge 100).
void cf::CfObjectPc::resetActionTable() {
    u8* self = (u8*)this;
    aiResetTables(self + 0x3380);
    aiActionStoreIndexedBytes(self + 0x3380, 0, 1, 0, 6, 0, 0, 7, 0, 0, 0, 2, 100, 0);
    aiActionStoreIndexedBytes(self + 0x3380, 1, 1, 0, 6, 0, 0, 0xE, 0, 0, 0, 2, 100, 0);
}

void cf::CfObjectPc::setupActionTable() {
    // The three uses of this+0x3380 are written in different syntaxes so MWCC
    // does not CSE the address into a callee-saved register (retail recomputes
    // addi r3, r31, 0x3380 at each call site).
    aiResetTables((u8*)&((PcAiSubView*)this)->ai);
    if (((CfObjectPcSubFields*)this)->field_0x3F28 == 4) {
        // Arts-count bdat row: enable the art column when the string value's
        // first byte is '1' (the value is returned packed; read its low byte).
        CfObjectPcArtsData* artsData = (CfObjectPcArtsData*)CtrlObjectParam_GetCharEntry(
            ((CfObjectPcSubFields*)this)->field_0x3F28);
        u32 val = getBdatStringColumnValue(lbl_eu_806640F4,
            (const char*)lbl_eu_804FC5EC + 0x24, artsData->field_0xC);
        if (*(s8*)&val == 1) {
        aiInstallArts((u8*)&((PcAiSubView*)this)->ai, 0x70, 0xd);
            return;
        }
    }
    aiInstallArts((u8*)&((PcAiSubView*)this)->ai, 0, 0);
}



// Main per-frame update. Gate checks via the +0x4/+0x8 sub-objects decide
// between the cancel path (slot 0x20 of the +0x8 sub-object) and the full
// update: battle-manager list probe, action-table probes 0x65..0x6D/0x35,
// gauge scale refresh (+0x8C slot x global scale), flag OR-ing, and the
// player-only event branches; finally the move-subobject runMoveUpdate
// (Class_-qualified CfObjectMove::UnkVirtualFunc4 for a direct bl).

// Retail helper defined below (stub): gate check feeding the cancel path.
extern "C" int CfObjectPc_refreshGauge(cf::CfObjectPc* self, int flag);

// Retail symbol is the arg-less Fv member; defined as a free function with
// C linkage so the first gate probe reads the state pointer straight off
// the incoming parameter register like retail (lwz r3, 0x4(r3)).
void cf::CfObjectPc::CfObject_UnkVirtualFunc4() {
    CfObjectPcSubFields* f = (CfObjectPcSubFields*)this;
    // Two gate probes against the +0x4 sub-object state getter, written as
    // single expressions (CSuddenCommu idiom): the state word is copied into
    // a local through the pointer returned by slot 0x30.
    void* st1;
    void* st2;
    void* st3;
    cf::CObjectState* s4a = *(cf::CObjectState**)((u8*)this + 4);
    int chk1 = testActorStatus(this, (int*)&(st1 = *(void**)s4a->getStateData()), 0xE);
    cf::CObjectState* s4b = *(cf::CObjectState**)((u8*)this + 4);
    int chk2 = testActorStatus(this, (int*)&(st2 = *(void**)s4b->getStateData()), 0x803);

    int any = (chk1 | chk2) != 0;
    int dd = CfObjectPc_refreshGauge(this, any);
    if ((any | dd) != 0) {
        if (isBattleStatusActive((u8*)this + 8, 0x35)) {
            reinterpret_cast<cf::CBattleState*>((u8*)this + 8)->CBattleState_clearStatusId(0x35);
        }
    } else {
        // Count entries in the battle-manager list at +0x48.
        BmListNode* head =
            &((BattleMgrListView*)getInstance__Q22cf14CBattleManagerFv())
                ->mListHead;
        int count = 0;
        for (BmListNode* n = head->mNext; n != head; n = n->mNext) {
            count++;
        }
        // Busy with any listed action -> clear action 0x35; idle -> install it.
        if (count != 0 ||
            isBattleStatusActive((u8*)this + 8, 0x65) ||
            isBattleStatusActive((u8*)this + 8, 0x66) ||
            isBattleStatusActive((u8*)this + 8, 0x67) ||
            isBattleStatusActive((u8*)this + 8, 0x67) ||
            isBattleStatusActive((u8*)this + 8, 0x69) ||
            isBattleStatusActive((u8*)this + 8, 0x6a) ||
            isBattleStatusActive((u8*)this + 8, 0x6b) ||
            isBattleStatusActive((u8*)this + 8, 0x6c) ||
            isBattleStatusActive((u8*)this + 8, 0x6d)) {
            if (isBattleStatusActive((u8*)this + 8, 0x35)) {
                reinterpret_cast<cf::CBattleState*>((u8*)this + 8)->CBattleState_clearStatusId(0x35);
            }
        } else {
            if (!isBattleStatusActive((u8*)this + 8, 0x35)) {
                reinterpret_cast<cf::CBattleState*>((u8*)this + 8)->CBattleState_setBattleParam(0x35);
            }
        }
    }

    // Gauge scale: slot 0x8C value times the global scale factor.
    float gauge = reinterpret_cast<cf::CfObjectMove*>((u8*)this + 0x3E9C)->CfObject_getMoveSpeedRate();
    float scaled = Scn_GetFrameDelta(gmScene) * gauge;
    CActorParam_updateTick(this, scaled, 1);

    cf::CObjectState* s4c = *(cf::CObjectState**)((u8*)this + 4);
    int chkFlag = testActorStatus(this, (int*)&(st3 = *(void**)s4c->getStateData()), 1);
    if (chkFlag != 0 && f->mPtr3F60 != NULL) {
        f->mPtr3F60->field_0x4EC |= 0x1000;
    }

    // Player-only event handling (null-checked adjust to the move sub-object).
    u8* moveSub = (u8*)this;
    if (this != NULL) {
        moveSub += 0x3E9C;
    }
    if (moveSub == (u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
        getInstance__Q22cf13CfGameManagerFv();
        if (isGlobalCamFlagSet__Fi(0x100000)) {
            // Distinct fake types per +0x3E9C dispatch so MWCC re-materializes
            // addi r3, r31, 0x3E9C like retail instead of caching one temp.
            if (chkFlag == 0 && this->checkStateFlags(1) == 0) {
                if (f->field_0x3F34 != NULL) {
                    f->field_0x3F34->field_0x7A4 |= 0x8000;
                }
                ((cf::CfObject*)((u8*)this + 0x3E9C))->CfObject_refreshSubB0();
                ((cf::CfObject*)((u8*)this + 0x3E9C))->CfObject_updateMoveRate();
                return;
            }
        } else {
            if (chkFlag != 0 && UIWin_HasPendingUpdate(1) == 0) {
                dispatchItemLoad(this, 3);
                this->CObjectParam_signalActionEnd(0);
                reinterpret_cast<cf::CfObjectMove*>((u8*)this + 0x3E9C)->CfObjectModel_bindModelTo(0,
                    (const char*)lbl_eu_804FC5EC + 0x2D);
            }
        }
    }
    // Retail: direct bl to CfObjectMove::UnkVirtualFunc4 (alias runMoveUpdate).
    // Class_ keeps the non-virtual bl; alias virtual-dispatch would grow size.
    ((cf::CfObjectMove*)((u8*)this + 0x3E9C))->cf::CfObjectMove::CfObject_UnkVirtualFunc4();
}

// Gauge-scale refresh: runs the CActorParam base handler, then reads a text
// literal through slot 0x308 and scales/divides +0x1824 by an indexed s16
// from it, normalized by the three float constants.
void cf::CfObjectPc::CActorParam_resetArtsStatus(void* arts) {
    CfObjectPcSubFields* f = (CfObjectPcSubFields*)this;
    // Direct (non-virtual) call to the CActorParam base implementation.
    ((cf::CActorParam*)this)->cf::CActorParam::CActorParam_resetArtsStatus(nullptr);
    s16 scale[5] = {20, 10, 0, 10, 20};
    int n = this->CActorParam_getStatusCount();
    if (n <= 1) {
        int idx = this->CActorParam_getStatusCount();
        // (float)(s32) builtin cast: MWCC emits the 0x43300000 biased-magic
        // conversion (single-rounded fsubs) whose pooled constant is the
        // retail lbl_eu_80666B30 literal.
        float v = (float)scale[idx];
        f->field_0x1824 =
            f->field_0x1824 * (lbl_eu_80666B24 + v / lbl_eu_80666B28);
    } else {
        int n2 = this->CActorParam_getStatusCount();
        if (n2 >= 3) {
            int idx = this->CActorParam_getStatusCount();
            float v = (float)scale[idx];
            f->field_0x1824 =
                f->field_0x1824 / (lbl_eu_80666B24 + v / lbl_eu_80666B28);
        }
    }
}

void cf::CfObjectPc::CActorParam_UnkVirtualFunc173() {}

// CActorParam virtual 0x354 override. The retail mangled name is arg-less
// (Fv) but the dispatcher passes a float in f1; forward it to the base
// implementation (same naming quirk) after touching the battle singleton.
void cf::CfObjectPc::CActorParam_decayArtsMatrix(float value) {
    getInstance__Q22cf14CBattleManagerFv();
    cf::CActorParam::CActorParam_decayArtsMatrix(value);
}

// Retail symbol CActorParam_getCurrencyBalance__Q22cf10CfObjectPcFv (vtable slot
// 0x1EC override). Written as a global with the mangled name: the base
// CActorParam override returns u32 in retail but the base header declares it
// void, so a real member override would fail to compile.
int cf::CfObjectPc::CActorParam_getCurrencyBalance() {
    // Arts count from the param object (vtable 0xFC); the arts table index is
    // count+1, capped at 99 entries. Returns 1 when over the cap.
    void* bdat = lbl_eu_806640DC;
    u32 idx = this->CActorParam_getArtsLevel() + 1;
    if (idx > 0x63) return 1;
    u32 sval = getBdatStringColumnValue(bdat,
        (const char*)lbl_eu_804FC5EC + 0x34, idx);
    return sval - this->CActorParam_getSpentCurrency();
}

// Vtable caller supplies three args despite the arg-less Fv retail name.
// Adds arg1/arg2 to the two clamped counters (+0x1604/+0x1608), refreshes
// state via slots 0x290/0x35C/0x5EC, then notifies the effect target.
void cf::CfObjectPc::CActorParam_UnkVirtualFunc88(u32 arg1, u32 arg2, u32 arg3) {
    CfObjectPcSubFields* f = (CfObjectPcSubFields*)this;
    CfObjectPcArtsData* arts =
        (CfObjectPcArtsData*)CtrlObjectParam_GetCharEntry(f->field_0x3F28);
    // Clamp cap 0x5F5E0FF; keep the subtraction inline at each use so MWCC
    // recomputes it (lis/subi) instead of caching it in a register.
    u32 v1604 = f->field_0x1604 + (u32)arg1;
    f->field_0x1604 = v1604;
    if (v1604 > 0x05F60000u - 0x1F01u) {
        f->field_0x1604 = 0x05F60000u - 0x1F01u;
    }
    u32 v1608 = f->field_0x1608 + (u32)arg2;
    f->field_0x1608 = v1608;
    if (v1608 > 0x05F60000u - 0x1F01u) {
        f->field_0x1608 = 0x05F60000u - 0x1F01u;
    }
    Obj89cField* obj = (Obj89cField*)this->CActorParam_getStatusTable();
    Counter_AddClamped((CUnk8025FB10*)(u8*)obj, obj->field_0x89C, arg3);
    int acted = 0;
    // Drain the action queue through slot 0x35C.
    while (this->CActorParam_consumeCurrencyStep() != 0) {
        acted = 1;
        CtrlObjectParam_BumpArtsCounter((u8*)arts, 1);
    }
    BpsStateQueueDamage(f->field_0x3F10, arg1, arg2, arg3);
    if (arg3 != 0 && CtrlObjectParam_ReapplyArtsSet((u8*)arts) != 0) {
        fireLvUpMode4(f->field_0x3F10);
    }
    if (acted != 0) {
        fireLvUpCombo(f->field_0x3F10, this->resetArtsState());
        void* state;
        void* state2;
        if (((BattleMgrRangeView*)getInstance__Q22cf14CBattleManagerFv())->isInRange()) goto actedDone;
        if (((BattleMgrRangeView*)getInstance__Q22cf14CBattleManagerFv())->field_0x20C8 != 0) goto actedDone;
        state = *(void**)(*(cf::CObjectState**)((u8*)this + 4))->getStateData();
        if (testActorStatus(this, (int*)&state, 6) ||
            ((state2 = *(void**)(*(cf::CObjectState**)((u8*)this + 4))->getStateData()),
                testActorStatus(this, (int*)&state2, 9))) {
            CfObjectMove_setAnimModeArgs((u8*)this + 0x3E9C, 0x1B, 0, 6, 1);
        }
    actedDone:;
    }
    CtrlObjectParam_SyncParamFromActorEx((u8*)arts, 0);
    releaseVisionSlot((u8*)this, 0);
}

// Global definition under the retail mangled name: the base CActorParam
// declares this virtual returning void, so an int-returning member override
// would not compile.
int cf::CfObjectPc::CActorParam_UnkVirtualFunc178() {
    CfObjectPcSubFields* f = (CfObjectPcSubFields*)this;
    int spent = this->CActorParam_getCurrencyBalance();
    if (spent <= 0) {
        CfObjectPcArtsData* data =
            (CfObjectPcArtsData*)CtrlObjectParam_GetCharEntry(f->field_0x3F28);
        CtrlObjectParam_RaiseArtsLevelBy((u8*)data, 1);
        this->CActorParam_copyArtsBlock1650(
            (const void*)reinterpret_cast<cf::CActorParam*>(&data->field_0x17C)->CActorParam_getArtsDataBlock());
        u32 gained = this->CActorParam_getSpentCurrency();
        u32 cur = f->field_0x1600;
        u32 total = (u32)spent + gained;
        // Best-of-N (19 formulations tried: inline chains x4 orders,
        // signed/mixed-sign casts, compound assigns, guard-folded store,
        // volatile pins, decl/textual reorders). MWCC collapses every
        // variant to "add r4, r4, r0"; retail has "add r4, r0, r4".
        total = cur + total;
        f->field_0x1600 = total;
        if (total > 0x05F60000u - 0x1F01u) {
            f->field_0x1600 = 0x05F60000u - 0x1F01u;
        }
        f->field_0x1604 = -spent;
        return 1;
    }
    return 0;
}

// Retail CfObjectPc_refreshGauge: per-frame gauge/damage refresh gate.
// 1) global gate word, 2) resource flags bits 11/23 drive cancel-style
// dispatches, 3) battle-manager state gates, 4) scans bdat rows for the
// entry matching (level byte, key halfword), then applies the scaled
// damage to the gauge (+0x45C0) with sound feedback. Returns 1 when the
// matched row was applied.
// C linkage: retail's symbol is the unmangled CfObjectPc_refreshGauge.
extern "C" int CfObjectPc_refreshGauge(cf::CfObjectPc* self, int flag) {
    if ((gmGameFlags & 0xAFA40000u) != 0) {
        return 0;
    }
    if (((CfObjectPcSubFields*)self)->mPtr3F60 != NULL) {
        if (((CfObjectPcSubFields*)self)->mPtr3F60->field_0x4EC & 0x100000) {
            // slot 0x210 false -> primary slot 0x608 cancel call
            if (!self->CfObjectMove_isEffectSlotUsed(0x25)) {
                self->CfObjectMove_attachEffectSlot(0x25, 0, -1, 0, 0);
            }
        } else {
            if (self->CfObjectMove_isEffectSlotUsed(0x25)) {
                self->CfObjectMove_transferSlotBits(0x25);
            }
        }
        if ((((CfObjectPcSubFields*)self)->mPtr3F60->field_0x4EC & 0x100) == 0 ||
            self->CActorParam_isBattleLocked() != 0) {
            goto resetGauge;
        }
        BattleMgrRangeView* bm =
            (BattleMgrRangeView*)getInstance__Q22cf14CBattleManagerFv();
        if (bm->field_0x1AA >= 1 && bm->field_0x1AA <= 0x18) {
            return 0;
        }
        if (SuddenCommuIsStateActive((u8*)bm + 0x216C)) {
            return 0;
        }
        if (((BattleMgrRangeView*)getInstance__Q22cf14CBattleManagerFv())
                ->field_0x20C8 != 0) {
            return 0;
        }
        u32 fourB0 =
            ((CfObjectPcSubFields*)self)->mPtr3F60->field_0x4B0;
        // key lookup mirrors CfObjectEne.cpp: area word indexes a table at
        // gmGlobalFlag44 with a x100 byte stride; result adds the offset back.
        u32 modeOff = (u32)gmGlobalFlag42 * 100;
        int channel = (int)*(u16*)((u8*)&gmGlobalFlag44 + modeOff) + (int)modeOff;
        Bdat_GetTable_AA34();
        void* mgr = lbl_eu_806640D4;
        int i = Bdat_GetRowBase_B41C(mgr);
        int end = i + Bdat_GetMaxRow_B1EC(mgr);
        const char* names = (const char*)lbl_eu_804FC5EC;
        for (; i < end; i++) {
            u32 vA = getBdatStringColumnValue(mgr, names + 0x3E, i);
            if ((int)(u8)vA != (int)fourB0) continue;
            u32 vB = getBdatStringColumnValue(mgr, names + 0x45, i);
            if ((int)(u16)vB != channel) continue;
            u32 vC = getBdatStringColumnValue(mgr, names + 0x49, i);
            float sv = self->CfObject_getMoveSpeedRate();
            float newGauge = Scn_GetFrameDelta(gmScene) * sv +
                ((CfObjectPcSubFields*)self)->field_0x45C0;
            ((CfObjectPcSubFields*)self)->field_0x45C0 = newGauge;
            double thr = (double)(int)((vC & 0xFF) * 30);
            if (newGauge < thr) return 1;
            u32 vD = getBdatStringColumnValue(mgr, names + 0x52, i);
            double base = (double)(int)(vD & 0xFF) - lbl_eu_80666B40;
            float dmg = lbl_eu_80666B38 *
                (base * self->CActorParam_getDamageScale());
            if (isBattleStatusActive((u8*)self + 8, 0xE9)) {
                Res10View* e = (Res10View*)findBattleStatusEntry((u8*)self + 8, 0xE9);
                double dT = (double)(int)e->field_0x10;
                dmg = dmg * (lbl_eu_80666B24 -
                    ((dT - lbl_eu_80666B30) / lbl_eu_80666B28));
            }
            float dv = (float)(int)dmg;
            if (dv == lbl_eu_80666B14) dv = lbl_eu_80666B24;
            u8* moveSub = (u8*)self;
            if (self != NULL) {
                moveSub += 0x3E9C;
            }
            if (moveSub != (u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
                float hp = self->CActorParam_getHp();
                if (hp <= dv) {
                    dv = self->CActorParam_getHp() -
                        lbl_eu_80666B24;
                }
            }
            if (dv != lbl_eu_80666B14) {
                self->CActorParam_addHp(-dv);
                cf::CfSoundMan::playActorSound(0, 0x1AE, 0, 0, lbl_eu_80666B24);
            }
            ((CfObjectPcSubFields*)self)->field_0x45C0 = lbl_eu_80666B14;
            return 1;
        }
    }
resetGauge:
    ((CfObjectPcSubFields*)self)->field_0x45C0 = lbl_eu_80666B14;
    return 0;
}














// Returns the object only when its flags word at +0x64 has bit 0x4000 set
// (the "valid" mark); otherwise returns NULL. The C++ name mangles to the
// retail symbol getValidObject__FPv.
UNKTYPE* getValidObject(UNKTYPE* obj) {
    if (obj != NULL) {
        if (((FlagsObj800C1228*)obj)->field_0x64 & 0x4000) {
            return obj;
        }
    }
    return NULL;
}

int cf::CfObjectPoint::validatePointState() {
    CfObject_notifyEventDone();
    return 1;
}

void cf::CfObjectPc::triggerActionRefreshA() { this->dispatchPlayerBranch(); }

void cf::CfObjectPc::triggerActionRefreshB() { this->dispatchPlayerBranch(); }

__declspec(section ".rodata") __attribute__((used, aligned(4)))
const char lbl_eu_804FC5EC[0x5C] = "tp_atk\0tp_arts\0tp_atkdmg\0tp_artsdmg\0wpn_type\0JUhead\0level_exp\0mat_no\0map\0interval\0dmg";

// .sdata2 pool (retail 0x40) as typed consts in retail declaration order.
// lbl_eu_80666B08 has no code use in this TU; it is still part of the retail
// pool. The two doubles are the int/uint-double conversion magic (HI + 2^52).
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80666B08 = 32.2f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80666B0C = 0.6f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80666B10 = 1.8f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80666B14 = 0.0f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80666B18 = 32.2f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80666B1C = 0.1552795f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80666B20 = 16.1f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80666B24 = 1.0f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80666B28 = 100.0f;
__declspec(section ".sdata2") __attribute__((used))
const double lbl_eu_80666B30 = 4503601774854144.0;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80666B38 = 0.01f;
__declspec(section ".sdata2") __attribute__((used))
const double lbl_eu_80666B40 = 4503599627370496.0;

