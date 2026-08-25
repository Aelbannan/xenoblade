// The catalog header declares the post-update hook func_804BF940 as (void),
// which conflicts with the real signature (the retail call site passes the
// data pointer in r3). Rename the catalog's declaration out of the way for
// every inclusion in this TU so only the correct one is visible.
#define func_804BF940 func_804BF940_catalog_unused
// Auto-scaffolded catalog TU for monolib/src/scn/CScnEnvLgtCtrl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/scn/CScnTimeApi.hpp"
#include <nw4r/db/db_assert.h>            // nw4r::db::Panic
#include <nw4r/g3d/g3d_anmclr.h>
#include <nw4r/g3d/g3d_anmobj.h>
#include <nw4r/g3d/g3d_anmscn.h>
#include <nw4r/g3d/g3d_anmtexpat.h>
#include <nw4r/g3d/g3d_anmtexsrt.h>
#include <nw4r/g3d/g3d_fog.h>
#include <nw4r/g3d/g3d_scnmdl.h>
#include <nw4r/g3d/g3d_scnobj.h>
#include <nw4r/g3d/g3d_scnroot.h>         // ScnRoot::GetFog / G3DState::SetFog
#include <nw4r/g3d/res/g3d_resfile.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include <nw4r/math/math_arithmetic.h>   // nw4r::math::FrSqrt
#include <nw4r/math/math_triangular.h>   // nw4r::math::SinFIdx / CosFIdx
#include <revolution/MTX.h>              // PSMTXConcat / PSMTXIdentity / PSVECNormalize
#include "monolib/math/CVec3.hpp"        // ml::CVec3::zero
#include "libs/monolib/src/scn/CScnEnvLgtCtrl.hpp"
#include "libs/monolib/src/scn/CScnItemModelNw4r.hpp"  // func_8048ECD8 / func_80496288 queries
#include "monolib/math/CVec4.hpp"       // ml::CVec4 (func_804C1338 slot/walk vecs)
// The catalog header declares the post-update hook func_804BF940 as (void),
// which conflicts with the real signature declared in CScnEnvLgtCtrl.hpp
// (the retail call site passes the data pointer in r3). Rename the catalog's
// declaration out of the way so only the correct one is visible.
#include "monolib/scn/code_804BF59C.hpp"  // CScnEnvLgtData (mAmbColorBase / mFlags)
#undef func_804BF940
// Real signature of the post-update hook (retail passes the data pointer in
// r3); the catalog header's (void) declaration is renamed out of the way.
extern "C" void func_804BF940(CScnEnvLgtData* self);
extern "C" void func_804BF8A8(CScnEnvLgtData* self);
#include "monolib/util/MemManager.hpp"    // mtl::MemManager (allocate / deallocate)
#include "monolib/util/reslist.hpp"       // reslist<T> (embedded light-item list)

// shared float constant in .sdata2 (referenced via @sda21 by this TU)
extern float lbl_eu_8066B010;

// Float -> u32 bit-pattern helper: forces the retail stfs-to-stack/lwz
// roundtrip per element (func_804C5E9C / func_804C2124).
static inline u32 f32tou32(f32 f) {
    union { f32 f; u32 u; } cv;
    cv.f = f;
    return cv.u;
}

// Float -> u32 bit-cast via address-of: MWCC spills the FPR to a stack temp
// (ascending temp-slot allocation, cf. func_804C6D64/6F78 tail roundtrips)
// instead of the union-local form above (descending local slots).
static inline u32 f32tou32b(f32 f) { return *(u32*)&f; }

// Inverse read: reinterpret a u32 memory operand as float (direct lfs).
static inline f32 u32tof32b(u32 u) { return *(f32*)&u; }

// Bit-read of an f32 lvalue in place: loads the original memory location as
// u32 (lwz), with no stack roundtrip (cf. f32tou32b's by-value spill form).
static inline u32 f32bits_ref(const f32& f) { return *(u32*)&f; }

// Curve-window int->double: retail builds a 0x43300000 double temp and
// subtracts the shared 2^52 constant lbl_eu_8066B018, keeping the value as
// a raw double (no frsp).
extern const f64 lbl_eu_8066B018;
static inline f64 negIntToF64(int v) {
    union { u32 w[2]; f64 d; } c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v ^ 0x80000000u;
    return c.d;
}
static inline f32 curveToF(s32 v) {
    union { u32 w[2]; double d; } c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v ^ 0x80000000u;
    return (f32)(c.d - lbl_eu_8066B018);
}

extern const f64 lbl_eu_8066B0E8;  // 2^52 (u32->f32 magic, shared pool)
static inline f32 u32ToF_b0e8(u32 v) {
    union { u32 w[2]; double d; } c;
    c.w[0] = 0x43300000u;
    c.w[1] = v;
    return (f32)(c.d - lbl_eu_8066B0E8);
}
static inline f32 s32ToF_2(s32 v) {
    union { u32 w[2]; double d; } c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v ^ 0x80000000u;
    return (f32)(c.d - lbl_eu_8066B0E8);
}


extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }


// func_804C1500 (us-804c565c): reset the light-data blob, then walk the
// light-object ring forward (v14 push) and backward (v15 query, accumulating
// the active-light count); finish with the post-update hook and store the
// count into CScnEnvLgtData::mActiveLightCount. The float param stays live
// across every virtual call (MWCC f31-save prologue).
void func_804C1500(CScnEnvLgtCtrl* self, CScnEnvLgtData* data, void* arg,
                   float f) {
    u32 count = 0;
    func_804BF8A8(data);
    CScnEnvLgtCtrlListNode* node;
    for (node = self->field_0x0C->mNext; node != self->field_0x0C;
         node = node->mNext) {
        if (node->mItem->v23()) {
            node->mItem->v14(data);
        }
    }
    for (node = self->field_0x0C->mPrev; node != self->field_0x0C;
         node = node->mPrev) {
        if (node->mItem->v23()) {
            count = node->mItem->v15(f, data, arg, count);
        }
    }
    func_804BF940(data);
    data->mActiveLightCount = count;
}

// func_804C1600 (us-804c575c): walk the light-object ring at +0x0C and
// dispatch vtable slot 10 (0x28) on every item with `arg`; the arg is then
// remembered at +0x3C.
void func_804C1600(CScnEnvLgtCtrl* self, void* arg) {
    CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
    while (node != self->field_0x0C) {
        node->mItem->v8(arg);
        node = node->mNext;
    }
    self->alt.field_0x3C = arg;
}

// func_804C1674 (us-804c57d0): set +0x00 bit 0x20 and clear bit 0x10, then
// walk the light-object ring; every active item (vtable 0x64) receives the
// scene root (func_8048ECD8) plus the fog-manager byte at +0x28 through
// vtable slot 0x34.
void func_804C1674(CScnEnvLgtCtrl* self) {
    self->flags = (self->flags & ~0x10) | 0x20;
    CScnItemModelNw4rOwner* owner;  // declared before node: retail colors owner r31 / node r30
    CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
    while (node != self->field_0x0C) {
        if (node->mItem->v23()) {
            owner = (CScnItemModelNw4rOwner*)self->field_0x04_ptr;
            node->mItem->v11(func_8048ECD8(owner), owner->field_0x78[0x28]);
        }
        node = node->mNext;
    }
}

extern "C" void func_8048D124(void* a);
extern "C" void func_804C1720(u8* self, CScnItemModelNw4rEnvLight* lgt) { func_8048D124(*(void**)((u8*)*(void**)((u8*)self + 4) + 0x64)); }

// func_804C172C (us-804c5888): refresh the scene's light manager
// (func_8048D160 on the owner's +0x64 slot), then clear +0x00 bit 0x40;
// unless bit 0x20 was set, walk the light-object ring and hand every active
// item (vtable 0x64) the scene root (func_8048ECD8) through vtable slot
// 0x4C, then set bit 0x20 and clear bit 0x10 in +0x00. Finally, when bit
// 0x400 is set, push the fog constants into scene-root fog slot 0x1E: reset
// the type, copy the AFE0 color word byte-wise into FogData::color, and set
// startz/endz (AFE4/AFE8) plus nearz/farz (AFE4/AFE8).
void func_804C172C(CScnEnvLgtCtrl* self) {
    func_8048D160(
        ((CScnEnvLgtCtrlOwnerLightMan*)self->field_0x04_ptr)->field_0x64);
    u32 flags = self->flags;
    self->flags = flags & ~0x40;
    if (!(flags & 0x20)) {
        CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
        while (node != self->field_0x0C) {
            if (node->mItem->v23()) {
                node->mItem->v17(func_8048ECD8(self->field_0x04_ptr));
            }
            node = node->mNext;
        }
        self->flags = (self->flags | 0x20) & ~0x10;
    }
    if (self->flags & 0x400) {
        // GXColor struct copy from the shared pool word; MWCC keeps the
        // loaded word live across the calls and member-copies the bytes
        // into FogData::color through a stack spill.
        GXColor col = lbl_eu_8066AFE0;
        nw4r::g3d::Fog fog =
            ((nw4r::g3d::ScnRoot*)func_8048ECD8(self->field_0x04_ptr))
                ->GetFog(0x1e);
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC00, 0x41, lbl_eu_8056EBE0);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->type = GX_FOG_NONE;
        }
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC60, 0x63, lbl_eu_8056EC40);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->color = col;
        }
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC30, 0x4b, lbl_eu_8056EC10);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->startz = lbl_eu_8066AFE4;
            fog.ptr()->endz = lbl_eu_8066AFE8;
        }
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056F9A8, 0x57, lbl_eu_8056F988);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->nearz = lbl_eu_8066AFE4;
            fog.ptr()->farz = lbl_eu_8066AFE8;
        }
    }
}

// func_804C190C (us-804c5a68): clear +0x00 bit 0x40; unless bit 0x10 was
// set, walk the light-object ring and hand every active item (vtable 0x64)
// the scene root through vtable slot 0x48, then set bit 0x10 and clear bit
// 0x20 in +0x00.
void func_804C190C(CScnEnvLgtCtrl* self) {
    u32 flags = self->flags;
    self->flags = flags & ~0x40;
    if (!(flags & 0x10)) {
        CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
        while (node != self->field_0x0C) {
            if (node->mItem->v23()) {
                node->mItem->v16(func_8048ECD8(self->field_0x04_ptr));
            }
            node = node->mNext;
        }
        self->flags = (self->flags | 0x10) & ~0x20;
    }
}

// func_804C19B8 (us-804c5b14): clear +0x00 bit 0x40; unless bit 0x20 was
// set, walk the light-object ring and hand every active item (vtable 0x64)
// the scene root through vtable slot 0x4C, then set bit 0x20 and clear bit
// 0x10 in +0x00 (mirror image of func_804C190C's bit dance).
void func_804C19B8(CScnEnvLgtCtrl* self) {
    u32 flags = self->flags;
    self->flags = flags & ~0x40;
    if (!(flags & 0x20)) {
        CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
        while (node != self->field_0x0C) {
            if (node->mItem->v23()) {
                node->mItem->v17(func_8048ECD8(self->field_0x04_ptr));
            }
            node = node->mNext;
        }
        self->flags = (self->flags | 0x20) & ~0x10;
    }
}

// func_804C1A64 (us-804c5bc0): clear +0x00 flag bits 0x30 unconditionally;
// when bit 0x40 was clear, set it and push fog slot 0 to GX - reset the fog
// type to GX_FOG_NONE and forward the scene root's fog 0 to G3DState.
void func_804C1A64(CScnEnvLgtCtrl* self) {
    u32 flags = self->flags;
    self->flags = flags & ~0x30;
    if (!(flags & 0x40)) {
        self->flags |= 0x40;
        nw4r::g3d::ScnRoot* root =
            (nw4r::g3d::ScnRoot*)func_8048ECD8(self->field_0x04_ptr);
        nw4r::g3d::Fog fog = root->GetFog(0);
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC00, 0x41, lbl_eu_8056EBE0);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->type = GX_FOG_NONE;
        }
        nw4r::g3d::G3DState::SetFog(fog, 0);
    }
}

// func_804C1AFC (us-804c5c58): clear the +0x00 flag bits 0x70, then walk
// the light-object ring; every active item (vtable 0x64) gets the scene
// root (func_8048ECD8) plus this function's float through vtable slot 0x50.
void func_804C1AFC(CScnEnvLgtCtrl* self, float f) {
    self->flags &= ~0x70;
    CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
    while (node != self->field_0x0C) {
        if (node->mItem->v23()) {
            node->mItem->v18(func_8048ECD8(self->field_0x04_ptr), f);
        }
        node = node->mNext;
    }
}

// distance-style walk over the light-item ring; taking the iterators by
// value reproduces retail's four frame-spilled cursor slots.
static inline u32 LgtRingDistance(
    reslist<CScnEnvLgtCtrlListItem*>::iterator last,
    reslist<CScnEnvLgtCtrlListItem*>::iterator first) {
    u32 n = 0;
    while (first != last) {
        ++first;
        n++;
    }
    return n;
}

// func_804C1BA0 (us-804c5cfc): create a new env-light controller through the
// 'STGL' factory (func_804C6A70) on the scene owner's handle and attach it
// to the +0x1C node array (0xc-stride ring nodes linked into the +0x0C
// sentinel; the first slot whose mNext is null is reused). On success set
// +0x00 bit 1, and when the new controller reports "in use" (vtable 0x80)
// re-scan the ring: every idle item (vtable 0x7C) receives the teardown
// dispatch v25(1) (vtable 0x6C). Then refresh via func_804C26F0 and, when
// the ring holds exactly one item, push the teardown dispatch v37(1)
// (vtable 0x9C) on it. Returns the created controller.
void* func_804C1BA0(CScnEnvLgtCtrl* self, const u32* data, void* arg) {
    // The +0x08 work area hosts an embedded reslist<...>: start-node pointer
    // at +0x0C, sentinel node at +0x10, 0xc-stride node pool at +0x1C and
    // capacity at +0x20. push_back/size inline to the retail shape (the
    // setItem try/catch supplies the EH frame pointer and stw-r1 anchors).
    reslist<CScnEnvLgtCtrlListItem*>* list =
        (reslist<CScnEnvLgtCtrlListItem*>*)&self->field_0x08;
    CScnEnvLgtCtrlListItem* obj = (CScnEnvLgtCtrlListItem*)func_804C6A70(
        func_80496018((CScnItemModelNw4rOwner*)self->field_0x04_ptr), data, arg);
    if (obj != NULL) {
        list->push_back(obj);
        self->flags |= 2;
        if (obj->v30()) {
            for (CScnEnvLgtCtrlListNode* n = self->field_0x0C->mNext;
                 n != self->field_0x0C; n = n->mNext) {
                if (n->mItem->v29() == 0) {
                    n->mItem->v25(1);
                }
            }
        }
    }    func_804C26F0(self);
    if (LgtRingDistance(list->end(), list->begin()) == 1) {
        for (CScnEnvLgtCtrlListNode* n = self->field_0x0C->mNext;
             n != self->field_0x0C; n = n->mNext) {
            n->mItem->v37(1);
        }
    }
    return obj;
}

// func_804C1D7C (us-804c5ed8): remove `target` from the light-item ring at
// +0x0C. If the removed item reported "in use" (v30), re-scan the ring: when
// every item is free, dispatch the teardown pass (v25(0)) on each. Finally,
// when the ring is empty, clear +0x00 bit 1 and refresh via func_804C26F0.
void func_804C1D7C(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlListItem* target) {
    CScnEnvLgtCtrlListNode* node;
    bool freed = false;
    if (target != NULL) {
        CScnEnvLgtCtrlListNode* sentinel = self->field_0x0C;
        node = sentinel->mNext;
        while (node != sentinel && node->mItem != target) {
            node = node->mNext;
        }
        if (node != sentinel) {
            if (node->mItem->v30() != 0) {
                freed = true;
            }
            CScnEnvLgtCtrlListItem* item = node->mItem;
            if (item != 0) {
                if (item != 0) {
                    item->v0(1);
                }
                node->mItem = 0;
            }
            CScnEnvLgtCtrlListNode* prev = node->mPrev;
            CScnEnvLgtCtrlListNode* next = node->mNext;
            prev->mNext = next;
            next->mPrev = prev;
            node->mNext = 0;
        }
    }
    if (freed) {
        // Re-scan: if any item is still "in use", abort the teardown pass.
        node = self->field_0x0C->mNext;
        while (node != self->field_0x0C) {
            if (node->mItem->v30() != 0) {
                freed = false;
                break;
            }
            node = node->mNext;
        }
        if (freed) {
            node = self->field_0x0C->mNext;
            while (node != self->field_0x0C) {
                node->mItem->v25(0);
                node = node->mNext;
            }
        }
    }
    CScnEnvLgtCtrlListNode* n;
    int count;
    CScnEnvLgtCtrlListNode* sentinel = self->field_0x0C;
    n = sentinel->mNext;
    count = 0;
    while (n != sentinel) {
        n = n->mNext;
        count++;
    }
    if (count == 0) {
        self->flags &= ~2;
    }
    func_804C26F0(self);
}

// func_804C1F10 (us-804c606c): walk the light-item ring and push the float
// value to every active item (v23 gate, vtable 0x60). The active pass
// (flag != 0) hands v22(f, 1, flag-1); the idle pass hands v22(f, 0, 0).
void func_804C1F10(CScnEnvLgtCtrl* self, s32 flag, float f) {
    if (flag != 0) {
        CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
        while (node != self->field_0x0C) {
            if (node->mItem->v23()) {
                node->mItem->v22(f, 1, flag - 1);
            }
            node = node->mNext;
        }
    } else {
        CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
        while (node != self->field_0x0C) {
            if (node->mItem->v23()) {
                node->mItem->v22(f, 0, 0);
            }
            node = node->mNext;
        }
    }
}

// func_804C2014 (us-804c6170): same ring walk as func_804C1600, dispatching
// vtable slot 28 (0x70) with two args on every item.
void func_804C2014(CScnEnvLgtCtrl* self, void* a, void* b) {
    CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
    while (node != self->field_0x0C) {
        node->mItem->v26(a, b);
        node = node->mNext;
    }
}

// func_804C2094 (us-804c61f0): ring walk dispatching vtable slot 29 (0x74)
// with two args plus a float on every item; the float stays in f31 across
// the virtual calls (MWCC FPR-save prologue).
void func_804C2094(CScnEnvLgtCtrl* self, void* a, void* b, float f) {
    CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
    while (node != self->field_0x0C) {
        node->mItem->v27(a, b, f);
        node = node->mNext;
    }
}

// func_804C2124 (us-804c6280): interpolate the +0x3C word against four
// .sdata bounds (signed) to pick a blend factor, then blend the +0x34/+0x38
// float pair toward it and push a 7-word color/scale block into the +0x28
// object (each float bit pattern lands as a u32 via the stfs-to-stack/lwz
// roundtrip) plus a direction selector at +0x5C (1 or 3).
void func_804C2124(CScnEnvLgtCtrl* self, int flag) {
    int v = (int)self->alt6.field_0x3C;
    f32 f1;
    if (lbl_eu_80663AE8 >= v || lbl_eu_80663AF4 <= v || flag != 0) {
        f1 = lbl_eu_8066AFF0;
    } else if (v > lbl_eu_80663AE8 && v < lbl_eu_80663AEC) {
        f32 vf = s32ToF_2((s32)v);
        f32 t = (vf - u32ToF_b0e8(lbl_eu_80663AE8)) / (u32ToF_b0e8(lbl_eu_80663AEC) - u32ToF_b0e8(lbl_eu_80663AE8));
        f1 = lbl_eu_8066AFD0 * t + lbl_eu_8066AFF0 * (lbl_eu_8066AFDC - t);
    } else if (v > lbl_eu_80663AF0 && v < lbl_eu_80663AF4) {
        f32 vf = s32ToF_2((s32)v);
        f32 t = (vf - u32ToF_b0e8(lbl_eu_80663AF0)) / (u32ToF_b0e8(lbl_eu_80663AF4) - u32ToF_b0e8(lbl_eu_80663AF0));
        f1 = lbl_eu_8066AFD0 * (lbl_eu_8066AFDC - t) + lbl_eu_8066AFF0 * t;
    } else {
        f1 = lbl_eu_8066AFD0;
    }
    f32 n38 = lbl_eu_8066AFEC * (lbl_eu_8066AFD4 - self->field_0x38[0]) +
              self->field_0x38[0];
    f32 n34 = lbl_eu_8066AFEC * (f1 - self->field_0x34_f) + self->field_0x34_f;
    self->field_0x38[0] = n38;
    self->field_0x34_f = n34;
    f32 f2;
    if (n38 < lbl_eu_8066AFD8) {
        ((CScnEnvLgtCtrlLgtSink*)self->field_0x28)->field_0x5C = 3;
        f2 = -self->field_0x38[0];
    } else {
        ((CScnEnvLgtCtrlLgtSink*)self->field_0x28)->field_0x5C = 1;
        f2 = self->field_0x38[0];
    }
    ((CScnEnvLgtCtrlLgtSink*)self->field_0x28)->field_0x38 =
        f32tou32(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlLgtSink*)self->field_0x28)->field_0x3C =
        f32tou32(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlLgtSink*)self->field_0x28)->field_0x40 = f32tou32(f2);
    ((CScnEnvLgtCtrlLgtSink*)self->field_0x28)->field_0x44 = f32tou32(f2);
    ((CScnEnvLgtCtrlLgtSink*)self->field_0x28)->field_0x48 =
        f32tou32(self->field_0x34_f);
    ((CScnEnvLgtCtrlLgtSink*)self->field_0x28)->field_0x4C =
        f32tou32(self->field_0x34_f);
    ((CScnEnvLgtCtrlLgtSink*)self->field_0x28)->field_0x50 =
        f32tou32(self->field_0x34_f);
}

// func_804C22F0 (us-804c644c): per-frame scene light update. Refreshes the
// fog manager / camera when flagged, runs the BE398 flash-request pipeline,
// then walks the light-item ring accumulating two virtual-call results and
// pushes the timeout word into the fog manager.
extern "C" __declspec(noinline) void func_804C22F0(CScnEnvLgtCtrl* self) {
    // Declaration order pins MWCC's nonvolatile coloring: retail keeps flag
    // in r31, hitA r30, hitB r29 and the ring cursor r27.
    int flag;
    u32 hitA;
    u32 hitB;
    CScnEnvLgtCtrlListNode* node;
    if (self->flags & 0x100) {
        func_8049DE68(
            ((CScnItemModelNw4rOwner*)self->field_0x04_ptr)->field_0x78, 2);
    }
    if (self->flags & 0x8) {
        CScnEnvLgtCtrlCamWork* cam =
            ((CScnEnvLgtCtrlOwnerLight*)self->field_0x04_ptr)->field_0x68;
        func_8049B158(cam, cam->mCamId);
    }
    flag = 0;
    if (self->flags & 0x80) {
        CScnEnvLgtMgrView* mgr = (CScnEnvLgtMgrView*)(
            (CScnEnvLgtCtrlOwnerLight*)self->field_0x04_ptr)->field_0x5C;
        if (mgr->field_0xD0 != 0) {
            ml::CVec3 v;
            ml::CVec3 base;
            base.x = lbl_eu_8066AFD8;
            base.y = lbl_eu_8066AFDC;
            base.z = lbl_eu_8066AFD8;
            v.x = mgr->field_0x10C[0] + base.x;
            v.y = mgr->field_0x10C[1] + base.y;
            v.z = mgr->field_0x10C[2] + base.z;
            ml::CVec3 req;
            req.x = v.x;
            req.y = v.y;
            req.z = v.z;
            if (func_804BE398(&req, 0x6a01, 0, 0, lbl_eu_8066B000)) {
                if (func_804BE5AC()) {
                    flag = 1;
                    if (self->flags & 4) {
                        self->altCnt.flashCounter = 0;
                    } else {
                        u32 t = self->altCnt.flashCounter;
                        self->altCnt.flashCounter = t + 1;
                        if (t <= 5) flag = 0;
                    }
                } else {
                    flag = 0;
                    if (self->flags & 4) {
                        u32 t = self->altCnt.flashCounter;
                        self->altCnt.flashCounter = t + 1;
                        if (t <= 5) flag = 1;
                    } else {
                        self->altCnt.flashCounter = 0;
                    }
                }
            } else if (self->flags & 4) {
                flag = 1;
            }
            if (func_804BE5A0(0x20)) {
                self->flags |= 0x200;
            } else {
                self->flags &= ~0x200;
            }
            if (flag != 0) {
                self->flags |= 4;
            } else {
                self->flags &= ~4;
            }
        } else {
            self->flags &= ~4;
        }
    }
    // Ring walk: the enable bit feeds three virtual dispatches whose results
    // fold into two sticky flags (retail re-reads the sentinel from self each
    // iteration).
    flag = (int)((self->flags >> 2) & 1);
    hitA = 0;
    hitB = 0;
    func_80494208(
        ((CScnEnvLgtCtrlOwnerLight*)self->field_0x04_ptr)->field_0x5C, flag);
    u32 timeout = 0;
    node = self->field_0x0C->mNext;
    while (node != self->field_0x0C) {
        if (!node->mItem->v23()) {
            node = node->mNext;
            continue;
        }
        hitA = (hitA | (u32)node->mItem->v19(self->field_0x28, flag)) != 0;
        hitB = (hitB | (u32)node->mItem->v20(self->field_0x2C, flag)) != 0;
        node->mItem->v28(&timeout, flag);
        node = node->mNext;
    }
    if (flag == 0) {
        timeout = 0;
    }
    if (self->flags & 0x400) {
        timeout = 0x1e;
    }
    ((CScnEnvLgtFogManView*)(
        (CScnItemModelNw4rOwner*)self->field_0x04_ptr)->field_0x78)
        ->field_0x24 = timeout;
    if (hitB == 0) {
        ((CScnEnvLgtResFront*)self->field_0x2C)->field_0x50 &= ~2;
    }
    if (hitA == 0 && (self->flags & 0x80)) {
        func_804C2124(self, flag);
        hitA = 1;
    }
    if (hitA == 0) {
        self->field_0x28[2] &= ~2;
    }
}

