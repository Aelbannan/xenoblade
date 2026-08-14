#pragma once

#include <types.h>
#include "monolib/scn/CLight.hpp"

// 0x30-byte light-slot entry in the array at CScnEnvLgtCtrl+0x24.
// func_804C5F6C / 5FB0 / 6010 poke the u16 word at +0x28 (control flags:
// bit 0 and bit 2 cleared/set there) and the three u16 params at +0x2A..+0x2E.
// 12-byte vec3 used by the light-slot entries; copied wholesale by
// func_804C5E04 (word moves - retail treats the triple as u32 words).
struct CScnEnvLgtCtrlLgtVec3 {
    u32 x;  // +0x00
    u32 y;  // +0x04
    u32 z;  // +0x08
};

struct CScnEnvLgtCtrlLgtSlot {
    CScnEnvLgtCtrlLgtVec3 field_0x00;   // +0x00
    u8 field_0x0C[0x0C];                // +0x0C
    CScnEnvLgtCtrlLgtVec3 field_0x18;   // +0x18
    u8 field_0x24[4];                   // +0x24
    u16 field_0x28;                     // +0x28 control flags
    u16 field_0x2A;                     // +0x2a
    u16 field_0x2C;                     // +0x2c
    u16 field_0x2E;                     // +0x2e
};

// Virtual-dispatch view of the light-ring items (the objects linked at
// CScnEnvLgtCtrl+0x0C). MWCC reserves vtable slots 0-1 for RTTI, so declared
// virtual N sits at slot N+2 (offset (N+2)*4): the dummies below pin slot 10
// (0x28, func_804C1600) and slot 28 (0x70, func_804C2014). Abstract on
// purpose - never instantiated in this TU.
struct CScnEnvLgtCtrlListItem {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2(void* arg) = 0;            // vtable slot 4 (0x10)
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8(void* arg) = 0;            // vtable slot 10 (0x28)
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11(void* arg, u8 byte) = 0;  // vtable slot 13 (0x34)
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16(void* arg) = 0;           // vtable slot 18 (0x48)
    virtual void v17() = 0;
    virtual void v18(void* arg, float f) = 0;  // vtable slot 20 (0x50)
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual int v23() = 0;                     // vtable slot 25 (0x64)
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26(void* a, void* b) = 0;    // vtable slot 28 (0x70)
    virtual void v27(void* a, void* b, float f) = 0;  // vtable slot 29 (0x74)
};

// Ring-list node for the light-object list cached at CScnEnvLgtCtrl+0x0C
// (mirrors _reslist_node<T>: links at +0/+0x4, item pointer at +0x8).
struct CScnEnvLgtCtrlListNode {
    CScnEnvLgtCtrlListNode* mNext;   // +0x00
    CScnEnvLgtCtrlListNode* mPrev;   // +0x04
    CScnEnvLgtCtrlListItem* mItem;   // +0x08
};

// Scene environment light controller.
// vtable pointer at +0x00 (the retail dtor installs it), followed by the
// light-state arrays: field_0x38/0x58 hold the previous frame and
// field_0x78/0x98 the current frame (func_804C7530 copies previous -> current).
// +0x24 points at the light-slot array (func_804C5F6C/5FB0/6010) and +0x28 at
// a bit array (func_804C123C / func_804C5C08).
class CScnEnvLgtCtrl {
public:
    CScnEnvLgtCtrl();
    ~CScnEnvLgtCtrl();

    // +0x00: vtable pointer (dtor installs lbl_eu_8056F9B8); also used as a
    // flags word by func_804C12A4 / func_804C1A64.
    union {
        void* mVtable;                      // +0x00 vtable pointer
        u32 flags;                          // +0x00 flags view
    };
    // +0x04: control flags (0x800/0x1000 light bits, func_804C5DA0) and the
    // scene-view pointer handed to func_80496288 / func_8048ECD8.
    union {
        u32 field_0x04;                     // +0x04 control flags
        void* field_0x04_ptr;               // +0x04 view/owner pointer
    };
    void* field_0x08;                       // +0x08 owned buffer (dtor deallocates)
    CScnEnvLgtCtrlListNode* field_0x0C;     // +0x0C light-list ring sentinel
    u16 field_0x10;                         // +0x10
    u16 field_0x12;                         // +0x12 light count
    u8 field_0x14[8];                       // +0x14
    f32 field_0x1C;                         // +0x1C (value sampled by func_804C7880)
    u8 field_0x20[4];                       // +0x20
    CScnEnvLgtCtrlLgtSlot* field_0x24;      // +0x24 light-slot array (0x30-byte entries)
    u32* field_0x28;                        // +0x28 light bit-array (>= 10 words)
    u8 field_0x2C[4];                       // +0x2C
    f32* field_0x30;                        // +0x30 pointer to a light value (func_804C3404 derefs it)
    u8 field_0x34[4];                       // +0x34
    // +0x38 .. +0xB8: 4 rows of 8 floats at 0x38/0x58/0x78/0x98
    // (func_804C7530 copies rows 2,3 <- rows 0,1). +0x3C aliases row 0's
    // second word (func_804C1600 stores a light-data pointer there).
    union {
        f32 field_0x38[0x20];               // +0x38
        struct {
            u8 pad_0x38[0x04];              // +0x38
            void* field_0x3C;               // +0x3C
        } alt;
    };
    u8 field_0xB8[0x18];                    // +0xB8 .. +0xD0
    u32 field_0xD0;                         // +0xD0 ambient color triple (func_804C3404)
    u32 field_0xD4;                         // +0xD4
    u32 field_0xD8;                         // +0xD8
    u8 field_0xDC[8];                       // +0xDC .. +0xE4
    u32 field_0xE4;                         // +0xE4 (light slot field, func_804C3404)
    u8 field_0xE8[4];                       // +0xE8
    f32 field_0xEC;                         // +0xEC (light value, func_804C3404)
};

