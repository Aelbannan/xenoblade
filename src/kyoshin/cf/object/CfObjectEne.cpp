#include "kyoshin/cf/CBattleManager.hpp"
#include <types.h>
#include "kyoshin/code_8025FB10.hpp"
#include "monolib/scn/CScn.hpp"
#include <cstring>
// harness_catalog.hpp pulls cf/CfGameManager.hpp (via CTaskGameEff.hpp),
// whose extern "C" getInstance__Q22cf14CBattleManagerFv returns
// CBattleManagerView* and conflicts with CAIAction.hpp's void* form (an
// illegal C-linkage overload in this TU). The cpp only needs mtl::MemManager
// (ctor) and getFP (bdat lookups) from that umbrella, so include them
// directly instead - the rest (getBdatStringColumnValue, func_* imports,
// labels) come from the CfObjectEne.hpp / CfObjectActor.hpp chain.
#include "monolib/util/MemManager.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
// dispatchItemLoad / testActorStatus: single unified decls on the owner header.
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "kyoshin/cf/object/CfObjectEne.hpp" // cf::CfObjectEne + CfEneTablePtrs/CfEneTailView/CfEneReloadSlot views

#include "kyoshin/code_801F3BE0.hpp"  // getUnk80664658, GimBind/UnbindEnemyActor (owner)
#include "kyoshin/cf/CSuddenCommu.hpp"
extern "C" void ColiSetMoveVec2(void* region, int flag, f32 a, f32 b);
// CAIAction.cpp C-ABI imports: the +0x3380 CAIAction subobject helpers
// (declared in CfObjectPc.hpp for the Pc TU; this TU does not include it).
extern "C" void aiResetTables(u8* obj);
extern "C" void aiInstallArts(u8* obj, u32 a, u32 b);

// Copy block: 0x00-0x78 (120 bytes), loaded from r4 then stored to self+0x17E4/0x1650
struct ParamCopyBlock {
    u32 f00;
    f32 f04, f08, f0C, f10, f14, f18;
    s16 f1C, f1E, f20;
    f32 f24, f28;
    s16 h2C, h2E, h30, h32, h34, h36, h38, h3A;
    u8 f3C;
    f32 f40, f44, f48, f4C, f50;
    u8 f54, f55, f56, f57, f58;
    f32 f5C;
    u32 f60, f64, f68, f6C, f70, f74;
};



namespace cf {

// us-800ae154. The CfResReloadImpl child is handed the CfObjectMove
// subobject (the upcast carries the retail null guard).
CfObjectEne::CfObjectEne()
    : field_0x45c0(0), field_0x45c6(0), field_0x45c8(0), field_0x45ca(0), field_0x45cc(0.0f) {
    void* mem = mtl::MemManager::allocate(0x20, CfRes_getAllocHandle());
    if (mem != 0) {
        mem = __ct__cf_CfResReloadImpl((CfResReloadImpl*)mem,
                                       (CfResReloadParent*)static_cast<CfObjectMove*>(this));
    }
    mSubObjB0 = mem;
}


// us-800ae320: releases the enemy-gimmick binding (retail calls the getter
// twice) and the move targets; the base destructors follow inline.
CfObjectEne::~CfObjectEne() {
    if (getUnk80664658() != 0) {
        GimUnbindEnemyActor(getUnk80664658(), this);
    }
    CfObject_releaseMoveTargets();
}

} // namespace cf


// us-800ae3f8: slot 0x58 (initialise). Runs the CfObjectModel step, enables
// the point, raises three actor flags and seeds the +0x44A8 collision region.
int cf::CfObjectEne::CfObject_UnkVirtualFunc2() {
    CfObjectModel::CfObject_UnkVirtualFunc2();
    setPointEnabled(1);
    CfObjectMove_setModelDisplayFlag((cf::CfObjectMove*)this, 1);
    dispatchItemLoad(this, 0x100000);
    dispatchItemLoad(this, 0x08000000);
    dispatchItemLoad(this, 0x10000000);
    CfObjectMove_setRegionAttached(static_cast<CfObjectMove*>(this), 1);
    u8* region = (u8*)this + 0x44A8;
    ColiSetMoveVec2(region, 0, 0.6f, 1.8f);
    ((cf::CfEneReloadArea*)region)->field_0xB2 = 0xC8;
    return 1;
}

// Level-table column names; CObjectParam_UnkVirtualFunc4 patches the digit at
// [3] of both with the enemy's level-set letter before the lookups.
char* eneLvColumn = "ene1Lv";
char* eneScaleColumn = "ene1Scale";

// us-800ae4a0: CfObjectEne::updateEnemyBattleState (retail Fv; this in r3). Probes
// the +0x4 sub-object's actor id (vtable +0x30 first word) against
// testActorStatus masks 0x803/0xE; on the clean path runs the +0x8 CBattleState
// subobject slot +0x14 when isBattleStatusActive(&+0x8, 0x35) is clear, else runs
// applyEnemyReward and slot +0x20. Then accumulates `scene time * sub8C` into
// CActorParam_updateTick, and when the game-manager bit 0x100 is set applies the
// +0x3F34 target's bit-0x80000000 flag + subobject slots +0x80/+0x64, else
// runs the CfObjectMove subobject's CfObject_runMoveUpdate (UVF4 / +0x60).
void cf::CfObjectEne::CfObject_UnkVirtualFunc4() {
    // unk4 = Move subobject (this+0x3E9C); +0x30 = CObjectState::UVF11.
    u32 wordA = *(u32*)reinterpret_cast<cf::CObjectState*>(
        reinterpret_cast<cf::CActorState*>(this)->unk4)
        ->getStateData();
    u32 wordB;
    if (testActorStatus((void*)this, (int*)&wordA, 0x803) == 0 &&
        ((wordB = *(u32*)reinterpret_cast<cf::CObjectState*>(
              reinterpret_cast<cf::CActorState*>(this)->unk4)
              ->getStateData()),
         testActorStatus((void*)this, (int*)&wordB, 0xE)) == 0) {
        if (isBattleStatusActive(&((cf::CfEneB8View*)this)->field_0x8, 0x35) == 0) {
            // +0x8 CBattleState: slot +0x14 = UnkVirtualFunc4.
            ((cf::CBattleStateAt8*)this)->CBattleState_setBattleParam(0x35);
        }
    } else {
        applyEnemyReward();
        if (isBattleStatusActive(&((cf::CfEneB8View*)this)->field_0x8, 0x35) != 0) {
            // +0x8 CBattleState: slot +0x20 = UnkVirtualFunc7.
            ((cf::CBattleStateAt8*)this)->CBattleState_clearStatusId(0x35);
        }
    }
    // local pins the scene-time source across the UVF15 call into a
    // callee-saved reg (retail r31); the subobject address is re-derived per
    // site through distinct expression shapes (member-view / u32-domain /
    // byte-add) so GVN cannot merge them into one cached register
    CScn* sceneTimeSrc = gmScene;
    f32 sub = ((cf::CfObjectAt3E9C*)this)->CfObject_getMoveSpeedRate();
    // retail multiplies as time * sub (fmuls f1,f1,f31 with f31=sub)
    f32 t = Scn_GetFrameDelta(sceneTimeSrc);
    f32 f = t * sub;
    CActorParam_updateTick((u8*)this, f, 1);
    getInstance__Q22cf13CfGameManagerFv();
    // retail flattens the nested guards into two branches on one shared
    // else label (the CfObject_runMoveUpdate / UVF4 call); goto pins that shape
    if (isGlobalCamFlagSet__Fi(0x1000000) == 0)
        goto defaultPath;
    {
        u32 wordC = *(u32*)reinterpret_cast<cf::CObjectState*>(
            reinterpret_cast<cf::CActorState*>(this)->unk4)
            ->getStateData();
        if (testActorStatus(this, &wordC, 0x1) != 0)
            goto defaultPath;
        u8* p = ((cf::CfEneField3F34*)this)->field_0x3F34;
        if (p != 0)
            ((cf::CfEneField7A4*)p)->field_0x7A4 |= 0x80000000;
        // distinct expression families keep MWCC from CSE-ing +0x3E9C
        // (slots +0x80/+0x64 = UVF12/UVF5)
        ((cf::CfObject*)((u8*)this + 0x3E9C))->CfObject_refreshSubB0();
        ((cf::CfObject*)(u32)&((cf::CfEneTablePtrs*)this)->vt3E9C)->CfObject_updateMoveRate();
    }
    goto done;
defaultPath:
    // retail: direct bl to the concrete implementation (not a vdispatch)
    CfObjectMove::CfObject_UnkVirtualFunc4();
done:;
}



