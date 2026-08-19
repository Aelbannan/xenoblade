#include <types.h>
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
#include "kyoshin/cf/object/CfObjectEne.hpp"


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
// The subobject address goes through a DIFFERENT struct member at each call
// site so MWCC re-materializes addi r3, r31, 0x3e9c per call (retail keeps
// only r31 = self; a CSE'd address would live in a callee-saved register and
// grow the frame).
// us-800ae3f8: CfObjectEne vf2. Runs the CfObjectMove subobject's
// CfObject_UnkVirtualFunc2 (CfObjectModel slot), dispatches the subobject
// vtable slot +0x158 with flag 1, clears two battle-state flag words
// (func_800BE33C / func_800BE824 on the subobject with 1), raises three
// actor flags via func_80174B4C, seeds the +0x44A8 region via func_804B0AD4
// (two sdata2 floats) and sets its u16 at +0xB2 to 0xC8. Returns 1.
// The dispatch is a real virtual call through CfEneSubFake (retail
// folded-vptr + recomputed adjusted-this); KNOWN WALL: MWCC CSEs the three
// plain subobject args into a callee-saved register (frame 0xB4 vs retail
// 0xA8) - the cross-call address CSE is irreducible (cf. MWCC_CASES
// HBMSYN NoteOn / GKI notes).
int func_800ADB2C__Q22cf11CfObjectEneFv(cf::CfObjectEne* self) {
    CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv((cf::CfObjectModel*)((u8*)self + 0x3E9C));
    ((cf::CfEneSubFake*)self)->vf158(1);
    func_800BE33C((u8*)self + 0x3E9C, 1);
    func_80174B4C(self, 0x100000);
    func_80174B4C(self, 0x08000000);
    func_80174B4C(self, 0x10000000);
    func_800BE824((u8*)self + 0x3E9C, 1);
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
    if (func_80174C98((u8*)this, &wordA, 0x803) == 0 &&
        ((wordB = *((cf::CfActorUnk4Vt30*)((cf::CfActorField04*)this)->field_0x04)->vf30()),
         func_80174C98((u8*)this, &wordB, 0xE)) == 0) {
        if (func_80148778(&((cf::CfEneB8View*)this)->field_0x8, 0x35) == 0) {
            ((cf::CfEneB8V14*)this)->m14(0x35);
        }
    } else {
        func_800AF870(this);
        if (func_80148778(&((cf::CfEneB8View*)this)->field_0x8, 0x35) != 0) {
            ((cf::CfEneB8V20*)this)->m20(0x35);
        }
    }
    f32 sub = ((cf::CfEneSubVt64*)this)->m8C();
    f32 f = func_80496288(lbl_eu_80663E14) * sub;
    func_801765A4((u8*)this, f, 1);
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x1000000) != 0) {
        u32 wordC = *((cf::CfActorUnk4Vt30*)((cf::CfActorField04*)this)->field_0x04)->vf30();
        if (func_80174C98((u8*)this, &wordC, 0x1) == 0) {
            u8* p = ((cf::CfEneField3F34*)this)->field_0x3F34;
            if (p != 0)
                ((cf::CfEneField7A4*)p)->field_0x7A4 |= 0x80000000;
            ((cf::CfEneSubVt64b*)this)->m80();
            ((cf::CfEneSubVt64c*)this)->m64();
        }
    } else {
        CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv((cf::CfObjectMove*)((cf::CfEneSubPad*)this));
    }
}

