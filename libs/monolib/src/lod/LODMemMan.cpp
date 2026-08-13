// Auto-scaffolded catalog TU for monolib/src/lod/LODMemMan
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/lod/LODMemMan.hpp"
#include "monolib/core/CView.hpp"           // CView::getCurrentView
#include "monolib/core/code_804E36DC.hpp"   // func_80496288 (frame delta, C ABI)
#include "nw4r/g3d/g3d_obj.h"               // nw4r::g3d::G3dObj::Destroy
#include <string.h>                         // strlen / strcpy                         // strlen / strcpy
#include "monolib/util/MemManager.hpp"      // mtl::MemManager::deallocate
#include "monolib/device/CDeviceFile.hpp"   // CDeviceFile::cancel
#include <nw4r/g3d/g3d_scnobj.h>            // nw4r::g3d::ScnGroup (Remove at vtable+0x3C)

class CScn;  // scene wrapper (forward decl; CScnMem.hpp is not included here)

// Retail data labels used by this TU.  Global-scope data imports keep their
// plain unmangled names, so plain extern declarations suffice (no extern "C").
extern u32 lbl_eu_80663828;          // sentinel "no next" index for the element list
extern int lbl_eu_80665830;          // global reset flag (0 = release buffers)
extern const char lbl_eu_80523D90[];  // reset-time name string (copied into +0x34)

// ---------------------------------------------------------------------------
// Recovered LODMemMan field layout.  The shared header only exposes opaque
// padding up to 0x7c plus the 0xabc embedded sub-object, so the fields used
// by the targets below are described here and the object is viewed through
// this struct instead of raw pointer arithmetic.
// ---------------------------------------------------------------------------

// 0x20-byte LOD element.  The flag word at 0x0 is the embedded
// UnkClass_8046A530 view (func_8046CFB4 is called with the element pointer).
// The element also participates in an index-linked list: +0x16 holds the next
// element index (sentinel lbl_eu_80663828), +0x1A the element's own index,
// and +0x1C/+0x1E/+0x1F are per-element byte state.
struct LODElem20 {
    u32 field_0x0;                 // 0x00 flag word (UnkClass_8046A530 view)
    f32 field_0x4;                 // 0x04 active value
    f32 field_0x8;                 // 0x08 cached value
    u8  mPad_0C[0x16 - 0xC];       // 0x0c..0x15
    u16 field_0x16;                // 0x16 next element index (list link)
    u8  mPad_18[0x1A - 0x18];      // 0x18..0x19
    s16 field_0x1A;                // 0x1a own element index (list link)
    u8  field_0x1C;                // 0x1c
    u8  mPad_1D;                   // 0x1d
    u8  field_0x1E;                // 0x1e
    u8  field_0x1F;                // 0x1f
};

// 0x48-byte LOD descriptor.  The u16 fields at 0x40/0x42/0x44 gate the
// per-element updates.
struct LODElem48 {
    u8  mPad_00[0x40];             // 0x00..0x3f
    u16 field_0x40;                // 0x40 element id
    u16 field_0x42;                // 0x42 enabled flag (nonzero)
    u16 field_0x44;                // 0x44 flag bits (bit 3: follow-through)
    u8  mPad_46[0x48 - 0x46];      // 0x46..0x47
};

// 0xc-byte g3d object slot (func_80471184 destroys the pointer at +0x0).
struct LODG3dSlot {
    nw4r::g3d::G3dObj* field_0x0;  // 0x00 g3d object
    u8  mPad_04[0xC - 0x4];        // 0x04..0x0b
};

