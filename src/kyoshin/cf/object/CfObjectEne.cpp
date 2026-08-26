#include "kyoshin/cf/CBattleManagerApi.hpp"
#include <types.h>
#include "monolib/scn/CScnTimeApi.hpp"
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
// func_80174B4C / func_80174C98: single unified decls on the owner header.
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "kyoshin/cf/object/CfObjectEne.hpp" // cf::CfObjectEne + CfEneVtables/CfEneTailView/CfEneReloadSlot views

// Global gimmick-settings singleton accessor (defined in code_801F3BE0.cpp);
// this TU only null-tests it and forwards it to opaque-taking helpers. Typed
// after the owner declaration on CSuddenCommu.hpp (the singleton behind the
// lbl_eu_80664658 .sbss word).
namespace cf {
struct CSuddenCommuGlobal;
}
extern "C" cf::CSuddenCommuGlobal* getUnk80664658(); // getter for the lbl_eu_80664658 holder


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


struct BSIf {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void vf0038();
    virtual void vf003C();
    virtual void vf0040();
    virtual void vf0044();
};

// us-800ae154: CfObjectEne ctor. The header's C++ MI layout is +0x80 off
// retail (see the CfObjectActor dtor note below), so a real member ctor
// would store the subobject vtables at the wrong offsets; written as a free
// function with the verbatim __ name (same as __ct__cf_CfResReloadImpl).
// Chains the CfObjectActor base ctor, then overwrites the four subobject
// vtables (lbl_eu_80528A18 + 0x0/0xC/0x36C/0x37C), zeroes the tail fields,
// and placement-constructs a CfResReloadImpl child at +0x3F4C whose parent
// arg is `self + 0x3E9C` (the null-guarded address form retail emits).
cf::CfObjectEne* __ct__cf_CfObjectEne(cf::CfObjectEne* self) {
    __ct__Q22cf13CfObjectActorFv((cf::CfObjectActor*)self);
    u8* v = (u8*)lbl_eu_80528A18;
    cf::CfEneVtables* vt = (cf::CfEneVtables*)self;
    vt->vt0 = (u32)v;
    vt->vt8 = (u32)(v + 0xC);
    vt->vt3380 = (u32)(v + 0x36C);
    vt->vt3E9C = (u32)(v + 0x37C);
    cf::CfEneTailView* tail = (cf::CfEneTailView*)self;
    tail->field_0x45C0 = 0;
    tail->field_0x45C6 = 0;
    tail->field_0x45C8 = 0;
    tail->field_0x45CA = 0;
    tail->field_0x45CC = 0.0f;
    void* mem = mtl::MemManager::allocate(0x20, func_80061FFC());
    if (mem != 0) {
        // Retail: mr r4, r31 (default parent = self, i.e. 0 in the null case)
        // then a guarded addi for the +0x3E9C CfObjectMove-subobject address.
        u8* parent = (u8*)self;
        if (self != 0)
            parent = (u8*)self + 0x3E9C;
        mem = __ct__cf_CfResReloadImpl((cf::CfResReloadImpl*)mem,
                                       (cf::CfResReloadParent*)parent);
    }
    ((cf::CfEneReloadSlot*)self)->field_0x3F4C = mem;
    return self;
}

// us-800ae274: CAIAction dtor. Clears the slot-block tail words, guarded by
// addic.-beq null checks on the block addresses at +4 and +0xC (MWCC cannot
// fold `&this->unk4 != 0` / `addr+8 != 0`, so both stay as folded address
// tests). The deleting-dtor wrapper frees when the flag > 0.
cf::CAIAction::~CAIAction() {
    cf::CfCAIActionBlockView* blk = (cf::CfCAIActionBlockView*)&this->unk4;
    if (blk != NULL) {
        if (&blk->field_8 != NULL) {
            blk->field_210 = 0;
            blk->field_20C = 0;
        }
    }
}

// us-800ae1f4: CfObjectActor dtor. The retail explicitly destroys the
// CfObjectMove subobject at self+0x3E9C (non-deleting flag 0, direct bl), then
// runs the CAIAction subobject cleanup inline (address-null-guarded zeroing of
// CAIAction+0x210/+0x214). Retail emits NO class vptr store and no automatic
// base destruction, so the retail class is not a polymorphic MI hierarchy as
// the headers declare it; this is written as a free function with the verbatim
// `__` name (same as __dt__800BAA24) and the deleting-dtor wrapper is written
// out. Note the header's MI layout is +0x80 off retail (CfObjectMove lands at
// +0x3F1C), so the subobjects are reached through raw views pinning the
// retail offsets.
cf::CfObjectActor* __dt__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, s32 deleteFlag) {
    if (self != 0) {
        cf::CfObjectMove* move = (cf::CfObjectMove*)((u8*)self + 0x3E9C);
        __dt__Q22cf12CfObjectMoveFv(move, 0);
        cf::CfObjectCAISubView* ai = (cf::CfObjectCAISubView*)((u8*)self + 0x3380);
        if (ai != NULL) {
            cf::CfCAIActionBlockView* blk = (cf::CfCAIActionBlockView*)&ai->field_4;
            if (blk != NULL) {
                if (&blk->field_8 != NULL) {
                    blk->field_210 = 0;
                    blk->field_20C = 0;
                }
            }
        }
        if (deleteFlag > 0)
            __dl__FPv(self);
    }
    return self;
}

// Splitter-renamed deleting destructor (retail symbol is address-derived, no
// class name): zero the +0x208/+0x204 words, then free when a delete flag is
// passed, then return the object. Mirrors the matched __dt__800BAA24 pattern
// (CfObjectModel.cpp) - the `__` in the name makes MWCC keep it verbatim.
u8* __dt__800ADA08(u8* object, s32 deleteFlag) {
    if (object != 0) {
        ((cf::CfDtor800ADA08View*)object)->field_0x208 = 0;
        ((cf::CfDtor800ADA08View*)object)->field_0x204 = 0;
        if (deleteFlag > 0)
            __dl__FPv(object);
    }
    return object;
}

// us-800ae320: CfObjectEne (deleting) dtor. Free-function treatment (same as
// the base CfObjectActor dtor above: the header MI layout is +0x80 off
// retail, so the automatic base-destruction chain cannot be reproduced).
// Restores the retail subobject vtables, releases the enemy-gimmick binding
// when the gimmick singleton exists (retail calls the getter twice),
// dispatches the CfObjectMove subobject vtable slot +0x68, then explicitly
// destroys the CfObjectMove subobject and runs the inlined CAIAction
// slot-block cleanup before freeing when the delete flag > 0.
cf::CfObjectEne* __dt__Q22cf11CfObjectEneFv(cf::CfObjectEne* self, s32 deleteFlag) {
    if (self != 0) {
        u8* v = (u8*)lbl_eu_80528A18;
        ((cf::CfEneVtables*)self)->vt0 = (u32)v;
        ((cf::CfEneVtables*)self)->vt8 = (u32)(v + 0xC);
        ((cf::CfEneVtables*)self)->vt3380 = (u32)(v + 0x36C);
        ((cf::CfEneVtables*)self)->vt3E9C = (u32)(v + 0x37C);
        if (getUnk80664658() != 0) {
            func_801F4DDC(getUnk80664658(), self);
        }
        // Virtual dispatch on the +0x3E9C subobject (slot +0x68): a real
        // virtual call reproduces the retail folded vptr load + recomputed
        // adjusted-this (CfEneSubIf).
        ((cf::CfEneSubFake*)self)->vf68();
        if (self != 0) {
            __dt__Q22cf12CfObjectMoveFv((cf::CfObjectMove*)((u8*)self + 0x3E9C), 0);
            cf::CfObjectCAISubView* ai = (cf::CfObjectCAISubView*)((u8*)self + 0x3380);
            if (ai != NULL) {
                cf::CfCAIActionBlockView* blk = (cf::CfCAIActionBlockView*)&ai->field_4;
                if (blk != NULL) {
                    if (&blk->field_8 != NULL) {
                        blk->field_210 = 0;
                        blk->field_20C = 0;
                    }
                }
            }
        }
        if (deleteFlag > 0)
            __dl__FPv(self);
    }
    return self;
}