// us-800ae674: CfObjectEne::func_800ADDA8 (retail Fv; this in r3). Full
// enemy-parameter setup: reads the +0x3F14-name bdat table and the
// lbl_eu_80664094 arts bdat table, dispatches the +0x3E9C subobject
// (+0x3C with the col-0 value, +0x1D4/+0x134 with the signed-byte
// conversions, +0x13C with 1.0f) and the primary vtable setup slots
// (+0xDC/+0xE4/+0xE8, +0x254..+0x26C, +0x170..+0x1BC), accumulates the
// +0x1648/+0x164A/+0x164C status words, builds the rates record from the
// +0x20C slot (+0xA2..+0xF9 columns, x100 weight, s16 offsets), writes the
// level-name letters to lbl_eu_80661CB8/80661CBC (+3), scales the
// +0x1604/+0x1608 weights by (s16+10)/10, initializes the +0x8 CBattleState
// subobject (+0x18 buffer dispatch, gated on +0x3374 bit 0x40000) and the
// +0x18 dispatches the tail vtable slots +0x21C/+0x330/+0x32C/+0x5E0;
// gates the second buffer on +0x3374 bit 0x10000.
void cf::CfObjectEne::func_800ADDA8() {
    cf::CfEneLookupView* v = (cf::CfEneLookupView*)this;
    cf::CfEneParamsView* p = (cf::CfEneParamsView*)this;
    u8* bdatArts = lbl_eu_80664094;
    void* bdat = getFP(v->field_0x3F14);
    u16 row = v->field_0x3F28;
    u32 c0 = getBdatStringColumnValue(bdatArts, &lbl_eu_804FC168[0x0], row);
    ((cf::CfEneSubVt3C*)this)->vf3C(c0);
    ((cf::CfEneVtSetup1*)this)->vfE4((u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x5], row));
    ((cf::CfEneVtSetup1*)this)->vfE8((f32)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xA], row) / lbl_eu_8066697C);
    ((cf::CfEneVtSetup1*)this)->vfDC((u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x10], row));
    p->field_0x15F0 = (u32)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x17], row);
    u32 flags = p->field_0x3374;
    flags |= (u32)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x1D], row);
    p->field_0x3374 = flags;
    if (!(flags & 0x800)) {
        u8 k22 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x22], row);
        ((cf::CfEneSubVt3C*)this)->vf1D4((f32)(s32)k22);
        u8 k2D = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x2D], row);
        ((cf::CfEneSubVt3C*)this)->vf134((f32)(s32)k2D);
        ((cf::CfEneSubVt3C*)this)->vf13C(lbl_eu_80666980);
        if (k22 == 0 || k2D == 0)
            p->field_0x3374 |= 0x1000;
    }
    ((cf::CfEneVtSetup3*)this)->vf254((u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x37], row));
    ((cf::CfEneVtSetup3*)this)->vf25C((u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x3F], row) != 0);
    ((cf::CfEneVtSetup3*)this)->vf264((f32)(u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x46], row));
    ((cf::CfEneVtSetup3*)this)->vf26C((f32)(u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x53], row) * lbl_eu_8066A210);
    ((cf::CfEneVtSetup2*)this)->vf170(1);
    ((cf::CfEneVtSetup2*)this)->vf188(1);
    ((cf::CfEneVtSetup2*)this)->vf198(lbl_eu_80666984);
    ((cf::CfEneVtSetup2*)this)->vf1A4(lbl_eu_80666984);
    ((cf::CfEneVtSetup2*)this)->vf1BC(lbl_eu_80666988 * (lbl_eu_8066A1F8 / lbl_eu_8066698C));
    p->field_0x1648 |= (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x60], row);
    p->field_0x164A |= (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x6A], row);
    p->field_0x164C |= (u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x80], row);
    ((cf::CfBattleVt7C*)((u8*)this + 8))->vf7C((u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x8B], row));
    ((cf::CfBattleVt84*)((u8*)this + 8))->vf84((u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x96], row));
    u8 v96 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x96], row);
    if (v96 != 0) {
        ((cf::CfEneVtTail*)this)->vf334((lbl_eu_8066A1F8 / lbl_eu_80666990) *
                                       (f32)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x96], row));
    }
    cf::CfEneRatesView* g = ((cf::CfEneVtActs*)this)->vf20C();
    g->field_0x0 = (u32)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xA2], row);
    u16 wA5 = (u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xA5], row);
    f32 w = (f32)(s32)((u16)wA5 * 100);
    g->field_0x10 = w;
    g->field_0x4 = w;
    g->field_0xC = lbl_eu_80666968;
    g->field_0x18 = lbl_eu_80666994;
    g->field_0x1C = (s16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xAC], row);
    g->field_0x1E = (s16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xB0], row);
    g->field_0x20 = (s16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xB4], row);
    g->field_0x40 = (f32)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xBA], row) / lbl_eu_8066697C;
    g->field_0x44 = (f32)(u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xC9], row) / lbl_eu_80666994;
    g->field_0x48 = (f32)(u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xD7], row) / lbl_eu_80666994;
    g->field_0x56 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xDF], row);
    g->field_0x57 = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xE7], row);
    g->field_0x60 = (u16)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xF0], row);
    g->field_0x62 = (u16)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xF9], row);
    g->field_0x73 = (u8)(getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xF9], row) & 0xF);
    getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xF9], row);
    u8 zero = 0;
    g->field_0x70 = zero;
    getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xF9], row);
    g->field_0x72 = zero;
    p->field_0x1629 = zero;
    p->field_0x162A = zero;
    p->field_0x162B = 3;
    p->field_0x162C = 7;
    u16 w456C = ((cf::CfObjectEne456CView*)this)->field_0x456C;
    s32 half = (s32)w456C >> 4;
    u32 lo = w456C & 0xF;
    if (half != 0) {
        u8 letter = (u8)(lo + 0x30);
        lbl_eu_80661CB8[3] = letter;
        lbl_eu_80661CBC[3] = letter;
        u8* bdat9C = lbl_eu_8066409C;
        s8 sn = (s8)getBdatStringColumnValue(bdat9C, (const char*)lbl_eu_80661CBC, (u16)half);
        u8 u0A = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xA], row);
        s8 s2 = (s8)getBdatStringColumnValue(bdat9C, (const char*)lbl_eu_80661CBC, (u16)half);
        u16 u102 = (u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x102], row);
        u16 u10B = (u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x10B], row);
        u16 u115 = (u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x115], row);
        u16 u11F = (u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x11F], row);
        s32 sum = (s32)u0A + (s32)s2;
        u8 u129 = (u8)getBdatStringColumnValue(bdat9C, &lbl_eu_804FC168[0x129], u11F);
        if (u129 & 0x20) {
            ((cf::CfEneVtSetup1*)this)->vfE8((f32)(s32)sum);
        } else {
            ((cf::CfEneVtSetup1*)this)->vfE8(lbl_eu_80666998 * (f32)(s32)sum);
        }
        g->field_0x0 = g->field_0x0 + (u32)sn;
        g->field_0x4 = (f32)sn * (f32)u102 + g->field_0x4;
        g->field_0x1C = (s16)((f32)sn * (f32)u10B + (f32)g->field_0x1C);
        g->field_0x1E = (s16)((f32)sn * (f32)u115 + (f32)g->field_0x1E);
        g->field_0x20 = (s16)((f32)sn * (f32)u11F + (f32)g->field_0x20);
        if (g->field_0x0 < 1) g->field_0x0 = 1;
        if (g->field_0x4 < lbl_eu_80666980) g->field_0x4 = lbl_eu_80666980;
        if (g->field_0x1C < 1) g->field_0x1C = 1;
        if (g->field_0x1E < 1) g->field_0x1E = 1;
        if (g->field_0x20 < 1) g->field_0x20 = 1;
        g->field_0x10 = g->field_0x4;
    } else {
        ((cf::CfEneVtSetup1*)this)->vfE8(lbl_eu_80666998 *
                                         (f32)(s32)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0xA], row));
        p->field_0x3F00 |= 0x04000000;
    }
    u32 cap = g->field_0x0;
    if (cap > 0x63) cap = 0x63;
    u8* bdatDC = (u8*)lbl_eu_806640DC;
    u16 w132 = (u16)getBdatStringColumnValue(bdatDC, &lbl_eu_804FC168[0x132], cap);
    u16 w139 = (u16)getBdatStringColumnValue(bdatDC, &lbl_eu_804FC168[0x139], cap);
    s32 base = (s32)(s16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x13F], row) + 10;
    p->field_0x1604 = (u32)w132 * (u32)base / 10;
    p->field_0x1608 = (u32)w139 * (u32)base / 10;
    u8 n = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x143], row);
    p->field_0x1634 = n;
    if (n != 0) {
        u8 kind = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x14E], row);
        p->field_0x1638 = (u32)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x159], row);
        p->field_0x163C = (u32)((u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x165], row) * 10);
        p->field_0x1640 = (u32)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x16F], row);
        p->field_0x1644 = (u32)(u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x17B], row);
        if (kind == 2) p->field_0x1634 = p->field_0x1634 + 5;
        if (kind == 3) p->field_0x1634 = p->field_0x1634 + 10;
        if (!(p->field_0x3374 & 0x40000)) {
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
    u8 cnt = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x18B], row);
    if (cnt != 0) {
        cf::CfEneSetupBuf bufb;
        std::memset(&bufb, 0, 0x34);
        bufb.field_0x0 = p->field_0x3F10;
        bufb.field_0x4 = 0;
        bufb.field_0xC = 0xC3;
        bufb.field_0x10 = cnt;
        bufb.field_0x20 = lbl_eu_80666968;
        ((cf::CfBattleVt18*)((u8*)this + 8))->m18(&bufb);
    }
    p->field_0x3370 = (u32)(u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x191], row);
    ((cf::CfEneVtSetup3*)this)->vf21C(((cf::CfEneVtActs*)this)->vf20C());
    ((cf::CfEneVtTail*)this)->vf330();
    ((cf::CfEneVtTail*)this)->vf32C();
    ((cf::CfEneVt5E0*)this)->vf5E0();
    if (p->field_0x3374 & 0x80000) {
        p->field_0x3F04 |= 0x20000000;
    }
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

