// Auto-scaffolded catalog TU for monolib/src/mpfsys/code_8047BB54
// Replace stubs with high-level C/C++ during decomp.

#include "monolib/mpfsys/code_8047BB54.hpp"
#include "monolib/mpfsys/UnkClass_80471EC8.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/mpfsys/MPFDrawBillboard.hpp"
#include "monolib/mpfsys/MPFDrawBillLayTex.hpp"
#include "monolib/mpfsys/MPFDrawCross.hpp"
#include "monolib/mpfsys/MPFDrawDisplayList.hpp"
#include "monolib/mpfsys/MPFDrawMdlColor.hpp"
#include "monolib/mpfsys/MPFDrawMdlNoColor.hpp"
#include "monolib/mpfsys/code_8047CA88.hpp"
#include "monolib/util/MemManager.hpp"
#include <cstring>
#include <nw4r/math.h>
#include <nw4r/g3d/g3d_obj.h>
#include <nw4r/g3d/g3d_scnobj.h>
#include <nw4r/g3d/g3d_scnproc.h>
#include <nw4r/g3d/res/g3d_resfile.h>
#include <harness_catalog.h>

// ---------------------------------------------------------------------------
// Layout overlay + imports. The class in code_8047BB54.hpp carries no fields
// yet, so field accesses go through this overlay (identical single-inheritance
// layout to the retail object).
// ---------------------------------------------------------------------------

// Scene object base (declared in monolib/src/scn/CScnMem.hpp).
class CScn;

// Opaque scene-data object held by the global singleton slot
// (lbl_eu_806658B8) and by field_0x14 of the class below. The singleton slot
// is (re)allocated as a 0xFA000-byte, 32-aligned block by __dt__8047BFA8;
// only pointer identity is used in this TU.
struct UnkSceneDataObj {
    u32 field_0x0;
    ~UnkSceneDataObj() {}
};

// Global singleton slot (retail .sbss 0x806658B8).
extern UnkSceneDataObj* lbl_eu_806658B8;

// Overlay for the ScnObj returned by func_8048EC14: the destructor probes
// the flag at +0xE4 before removing the object from its parent group.
struct UnkScnObjFlagE4 {
    u8 pad[0xE4];
    u32 field_0xE4;
};

// Singleton draw-object table filled by sinit_8047CA2C (retail .bss
// 0x806584E8, six pointers).
struct MPFDrawSingletonTable {
    mpfsys::MPFDrawBillboard* billboard;     // +0x00
    mpfsys::MPFDrawCross* cross;             // +0x04
    mpfsys::MPFDrawDisplayList* displayList; // +0x08
    mpfsys::MPFDrawMdlNoColor* mdlNoColor;   // +0x0C
    mpfsys::MPFDrawMdlColor* mdlColor;       // +0x10
    mpfsys::MPFDrawBillLayTex* billLayTex;   // +0x14
};
extern MPFDrawSingletonTable lbl_eu_806584E8;

// Global scratch draw context returned by func_8047C034 (retail .bss
// 0x80658500, 0x18 bytes). func_8047C588 / func_8047C904 fill it from the
// map descriptor before issuing the per-layer draw calls.

// One 0x34-byte entry in the layer array (obj->field_0x10). field_0x0 is the
// sub-element count: the mark walk treats entry[0..field_0x0] of the same
// array as this entry's sub-elements.
struct UnkDrawEntry {
    u16 field_0x0;        // +0x00 sub-element count (mark-loop bound)
    u16 field_0x2;        // +0x02 index into the singleton draw table
    f32 field_0x4;        // +0x04
    f32 field_0x8;        // +0x08
    f32 field_0xC;        // +0x0C
    f32 field_0x10;       // +0x10
    f32 field_0x14;       // +0x14
    u32 field_0x18;       // +0x18 offset added to obj->field_0x0
    u8 field_0x1C;        // +0x1C group id (bitmask table index)
    u8 field_0x1D;        // +0x1D
    u16 field_0x1E;       // +0x1E layer flag (bit 0)
    u8 field_0x20[0x34 - 0x20];
};

struct UnkDrawObj {
    u32 field_0x0;        // +0x00 map descriptor pointer
    u32* field_0x4;       // +0x04 mark array ([0] = per-element stride)
    u16 field_0x8;        // +0x08 element count (halfword)
    u8 pad_0xA[2];
    u8* field_0xC;        // +0x0C resolved relative pointer
    UnkDrawEntry* field_0x10; // +0x10 per-layer entry array (0x34-byte stride)
    u8* field_0x14;       // +0x14 resolved relative pointer
};