// us-800ae674: CfObjectEne::loadEnemyParams (retail Fv; this in r3). Full
// enemy-parameter setup from two bdat tables. The row id (+0x3F28) is
// RE-LOADED before every column read (the bdat calls may modify it - retail
// emits one lhz per call), so it is never cached in a local.
void cf::CfObjectEne::CObjectParam_UnkVirtualFunc4() {
    cf::CfEneLookupView* v = (cf::CfEneLookupView*)this;
    cf::CfEneParamsView* p = (cf::CfEneParamsView*)this;
    // Union-typed locals stay in memory (MWCC), reproducing retail's
    // stw-home + narrow-reload pattern around every column call. Declared at
    // top so each gets a distinct frame home in first-use order.
    cf::CfEneColNarrow c5, cA, c10, c17, c37, c3F, c46, c53, c60, c6A, c80;
    cf::CfEneColNarrow c8B, c96a, c96t, c96b, cA2, cA5, cAC, cB0, cB4, cBA;
    cf::CfEneColNarrow cC9, cD7, cDF, cE7, cF0, u0A, u102, u10B, u115, u11F;
    cf::CfEneColNarrow u129, elseA, w132, w139, base13F, n1634, c159, c165;
    cf::CfEneColNarrow c16F, c17B, c18B, c191;
    // arts-table pointer is materialised before the getFP call (retail r25)
    u8* bdatArts = lbl_eu_80664094;
    void* bdat = getFP(v->field_0x3F14);
    // +0x3E9C CfObjectMove: slot +0x3C = CObjectParam_setObjectName.
    ((cf::CObjectParamAt3E9C*)this)->CObjectParam_setObjectName(
        (const char*)getBdatStringColumnValue(bdatArts, "name", v->field_0x3F28));
    c5.w = getBdatStringColumnValue(bdat, "size", v->field_0x3F28);
    this->CActorParam_setEnemyType(c5.b);
    cA.w = getBdatStringColumnValue(bdat, "scale", v->field_0x3F28);
    this->CActorParam_propagateScale((f32)(u32)cA.b / 10.0f);
    c10.w = getBdatStringColumnValue(bdat, "family", v->field_0x3F28);
    this->CActorParam_setActorType(c10.b);
    c17.w = getBdatStringColumnValue(bdat, "named", v->field_0x3F28);
    p->field_0x15F0 = (u32)c17.b;
    u32 flags = p->field_0x3374 | (u32)getBdatStringColumnValue(bdat, "flag", v->field_0x3F28);
    p->field_0x3374 = flags;
    // rlwinm bit 20 -> mask 0x00100000
    if (!(flags & 0x00100000)) {
        // k22/k2d live across later calls: retail keeps them in callee-saved regs
        u8 k22 = (u8)getBdatStringColumnValue(bdat, "move_speed", v->field_0x3F28);
        // slots +0x1D4/+0x134/+0x13C = Move UVF4 / Object UVF57 / UVF59
        ((cf::CfObjectMoveAt3E9C*)this)->CfObjectMove_recordMoveValue((f32)(s32)(u32)k22);
        u8 k2d = (u8)getBdatStringColumnValue(bdat, "run_speed", v->field_0x3F28);
        ((cf::CfObjectAt3E9C*)this)->CfObject_setMoveValue((f32)(s32)(u32)k2d);
        ((cf::CfObjectAt3E9C*)this)->CfObject_setMoveScale(1.0f);
        if (k22 == 0 || k2d == 0)
            p->field_0x3374 |= 0x1000;
    }
    c37.w = getBdatStringColumnValue(bdat, "detects", v->field_0x3F28);
    this->CActorParam_setParam161C(c37.b);
    c3F.w = getBdatStringColumnValue(bdat, "assist", v->field_0x3F28);
    this->CActorParam_setFlag1628(c3F.b != 0);
    c46.w = getBdatStringColumnValue(bdat, "search_range", v->field_0x3F28);
    this->CActorParam_setGaugeFloat((f32)(u32)c46.h);
    c53.w = getBdatStringColumnValue(bdat, "search_angle", v->field_0x3F28);
    this->CActorParam_setGauge1624((f32)(u32)c53.h * ml::deg2rad);
    this->CActorParam_setGaugeFlagA(1);
    this->CActorParam_setGaugeFlagB(1);
    this->CActorParam_setGaugeRateA(4.0f);
    this->CActorParam_setGaugeRateB(4.0f);
    this->CActorParam_setMoveRate(2.0f * (ml::pi / 3.0f));
    c60.w = getBdatStringColumnValue(bdat, "faint_grd", v->field_0x3F28);
    p->field_0x1648 |= c60.b;
    c6A.w = getBdatStringColumnValue(bdat, "down_grd", v->field_0x3F28);
    p->field_0x164A |= c6A.b;
    c80.w = getBdatStringColumnValue(bdat, "elem_tol_dir", v->field_0x3F28);
    p->field_0x164C |= c80.h;
    c8B.w = getBdatStringColumnValue(bdat, "anti_state", v->field_0x3F28);
    // +0x8 CBattleState: slots +0x7C/+0x84 = UnkVirtualFunc30/32.
    ((cf::CBattleStateAt8*)this)->CBattleState_addStatusBits4(c8B.h);
    c96a.w = getBdatStringColumnValue(bdat, "resi_state", v->field_0x3F28);
    ((cf::CBattleStateAt8*)this)->CBattleState_addStatusBits6(c96a.h);
    c96t.w = getBdatStringColumnValue(bdat, "front_angle", v->field_0x3F28);
    if (c96t.b != 0) {
        c96b.w = getBdatStringColumnValue(bdat, "front_angle", v->field_0x3F28);
        this->CActorParam_setHitRange((ml::pi / 180.0f) * (f32)(u32)c96b.b);
    }
    cf::CActorParam_UnkStruct3* g = (cf::CActorParam_UnkStruct3*)this->CActorParam_getArtsDataBlock();
    cA2.w = getBdatStringColumnValue(bdat, "lv", v->field_0x3F28);
    g->field_00 = (u32)cA2.b;
    cA5.w = getBdatStringColumnValue(bdat, "hp", v->field_0x3F28);
    f32 w = (f32)(s32)((u32)cA5.h * 100);
    g->field_10 = w;
    g->field_04 = w;
    g->field_0C = 0.0f;
    g->field_18 = 100.0f;
    cAC.w = getBdatStringColumnValue(bdat, "str", v->field_0x3F28);
    g->field_1C = (s16)cAC.h;
    cB0.w = getBdatStringColumnValue(bdat, "agi", v->field_0x3F28);
    g->field_1E = (s16)cB0.h;
    cB4.w = getBdatStringColumnValue(bdat, "eth", v->field_0x3F28);
    g->field_20 = (s16)cB4.h;
    cBA.w = getBdatStringColumnValue(bdat, "delay", v->field_0x3F28);
    g->field_40 = (f32)(u32)cBA.b / 10.0f;
    cC9.w = getBdatStringColumnValue(bdat, "hit_range_near", v->field_0x3F28);
    g->field_44 = (f32)(u32)cC9.h / 100.0f;
    cD7.w = getBdatStringColumnValue(bdat, "hit_range_far", v->field_0x3F28);
    g->field_48 = (f32)(u32)cD7.h / 100.0f;
    cDF.w = getBdatStringColumnValue(bdat, "dbl_atk", v->field_0x3F28);
    g->field_56 = cDF.b;
    cE7.w = getBdatStringColumnValue(bdat, "cnt_atk", v->field_0x3F28);
    g->field_57 = cE7.b;
    cF0.w = getBdatStringColumnValue(bdat, "elem_phx", v->field_0x3F28);
    g->field_60 = (u16)cF0.b;
    g->field_62 = (u16)(u8)getBdatStringColumnValue(bdat, "elem_eth", v->field_0x3F28);
    // three redundant column reads follow; the first feeds the +0x73 byte from
    // the raw return (clrlwi 28), the next two are dead with constant stores
    g->unk70[3].unk0 = (u8)(getBdatStringColumnValue(bdat, "elem_tol", v->field_0x3F28) & 0xF);
    getBdatStringColumnValue(bdat, "elem_tol", v->field_0x3F28);
    g->unk70[0].unk0 = 0;
    getBdatStringColumnValue(bdat, "elem_tol", v->field_0x3F28);
    g->unk70[2].unk0 = 0;
    p->field_0x1629 = 0;
    p->field_0x162A = 0;
    p->field_0x162B = 3;
    p->field_0x162C = 7;
    u16 w456c = ((cf::CfObjectEne456CView*)this)->field_0x456C;
    s32 half = (s32)w456c >> 4;
    if (half != 0) {
        // level-name letter written into both name strings at +3
        u8 letter = (u8)((w456c & 0xF) + 0x30);
        eneLvColumn[3] = letter;
        eneScaleColumn[3] = letter;
        u8* bdat9c = lbl_eu_8066409C;
        s8 sn = (s8)getBdatStringColumnValue(bdat9c, eneScaleColumn, (u16)half);
        u0A.w = getBdatStringColumnValue(bdat, "scale", v->field_0x3F28);
        s8 s2 = (s8)getBdatStringColumnValue(bdat9c, eneScaleColumn, (u16)half);
        u102.w = getBdatStringColumnValue(bdat, "Lv_up_hp", v->field_0x3F28);
        u10B.w = getBdatStringColumnValue(bdat, "Lv_up_str", v->field_0x3F28);
        u115.w = getBdatStringColumnValue(bdat, "Lv_up_agi", v->field_0x3F28);
        u11F.w = getBdatStringColumnValue(bdat, "Lv_up_eth", v->field_0x3F28);
        u129.w = getBdatStringColumnValue(bdat9c, "ene_type", u11F.h);
        s32 sum = (s32)u0A.b + (s32)s2;
        // rlwinm bit 26 -> mask 0x20 picks between raw and scaled HP scaling
        if (u129.b & 0x20) {
            this->CActorParam_propagateScale((f32)(s32)sum);
        } else {
            this->CActorParam_propagateScale(0.1f * (f32)(s32)sum);
        }
        g->field_00 = g->field_00 + (u32)sn;
        g->field_04 = (f32)sn * (f32)u102.h + g->field_04;
        g->field_1C = (s16)((f32)sn * (f32)u10B.h + (f32)g->field_1C);
        g->field_1E = (s16)((f32)sn * (f32)u115.h + (f32)g->field_1E);
        g->field_20 = (s16)((f32)sn * (f32)u11F.h + (f32)g->field_20);
        if (g->field_00 < 1) g->field_00 = 1;
        if (g->field_04 < 1.0f) g->field_04 = 1.0f;
        if (g->field_1C < 1) g->field_1C = 1;
        if (g->field_1E < 1) g->field_1E = 1;
        if (g->field_20 < 1) g->field_20 = 1;
        g->field_10 = g->field_04;
    } else {
        elseA.w = getBdatStringColumnValue(bdat, "scale", v->field_0x3F28);
        this->CActorParam_propagateScale(0.1f * (f32)(s32)(u32)elseA.b);
        p->field_0x3F00 |= 0x04000000;
    }
    u32 cap = g->field_00;
    if (cap > 0x63) cap = 0x63;
    u8* bdatDC = (u8*)lbl_eu_806640DC;
    w132.w = getBdatStringColumnValue(bdatDC, "en_exp", cap);
    w139.w = getBdatStringColumnValue(bdatDC, "en_ap", cap);
    base13F.w = getBdatStringColumnValue(bdat, "exp", v->field_0x3F28);
    s32 base = (s32)base13F.h + 10;
    // mulhwu 0xcccd/srwi 3 division: unsigned 32-bit domain throughout
    p->field_0x1604 = (u32)((u32)w132.h * (u32)base) / 10;
    p->field_0x1608 = (u32)((u32)w139.h * (u32)base) / 10;
    n1634.w = getBdatStringColumnValue(bdat, "spike_elem", v->field_0x3F28);
    p->field_0x1634 = (u32)n1634.b;
    if (n1634.b != 0) {
        // kind lives across later calls: retail keeps it in a callee-saved reg
        u8 kind = (u8)getBdatStringColumnValue(bdat, "spike_type", v->field_0x3F28);
        c159.w = getBdatStringColumnValue(bdat, "spike_range", v->field_0x3F28);
        p->field_0x1638 = (u32)c159.b;
        c165.w = getBdatStringColumnValue(bdat, "spike_dmg", v->field_0x3F28);
        p->field_0x163C = (u32)(c165.b * 10);
        c16F.w = getBdatStringColumnValue(bdat, "spike_state", v->field_0x3F28);
        p->field_0x1640 = (u32)c16F.b;
        c17B.w = getBdatStringColumnValue(bdat, "spike_state_val", v->field_0x3F28);
        p->field_0x1644 = (u32)c17B.b;
        if (kind == 2) p->field_0x1634 = p->field_0x1634 + 5;
        if (kind == 3) p->field_0x1634 = p->field_0x1634 + 10;
        // rlwinm bit 13 -> mask 0x2000 gates the first setup-buffer dispatch
        if (!(p->field_0x3374 & 0x2000)) {
            cf::CfEneSetupBuf bufa;
            std::memset(&bufa, 0, 0x34);
            bufa.field_0x0 = p->field_0x3F10;
            bufa.field_0x4 = 0;
            bufa.field_0x8 = 0x2000;
            bufa.field_0xC = (u16)p->field_0x1634;
            bufa.field_0x10 = p->field_0x163C;
            bufa.field_0x14 = (u16)p->field_0x1638;
            bufa.field_0x16 = (u16)p->field_0x1640;
            bufa.field_0x18 = (u16)p->field_0x1644;
            bufa.field_0x20 = 0.0f;
            bufa.field_0x30 = 1;
            ((cf::CBattleStateAt8*)this)->CBattleState_enterStatusEntry(
                (cf::CBattleStateEntry*)&bufa);
        }
    }
    c18B.w = getBdatStringColumnValue(bdat, "limit", v->field_0x3F28);
    if (c18B.b != 0) {
        cf::CfEneSetupBuf bufb;
        std::memset(&bufb, 0, 0x34);
        bufb.field_0x0 = p->field_0x3F10;
        bufb.field_0x4 = 0;
        bufb.field_0xC = 0xC3;
        bufb.field_0x10 = (u32)c18B.b;
        bufb.field_0x20 = 0.0f;
        ((cf::CBattleStateAt8*)this)->CBattleState_enterStatusEntry(
            (cf::CBattleStateEntry*)&bufb);
    }
    c191.w = getBdatStringColumnValue(bdat, "drop_vlb", v->field_0x3F28);
    p->field_0x3370 = (u32)c191.h;
    this->CActorParam_copyArtsBlock17E4((const void*)this->CActorParam_getArtsDataBlock());
    CActorParam_UnkVirtualFunc167();
    CActorParam_UnkVirtualFunc166();
    this->initEnemyBdatParams();
    // rlwinm bit 12 -> mask 0x1000
    if (p->field_0x3374 & 0x1000) {
        p->field_0x3F04 |= 0x20000000;
    }
    // retail calls the getter twice (test + argument)
    if (getUnk80664658() != 0) {
        GimBindEnemyActor(getUnk80664658(), this);
    }
}