void cf::CActorParam::CActorParam_UnkVirtualFunc98() {}

// us-800af534: CfObjectEne::func_800AEC68 (retail Fv; this in r3). Resets
// the bdat manager, reads the enemy bdat column value (getFP on the
// +0x3F14 name, row id at +0x3F28), multiplies it by the vtable +0xF0
// result and the 0x806669B0 scale, feeds the vtable +0xD4 slot, runs the
// primary vtable +0x288 slot six times and resets the +0x3380 CAIAction
// subobject (func_8014B7B0 / func_8015396C).
void cf::CfObjectEne::func_800AEC68() {
    func_8003AA34();
    cf::CfEneLookupView* v = (cf::CfEneLookupView*)this;
    u16 value = (u16)getBdatStringColumnValue(getFP(v->field_0x3F14),
                                              &lbl_eu_804FC168[0x19A],
                                              v->field_0x3F28);
    f32 base = ((cf::CfEneVtD4*)this)->mF0();
    cf::CfEneF64Conv conv;
    conv.w[1] = (u32)value;
    conv.w[0] = 0x43300000;
    f32 f = (f32)(conv.d - lbl_eu_806669A0);
    ((cf::CfEneVtD4*)this)->mD4(lbl_eu_806669B0 * f * base);
    for (int i = 0; i < 6; i++)
        ((cf::CfEneVtD4*)this)->m288();
    func_8014B7B0(&((cf::CfEneAI3380View*)this)->field_0x3380);
    func_8015396C(&((cf::CfEneAI3380View2*)this)->field_0x3380, 0, 0);
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
    u32 s0 = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x2A7], ((cf::CfEneLookupView*)this)->field_0x3F28);
    ((cf::CfEneVtActs*)this)->vf288()[0] = (u16)s0;
    u32 s1 = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x2AC], ((cf::CfEneLookupView*)this)->field_0x3F28);
    ((cf::CfEneVtActs*)this)->vf288()[1] = (u16)s1;
    u32 s2 = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x2B1], ((cf::CfEneLookupView*)this)->field_0x3F28);
    ((cf::CfEneVtActs*)this)->vf288()[2] = (u16)s2;
    ((cf::CfEneVtActs*)this)->vf288()[3] = 0;
    ((cf::CfEneVtActs*)this)->vf288()[4] = 0;
    ((cf::CfEneVtActs*)this)->vf288()[5] = 0;

    func_8003AA34();
    void* bdat2 = getFP(&lbl_eu_804FC168[0x2B6]);
    f32 zero = lbl_eu_80666968;
    f32 scale = lbl_eu_8066A20C;
    for (int i = 0; i < 6; i++) {
        u16* slots = ((cf::CfEneVtActs*)this)->vf288();
        u16 atkId = slots[(s16)i];
        if (atkId == 0) continue;
        cf::CfEneAtkParamView* atk = (cf::CfEneAtkParamView*)getAtkParam(((cf::CfEneVtActs*)this)->vf288(), i);
        char* nm = (char*)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x0], atkId);
        atk->field_0x20 = strlen(nm);
        strcpy(atk->name, nm);
        u16 v1D = (u16)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x1D], atkId);
        atk->field_0x78 = v1D;
        atk->field_0x2C = zero;
        atk->field_0x30 = zero;
        s16 v1EF = (s16)(s8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x1EF], atkId);
        atk->field_0x36 = v1EF;
        s16 v2BE = (s16)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x2BE], atkId);
        atk->field_0x38 = v2BE;
        atk->field_0x3A = 0;
        atk->field_0x42 = 0;
        s16 v209 = (s16)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x209], atkId);
        atk->field_0x3C = v209;
        s16 v212 = (s16)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x212], atkId);
        atk->field_0x40 = v212;
        atk->field_0x43 = 1;
        atk->field_0x44 = 1;
        atk->field_0x46 = 0;
        atk->field_0x58 = 0;
        atk->field_0x5A = 0;
        atk->field_0x5C = 1;
        atk->field_0x5E = 0;
        cf::CfEneRatesView* gr = ((cf::CfEneVtActs*)this)->vf20C();
        f32 gauge;
        if (gr->field_0x48 == zero) {
            gr = ((cf::CfEneVtActs*)this)->vf20C();
            gauge = gr->field_0x44;
        } else {
            gr = ((cf::CfEneVtActs*)this)->vf20C();
            gauge = gr->field_0x48;
        }
        atk->field_0x60 = gauge;
        f32 rate = ((cf::CfEneVtActs*)this)->vf1C4()->field_0x0;
        atk->field_0x64 = (s16)(rate * scale);
        u8 v256 = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x256], atkId);
        atk->field_0x76 = v256;
        u8 v25E = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x25E], atkId);
        atk->field_0x77 = v25E;
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
    func_8003AA34();
    cf::CfEneLookupView* v = (cf::CfEneLookupView*)this;
    void* bdat = getFP(v->field_0x3F14);
    u16 row = v->field_0x3F28;
    u32 a0 = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x1A7], row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), (u16)a0, 0);
    u32 a1 = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x1AD], row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), (u16)a1, 1);
    u32 a2 = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x1B3], row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), (u16)a2, 2);
    u32 a3 = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x1B9], row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), (u16)a3, 3);
    u32 a4 = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x1BF], row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), (u16)a4, 4);
    u32 a5 = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x1C5], row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), (u16)a5, 5);
    u32 a6 = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x1CB], row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), (u16)a6, 6);
    u32 a7 = getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x1D1], row);
    setArtsSlotByIdx(((cf::CfEneVtActs*)this)->vf27C(), (u16)a7, 7);

    func_8003AA34();
    void* bdat2 = getFP(&lbl_eu_804FC168[0x1D7]);
    cf::CfEneF64Conv cvA;   // magic conversion pairs (retail pins them to the frame
    cf::CfEneF64Conv cvB;   // at 0xA8/0xB0 and hoists the magic double + divisors
    for (int i = 0; i < 8; i++) {
        u16 slot = func_80153CAC(((cf::CfEneVtActs*)this)->vf27C(), (s16)i);
        if (slot == 0) continue;
        cf::CfEneArtsParamView* arts = (cf::CfEneArtsParamView*)getArtsParamByIdx(((cf::CfEneVtActs*)this)->vf27C(), i);
        char* nm = (char*)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x0], slot);
        arts->field_0x20 = strlen(nm);
        strcpy(arts->name, nm);
        arts->field_0x78 = getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x1D], slot);
        cvA.w[0] = 0x43300000;
        cvA.w[1] = (u32)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x1E0], slot);
        arts->field_0x2C = ((f32)(cvA.d - lbl_eu_806669A0) / lbl_eu_8066697C);
        u8 e5 = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x1E5], slot);
        if (e5 == 0xFF) {
            arts->field_0x30 = lbl_eu_806669B4;
        } else {
            e5 = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x1E5], slot);
            cvB.w[0] = 0x43300000;
            cvB.w[1] = (u32)e5;
            arts->field_0x30 = (f32)(cvB.d - lbl_eu_806669A0);
        }
        arts->field_0x34 = (s16)(s8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x1EC], slot);
        arts->field_0x36 = (s16)(s8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x1EF], slot);
        arts->field_0x38 = (s16)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x1F3], slot);
        arts->field_0x3A = (s16)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x1F9], slot);
        arts->field_0x42 = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x1FF], slot);
        u8 a2 = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0xA2], slot);
        if (a2 != 0)
            arts->field_0x2A = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0xA2], slot);
        arts->field_0x3C = (u16)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x209], slot);
        arts->field_0x40 = (u16)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x212], slot);
        arts->field_0x43 = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x217], slot);
        arts->field_0x44 = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x220], slot);
        arts->field_0x46 = 0;
        if ((u16)(arts->field_0x3C - 1) <= 1 && arts->field_0x44 == 0) {
            ((cf::CfEneAtkVtblRec*)arts)->vtInit();
            continue;
        }
        arts->field_0x58 = (u16)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x229], slot);
        arts->field_0x5A = (u16)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x231], slot);
        arts->field_0x5C = (u16)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x237], slot);
        arts->field_0x5E = (u16)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x23B], slot);
        u16 w246 = (u16)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x246], slot);
        f32 r44 = ((cf::CfEneVtActs*)this)->vf20C()->field_0x44;
        cvA.w[0] = 0x43300000;
        cvA.w[1] = (u32)w246;
        arts->field_0x60 = (f32)(cvA.d - lbl_eu_806669A0) / lbl_eu_80666994 + r44;
        arts->field_0x64 = (u16)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x24C], slot);
        arts->field_0x76 = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x25E], slot);
        arts->field_0x77 = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x262], slot);
        arts->field_0x48 = (u16)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x26A], slot);
        arts->field_0x4A = (u16)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x271], slot);
        arts->field_0x4C = (u16)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x279], slot);
        cvB.w[0] = 0x43300000;
        cvB.w[1] = (u32)(u16)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x281], slot);
        arts->field_0x50 = (f32)(cvB.d - lbl_eu_806669A0) / lbl_eu_8066697C;
        cvA.w[0] = 0x43300000;
        cvA.w[1] = (u32)(u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x288], slot);
        arts->field_0x54 = (f32)(cvA.d - lbl_eu_806669A0) / lbl_eu_8066697C;
        arts->field_0x66 = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x28F], slot);
        arts->field_0x67 = (u8)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x297], slot);
        arts->field_0x68 = (s16)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x29F], slot);
        arts->field_0x6A = (s16)getBdatStringColumnValue(bdat2, &lbl_eu_804FC168[0x2A7], slot);
        arts->field_0x78 |= 0x40000000;
        if (arts->field_0x42 == 1)
            arts->field_0x78 |= 0x80000000;
        if (arts->field_0x78 & 0x8000)
            arts->field_0x2C *= lbl_eu_80666994;
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
    u8* tgt = ((cf::CfActorField3F60*)self)->field_0x3F60;
    if (tgt == 0) return;
    if ((((cf::CfEneMoveTgtView*)tgt)->field_0x4EC & 0x100) == 0) return;
    if (((cf::CfEneVt2BC*)self)->m2BC() != 0) return;
    cf::CfEneBmView* bm = (cf::CfEneBmView*)getInstance__Q22cf14CBattleManagerFv();
    u8 part = bm->field_0x1AA;
    if (part >= 1 && part <= 0x18) return;
    if (func_801BA2C8(&((cf::CfEneBmView*)getInstance__Q22cf14CBattleManagerFv())->field_0x216C) != 0) return;
    if (((cf::CfEneBmView*)getInstance__Q22cf14CBattleManagerFv())->field_0x20C8 != 0) return;
    int channel = (int)lbl_eu_80663E44 + (int)lbl_eu_80663E42 * 100;
    s32 key = ((cf::CfEneMoveTgtView*)tgt)->field_0x4B0;
    func_8003AA34();
    u8* bdat = lbl_eu_806640D4;
    int row = (int)func_8003B41C(bdat);
    int endRow = row + (int)func_8003B1EC(bdat);
    for (; row < endRow; row++) {
        u8 model = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x2C3], row);
        if ((s32)model != key) continue;
        u16 ch = (u16)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x2CA], row);
        if ((int)ch != channel) continue;
        u8 reward = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x2CE], row);
        f32 sub = ((cf::CfEneSubVt64*)self)->m8C();
        f32 t = func_80496288(lbl_eu_80663E14);
        f32 now = t * sub + ((cf::CfEneTailView*)self)->field_0x45CC;
        ((cf::CfEneTailView*)self)->field_0x45CC = now;
        cf::CfEneF64Conv cvR;
        cvR.w[0] = 0x43300000;
        cvR.w[1] = (u32)((s32)reward * 30) ^ 0x80000000;
        f32 thresh = (f32)(cvR.d - lbl_eu_806669A8);
        if (now < thresh) return;
        u8 count = (u8)getBdatStringColumnValue(bdat, &lbl_eu_804FC168[0x2D7], row);
        cf::CfEneF64Conv cvN;
        cvN.w[1] = (u32)count;
        cvN.w[0] = 0x43300000;
        f32 n = (f32)(cvN.d - lbl_eu_806669A0);
        f32 f = n * ((cf::CfActorParamVt118*)self)->m12C() * lbl_eu_806669B0;
        if (f != 0.0f) {
            s32 iv = (s32)f;
            cf::CfEneF64Conv cvV;
            cvV.w[0] = 0x43300000;
            cvV.w[1] = (u32)iv ^ 0x80000000;
            f32 rv = (f32)(cvV.d - lbl_eu_806669A8);
            if (rv == 0.0f) rv = lbl_eu_80666980;
            ((cf::CfActorParamVt118*)self)->m11C(-rv);
        }
        ((cf::CfEneTailView*)self)->field_0x45CC = 0.0f;
        return;
    }
}