// Map descriptor (the object pointed to by field_0xC of UnkClass_8047BB54).
// Several fields are relative offsets resolved against the descriptor base.
struct UnkDesc8047BB54 {
    u32 field_0x0;        // +0x00 magic / id
    u32 field_0x4;        // +0x04
    u32 field_0x8;        // +0x08 relative offset -> obj->field_0x10
    u32 field_0xC;        // +0x0C element count
    u32 field_0x10;       // +0x10 display-list sub-count
    u32 field_0x14;       // +0x14
    u32 field_0x18;       // +0x18
    u32 field_0x1C;       // +0x1C relative offset -> obj->field_0x14
    u32 field_0x20;       // +0x20
    u32 field_0x24;       // +0x24 relative offset to a u32 slot
    u32 field_0x28;       // +0x28 mark-loop gate count
    u32 field_0x2C;       // +0x2C
    u32 field_0x30;       // +0x30 flag (bit 0 = xlu layers)
};

// The u32 slot reached through desc->field_0x24 holds another relative offset
// (-> obj->field_0xC) plus a float at +0x10 (used by func_8047C588).
struct UnkDescSlot24 {
    u32 field_0x0;        // +0x00 second-level relative offset
    u8 pad_0x4[0x10 - 0x4];
    f32 field_0x10;       // +0x10
};

// Buffer header written by __dt__8047BC74 at field_0x14.
struct UnkBuf8047BB54 {
    u32 field_0x0;        // +0x00 (a - headerSize)
};

// View frame returned by func_8049626C; +0x9C is handed to the layer draws,
// +0x1E0 holds the layer scale.
struct UnkViewFrame {
    u8 pad_0x0[0x9C];
    u8 field_0x9C[0x1E0 - 0x9C];
    f32 field_0x1E0;
};

// Scene-root child object: the byte at +0x19 gates the map callback.
struct CScnRootNw4rGate {
    u8 pad[0x19];
    u8 field_0x19;
};

// MPF draw-object vtable mirrors (MWCC vtables carry an 8-byte RTTI header,
// so declared slot 0 sits at vtable+0x8, slot 1 at vtable+0xC).
struct MPFDrawVt8 {
    virtual void vf0(void* arg) = 0;
};
struct MPFDrawVtC {
    virtual void vf0(void* arg) = 0;
    virtual void vf1(void* arg) = 0;
};

class CView;

// Forward declarations of this TU's target functions (defined below with C
// linkage so the retail names stay verbatim at call sites).
extern "C" void __dt__8047BC74(UnkClass_8047BB54* self, u32 sizeA, u32 sizeB);
extern "C" void func_8047BE8C__17UnkClass_8047BB54Fv(
    nw4r::g3d::ScnProc* proc, bool opa);
extern "C" void func_8047C588__17UnkClass_8047BB54Fv(
    UnkDrawObj* obj, UnkDesc8047BB54* desc, void* a5, void* a6, void* a7,
    void* a8, void* a9, u32 a10, f32 f1);
extern "C" void func_8047C904__17UnkClass_8047BB54Fv(
    UnkDrawObj* obj, UnkDesc8047BB54* desc, void* a5, void* a6, void* a7,
    void* a8, void* a9, u32 a10, f32 f1);

// ---------------------------------------------------------------------------
// Retail-named imports. Retail keeps the Fv mangling but the functions
// actually receive extra args; declaring them with the exact mangled name
// (global scope keeps the name verbatim) matches the call-site relocs.
// ---------------------------------------------------------------------------
extern "C" int func_80471EC8__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, void* a4, void* a5, void* a6, u32 a7);
extern "C" void* func_80471FCC__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, void* a4, void* a5, void* a6, u32 a7);
extern "C" void func_80472064__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, void* a4, void* a5, u32 a6);
extern "C" void func_8047230C__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, f32 f1);
extern "C" void func_80473984__Q26mpfsys17UnkClass_80471EC8Fv(
    void* self, void* a4, void* a5, void* a6, void* a7, u32 a8);
extern "C" void func_80474EB0__Q26mpfsys17UnkClass_80471EC8Fv();
extern "C" void func_8047958C__Q26mpfsys18MPFDrawDisplayListFv(
    mpfsys::MPFDrawDisplayList* self, void* arg);
extern "C" void func_804795A4__Q26mpfsys18MPFDrawDisplayListFv(
    mpfsys::MPFDrawDisplayList* self, void* arg);
