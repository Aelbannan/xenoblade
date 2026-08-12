#pragma once

/**
 * @file CfGimmickObject.hpp
 * @brief cf::CfGimmickObject -- script/OC gimmick object class.
 *
 * CfGimmickObject is a gimmick entity managed by the cf script VM.
 *
 * Like the sibling gimmick classes (CfGimmickItem / CfGimmickElv) the class
 * carries a MANUAL vtable pointer at +0x00 (no C++ inheritance -- the shared
 * CfGimmick base layout is replicated inline) so the retail vtable
 * `lbl_eu_80534F70` is stored verbatim instead of a compiler-generated one;
 * the base CfGimmick destructor is invoked by name.
 */

#include "kyoshin/cf/CfGimmick.hpp"
#include "types.h"

// Per-area table entry at +0x84 (2 entries x 0x10 bytes). Each entry holds
// u16 sequence windows / ids scanned by the per-frame eligibility checks
// (func_801F7978 / func_801F7B44 / func_801F7D38).
struct CfGimmickObjectArea {
    /* 0x00 */ u16 field_00;
    /* 0x02 */ u16 field_02;
    /* 0x04 */ u16 field_04;
    /* 0x06 */ u16 field_06;
    /* 0x08 */ u16 field_08;
    /* 0x0A */ u16 field_0A;
    /* 0x0C */ u16 field_0C;
    /* 0x0E */ u16 field_0E;
};

// Third argument of func_801F6B98: per-frame LOD state record. Only the flag
// byte (+0x05), id byte (+0x06) and frame count (+0x0C) are read.
struct CfGimmickLodFrame {
    /* 0x00 */ u8 gap_00[0x05];
    /* 0x05 */ u8 field_05;   // LOD control flags (bits 0..5 dispatched)
    /* 0x06 */ u8 field_06;   // id byte stored to +0x194
    /* 0x07 */ u8 gap_07[0x0C - 0x07];
    /* 0x0C */ u16 field_0C;  // frame count converted to f32 for func_80462FD8
};

namespace cf {

class CfGimmickObject {
public:
    ~CfGimmickObject();

    /* 0x00 */ void* vtable;           // manual vptr (lbl_eu_80534F70)
    /* 0x04 */ f32 field_04[3];        // position basis handed to func_802089BC
    /* 0x10 */ CfGimmickVec3 field_10; // reference point for the colliders
    /* 0x1C */ u8 field_1C[0x30];      // collider A 3x4 matrix dest (func_802089BC)
    /* 0x4C */ u8 gap_4C[0x64 - 0x4C];
    /* 0x64 */ u16 field_64;
    /* 0x66 */ u16 field_66;
    /* 0x68 */ u16 field_68;           // map-object id passed to func_80186BC8
    /* 0x6A */ u16 field_6A;
    /* 0x6C */ u16 field_6C;
    /* 0x6E */ u16 field_6E;
    /* 0x70 */ u8 field_70[4];         // per-LOD CTaskLOD ids (loop uses [0]/[1])
    /* 0x74 */ u32 field_74;           // state flag word
    /* 0x78 */ u32 field_78;
    /* 0x7C */ CfGimmickReg field_7C;  // gimmick-object registration slot
    /* 0x80 */ u16 field_80;
    /* 0x82 */ u8 gap_82[0x84 - 0x82];
    /* 0x84 */ CfGimmickObjectArea field_84[2];  // 2 x 0x10-byte gimmick-area tables
    /* 0xA4 */ u8 gap_A4[0xC4 - 0xA4];
    /* 0xC4 */ u16 field_C4;
    /* 0xC6 */ u8 gap_C6[0xF4 - 0xC6];
    /* 0xF4 */ u8 field_F4[0x30];      // collider B 3x4 matrix dest (func_802089BC)
    /* 0x124 */ u8 gap_124[0x13C - 0x124];
    /* 0x13C */ u16 field_13C;
    /* 0x13E */ u16 field_13E;
    /* 0x140 */ u16 field_140;
    /* 0x142 */ u16 field_142;
    /* 0x144 */ u16 field_144[3];      // u16 id table fired by func_801F8BB8
    /* 0x14A */ u16 field_14A[4];      // u16 table scanned by func_801F72A4
    /* 0x152 */ u16 field_152;         // party-member flag word (func_801F6D8C)
    /* 0x154 */ u8 gap_154[0x156 - 0x154];
    /* 0x156 */ u16 field_156;
    /* 0x158 */ u16 field_158;         // gimmick id passed to func_80208C48
    /* 0x15A */ u8 gap_15A[0x15E - 0x15A];
    /* 0x15E */ u8 field_15E;          // mode byte (1 = active) checked by func_801F8BB8
    /* 0x15F */ u8 field_15F;
    /* 0x160 */ u8 field_160;
    /* 0x161 */ u8 field_161;          // flag byte (bit 7) tested by func_801F8BB8
    /* 0x162 */ u8 field_162;
    /* 0x163 */ u8 gap_163;
    /* 0x164 */ u8 field_164;          // mode byte (1 = active) checked by func_801F8BB8
    /* 0x165 */ u8 gap_165[0x170 - 0x165];
    /* 0x170 */ f32 field_170;         // countdown timer (func_801F75CC)
    /* 0x174 */ f32 field_174;
    /* 0x178 */ f32 field_178;         // LOD timer diff (func_801F627C)
    /* 0x17C */ f32 field_17C;
    /* 0x180 */ u8 gap_180[0x188 - 0x180];
    /* 0x188 */ s16 field_188;         // step counter (clamped to 0..6)
    /* 0x18A */ u8 gap_18A[0x194 - 0x18A];
    /* 0x194 */ u16 field_194;
    /* 0x196 */ u8 gap_196[0x60C - 0x196];
    /* 0x60C */ u8 field_60C[0x20];    // chain/attach data (func_804B1DC0)
};

} // namespace cf