extern "C" int CfObjectActor_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void* self) { return 1; }

extern "C" void CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv(void) {}

extern "C" void CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void) {}

struct If38 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void vf38();
};
extern "C" void CActorState_UnkVirtualFunc1__Q22cf11CActorStateFv(cf::CActorState* self) {
    reinterpret_cast<If38*>(*(void**)((u8*)self + 4))->vf38();
}

extern "C" void* CBattleState_UnkVirtualFunc28__Q22cf12CBattleStateFv(cf::CBattleState* self, unsigned long index) {
    return ((cf::CBattleState*)self)->unk152C + (index << 4);
}

extern "C" void* CBattleState_UnkVirtualFunc27__Q22cf12CBattleStateFv(cf::CBattleState* self) { return (void*)((u8*)self + 0x152c); }

extern "C" void* CBattleState_UnkVirtualFunc25__Q22cf12CBattleStateFv(cf::CBattleState* self) { return (void*)((u8*)self + 0x8); }

// us-800b0404: retail copies 0x1520 bytes (0x2a4 * 8) from src to this->unk8
// (offset 0x8) with a counted lwzu/stwu loop - full struct assignment of the
// 0x1520-byte block at CBattleState+0x8.
void CBattleState_UnkVirtualFunc24__Q22cf12CBattleStateFv(cf::CBattleState* self, const cf::CBattleState* src) {
    *(cf::CBattleStateUnk1520Block*)self->unk8 = *(const cf::CBattleStateUnk1520Block*)src;
}





