// Auto-scaffolded catalog TU for kyoshin/cf/CVision
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <PowerPC_EABI_Support/Runtime/MWCPlusLib.h>
#include <new>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include "kyoshin/cf/object/CBattleState.hpp"

// Forward decls for the slot-array construction in CVision::CVision(). The
// retail names are address-mangled (__ct__801A33AC / __dt__801A36D0), so they
// are kept as C-linkage free functions with typed `self` params (same idiom
// as __dt__801A3728 below) to keep the __construct_array reloc names exact.
extern "C" void* __ct__801A33AC(cf::CVisionSlot* self);
extern "C" void* __dt__801A36D0(cf::UnkClass_801A36D0* self, int deleting);

// ---------------------------------------------------------------------------
// Include-order workarounds (CMenuArtsSelect.cpp idiom: rename the later
// header's symbols so its C-linkage decls don't overload-conflict with the
// chain headers pulled in by CBattleManager.hpp).
//
// CSuddenCommu.hpp's imports now match the chain headers (see CSuddenCommu.hpp)
// except func_8049603C (void* arg here vs CScn* in CTaskGame.hpp), so only
// that one is renamed; the symbols this TU uses (lbl_eu_80663E24,
// func_80496288, func_8006EF04__Fi, func_80260264) resolve to the
// chain-header / local declarations instead.
//
// func_8016FE34 is declared `void* (void*)` by CAIAction.hpp and
// CChainTimer.hpp but takes an int id at the retail call sites here, so those
// two headers are pre-included under a rename and CVision.cpp keeps its
// (int id) declaration.
#define func_8049603C visionCppCamView
#include "kyoshin/cf/CSuddenCommu.hpp"
#undef func_8049603C
#define func_8016FE34 visionCppChainActorLookup
#include "kyoshin/cf/object/CAIAction.hpp"
#undef func_8016FE34
#define func_8016FE34 visionCppChainTimerActorLookup
#include "kyoshin/cf/chain/CChainTimer.hpp"
#undef func_8016FE34
#include "kyoshin/cf/CBattleManager.hpp"

using namespace cf;

// Named .sdata2 conversion magics: defining them lets MWCC's constant pool
// reuse the retail symbols for the (double) casts instead of emitting TU-local
// @N labels (CMiniMap / CfResReloadImpl idiom). 80667D28 is the signed
// int->float magic (2^52 + 2^31); 80667D58 the unsigned one (2^52).
extern "C" const double lbl_eu_80667D28 = 0x4330000080000000ll;
extern "C" const double lbl_eu_80667D58 = 0x4330000000000000ll;

// Most C-linkage pseudo-imports for this TU now live in the "C-linkage
// imports" section of kyoshin/cf/CVision.hpp (real imports). The few kept
// inline below stay because sibling TUs that include this header declare the
// same retail symbols with different signatures or stub definitions -- moving
// them here would break those TUs' builds (see the task report).

// Fragment-anchored destructors for the UnkClass_801A36D0 / UnkClass_801A3728
// sub-objects are defined below and passed by address to __destroy_arr.

// Kept inline (not moved to CVision.hpp): sibling TUs that include this
// header declare the same retail symbols with conflicting signatures/stubs.
extern "C" void func_800EA484(cf::CBattleManager* bm, f32 volume, int id); // CBattleManager.cpp re-declares (float,u32) + defines stub
extern "C" int func_80148778(void* obj, int id);   // CBattleManager.cpp declares with C++ linkage
extern "C" void func_801AD504(int id);             // CMenuVision.cpp defines an empty stub
extern "C" void* func_8016FE34(int id);            // 4 TUs declare a void* -param version
extern "C" void func_802A1DF0(u32 a);              // CCharVoiceMan.cpp defines an empty stub
extern "C" int func_80260264(void* self, int id, void* result); // matches CSuddenCommu.hpp / CBattleManager.cpp

// Kept inline (not moved to CVision.hpp): sibling TUs declare/stub these.
extern "C" void func_800BE12C(u8* a, int b, int c, int d, int e); // CfObjectActor.hpp/CfObjectMove.hpp declare (u8*,int,int,int,int)
extern "C" void func_800E921C(void* a, void* b, void* c, void* d, void* e); // CBattleManager.cpp defines an empty stub
extern "C" void* func_8009EC9C(void); // returns a pointer (used by func_801A8244); CBattleManager.cpp/pluginUnit.cpp declare different signatures

// Per-battle-slot object iterated in func_801A897C (stride 0x834).
struct CVisionBtlSlot {
    u32 w_00;       // 0x00
    u32 w_04;       // 0x04
    u8 unk08[0x88 - 0x08];
    u32 w_88;       // 0x88
    u8 unk8C[0x824 - 0x8C];
    u32 w_824;      // 0x824
    u32 w_828;      // 0x828
    u32 w_82C;      // 0x82C
};

// Parameter block used by func_801A6540's FX-setup path.
struct CVisionFxParam {
    u8 unk0[0x6];   // 0x00
    u8 b_06;        // 0x06
    u8 unk07[0xD - 0x07];
    u8 b_0D;        // 0x0D
    u8 unk0E[0x10 - 0x0E];
    u16 h_10;       // 0x10
    u16 h_12;       // 0x12
    f32 f_14;       // 0x14
    u8 unk18[0x20 - 0x18];
};

// Kept inline (not moved to CVision.hpp): sibling TUs stub these.
extern "C" void func_802A1FB4(void* a, void* b); // CCharVoiceMan.cpp defines an empty stub
extern "C" void func_801AF934(u32 a);            // CMenuVision.cpp defines an empty stub

// Object handed to func_801A5E58 by func_800F477C (reads byte at 0x42).
struct CVisionRefObj {
    u8 unk0[0x42];  // 0x00
    u8 b_42;         // 0x42
    u8 unk43[0x78 - 0x43];
    u32 w_78;        // 0x78
};
// Kept inline (not moved to CVision.hpp): sibling TUs declare these.
extern "C" CVisionRefObj* func_800F477C(void* self); // pluginUnit.cpp declares CfUnknownSub* version
extern "C" bool func_8006EF04__Fi(int mask);         // CfObjectActor.hpp/CChain.hpp declare bool(int); s32=long would clash

// Object passed to func_801A6A7C / func_801A380C-family (battle object).
struct CVisionObj {
    u8 unk00[0x08];
    u8 field_08;           //0x08 (region handed to func_80148778)
    u8 unk09[0x3F00 - 0x09];
    u32 field_3F00;        //0x3F00 flag bits
    u8 unk3F04[0x3F10 - 0x3F04];
    u32 field_3F10;        //0x3F10 id/ptr
    u8 unk3F14[0x3F28 - 0x3F14];
    u16 field_3F28;        //0x3F28
    u8 unk3F2A[0x4FFC - 0x3F2A];
};

// Result of func_8016FE34 (per-model battle state). The retail v20 call
// dispatches at vtable offset 0x20; the vtable carries 2 leading slots plus
// 7 declared virtuals (v20 = 7th, index 8).
class CVisionFusionSub {
public:
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20(u32 r4);   // 0x20
};

struct CVisionFusion {
    u8 unk0[0x4];
    CVisionFusionSub* field_04; //0x4 vtable-holder
    u8 unk08[0x3380 - 0x08];
    void* field_3380;           //0x3380
    u8 unk3384[0x3388 - 0x3384];
    u16 field_3388;             //0x3388
    u8 unk338A[0x3E9C - 0x338A];
    void* field_3E9C;           //0x3E9C vtable
    u8 unk3EA0[0x3F00 - 0x3EA0];
    u32 field_3F00;             //0x3F00 flags
    u8 unk3F04[0x3F60 - 0x3F04];
    void* field_3F60;           //0x3F60
    u8 unk3F64[0x4FFC - 0x3F64];
};


