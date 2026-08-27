// Auto-scaffolded catalog TU for monolib/src/scn/CScn_80496B0C
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/scn/CScnTimeApi.hpp"
#include <string.h>

#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "nw4r/g3d/g3d_anmchr.h"
#include "nw4r/db/db_assert.h"
#include "monolib/math/CVec3.hpp"
#include <math.h>

// CScnItemAnim is only ever a forward pointer here (host scene object); the
// full header also declares the chr-anm panic strings const, which conflicts
// with the non-const .data definitions below, so it is not pulled in.
// Host scene item-anim object (TU-local layout view): +0x04 is the scene.
// Host scene item-anim object (TU-local layout view): +0x04 is the scene.
struct CScn;
struct CScnItemAnim {
    char field_0x0[4];
    CScn* scn;
};

// Default value returned by the accessor helpers when the target object
// chain is missing (sda2 constant).
extern "C" { extern const float lbl_eu_8066AAC0; }

// 0x4330000000000000 magic double used for unsigned -> float conversion.
extern const f64 lbl_eu_8066AAC8;
// 0x4330000080000000 magic double used for signed -> float conversion.
extern const f64 lbl_eu_8066AAD0;

// Caller-owned punning slot for int -> float conversions (see
// code_804CC2B8.cpp fconvS32).
typedef union {
    f64 d;
    u32 w[2];
} FConv;

// u32 -> float through the shared 0x4330000000000000 magic double. Keeping
// the subtrahend in a parameter pins the sdata2 reloc to the retail
// constant name (a plain (f32) cast would emit the TU's own pool entry).
// Integer -> float through the shared 0x4330 magic doubles. Keeping the
// integer as a parameter pins the whole sequence to one expression (retail
// schedules it as a unit); returning f64 lets the stfs do the narrowing.
static inline f64 fconvS(u32 val, FConv& c, f64 sub) {
    c.w[0] = 0x43300000u;
    c.w[1] = val;
    return c.d - sub;
}
extern "C" { extern const float lbl_eu_8066AAC4; }
extern "C" { extern const float lbl_eu_8066AAD8; }
extern "C" { extern const float lbl_eu_8066AADC; }

// Panic file/format strings used by the ResMdl node-binding assert.
extern const char lbl_eu_8056E1C8[];
extern const char lbl_eu_8056E1A8[];

// Panic strings for the node-name / anm-resource asserts (retail linker
// names; global-scope names are not mangled by MWCC). The sdata2 ones are
// sized so MWCC emits sda21 addressing, matching the retail call sites.
extern const char lbl_eu_8056E194[];   // file (node-name assert, line 0x2c)
extern const char lbl_eu_8056E178[];   // fmt
extern const char lbl_eu_80663910[8];  // arg (sda2 string)
extern const char lbl_eu_806639E0[4];  // arg (sda2 string)
extern const char lbl_eu_806639E4[4];  // arg (sda2 string)
extern const char lbl_eu_80530F08[];   // file (ResDic alignment assert, line 0x54)
extern const char lbl_eu_80530EE0[];   // fmt

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056E8B8-0x8056E9E8 (0x130): CVirtualLightObj / CScn / CTTask<CScn>
// vtables followed by the chr-anm panic strings. Post blob-split the retail
// slice carries REAL relocations against named targets (all defined in
// sibling TUs: CVirtualLightObj.s / CScn.s / foreign libs), so every pointer
// word below is an explicit &reloc; only the RTTI locators, the base-list
// offset word (0xFFFFFFAC) and the CTTask<CScn> slots stay literal. Those
// three slots carry unspellable template names (<...> cannot appear in a
// C++ identifier) - they stay literal-zero words and are wired to their
// retail symbols via UNIT_RULES inject_relocs (CDevice.o pattern). The
// panic strings are defined here (non-const -> .data) with align(4) so
// MWCC reproduces the retail inter-string gap words; the 5-byte section
// tail is restored by pad_data_section.
extern "C" void __dt__16CVirtualLightObjFv();
extern "C" void func_804947EC();
extern "C" void func_8049488C();
extern "C" void __dt__4CScnFv();
extern "C" void Reset__14CChildListNodeFv();
extern "C" void Init__4CScnFv();
extern "C" void Term__4CScnFv();
extern "C" void Move__4CScnFv();
extern "C" void Draw__4CScnFv();
extern "C" void Tail__8CProcessFv();
extern "C" void func_80496B04();
extern "C" void func_80496970();
extern "C" void func_8049695C();
extern "C" int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
extern "C" int OnFileEvent__10IWorkEventFP10CEventFile(void*);
extern "C" int WorkEvent3__4CScnFPv(void*);
extern "C" int WorkEvent4__4CScnFv();
extern "C" void OnPauseTrigger__10IWorkEventFb(int);
extern "C" int WorkEvent6__10IWorkEventFv();
extern "C" int WorkEvent7__10IWorkEventFv();
extern "C" int WorkEvent8__10IWorkEventFv();
extern "C" int WorkEvent9__10IWorkEventFv();
extern "C" int WorkEvent10__10IWorkEventFv();
extern "C" int WorkEvent11__10IWorkEventFv();
extern "C" int WorkEvent12__10IWorkEventFv();
extern "C" int WorkEvent13__10IWorkEventFv();
extern "C" int WorkEvent14__10IWorkEventFv();
extern "C" int WorkEvent15__10IWorkEventFv();
extern "C" int WorkEvent16__10IWorkEventFv();
extern "C" int WorkEvent17__10IWorkEventFv();
extern "C" int WorkEvent18__10IWorkEventFv();
extern "C" int WorkEvent19__10IWorkEventFv();
extern "C" int WorkEvent20__10IWorkEventFv();
extern "C" int WorkEvent21__10IWorkEventFv();
extern "C" int WorkEvent22__10IWorkEventFv();
extern "C" int WorkEvent23__10IWorkEventFv();
extern "C" int WorkEvent24__10IWorkEventFv();
extern "C" int WorkEvent25__10IWorkEventFv();
extern "C" int WorkEvent26__10IWorkEventFv();
extern "C" int WorkEvent27__10IWorkEventFv();
extern "C" int WorkEvent28__10IWorkEventFv();
extern "C" int WorkEvent29__10IWorkEventFv();
extern "C" int WorkEvent30__10IWorkEventFv();
extern "C" int WorkEvent31__10IWorkEventFv();
extern "C" u32 lbl_eu_806639D0[];
extern "C" u32 lbl_eu_80663988[];
extern "C" u32 lbl_eu_80663990[];

