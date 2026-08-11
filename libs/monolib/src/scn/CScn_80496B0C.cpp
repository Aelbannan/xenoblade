// Auto-scaffolded catalog TU for monolib/src/scn/CScn_80496B0C
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "nw4r/g3d/g3d_anmchr.h"
#include "nw4r/db/db_assert.h"
#include "monolib/math/CVec3.hpp"
#include "libs/monolib/src/scn/CScnItemAnim.hpp"

// Default value returned by the accessor helpers when the target object
// chain is missing (sda2 constant).
extern "C" { extern const float lbl_eu_8066AAC0; }
extern "C" { extern const float lbl_eu_8066AAC4; }

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
extern const char lbl_eu_8056E9D0[];   // file (chr-anm data assert, line 0x27)
extern const char lbl_eu_8056E9B4[];   // fmt
extern const char lbl_eu_8056E9A8[];   // arg
extern const char lbl_eu_806639E4[4];  // arg (sda2 string)
extern const char lbl_eu_80530F08[];   // file (ResDic alignment assert, line 0x54)
extern const char lbl_eu_80530EE0[];   // fmt

// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).
struct VTarget {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual f32 v6() = 0;     // vtable + 0x20
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;   // vtable + 0x34 (AnmObjChr::Release())
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19(nw4r::g3d::ResMdl mdl, u32 target, u32 option) =
        0;                    // vtable + 0x54 (AnmObjChr::Release(mdl,target,opt))
    virtual ~VTarget() {}

    /* +0x04 */ u8 pad_0x04[0x2C - 0x04];
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
    virtual void v11(void* pBlend, u32 count) = 0;   // vtable + 0x34
    virtual void v12(void* pTarget) = 0;   // vtable + 0x38
    virtual ~CScnAnmChr80496B0C() {}
};

// Model/owner object passed to func_80496F14: ResMdl data pointer at +0x146C
// and the animation object at +0x147C.
struct CScnModel80496B0C {
    char field_0x0[0x146C];                // +0x00
    nw4r::g3d::ResMdlData* field_0x146C;   // +0x146C
    char field_0x1470[0x147C - 0x1470];    // +0x1470
    CScnAnmChr80496B0C* field_0x147C;      // +0x147C
};

// One 0x2C-sized slot of the child's three-element node-binding array.
struct CScnChildElem80496B0C {
    u16 field_0x0;                        // +0x00
    u16 field_0x2;                        // +0x02
    char field_0x4[0x14 - 0x4];           // +0x04
    u32 field_0x14;                       // +0x14
    char field_0x18[0x2C - 0x18];         // +0x18
};

// Child object: node pointers at +0x84 / +0x88 / +0x8C plus anim/scale state.
struct CScnChild80496B0C {
    CScnChildElem80496B0C elems[3];       // +0x00
    CScnNode80496B0C* field_0x84[3];      // +0x84 (node-binding slots)
    s16 field_0x90;                       // +0x90
    s16 field_0x92;                       // +0x92
    u16 field_0x94;                       // +0x94
    s16 field_0x96;                       // +0x96
    nw4r::g3d::AnmObjChrBlend* field_0x98; // +0x98
    ml::CVec3 field_0x9C;                 // +0x9C
    nw4r::g3d::ResMdlData* field_0xA8;    // +0xA8
    CScn80496B0C* field_0xAC;             // +0xAC
};

// Leaf node reached via child->field_0x84.
struct CScnNode80496B0C {
    s16 field_0x0;                        // +0x00
    u16 field_0x2;                        // +0x02
    char field_0x4[0x10 - 0x4];           // +0x04
    VTarget* field_0x10;                  // +0x10 polymorphic dispatch object
    u32 field_0x14;                       // +0x14
    u32 field_0x18;                       // +0x18
    f32 field_0x1C;                       // +0x1C
    f32 field_0x20;                       // +0x20
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
    u32 field_0x1DC;                      // +0x1DC
    u32 field_0x1E0;                      // +0x1E0
    u32 field_0x1E4;                      // +0x1E4
    u32 field_0x1E8;                      // +0x1E8
};

