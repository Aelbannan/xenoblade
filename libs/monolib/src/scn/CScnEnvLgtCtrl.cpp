// Auto-scaffolded catalog TU for monolib/src/scn/CScnEnvLgtCtrl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
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
#include "libs/monolib/src/scn/CScnEnvLgtCtrl.hpp"
#include "libs/monolib/src/scn/CScnItemModelNw4r.hpp"  // func_8048ECD8 / func_80496288 queries
#include "monolib/math/CVec4.hpp"       // ml::CVec4 (func_804C1338 slot/walk vecs)
#include "monolib/scn/code_804BF59C.hpp"  // CScnEnvLgtData (mAmbColorBase / mFlags)
#include "monolib/util/MemManager.hpp"    // mtl::MemManager (allocate / deallocate)

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


void func_804C1500(){}

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
        union { u32 u; GXColor c; } cv;
        cv.u = lbl_eu_8066AFE0;
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
            fog.ptr()->color = cv.c;
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
    CScnEnvLgtCtrl* obj = func_804C6A70(
        func_80496018((CScnItemModelNw4rOwner*)self->field_0x04_ptr), data, arg);
    if (obj != 0) {
        CScnEnvLgtCtrlListNode* sentinel = self->field_0x0C;
        CScnEnvLgtCtrlListNode* base = self->field_0x1C_nodes;
        int count = 0;
        int off = 0;
        while (count < self->field_0x20_count &&
               *(CScnEnvLgtCtrlListNode**)((u8*)base + off) != 0) {
            off += 0xc;
            count++;
        }
        CScnEnvLgtCtrlListNode* node =
            (CScnEnvLgtCtrlListNode*)((u8*)base + count * 0xc);
        if (node != 0) {
            node->mItem = (CScnEnvLgtCtrlListItem*)obj;
        }
        node->mNext = sentinel;
        node->mPrev = sentinel->mPrev;
        sentinel->mPrev->mNext = node;
        sentinel->mPrev = node;
        self->flags |= 2;
        CScnEnvLgtCtrlListItem* item = (CScnEnvLgtCtrlListItem*)obj;
        if (item->v30()) {
            CScnEnvLgtCtrlListNode* n = sentinel->mNext;
            while (n != sentinel) {
                if (n->mItem->v29() == 0) {
                    n->mItem->v25(1);
                }
                n = n->mNext;
            }
        }
    }
    func_804C26F0(self);
    CScnEnvLgtCtrlListNode* n;
    int c;
    CScnEnvLgtCtrlListNode* sentinel = self->field_0x0C;
    n = sentinel->mNext;
    c = 0;
    while (n != sentinel) {
        n = n->mNext;
        c++;
    }
    if (c == 1) {
        n = sentinel->mNext;
        while (n != sentinel) {
            n->mItem->v37(1);
            n = n->mNext;
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
        node = self->field_0x0C->mNext;
        while (node != self->field_0x0C) {
            if (node->mItem->v30() != 0) {
                freed = false;
                break;
            }
            node = node->mNext;
        }
    }
    if (freed) {
        node = self->field_0x0C->mNext;
        while (node != self->field_0x0C) {
            node->mItem->v25(0);
            node = node->mNext;
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

void func_804C1F10(){}

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

void func_804C22F0(){}

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

extern "C" __declspec(noinline) void func_804C26F0(CScnEnvLgtCtrl* self) {}

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
    CScnEnvLgtCtrlLgtSlot* slot = self->field_0x24;
    if (slot == 0) return;
    for (int i = 0; i < 4; i++) {
        u16 flags = slot->field_0x28;
        if (flags & 1) {
            if (flags & 4) {
                int bound = self->alt7.field_0xA8;
                int v0 = (int)slot->field_0x2A * 60;
                int v1 = (int)slot->field_0x2C * 60;
                int v2 = (int)slot->field_0x2E * 60;
                f32 f = lbl_eu_8066B014;
                int changed = 0;
                if (v0 > v1) {
                    if (v0 <= bound) {
                        if (v2 != 0 && v0 + v2 > bound) {
                            f = lbl_eu_8066B014 - u32ToF_b0e8((u32)(v0 + v2 - bound)) / u32ToF_b0e8((u32)v2);
                        }
                        changed = 1;
                    } else {
                        if (v1 >= bound) {
                            if (v2 != 0 && v1 - v2 < bound) {
                                f = u32ToF_b0e8((u32)(v1 - bound)) / u32ToF_b0e8((u32)v2);
                            }
                            changed = 1;
                        }
                    }
                } else {
                    if (v0 <= bound && v1 >= bound) {
                        if (v2 != 0 && v0 + v2 > bound) {
                            f = lbl_eu_8066B014 - u32ToF_b0e8((u32)(v0 + v2 - bound)) / u32ToF_b0e8((u32)v2);
                        } else if (v2 != 0 && v1 - v2 < bound) {
                            f = u32ToF_b0e8((u32)(v1 - bound)) / u32ToF_b0e8((u32)v2);
                        }
                        changed = 1;
                    }
                }
                if (changed) {
                    slot->field_0x28 |= 8;
                    f32* v3 = (f32*)&slot->field_0x0C;
                    slot->field_0x00.x = f32tou32b(v3[0] * f);
                    slot->field_0x00.y = f32tou32b(v3[1] * f);
                    slot->field_0x00.z = f32tou32b(v3[2] * f);
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

extern "C" void func_804C34A0(void* self);

extern "C" void func_804C33F0(void* self) {
    if (*(u32*)((u8*)self + 0x30) == 0)
        return;
    func_804C34A0(self);
}

// func_804C3404 (us-804c7560): push the default light value to +0xEC (the
// +0x30 pointer is only null-checked; the value itself is the .sdata2
// constant lbl_eu_8066B010) and the 3-word color triple to +0xD0, then
// refresh the four CLightEnv slot fields: each slot field is copied to +0xE4
// and recomputed by func_804C64A8. The loop walks a byte offset (off)
// against the env base (p) so MWCC keeps the counter/offset double induction
// (add r30,r28,r31 per iteration) like retail.
extern "C" void func_804C3404(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLightEnvView* env,
                   const u32* data) {
    if (self->field_0x30 == 0) return;
    self->field_0xEC = lbl_eu_8066B010;
    self->field_0xD0 = data[0];
    self->field_0xD4 = data[1];
    self->field_0xD8 = data[2];
    u8* p = (u8*)env;
    u32* slot;   // slot address, live across the func_804C64A8 call
    u32 off = 0;
    u32 i = 0;
    do {
        slot = (u32*)(p + off + 0x1160);
        self->field_0xE4 = *slot;
        *slot = func_804C64A8(self, *(CLight**)(p + off + 0x1140), i, 0);
        off += 4;
        i++;
    } while (i < 4);
}

extern "C" void func_804C34A0(void* self) {
    func_804C34A0(self); // stub: recursion prevents MWCC from inlining the (unreconstructed) body
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
    if (self->field_0x28[ctl->field_0x00 >> 5] &
        (1u << (ctl->field_0x00 & 31))) {
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

extern "C" __declspec(noinline) void func_804C3C9C(
    CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtEnv484* env, int flag) {}

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
        out->x = ctl->field_0x08 * scale;
        out->y = ctl->field_0x0C * scale;
        out->z = ctl->field_0x10 * scale;
    }
    return 1;
}


extern "C" __declspec(noinline) void func_804C406C(CScnEnvLgtCtrl* self, float f) {}

// Work-blob builder (matched, defined below); extern "C" keeps the flat
// retail name on the call-site reloc and noinline keeps the call out-of-line
// (the retail bl must survive).
extern "C" __declspec(noinline) void func_804C7774(void* self, u32 a, u32 b,
                                                    float f, u32 c);
extern "C" __declspec(noinline) void func_804C7910(CScnEnvLgtCtrlWorkBlob* blob);
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
    f32 val = self->field_0xC8 + f;
    self->field_0xC8 = val;
    f32 limit = self->alt2.field_0x4C->field_0x08;
    if (limit < val) {
        if (flags & 0x100) {
            self->field_0xC8 = limit;
        } else {
            self->field_0xC8 = lbl_eu_8066B010;
        }
    }
    CScnEnvLgtCtrlWorkBlob blob;
    func_804C7774(&blob, (u32)self->field_0x2C, (u32)self->alt2.field_0x4C,
                  self->field_0xC8, (u32)self->field_0x0C);
    if (self->field_0x30 != 0 && self->field_0x20 != 0) {
        func_804C7910(&blob);
    }
    CScnEnvLgtCtrlLgtCtl* ctl = self->alt2.field_0x48;
    if (ctl != 0 && (ctl->field_0x00 & 0x10)) {
        for (u32 i = 0, off = 0; i < self->alt2.field_0x48->mCount;
             i++, off += 0xd8) {
            func_804C8054(&blob, (u8*)self->field_0x14_ptr + off);
        }
    }
}

void func_804C43A4(){}

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
    out5[0] *= f2;
    out6[0] *= f2;
    out4[0] *= f2;
    out4[1] *= f2;
    out4[2] *= f2;
    f32* prev = (f32*)((u8*)self->alt2.field_0x50 + r7 * 4 + (r8 - 1) * 0x14);
    out4[0] += prev[0] * f1;
    out4[1] += prev[1] * f1;
    out4[2] += prev[2] * f1;
    out5[0] += prev[3] * f1;
    out6[0] += prev[4] * f1;
}

void func_804C4954(){}

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
void* __dt__reslist_IScnEnvCtl(CScnEnvLgtCtrlIScnResBase* self, int deleting) {
    CScnEnvLgtCtrlIScnResNode* node;
    CScnEnvLgtCtrlIScnResNode* cur;
    if (self != 0) {
        if (self != 0) {
            self->mVtable = (void*)lbl_eu_8056F978;
            node = self->mStartNodePtr->mNext;
            while (node != self->mStartNodePtr) {
                cur = node;
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
        u32 v7 = src->field_0x00;   // reads 0x54
        u32 v6 = src->field_0x04;   // reads 0x58
        u32 v5 = src->field_0x08;   // reads 0x5c
        u32 v0 = src->field_0x0C;   // reads 0x60
        out->field_0x00 = v7;
        out->field_0x04 = v6;
        out->field_0x08 = v5;
        out->field_0x0C = v0;
        return true;
    }
    return false;
}

extern "C" bool func_804C51D4(void* r3, void* r4) {
    if (!(*(unsigned int*)((char*)r3 + 4) & 0x400)) return false;
    unsigned int v7 = *(unsigned int*)((char*)r3 + 0x64);
    unsigned int v6 = *(unsigned int*)((char*)r3 + 0x68);
    unsigned int v5 = *(unsigned int*)((char*)r3 + 0x6c);
    unsigned int v0 = *(unsigned int*)((char*)r3 + 0x70);
    *(unsigned int*)((char*)r4 + 0) = v7;
    *(unsigned int*)((char*)r4 + 4) = v6;
    *(unsigned int*)((char*)r4 + 8) = v5;
    *(unsigned int*)((char*)r4 + 0xc) = v0;
    return true;
}

// Push the light view's ambient color triple into CScnEnvLgtData::mAmbColorBase.
// Outermost flag (mFlags bit 0) picks between the view's +0x64 triple (bit 0x400)
// and its +0x54 triple (bit 0x200); each path early-returns when its bit is clear.
extern "C" void func_804C5210(CScnEnvLgtCtrlLgtView* view, CScnEnvLgtData* data) {
    if (data->mFlags & 1) {
        if (view->flags & 0x400) {
            data->mAmbColorBase[0] = view->field_0x64;
            data->mAmbColorBase[1] = view->field_0x68;
            data->mAmbColorBase[2] = view->field_0x6C;
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
                func_804C3C9C(self, env, 1);
            }
            ret = func_804C64A8(
                self, (CLight*)((u8*)env + self->field_0xE4 * 0x40), 3, 1);
        }
    } else {
        if (self->field_0x30 != 0) {
            if (self->field_0x04 & 0x800) {
                func_804C3C9C(self, env, 0);
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

extern "C" void func_804C5628(){}

// func_804C58D8 (us-804c9a34): when the light-count control at +0x48 exists
// and +0x04 bit 0 is set, scan the 0xd8-byte entry array at +0x14 for the
// first entry whose u16 type word is 1, hand it to func_804C6D64 and toggle
// arg +0x08 bit 1 on the result; return 1 on a hit, 0 otherwise. The loop
// keeps the byte-offset (off) and index (i) double induction so MWCC emits
// the add-for-the-type-check / mulli-for-the-call pair like retail.
extern "C" int func_804C58D8(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtTarget* arg) {
    CScnEnvLgtCtrlLgtCtl* ctl = self->alt2.field_0x48;
    if (ctl == 0) return 0;
    if ((self->field_0x04 & 1) == 0) return 0;
    u32 count = ctl->mCount;
    u32 i = 0;
    u32 off = 0;
    if (count > 0) {
        do {
            u8* base = (u8*)self->field_0x14_ptr;
            if (*(u16*)(base + off + 2) == 1) {
                if (func_804C6D64(base + i * 0xd8, arg)) {
                    arg->field_0x08 |= 2;
                } else {
                    arg->field_0x08 &= ~2u;
                }
                return 1;
            }
            off += 0xd8;
            i++;
        } while (i < count);
    }
    return 0;
}

// func_804C5990 (us-804c9aec): twin of func_804C58D8 with +0x04 bit 1 as
// the guard, u16 type word 2 as the selector, func_804C6F78 as the dispatch
// and arg +0x64 bit 0 toggled on the result.
extern "C" int func_804C5990(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtTarget* arg) {
    CScnEnvLgtCtrlLgtCtl* ctl = self->alt2.field_0x48;
    if (ctl == 0) return 0;
    if ((self->field_0x04 & 2) == 0) return 0;
    u32 count = ctl->mCount;
    u32 i = 0;
    u32 off = 0;
    if (count > 0) {
        do {
            u8* base = (u8*)self->field_0x14_ptr;
            if (*(u16*)(base + off + 2) == 2) {
                if (func_804C6F78(base + i * 0xd8, arg)) {
                    arg->field_0x64b |= 1;
                } else {
                    arg->field_0x64b &= ~1;
                }
                return 1;
            }
            off += 0xd8;
            i++;
        } while (i < count);
    }
    return 0;
}

// func_804C5A48 (us-804c9ba4): twin of func_804C58D8 with +0x04 bit 2 as
// the guard, u16 type word 3 as the selector, func_804C6F78 as the dispatch
// and arg +0x50 bit 1 toggled on the result.
extern "C" int func_804C5A48(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtTarget* arg) {
    CScnEnvLgtCtrlLgtCtl* ctl = self->alt2.field_0x48;
    if (ctl != 0 && (self->field_0x04 & 0x4)) {
        u32 count = ctl->mCount;
        for (int i = 0, off = 0; i < count; off += 0xd8, i++) {
            u8* base = (u8*)self->field_0x14_ptr;
            if (*(u16*)(base + off + 2) == 3) {
                if (func_804C6F78(base + i * 0xd8, arg)) {
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
    self->field_0x24[r5].field_0x00 = *r6;
    self->field_0x24[r5].field_0x0C = *r6;
    self->field_0x24[r5].field_0x18.x = f32tou32(v);
    self->field_0x24[r5].field_0x18.y = f32tou32(v);
    self->field_0x24[r5].field_0x18.z = f32tou32(v);
    self->field_0x24[r5].field_0x24 = f1;
    self->field_0x24[r5].field_0x28 |= 0x1;
    self->field_0x24[r5].field_0x28 |= 0x2;
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
        u8* base = self->field_0x2C;
        u32 count = ctl->mCount;
        CScnEnvLgtCtrlLgtParamEntry* p =
            (CScnEnvLgtCtrlLgtParamEntry*)(base + off);
        for (u32 i = 0; i < count; i++, p++) {
            if (p->field_0x04 & 0x10000) {
                CScnEnvLgtCtrlLgtItem20* item =
                    (CScnEnvLgtCtrlLgtItem20*)((u8*)self->field_0x0C +
                                               p->field_0x20 * 0x14);
                item->field_0x10 |= 2;
                return;
            }
        }
    } else {
        u32 off = ctl->mOffset;
        u8* base = self->field_0x2C;
        u32 count = ctl->mCount;
        CScnEnvLgtCtrlLgtParamEntry* p =
            (CScnEnvLgtCtrlLgtParamEntry*)(base + off);
        for (u32 i = 0; i < count; i++, p++) {
            if (p->field_0x04 & 0x10000) {
                CScnEnvLgtCtrlLgtItem20* item =
                    (CScnEnvLgtCtrlLgtItem20*)((u8*)self->field_0x0C +
                                               p->field_0x20 * 0x14);
                item->field_0x10 &= ~2u;
                return;
            }
        }
    }
}

extern "C" __declspec(noinline) void func_804C6110(CScnEnvLgtCtrl* self) {}

extern "C" __declspec(noinline) u32 func_804C64A8(CScnEnvLgtCtrl* self,
                                                    CLight* light, u32 slot,
                                                    u32 reset) {
    return 0;  // stub - separate target (us-804ca604)
}

void func_804C678C(){}

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
                dst->field_0x28[i] = off;
                off += dst->field_0x12 * (dst->field_0x18[i] + 1) * 4;
            }
            if (src->field_0x00 & 0x10) {
                dst->field_0x14 |= 0x20;
                dst->field_0x2C[i] = off;
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
        f32 f1 = lbl_eu_8066B058;
        f32* src = (f32*)((u8*)e + row * 0x20);
        int i = 0;
        while (i < e->field_0x12w) {
            dst[i] = f1 * (src[i + 0x0E] - dst[i]) + dst[i];
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
        f32 f1 = lbl_eu_8066B058;
        f32* src = (f32*)((u8*)e + row * 0x20);
        int i = 0;
        while (i < e->field_0x12w) {
            dst[i] = f1 * (src[i + 0x0E] - dst[i]) + dst[i];
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

void func_804C7190(){}

// copy previous-frame light rows (0x38/0x58) to the current frame (0x78/0x98)
void func_804C7530(CScnEnvLgtCtrl* self) {
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
    CScnEnvLgtCtrlGrad16Entry* p = (CScnEnvLgtCtrlGrad16Entry*)(arr + 4);
    for (int i = 1; i < grad->field_0x0A; i++) {
        if (p[0].time > self->field_0x1C) {
            f32 t = (self->field_0x1C - p[-1].time) / (p[0].time - p[-1].time);
            f32 t2 = t * t;
            f32 t3 = t2 * t;
            f32 k = lbl_eu_8066B074 * t2;
            f32 h10 = t + (t3 - lbl_eu_8066B070 * t2);
            f32 h00 = lbl_eu_8066B060 + (lbl_eu_8066B070 * t3 - k);
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
    f32* arr = (f32*)((u8*)self->mVtable + grad->field_0x10);
    if (self->field_0x1C <= grad->field_0x00) {
        return arr[1];
    }
    if (self->field_0x1C >= grad->field_0x04) {
        return arr[2 * (grad->field_0x0A - 1) + 1];
    }
    u16 count = grad->field_0x0A;
    f32* p = arr + 2;
    for (int i = 1; i < count; i++) {
        if (self->field_0x1C < p[0]) {
            return p[-1];
        }
        p += 2;
    }
    return lbl_eu_8066B05C;
}
extern "C" __declspec(noinline) void func_804C7910(CScnEnvLgtCtrlWorkBlob* blob){}

void func_804C7B54(){}

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
        u16 idx = e->field_0x16;
        e->field_0x14 &= ~0x8000;
        CScnEnvLgtCtrlWorkTable* tbl =
            (CScnEnvLgtCtrlWorkTable*)((u8*)blob->field_0x08 + idx * 0xC);
        u32 off = tbl->offset;
        int count = tbl->count;
        CScnEnvLgtCtrlLgtTypeItem* item =
            (CScnEnvLgtCtrlLgtTypeItem*)(blob->field_0x00 + off);
        for (u32 i = 0; i < (u32)count; i++) {
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
    // loads: wave f [0x118..0x16C) then wave g [0x16C..0x1A8)
    u32 f118 = *(u32*)(src + 0x114), f11C = *(u32*)(src + 0x118),
        f120 = *(u32*)(src + 0x11C), f124 = *(u32*)(src + 0x120),
        f128 = *(u32*)(src + 0x124), f12C = *(u32*)(src + 0x128),
        f130 = *(u32*)(src + 0x12C), f134 = *(u32*)(src + 0x130),
        f138 = *(u32*)(src + 0x134), f13C = *(u32*)(src + 0x138),
        f140 = *(u32*)(src + 0x13C), f144 = *(u32*)(src + 0x140),
        f148 = *(u32*)(src + 0x144), f14C = *(u32*)(src + 0x148),
        f150 = *(u32*)(src + 0x14C), f154 = *(u32*)(src + 0x150),
        f158 = *(u32*)(src + 0x154), f15C = *(u32*)(src + 0x158),
        f160 = *(u32*)(src + 0x15C), f164 = *(u32*)(src + 0x160),
        f168 = *(u32*)(src + 0x164),
        // then wave g [0x16C..0x1A8) (15 words, spilled while f is live)
        g16C = *(u32*)(src + 0x168), g170 = *(u32*)(src + 0x16C),
        g174 = *(u32*)(src + 0x170), g178 = *(u32*)(src + 0x174),
        g17C = *(u32*)(src + 0x178), g180 = *(u32*)(src + 0x17C),
        g184 = *(u32*)(src + 0x180), g188 = *(u32*)(src + 0x184),
        g18C = *(u32*)(src + 0x188), g190 = *(u32*)(src + 0x18C),
        g194 = *(u32*)(src + 0x190), g198 = *(u32*)(src + 0x194),
        g19C = *(u32*)(src + 0x198), g1A0 = *(u32*)(src + 0x19C),
        g1A4 = *(u32*)(src + 0x1A0);
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
            CScnEnvLgtOwnerLgt* owner = (CScnEnvLgtOwnerLgt*)self->field_0x04_ptr;
            func_8049D1EC(owner->field_0x70, blend);
            func_8049D1EC(owner->field_0x70,
                          bloom ? (CScnEnvLgtBlend*)((u8*)bloom + 0x48) : 0);
            self->flags |= 0x8;
            func_804C26F0(self);
            ((CScnEnvLgtCtrlLgtParamCtlTail*)self->field_0x30)->field_0x64 &= ~1;
            self->field_0x28_blend->mFlags &= ~2;
            self->field_0x2C_bloom->field_0x50 &= ~2;
        }
    } else {
        if (self->flags & 0x8) {
            CScnEnvLgtOwnerLgt* owner = (CScnEnvLgtOwnerLgt*)self->field_0x04_ptr;
            func_8049D3D8(owner->field_0x70, self->field_0x28_blend);
            func_8049D3D8(owner->field_0x70,
                          self->field_0x2C_bloom
                              ? (CScnEnvLgtBlend*)((u8*)self->field_0x2C_bloom + 0x48)
                              : 0);
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
    CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
    u32 ret = 0;
    goto cond;
body:
    func_80496288((CScnItemModelNw4rOwner*)self->field_0x04_ptr);
    node->mItem->v1();
    ret = 1;
    node = node->mNext;
cond:
    if (node != self->field_0x0C) goto body;
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
extern "C" void func_804C4E04();
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
DECOMP_FORCEACTIVE(CScnEnvLgtCtrl_cpp, lbl_eu_8065FAF8);
DECOMP_FORCEACTIVE(CScnEnvLgtCtrl_cpp, lbl_eu_8065FBD0);