// Wave-62: CActorParam_setEnemyType (+0xE4) / setActorType (+0xDC) base
// impls. Free-function Fv forms keep the Unk linker names that hand-built
// tables / CREvtModel reference.
void cf::CActorParam::CActorParam_setEnemyType(u32 val) { this->unk15E4 = val; }



void cf::CActorParam::CActorParam_setActorType(u32 val) { this->unk15EC = val; }



void cf::CfObjectMove::CfObject_setMoveScale(float val) { this->mField6EC = val; }



// Wave-62: CActorParam_setParam161C / setFlag1628 / setGauge1624 / setHitRange
// base impls. Free-function Fv forms keep the Unk linker names.
void cf::CActorParam::CActorParam_setParam161C(u32 val) { this->unk161C = val; }



void cf::CActorParam::CActorParam_setFlag1628(u8 val) {
    this->unk1628 = val;
}



void cf::CActorParam::CActorParam_setGauge1624(float val) { this->unk1624 = val; }



void cf::CActorParam::CActorParam_setHitRange(float val) { this->unk1630 = val; }



void cf::CActorParam::CActorParam_UnkVirtualFunc98(const void* src) {
    *(ParamCopyBlock*)&this->unk17E4 = *(const ParamCopyBlock*)src;
}



// us-800af534: CfObjectEne::initEnemyBdatParams (retail Fv; this in r3). Resets
// the bdat manager, reads the enemy bdat column value (getFP on the
// +0x3F14 name, row id at +0x3F28), multiplies it by the vtable +0xF0
// result and the 0x806669B0 scale, feeds the vtable +0xD4 slot, runs the
// primary vtable +0x288 slot six times and resets the +0x3380 CAIAction
// subobject (aiResetTables / aiInstallArts).
void cf::CfObjectEne::initEnemyBdatParams() {
    Bdat_GetTable_AA34();
    // addressable union pins the raw column word to a frame home across the
    // slot +0xF0 vcall; the u16 punned read narrows on reload (retail lhz)
    cf::CfEneColNarrow col;
    col.w = getBdatStringColumnValue(getFP(((cf::CfEneLookupView*)this)->field_0x3F14),
                                     "chest_height",
                                     ((cf::CfEneLookupView*)this)->field_0x3F28);
    f32 base = this->CActorParam_getSubScale();
    f32 scaled = 0.01f * (f32)col.h;
    this->CActorParam_setStatScale(scaled * base);
    for (int i = 0; i < 6; i++)
        this->CActorParam_getArtsSlotIds();
    // Different syntax families (pointer-add vs &subscript) stop MWCC from
    // CSE-ing the subobject address into a callee-saved register - retail
    // re-materializes addi r3, rX, 0x3380 at each call (cf. pluginBtl.cpp).
    aiResetTables((u8*)this + 0x3380);
    aiInstallArts(&((u8*)this)[0x3380], 0, 0);
}