// us-800ae3f8: CfObjectEne vf2. Runs the CfObjectMove subobject's
// CfObject_UnkVirtualFunc2 (CfObjectModel slot), dispatches the subobject
// vtable slot +0x158 with flag 1, clears two battle-state flag words
// (func_800BE33C / func_800BE824 on the subobject with 1), raises three
// actor flags via func_80174B4C, seeds the +0x44A8 region via func_804B0AD4
// (two sdata2 floats) and sets its u16 at +0xB2 to 0xC8. Returns 1.
// KNOWN WALL (allocation-grind; MWCC_PATTERMS 7j negative result): retail
// recomputes addi r3,r31,0x3e9c per call keeping only r31=self, but MWCC
// GVN-merges the three identical subobject-address expressions into one
// value hoisted to a callee-saved reg (r31=sub, r30=self, frame 0x18 vs
// 0x10). Probed without effect: distinct view types per site, u32-domain
// arithmetic, typed-stride pointer arithmetic ((T*)self+1), per-site
// always_inline wrappers - all normalize to the same value number. The
// sibling dtor __dt__Q22cf11CfObjectEneFv matches only because it has a
// single explicit expression (its other use is the folded virtual-dispatch
// adjustment, which GVN does not model).
int func_800ADB2C__Q22cf11CfObjectEneFv(cf::CfObjectEne* self) {
    CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv(
        (cf::CfObjectModel*)&((cf::CfEneMoveBaseA*)self)->base);
    ((cf::CfEneSubFake*)self)->vf158(1);
    func_800BE33C(&((cf::CfEneMoveBaseB*)self)->base, 1);
    func_80174B4C(self, 0x100000);
    func_80174B4C(self, 0x08000000);
    func_80174B4C(self, 0x10000000);
    func_800BE824(&((cf::CfEneMoveBaseC*)self)->base, 1);
    u8* region = (u8*)self + 0x44A8;
    func_804B0AD4(region, 0, lbl_eu_8066696C, lbl_eu_80666970);
    ((cf::CfEneReloadArea*)region)->field_0xB2 = 0xC8;
    return 1;
}

// us-800ae4a0: CfObjectEne::func_800ADBD4 (retail Fv; this in r3). Probes
// the +0x4 sub-object's actor id (vtable +0x30 first word) against
// func_80174C98 masks 0x803/0xE; on the clean path runs the +0x8 CBattleState
// subobject slot +0x14 when func_80148778(&+0x8, 0x35) is clear, else runs
// func_800AF870 and slot +0x20. Then accumulates `scene time * sub8C` into
// func_801765A4, and when the game-manager bit 0x100 is set applies the
// +0x3F34 target's bit-0x80000000 flag + subobject slots +0x80/+0x64, else
// runs the CfObjectMove subobject's CfObject_UnkVirtualFunc4.
void cf::CfObjectEne::func_800ADBD4() {
    u32 wordA = *((cf::CfActorUnk4Vt30*)((cf::CfActorField04*)this)->field_0x04)->vf30();
    u32 wordB;
    if (func_80174C98((void*)this, (int*)&wordA, 0x803) == 0 &&
        ((wordB = *((cf::CfActorUnk4Vt30*)((cf::CfActorField04*)this)->field_0x04)->vf30()),
         func_80174C98((void*)this, (int*)&wordB, 0xE)) == 0) {
        if (func_80148778(&((cf::CfEneB8View*)this)->field_0x8, 0x35) == 0) {
            ((cf::CfEneB8V14*)this)->m14(0x35);
        }
    } else {
        func_800AF870(this);
        if (func_80148778(&((cf::CfEneB8View*)this)->field_0x8, 0x35) != 0) {
            ((cf::CfEneB8V20*)this)->m20(0x35);
        }
    }
    // local pins the scene-time source across the m8C() call into a
    // callee-saved reg (retail r31); the subobject address is re-derived per
    // site through distinct expression shapes (member-view / u32-domain /
    // byte-add) so GVN cannot merge them into one cached register
    CScn* sceneTimeSrc = lbl_eu_80663E14;
    f32 sub = ((cf::CfEneSubVt64*)this)->m8C();
    // retail multiplies as time * sub (fmuls f1,f1,f31 with f31=sub)
    f32 t = func_80496288(sceneTimeSrc);
    f32 f = t * sub;
    func_801765A4((u8*)this, f, 1);
    getInstance__Q22cf13CfGameManagerFv();
    // retail flattens the nested guards into two branches on one shared
    // else label (the CfObject_UnkVirtualFunc4 call); goto pins that shape
    if (func_8006EF04__Fi(0x1000000) == 0)
        goto defaultPath;
    {
        u32 wordC = *((cf::CfActorUnk4Vt30*)((cf::CfActorField04*)this)->field_0x04)->vf30();
        if (func_80174C98(this, &wordC, 0x1) != 0)
            goto defaultPath;
        u8* p = ((cf::CfEneField3F34*)this)->field_0x3F34;
        if (p != 0)
            ((cf::CfEneField7A4*)p)->field_0x7A4 |= 0x80000000;
        // three mutually-distinct expression families (member view /
        // u32-cast address / u32 arithmetic) keep MWCC from CSE-ing the
        // subobject base into a saved register - retail recomputes
        // addi r3, rX, 0x3e9c per dispatch (cf. pluginBtl.cpp)
        ((cf::CfEneSubVt64If*)((u8*)this + 0x3E9C))->m80();
        ((cf::CfEneSubVt64If*)(u32)&((cf::CfEneVtables*)this)->vt3E9C)->m64();
    }
    goto done;
defaultPath:
    // retail: direct bl to the concrete implementation (not a vdispatch)
    CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(
        (cf::CfObjectMove*)&((cf::CfEneMoveBaseD*)this)->base);
done:;
}