// Vtable mirror for the g3d manager object (LODMemMan+0x0).  MWCC vtables
// carry an 8-byte RTTI header, so declared slot 0 sits at vtable+0x8 and the
// slot func_80471184 dispatches through (vtable+0x3C) is the 14th virtual.
struct LODG3dMgrVt {
    virtual void vf00(nw4r::g3d::G3dObj*) = 0;  // vtable+0x08
    virtual void vf01(nw4r::g3d::G3dObj*) = 0;  // vtable+0x0C
    virtual void vf02(nw4r::g3d::G3dObj*) = 0;  // vtable+0x10
    virtual void vf03(nw4r::g3d::G3dObj*) = 0;  // vtable+0x14
    virtual void vf04(nw4r::g3d::G3dObj*) = 0;  // vtable+0x18
    virtual void vf05(nw4r::g3d::G3dObj*) = 0;  // vtable+0x1C
    virtual void vf06(nw4r::g3d::G3dObj*) = 0;  // vtable+0x20
    virtual void vf07(nw4r::g3d::G3dObj*) = 0;  // vtable+0x24
    virtual void vf08(nw4r::g3d::G3dObj*) = 0;  // vtable+0x28
    virtual void vf09(nw4r::g3d::G3dObj*) = 0;  // vtable+0x2C
    virtual void vf0A(nw4r::g3d::G3dObj*) = 0;  // vtable+0x30
    virtual void vf0B(nw4r::g3d::G3dObj*) = 0;  // vtable+0x34
    virtual void vf0C(nw4r::g3d::G3dObj*) = 0;  // vtable+0x38
    virtual void vf0D(nw4r::g3d::G3dObj*) = 0;  // vtable+0x3C (called slot)
};

// View frame returned by func_8049626C; +0x9C is handed to the LOD layer
// updates and +0x1E0 holds the scale (mirrors UnkViewFrame in
// code_8047BB54.cpp).
struct LODViewFrame {
    u8  mPad_00[0x9C];             // 0x00..0x9b
    u8  field_0x9C[0x1E0 - 0x9C];  // 0x9c..0x1df
    f32 field_0x1E0;               // 0x1e0 layer scale
};

// Scene object returned by func_8048ECD0; the byte at +0x19 gates the
// per-frame LOD layer update.
struct LODScnGate {
    u8 mPad_00[0x19];              // 0x00..0x18
    u8 field_0x19;                 // 0x19 gate byte
};

// Full field overlay for the LODMemMan targets in this TU.
struct LODMemManLayout {
    /* 0x00 */ nw4r::g3d::G3dObj* field_0x0;  // g3d manager object
    /* 0x04 */ LODElem20* field_0x4;          // 0x20-strided element list
    /* 0x08 */ u8* field_0x8;                 // secondary buffer (released on reset)
    /* 0x0C */ u32 field_0xC;                 // cleared on reset
    /* 0x10 */ LODElem20* field_0x10;         // index-list head
    /* 0x14 */ LODElem20* field_0x14;         // index-list tail
    /* 0x18 */ s32 mCount_18;                 // element count
    /* 0x1C */ u8* mView_1C;                  // view (func_80496288 arg)
    /* 0x20 */ u32 field_0x20;                // cleared on reset
    /* 0x24 */ u8 mPad_24[0x30 - 0x24];
    /* 0x30 */ CFileHandle* field_0x30;       // file handle cancelled on reset
    /* 0x34 */ char mStr_34[0x6C - 0x34];     // reset-time name string
    /* 0x6C */ u32 field_0x6C;                // flag bits (bit 0: enabled)
    /* 0x70 */ u8 mPad_70[0x74 - 0x70];
    /* 0x74 */ s32 field_0x74;                // strlen of the name string
    /* 0x78 */ u8 mPad_78[0x7C - 0x78];
    /* 0x7C */ f32 field_0x7C;                // base scale
    /* 0x80 */ f32 field_0x80;                // scaled frame value
    /* 0x84 */ u8 mPad_84[0x90 - 0x84];
    /* 0x90 */ LODElem48* field_0x90;         // 0x48-strided descriptor list
    /* 0x94 */ u8 mPad_94[0xC8 - 0x94];
    /* 0xC8 */ LODG3dSlot field_0xC8[16];     // g3d object slots (0xc8..0x188)
};