// us-800af614: CfObjectEne::CActorParam_UnkVirtualFunc166 (retail Fv; this
// in r3). Arts-set loader: reloads the bdat manager, reads the enemy bdat
// information table, writes the eight arts-slot ids (columns 0x1A7..0x1D1,
// step 6) through the primary vtable slot +0x27C arts-set, then for each
// non-zero slot loads the CArtsParam record (getArtsParamByIdx), copies the
// name, fills the typed fields from the bdat columns, dispatches the +0x84
// record vtable slot 2 (+0x8) for slots with (u16)(unk3C - 1) <= 1 and
// unk44 == 0, accumulates the +0x78 flag bits (0x40000000 always,
// 0x80000000 for unk42 == 1) and scales the +0x2C gauge by the 0x8000 bit.
void cf::CfObjectEne::CActorParam_UnkVirtualFunc166() {
    cf::CfEneLookupView* v = (cf::CfEneLookupView*)this;
    Bdat_GetTable_AA34();
    void* bdat = getFP(v->field_0x3F14);
    u16 row = v->field_0x3F28;
    u32 a0 = getBdatStringColumnValue(bdat, "arts1", row);
    setArtsSlotByIdx(this->CActorParam_getArtsSet(), a0, 0);
    u32 a1 = getBdatStringColumnValue(bdat, "arts2", row);
    setArtsSlotByIdx(this->CActorParam_getArtsSet(), a1, 1);
    u32 a2 = getBdatStringColumnValue(bdat, "arts3", row);
    setArtsSlotByIdx(this->CActorParam_getArtsSet(), a2, 2);
    u32 a3 = getBdatStringColumnValue(bdat, "arts4", row);
    setArtsSlotByIdx(this->CActorParam_getArtsSet(), a3, 3);
    u32 a4 = getBdatStringColumnValue(bdat, "arts5", row);
    setArtsSlotByIdx(this->CActorParam_getArtsSet(), a4, 4);
    u32 a5 = getBdatStringColumnValue(bdat, "arts6", row);
    setArtsSlotByIdx(this->CActorParam_getArtsSet(), a5, 5);
    u32 a6 = getBdatStringColumnValue(bdat, "arts7", row);
    setArtsSlotByIdx(this->CActorParam_getArtsSet(), a6, 6);
    u32 a7 = getBdatStringColumnValue(bdat, "arts8", row);
    setArtsSlotByIdx(this->CActorParam_getArtsSet(), a7, 7);

    Bdat_GetTable_AA34();
    void* bdat2 = getFP("ene_arts");
    int i;
    s16 zero46 = 0;  // retail keeps a dedicated zero reg for field_0x46
    for (i = 0; i < 8; i++) {
        u16 slot = getArtsSlotByFlatIdx(this->CActorParam_getArtsSet(), (s16)i);
        if (slot == 0) continue;
        cf::CArtsParam* arts = (cf::CArtsParam*)getArtsParamByIdx(this->CActorParam_getArtsSet(), i);
        char* nm = (char*)getBdatStringColumnValue(bdat2, "name", slot);
        arts->unk20 = strlen(nm);
        strcpy(reinterpret_cast<char*>(arts), nm);
        arts->unk78 = getBdatStringColumnValue(bdat2, "flag", slot);
        arts->unk2C = (f32)(u8)getBdatStringColumnValue(bdat2, "cast", slot) / 10.0f;
        u8 e5 = (u8)getBdatStringColumnValue(bdat2, "recast", slot);
        if (e5 == 0xFF) {
            arts->unk30 = 10000.0f;
        } else {
            e5 = (u8)getBdatStringColumnValue(bdat2, "recast", slot);
            arts->unk30 = (f32)e5;
        }
        arts->unk34 = (s16)(s8)getBdatStringColumnValue(bdat2, "tp", slot);
        arts->unk36 = (s16)(s8)getBdatStringColumnValue(bdat2, "dex", slot);
        ((s16*)arts->unk38)[0] = (s16)getBdatStringColumnValue(bdat2, "rate1", slot);
        ((s16*)arts->unk38)[1] = (s16)getBdatStringColumnValue(bdat2, "rate2", slot);
        arts->unk42 = (u8)getBdatStringColumnValue(bdat2, "arts_type", slot);
        u8 a2b = (u8)getBdatStringColumnValue(bdat2, "lv", slot);
        if (a2b != 0)
            arts->unk2A = (u8)getBdatStringColumnValue(bdat2, "lv", slot);
        arts->unk3C = (u16)(u8)getBdatStringColumnValue(bdat2, "atk_type", slot);
        arts->unk40 = (u16)(u8)getBdatStringColumnValue(bdat2, "elem", slot);
        arts->unk43 = (u8)getBdatStringColumnValue(bdat2, "dmg_type", slot);
        arts->unk44 = (u8)getBdatStringColumnValue(bdat2, "dmg_time", slot);
        arts->unk46 = zero46;
        // simple attack classes (field_0x3C in {0,1}) without the 0x44 flag are
        // initialized through the record vtable and skipped
        if ((u16)(arts->unk3C - 1) <= 1 && arts->unk44 == 0) {
            ((cf::CAttackParam*)arts)->CAttackParam_clearArtsRecord();
            continue;
        }
        arts->unk58 = (u16)(u8)getBdatStringColumnValue(bdat2, "kb_type", slot);
        arts->unk5A = (u16)(u8)getBdatStringColumnValue(bdat2, "kb_lv", slot);
        arts->unk5C = (u16)(u8)getBdatStringColumnValue(bdat2, "tgt", slot);
        arts->unk5E = (u16)(u8)getBdatStringColumnValue(bdat2, "range_type", slot);
        u16 w246 = (u16)getBdatStringColumnValue(bdat2, "range", slot);
        f32 rateBonus = ((cf::CActorParam_UnkStruct3*)this->CActorParam_getArtsDataBlock())->field_44;
        arts->unk60 = (f32)w246 / 100.0f + rateBonus;
        arts->unk64 = (u16)getBdatStringColumnValue(bdat2, "range_val", slot);
        arts->unk76 = (u8)getBdatStringColumnValue(bdat2, "act_idx", slot);
        arts->unk77 = (u8)getBdatStringColumnValue(bdat2, "idx", slot);
        arts->unk48 = (u16)getBdatStringColumnValue(bdat2, "st_type", slot);
        arts->unk4A = (u16)(u8)getBdatStringColumnValue(bdat2, "st_val", slot);
        arts->unk4C = (u16)(u8)getBdatStringColumnValue(bdat2, "st_val2", slot);
        arts->unk50 = (f32)(u16)getBdatStringColumnValue(bdat2, "st_time", slot) / 10.0f;
        arts->unk54 = (f32)(u8)getBdatStringColumnValue(bdat2, "st_itv", slot) / 10.0f;
        arts->unk66 = (u8)getBdatStringColumnValue(bdat2, "sp_cnd", slot);
        arts->unk67 = (u8)getBdatStringColumnValue(bdat2, "sp_proc", slot);
        arts->unk68 = (s16)getBdatStringColumnValue(bdat2, "sp_val1", slot);
        arts->unk6A = (s16)getBdatStringColumnValue(bdat2, "sp_val2", slot);
        arts->unk78 |= 0x40000000;
        if (arts->unk42 == 1)
            arts->unk78 |= 0x80000000;
        if (arts->unk78 & 0x8000)
            arts->unk2C *= 100.0f;
    }
}