// func_804C2654 (us-804c67b0): resolve the active camera item through the
// scene's camera-work blob (CScn+0x68) and walk the light-object ring;
// every active item (vtable 0x64) receives the camera data at +0x10c of
// the camera item through vtable slot 0x10.
void func_804C2654(CScnEnvLgtCtrl* self) {
    CScnEnvLgtCtrlCamWork* cam =
        *(CScnEnvLgtCtrlCamWork**)((u8*)self->field_0x04_ptr + 0x68);
    CScnEnvLgtCtrlCamView* camView =
        (CScnEnvLgtCtrlCamView*)func_8049B158(cam, cam->mCamId);
    CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
    while (node != self->field_0x0C) {
        if (node->mItem->v23()) {
            node->mItem->v2(&camView->field_0x10C);
        }
        node = node->mNext;
    }
}

// func_804C26F0 (us-804c684c): reset the light-param control (+0x30), the
// bit-array object (+0x28) and the secondary object (+0x2C) to their default
// state, gated by +0x00 bit 0x8. Float constants land as raw words via stack
// temps (f32tou32 union form); the +0x24 fields keep direct float stores. The base
// pointers are re-read from self per statement (retail reloads 0x30(r3) /
// 0x28(r26) / 0x2C(r26) between store groups), and the small int constants
// stay live in nonvolatiles across both waves (retail r27..r31).
extern "C" __declspec(noinline) void func_804C26F0(CScnEnvLgtCtrl* self) {
    if (!(self->flags & 0x8)) {
        return;
    }
    // Non-const so MWCC keeps each in a dedicated nonvolatile (retail
    // r27..r31) instead of folding them back to immediates.
    u32 one = 1;
    u32 maskF = 0xf;
    u32 valEB = 0xeb;
    u32 four = 4;
    u32 zero = 0;

    // Wave A: reset the +0x30 control blob (float-bit quads, then scalars).
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x00[0] =
        f32tou32b(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x00[1] =
        f32tou32b(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x00[2] =
        f32tou32b(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x00[3] =
        f32tou32b(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x68[0] =
        f32tou32b(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x68[1] =
        f32tou32b(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x68[2] =
        f32tou32b(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x68[3] =
        f32tou32b(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x78[0] =
        f32tou32b(lbl_eu_8066AFD4);
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x78[1] =
        f32tou32b(lbl_eu_8066AFD4);
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x78[2] =
        f32tou32b(lbl_eu_8066AFD4);
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x64 &= ~4;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x20 = one;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x24 = lbl_eu_8066B004;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x21 = 3;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x10 = maskF;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x11 = valEB;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x34 = one;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x38 = four;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x3C = one;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x40 = zero;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x84[0] = one;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x84[1] = four;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x84[2] = one;
    ((CScnEnvLgtCtrlParamInit*)self->field_0x30)->field_0x84[3] = zero;

    // Wave B: defaults for the bit-array float septet and the +0x2C object.
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x5C = one;
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x60 = four;
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x64 = one;
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x68 = zero;
    func_80498D98((CScnEnvLgtCtrlBits28*)self->field_0x28);
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x58 = four;
    func_80498DC0((u8*)((CScnEnvLgtCtrlBits28*)self->field_0x28), 1);
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x38[0] =
        f32tou32b(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x38[1] =
        f32tou32b(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x38[2] =
        f32tou32b(lbl_eu_8066AFDC);
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x38[3] =
        f32tou32b(lbl_eu_8066AFD4);
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x38[4] =
        f32tou32b(lbl_eu_8066AFD0);
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x38[5] =
        f32tou32b(lbl_eu_8066AFD0);
    ((CScnEnvLgtCtrlBits28*)self->field_0x28)->field_0x38[6] =
        f32tou32b(lbl_eu_8066AFD0);

    CScnEnvLgtCtrlBits2C* obj2 = (CScnEnvLgtCtrlBits2C*)self->field_0x2C;
    obj2->field_0x34 = one;
    obj2->field_0x38 = four;
    obj2->field_0x3C = one;
    obj2->field_0x40 = zero;
    obj2->field_0x14 = four;
    obj2->field_0x28 = four;
    obj2->field_0x44 = 8;
    obj2->field_0x20 = zero;
    obj2->field_0x21 = 2;
    obj2->field_0x24 = lbl_eu_8066B008;
    obj2->field_0x00[0] = f32tou32b(lbl_eu_8066AFDC);
    obj2->field_0x00[1] = f32tou32b(lbl_eu_8066AFDC);
    obj2->field_0x00[2] = f32tou32b(lbl_eu_8066AFDC);
    obj2->field_0x00[3] = f32tou32b(lbl_eu_8066AFDC);
    obj2->field_0x10 = maskF;
    obj2->field_0x11 = valEB;
}

// Compact light-header builder (defined later in this TU; forward decl so
// the ctor can call it).
extern "C" void func_804C6BA8(CScnEnvLgtCtrlLgtHeader* dst,
                              const CScnEnvLgtCtrlLgtSrc* src, u32 base);

extern "C" __declspec(noinline) CScnEnvLgtCtrl* __ct__CScnEnvLgtCtrl(
    CScnEnvLgtCtrl* self, const u32* data, void* arg) {
    CScnEnvLgtCtrlCtorView* s = (CScnEnvLgtCtrlCtorView*)self;
    s->mVtable = (void*)lbl_eu_8056F9B8;
    s->field_0x04 = 0;
    s->field_0x08 = 0;
    s->field_0x0C = 0;
    s->field_0x10 = 0;
    s->field_0x20 = 0;
    s->field_0x24 = 0;
    s->field_0x28 = 0;
    s->field_0x30 = 0;
    s->field_0x38 = 0;
    s->field_0x40 = 0;
    s->field_0x48 = 0;
    s->field_0x4C = 0;
    s->field_0x50 = 0;
    s->field_0xA8 = 0;
    s->field_0x2C = (u8*)data;
    s->field_0xBE = 0;
    s->field_0xC0 = lbl_eu_8066B010;
    s->field_0xC4 = lbl_eu_8066B014;
    s->field_0xC8 = lbl_eu_8066B010;
    s->field_0xCC = lbl_eu_8066B014;
    s->field_0xBC = -1;
    s->ctor90.field_0x90 = lbl_eu_80656C58[0];
    s->ctor90.field_0x94 = lbl_eu_80656C58[1];
    s->ctor90.field_0x98 = lbl_eu_80656C58[2];
    s->ctor90.field_0x9C = lbl_eu_80656C58[0];
    s->ctor90.field_0xA0 = lbl_eu_80656C58[1];
    s->ctor90.field_0xA4 = lbl_eu_80656C58[2];
    s->field_0xAC = arg;
    s->field_0x04 = 0;
    if (data[3] & 1) {
        s->field_0x04 = 0x10000100;
    }
    if (data[3] & 2) {
        s->field_0x04 |= 0x2000;
    }
    // Walk the resource elements at data+0x1C (8-byte stride) and size the
    // work-buffer layout. Each element type contributes item counts (saved
    // in the nXX locals) and the running byte total.
    int total = 0;
    int n0C = 0;
    int n18 = 0;
    int n20 = 0;
    int n24 = 0;
    int n10 = 0;
    int n1C = 0;
    int n14 = 0;
    int n28 = 0;
    if (data[2] != 0) {
        const CScnEnvLgtCtrlCtorElem* elem =
            (const CScnEnvLgtCtrlCtorElem*)(data + 7);
        int i = 0;
        do {
            u8* base = (u8*)data;
            switch (elem->mType) {
            case 1: {
                CScnEnvLgtCtrlCtorCtl* ctl =
                    (CScnEnvLgtCtrlCtorCtl*)(base + elem->mOffset);
                s->field_0x30 = ctl;
                s->field_0x34 = base + ctl->mOff34;
                if (ctl->mFlag38) {
                    s->field_0x38 = base + ctl->mOff38;
                    s->field_0x3C = base + ctl->mOff3C;
                }
                if (ctl->mCount0C) {
                    n0C = ctl->mCount0C * 0x14;
                    total += n0C;
                }
                if (ctl->mFlags & 1) {
                    n18 = ctl->mCount18 * 0x14;
                    total += n18;
                }
                if (ctl->mFlags & 2) {
                    n20 = ctl->mCount20 * 0x40;
                    total += n20;
                }
                if (!(data[3] & 1)) {
                    n24 = 0xc0;
                    total += 0xc0;
                }
                n28 = 0x28;
                total += 0x28;
                break;
            }
            case 2: {
                CScnEnvLgtCtrlCtorElem2* obj2 =
                    (CScnEnvLgtCtrlCtorElem2*)(base + elem->mOffset);
                s->field_0x40 = obj2;
                s->field_0x44 = base + obj2->mOff44;
                if (obj2->mCount10) {
                    n10 = obj2->mCount10 * 0x1c;
                    total += n10;
                }
                if (obj2->mFlags & 2) {
                    n1C = obj2->mCount1C * 0x1c;
                    total += n1C;
                }
                break;
            }
            case 3:
                s->field_0x4C = base + elem->mOffset;
                s->field_0x04 |= 0x80;
                break;
            case 5: {
                CScnEnvLgtCtrlCtorElem5* obj5 =
                    (CScnEnvLgtCtrlCtorElem5*)(base + elem->mOffset);
                s->field_0x48 = obj5;
                n14 += obj5->mCount14 * 0xd8;
                total += n14;
                if (obj5->mFlags & 1) {
                    s->field_0x04 |= 1;
                }
                if (obj5->mFlags & 2) {
                    s->field_0x04 |= 2;
                }
                if (obj5->mFlags & 4) {
                    s->field_0x04 |= 4;
                }
                break;
            }
            case 6:
                s->field_0x50 = base + elem->mOffset;
                break;
            }
            elem++;
            i++;
        } while (i < data[2]);
    }
    if (total != 0) {
        s->field_0x08 = mtl::MemManager::allocate_head(
            mtl::MemManager::getHandleMEM1(), total, 0x20);
    }
    // Lay out the sub-buffers inside the allocation (s->field_0x08). The
    // running byte offset reuses `total` (retail keeps it in r31) and the
    // buffer base is reloaded per use (retail `lwz r0, 0x8(r22)` each time).
    total = 0;
    if (n0C != 0) {
        total = n0C;
        s->field_0x0C = s->field_0x08;
        CScnEnvLgtCtrlLgtItem20* item = (CScnEnvLgtCtrlLgtItem20*)s->field_0x08;
        for (u32 k = 0; k < s->field_0x30->mCount0C; k++) {
            item[k].field_0x10 = 0;
        }
    }
    if (n18 != 0) {
        s->field_0x18 = (u8*)s->field_0x08 + total;
        total += n18;
    }
    if (n20 != 0) {
        s->field_0x20 = (u8*)s->field_0x08 + total;
        total += n20;
    }
    if (n24 != 0) {
        s->field_0x24 = (u8*)s->field_0x08 + total;
        total += n24;
        u8* p24 = (u8*)s->field_0x24;
        *(u16*)(p24 + 0x28) = 0;
        *(u16*)(p24 + 0x58) = 0;
        *(u16*)(p24 + 0x88) = 0;
        *(u16*)(p24 + 0xb8) = 0;
    }
    if (n10 != 0 || n1C != 0) {
        s->field_0x10 = (u8*)s->field_0x08 + total;
        CScnEnvLgtCtrlCtorItem1C* it1 =
            (CScnEnvLgtCtrlCtorItem1C*)((u8*)s->field_0x08 + total);
        for (u32 k = 0; k < s->field_0x40->mCount10; k++) {
            it1[k].field_0x18 = 0;
        }
        total += n10;
        s->field_0x1C = (u8*)s->field_0x08 + total;
        CScnEnvLgtCtrlCtorItem1C* it2 =
            (CScnEnvLgtCtrlCtorItem1C*)((u8*)s->field_0x08 + total);
        for (u32 k = 0; k < s->field_0x40->mCount1C; k++) {
            it2[k].field_0x18 = 0;
        }
        total += n1C;
    }
    if (n14 != 0) {
        s->field_0x14 = (u8*)s->field_0x08 + total;
        total += n14;
        u8* src2 = (u8*)s->field_0x48 + 0x10;
        for (u32 k = 0; k < s->field_0x48->mCount14; k++) {
            func_804C6BA8((CScnEnvLgtCtrlLgtHeader*)((u8*)s->field_0x14 + k * 0xd8),
                          (const CScnEnvLgtCtrlLgtSrc*)(src2 + k * 0x5c),
                          (u32)s->field_0x2C);
        }
    }
    if (n28 != 0) {
        s->field_0x28 = (u8*)s->field_0x08 + total;
        memset(s->field_0x28, 0, n28);
    }
    // Set the light-enable bits for every active entry in the four
    // control sub-arrays (strides 0x30/0x3C/0x50/0x64).
    if (s->field_0x30 != 0 && s->field_0x28 != 0) {
        u32* bits = (u32*)s->field_0x28;
        CScnEnvLgtCtrlCtorCtl* ctl = s->field_0x30;
        u8* e = (u8*)s->field_0x2C + ctl->mBaseA;
        for (u32 k = 0; k < ctl->mCountA; k++) {
            if (*(u32*)(e + 4) & 0x4000) {
                u32 idx = *(u16*)e;
                bits[idx >> 5] |= 1u << (idx & 31);
            }
            e += 0x30;
        }
        e = (u8*)s->field_0x2C + ctl->mBaseB;
        for (u32 k = 0; k < ctl->mCountB; k++) {
            if (*(u32*)(e + 4) & 0x4000) {
                u32 idx = *(u16*)e;
                bits[idx >> 5] |= 1u << (idx & 31);
            }
            e += 0x3c;
        }
        e = (u8*)s->field_0x2C + ctl->mBaseC;
        for (u32 k = 0; k < ctl->mCountC; k++) {
            if (*(u32*)(e + 4) & 0x4000) {
                u32 idx = *(u16*)e;
                bits[idx >> 5] |= 1u << (idx & 31);
            }
            e += 0x50;
        }
        e = (u8*)s->field_0x2C + ctl->mBaseD;
        for (u32 k = 0; k < ctl->mCountD; k++) {
            if (*(u32*)(e + 4) & 0x4000) {
                u32 idx = *(u16*)e;
                bits[idx >> 5] |= 1u << (idx & 31);
            }
            e += 0x64;
        }
    }
    s->field_0x04 |= 0x80000000;
    f32 f0 = lbl_eu_8066B014;
    s->field_0x74[0] = f0;
    s->field_0x74[1] = f0;
    s->field_0x74[2] = f0;
    s->field_0x74[3] = f0;
    s->field_0x74[4] = f0;
    s->field_0x74[5] = f0;
    s->field_0x74[6] = f0;
    return self;
}

// us-804c71c8: full object destructor. Restores the vtable, releases the
// +0x08 buffer and lets MWCC emit the deleting-dtor scaffold (this null
// check, delete-flag test + operator delete) automatically. The nested
// identical null-tests reproduce retail's double `beq` before deallocate.
CScnEnvLgtCtrl::~CScnEnvLgtCtrl() {
    void* cache = field_0x08;
    this->mVtable = (void*)lbl_eu_8056F9B8;
    if (cache != 0) {
        if (cache != 0) {
            mtl::MemManager::deallocate(cache);
            field_0x08 = 0;
        }
    }
}

// us-804c7324: deleting destructor for the IScnEnvCtl base subobject.
// The base dtor is trivial, so MWCC's generated deleting dtor is just the
// null check, flag > 0 test and operator delete (__dl__FPv).
extern "C" void* __dl__FPv(void* p);
extern "C" void* __dt__804C1054(CScnEnvLgtCtrlIScnResBase* self, int flag) {
    if (self != 0) {
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void func_804C30E8(CScnEnvLgtCtrl* self, float f1);

// Compact light-header builder (defined later in this TU; forward decl so
// the ctor can call it).
void func_804C6BA8(CScnEnvLgtCtrlLgtHeader* dst,
                   const CScnEnvLgtCtrlLgtSrc* src, u32 base);

// Forward decls for the two per-frame light-update helpers called at the end
// of func_804C30E8 (stubs below; separate targets). extern "C" keeps the flat
// retail name on the call-site relocs; noinline keeps the calls out-of-line
// so the retail `bl`s survive (inlining the empty stubs would drop the
// FPR-save prologue and shrink the frame).
extern "C" __declspec(noinline) void func_804C406C(CScnEnvLgtCtrl* self, float f);
extern "C" __declspec(noinline) void func_804C42A8(CScnEnvLgtCtrl* self, float f);

// func_804C30E8 (us-804c7244): one-frame step of the env-light controller.
// Guarded by +0x04 bit 31 (MSB, `clrrwi. r0, r4, 31`); the frame delta is
// field_0xCC * f1 (kept in f31 across the virtual call and the two helper
// calls - MWCC FPR-save prologue).
// With +0x04 bits 0x6000 set, advance field_0xB0 by the delta and, when the
// old field_0xB4 no longer exceeds it, dispatch the self vtable slot 0x70
// (item index field_0xBC, arg 0), reset field_0xBC to -1 and clear bits
// 0x6000; otherwise write the remaining-ratio into field_0xB8 (either
// (b4-new)/b4 when bit 0x4000 is set, or new/b4 without it). Then hand the
// delta to the two helper calls.
extern "C" void func_804C30E8(CScnEnvLgtCtrl* self, float f1) {
    u32 flags = self->field_0x04;
    if (flags & 0x80000000) {
        f32 delta = self->field_0xCC * f1;
        if (flags & 0x6000) {
            f32 newB0 = self->alt3.field_0xB0 + delta;
            f32 b4 = self->alt3.field_0xB4;
            self->alt3.field_0xB0 = newB0;
            if (b4 <= newB0) {
                if (flags & 0x4000) {
                    ((CScnEnvLgtCtrlVt*)self)->v26(self->field_0xBC, 0);
                }
                self->field_0xBC = -1;
                self->field_0x04 &= ~0x6000;
            } else {
                if (flags & 0x4000)
                    self->field_0xB8 = (b4 - newB0) / b4;
                else
                    self->field_0xB8 = newB0 / b4;
            }
        }
        func_804C406C(self, delta);
        func_804C42A8(self, delta);
    }
}

// func_804C31C8 (us-804c7324): walk the 4 light-slot entries at +0x24. For
// each slot with bit 0 set in its +0x28 control flags, bit 2 selects a
// curve-blend: the three u16 params (+0x2A/+0x2C/+0x2E, each * 60) frame a
// window around the +0xA8 bound; when the window contains the bound, the
// slot's +0x0C vec3 is scaled by a blend factor (1.0 minus the fraction of
// the curve that overshoots the bound, or the fraction that undershoots it)
// into +0x00 and control bit 3 is set; otherwise bit 3 is cleared. Slots
// with bit 1 set also receive the caller's vec3 at +0x18.
extern "C" void func_804C31C8(CScnEnvLgtCtrl* self, const CScnEnvLgtCtrlLgtVec3* src) {
    // Shared int->double temps (retail writes the 0x43300000 words once,
    // hoisted above the slot-null check, and reuses the pair).
    union { u32 w[2]; double d; } tDen, tNum;
    tNum.w[0] = 0x43300000u;
    tDen.w[0] = 0x43300000u;
    CScnEnvLgtCtrlLgtSlot* slot = self->field_0x24;
    if (slot == 0) return;
    for (int i = 0; i < 4; i++) {
        u16 flags = slot->field_0x28;
        if (flags & 1) {
            if (flags & 4) {
                int v0 = (int)slot->field_0x2A * 60;
                int v1 = (int)slot->field_0x2C * 60;
                int v2 = (int)slot->field_0x2E * 60;
                int changed = 0;
                f32 f = lbl_eu_8066B014;
                if (v0 > v1) {
                    // The bound is re-read inside each branch (retail
                    // keeps it off the pre-branch path).
                    int bound = self->alt7.field_0xA8;
                    if (v0 <= bound) {
                        if (v2 != 0 && v0 + v2 > bound) {
                            tDen.w[1] = (u32)v2 ^ 0x80000000u;
                            tNum.w[1] = (u32)(v0 + v2 - bound) ^ 0x80000000u;
                            f = lbl_eu_8066B014 -
                                (f32)(tNum.d - lbl_eu_8066B018) /
                                    (f32)(tDen.d - lbl_eu_8066B018);
                        }
                        changed = 1;
                    } else {
                        if (v1 >= bound) {
                            if (v2 != 0 && v1 - v2 < bound) {
                                tDen.w[1] = (u32)v2 ^ 0x80000000u;
                                tNum.w[1] = (u32)(v1 - bound) ^ 0x80000000u;
                                f = (f32)(tNum.d - lbl_eu_8066B018) /
                                    (f32)(tDen.d - lbl_eu_8066B018);
                            }
                            changed = 1;
                        }
                    }
                } else {
                    int bound = self->alt7.field_0xA8;
                    if (v0 <= bound && v1 >= bound) {
                        if (v2 != 0) {
                            if (v0 + v2 > bound) {
                                tDen.w[1] = (u32)v2 ^ 0x80000000u;
                                tNum.w[1] = (u32)(v0 + v2 - bound) ^ 0x80000000u;
                                f = lbl_eu_8066B014 -
                                    (f32)(tNum.d - lbl_eu_8066B018) /
                                        (f32)(tDen.d - lbl_eu_8066B018);
                            } else if (v1 - v2 < bound) {
                                tDen.w[1] = (u32)v2 ^ 0x80000000u;
                                tNum.w[1] = (u32)(v1 - bound) ^ 0x80000000u;
                                f = (f32)(tNum.d - lbl_eu_8066B018) /
                                    (f32)(tDen.d - lbl_eu_8066B018);
                            }
                        }
                        changed = 1;
                    }
                }
                if (changed) {
                    slot->field_0x28 |= 8;
                    slot->field_0x00.x = f32tou32b(slot->field_0x0C.f.x * f);
                    slot->field_0x00.y = f32tou32b(slot->field_0x0C.f.y * f);
                    slot->field_0x00.z = f32tou32b(slot->field_0x0C.f.z * f);
                } else {
                    slot->field_0x28 &= 0xFFF7;
                }
            }
            if (slot->field_0x28 & 2) {
                slot->field_0x18 = *src;
            }
        }
        slot++;
    }
}

// func_804C34A0 (us-804c75fc): rebuild the four CLightEnv slot colors from
// the resource sub-array A. Each layer (bit 1<<layer) accumulates the
// entries whose id bit is clear and whose flags contain the layer bit;
// entries flagged 0x8000/0x4000 pull precomputed rows instead.
extern "C" __declspec(noinline) void func_804C34A0(CScnEnvLgtCtrl* self,
                                                   CLightEnv* env) {
    const f32 zeroF = lbl_eu_8066B010;
    // Clear both slot-color quads unconditionally (before the count gate).
    ((CScnEnvLgtCtrlLgtView*)self)->field_0x54[0] = zeroF;
    ((CScnEnvLgtCtrlLgtView*)self)->field_0x54[1] = zeroF;
    ((CScnEnvLgtCtrlLgtView*)self)->field_0x54[2] = zeroF;
    ((CScnEnvLgtCtrlLgtView*)self)->field_0x54[3] = zeroF;
    ((CScnEnvLgtCtrlLgtView*)self)->field_0x64[0] = zeroF;
    ((CScnEnvLgtCtrlLgtView*)self)->field_0x64[1] = zeroF;
    ((CScnEnvLgtCtrlLgtView*)self)->field_0x64[2] = zeroF;
    ((CScnEnvLgtCtrlLgtView*)self)->field_0x64[3] = zeroF;
    if (((CScnEnvLgtCtrlCtorCtl*)self->field_0x30)->mCountA == 0) return;
    const f32 oneF = lbl_eu_8066B014;
    for (u32 layer = 0; layer < 4; layer++) {
        u32 bit = 1 << layer;
        CScnEnvLgtCtrlLgtItem30* items =
            (CScnEnvLgtCtrlLgtItem30*)(self->field_0x2C +
                                       ((CScnEnvLgtCtrlCtorCtl*)self->field_0x30)
                                           ->mBaseA);
        CScnEnvLgtCtrlLgtAcc acc;
        acc.x = zeroF;
        acc.y = zeroF;
        acc.z = zeroF;
        acc.w = oneF;
        for (u32 j = 0;
             j < ((CScnEnvLgtCtrlCtorCtl*)self->field_0x30)->mCountA; j++) {
            CScnEnvLgtCtrlLgtItem30* item = &items[j];
            int idx = item->id;
            if (self->field_0x28[idx >> 5] & (1 << (idx & 31))) continue;
            u32 flg = item->flags;
            if (!(flg & bit)) continue;
            // Dead mirror quad: retail keeps these stores; only the flagged
            // paths write it, and each branch gets its own stack slot.
            CScnEnvLgtCtrlLgtSum t;
            if (flg & 0x8000) {
                CScnEnvLgtCtrlLgtEntry40* e =
                    &self->field_0x20_ptr[item->field_0x1E];
                acc.w += oneF;
                acc.x += e->field_0x18;
                acc.y += e->field_0x1C;
                acc.z += e->field_0x20;
                t.x = e->field_0x18;
                t.y = e->field_0x1C;
                t.z = e->field_0x20;
                t.w = oneF;
            } else if (flg & 0x4000) {
                CScnEnvLgtCtrlLgtItem20* row =
                    &((CScnEnvLgtCtrlLgtItem20*)self->field_0x0C)
                         [item->field_0x20];
                f32 sx = row->x * row->f;
                f32 sy = row->y * row->f;
                f32 sz = row->z * row->f;
                acc.w += oneF;
                acc.x += sx;
                acc.y += sy;
                acc.z += sz;
                t.x = sx;
                t.y = sy;
                t.z = sz;
                t.w = oneF;
            } else {
                acc.x += item->x * item->f;
                acc.y += item->y * item->f;
                acc.z += item->z * item->f;
            }
        }
        acc.w = oneF;
        func_80495644(env, layer);
        func_804952C4(env, (const ml::CVec3*)&acc);
        func_804956F8(env);
        if (layer == 1) {
            self->flags |= 0x200;
            ((CScnEnvLgtCtrlLgtView*)self)->field_0x54_pairs[0] = acc.pairs[0];
            ((CScnEnvLgtCtrlLgtView*)self)->field_0x54_pairs[1] = acc.pairs[1];
        }
        if (layer == 3) {
            self->flags |= 0x400;
            ((CScnEnvLgtCtrlLgtView*)self)->field_0x64_pairs[0] = acc.pairs[0];
            ((CScnEnvLgtCtrlLgtView*)self)->field_0x64_pairs[1] = acc.pairs[1];
        }
    }
}

// func_804C3404 (us-804c7560): push the default light value to +0xEC (the
// +0x30 pointer is only null-checked; the value itself is the .sdata2
// constant lbl_eu_8066B010) and the 3-word color triple to +0xD0, then
// refresh the four CLightEnv slot fields: each slot field is copied to +0xE4
// and recomputed by func_804C64A8. The loop walks a byte offset (off)
// against the env base (p); both inductions stay independent so MWCC keeps
// the five nonvolatile registers (savegpr/restgpr 27) like retail.
extern "C" void func_804C3404(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLightEnvView* env,
                   const u32* data) {
    if (self->field_0x30 == 0) return;
    self->field_0xEC = lbl_eu_8066B010;
    self->field_0xD0 = data[0];
    self->field_0xD4 = data[1];
    self->field_0xD8 = data[2];
    u8* p = (u8*)env;
    u32 off;
    u8* row;
    u32 i;
    for (i = 0, off = 0; i < 4; i++, off += 4) {
        // Per-iteration row base: the +0x1140/+0x1160 displacements stay
        // folded into the loads like retail.
        row = p + off;
        u32 res;
        self->field_0xE4 = *(u32*)(row + 0x1160);
        res = func_804C64A8(self, *(CLight**)(row + 0x1140), i, 0);
        *(u32*)(row + 0x1160) = res;
    }
}

extern "C" void func_804C33F0(void* self, CLightEnv* env) {
    if (*(u32*)((u8*)self + 0x30) == 0)
        return;
    func_804C34A0((CScnEnvLgtCtrl*)self, env);
}

// func_804C3778 (us-804c78d4): apply a light command with an implicit 1.0
// scale (sibling of func_804C392C without the f1 parameter). Returns 0 when
// the command's light index bit is already set in the +0x28 bit array, or
// when the fallback path's func_804C3F58 rejects the control. The +0x8000
// flag path uses the +0x20 0x40-byte entry rows (indexed by the control's
// u16 at +0x1E), scaling their vec3 by the +0xB8 slot factor when the index
// matches the stored +0xBC slot (else by 1.0).
int func_804C3778(CScnEnvLgtCtrl* self, CScnEnvLgtLightView* view,
                  CScnEnvLgtCtrlLgtCmd* ctl) {
    u16 index = ctl->field_0x00;
    if (self->field_0x28[index >> 5] &
        (1u << (index & 31))) {
        return 0;
    }
    if (ctl->field_0x04 & 0x8000) {
        CScnEnvLgtCtrlLgtEntry40* entry = &self->field_0x20_ptr[ctl->field_0x1E];
        func_804C03A0(view, 1);
        if (ctl->field_0x00 == self->lgt2.field_0xBC) {
            f32 s = self->lgt2.field_0xB8;
            CScnEnvLgtCtrlLgtVec3f tmp;
            tmp.z = entry->field_0x20 * s;
            tmp.y = entry->field_0x1C * s;
            tmp.x = entry->field_0x18 * s;
            CScnEnvLgtCtrlLgtVec4 out;
            out.x = tmp.x;
            out.y = tmp.y;
            out.z = tmp.z;
            out.w = lbl_eu_8066B014;
            func_804C07F0(view, &out);
        } else {
            CScnEnvLgtCtrlLgtVec4 out;
            out.x = entry->field_0x18;
            out.y = entry->field_0x1C;
            out.z = entry->field_0x20;
            out.w = lbl_eu_8066B014;
            func_804C07F0(view, &out);
        }
        func_804C0484(view, (const u8*)entry + 0xC);
        func_804C08C8(view, 1);
        return 1;
    }
    CScnEnvLgtCtrlLgtVec4 out;
    f32 scale;
    if (ctl->field_0x00 == self->lgt2.field_0xBC) {
        scale = self->lgt2.field_0xB8;
    } else {
        scale = lbl_eu_8066B014;
    }
    if (func_804C3F58(self, &out, (CScnEnvLgtCtrlLgtParamEntry*)ctl, scale)) {
        out.w = lbl_eu_8066B014;
        func_804C03A0(view, 1);
        func_804C07F0(view, &out);
        func_804C0484(view, (const u8*)ctl + 0x30);
        func_804C08C8(view, 1);
        return 1;
    }
    return 0;
}

// func_804C392C (us-804c7a88): apply a light command. Returns 0 when the
// command's light index bit is already set in the +0x28 bit array, or when
// the fallback path's func_804C3F58 rejects the control. The +0x8000 flag
// path uses the +0x20 64-byte entry rows (indexed by the control's u16 at
// +0x1E), scaling their vec3 by f1.
int func_804C392C(CScnEnvLgtCtrl* self, CScnEnvLgtLightView* view,
                  CScnEnvLgtCtrlLgtCmd* ctl, float f1) {
    s16 index = ctl->field_0x00;
    u32* bits = self->field_0x28;
    if (bits[index >> 5] & (1u << (index & 31))) {
        return 0;
    }
    f32 scale = f1;
    if (index == self->lgt2.field_0xBC) {
        scale = f1 * self->lgt2.field_0xB8;
    }
    if (ctl->field_0x04 & 0x8000) {
        CScnEnvLgtCtrlLgtEntry40* entry =
            &self->field_0x20_ptr[ctl->field_0x1E];
        func_804C03A0(view, 3);
        CScnEnvLgtCtrlLgtVec3f tmp;
        tmp.z = entry->field_0x20 * scale;
        tmp.x = entry->field_0x18 * scale;
        tmp.y = entry->field_0x1C * scale;
        CScnEnvLgtCtrlLgtVec4 out;
        out.x = tmp.x;
        out.y = tmp.y;
        out.z = tmp.z;
        out.w = lbl_eu_8066B014;
        func_804C07F0(view, &out);
        func_804C0454(view, (const u8*)entry);
        func_804C09E0(view, entry->field_0x34, entry->field_0x28,
                      entry->field_0x2C);
        func_804C08C8(view, 1);
        return 1;
    }
    CScnEnvLgtCtrlLgtVec4 out;
    if (func_804C3F58(self, &out, (CScnEnvLgtCtrlLgtParamEntry*)ctl, scale)) {
        out.w = lbl_eu_8066B014;
        func_804C03A0(view, 3);
        func_804C07F0(view, &out);
        func_804C0454(view, (const u8*)ctl + 0x30);
        func_804C09E0(view, ctl->field_0x44, ctl->field_0x48, ctl->field_0x4C);
        func_804C08C8(view, 1);
        return 1;
    }
    return 0;
}

// func_804C3AC8 (us-804c7c24): apply a light command with an explicit f1
// scale (superset of func_804C3778: the entry path also pushes the row's
// direction pair via func_804C09E0/func_804C0920, and the fallback pushes
// the control's +0x44/+0x50 block). Returns 0 when the index bit is already
// set or func_804C3F58 rejects the control.
int func_804C3AC8(CScnEnvLgtCtrl* self, CScnEnvLgtLightView* view,
                  CScnEnvLgtCtrlLgtCmd* ctl, float f1) {
    u16 index = ctl->field_0x00;
    u32* bits = self->field_0x28;
    if (bits[index >> 5] & (1u << (index & 31))) {
        return 0;
    }
    f32 scale = f1;
    if (index == self->lgt2.field_0xBC) {
        scale = f1 * self->lgt2.field_0xB8;
    }
    if (ctl->field_0x04 & 0x8000) {
        CScnEnvLgtCtrlLgtEntry40* entry = &self->field_0x20_ptr[ctl->field_0x1E];
        func_804C03A0(view, 4);
        CScnEnvLgtCtrlLgtVec3f tmp;
        tmp.z = entry->field_0x20 * scale;
        tmp.y = entry->field_0x1C * scale;
        tmp.x = entry->field_0x18 * scale;
        CScnEnvLgtCtrlLgtVec4 out;
        out.x = tmp.x;
        out.y = tmp.y;
        out.z = tmp.z;
        out.w = lbl_eu_8066B014;
        func_804C07F0(view, &out);
        func_804C0454(view, (const u8*)entry);
        func_804C0484(view, (const u8*)entry + 0xC);
        func_804C09E0(view, entry->field_0x34, entry->field_0x28,
                      entry->field_0x2C);
        func_804C0920(view, entry->field_0x38, entry->field_0x30);
        func_804C08C8(view, 1);
        return 1;
    }
    CScnEnvLgtCtrlLgtVec4 out;
    if (func_804C3F58(self, &out, (CScnEnvLgtCtrlLgtParamEntry*)ctl, scale)) {
        out.w = lbl_eu_8066B014;
        func_804C03A0(view, 4);
        func_804C07F0(view, &out);
        func_804C0454(view, (const u8*)ctl + 0x30);
        func_804C0484(view, (const u8*)ctl + 0x44);
        func_804C09E0(view, ctl->field_0x50, ctl->field_0x54, ctl->field_0x58);
        func_804C0920(view, ctl->field_0x5C, ctl->field_0x60);
        func_804C08C8(view, 1);
        return 1;
    }
    return 0;
}

// func_804C3C9C (us-804c7df8): refresh up to two light-slot entries into
// the CLight array. Each active slot entry either projects its position
// onto the attenuation sphere around the ambient color or pushes the raw
// position; accepted lights record a zero priority and bump the count.
extern "C" __declspec(noinline) void func_804C3C9C(
    CScnEnvLgtCtrl* self, CLight* lights, int flag) {
    f32 one = lbl_eu_8066B014;
    f32 zero = lbl_eu_8066B010;
    f32 kAtten = lbl_eu_8066B028;
    CScnEnvLgtCtrlLgtSlot* slot = &self->field_0x24[(flag != 0) * 2];
    // Address-taken by VEC3Sub below; MWCC keeps the address in a reserved
    // NV register hoisted above the loop.
    nw4r::math::VEC3 diff;
    for (int i = 0; i < 2; i++, slot++) {
        u16 fl = slot->field_0x28;
        if (!(fl & 1)) continue;
        CScnEnvLgtLightView* view =
            (CScnEnvLgtLightView*)&lights[self->field_0xE4];
        if ((fl & 4) && !(fl & 8)) continue;
        if (fl & 2) {
            // Distance path: attenuate toward the ambient color.
            nw4r::math::VEC3Sub(&diff, (const nw4r::math::VEC3*)&self->mAmbX,
                                (const nw4r::math::VEC3*)&slot->field_0x18);
            f32 d2 = nw4r::math::VEC3Dot(&diff, &diff);
            f32 radius = self->field_0xEC + slot->field_0x24;
            if (radius * radius >= d2) continue;
            func_804C03A0(view, 3);
            func_804C0454(view, (const u8*)&slot->field_0x18);
            func_804C09E0(view, 0, lbl_eu_8066B020, lbl_eu_8066B024);
            f32 inner = kAtten * radius;
            if (inner * inner < d2) {
                // Project onto the inner sphere.
                if (d2 < zero) {
                    nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 dist = (d2 > zero) ? d2 * nw4r::math::FrSqrt(d2) : zero;
                f32 t = (radius - dist) / (radius - inner);
                // Intermediate scaled vector; retail keeps both store waves.
                CScnEnvLgtCtrlLgtVec3f ofs;
                ofs.z = slot->field_0x18f.z * t;
                ofs.x = slot->field_0x18f.x * t;
                ofs.y = slot->field_0x18f.y * t;
                CScnEnvLgtCtrlLgtVec4 pos;
                pos.x = ofs.x;
                pos.y = ofs.y;
                pos.z = ofs.z;
                pos.w = one;
                func_804C07F0(view, &pos);
            } else {
                CScnEnvLgtCtrlLgtVec4 pos;
                pos.x = slot->field_0x18f.x;
                pos.y = slot->field_0x18f.y;
                pos.z = slot->field_0x18f.z;
                pos.w = one;
                func_804C07F0(view, &pos);
            }
        } else {
            // Simple path: push the raw slot position.
            func_804C03A0(view, 1);
            func_804C0484(view, (const u8*)&slot->field_0x18);
            CScnEnvLgtCtrlLgtVec4 pos;
            pos.x = slot->field_0x18f.x;
            pos.y = slot->field_0x18f.y;
            pos.z = slot->field_0x18f.z;
            pos.w = one;
            func_804C07F0(view, &pos);
        }
        self->field_0x130[self->field_0xE4] = zero;
        func_804C08C8(view, 1);
        self->field_0xE4++;
    }
}

// func_804C3F58 (us-804c80b4): fill a 4-float output from a light entry.
// When the param control's +0x04 bit 0x10000 is clear, scale the control's
// own +0x08 vec3 by f1*+0x14. Otherwise pick the 0x14-byte item row at
// +0x0C + index*0x14; bit 0 of its +0x10 flags rejects the update. When
// +0x04 bit 0x8000 and the item's bit 2 are both set, scale the
// controller's +0x90 vec3 and set bit 0x8000; otherwise scale the item's
// own vec3. The +0x0C output word is the shared .sdata2 constant.
extern "C" int func_804C3F58(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtVec4* out,
                  CScnEnvLgtCtrlLgtParamEntry* ctl, float f1) {
    if (ctl->field_0x04 & 0x10000) {
        CScnEnvLgtCtrlLgtItem20* item =
            (CScnEnvLgtCtrlLgtItem20*)((u8*)self->field_0x0C +
                                       ctl->field_0x20 * 0x14);
        if (item->field_0x10 & 1) return 0;
        if (self->field_0x04 & 0x8000 && item->field_0x10 & 4) {
            f32 scale = f1 * item->f;
            CScnEnvLgtCtrlLgtVec3f tmp;
            tmp.z = self->alt4.field_0x98 * scale;
            tmp.x = self->alt4.field_0x90 * scale;
            tmp.y = self->alt4.field_0x94 * scale;
            out->w = lbl_eu_8066B014;
            out->z = tmp.z;
            out->x = tmp.x;
            out->y = tmp.y;
            self->field_0x04 |= 0x8000;
        } else {
            f32 scale = f1 * item->f;
            CScnEnvLgtCtrlLgtVec3f tmp;
            tmp.z = item->z * scale;
            tmp.x = item->x * scale;
            tmp.y = item->y * scale;
            out->w = lbl_eu_8066B014;
            out->x = tmp.x;
            out->y = tmp.y;
            out->z = tmp.z;
        }
    } else {
        f32 scale = ctl->field_0x14 * f1;
        // Batch the three products before the stores (retail emits all
        // multiplies together, no store/load interleaving).
        f32 sx = ctl->field_0x08 * scale;
        f32 sy = ctl->field_0x0C * scale;
        f32 sz = ctl->field_0x10 * scale;
        out->x = sx;
        out->y = sy;
        out->z = sz;
    }
    return 1;
}


extern "C" __declspec(noinline) void func_804C406C(CScnEnvLgtCtrl* self, float f) {
    // Three shared loop locals: p = entry cursor, i = index / cached count /
    // slot index, j = the two unconditional counters, q = third-walk offset.
    // i/j are signed so the gate walk compares with `cmp` (the count compares
    // widen to unsigned because the counts are u32 fields).
    u8* p;
    u8* q;
    int i;
    int j;
    if (self->field_0x30_chk != NULL) {
        // Four control sub-arrays
        // with bit 16 set at +0x04 receives the refresh dispatch. The count
        // is re-read from self+0x30 every iteration (the dispatch call may
        // alias the control blob).
        { u8* p; u32 i;
        for (i = 0,
            p = self->field_0x2C +
                ((CScnEnvLgtCtrlCtorCtl*)self->field_0x30)->mBaseA;
             i < ((CScnEnvLgtCtrlCtorCtl*)self->field_0x30)->mCountA;
             i++, p += 0x30) {
            if (*(u32*)(p + 4) & 0x10000) {
                func_804C43A4(self, (CScnEnvLgtCtrlFadeEntry*)p);
            }
        }
        }
        { u8* p; u32 i;
        for (i = 0,
            p = self->field_0x2C +
                ((CScnEnvLgtCtrlCtorCtl*)self->field_0x30)->mBaseB;
             i < ((CScnEnvLgtCtrlCtorCtl*)self->field_0x30)->mCountB;
             i++, p += 0x3c) {
            if (*(u32*)(p + 4) & 0x10000) {
                func_804C43A4(self, (CScnEnvLgtCtrlFadeEntry*)p);
            }
        }
        }
        { u8* p; u32 i;
        for (i = 0,
            p = self->field_0x2C +
                ((CScnEnvLgtCtrlCtorCtl*)self->field_0x30)->mBaseC;
             i < ((CScnEnvLgtCtrlCtorCtl*)self->field_0x30)->mCountC;
             i++, p += 0x50) {
            if (*(u32*)(p + 4) & 0x10000) {
                func_804C43A4(self, (CScnEnvLgtCtrlFadeEntry*)p);
            }
        }
        }
        { u8* p; u32 i;
        for (i = 0,
            p = self->field_0x2C +
                ((CScnEnvLgtCtrlCtorCtl*)self->field_0x30)->mBaseD;
             i < ((CScnEnvLgtCtrlCtorCtl*)self->field_0x30)->mCountD;
             i++, p += 0x64) {
            if (*(u32*)(p + 4) & 0x10000) {
                func_804C43A4(self, (CScnEnvLgtCtrlFadeEntry*)p);
            }
        }
        }
    }
    if (self->alt2.field_0x40 != NULL) {
        // Fog-gate entry array: unconditional dispatch per entry; the count
        // is cached in i before the loop.
        j = 0;
        q = self->field_0x2C + self->alt2.field_0x40->mOffset;
        i = self->alt2.field_0x40->mCount;
        for (; j < i; j++, q += 0x3c) {
            func_804C4954(self, q);
        }
    }
    if (self->alt2.field_0x48 != NULL) {
        // Entry refresh over the 0xd8-stride array at +0x14; walked by byte
        // offset (kept in p) against a freshly loaded +0x14 base each call.
        // The slot index comes from +0xBE (+1), kept in i, only when flag
        // bit 0x8 is set.
        i = 0;
        if (self->field_0x04 & 0x8) {
            i = self->lgt2.field_0xBE + 1;
        }
        j = 0;
        p = (u8*)0;
        for (; j < self->alt2.field_0x48->mCount; p += 0xd8, j++) {
            func_804C7190((u8*)self->field_0x14_ptr + (u32)p,
                          self->alt7.field_0xA8, i,
                          self->lgt2.field_0xC0);
        }
    }
    // Advance the fog blend accumulator
    f32 lim = lbl_eu_8066B014;
    f32 c0;
    f32 c4;
    c4 = self->lgt2.field_0xC4;
    c0 = self->lgt2.field_0xC0;
    f32 v = f * c4 + c0;
    self->lgt2.field_0xC0 = v;
    if (v > lim) {
        self->lgt2.field_0xC0 = lim;
    }
    self->field_0x04 |= 0x40;
}

// Work-blob builder (matched, defined below); extern "C" keeps the flat
// retail name on the call-site reloc and noinline keeps the call out-of-line
// (the retail bl must survive).
extern "C" __declspec(noinline) void func_804C7774(void* self, u32 a, u32 b,
                                                    float f, u32 c);
extern "C" __declspec(noinline) void func_804C8054(CScnEnvLgtCtrlWorkBlob* blob, u8* entry);

// func_804C42A8 (us-804c8404): frame-step the env-light controller when
// +0x04 bit 0x80 is set: accumulate the delta into field_0xC8 and clamp it
// to the +0x4C object's +0x08 limit (bit 0x40 selects the limit, otherwise
// the shared .sdata2 constant), build the 0x20-byte work blob, push it to
// the +0x30/+0x20 pair when both exist, then dispatch func_804C8054 for
// every entry in the +0x48 control's list (the ctl and the +0x14 base are
// re-loaded per iteration because the dispatch call may alias them).
void func_804C42A8(CScnEnvLgtCtrl* self, float f) {
    u32 flags = self->field_0x04;
    if (!(flags & 0x80)) return;
    // reuse the parameter register for the accumulation (retail fadds f1)
    f = self->field_0xC8 + f;
    self->field_0xC8 = f;
    f32 limit = self->alt2.field_0x4C->field_0x08;
    if (limit < f) {
        if (flags & 0x100) {
            self->field_0xC8 = limit;
        } else {
            self->field_0xC8 = lbl_eu_8066B010;
        }
    }
    CScnEnvLgtCtrlWorkBlob blob;
    func_804C7774(&blob, (u32)self->field_0x2C, (u32)self->alt2.field_0x4C,
                  self->field_0xC8, (u32)self->field_0x0C);
    // The null checks double as argument setup (retail loads field_0x30 /
    // field_0x20 into r4/r5 while testing them, then calls).
    if (self->field_0x30 != 0 && self->field_0x20 != 0) {
        func_804C7910(&blob, (CScnEnvLgtCtrlCtorCtl*)self->field_0x30,
                      (u8*)self->field_0x20);
    }
    CScnEnvLgtCtrlLgtCtl* ctl = self->alt2.field_0x48;
    if (ctl != 0 && (ctl->field_0x00 & 0x10)) {
        u32 off;
        u32 i;
        for (i = 0, off = 0; i < self->alt2.field_0x48->mCount;
             off += 0xd8, i++) {
            func_804C8054(&blob, (u8*)self->field_0x14_ptr + i * 0xd8);
        }
    }
}

// func_804C43A4 (us-804c8500): per-entry fade refresh dispatched by
// func_804C406C. Phase 1 scans the 0x20-stride rows at self+0x34 starting at
// item->start for the last row whose weight*60 fits under the +0xA8 curve
// bound; the winning row is blended (by the overshoot ratio, when any) into
// the 0x14-stride item row at self+0x0C selected by item->sel20, and the
// row's flash-center fold runs when that row's bit 1 is set and exactly one
// candidate was accepted. Phase 2 (self flags 0x18 + entry flags 0xF00000)
// pulls a precomputed row pair from the table at self+0x50, applies the
// +0xC0 fade factor against another 0x14-stride row picked by item->sel22
// and folds the center triple into the controller's +0x90 vec3.
extern "C" __declspec(noinline) void func_804C43A4(CScnEnvLgtCtrl* self,
                                                   CScnEnvLgtCtrlFadeEntry* item) {
    // Phase-1 scan cursors (retail: best r5, prevBest r6, count r7, cur r10).
    CScnEnvLgtCtrlScanRow* best = NULL;
    CScnEnvLgtCtrlScanRow* prevBest = NULL;
    int acc = 0;
    u32 scanOff = item->start * sizeof(CScnEnvLgtCtrlScanRow);
    CScnEnvLgtCtrlScanRow* cur =
        (CScnEnvLgtCtrlScanRow*)((u8*)self->field_0x34_ptr + scanOff);
    for (int i = 0; i < item->count; i++, cur++) {
        // The curve bound is re-read from self every iteration (signed cmp).
        if (cur->weight * 60 > self->alt7.field_0xA8) break;
        prevBest = best;
        best = cur;
        acc = i + 1;   // retail keeps the running count tied to the index
    }
    CScnEnvLgtCtrlLgtItem20* dst =
        (CScnEnvLgtCtrlLgtItem20*)((u8*)self->field_0x0C + item->sel20 * 0x14);
    dst->field_0x10 &= ~6u;

    f32 ratio = lbl_eu_8066B010;   // fraction toward the previous candidate
    f32 fadeT = ratio;             // blend weight toward the winning row
    f32 outW;
    // volatile keeps both triples memory-resident like retail (frame spills).
    volatile CScnEnvLgtCtrlV3 outPos;
    volatile CScnEnvLgtCtrlV3 center;

    if (best == NULL) {
        // No fitting row: push the entry's own fallback quad.
        outPos.x = item->color[0];
        outPos.y = item->color[1];
        outPos.z = item->color[2];
        outW = item->wPrev;
        if (item->wPrev == lbl_eu_8066B010) {
            dst->field_0x10 |= 1;
        }
    } else if ((int)((best->weight + best->tail) * 60) > self->alt7.field_0xA8) {
        // Overshoot: scale by the fraction of the tail window inside the bound.
        int diff = self->alt7.field_0xA8 - (int)((best->weight + best->tail) * 60);
        int den = (int)best->tail * 60;
        ratio = (f32)((negIntToF64(-diff) - lbl_eu_8066B018) /
                      (negIntToF64(-den) - lbl_eu_8066B018));
        fadeT = lbl_eu_8066B014 - ratio;
        outPos.x = best->x * fadeT;
        outPos.y = best->y * fadeT;
        outPos.z = best->z * fadeT;
        outW = best->f * fadeT;
        if ((dst->field_0x10 & 2) && acc == 1) {
            // Flash-center fold: average the scaled triple, then pull the
            // previous candidate (or the entry's fallback triple) toward it.
            dst->field_0x10 |= 4;
            f32 val = lbl_eu_8066B02C * ((outPos.x + outPos.y) + outPos.z);
            center.z = val;
            center.y = val;
            center.x = val;
            if (prevBest != NULL) {
                center.x = val + prevBest->x * ratio;
                center.y = val + prevBest->y * ratio;
                center.z = val + prevBest->z * ratio;
            } else {
                center.x = val + item->color[0] * ratio;
                center.y = val + item->color[1] * ratio;
                center.z = val + item->color[2] * ratio;
            }
        } else if (prevBest != NULL) {
            outW += prevBest->f * ratio;
            outPos.x += prevBest->x * ratio;
            outPos.y += prevBest->y * ratio;
            outPos.z += prevBest->z * ratio;
        } else {
            outW += item->wPrev * ratio;
            outPos.x += item->color[0] * ratio;
            outPos.y += item->color[1] * ratio;
            outPos.z += item->color[2] * ratio;
        }
    } else {
        // Inside the bound: push the raw winning row.
        outPos.x = best->x;
        outPos.y = best->y;
        outPos.z = best->z;
        outW = best->f;
        if ((dst->field_0x10 & 2) && acc == 1) {
            dst->field_0x10 |= 4;
            f32 val = lbl_eu_8066B02C * ((outPos.x + outPos.y) + outPos.z);
            center.z = val;
            center.y = val;
            center.x = val;
        }
        if (best->flags14 & 1) {
            dst->field_0x10 |= 1;
        }
    }

    // Phase 2: precomputed-row refresh gated on the controller/entry flags
    // (+0x04 bits 0x18 select the feature, bit 8 the immediate mode).
    int mode = 0;
    if ((self->field_0x04 & 0x18) && (item->flags & 0x00F00000) &&
        (self->field_0x04 & 8)) {
        s16 idx = (s16)self->lgt2.field_0xBE;
        if (item->flags & (0x40000000u << idx)) {
            // Count the selected bits above bit 30-idx (the entry word is
            // re-read per iteration like retail).
            u32 cnt = 0;
            for (int j = 0; j < idx; j++) {
                if (item->flags & (0x40000000u >> j)) cnt++;
            }
            f32* tbl = (f32*)((u8*)self->alt2.field_0x50 +
                              ((item->rowBase + cnt * 0x10) << 2));
            f32* rowK = (f32*)((u8*)tbl + acc * 0x10);
            outPos.x = tbl[acc * 4];
            outPos.y = rowK[1];
            outPos.z = rowK[2];
            outW = rowK[3];
            if (ratio != lbl_eu_8066B010) {
                // Blend all four components toward the preceding table row.
                f32* prevRow = (f32*)((u8*)tbl + (acc - 1) * 0x10);
                outPos.x = tbl[(acc - 1) * 4] * ratio + outPos.x * fadeT;
                outPos.y = prevRow[1] * ratio + rowK[1] * fadeT;
                outPos.z = prevRow[2] * ratio + rowK[2] * fadeT;
                outW = prevRow[3] * ratio + rowK[3] * fadeT;
            }
            self->field_0x04 |= 0x10;
            mode = 1;
        }
    }

    // Fade fold: apply the +0xC0 progress against the sel22 row.
    f32 prog = self->lgt2.field_0xC0;
    if (prog < lbl_eu_8066B014) {
        f32 inv = lbl_eu_8066B014 - prog;
        outW *= prog;
        outPos.x *= prog;
        outPos.y *= prog;
        outPos.z *= prog;
        CScnEnvLgtCtrlLgtItem20* row22 =
            (CScnEnvLgtCtrlLgtItem20*)((u8*)self->alt14.field_0x18_buf +
                                       item->sel22 * 0x14);
        outPos.x += row22->x * inv;
        outPos.y += row22->y * inv;
        outPos.z += row22->z * inv;
        outW += row22->f * inv;
        if (dst->field_0x10 & 8) {
            // Refresh the flash center: when the controller's reset bit is
            // set the new output seeds it, otherwise the old center decays;
            // either way add the per-channel fade weights scaled by inv.
            if (self->field_0x04 & 8) {
                center.x = outPos.x * prog;
                center.y = outPos.y * prog;
                center.z = outPos.z * prog;
            } else {
                center.x *= prog;
                center.y *= prog;
                center.z *= prog;
            }
            center.x += self->lgtView.fadeW[0] * inv;
            center.y += self->lgtView.fadeW[1] * inv;
            center.z += self->lgtView.fadeW[2] * inv;
        }
    } else {
        if (mode == 0) {
            self->field_0x04 &= ~0x30u;
        }
        if (dst->field_0x10 & 8) {
            self->ctrView.ctr = *(const CScnEnvLgtCtrlV3*)&outPos;
            dst->field_0x10 &= ~0x1Cu;
        }
    }

    dst->pos = *(const CScnEnvLgtCtrlV3*)&outPos;
    dst->f = outW;
    if (dst->field_0x10 & 8) {
        self->ctrView.ctr = *(const CScnEnvLgtCtrlV3*)&center;
    }
}

// func_804C4D28 (us-804c8e84): copy the 5-float row selected by (r7, r8) out
// of the light-row array at +0x50 into three outputs (vec3 -> r4, row[3] ->
// r5, row[4] -> r6). Unless the caller's f1 equals the shared .sdata2
// constant, scale the outputs by f2 and blend in the previous row (r8-1)
// scaled by f1 (fmadds).
void func_804C4D28(CScnEnvLgtCtrl* self, f32* out4, f32* out5, f32* out6,
                   f32 f1, f32 f2, u32 r7, u32 r8) {
    f32 v = lbl_eu_8066B010;
    f32* row = (f32*)((u8*)self->alt2.field_0x50 + r7 * 4 + r8 * 0x14);
    f32 x = row[0];
    f32 y;
    f32 z;
    z = row[2];
    y = row[1];
    out4[1] = y;
    out4[0] = x;
    out4[2] = z;
    out5[0] = row[3];
    out6[0] = row[4];
    if (v == f1) return;
    // computed before the output rescale so the base/offset math schedules
    // with the first blend block (retail subi/mulli/add right after beqlr)
    f32* prev = (f32*)((u8*)self->alt2.field_0x50 + r7 * 4 + (r8 - 1) * 0x14);
    out5[0] *= f2;
    out6[0] *= f2;
    out4[0] *= f2;
    out4[1] *= f2;
    out4[2] *= f2;
    out4[0] += prev[0] * f1;
    out4[1] += prev[1] * f1;
    out4[2] += prev[2] * f1;
    out5[0] += prev[3] * f1;
    out6[0] += prev[4] * f1;
}

extern "C" __declspec(noinline) void func_804C4954(CScnEnvLgtCtrl* self,
                                                   void* entry) {}

// reslist<IScnEnvCtl> constructor (retail __ct__reslist_IScnEnvCtl, flat
// name spelled out like the two reslist dtors). Inlines the
// _reslist_base<IScnEnvCtl> ctor (base vtable, zeroed mList/mCapacity/flag,
// self-linked sentinel), overwrites the vtable with the derived reslist
// vtable, initialises the derived light-slot fields, then reserve(8):
// allocates the 0x60-byte node array through the scene owner's handle and
// clears each node's mNext (the mList member is re-loaded per store because
// the store aliases it - retail's unrolled pattern).
void* __ct__reslist_IScnEnvCtl(CScnEnvLgtCtrlResList* self,
                               CScnItemModelNw4rOwner* owner) {
    self->mCount = 0;
    self->mOwner = owner;
    self->base.mVtable = (void*)lbl_eu_8056F978;
    self->base.mList = 0;
    self->base.mCapacity = 0;
    self->base.field_0x1C = 0;
    self->base.mStartNodePtr = &self->base.mStartNode;
    self->base.mStartNodePtr->mNext = self->base.mStartNodePtr;
    self->base.mStartNodePtr->mPrev = self->base.mStartNodePtr;
    self->base.mVtable = (void*)lbl_eu_8056F960;
    self->field_0x28 = 0;
    self->field_0x2C = 0;
    self->field_0x30 = 0;
    self->field_0x34 = lbl_eu_8066AFD0;
    self->field_0x38 = lbl_eu_8066AFD4;
    self->field_0x3C = 0;
    self->field_0x40 = 0;
    self->base.mList = (CScnEnvLgtCtrlIScnResNode*)mtl::MemManager::allocate_array(
        0x60, func_80496018(self->mOwner));
    for (int i = 0; i < 8; i++) {
        self->base.mList[i].mNext = 0;
    }
    self->base.mCapacity = 8;
    self->mCount = 0;
    return self;
}

// _reslist_base<IScnEnvCtl> deleting destructor (retail
// __dt___reslist_base_IScnEnvCtl): install the base vtable, unlink the ring
// (clearList inlined: null each node's mNext), free the node array unless it
// is externally owned (+0x1C), then free the object when deleting > 0. The
// flat retail name cannot be produced by a template instantiation under
// Wii/1.1 (symbolic mangling), so it is spelled out (cf. the flattened
// reslist pattern in MWCC_CASES / CScnFilterMan.cpp).
void* __dt___reslist_base_IScnEnvCtl(CScnEnvLgtCtrlIScnResBase* self, int deleting) {
    if (self != 0) {
        self->mVtable = (void*)lbl_eu_8056F978;
        CScnEnvLgtCtrlIScnResNode* node = self->mStartNodePtr->mNext;
        while (node != self->mStartNodePtr) {
            CScnEnvLgtCtrlIScnResNode* cur = node;
            node = node->mNext;
            cur->mNext = 0;
        }
        self->mStartNodePtr->mNext = self->mStartNodePtr;
        self->mStartNodePtr->mPrev = self->mStartNodePtr;
        if (self->field_0x1C == 0) {
            if (self->mList != 0) {
                delete[] self->mList;
                self->mList = 0;
            }
        }
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}

// reslist<IScnEnvCtl> deleting destructor (retail __dt__reslist_IScnEnvCtl):
// the base destructor body is inlined (retail has no bl to the base dtor),
// keeping its own null check - the nested identical tests reproduce the
// retail double `beq` before the base body (same shape as ~CScnEnvLgtCtrl).
// reslist<IScenEnvCtl> deleting destructor (retail __dt__reslist_IScnEnvCtl):
// the base destructor body is inlined (retail has no bl to the base dtor);
// the nested identical tests reproduce the retail double `beq` off the one
// `cmpwi r3, 0` (outer guard + inlined base-dtor guard).
void* __dt__reslist_IScnEnvCtl(CScnEnvLgtCtrlIScnResBase* self, int deleting) {
    if (self != 0) {
        if (self != 0) {
            self->mVtable = (void*)lbl_eu_8056F978;
            CScnEnvLgtCtrlIScnResNode* node = self->mStartNodePtr->mNext;
            while (self->mStartNodePtr != node) {
                CScnEnvLgtCtrlIScnResNode* cur = node;
                node = node->mNext;
                cur->mNext = 0;
            }
            self->mStartNodePtr->mNext = self->mStartNodePtr;
            self->mStartNodePtr->mPrev = self->mStartNodePtr;
            if (self->field_0x1C == 0) {
                if (self->mList != 0) {
                    delete[] self->mList;
                    self->mList = 0;
                }
            }
        }
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}

// __dt__804C0E48 (us-804c4fa4): full destructor of the reslist-derived
// env-light controller (CScnEnvLgtCtrlResList: {+0x00 flags, +0x04 owner,
// _reslist_base at +0x08, blend at +0x28, bloom at +0x2C}). Releases every
// ring item through its deleting dtor (vtable slot 2) and nulls the item
// slot, then runs the node-array teardown (clearList x2 + delete[] when not
// externally owned) and zeroes the capacity. When +0x00 bit 3 is set the
// blend/bloom pair is released (blend via its slot-2 dtor, bloom via the
// main vtable at +0x80 slot 7). Finally the inlined _reslist_base dtor
// restores the base vtable, re-clearLists and frees the node array again.
// `deleting > 0` frees the object through operator delete.
void* __dt__804C0E48(CScnEnvLgtCtrlResList* self, int deleting) {
    if (self != 0) {
        CScnEnvLgtCtrlIScnResBase* base;
        CScnEnvLgtCtrlListNode* node =
            (CScnEnvLgtCtrlListNode*)self->base.mStartNodePtr->mNext;
        while (node != (CScnEnvLgtCtrlListNode*)self->base.mStartNodePtr) {
            CScnEnvLgtCtrlListItem* item = node->mItem;
            if (item != 0) {
                if (item != 0) {
                    item->v0(1);
                }
                node->mItem = 0;
            }
            node = node->mNext;
        }
        CScnEnvLgtCtrlIScnResNode* n1 = self->base.mStartNodePtr->mNext;
        while (n1 != self->base.mStartNodePtr) {
            CScnEnvLgtCtrlIScnResNode* cur = n1;
            n1 = n1->mNext;
            cur->mNext = 0;
        }
        self->base.mStartNodePtr->mNext = self->base.mStartNodePtr;
        self->base.mStartNodePtr->mPrev = self->base.mStartNodePtr;
        CScnEnvLgtCtrlIScnResNode* n2 = self->base.mStartNodePtr->mNext;
        while (n2 != self->base.mStartNodePtr) {
            CScnEnvLgtCtrlIScnResNode* cur = n2;
            n2 = n2->mNext;
            cur->mNext = 0;
        }
        self->base.mStartNodePtr->mNext = self->base.mStartNodePtr;
        self->base.mStartNodePtr->mPrev = self->base.mStartNodePtr;
        if (self->base.field_0x1C == 0) {
            if (self->base.mList != 0) {
                delete[] self->base.mList;
                self->base.mList = 0;
            }
        }
        self->base.mCapacity = 0;
        if (self->flags & 0x8) {
            if (self->field_0x28 != 0) {
                if (self->field_0x28 != 0) {
                    self->field_0x28->release(1);
                }
                self->field_0x28 = 0;
            }
            if (self->field_0x2C != 0) {
                if (self->field_0x2C != 0) {
                    self->field_0x2C->release(1);
                }
                self->field_0x2C = 0;
            }
        }
        // base dtor inlined: the double null-check on the base subobject
        // reproduces the retail `addic. r29, r30, 0x8; beq; beq` shape. The
        // base pointer is declared early (drives the saved-register color)
        // but assigned only here (deferred `addic` like retail).
        base = &self->base;
        if (base != 0) {
            if (base != 0) {
                base->mVtable = (void*)lbl_eu_8056F978;
                CScnEnvLgtCtrlIScnResNode* n3 = base->mStartNodePtr->mNext;
                while (n3 != base->mStartNodePtr) {
                    CScnEnvLgtCtrlIScnResNode* cur = n3;
                    n3 = n3->mNext;
                    cur->mNext = 0;
                }
                base->mStartNodePtr->mNext = base->mStartNodePtr;
                base->mStartNodePtr->mPrev = base->mStartNodePtr;
                if (base->field_0x1C == 0) {
                    if (base->mList != 0) {
                        delete[] base->mList;
                        base->mList = 0;
                    }
                }
            }
        }
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}

extern "C" bool func_804C5198(CScnEnvLgtCtrlLgtView* self, CScnEnvLgtCtrlLgtData* out) {
    if (self->flags & 0x200) {
        const CScnEnvLgtCtrlLgtData* src = &self->data;
        // Decl order fixes the register colors; assignment order keeps the
        // retail load sequence.
        const u32* s = &src->field_0x00;
        u32 v8;
        u32 v4;
        u32 v0;
        u32 vC;
        vC = s[3];
        v8 = s[2];
        v4 = s[1];
        v0 = s[0];
        out->field_0x00 = v0;
        out->field_0x04 = v4;
        out->field_0x08 = v8;
        out->field_0x0C = vC;
        return true;
    }
    return false;
}

extern "C" bool func_804C51D4(void* r3, void* r4) {
    if (!(*(unsigned int*)((char*)r3 + 4) & 0x400)) return false;
    *(unsigned int*)((char*)r4 + 0) = *(unsigned int*)((char*)r3 + 0x64);
    *(unsigned int*)((char*)r4 + 4) = *(unsigned int*)((char*)r3 + 0x68);
    *(unsigned int*)((char*)r4 + 8) = *(unsigned int*)((char*)r3 + 0x6c);
    *(unsigned int*)((char*)r4 + 0xc) = *(unsigned int*)((char*)r3 + 0x70);
    return true;
}

// Push the light view's ambient color triple into CScnEnvLgtData::mAmbColorBase.
// Outermost flag (mFlags bit 0) picks between the view's +0x64 triple (bit 0x400)
// and its +0x54 triple (bit 0x200); each path early-returns when its bit is clear.
extern "C" void func_804C5210(CScnEnvLgtCtrlLgtView* view, CScnEnvLgtData* data) {
    if (data->mFlags & 1) {
        if (view->flags & 0x400) {
            data->mAmbColorBase[0] = view->field_0x64[0];
            data->mAmbColorBase[1] = view->field_0x64[1];
            data->mAmbColorBase[2] = view->field_0x64[2];
        }
    } else if (view->flags & 0x200) {
        data->mAmbColorBase[0] = view->field_0x54[0];
        data->mAmbColorBase[1] = view->field_0x54[1];
        data->mAmbColorBase[2] = view->field_0x54[2];
    }
}

// func_804C526C (us-804c93c8): refresh the current light slot. When
// self->field_0xAC (the light-count cap) does not exceed the new index r6,
// return the stored +0xE4 slot field unchanged. Otherwise record the index,
// the r5 color triple and f1, then (when the +0x484 bit-0 path is selected
// and the +0x30 control exists) optionally refresh the control via
// func_804C3C9C and recompute the slot field via func_804C64A8; the
// +0x8000 flag is set around the recompute on the bit-0-clear path. The
// return value is the recomputed slot field, or the original r6 when no
// recompute ran.
extern "C" u32 func_804C526C(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtEnv484* env,
                  const u32* r5, u32 r6, float f1) {
    if (self->alt5.field_0xAC <= (s32)r6) {
        return self->field_0xE4;
    }
    u32 ret = r6;
    self->field_0xEC = f1;
    self->field_0xE4 = r6;
    self->field_0xD0 = r5[0];
    self->field_0xD4 = r5[1];
    self->field_0xD8 = r5[2];
    if (env->field_0x484 & 1) {
        if (self->field_0x30 != 0) {
            if (self->field_0x04 & 0x1000) {
                func_804C3C9C(self, (CLight*)env, 1);
            }
            ret = func_804C64A8(
                self, (CLight*)((u8*)env + self->field_0xE4 * 0x40), 3, 1);
        }
    } else {
        if (self->field_0x30 != 0) {
            if (self->field_0x04 & 0x800) {
                func_804C3C9C(self, (CLight*)env, 0);
            }
            self->field_0x04 |= 0x8000;
            ret = func_804C64A8(
                self, (CLight*)((u8*)env + self->field_0xE4 * 0x40), 0, 1);
            self->field_0x04 &= ~0x8000;
        }
    }
    return ret;
}


// func_804C5380 (us-804c94dc): push fog view 0 into the scene root's fog
// slot 0. Gated by the +0x40 fog-enable object's bit 0; the FogData fields
// (type/color/start/end) come from mFog view 0 (+0xF8/+0xF0/+0x100/+0x108).
extern "C" void func_804C5380(CScnEnvLgtCtrl* self, nw4r::g3d::ScnRoot* root) {
    CScnEnvLgtCtrlFogGate* gate = self->alt2.field_0x40;
    if (gate != NULL && (gate->mFlags & 1)) {
        nw4r::g3d::Fog fog = root->GetFog(0);
        u32 fogType = self->mFog.mType[0];
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC00, 0x41, lbl_eu_8056EBE0);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->type = (GXFogType)fogType;
        }
        union { u32 u; GXColor c; } cv;
        cv.u = self->mFog.mColor[0];
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC60, 0x63, lbl_eu_8056EC40);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->color = cv.c;
        }
        f32 start;
        f32 end;
        end = self->mFog.mEnd[0];
        start = self->mFog.mStart[0];
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC30, 0x4b, lbl_eu_8056EC10);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->startz = start;
            fog.ptr()->endz = end;
        }
        nw4r::g3d::G3DState::SetFog(fog, 0);
    }
}

// func_804C54D4 (us-804c9630): mirror of func_804C5380 pushing fog view 1
// (+0xF4/+0xFC/+0x104/+0x10C).
extern "C" void func_804C54D4(CScnEnvLgtCtrl* self, nw4r::g3d::ScnRoot* root) {
    CScnEnvLgtCtrlFogGate* gate = self->alt2.field_0x40;
    if (gate != NULL && (gate->mFlags & 1)) {
        nw4r::g3d::Fog fog = root->GetFog(0);
        u32 fogType = self->mFog.mType[1];
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC00, 0x41, lbl_eu_8056EBE0);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->type = (GXFogType)fogType;
        }
        union { u32 u; GXColor c; } cv;
        cv.u = self->mFog.mColor[1];
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC60, 0x63, lbl_eu_8056EC40);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->color = cv.c;
        }
        f32 start;
        f32 end;
        end = self->mFog.mEnd[1];
        start = self->mFog.mStart[1];
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC30, 0x4b, lbl_eu_8056EC10);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->startz = start;
            fog.ptr()->endz = end;
        }
        nw4r::g3d::G3DState::SetFog(fog, 0);
    }
}

// Byte -> float through the 0x43300000 double temp (retail subtracts the
// shared 2^52 constant lbl_eu_8066B040).
static inline f32 byteToF_b040(u8 v) {
    union { u32 w[2]; double d; } c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v;
    return (f32)(c.d - lbl_eu_8066B040);
}

// func_804C5628 (us-804c9784): push this controller's fog state into the
// scene root's fog slot 0 when the +0x40 gate permits it. The fog type word
// (view 0/1 of mFog.mType) is selected by the sign of f; the color bytes at
// mFog.mColor are converted from bytes and blended between view 0 and view 1
// by t = 1.0 - f, clamped to the shared ceiling; start/end get the same
// blend from mFog.mStart/mEnd. The fog slot is asserted free (Panic when a
// fog already exists) before every store.
extern "C" void func_804C5628(CScnEnvLgtCtrl* self, nw4r::g3d::ScnRoot* root,
                              float f) {
    // Byte -> float conversion temps: two shared 0x43300000 doubles
    // (retail sp+0x18 / sp+0x20); declared up front so MWCC hoists their
    // init into the prologue like retail.
    union { u32 w[2]; double d; } ta, tb;
    ta.w[0] = 0x43300000u;
    tb.w[0] = 0x43300000u;
    // Nested guards so both checks fall through to the call (retail emits
    // exactly two beq's and no join branch).
    CScnEnvLgtCtrlFogGate* gate = self->alt2.field_0x40;
    if (gate != NULL && (gate->mFlags & 1)) {
        nw4r::g3d::Fog fog = root->GetFog(0);
        // View select: explicit lt||eq comes closest to retail's single
        // fcmpo + cror eq,lt,eq shape.
        u32 type;
        if (f < lbl_eu_8066B010 || f == lbl_eu_8066B010) {
            type = self->mFog.mType[1];
        } else {
            type = self->mFog.mType[0];
        }
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC00, 0x41, lbl_eu_8056EBE0);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->type = (GXFogType)type;
        }
        f32 t = lbl_eu_8066B014 - f;
        // Color bytes: each channel blends the two views' bytes (view-0
        // weighted by f, view-1 by t), then clamps to the shared ceiling.
        u8* cb = (u8*)self->mFog.mColor;
        tb.w[1] = cb[4];
        ta.w[1] = cb[0];
        f32 r = (f32)(tb.d - lbl_eu_8066B040) * t +
                (f32)(ta.d - lbl_eu_8066B040) * f;
        tb.w[1] = cb[5];
        ta.w[1] = cb[1];
        f32 g = (f32)(tb.d - lbl_eu_8066B040) * t +
                (f32)(ta.d - lbl_eu_8066B040) * f;
        tb.w[1] = cb[6];
        ta.w[1] = cb[2];
        f32 b = (f32)(tb.d - lbl_eu_8066B040) * t +
                (f32)(ta.d - lbl_eu_8066B040) * f;
        if (r > lbl_eu_8066B030) r = lbl_eu_8066B030;
        if (g > lbl_eu_8066B030) g = lbl_eu_8066B030;
        if (b > lbl_eu_8066B030) b = lbl_eu_8066B030;
        // Retail builds the full GXColor on the stack from a seeded template
        // word (alpha comes along in the struct copy).
        union { u32 u; GXColor c; } cv;
        cv.u = lbl_eu_8066B048;
        cv.c.r = (u8)r;
        cv.c.g = (u8)g;
        cv.c.b = (u8)b;
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC60, 0x63, lbl_eu_8056EC40);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->color = cv.c;
        }
        // Near/far blend across the two views.
        f32 fa = self->mFog.mStart[0] * f + self->mFog.mStart[1] * t;
        f32 fb = self->mFog.mEnd[0] * f + self->mFog.mEnd[1] * t;
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC30, 0x4b, lbl_eu_8056EC10);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->startz = fa;
            fog.ptr()->endz = fb;
        }
        nw4r::g3d::G3DState::SetFog(fog, 0);
    }
}