// Sub-manager pointer at +0xF0, which sits inside the 0xc8 slot range, is
// accessed through its own overlay.
struct LODSubMgrView {
    u8 mPad_00[0xF0];
    LOD::LODMemMan* field_0xF0;               // 0xf0 sub-manager
};

// ---------------------------------------------------------------------------
// Retail-named imports for the LOD targets.  The Fv-mangled callees that
// receive extra args in retail are declared with the exact mangled names.
// MWCC re-mangles `__`-containing global names by appending the parameter
// signature, so the exact retail names are forced with C linkage (the
// reloc_map tool's approved fix for name drift).
// ---------------------------------------------------------------------------
extern "C" void func_804702F0__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LODElem20* elem);
extern "C" void func_8046DD9C__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u8* p, f32 f1);
extern "C" void func_8046E1DC__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u8* p, int a);

// Scene / view helpers defined in other monolib TUs.  Retail kept the plain
// unmangled names; C linkage forces the verbatim names at the call sites.
extern "C" CScn* func_8049698C();
extern "C" LODViewFrame* func_8049626C(CScn* camera, CView* view);
extern "C" u32 func_8048ECD0(CScn* self);
extern "C" nw4r::g3d::ScnObj* func_8048EC14(CScn* self, u32 idx);

// --- FULL_MATCH functions ---

// Simple getter: return the float at offset 0x7c.
float LOD::LODMemMan::func_8046F01C() {
    return mFloat_7C;
}

// Tail-forward call to embedded sub-object's func_8046AADC.
void LOD::LODMemMan::func_8046F088() {
    mSubObject.func_8046AADC();
}

// --- Remaining harness stubs (empty bodies) ---

LOD::LODMemMan::LODMemMan() {}

LOD::LODMemMan::~LODMemMan() {}

void __dt__8046D144(){}

void LOD::LODMemMan::func_8046D264() {}

void LOD::LODMemMan::func_8046D898() {}

void LOD::LODMemMan::func_8046DA04() {}

void LOD::LODMemMan::func_8046DA64() {}

void LOD::LODMemMan::func_8046DAC0() {}

void LOD::LODMemMan::func_8046DBC8() {}

void LOD::LODMemMan::func_8046DD9C() {}

void LOD::LODMemMan::func_8046E1DC() {}

void LOD::LODMemMan::func_8046E594() {}

void LOD::LODMemMan::func_8046E5BC() {}

void LOD::LODMemMan::func_8046E6DC() {}

void LOD::LODMemMan::func_8046E770() {}

void LOD::LODMemMan::func_8046E780() {}

void LOD::LODMemMan::func_8046E7D0() {}

// ---------------------------------------------------------------------------
// func_8046E820: when the LOD system is enabled, walk every element matching
// `task` and flip bit 1 of its flag word - set when `flag` is zero, cleared
// otherwise (retail keeps two copies of the loop, one per bit operation).
// ---------------------------------------------------------------------------
void func_8046E820__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task, int flag) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (flag) {
        if (!(l->field_0x6C & 1)) return;
        int i = 0;
        LODElem20* p20;
        LODElem48* p48;
        p48 = l->field_0x90;
        p20 = l->field_0x4;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) p20->field_0x0 &= ~2u;
            i++;
            p48++;
            p20++;
        }
    } else {
        if (!(l->field_0x6C & 1)) return;
        int i = 0;
        LODElem20* p20;
        LODElem48* p48;
        p48 = l->field_0x90;
        p20 = l->field_0x4;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) p20->field_0x0 |= 2u;
            i++;
            p48++;
            p20++;
        }
    }
}

void LOD::LODMemMan::func_8046E8C8() {}

void LOD::LODMemMan::func_8046E920() {}

void LOD::LODMemMan::func_8046E988() {}

void LOD::LODMemMan::func_8046EAE8() {}