extern "C" void* func_8048ECEC(CScn* self);
extern "C" u32 func_8048ECD0(CScn* self);
extern "C" void func_8047CC4C__17UnkClass_8047CA88Fv(
    UnkClass_8047CA88* self, void* desc, void* dataPtr,
    nw4r::math::VEC3* vec, f32 scale);
extern "C" CView* getCurrentView__5CViewFv();
extern "C" void* func_8049698C(CView* view);
extern "C" void* func_8049626C(void* camera, CView* view);

// .sdata2 float for the layer-scale setup (func_8047230C).
extern const f32 lbl_eu_8066A858;
// Embedded subobject pointer cache (this+0x24) written by func_8047BE8C.
extern void* lbl_eu_80665838;
// Draw-description VEC3 buffers handed to func_8047CC4C (retail .bss).
extern f32 lbl_eu_80658410[3];
extern f32 lbl_eu_8065841C[3];

// Object layout recovered from the retail destructor/accessor bodies.
struct UnkClass_8047BB54Layout {
    /* 0x00 */ ml::CVec3 mPos;                    // position
    /* 0x0C */ UnkDesc8047BB54* field_0xC;        // map descriptor
    /* 0x10 */ u32 field_0x10;                    // flags (bit0: ResFile live)
    /* 0x14 */ UnkSceneDataObj* field_0x14;       // scratch buffer
    /* 0x18 */ u8* field_0x18;                    // buffer + sizeA
    /* 0x1C */ u8* field_0x1C;                    // buffer + (sizeA - headerSize)
    /* 0x20 */ nw4r::g3d::ResFile mResFile;
    /* 0x24 */ u8 gap_0x24[0x2E2C - 0x24];        // embedded UnkClass_80471EC8
    /* 0x2E2C */ CScn* field_0x2E2C;
    /* 0x2E30 */ u8 gap_0x2E30[8];
    /* 0x2E38 */ nw4r::g3d::G3dObj* field_0x2E38;
};

// Scene-root child lookup defined in monolib/src/scn/CScnMem.cpp; retail kept
// the unmangled name func_8048EC14, so C linkage is required for the reloc.
extern "C" nw4r::g3d::ScnObj* func_8048EC14(CScn* self, u32 idx);

// MWCC's operator-delete runtime symbol; C linkage keeps the unmangled name
// (same pattern as CNBanner.cpp).
extern "C" void __dl__FPv(void* p);

extern "C" void func_8047BD84__17UnkClass_8047BB54Fv(u8* self) {
    ((void(*)(void*))func_80472870__Q26mpfsys17UnkClass_80471EC8Fv)((char*)self + 0x24);
}

extern "C" void func_8047BD7C__17UnkClass_8047BB54Fv(u8* self, float val) {
    *(float*)((u8*)self + 0x2e14) = val;
}

extern "C" void func_8047BD8C__17UnkClass_8047BB54Fv(u8* self) {
    ((void(*)(void*))func_8047233C__Q26mpfsys17UnkClass_80471EC8Fv)((char*)self + 0x24);
}

extern "C" void func_8047BD94__17UnkClass_8047BB54Fv(u8* self) {
    ((void(*)(void*))func_80472370__Q26mpfsys17UnkClass_80471EC8Fv)((char*)self + 0x24);
}

extern "C" void __dt__8047BDA8(UnkClass_8047BB54* obj);
void UnkClass_8047BB54::func_8047BD9C(void) { __dt__8047BDA8(this); }

extern "C" void func_8047BDA0__17UnkClass_8047BB54Fv(u8* self) {
    ((void(*)(void*))func_80472864__Q26mpfsys17UnkClass_80471EC8Fv)((char*)self + 0x24);

}

// --- Target 2: us-8047ffcc ---------------------------------------------------
// Global singleton cleanup: free the slot object and clear the pointer.
void __dt__8047BFFC() {
    if (lbl_eu_806658B8) {
        delete lbl_eu_806658B8;
    }
    lbl_eu_806658B8 = NULL;
}

// --- Target 3: us-8047ff78 ---------------------------------------------------
// Global singleton re-init: free the old slot object, then allocate a fresh
// 0xFA000-byte, 32-aligned block from the MEM1 region. The double null check
// mirrors retail's delete-expansion shape (MWCC emits the freeing call under
// two guards so the slot store stays inside the conditional).
void __dt__8047BFA8() {
    if (lbl_eu_806658B8) {
        if (lbl_eu_806658B8) {
            __dl__FPv(lbl_eu_806658B8);
            lbl_eu_806658B8 = NULL;
        }
    }
    lbl_eu_806658B8 = (UnkSceneDataObj*)mtl::MemManager::allocate_array_ex(
        0xFA000, mtl::MemManager::getHandleMEM1(), 0x20);
}