// func_804C58D8 (us-804c9a34): when the light-count control at +0x48 exists
// and +0x04 bit 0 is set, scan the 0xd8-byte entry array at +0x14 for the
// first entry whose u16 type word is 1, hand it to func_804C6D64 and toggle
// arg +0x08 bit 1 on the result; return 1 on a hit, 0 otherwise. The loop
// keeps the byte-offset (off) and index (i) double induction so MWCC emits
// the add-for-the-type-check / mulli-for-the-call pair like retail.
extern "C" int func_804C58D8(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtTarget* arg,
                              int row) {
    CScnEnvLgtCtrlLgtCtl* ctl = self->alt2.field_0x48;
    if (ctl != 0 && (self->field_0x04 & 1)) {
        u32 off = 0;
        u32 count = ctl->mCount;
        u32 i = 0;
        for (; i < count; off += 0xd8, i++) {
            if (*(u16*)((u8*)self->field_0x14_ptr + i * 0xd8 + 2) == 1) {
                // Forward the caller's row value (retail leaves r5 undefined
                // at this call - both sides treat it as don't-care).
                if (func_804C6D64((u8*)self->field_0x14_ptr + i * 0xd8, arg, row)) {
                    arg->field_0x08 |= 2;
                } else {
                    arg->field_0x08 &= ~2u;
                }
                return 1;
            }
        }
    }
    return 0;
}