// us-800afd5c: CfObjectEne::CActorParam_UnkVirtualFunc167 (retail Fv; this
// in r3). Attack-set loader: reloads the bdat manager, reads the enemy bdat
// table (getFP on +0x3F14 / row +0x3F28), writes the six attack-slot ids
// (columns 0x2A7/0x2AC/0x2B1, the last three zeroed) through the primary
// vtable slot +0x288 u16 array, then for each non-zero slot loads the
// CAttackParam record (getAtkParam), copies the name, fills the typed
// fields from the bdat columns, picks the +0x60 gauge from the +0x20C rate
// (+0x48, or +0x44 when the +0x48 is 0.0f) and scales the +0x64 attack
// count by the +0x1C4 rate * ml::rad2deg.
void cf::CfObjectEne::CActorParam_UnkVirtualFunc167() {
    Bdat_GetTable_AA34();
    void* bdat = getFP(((cf::CfEneLookupView*)this)->field_0x3F14);
    // each column result is a u32 frame temp live across the slot +0x288
    // virtual call that publishes it (retail spills to 0x2c/0x28/0x24(sp))
    u32 s0 = getBdatStringColumnValue(bdat, "atk1", ((cf::CfEneLookupView*)this)->field_0x3F28);
    ((u16*)this->CActorParam_getArtsSlotIds())[0] = (u16)s0;
    u32 s1 = getBdatStringColumnValue(bdat, "atk2", ((cf::CfEneLookupView*)this)->field_0x3F28);
    ((u16*)this->CActorParam_getArtsSlotIds())[1] = (u16)s1;
    u32 s2 = getBdatStringColumnValue(bdat, "atk3", ((cf::CfEneLookupView*)this)->field_0x3F28);
    ((u16*)this->CActorParam_getArtsSlotIds())[2] = (u16)s2;
    ((u16*)this->CActorParam_getArtsSlotIds())[3] = 0;
    ((u16*)this->CActorParam_getArtsSlotIds())[4] = 0;
    ((u16*)this->CActorParam_getArtsSlotIds())[5] = 0;

    Bdat_GetTable_AA34();
    u8* bdat2 = (u8*)getFP("ene_atk");
    f32 zero = 0.0f;
    f32 scale = ml::rad2deg;
    for (int i = 0; i < 6; i++) {
        // name column addresses the raw symbol; the rest go through the
        // offset copy (retail keeps both r29/r30 base forms)
        u16 atkId = ((u16*)this->CActorParam_getArtsSlotIds())[(s16)i];
        if (atkId == 0) continue;
        cf::CAttackParam* atk = (cf::CAttackParam*)getAtkParam(((u16*)this->CActorParam_getArtsSlotIds()), i);
        char* nm = (char*)getBdatStringColumnValue(bdat2, "name", atkId);
        atk->unk20 = strlen(nm);
        strcpy(reinterpret_cast<char*>(atk), nm);
        u32 v1D = getBdatStringColumnValue(bdat2, "flag", atkId);
        atk->unk78 = (u16)v1D;
        atk->unk2C = zero;
        atk->unk30 = zero;
        u32 v1EF = getBdatStringColumnValue(bdat2, "dex", atkId);
        atk->unk36 = (s16)(s8)v1EF;
        u32 v2BE = getBdatStringColumnValue(bdat2, "rate", atkId);
        ((s16*)atk->unk38)[0] = (s16)(u8)v2BE;
        ((s16*)atk->unk38)[1] = 0;
        atk->unk42 = 0;
        u32 v209 = getBdatStringColumnValue(bdat2, "atk_type", atkId);
        atk->unk3C = (s16)(u8)v209;
        u32 v212 = getBdatStringColumnValue(bdat2, "elem", atkId);
        atk->unk40 = (s16)(u8)v212;
        atk->unk43 = 1;
        atk->unk44 = 1;
        atk->unk46 = 0;
        atk->unk58 = 0;
        atk->unk5A = 0;
        atk->unk5C = 1;
        atk->unk5E = 0;
        f32 g48 = ((cf::CActorParam_UnkStruct3*)this->CActorParam_getArtsDataBlock())->field_48;
        f32 gauge;
        if (g48 == zero) {
            gauge = ((cf::CActorParam_UnkStruct3*)this->CActorParam_getArtsDataBlock())->field_44;
        } else {
            gauge = ((cf::CActorParam_UnkStruct3*)this->CActorParam_getArtsDataBlock())->field_48;
        }
        atk->unk60 = gauge;
        f32 rate = ((cf::CfEneMoveRateView*)this->CActorParam_getMoveRate())->field_0x0;
        atk->unk64 = (s16)(rate * scale);
        u32 v256 = getBdatStringColumnValue(bdat2, "act_idx", atkId);
        atk->unk76 = (u8)v256;
        u32 v25E = getBdatStringColumnValue(bdat2, "idx", atkId);
        atk->unk77 = (u8)v25E;
    }
}



// us-800b00b0: bdat lookup helper. Reads the u16 row id at self+0x456C
// (shifted right 4) and looks up the column value in the BDAT table returned
// by getGlobalPtr6409C. Returns whatever getBdatStringColumnValue produced (the
// retail leaves it in r3).
u32 cf::CfObjectEne::getEneBdatColumn(const char* column) {
    return getBdatStringColumnValue((void*)getGlobalPtr6409C__Q22cf13CfGameManagerFv(), column,
                                    (u16)_6D0 >> 4);
}



// us-800b00f8: bdat lookup helper (`this` is unused; the table is the
// getGlobalWord640A4 result).
u32 cf::CfObjectEne::getEneBdatByIndex(const char* col, u32 index) {
    return getBdatStringColumnValue((u8*)getGlobalWord640A4__Q22cf13CfGameManagerFv(), col, index);
}



// us-800b013c: enemy drop/reward application. Guard chain (flag bit
// 0x100000, shared gmGameFlags bits 0xAFA40000, +0x3F60 target bit 8,
// primary vtable +0x2BC, CBattleManager party-count range and mode words)
// then scans the enemy bdat rows [Bdat_GetRowBase_B41C, +Bdat_GetMaxRow_B1EC) for the
// model id and channel, accumulates `scene time * sub8C + field_0x45CC`
// against the reward threshold (`reward * 30`) and applies the scaled count
// through the primary vtable +0x11C slot before clearing field_0x45CC and
// returning on the first hit.
void cf::CfObjectEne::applyEnemyReward() {
    if ((((cf::CfActorParamFields*)this)->field_0x3374 & 0x100000) != 0) return;
    if ((gmGameFlags & 0xAFA40000) != 0) return;
    if (((cf::CfActorField3F60*)this)->field_0x3F60 == NULL) return;
    // retail reloads the +0x3F60 target for the flag test (short-lived r4)
    if (((((cf::CfEneMoveTgtView*)((cf::CfActorField3F60*)this)->field_0x3F60)->field_0x4EC) & 0x100) == 0) return;
    if (CActorParam_isBattleLocked() != 0) return;
    cf::CfEneBmView* bm = (cf::CfEneBmView*)getInstance__Q22cf14CBattleManagerFv();
    // goto form blocks MWCC's unsigned range-check fusion
    // (cf. CfObjectPc.cpp CItemBlock_testKindFlag)
    int ok = 0;
    if (bm->field_0x1AA < 1) goto bmCheck;
    if (0x18 < bm->field_0x1AA) goto bmCheck;
    ok = 1;
bmCheck:
    if (ok != 0) return;
    if (SuddenCommuIsStateActive(&((cf::CfEneBmView*)getInstance__Q22cf14CBattleManagerFv())->field_0x216C) != 0) return;
    cf::CfEneBmView* bm3 = (cf::CfEneBmView*)getInstance__Q22cf14CBattleManagerFv();
    s16 idx = bm3->field_0x20C8;
    if (idx != 0) return;
    // mode word is read SDA-indexed by the (always-zero) idx register
    u32 modeOff = (u32)*(u16*)((u8*)&gmGlobalFlag42 + idx) * 100;
    int channel = (int)*(u16*)((u8*)&gmGlobalFlag44 + modeOff) + (int)modeOff;
    s32 key = ((cf::CfEneMoveTgtView*)((cf::CfActorField3F60*)this)->field_0x3F60)->field_0x4B0;
    Bdat_GetTable_AA34();
    u8* bdat = (u8*)lbl_eu_806640D4;
    int row = (int)Bdat_GetRowBase_B41C(bdat);
    int endRow = row + (int)Bdat_GetMaxRow_B1EC(bdat);
    for (; row < endRow; row++) {
        u8 model = (u8)getBdatStringColumnValue(bdat, "mat_no", row);
        if ((s32)model != key) continue;
        u16 ch = (u16)getBdatStringColumnValue(bdat, "map", row);
        if ((s32)ch != channel) continue;
        u8 reward = (u8)getBdatStringColumnValue(bdat, "interval", row);
        // pin the scene-time source across the m8C() call so MWCC hoists
        // the SDA load above the bctrl (retail keeps it in a saved reg)
        CScn* sceneSrc = gmScene;
        f32 sub = ((cf::CfObjectAt3E9C*)this)->CfObject_getMoveSpeedRate();
        f32 t = Scn_GetFrameDelta(sceneSrc);
        f32 now = t * sub + this->field_0x45cc;
        // retail stores the accumulated time back BEFORE the threshold compare
        this->field_0x45cc = now;
        f32 thresh = (f32)((s32)reward * 30);
        if (now < thresh) return;
        u8 count = (u8)getBdatStringColumnValue(bdat, "dmg", row);
        f32 n = (f32)count;
        f32 prod = 0.01f * (n * CActorParam_getDamageScale());
        if (prod != 0.0f) {
            s32 iv = (s32)prod;
            f32 rv = (f32)iv;
            if (rv == 0.0f) rv = 1.0f;
            CActorParam_addHp(-rv);
        }
        this->field_0x45cc = 0.0f;
        return;
    }
}