// --- Target 5: us-8047fd78 ---------------------------------------------------
// Task-block destructor: reset the position, tear down the embedded ResFile
// when its flag is set, free the scratch buffer unless the global scene is
// present, then detach + destroy the G3D object held at +0x2E38.
extern "C" void __dt__8047BDA8(UnkClass_8047BB54* obj) {
    UnkClass_8047BB54Layout* self = (UnkClass_8047BB54Layout*)obj;
    self->mPos = ml::CVec3::zero;
    self->field_0xC = 0;
    if (self->field_0x10 & 1) {
        self->mResFile.Terminate();
    }
    self->field_0x10 = 0;
    if (lbl_eu_806658B8 == NULL && self->field_0x14 != NULL) {
        delete self->field_0x14;
        self->field_0x14 = NULL;
    }
    self->field_0x14 = NULL;
    if (self->field_0x2E38 != NULL) {
        nw4r::g3d::ScnObj* scnObj = func_8048EC14(self->field_0x2E2C, 8);
        if (((UnkScnObjFlagE4*)scnObj)->field_0xE4 != 0) {
            ((nw4r::g3d::ScnGroup*)scnObj)->Remove(
                (nw4r::g3d::ScnObj*)self->field_0x2E38);
            self->field_0x2E38->Destroy();
        }
        self->field_0x2E38 = NULL;
    }
}

extern u8 lbl_eu_80658500[];
void* UnkClass_8047BB54::func_8047C034(void) { return lbl_eu_80658500; }

// ---------------------------------------------------------------------------
// func_8047C040 helpers (retail keeps the Fv mangling but the function
// actually receives view/count in r4/r5, so it is emitted with C linkage and
// the exact mangled name - same pattern as the other Fv-with-hidden-params
// functions in this TU).
// ---------------------------------------------------------------------------

// Layer-view descriptor: one 8-byte record per outer-loop iteration (retail
// advances the pointer by 8 each iteration).
struct UnkLayerView {
    u16 field_0x0;   // +0x00 layer entry index into obj->field_0x10
    u16 field_0x2;   // +0x02 element count (inner loop bound)
    u32 field_0x4;   // +0x04 element base index (x 0xC into obj->field_0xC)
};

// Tail of the embedded UnkClass_80471EC8 subobject (self+0x24) holding the
// two per-frame culling constants read by func_8047C040.
struct EmbeddedCullParams {
    u8 pad_0x0[0x2DFC];
    f32 field_0x2DFC;   // +0x2DFC
    f32 field_0x2E00;   // +0x2E00
};

// MPF draw-object vtable mirrors for the two-argument mark-flush virtuals:
// path C flushes via vtable slot 0 (+0x8), paths A/B via slot 1 (+0xC).
struct MPFDrawVt8_2 {
    virtual void vf0(void* arg, u32* slot) = 0;
};
struct MPFDrawVtC_2 {
    virtual void vf0(void* arg, u32* slot) = 0;
    virtual void vf1(void* arg, u32* slot) = 0;
};

