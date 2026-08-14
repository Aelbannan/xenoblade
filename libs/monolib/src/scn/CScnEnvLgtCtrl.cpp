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
#include "monolib/scn/CScn.hpp"              // CScn (mCamWork / UnkScn68, func_804C2654)
#include "libs/monolib/src/scn/CScnItemModelNw4r.hpp"  // func_8048ECD8 / func_80496288 queries
#include "monolib/scn/code_804BF59C.hpp"  // CScnEnvLgtData (mAmbColorBase / mFlags)
#include "monolib/util/MemManager.hpp"    // mtl::MemManager (allocate / deallocate)

// shared float constant in .sdata2 (referenced via @sda21 by this TU)
extern float lbl_eu_8066B010;


void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }


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

void func_804C172C(){}

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

void func_804C19B8(){}

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

void func_804C1BA0(){}

void func_804C1D7C(){}

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

void func_804C2124(){}

void func_804C22F0(){}

// func_804C2654 (us-804c67b0): resolve the active camera item through the
// scene's camera-work blob (CScn+0x68) and walk the light-object ring;
// every active item (vtable 0x64) receives the camera data at +0x10c of
// the camera item through vtable slot 0x10.
void func_804C2654(CScnEnvLgtCtrl* self) {
    CScn* scn = (CScn*)self->field_0x04_ptr;
    CScnCameraItem* camItem =
        func_8049B158(scn->mCamWork, scn->mCamWork->unk34);
    CScnEnvLgtCtrlListNode* node = self->field_0x0C->mNext;
    while (node != self->field_0x0C) {
        if (node->mItem->v23()) {
            node->mItem->v2((u8*)camItem + 0x10c);
        }
        node = node->mNext;
    }
}

void func_804C26F0(){}