cf::CVision::CVision() {
    // Initialise the four CVisionSub battle slots (0x4..0x20D4, 0x834 stride):
    // field stores, two memset regions, the 10 x 0xBC item array, then the
    // per-slot release callback. Retail hoists one sdata2 zero into a saved FPR.
    f32 zero = lbl_eu_80667CD4;
    CVisionSub* p = &sub;
    CVisionSub* pEnd = p + 4;
    do {
        p->field_84 = 0;
        for (u32* wp = &p->field_88; wp < &p->field_8C; wp++) *wp = 0;
        p->field_10 = 0;
        p->field_14 = 0;
        p->field_58 = 0;
        p->field_5C = -1;
        p->field_60 = 0;
        p->f_64 = zero; p->f_68 = zero; p->f_6C = zero; p->f_70 = zero; p->f_74 = zero;
        p->field_8C = 0;
        p->field_90 = 0;
        p->field_C8 = 0;
        p->f_78 = zero; p->f_7C = zero;
        p->h_80 = 0; p->h_82 = 0;
        memset(p->unk18, 0, 0x40);
        memset(p->unk94, 0, 0x34);
        p->field_84 = 0;
        p->field_88 = 0;
        CVisionInitBlock* ip = p->items;
        CVisionInitBlock* ipEnd = ip + 10;
        do {
            CVisionInitBlock& it = *ip;
            it.w_74 = 0;
            for (u32* wp = &it.w_78; wp < &it.w_7C; wp++) *wp = 0;
            it.w_00 = 0;
            it.w_04 = 0;
            it.w_48 = 0;
            it.w_4C = -1;
            it.w_50 = 0;
            it.f_54 = zero; it.f_58 = zero; it.f_5C = zero; it.f_60 = zero; it.f_64 = zero;
            it.w_7C = 0;
            it.h_80 = 0;
            it.w_B8 = 0;
            it.f_68 = zero; it.f_6C = zero;
            it.h_70 = 0; it.h_72 = 0;
            memset(it.unk_08, 0, 0x40);
            memset(it.unk_84, 0, 0x34);
            it.w_74 = 0;
            it.w_78 = 0;
            ip++;
        } while (ip < ipEnd);
        p->field_824 = 0;
        func_800F449C(p);
        p++;
    } while (p < pEnd);

    // The eight 0x4818-byte vision slots, via the retail slot ctor/dtor.
    __construct_array(&unk20D4[0], (void*)&__ct__801A33AC,
                      (void*)&__dt__801A36D0, sizeof(CVisionSlot), 8);

    // unk261C4 head (u32/f32/u32 triplet) plus seven more at 0xC stride.
    // The entry-testing for-loop matches the retail cmplw/bge + mtctr/bdnz.
    f32 zero261 = lbl_eu_80667CD4;
    unk261C4.w0 = 0;
    unk261C4.f4 = zero261;
    unk261C4.w8 = 0;
    CVisionU32F32U32* upEnd = (CVisionU32F32U32*)((u8*)this + 0x26224);
    for (CVisionU32F32U32* up = unk261C4.arr; up < upEnd; up++) {
        up->a = 0;
        up->b = zero261;
        up->c = 0;
    }

    // unk261C4 tail (self pointer / count) then the vision field resets.
    f32 zeroFields = lbl_eu_80667CD4;
    unk261C4.w6C = 8;
    unk261C4.w60 = (u32)&unk261C4;
    unk261C4.field_68 = 0;
    unk261C4.field_64 = 0;
    field_26194 = zeroFields;
    field_26198 = zeroFields;
    field_2619C = zeroFields;
    field_261A0 = zeroFields;
    field_261A4 = 0;
    memset(effectArray, 0, sizeof(effectArray));

    // Install the null ptmf callback and hand it to __ptmf_test.
    CVisionPtmf cb;
    cb.mPfn = __ptmf_null[0];
    cb.mObj = __ptmf_null[1];
    cb.mDelta = __ptmf_null[2];
    mPtmf.mPfn = cb.mPfn;
    mPtmf.mObj = cb.mObj;
    mPtmf.mDelta = cb.mDelta;
    field_2619C = lbl_eu_80667CD4;
    field_261A0 = field_26194;
    __ptmf_test(&cb);
}
// ---------------------------------------------------------------------------
// us-801a4acc: CVisionSlot constructor (retail __ct__801A33AC).
// Each 0x4818-byte vision slot carries a CBattleState, two func_801751DC
// sub-objects, a CArtsSet (vtable at +0x34) and CAttackSet (vtable at +0x0),
// plus the same 0xBC-byte init blocks as CVisionSub. Member construction is
// manual (placement new / direct calls) so the retail call order is kept.
// ---------------------------------------------------------------------------
extern "C" void* __ct__801A33AC(CVisionSlot* self) {
    self->field_00 = (u32)&lbl_eu_8052813C[0];
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    __ct__Q22cf12CBattleStateFv(&self->unk10[0]);
    func_801751DC(self->unk15F0);
    func_801751DC(self->unk1668);
    memset(self->unk16E0, 0, 0x1520);
    u8* p = self->unk2C00;
    u8* pEnd = self->unk2C00 + 0xC0;
    do {
        memset(p, 0, 0x18);
        p += 0x18;
    } while (p < pEnd);
    __ct__Q22cf9_sArtsSetFv(&self->unk2CC8[0]);
    self->field_2CFC = (u32)&lbl_eu_8052F5C0[0];
    cf::CArtsParam* ap = (cf::CArtsParam*)self->unk2D00;
    cf::CArtsParam* apEnd = (cf::CArtsParam*)((u8*)self + 0x3A20);
    do {
        __ct__Q22cf10CArtsParamFv(ap);
        ap++;
    } while (ap < apEnd);
    self->field_3A2C = (u32)&lbl_eu_8052F5A8[0];
    cf::CAttackParam* tp = (cf::CAttackParam*)self->unk3A30;
    cf::CAttackParam* tpEnd = (cf::CAttackParam*)((u8*)self + 0x3D60);
    do {
        __ct__Q22cf12CAttackParamFv(tp);
        tp++;
    } while (tp < tpEnd);
    u8* q = self->unk3D8C;
    u8* qEnd = self->unk3D8C + 0x200;
    do {
        memset(q + 4, 0, 0xe);
        memset(q, 0, 0x20);
        q += 0x20;
    } while (q < qEnd);
    self->field_3F98 = 0x10;
    self->field_3F8C = (u32)self->unk3D8C;
    self->field_3F94 = 0;
    self->field_3F90 = 0;
    memset(self->unk3FA0, 0, 0xe);
    memset(self->unk3F9C, 0, 0x20);
    // Item array: retail hoists one sdata2 zero load into a saved FPR.
    f32 zeroItems = lbl_eu_80667CD4;
    CVisionInitBlock* ip = self->items;
    CVisionInitBlock* ipEnd = ip + 10;
    do {
        CVisionInitBlock& it = *ip;
        it.w_74 = 0;
        for (u32* wp = &it.w_78; wp < &it.w_7C; wp++) *wp = 0;
        it.w_00 = 0;
        it.w_04 = 0;
        it.w_48 = 0;
        it.w_4C = -1;
        it.w_50 = 0;
        it.f_54 = zeroItems; it.f_58 = zeroItems; it.f_5C = zeroItems;
        it.f_60 = zeroItems; it.f_64 = zeroItems;
        it.w_7C = 0;
        it.h_80 = 0;
        it.w_B8 = 0;
        it.f_68 = zeroItems; it.f_6C = zeroItems;
        it.h_70 = 0; it.h_72 = 0;
        memset(it.unk_08, 0, 0x40);
        memset(it.unk_84, 0, 0x34);
        it.w_74 = 0;
        it.w_78 = 0;
        ip++;
    } while (ip < ipEnd);
    CVisionInitBlock& b = self->block4720;
    b.w_74 = 0;
    for (u32* wp = &b.w_78; wp < &b.w_7C; wp++) *wp = 0;
    f32 zeroBlock = lbl_eu_80667CD4;
    b.w_00 = 0;
    b.w_04 = 0;
    b.w_48 = 0;
    b.w_4C = -1;
    b.w_50 = 0;
    b.f_54 = zeroBlock; b.f_58 = zeroBlock; b.f_5C = zeroBlock;
    b.f_60 = zeroBlock; b.f_64 = zeroBlock;
    b.w_7C = 0;
    b.h_80 = 0;
    b.w_B8 = 0;
    b.f_68 = zeroBlock; b.f_6C = zeroBlock;
    b.h_70 = 0; b.h_72 = 0;
    memset(&b.unk_08, 0, 0x40);
    memset(&b.unk_84, 0, 0x34);
    b.w_74 = 0;
    b.w_78 = 0;
    func_80174658(self->unk47EC);
    // Slot tail resets (retail store order: f_15E8, h_15EE, h_15EC, ...).
    f32 zeroTail = lbl_eu_80667CD4;
    self->f_15E8 = zeroTail;
    self->h_15EE = 0;
    self->h_15EC = 0;
    self->w_2CC4 = 0;
    self->w_2CC0 = 0;
    self->f_3D6C = zeroTail;
    self->w_3FBC = 0;
    self->f_3FC0 = zeroTail;
    self->w_4800 = 0;
    self->f_4808 = zeroTail;
    self->w_4810 = 0;
    self->w_4804 = 0;
    self->f_480C = zeroTail;
    self->w_4814 = 0;
    return self;
}

// ---------------------------------------------------------------------------
// us-801a4f2c: Initialise all vision slots / effects (retail func_801A380C).
// ---------------------------------------------------------------------------
void func_801A380C(CVision* self) {
    self->field_26194 = lbl_eu_80667CD4;
    self->field_26198 = lbl_eu_80667CD4;
    self->field_2619C = lbl_eu_80667CD4;
    self->field_261A0 = lbl_eu_80667CD4;
    self->field_261A4 = 0;
    CVisionPtmf cb;
    u32* p = __ptmf_null;
    u32 v0 = *p++;
    u32 v1 = *p++;
    u32 v2 = *p++;
    cb.mPfn = v0;
    cb.mObj = v1;
    cb.mDelta = v2;
    self->mPtmf = cb;
    self->field_2619C = lbl_eu_80667CD4;
    self->field_261A0 = lbl_eu_80667CD4;
    __ptmf_test(&cb);
    self->vt_34();
    for (int i = 0; i < 4; i++) {
        func_800F449C(&self->sub + i);
    }
    for (int i = 0; i < 8; i++) {
        CVisionSlot& s = self->unk20D4[i];
        s.f_15E8 = lbl_eu_80667CD4;
        s.h_15EE = 0;
        s.h_15EC = 0;
        s.w_2CC4 = 0;
        s.w_2CC0 = 0;
        s.f_3D6C = lbl_eu_80667CD4;
        s.w_3FBC = 0;
        s.f_3FC0 = lbl_eu_80667CD4;
        s.w_4800 = 0;
        s.f_4808 = lbl_eu_80667CD4;
        s.w_4810 = 0;
        s.w_4804 = 0;
        s.f_480C = lbl_eu_80667CD4;
        s.w_4814 = 0;
    }
}