// In-unit helpers (bodies recovered later; noinline keeps the call sites as
// real `bl` relocs).
extern "C" __declspec(noinline) void func_80498A5C(
    CScnChild80496B0C* child, ml::CVec3* outA = 0, ml::CVec3* outB = 0,
    ml::CVec3* outC = 0, u32 nodeIdx = 0);
extern "C" __declspec(noinline) u32 func_80497544(
    CScn80496B0C* self, CScnChild80496B0C* child, CScnItemAnim* anim,
    nw4r::g3d::ResAnmChr* anmRes, u32 c, u32 e, s32 f);

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
    CScnChild80496B0C* a = self->field_0x8;
    CScnChild80496B0C* b = self->field_0xC;
    CScnNode80496B0C* na;
    CScnNode80496B0C* nb;
    if ((na = a->field_0x84[0]) == 0 || (nb = b->field_0x84[0]) == 0) return 0;
    return nb->field_0x14 != na->field_0x14;
}

// Returns whether both chains are complete (nodes present, no +0x88 child),
// the two nodes share a +0x18 tag, and their +0x14 ids match.
u32 func_80497914(CScn80496B0C* self) {
    CScnChild80496B0C* a = self->field_0x8;
    CScnChild80496B0C* b = self->field_0xC;
    if (a->field_0x84[0] == 0 || b->field_0x84[0] == 0) return 0;
    if (a->field_0x84[1] != 0 || b->field_0x84[1] != 0) return 0;
    if (b->field_0x84[0]->field_0x18 != a->field_0x84[0]->field_0x18) return 0;
    return a->field_0x84[0]->field_0x14 == b->field_0x84[0]->field_0x14;
}

extern "C" u32 func_8049798C(u8* self) { return *(u32*)(*(u32*)((u8*)self + 0xC) + 0x84) != 0; }

// Refreshes the child's TRS outputs from the model node selected by nodeIdx:
// the translate output receives the node's position (or zero when the node is
// absent), and the other two outputs are zeroed. When a node is already bound,
// the child is released instead and the outputs are left untouched.
void func_804979A4(CScn80496B0C* self, ml::CVec3* outA, ml::CVec3* outB,
                   ml::CVec3* outC, u32 nodeIdx) {
    if (self->field_0x8->field_0x84[0] != 0) {
        func_80498A5C(self->field_0x8);
        return;
    }
    nw4r::g3d::ResNode node = self->field_0x1D0.GetResNode(nodeIdx);
    if (node.IsValid()) {
        // The inner check mirrors the retail DB_ASSERT on the same validity
        // test: MWCC reuses the outer compare and leaves the Panic block as
        // unreachable fall-through.
        if (!node.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639E0);
        }
        *outC = ml::CVec3(node.GetTranslate().x, node.GetTranslate().y,
                          node.GetTranslate().z);
    } else {
        outC->set(lbl_eu_8066AAC0, lbl_eu_8066AAC0, lbl_eu_8066AAC0);
    }
    outA->set(lbl_eu_8066AAC0, lbl_eu_8066AAC0, lbl_eu_8066AAC0);
    outB->set(lbl_eu_8066AAC0, lbl_eu_8066AAC0, lbl_eu_8066AAC0);
}

void func_80497AA8(){}