// Stub body only - the constructor is a separate target (us-804c6afc). The
// signature matches its retail caller func_804C6A70. noinline keeps the call
// out-of-line (the retail factory emits `bl __ct__CScnEnvLgtCtrl`).
extern "C" __declspec(noinline) CScnEnvLgtCtrl* __ct__CScnEnvLgtCtrl(
    CScnEnvLgtCtrl* self, const u32* data, void* arg) {
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

void func_804C30E8(){}

void func_804C31C8(){}

extern "C" void func_804C34A0(void* self);

void func_804C33F0(void* self) {
    if (*(u32*)((u8*)self + 0x30) == 0)
        return;
    func_804C34A0(self);
}

// func_804C3404 (us-804c7560): push the default light value (+0x30 deref, a
// .sdata2 float) to +0xEC and the 3-word color triple to +0xD0, then
// refresh the four CLightEnv slot fields: each slot field is copied to +0xE4
// and recomputed by func_804C64A8. The loop walks the bank with an explicit
// byte offset (i counter + off induction) to reproduce retail's
// counter/offset double induction (add r30,r28,r31 per iteration).
void func_804C3404(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLightEnvView* env,
                   const u32* data) {
    if (self->field_0x30 == 0) return;
    self->field_0xEC = *self->field_0x30;
    self->field_0xD0 = data[0];
    self->field_0xD4 = data[1];
    self->field_0xD8 = data[2];
    u32 i = 0;
    u32 off = 0;
    do {
        u8* slot = (u8*)env + off;
        self->field_0xE4 = *(u32*)(slot + 0x1160);
        *(u32*)(slot + 0x1160) =
            func_804C64A8(self, *(CLight**)(slot + 0x1140), i, 0);
        i++;
        off += 4;
    } while (i < 4);
}

extern "C" void func_804C34A0(void* self) {
    func_804C34A0(self); // stub: recursion prevents MWCC from inlining the (unreconstructed) body
}

void func_804C3778(){}

void func_804C392C(){}

void func_804C3AC8(){}

void func_804C3C9C(){}

void func_804C3F58(){}

void func_804C406C(){}

void func_804C42A8(){}

void func_804C43A4(){}

void func_804C4954(){}

void func_804C4D28(){}

void func_804C4E04(){}

bool func_804C5198(CScnEnvLgtCtrlLgtView* self, CScnEnvLgtCtrlLgtData* out) {
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

bool func_804C51D4(void* r3, void* r4) {
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
void func_804C5210(CScnEnvLgtCtrlLgtView* view, CScnEnvLgtData* data) {
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

void func_804C526C(){}

void func_804C5380(){}

void func_804C54D4(){}

void func_804C5628(){}

void func_804C58D8(){}

void func_804C5990(){}

void func_804C5A48(){}

void func_804C5B00(){}

// Set/clear one bit in the +0x28 bit array. bit is validated < 0x140 (320 bits);
// `clear` non-zero clears the bit (andc), zero sets it (or).
void func_804C5C08(CScnEnvLgtCtrl* self, int bit, int clear) {
    u32* arr = self->field_0x28;
    if (arr == 0) return;
    if (bit >= 0x140) return;
    if (clear != 0) {
        arr[bit >> 5] &= ~(1u << (bit & 31));
    } else {
        arr[bit >> 5] |= (1u << (bit & 31));
    }
}

void func_804C5C6C(){}

void func_804C5D7C(void* self, unsigned int* out) {
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
void func_804C5DA0(CScnEnvLgtCtrl* self, int r4, int r5) {
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
void func_804C5E04(CScnEnvLgtCtrl* self, int r4, int r5,
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

void func_804C5E9C(){}

// Clear bit 0 of a light slot's +0x28 flags. Slot index: r5 clamped to 0..1,
// plus 2 when r4 is non-zero (the (r4 | -r4) >> 31 idiom yields -1 iff r4 != 0).
void func_804C5F6C(CScnEnvLgtCtrl* self, int r4, int r5) {
    CScnEnvLgtCtrlLgtSlot* slot = self->field_0x24;
    if (slot == 0) return;
    if (r5 > 1) r5 = 0;
    r5 += ((-r4 | r4) >> 31) & 2;
    slot[r5].field_0x28 &= 0xFFFE;
}

// Set bit 2 of a light slot's +0x28 flags and write the three u16 params at
// +0x2A..+0x2E (curve values: base + idx*60, base + idx*60, and a raw u16).
void func_804C5FB0(CScnEnvLgtCtrl* self, int r4, int r5, int r6, int r7, int r8, int r9, int r10) {
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
void func_804C6010(CScnEnvLgtCtrl* self, int r4, int r5) {
    CScnEnvLgtCtrlLgtSlot* slot = self->field_0x24;
    if (slot == 0) return;
    if (r5 > 1) r5 = 0;
    r5 += ((-r4 | r4) >> 31) & 2;
    slot[r5].field_0x28 &= 0xFFFB;
}

void func_804C6054(){}

void func_804C6110(){}

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
CScnEnvLgtCtrl* func_804C6A70(u32 handle, const u32* data, void* arg) {
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

int func_804C6AE8(unsigned int* arg0) {
    return (arg0[1] >> 28) & 1;
}

void func_804C6AF4(void* ptr, int flag) {
    unsigned int* word = (unsigned int*)((char*)ptr + 4);
    if (flag != 0)
        *word |= 0x40000000;
    else
        *word &= ~0x40000000;
}

void func_804C6B1C(void* r3, int r4) {
    if (r4 != 0) {
        *(unsigned int*)((char*)r3 + 4) |= 0x80000000u;
    } else {
        *(unsigned int*)((char*)r3 + 4) &= ~0x80000000u;
    }
}

// retail: flags(0x4): return 1 iff bit 0x80000000 set and bit 0x40000000 clear
u32 func_804C6B44(void* self) {
    u32 flags = *(u32*)((u8*)self + 4);
    u32 ret = 0;
    if ((flags & 0x80000000) && !(flags & 0x40000000))
        ret = 1;
    return ret;
}


float func_804C6B6C(void* self) {
    void* obj = *(void**)((u8*)self + 0x4c);
    if (obj != 0)
        return *(float*)((u8*)obj + 8);
    return lbl_eu_8066B010;
}

float func_804C6B88(void* self) { return *(float*)((u8*)self + 0xc8); }

void func_804C6B90(void* self, float val) { *(float*)((u8*)self + 0xc8) = val; }

float func_804C6B98(void* self) { return *(float*)((u8*)self + 0xcc); }

void func_804C6BA0(void* self, float val) { *(float*)((u8*)self + 0xcc) = val; }

void func_804C6BA8(){}

void func_804C6D64(){}

void func_804C6F78(){}

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

u32 func_804C75B4(u32 unused, void* obj, float val) { *(float*)((char*)obj + 0) = val; return unused; }

u32 func_804C75BC(u32 unused, void* obj, float val) { *(float*)((char*)obj + 4) = val; return unused; }

u32 func_804C75C4(u32 unused, void* obj, float val) { *(float*)((char*)obj + 8) = val; return unused; }

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

u32 func_804C7608(u32 unused, void* obj, float val) { *(float*)((char*)obj + 40) = val; return unused; }

u32 func_804C7610(u32 unused, void* obj, float val) { *(float*)((char*)obj + 44) = val; return unused; }

u32 func_804C7618(u32 unused, void* obj, float val) { *(float*)((char*)obj + 48) = val; return unused; }

void func_804C7620(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(int*)((char*)ptr + 0x34) = val;
}

void func_804C763C(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(int*)((char*)ptr + 0x38) = val;
}

void func_804C7658(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0x4) = (short)val;
}

u32 func_804C7674(u32 unused, void* obj, float val) { *(float*)((char*)obj + 184) = val; return unused; }

u32 func_804C767C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 188) = val; return unused; }

u32 func_804C7684(u32 unused, void* obj, float val) { *(float*)((char*)obj + 192) = val; return unused; }

u32 func_804C768C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 196) = val; return unused; }

u32 func_804C7694(u32 unused, void* obj, float val) { *(float*)((char*)obj + 200) = val; return unused; }

u32 func_804C769C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 204) = val; return unused; }

u32 func_804C76A4(u32 unused, void* obj, float val) { *(float*)((char*)obj + 208) = val; return unused; }

void func_804C76AC(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0x6) = (short)val;
}

void func_804C76C8(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0x8) = (short)val;
}