// us-801a50f8: Drive the vision-slot FX update loop from the element array.
void func_801A39D8(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    if (sub == 0) {
        if (*(u32*)((u8*)self + 0x23d4) != 0) {
            *(u32*)((u8*)self + 0x23d4) = 0;
            *(u32*)((u8*)self + 0x23d0) = 0;
        }
        if (__ptmf_test(&self->mPtmf) != 0) {
            self->vt_1C();
        }
        *(u8*)((u8*)self + 0x23ac) = 0;
        return;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x40000) != 0) {
        return;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (CfRes_getE24Bit22() != 0) {
        return;
    }
    f32 f30 = lbl_eu_80667CDC;
    f32 f31 = lbl_eu_80667CD4;
    for (u32 i = 0;; i++) {
        u32 v28 = *(u32*)((u8*)self + 0x23d0);
        u32 v30 = *(u32*)((u8*)self + 0x23d8);
        u32 base = *(u32*)((u8*)self + 0x23cc);
        u32 idx = (v28 + i) / v30;
        u32 rem = (v28 + i) - idx * v30;
        if (*(u32*)base != idx) {
            // nop -- retail just reads the element base via the divwu index
        }
        // ... (elided middle: per-element switch calling func_801AD504)
        func_80496288((void*)lbl_eu_80663E14);
        break;
    }
    // ... (elided rest)
    if (*(u32*)((u8*)self + 0x23d0) == 0) {
        for (int i = 0; i < 3; i++) {
            void* p = func_8016FE34((int)getPlayer__Q22cf13CfGameManagerFi(i));
            if (p != 0) {
                *(u16*)((u8*)p + 0x3388) &= 0xfff7;
            }
        }
        CVisionPtmf cb;
        cb.mPfn = __ptmf_null[0];
        cb.mObj = __ptmf_null[1];
        cb.mDelta = __ptmf_null[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A4194(CVision* self) {
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0 || p2 == 0) {
        self->vt_20(1);
        return;
    }
    // Retail materializes the equality test via mfcr/extrwi (value context).
    bool atBase = (lbl_eu_80667CD4 == self->field_2619C);
    if (atBase) {
        func_8006E2FC(0);
        CBattleManager* bm = CBattleManager::getInstance();
        ((void (*)(void*))(*(void***)bm)[0x38 / 4])(bm);
    }
    // Materialized timer-window checks (retail evaluates each into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CE4, 3);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 3);
    }
    int w3;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w3 = 1;
    } else {
        w3 = 0;
    }
    if (w3) {
        self->vt_2C(0);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1be, 0, 0, lbl_eu_80667CE0);
    }
    int w4;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE8 &&
        lbl_eu_80667CE8 < self->field_26194 - self->field_261A0) {
        w4 = 1;
    } else {
        w4 = 0;
    }
    if (w4) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x328, 0);
        u32 i = 0;
        while (i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620)) {
            void* elem = func_800F6EAC(func_80043F18(vdlist), i);
            void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
            if (obj == 0) {
                self->vt_20(1);
                __dt__80043E88(vdlist, -1);
                return;
            }
            u32 id = *(u32*)((u8*)obj + 0x74);
            if (id != sub->field_00 && id != sub->field_04) {
                ((void (*)(void*, u32))(*(void***)obj)[0x190 / 4])(obj, 1);
            }
            i++;
        }
        __dt__80043E88(vdlist, -1);
        if (getGlobalSda() != 0) {
            getGlobalSda()[0xa8] = 1;
        }
        self->vt_2C(1);
        func_801C01A8(0, 0x2, lbl_eu_80667CE4);
        func_801C01A8(1, 0x0, lbl_eu_80667CE4);
        func_802A1DF0(1);
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533134[0];
        cb.mObj = lbl_eu_80533134[1];
        cb.mDelta = lbl_eu_80533134[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
    if (((void* (*)(void*))(*(void***)p1)[0x2BC / 4])(p1) != 0 ||
        ((void* (*)(void*))(*(void***)p2)[0x2BC / 4])(p2) != 0) {
        self->vt_1C();
    }
}

// ---------------------------------------------------------------------------
// us-801a5c98: Per-player vision callback management (retail func_801A4578).
// ---------------------------------------------------------------------------
void func_801A4578(CVision* self) {
    CVisionFusion* p1;
    CVisionFusion* p2;
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    if (sub->field_824 & 0x40000) {
        p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
        p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
        if (p1 == 0 || p2 == 0) {
            self->vt_20(1);
            return;
        }
        // Retail materializes the equality test via mfcr/extrwi (value context).
        bool atBase = (lbl_eu_80667CD4 == self->field_2619C);
        if (atBase) {
            func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CEC, 3);
        }
        // Materialized timer-window checks (retail evaluates both into r0).
        int w1;
        if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CF0 &&
            lbl_eu_80667CF0 < self->field_26194 - self->field_261A0) {
            w1 = 1;
        } else {
            w1 = 0;
        }
        if (w1) {
            func_80133F48(5, lbl_eu_80667CF0);
        }
        if (sub->field_824 & 0x80000) {
            u32* src = &lbl_eu_80533140.mPfn;
            u32 dlt, obj, pfn;
            pfn = *src++;
            obj = *src++;
            dlt = *src++;
            CVisionPtmf cb;
            cb.mPfn = pfn;
            cb.mObj = obj;
            cb.mDelta = dlt;
            f32 f_6194 = self->field_26194;
            f32 f_cd4 = lbl_eu_80667CD4;
            self->mPtmf.mPfn = pfn;
            self->mPtmf.mObj = obj;
            self->mPtmf.mDelta = cb.mDelta;
            self->field_2619C = f_cd4;
            self->field_261A0 = f_6194;
            __ptmf_test(&cb);
        }
        int w2;
        if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CF4 &&
            lbl_eu_80667CF4 < self->field_26194 - self->field_261A0) {
            w2 = 1;
        } else {
            w2 = 0;
        }
        if (w2) {
            u32* src = &lbl_eu_8053314C.mPfn;
            u32 dlt, obj, pfn;
            pfn = *src++;
            obj = *src++;
            dlt = *src++;
            CVisionPtmf cb;
            cb.mPfn = pfn;
            cb.mObj = obj;
            cb.mDelta = dlt;
            f32 f_6194 = self->field_26194;
            f32 f_cd4 = lbl_eu_80667CD4;
            self->mPtmf.mPfn = pfn;
            self->mPtmf.mObj = obj;
            self->mPtmf.mDelta = cb.mDelta;
            self->field_2619C = f_cd4;
            self->field_261A0 = f_6194;
            __ptmf_test(&cb);
        }
        if (((void* (*)(void*))((void**)p1)[0x2BC / 4])(p1) != 0 ||
            ((void* (*)(void*))((void**)p2)[0x2BC / 4])(p2) != 0) {
            self->vt_1C();
        }
    }
}

void func_801A47D0(CVision* self) {
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0 || p2 == 0) {
        self->vt_20(1);
        return;
    }
    // Retail materializes the equality test via mfcr/extrwi (value context).
    bool atBase = (lbl_eu_80667CD4 == self->field_2619C);
    if (atBase) {
        void* mem = getHandleMEM2__Q23mtl10MemManagerFv();
        func_8004392C(1, 0x12f, mem, 2, 1, lbl_eu_80667CF8);
    }
    // Materialized timer-window checks (retail evaluates each into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CE4, 3);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 3);
    }
    int w3;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w3 = 1;
    } else {
        w3 = 0;
    }
    if (w3) {
        self->vt_2C(0);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1be, 0, 0, lbl_eu_80667CE0);
        func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
    }
    int w4;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE8 &&
        lbl_eu_80667CE8 < self->field_26194 - self->field_261A0) {
        w4 = 1;
    } else {
        w4 = 0;
    }
    if (w4) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x328, 0x1);
        u32 i = 0;
        while (i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620)) {
            void* elem = func_800F6EAC(func_80043F18(vdlist), i);
            void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
            if (obj == 0) {
                self->vt_20(1);
                __dt__80043E88(vdlist, -1);
                return;
            }
            u32 id = *(u32*)((u8*)obj + 0x74);
            if (id != sub->field_00 && id != sub->field_04) {
                ((void (*)(void*, u32))(*(void***)obj)[0x190 / 4])(obj, 1);
            }
            i++;
        }
        __dt__80043E88(vdlist, -1);
        if (getGlobalSda() != 0) {
            getGlobalSda()[0xa8] = 1;
        }
        self->vt_2C(1);
        func_801C01A8(0, 0x2, lbl_eu_80667CE4);
        func_801C01A8(1, 0x0, lbl_eu_80667CE4);
        func_802A1DF0(1);
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533158[0];
        cb.mObj = lbl_eu_80533158[1];
        cb.mDelta = lbl_eu_80533158[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
    if (((void* (*)(void*))(*(void***)p1)[0x2BC / 4])(p1) != 0 ||
        ((void* (*)(void*))(*(void***)p2)[0x2BC / 4])(p2) != 0) {
        self->vt_1C();
    }
}

void func_801A4BC8(CVision* self) {
    // Retail materializes the equality test via mfcr/extrwi (value context).
    bool atBase = (lbl_eu_80667CD4 == self->field_2619C);
    if (atBase) {
        self->vt_2C(3);
        void* mem = getHandleMEM2__Q23mtl10MemManagerFv();
        func_8004392C(1, 0x130, mem, 2, 1, lbl_eu_80667CF8);
    }
    // Materialized timer-window checks (retail evaluates both into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
        func_8006E5D8();
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CFC &&
        lbl_eu_80667CFC < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        self->vt_20(0);
    }
}

void func_801A4CF8(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    if (sub == 0) {
        self->vt_20(1);
        return;
    }
    CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0 || p2 == 0) {
        self->vt_20(1);
        return;
    }
    // Resume callback clear: restore the sub fader, play a sound.
    if (self->field_2619C == lbl_eu_80667CD4) {
        if (sub->field_824 & 0x20000) {
            sub->field_0C->field_7C = lbl_eu_80667D00;
        } else {
            sub->field_0C->field_7C = lbl_eu_80667CF4;
        }
        if (sub->field_824 & 0x20000) {
            func_80081F90__Q22cf13CfGameManagerFv(0x26, 0);
        }
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1bf, 0, 0, lbl_eu_80667CE0);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        ((void (*)(void*, f32))((void**)p1)[0x5C4 / 4])(p1, lbl_eu_80667CF0);
        if (sub->field_824 & 0x20000) {
            ((void (*)(void*, u32))((void**)p1)[0x5C8 / 4])(p1, 1);
            func_800BE12C((u8*)p1 + 0x3e9c, 0x11, 1, -1, 1);
        } else {
            func_800BE12C((u8*)p1 + 0x3e9c, 0x1, 1, -1, 1);
        }
        func_80174B4C(p1, 0x4000);
        if (p1 != p2) {
            ((void (*)(void*, f32))((void**)p2)[0x5C4 / 4])(p2, lbl_eu_80667CF0);
            func_800BE12C((u8*)p2 + 0x3e9c, 0x1, 1, -1, 1);
            func_80174B4C(p2, 0x4000);
            p2->field_3388 |= 0x2;
        }
        if (p2->field_3F00 & 0x2) {
            bool cancel;
            if (p2 != 0 && p2->field_3F60 != 0) {
                void* vt = *(void**)((u8*)p2 + 0x3e9c);
                void* r = ((void* (*)(void*))((void**)vt)[0xAC / 4])((u8*)p2 + 0x3e9c);
                f32 diff = *(f32*)((u8*)p2->field_3F60 + 0x3e8) - *(f32*)((u8*)r + 4);
                cancel = diff > lbl_eu_80667CD0;
            } else {
                cancel = true;
            }
            if (cancel) {
                self->vt_20(1);
            }
        }
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D04 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667D04) {
        func_801AD504(1);
    }
    f32 f = ((f32 (*)(void*))((void**)p1)[0x5BC / 4])(p1);
    if (f == lbl_eu_80667CD4) {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533164[0];
        cb.mObj = lbl_eu_80533164[1];
        cb.mDelta = lbl_eu_80533164[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

// ---------------------------------------------------------------------------
// us-801a678c: Second-fusion vision check. When the fusion's 0x2 flag is up,
// cancel vision if the remaining time is short, then run the D08/CE4 timer
// windows (retail func_801A506C).
// ---------------------------------------------------------------------------
void func_801A506C(CVision* self) {
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p2->field_3F00 & 0x2) {
        bool cancel;
        if (p2 == 0 || p2->field_3F60 == 0) {
            cancel = true;
        } else {
            void* f60 = p2->field_3F60;
            void* sub = (u8*)p2 + 0x3e9c;
            void* r = ((void* (*)(void*))((void**)*(void**)sub)[0xAC / 4])(sub);
            f32 f_ret = *(f32*)((u8*)r + 4);
            f32 diff = *(f32*)((u8*)f60 + 0x3e8) - f_ret;
            cancel = diff < lbl_eu_80667CD0;
        }
        if (cancel) {
            self->vt_20(1);
        }
    }
    // Materialized timer-window checks (retail evaluates both into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D08 &&
        lbl_eu_80667D08 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 0x24);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D08 &&
        lbl_eu_80667D08 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        func_801AD504(2);
    }
    int w3;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w3 = 1;
    } else {
        w3 = 0;
    }
    if (w3) {
        u32* src = &lbl_eu_80533170.mPfn;
        u32 dlt, obj, pfn;
        pfn = *src++;
        obj = *src++;
        dlt = *src++;
        CVisionPtmf cb;
        cb.mPfn = pfn;
        cb.mObj = obj;
        cb.mDelta = dlt;
        f32 f_6194 = self->field_26194;
        f32 f_cd4 = lbl_eu_80667CD4;
        self->mPtmf.mPfn = pfn;
        self->mPtmf.mObj = obj;
        self->mPtmf.mDelta = cb.mDelta;
        self->field_2619C = f_cd4;
        self->field_261A0 = f_6194;
        __ptmf_test(&cb);
    }
}

