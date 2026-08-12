#pragma once

#include <types.h>

struct CfHikariItemRecord;

class CScn;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
// Global-scope data/function names are not mangled by MWCC; lbl_* keeps the
// exact retail label.
extern "C" CScn* lbl_eu_80663E14; // scene singleton (removeRenderCB receiver)
extern char lbl_eu_8053AE90[];    // CfHikariItemManager vtable (.data)
void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 heap);
u32 func_80061FFC();

namespace cf {
class CfHikariItemManager {
public:
    CfHikariItemManager();
    ~CfHikariItemManager();
    void cbRenderBefore();

    /* 0x00 */ u32 field_00;                        // manual vptr (lbl_eu_8053AE90)
    /* 0x04 */ ::CfHikariItemRecord* records[0x40]; // 0x40 record pointers
    /* 0x104 */ u8 unk104[0x1104 - 0x104];
    /* 0x1104 */ u8 unk1104[0x10];                  // zeroed by memset in reset/dtor
    /* 0x1114 */ u8 unk1114[0x1194 - 0x1114];
    /* 0x1194 */ u32 field_1194;                    // flag word (bit0 toggle, bit1 func_802B2A08)
    /* 0x1198 */ u32 field_1198;                    // active record count
    /* 0x119C */ u32 field_119C;
    /* 0x11A0 */ u32 field_11A0;
};
} // namespace cf

// ---------------------------------------------------------------------------
// Catalog-TU shared context for the Hikari (light) item manager functions.
// Struct layouts recovered from retail accesses; unknown fields keep
// field_0xNN names.
// ---------------------------------------------------------------------------

// sdata2 float constant loaded by func_802B37F4 (lfs with r0 zero-base).
// const so MWCC treats the load as a constant and schedules it early.
extern const float lbl_eu_80668EF8;
// sdata2 random-velocity scale (func_802B3568) - NON-const so MWCC reloads
// it per mtRand conversion like retail.
extern float lbl_eu_80668EEC;
// sdata2 draw-pass constants for cbRenderBefore (func_8049626C view axes).
extern const float lbl_eu_80668ED0; // 1.0f
extern const float lbl_eu_80668ED4; // 0.4f
extern const float lbl_eu_80668ED8; // -0.4f
extern const float lbl_eu_80668EDC; // 0.5f
extern const float lbl_eu_80668EE0; // -0.5f
extern const float lbl_eu_80668EE4; // 0.125f
extern const float lbl_eu_80668EE8; // -0.125f
extern u32 lbl_eu_80668ECC;         // 0xFFFFFFFF mat/amb color word
// sdata2 spawn/update constants for func_802B3810.  EFC/F00 are NON-const:
// they are used inside the 8-iteration spawn loop, so retail reloads them
// per call (lfs inside the loop body).
extern float lbl_eu_80668EFC;        // 0.1f
extern float lbl_eu_80668F00;        // 1.5f
extern const float lbl_eu_80668F04;  // 10.0f
extern const float lbl_eu_80668F08;  // 15.0f
extern const float lbl_eu_80668F0C;  // 2.0f
extern const float lbl_eu_80668F10;  // 2500.0f
extern const float lbl_eu_80668F14;  // 4225.0f
extern const float lbl_eu_80668F18;  // 0.75f
extern const float lbl_eu_80668F1C;  // 0.005f
extern const float lbl_eu_80668F20;  // 20.0f
extern const double lbl_eu_80668F28; // 20.0 (double compare)
extern const float lbl_eu_80668F30;  // 225.0f
extern const float lbl_eu_80668F34;  // 0.2f
extern const double lbl_eu_80668F38; // 4503601774854144.0 (int->float magic)
// sdata2 int->float conversion magic double (0x4330000080000000), used by
// func_802B3568's (f32)mtRand() conversions.
extern double lbl_eu_80668EF0;
// sdata2 gradient-table constants for sinit_802B46E0 (all loaded up-front
// with lfs and CSE'd into callee-saved FPRs - no calls in the function, so
// const scheduling is safe here).
extern const float lbl_eu_80668F70;
extern const float lbl_eu_80668F74;
extern const float lbl_eu_80668F78;
extern const float lbl_eu_80668F7C;
extern const float lbl_eu_80668F80;
extern const float lbl_eu_80668F84;
extern const float lbl_eu_80668F88;
extern const float lbl_eu_80668F8C;
extern const float lbl_eu_80668F90;
extern const float lbl_eu_80668F94;
extern const float lbl_eu_80668F98;
extern const float lbl_eu_80668F9C;
extern const float lbl_eu_80668FA0;
extern const float lbl_eu_80668FA4;
extern const float lbl_eu_80668FA8;
extern const float lbl_eu_80668FAC;
extern const float lbl_eu_80668FB0;
extern const float lbl_eu_80668FB4;
// sdata2 gradient-scale constants for func_802B44C8.  NON-const like
// lbl_eu_80668EC8: the loads must stay after the func_802B41E4 call.
extern float lbl_eu_80668F64;
extern float lbl_eu_80668F68;
extern float lbl_eu_80668F6C;

