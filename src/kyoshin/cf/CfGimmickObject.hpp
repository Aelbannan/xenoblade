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
#include "kyoshin/cf/CtrlMovePC.hpp"  // CCtrlMovePC (embedded at CfGimmickMoveSub +0x8C)
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

// 16-byte per-step entry at +0xA4 (indexed by field_188 in func_801F6780).
// The +0x05/+0x06/+0x0C bytes double as the CfGimmickLodFrame layout handed
// to func_801F6B98 (retail passes the step pointer through unchanged).
struct CfGimmickObjectStep {
    /* 0x00 */ u16 field_00;   // activation frames (seeds field_170)
    /* 0x02 */ u16 field_02;   // camera-event id passed to func_8007B0C8
    /* 0x04 */ u8 field_04;    // map-object status id (func_800BE12C)
    /* 0x05 */ u8 field_05;    // LOD flags (CfGimmickLodFrame alias)
    /* 0x06 */ u8 field_06;    // LOD id byte (CfGimmickLodFrame alias)
    /* 0x07 */ u8 field_07;    // area-manager id (func_800817BC)
    /* 0x08 */ u16 field_08;   // effect/flag bitmask
    /* 0x0A */ u16 field_0A;   // sound id (func_80208C60 / func_80208C48)
    /* 0x0C */ u8 gap_0C[2];   // frame count (CfGimmickLodFrame alias)
    /* 0x0E */ u8 field_0E;    // sound flag byte
    /* 0x0F */ u8 gap_0F;
};

// CfGameManager area object (func_800817BC result) stored in field_78. Only
// vtable slots 0x9C / 0xC4 and the +0xB0 back-pointer are used by this TU.
struct CfGimmickObjectMgr {
    void** vtable;              // +0x00
    u8 gap_B0[0xB0 - 0x04];
    void* field_B0;             // +0xB0: owning gimmick
};

// Sound-slot entry returned by func_801BFAE4; +0x1C holds the volume scale.
struct CfGimmickSoundSlot {
    u8 gap[0x1C];
    f32 field_1C;   // +0x1C
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
    /* 0x78 */ CfGimmickObjectMgr* field_78;  // area manager (func_800817BC)
    /* 0x7C */ CfGimmickReg field_7C;  // gimmick-object registration slot
    /* 0x80 */ u16 field_80;
    /* 0x82 */ u8 gap_82[0x84 - 0x82];
    /* 0x84 */ CfGimmickObjectArea field_84[2];  // 2 x 0x10-byte gimmick-area tables
    /* 0xA4 */ CfGimmickObjectStep field_A4[2];  // 2 x 0x10 per-step entries
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
    /* 0x15A */ u16 field_15A;
    /* 0x15C */ u16 field_15C;         // countdown seed (func_801F879C)
    /* 0x15E */ u8 field_15E;          // mode byte (1 = active) checked by func_801F8BB8
    /* 0x15F */ u8 field_15F;
    /* 0x160 */ u8 field_160;
    /* 0x161 */ u8 field_161;          // flag byte (bit 7) tested by func_801F8BB8
    /* 0x162 */ u8 field_162;
    /* 0x163 */ u8 field_163;          // LOD refresh id (func_801F879C)
    /* 0x164 */ u8 field_164;          // mode byte (1 = active) checked by func_801F8BB8
    /* 0x165 */ u8 field_165;
    /* 0x166 */ u8 gap_166[0x168 - 0x166];
    /* 0x168 */ CfGimmickObject* field_168;  // linked peer object (busy check func_801F634C)
    /* 0x16C */ f32 field_16C;         // LOD fade countdown timer (func_801F634C / func_801F6E60)
    /* 0x170 */ f32 field_170;         // countdown timer (func_801F75CC)
    /* 0x174 */ f32 field_174;
    /* 0x178 */ f32 field_178;         // LOD timer diff (func_801F627C)
    /* 0x17C */ f32 field_17C;
    /* 0x180 */ f32 field_180;         // effect timer (func_801F76A8 / func_801F6780)
    /* 0x184 */ u32 field_184;         // player bitmask (func_801F72A4)
    /* 0x188 */ s16 field_188;         // step counter (clamped to 0..6)
    /* 0x18A */ s16 field_18A;         // LOD fade frames (func_801F634C / func_801F6E60)
    /* 0x18C */ s16 field_18C;         // +0x14A table scan index (func_801F72A4)
    /* 0x18E */ s16 field_18E;         // countdown frames (func_801F89B8)
    /* 0x190 */ u16 field_190;         // sound id (func_801F6780 / func_801F76A8)
    /* 0x192 */ u16 field_192;         // sound kind selector
    /* 0x194 */ u16 field_194;
    /* 0x196 */ s16 field_196;         // remaining target count (func_801F72A4)
    /* 0x198 */ s16 field_198;
    /* 0x19A */ u8 gap_19A[0x60C - 0x19A];
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
void func_801F61B0(cf::CfGimmickObject* self, int mode);
void func_801F6E60(cf::CfGimmickObject* self, u8 arg);
void func_801F5C2C(cf::CfGimmickObject* self, int a, int b);
void func_801F6B98(cf::CfGimmickObject* self, u8 lod,
                   const CfGimmickLodFrame* frame);
// Same-TU per-frame updates (raw retail names).
int func_801F879C(cf::CfGimmickObject* self);
int func_801F89B8(cf::CfGimmickObject* self);
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
void func_80462E3C__8CTaskLODFv(u8 lod, f32 f);
void func_80462F10__8CTaskLODFv(u8 lod);
void func_80462F4C__8CTaskLODFv(u8 lod, int mode);
void func_80462F70__8CTaskLODFv(u8 lod, int mode);
void func_80462ED0__8CTaskLODFv(u8 lod, int mode);
void func_80462F94__8CTaskLODFv(u8 lod, u16 id);
int* func_8009ECB0();
int func_8009E284(int* data, int id);
void func_8020A010();
void func_80209F2C();
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
void func_80193678(int id);
// Step-table / sound helpers (func_801F6780 / func_801F76A8).
void func_80140E00(u32 a, u32 b, u32 c);
void func_8015B25C(u16 id);
void func_8020A6B0(void* reg, const CfGimmickVec3* point, u16 c, f32 d,
                  int e, int g);
void func_800ACC64(void* obj, const void* src);
int func_801BFABC(int a);
u16 func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(u32 a, u32 b, u32 c,
                                                u32 d, f32 f);
u16 func_80208C60(u16 id, f32* pos, f32 d);
void func_801BFF78(int a, u16 b, int c);
CfGimmickSoundSlot* func_801BFAE4(u16 handle);
int func_80195B04(int id);
void func_8007B0C8(int idx);
void func_8020A0F8();
CfGimmickObjectMgr* func_800817BC__Q22cf13CfGameManagerFv(u32 id, u32 mode);
void* getPlayer__Q22cf13CfGameManagerFi(int index);
void func_800BE12C(u8* obj, int a, int b, int c, int d);
}