void func_801A5260(CVision* self) {
    // Materialized timer-window checks (retail evaluates both into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D04 &&
        lbl_eu_80667D04 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_801AD504(4);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D0C &&
        lbl_eu_80667D0C < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        CVisionPtmf cb;
        u32* p = lbl_eu_8053317C;
        u32 v0 = *p++;
        u32 v1 = *p++;
        u32 v2 = *p++;
        cb.mPfn = v0;
        cb.mObj = v1;
        cb.mDelta = v2;
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A5360(CVision* self) {
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x10);
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x24);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D10 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667D10) {
        self->vt_1C();
    }
}

// us-801a6b64: Match the vision sub-object's model against `obj`; when it
// matches one of the callback identities, install the associated vision FX
// callback on the vision slots.
void func_801A5444(CVision* self, CVisionBattleObj* obj, CVisionBattleObj* r5) {
    CVisionPtmf cur;
    cur.mPfn = self->mPtmf.mPfn;
    cur.mObj = self->mPtmf.mObj;
    cur.mDelta = self->mPtmf.mDelta;
    if (__ptmf_test(&cur) == 0) {
        return;
    }
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    void* fu = func_8016FE34(func_800B708C__Fi(sub->field_00));
    if (fu == 0) {
        self->vt_20(1);
        return;
    }
    if (obj != (void*)fu) {
        return;
    }

    // The callback must equal one of the two identity triplets at
    // lbl_eu_80533128+0x60 / +0x6C for the FX-install path to run.
    int ok = 1;
    CVisionPtmf t1;
    t1.mPfn = lbl_eu_80533128[0x60 / 4];
    t1.mObj = lbl_eu_80533128[0x64 / 4];
    t1.mDelta = lbl_eu_80533128[0x68 / 4];
    if (__ptmf_cmpr(&self->mPtmf, &t1) == 0) {
        CVisionPtmf t2;
        t2.mPfn = lbl_eu_80533128[0x6C / 4];
        t2.mObj = lbl_eu_80533128[0x70 / 4];
        t2.mDelta = lbl_eu_80533128[0x74 / 4];
        if (__ptmf_cmpr(&self->mPtmf, &t2) == 0) {
            ok = 0;
        }
    }
    if (ok == 0) {
        // Not one of the two FX identities: the +0x84 triplet releases the
        // slot, the +0x90 triplet is checked and ignored.
        CVisionPtmf t4;
        t4.mPfn = lbl_eu_80533128[0x84 / 4];
        t4.mObj = lbl_eu_80533128[0x88 / 4];
        t4.mDelta = lbl_eu_80533128[0x8C / 4];
        if (__ptmf_cmpr(&self->mPtmf, &t4) == 0) {
            func_800F449C(sub);
        } else {
            CVisionPtmf t5;
            t5.mPfn = lbl_eu_80533128[0x90 / 4];
            t5.mObj = lbl_eu_80533128[0x94 / 4];
            t5.mDelta = lbl_eu_80533128[0x98 / 4];
            __ptmf_cmpr(&self->mPtmf, &t5);
        }
        return;
    }

    // FX-install path: pull the item block and the mid block from `obj`.
    CVisionInitBlock* src = obj->vf298();
    sub->items[obj->vf2A8()] = *src;
    if (obj->vf2A8() == 0) {
        CVisionInitBlock* mid = obj->vf298();
        *(CVisionSubMid*)&sub->field_10 = *(CVisionSubMid*)mid;
        f32 sum = mid->f_5C + mid->f_60;
        int r0;
        if (sum >= lbl_eu_80667CD4) {
            if (sub->field_84 & 0x80) {
                r0 = 0;
            } else {
                f64 adj = (sum > lbl_eu_80667CD4) ? lbl_eu_80667D18 : lbl_eu_80667D20;
                r0 = (int)(sum + adj);
            }
        } else {
            r0 = 0;
        }
        if (r0 <= 0) {
            sub->f_6C = lbl_eu_80667CD4;
        }
    } else {
        CVisionInitBlock* d = obj->vf298();
        if (d->w_74 & 1) {
            sub->field_84 |= d->w_74;
        } else {
            sub->field_88 |= d->w_74;
        }
    }
    if (obj->vf2A8() > 0) {
        CVisionInitBlock* d2 = obj->vf298();
        f32 sum2 = d2->f_5C + d2->f_60;
        int r0b;
        if (sum2 >= lbl_eu_80667CD4) {
            if (d2->w_74 & 0x80) {
                r0b = 0;
            } else {
                f64 adj2 = (sum2 > lbl_eu_80667CD4) ? lbl_eu_80667D18 : lbl_eu_80667D20;
                r0b = (int)(sum2 + adj2);
            }
        } else {
            r0b = 0;
        }
        if (r0b > 0) {
            CVisionInitBlock* d3 = obj->vf298();
            f32 sum3 = d3->f_5C + d3->f_60;
            int r0c;
            if (sum3 >= lbl_eu_80667CD4) {
                if (d3->w_74 & 0x80) {
                    r0c = 0;
                } else {
                    f64 adj3 = (sum3 > lbl_eu_80667CD4) ? lbl_eu_80667D18 : lbl_eu_80667D20;
                    r0c = (int)(sum3 + adj3);
                }
            } else {
                r0c = 0;
            }
            if (r0c > 0) {
                sub->f_6C += (f32)r0c;
            }
        }
    }
    if (r5->vf2BC() != 0 || (obj->vf298()->w_78 & 0x1000)) {
        sub->field_824 |= 0x400;
    }

    // Install the +0x78 identity callback with the fader fields.
    CVisionPtmf t3;
    t3.mPfn = lbl_eu_80533128[0x78 / 4];
    t3.mObj = lbl_eu_80533128[0x7C / 4];
    t3.mDelta = lbl_eu_80533128[0x80 / 4];
    self->mPtmf = t3;
    self->field_2619C = lbl_eu_80667CD4;
    self->field_261A0 = self->field_26194;
    __ptmf_test(&t3);
}