// Retail vtable for cf::CfGimmickObject (stored at +0x00 by the dtor).
extern u8 lbl_eu_80534F70[];

// PMF dispatch tables (6 x 12-byte member pointers) indexed by field_188 in
// func_801F5B00; MWCC lowers (self->*table[idx])() to mulli/add + `bl
// __ptmf_scall` against the retail __ptmf_scall helper.
typedef int (cf::CfGimmickObject::*CfGimmickObjectPMF)();
extern CfGimmickObjectPMF lbl_eu_80534E70[6];
extern CfGimmickObjectPMF lbl_eu_80534EB8[6];

// Global flag word tested by func_801F856C (bit 5 = object busy).
extern u32 lbl_eu_80663E28;

// ---------------------------------------------------------------------------
// C-linkage imports (unmangled retail symbols used by this TU).  These are
// CfGimmick-family helpers / engine calls defined in other units; they have
// C linkage in retail so extern "C" is required to emit the raw name.
// ---------------------------------------------------------------------------
extern "C" {
void func_802089BC(void* matrix, const f32* basis, const CfGimmickVec3* point);
void func_80208EE4(void* self);
void func_8020A434(void* self);
void __dt__Q22cf9CfGimmickFv(void* self, int mode);
void func_80462DB4__8CTaskLODFv(u8 lod, int mode);
void* func_804BC9EC__Fv(void);
void func_804BCC30(void* snd, u8 id);
void func_804BCC3C(void* snd, u8 id);
void* func_80186BC8(int id);
void func_804B1DC0(void* self, int mode);
// Same-TU call targets (scaffold stubs; signatures match the stubs in the .cpp).
int func_801F72A4(cf::CfGimmickObject* self, u16* table);
void func_801F6780(cf::CfGimmickObject* self);
int func_801F634C(cf::CfGimmickObject* self);
void func_801F76A8(cf::CfGimmickObject* self);
void func_801F6E60(cf::CfGimmickObject* self, u8 arg);
void func_801F5C2C(cf::CfGimmickObject* self, int a, int b);
// Same-TU matched functions (raw retail names).
int func_801F75CC(cf::CfGimmickObject* self);
int func_801F6D8C(cf::CfGimmickObject* self);
void func_801F627C(cf::CfGimmickObject* self, u8 lod, int mode);
// Cross-TU imports (retail names - C linkage keeps the raw names unmangled;
// MWCC would otherwise append __F<argtypes> to global functions).
f32 func_80462F2C__8CTaskLODFv(u8 lod);
f32 func_80462FF4__8CTaskLODFv(u8 lod);
void func_80462FD8__8CTaskLODFv(u8 lod, f32 f);
void func_80462EF4__8CTaskLODFv(u8 lod, f32 f);
void func_80462F10__8CTaskLODFv(u8 lod);
void func_80462F4C__8CTaskLODFv(u8 lod, int mode);
void func_80462F70__8CTaskLODFv(u8 lod, int mode);
void func_80462ED0__8CTaskLODFv(u8 lod, int mode);
void func_80462F94__8CTaskLODFv(u8 lod, u16 id);
int* func_8009ECB0();
int func_8009E284(int* data, int id);
void func_8020A010();
void func_8020A0CC();
void func_80209F5C();
void func_8020A068(int arg0, int flag, u32 value);
unsigned int func_8020A5DC();
int func_8020A87C(void* self, u32 arg);
void func_8020A484(u16 id);
int func_8020971C(void* obj);
int func_802096EC(void* obj);
int func_8006A33C();
int func_802098EC(u32 mask, cf::CfGimmick* gimmick, const CfGimmickVec3* point,
                  const f32* ang, void* partyId);
void func_80159C04(unsigned int a, int b);
void func_8020974C(unsigned int a, int b);
unsigned int func_801587E8(unsigned short id);
u32 func_800822F4__Q22cf13CfGameManagerFv();
u32 func_80082354__Q22cf13CfGameManagerFv(u32 resourceId);
f32 func_80496288(void* ptr);
u16 func_80208C48(u16 id, f32* vec);
void func_80193678(u16 id);
}

// ---------------------------------------------------------------------------
// Small-data globals read by this TU (retail @sda21 accesses; global-scope
// variables keep their names without `extern "C"`).
// ---------------------------------------------------------------------------
extern void* lbl_eu_80663E14;   // .sbss scene/handle pointer
// .sdata2 countdown constants (func_801F75CC).
extern f32 lbl_eu_806681A0;
extern f32 lbl_eu_806681B8;
// .sdata2 LOD constants (func_801F6B98).
extern f32 lbl_eu_806681A4;
extern f32 lbl_eu_806681B4;