extern "C" u32 lbl_eu_8056E8B8[6] = {
    (u32)&lbl_eu_806639D0, 0x00000000,
    (u32)&__dt__16CVirtualLightObjFv,
    (u32)&func_804947EC,
    (u32)&func_8049488C,
    0x00000000,
};
extern "C" u32 lbl_eu_8056E8D0[45] = {
    (u32)&lbl_eu_80663988, 0x00000000,
    (u32)&__dt__4CScnFv,
    (u32)&Reset__14CChildListNodeFv,
    (u32)&Init__4CScnFv,
    (u32)&Term__4CScnFv,
    (u32)&Move__4CScnFv,
    (u32)&Draw__4CScnFv,
    (u32)&Tail__8CProcessFv,
    (u32)&lbl_eu_80663988, 0xFFFFFFAC,
    (u32)&func_80496B04,
    (u32)&WorkEvent1__10IWorkEventFPvPCc,
    (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__4CScnFPv,
    (u32)&WorkEvent4__4CScnFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv,
    (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv,
    (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv,
    (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv,
    (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv,
    (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv,
    (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv,
    (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv,
    (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv,
    (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv,
    (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv,
    (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv,
    (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv,
    (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&func_80496970,
    (u32)&func_8049695C,
};
extern "C" u32 lbl_eu_8056E984[9] = {
    (u32)&lbl_eu_80663990, 0x00000000,
    0x00000000,                      // -> __dt__13CTask<4CScn>Fv (inject_relocs)
    (u32)&Reset__14CChildListNodeFv,
    0x00000000,
    0x00000000,
    0x00000000,                      // -> Move__13CTask<4CScn>Fv (inject_relocs)
    0x00000000,                      // -> Draw__13CTask<4CScn>Fv (inject_relocs)
    (u32)&Tail__8CProcessFv,
};
// ASCII panic-string tail, owned by THIS .data slice post blob-split.
// Non-const so they land in .data; __declspec(align(4)) reproduces the
// 2-byte linker gaps between them (retail layout). Retail loads these via
// lis/addi @ha/@l at the Panic call sites, which is exactly what MWCC
// emits for locally-defined .data objects.
extern "C" __declspec(align(4)) char lbl_eu_8056E9A8[0xA] = "ResAnmChr";
extern "C" __declspec(align(4)) char lbl_eu_8056E9B4[0x1A] = "%s::%s: Object not valid.";
extern "C" __declspec(align(4)) char lbl_eu_8056E9D0[0x13] = "g3d_resanmchr_ac.h";

// Default-constructible stand-in with nw4r::g3d::ResCommon's exact layout
// (single data pointer). Used so a ResDic home can be declared without an
// initializer store while still occupying the first frame slot.
struct ResDicSlot {
    void* mpData;
};

// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).
struct CScnModel80496B0C;
struct VTarget {
    virtual void v0() = 0;
    virtual void v1(CScnModel80496B0C* model, u32 id, u32 flag) = 0; // +0x0c
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual f32 v6() = 0;     // vtable + 0x20
    virtual void v7() = 0;
    virtual void v8(f32 frame) = 0; // +0x28 (set frame)
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;   // vtable + 0x34 (AnmObjChr::Release())
    virtual void v12(void* pResult, u32 arg) = 0; // vtable + 0x38 (GetResult?)
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16(f32 weight, int idx) = 0; // +0x48 (blend weight)
    virtual void v17() = 0;
    virtual void v18(nw4r::g3d::ResMdl* mdl, u32 target, u32 option) =
        0;                    // vtable + 0x50
    virtual void v19(nw4r::g3d::ResMdl* mdl, u32 target, u32 option) =
        0;                    // vtable + 0x54 (AnmObjChr::Release(mdl,target,opt))
    virtual ~VTarget() {}

    /* +0x04 */ u8 pad_0x04[0x20 - 0x04];
    /* +0x20 */ u16 field_0x20;
    /* +0x22 */ u16 pad_0x22;
    /* +0x24 */ u32 field_0x24;
    /* +0x28 */ u32 pad_0x28;
    /* +0x2C */ nw4r::g3d::ResAnmChrData* field_0x2C;
};

// Root object the free accessors below operate on: two child pointers.
struct CScnChild80496B0C;
struct CScnNode80496B0C;
struct CScn80496B0C;
class CScn;

// Animation object stored in CScn80496B0C::field_0x1D4: only vtable slot
// +0x38 (index 12) is dispatched from this TU, with the blend object as its
// sole argument (the retail call site passes one argument).
struct CScnAnmChr80496B0C {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12(void* pTarget) = 0;   // vtable + 0x38
    virtual ~CScnAnmChr80496B0C() {}
};

// Model/owner object passed to func_80496F14: ResMdl data pointer at +0x146C
// and the animation object at +0x147C.
struct CScnModel80496B0C {
    char field_0x0[0x7A4];                 // +0x00
    u32 field_0x7A4;                       // +0x7A4 state flags
    char field_0x7A8[0x146C - 0x7A8];      // +0x7A8
    nw4r::g3d::ResMdlData* field_0x146C;   // +0x146C
    char field_0x1470[0x147C - 0x1470];    // +0x1470
    CScnAnmChr80496B0C* field_0x147C;      // +0x147C
};

// Leaf node reached via child->field_0x84: one chr-animation binding record.
struct CScnNode80496B0C {
    s16 field_0x0;                        // +0x00 bind index
    u16 field_0x2;                        // +0x02 flags
    f32 field_0x4;                        // +0x04
    f32 field_0x8;                        // +0x08
    f32 field_0xC;                        // +0x0C normalized weight
    VTarget* field_0x10;                  // +0x10 animation object
    u32 field_0x14;                       // +0x14 id
    CScnItemAnim* field_0x18;             // +0x18 owning item anim
    f32 field_0x1C;                       // +0x1C
    f32 field_0x20;                       // +0x20 anm scale factor
    f32 field_0x24;                       // +0x24
    u16 field_0x28;                       // +0x28 counter stamp
};

// Child object: node pointers at +0x84 / +0x88 / +0x8C plus anim/scale state.
struct CScnChild80496B0C {
    CScnNode80496B0C elems[3];            // +0x00 binding records
    CScnNode80496B0C* field_0x84[3];      // +0x84 (node-binding slots)
    s16 field_0x90;                       // +0x90
    s16 field_0x92;                       // +0x92
    s16 field_0x94;                       // +0x94
    s16 field_0x96;                       // +0x96
    nw4r::g3d::AnmObjChrBlend* field_0x98; // +0x98
    ml::CVec3 field_0x9C;                 // +0x9C
    nw4r::g3d::ResMdlData* field_0xA8;    // +0xA8
    CScn80496B0C* field_0xAC;             // +0xAC
};


// Root object: two embedded child objects at +0x10/+0xC0 whose addresses are
// cached in field_0x8/field_0xC, plus the ResMdl / animation handles.
struct CScn80496B0C {
    CScn* field_0x0;                      // +0x00
    CScnModel80496B0C* field_0x4;         // +0x04
    CScnChild80496B0C* field_0x8;         // +0x08
    CScnChild80496B0C* field_0xC;         // +0x0C
    CScnChild80496B0C field_0x10;         // +0x10
    CScnChild80496B0C field_0xC0;         // +0xC0
    f32 field_0x170;                      // +0x170
    f32 field_0x174;                      // +0x174
    f32 field_0x178;                      // +0x178
    u16 field_0x17C;                      // +0x17C
    u16 field_0x17E;                      // +0x17E
    char field_0x180[0x1CC - 0x180];      // +0x180
    nw4r::g3d::AnmObjChrBlend* field_0x1CC; // +0x1CC
    nw4r::g3d::ResMdl field_0x1D0;        // +0x1D0
    CScnAnmChr80496B0C* field_0x1D4;      // +0x1D4
    s32 field_0x1D8;                      // +0x1D8
    VTarget* field_0x1DC[4];              // +0x1DC model listener slots
};

// In-unit helpers (bodies recovered later; noinline keeps the call sites as
// real `bl` relocs).
__declspec(noinline) void func_80498A5C(CScnChild80496B0C* child,
                                        ml::CVec3* outDelta, ml::CVec3* vecB,
                                        ml::CVec3* vecC, u32 arg);
u32 func_804972E8(CScn80496B0C* self, CScnItemAnim* anim, int index,
                  unsigned int frame, u32 g);

extern "C" void func_804986F8(CScnChild80496B0C* self, f32 rate);
// Scene lookup used by the blend-weight updates (retail free function).
// Character-animation setup helper (retail keeps an unmangled linker name,
// so it is declared and defined with C linkage).
extern "C" __declspec(noinline) u32 func_80497544(
    CScn80496B0C* self, CScnChild80496B0C* child, CScnItemAnim* anim,
    nw4r::g3d::ResAnmChr* anmRes, u32 c, u32 e, int f);

// Imports from other monolib TUs (retail free functions). C linkage keeps the
// retail reloc names verbatim (MWCC would otherwise mangle them).
extern "C" void* func_8048ECFC(CScn* scn);
extern "C" __declspec(noinline) void func_804980E4(CScnChild80496B0C* child, u32 tag);

// Character-animation resource lookup by index (retail free function in
// CScnItemAnim.cpp; C linkage keeps the plain global reloc name).
extern "C" nw4r::g3d::ResAnmChr func_8049E708(CScnItemAnim* self, int index);

// nw4r AnmObjChrBlend::Construct is a private static in g3d_anmchr.h, so it
// is re-declared under its retail mangled linker name (extern "C" keeps the
// identifier verbatim).
extern "C" nw4r::g3d::AnmObjChrBlend*
Construct__Q34nw4r3g3d14AnmObjChrBlendFP12MEMAllocatorPUlQ34nw4r3g3d6ResMdli(
    MEMAllocator* pAllocator, u32* pSize, nw4r::g3d::ResMdl mdl,
    int numChildren);

void func_8049771C(u8* self, float val) { *(float*)((u8*)self + 0x178) = val; }


// Returns the virtual value at vtable offset 0x20 of the object at
// self->field_0x8->field_0x84->field_0x10, or the default constant when
// that chain is missing.
f32 func_80497724(CScn80496B0C* self) {
    CScnChild80496B0C* child = self->field_0x8;
    if (child == 0) return lbl_eu_8066AAC0;
    CScnNode80496B0C* node = child->field_0x84[0];
    if (node != 0) return node->field_0x10->v6();
    return lbl_eu_8066AAC0;
}

// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).
// (type definitions live at the top of the file)

extern "C" void func_80497760(u8* self) {
    void* p = *(void**)((u8*)self + 8);
    if (p == 0) return;
    p = *(void**)((u8*)p + 0x84);
    if (p == 0) return;
    ((VTarget*)*(void**)((u8*)p + 0x10))->v5();
}

extern "C" float func_80497790(u8* self) {
    void* p = *(void**)((u8*)self + 8);
    if (p == 0) return lbl_eu_8066AAC0;
    p = *(void**)((u8*)p + 0x84);
    if (p != 0) return *(float*)((u8*)p + 0x1C);
    return lbl_eu_8066AAC0;
}

extern "C" float func_804977C0(u8* self) {
    void* p = *(void**)((u8*)self + 0xC);
    if (p == 0) return lbl_eu_8066AAC0;
    p = *(void**)((u8*)p + 0x84);
    if (p != 0) return *(float*)((u8*)p + 0x1C);
    return lbl_eu_8066AAC0;
}

// Same as func_80497724 but walks the second child (self->field_0xC).
f32 func_804977F0(CScn80496B0C* self) {
    CScnChild80496B0C* child = self->field_0xC;
    if (child == 0) return lbl_eu_8066AAC0;
    CScnNode80496B0C* node = child->field_0x84[0];
    if (node != 0) return node->field_0x10->v6();
    return lbl_eu_8066AAC0;
}

// Refreshes both child chains, then, when the first chain is unbound and an
// animation object is present, resets the pending-blend flag and asks the
// animation object to pick up the blend (virtual, vtable +0x38).
void func_8049782C(CScn80496B0C* self, u32 tag) {
    if (self->field_0x8 != 0) {
        func_804980E4(self->field_0x8, tag);
        func_804980E4(self->field_0xC, tag);
        if (self->field_0x1D4 != 0 && self->field_0x8->field_0x84[0] == 0) {
            self->field_0x17E = 0;
            self->field_0x1D4->v12(self->field_0x1CC);
        }
    }
}

extern "C" u32 func_804978B8(u8* self) { return *(u32*)(*(u32*)((u8*)self + 8) + 0x84) != 0; }

// Returns whether the +0x14 ids of the two chains' nodes differ, or 0 when
// either chain is missing.
u32 func_804978D0(CScn80496B0C* self) {
    // Sign bit of (na_id - nb_id) OR'd both ways: set iff the two ids differ.
    CScnChild80496B0C* a = self->field_0x8;
    CScnChild80496B0C* b = self->field_0xC;
    CScnNode80496B0C* na;
    CScnNode80496B0C* nb;
    if ((na = a->field_0x84[0]) == 0 || (nb = b->field_0x84[0]) == 0)
        return 0;
    return ((na->field_0x14 - nb->field_0x14) |
            (nb->field_0x14 - na->field_0x14)) >> 31;
}

// Returns whether both chains are complete (nodes present, no +0x88 child),
// the two nodes share a +0x18 tag, and their +0x14 ids match.
u32 func_80497914(CScn80496B0C* self) {
    CScnChild80496B0C* a = self->field_0x8;
    CScnChild80496B0C* b = self->field_0xC;
    CScnNode80496B0C* na;
    CScnNode80496B0C* nb;
    if ((na = a->field_0x84[0]) == 0 || (nb = b->field_0x84[0]) == 0) return 0;
    if (a->field_0x84[1] != 0 || b->field_0x84[1] != 0) return 0;
    if (nb->field_0x18 != na->field_0x18) return 0;
    // Equality compiled by MWCC as subf/cntlzw/srwi.
    return !(nb->field_0x14 - na->field_0x14);
}

extern "C" u32 func_8049798C(u8* self) { return *(u32*)(*(u32*)((u8*)self + 0xC) + 0x84) != 0; }

// Refreshes the child's TRS outputs from the model node selected by nodeIdx:
// the translate output receives the node's position (or zero when the node is
// absent), and the other two outputs are zeroed. When a node is already bound,
// the child is released instead and the outputs are left untouched.
void func_804979A4(CScn80496B0C* self, ml::CVec3* outA, ml::CVec3* outB,
                   ml::CVec3* outC, u32 nodeIdx) {
    // Volatile view of field_0x8: forces the second read (below the calls)
    // to be a fresh load, matching retail.
    CScnChild80496B0C* volatile& childRef =
        (CScnChild80496B0C* volatile&)self->field_0x8;
    if (childRef->field_0x84[0] == 0) {
        nw4r::g3d::ResNode node = self->field_0x1D0.GetResNode(nodeIdx);
        if (node.IsValid()) {
            // The inner check mirrors the retail DB_ASSERT on the same validity
            // test: MWCC reuses the outer compare and leaves the Panic block as
            // unreachable fall-through.
            if (!node.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639E0);
            }
            const nw4r::math::VEC3& gv = node.GetTranslate();
            // Direct CVec3 view of the resource vector: MWCC buffers the
            // float source through a single stack temp and word-copies it
            // to the destination (retail's exact shape).
            *outC = *(ml::CVec3*)&gv;
        } else {
            outC->set(lbl_eu_8066AAC0, lbl_eu_8066AAC0, lbl_eu_8066AAC0);
        }
        outA->set(lbl_eu_8066AAC0, lbl_eu_8066AAC0, lbl_eu_8066AAC0);
        outB->set(lbl_eu_8066AAC0, lbl_eu_8066AAC0, lbl_eu_8066AAC0);
        return;
    }
    // Retail re-reads field_0x8 here instead of reusing the earlier load.
    func_80498A5C(childRef, outA, outB, outC, nodeIdx);
}

// Per-frame cross-fade update: advances both children's animations toward the
// shared frame counter, then when the two heads belong to the same item anim
// releases the fading child completely and re-arms the model release.
void func_80497AA8(CScn80496B0C* self) {
    // No scene local: MWCC keeps the first load in a volatile reg and
    // rematerializes self->field_0x0 after intervening calls.
    if (self->field_0x0 == NULL) {
        return;
    }
    if (self->field_0x8->field_0x84[0] == NULL) {
        return;
    }
    FConv conv;
    if (self->field_0x4->field_0x7A4 & 0x2000) {
        // Quantized mode: round the scaled frame to an integer and feed the
        // same quantized frame to every bound animation object. The int->float
        // narrowing goes through the shared 0x4330 magic double (retail
        // rematerializes the whole conversion per loop).
        s32 frames =
            (s32)(self->field_0x178 * func_80496288(self->field_0x0) +
                  lbl_eu_8066AAD8);
        CScnChild80496B0C* walk = self->field_0x8;
        for (u32 i = 0; i < 3; i++, walk = (CScnChild80496B0C*)((u8*)walk + 4)) {
            CScnNode80496B0C* n = walk->field_0x84[0];
            if (n == NULL) {
                continue;
            }
            n->field_0x10->v8((f32)fconvS(
                (u32)frames ^ 0x80000000u, conv, lbl_eu_8066AAD0));
        }
        walk = self->field_0xC;
        for (u32 i = 0; i < 3; i++, walk = (CScnChild80496B0C*)((u8*)walk + 4)) {
            CScnNode80496B0C* n = walk->field_0x84[0];
            if (n == NULL) {
                continue;
            }
            n->field_0x10->v8((f32)fconvS(
                (u32)frames ^ 0x80000000u, conv, lbl_eu_8066AAD0));
        }
    } else {
        f32 s = self->field_0x178 * func_80496288(self->field_0x0);
        CScnChild80496B0C* walk = self->field_0x8;
        for (u32 i = 0; i < 3; i++, walk = (CScnChild80496B0C*)((u8*)walk + 4)) {
            CScnNode80496B0C* n = walk->field_0x84[0];
            if (n == NULL) {
                continue;
            }
            n->field_0x10->v8(s);
        }
        // Retail re-reads the scene pointer here (volatile calls clobbered
        // the cached register).
        s = self->field_0x178 * func_80496288(self->field_0x0);
        walk = self->field_0xC;
        for (u32 i = 0; i < 3; i++, walk = (CScnChild80496B0C*)((u8*)walk + 4)) {
            CScnNode80496B0C* n = walk->field_0x84[0];
            if (n == NULL) {
                continue;
            }
            n->field_0x10->v8(s);
        }
    }

    // Advance the fade weight on the second child. The fading child stays
    // cached in a callee-saved register from here to the end of the update.
    CScnChild80496B0C* fade = self->field_0xC;
    f32 weight = lbl_eu_8066AAC4;
    if (fade->field_0x84[0] != NULL) {
        // No accumulator local: retail re-reads field_0x174 around the rate
        // call, keeping the running sum only in volatile registers. prev is
        // also read after the call so nothing floats across it.
        self->field_0x174 =
            self->field_0x174 +
            self->field_0x178 * func_80496288(self->field_0x0);
        f32 prev = self->field_0x170;
        if (prev > self->field_0x174) {
            // Fade finished backwards: ease the weight toward the floor.
            weight = lbl_eu_8066AAD8 +
                     lbl_eu_8066AAD8 *
                         ((prev - self->field_0x174) / prev);
        } else {
            // Fade progressing normally: drop to half, and when the fade has
            // regressed past the previous mark, release the head binding.
            weight = lbl_eu_8066AAD8;
            if (prev > self->field_0x174 -
                           self->field_0x178 *
                               func_80496288(self->field_0x0)) {
                int nodeId = self->field_0x1D8;
                if (((u32)nodeId + 0x10000) != 0xFFFF) {
                    // Countdown scan over child1's slots; first bound slot
                    // wins. Compiled as mtctr/bdnz.
                    CScnChild80496B0C* wk = self->field_0x8;
                    for (int k = 3; k != 0; k--) {
                        if (wk->field_0x84[0] != NULL) {
                            nw4r::g3d::ResMdl mdl(self->field_0x8->field_0xA8);
                            wk->field_0x84[0]->field_0x10->v19(&mdl, nodeId, 1);
                            break;
                        }
                        wk = (CScnChild80496B0C*)((u8*)wk + 4);
                    }
                }
            }
        }
    }

    func_804986F8(self->field_0x8, weight);

    // When both heads reference the same item anim, retire the fading child.
    CScnNode80496B0C* h2 = fade->field_0x84[0];
    if (h2 != NULL && (h2->field_0x2 & 8)) {
        return;
    }
    bool same = false;
    if (h2 != NULL) {
        CScnNode80496B0C* h1 = self->field_0x8->field_0x84[0];
        if (h1 != NULL && fade->field_0x84[1] == NULL &&
            self->field_0x8->field_0x84[1] == NULL &&
            h1->field_0x18 == h2->field_0x18) {
            // Equality compiled by MWCC as subf/cntlzw/srwi.
            same = !(h2->field_0x14 - h1->field_0x14);
        }
    }
    if (!same) {
        return;
    }

    // Retire every binding on the fading child (walking-pointer scan).
    CScnChild80496B0C* walk2 = fade;
    u32 i = 0;
    while (i < 3) {
        CScnNode80496B0C* node = walk2->field_0x84[0];
        if (node != NULL) {
            if (node->field_0x10 != NULL) {
                fade->field_0x98->Detach(fade->field_0x90 * 3 + node->field_0x0);
                fade->field_0x92--;
                node->field_0x10->v11();
                ((nw4r::g3d::G3dObj*)node->field_0x10)->Destroy();
                node->field_0x10 = NULL;
            }
            node->field_0x18 = NULL;
            walk2->field_0x84[0] = NULL;
            fade->field_0x9C = ml::CVec3::zero;
        }
        i++;
        walk2 = (CScnChild80496B0C*)((u8*)walk2 + 4);
    }

    // Re-arm the model-node release on the surviving child's head binding.
    if (((u32)self->field_0x1D8 + 0x10000) != 0xFFFF) {
        int nodeId = self->field_0x1D8;
        CScnChild80496B0C* wk = self->field_0x8;
        for (int k = 3; k != 0; k--) {
            if (wk->field_0x84[0] != NULL) {
                nw4r::g3d::ResMdl mdl(self->field_0x8->field_0xA8);
                wk->field_0x84[0]->field_0x10->v18(&mdl, nodeId, 0);
                break;
            }
            wk = (CScnChild80496B0C*)((u8*)wk + 4);
        }
    }
    func_804986F8(fade, lbl_eu_8066AAC4 - weight);
}

// Child-object initializer: zeroes the three node-binding slots and the
// anim/scale state, then stamps the per-slot ids 0/1/2. C linkage keeps the
// retail reloc name verbatim (the ctor call sites use the unmangled name).
extern "C" CScnChild80496B0C* func_80497F34(CScnChild80496B0C* self) {
    // Pointer-walk do-while with the increment inside the condition: the
    // SDK loop shape MWCC emits without unrolling (MWCC_CASES).
    CScnNode80496B0C* it = self->elems;
    do {
        it->field_0x2 = 0;
        it->field_0x14 = 0;
    } while (++it < &self->elems[3]);

    self->field_0x90 = 0;
    self->field_0x92 = 0;
    self->field_0x94 = 0;
    self->field_0x96 = 0;
    self->field_0x98 = 0;
    self->field_0x9C.x = lbl_eu_8066AAC0;
    self->field_0x9C.y = lbl_eu_8066AAC0;
    self->field_0x9C.z = lbl_eu_8066AAC0;
    self->field_0xA8 = 0;
    self->field_0xAC = 0;
    memset((void*)self, 0, 0x84);
    memset((void*)&self->field_0x84, 0, 0xC);
    self->elems[0].field_0x0 = 0;
    self->elems[1].field_0x0 = 1;
    self->elems[2].field_0x0 = 2;
    return self;
}

// Child-object destructor: detaches the three bound nodes from the chr-blend,
// releases their animation objects, and zeroes the anim state; frees the
// object itself when the MWCC delete flag is set (`flags > 0`).
CScnChild80496B0C* __dt__80497FEC(CScnChild80496B0C* self, int flags) {
    u32 i;
    CScnChild80496B0C* it;
    CScnNode80496B0C* node;
    if (self != 0) {
        it = self;
        i = 0;
        do {
            node = it->field_0x84[0];
            if (node != 0) {
                if (node->field_0x10 != 0) {
                    self->field_0x98->Detach(self->field_0x90 * 3 +
                                             node->field_0x0);
                    self->field_0x92--;
                    node->field_0x10->v11();
                    ((nw4r::g3d::G3dObj*)node->field_0x10)->Destroy();
                    node->field_0x10 = 0;
                }
                node->field_0x18 = 0;
                it->field_0x84[0] = 0;
                self->field_0x9C = ml::CVec3::zero;
            }
            i++;
            it = (CScnChild80496B0C*)((u8*)it + 4);
        } while (i < 3);
        if (flags > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

// Removes the binding whose item-anim tag matches (or, when tag is 0, the
// first bound slot) from the child's slot stack: releases its animation
// object, then shifts the remaining slots down and zeroes the tail. On a
// match the cursor does NOT advance -- the shift pulls the next binding into
// the current slot, so it is re-examined on the next pass.
extern "C" __declspec(noinline) void func_804980E4(CScnChild80496B0C* child, u32 tag) {
    CScnChild80496B0C* walk;
    CScnNode80496B0C* node;
    u32 i;
    u32 off;
    i = 0;
    off = 0;
    walk = child;
    while (i < 3) {
        node = walk->field_0x84[0];
        if (node == 0) {
            return;
        }
        if ((u32)node->field_0x18 == tag || tag == 0) {
            if (node->field_0x10 != 0) {
                child->field_0x98->Detach(child->field_0x90 * 3 + node->field_0x0);
                child->field_0x92--;
                node->field_0x10->v11();
                ((nw4r::g3d::G3dObj*)node->field_0x10)->Destroy();
                node->field_0x10 = 0;
            }
            node->field_0x18 = 0;
            // Shift the remaining bindings down; empty when i == 2.
            for (u32 k = i; k < 2; k++) {
                child->field_0x84[k] = child->field_0x84[k + 1];
                child->field_0x84[k + 1] = 0;
            }
        } else {
            // Not our binding: advance to the next overlapping slot view.
            walk = (CScnChild80496B0C*)((u8*)walk + 4);
            i++;
            off += 4;
        }
    }
}

// Binds a chr animation into the child's free slot: tears down stale
// bindings, evicts the oldest overflow binding when full, fills the slot from
// the animation resource, attaches it to the chr blend, then shifts the
// slot-stack and notifies the model listeners.
u32 func_80498288(CScnChild80496B0C* self, s32* pFrame, VTarget* pAnmObj,
                  CScnItemAnim* tag, u32 value, u32 flag) {
    if (value == 0) {
        // Release every current binding of this child. The slot view is a
        // walking pointer advanced by one word per iteration (retail shape).
        u32 i;
        CScnChild80496B0C* walk;
        for (i = 0, walk = self; i < 3; i++, walk = (CScnChild80496B0C*)((u8*)walk + 4)) {
            CScnNode80496B0C* node = walk->field_0x84[0];
            if (node == NULL) {
                continue;
            }
            if (node->field_0x10 != NULL) {
                self->field_0x98->Detach(self->field_0x90 * 3 + node->field_0x0);
                self->field_0x92--;
                node->field_0x10->v11();
                ((nw4r::g3d::G3dObj*)node->field_0x10)->Destroy();
                node->field_0x10 = NULL;
            }
            node->field_0x18 = NULL;
            walk->field_0x84[0] = NULL;
            self->field_0x9C = ml::CVec3::zero;
        }
    }

    // Count occupied slots.
    int count = 0;
    if (self->field_0x84[0] != NULL) {
        count = 1;
        if (self->field_0x84[1] != NULL) {
            count = 2;
            if (self->field_0x84[2] != NULL) {
                count = 3;
            }
        }
    }

    if (count >= self->field_0x94) {
        // Full: evict the newest occupied overflow binding (slot 2, else 1).
        CScnChild80496B0C* view = (CScnChild80496B0C*)((u8*)self + 8);
        for (int k = 2; k >= 1; view = (CScnChild80496B0C*)((u8*)view - 4), k--) {
            if (self->field_0x84[k] == NULL) {
                continue;
            }
            CScnNode80496B0C* node = self->field_0x84[k];
            if (node->field_0x10 != NULL) {
                self->field_0x98->Detach(self->field_0x90 * 3 + node->field_0x0);
                self->field_0x92--;
                node->field_0x10->v11();
                ((nw4r::g3d::G3dObj*)node->field_0x10)->Destroy();
                node->field_0x10 = NULL;
            }
            node->field_0x18 = NULL;
            self->field_0x84[k] = NULL;
            break;
        }
    }

    // Find the first free binding record (index recomputed after the scan,
    // matching the retail mulli-based address formation).
    CScnNode80496B0C* rec = NULL;
    for (u32 freeIdx = 0; freeIdx < 3; freeIdx++) {
        if (self->elems[freeIdx].field_0x10 == NULL) {
            rec = &self->elems[freeIdx];
            break;
        }
    }

    rec->field_0x14 = *pFrame;
    rec->field_0x10 = pAnmObj;
    rec->field_0x18 = tag;
    // Plain int->float casts: MWCC emits its own 0x4330-magic sequence
    // (lis/stw punning + single-precision fsubs + stfs).
    rec->field_0x4 = (f32)value;
    rec->field_0x8 = lbl_eu_8066AAC0;
    rec->field_0xC = lbl_eu_8066AAC4;
    rec->field_0x20 = lbl_eu_8066AAC0;
    // Anm-resource guard: one load serves the null test and the frame-rate
    // fetch; the later block reloads +0x2C (the flag stores force it).
    nw4r::g3d::ResAnmChrData* data = ((VTarget*)pAnmObj)->field_0x2C;
    if (data == NULL) {
        nw4r::db::Panic(lbl_eu_8056E9D0, 0x27, lbl_eu_8056E9B4,
                        lbl_eu_8056E9A8, lbl_eu_806639E4);
    }
    rec->field_0x1C = (f32)(s16)((VTarget*)data)->field_0x20;
    rec->field_0x2 = 0;
    if (flag != 0) {
        rec->field_0x2 |= 1;
    } else {
        rec->field_0x2 &= ~1;
    }
    rec->field_0x24 = lbl_eu_8066AAC0;

    if (data == NULL) {
        nw4r::db::Panic(lbl_eu_8056E9D0, 0x27, lbl_eu_8056E9B4,
                        lbl_eu_8056E9A8, lbl_eu_806639E4);
    }
    if (((VTarget*)data)->field_0x24 == 1) {
        rec->field_0x2 |= 2;
    } else {
        rec->field_0x2 &= ~2;
    }

    // Stamp the record with the model-wide rotating counter (0 is reserved).
    CScn80496B0C* root = self->field_0xAC;
    root->field_0x17C = root->field_0x17C + 1;
    if (root->field_0x17C == 0) {
        root->field_0x17C = 1;
    }
    rec->field_0x28 = root->field_0x17C;

    // Swap the record into the chr blend slot for this child.
    nw4r::g3d::AnmObjChrBlend* blend = self->field_0x98;
    int bindIdx = (int)blend->Detach(self->field_0x90 * 3 + rec->field_0x0);
    blend->Attach(bindIdx, (nw4r::g3d::AnmObjChrRes*)pAnmObj);

    CScnNode80496B0C* prev = self->field_0x84[0];
    self->field_0x96 = flag;
    self->field_0x92++;
    if (prev != NULL && !(prev->field_0x2 & 2)) {
        prev->field_0x10->v8(lbl_eu_8066AAC0);
    }

    // Shift the slot stack down one level (re-reads slot 0, as in retail).
    self->field_0x84[2] = self->field_0x84[1];
    self->field_0x84[1] = self->field_0x84[0];

    if (prev != NULL) {
        // Notify each model listener with the evicted binding's stamp; the
        // flag mirrors the sign of the child's base index.
        u16 stamp = prev->field_0x28;
        u32 neg = (u32)(((u32)(-(s32)self->field_0x90) |
                         (u32)(s32)self->field_0x90) >>
                        31);
        u32 li;
        CScn80496B0C* walk;
        for (li = 0, walk = root; li < 4; li++, walk = (CScn80496B0C*)((u8*)walk + 4)) {
            VTarget* listener = walk->field_0x1DC[0];
            if (listener != NULL) {
                listener->v1(root->field_0x4, stamp, neg);
            }
        }
    }

    self->field_0x84[0] = rec;
    return rec->field_0x28;
}

// Cross-fade weight step: recomputes the head binding's blend factor from its
// growth state, sorts the two overflow bindings by weight, redistributes the
// remaining weight over them, and pushes the per-slot weights into the chr
// blend scaled by `rate`.
extern "C" void func_804986F8(CScnChild80496B0C* self, f32 rate) {
    CScnNode80496B0C* node = self->field_0x84[0];
    if (node == NULL) {
        return;
    }

    f32 w;
    if (node != NULL) {
        w = node->field_0x10->v6();
    } else {
        w = lbl_eu_8066AAC0;
    }
    self->field_0x84[0]->field_0x24 = w;

    // Single bubble pass: keep the two overflow bindings sorted ascending by
    // their normalized weight.
    bool swapped;
    do {
        swapped = false;
        CScnNode80496B0C* b = self->field_0x84[2];
        if (b != NULL) {
            CScnNode80496B0C* a = self->field_0x84[1];
            if (a->field_0xC < b->field_0xC) {
                self->field_0x84[2] = a;
                swapped = true;
                self->field_0x84[1] = b;
            }
        }
    } while (swapped);

    CScnNode80496B0C* head = self->field_0x84[0];
    CScnNode80496B0C* n;
    CScnNode80496B0C** slot;
    s32 k;
    if (head->field_0x8 < head->field_0x4) {
        // Growing: advance toward the target by the scene-reported step.
        // Double fabs before the f32 narrowing pins retail's fabs+frsp pair.
        f32 step = (f32)__fabs(func_80496288(head->field_0x18->scn));
        head->field_0x8 = head->field_0x8 + step;
        if (head->field_0x8 > head->field_0x4) {
            head->field_0x8 = head->field_0x4;
        }
        head->field_0xC = head->field_0x8 / head->field_0x4;
        if (!(head->field_0x8 >= head->field_0x4)) {
            // Still growing: drop the overflow bindings (slots 2 -> 1).
            for (slot = &self->field_0x84[2], k = 2; k >= 1; k--) {
                n = self->field_0x84[k];
                if (n != NULL) {
                    if (n->field_0x10 != NULL) {
                        self->field_0x98->Detach(self->field_0x90 * 3 +
                                                 n->field_0x0);
                        self->field_0x92--;
                        n->field_0x10->v11();
                        ((nw4r::g3d::G3dObj*)n->field_0x10)->Destroy();
                        n->field_0x10 = NULL;
                    }
                    n->field_0x18 = NULL;
                    self->field_0x84[k] = NULL;
                }
            }
        }
    } else if (!(head->field_0x4 <= lbl_eu_8066AADC)) {
        // Fully grown past the threshold: drop the overflow bindings too.
        for (slot = &self->field_0x84[2], k = 2; k >= 1; k--) {
            n = self->field_0x84[k];
            if (n != NULL) {
                if (n->field_0x10 != NULL) {
                    self->field_0x98->Detach(self->field_0x90 * 3 +
                                             n->field_0x0);
                    self->field_0x92--;
                    n->field_0x10->v11();
                    ((nw4r::g3d::G3dObj*)n->field_0x10)->Destroy();
                    n->field_0x10 = NULL;
                }
                n->field_0x18 = NULL;
                self->field_0x84[k] = NULL;
            }
        }
    }

    // Redistribute: the head keeps its share of the fixed total; the rest is
    // split over the surviving overflow bindings by their weights.
    f32 base = head->field_0xC;
    if ((s16)self->field_0x92 == 1) {
        base = lbl_eu_8066AAC4;
    }
    head->field_0x20 = base;
    // Double round-trip pins the retail frsp on the running total.
    f32 total = (f32)(f64)base;
    f32 rem = lbl_eu_8066AAC4 - base;

    if (self->field_0x84[1] != NULL) {
        self->field_0x84[1]->field_0x20 = rem * self->field_0x84[1]->field_0xC;
        total += self->field_0x84[1]->field_0x20;
    }
    if (self->field_0x84[2] != NULL) {
        self->field_0x84[2]->field_0x20 = rem * self->field_0x84[2]->field_0xC;
        total += self->field_0x84[1]->field_0x20;
    }
    if (total > lbl_eu_8066AAC0) {
        if (self->field_0x84[0] != NULL) {
            self->field_0x84[0]->field_0x20 /= total;
        }
        if (self->field_0x84[1] != NULL) {
            self->field_0x84[1]->field_0x20 /= total;
        }
        if (self->field_0x84[2] != NULL) {
            self->field_0x84[2]->field_0x20 /= total;
        }
    }

    // Push each live slot's weight into the chr blend, front to back.
    for (u32 i = 0; i < 3; i++) {
        CScnNode80496B0C* n = self->field_0x84[i];
        if (n == NULL) {
            break;
        }
        ((VTarget*)self->field_0x98)
            ->v16(rate * n->field_0x20, self->field_0x90 * 3 + n->field_0x0);
    }
}

// Advances each bound node's chr animation by one frame: fetches the animated
// result, scales its translate by the node's +0x20 factor, and accumulates it
// into vecB/vecC. Slot 0 accumulates x/z into vecB and y into vecC; later
// slots only accumulate y into vecC (nodes flagged +0x2 bit0) or the full
// translate otherwise. Finally produces a delta against the stored position
// when the first slot's node is flagged.
__declspec(noinline) void func_80498A5C(CScnChild80496B0C* child,
                                        ml::CVec3* outDelta, ml::CVec3* vecB,
                                        ml::CVec3* vecC, u32 arg) {
    nw4r::g3d::ChrAnmResult result;
    nw4r::math::VEC3 t;
    outDelta->x = lbl_eu_8066AAC0;
    outDelta->y = lbl_eu_8066AAC0;
    outDelta->z = lbl_eu_8066AAC0;
    vecB->x = lbl_eu_8066AAC0;
    vecB->y = lbl_eu_8066AAC0;
    vecB->z = lbl_eu_8066AAC0;
    vecC->x = lbl_eu_8066AAC0;
    vecC->y = lbl_eu_8066AAC0;
    vecC->z = lbl_eu_8066AAC0;
    // Each access re-reads the slot: the virtual call forces MWCC to
    // reload the binding record around it (retail keeps the base+off
    // view pointer in a register).
    u32 i = 0;
    nw4r::math::VEC3* pT = &t;
    do {
        if (child->field_0x84[i] == NULL)
            break;
        child->field_0x84[i]->field_0x10->v12(&result, arg);
        result.GetTranslate(pT);
        f32 scale = child->field_0x84[i]->field_0x20;
        pT->x *= scale;
        pT->y *= scale;
        pT->z *= scale;
        if (i == 0) {
            if (child->field_0x84[i]->field_0x2 & 1) {
                vecB->x += t.x;
                vecC->y += t.y;
                vecB->z += t.z;
            } else {
                vecC->x += t.x;
                vecC->y += t.y;
                vecC->z += t.z;
            }
        } else {
            if (child->field_0x84[i]->field_0x2 & 1) {
                vecC->y += t.y;
            } else {
                vecC->x += t.x;
                vecC->y += t.y;
                vecC->z += t.z;
            }
        }
        i++;
    } while (i < 3);

    // Delta bookkeeping on the first bound slot.
    CScnNode80496B0C* node = child->field_0x84[0];
    if (node != 0 && (node->field_0x2 & 1)) {
        if (child->field_0x96 != 0) {
            child->field_0x96 = 0;
            child->field_0x9C = *vecB;
        } else {
            outDelta->x = vecB->x - child->field_0x9C.x;
            outDelta->z = vecB->z - child->field_0x9C.z;
            child->field_0x9C = *vecB;
        }
    }
}

// Root-object destructor: tears down both embedded children's node bindings
// (the +0xC0 child first, the +0x10 child second) and frees the object
// itself when flags > 0.
CScn80496B0C* __dt__80496BB4(CScn80496B0C* self, int flags) {
    if (self != 0) {
        // MWCC keeps the embedded-child address checks (addic.) from the
        // inlined release helper's defensive null test.
        if ((&self->field_0xC0) != 0) {
            for (u32 i = 0; i < 3; i++) {
                CScnNode80496B0C* node = self->field_0xC0.field_0x84[i];
                if (node != 0) {
                    if (node->field_0x10 != 0) {
                        self->field_0xC0.field_0x98->Detach(
                            self->field_0xC0.field_0x90 * 3 + node->field_0x0);
                        self->field_0xC0.field_0x92--;
                        node->field_0x10->v11();
                        ((nw4r::g3d::G3dObj*)node->field_0x10)->Destroy();
                        node->field_0x10 = 0;
                    }
                    node->field_0x18 = 0;
                    self->field_0xC0.field_0x84[i] = 0;
                    self->field_0xC0.field_0x9C = ml::CVec3::zero;
                }
            }
        }
        if ((&self->field_0x10) != 0) {
            for (u32 i = 0; i < 3; i++) {
                CScnNode80496B0C* node = self->field_0x10.field_0x84[i];
                if (node != 0) {
                    if (node->field_0x10 != 0) {
                        self->field_0x10.field_0x98->Detach(
                            self->field_0x10.field_0x90 * 3 + node->field_0x0);
                        self->field_0x10.field_0x92--;
                        node->field_0x10->v11();
                        ((nw4r::g3d::G3dObj*)node->field_0x10)->Destroy();
                        node->field_0x10 = 0;
                    }
                    node->field_0x18 = 0;
                    self->field_0x10.field_0x84[i] = 0;
                    self->field_0x10.field_0x9C = ml::CVec3::zero;
                }
            }
        }
        if (flags > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

// Inlined node-release sequence shared by the teardown paths: detaches the
// bound node from the chr-blend, releases its animation object, and zeroes
// the anim state.
inline void ScnReleaseChild(CScnChild80496B0C* self, CScnChild80496B0C* walk,
                            CScnNode80496B0C* node) {
    if (node->field_0x10 != 0) {
        self->field_0x98->Detach(self->field_0x90 * 3 + node->field_0x0);
        self->field_0x92--;
        node->field_0x10->v11();
        ((nw4r::g3d::G3dObj*)node->field_0x10)->Destroy();
        node->field_0x10 = 0;
    }
    node->field_0x18 = 0;
    walk->field_0x84[0] = 0;
    self->field_0x9C = ml::CVec3::zero;
}

// Tears down both children's node bindings (same release sequence as the child
// destructor, inlined for the two embedded children) and destroys the shared
// chr-blend object.
void func_80496D74(CScn80496B0C* self) {
    // Locals declared so MWCC allocates node/walk/counter to r26/r27/r28.
    CScnNode80496B0C* node;
    CScn80496B0C* walk;
    u32 i;
    for (i = 0, walk = self; i < 3; i++, walk = (CScn80496B0C*)((u8*)walk + 4)) {
        node = walk->field_0x10.field_0x84[0];
        if (node != 0) {
            ScnReleaseChild(&self->field_0x10, &walk->field_0x10, node);
        }
    }
    for (i = 0, walk = self; i < 3; i++, walk = (CScn80496B0C*)((u8*)walk + 4)) {
        node = walk->field_0xC0.field_0x84[0];
        if (node != 0) {
            ScnReleaseChild(&self->field_0xC0, &walk->field_0xC0, node);
        }
    }
    if (self->field_0x1CC != 0) {
        self->field_0x1CC->Destroy();
        self->field_0x1CC = 0;
    }
}

// Evaluates the chr animation named by name into the embedded result buffer
// and returns it. The ResAnmChr resource is fetched from the bound node's
// animation object (+0x2C); the node-name dictionary is resolved through the
// resource's toChrDataDic offset (asserted 4-aligned).
nw4r::g3d::ChrAnmResult* func_80496FC4(CScn80496B0C* self, const char* name,
                                       f32 frame) {
    CScnChild80496B0C* child = self->field_0x8;
    if (child == 0) {
        return 0;
    }
    CScnNode80496B0C* node = child->field_0x84[0];
    if (node != 0) {
        // Declared first: MWCC gives the first local the higher sp slot, and
        // retail frames the dictionary at sp+0xc with the anm object below
        // it at sp+0x8. The stand-in avoids an initializer store.
        ResDicSlot dicSlot;
        nw4r::g3d::ResAnmChr anm(node->field_0x10->field_0x2C);
        if (!anm.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E9D0, 0x27, lbl_eu_8056E9B4,
                            lbl_eu_8056E9A8, lbl_eu_806639E4);
        }
        u32 ofs = anm.ref().toChrDataDic;
        nw4r::g3d::ResDicData* pDicData;
        if (ofs != 0) {
            u8* pDic = (u8*)anm.ptr() + ofs;
            if (((u32)pDic & 3) != 0) {
                nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
            }
            pDicData = (nw4r::g3d::ResDicData*)pDic;
        } else {
            pDicData = NULL;
        }
        dicSlot.mpData = pDicData;
        s32 idx =
            ((nw4r::g3d::ResDic*)&dicSlot)->GetIndex(name);
        anm.GetAnmResult((nw4r::g3d::ChrAnmResult*)self->field_0x180, idx,
                         frame);
    }
    return (nw4r::g3d::ChrAnmResult*)self->field_0x180;
}
// Returns bit 1 of the +0x02 flag word on the node reached via
// self->field_0x8->field_0x84, or 0 when that chain is absent.
u32 func_8049715C(CScn80496B0C* self) {
    CScnChild80496B0C* child = self->field_0x8;
    if (child == 0) return 0;
    CScnNode80496B0C* node = child->field_0x84[0];
    if (node == 0) return 0;
    return (node->field_0x2 >> 1) & 1;
}
// Binds the chr animation from the item-anim to the child (child2 with a
// forced zero flag, child1 with the caller's), packing the two results into
// a u32 (high word from child2, low word from child1); then re-arms the model
// node release for the first bound slot.
//
// Virtual dispatch surface used by this helper only (AnmObjChrRes layout):
// +0x1C SetFrame(f32), +0x30 Bind(mdl), +0x34 Attach(obj, idx), +0x50
// Bind(mdl, chr, opt).
struct AnmObjResVt80497544 {
    virtual void q0() = 0;  // +0x08
    virtual void q1() = 0;  // +0x0C
    virtual void q2() = 0;  // +0x10
    virtual void q3() = 0;  // +0x14
    virtual void q4() = 0;  // +0x18
    virtual void setFrame(f32 frame) = 0;        // +0x1C
    virtual void q6() = 0;  // +0x20
    virtual void q7() = 0;  // +0x24
    virtual void q8() = 0;  // +0x28
    virtual void q9() = 0;  // +0x2C
    virtual bool bindMdl(void* pMdl) = 0;        // +0x30
    virtual void attachObj(void* pObj, int idx) = 0;  // +0x34
    virtual void q12() = 0;  // +0x38
    virtual void q13() = 0;  // +0x3C
    virtual void q14() = 0;  // +0x40
    virtual void q15() = 0;  // +0x44
    virtual void q16() = 0;  // +0x48
    virtual void q17() = 0;  // +0x4C
    // Bind with target node id: retail passes the ResMdl copy, the raw
    // node-id word and option 1.
    virtual bool bind3(void* pMdl, int nodeId, int opt) = 0;  // +0x50
};

// nw4r AnmObjChrRes::Construct is a private static in g3d_anmchr.h; retail's
// mangled linker name is reproduced verbatim under extern "C".
extern "C" nw4r::g3d::AnmObjChrRes*
Construct__Q34nw4r3g3d12AnmObjChrResFP12MEMAllocatorPUlQ34nw4r3g3d9ResAnmChrQ34nw4r3g3d6ResMdlb(
    MEMAllocator* pAllocator, u32* pSize, void* pChr, void* pMdl, int cache);

extern "C" { extern const double lbl_eu_8066AAD0; }

extern "C" __declspec(noinline) u32 func_80497544(
    CScn80496B0C* self, CScnChild80496B0C* child, CScnItemAnim* anim,
    nw4r::g3d::ResAnmChr* anmRes, u32 c, u32 e, int f) {
    nw4r::g3d::ResMdl mdl(self->field_0x1D0);
    nw4r::g3d::ResAnmChr chr(*anmRes);
    u32 size;
    nw4r::g3d::AnmObjChrRes* anmObj =
        Construct__Q34nw4r3g3d12AnmObjChrResFP12MEMAllocatorPUlQ34nw4r3g3d9ResAnmChrQ34nw4r3g3d6ResMdlb(
            (MEMAllocator*)func_8048ECFC(self->field_0x0), &size, &chr,
            &mdl, 0);

    if (child->field_0x90 != 0) {
        // Retail compiles the != -1 test as addis/cmplwi: (id + 0x10000)
        // == 0xFFFF holds exactly when id == -1. Keeping the id in a local
        // lets MWCC reuse the test-time register for the Bind call argument.
        int nodeId = self->field_0x1D8;
        if (((u32)nodeId + 0x10000) != 0xFFFF) {
            nw4r::g3d::ResMdl bindMdl(self->field_0x1D0);
            ((AnmObjResVt80497544*)anmObj)->bind3(&bindMdl, nodeId, 1);
        } else {
            nw4r::g3d::ResMdl bindMdl(self->field_0x1D0);
            ((AnmObjResVt80497544*)anmObj)->bindMdl(&bindMdl);
        }
    } else {
        nw4r::g3d::ResMdl bindMdl(self->field_0x1D0);
        ((AnmObjResVt80497544*)anmObj)->bindMdl(&bindMdl);
    }

    nw4r::g3d::ResAnmChr bind(*anmRes);
    u32 result = func_80498288(child, (s32*)&bind, (VTarget*)anmObj, anim, c, e);

    if (self->field_0x17E == 0) {
        ((AnmObjResVt80497544*)self->field_0x1D4)->attachObj(self->field_0x1CC, 6);
        self->field_0x17E = 1;
    }

    if (f == 0) {
        CScnNode80496B0C* node = child->field_0x84[0];
        f32 frame = (node != NULL) ? node->field_0x1C : lbl_eu_8066AAC0;
        if (node != NULL) {
            ((AnmObjResVt80497544*)node->field_0x10)->setFrame(frame - lbl_eu_8066AAC4);
        }
    } else if (f > 0) {
        CScnNode80496B0C* node = child->field_0x84[0];
        if (node != NULL) {
            // Retail lowers this int->float conversion through the shared
            // sdata2 magic double (lbl_eu_8066AAD0); our TU's anonymous pool
            // copy of the same bytes is what MWCC emits here.
            ((AnmObjResVt80497544*)node->field_0x10)->setFrame((f32)(s32)f);
        }
    }
    return result;
}

u32 func_80497190(CScn80496B0C* self, CScnItemAnim* anim, int index, u32 c,
                  u32 d, u32 e, u32 f) {
    u32 result = 0;
    if (self->field_0x1D8 != 0xFFFFFFFF && self->field_0xC->field_0x84[0] != 0 &&
        e != 0) {
        nw4r::g3d::ResAnmChr t = func_8049E708(anim, index);
        result = func_80497544(self, self->field_0xC, anim, &t, c, 0, f) << 16;
    }
    nw4r::g3d::ResAnmChr t2 = func_8049E708(anim, index);
    result |= func_80497544(self, self->field_0x8, anim, &t2, c, d, f) & 0xFFFF;
    if (self->field_0xC->field_0x84[0] != 0) {
        CScnChild80496B0C* child = self->field_0x8;
        u32 target = self->field_0x1D8;
        if (target != 0xFFFFFFFF) {
            for (u32 i = 0; i < 3; i++) {
                if (child->field_0x84[i] == 0) {
                    continue;
                }
                nw4r::g3d::ResMdl mdl(child->field_0xA8);
                child->field_0x84[i]->field_0x10->v19(&mdl, target, 1);
                break;
            }
        }
    }
    return result;
}

// Root-object constructor: zeroes the header, initializes both embedded
// child objects, fills the anim/scale state, then caches the child
// addresses in field_0x8/field_0xC.
CScn80496B0C* __ct__80496B0C(CScn80496B0C* obj) {
    obj->field_0x0 = 0;
    obj->field_0x4 = 0;
    obj->field_0x8 = 0;
    obj->field_0xC = 0;
    func_80497F34(&obj->field_0x10);
    func_80497F34(&obj->field_0xC0);
    obj->field_0x170 = lbl_eu_8066AAC0;
    obj->field_0x174 = lbl_eu_8066AAC0;
    obj->field_0x178 = lbl_eu_8066AAC4;
    obj->field_0x17C = 0;
    obj->field_0x17E = 0;
    obj->field_0x1CC = 0;
    obj->field_0x1D0 = nw4r::g3d::ResMdl(NULL);
    obj->field_0x1D4 = 0;
    obj->field_0x1D8 = -1;
    for (int i = 0; i < 4; i++) {
        obj->field_0x1DC[i] = 0;
    }
    obj->field_0x8 = &obj->field_0x10;
    obj->field_0xC = &obj->field_0xC0;
    return obj;
}

// Binds the model's named ResNode id into the root (or -1 when the name is
// NULL, 0 when the node is absent).
void func_804970D0(CScn80496B0C* self, const char* name) {
    if (name != 0) {
        nw4r::g3d::ResNode node = self->field_0x1D0.GetResNode(name);
        if (!node.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
        }
        self->field_0x1D8 = node.GetID();
    } else {
        self->field_0x1D8 = -1;
    }
}

// Sets up both child slots from the model: stores the scene/model handles,
// copies the ResMdl and animation object out of the model, constructs the
// chr-blend with the scene allocator, then wires both children to it.
void func_80496F14(CScn80496B0C* self, CScn* scn, CScnModel80496B0C* model,
                   u16 slot) {
    self->field_0x0 = scn;
    self->field_0x4 = model;
    nw4r::g3d::ResMdl resMdl(model->field_0x146C);
    self->field_0x1D0 = resMdl;
    self->field_0x1D4 = model->field_0x147C;
    u32 size;
    nw4r::g3d::AnmObjChrBlend* blend =
        Construct__Q34nw4r3g3d14AnmObjChrBlendFP12MEMAllocatorPUlQ34nw4r3g3d6ResMdli(
            (MEMAllocator*)func_8048ECFC(scn), &size, resMdl, 6);
    self->field_0x1CC = blend;
    nw4r::g3d::ResMdlData* resData = self->field_0x1D0.ptr();
    CScnChild80496B0C* a = self->field_0x8;
    a->field_0xAC = self;
    a->field_0x98 = blend;
    a->field_0x94 = slot;
    a->field_0x90 = 0;
    a->field_0xA8 = resData;
    CScnChild80496B0C* b;
    nw4r::g3d::AnmObjChrBlend* bBlend;
    nw4r::g3d::ResMdlData* bResData;
    bBlend = self->field_0x1CC;
    bResData = self->field_0x1D0.ptr();
    b = self->field_0xC;
    b->field_0xAC = self;
    b->field_0x98 = bBlend;
    b->field_0x94 = slot;
    b->field_0x90 = 1;
    b->field_0xA8 = bResData;
}

// Cross-fade step: when the second child is still free, evaluates the chr anim
// into it (phase 1), then re-evaluates the active child (phase 2) and packs
// both setup results into the returned u32. Marks the newly bound node with
// flag bit 2.
u32 func_804972E8(CScn80496B0C* self, CScnItemAnim* anim, int index,
                  unsigned int frame, u32 g) {
    u32 result = 0;
    self->field_0x170 = (f32)frame;

    // Phase 1: bind the incoming animation into the free second child.
    // Flat && chain: the flag test re-reads through the full pointer path
    // with an explicit ternary so MWCC keeps retail's paired conditional
    // branches on the cached head pointer.
    if (self->field_0x1D8 != 0xFFFFFFFF &&
        self->field_0xC->field_0x84[0] == 0 &&
        self->field_0x8->field_0x84[0] != 0 &&
        (self->field_0x8->field_0x84[0] != NULL
             ? ((self->field_0x8->field_0x84[0]->field_0x2 >> 2) & 1)
             : 0) == 0) {
        // The anm resource argument is a by-value ResAnmChr copy of the
        // head's id word (retail stores it to a stack temp and passes the
        // temp's address).
        nw4r::g3d::ResAnmChr anmTmp(
            (void*)self->field_0x8->field_0x84[0]->field_0x14);
        result = func_80497544(
                     self, self->field_0xC,
                     self->field_0x8->field_0x84[0]->field_0x18, &anmTmp, 0,
                     0, -1)
                 << 16;

        // Current-frame query: result unused, kept for the virtual call's
        // side effect. Reading child1 through a volatile view gives an
        // unprovable head pointer, so retail's dead default-constant arm
        // survives.
        CScnChild80496B0C* volatile& c1v =
            (CScnChild80496B0C* volatile&)self->field_0x8;
        CScnNode80496B0C* fh = c1v->field_0x84[0];
        f32 fv = fh != NULL ? fh->field_0x10->v6() : lbl_eu_8066AAC0;
        (void)fv;
        if (self->field_0xC->field_0x84[0] != 0) {
            self->field_0xC->field_0x84[0]->field_0x10->v5();
        }
        // Re-arm the model node release on the first bound slot (the -1
        // recheck is dead here -- phase 1 already proved id != -1 -- but
        // retail still emits the test).
        u32 target = self->field_0x1D8;
        if (target != 0xFFFFFFFF) {
            CScnChild80496B0C* child1 = self->field_0x8;
            for (u32 i = 0; i < 3; i++) {
                if (child1->field_0x84[i] == 0) {
                    continue;
                }
                nw4r::g3d::ResMdl mdl(child1->field_0xA8);
                child1->field_0x84[i]->field_0x10->v19(&mdl, target, 1);
                break;
            }
        }
        self->field_0x170 = lbl_eu_8066AAC0;
    }

    // Phase 2: evaluate into whichever child is (now) bound. Branches are
    // ordered bit-set-first so the temporaries get retail's stack slots
    // (sp+0x10 for the child1 path, sp+0x14 for the child2 path).
    CScnNode80496B0C* node = self->field_0x8->field_0x84[0];
    if ((node != 0 ? ((node->field_0x2 >> 2) & 1) : 0) != 0) {
        nw4r::g3d::ResAnmChr t2 = func_8049E708(anim, index);
        result |=
            func_80497544(self, self->field_0x8, anim, &t2, frame, 0, g) &
            0xFFFF;
        // Mark the freshly bound head so the next call skips phase 1.
        CScnNode80496B0C* hit = self->field_0x8->field_0x84[0];
        if (hit != 0) {
            hit->field_0x2 |= 4;
        }
    } else {
        nw4r::g3d::ResAnmChr t = func_8049E708(anim, index);
        result = func_80497544(self, self->field_0xC, anim, &t, frame, 0, g)
                 << 16;
    }
    return result;
}

// ===== CTTask<CScn> vtable-slot name carriers (kept last in emission order)
// The three unspellable CTTask<CScn> retail symbols (<...> cannot be
// declared in C++) must exist in the symtab for the injected .data vtable
// relocs. These stubs are defined LAST so the UNIT_RULES
// drop_text_symbols_as_undef pass removes their bytes from the .text tail
// without shifting any matched function; the definitions become UNDEF and
// resolve to CScn.o's real implementations at link.
extern "C" void __ct_CTask_CScn_name_carrier_1(void) {}
extern "C" void __ct_CTask_CScn_name_carrier_2(void) {}
extern "C" void __ct_CTask_CScn_name_carrier_3(void) {}