// func_804C5990 (us-804c9aec): twin of func_804C58D8 with +0x04 bit 1 as
// the guard, u16 type word 2 as the selector, func_804C6F78 as the dispatch
// and arg +0x64 bit 0 toggled on the result.
extern "C" int func_804C5990(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtTarget* arg,
                              int row) {
    CScnEnvLgtCtrlLgtCtl* ctl = self->alt2.field_0x48;
    if (ctl != 0 && (self->field_0x04 & 2)) {
        u32 off = 0;
        u32 count = ctl->mCount;
        u32 i = 0;
        for (; i < count; off += 0xd8, i++) {
            if (*(u16*)((u8*)self->field_0x14_ptr + i * 0xd8 + 2) == 2) {
                // See func_804C58D8: forward the caller's row value.
                if (func_804C6F78((u8*)self->field_0x14_ptr + i * 0xd8, arg, row)) {
                    arg->field_0x64b |= 1;
                } else {
                    arg->field_0x64b &= ~1;
                }
                return 1;
            }
        }
    }
    return 0;
}

// func_804C5A48 (us-804c9ba4): twin of func_804C58D8 with +0x04 bit 2 as
// the guard, u16 type word 3 as the selector, func_804C6F78 as the dispatch
// and arg +0x50 bit 1 toggled on the result.
extern "C" int func_804C5A48(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtTarget* arg,
                              int row) {
    CScnEnvLgtCtrlLgtCtl* ctl = self->alt2.field_0x48;
    if (ctl != 0 && (self->field_0x04 & 4)) {
        u32 off = 0;
        u32 count = ctl->mCount;
        u32 i = 0;
        for (; i < count; off += 0xd8, i++) {
            if (*(u16*)((u8*)self->field_0x14_ptr + i * 0xd8 + 2) == 3) {
                // See func_804C58D8: forward the caller's row value.
                if (func_804C6F78((u8*)self->field_0x14_ptr + i * 0xd8, arg, row)) {
                    arg->field_0x50 |= 2;
                } else {
                    arg->field_0x50 &= ~2u;
                }
                return 1;
            }
        }
    }
    return 0;
}