// ---------------------------------------------------------------------------
// func_8046EB50: if the LOD system is enabled, push the scale value into
// every element whose id matches `task`; elements with the follow-through
// flag bit and a nonzero enable field then refresh their animation state.
// ---------------------------------------------------------------------------
void func_8046EB50__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task, f32 v) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x6C & 1) {
        LODElem48* p48 = l->field_0x90;
        LODElem20* p20 = l->field_0x4;
        int i = 0;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) {
                p20->field_0x4 = v;
                if (p48->field_0x44 & 8) {
                    if (p48->field_0x42 != 0) {
                        ((LOD::UnkClass_8046A530*)p20)->func_8046CFB4();
                    }
                }
            }
            i++;
            p48++;
            p20++;
        }
    }
}

// ---------------------------------------------------------------------------
// func_8046EBEC: same id walk as func_8046EB50, but the active value is
// refreshed from the cached value instead of a parameter.
// ---------------------------------------------------------------------------
void func_8046EBEC__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x6C & 1) {
        LODElem48* p48 = l->field_0x90;
        LODElem20* p20 = l->field_0x4;
        int i = 0;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) {
                p20->field_0x4 = p20->field_0x8;
                if (p48->field_0x44 & 8) {
                    if (p48->field_0x42 != 0) {
                        ((LOD::UnkClass_8046A530*)p20)->func_8046CFB4();
                    }
                }
            }
            i++;
            p48++;
            p20++;
        }
    }
}

void LOD::LODMemMan::func_8046EC88() {}

void LOD::LODMemMan::func_8046ECD4() {}

void LOD::LODMemMan::func_8046ED68() {}

// ---------------------------------------------------------------------------
// func_8046EDD0: for each element matching `task`, when its byte state (+0x1E)
// differs from `value` and the 0x1000 flag is set, refresh +0x1C/+0x1E/+0x1F
// and link the element into the index-linked list (head/tail at +0x10/+0x14)
// unless it is already linked (0x10000 flag).
// ---------------------------------------------------------------------------
void func_8046EDD0__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task, int value) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(l->field_0x6C & 1)) return;
    LODElem48* p48 = l->field_0x90;
    LODElem20* p20 = l->field_0x4;
    int i = 0;
    while (i < l->mCount_18) {
        if (p48->field_0x40 == task) {
            int changed;
            if (p20->field_0x1E != value && (p20->field_0x0 & 0x1000)) {
                p20->field_0x0 |= 0x2000;
                changed = 1;
                p20->field_0x1F = p20->field_0x1E;
                p20->field_0x1E = value;
                p20->field_0x1C = 0;
            } else {
                changed = 0;
            }
            if (changed) {
                if (!(p20->field_0x0 & 0x10000)) {
                    if (l->field_0x14 == 0) {
                        l->field_0x10 = p20;
                    } else {
                        l->field_0x14->field_0x16 = p20->field_0x1A;
                    }
                    l->field_0x14 = p20;
                    p20->field_0x16 = lbl_eu_80663828;
                    p20->field_0x0 |= 0x10000;
                }
            }
        }
        i++;
        p48++;
        p20++;
    }
}

void LOD::LODMemMan::func_8046EE9C() {}

void LOD::LODMemMan::func_8046EEE8() {}

void LOD::LODMemMan::func_8046EF30() {}

// ---------------------------------------------------------------------------
// func_8046EF7C: when the LOD system is enabled, scale the base value by the
// current frame delta and forward the matching element to func_804702F0.
// ---------------------------------------------------------------------------
void func_8046EF7C__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int id) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x6C & 1) {
        f32 dt = func_80496288(l->mView_1C);
        l->field_0x80 = l->field_0x7C * dt;
        LODElem48* p48 = l->field_0x90;
        LODElem20* p20 = l->field_0x4;
        int i = 0;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == id) {
                func_804702F0__Q23LOD9LODMemManFv(self, p20);
            }
            i++;
            p48++;
            p20++;
        }
    }
}