// us-800ae674: CfObjectEne::func_800ADDA8 (retail Fv; this in r3). Full
// enemy-parameter setup from two bdat tables. The row id (+0x3F28) is
// RE-LOADED before every column read (the bdat calls may modify it - retail
// emits one lhz per call), so it is never cached in a local.
void cf::CfObjectEne::func_800ADDA8() {
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
    ((cf::CfEneSubVt3C*)this)->vf3C(getBdatStringColumnValue(bdatArts, &lbl_eu_804FC168[0x0], v->field_0x3F28));
    c5.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x5], v->field_0x3F28);
    ((cf::CfEneVtSetup1*)this)->vfE4(c5.b);
    cA.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xA], v->field_0x3F28);
    ((cf::CfEneVtSetup1*)this)->vfE8((f32)(u32)cA.b / lbl_eu_8066697C);
    c10.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x10], v->field_0x3F28);
    ((cf::CfEneVtSetup1*)this)->vfDC(c10.b);
    c17.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x17], v->field_0x3F28);
    p->field_0x15F0 = (u32)c17.b;
    u32 flags = p->field_0x3374 | (u32)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x1D], v->field_0x3F28);
    p->field_0x3374 = flags;
    // rlwinm bit 20 -> mask 0x00100000
    if (!(flags & 0x00100000)) {
        // k22/k2d live across later calls: retail keeps them in callee-saved regs
        u8 k22 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x22], v->field_0x3F28);
        ((cf::CfEneSubVt3C*)this)->vf1D4((f32)(s32)(u32)k22);
        u8 k2d = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x2D], v->field_0x3F28);
        ((cf::CfEneSubVt3C*)this)->vf134((f32)(s32)(u32)k2d);
        ((cf::CfEneSubVt3C*)this)->vf13C(lbl_eu_80666980);
        if (k22 == 0 || k2d == 0)
            p->field_0x3374 |= 0x1000;
    }
    c37.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x37], v->field_0x3F28);
    ((cf::CfEneVtSetup3*)this)->vf254(c37.b);
    c3F.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x3F], v->field_0x3F28);
    ((cf::CfEneVtSetup3*)this)->vf25C(c3F.b != 0);
    c46.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x46], v->field_0x3F28);
    ((cf::CfEneVtSetup3*)this)->vf264((f32)(u32)c46.h);
    c53.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x53], v->field_0x3F28);
    ((cf::CfEneVtSetup3*)this)->vf26C((f32)(u32)c53.h * lbl_eu_8066A210);
    ((cf::CfEneVtSetup2*)this)->vf170(1);
    ((cf::CfEneVtSetup2*)this)->vf188(1);
    ((cf::CfEneVtSetup2*)this)->vf198(lbl_eu_80666984);
    ((cf::CfEneVtSetup2*)this)->vf1A4(lbl_eu_80666984);
    ((cf::CfEneVtSetup2*)this)->vf1BC(lbl_eu_80666988 * (lbl_eu_8066A1F8 / lbl_eu_8066698C));
    c60.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x60], v->field_0x3F28);
    p->field_0x1648 |= c60.b;
    c6A.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x6A], v->field_0x3F28);
    p->field_0x164A |= c6A.b;
    c80.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x80], v->field_0x3F28);
    p->field_0x164C |= c80.h;
    c8B.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x8B], v->field_0x3F28);
    ((cf::CfBattleVt7C*)((u8*)this + 8))->vf7C(c8B.h);
    c96a.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x96], v->field_0x3F28);
    ((cf::CfBattleVt84*)((u8*)this + 8))->vf84(c96a.h);
    c96t.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x96], v->field_0x3F28);
    if (c96t.b != 0) {
        c96b.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x96], v->field_0x3F28);
        ((cf::CfEneVtTail*)this)->vf334((lbl_eu_8066A1F8 / lbl_eu_80666990) * (f32)(u32)c96b.b);
    }
    cf::CfEneRatesView* g = ((cf::CfEneVtActs*)this)->vf20C();
    cA2.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xA2], v->field_0x3F28);
    g->field_0x0 = (u32)cA2.b;
    cA5.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xA5], v->field_0x3F28);
    f32 w = (f32)(s32)((u32)cA5.h * 100);
    g->field_0x10 = w;
    g->field_0x4 = w;
    g->field_0xC = lbl_eu_80666968;
    g->field_0x18 = lbl_eu_80666994;
    cAC.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xAC], v->field_0x3F28);
    g->field_0x1C = (s16)cAC.h;
    cB0.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xB0], v->field_0x3F28);
    g->field_0x1E = (s16)cB0.h;
    cB4.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xB4], v->field_0x3F28);
    g->field_0x20 = (s16)cB4.h;
    cBA.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xBA], v->field_0x3F28);
    g->field_0x40 = (f32)(u32)cBA.b / lbl_eu_8066697C;
    cC9.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xC9], v->field_0x3F28);
    g->field_0x44 = (f32)(u32)cC9.h / lbl_eu_80666994;
    cD7.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xD7], v->field_0x3F28);
    g->field_0x48 = (f32)(u32)cD7.h / lbl_eu_80666994;
    cDF.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xDF], v->field_0x3F28);
    g->field_0x56 = cDF.b;
    cE7.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xE7], v->field_0x3F28);
    g->field_0x57 = cE7.b;
    cF0.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xF0], v->field_0x3F28);
    g->field_0x60 = (u16)cF0.b;
    g->field_0x62 = (u16)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xF9], v->field_0x3F28);
    // three redundant column reads follow; the first feeds field_0x73 from
    // the raw return (clrlwi 28), the next two are dead with constant stores
    g->field_0x73 = (u8)(getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xF9], v->field_0x3F28) & 0xF);
    getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xF9], v->field_0x3F28);
    g->field_0x70 = 0;
    getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xF9], v->field_0x3F28);
    g->field_0x72 = 0;
    p->field_0x1629 = 0;
    p->field_0x162A = 0;
    p->field_0x162B = 3;
    p->field_0x162C = 7;
    u16 w456c = ((cf::CfObjectEne456CView*)this)->field_0x456C;
    s32 half = (s32)w456c >> 4;
    if (half != 0) {
        // level-name letter written into both name strings at +3
        u8 letter = (u8)((w456c & 0xF) + 0x30);
        lbl_eu_80661CB8[3] = letter;
        lbl_eu_80661CBC[3] = letter;
        u8* bdat9c = lbl_eu_8066409C;
        s8 sn = (s8)getBdatStringColumnValue(bdat9c, (const char*)lbl_eu_80661CBC, (u16)half);
        u0A.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xA], v->field_0x3F28);
        s8 s2 = (s8)getBdatStringColumnValue(bdat9c, (const char*)lbl_eu_80661CBC, (u16)half);
        u102.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x102], v->field_0x3F28);
        u10B.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x10B], v->field_0x3F28);
        u115.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x115], v->field_0x3F28);
        u11F.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x11F], v->field_0x3F28);
        u129.w = getBdatStringColumnValue(bdat9c, &lbl_eu_804FC168[0x129], u11F.h);
        s32 sum = (s32)u0A.b + (s32)s2;
        // rlwinm bit 26 -> mask 0x20 picks between raw and scaled HP scaling
        if (u129.b & 0x20) {
            ((cf::CfEneVtSetup1*)this)->vfE8((f32)(s32)sum);
        } else {
            ((cf::CfEneVtSetup1*)this)->vfE8(lbl_eu_80666998 * (f32)(s32)sum);
        }
        g->field_0x0 = g->field_0x0 + (u32)sn;
        g->field_0x4 = (f32)sn * (f32)u102.h + g->field_0x4;
        g->field_0x1C = (s16)((f32)sn * (f32)u10B.h + (f32)g->field_0x1C);
        g->field_0x1E = (s16)((f32)sn * (f32)u115.h + (f32)g->field_0x1E);
        g->field_0x20 = (s16)((f32)sn * (f32)u11F.h + (f32)g->field_0x20);
        if (g->field_0x0 < 1) g->field_0x0 = 1;
        if (g->field_0x4 < lbl_eu_80666980) g->field_0x4 = lbl_eu_80666980;
        if (g->field_0x1C < 1) g->field_0x1C = 1;
        if (g->field_0x1E < 1) g->field_0x1E = 1;
        if (g->field_0x20 < 1) g->field_0x20 = 1;
        g->field_0x10 = g->field_0x4;
    } else {
        elseA.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xA], v->field_0x3F28);
        ((cf::CfEneVtSetup1*)this)->vfE8(lbl_eu_80666998 * (f32)(s32)(u32)elseA.b);
        p->field_0x3F00 |= 0x04000000;
    }
    u32 cap = g->field_0x0;
    if (cap > 0x63) cap = 0x63;
    u8* bdatDC = (u8*)lbl_eu_806640DC;
    w132.w = getBdatStringColumnValue(bdatDC, &lbl_eu_804FC168[0x132], cap);
    w139.w = getBdatStringColumnValue(bdatDC, &lbl_eu_804FC168[0x139], cap);
    base13F.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x13F], v->field_0x3F28);
    s32 base = (s32)base13F.h + 10;
    // mulhwu 0xcccd/srwi 3 division: unsigned 32-bit domain throughout
    p->field_0x1604 = (u32)((u32)w132.h * (u32)base) / 10;
    p->field_0x1608 = (u32)((u32)w139.h * (u32)base) / 10;
    n1634.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x143], v->field_0x3F28);
    p->field_0x1634 = (u32)n1634.b;
    if (n1634.b != 0) {
        // kind lives across later calls: retail keeps it in a callee-saved reg
        u8 kind = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x14E], v->field_0x3F28);
        c159.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x159], v->field_0x3F28);
        p->field_0x1638 = (u32)c159.b;
        c165.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x165], v->field_0x3F28);
        p->field_0x163C = (u32)(c165.b * 10);
        c16F.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x16F], v->field_0x3F28);
        p->field_0x1640 = (u32)c16F.b;
        c17B.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x17B], v->field_0x3F28);
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
            bufa.field_0x20 = lbl_eu_80666968;
            bufa.field_0x30 = 1;
            ((cf::CfBattleVt18*)((u8*)this + 8))->m18(&bufa);
        }
    }
    c18B.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x18B], v->field_0x3F28);
    if (c18B.b != 0) {
        cf::CfEneSetupBuf bufb;
        std::memset(&bufb, 0, 0x34);
        bufb.field_0x0 = p->field_0x3F10;
        bufb.field_0x4 = 0;
        bufb.field_0xC = 0xC3;
        bufb.field_0x10 = (u32)c18B.b;
        bufb.field_0x20 = lbl_eu_80666968;
        ((cf::CfBattleVt18*)((u8*)this + 8))->m18(&bufb);
    }
    c191.w = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x191], v->field_0x3F28);
    p->field_0x3370 = (u32)c191.h;
    ((cf::CfEneVtSetup3*)this)->vf21C(((cf::CfEneVtActs*)this)->vf20C());
    ((cf::CfEneVtTail*)this)->vf330();
    ((cf::CfEneVtTail*)this)->vf32C();
    ((cf::CfEneVt5E0*)this)->vf5E0();
    // rlwinm bit 12 -> mask 0x1000
    if (p->field_0x3374 & 0x1000) {
        p->field_0x3F04 |= 0x20000000;
    }
    // retail calls the getter twice (test + argument)
    if (getUnk80664658() != 0) {
        func_801F4D50(getUnk80664658(), this);
    }
}