// .sdata2 constants / bitmask-table pointer / warning strings used below.
extern const f32 lbl_eu_8066A860;
extern const f32 lbl_eu_8066A864;
extern const f32 lbl_eu_8066A868;
extern const f32 lbl_eu_80665880;
extern const f32 lbl_eu_80665884;
extern f32 lbl_eu_806638A0[2];  // defined in this TU (.sdata, dissolved monolibdata2)
extern u32* lbl_eu_80665864;
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// --- Target 1: us-80480010 ---------------------------------------------------
// Map-draw culling/marking pass. For each 8-byte layer-view record, resolve
// the layer entry, cull its VEC3 elements against a cone test (paired-single
// nw4r math), then walk the entry's sub-elements and append the surviving
// (layer, element) pairs to the mark array. When a slot fills, the matching
// draw-object virtual (slot 0 for the near branch, slot 1 otherwise) flushes
// it. The object's first 0x14 bytes are the UnkDrawObj layout. Signature
// matches the canonical declaration in monolib/mpfsys/code_8047CA88.hpp.
extern "C" void func_8047C040__17UnkClass_8047BB54Fv(void* self, void* ptr,
                                         u32 count) {
    UnkDrawObj* obj = (UnkDrawObj*)self;
    UnkLayerView* view = (UnkLayerView*)ptr;
    const EmbeddedCullParams* subobj =
        (const EmbeddedCullParams*)lbl_eu_80665838;
    f32 f29 = subobj->field_0x2DFC;
    f32 f28 = subobj->field_0x2E00;
    nw4r::math::VEC3 diff;
    nw4r::math::VEC3 scratch;

    for (int i = 0; i < count; i++) {
        UnkDrawEntry* entry = &obj->field_0x10[view->field_0x0];
        u8 group = entry->field_0x1C;
        if (lbl_eu_80665864[group >> 5] & (1u << (group & 0x1F))) continue;

        const nw4r::math::VEC3* elems = (const nw4r::math::VEC3*)(
            obj->field_0xC + (u32)view->field_0x4 * 0xC);
        f32 radiusSq = entry->field_0xC * entry->field_0xC;
        f32 f1v = entry->field_0x10;
        f32 f27v;
        if (f29 > f1v) {
            f1v = f29;
            f27v = f28;
        } else {
            f27v = entry->field_0x14;
        }
        f32 f24 = f1v * f1v;
        f32 f23 = f27v * f27v;
        f32 f26 = lbl_eu_8066A860 / (f1v - f27v);
        int n = entry->field_0x0;

        for (int j = 0; j < view->field_0x2; j++) {
            nw4r::math::VEC3Sub(&diff, &elems[j],
                                (const nw4r::math::VEC3*)lbl_eu_80658410);
            f32 dist2 = nw4r::math::VEC3Dot(&diff, &diff);
            if (dist2 > radiusSq) {
                f32 dot = nw4r::math::VEC3Dot(
                    &diff, (const nw4r::math::VEC3*)lbl_eu_8065841C);
                if (dot < lbl_eu_8066A864) continue;
                f32 t = dot * *(volatile const f32*)&lbl_eu_80665880;
                nw4r::math::VEC3Scale(
                    &scratch, (const nw4r::math::VEC3*)lbl_eu_8065841C, t);
                f32 delta = t * *(volatile const f32*)&lbl_eu_80665884;
                f32 radSq = (entry->field_0xC + delta) *
                            (entry->field_0xC + delta);
                nw4r::math::VEC3Sub(&scratch, &diff, &scratch);
                if (nw4r::math::VEC3Dot(&scratch, &scratch) > radSq) {
                    continue;
                }
            }

            for (int k = 0; k <= n; k++) {
                UnkDrawEntry* sub = &entry[k];
                if (sub->field_0x8 <= dist2) continue;
                if (dist2 >= f24) {
                    f32 d = sub->field_0x4 - lbl_eu_8066A868;
                    if (dist2 <= d * d) {
                        // Near branch: append without extra data.
                        u32* slot =
                            &obj->field_0x4[obj->field_0x4[0] *
                                            (view->field_0x0 + k) +
                                            1];
                        slot[slot[0]] = view->field_0x4 + j;
                        slot[0] += 1;
                        if (slot[0] >= obj->field_0x4[0]) {
                            void* draw =
                                ((void**)&lbl_eu_806584E8)[sub->field_0x2];
                            ((MPFDrawVt8_2*)draw)->vf0(
                                (u8*)obj->field_0x0 + sub->field_0x18, slot);
                            slot[0] = 1;
                        }
                        break;
                    }
                    // Far branch: append with a scaled height value.
                    f32 t2;
                    if (dist2 < lbl_eu_8066A864) {
                        nw4r::db::Warning(lbl_eu_80526324, 0x273,
                                          lbl_eu_80526300);
                    }
                    t2 = dist2 <= lbl_eu_8066A864
                             ? lbl_eu_8066A864
                             : dist2 * nw4r::math::FrSqrt(dist2);
                    u32* slot = &obj->field_0x4[obj->field_0x4[0] *
                                                (obj->field_0x8 + k +
                                                 view->field_0x0) +
                                                1];
                    slot[slot[0]] = view->field_0x4 + j;
                    *(f32*)&slot[slot[0] + 1] =
                        (sub->field_0x4 - t2) *
                        *(volatile const f32*)&lbl_eu_806638A0;
                    slot[0] += 2;
                    if (slot[0] >= obj->field_0x4[0] - 2) {
                        void* draw =
                            ((void**)&lbl_eu_806584E8)[sub->field_0x2];
                        ((MPFDrawVtC_2*)draw)->vf1(
                            (u8*)obj->field_0x0 + sub->field_0x18, slot);
                        slot[0] = 1;
                    }
                    break;
                }
                if (dist2 <= f23) break;
                // Middle branch: append with a projected height value.
                f32 t1;
                if (dist2 < lbl_eu_8066A864) {
                    nw4r::db::Warning(lbl_eu_80526324, 0x273,
                                      lbl_eu_80526300);
                }
                t1 = dist2 <= lbl_eu_8066A864
                         ? lbl_eu_8066A864
                         : dist2 * nw4r::math::FrSqrt(dist2);
                u32* slot = &obj->field_0x4[obj->field_0x4[0] *
                                            (obj->field_0x8 + k +
                                             view->field_0x0) +
                                            1];
                slot[slot[0]] = view->field_0x4 + j;
                *(f32*)&slot[slot[0] + 1] = f26 * (t1 - f27v);
                slot[0] += 2;
                if (slot[0] >= obj->field_0x4[0] - 2) {
                    void* draw =
                        ((void**)&lbl_eu_806584E8)[sub->field_0x2];
                    ((MPFDrawVtC_2*)draw)->vf1(
                        (u8*)obj->field_0x0 + sub->field_0x18, slot);
                    slot[0] = 1;
                }
                break;
            }
        }
        view++;
    }
}