extern "C" void CActorParam_UnkVirtualFunc181__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x2A80) = val; }

extern "C" int CActorParam_UnkVirtualFunc178__Q22cf11CActorParamFv(cf::CActorParam* self) { return 0; }

extern "C" void CActorParam_UnkVirtualFunc173__Q22cf11CActorParamFv(void) {}

extern "C" u32 CActorParam_UnkVirtualFunc172__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x3370); }

extern "C" float CActorParam_UnkVirtualFunc171__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1630); }

extern "C" float CActorParam_UnkVirtualFunc170__Q22cf11CActorParamFv(cf::CActorParam* self) {
    extern float lbl_eu_8066A1F8;
    return lbl_eu_8066A1F8 - *(float*)((char*)self + 0x1630);
}

extern "C" float CActorParam_UnkVirtualFunc169__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1630); }

extern "C" void* CActorParam_UnkVirtualFunc164__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x164a); }

extern "C" void* CActorParam_UnkVirtualFunc163__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1648); }

extern "C" u32 CActorParam_UnkVirtualFunc162__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x336C); }

extern "C" void CActorParam_UnkVirtualFunc161__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x336C) = val; }

extern "C" void* CActorParam_UnkVirtualFunc151__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x3358); }

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

extern "C" void* CActorParam_UnkVirtualFunc147__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1928); }