void CActorParam_UnkVirtualFunc20__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x15E4) = val; }

void CActorParam_UnkVirtualFunc18__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x15EC) = val; }

void CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv(void* self, float val) { *(float*)((u8*)self + 0x6ec) = val; }

void CActorParam_UnkVirtualFunc112__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x161C) = val; }

void CActorParam_UnkVirtualFunc114__Q22cf11CActorParamFv(cf::CActorParam* self, u8 val) {
    // retail: stb r4, 0x1628(r3) - member block is +8 shifted, raw view pins the offset
    ((cf::CfObjectEne1628View*)self)->field_0x1628 = val;
}

void CActorParam_UnkVirtualFunc118__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1624) = val; }

void CActorParam_UnkVirtualFunc168__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1630) = val; }

// us-800af534: CfObjectEne::func_800AEC68 (retail Fv; this in r3). Resets
// the bdat manager, reads the enemy bdat column value (getFP on the
// +0x3F14 name, row id at +0x3F28), multiplies it by the vtable +0xF0
// result and the 0x806669B0 scale, feeds the vtable +0xD4 slot, runs the
// primary vtable +0x288 slot six times and resets the +0x3380 CAIAction
// subobject (func_8014B7B0 / func_8015396C).
void cf::CfObjectEne::func_800AEC68() {
    func_8003AA34();
    // addressable union pins the raw column word to a frame home across the
    // slot +0xF0 vcall; the u16 punned read narrows on reload (retail lhz)
    cf::CfEneColNarrow col;
    col.w = getBdatStringColumnValue(getFP(((cf::CfEneLookupView*)this)->field_0x3F14),
                                     &lbl_eu_804FC168[0x19A],
                                     ((cf::CfEneLookupView*)this)->field_0x3F28);
    f32 base = ((cf::CfEneVtD4*)this)->mF0();
    f32 scaled = lbl_eu_806669B0 * (f32)col.h;
    ((cf::CfEneVtD4*)this)->mD4(scaled * base);
    for (int i = 0; i < 6; i++)
        ((cf::CfEneVtD4*)this)->m288();
    // Different syntax families (pointer-add vs &subscript) stop MWCC from
    // CSE-ing the subobject address into a callee-saved register - retail
    // re-materializes addi r3, rX, 0x3380 at each call (cf. pluginBtl.cpp).
    func_8014B7B0((u8*)this + 0x3380);
    func_8015396C(&((u8*)this)[0x3380], 0, 0);
}