// --- Target 1: us-8047fb24 ---------------------------------------------------
// Map-attach entry: tear down the previous state, ask the embedded
// UnkClass_80471EC8 to (re)attach the descriptor + ResFile, and on success
// allocate the draw buffer, create the ScnProc callback node and append it
// to the scene root group.
extern "C" int func_8047BB54__17UnkClass_8047BB54Fv(UnkClass_8047BB54* self,
                                         UnkDesc8047BB54* desc,
                                         CScn* cscn, u32 a6) {
    UnkClass_8047BB54Layout* layout = (UnkClass_8047BB54Layout*)self;
    __dt__8047BDA8(self);
    if (func_80471EC8__Q26mpfsys17UnkClass_80471EC8Fv(
            (mpfsys::UnkClass_80471EC8*)layout->gap_0x24, desc,
            &layout->mResFile, cscn, a6) != 0) {
        layout->field_0xC = desc;
        layout->field_0x10 = 0x15;
        __dt__8047BC74(self, 0x96000, 0x64000);
        func_8047230C__Q26mpfsys17UnkClass_80471EC8Fv(
            (mpfsys::UnkClass_80471EC8*)layout->gap_0x24, lbl_eu_8066A858);
        u32 size;
        nw4r::g3d::ScnProc* proc = nw4r::g3d::ScnProc::Construct(
            (MEMAllocator*)func_8048ECEC(cscn), &size,
            &func_8047BE8C__17UnkClass_8047BB54Fv, true, true, 0);
        layout->field_0x2E38 = proc;
        proc->SetUserData(self);
        nw4r::g3d::ScnObj* procCopy =
            (nw4r::g3d::ScnObj*)layout->field_0x2E38;
        nw4r::g3d::ScnGroup* group =
            (nw4r::g3d::ScnGroup*)func_8048EC14(cscn, 8);
        group->PushBack(procCopy);
        return 1;
    }
    return 0;
}

// --- Target 2: us-8047fc44 ---------------------------------------------------
// Buffer (re)allocation for the map draw: reuse the global scene-data slot
// when present, otherwise allocate a fresh sizeA+sizeB block and lay out the
// header. A failed allocation falls back to the full destructor.
extern "C" void __dt__8047BC74(UnkClass_8047BB54* self, u32 sizeA, u32 sizeB) {
    UnkClass_8047BB54Layout* layout = (UnkClass_8047BB54Layout*)self;
    if (lbl_eu_806658B8) {
        layout->field_0x14 = lbl_eu_806658B8;
    } else {
        if (layout->field_0x14) {
            if (layout->field_0x14) {
                __dl__FPv(layout->field_0x14);
                layout->field_0x14 = NULL;
            }
        }
        layout->field_0x14 = (UnkSceneDataObj*)mtl::MemManager::allocate_array_ex(
            sizeA + sizeB, mtl::MemManager::getHandleMEM1(), 0x20);
    }
    if (!layout->field_0x14) {
        __dt__8047BDA8(self);
    } else {
        layout->field_0x10 |= 2;
        memset(layout->field_0x14, 0, 0x2c);
        u32 headerSize = ((layout->field_0xC->field_0xC << 8) + 1) << 2;
        ((UnkBuf8047BB54*)layout->field_0x14)->field_0x0 = sizeA - headerSize;
        layout->field_0x1C = (u8*)layout->field_0x14 + (sizeA - headerSize);
        ((UnkBuf8047BB54*)layout->field_0x1C)->field_0x0 = 0x80;
        layout->field_0x18 = (u8*)layout->field_0x14 + sizeA;
        func_80472064__Q26mpfsys17UnkClass_80471EC8Fv(
            (mpfsys::UnkClass_80471EC8*)layout->gap_0x24, layout->field_0xC,
            (u8*)layout->field_0x14 + sizeA, sizeB);
    }
}