extern "C" u32 CActorParam_UnkVirtualFunc139__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x2A80); }

void cf::CActorParam::CActorParam_UnkVirtualFunc134() {
    *(u8*)((u8*)this + 0x3354) = 0;
}

u8 CActorParam_UnkVirtualFunc133__Q22cf11CActorParamFv(cf::CActorParam* self) {
    // retail: lbz r3, 0x3354(r3) - member block is +8 shifted, raw view pins the offset
    return ((cf::CActorParam3354View*)self)->field_0x3354;
}

extern "C" void* CActorParam_UnkVirtualFunc131__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x31dc); }

extern "C" void* CActorParam_UnkVirtualFunc130__Q22cf11CActorParamFv(cf::CActorParam* self, unsigned long index) { return (u8*)self + index * 0xBC + 0x2A84; }

extern "C" u32 CActorParam_UnkVirtualFunc128__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x15E0); }

extern "C" void* CActorParam_UnkVirtualFunc124__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x2740); }

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
    dst->header0 = src->header0;
    dst->header8 = src->header8;
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

extern "C" void* CActorParam_UnkVirtualFunc121__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x19e8); }

// us-800b07e8: CActorParam::CActorParam_UnkVirtualFunc120 (Fv retail; real
// ABI (self, r4 = arts-set data block)): copies the 0x34-byte arts-set
// header to self+0x19E8, then for each of the 24 0x8C-byte CArtsParam
// records at self+0x1A20 stores strlen at +0x20, strcpy's the name and
// copies the +0x24..+0x80 fields plus the +0x88 word (the +0x84 vtable
// word is skipped).
void CActorParam_UnkVirtualFunc120__Q22cf11CActorParamFv(cf::CActorParam* self,
                                                         const cf::CfEneArtsData* src) {
    cf::CfEneArtsArea* dst = (cf::CfEneArtsArea*)self;
    const cf::CfEneArtsEntry* s = &src->records[0];
    cf::CfEneArtsEntry* d = &dst->records[0];
    cf::CfEneArtsEntry* end = &dst->records[24];
    dst->header = src->header;
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

extern "C" void CActorParam_UnkVirtualFunc105__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x15fc) = val; }