// us-800afd5c: CfObjectEne::CActorParam_UnkVirtualFunc167 (retail Fv; this
// in r3). Attack-set loader: reloads the bdat manager, reads the enemy bdat
// table (getFP on +0x3F14 / row +0x3F28), writes the six attack-slot ids
// (columns 0x2A7/0x2AC/0x2B1, the last three zeroed) through the primary
// vtable slot +0x288 u16 array, then for each non-zero slot loads the
// CAttackParam record (getAtkParam), copies the name, fills the typed
// fields from the bdat columns, picks the +0x60 gauge from the +0x20C rate
// (+0x48, or +0x44 when the +0x48 is 0.0f) and scales the +0x64 attack
// count by the +0x1C4 rate * lbl_eu_8066A20C.
void cf::CfObjectEne::CActorParam_UnkVirtualFunc167() {
    func_8003AA34();
    void* bdat = getFP(((cf::CfEneLookupView*)this)->field_0x3F14);
    char* cols = lbl_eu_804FC168;
    // each column result is a u32 frame temp live across the slot +0x288
    // virtual call that publishes it (retail spills to 0x2c/0x28/0x24(sp))
    u32 s0 = getBdatStringColumnValue(bdat, &cols[0x2A7], ((cf::CfEneLookupView*)this)->field_0x3F28);
    ((cf::CfEneVtActs*)this)->vf288()[0] = (u16)s0;
    u32 s1 = getBdatStringColumnValue(bdat, &cols[0x2AC], ((cf::CfEneLookupView*)this)->field_0x3F28);
    ((cf::CfEneVtActs*)this)->vf288()[1] = (u16)s1;
    u32 s2 = getBdatStringColumnValue(bdat, &cols[0x2B1], ((cf::CfEneLookupView*)this)->field_0x3F28);
    ((cf::CfEneVtActs*)this)->vf288()[2] = (u16)s2;
    ((cf::CfEneVtActs*)this)->vf288()[3] = 0;
    ((cf::CfEneVtActs*)this)->vf288()[4] = 0;
    ((cf::CfEneVtActs*)this)->vf288()[5] = 0;

    func_8003AA34();
    u8* bdat2 = (u8*)getFP(&cols[0x2B6]);
    f32 zero = lbl_eu_80666968;
    f32 scale = lbl_eu_8066A20C;
    for (int i = 0; i < 6; i++) {
        // name column addresses the raw symbol; the rest go through the
        // offset copy (retail keeps both r29/r30 base forms)
        u16 atkId = ((cf::CfEneVtActs*)this)->vf288()[(s16)i];
        if (atkId == 0) continue;
        cf::CfEneAtkParamView* atk = (cf::CfEneAtkParamView*)getAtkParam(((cf::CfEneVtActs*)this)->vf288(), i);
        char* nm = (char*)getBdatStringColumnValue(bdat2, lbl_eu_804FC168, atkId);
        atk->field_0x20 = strlen(nm);
        strcpy(atk->name, nm);
        u32 v1D = getBdatStringColumnValue(bdat2, &cols[0x1D], atkId);
        atk->field_0x78 = (u16)v1D;
        atk->field_0x2C = zero;
        atk->field_0x30 = zero;
        u32 v1EF = getBdatStringColumnValue(bdat2, &cols[0x1EF], atkId);
        atk->field_0x36 = (s16)(s8)v1EF;
        u32 v2BE = getBdatStringColumnValue(bdat2, &cols[0x2BE], atkId);
        atk->field_0x38 = (s16)(u8)v2BE;
        atk->field_0x3A = 0;
        atk->field_0x42 = 0;
        u32 v209 = getBdatStringColumnValue(bdat2, &cols[0x209], atkId);
        atk->field_0x3C = (s16)(u8)v209;
        u32 v212 = getBdatStringColumnValue(bdat2, &cols[0x212], atkId);
        atk->field_0x40 = (s16)(u8)v212;
        atk->field_0x43 = 1;
        atk->field_0x44 = 1;
        atk->field_0x46 = 0;
        atk->field_0x58 = 0;
        atk->field_0x5A = 0;
        atk->field_0x5C = 1;
        atk->field_0x5E = 0;
        f32 g48 = ((cf::CfEneVtActs*)this)->vf20C()->field_0x48;
        f32 gauge;
        if (g48 == zero) {
            gauge = ((cf::CfEneVtActs*)this)->vf20C()->field_0x44;
        } else {
            gauge = ((cf::CfEneVtActs*)this)->vf20C()->field_0x48;
        }
        atk->field_0x60 = gauge;
        f32 rate = ((cf::CfEneVtActs*)this)->vf1C4()->field_0x0;
        atk->field_0x64 = (s16)(rate * scale);
        u32 v256 = getBdatStringColumnValue(bdat2, &cols[0x256], atkId);
        atk->field_0x76 = (u8)v256;
        u32 v25E = getBdatStringColumnValue(bdat2, &cols[0x25E], atkId);
        atk->field_0x77 = (u8)v25E;
    }
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
    // Retail pins two 0x4330xxxx conversion words (sp+0xA8/sp+0xB0) for the
    // whole function; only the low word is refreshed per conversion.
    cf::CfEneF64Conv cvA;
    cf::CfEneF64Conv cvB;
    cvA.w[0] = 0x43300000;
    cvB.w[0] = 0x43300000;
    cf::CfEneLookupView* v = (cf::CfEneLookupView*)this;
    func_8003AA34();
    void* bdat = getFP(v->field_0x3F14);
    const char* tbl = (const char*)lbl_eu_804FC168;
    u16 row = v->field_0x3F28;
    u32 a0 = getBdatStringColumnValue(bdat, tbl + 0x1A7, row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), a0, 0);
    u32 a1 = getBdatStringColumnValue(bdat, tbl + 0x1AD, row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), a1, 1);
    u32 a2 = getBdatStringColumnValue(bdat, tbl + 0x1B3, row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), a2, 2);
    u32 a3 = getBdatStringColumnValue(bdat, tbl + 0x1B9, row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), a3, 3);
    u32 a4 = getBdatStringColumnValue(bdat, tbl + 0x1BF, row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), a4, 4);
    u32 a5 = getBdatStringColumnValue(bdat, tbl + 0x1C5, row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), a5, 5);
    u32 a6 = getBdatStringColumnValue(bdat, tbl + 0x1CB, row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), a6, 6);
    u32 a7 = getBdatStringColumnValue(bdat, tbl + 0x1D1, row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), a7, 7);

    func_8003AA34();
    void* bdat2 = getFP(&tbl[0x1D7]);
    // Hoisted constants the loop keeps live across calls (retail f29/f30/f31/f28)
    f32 subA = lbl_eu_806669A0;
    f32 divB = lbl_eu_8066697C;
    f32 cstFF = lbl_eu_806669B4;
    f32 divC = lbl_eu_80666994;
    int i;
    s16 zero46 = 0;  // retail keeps a dedicated zero reg for field_0x46
    for (i = 0; i < 8; i++) {
        u16 slot = func_80153CAC(((cf::CfEneVtActs*)this)->vf27C(), (s16)i);
        if (slot == 0) continue;
        cf::CfEneArtsParamView* arts = (cf::CfEneArtsParamView*)getArtsParamByIdx(((cf::CfEneVtActs*)this)->vf27C(), i);
        // name column referenced through the raw label (not tbl), as in retail
        char* nm = (char*)getBdatStringColumnValue(bdat2, (char*)lbl_eu_804FC168, slot);
        arts->field_0x20 = strlen(nm);
        strcpy(arts->name, nm);
        arts->field_0x78 = getBdatStringColumnValue(bdat2, tbl + 0x1D, slot);
        cvA.w[1] = (u32)(u8)getBdatStringColumnValue(bdat2, tbl + 0x1E0, slot);
        arts->field_0x2C = ((f32)(cvA.d - subA) / divB);
        u8 e5 = (u8)getBdatStringColumnValue(bdat2, tbl + 0x1E5, slot);
        if (e5 == 0xFF) {
            arts->field_0x30 = cstFF;
        } else {
            e5 = (u8)getBdatStringColumnValue(bdat2, tbl + 0x1E5, slot);
            cvB.w[1] = (u32)e5;
            arts->field_0x30 = (f32)(cvB.d - subA);
        }
        arts->field_0x34 = (s16)(s8)getBdatStringColumnValue(bdat2, tbl + 0x1EC, slot);
        arts->field_0x36 = (s16)(s8)getBdatStringColumnValue(bdat2, tbl + 0x1EF, slot);
        arts->field_0x38 = (s16)getBdatStringColumnValue(bdat2, tbl + 0x1F3, slot);
        arts->field_0x3A = (s16)getBdatStringColumnValue(bdat2, tbl + 0x1F9, slot);
        arts->field_0x42 = (u8)getBdatStringColumnValue(bdat2, tbl + 0x1FF, slot);
        u8 a2b = (u8)getBdatStringColumnValue(bdat2, tbl + 0xA2, slot);
        if (a2b != 0)
            arts->field_0x2A = (u8)getBdatStringColumnValue(bdat2, tbl + 0xA2, slot);
        arts->field_0x3C = (u16)(u8)getBdatStringColumnValue(bdat2, tbl + 0x209, slot);
        arts->field_0x40 = (u16)(u8)getBdatStringColumnValue(bdat2, tbl + 0x212, slot);
        arts->field_0x43 = (u8)getBdatStringColumnValue(bdat2, tbl + 0x217, slot);
        arts->field_0x44 = (u8)getBdatStringColumnValue(bdat2, tbl + 0x220, slot);
        arts->field_0x46 = zero46;
        // simple attack classes (field_0x3C in {0,1}) without the 0x44 flag are
        // initialized through the record vtable and skipped
        if ((u16)(arts->field_0x3C - 1) <= 1 && arts->field_0x44 == 0) {
            ((cf::CfEneAtkVtblRec*)arts)->vtInit();
            continue;
        }
        arts->field_0x58 = (u16)(u8)getBdatStringColumnValue(bdat2, tbl + 0x229, slot);
        arts->field_0x5A = (u16)(u8)getBdatStringColumnValue(bdat2, tbl + 0x231, slot);
        arts->field_0x5C = (u16)(u8)getBdatStringColumnValue(bdat2, tbl + 0x237, slot);
        arts->field_0x5E = (u16)(u8)getBdatStringColumnValue(bdat2, tbl + 0x23B, slot);
        u16 w246 = (u16)getBdatStringColumnValue(bdat2, tbl + 0x246, slot);
        f32 r44 = ((cf::CfEneVtActs*)this)->vf20C()->field_0x44;
        cvA.w[1] = (u32)w246;
        arts->field_0x60 = (f32)(cvA.d - subA) / divC + r44;
        arts->field_0x64 = (u16)getBdatStringColumnValue(bdat2, tbl + 0x24C, slot);
        arts->field_0x76 = (u8)getBdatStringColumnValue(bdat2, tbl + 0x25E, slot);
        arts->field_0x77 = (u8)getBdatStringColumnValue(bdat2, tbl + 0x262, slot);
        arts->field_0x48 = (u16)getBdatStringColumnValue(bdat2, tbl + 0x26A, slot);
        arts->field_0x4A = (u16)(u8)getBdatStringColumnValue(bdat2, tbl + 0x271, slot);
        arts->field_0x4C = (u16)(u8)getBdatStringColumnValue(bdat2, tbl + 0x279, slot);
        cvB.w[1] = (u32)(u16)getBdatStringColumnValue(bdat2, tbl + 0x281, slot);
        arts->field_0x50 = (f32)(cvB.d - subA) / divB;
        cvA.w[1] = (u32)(u8)getBdatStringColumnValue(bdat2, tbl + 0x288, slot);
        arts->field_0x54 = (f32)(cvA.d - subA) / divB;
        arts->field_0x66 = (u8)getBdatStringColumnValue(bdat2, tbl + 0x28F, slot);
        arts->field_0x67 = (u8)getBdatStringColumnValue(bdat2, tbl + 0x297, slot);
        arts->field_0x68 = (s16)getBdatStringColumnValue(bdat2, tbl + 0x29F, slot);
        arts->field_0x6A = (s16)getBdatStringColumnValue(bdat2, tbl + 0x2A7, slot);
        arts->field_0x78 |= 0x40000000;
        if (arts->field_0x42 == 1)
            arts->field_0x78 |= 0x80000000;
        if (arts->field_0x78 & 0x8000)
            arts->field_0x2C *= divC;
    }
}
// us-800b00b0: bdat lookup helper. Reads the u16 row id at self+0x456C
// (shifted right 4) and looks up the column value in the BDAT table returned
// by func_80086B24. Returns whatever getBdatStringColumnValue produced (the
// retail leaves it in r3).
u32 func_800AF7E4(cf::CfObjectEne* self, const char* column) {
    return getBdatStringColumnValue((void*)func_80086B24__Q22cf13CfGameManagerFv(), column,
                                    ((cf::CfObjectEne456CView*)self)->field_0x456C >> 4);
}

// us-800b00f8: bdat lookup helper. r3 is a dead incoming arg (retail never
// reads it); the bdat pointer is func_80086B34's return (party-count
// getter), column/index come from r4/r5.
u32 func_800AF82C(u32 unused, const char* col, u32 index) {
    return getBdatStringColumnValue((u8*)func_80086B34__Q22cf13CfGameManagerFv(), col, index);
}