void LOD::LODMemMan::func_8046F010(float a) { *(float*)((u8*)this + 0x1CC8) = a; *(float*)((u8*)this + 0x7C) = a; }

void LOD::LODMemMan::func_8046F024() {}

void LOD::LODMemMan::func_8046F090() {}

void LOD::LODMemMan::func_8046F164() {}

void LOD::LODMemMan::func_8046F258() {}

void LOD::LODMemMan::func_8046F594() {}

void LOD::LODMemMan::func_8046FC04() {}

// ---------------------------------------------------------------------------
// func_8046FEB8: unlink `elem` from the index-linked list.  Each element's
// +0x16 holds the next element index (sentinel lbl_eu_80663828) and +0x1A the
// element's own index; head removal advances the head, empty-list removal
// clears both pointers, and mid-list removal splices the successor into the
// predecessor's link.
// ---------------------------------------------------------------------------
void func_8046FEB8__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LODElem20* elem) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(elem->field_0x0 & 0x10000)) return;
    elem->field_0x0 &= ~0x10000;
    if (l->field_0x14 == 0 || l->field_0x10 == 0) {
        elem->field_0x16 = lbl_eu_80663828;
        return;
    }
    if (l->field_0x10 == elem) {
        int next = elem->field_0x16;
        if (next != lbl_eu_80663828) {
            l->field_0x10 = &l->field_0x4[next];
        } else {
            l->field_0x10 = 0;
            l->field_0x14 = 0;
        }
    } else {
        int idx = l->field_0x10->field_0x1A;
        while (idx != lbl_eu_80663828) {
            LODElem20* p = &l->field_0x4[idx];
            if (p->field_0x16 == elem->field_0x1A) {
                p->field_0x16 = elem->field_0x16;
                if (l->field_0x14 == elem) l->field_0x14 = p;
                break;
            }
            idx = p->field_0x16;
        }
    }
    elem->field_0x16 = lbl_eu_80663828;
}

void LOD::LODMemMan::func_8046FF84() {}

void LOD::LODMemMan::func_80470184() {}

void LOD::LODMemMan::func_804702F0() {}

void LOD::LODMemMan::func_80470634() {}

void LOD::LODMemMan::func_804708B4() {}

// ---------------------------------------------------------------------------
// func_804709FC: resolve the current view frame and, when the caller passes a
// nonzero task, push the layer scale and the frame data into the sub-manager
// (func_8046DD9C is skipped while the scene gate byte is set).
// ---------------------------------------------------------------------------
void func_804709FC__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    CView* view = CView::getCurrentView();
    LODViewFrame* frame = func_8049626C(func_8049698C(), view);
    LOD::LODMemMan* sub = ((LODSubMgrView*)self)->field_0xF0;
    if (task != 0) {
        if (((LODScnGate*)func_8048ECD0(func_8049698C()))->field_0x19 == 0) {
            func_8046DD9C__Q23LOD9LODMemManFv(sub, frame->field_0x9C, frame->field_0x1E0);
        }
        func_8046E1DC__Q23LOD9LODMemManFv(sub, frame->field_0x9C, task);
    }
}

void LOD::LODMemMan::func_80470A90() {}

void LOD::LODMemMan::func_80470B10() {}

void LOD::LODMemMan::func_80470DB0() {}

void LOD::LODMemMan::func_80470DCC() {}

void LOD::LODMemMan::func_80470EF8() {}

void LOD::LODMemMan::func_8047108C() {}

// ---------------------------------------------------------------------------
// func_80471184: destroy all 16 g3d object slots (notifying the manager via
// the vtable+0x3C callback first) and then the manager object itself.
// ---------------------------------------------------------------------------
void func_80471184__Q23LOD9LODMemManFv(LOD::LODMemMan* self) {
    int i = 0;
    LODMemManLayout* l = (LODMemManLayout*)self;
    while (i < 16) {
        if (l->field_0xC8[i].field_0x0) {
            ((LODG3dMgrVt*)l->field_0x0)->vf0D(l->field_0xC8[i].field_0x0);
            l->field_0xC8[i].field_0x0->Destroy();
            l->field_0xC8[i].field_0x0 = 0;
        }
        i++;
    }
    if (l->field_0x0) {
        l->field_0x0->Destroy();
        l->field_0x0 = 0;
    }
}