// --- Target 3: us-8047fe5c ---------------------------------------------------
// ScnProc draw callback: gate on the ResFile/active flag bits and the scene
// gate byte, then forward into the per-layer draws (func_8047C588 for opa,
// func_8047C904 for xlu).
extern "C" void func_8047BE8C__17UnkClass_8047BB54Fv(
    nw4r::g3d::ScnProc* proc, bool opa) {
    CView* view;
    void* vf;
    UnkClass_8047BB54Layout* self =
        (UnkClass_8047BB54Layout*)proc->GetUserData();
    if ((self->field_0x10 & 1) == 0) return;
    if ((self->field_0x10 & 4) == 0) return;
    if (((CScnRootNw4rGate*)func_8048ECD0(self->field_0x2E2C))->field_0x19 != 0)
        return;
    view = getCurrentView__5CViewFv();
    vf = func_8049626C(func_8049698C(view), view);
    lbl_eu_80665838 = (u8*)self + 0x24;
    if (opa) {
        void* frame = func_80471FCC__Q26mpfsys17UnkClass_80471EC8Fv(
            (mpfsys::UnkClass_80471EC8*)self->gap_0x24, self->field_0xC,
            self->field_0x14, self->field_0x18, (self->field_0x10 >> 4) & 1);
        f32 f = ((UnkViewFrame*)vf)->field_0x1E0;
        void* obj = func_8047C034__17UnkClass_8047BB54Fv(frame);
        func_8047C588__17UnkClass_8047BB54Fv(
            (UnkDrawObj*)obj, self->field_0xC, self->field_0x14,
            self->field_0x18, self->field_0x1C, (u8*)self + 0x20,
            ((UnkViewFrame*)vf)->field_0x9C, self->field_0x10 & 0x8, f);
    } else {
        f32 f = ((UnkViewFrame*)vf)->field_0x1E0;
        void* obj = func_8047C034__17UnkClass_8047BB54Fv(vf);
        func_8047C904__17UnkClass_8047BB54Fv(
            (UnkDrawObj*)obj, self->field_0xC, self->field_0x14,
            self->field_0x18, self->field_0x1C, (u8*)self + 0x20,
            ((UnkViewFrame*)vf)->field_0x9C, self->field_0x10 & 0x8, f);
    }
}

// --- Target 4: us-804808d4 ---------------------------------------------------
// Layer dispatch for the xlu pass: resolve the descriptor pointers into the
// scratch draw context, then walk the marked layers and invoke the draw
// table's second virtual slot for each live entry.
extern "C" void func_8047C904__17UnkClass_8047BB54Fv(
    UnkDrawObj* obj, UnkDesc8047BB54* desc, void* a5, void* a6, void* a7,
    void* a8, void* a9, u32 a10, f32 f1) {
    obj->field_0x0 = (u32)desc;
    obj->field_0x10 = (UnkDrawEntry*)((u8*)desc + desc->field_0x8);
    obj->field_0xC = (u8*)desc + *(u32*)((u8*)desc + desc->field_0x24);
    obj->field_0x14 = (u8*)desc + *(u32*)((u8*)desc + desc->field_0x1C);
    func_80473984__Q26mpfsys17UnkClass_80471EC8Fv(
        desc, a8, a9, a5, a6, a10);
    if (desc->field_0x28) {
        int count = (int)desc->field_0xC;
        for (int i = 0; i < count; i++) {
            if (obj->field_0x4[obj->field_0x4[0] * (i + count) + 1] > 1) {
                UnkDrawEntry* e = &obj->field_0x10[i];
                void* draw = ((void**)&lbl_eu_806584E8)[e->field_0x2];
                ((MPFDrawVtC*)draw)->vf1((u8*)obj->field_0x0 + e->field_0x18);
            }
        }
    }
    if (desc->field_0x10) {
        mpfsys::MPFDrawDisplayList* dl = mpfsys::MPFDrawDisplayList::getInstance();
        func_804795A4__Q26mpfsys18MPFDrawDisplayListFv(
            dl, (u8*)obj->field_0x0 + desc->field_0x10);
    }
    func_80474EB0__Q26mpfsys17UnkClass_80471EC8Fv();
}