void func_801A5BA8(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    CVisionFusion* p = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    if (p == 0) {
        self->vt_20(1);
        return;
    }
    if (self->field_2619C == lbl_eu_80667CD4) {
        p->field_3388 |= 0x2;
    }
    void* a = ((void* (*)(void*))((void**)p)[0x2A4 / 4])(p);
    void* prm = *(void**)((u8*)a + 0x50);
    s32 byte = *(u8*)((u8*)prm + 0x44);
    s32 result = (s32)((s32(*)(void*, void*))((void**)p)[0x2A8 / 4])(p, prm);
    // r29 = (result - byte) with sign: nonzero when the value moved out of range.
    s32 diff = result - byte;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4 && (diff != 0 || result < 0)) {
        CBattleManager* bm = CBattleManager::getInstance();
        ((void (*)(void*, u32, f32, f32))((void**)bm)[0x34 / 4])(bm, 8, lbl_eu_80667D30, lbl_eu_80667D34);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        func_801AD504(8);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D34 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667D34 && (diff != 0 || result < 0)) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 3);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4 &&
        ((diff != 0 || result < 0) ||
         ((s32)((s32(*)(void*))((void**)p)[0x2BC / 4])(p) != 0))) {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_805331C4[0];
        cb.mObj = lbl_eu_805331C4[1];
        cb.mDelta = lbl_eu_805331C4[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

// ---------------------------------------------------------------------------
// us-801a7578: When the vision field is active, ask each player's battle
// state whether it should show, and forward it to func_80133F48 (retail
// func_801A5E58).
// ---------------------------------------------------------------------------
void func_801A5E58(CVision* self) {
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    // Materialized timer-window check (retail evaluates the condition into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        f32 scl = lbl_eu_80667D38;
        if (func_800F477C(sub) != 0 && func_800F477C(sub)->b_42 == 1) {
            scl = lbl_eu_80667D3C;
        }
        // Accumulate a per-player float offset, then derive the final scale
        // from the sub object's id / the battle-manager flags, and write it out.
        int i = 0;
        do {
            void* p = func_8016FE34((int)getPlayer__Q22cf13CfGameManagerFi(i));
            if (p != 0) {
                void* r = ((void* (*)(void*))((void**)p)[0x290 / 4])(p);
                if (r != 0) {
                    s32 out;
                    if (func_80260264(r, 0x6c, &out) != 0) {
                        scl += (f32)(f64)out;
                    }
                }
            }
            i++;
        } while (i < 3);
        if (sub->field_824 & 0x20000) {
            scl = sub->field_0C->field_2C;
        }
        CBattleManager* bm = CBattleManager::getInstance();
        if (((void* (*)(void*, u32))((void**)bm)[0x28 / 4])(bm, 0x400)) {
            scl = lbl_eu_80667D00;
        }
        if (((void* (*)(void*, u32))((void**)bm)[0x28 / 4])(bm, 0x800)) {
            scl = lbl_eu_80667D40;
        }
        sub->field_830 = scl;
        func_801AD504(0x10);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D04 &&
        lbl_eu_80667D04 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_805331D0[0];
        cb.mObj = lbl_eu_805331D0[1];
        cb.mDelta = lbl_eu_805331D0[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A60B0(CVision* self) {
    // Materialized timer-window check (retail evaluates the condition into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        self->vt_2C(2);
        func_801BFE8C(0, 0x1bf, 0);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1c0, 0, 0, lbl_eu_80667CE0);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D04 &&
        lbl_eu_80667D04 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x328, 0);
        // Flag every matching player object; bail out with vt_20(1) if the
        // cast fails mid-iteration.
        u32 i = 0;
        while (i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620)) {
            void* elem = func_800F6EAC(func_80043F18(vdlist), i);
            void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
            if (obj == 0) {
                self->vt_20(1);
                __dt__80043E88(vdlist, -1);
                return;
            }
            ((void (*)(void*, u32))((void**)obj)[0x190 / 4])(obj, 0);
            i++;
        }
        __dt__80043E88(vdlist, -1);
        if (getGlobalSda() != 0) {
            getGlobalSda()[0xa8] = 0;
        }
        self->vt_18();
        self->vt_30(1);
        func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
        func_8006E5D8();
    }
    int w3;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D44 &&
        lbl_eu_80667D44 < self->field_26194 - self->field_261A0) {
        w3 = 1;
    } else {
        w3 = 0;
    }
    if (w3) {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_805331DC[0];
        cb.mObj = lbl_eu_805331DC[1];
        cb.mDelta = lbl_eu_805331DC[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A6340(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 == 0) ? 0 : &self->sub;
    CVisionFusion* fr = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* fr2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    // Retail materializes the equality test via mfcr/extrwi (value context).
    bool atBase = (lbl_eu_80667CD4 == self->field_2619C);
    if (atBase) {
        func_801537E0(&fr->field_3380);
    }
    // Materialized timer-window checks (retail evaluates both into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 0x3);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
        fr->field_3388 &= 0xFFFD;
        fr->field_04->v20(0x4000);
        if (fr != fr2) {
            fr2->field_3388 &= 0xFFFD;
            fr2->field_04->v20(0x4000);
        }
        sub->field_0C->field_7C = func_800F42AC(sub);
        self->vt_1C();
    }
}

// us-801aa0b0: Manage per-battle-slot vision FX. Iterates the battle
// manager's vision slots; when a slot matches `slot` it runs a timing/rounding
// check and either re-engages FX via func_801A8244 or clears its flag bits.
void func_801A897C(CVision* self, void* slot, void* r28) {
    if (slot != 0) {
        getInstance__Q22cf13CfGameManagerFv();
        if (func_8006EF04__Fi(0x400) != 0) {
            return;
        }
        CVisionBtlSlot* bs = (CVisionBtlSlot*)func_800EA444(CBattleManager::getInstance());
        if (bs == 0) {
            return;
        }
        if (bs->w_824 & 0x8000) {
            return;
        }
        if (func_800F4730() > 2) {
            return;
        }
        u32 x = *(u32*)((u8*)slot + 0x3f10);
        if (bs->w_00 != x && bs->w_04 != x) {
            return;
        }
        CVisionBattleObj* fu = (CVisionBattleObj*)func_8016FE34(func_800B708C__Fi(bs->w_00));
        if (fu == 0) {
            return;
        }
        if (fu->vf298()->w_04 != 0) {
            u32 v26 = bs->w_828;
            u32 v25 = bs->w_82C;
            void* r27 = func_800F4648(bs);
            func_800E921C(CBattleManager::getInstance(), bs, fu, &self->field_26194, &self->field_26198);
            if (func_800F46C0(bs, fu) != 0) {
                return;
            }
            f32 f1 = self->field_26194;
            f64 adj = (f1 > lbl_eu_80667CD4) ? lbl_eu_80667D18 : lbl_eu_80667D20;
            int r = (int)(f1 + adj);
            if ((u32)r27 != (u32)r || ((bs->w_824 >> 10) & 1) != ((bs->w_88 >> 12) & 1) ||
                v26 != bs->w_828 || v25 != bs->w_82C) {
                int rc = (int)self->field_26194;
                int xorv = (u32)r27 ^ (u32)rc;
                int r25v = ((u32)(xorv - (xorv & (u32)r27)) >> 1) >> 31;
                func_801A8244(&CBattleManager::getInstance()->mVision, bs, 2, r25v, 0);
                return;
            }
            if ((void*)fu != slot) {
                return;
            }
            if (r28 == 0) {
                return;
            }
            if (*(u16*)((u8*)r28 + 0xc) == 0x10) {
                if (func_80148778((u8*)slot + 8, 0x10) != 0) {
                    func_801A8244(&CBattleManager::getInstance()->mVision, bs, 1, 1, 0);
                    return;
                }
                if (*(f32*)((u8*)r28 + 0x20) > lbl_eu_80667CD4) {
                    func_801A8244(&CBattleManager::getInstance()->mVision, bs, 1, 1, 0);
                    return;
                }
            }
        } else if ((void*)fu == slot && r28 != 0 &&
                   *(u16*)((u8*)r28 + 0xc) == 0x9 &&
                   func_80148778((u8*)slot + 8, 0x9) != 0) {
            func_801A8244(&CBattleManager::getInstance()->mVision, bs, 4, 1, 0);
            return;
        }
        return;
    }

    // r4 == 0 path: walk the four 0x834-strided vision slots.
    f32 f31 = lbl_eu_80667CD4;
    CVisionBtlSlot* p = (CVisionBtlSlot*)&self->sub;
    for (int i = 0; i < 4; i++, p++) {
        if (p->w_00 == 0) {
            continue;
        }
        CVisionBattleObj* obj = (CVisionBattleObj*)func_8016FE34(func_800B708C__Fi(p->w_00));
        func_8016FE34(func_800B708C__Fi(p->w_04));
        if (obj == 0) {
            self->vt_20(1);
            continue;
        }
        void* subp = *(void**)((u8*)obj + 4);
        void* sr = ((void* (*)(void*))(*(void***)subp)[0x30 / 4])(subp);
        int local0 = *(int*)sr;
        if (func_80174C98(obj, &local0, 0x802) != 0) {
            self->vt_20(1);
            continue;
        }
        if (p->w_824 & 0x8000) {
            int local1 = *(int*)((void* (*)(void*))(*(void***)subp)[0x30 / 4])(subp);
            if (func_80174C98(obj, &local1, 0x801) == 0 &&
                func_80174C98(obj, &local1, 0x13) == 0 &&
                func_80174C98(obj, &local1, 0x12) == 0 &&
                func_80174C98(obj, &local1, 0x800) == 0 &&
                func_80174C98(obj, &local1, 0x6) == 0) {
                self->vt_20(1);
                continue;
            }
        }
        if (func_800F4424(p) == f31) {
            u32 flags = p->w_824;
            if (!((flags & 1) && (flags & 0x20)) && !((flags & 2) && (flags & 0x40)) &&
                !((flags & 4) && (flags & 0x80)) && !((flags & 8) && (flags & 0x100)) &&
                !((flags & 0x10) && (flags & 0x200))) {
                if (!(flags & 0x8000)) {
                    func_801AF934(0);
                    p->w_824 |= 0x10000;
                    if (p->w_824 & 0x20000) {
                        func_8009D018(0x30e3, 0);
                        func_801412D0(0x375);
                        func_8013F244();
                    }
                    continue;
                }
            }
        }
        int r28b = 0;
        if (!(p->w_824 & 1) && obj->vf2BC() != 0) {
            r28b = 5;
        }
        if (!(p->w_824 & 2) && r28b == 0) {
            u32 locals[9];
            for (int k = 0; k < 9; k++) {
                locals[k] = *(u32*)((void* (*)(void*))(*(void***)subp)[0x30 / 4])(subp);
            }
            if (func_80174C98(obj, (int*)&locals[0], 0x806) == 0 &&
                func_80174C98(obj, (int*)&locals[1], 0x800) == 0 &&
                func_80174C98(obj, (int*)&locals[2], 0x805) == 0 &&
                func_80174C98(obj, (int*)&locals[3], 0x18) == 0 &&
                func_80174C98(obj, (int*)&locals[4], 0x6) == 0 &&
                func_80174C98(obj, (int*)&locals[5], 0x801) == 0 &&
                func_80174C98(obj, (int*)&locals[6], 0x13) == 0 &&
                func_80174C98(obj, (int*)&locals[7], 0x12) == 0 &&
                func_80174C98(obj, (int*)&locals[8], 0x19) == 0) {
                r28b = 4;
            }
        }
        if (func_80148778((u8*)obj + 8, 0xf) != 0 ||
            func_80148778((u8*)obj + 8, 0x9) != 0 ||
            func_80148778((u8*)obj + 8, 0xb) != 0) {
            r28b = 4;
        }
        if (func_80148778((u8*)obj + 8, 0xc) != 0 &&
            (func_800F477C(p) == 0 || (func_800F477C(p)->w_78 & 1))) {
            r28b = 4;
        }
        if (p->w_824 & 2) {
            if (p->w_824 & 0x40) {
                p->w_824 &= ~(0x2 | 0x20);
            }
        }
        if (p->w_824 & 4) {
            if (p->w_824 & 0x80) {
                p->w_824 &= ~(0x4 | 0x40);
            }
        }
        if (p->w_824 & 8) {
            if (p->w_824 & 0x100) {
                p->w_824 &= ~(0x8 | 0x80);
            }
        }
        if (r28b != 0) {
            func_801A8244(self, p, r28b, 1, 0);
        }
    }
}