// ---------------------------------------------------------------------------
// Small-data globals read by this TU (retail @sda21 accesses; global-scope
// variables keep their names without `extern "C"`).
// ---------------------------------------------------------------------------
extern void* lbl_eu_80663E14;   // .sbss scene/handle pointer
// .sdata2 countdown constants (func_801F75CC).
extern f32 lbl_eu_806681A0;
extern f32 lbl_eu_806681B8;
extern f32 lbl_eu_806681B0;   // 20.0f heal/map-object value (func_801F6E60)
// .sdata2 s16->f32 magic double (2^52 + 2^31; func_801F634C / func_801F6E60).
extern f64 lbl_eu_806681A8;
// .sdata2 LOD constants (func_801F6B98).
extern f32 lbl_eu_806681A4;
extern f32 lbl_eu_806681B4;
// .sdata2 2^52 magic double for the u16/u32 -> f32 conversions
// (func_801F8658 / func_801F6B98; CfTFile.cpp convention).
extern f64 lbl_eu_806681C0;
// .sdata2 sound-distance / spawn constants (func_801F6780 / func_801F76A8).
extern f32 lbl_eu_806681BC;
extern f32 lbl_eu_806681C8;

// ---------------------------------------------------------------------------
// Cast-only helper layouts (CfGimmickObject.cpp func_801F6780 / func_801F72A4).
// ---------------------------------------------------------------------------

// Cast-only view of CfObjectMove (getPlayer result): vtable slot 0x110
// returns the move sub-object.
struct CfGimmickObjectMoveIf {
    void** vtable;   // +0x00
};

// Sub-object returned by CfGimmickObjectMoveIf slot 0x110; a CCtrlMovePC is
// embedded at +0x8C (address passed to func_80199678).
struct CfGimmickMoveSub {
    u8 gap_8C[0x8C];
    cf::CCtrlMovePC ctrl;   // +0x8C
};

// Base of a CfObjectMove player (func_800B6BC8 nodes point at base+0x3E9C);
// +0x456C holds the u16 id/bit byte scanned by func_801F72A4.
struct CfGimmickPlayerBase {
    void** vtable;           // +0x00
    u8 gap_456C[0x456C - 0x04];
    u16 field_456C;          // +0x456C
};

// Circular object list returned by func_800B6BC8 (mirror of
// CfGimmickList/CfGimmickListNode in CfGimmick.hpp with a typed node).
struct CfGimmickObjectListNode {
    CfGimmickObjectListNode* next;  // +0x00
    u8 gap_04[4];
    void* object;                   // +0x08
};
struct CfGimmickObjectList {
    void* field_00;                 // +0x00
    CfGimmickObjectListNode* head;  // +0x04
};