// Retail vtable data blob installed at CScnEnvLgtCtrl+0x00 by the dtor.
extern u8 lbl_eu_8056F9B8[];

// View of the CLightEnv light-bank layout read/written by func_804C3404:
// per-slot CLight pointers (+0x1140) and the parallel u32 slot fields
// (+0x1160) refreshed by func_804C64A8. Mirrors CLightEnv in
// CVirtualLightObj.hpp.
struct CScnEnvLgtCtrlLightEnvView {
    u8 _00[0x1140];          // +0x00
    CLight* mSlotPtrs[4];    // +0x1140
    u8 _1150[0x10];          // +0x1150 (mSlotCounts)
    u32 mSlotFields[4];      // +0x1160
};

// Camera-item lookup (defined in CScnCameraMan.cpp under the retail flat
// name func_8049B158). CScn.hpp only declares a 1-arg stub; the retail call
// site in func_804C2654 passes the (camera-work blob, id) pair, so the full
// signature is declared here. Global scope keeps the name unmangled (cf.
// CScnVirtualLight.hpp).
struct UnkScn68;      // CScn.hpp (camera-work blob at CScn+0x68)
struct CScnCameraItem;  // CScnCameraMan.hpp
CScnCameraItem* func_8049B158(UnkScn68* cam, s32 id);

// Light-slot refresh (same-TU function, retail flat name): recomputes one
// CLightEnv slot field. Signature from the func_804C3404 call site
// (self, light, slot index, reset flag) -> new slot field value. extern "C"
// keeps the flat retail name (typed-param globals get C++-mangled); noinline
// keeps the call out-of-line (the stub body would otherwise be inlined).
extern "C" __declspec(noinline) u32 func_804C64A8(CScnEnvLgtCtrl* self,
                                                   CLight* light, u32 slot,
                                                   u32 reset);

// 8-byte gradient entry: time bound + color, sampled by func_804C7880.
struct CScnEnvLgtCtrlGradEntry {
    f32 time;   // +0x00
    f32 color;  // +0x04
};

// Time/gradient lookup table read by func_804C7880: min/max bounds, an
// 8-byte-strided (time,color) float-pair array base, and the pair count.
struct CScnEnvLgtCtrlGrad {
    f32 field_0x00;    // +0x00 min bound
    f32 field_0x04;    // +0x04 max bound
    u8 field_0x08[2];  // +0x08
    u16 field_0x0A;    // +0x0A entry count
    u8 field_0x0C[4];  // +0x0C
    u32 field_0x10;    // +0x10 array base offset (added to CScnEnvLgtCtrl+0x00)
};

// Shared .sdata2 float constant returned when no gradient entry matches.
extern float lbl_eu_8066B05C;

// Panic assert strings referenced by func_804C1A64 (retail .data symbols).
extern char lbl_eu_8056EC00[];
extern char lbl_eu_8056EBE0[];

// CScnEnvLgtCtrl constructor (retail symbol is the unmangled short form).
// Builds the controller from an 'STGL' resource header plus an opaque arg.
// noinline matches the retail out-of-line call from func_804C6A70.
extern "C" __declspec(noinline) CScnEnvLgtCtrl* __ct__CScnEnvLgtCtrl(
    CScnEnvLgtCtrl* self, const u32* data, void* arg);

// 16-byte light-data block copied by func_804C5198 (self+0x54 -> out).
// Kept as a named struct so the copy sites use typed pointers: MWCC only
// emits grouped load-all/store-all for typed sources (a void* param forces
// interleaved r0-reuse pairs, cf. MWCC_REFERENCE 3-word struct copy).
struct CScnEnvLgtCtrlLgtData {
    u32 field_0x00;   // +0x00
    u32 field_0x04;   // +0x04
    u32 field_0x08;   // +0x08
    u32 field_0x0C;   // +0x0c
};

// Typed view over the CScnEnvLgtCtrl layout used by func_804C5198 (u32 copy
// of the +0x54 block) and func_804C5210 (float copy of +0x54 or +0x64 into
// CScnEnvLgtData::mAmbColorBase, selected by flags bits 0x200 / 0x400).
struct CScnEnvLgtCtrlLgtView {
    u8 _pad[0x04];                     // +0x00
    u32 flags;                         // +0x04 (bit 0x200 = light data at 0x54; bit 0x400 = data at 0x64)
    u8 _pad2[0x4C];                    // +0x08
    union {
        CScnEnvLgtCtrlLgtData data;    // +0x54 (u32 view, func_804C5198)
        f32 field_0x54[4];             // +0x54 (float view, func_804C5210)
    };
    f32 field_0x64;                    // +0x64
    f32 field_0x68;                    // +0x68
    f32 field_0x6C;                    // +0x6c
};