// func_804C5B00 (us-804c9c5c): toggle the +0x04 bit-0x8 light-control flag
// (set when r4 != 0, clear otherwise) and update the s16 slot index at
// +0xBE (clamped to 1 when r4 != 0). Both paths run a light refresh through
// func_804C6110 when +0x04 bit 0x40 is set - the bit-0x8-set path only when
// the stored +0xBE index differs from the new one - and otherwise reset the
// +0xC0 color to the shared .sdata2 constant. Then the +0xC0/+0xC4 color
// pair is derived from f1 (B014/f1, or both set to B014 when f1 equals the
// 1.0 constant).
extern "C" __declspec(noinline) void func_804C6110(CScnEnvLgtCtrl* self);

extern "C" void func_804C5B00(CScnEnvLgtCtrl* self, int r4, int r5, float f1) {
    if (r4 != 0) {
        if (r5 >= 2) r5 = 1;
        u32 flags = self->field_0x04;
        if (flags & 0x40) {
            if (flags & 0x8) {
                if (r5 != self->lgt2.field_0xBE) {
                    func_804C6110(self);
                }
            } else {
                func_804C6110(self);
            }
        } else {
            self->lgt2.field_0xC0 = lbl_eu_8066B014;
        }
        self->lgt2.field_0xBE = r5;
        self->field_0x04 |= 0x8;
    } else {
        u32 flags = self->field_0x04;
        if (flags & 0x40) {
            if (flags & 0x8) {
                func_804C6110(self);
            }
        } else {
            self->lgt2.field_0xC0 = lbl_eu_8066B014;
        }
        self->field_0x04 &= ~0x8;
    }
    if (f1 != lbl_eu_8066B010) {
        self->lgt2.field_0xC4 = lbl_eu_8066B014 / f1;
    } else {
        f32 v = lbl_eu_8066B014;
        self->lgt2.field_0xC0 = v;
        self->lgt2.field_0xC4 = v;
    }
}


// Set/clear one bit in the +0x28 bit array. bit is validated < 0x140 (320 bits);
// `clear` non-zero clears the bit (andc), zero sets it (or).
extern "C" void func_804C5C08(CScnEnvLgtCtrl* self, int bit, int clear) {
    u32* arr = self->field_0x28;
    if (arr == 0) return;
    if (bit >= 0x140) return;
    if (clear != 0) {
        arr[bit >> 5] &= ~(1u << (bit & 31));
    } else {
        arr[bit >> 5] |= (1u << (bit & 31));
    }
}

// func_804C5C6C (us-804c9dc8): update the light-flash state. With +0x04 bit
// 0x4000 set, re-dispatch the stored s16 slot index through the controller
// vtable slot 0x70; then clear +0x04 bits 0xC000 and, unless f1 exceeds the
// shared .sdata2 bound, dispatch v26(r4, r5) and reset +0xBC to -1.
// Otherwise store the new slot index, dispatch v26((s16)r4, 1), reset the
// +0xB0/+0xB4/+0xB8 timers and record the direction bit (0x2000 when
// r5 != 0, else 0x4000).
extern "C" void func_804C5C6C(CScnEnvLgtCtrl* self, int r4, int r5, float f1) {
    if (self->field_0x04 & 0x4000) {
        ((CScnEnvLgtCtrlVt*)self)->v26(self->field_0xBC, 0);
    }
    f32 v = lbl_eu_8066B010;
    self->field_0x04 &= ~0x6000;
    if (f1 <= v) {
        ((CScnEnvLgtCtrlVt*)self)->v26(r4, r5);
        self->field_0xBC = -1;
    } else {
        self->field_0xBC = r4;
        ((CScnEnvLgtCtrlVt*)self)->v26((s16)r4, 1);
        f32 w = lbl_eu_8066B010;
        self->alt3.field_0xB0 = w;
        self->alt3.field_0xB4 = f1;
        self->field_0xB8 = w;
        if (r5 != 0) {
            self->field_0x04 |= 0x2000;
        } else {
            self->field_0x04 |= 0x4000;
        }
    }
}


extern "C" void func_804C5D7C(void* self, unsigned int* out) {
    struct SelfStruct {
        char pad[0x40];
        void* ptr;
    };
    SelfStruct* s = static_cast<SelfStruct*>(self);
    void* subPtr = s->ptr;
    if (subPtr == nullptr) return;
    struct SubStruct {
        unsigned short flags;
        unsigned short value;
    };
    SubStruct* sub = static_cast<SubStruct*>(subPtr);
    if (!(sub->flags & 1)) return;
    *out = sub->value;
}

// func_804C5DA0 (us-804c9efc): set/clear one of two light-control flag bits
// on the controller (0x1000 when r5 != 0, else 0x800); r4 selects set vs
// clear. No-op while the light-slot array at +0x24 is not yet allocated.
extern "C" void func_804C5DA0(CScnEnvLgtCtrl* self, int r4, int r5) {
    if (self->field_0x24 == 0) return;
    if (r5 != 0) {
        if (r4 != 0)
            self->field_0x04 |= 0x1000;
        else
            self->field_0x04 &= ~0x1000;
    } else {
        if (r4 != 0)
            self->field_0x04 |= 0x800;
        else
            self->field_0x04 &= ~0x800;
    }
}

// func_804C5E04 (us-804c9f60): write two vec3s (r7 -> +0x00, r6 -> +0x18)
// into the selected light slot (same index math as func_804C5F6C) and poke
// the slot's +0x28 flags: set bit 0, clear bit 1. The member accesses are
// written as self->field_0x24[r5] each time: the stores through the slot
// alias the field, so MWCC reloads the base after each (as retail does).
extern "C" void func_804C5E04(CScnEnvLgtCtrl* self, int r4, int r5,
                   const CScnEnvLgtCtrlLgtVec3* r6,
                   const CScnEnvLgtCtrlLgtVec3* r7) {
    if (self->field_0x24 == 0) return;
    if (r5 > 1) r5 = 0;
    r5 += ((-r4 | r4) >> 31) & 2;
    self->field_0x24[r5].field_0x00 = *r7;
    self->field_0x24[r5].field_0x18 = *r6;
    self->field_0x24[r5].field_0x28 |= 0x1;
    self->field_0x24[r5].field_0x28 &= ~0x2;
}

// func_804C5E9C (us-804c9ff8): same slot-index math as func_804C5E04 with a
// third flavor: the r6 vec3 is copied to both +0x00 and +0x0C, the +0x18
// triple is filled with the shared .sdata2 float's bit pattern, the +0x24
// word receives f1 and the +0x28 flags get bits 0 and 1 set. Each
// float->u32 bit copy goes through its own stack temp, so MWCC emits the
// retail lfs/stfs-to-stack/lwz roundtrip per element.
extern "C" void func_804C5E9C(CScnEnvLgtCtrl* self, int r4, int r5,
                   const CScnEnvLgtCtrlLgtVec3* r6, float f1) {
    if (self->field_0x24 == 0) return;
    if (r5 > 1) r5 = 0;
    f32 v = lbl_eu_8066B010;
    r5 += ((-r4 | r4) >> 31) & 2;
    // explicit base refetch per member block: the stores through the slot
    // alias field_0x24, so retail reloads and re-adds the stride each time
    CScnEnvLgtCtrlLgtSlot* slot = &self->field_0x24[r5];
    slot->field_0x00 = *r6;
    slot = &self->field_0x24[r5];
    slot->field_0x0C.w = *r6;
    slot = &self->field_0x24[r5];
    // Each bit-cast uses its own float local so MWCC gives it a dedicated
    // stack temp (retail: x->sp+0x8, y->sp+0xc, z->sp+0x10).
    f32 tz = v;
    f32 ty = v;
    f32 tx = v;
    // Aggregate float triple: natural layout pins the roundtrip temps to
    // sp+0x8/0xc/0x10 like retail. Reference helper avoids per-call copies.
    CScnEnvLgtCtrlLgtSum t;
    t.x = v;
    t.y = v;
    t.z = v;
    slot->field_0x18.x = *(u32*)&t.x;
    slot->field_0x18.y = *(u32*)&t.y;
    slot->field_0x18.z = *(u32*)&t.z;
    slot = &self->field_0x24[r5];
    slot->field_0x24 = f1;
    slot = &self->field_0x24[r5];
    slot->field_0x28 |= 0x1;
    slot = &self->field_0x24[r5];
    slot->field_0x28 |= 0x2;
}

// Clear bit 0 of a light slot's +0x28 flags. Slot index: r5 clamped to 0..1,
// plus 2 when r4 is non-zero (the (r4 | -r4) >> 31 idiom yields -1 iff r4 != 0).
extern "C" void func_804C5F6C(CScnEnvLgtCtrl* self, int r4, int r5) {
    CScnEnvLgtCtrlLgtSlot* slot = self->field_0x24;
    if (slot == 0) return;
    if (r5 > 1) r5 = 0;
    r5 += ((-r4 | r4) >> 31) & 2;
    slot[r5].field_0x28 &= 0xFFFE;
}

// Set bit 2 of a light slot's +0x28 flags and write the three u16 params at
// +0x2A..+0x2E (curve values: base + idx*60, base + idx*60, and a raw u16).
extern "C" void func_804C5FB0(CScnEnvLgtCtrl* self, int r4, int r5, int r6, int r7, int r8, int r9, int r10) {
    CScnEnvLgtCtrlLgtSlot* slot = self->field_0x24;
    if (slot == 0) return;
    if (r5 > 1) r5 = 0;
    r5 += ((-r4 | r4) >> 31) & 2;
    CScnEnvLgtCtrlLgtSlot* s = &slot[r5];
    s->field_0x28 |= 0x4;
    s->field_0x2A = (u16)(r7 + r6 * 0x3C);
    s->field_0x2C = (u16)(r9 + r8 * 0x3C);
    s->field_0x2E = (u16)r10;
}

// Clear bit 2 of a light slot's +0x28 flags (same slot-index math as func_804C5F6C).
extern "C" void func_804C6010(CScnEnvLgtCtrl* self, int r4, int r5) {
    CScnEnvLgtCtrlLgtSlot* slot = self->field_0x24;
    if (slot == 0) return;
    if (r5 > 1) r5 = 0;
    r5 += ((-r4 | r4) >> 31) & 2;
    slot[r5].field_0x28 &= 0xFFFB;
}

// func_804C6054 (us-804ca1b0): walk the 0x3C-stride parameter array at
// field_0x2C + mOffset for the first entry with bit 15 set; poke bit 1 of
// the item row (0x14-stride from +0x0C) selected by the entry's u16 index:
// set when r4 != 0, clear otherwise. The two loops are duplicated in the
// source so MWCC emits the two identical retail walk bodies (the branch on
// r4 happens before either walk's setup).
extern "C" void func_804C6054(CScnEnvLgtCtrl* self, int set) {
    CScnEnvLgtCtrlLgtParamCtl* ctl = self->field_0x30;
    if (ctl == 0) return;
    if (set != 0) {
        u32 off = ctl->mOffset;
        CScnEnvLgtCtrlLgtParamEntry* p =
            (CScnEnvLgtCtrlLgtParamEntry*)(self->field_0x2C + off);
        u32 count = ctl->mCount;
        for (u32 i = 0; i < count; i++, p++) {
            if (p->field_0x04 & 0x10000) {
                u32 idx = p->field_0x20;
                CScnEnvLgtCtrlLgtItem20* item =
                    (CScnEnvLgtCtrlLgtItem20*)((u8*)self->field_0x0C +
                                               idx * 0x14);
                item->field_0x10 |= 2;
                return;
            }
        }
    } else {
        u32 off = ctl->mOffset;
        CScnEnvLgtCtrlLgtParamEntry* p =
            (CScnEnvLgtCtrlLgtParamEntry*)(self->field_0x2C + off);
        u32 count = ctl->mCount;
        for (u32 i = 0; i < count; i++, p++) {
            if (p->field_0x04 & 0x10000) {
                u32 idx = p->field_0x20;
                CScnEnvLgtCtrlLgtItem20* item =
                    (CScnEnvLgtCtrlLgtItem20*)((u8*)self->field_0x0C +
                                               idx * 0x14);
                item->field_0x10 &= ~2u;
                return;
            }
        }
    }
}

// Flat retail name; defined later in this TU (row refresh per 0xD8 entry).
extern "C" void func_804C7530(CScnEnvLgtCtrl* self);

// func_804C6110 (us-804ca26c): per-frame light-row refresh. Walks the four
// control sub-arrays (strides 0x30/0x3C/0x50/0x64): entries whose flags have
// any of bits 10-13 set copy their indexed 0x14-byte source row
// (self+0x0C array) into the destination row (self+0x18 array), and when the
// copied row's flag word has bit 30 set the +0x90 color triple rotates into
// +0x9C. Then walks the fog-gate's entry list (+0x3C stride, bits 27-30)
// copying 0x1C rows between the self+0x10 / self+0x1C arrays, and finally
// calls func_804C7530 on every 0xD8 entry of the +0x14 array before storing
// the shared constant into +0xC0.
extern "C" __declspec(noinline) void func_804C6110(CScnEnvLgtCtrl* self) {
    // Field accesses go through self (retail colors self r31 and reloads
    // each field instead of caching the view pointer).
#define LGT_VIEW ((CScnEnvLgtCtrlRowsView volatile*)self)
    CScnEnvLgtCtrlCtorCtl* ctl = LGT_VIEW->ctl30;
    if (ctl != NULL) {
        // Passes A-D over the four control sub-arrays; the counts are
        // re-read through self each iteration (retail reloads the +0x30
        // handle every loop test).
        CScnEnvLgtCtrlWorkEnt30* eA =
            (CScnEnvLgtCtrlWorkEnt30*)(LGT_VIEW->base2C + ctl->mBaseA);
        for (u32 i = 0; i < LGT_VIEW->ctl30->mCountA; i++, eA++) {
            if (eA->flags & 0x003C0000) {
                CScnEnvLgtCtrlRow14* src =
                    &LGT_VIEW->rows0C[eA->idxSrc];
                CScnEnvLgtCtrlRow14* dst =
                    &LGT_VIEW->rows18[eA->idxDst];
                dst->field_0x00 = src->field_0x00;
                dst->field_0x04 = src->field_0x04;
                dst->field_0x08 = src->field_0x08;
                dst->field_0x0C = src->field_0x0C;
                dst->field_0x10 = src->field_0x10;
                if (src->field_0x10 & 2) {
                    // Rotate the +0x90 ambient triple into +0x9C.
                    LGT_VIEW->rot.field_0x9C = LGT_VIEW->rot.field_0x90;
                    LGT_VIEW->rot.field_0xA0 = LGT_VIEW->rot.field_0x94;
                    LGT_VIEW->rot.field_0xA4 = LGT_VIEW->rot.field_0x98;
                }
            }
        }
        CScnEnvLgtCtrlWorkEnt3C* eB =
            (CScnEnvLgtCtrlWorkEnt3C*)(LGT_VIEW->base2C + ctl->mBaseB);
        for (u32 i = 0; i < LGT_VIEW->ctl30->mCountB; i++, eB++) {
            if (eB->flags & 0x003C0000) {
                CScnEnvLgtCtrlRow14* src =
                    &LGT_VIEW->rows0C[eB->idxSrc];
                CScnEnvLgtCtrlRow14* dst =
                    &LGT_VIEW->rows18[eB->idxDst];
                dst->field_0x00 = src->field_0x00;
                dst->field_0x04 = src->field_0x04;
                dst->field_0x08 = src->field_0x08;
                dst->field_0x0C = src->field_0x0C;
                dst->field_0x10 = src->field_0x10;
                if (src->field_0x10 & 2) {
                    LGT_VIEW->rot.field_0x9C = LGT_VIEW->rot.field_0x90;
                    LGT_VIEW->rot.field_0xA0 = LGT_VIEW->rot.field_0x94;
                    LGT_VIEW->rot.field_0xA4 = LGT_VIEW->rot.field_0x98;
                }
            }
        }
        CScnEnvLgtCtrlWorkEnt50* eC =
            (CScnEnvLgtCtrlWorkEnt50*)(LGT_VIEW->base2C + ctl->mBaseC);
        for (u32 i = 0; i < LGT_VIEW->ctl30->mCountC; i++, eC++) {
            if (eC->flags & 0x003C0000) {
                CScnEnvLgtCtrlRow14* src =
                    &LGT_VIEW->rows0C[eC->idxSrc];
                CScnEnvLgtCtrlRow14* dst =
                    &LGT_VIEW->rows18[eC->idxDst];
                dst->field_0x00 = src->field_0x00;
                dst->field_0x04 = src->field_0x04;
                dst->field_0x08 = src->field_0x08;
                dst->field_0x0C = src->field_0x0C;
                dst->field_0x10 = src->field_0x10;
                if (src->field_0x10 & 2) {
                    LGT_VIEW->rot.field_0x9C = LGT_VIEW->rot.field_0x90;
                    LGT_VIEW->rot.field_0xA0 = LGT_VIEW->rot.field_0x94;
                    LGT_VIEW->rot.field_0xA4 = LGT_VIEW->rot.field_0x98;
                }
            }
        }
        CScnEnvLgtCtrlWorkEnt64* eD =
            (CScnEnvLgtCtrlWorkEnt64*)(LGT_VIEW->base2C + ctl->mBaseD);
        for (u32 i = 0; i < LGT_VIEW->ctl30->mCountD; i++, eD++) {
            if (eD->flags & 0x003C0000) {
                CScnEnvLgtCtrlRow14* src =
                    &LGT_VIEW->rows0C[eD->idxSrc];
                CScnEnvLgtCtrlRow14* dst =
                    &LGT_VIEW->rows18[eD->idxDst];
                dst->field_0x00 = src->field_0x00;
                dst->field_0x04 = src->field_0x04;
                dst->field_0x08 = src->field_0x08;
                dst->field_0x0C = src->field_0x0C;
                dst->field_0x10 = src->field_0x10;
                if (src->field_0x10 & 2) {
                    LGT_VIEW->rot.field_0x9C = LGT_VIEW->rot.field_0x90;
                    LGT_VIEW->rot.field_0xA0 = LGT_VIEW->rot.field_0x94;
                    LGT_VIEW->rot.field_0xA4 = LGT_VIEW->rot.field_0x98;
                }
            }
        }
    }
    // Fog-gate pass: copy the gated 0x1C rows between the two arrays.
    CScnEnvLgtCtrlFogGate* gate = LGT_VIEW->gate40;
    if (gate != NULL && gate->mCount != 0) {
        CScnEnvLgtCtrlFogEnt* eF =
            (CScnEnvLgtCtrlFogEnt*)(LGT_VIEW->base2C + gate->mOffset);
        for (u32 i = 0; i < gate->mCount; i++, eF++) {
            if (!(eF->flags & 0x1E)) {
                continue;
            }
            CScnEnvLgtCtrlRow1C* src = &LGT_VIEW->rows10[eF->idxSrc];
            CScnEnvLgtCtrlRow1C* dst = &LGT_VIEW->rows1C[eF->idxDst];
            dst->field_0x00 = src->field_0x00;
            dst->field_0x04 = src->field_0x04;
            dst->field_0x08 = src->field_0x08;
            dst->field_0x0C = src->field_0x0C;
            dst->field_0x10 = src->field_0x10;
            dst->field_0x18 = src->field_0x18;
        }
    }
    // Refresh every 0xD8 entry of the +0x14 array (the +0x48 handle and
    // count are re-read from self every iteration - retail shape).
    if (LGT_VIEW->ctl48 != NULL) {
        for (u32 i = 0; i < LGT_VIEW->ctl48->mCount; i++) {
            func_804C7530((CScnEnvLgtCtrl*)(LGT_VIEW->rows14 + i));
        }
    }
    LGT_VIEW->field_0xC0 = lbl_eu_8066B010;
#undef LGT_VIEW
}

extern "C" __declspec(noinline) u32 func_804C64A8(CScnEnvLgtCtrl* self,
                                                    CLight* light, u32 slot,
                                                    u32 reset) {
    return 0;  // stub - separate target (us-804ca604)
}