// us-800b013c: enemy drop/reward application. Guard chain (flag bit
// 0x100000, shared lbl_eu_80663E24 bits 0xAFA40000, +0x3F60 target bit 8,
// primary vtable +0x2BC, CBattleManager party-count range and mode words)
// then scans the enemy bdat rows [func_8003B41C, +func_8003B1EC) for the
// model id and channel, accumulates `scene time * sub8C + field_0x45CC`
// against the reward threshold (`reward * 30`) and applies the scaled count
// through the primary vtable +0x11C slot before clearing field_0x45CC and
// returning on the first hit.
void func_800AF870(cf::CfObjectEne* self) {
    if ((((cf::CfActorParamFields*)self)->field_0x3374 & 0x100000) != 0) return;
    if ((lbl_eu_80663E24 & 0xAFA40000) != 0) return;
    if (((cf::CfActorField3F60*)self)->field_0x3F60 == NULL) return;
    // retail reloads the +0x3F60 target for the flag test (short-lived r4)
    if (((((cf::CfEneMoveTgtView*)((cf::CfActorField3F60*)self)->field_0x3F60)->field_0x4EC) & 0x100) == 0) return;
    if (((cf::CfEneVt2BC*)self)->m2BC() != 0) return;
    cf::CfEneBmView* bm = (cf::CfEneBmView*)getInstance__Q22cf14CBattleManagerFv();
    // goto form blocks MWCC's unsigned range-check fusion
    // (cf. CfObjectPc.cpp func_801575B0)
    int ok = 0;
    if (bm->field_0x1AA < 1) goto bmCheck;
    if (0x18 < bm->field_0x1AA) goto bmCheck;
    ok = 1;
bmCheck:
    if (ok != 0) return;
    if (func_801BA2C8(&((cf::CfEneBmView*)getInstance__Q22cf14CBattleManagerFv())->field_0x216C) != 0) return;
    cf::CfEneBmView* bm3 = (cf::CfEneBmView*)getInstance__Q22cf14CBattleManagerFv();
    s16 idx = bm3->field_0x20C8;
    if (idx != 0) return;
    // mode word is read SDA-indexed by the (always-zero) idx register
    u32 modeOff = (u32)*(u16*)((u8*)&lbl_eu_80663E42 + idx) * 100;
    int channel = (int)*(u16*)((u8*)&lbl_eu_80663E44 + modeOff) + (int)modeOff;
    s32 key = ((cf::CfEneMoveTgtView*)((cf::CfActorField3F60*)self)->field_0x3F60)->field_0x4B0;
    func_8003AA34();
    u8* bdat = (u8*)lbl_eu_806640D4;
    int row = (int)func_8003B41C(bdat);
    int endRow = row + (int)func_8003B1EC(bdat);
    const char* tbl = (const char*)lbl_eu_804FC168;
    for (; row < endRow; row++) {
        u8 model = (u8)getBdatStringColumnValue(bdat, tbl + 0x2C3, row);
        if ((s32)model != key) continue;
        u16 ch = (u16)getBdatStringColumnValue(bdat, tbl + 0x2CA, row);
        if ((s32)ch != channel) continue;
        u8 reward = (u8)getBdatStringColumnValue(bdat, tbl + 0x2CE, row);
        // pin the scene-time source across the m8C() call so MWCC hoists
        // the SDA load above the bctrl (retail keeps it in a saved reg)
        CScn* sceneSrc = lbl_eu_80663E14;
        f32 sub = ((cf::CfEneSubVt64*)self)->m8C();
        f32 t = func_80496288(sceneSrc);
        f32 now = t * sub + ((cf::CfEneTailView*)self)->field_0x45CC;
        // retail stores the accumulated time back BEFORE the threshold compare
        ((cf::CfEneTailView*)self)->field_0x45CC = now;
        cf::CfEneF64Conv cvR;
        cvR.w[0] = 0x43300000;
        cvR.w[1] = (u32)((s32)reward * 30) ^ 0x80000000;
        f32 thresh = (f32)(cvR.d - lbl_eu_806669A8);
        if (now < thresh) return;
        u8 count = (u8)getBdatStringColumnValue(bdat, tbl + 0x2D7, row);
        cf::CfEneF64Conv cvN;
        cvN.w[0] = 0x43300000;
        cvN.w[1] = (u32)count;
        f32 n = (f32)(cvN.d - lbl_eu_806669A0);
        f32 prod = lbl_eu_806669B0 * (n * ((cf::CfActorParamVt118*)self)->m12C());
        if (prod != lbl_eu_80666968) {
            s32 iv = (s32)prod;
            cf::CfEneF64Conv cvV;
            cvV.w[0] = 0x43300000;
            cvV.w[1] = (u32)iv ^ 0x80000000;
            f32 rv = (f32)(cvV.d - lbl_eu_806669A8);
            if (rv == lbl_eu_80666968) rv = lbl_eu_80666980;
            ((cf::CfActorParamVt118*)self)->m11C(-rv);
        }
        ((cf::CfEneTailView*)self)->field_0x45CC = lbl_eu_80666968;
        return;
    }
}

int CfObjectActor_UnkVirtualFunc2__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { return 1; }

void CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv(void) {}

void CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void) {}

struct If38 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void vf38();
};
void CActorState_UnkVirtualFunc1__Q22cf11CActorStateFv(cf::CActorState* self) {
    reinterpret_cast<If38*>(*(void**)((u8*)self + 4))->vf38();
}

u8* CBattleState_UnkVirtualFunc28__Q22cf12CBattleStateFv(cf::CBattleState* self, unsigned long index) {
    return ((cf::CBattleState*)self)->unk152C + (index << 4);
}

u8* CBattleState_UnkVirtualFunc27__Q22cf12CBattleStateFv(cf::CBattleState* self) { return (u8*)self + 0x152c; }

u8* CBattleState_UnkVirtualFunc25__Q22cf12CBattleStateFv(cf::CBattleState* self) { return (u8*)self + 0x8; }

// us-800b0404: retail copies 0x1520 bytes (0x2a4 * 8) from src to this->unk8
// (offset 0x8) with a counted lwzu/stwu loop - full struct assignment of the
// 0x1520-byte block at CBattleState+0x8.
void CBattleState_UnkVirtualFunc24__Q22cf12CBattleStateFv(cf::CBattleState* self, const cf::CBattleState* src) {
    *(cf::CBattleStateUnk1520Block*)self->unk8 = *(const cf::CBattleStateUnk1520Block*)src;
}





void CActorParam_UnkVirtualFunc181__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x2A80) = val; }

int CActorParam_UnkVirtualFunc178__Q22cf11CActorParamFv(cf::CActorParam* self) { return 0; }

void CActorParam_UnkVirtualFunc173__Q22cf11CActorParamFv(void) {}

u32 CActorParam_UnkVirtualFunc172__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x3370); }

float CActorParam_UnkVirtualFunc171__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1630); }

float CActorParam_UnkVirtualFunc170__Q22cf11CActorParamFv(cf::CActorParam* self) {
    // lbl_eu_8066A1F8 declared (const form) in CfObjectEne.hpp / CChain.hpp
    return lbl_eu_8066A1F8 - *(float*)((char*)self + 0x1630);
}

float CActorParam_UnkVirtualFunc169__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1630); }

void* CActorParam_UnkVirtualFunc164__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x164a); }

void* CActorParam_UnkVirtualFunc163__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1648); }

u32 CActorParam_UnkVirtualFunc162__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x336C); }

void CActorParam_UnkVirtualFunc161__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x336C) = val; }

void* CActorParam_UnkVirtualFunc151__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x3358); }

// us-800b04d0: zeroes the 8-entry CActorParam_UnkStruct5 array at 0x1928.
// Retail is fully unrolled (48 straight stores: the u32 slot then the five
// floats, in the init() order unk14/unk4/unk0/unkC/unk8/unk10 per entry), so
// the 8 explicit init() calls are kept instead of a counted loop.
void cf::CActorParam::CActorParam_UnkVirtualFunc148() {
    cf::CActorParamUnk1928View* v = (cf::CActorParamUnk1928View*)this;
    v->entries[0].init();
    v->entries[1].init();
    v->entries[2].init();
    v->entries[3].init();
    v->entries[4].init();
    v->entries[5].init();
    v->entries[6].init();
    v->entries[7].init();
}

// us-800b059c: retail copies 0xC0 bytes (0x18 * 8) from src to the 8-entry
// CActorParam UnkStruct5 array at offset 0x1928 (stride 0x18) with a
// counted lwzu/stwu loop.
void CActorParam_UnkVirtualFunc146__Q22cf11CActorParamFv(cf::CActorParam* self, const cf::CActorParam* src) {
    *(cf::CActorParamUnk1928Block*)((cf::CActorParamUnk1928View*)self)->entries = *(const cf::CActorParamUnk1928Block*)src;
}