// Child-object initializer: zeroes the three node-binding slots and the
// anim/scale state, then stamps the per-slot ids 0/1/2. C linkage keeps the
// retail reloc name verbatim (the ctor call sites use the unmangled name).
extern "C" CScnChild80496B0C* func_80497F34(CScnChild80496B0C* self) {
    // Pointer-walk do-while with the increment inside the condition: the
    // SDK loop shape MWCC emits without unrolling (MWCC_REFERENCE).
    CScnChildElem80496B0C* it = self->elems;
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

// Detaches the node matching `tag` (or the first bound node when tag == 0)
// from the child's chr-blend, releases it, then shifts the remaining binding
// slots down to close the gap (and re-examines the slot, which now holds the
// next node).
extern "C" __declspec(noinline) void func_804980E4(CScnChild80496B0C* child,
                                                   u32 tag) {
    CScnChild80496B0C* walk = child;
    CScnNode80496B0C* node;
    u32 i = 0;
    while (i < 3) {
        node = walk->field_0x84[0];
        if (node == 0) break;
        if (node->field_0x18 == tag || tag == 0) {
            if (node->field_0x10 != 0) {
                child->field_0x98->Detach(child->field_0x90 * 3 +
                                          node->field_0x0);
                child->field_0x92--;
                node->field_0x10->v11();
                ((nw4r::g3d::G3dObj*)node->field_0x10)->Destroy();
                node->field_0x10 = 0;
            }
            node->field_0x18 = 0;
            // shift the remaining binding slots down one, clearing each
            // vacated slot as we go (the last iteration clears slot 2)
            for (u32 j = i; j < 2; j++) {
                child->field_0x84[j] = child->field_0x84[j + 1];
                child->field_0x84[j + 1] = 0;
            }
        } else {
            walk = (CScnChild80496B0C*)((u8*)walk + 4);
            i++;
        }
    }
}

extern "C" __declspec(noinline) u32 func_80498288(
    CScnChild80496B0C* child, nw4r::g3d::ResAnmChr anmRes,
    nw4r::g3d::AnmObjChrRes* anmObj, CScnItemAnim* anim, u32 c, u32 e) {
    return 0;
}

void func_804986F8(){}

// Accumulates each bound node's scaled translate result into outB/outC (and
// the blend offset delta into outA), then syncs the stored blend reference
// from outB. The per-node flag in field_0x2 selects which outputs receive
// the translate components.
extern "C" __declspec(noinline) void func_80498A5C(
    CScnChild80496B0C* child, ml::CVec3* outA, ml::CVec3* outB,
    ml::CVec3* outC, u32 nodeIdx) {
    outA->set(lbl_eu_8066AAC0, lbl_eu_8066AAC0, lbl_eu_8066AAC0);
    outB->set(lbl_eu_8066AAC0, lbl_eu_8066AAC0, lbl_eu_8066AAC0);
    outC->set(lbl_eu_8066AAC0, lbl_eu_8066AAC0, lbl_eu_8066AAC0);
    nw4r::g3d::ChrAnmResult result;
    nw4r::math::VEC3 t;
    CScnNode80496B0C* node;
    int i = 0;
    do {
        node = child->field_0x84[i];
        if (node == 0) break;
        ((nw4r::g3d::AnmObjChr*)node->field_0x10)->GetResult(&result, nodeIdx);
        result.GetTranslate(&t);
        node = child->field_0x84[i];
        f32 scale = node->field_0x20;
        t.x *= scale;
        t.y *= scale;
        t.z *= scale;
        if (i == 0) {
            node = child->field_0x84[i];
            if (node->field_0x2 & 1) {
                outB->x += t.x;
                outC->y += t.y;
                outB->z += t.z;
            } else {
                outC->x += t.x;
                outC->y += t.y;
                outC->z += t.z;
            }
        } else {
            node = child->field_0x84[i];
            if (node->field_0x2 & 1) {
                outC->y += t.y;
            } else {
                outC->x += t.x;
                outC->y += t.y;
                outC->z += t.z;
            }
        }
        i++;
    } while (i < 3);
    node = child->field_0x84[0];
    if (node != 0 && (node->field_0x2 & 1)) {
        if (child->field_0x96 != 0) {
            child->field_0x96 = 0;
            child->field_0x9C = *outB;
        } else {
            outA->x = outB->x - child->field_0x9C.x;
            outA->z = outB->z - child->field_0x9C.z;
            child->field_0x9C = *outB;
        }
    }
}

// Tears down both children's node bindings (same release sequence as the child
// destructor, inlined for the two embedded children) and destroys the shared
// chr-blend object.
void func_80496D74(CScn80496B0C* self) {
    // Locals declared so MWCC allocates node, walk, counter to r26/r27/r28.
    CScnNode80496B0C* node;
    CScn80496B0C* walk;
    u32 i;
    walk = self;
    i = 0;
    do {
        node = walk->field_0x10.field_0x84[0];
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
            walk->field_0x10.field_0x84[0] = 0;
            self->field_0x10.field_0x9C = ml::CVec3::zero;
        }
        i++;
        walk = (CScn80496B0C*)((u8*)walk + 4);
    } while (i < 3);
    walk = self;
    i = 0;
    do {
        node = walk->field_0xC0.field_0x84[0];
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
            walk->field_0xC0.field_0x84[0] = 0;
            self->field_0xC0.field_0x9C = ml::CVec3::zero;
        }
        i++;
        walk = (CScn80496B0C*)((u8*)walk + 4);
    } while (i < 3);
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
        nw4r::g3d::ResAnmChr anm(node->field_0x10->field_0x2C);
        if (!anm.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E9D0, 0x27, lbl_eu_8056E9B4,
                            lbl_eu_8056E9A8, lbl_eu_806639E4);
        }
        nw4r::g3d::ResDic dic;
        if (anm.ref().toChrDataDic != 0) {
            u8* pDic = (u8*)anm.ptr() + anm.ref().toChrDataDic;
            if (((u32)pDic & 3) != 0) {
                nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
            }
            dic = nw4r::g3d::ResDic(pDic);
        } else {
            dic = nw4r::g3d::ResDic(NULL);
        }
        s32 idx = dic.GetIndex(name);
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
// Binds the chr animation from the item-anim to both children (child2 with a
// forced zero flag, child1 with the caller's), packing the two results into a
// u32 (high word from child2, low word from child1); then re-arms the model
// node release for the first bound slot.
// (func_80497544 is the in-unit setup helper; its body is recovered later.)
// Builds a fresh AnmObjChrRes from the caller's resource and binds it to the
// node selected by child->field_0x90 (two-arg Bind when the model id is
// valid), hands it to func_80498288 to replace any existing binding, syncs
// the animation frame from the bound node, and returns func_80498288's
// result. A nonzero `f` overrides the frame with the raw value.
extern "C" __declspec(noinline) u32 func_80497544(
    CScn80496B0C* self, CScnChild80496B0C* child, CScnItemAnim* anim,
    nw4r::g3d::ResAnmChr* anmRes, u32 c, u32 e, s32 f) {
    u32 size;
    nw4r::g3d::ResMdl mdl = self->field_0x1D0;
    nw4r::g3d::ResAnmChr chr = *anmRes;
    nw4r::g3d::AnmObjChrRes* anmObj = nw4r::g3d::AnmObjChrRes::Construct(
        (MEMAllocator*)func_8048ECFC(self->field_0x0), &size, chr, mdl,
        false);
    if (child->field_0x90 != 0) {
        if (self->field_0x1D8 != 0xFFFFFFFF) {
            anmObj->Bind(self->field_0x1D0, self->field_0x1D8,
                         nw4r::g3d::AnmObjChr::BIND_PARTIAL);
        } else {
            anmObj->Bind(self->field_0x1D0);
        }
    } else {
        anmObj->Bind(self->field_0x1D0);
    }
    u32 result = func_80498288(child, *anmRes, anmObj, anim, c, e);
    if (self->field_0x17E == 0) {
        self->field_0x1D4->v11(self->field_0x1CC, 6);
        self->field_0x17E = 1;
    }
    if (f == 0) {
        CScnNode80496B0C* node = child->field_0x84[0];
        f32 frame = (node != 0) ? node->field_0x1C : lbl_eu_8066AAC0;
        if (node != 0) {
            ((nw4r::g3d::AnmObjChr*)node->field_0x10)
                ->SetFrame(frame - lbl_eu_8066AAC4);
        }
    } else if (f > 0) {
        CScnNode80496B0C* node = child->field_0x84[0];
        if (node != 0) {
            ((nw4r::g3d::AnmObjChr*)node->field_0x10)->SetFrame((float)f);
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
    u32 r = func_80497544(self, self->field_0x8, anim, &t2, c, d, f);
    result |= r & 0xFFFF;
    if (self->field_0xC->field_0x84[0] != 0 && self->field_0x1D8 != 0xFFFFFFFF) {
        CScnChild80496B0C* child = self->field_0x8;
        for (u32 i = 0; i < 3; i++) {
            CScnNode80496B0C* node = child->field_0x84[i];
            if (node != 0) {
                node->field_0x10->v19(nw4r::g3d::ResMdl(child->field_0xA8),
                                      self->field_0x1D8, 1);
                break;
            }
        }
    }
    return result;
}

// Binds the chr animation to whichever child is free (child2 when node1 is
// still bound and its flag is clear, child1 otherwise), transferring node1's
// current frame to node2. Returns a packed result: high word = child2 setup,
// low word = child1 setup.
extern "C" u32 func_804972E8(CScn80496B0C* self, CScnItemAnim* anim,
                             int index, u32 c, s32 f) {
    CScnNode80496B0C* node;
    u32 result = 0;
    self->field_0x170 = (float)c;
    if (self->field_0x1D8 != 0xFFFFFFFF && self->field_0xC->field_0x84[0] == 0 &&
        self->field_0x8->field_0x84[0] != 0 &&
        !((self->field_0x8->field_0x84[0]->field_0x2 >> 2) & 1)) {
        node = self->field_0x8->field_0x84[0];
        result = func_80497544(
                     self, self->field_0xC,
                     (CScnItemAnim*)node->field_0x18,
                     (nw4r::g3d::ResAnmChr*)&node->field_0x14, 0, 0, -1)
                 << 16;
        f32 frame;
        node = self->field_0x8->field_0x84[0];
        if (node != 0) {
            frame = ((nw4r::g3d::AnmObjChr*)node->field_0x10)->GetFrame();
        } else {
            frame = lbl_eu_8066AAC0;
        }
        node = self->field_0xC->field_0x84[0];
        if (node != 0) {
            ((nw4r::g3d::AnmObjChr*)node->field_0x10)->SetFrame(frame);
        }
        if (self->field_0x1D8 != 0xFFFFFFFF) {
            for (u32 i = 0; i < 3; i++) {
                node = self->field_0x8->field_0x84[i];
                if (node != 0) {
                    node->field_0x10->v19(
                        nw4r::g3d::ResMdl(self->field_0x8->field_0xA8),
                        self->field_0x1D8, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
                    break;
                }
            }
        }
        self->field_0x170 = lbl_eu_8066AAC0;
    }
    node = self->field_0x8->field_0x84[0];
    if (node != 0 && !((node->field_0x2 >> 2) & 1)) {
        nw4r::g3d::ResAnmChr anm = func_8049E708(anim, index);
        result = func_80497544(self, self->field_0xC, anim, &anm, c, 0, f)
                 << 16;
    } else {
        nw4r::g3d::ResAnmChr anm = func_8049E708(anim, index);
        u32 r = func_80497544(self, self->field_0x8, anim, &anm, c, 0, f);
        result |= r & 0xFFFF;
        node = self->field_0x8->field_0x84[0];
        if (node != 0) {
            node->field_0x2 |= 0x4;
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
    obj->field_0x1DC = 0;
    obj->field_0x1E0 = 0;
    obj->field_0x1E4 = 0;
    obj->field_0x1E8 = 0;
    obj->field_0x8 = &obj->field_0x10;
    obj->field_0xC = &obj->field_0xC0;
    return obj;
}

// Destructor: tears down both embedded children (reverse declaration order -
// field_0xC0 first, then field_0x10), then frees the object when the MWCC
// delete flag is set.
CScn80496B0C* __dt__80496BB4(CScn80496B0C* self, int flags) {
    CScnNode80496B0C* node;
    CScn80496B0C* walk;
    u32 i;
    if (self != 0) {
        if (&self->field_0xC0 != 0) {
            walk = self;
            i = 0;
            do {
                node = walk->field_0xC0.field_0x84[0];
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
                    walk->field_0xC0.field_0x84[0] = 0;
                    self->field_0xC0.field_0x9C = ml::CVec3::zero;
                }
                i++;
                walk = (CScn80496B0C*)((u8*)walk + 4);
            } while (i < 3);
        }
        if (&self->field_0x10 != 0) {
            i = 0;
            do {
                node = self->field_0x10.field_0x84[i];
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
                i++;
            } while (i < 3);
        }
        if (flags > 0) {
            ::operator delete(self);
        }
    }
    return self;
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
    nw4r::g3d::AnmObjChrBlend* bBlend = self->field_0x1CC;
    nw4r::g3d::ResMdlData* bResData = self->field_0x1D0.ptr();
    CScnChild80496B0C* b = self->field_0xC;
    b->field_0xAC = self;
    b->field_0x98 = bBlend;
    b->field_0x94 = slot;
    b->field_0x90 = 1;
    b->field_0xA8 = bResData;
}