// CActorState setObjStateBit3: unk4 is the Move subobject (this+0x3E9C); slot
// +0x38 = CObjectState::setStateBitMask3.
void cf::CActorState::CActorState_setObjStateBit3() {
    reinterpret_cast<cf::CObjectState*>(this->unk4)->setStateBitMask3();
}



void* cf::CBattleState::CBattleState_getStagingRecord(unsigned long index) {
    return this->unk152C + (index << 4);
}



void* cf::CBattleState::CBattleState_getStatusBlock() { return &this->unk152C; }



void* cf::CBattleState::CBattleState_getEntryArray() { return &this->unk8; }







// us-800b0404: retail copies 0x1520 bytes (0x2a4 * 8) from src to this->unk8
// (offset 0x8) with a counted lwzu/stwu loop - full struct assignment of the
// 0x1520-byte block at CBattleState+0x8.
void cf::CBattleState::CBattleState_copyStatusEntries(const cf::CBattleState* src) {
    *(cf::CBattleStateUnk1520Block*)this->unk8 = *(const cf::CBattleStateUnk1520Block*)src;
}



extern "C" void CBattleState_getStatusEntry__Q22cf12CBattleStateFi(cf::CBattleState* self) {
    self->CBattleState_readStatusSlot();
}



extern "C" void CBattleState_getEventEntry__Q22cf12CBattleStateFi(cf::CBattleState* self) {
    self->CBattleState_getEventSlot();
}



extern "C" void CBattleState_fetchStatusEntry__Q22cf12CBattleStateFi(cf::CBattleState* self) {
    self->CBattleState_fetchStatusSlot();
}



// UVF13 is Fi; keep r4 live into the callee (0x10-byte bctr).
void* cf::CBattleState::CBattleState_getEntryByIndex(int index) {
    return this->CBattleState_getStatusSlot(index);
}



extern "C" void CActorParam_UnkVirtualFunc181__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { self->unk2A80 = val; }



int cf::CActorParam::CActorParam_UnkVirtualFunc178() { return 0; }



void cf::CActorParam::CActorParam_UnkVirtualFunc173() {}



u32 cf::CActorParam::CActorParam_UnkVirtualFunc172() { return this->unk3370; }



float cf::CActorParam::CActorParam_UnkVirtualFunc171() { return this->unk1630; }



float cf::CActorParam::CActorParam_UnkVirtualFunc170() {
    // ml::pi declared (const form) in CfObjectEne.hpp / CChain.hpp
    return ml::pi - this->unk1630;
}



float cf::CActorParam::CActorParam_UnkVirtualFunc169() { return this->unk1630; }



void* cf::CActorParam::CActorParam_getActorModeFlags() { return &this->unk164A; }



void* cf::CActorParam::CActorParam_UnkVirtualFunc163() { return &this->unk1648; }



u32 cf::CActorParam::CActorParam_UnkVirtualFunc162() { return this->unk336C; }



extern "C" void CActorParam_UnkVirtualFunc161__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { self->unk336C = val; }



u16* cf::CActorParam::CActorParam_UnkVirtualFunc151() { return &this->unk3358; }



// us-800b04d0: zeroes the 8-entry CActorParam_UnkStruct5 array at 0x1928.
// Retail is fully unrolled (48 straight stores: the u32 slot then the five
// floats, in the init() order unk14/unk4/unk0/unkC/unk8/unk10 per entry), so
// the 8 explicit init() calls are kept instead of a counted loop.
// Wave-62: override of CActorParam_resetGaugeEntries (+0x2E4). Free-function
// Fv form keeps the Unk linker name that hand-built tables / CREvtModel
// reference; the renamed member decl serves call-site codegen.
void cf::CActorParam::CActorParam_resetGaugeEntries() {
    this->unk1928[0].init();
    this->unk1928[1].init();
    this->unk1928[2].init();
    this->unk1928[3].init();
    this->unk1928[4].init();
    this->unk1928[5].init();
    this->unk1928[6].init();
    this->unk1928[7].init();
}



// us-800b059c: retail copies 0xC0 bytes (0x18 * 8) from src to the 8-entry
// CActorParam UnkStruct5 array at offset 0x1928 (stride 0x18) with a
// counted lwzu/stwu loop.
extern "C" void CActorParam_UnkVirtualFunc146__Q22cf11CActorParamFv(cf::CActorParam* self, const cf::CActorParam* src) {
    *(cf::CActorParamUnk1928Block*)self->unk1928 = *(const cf::CActorParamUnk1928Block*)src;
}



extern "C" void* CActorParam_UnkVirtualFunc145__Q22cf11CActorParamFv(cf::CActorParam* self, unsigned long idx) {
    return &self->unk1928[idx];
}



extern "C" void* CActorParam_UnkVirtualFunc147__Q22cf11CActorParamFv(cf::CActorParam* self) { return &self->unk1928; }



u32 cf::CActorParam::CActorParam_UnkVirtualFunc139() { return this->unk2A80; }



// Wave-62: override of CActorParam_resetStatsSlot (+0x2AC). Free-function
// Fv form keeps the Unk linker name that hand-built tables / CREvtModel
// reference; the renamed member decl serves call-site codegen.
void cf::CActorParam::CActorParam_resetStatsSlot() {
    this->unk3354 = 0;
}

int cf::CActorParam::CActorParam_getStatsSlot() {
    return this->unk3354;
}



void* cf::CActorParam::CActorParam_UnkVirtualFunc131() { return &this->unk31DC; }

void* cf::CActorParam::CActorParam_UnkVirtualFunc130(int index) { return (u8*)this + index * 0xBC + 0x2A84; }



cf::CActorParamStatusTable* cf::CActorParam::CActorParam_UnkVirtualFunc128() { return this->unk15E0; }



void* cf::CActorParam::CActorParam_getAtkSetBlock() { return &this->unk2740; }



 // getAtkSetBlock +0x284

// us-800b0620: CActorParam::copyAtkSetBlock (virtual +0x280). Fv retail
// linker name kept for CREvtModel; real ABI passes (self, r4 = attack-set
// data block): copies the 12-byte header to self+0x2740, then for each of
// the six 0x88-byte CAttackParam records at self+0x2750 stores strlen at
// +0x20, strcpy's the +0x00 name and copies the +0x24..+0x80 typed fields
// (the +0x84 vtable word is skipped).
extern "C" void CActorParam_copyAtkSetBlock__Q22cf11CActorParamFv(cf::CActorParam* self,
                                                         const cf::CfEneAttackData* src) {
    cf::CfEneAttackArea* dst = (cf::CfEneAttackArea*)self;
    const cf::CfEneAttackEntry* s = &src->records[0];
    cf::CfEneAttackEntry* d = &dst->records[0];
    cf::CfEneAttackEntry* end = &dst->records[6];
    // Header copy stays field-by-field (retail: three separate u32 copies
    // stored +4 / +0 / +8).
    u32 t0 = src->header.h0;
    u32 t4 = src->header.h4;
    dst->header.h4 = t4;
    dst->header.h0 = t0;
    dst->header.h8 = src->header.h8;
    do {
        d->field_0x20 = strlen(s->name);
        strcpy(d->name, s->name);
        d->field_0x24 = s->field_0x24;
        d->field_0x28 = s->field_0x28;
        d->field_0x2A = s->field_0x2A;
        d->field_0x2B = s->field_0x2B;
        d->field_0x2C = s->field_0x2C;
        d->field_0x30 = s->field_0x30;
        d->field_0x34 = s->field_0x34;
        d->field_0x36 = s->field_0x36;
        d->field_0x38 = s->field_0x38;
        d->field_0x3C = s->field_0x3C;
        d->field_0x3E = s->field_0x3E;
        d->field_0x40 = s->field_0x40;
        d->field_0x42 = s->field_0x42;
        d->field_0x43 = s->field_0x43;
        d->field_0x44 = s->field_0x44;
        d->field_0x46 = s->field_0x46;
        d->field_0x48 = s->field_0x48;
        d->field_0x4A = s->field_0x4A;
        d->field_0x4C = s->field_0x4C;
        d->field_0x50 = s->field_0x50;
        d->field_0x54 = s->field_0x54;
        d->field_0x58 = s->field_0x58;
        d->field_0x5A = s->field_0x5A;
        d->field_0x5C = s->field_0x5C;
        d->field_0x5E = s->field_0x5E;
        d->field_0x60 = s->field_0x60;
        d->field_0x64 = s->field_0x64;
        d->field_0x66 = s->field_0x66;
        d->field_0x67 = s->field_0x67;
        d->field_0x68 = s->field_0x68;
        d->field_0x6A = s->field_0x6A;
        d->field_0x6C = s->field_0x6C;
        d->field_0x70 = s->field_0x70;
        d->field_0x72 = s->field_0x72;
        d->field_0x74 = s->field_0x74;
        d->field_0x76 = s->field_0x76;
        d->field_0x77 = s->field_0x77;
        d->field_0x78 = s->field_0x78;
        d->field_0x7C = s->field_0x7C;
        d->field_0x80 = s->field_0x80;
        s++;
        d++;
    } while (d < end);
}