// us-801a7c60: Manage vision FX on the sub-object. When the vision field is
// at baseline (field_2619C == 0) it releases/clears via the short path; otherwise
// it drives the FX setup path on the fusion's parameter block.
void func_801A6540(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    if (sub == 0) {
        return;
    }
    CVisionFusion* fu = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    if (fu == 0) {
        self->vt_20(1);
        return;
    }
    if (self->field_2619C == lbl_eu_80667CD4) {
        // Short (release) path: engage only within the D48 timing window.
        if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D48 &&
            self->field_26194 - self->field_261A0 > lbl_eu_80667D48) {
            func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
            CVisionPtmf cb;
            cb.mPfn = lbl_eu_805331F4[0];
            cb.mObj = lbl_eu_805331F4[1];
            cb.mDelta = lbl_eu_805331F4[2];
            self->mPtmf = cb;
            self->field_2619C = lbl_eu_80667CD4;
            self->field_261A0 = self->field_26194;
            __ptmf_test(&cb);
        }
        return;
    }

    // Long (setup) path: begin driving the FX on the fusion parameter block.
    func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 0x13);
    sub->field_824 |= 0x100000;
    func_801537F0(&fu->field_3380);
    CVisionFxParam p;
    memset(&p, 0, sizeof(p));
    if (sub->field_0C != 0) {
        p.b_0D = 0x5a;
        p.h_12 = *((u8*)sub->field_0C + 0x77);
        p.f_14 = lbl_eu_80667CD4;
        p.h_10 = 0;
        p.b_06 = 6;
    } else {
        p.b_0D = 0xd;
        p.h_12 = 0xFFFF;
        p.f_14 = lbl_eu_80667CD4;
        p.h_10 = 0;
        p.b_06 = 6;
    }
    func_8014AC38(&fu->field_3380, &p);
    func_800BE12C((u8*)fu + 0x3e9c, 1, 0, -1, 1);
    if (func_8014B8BC(&fu->field_3380, &p) == 0) {
        func_801537E0(&fu->field_3380);
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
        // ... (FX-callback install continues in the elided retail middle)
    }
}

// ---------------------------------------------------------------------------
// us-801a819c: Validate a battle object / slot and, when it passes, install
// the state-change callback (retail func_801A6A7C).
// ---------------------------------------------------------------------------
int func_801A6A7C(CVision* self, CVisionObj* obj) {
    if (__ptmf_test(&self->mPtmf) == 0) {
        return 0;
    }
    if ((obj->field_3F00 & 0x4) == 0) {
        return 0;
    }
    CVisionSub* sub = (self->sub.field_00 == 0) ? 0 : &self->sub;
    if (sub == 0) {
        return 0;
    }
    if (obj->field_3F10 != sub->field_00) {
        return 0;
    }
    if (func_80148778(&obj->field_08, 0x113) == 0 &&
        func_80148778(&obj->field_08, 0x114) == 0 &&
        func_80148778(&obj->field_08, 0x115) == 0) {
        return 0;
    }
    if ((sub->field_824 & 0x4000) == 0) {
        return 0;
    }
    CVisionPtmf cb;
    cb.mPfn = lbl_eu_80533200[0];
    cb.mObj = lbl_eu_80533200[1];
    cb.mDelta = lbl_eu_80533200[2];
    self->mPtmf = cb;
    self->field_2619C = lbl_eu_80667CD4;
    self->field_261A0 = self->field_26194;
    __ptmf_test(&cb);
    return 1;
}

// us-801a82ec: Guard chain deciding whether vision may be engaged on `obj`,
// then per-player timing checks and a vision-callback install. Returns 1 on success.
int func_801A6BCC(CVision* self, CVisionObj* obj, CVisionObj* r5) {
    if ((obj->field_3F00 & 0x4) == 0) {
        return 0;
    }
    if (((s32 (*)(void*))((void**)obj)[0x2BC / 4])(obj) != 0) {
        return 0;
    }
    if (func_eu_80053FD4() != 0) {
        return 0;
    }
    u8 nc = *((u8*)CBattleManager::getInstance() + 0x1aa);
    if (nc >= 1 && nc <= 0x18) {
        return 0;
    }
    if (*(s16*)((u8*)CBattleManager::getInstance() + 0x20c8) != 0) {
        return 0;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x40000) != 0) {
        return 0;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (CfRes_getE24Bit22() != 0) {
        return 0;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x200) != 0) {
        return 0;
    }
    if (((void* (*)(void*, u32))((void**)CBattleManager::getInstance())[0x28 / 4])(
            CBattleManager::getInstance(), 0x200) != 0) {
        return 0;
    }
    if (func_801B481C() != 0) {
        return 0;
    }

    // Per-player timing check: flag whether vision should engage on a target.
    int engage = 0;
    CVisionFusion* target = 0;
    for (int i = 0; i < 4; i++) {
        void* p = getPlayer__Q22cf13CfGameManagerFi(i);
        if (p == 0) {
            continue;
        }
        CVisionFusion* fr = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(0));
        f32 a = ((f32 (*)(void*))((void**)p)[0x128 / 4])(p);
        if (a >= lbl_eu_80667CD4) {
            engage = 1;
        }
        target = fr;
    }
    if (engage != 0) {
        if (((s32 (*)(void*, CVisionObj*, void*))((void**)self)[0x14 / 4])(self, obj, target) == 0) {
            return 0;
        }
    }
    return 1;
}