// func_804C678C (us-804ca8e8): rank the gate's phase-1 light rows by
// distance from the controller's ambient position and record the best ones
// into the +0x110/+0x130/+0x150/+0x160 tables (bounded by the +0xAC cap,
// replacing the lowest-priority entry once full), then walk the phase-2
// vec4 rows and recurse into each row whose radius reaches it.
extern "C" __declspec(noinline) void func_804C678C(CScnEnvLgtCtrl* self,
                                                   CScnEnvLgtCtrlLgtGate* gate) {
    u32 count1 = gate->mCount1;
    if (count1 != 0) {
        const CScnEnvLgtCtrlLgtIdPair* ids =
            (const CScnEnvLgtCtrlLgtIdPair*)((const u16*)self->alt6.field_0x3C +
                                             gate->mOff64);
        for (u32 i = 0; i < count1; i++, ids++) {
            // sel picks the control sub-table the row index points into.
            CScnEnvLgtCtrlCtorCtl* ctl =
                (CScnEnvLgtCtrlCtorCtl*)self->field_0x30;
            CScnEnvLgtCtrlLgtRankRow* row;
            if (ids->sel != 0) {
                row = (CScnEnvLgtCtrlLgtRankRow*)(self->field_0x2C + ctl->mBaseD +
                                                  ids->idx * 0x64);
            } else {
                row = (CScnEnvLgtCtrlLgtRankRow*)(self->field_0x2C + ctl->mBaseC +
                                                  ids->idx * 0x50);
            }
            u32 flags = row->field_0x04;
            // Row already driven through one of the two item tables: skip.
            if (flags & 0x4000) {
                CScnEnvLgtCtrlLgtItem20* item =
                    &((CScnEnvLgtCtrlLgtItem20*)self->field_0x0C)[row->field_0x20];
                if (item->field_0x10 & 0x80000000) {
                    continue;
                }
            }
            if (flags & 0x8000) {
                CScnEnvLgtCtrlLgtEntry40* e =
                    &self->field_0x20_ptr[row->field_0x1E];
                if (e->field_0x3C & 0x80000000) {
                    continue;
                }
            }
            // Accept when the mask hits or the top nibble matches the
            // reset selector.
            if (!(flags & self->dcw.field_0xDC_w)) {
                if ((flags >> 28) != self->dcw.field_0xE0) {
                    continue;
                }
            }
            // Distance test against the ambient position; the aggregate
            // local keeps MWCC staging dx/dy/dz through stack temps like
            // retail (psq_st then lfs reloads).
            // Distance test against the ambient position; the aggregate
            // local keeps MWCC staging dx/dy/dz through stack temps like
            // retail (psq_st then lfs reloads).
            ml::CVec3 diff;
            f32 radius = row->field_0x3C;
            diff.x = self->mAmbX - row->px;
            diff.y = self->mAmbY - row->py;
            f32 reach = self->field_0xEC + radius;
            diff.z = self->mAmbZ - row->pz;
            f32 distSq = diff.x * diff.x + diff.y * diff.y +
                         diff.z * diff.z;
            if (!(reach * reach >= distSq)) {
                continue;
            }
            // Weight: constant inside the shared bound, else the distance.
            f32 weight;
            if (row->field_0x3C < lbl_eu_8066B050) {
                weight = distSq;
            } else {
                weight = lbl_eu_8066B04C;
            }
            u32 cap = self->alt5.field_0xAC;
            u32 cnt = self->field_0xE4;
            u32 slot;
            bool found = false;
            if (cnt < cap) {
                slot = cnt;
                self->field_0xE4 = cnt + 1;
            } else {
                // Full table: scan the recorded weights for the best
                // replacement slot (lowest weight at/after the saved start).
                for (u32 k = self->field_0xE8_count; k < cap; k++) {
                    f32 p = self->field_0x130[k];
                    if (p >= weight) {
                        if (found && self->field_0x130[slot] > p) {
                            continue;
                        }
                        found = true;
                        slot = k;
                    }
                }
                if (!found) {
                    continue;
                }
            }
            self->field_0x150[slot] = ids->idx;
            self->field_0x110[slot] = distSq;
            self->field_0x130[slot] = weight;
            self->field_0x160[slot] = (u8)ids->sel;
        }
    }
    u32 count2 = gate->mCount2;
    if (count2 == 0) {
        return;
    }
    // The recursive call may alias *gate, so the loop bounds are re-read
    // from gate on every iteration (retail reloads 0x1c/0x14(r31)).
    const u16* ids = (const u16*)self->alt6.field_0x3C + gate->mIdsOff;
    for (u32 i = 0; i < count2; i++, ids++) {
        const CScnEnvLgtCtrlRow20* row =
            (const CScnEnvLgtCtrlRow20*)((const u8*)self->ctor38.field_0x38 +
                                         (*ids) * 0x20);
        ml::CVec3 diff;
        f32 radius = row->w;
        diff.x = self->mAmbX - row->x;
        diff.y = self->mAmbY - row->y;
        f32 reach = self->field_0xEC + radius;
        diff.z = self->mAmbZ - row->z;
        f32 distSq = diff.x * diff.x + diff.y * diff.y +
                     diff.z * diff.z;
        if (!(reach * reach >= distSq)) {
            continue;
        }
        func_804C678C(self, (CScnEnvLgtCtrlLgtGate*)row);
    }
}

// func_804C6A70 (us-804cabcc): 'STGL' resource factory. Validates the magic
// word at data[0], allocates a CScnEnvLgtCtrl (0x168 bytes) on `handle` and
// runs the constructor over the resource header.
// Note: the -O4,p scheduler hoists the magic-check chain into the prologue
// store shadow (retail keeps it after the arg saves); 6 structural + 2
// reg_swap remain in the 8-instruction prologue window (schedule artifact).
// noinline keeps the retail out-of-line `bl func_804C6A70` from
// func_804C1BA0 (the factory body would otherwise be inlined there).
// extern "C" keeps the flat retail name on the call-site reloc.
extern "C" __declspec(noinline) CScnEnvLgtCtrl* func_804C6A70(u32 handle, const u32* data, void* arg) {
    CScnEnvLgtCtrl* obj;
    if (data[0] == 0x5354474C) {   // 'STGL'
        obj = (CScnEnvLgtCtrl*)mtl::MemManager::allocate(0x168, handle);
        if (obj != 0) {
            obj = __ct__CScnEnvLgtCtrl(obj, data, arg);
        }
    } else {
        obj = 0;
    }
    return obj;
}

// retail: lwz r0,0x4(r3); extrwi r3,r0,1,2 = (x>>29)&1
extern "C" u32 func_804C6ADC(void* self) { return (*(u32*)((char*)self + 4) >> 29) & 1; }

extern "C" int func_804C6AE8(unsigned int* arg0) {
    return (arg0[1] >> 28) & 1;
}

extern "C" void func_804C6AF4(void* ptr, int flag) {
    unsigned int* word = (unsigned int*)((char*)ptr + 4);
    if (flag != 0)
        *word |= 0x40000000;
    else
        *word &= ~0x40000000;
}

extern "C" void func_804C6B1C(void* r3, int r4) {
    if (r4 != 0) {
        *(unsigned int*)((char*)r3 + 4) |= 0x80000000u;
    } else {
        *(unsigned int*)((char*)r3 + 4) &= ~0x80000000u;
    }
}

// retail: flags(0x4): return 1 iff bit 0x80000000 set and bit 0x40000000 clear
extern "C" u32 func_804C6B44(void* self) {
    u32 flags = *(u32*)((u8*)self + 4);
    u32 ret = 0;
    if ((flags & 0x80000000) && !(flags & 0x40000000))
        ret = 1;
    return ret;
}


extern "C" float func_804C6B6C(void* self) {
    void* obj = *(void**)((u8*)self + 0x4c);
    if (obj != 0)
        return *(float*)((u8*)obj + 8);
    return lbl_eu_8066B010;
}

extern "C" float func_804C6B88(void* self) { return *(float*)((u8*)self + 0xc8); }

extern "C" void func_804C6B90(void* self, float val) { *(float*)((u8*)self + 0xc8) = val; }

extern "C" float func_804C6B98(void* self) { return *(float*)((u8*)self + 0xcc); }

extern "C" void func_804C6BA0(void* self, float val) { *(float*)((u8*)self + 0xcc) = val; }

// func_804C6BA8 (us-804cad04): build a compact light header from a source
// blob: copy the u16 words, copy five float rows of dst->field_0x12
// elements from src->field_0x28, then for each of the two light entries
// selected by src->field_0x00 bits 0/1 push the pointer pair (src +0x04 ->
// dst +0x18, base + src +0x0C -> dst +0x20) and, when source bits 3/4 are
// set, record the running base+0x58 offset into dst +0x28/+0x2C (advancing
// it by count * (dst->field_0x18[i] + 1) * 4). Source bit 2 sets dst flag
// 0x8.
extern "C" void func_804C6BA8(CScnEnvLgtCtrlLgtHeader* dst,
                              const CScnEnvLgtCtrlLgtSrc* src, u32 base) {
    dst->field_0x00 = (u16)src->field_0x00;
    dst->field_0x02 = src->field_0x14;
    dst->field_0x04 = src->field_0x16;
    dst->field_0x06 = src->field_0x18;
    dst->field_0x08 = src->field_0x1A;
    dst->field_0x0A = src->field_0x1C;
    dst->field_0x0C = src->field_0x1E;
    dst->field_0x0E = src->field_0x20;
    dst->field_0x10 = src->field_0x22;
    dst->field_0x12 = src->field_0x24;
    dst->field_0x14 = 0;
    dst->field_0x16 = src->field_0x26;
    for (int r = 0; r < 5; r++) {
        for (int i = 0; i < dst->field_0x12; i++) {
            dst->field_0x38[r][i] = src->field_0x28[i];
        }
    }
    u32 off = base + src->field_0x58;
    for (int i = 0; i < 2; i++) {
        if (src->field_0x00 & (1u << i)) {
            dst->field_0x14 |= (u16)(2u << i);
            dst->field_0x18[i] = src->field_0x04[i];
            dst->field_0x20[i] = base + src->field_0x0C[i];
            if (src->field_0x00 & 0x8) {
                dst->field_0x14 |= 0x10;
                dst->field_0x28[i].off28 = off;
                off += dst->field_0x12 * (dst->field_0x18[i] + 1) * 4;
            }
            if (src->field_0x00 & 0x10) {
                dst->field_0x14 |= 0x20;
                dst->field_0x28[i].off2C = off;
                off += dst->field_0x12 * (dst->field_0x18[i] + 1) * 4;
            }
        }
    }
    if (src->field_0x00 & 0x4) {
        dst->field_0x14 |= 0x8;
    }
}

// func_804C6D64 (us-804caec0): fill a light target's config from the
// 0xd8-byte entry. Bit 0 of the +0x14 control flags rejects the update; the
// +0x04 switch picks the 4-word config at +0x5C..0x68, +0x58 gets the
// rodata table word for +0x06, and func_80498DC0 toggles the +0x54 flag
// from +0x08. When bits 1-2 of +0x14 are set, blend the selected source row
// (entry + row*0x20 + 0x38, count = +0x12) into the dst row at +0xB8 with
// the constant weight; a dst[3] above the .sdata2 sentinel still returns 0.
// Finally copy dst[0..3] and dst[5..7] as u32 bit patterns to +0x38/+0x48.
extern "C" __declspec(noinline) int func_804C6D64(u8* entry,
                                                    CScnEnvLgtCtrlLgtTarget* out,
                                                    int row) {
    CScnEnvLgtCtrlLgtTypeEntry* e = (CScnEnvLgtCtrlLgtTypeEntry*)entry;
    f32 s, d;
    f32 v0, v1, v2, v3;
    if (e->field_0x14 & 1) return 0;
    switch (e->field_0x04w) {
    case 0:
        out->field_0x5C = 1;
        out->field_0x60 = 4;
        out->field_0x64 = 5;
        out->field_0x68 = 0;
        break;
    case 1:
        out->field_0x5C = 1;
        out->field_0x60 = 4;
        out->field_0x64 = 1;
        out->field_0x68 = 0;
        break;
    case 2:
        out->field_0x5C = 3;
        out->field_0x60 = 4;
        out->field_0x64 = 5;
        out->field_0x68 = 0;
        break;
    case 3:
        out->field_0x5C = 1;
        out->field_0x60 = 3;
        out->field_0x64 = 0;
        out->field_0x68 = 0;
        break;
    }
    out->field_0x58 = lbl_eu_805244C0[e->field_0x06w];
    func_80498DC0((u8*)out, e->field_0x08w != 0);
    f32* dst = e->field_0xB8_row;
    if (e->field_0x14 & 0x6) {
        f32* src;
        f32 f1 = lbl_eu_8066B058;
        src = (f32*)((u8*)e + row * 0x20);
        int i = 0;
        while (i < e->field_0x12w) {
            // Load the source value first, then the current row value
            // (retail keeps both live in separate registers across the
            // blend).
            f32 s = src[i + 0x0E];
            f32 d = dst[i];
            dst[i] = d + f1 * (s - d);
            i++;
        }
        if (dst[3] <= lbl_eu_8066B05C) {
            return 0;
        }
    }
    v3 = dst[3];
    v2 = dst[2];
    v1 = dst[1];
    v0 = dst[0];
    union { f32 f; u32 u; } cu3, cu2, cu1, cu0;
    cu0.f = v0;
    cu1.f = v1;
    cu2.f = v2;
    cu3.f = v3;
    out->field_0x38 = cu0.u;
    out->field_0x3C = cu1.u;
    out->field_0x40 = cu2.u;
    out->field_0x44 = cu3.u;
    f32 w0, w1, w2;
    w2 = dst[6];
    w1 = dst[5];
    w0 = dst[4];
    union { f32 f; u32 u; } cw2, cw1, cw0;
    cw0.f = w0;
    cw1.f = w1;
    cw2.f = w2;
    out->field_0x48 = cw0.u;
    out->field_0x4C = cw1.u;
    out->field_0x50 = cw2.u;
    return 1;
}

// func_804C6F78 (us-804cb0d4): sibling of func_804C6D64 writing the target's
// front half. Bit 0 of +0x14 rejects; the +0x04 switch picks the +0x34..0x40
// config; +0x14/+0x28/+0x44 come from the two rodata tables (+0x0A/+0x0C via
// C0, +0x10 via D0) and +0x20 from +0x0E. The same row blend runs on bits
// 1-2 of +0x14, then dst[0..3] go to +0x00 as u32 bit patterns, dst[5] to
// +0x24 as a float, dst[6] to +0x21 as a byte (int-cast) and, when +0x02 is
// 3, dst[7]/dst[8] to +0x11/+0x10 as bytes.
extern "C" __declspec(noinline) int func_804C6F78(u8* entry,
                                                    CScnEnvLgtCtrlLgtTarget* out,
                                                    int row) {
    CScnEnvLgtCtrlLgtTypeEntry* e = (CScnEnvLgtCtrlLgtTypeEntry*)entry;
    if (e->field_0x14 & 1) return 0;
    switch (e->field_0x04w) {
    case 0:
        out->field_0x34 = 1;
        out->field_0x38 = 4;
        out->field_0x3C = 5;
        out->field_0x40 = 0;
        break;
    case 1:
        out->field_0x34 = 1;
        out->field_0x38 = 4;
        out->field_0x3C = 1;
        out->field_0x40 = 0;
        break;
    case 2:
        out->field_0x34 = 3;
        out->field_0x38 = 4;
        out->field_0x3C = 5;
        out->field_0x40 = 0;
        break;
    case 3:
        out->field_0x34 = 1;
        out->field_0x38 = 3;
        out->field_0x3C = 0;
        out->field_0x40 = 0;
        break;
    }
    f32* dst = e->field_0xB8_row;
    out->field_0x14 = lbl_eu_805244C0[e->field_0x0Aw];
    out->field_0x28 = lbl_eu_805244C0[e->field_0x0Cw];
    out->field_0x44 = lbl_eu_805244D0[e->field_0x10w];
    out->field_0x20 = (u8)e->field_0x0Ew;
    if (e->field_0x14 & 0x6) {
        f32* src;
        f32 f1 = lbl_eu_8066B058;
        src = (f32*)((u8*)e + row * 0x20);
        int i = 0;
        while (i < e->field_0x12w) {
            // Load the source value first, then the current row value
            // (retail keeps both live in separate registers across the
            // blend).
            f32 s = src[i + 0x0E];
            f32 d = dst[i];
            dst[i] = d + f1 * (s - d);
            i++;
        }
        if (dst[3] <= lbl_eu_8066B05C) {
            return 0;
        }
    }
    f32 v0, v1, v2, v3;
    v3 = dst[3];
    v2 = dst[2];
    v1 = dst[1];
    v0 = dst[0];
    union { f32 f; u32 u; } cu3, cu2, cu1, cu0;
    cu0.f = v0;
    cu1.f = v1;
    cu2.f = v2;
    cu3.f = v3;
    out->field_0x00 = cu0.u;
    out->field_0x04 = cu1.u;
    out->field_0x08 = cu2.u;
    out->field_0x0C = cu3.u;
    out->field_0x24 = dst[4];
    out->field_0x21 = (u8)(int)dst[5];
    if (e->field_0x02 == 3) {
        out->field_0x11 = (u8)(int)dst[6];
        out->field_0x10 = (u8)(int)dst[7];
    }
    return 1;
}

// Color-row table base for (idx, layer): pointer slot at entry+0x24+idx*4
// advanced 8 bytes per layer (overlaps the layer-1 curve-list slot, as in
// retail).
static inline CScnEnvLgtCtrlFadeRows* fadeRows(CScnEnvLgtCtrlFadeView* e,
                                               int idx, int layer) {
    return ((CScnEnvLgtCtrlFadeRows**)((u8*)e + 0x24 + idx * 4))[layer];
}

// func_804C7190 (us-804cb2ec): per-slot fade evaluation over one 0xd8-byte
// light-type entry. `idx` selects the slot (0 = fade-finish check, nonzero
// = start a new fade on the layer enabled by bit 0x10 << (idx-1)); `time`
// is the curve bound (frames), `val` the global fade value. For each
// enabled layer (bit 2<<layer in mFlags) the matching curve entry is
// located by scanning start times against `time`; its float row is copied
// or blended into the layer's destination row (+0x38/+0x58), optionally
// overridden by the color-row tables at +0x24+idx*4+layer*8 when this call
// started a fade, and finally cross-blended with the mirror rows (+0x78)
// while the active bit 0x8000 is set.
extern "C" __declspec(noinline) void func_804C7190(u8* entryPtr, u32 time,
                                                   int idx, float val) {
    CScnEnvLgtCtrlFadeView* e = (CScnEnvLgtCtrlFadeView*)entryPtr;
    // Per-layer cursors mirroring the retail walk: counts/lists advance by
    // 4, destination rows by 0x20 and the color-row slot by 8 per layer.
    u8* c6;
    u8* c7;
    u8* c8;
    int changed = 0;
    if (idx != 0) {
        u16 flg = e->mFlags;
        if (flg & (0x10 << (idx - 1))) {
            e->mFlags = flg | 0x8000;
            changed = 1;
        }
    } else {
        u16 flg = e->mFlags;
        if (flg & 0x8000) {
            // Fade finished: the global value reached this slot's target.
            if (lbl_eu_8066B060[0] == val) {
                e->mFlags = flg & ~0x8000;
            }
        }
    }
    f32 scale = lbl_eu_8066B060[0];
    f64 bias = lbl_eu_8066B068;
    c6 = (u8*)e;
    c7 = (u8*)e;
    c8 = (u8*)e + idx * 4;
    for (int layer = 0; layer < 2; layer++, c6 += 4, c7 += 0x20, c8 += 8) {
        if (!(e->mFlags & (2 << layer))) continue;        CScnEnvLgtCtrlFadeList* list = *(CScnEnvLgtCtrlFadeList**)(c6 + 0x20);
        // Scan for the last curve entry whose start time has passed.
        CScnEnvLgtCtrlFadeCurve* cur = NULL;
        CScnEnvLgtCtrlFadeCurve* prev = NULL;
        int pos = 0;
        CScnEnvLgtCtrlFadeCurve* ce = list->mEntries;
        for (u32 k2 = *(u32*)(c6 + 0x18); k2 != 0; k2--) {
            if ((s32)(ce->mStart * 60) > (s32)time) break;
            prev = cur;
            cur = ce;
            pos++;
            ce++;
        }
        if (cur == NULL) {
            // No curve running yet: copy the list's default row.
            f32* dst = (f32*)(c7 + 0x38);
            for (int j = 0; j < e->mCount; j++) dst[j] = list->mDefaults[j];
            if (changed) {
                const f32* rt = *(f32**)(c8 + 0x24);
                for (int j = 0; j < e->mCount; j++) dst[j] = rt[j];
            }
        } else {
            int end60 = cur->mEnd * 60;
            int tot60 = (cur->mStart + cur->mEnd) * 60;
            if (tot60 <= (s32)time) {
                // Past the matched entry's end: copy it out verbatim.
                f32* dst = (f32*)(c7 + 0x38);
                for (int j = 0; j < e->mCount; j++) dst[j] = cur->mVals[j];
                if (changed) {
                    const f32* rt = *(f32**)(c8 + 0x24);
                    for (int j = 0; j < e->mCount; j++)
                        dst[j] = rt[pos * e->mCount + j];
                }
            } else {
                // Inside the entry: blend from the previous key toward the
                // matched one (frac = remaining time fraction).
                union { u32 w[2]; f64 d; } tn, td;
                tn.w[0] = 0x43300000u;
                td.w[0] = 0x43300000u;
                tn.w[1] = (u32)(tot60 - (s32)time) ^ 0x80000000u;
                td.w[1] = (u32)end60 ^ 0x80000000u;
                f32 frac = (f32)((tn.d - bias) / (td.d - bias));
                f32 om = scale - frac;
                f32* dst = (f32*)(c7 + 0x38);
                for (int j = 0; j < e->mCount; j++) dst[j] = om * cur->mVals[j];
                // Blend source: the previous key, or the list's default row
                // when this is the first one (both keep floats at +0x08).
                const CScnEnvLgtCtrlFadeCurve* src2 =
                    prev ? prev : (const CScnEnvLgtCtrlFadeCurve*)(*
                        (CScnEnvLgtCtrlFadeList**)(c6 + 0x20));
                for (int j = 0; j < e->mCount; j++)
                    dst[j] = frac * src2->mVals[j] + dst[j];
                if (changed) {
                    // This call started the fade: fold the color-row table
                    // row (previous row when one exists) into the result.
                    const f32* rt = *(f32**)(c8 + 0x24);
                    const f32* rCur = rt + pos * e->mCount;
                    const f32* rPrv = rt + (prev ? pos - 1 : 0) * e->mCount;
                    for (int j = 0; j < e->mCount; j++)
                        dst[j] = frac * rPrv[j] + om * rCur[j];
                }
            }
        }
        if (e->mFlags & 0x8000) {
            // Active fade: cross-blend the destination row with the mirror.
            f32 kf = scale - val;
            f32* dst = (f32*)(c7 + 0x38);
            f32* mir = (f32*)(c7 + 0x78);
            for (int j = 0; j < e->mCount; j++)
                dst[j] = val * dst[j] + kf * mir[j];
        }
    }
}

// copy previous-frame light rows (0x38/0x58) to the current frame (0x78/0x98)
// noinline keeps the retail out-of-line call sites intact.
extern "C" __declspec(noinline) void func_804C7530(CScnEnvLgtCtrl* self) {
    for (int i = 0; i < self->field_0x12; i++) {
        self->field_0x38[0x10 + i] = self->field_0x38[i];
        self->field_0x38[0x18 + i] = self->field_0x38[0x08 + i];
    }
}

extern "C" void func_804C7564(void* self, void* target, float v) {
    *(float*)((u8*)target + 0x18) = v;
    *(u32*)((u8*)self + 0x20) |= 0x8;
}

extern "C" void func_804C7578(void* self, void* target, float v) {
    *(float*)((u8*)target + 0x1C) = v;
    *(u32*)((u8*)self + 0x20) |= 0x8;
}

extern "C" void func_804C758C(void* self, void* target, float v) {
    *(float*)((u8*)target + 0x20) = v;
    *(u32*)((u8*)self + 0x20) |= 0x8;
}

extern "C" void func_804C75A0(void* self, void* target, float v) {
    *(float*)((u8*)target + 0x24) = v;
    *(u32*)((u8*)self + 0x20) |= 0x8;
}

extern "C" u32 func_804C75B4(u32 unused, void* obj, float val) { *(float*)((char*)obj + 0) = val; return unused; }

extern "C" u32 func_804C75BC(u32 unused, void* obj, float val) { *(float*)((char*)obj + 4) = val; return unused; }

extern "C" u32 func_804C75C4(u32 unused, void* obj, float val) { *(float*)((char*)obj + 8) = val; return unused; }

extern "C" void func_804C75CC(void* self, float v) {
    *(float*)((u8*)self + 0x0C) = v;
    *(u32*)((u8*)self + 0x20) |= 0x1;
}

extern "C" void func_804C75E0(void* self, float v) {
    *(float*)((u8*)self + 0x10) = v;
    *(u32*)((u8*)self + 0x20) |= 0x2;
}

extern "C" void func_804C75F4(void* self, float v) {
    *(float*)((u8*)self + 0x14) = v;
    *(u32*)((u8*)self + 0x20) |= 0x4;
}

extern "C" u32 func_804C7608(u32 unused, void* obj, float val) { *(float*)((char*)obj + 40) = val; return unused; }

extern "C" u32 func_804C7610(u32 unused, void* obj, float val) { *(float*)((char*)obj + 44) = val; return unused; }

extern "C" u32 func_804C7618(u32 unused, void* obj, float val) { *(float*)((char*)obj + 48) = val; return unused; }

extern "C" void func_804C7620(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(int*)((char*)ptr + 0x34) = val;
}

extern "C" void func_804C763C(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(int*)((char*)ptr + 0x38) = val;
}

extern "C" void func_804C7658(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0x4) = (short)val;
}

extern "C" u32 func_804C7674(u32 unused, void* obj, float val) { *(float*)((char*)obj + 184) = val; return unused; }

extern "C" u32 func_804C767C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 188) = val; return unused; }

extern "C" u32 func_804C7684(u32 unused, void* obj, float val) { *(float*)((char*)obj + 192) = val; return unused; }

extern "C" u32 func_804C768C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 196) = val; return unused; }

extern "C" u32 func_804C7694(u32 unused, void* obj, float val) { *(float*)((char*)obj + 200) = val; return unused; }

extern "C" u32 func_804C769C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 204) = val; return unused; }

extern "C" u32 func_804C76A4(u32 unused, void* obj, float val) { *(float*)((char*)obj + 208) = val; return unused; }

extern "C" void func_804C76AC(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0x6) = (short)val;
}

extern "C" void func_804C76C8(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0x8) = (short)val;
}

extern "C" void func_804C76E4(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0xa) = (short)val;
}