void func_804C76E4(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0xa) = (short)val;
}

void func_804C7700(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0xc) = (short)val;
}

void func_804C771C(int, void* ptr, float value) {
    int i = (int)value;
    *(short*)((char*)ptr + 0xe) = (short)i;
}

void func_804C7738(int dummy, float f, void* ptr) {
    int val = (int)f;
    *(short*)((char*)ptr + 0x10) = (short)val;
}

u32 func_804C7754(u32 unused, void* obj, float val) { *(float*)((char*)obj + 200) = val; return unused; }

u32 func_804C775C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 204) = val; return unused; }

extern "C" u32 func_804C7764(u32 unused, void* obj, float val) { *(float*)((char*)obj + 208) = val; return unused; }

extern "C" u32 func_804C776C(u32 unused, void* obj, float val) { *(float*)((char*)obj + 212) = val; return unused; }

void func_804C7774(void* self, u32 a, u32 b, float f, u32 c) {
    *(u32*)((u8*)self + 0x0) = a;
    *(u32*)((u8*)self + 0x4) = b;
    *(u32*)((u8*)self + 0x8) = b + 0x18;
    *(float*)((u8*)self + 0x1c) = f;
    *(u32*)((u8*)self + 0x18) = c;
}

void func_804C7790(){}

// func_804C7880 (us-804cb9dc): sample self->field_0x1C against the gradient
// table: color of the first entry below the min bound, of the last entry
// above the max bound, otherwise of the highest entry whose time is <= the
// sample; a sentinel float when no entry applies.
float func_804C7880(CScnEnvLgtCtrl* self, const CScnEnvLgtCtrlGrad* grad) {
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
void func_804C7910(){}

void func_804C7B54(){}

void func_804C8054(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804C8174(){}

extern "C" void func_804C1094() {}
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
extern "C" void func_804C1338() {}