// .rodata gradient thresholds/blend weights for func_802B44C8 (count=2:
// table[0..1] thresholds, table[2..3] weights).  NON-const: a const load
// gets hoisted before the func_802B41E4 call into a callee-saved FPR;
// retail loads the table at its use sites after the call.
extern float lbl_eu_805135A8[4];
// .bss colour-gradient rows for func_802B44C8, indexed by the record's
// (s16)field_1C_h[1] * 8 floats.
extern f32 lbl_eu_80577750[0x40 / 4];
// sdata2 1.0f constant for func_802B41E4's gradient lerp (1.0f - blend).
extern const float lbl_eu_80668F54;
// sdata2 scale constants for func_802B4358's random velocity setup.
extern const float lbl_eu_80668F58;
extern const float lbl_eu_80668F5C;
// sdata2 limit for func_802B4470's +0x18 accumulator check.
// const so MWCC treats the load as a constant and schedules it early.
extern const float lbl_eu_80668F60;
// sdata2 LOD constant for the ctor's four GXInitTexObjLOD calls (all three
// LOD args share one load; MWCC fmr's the pair).  Deliberately NON-const:
// a const sdata2 load is loop-invariant and MWCC hoists it out of the tex
// loop into a callee-saved FPR; retail loads it at the use site instead.
extern float lbl_eu_80668EC8;

// .sbss manager singleton published by the ctor (retail stw + sda21).
extern cf::CfHikariItemManager* lbl_eu_80664C10;

// Current view / game-state flags (retail globals defined in CfGameManager).
class CView;
extern CView* lbl_eu_80663E10;
extern u32 lbl_eu_80663E24;

// Scene/camera helpers (retail C-linkage symbols).
extern "C" void* func_8049626C(void* scene, void* view);
extern "C" f32 func_80496288(void* scene);
// Manager singleton getter (retail func_802B262C, defined in CMenuGameClear).
extern "C" cf::CfHikariItemManager* func_802B262C();

// .rodata static-archive file path passed to CLibStaticData::getStaticFileData
// by the ctor (retail lis/addi, so a plain extern array - no sdata).
extern const char lbl_eu_80513580[];
// .data Panic strings (file + format) for the ctor's ResFile alignment check.
extern char lbl_eu_80526354[];
extern char lbl_eu_8052637C[];

// sbss counter block; the four words are zeroed together by func_802B371C,
// func_802B4460/4470 bump +0x24.  (unsigned long matches func_802B4460's
// local extern so the matched function stays untouched.)
extern unsigned long lbl_eu_80664C18;
extern unsigned long lbl_eu_80664C1C;
extern unsigned long lbl_eu_80664C20;
extern unsigned long lbl_eu_80664C24;

// .bss global state block.  func_802B371C copies the record's
// first three words into +0x00/+0x04/+0x08.  Declared as an array (like
// CfGimmick's lbl_eu_805765B0) so MWCC materializes a base register and
// merges the first store into stwu.  func_802B3CA0 reads 4-float color
// gradient rows at +0x10/+0x50/+0x90 indexed by field_40; sinit_802B46E0
// fills four 16-float rows at +0x10/+0x50/+0x90/+0xd0.
extern u32 lbl_eu_80577680[0xD0 / 4];

// sdata2/.data gradient keyframe tables for func_802B41E4 (2*count floats:
// count thresholds then count blend weights) - func_802B3CA0 passes these.
extern const float lbl_eu_80513588[];
extern const float lbl_eu_80513598[];

// Hikari item record.  +0x00..+0x14 hold two VEC3s (f32 views via cast in
// func_802B4470; +0x00..+0x08 are also copied as u32 words by func_802B371C).
// Four 3-float corner offsets (12 consecutive floats) passed to the GX FIFO
// quad emitters func_802B3E04 / func_802B3F20.
struct CfHikariQuadCorners {
    /* 0x00 */ f32 v[4][3]; // [corner][x/y/z]
};