extern "C" void func_804C7700(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0xc) = (short)val;
}

extern "C" void func_804C771C(int, void* ptr, float value) {
    int i = (int)value;
    *(short*)((char*)ptr + 0xe) = (short)i;
}

extern "C" void func_804C7738(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0x10) = (short)val;
}

extern "C" u32 func_804C7754(u32 unused, void* obj, float val) { *(float*)((char*)obj + 200) = val; return unused; }

extern "C" u32 func_804C775C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 204) = val; return unused; }

extern "C" u32 func_804C7764(u32 unused, void* obj, float val) { *(float*)((char*)obj + 208) = val; return unused; }

extern "C" u32 func_804C776C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 212) = val; return unused; }

extern "C" __declspec(noinline) void func_804C7774(void* self, u32 a, u32 b,
                                                    float f, u32 c) {
    *(u32*)((u8*)self + 0x0) = a;
    *(u32*)((u8*)self + 0x4) = b;
    *(u32*)((u8*)self + 0x8) = b + 0x18;
    *(float*)((u8*)self + 0x1c) = f;
    *(u32*)((u8*)self + 0x18) = c;
}

// func_804C7790 (us-804cb8ec): sample self->field_0x1C against a 16-byte
// keyframe table (time + 3 values per entry, base at CScnEnvLgtCtrl+0x00 +
// grad->field_0x10): below the min bound returns the table's first value,
// above the max bound returns the last entry's first value, otherwise
// cubic-Hermite-interpolates between the flanking entries (prev.b/prev.a
// weighted by h10/h00, cur.a/cur.c by h01/h11) using the shared .sdata2
// coefficients; a sentinel float when no entry applies.
extern "C" float func_804C7790(CScnEnvLgtCtrl* self, const CScnEnvLgtCtrlGrad* grad) {
    f32* arr = (f32*)((u8*)self->mVtable + grad->field_0x10);
    if (self->field_0x1C <= grad->field_0x00) {
        return arr[1];
    }
    if (self->field_0x1C >= grad->field_0x04) {
        return ((CScnEnvLgtCtrlGrad16Entry*)arr)[grad->field_0x0A - 1].a;
    }
    // byte-offset form: keeps the array-base register distinct from the
    // walking cursor register (retail r5 vs r4)
    CScnEnvLgtCtrlGrad16Entry* p = (CScnEnvLgtCtrlGrad16Entry*)((u8*)arr + 0x10);
    for (int i = 1; i < grad->field_0x0A; i++) {
        // sample compared against the current entry's time (retail loads the
        // sample into f2 first)
        if (self->field_0x1C < p[0].time) {
            // denominator first: retail evaluates cur.time - prev.time
            // (f1) before the numerator, leaving the sample in f2
            f32 denom = p[0].time - p[-1].time;
            f32 t = (self->field_0x1C - p[-1].time) / denom;
            f32 t2 = t * t;
            f32 t3 = t2 * t;
            f32 k = lbl_eu_8066B074 * t2;
            f32 h10 = t + (t3 - lbl_eu_8066B070 * t2);
            f32 h00 = lbl_eu_8066B060[0] + (lbl_eu_8066B070 * t3 - k);
            f32 h01 = lbl_eu_8066B078 * t3 + k;
            f32 h11 = t3 - t2;
            return p[-1].b * h10 + p[-1].a * h00 + p[0].a * h01 + p[0].c * h11;
        }
        p += 1;
    }
    return lbl_eu_8066B05C;
}

// func_804C7880 (us-804cb9dc): sample self->field_0x1C against the gradient
// table: color of the first entry below the min bound, of the last entry
// above the max bound, otherwise of the highest entry whose time is <= the
// sample; a sentinel float when no entry applies.
extern "C" float func_804C7880(CScnEnvLgtCtrl* self, const CScnEnvLgtCtrlGrad* grad) {
    // Entry array base is the object's vtable pointer plus the gradient's
    // byte offset (retail reads self+0x00, not a data member).
    const CScnEnvLgtCtrlGradEntry* arr =
        (const CScnEnvLgtCtrlGradEntry*)((u8*)self->mVtable + grad->field_0x10);
    if (self->field_0x1C <= grad->field_0x00) {
        return arr[0].color;
    }
    if (self->field_0x1C >= grad->field_0x04) {
        // Byte-scaled index ((count-1)*8) so MWCC emits slwi 3 like retail.
        return arr[grad->field_0x0A - 1].color;
    }
    const CScnEnvLgtCtrlGradEntry* p = &arr[1];
    int i = 1;
    for (; i < grad->field_0x0A; i++, p++) {
        if (self->field_0x1C < p->time) {
            return p[-1].color;
        }
    }
    return lbl_eu_8066B05C;
}
// func_804C7910 (us-804cba6c): push the four light-control sub-arrays
// (strides 0x30/0x3C/0x50/0x64, counts/bases in `data`) into the 0x40-byte
// destination rows selected by each item's +0x1E index. Items flagged with
// bit 0x10000 at +0x04 are dispatched: loops 1/2/4 through func_804C7B54
// (loop 2 pre-copies its +0x30 vec3 into dst+0x0C), loop 3 writes its fields
// directly before the dispatch.
extern "C" __declspec(noinline) void func_804C7910(CScnEnvLgtCtrlWorkBlob* blob,
                                                   const CScnEnvLgtCtrlCtorCtl* data,
                                                   u8* dst) {
    // Retail stages loop-2/loop-4 vec3s through the blob's float slots at
    // +0x0C/+0x10/+0x14 before copying them into the destination rows.
    CScnEnvLgtCtrlWorkBlobF* wf = (CScnEnvLgtCtrlWorkBlobF*)blob;
    CScnEnvLgtCtrlWorkItem30* pA =
        (CScnEnvLgtCtrlWorkItem30*)((u8*)blob->field_0x00 + data->mBaseA);
    for (u32 i = 0; i < data->mCountA; i++, pA++) {
        if (pA->field_0x04 & 0x8000) {
            func_804C7B54(blob, (CScnEnvLgtCtrlWorkDst40*)(dst + pA->field_0x1E * 0x40),
                          (const CScnEnvLgtCtrlWorkItemHead*)pA,
                          lbl_eu_8066B060[0]);
        }
    }
    CScnEnvLgtCtrlWorkItem3C* pB =
        (CScnEnvLgtCtrlWorkItem3C*)((u8*)blob->field_0x00 + data->mBaseB);
    for (u32 i = 0; i < data->mCountB; i++, pB++) {
        if (pB->field_0x04 & 0x8000) {
            CScnEnvLgtCtrlWorkDst40* d =
                (CScnEnvLgtCtrlWorkDst40*)(dst + pB->field_0x1E * 0x40);
            // Hoist the vec3 loads (retail order 0x38/0x34/0x30), stage them
            // through the blob's float slots, then copy word-wise into d.
            f32 vz = pB->field_0x38;
            f32 vy = pB->field_0x34;
            f32 vx = pB->field_0x30;
            wf->field_0x10 = vy;
            wf->field_0x0C = vx;
            d->field_0x10 = f32bits_ref(wf->field_0x10);
            wf->field_0x14 = vz;
            d->field_0x0C = f32bits_ref(wf->field_0x0C);
            d->field_0x14 = f32bits_ref(wf->field_0x14);
            func_804C7B54(blob, d, (const CScnEnvLgtCtrlWorkItemHead*)pB,
                          lbl_eu_8066B060[0]);
        }
    }
    CScnEnvLgtCtrlWorkItem50* pC =
        (CScnEnvLgtCtrlWorkItem50*)((u8*)blob->field_0x00 + data->mBaseC);
    for (u32 i = 0; i < data->mCountC; i++, pC++) {
        if (pC->field_0x04 & 0x8000) {
            CScnEnvLgtCtrlWorkDst40* d =
                (CScnEnvLgtCtrlWorkDst40*)(dst + pC->field_0x1E * 0x40);
            u32 w30 = pC->field_0x30;
            u32 w34 = pC->field_0x34;
            d->field_0x00 = w30;
            d->field_0x04 = w34;
            d->field_0x08 = pC->field_0x38;
            d->field_0x28 = pC->field_0x48;
            d->field_0x2C = pC->field_0x4C;
            d->field_0x34 = pC->field_0x44;
            func_804C7B54(blob, d, (const CScnEnvLgtCtrlWorkItemHead*)pC,
                          lbl_eu_8066B060[0]);
        }
    }
    // Loop 4 declares its cursor before the counter: retail allocates the
    // counter to r30 and the item pointer to r31 here (mirrored vs loops 1-3).
    CScnEnvLgtCtrlWorkItem64* pD =
        (CScnEnvLgtCtrlWorkItem64*)((u8*)blob->field_0x00 + data->mBaseD);
    u32 j = 0;
    while (j < data->mCountD) {
        if (pD->field_0x04 & 0x8000) {
            f32 vz = pD->field_0x4C;
            f32 vx = pD->field_0x44;
            f32 vy = pD->field_0x48;
            CScnEnvLgtCtrlWorkDst40* d =
                (CScnEnvLgtCtrlWorkDst40*)(dst + pD->field_0x1E * 0x40);
            wf->field_0x0C = vx;
            wf->field_0x10 = vy;
            wf->field_0x14 = vz;
            d->field_0x00 = pD->field_0x30;
            d->field_0x04 = pD->field_0x34;
            d->field_0x08 = pD->field_0x38;
            d->field_0x10 = f32bits_ref(wf->field_0x10);
            d->field_0x0C = f32bits_ref(wf->field_0x0C);
            d->field_0x14 = f32bits_ref(wf->field_0x14);
            d->field_0x28 = pD->field_0x54;
            d->field_0x2C = pD->field_0x58;
            d->field_0x30 = pD->field_0x60;
            d->field_0x34 = pD->field_0x50;
            d->field_0x38 = pD->field_0x5C;
            func_804C7B54(blob, d, (const CScnEnvLgtCtrlWorkItemHead*)pD,
                          lbl_eu_8066B07C);
        }
        pD++;
        j++;
    }
}

// func_804C7B54 (us-804cbcb0): single work-entry refresh dispatched by
// func_804C7910. Copies the item's position/scale front half into the
// destination row (word-wise), dispatches the item's member-function handlers
// through the two 12-byte ptmf tables selected by its u16 words at +0x1C
// (table 1, receives the scan cursor) and +0x08 (table 2, receives the
// destination row), then - when the handlers raised status bits 0-2 of the
// blob's +0x20 word - rebuilds the destination direction from the XYZ Euler
// triplet in dst+0x0C/+0x10/+0x14 via rotation matrices and normalization.
// Finally, when the item's flag word has bit 0x8000, the fade weight row is
// consulted: its low bit marks the destination dirty, otherwise it scales
// dst+0x24; the remaining block rescales the stored position vec3 by that
// weight (clamped to 1.0 when item bit 0x8 is set).
// Direct .sdata2 references (MWCC reloads these around the Sin/Cos calls
// like retail instead of caching them in nonvolatile FPRs).
#define kZero lbl_eu_8066B05C
#define kOne lbl_eu_8066B060[0]
extern "C" __declspec(noinline) void func_804C7B54(
    CScnEnvLgtCtrlWorkBlob* blob, CScnEnvLgtCtrlWorkDst40* dst,
    const CScnEnvLgtCtrlWorkItemHead* item, float f) {
    // Word-copy the item's vec3 (+0x08..0x10) and weight (+0x14).
    dst->field_0x18 = item->field_0x08;
    dst->field_0x1C = item->field_0x0C;
    dst->field_0x20 = item->field_0x10;
    dst->field_0x24 = item->field_0x14;

    const CScnEnvLgtCtrlWorkTable* tbl =
        (const CScnEnvLgtCtrlWorkTable*)((u8*)blob->field_0x08 +
                                         item->field_0x1C * 0xC);
    int count = tbl->count;
    const CScnEnvLgtCtrlLgtTypeItem* cur =
        (const CScnEnvLgtCtrlLgtTypeItem*)(blob->field_0x00 + tbl->offset);
    blob->field_0x20 = 0;
    for (int i = 0; i < count; i++, cur++) {
        (blob->*lbl_eu_8065FBD0[cur->field_0x0C])((void*)cur);
        (blob->*((CScnEnvLgtCtrlWorkBlobFn*)lbl_eu_8065FA40)[cur->field_0x08])(dst);
    }

    if (blob->field_0x20 & 7) {
    int applied = 0;
    nw4r::math::MTX34 rot;
    nw4r::math::MTX34 mtx;

    // X-axis rotation from dst+0x0C.
    f32 ang = u32tof32b(dst->field_0x0C);
    if (ang != kZero) {
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066B080 * ang);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066B080 * ang);
        mtx._00 = kOne;
        mtx._01 = kZero;
        mtx._02 = kZero;
        mtx._03 = kZero;
        mtx._10 = kZero;
        mtx._11 = c;
        mtx._12 = -s;
        mtx._13 = kZero;
        mtx._20 = kZero;
        mtx._21 = s;
        mtx._22 = c;
        mtx._23 = kZero;
        applied = 1;
    }
    // Y-axis rotation from dst+0x10; concatenates onto any earlier rotation.
    ang = u32tof32b(dst->field_0x10);
    if (ang != kZero) {
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066B080 * ang);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066B080 * ang);
        if (applied != 0) {
            rot._00 = c;
            rot._01 = kZero;
            rot._02 = s;
            rot._03 = kZero;
            rot._10 = kZero;
            rot._11 = kOne;
            rot._12 = kZero;
            rot._13 = kZero;
            rot._20 = -s;
            rot._21 = kZero;
            rot._22 = c;
            rot._23 = kZero;
            PSMTXConcat(rot, mtx, mtx);
        } else {
            mtx._00 = c;
            mtx._01 = kZero;
            mtx._02 = s;
            mtx._03 = kZero;
            mtx._10 = kZero;
            mtx._11 = kOne;
            mtx._12 = kZero;
            mtx._13 = kZero;
            mtx._20 = -s;
            mtx._21 = kZero;
            mtx._22 = c;
            mtx._23 = kZero;
            applied = 1;
        }
    }
    // Z-axis rotation from dst+0x14.
    ang = u32tof32b(dst->field_0x14);
    if (ang != kZero) {
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066B080 * ang);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066B080 * ang);
        if (applied != 0) {
            rot._00 = c;
            rot._01 = -s;
            rot._02 = kZero;
            rot._03 = kZero;
            rot._10 = s;
            rot._11 = c;
            rot._12 = kZero;
            rot._13 = kZero;
            rot._20 = kZero;
            rot._21 = kZero;
            rot._22 = kOne;
            rot._23 = kZero;
            PSMTXConcat(rot, mtx, mtx);
        } else {
            mtx._00 = c;
            mtx._01 = -s;
            mtx._02 = kZero;
            mtx._03 = kZero;
            mtx._10 = s;
            mtx._11 = c;
            mtx._12 = kZero;
            mtx._13 = kZero;
            mtx._20 = kZero;
            mtx._21 = kZero;
            mtx._22 = kOne;
            mtx._23 = kZero;
            applied = 1;
        }
    }
    if (applied == 0) {
        PSMTXIdentity(mtx);
    }

    // Rotate the unit Z axis (the float argument) by the composed matrix and
    // normalize the result (falling back to the shared zero vector).
    nw4r::math::VEC3 vec;
    vec.x = kZero;
    vec.y = kZero;
    vec.z = f;
    nw4r::math::VEC3TransformNormal(&vec, &mtx, &vec);
    f32 lenSq = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
    if (lenSq == kZero) {
        vec = *(nw4r::math::VEC3*)&ml::CVec3::zero;
    } else {
        PSVECNormalize((Vec*)&vec, (Vec*)&vec);
    }
    dst->field_0x0C = *(u32*)&vec.x;
    dst->field_0x10 = *(u32*)&vec.y;
    dst->field_0x14 = *(u32*)&vec.z;
    }

    // Fade-weight row update (item flag bit 0x8000).
    if (item->field_0x04 & 0x8000) {
        const CScnEnvLgtCtrlScanRow* row =
            (const CScnEnvLgtCtrlScanRow*)((u8*)blob->field_0x18 +
                                           item->field_0x20 * 0x14);
        if (row->weight & 1) {
            dst->field_0x3C |= 1;
            return;
        }
        dst->field_0x24 *= row->f;
    }

    // Rescale the stored position vec3 by the accumulated weight; when all
    // three components are zero the destination is marked done instead.
    f32 w = dst->field_0x24;
    if (w == kZero) {
        dst->field_0x3C |= 1;
        return;
    }
    if (dst->field_0x18 == 0 && dst->field_0x1C == 0 && dst->field_0x20 == 0) {
        dst->field_0x3C |= 1;
        return;
    }
    struct V { f32 x, y, z; } v;
    v.z = u32tof32b(dst->field_0x20) * w;
    v.y = u32tof32b(dst->field_0x1C) * w;
    v.x = u32tof32b(dst->field_0x18) * w;
    if (item->field_0x04 & 0x8) {
        // Clamp each component to the shared 1.0 ceiling.
        if (v.x > kOne) v.x = kOne;
        if (v.y > kOne) v.y = kOne;
        if (v.z > kOne) v.z = kOne;
    }
    dst->field_0x18 = f32bits_ref(v.x);
    dst->field_0x1C = f32bits_ref(v.y);
    dst->field_0x20 = f32bits_ref(v.z);
}
#undef kZero
#undef kOne

// func_804C8054 (us-804cc1b0): dispatch a light entry's event handlers.
// Guarded by the +0x14 bit 0x8 flag (bit 0x8000 cleared afterwards): the
// +0x16 index selects a 12-byte table (from the work blob's +0x08 base)
// whose u16 count and u32 offset locate the 0x14-byte item rows (offset
// from the blob's +0x00 base). Every item dispatches two member-function
// pointers (tables lbl_eu_8065FBD0 / lbl_eu_8065FAF8, indexed by the item's
// +0x0C / +0x08 u16s) with the blob as `this`. Finally the +0xB8..+0xC4
// float quartet decides whether to set the +0x14 bit 0.
extern "C" __declspec(noinline) void func_804C8054(CScnEnvLgtCtrlWorkBlob* blob, u8* entry) {
    CScnEnvLgtCtrlLgtTypeEntry* e = (CScnEnvLgtCtrlLgtTypeEntry*)entry;
    if (e->field_0x14 & 0x8) {
        // Fresh read-modify-write: retail reloads +0x14 and clears bit 0
        // (rlwinm 16,30), keeping it independent from the gate test above.
        e->field_0x14 = e->field_0x14 & ~1;
        CScnEnvLgtCtrlWorkTable* tbl = (CScnEnvLgtCtrlWorkTable*)(
            (u8*)blob->field_0x08 + e->field_0x16 * 0xC);
        u32 off = tbl->offset;
        u16 count = tbl->count;
        CScnEnvLgtCtrlLgtTypeItem* item =
            (CScnEnvLgtCtrlLgtTypeItem*)(blob->field_0x00 + off);
        for (u16 i = 0; i < count; i++) {
            (blob->*lbl_eu_8065FBD0[item->field_0x0C])(item);
            (blob->*lbl_eu_8065FAF8[item->field_0x08])(entry);
            item++;
        }
        f32 v = lbl_eu_8066B05C;
        if ((e->field_0xB8 <= v && e->field_0xBC <= v && e->field_0xC0 <= v) ||
            e->field_0xC4 <= v) {
            e->field_0x14 |= 1;
        }
    }
}

// sinit_804C8174 (us-804cc2d0): static-initializer blob copy. The retail
// body copies the .data template lbl_eu_8056FA68 into the .bss object
// lbl_eu_8065FA40 (dest has a 4-byte gap at +0xB4 that is not copied).
// The retail emits seven unrolled lwz/stw copy waves; explicit block-local
// u32 temps reproduce MWCC's load-all/store-all codegen (GXFrameBuf idiom).
void sinit_804C8174() {
    const u8* src = (const u8*)&lbl_eu_8056FA68;
    u8* dst = (u8*)&lbl_eu_8065FA40;
    // loads: wave a [0x00..0x58) first (22 words, takes the registers)
    u32 a00 = *(u32*)(src + 0x00), a04 = *(u32*)(src + 0x04),
        a08 = *(u32*)(src + 0x08), a0C = *(u32*)(src + 0x0C),
        a10 = *(u32*)(src + 0x10), a14 = *(u32*)(src + 0x14),
        a18 = *(u32*)(src + 0x18), a1C = *(u32*)(src + 0x1C),
        a20 = *(u32*)(src + 0x20), a24 = *(u32*)(src + 0x24),
        a28 = *(u32*)(src + 0x28), a2C = *(u32*)(src + 0x2C),
        a30 = *(u32*)(src + 0x30), a34 = *(u32*)(src + 0x34),
        a38 = *(u32*)(src + 0x38), a3C = *(u32*)(src + 0x3C),
        a40 = *(u32*)(src + 0x40), a44 = *(u32*)(src + 0x44),
        a48 = *(u32*)(src + 0x48), a4C = *(u32*)(src + 0x4C),
        a50 = *(u32*)(src + 0x50), a54 = *(u32*)(src + 0x54),
        // then wave b [0x58..0x84) (11 words, spilled while a is live)
        b58 = *(u32*)(src + 0x58), b5C = *(u32*)(src + 0x5C),
        b60 = *(u32*)(src + 0x60), b64 = *(u32*)(src + 0x64),
        b68 = *(u32*)(src + 0x68), b6C = *(u32*)(src + 0x6C),
        b70 = *(u32*)(src + 0x70), b74 = *(u32*)(src + 0x74),
        b78 = *(u32*)(src + 0x78), b7C = *(u32*)(src + 0x7C),
        b80 = *(u32*)(src + 0x80);
    // stores: wave b first, then wave a
    *(u32*)(dst + 0x58) = b58;
    *(u32*)(dst + 0x5C) = b5C;
    *(u32*)(dst + 0x60) = b60;
    *(u32*)(dst + 0x64) = b64;
    *(u32*)(dst + 0x68) = b68;
    *(u32*)(dst + 0x6C) = b6C;
    *(u32*)(dst + 0x70) = b70;
    *(u32*)(dst + 0x74) = b74;
    *(u32*)(dst + 0x78) = b78;
    *(u32*)(dst + 0x7C) = b7C;
    *(u32*)(dst + 0x80) = b80;
    *(u32*)(dst + 0x00) = a00;
    *(u32*)(dst + 0x04) = a04;
    *(u32*)(dst + 0x08) = a08;
    *(u32*)(dst + 0x0C) = a0C;
    *(u32*)(dst + 0x10) = a10;
    *(u32*)(dst + 0x14) = a14;
    *(u32*)(dst + 0x18) = a18;
    *(u32*)(dst + 0x1C) = a1C;
    *(u32*)(dst + 0x20) = a20;
    *(u32*)(dst + 0x24) = a24;
    *(u32*)(dst + 0x28) = a28;
    *(u32*)(dst + 0x2C) = a2C;
    *(u32*)(dst + 0x30) = a30;
    *(u32*)(dst + 0x34) = a34;
    *(u32*)(dst + 0x38) = a38;
    *(u32*)(dst + 0x3C) = a3C;
    *(u32*)(dst + 0x40) = a40;
    *(u32*)(dst + 0x44) = a44;
    *(u32*)(dst + 0x48) = a48;
    *(u32*)(dst + 0x4C) = a4C;
    *(u32*)(dst + 0x50) = a50;
    *(u32*)(dst + 0x54) = a54;
    // loads: wave c [0x84..0xB4) + wave d [0xB8..0xDC)
    u32 c84 = *(u32*)(src + 0x84), c88 = *(u32*)(src + 0x88),
        c8C = *(u32*)(src + 0x8C), c90 = *(u32*)(src + 0x90),
        c94 = *(u32*)(src + 0x94), c98 = *(u32*)(src + 0x98),
        c9C = *(u32*)(src + 0x9C), cA0 = *(u32*)(src + 0xA0),
        cA4 = *(u32*)(src + 0xA4), cA8 = *(u32*)(src + 0xA8),
        cAC = *(u32*)(src + 0xAC), cB0 = *(u32*)(src + 0xB0),
        dB8 = *(u32*)(src + 0xB4), dBC = *(u32*)(src + 0xB8),
        dC0 = *(u32*)(src + 0xBC), dC4 = *(u32*)(src + 0xC0),
        dC8 = *(u32*)(src + 0xC4), dCC = *(u32*)(src + 0xC8),
        dD0 = *(u32*)(src + 0xCC), dD4 = *(u32*)(src + 0xD0),
        dD8 = *(u32*)(src + 0xD4),
        // then wave e [0xDC..0x118) (15 words, spilled while c+d live)
        eDC = *(u32*)(src + 0xD8), eE0 = *(u32*)(src + 0xDC),
        eE4 = *(u32*)(src + 0xE0), eE8 = *(u32*)(src + 0xE4),
        eEC = *(u32*)(src + 0xE8), eF0 = *(u32*)(src + 0xEC),
        eF4 = *(u32*)(src + 0xF0), eF8 = *(u32*)(src + 0xF4),
        eFC = *(u32*)(src + 0xF8), e100 = *(u32*)(src + 0xFC),
        e104 = *(u32*)(src + 0x100), e108 = *(u32*)(src + 0x104),
        e10C = *(u32*)(src + 0x108), e110 = *(u32*)(src + 0x10C),
        e114 = *(u32*)(src + 0x110);
    // stores: wave e first, then c, then d
    *(u32*)(dst + 0xDC) = eDC;
    *(u32*)(dst + 0xE0) = eE0;
    *(u32*)(dst + 0xE4) = eE4;
    *(u32*)(dst + 0xE8) = eE8;
    *(u32*)(dst + 0xEC) = eEC;
    *(u32*)(dst + 0xF0) = eF0;
    *(u32*)(dst + 0xF4) = eF4;
    *(u32*)(dst + 0xF8) = eF8;
    *(u32*)(dst + 0xFC) = eFC;
    *(u32*)(dst + 0x100) = e100;
    *(u32*)(dst + 0x104) = e104;
    *(u32*)(dst + 0x108) = e108;
    *(u32*)(dst + 0x10C) = e10C;
    *(u32*)(dst + 0x110) = e110;
    *(u32*)(dst + 0x114) = e114;
    *(u32*)(dst + 0x84) = c84;
    *(u32*)(dst + 0x88) = c88;
    *(u32*)(dst + 0x8C) = c8C;
    *(u32*)(dst + 0x90) = c90;
    *(u32*)(dst + 0x94) = c94;
    *(u32*)(dst + 0x98) = c98;
    *(u32*)(dst + 0x9C) = c9C;
    *(u32*)(dst + 0xA0) = cA0;
    *(u32*)(dst + 0xA4) = cA4;
    *(u32*)(dst + 0xA8) = cA8;
    *(u32*)(dst + 0xAC) = cAC;
    *(u32*)(dst + 0xB0) = cB0;
    *(u32*)(dst + 0xB8) = dB8;
    *(u32*)(dst + 0xBC) = dBC;
    *(u32*)(dst + 0xC0) = dC0;
    *(u32*)(dst + 0xC4) = dC4;
    *(u32*)(dst + 0xC8) = dC8;
    *(u32*)(dst + 0xCC) = dCC;
    *(u32*)(dst + 0xD0) = dD0;
    *(u32*)(dst + 0xD4) = dD4;
    *(u32*)(dst + 0xD8) = dD8;
    // loads: wave f [0x114..0x16C) then wave g
    u32 f118 = *(u32*)(src + 0x114), f11C = *(u32*)(src + 0x118),
        f120 = *(u32*)(src + 0x11C), f124 = *(u32*)(src + 0x120),
        f128 = *(u32*)(src + 0x124), f12C = *(u32*)(src + 0x128),
        f130 = *(u32*)(src + 0x12C), f134 = *(u32*)(src + 0x130),
        f138 = *(u32*)(src + 0x134), f13C = *(u32*)(src + 0x138),
        f140 = *(u32*)(src + 0x140), f144 = *(u32*)(src + 0x144),
        f148 = *(u32*)(src + 0x148), f14C = *(u32*)(src + 0x14C),
        f150 = *(u32*)(src + 0x150), f154 = *(u32*)(src + 0x154),
        f158 = *(u32*)(src + 0x158), f15C = *(u32*)(src + 0x15C),
        f160 = *(u32*)(src + 0x160), f164 = *(u32*)(src + 0x164),
        f168 = *(u32*)(src + 0x168),
        // then wave g [0x16C..0x1A8) (15 words, spilled while f is live)
        g16C = *(u32*)(src + 0x16C), g170 = *(u32*)(src + 0x170),
        g174 = *(u32*)(src + 0x174), g178 = *(u32*)(src + 0x178),
        g17C = *(u32*)(src + 0x17C), g180 = *(u32*)(src + 0x180),
        g184 = *(u32*)(src + 0x184), g188 = *(u32*)(src + 0x188),
        g18C = *(u32*)(src + 0x18C), g190 = *(u32*)(src + 0x190),
        g194 = *(u32*)(src + 0x194), g198 = *(u32*)(src + 0x198),
        g19C = *(u32*)(src + 0x19C), g1A0 = *(u32*)(src + 0x1A0),
        g1A4 = *(u32*)(src + 0x1A4);
    // stores: wave g first, then wave f
    *(u32*)(dst + 0x16C) = g16C;
    *(u32*)(dst + 0x170) = g170;
    *(u32*)(dst + 0x174) = g174;
    *(u32*)(dst + 0x178) = g178;
    *(u32*)(dst + 0x17C) = g17C;
    *(u32*)(dst + 0x180) = g180;
    *(u32*)(dst + 0x184) = g184;
    *(u32*)(dst + 0x188) = g188;
    *(u32*)(dst + 0x18C) = g18C;
    *(u32*)(dst + 0x190) = g190;
    *(u32*)(dst + 0x194) = g194;
    *(u32*)(dst + 0x198) = g198;
    *(u32*)(dst + 0x19C) = g19C;
    *(u32*)(dst + 0x1A0) = g1A0;
    *(u32*)(dst + 0x1A4) = g1A4;
    *(u32*)(dst + 0x118) = f118;
    *(u32*)(dst + 0x11C) = f11C;
    *(u32*)(dst + 0x120) = f120;
    *(u32*)(dst + 0x124) = f124;
    *(u32*)(dst + 0x128) = f128;
    *(u32*)(dst + 0x12C) = f12C;
    *(u32*)(dst + 0x130) = f130;
    *(u32*)(dst + 0x134) = f134;
    *(u32*)(dst + 0x138) = f138;
    *(u32*)(dst + 0x13C) = f13C;
    *(u32*)(dst + 0x140) = f140;
    *(u32*)(dst + 0x144) = f144;
    *(u32*)(dst + 0x148) = f148;
    *(u32*)(dst + 0x14C) = f14C;
    *(u32*)(dst + 0x150) = f150;
    *(u32*)(dst + 0x154) = f154;
    *(u32*)(dst + 0x158) = f158;
    *(u32*)(dst + 0x15C) = f15C;
    *(u32*)(dst + 0x160) = f160;
    *(u32*)(dst + 0x164) = f164;
    *(u32*)(dst + 0x168) = f168;
}