int func_801A70DC(CVision* self, void* obj, void* obj2) {
    if (obj == 0) {
        self->vt_20(1);
        return 0;
    }
    void* f3e74 = *(void**)((u8*)obj + 0x3e74);
    if (f3e74 == 0) {
        self->vt_20(1);
        return 0;
    }
    CVisionSub* subs = &self->sub;
    CVisionSub* sub = (subs[0].field_00 != 0) ? subs : 0;
    if (sub != 0) {
        if (*(u32*)((u8*)f3e74 + 0x78) & 0x8000) {
            self->vt_1C();
        } else if (sub->field_00 != *(u32*)((u8*)obj + 0x3f10)) {
            return 0;
        }
    }
    if (sub == 0) {
        u32 want = *(u32*)((u8*)obj + 0x3f10);
        if (want != 0) {
            for (int i = 0; i < 4; i++) {
                if (subs[i].field_00 == want) {
                    sub = subs + i;
                    break;
                }
            }
        }
        if (sub != 0) {
            sub = 0;
        } else {
            for (int i = 0; i < 4; i++) {
                if (subs[i].field_00 == 0) {
                    sub = &subs[i];
                    break;
                }
            }
        }
    }
    if (sub == 0) {
        return 0;
    }
    f32 f1 = *(f32*)((u8*)f3e74 + 0x2c);
    if (*(f32*)((u8*)f3e74 + 0x2c) != lbl_eu_80667CD4) {
        f1 = *(f32*)((u8*)f3e74 + 0x7c) / f1;
    } else {
        f1 = lbl_eu_80667CD8;
    }
    if (f1 == lbl_eu_80667CD4) {
        return 0;
    }
    func_800F449C(sub);
    sub->field_00 = *(u32*)((u8*)obj + 0x3f10);
    sub->field_04 = *(u32*)((u8*)obj2 + 0x3f10);
    sub->field_0C = (CVisionSubRef*)f3e74;
    if (*(u32*)((u8*)f3e74 + 0x78) & 0x8000) {
        sub->field_824 |= 0x20000;
        func_8009D018(0x30e3, 0);
        func_801412D0(0x375);
        func_8013F244();
    }
    if (*(u32*)((u8*)obj + 0x3f00) & 0x4) {
        u16 h = *(u16*)((u8*)obj + 0x3f28);
        if (h == 0x967 || h == 0x96b || h == 0x96c || h == 0x969) {
            sub->field_824 |= 0x40000;
        }
    }
    *(f32*)((u8*)f3e74 + 0x7c) = *(f32*)((u8*)f3e74 + 0x2c);
    func_8016FF14(obj, (u8*)self + 0x20d4);
    if (obj2 != 0 && obj2 != obj) {
        func_8016FF14(obj2, (u8*)self + 0x68ec);
    }
    u8 vdlist[8];
    func_80043D90(vdlist);
    func_800F4A98(func_80043F18(vdlist), 0x20, 0);
    for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620); i++) {
        void* elem = func_800F6EAC(func_80043F18(vdlist), i);
        void* actor = __dynamic_cast(elem, 0, &__RTTI__Q22cf13CfObjectActor, &lbl_eu_806618F0, 0);
        if (actor != obj && actor != obj2) {
            *(u16*)((u8*)actor + 0x3388) &= ~0x8;
            func_8016FF14(actor, (u8*)self + 0x20d4 + i * 0x4818);
        }
    }
    __dt__80043E88(vdlist, -1);
    *(f32*)((u8*)f3e74 + 0x7c) = lbl_eu_80667D10;
    self->field_26194 = lbl_eu_80667CD4;
    self->field_26198 = lbl_eu_80667CD4;
    lbl_eu_80663DA0 |= 0x1;
    lbl_eu_80663E24 |= 0x400;
    func_802A1FB4(obj, obj2);
    if (sub->field_824 & 0x40000) {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533224[0];
        cb.mObj = lbl_eu_80533224[1];
        cb.mDelta = lbl_eu_80533224[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    } else {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533230[0];
        cb.mObj = lbl_eu_80533230[1];
        cb.mDelta = lbl_eu_80533230[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
    return 1;
}

// ---------------------------------------------------------------------------
// us-801a8c10: Vision state check. If the installed callback matches the
// 0x114 table entry, clear the vision field; otherwise branch on the 0x120
// entry (retail func_801A74DC).
// ---------------------------------------------------------------------------
void func_801A74DC(CVision* self) {
    CVisionPtmf cb114;
    cb114.mPfn = lbl_eu_80533128[0x45];
    cb114.mObj = lbl_eu_80533128[0x46];
    cb114.mDelta = lbl_eu_80533128[0x47];
    if (__ptmf_cmpr(&self->mPtmf, &cb114) == 0) {
        self->field_26194 = lbl_eu_80667CD4;
        lbl_eu_80663DA0 &= 0x7E;
        lbl_eu_80663E24 &= 0xFBFFFFFF;
        CVisionSub* sub;
        if (self->sub.field_00 == 0) {
            sub = 0;
        } else {
            sub = &self->sub;
        }
        if (sub->field_824 & 0x20000) {
            func_8009D018(0x30e3, 1);
        }
        // Per-player: fetch a stat, convert to float, push through the 0x154
        // callback on the fusion state.
        int i = 0;
        do {
            void* p = func_8016FE34((int)getPlayer__Q22cf13CfGameManagerFi(i));
            if (p != 0) {
                void* r = ((void* (*)(void*))((void**)p)[0x290 / 4])(p);
                if (r != 0) {
                    s32 out;
                    if (func_80260264(r, 0x39, &out) != 0) {
                        ((void (*)(void*, f32))((void**)r)[0x154 / 4])(r, (f32)(f64)out);
                    }
                }
            }
            i++;
        } while (i < 3);
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533128[0x4B];
        cb.mObj = lbl_eu_80533128[0x4C];
        cb.mDelta = lbl_eu_80533128[0x4D];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    } else {
        CVisionPtmf cb120;
        cb120.mPfn = lbl_eu_80533128[0x48];
        cb120.mObj = lbl_eu_80533128[0x49];
        cb120.mDelta = lbl_eu_80533128[0x4A];
        if (__ptmf_cmpr(&self->mPtmf, &cb120) == 0) {
            self->vt_20(0);
        } else {
            self->vt_20(1);
        }
    }
}

// us-801a8e38: Drive vision FX updates on the two sub-object fusions and on
// every other in-play actor, then run the per-slot FX / flag management.
void func_801A7704(CVision* self) {
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    CVisionBattleObj* p1 = (CVisionBattleObj*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionBattleObj* p2 = (CVisionBattleObj*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0) {
        self->vt_20(1);
        return;
    }
    if (p2 == 0) {
        self->vt_20(1);
        return;
    }
    p1->sub8.s30(0x10);
    func_80170AB0(p1, &self->unk20D4[0]);
    if (p2 != 0 && p1 != p2) {
        p2->sub8.s30(0x10);
        func_80170AB0(p2, &self->unk20D4[1]);
        func_80174C24(p2, 0x40);
    }

    u8 vdlist[8];
    func_80043D90(vdlist);
    func_800F4A98(func_80043F18(vdlist), 0x20, 0);
    u32 off = 0;
    for (u32 i = 0; i < ((CVisionEnumList*)func_80043F18(vdlist))->count; off += 0x4818, i++) {
        void* elem = func_800F6EAC(func_80043F18(vdlist), i);
        void* obj = __dynamic_cast(elem, 0, &__RTTI__Q22cf13CfObjectActor, &lbl_eu_806618F0, 0);
        if (obj == (void*)p1 || obj == (void*)p2) {
            continue;
        }
        func_80170AB0(obj, (u8*)self + (off + 0x10000) - 0x4efc);
        func_80174C24(obj, 0x40);
    }
    __dt__80043E88(vdlist, -1);
    func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 3);

    // Push the five sub-item blocks onto the fusion, then copy the first
    // block back into the fusion's own second block.
    CVisionInitBlock* src = sub->items;
    int i = 0;
    do {
        *p1->vf29C(i) = *src;
        i++;
        src++;
    } while (i < 5);
    CVisionInitBlock* d1 = p1->vf29C(0);
    CVisionInitBlock* d2 = p1->vf2A4();
    *d2 = *d1;
    if (p1 != p2) {
        p1->vf2C4(p2, lbl_eu_80667CD4, lbl_eu_80667D50, lbl_eu_80667CD4);
    }
    sub->field_0C->field_7C = lbl_eu_80667CFC;
    func_801C028C(0, 0);
    func_801C028C(1, 0);
    func_802A1DF0(0);

    // Enumerate the 0x8000000-tagged list; raise a state flag on every
    // actor whose battle-state id is outside [0xa1,0xa4] and != 0xb8.
    u8 vdlist2[8];
    func_80043D90(vdlist2);
    func_800F4A98(func_80043F18(vdlist2), 0x8000000, 0);
    for (u32 i = 0; i < ((CVisionEnumList*)func_80043F18(vdlist2))->count; i++) {
        void* elem = func_800F6EAC(func_80043F18(vdlist2), i);
        void* obj = __dynamic_cast(elem, 0, &lbl_eu_80661970, &lbl_eu_806618F0, 0);
        if (obj == 0) {
            self->vt_20(1);
            __dt__80043E88(vdlist2, -1);
            return;
        }
        CVisionActorRef* act = (CVisionActorRef*)obj;
        // vfn dispatch: load the vtable at obj+0, then the slot at +0x1a4.
        if (((int (*)(void*, u32))(*(void***)obj)[0x1a4 / 4])(obj, 2) != 0) {
            u16 v = act->field_8C;
            if (v >= 0xa1 && v <= 0xa4) {
                // id in [0xa1,0xa4]: leave the flag alone
            } else if (v >= 0xb8 && v <= 0xb8) {
                // id == 0xb8: leave the flag alone
            } else {
                act->field_68 |= 0x40;
            }
        }
    }
    __dt__80043E88(vdlist2, -1);
}


void func_801A7D6C(CVision* self, void* r4) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    u8 vdlist[8];
    func_80043D90(vdlist);
    func_800F4A98(func_80043F18(vdlist), 0x328, 0);
    for (u32 i = 0;; i++) {
        void* list = func_80043F18(vdlist);
        if (i >= *(u32*)((u8*)list + 0x620)) break;
        void* elem = func_800F6EAC(list, i);
        void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
        if (obj == 0) {
            self->vt_20(1);
            __dt__80043E88(vdlist, -1);
            return;
        }
        ((void (*)(void*, u32))((void**)obj)[0x190 / 4])(obj, 0);
    }
    if (getGlobalSda() != 0) {
        getGlobalSda()[0xa8] = 0;
    }
    self->vt_30(1);
    if (r4 != 0) {
        func_80081E90__Q22cf13CfGameManagerFv(1, 0, 0);
        func_8006E5D8();
    }
    func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
    func_801AF934(0);
    if (sub != 0) {
        CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
        CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
        if (p1 != 0) {
            p1->field_3388 &= 0xFFFD;
            p1->field_04->v20(0x4000);
        }
        if (p2 != 0) {
            p2->field_3388 &= 0xFFFD;
            p2->field_04->v20(0x4000);
        }
        func_800F449C(sub);
    }
    func_800F4A98(func_80043F18(vdlist), 0x20, 0);
    for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620); i++) {
        void* elem = func_800F6EAC(func_80043F18(vdlist), i);
        CVisionFusion* actor = (CVisionFusion*)__dynamic_cast(elem, 0, &__RTTI__Q22cf13CfObjectActor, &lbl_eu_806618F0, 0);
        actor->field_3388 &= 0xFFFD;
        actor->field_04->v20(0x4000);
    }
    self->field_26194 = lbl_eu_80667CD4;
    func_801C028C(0, 0);
    func_801C028C(1, 0);
    func_802A1DF0(0);
    lbl_eu_80663E24 &= 0xFFFFFFC0;
    lbl_eu_80663DA0 &= 0x7F;
    CVisionPtmf cb;
    cb.mPfn = __ptmf_null[0];
    cb.mObj = __ptmf_null[1];
    cb.mDelta = __ptmf_null[2];
    self->mPtmf = cb;
    self->field_2619C = lbl_eu_80667CD4;
    self->field_261A0 = self->field_26194;
    __ptmf_test(&cb);
    __dt__80043E88(vdlist, -1);
}

int* func_801A8070(int* param) {
    if (param[1] == 0) {
        return 0;
    }
    return &param[1];
}

// ---------------------------------------------------------------------------
// us-801a986c: Clear all four vision effect slots (retail func_801A8138).
// ---------------------------------------------------------------------------
void func_801A8138(CVision* self) {
    for (int i = 0; i < 4; i++) {
        if (self->effectArray[i] != 0) {
            self->effectArray[i]->field_B0 = 0;
            self->effectArray[i]->field_68 |= 0x40;
        }
        self->effectArray[i] = 0;
    }
}