void* cf::CActorParam::CActorParam_getArtsSetBlock() { return &this->mArtsSet; }



 // getArtsSetBlock +0x278

// us-800b07e8: CActorParam::copyArtsSetBlock (virtual +0x274). Fv retail
// linker name kept for CREvtModel; real ABI (self, r4 = arts-set data
// block): copies the 0x34-byte arts-set header to self+0x19E8, then for
// each of the 24 0x8C-byte CArtsParam records at self+0x1A20 stores
// strlen at +0x20, strcpy's the name and copies the +0x24..+0x80 fields
// plus the +0x88 word (the +0x84 vtable word is skipped).
extern "C" void CActorParam_copyArtsSetBlock__Q22cf11CActorParamFv(cf::CActorParam* self,
                                                         const cf::CfEneArtsData* src) {
    cf::CfEneArtsArea* dst = (cf::CfEneArtsArea*)self;
    const cf::CfEneArtsEntry* s = &src->records[0];
    cf::CfEneArtsEntry* d = &dst->records[0];
    cf::CfEneArtsEntry* end = &dst->records[24];
    // retail expands the 0x34 header copy field-by-field; volatile keeps each
    // access strictly in program order (plain assignments batch the loads into
    // callee-saved registers, which retail does not emit)
    volatile cf::CfEneArtsHdrBlock* vh = (volatile cf::CfEneArtsHdrBlock*)&dst->header;
    const volatile cf::CfEneArtsSrcHdrBlock* vsh =
        (const volatile cf::CfEneArtsSrcHdrBlock*)&src->header;
    vh->header.field_0 = vsh->header.field_0;
    vh->header.field_2 = vsh->header.field_2;
    // second-assigned temp first: matches retail's r5/r0 coloring
    u32 tB = vsh->header.field_4;
    u32 tA = vsh->header.field_8;
    vh->header.field_8 = tA;
    vh->header.field_4 = tB;
    tB = vsh->header.field_C;
    tA = vsh->header.field_10;
    vh->header.field_10 = tA;
    vh->header.field_C = tB;
    tB = vsh->header.field_14;
    tA = vsh->header.field_18;
    vh->header.field_18 = tA;
    vh->header.field_14 = tB;
    tB = vsh->header.field_1C;
    tA = vsh->header.field_20;
    vh->header.field_20 = tA;
    vh->header.field_1C = tB;
    tB = vsh->header.field_24;
    tA = vsh->header.field_28;
    vh->header.field_28 = tA;
    vh->header.field_24 = tB;
    tB = vsh->header.field_2C;
    tA = vsh->header.field_30;
    vh->header.field_30 = tA;
    vh->header.field_2C = tB;
    do {
        d->field_0x20 = strlen((char*)s->name);
        strcpy((char*)d->name, (const char*)s->name);
        d->field_0x24 = s->field_0x24;
        d->field_0x28 = s->field_0x28;
        d->field_0x2A = s->field_0x2A;
        d->field_0x2B = s->field_0x2B;
        d->field_0x2C = s->field_0x2C;
        d->field_0x30 = s->field_0x30;
        d->field_0x34 = s->field_0x34;
        d->field_0x36 = s->field_0x36;
        d->field_0x38 = s->field_0x38;
        d->field_0x3C = s->field_0x3C;
        d->field_0x3E = s->field_0x3E;
        d->field_0x40 = s->field_0x40;
        d->field_0x42 = s->field_0x42;
        d->field_0x43 = s->field_0x43;
        d->field_0x44 = s->field_0x44;
        d->field_0x46 = s->field_0x46;
        d->field_0x48 = s->field_0x48;
        d->field_0x4A = s->field_0x4A;
        d->field_0x4C = s->field_0x4C;
        d->field_0x50 = s->field_0x50;
        d->field_0x54 = s->field_0x54;
        d->field_0x58 = s->field_0x58;
        d->field_0x5A = s->field_0x5A;
        d->field_0x5C = s->field_0x5C;
        d->field_0x5E = s->field_0x5E;
        d->field_0x60 = s->field_0x60;
        d->field_0x64 = s->field_0x64;
        d->field_0x66 = s->field_0x66;
        d->field_0x67 = s->field_0x67;
        d->field_0x68 = s->field_0x68;
        d->field_0x6A = s->field_0x6A;
        d->field_0x6C = s->field_0x6C;
        d->field_0x70 = s->field_0x70;
        d->field_0x72 = s->field_0x72;
        d->field_0x74 = s->field_0x74;
        d->field_0x76 = s->field_0x76;
        d->field_0x77 = s->field_0x77;
        d->field_0x78 = s->field_0x78;
        d->field_0x7C = s->field_0x7C;
        d->field_0x80 = s->field_0x80;
        d->field_0x88 = s->field_0x88;
        s++;
        d++;
    } while (d < end);
}



extern "C" u8 CActorParam_UnkVirtualFunc111__Q22cf11CActorParamFv(cf::CActorParam* self) {
    // retail: lbz r3, 0x183c(r3) - member block is +8 shifted, raw view pins the offset
    return ((cf::CfObjectEne183AView*)self)->field_0x183C;
}



u32 cf::CActorParam::CActorParam_UnkVirtualFunc110() {
    // retail: lbz r3, 0x183b(r3) - member block is +8 shifted, raw view pins the offset
    return ((cf::CfObjectEne183AView*)this)->field_0x183B;
}



u32 cf::CActorParam::CActorParam_UnkVirtualFunc109() {
    // retail: lbz r3, 0x183a(r3) - member block is +8 shifted, raw view pins the offset
    return ((cf::CfObjectEne183AView*)this)->field_0x183A;
}



extern "C" void CActorParam_UnkVirtualFunc108__Q22cf11CActorParamFv(cf::CActorParam* self, u8 val) {
    // retail: stb r4, 0x183b(r3) - member block is +8 shifted, raw view pins the offset
    ((cf::CfObjectEne183AView*)self)->field_0x183B = val;
}



extern "C" void CActorParam_UnkVirtualFunc107__Q22cf11CActorParamFv(cf::CActorParam* self, u8 val) {
    // retail: stb r4, 0x183a(r3) - member block is +8 shifted, raw view pins the offset
    ((cf::CfObjectEne183AView*)self)->field_0x183A = val;
}



extern "C" void CActorParam_UnkVirtualFunc105__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { self->unk15FC = val; }



float* cf::CActorParam::CActorParam_UnkVirtualFunc104() { return &this->unk15FC; }




// Wave-62: override of CActorParam_refreshBattleAction (+0x230). Free-function
// Fv form keeps the Unk linker name that hand-built tables / CREvtModel
// reference; the renamed member decl serves call-site codegen.
void cf::CActorParam::CActorParam_refreshBattleAction() {
    this->unk15FC = 0.0f;
}



void* cf::CActorParam::CActorParam_getBonusStats() { return &this->unk18D4; }



void* cf::CActorParam::CActorParam_getBaseStats() { return &this->unk185C; }




cf::CActorParam_UnkStruct3* cf::CActorParam::CActorParam_UnkVirtualFunc99() { return &this->unk17E4; }



cf::CActorParam_UnkStruct4* cf::CActorParam::CActorParam_UnkVirtualFunc97() { return &this->unk1792; }



void* cf::CActorParam::CActorParam_getArtsSyncBlock() { return &this->unk1740; }



void* cf::CActorParam::CActorParam_getStatusSyncBlock() { return &this->unk16C8; }



cf::CActorParam_UnkStruct3* cf::CActorParam::CActorParam_UnkVirtualFunc93() { return &this->unk1650; }



void cf::CActorParam::CActorParam_UnkVirtualFunc92(const void* src) {
    *(ParamCopyBlock*)&this->unk1650 = *(const ParamCopyBlock*)src;
}