// func_804C1094 (us-804c51f0): init/destroy the blend+bloom filter pair.
// With r4 != 0 and +0x00 bit 3 clear: allocate CScnBlend (0x6C) and
// CScnBloom (0x84), construct them, resolve the +0x30 control, register the
// filters with the owner's +0x70 manager, then clear the new objects' idle
// flags. With r4 == 0 and bit 3 set: unregister both filters, release the
// blend and bloom objects (deleting-dtor dispatch through their vtable
// entries; the bloom's main vtable sits at +0x80) and clear +0x00 bit 3.
void func_804C1094(CScnEnvLgtCtrl* self, int init) {
    if (init != 0) {
        if (!(self->flags & 0x8)) {
            CScnEnvLgtBlend* blend = (CScnEnvLgtBlend*)mtl::MemManager::allocate(
                0x6C, func_80496018((CScnItemModelNw4rOwner*)self->field_0x04_ptr));
            if (blend != 0) {
                blend = __ct__CScnBlend(blend);
            }
            self->field_0x28_blend = blend;
            CScnEnvLgtBloom* bloom = (CScnEnvLgtBloom*)mtl::MemManager::allocate(
                0x84, func_80496018((CScnItemModelNw4rOwner*)self->field_0x04_ptr));
            if (bloom != 0) {
                bloom = __ct__CScnBloom(bloom);
            }
            self->field_0x2C_bloom = bloom;
            self->field_0x30 =
                func_8049699C((CScnItemModelNw4rOwner*)self->field_0x04_ptr);
            // Reload everything from self (retail keeps only r31 alive).
            func_8049D1EC(((CScnEnvLgtOwnerLgt*)self->field_0x04_ptr)->field_0x70,
                          self->field_0x28_blend);
            // Conditional rebase of a local (no explicit else) keeps
            // retail's `beq` over the addi - no materialized null store.
            CScnEnvLgtBlend* filter = (CScnEnvLgtBlend*)self->field_0x2C_bloom;
            if (filter != 0) {
                filter = (CScnEnvLgtBlend*)((u8*)filter + 0x48);
            }
            func_8049D1EC(
                ((CScnEnvLgtOwnerLgt*)self->field_0x04_ptr)->field_0x70,
                filter);
            self->flags |= 0x8;
            func_804C26F0(self);
            ((CScnEnvLgtCtrlLgtParamCtlTail*)self->field_0x30)->field_0x64 &= ~1;
            self->field_0x28_blend->mFlags &= ~2;
            self->field_0x2C_bloom->field_0x50 &= ~2;
        }
    } else {
        if (self->flags & 0x8) {
            // Reload the manager slot per call (retail register profile).
            func_8049D3D8(
                ((CScnEnvLgtOwnerLgt*)self->field_0x04_ptr)->field_0x70,
                self->field_0x28_blend);
            // Same conditional-rebase form as the init pass.
            CScnEnvLgtBlend* filter = (CScnEnvLgtBlend*)self->field_0x2C_bloom;
            if (filter != 0) {
                filter = (CScnEnvLgtBlend*)((u8*)filter + 0x48);
            }
            func_8049D3D8(
                ((CScnEnvLgtOwnerLgt*)self->field_0x04_ptr)->field_0x70,
                filter);
            if (self->field_0x28_blend != 0) {
                if (self->field_0x28_blend != 0) {
                    self->field_0x28_blend->release(1);
                }
                self->field_0x28_blend = 0;
            }
            if (self->field_0x2C_bloom != 0) {
                if (self->field_0x2C_bloom != 0) {
                    self->field_0x2C_bloom->release(1);
                }
                self->field_0x2C_bloom = 0;
            }
            self->field_0x30 = 0;
            self->flags &= ~8;
        }
    }
}
void func_804C123C(void* self, int flag) {
    u32* obj = *(u32**)((u8*)self + 0x28);
    if (obj == 0) return;
    if (flag != 0)
        obj[2] |= 2;
    else
        obj[2] &= ~2;
}
void func_804C1270(void* self, int flag) {
    u8* obj = *(u8**)((u8*)self + 0x30);
    if (obj == 0) return;
    if (flag != 0)
        obj[0x64] |= 1;
    else
        obj[0x64] &= ~1;
}
// func_804C12A4 (us-804c5400): advance every light item's frame delta
// (func_80496288 on the shared view) and dispatch vtable slot 3 (0x0C).
// Sets +0x00 bit 0x100 when any item was visited, clears it otherwise, and
// returns whether the ring was non-empty.
u32 func_804C12A4(CScnEnvLgtCtrl* self) {
    u32 ret = 0;
    CScnEnvLgtCtrlListNode* sentinel = self->field_0x0C;
    CScnEnvLgtCtrlListNode* node = sentinel->mNext;
    // Guard + do-while with a single post-loop `ret = 1`: keeping the flag
    // definition after both calls lets MWCC color ret into r3 (matching
    // retail); an in-loop `ret = 1` gets hoisted into a callee-saved reg
    // instead, rotating the whole allocation away from retail.
    if (node != sentinel) {
        do {
            func_80496288((CScnItemModelNw4rOwner*)self->field_0x04_ptr);
            node->mItem->v1();
            node = node->mNext;
        } while (node != self->field_0x0C);
        ret = 1;
    }
    if (ret != 0) {
        self->flags |= 0x100;
    } else {
        self->flags &= ~0x100;
    }
    return ret;
}
// func_804C1338 (us-804c5494): per-frame light-env dispatch. Resolves the
// camera item through the owner's +0x68 camera-work blob, re-inits the four
// CLightEnv slots (func_80495644 + func_804952C4 with the AFD8/AFDC color
// pair + func_804956F8), then walks the light-item ring forward dispatching
// v9(env) plus the v12/v13 vec4 queries (applying results through
// func_8049347C / func_804935C0 on the owner's +0x5C virtual-light manager
// when they return nonzero), and backward dispatching v10(env,
// &camItem->field_0x10C) on active items.
void func_804C1338(CScnVirtualLightSub* sub, CLightEnv* env) {
    CScnEnvLgtCtrl* self = (CScnEnvLgtCtrl*)sub;
    CScnEnvLgtCtrlCamView* camView = (CScnEnvLgtCtrlCamView*)func_8049B158(
        ((CScnEnvLgtCtrlOwnerLight*)self->field_0x04_ptr)->field_0x68,
        ((CScnEnvLgtCtrlOwnerLight*)self->field_0x04_ptr)->field_0x68->mCamId);
    ml::CVec4 v18;   // sp+0x18 ring-walk vec4 (declared first -> higher slot)
    ml::CVec4 v8;    // sp+0x8 slot color
    for (u32 i = 0; i < 4; i++) {
        func_80495644(env, i);
        v8.x = lbl_eu_8066AFD8;
        v8.y = lbl_eu_8066AFD8;
        v8.z = lbl_eu_8066AFD8;
        v8.w = lbl_eu_8066AFDC;
        func_804952C4(env, (const ml::CVec3*)&v8);
        func_804956F8(env);
    }
    CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
    while (node != self->field_0x0C) {
        if (node->mItem->v23()) {
            node->mItem->v9(env);
            if (node->mItem->v12(&v18)) {
                func_8049347C(((CScnEnvLgtCtrlOwnerLight*)self->field_0x04_ptr)
                                  ->field_0x5C,
                              &v18, lbl_eu_8066AFDC);
            }
            if (node->mItem->v13(&v18)) {
                func_804935C0(((CScnEnvLgtCtrlOwnerLight*)self->field_0x04_ptr)
                                  ->field_0x5C,
                              &v18, lbl_eu_8066AFDC);
            }
        }
        node = node->mNext;
    }
    node = self->field_0x0C->mPrev;
    while (node != self->field_0x0C) {
        if (node->mItem->v23()) {
            node->mItem->v10(env, &camView->field_0x10C);
        }
        node = node->mPrev;
    }
}
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056F9B8-0x8056FC10 (0x258): vtable + tail callback table.
// In-TU functions (extern "C" above) carry the retail reloc names.
extern "C" u32 lbl_eu_80663B08;  // .sdata RTTI locator
extern "C" u32 lbl_eu_80663B10;  // .sdata locator (defined below)
extern "C" u32 lbl_eu_8056F96C;  // .data tail object (foreign)
extern "C" u32 lbl_eu_8056FA58;  // .data tail object (foreign)
extern "C" void __dt__14CScnEnvLgtCtrlFv();  // member dtor (this TU)
// us-804c8f60: fog refresh (see body below; referenced by the vtable).
extern "C" __declspec(noinline) void func_804C4E04(CScnEnvLgtCtrl* self,
                                                    nw4r::g3d::ScnRoot* root,
                                                    u32 flag) {
    CScnEnvLgtCtrlRowsView* view = (CScnEnvLgtCtrlRowsView*)self;
    // Fog refresh: walk the 0x3C-stride fog-entry list selected by the
    // +0x40 gate, push each entry's color/start/end into its scene fog slot,
    // then (gate bit 0) fetch the two per-view fog parameter sets into mFog
    // and blend the stored color triples when `flag` is set.
    CScnEnvLgtCtrlFogGate* gate = view->gate40;
    if (gate == NULL) {
        return;
    }
    u8* entries = view->base2C + gate->mOffset;
    f32 scale = *(const f32*)((const u8*)&lbl_eu_8066B030 + gate->mOffset);
    for (u32 i = 0; i < gate->mCount; i++, entries += 0x3c) {
        nw4r::g3d::Fog fog = root->GetFog((int) * (u16*)(entries + 0x38));
        CScnEnvLgtCtrlRow1C* row = &view->rows10[*(u16*)(entries + 0x10)];
        GXColor col;
        col.r = (u8)(int)(scale * *(f32*)((u8*)row + 0x00));
        col.g = (u8)(int)(scale * *(f32*)((u8*)row + 0x04));
        col.b = (u8)(int)(scale * *(f32*)((u8*)row + 0x08));
        col.a = 0xff;
        if (*(u32*)&col == 0) {
            nw4r::db::Panic(lbl_eu_8056EC60, 0x63, lbl_eu_8056EC40);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->color = col;
        }
        f32 endz = *(f32*)((u8*)row + 0x10);
        f32 startz = row->field_0x0C;
        if (fog.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056EC30, 0x4b, lbl_eu_8056EC10);
        }
        if (fog.ptr() != NULL) {
            fog.ptr()->startz = startz;
            fog.ptr()->endz = endz;
        }
        if ((row->field_0x18 & 1) == 0) {
            GXFogType ty;
            fog.GetFog(&ty, NULL, NULL, NULL, NULL, NULL);
            *(GXFogType*)((u8*)row + 0x14) = ty;
            row->field_0x18 |= 1;
        } else {
            void* saved = *(void**)((u8*)row + 0x14);
            if (fog.ptr() == NULL) {
                nw4r::db::Panic(lbl_eu_8056EC00, 0x41, lbl_eu_8056EBE0);
            }
            if (fog.ptr() != NULL) {
                *(void**)fog.ptr() = saved;
            }
        }
    }
    // Re-fetch the gate: the loop calls may have aliased it.
    gate = view->gate40;
    if ((gate->mFlags & 1) == 0) {
        return;
    }
    nw4r::g3d::Fog fogA = root->GetFog((int) * (u16*)((u8*)gate + 2));
    fogA.GetFog((GXFogType*)&self->mFog.mType[0], &self->mFog.mStart[0],
                &self->mFog.mEnd[0], NULL, NULL,
                (GXColor*)&self->mFog.mColor[0]);
    nw4r::g3d::Fog fogB = root->GetFog(0);
    fogB.GetFog((GXFogType*)&self->mFog.mType[1], &self->mFog.mStart[1],
                &self->mFog.mEnd[1], NULL, NULL,
                (GXColor*)&self->mFog.mColor[1]);
    if (flag == 0) {
        return;
    }
    // Blend each stored color triple into a single clamped byte triple
    // (byte -> double via the 0x43300000 temp, minus the shared 2^52 const).
#define FOG_BLEND_TRIPLE(basePtr)                                              \
    {                                                                          \
        u8* c = (u8*)(basePtr);                                                \
        union { u32 w[2]; double d; } t1, t0, t2;                              \
        t1.w[0] = 0x43300000u;                                                 \
        t1.w[1] = (u32)c[1];                                                   \
        double acc = lbl_eu_8066B03C * (t1.d - lbl_eu_8066B040);               \
        t0.w[0] = 0x43300000u;                                                 \
        t0.w[1] = (u32)c[0];                                                   \
        acc = lbl_eu_8066B038 * (t0.d - lbl_eu_8066B040) + acc;                \
        t2.w[0] = 0x43300000u;                                                 \
        t2.w[1] = (u32)c[2];                                                   \
        acc = lbl_eu_8066B034 * (t2.d - lbl_eu_8066B040) + acc;                \
        int v = (int)acc;                                                      \
        if (v > 255) {                                                         \
            v = 255;                                                           \
        }                                                                      \
        c[0] = (u8)v;                                                          \
        c[1] = (u8)v;                                                          \
        c[2] = (u8)v;                                                          \
    }
    FOG_BLEND_TRIPLE(&self->mFog.mColor[0]);
    FOG_BLEND_TRIPLE(&self->mFog.mColor[1]);
#undef FOG_BLEND_TRIPLE
}
extern "C" u32 lbl_eu_80524480;
extern "C" u32 lbl_eu_805244A0;
extern "C" u32 lbl_eu_805244B0;

extern "C" u32 lbl_eu_8056F9B8[150] = {
    (u32)&lbl_eu_80663B08, 0x00000000, (u32)&__dt__14CScnEnvLgtCtrlFv, (u32)&func_804C30E8, (u32)&func_804C31C8, (u32)&func_804C6BA0, (u32)&func_804C6B98, (u32)&func_804C6B88,
    (u32)&func_804C6B90, (u32)&func_804C6B6C, (u32)&func_804C6B64, (u32)&func_804C33F0, (u32)&func_804C3404, (u32)&func_804C4E04, (u32)&func_804C5198, (u32)&func_804C51D4,
    (u32)&func_804C5210, (u32)&func_804C526C, (u32)&func_804C5380, (u32)&func_804C54D4, (u32)&func_804C5628, (u32)&func_804C58D8, (u32)&func_804C5990, (u32)&func_804C5A48,
    (u32)&func_804C5B00, (u32)&func_804C6B44, (u32)&func_804C6B1C, (u32)&func_804C6AF4, (u32)&func_804C5C08, (u32)&func_804C5C6C, (u32)&func_804C5D7C, (u32)&func_804C6AE8,
    (u32)&func_804C6ADC, (u32)&func_804C5DA0, (u32)&func_804C5E04, (u32)&func_804C5E9C, (u32)&func_804C5F6C, (u32)&func_804C5FB0, (u32)&func_804C6010, (u32)&func_804C6054,
    (u32)&lbl_eu_80663B10, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, (u32)&func_804C7564, 0x00000000,
    0xFFFFFFFF, (u32)&func_804C7578, 0x00000000, 0xFFFFFFFF, (u32)&func_804C758C, 0x00000000, 0xFFFFFFFF, (u32)&func_804C75A0,
    0x00000000, 0xFFFFFFFF, (u32)&func_804C75B4, 0x00000000, 0xFFFFFFFF, (u32)&func_804C75BC, 0x00000000, 0xFFFFFFFF,
    (u32)&func_804C75C4, 0x00000000, 0xFFFFFFFF, (u32)&func_804C75CC, 0x00000000, 0xFFFFFFFF, (u32)&func_804C75E0, 0x00000000,
    0xFFFFFFFF, (u32)&func_804C75F4, 0x00000000, 0xFFFFFFFF, (u32)&func_804C7608, 0x00000000, 0xFFFFFFFF, (u32)&func_804C7610,
    0x00000000, 0xFFFFFFFF, (u32)&func_804C7618, 0x00000000, 0xFFFFFFFF, (u32)&func_804C7620, 0x00000000, 0xFFFFFFFF,
    (u32)&func_804C763C, 0x00000000, 0xFFFFFFFF, (u32)&func_804C7658, 0x00000000, 0xFFFFFFFF, (u32)&func_804C7674, 0x00000000,
    0xFFFFFFFF, (u32)&func_804C767C, 0x00000000, 0xFFFFFFFF, (u32)&func_804C7684, 0x00000000, 0xFFFFFFFF, (u32)&func_804C768C,
    0x00000000, 0xFFFFFFFF, (u32)&func_804C7694, 0x00000000, 0xFFFFFFFF, (u32)&func_804C769C, 0x00000000, 0xFFFFFFFF,
    (u32)&func_804C76A4, 0x00000000, 0xFFFFFFFF, (u32)&func_804C76AC, 0x00000000, 0xFFFFFFFF, (u32)&func_804C76C8, 0x00000000,
    0xFFFFFFFF, (u32)&func_804C76E4, 0x00000000, 0xFFFFFFFF, (u32)&func_804C7700, 0x00000000, 0xFFFFFFFF, (u32)&func_804C771C,
    0x00000000, 0xFFFFFFFF, (u32)&func_804C7738, 0x00000000, 0xFFFFFFFF, (u32)&func_804C7754, 0x00000000, 0xFFFFFFFF,
    (u32)&func_804C775C, 0x00000000, 0xFFFFFFFF, (u32)&func_804C7764, 0x00000000, 0xFFFFFFFF, (u32)&func_804C776C, 0x00000000,
    0xFFFFFFFF, (u32)&func_804C7790, 0x00000000, 0xFFFFFFFF, (u32)&func_804C7880, 0x00000000,
};

// [.rodata] 0x80524468-0x805244E0 (0x78): RTTI name strings + raw words.
extern "C" const u32 lbl_eu_80524468[30] = {
    0x7265736C, 0x6973743C, 0x4953636E, 0x456E7643, 0x746C202A, 0x3E000000, 0x5F726573, 0x6C697374,
    0x5F626173, 0x653C4953, 0x636E456E, 0x7643746C, 0x202A3E00, 0x00000000, 0x4353636E, 0x456E764C,
    0x67744374, 0x726C0000, 0x4953636E, 0x456E7643, 0x746C0000, 0x00000000, 0x00000004, 0x00000005,
    0x00000006, 0x00000000, 0x00000004, 0x00000008, 0x00000010, 0x00000020,
};

// [.sdata] 0x80663AE8-0x80663B20 (0x38): 4 raw words + 5 RTTI locator pairs.
extern "C" u32 lbl_eu_80663AE8 = 0x00004650;
extern "C" u32 lbl_eu_80663AEC = 0x00005460;
extern "C" u32 lbl_eu_80663AF0 = 0x00010428;
extern "C" u32 lbl_eu_80663AF4 = 0x00010B30;
extern "C" u32 lbl_sd_04[2] = { (u32)&lbl_eu_80524468, (u32)&lbl_eu_8056F96C };
extern "C" u32 lbl_sd_06[2] = { (u32)&lbl_eu_80524480, 0x00000000 };
extern "C" u32 lbl_sd_08[2] = { (u32)&lbl_eu_805244A0, (u32)&lbl_eu_8056FA58 };
extern "C" u32 lbl_sd_10[2] = { (u32)&lbl_eu_805244B0, 0x00000000 };
extern "C" u32 lbl_sd_12[2] = { 0x01000000, 0x00000000 };

// [.bss] 0x8065FA40-0x8065FBE8 (0x1A8 = 424B) zero-fill, typed to match the
// .hpp externs used by the sinit / blob-dispatch code.
u8 lbl_eu_8065FA40[0xB8];
CScnEnvLgtCtrlWorkBlobFn lbl_eu_8065FAF8[0xD8 / 12];
CScnEnvLgtCtrlWorkBlobFn lbl_eu_8065FBD0[0x18 / 12];
// (retired DECOMP_FORCEACTIVE keep-alives: both dispatch tables are
// live-referenced by the blob-dispatch code above.)