// ---------------------------------------------------------------------------
// func_80471224: notify the g3d manager (vtable+0x3C) about every live object
// slot, then - when a scene is supplied - remove the manager object from the
// scene's root group (index 7).
// ---------------------------------------------------------------------------
void func_80471224__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CScn* scene) {
    int i = 0;
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x0) {
        while (i < 16) {
            if (l->field_0xC8[i].field_0x0) {
                ((LODG3dMgrVt*)l->field_0x0)->vf0D(l->field_0xC8[i].field_0x0);
            }
            i++;
        }
    }
    if (scene) {
        if (l->field_0x0) {
            ((nw4r::g3d::ScnGroup*)func_8048EC14(scene, 7))
                ->Remove((nw4r::g3d::ScnObj*)l->field_0x0);
        }
    }
}

void LOD::LODMemMan::func_804712E0() {}

void LOD::LODMemMan::func_8047130C() {}

void LOD::LODMemMan::func_8047133C() {}

void LOD::LODMemMan::func_80471450() {}

void LOD::LODMemMan::func_8047146C() {}

void LOD::LODMemMan::func_80471484() {}

void LOD::LODMemMan::func_8047163C() {}

#pragma push
#pragma auto_inline off
void LOD::LODMemMan::func_804716B8() {}
#pragma pop

void LOD::LODMemMan::func_80471718() {}

void LOD::LODMemMan::func_80471748() {}

void LOD::LODMemMan::func_80471780() {}

void LOD::LODMemMan::func_80471794() {}

void LOD::LODMemMan::func_804717FC() {}

void LOD::LODMemMan::func_80471834() {}

// ---------------------------------------------------------------------------
// func_80471938: reset the manager.  The element buffers are released back to
// the MemManager only while the global reset flag is clear (retail keeps a
// redundant inner null-check that MWCC emits as a doubled beq), the file
// handle is cancelled, and the reset-time name string is re-seeded into +0x34
// with its length recorded at +0x74.
// ---------------------------------------------------------------------------
void LOD::LODMemMan::func_80471938() {
    LODMemManLayout* l = (LODMemManLayout*)this;
    if (lbl_eu_80665830 == 0) {
        if (l->field_0x4) {
            if (l->field_0x4) {
                mtl::MemManager::deallocate(l->field_0x4);
                l->field_0x4 = 0;
            }
        }
    } else {
        l->field_0x4 = 0;
    }
    if (l->field_0x8) {
        if (l->field_0x8) {
            mtl::MemManager::deallocate(l->field_0x8);
            l->field_0x8 = 0;
        }
    }
    if (l->field_0x30) {
        CDeviceFile::cancel(l->field_0x30);
        l->field_0x30 = 0;
    }
    l->field_0xC = 0;
    l->field_0x20 = 0;
    const char* name = lbl_eu_80523D90;
    l->field_0x74 = strlen(name);
    strcpy(l->mStr_34, name);
}

void LOD::LODMemMan::func_804719FC() {}

void LOD::LODMemMan::func_80471A70() {}

void LOD::LODMemMan::func_80471ACC() {}

void LOD::LODMemMan::func_80471BC8() {}

void LOD::LODMemMan::func_80471BF4() {}

// retail: lwz r3,0xC(r3); b func_804716B8 -- delegates to the sub-manager stored at +0xC
void LOD::LODMemMan::func_80471CC4() {
    (*(LOD::LODMemMan**)((u8*)this + 0xC))->func_804716B8();
}

void LOD::LODMemMan::func_80471CCC() {}

void LOD::LODMemMan::func_80471CDC() {}

void LOD::LODMemMan::OnFileEvent() {}