struct CfHikariItemRecord {
    union {
        /* 0x00 */ u32 field_00; // vector A x (word view)
        /* 0x00 */ f32 field_00f; // vector A x (float view)
    };
    union {
        /* 0x04 */ u32 field_04; // vector A y (word view)
        /* 0x04 */ f32 field_04f; // vector A y (float view)
    };
    union {
        /* 0x08 */ u32 field_08; // vector A z (word view)
        /* 0x08 */ f32 field_08f; // vector A z (float view)
    };
    /* 0x0C */ f32 field_0C; // vector B x (gradient progress value)
    /* 0x10 */ f32 field_10; // vector B y (gradient progress value)
    /* 0x14 */ f32 field_14; // vector B z
    /* 0x18 */ f32 field_18; // accumulator (func_802B4470)
    union {
        /* 0x1C */ f32 field_1C; // float view (func_802B37F4)
        /* 0x1C */ u16 field_1C_h[2]; // halfword view (func_802B4358)
    };
    /* 0x20 */ f32 field_20;
    /* 0x24 */ f32 field_24;
    /* 0x28 */ f32 field_28; // sparkle offset (func_802B3810)
    /* 0x2C */ f32 field_2C; // sparkle offset (func_802B3810)
    // Four color words; the array view is used by func_802B3CA0's loop.
    union {
        struct {
            /* 0x30 */ u32 field_30; // color 0x96FFFFFF
            /* 0x34 */ u32 field_34; // color 0x20402064
            /* 0x38 */ u32 field_38; // color 0x20AAFF00
            /* 0x3C */ u32 field_3C; // color 0x20AAFF64
        };
        /* 0x30 */ u32 colors[4];
    };
    /* 0x40 */ u16 field_40;
    /* 0x42 */ u16 field_42; // flag word (0x40 set by func_802B37F4)
    /* 0x44 */ u8 field_44[0x1194 - 0x44];
    /* 0x1194 */ u32 field_1194; // flag word (bit0 toggle, bit1 func_802B2A08)
};

// Hikari item update / emitter functions (retail symbols).  extern "C" so the
// call relocs and definitions carry the exact retail names (MWCC mangles
// plain C++ global functions, which would drift the REL24 reloc names).
extern "C" void func_802B3CA0(CfHikariItemRecord* self);
extern "C" void func_802B3E04(CfHikariItemRecord* self, const CfHikariQuadCorners* corners);
extern "C" void func_802B3F20(CfHikariItemRecord* self, const CfHikariQuadCorners* corners);
extern "C" u32 func_802B41E4(f32* self, const f32* rows, const f32* table, int count);
extern "C" void func_802B4358(CfHikariItemRecord* self, const u32* src, u16 val, f32 scale);

// Initializes a freshly allocated 0x44-byte Hikari item record (us-802b61c0):
// zeroes the vector/accumulator floats, sets the color words and the u16 at
// +0x40.  Declared here so func_802B2894 (defined earlier in the TU) can call it.
extern "C" void func_802B3750(CfHikariItemRecord* self, u16 value);

// Record deleting-destructor helper (retail __dt__802B37B4, already matched in
// this TU).  Declared here for func_802B2938 / func_802B2A18 / the manager dtor.
extern "C" void* __dt__802B37B4(void* self, int flag);

// Per-frame update entry points (defined in this TU; extern "C" so
// cbRenderBefore's call relocs carry the exact retail names).
extern "C" void func_802B371C(CfHikariItemRecord* self);
extern "C" s32 func_802B3810(CfHikariItemRecord* self, f32 delta);
extern "C" void func_802B403C(CfHikariItemRecord* self, const CfHikariQuadCorners* corners);
extern "C" void func_802B4460(CfHikariItemRecord* self);
extern "C" s32 func_802B4470(CfHikariItemRecord* self, f32 delta);
extern "C" void func_802B44C8(CfHikariItemRecord* self, const CfHikariQuadCorners* corners);

// Minimal view of the frame object returned by func_8049626C (retail field
// accesses: 3x4 matrix at +0x9C, Hikari record block at +0x10C, projection at
// +0x194).  Used by cbRenderBefore for the inverse-matrix / projection setup.
// 3x4 matrix copy type: struct assignment makes MWCC emit the retail lwz/stw
// pair copy inline (memcpy would degrade to a `bl memcpy` call).
struct CfMtxCopy {
    /* 0x00 */ f32 m[3][4];
};
struct CfViewFrame {
    /* 0x00 */ u8 pad00[0x9C];
    /* 0x9C */ CfMtxCopy mtx;
    /* 0xCC */ u8 padCC[0x10C - 0xCC];
    /* 0x10C */ u8 rec[0x44];
    /* 0x150 */ u8 pad150[0x194 - 0x150];
    /* 0x194 */ f32 projection[4][4];
};

// Fog source object reached through CScn+0x78 (retail reads the fog id at +0x24).
struct CfFogInfo {
    /* 0x00 */ u8 pad00[0x24];
    /* 0x24 */ int fogId;
};

// CScn layout view exposing the +0x78 fog-source pointer (CScn.hpp hides the
// offset inside a padding byte region, so this TU-local view is used for the
// single fog probe in cbRenderBefore).
struct CfScnFogView {
    /* 0x00 */ u8 pad00[0x78];
    /* 0x78 */ CfFogInfo* fogInfo;
    /* 0x7C */ u8 pad7C[0x80 - 0x7C];
};