// us-800b0be0: retail symbol is Fv; the real ABI passes (self, val, argA, argB).
// Forwards to Unk82(val) / Unk90(argA), then the +0x15E0 sub-object to
// Counter_AddClamped when present (flag = argB).
void cf::CActorParam::CActorParam_UnkVirtualFunc88(u32 val, u32 argA, u32 argB) {
    this->CActorParam_addSpentCurrency(val);
    this->CActorParam_addSecondCurrency(argA);
    u8* obj = (u8*)this->unk15E0;
    if (obj != NULL) {
        Counter_AddClamped((CUnk8025FB10*)obj, ((cf::CfActorObj89CView*)obj)->field_0x89C, argB);
    }
}



u32 cf::CActorParam::CActorParam_getTotalCurrency() { return this->unk1600 + this->unk1604; }



float* cf::CActorParam::CActorParam_UnkVirtualFunc80() { return &this->unk17E4.field_50; }



float* cf::CActorParam::CActorParam_UnkVirtualFunc79() { return &this->unk17E4.field_48; }



extern "C" void CActorParam_UnkVirtualFunc78__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { self->unk17E4.field_50 = val; }



extern "C" void CActorParam_UnkVirtualFunc77__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { self->unk17E4.field_48 = val; }



extern "C" void* CActorParam_UnkVirtualFunc75__Q22cf11CActorParamFi(cf::CActorParam* self) { return &self->unk17E4.field_44; }



extern "C" void CActorParam_UnkVirtualFunc73__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { self->unk17E4.field_44 = val; }



float cf::CActorParam::CActorParam_UnkVirtualFunc72() { return this->unk17E4.field_40; }



extern "C" void CActorParam_UnkVirtualFunc71__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { self->unk17E4.field_40 = val; }



float cf::CActorParam::CActorParam_UnkVirtualFunc69() { return this->unk1618; }



float cf::CActorParam::CActorParam_UnkVirtualFunc66() { return this->unk1610; }



int cf::CActorParam::CActorParam_getSecondGaugeMax() { return *(s16*)&this->unk1616; }



int cf::CActorParam::CActorParam_getSecondGauge() { return *(s16*)&this->unk1614; }



void cf::CActorParam::CActorParam_UnkVirtualFunc59(int val) { this->unk1614 = val; }



int cf::CActorParam::CActorParam_getGaugeMax() { return *(s16*)&this->unk160E; }



int cf::CActorParam::CActorParam_getGauge() { return *(s16*)&this->unk160C; }



void cf::CActorParam::CActorParam_UnkVirtualFunc53(int val) { this->unk160C = val; }



float cf::CActorParam::CActorParam_UnkVirtualFunc51() { return this->unk17E4.field_0C / this->unk17E4.field_18; }



float cf::CActorParam::CActorParam_UnkVirtualFunc50() { return this->unk17E4.field_18; }



float cf::CActorParam::CActorParam_getArtsGauge() { return this->unk17E4.field_0C; }



void cf::CActorParam::CActorParam_setTensionPoints(float val) { this->unk17E4.field_0C = val; }



float cf::CActorParam::CActorParam_UnkVirtualFunc45() { return this->unk17E4.field_08 / this->unk17E4.field_14; }



float cf::CActorParam::CActorParam_UnkVirtualFunc44() { return this->unk17E4.field_14; }



float cf::CActorParam::CActorParam_UnkVirtualFunc43() { return this->unk17E4.field_08; }



// us-800b0d38: add amount to the 0x17EC gauge and clamp into
// [0.0f, field_0x17F8]. Same arg-reuse pattern as Func31
// (lfs f2,cur; lfs f0,min; fadds f1,f2,f1; lfs f2,max; stfs f1); the upper
// compare is against the max local (fcmpo f1,f2; blelr stores max when the
// value exceeds it or is unordered).
void cf::CActorParam::CActorParam_UnkVirtualFunc42(float amount) {
    float min = 0.0f;
    float cur = this->unk17E4.field_08;
    float sum = cur + amount;
    float max = this->unk17E4.field_14;
    this->unk17E4.field_08 = sum;
    if (sum < min)
        this->unk17E4.field_08 = min;
    else if (sum > max)
        this->unk17E4.field_08 = max;
}



void cf::CActorParam::CActorParam_UnkVirtualFunc41(float val) { this->unk17E4.field_08 = val; }



float cf::CActorParam::CActorParam_UnkVirtualFunc39() { return this->unk17E4.field_04 / this->unk17E4.field_10; }



// us-800b0d84: retail symbol is Fv; the real ABI passes (self, val). Scales
// val by the gauge field at 0x17F4 and tail-dispatches vtable slot +0x11C
// (CActorParam_addHp).
extern "C" void CActorParam_UnkVirtualFunc36__Q22cf11CActorParamFv(cf::CActorParam* self, float val) {
    self->CActorParam_addHp(((cf::CfActorParamFields*)self)->field_0x17F4 * val);
}



// us-800b0d9c: retail symbol is Fv; the real ABI passes (self, amount). Adds
// amount to the 0x17E8 gauge, stores it, rounds to the nearest int (half
// away from zero: (s32)(v +/- 0.5) via the fctiwz + s32->f32 magic pair),
// floors sub-1.0 results back to 0.0f, then clamps into [0.0f, 0x17F4].
// Same rounding tail as CActorParam_applyDamage (CfObjectActor.cpp).
extern "C" void CActorParam_addHp__Q22cf11CActorParamFv(cf::CActorParam* self, float amount) {
    float v = ((cf::CfActorParamFields*)self)->field_0x17E8 + amount;
    ((cf::CfActorParamFields*)self)->field_0x17E8 = v;
    double round;
    if (v > 0.0f) {
        round = 0.5;
    } else {
        round = -0.5;
    }
    v = (float)(s32)((double)v + round);
    ((cf::CfActorParamFields*)self)->field_0x17E8 = v;
    if (v < 1.0f) {
        ((cf::CfActorParamFields*)self)->field_0x17E8 = 0.0f;
    }
    v = ((cf::CfActorParamFields*)self)->field_0x17E8;
    float max = ((cf::CfActorParamFields*)self)->field_0x17F4;
    if (v < 0.0f) {
        ((cf::CfActorParamFields*)self)->field_0x17E8 = 0.0f;
    } else if (v > max) {
        ((cf::CfActorParamFields*)self)->field_0x17E8 = max;
    }
}



float cf::CActorParam::CActorParam_UnkVirtualFunc32() { return this->unk1650.field_10; }



// us-800b0e40: add amount to the 0x1660 gauge and clamp at the 0.0f floor
// Retail emits lfs f2,cur; lfs f0,lbl(0.0f);
// fadds f1,f2,f1; stfs f1 - the floor stays in f0 and the accumulated value
// reuses the arg register. The else-if is the retail dev's always-false
// `amount > amount` guard: fcmpo cr0,f1,f1 / blelr over a redundant store
// (bclr 4,1 = branch when NOT GT keeps the tail-return shape).
extern "C" void CActorParam_UnkVirtualFunc31__Q22cf11CActorParamFv(cf::CActorParam* self, float amount) {
    float min = 0.0f;
    float cur = self->unk1650.field_10;
    amount = cur + amount;
    self->unk1650.field_10 = amount;
    if (amount < min)
        self->unk1650.field_10 = min;
    else if (amount > amount)
        self->unk1650.field_10 = amount;
}



extern "C" void CActorParam_UnkVirtualFunc30__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { self->unk1650.field_10 = val; }



extern "C" void CActorParam_UnkVirtualFunc28__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { self->unk17E4.field_00 += val; }



extern "C" void CActorParam_UnkVirtualFunc27__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { self->unk17E4.field_00 = val; }



extern "C" void CActorParam_UnkVirtualFunc25__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { self->unk1650.field_00 += val; }



extern "C" void CActorParam_UnkVirtualFunc24__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { self->unk1650.field_00 = val; }



float cf::CActorParam::CActorParam_UnkVirtualFunc17() { return this->unk15F8; }



extern "C" u8 CActorParam_UnkVirtualFunc15__Q22cf11CActorParamFv(cf::CActorParam* self) {
    return self->unk15F4[0];
}



u32 cf::CfObjectModel::CfObjectModel_getModelVisible() { return (this->mFlags68 >> 21) & 0x1u; }



u32 cf::CfObjectModel::CfObjectModel_getBindSource() { return (u32)this->field_B4; }



float cf::CfObjectModel::CfObjectModel_getModelRate() { return *(float*)&this->field_0xA4[8]; }



float cf::CfObjectModel::CfObjectModel_getModelFloatA8() { return *(float*)&this->field_0xA4[4]; }



u32 cf::CfObjectMove::CfObjectModel_getAnimFlags() { return (u32)this->mField704; }


void* cf::CfObjectMove::CfObjectModel_getAnimState() { return (void*)this->mField708; }