// --- Target 5: us-80480558 ---------------------------------------------------
// Layer dispatch for the opa pass: like func_8047C904 but also marks the
// per-element array, forwards the scale to the UnkClass_8047CA88 singleton,
// and splits the layers by the entry flag bit.
extern "C" void func_8047C588__17UnkClass_8047BB54Fv(
    UnkDrawObj* obj, UnkDesc8047BB54* desc, void* a5, void* a6, void* a7,
    void* a8, void* a9, u32 a10, f32 f1) {
    obj->field_0x0 = (u32)desc;
    obj->field_0x10 = (UnkDrawEntry*)((u8*)desc + desc->field_0x8);
    UnkDescSlot24* slot = (UnkDescSlot24*)((u8*)desc + desc->field_0x24);
    obj->field_0xC = (u8*)desc + slot->field_0x0;
    obj->field_0x14 = (u8*)desc + *(u32*)((u8*)desc + desc->field_0x1C);
    func_80473984__Q26mpfsys17UnkClass_80471EC8Fv(
        desc, a8, a9, a5, a6, a10);
    if (desc->field_0x28) {
        obj->field_0x4 = (u32*)a7;
        obj->field_0x8 = (u16)desc->field_0xC;
        int count = (int)desc->field_0xC * 2;
        for (int i = 0; i < count; i++) {
            obj->field_0x4[obj->field_0x4[0] * i + 1] = 1;
        }
    }
    func_8047CC4C__17UnkClass_8047CA88Fv(
        UnkClass_8047CA88::getInstance(), (void*)obj->field_0x0,
        lbl_eu_80658410, (nw4r::math::VEC3*)lbl_eu_8065841C, slot->field_0x10);
    if (desc->field_0x10) {
        mpfsys::MPFDrawDisplayList* dl = mpfsys::MPFDrawDisplayList::getInstance();
        func_8047958C__Q26mpfsys18MPFDrawDisplayListFv(
            dl, (u8*)obj->field_0x0 + desc->field_0x10);
    }
    if (desc->field_0x28 && (desc->field_0x30 & 1)) {
        int count = (int)desc->field_0xC;
        for (int i = 0; i < count; i++) {
            if (obj->field_0x4[obj->field_0x4[0] * i + 1] > 1) {
                UnkDrawEntry* e = &obj->field_0x10[i];
                if (e->field_0x1E & 1) {
                    void* draw = ((void**)&lbl_eu_806584E8)[e->field_0x2];
                    ((MPFDrawVt8*)draw)->vf0((u8*)obj->field_0x0 + e->field_0x18);
                }
            }
        }
    }
}

// --- Target 4: us-804809fc ---------------------------------------------------
// Static-init: populate the singleton draw-object table.
void sinit_8047CA2C() {
    lbl_eu_806584E8.billboard = mpfsys::MPFDrawBillboard::getInstance();
    lbl_eu_806584E8.cross = mpfsys::MPFDrawCross::getInstance();
    lbl_eu_806584E8.displayList = mpfsys::MPFDrawDisplayList::getInstance();
    lbl_eu_806584E8.mdlNoColor = mpfsys::MPFDrawMdlNoColor::getInstance();
    lbl_eu_806584E8.mdlColor = mpfsys::MPFDrawMdlColor::getInstance();
    lbl_eu_806584E8.billLayTex = mpfsys::MPFDrawBillLayTex::getInstance();
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.bss] 0x806584E8-0x80658518 (0x30 = 48B): singleton draw-object table
// (6 pointers) + scratch draw context (0x18).
MPFDrawSingletonTable lbl_eu_806584E8;
u8 lbl_eu_80658500[0x18];
DECOMP_FORCEACTIVE(code_8047BB54_cpp, lbl_eu_80658500);

// [.sdata] 0x806638A0-0x806638A8 (8B): 1/3.0f + zero pad (retail raw words).
extern "C" f32 lbl_eu_806638A0[2] = { 0.33333334f, 0.0f };

// [.sbss] 0x806658B8-0x806658C0 (8B): singleton slot (4B used) + pad.
UnkSceneDataObj* lbl_eu_806658B8;
u32 lbl_eu_806658B8pad;
DECOMP_FORCEACTIVE(code_8047BB54_cpp, lbl_eu_806658B8pad);

// --- Target 1: us-8047fc24 ---------------------------------------------------
// Gate on two flag bits; when both are set, forward into the embedded
// subobject at +0x24 (tail call).
void UnkClass_8047BB54::func_8047BC54() {
    UnkClass_8047BB54Layout* self = (UnkClass_8047BB54Layout*)this;
    u32 flags = self->field_0x10;
    if ((flags & 1) == 0) return;
    if ((flags & 4) == 0) return;
    ((mpfsys::UnkClass_80471EC8*)self->gap_0x24)->func_80471FC8();
}