void* CActorParam_UnkVirtualFunc145__Q22cf11CActorParamFv(cf::CActorParam* self, unsigned long idx) {
    // retail: mulli r4,0x18 / add / addi r3,r3,0x1928 - the declared unk1928
    // member is +8 shifted (lands at 0x1930), so the absolute-offset view
    // pins the retail base 0x1928
    return &((cf::CActorParamUnk1928View*)self)->entries[idx];
}

void* CActorParam_UnkVirtualFunc147__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1928); }

u32 CActorParam_UnkVirtualFunc139__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x2A80); }

void cf::CActorParam::CActorParam_UnkVirtualFunc134() {
    *(u8*)((u8*)this + 0x3354) = 0;
}

u8 CActorParam_UnkVirtualFunc133__Q22cf11CActorParamFv(cf::CActorParam* self) {
    // retail: lbz r3, 0x3354(r3) - member block is +8 shifted, raw view pins the offset
    return ((cf::CActorParam3354View*)self)->field_0x3354;
}

void* CActorParam_UnkVirtualFunc131__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x31dc); }

void* CActorParam_UnkVirtualFunc130__Q22cf11CActorParamFv(cf::CActorParam* self, unsigned long index) { return (u8*)self + index * 0xBC + 0x2A84; }

u32 CActorParam_UnkVirtualFunc128__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x15E0); }

void* CActorParam_UnkVirtualFunc124__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x2740); }

// us-800b0620: CActorParam::CActorParam_UnkVirtualFunc123. Retail symbol is
// Fv but the real ABI passes (self, r4 = attack-set data block): copies the
// 12-byte header to self+0x2740, then for each of the six 0x88-byte
// CAttackParam records at self+0x2750 stores strlen at +0x20, strcpy's the
// +0x00 name and copies the +0x24..+0x80 typed fields (the +0x84 vtable
// word is skipped).
void CActorParam_UnkVirtualFunc123__Q22cf11CActorParamFv(cf::CActorParam* self,
                                                         const cf::CfEneAttackData* src) {
    cf::CfEneAttackArea* dst = (cf::CfEneAttackArea*)self;
    const cf::CfEneAttackEntry* s = &src->records[0];
    cf::CfEneAttackEntry* d = &dst->records[0];
    cf::CfEneAttackEntry* end = &dst->records[6];
    // volatile keeps each access strictly in program order (retail: three
    // separate u32 copies stored +4 / +0 / +8, never hoisted)
    volatile cf::CfEneAttackHdrBlock* vh =
        (volatile cf::CfEneAttackHdrBlock*)&dst->header;
    const volatile cf::CfEneAttackSrcHdrBlock* vsh =
        (const volatile cf::CfEneAttackSrcHdrBlock*)&src->header;
    u32 t0 = vsh->header.h0;
    u32 t4 = vsh->header.h4;
    vh->header.h4 = t4;
    vh->header.h0 = t0;
    vh->header.h8 = vsh->header.h8;
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

void* CActorParam_UnkVirtualFunc121__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x19e8); }

// us-800b07e8: CActorParam::CActorParam_UnkVirtualFunc120 (Fv retail; real
// ABI (self, r4 = arts-set data block)): copies the 0x34-byte arts-set
// header to self+0x19E8, then for each of the 24 0x8C-byte CArtsParam
// records at self+0x1A20 stores strlen at +0x20, strcpy's the name and
// copies the +0x24..+0x80 fields plus the +0x88 word (the +0x84 vtable
// word is skipped).
void CActorParam_UnkVirtualFunc120__Q22cf11CActorParamFv(cf::CActorParam* self,
                                                         const cf::CfEneArtsData* src) {
    cf::CfEneArtsArea* dst = (cf::CfEneArtsArea*)self;
    const volatile cf::CfEneArtsEntry* s = &src->records[0];
    volatile cf::CfEneArtsEntry* d = &dst->records[0];
    cf::CfEneArtsEntry* end = &dst->records[24];
    // retail expands the 0x34 header copy field-by-field; volatile keeps each
    // access strictly in program order (plain assignments collapse into a
    // block struct copy, which retail does not emit)
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

u8 CActorParam_UnkVirtualFunc111__Q22cf11CActorParamFv(cf::CActorParam* self) {
    // retail: lbz r3, 0x183c(r3) - member block is +8 shifted, raw view pins the offset
    return ((cf::CfObjectEne183AView*)self)->field_0x183C;
}

u8 CActorParam_UnkVirtualFunc110__Q22cf11CActorParamFv(cf::CActorParam* self) {
    // retail: lbz r3, 0x183b(r3) - member block is +8 shifted, raw view pins the offset
    return ((cf::CfObjectEne183AView*)self)->field_0x183B;
}

u8 CActorParam_UnkVirtualFunc109__Q22cf11CActorParamFv(cf::CActorParam* self) {
    // retail: lbz r3, 0x183a(r3) - member block is +8 shifted, raw view pins the offset
    return ((cf::CfObjectEne183AView*)self)->field_0x183A;
}

void CActorParam_UnkVirtualFunc108__Q22cf11CActorParamFv(cf::CActorParam* self, u8 val) {
    // retail: stb r4, 0x183b(r3) - member block is +8 shifted, raw view pins the offset
    ((cf::CfObjectEne183AView*)self)->field_0x183B = val;
}

void CActorParam_UnkVirtualFunc107__Q22cf11CActorParamFv(cf::CActorParam* self, u8 val) {
    // retail: stb r4, 0x183a(r3) - member block is +8 shifted, raw view pins the offset
    ((cf::CfObjectEne183AView*)self)->field_0x183A = val;
}

void CActorParam_UnkVirtualFunc105__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x15fc) = val; }

void* CActorParam_UnkVirtualFunc104__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x15fc); }

// (lbl_eu_80666968 declared in CfObjectEne.hpp)
void cf::CActorParam::CActorParam_UnkVirtualFunc103() {
    *(f32*)((u8*)this + 0x15FC) = lbl_eu_80666968;
}

void* CActorParam_UnkVirtualFunc102__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x18d4); }

void* CActorParam_UnkVirtualFunc101__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x185c); }

void* CActorParam_UnkVirtualFunc99__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x17e4); }

void* CActorParam_UnkVirtualFunc97__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1792); }

void* CActorParam_UnkVirtualFunc96__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1740); }

void* CActorParam_UnkVirtualFunc95__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x16c8); }

void* CActorParam_UnkVirtualFunc93__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1650); }


// us-800b0be0: retail symbol is Fv; the real ABI passes (self, r4-unused,
// arg, flag). Dispatches the primary vtable slots +0x1DC (no arg) and +0x1FC
// (arg), then forwards the +0x15E0 sub-object to func_802617B8 when present.
void CActorParam_UnkVirtualFunc88__Q22cf11CActorParamFv(cf::CActorParam* self, int unused, void* arg, int flag) {
    cf::CfActorParamVt1DC* vt = reinterpret_cast<cf::CfActorParamVt1DC*>(self);
    vt->vf1DC();
    vt->vf1FC(arg);
    u8* obj = ((cf::CfActorParam15E0View*)self)->field_0x15E0;
    if (obj != NULL) {
        func_802617B8(obj, ((cf::CfActorObj89CView*)obj)->field_0x89C, flag);
    }
}

u32 CActorParam_UnkVirtualFunc84__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x1600) + *(u32*)((u8*)self + 0x1604); }

void* CActorParam_UnkVirtualFunc80__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1834); }

void* CActorParam_UnkVirtualFunc79__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x182c); }

void CActorParam_UnkVirtualFunc78__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x1834) = val; }

void CActorParam_UnkVirtualFunc77__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x182c) = val; }

void* CActorParam_UnkVirtualFunc75__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1828); }

void CActorParam_UnkVirtualFunc73__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x1828) = val; }

float CActorParam_UnkVirtualFunc72__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1824); }

void CActorParam_UnkVirtualFunc71__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x1824) = val; }

float CActorParam_UnkVirtualFunc69__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1618); }

float CActorParam_UnkVirtualFunc66__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1610); }

s16 CActorParam_UnkVirtualFunc63__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(s16*)((u8*)self + 0x1616); }

s16 CActorParam_UnkVirtualFunc62__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(s16*)((u8*)self + 0x1614); }

void CActorParam_UnkVirtualFunc59__Q22cf11CActorParamFv(cf::CActorParam* self, u16 val) { *(u16*)((u8*)self + 0x1614) = val; }