extern "C" void* CActorParam_UnkVirtualFunc104__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x15fc); }

extern float lbl_eu_80666968;
void cf::CActorParam::CActorParam_UnkVirtualFunc103() {
    *(f32*)((u8*)this + 0x15FC) = lbl_eu_80666968;
}

extern "C" void* CActorParam_UnkVirtualFunc102__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x18d4); }

extern "C" void* CActorParam_UnkVirtualFunc101__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x185c); }

extern "C" void* CActorParam_UnkVirtualFunc99__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x17e4); }

extern "C" void* CActorParam_UnkVirtualFunc97__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1792); }

extern "C" void* CActorParam_UnkVirtualFunc96__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1740); }

extern "C" void* CActorParam_UnkVirtualFunc95__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x16c8); }

extern "C" void* CActorParam_UnkVirtualFunc93__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1650); }

void cf::CActorParam::CActorParam_UnkVirtualFunc92() {}

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

extern "C" u32 CActorParam_UnkVirtualFunc84__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x1600) + *(u32*)((u8*)self + 0x1604); }

extern "C" void* CActorParam_UnkVirtualFunc80__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1834); }

extern "C" void* CActorParam_UnkVirtualFunc79__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x182c); }

extern "C" void CActorParam_UnkVirtualFunc78__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x1834) = val; }

extern "C" void CActorParam_UnkVirtualFunc77__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x182c) = val; }

extern "C" void* CActorParam_UnkVirtualFunc75__Q22cf11CActorParamFv(cf::CActorParam* self) { return (void*)((u8*)self + 0x1828); }

extern "C" void CActorParam_UnkVirtualFunc73__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x1828) = val; }

extern "C" float CActorParam_UnkVirtualFunc72__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1824); }

extern "C" void CActorParam_UnkVirtualFunc71__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x1824) = val; }

extern "C" float CActorParam_UnkVirtualFunc69__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1618); }

extern "C" float CActorParam_UnkVirtualFunc66__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1610); }

extern "C" s16 CActorParam_UnkVirtualFunc63__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(s16*)((u8*)self + 0x1616); }

extern "C" s16 CActorParam_UnkVirtualFunc62__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(s16*)((u8*)self + 0x1614); }

extern "C" void CActorParam_UnkVirtualFunc59__Q22cf11CActorParamFv(cf::CActorParam* self, u16 val) { *(u16*)((u8*)self + 0x1614) = val; }

extern "C" s16 CActorParam_UnkVirtualFunc57__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(s16*)((u8*)self + 0x160e); }

extern "C" s16 CActorParam_UnkVirtualFunc56__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(s16*)((u8*)self + 0x160c); }

extern "C" void CActorParam_UnkVirtualFunc53__Q22cf11CActorParamFv(cf::CActorParam* self, u16 val) { *(u16*)((u8*)self + 0x160C) = val; }

extern "C" float CActorParam_UnkVirtualFunc51__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17F0) / *(float*)((u8*)self + 0x17FC); }

extern "C" float CActorParam_UnkVirtualFunc50__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17fc); }

extern "C" float CActorParam_UnkVirtualFunc49__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17f0); }

extern "C" void CActorParam_UnkVirtualFunc47__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x17f0) = val; }

extern "C" float CActorParam_UnkVirtualFunc45__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17EC) / *(float*)((u8*)self + 0x17F8); }

extern "C" float CActorParam_UnkVirtualFunc44__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17f8); }