// us-801a9978: Validate a battle slot against the vision state and, per the
// mode `r26`, either route its FX type or engage per-player FX on it.
void func_801A8244(CVision* self, void* r25, int r26, int r27, int r28) {
    CVisionBattleObj* fu = (CVisionBattleObj*)func_8016FE34(func_800B708C__Fi(*(u32*)((u8*)r25 + 0x00)));
    func_8016FE34(func_800B708C__Fi(*(u32*)((u8*)r25 + 0x04)));
    if (fu == 0) {
        self->vt_20(1);
        return;
    }
    int r30 = 0;
    int r29 = 0;
    if (r26 == 1) {
        if (*(u32*)((u8*)r25 + 0x824) & 0x8) {
            return;
        }
    } else if (r26 == 2) {
        if (*(u32*)((u8*)r25 + 0x824) & 0x4) {
            return;
        }
    } else if (r26 == 3) {
        if (*(u32*)((u8*)r25 + 0x824) & 0x40) {
            return;
        }
    }
    if (r26 != 0) {
        self->unk261C4.field_68 = 0;
        self->unk261C4.field_64 = 0;
    }
    if (r26 != 0) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x20, 0);
        for (u32 i = 0; i < ((CVisionEnumList*)func_80043F18(vdlist))->count; i++) {
            CVisionBattleObj* obj = (CVisionBattleObj*)func_8016FE34((int)func_800F6EAC(func_80043F18(vdlist), i));
            if (obj->vf308() == 3) {
                obj->vf304(4);
            } else if (obj->vf308() == 0) {
                obj->vf304(2);
            }
        }
        __dt__80043E88(vdlist, -1);
    }
    if (r26 != 0) {
        if (r27 != 0) {
            func_802A2078(r26, r28, fu);
        }
        func_80280D04(r26);
    }

    // Per-mode FX routing. The cases fall through; each block sets its flag
    // bit (and clears the bit the next mode owns) and lazily engages the
    // gauge, then the r26 < 4 guards push a {u32,f32,u32} entry into the
    // element array at unk261C4.w60.
    switch (r26) {
    case 5:
        *(u32*)((u8*)r25 + 0x824) |= 0x1;
        func_801ACD5C(1);
        func_800F449C(r25);
        func_8018C820((u8*)CBattleManager::getInstance() + 0x194, 0);
        func_800F3970(CBattleManager::getInstance(), fu, 0, 0x17, 0);
        r30 = 1;
    case 4:
        *(u32*)((u8*)r25 + 0x824) = (*(u32*)((u8*)r25 + 0x824) | 0x2) & ~0x40;
        func_801ACD5C(2);
        if (r30 == 0) {
            func_8018C820((u8*)CBattleManager::getInstance() + 0x194, 0);
            func_800F3970(CBattleManager::getInstance(), fu, 0, 0x18, 0);
            r30 = 1;
        }
    case 3:
        *(u32*)((u8*)r25 + 0x824) = (*(u32*)((u8*)r25 + 0x824) | 0x4) & ~0x80;
        func_801ACD5C(4);
        if (r30 == 0) {
            func_8018C820((u8*)CBattleManager::getInstance() + 0x194, 0);
            func_800F3970(CBattleManager::getInstance(), fu, 0, 0x19, 0);
            r30 = 1;
        }
        if (r26 < 4) {
            u32 t3 = 0;
            CVisionU32F32U32* base = (CVisionU32F32U32*)self->unk261C4.w60;
            u32 sum = self->unk261C4.field_64 + self->unk261C4.field_68;
            u32 idx = sum / self->unk261C4.w6C;
            CVisionU32F32U32* el = &base[sum - idx * self->unk261C4.w6C];
            el->a = 3;
            el->b = lbl_eu_80667CE4 * (double)(u32)t3 + lbl_eu_80667CF0;
            el->c = 0;
            self->unk261C4.field_68++;
        }
        r29 = 1;
    case 2:
        *(u32*)((u8*)r25 + 0x824) = (*(u32*)((u8*)r25 + 0x824) | 0x8) & ~0x100;
        func_801ACD5C(8);
        if (*(u16*)((u8*)r25 + 0x88) & 0x1000) {
            *(u32*)((u8*)r25 + 0x824) |= 0x400;
        } else {
            *(u32*)((u8*)r25 + 0x824) &= ~0x400;
        }
        if (r30 == 0) {
            func_8018C820((u8*)CBattleManager::getInstance() + 0x194, 0);
            func_800F3970(CBattleManager::getInstance(), fu, 0, 0x1a, 0);
            r30 = 1;
        }
        if (r26 < 4) {
            CVisionU32F32U32* base = (CVisionU32F32U32*)self->unk261C4.w60;
            u32 sum = self->unk261C4.field_64 + self->unk261C4.field_68;
            u32 idx = sum / self->unk261C4.w6C;
            CVisionU32F32U32* el = &base[sum - idx * self->unk261C4.w6C];
            el->a = 4;
            el->b = lbl_eu_80667CE4 * (double)(u32)r29 + lbl_eu_80667CF0;
            el->c = 0;
            self->unk261C4.field_68++;
        }
        r29++;
    case 1:
        *(u32*)((u8*)r25 + 0x824) = ((*(u32*)((u8*)r25 + 0x824) | 0x10) & ~0x200) | 0x4000;
        func_801ACD5C(0x10);
        if (r30 == 0) {
            func_8018C820((u8*)CBattleManager::getInstance() + 0x194, 0);
            func_800F3970(CBattleManager::getInstance(), fu, 0, 0x1b, 0);
        }
        {
            CBattleManager* bm = CBattleManager::getInstance();
            ((void (*)(void*, u32, f32, f32))(*(void***)bm)[0x34 / 4])(
                bm, 0x13, lbl_eu_80667D54,
                lbl_eu_80667CE4 * (double)(u32)r26);
        }
        if (r26 < 4) {
            CVisionU32F32U32* base = (CVisionU32F32U32*)self->unk261C4.w60;
            u32 sum = self->unk261C4.field_64 + self->unk261C4.field_68;
            u32 idx = sum / self->unk261C4.w6C;
            CVisionU32F32U32* el = &base[sum - idx * self->unk261C4.w6C];
            el->a = 5;
            el->b = lbl_eu_80667CE4 * (double)(u32)r29 + lbl_eu_80667CF0;
            el->c = 0;
            self->unk261C4.field_68++;
        }
    default:
        break;
    }

    lbl_eu_80663E28 |= 0x8;
    if (r26 != 5) {
        return;
    }
    // Engage FX on each of the three players whose fusion state is ready.
    {
        u32 data[5];
        for (int i = 0; i < 5; i++) data[i] = lbl_eu_80503F80[i];
        CVisionArg5 a5;
        a5.w[0] = data[0];
        a5.w[1] = data[1];
        a5.w[2] = data[2];
        a5.w[3] = data[3];
        a5.w[4] = data[4];
        for (int i = 0; i < 3; i++) {
            void* pl = func_8016FE34((int)getPlayer__Q22cf13CfGameManagerFi(i));
            if (pl == 0) continue;
            CVisionBattleObj* pobj = (CVisionBattleObj*)pl;
            if (pobj->vf290() == 0) continue;
            if (func_8026178C((void*)pobj->vf290(), 0x8c) == 0) continue;
            u32 r = (u32)fu->vf1E8(a5);
            f32 scale = *(f32*)((u8*)data + 0x10);
            int n = (int)(scale * (double)(u32)r);
            void* v = func_8009EC9C();
            func_800A26A4(v, n, 0, (int)fu->vf108(), 0, 0, 1);
        }
    }
}

// ---------------------------------------------------------------------------
// us-801a4df0: UnkClass_801A36D0 deleting destructor (__dt__801A36D0).
// Clears the sub-object at +0x3d8c (fields at +0x3f90/+0x3f94), then frees.
// ---------------------------------------------------------------------------
extern "C" void* __dt__801A36D0(UnkClass_801A36D0* self, int deleting) {
    if (self != 0) {
        if ((char*)self + 0x3d8c != 0) {
            *(u32*)((char*)self + 0x3f94) = 0;
            *(u32*)((char*)self + 0x3f90) = 0;
        }
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// us-801a4e48: UnkClass_801A3728 deleting destructor (__dt__801A3728).
// Clears field_68/field_64, then frees when the deleting flag is set.
// ---------------------------------------------------------------------------
extern "C" void* __dt__801A3728(UnkClass_801A3728* self, int deleting) {
    if (self != 0) {
        self->field_68 = 0;
        self->field_64 = 0;
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

cf::CVision::~CVision() {
    vt_34();
    cf::UnkClass_801A3728* m = &this->unk261C4;
    if (m != NULL) {
        m->field_64 = 0;
        m->field_68 = 0;
    }
    __destroy_arr(&this->unk20D4[0],
                  reinterpret_cast<ConstructorDestructor*>(
                      const_cast<void*>(reinterpret_cast<const void*>(&__dt__801A36D0))),
                  sizeof(CVisionSlot), 8);
}

// ---------------------------------------------------------------------------
// us-801a7c18: Release the zero/zero slot of the battle manager's vision
// sub-object (retail func_801A64F8).
// ---------------------------------------------------------------------------
void func_801A64F8() {
    if (CBattleManager::getInstance() &&
        &CBattleManager::getInstance()->mVision) {
        func_801A897C(&CBattleManager::getInstance()->mVision, 0, 0);
    }
}

// ---------------------------------------------------------------------------
// us-801aa050: Release an arbitrary slot of the battle manager's vision
// sub-object (retail func_801A891C). Args are forwarded untouched.
// ---------------------------------------------------------------------------
void func_801A891C(int a, int b) {
    if (CBattleManager::getInstance() &&
        &CBattleManager::getInstance()->mVision) {
        func_801A897C(&CBattleManager::getInstance()->mVision, a, b);
    }
}

// ---------------------------------------------------------------------------
// us-801a97c0: Lazily create the vision effect at `index` and record the
// owning CVision back-pointer (retail func_801A808C).
// ---------------------------------------------------------------------------
void func_801A808C(CVision* self, int index) {
    if (self->effectArray[index] == 0) {
        CVisionEffect* eff =
            (CVisionEffect*)func_800451D8(lbl_eu_80503F60[index].field_00, 0);
        self->effectArray[index] = eff;
        if (eff != 0) {
            eff->field_B0 = (u32)self;
        }
    }
}

// ---------------------------------------------------------------------------
// us-801a9830: Clear the vision effect at `index` (retail func_801A80FC).
// ---------------------------------------------------------------------------
void func_801A80FC(CVision* self, int index) {
    if (self->effectArray[index] != 0) {
        self->effectArray[index]->field_B0 = 0;
        self->effectArray[index]->field_68 |= 0x40;
    }
    self->effectArray[index] = 0;
}

// ---------------------------------------------------------------------------
// us-801a9930: Null out the first effect whose object identity matches
// `value` (retail func_801A81FC).
// ---------------------------------------------------------------------------
void func_801A81FC(CVision* self, u32 value) {
    for (int i = 0; i < 4; i++) {
        if (self->effectArray[i] == (CVisionEffect*)value) {
            self->effectArray[i] = 0;
            return;
        }
    }
}

// ---------------------------------------------------------------------------
// us-801aa980: Like func_801A81FC, but also clears the effect's owning
// back-pointer before nulling the slot (retail func_801A924C).
// ---------------------------------------------------------------------------
void func_801A924C(CVision* self, u32 value) {
    for (int i = 0; i < 4; i++) {
        if (self->effectArray[i] == (CVisionEffect*)value) {
            CVisionEffect* e = self->effectArray[i];
            e->field_B0 = 0;
            self->effectArray[i] = 0;
            return;
        }
    }
}

void cf::CVision::func_801A929C(u32 r4) {
    f32 scl = (r4 != 0) ? lbl_eu_80667CD4 : lbl_eu_80667CF0;
    for (int i = 0; i < 4; i++) {
        if (effectArray[i] != 0) {
            func_800ACC50(effectArray[i], scl);
            if (effectArray[i]->mSchedule != 0) {
                func_804E36DC(effectArray[i]->mSchedule, lbl_eu_80667CF0);
            }
        }
    }
}