s16 CActorParam_UnkVirtualFunc57__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(s16*)((u8*)self + 0x160e); }

s16 CActorParam_UnkVirtualFunc56__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(s16*)((u8*)self + 0x160c); }

void CActorParam_UnkVirtualFunc53__Q22cf11CActorParamFv(cf::CActorParam* self, u16 val) { *(u16*)((u8*)self + 0x160C) = val; }

float CActorParam_UnkVirtualFunc51__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17F0) / *(float*)((u8*)self + 0x17FC); }

float CActorParam_UnkVirtualFunc50__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17fc); }

float CActorParam_UnkVirtualFunc49__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17f0); }

void CActorParam_UnkVirtualFunc47__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x17f0) = val; }

float CActorParam_UnkVirtualFunc45__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17EC) / *(float*)((u8*)self + 0x17F8); }

float CActorParam_UnkVirtualFunc44__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17f8); }

float CActorParam_UnkVirtualFunc43__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17ec); }

// us-800b0d38: add amount to the 0x17EC gauge and clamp into
// [lbl_eu_80666968 (0.0f), field_0x17F8]. Same arg-reuse pattern as Func31
// (lfs f2,cur; lfs f0,min; fadds f1,f2,f1; lfs f2,max; stfs f1); the upper
// compare is against the max local (fcmpo f1,f2; blelr stores max when the
// value exceeds it or is unordered).
void CActorParam_UnkVirtualFunc42__Q22cf11CActorParamFv(cf::CActorParam* self, float amount) {
    float min = lbl_eu_80666968;
    float cur = ((cf::CActorParam17ECView*)self)->field_0x17EC;
    float sum = cur + amount;
    float max = ((cf::CActorParam17ECView*)self)->field_0x17F8;
    ((cf::CActorParam17ECView*)self)->field_0x17EC = sum;
    if (sum < min)
        ((cf::CActorParam17ECView*)self)->field_0x17EC = min;
    else if (sum > max)
        ((cf::CActorParam17ECView*)self)->field_0x17EC = max;
}

void CActorParam_UnkVirtualFunc41__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x17ec) = val; }

float CActorParam_UnkVirtualFunc39__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17E8) / *(float*)((u8*)self + 0x17F4); }

// us-800b0d84: retail symbol is Fv; the real ABI passes (self, val). Scales
// val by the gauge field at 0x17F4 and tail-dispatches vtable slot +0x11C
// (CActorParam_UnkVirtualFunc34).
void CActorParam_UnkVirtualFunc36__Q22cf11CActorParamFv(cf::CActorParam* self, float val) {
    reinterpret_cast<cf::CfActorParamVt118*>(self)->m11C(((cf::CfActorParamFields*)self)->field_0x17F4 * val);
}

// us-800b0d9c: retail symbol is Fv; the real ABI passes (self, amount). Adds
// amount to the 0x17E8 gauge, stores it, rounds to the nearest int (half
// away from zero: (s32)(v +/- 0.5) via the fctiwz + s32->f32 magic pair),
// floors sub-1.0 results back to 0.0f, then clamps into [0.0f, 0x17F4].
// Same rounding tail as CActorParam_UnkVirtualFunc35 (CfObjectActor.cpp).
void CActorParam_UnkVirtualFunc34__Q22cf11CActorParamFv(cf::CActorParam* self, float amount) {
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

float CActorParam_UnkVirtualFunc32__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1660); }

// us-800b0e40: add amount to the 0x1660 gauge and clamp at the 0.0f floor
// (lbl_eu_80666968 = 0.0f). Retail emits lfs f2,cur; lfs f0,lbl(0.0f);
// fadds f1,f2,f1; stfs f1 - the floor stays in f0 and the accumulated value
// reuses the arg register. The else-if is the retail dev's always-false
// `amount > amount` guard: fcmpo cr0,f1,f1 / blelr over a redundant store
// (bclr 4,1 = branch when NOT GT keeps the tail-return shape).
void CActorParam_UnkVirtualFunc31__Q22cf11CActorParamFv(cf::CActorParam* self, float amount) {
    float min = lbl_eu_80666968;
    float cur = ((cf::CActorParam1660View*)self)->field_0x1660;
    amount = cur + amount;
    ((cf::CActorParam1660View*)self)->field_0x1660 = amount;
    if (amount < min)
        ((cf::CActorParam1660View*)self)->field_0x1660 = min;
    else if (amount > amount)
        ((cf::CActorParam1660View*)self)->field_0x1660 = amount;
}

void CActorParam_UnkVirtualFunc30__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x1660) = val; }

void CActorParam_UnkVirtualFunc28__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x17E4) += val; }

void CActorParam_UnkVirtualFunc27__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x17E4) = val; }

void CActorParam_UnkVirtualFunc25__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x1650) += val; }

void CActorParam_UnkVirtualFunc24__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x1650) = val; }

float CActorParam_UnkVirtualFunc17__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x15f8); }

u8 CActorParam_UnkVirtualFunc15__Q22cf11CActorParamFv(cf::CActorParam* self) {
    // retail: lbz r3, 0x15f4(r3) - the declared unk15F4[4] member is +8
    // shifted (lands at 0x15FC), raw view pins the retail offset
    return ((cf::CfObjectEne15F4View*)self)->field_0x15F4;
}

u32 CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return (*(u32*)((u8*)self + 104) >> 21) & 0x1u; }

u32 CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return *(u32*)((u8*)self + 0xB4); }

float CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return *(float*)((u8*)self + 0xac); }

float CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return *(float*)((u8*)self + 0xa8); }

u32 CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) { return *(u32*)((u8*)self + 0x704); }

u32 CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) { return *(u32*)((u8*)self + 0x708); }

void CfObjectActor_UnkVirtualFunc13__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) {
    *(long*)((char*)self + 0x45bc) = -1;
}

u32 CfObjectActor_UnkVirtualFunc12__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { return *(u32*)((u8*)self + 0x45BC); }

void CBattleState_UnkVirtualFunc18__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

void CBattleState_UnkVirtualFunc17__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

void CBattleState_UnkVirtualFunc3__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

void CBattleState_UnkVirtualFunc1__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

void CBattleState_UnkVirtualFunc2__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

void CObjectParam_UnkVirtualFunc4__Q22cf11CfObjectEneFv(cf::CfObjectEne* self) { ((void(*)(void*))func_800ADDA8__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

void CfObject_UnkVirtualFunc31__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc6__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

void CfObjectMove_UnkVirtualFunc15__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc2__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }


void CObjectParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

void CfObjectMove_UnkVirtualFunc6__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc11__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

void CfObject_UnkVirtualFunc14__Q22cf13CfObjectActorFf(cf::CfObjectActor* self, float value) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc10__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

void CfObject_UnkVirtualFunc2__Q22cf11CfObjectEneFv(cf::CfObjectEne* self) { ((void(*)(void*))func_800ADB2C__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

void CfObject_UnkVirtualFunc4__Q22cf11CfObjectEneFv(cf::CfObjectEne* self) { ((void(*)(void*))func_800ADBD4__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

void func_800B069C__Q22cf11CfObjectEneFv(cf::CfObjectEne* self) { ((void(*)(void*))__dt__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

void CBattleState_UnkVirtualFunc23__Q22cf12CBattleStateFv(cf::CBattleState* self) { reinterpret_cast<BSIf*>(self)->vf0044(); }

void CBattleState_UnkVirtualFunc22__Q22cf12CBattleStateFv(cf::CBattleState* self) { reinterpret_cast<BSIf*>(self)->vf0040(); }

void CBattleState_UnkVirtualFunc21__Q22cf12CBattleStateFv(cf::CBattleState* self) { reinterpret_cast<BSIf*>(self)->vf003C(); }

void CBattleState_UnkVirtualFunc20__Q22cf12CBattleStateFv(cf::CBattleState* self) { reinterpret_cast<BSIf*>(self)->vf0038(); }

void CActorParam_UnkVirtualFunc98__Q22cf11CActorParamFv(cf::CActorParam* self, const void* src) {
    *(ParamCopyBlock*)((u8*)self + 0x17e4) = *(const ParamCopyBlock*)src;
}

void CActorParam_UnkVirtualFunc92__Q22cf11CActorParamFv(cf::CActorParam* self, const void* src) {
    *(ParamCopyBlock*)((u8*)self + 0x1650) = *(const ParamCopyBlock*)src;
}