extern "C" float CActorParam_UnkVirtualFunc43__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17ec); }

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

extern "C" void CActorParam_UnkVirtualFunc41__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x17ec) = val; }

extern "C" float CActorParam_UnkVirtualFunc39__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x17E8) / *(float*)((u8*)self + 0x17F4); }

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

extern "C" float CActorParam_UnkVirtualFunc32__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x1660); }

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

extern "C" void CActorParam_UnkVirtualFunc30__Q22cf11CActorParamFv(cf::CActorParam* self, float val) { *(float*)((u8*)self + 0x1660) = val; }

extern "C" void CActorParam_UnkVirtualFunc28__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x17E4) += val; }

extern "C" void CActorParam_UnkVirtualFunc27__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x17E4) = val; }

extern "C" void CActorParam_UnkVirtualFunc25__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x1650) += val; }

extern "C" void CActorParam_UnkVirtualFunc24__Q22cf11CActorParamFv(cf::CActorParam* self, u32 val) { *(u32*)((u8*)self + 0x1650) = val; }

extern "C" float CActorParam_UnkVirtualFunc17__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(float*)((u8*)self + 0x15f8); }

u8 CActorParam_UnkVirtualFunc15__Q22cf11CActorParamFv(cf::CActorParam* self) {
    // retail: lbz r3, 0x15f4(r3) - the declared unk15F4[4] member is +8
    // shifted (lands at 0x15FC), raw view pins the retail offset
    return ((cf::CfObjectEne15F4View*)self)->field_0x15F4;
}

extern "C" u32 CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return (*(u32*)((u8*)self + 104) >> 21) & 0x1u; }

extern "C" u32 CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return *(u32*)((u8*)self + 0xB4); }

extern "C" float CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return *(float*)((u8*)self + 0xac); }

extern "C" float CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv(cf::CfObjectModel* self) { return *(float*)((u8*)self + 0xa8); }

extern "C" u32 CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) { return *(u32*)((u8*)self + 0x704); }

extern "C" u32 CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) { return *(u32*)((u8*)self + 0x708); }

extern "C" void CfObjectActor_UnkVirtualFunc13__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) {
    *(long*)((char*)self + 0x45bc) = -1;
}

extern "C" u32 CfObjectActor_UnkVirtualFunc12__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { return *(u32*)((u8*)self + 0x45BC); }

extern "C" void CBattleState_UnkVirtualFunc18__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CBattleState_UnkVirtualFunc17__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CBattleState_UnkVirtualFunc3__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CBattleState_UnkVirtualFunc1__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CBattleState_UnkVirtualFunc2__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CObjectParam_UnkVirtualFunc4__Q22cf11CfObjectEneFv(cf::CfObjectEne* self) { ((void(*)(void*))func_800ADDA8__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

extern "C" void CfObject_UnkVirtualFunc31__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc6__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

extern "C" void CfObjectMove_UnkVirtualFunc15__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc2__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }


extern "C" void CObjectParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

extern "C" void CfObjectMove_UnkVirtualFunc6__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc11__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

extern "C" void CfObject_UnkVirtualFunc14__Q22cf13CfObjectActorFf(cf::CfObjectActor* self, float value) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc10__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

extern "C" void CfObject_UnkVirtualFunc2__Q22cf11CfObjectEneFv(cf::CfObjectEne* self) { ((void(*)(void*))func_800ADB2C__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

extern "C" void CfObject_UnkVirtualFunc4__Q22cf11CfObjectEneFv(cf::CfObjectEne* self) { ((void(*)(void*))func_800ADBD4__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

extern "C" void func_800B069C__Q22cf11CfObjectEneFv(cf::CfObjectEne* self) { ((void(*)(void*))__dt__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

extern "C" void CBattleState_UnkVirtualFunc23__Q22cf12CBattleStateFv(cf::CBattleState* self) { reinterpret_cast<BSIf*>(self)->vf0044(); }

extern "C" void CBattleState_UnkVirtualFunc22__Q22cf12CBattleStateFv(cf::CBattleState* self) { reinterpret_cast<BSIf*>(self)->vf0040(); }

extern "C" void CBattleState_UnkVirtualFunc21__Q22cf12CBattleStateFv(cf::CBattleState* self) { reinterpret_cast<BSIf*>(self)->vf003C(); }

extern "C" void CBattleState_UnkVirtualFunc20__Q22cf12CBattleStateFv(cf::CBattleState* self) { reinterpret_cast<BSIf*>(self)->vf0038(); }

extern "C" void CActorParam_UnkVirtualFunc98__Q22cf11CActorParamFv(cf::CActorParam* self, const void* src) {
    *(ParamCopyBlock*)((u8*)self + 0x17e4) = *(const ParamCopyBlock*)src;
}

extern "C" void CActorParam_UnkVirtualFunc92__Q22cf11CActorParamFv(cf::CActorParam* self, const void* src) {
    *(ParamCopyBlock*)((u8*)self + 0x1650) = *(const ParamCopyBlock*)src;
}
