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
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// View of the singleton returned by func_8003AA34 whose members are the
// per-gimmick-type bdat table pointers; the ctor loads the object-gimmick
// table pointer through an sda21 member reloc.
struct CfGimmickTableSet {
    u8* lbl_eu_80664128;   // object-gimmick bdat table
};

// NOTE: CtrlMovePC.hpp deliberately NOT included here -- its extern "C"
// getUnk80664658() declaration conflicts with CfGimmick.hpp's. Only
// cf::CCtrlMovePC / func_80199678 were needed, declared below.
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
    /* 0x07 */ u8 field_07;    // area-manager id (createBattleActor)
    /* 0x08 */ u16 field_08;   // effect/flag bitmask
    /* 0x0A */ u16 field_0A;   // sound id (func_80208C60 / func_80208C48)
    /* 0x0C */ u16 field_0C;   // frame count (CfGimmickLodFrame alias)
    /* 0x0E */ u8 field_0E;    // sound flag byte
    /* 0x0F */ u8 gap_0F;
};

// CfGameManager area object (createBattleActor result) stored in field_78. Only
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

// Abstract view of the retail CfGimmickObject vtable: calling slot 0x158
// through a real polymorphic type makes MWCC emit its canonical virtual-call
// sequence (lwz r12,0(r3); lwz r12,off(r12); mtctr r12) instead of the
// generic function-pointer chain. Pure virtuals only -- no vtable is emitted
// for this class.
class ICfGimmickObjectVt {
public:
    virtual void vt00() = 0;
    virtual void vt01() = 0;
    virtual void vt02() = 0;
    virtual void vt03() = 0;
    virtual void vt04() = 0;
    virtual void vt05() = 0;
    virtual void vt06() = 0;
    virtual void vt07() = 0;
    virtual void vt08() = 0;
    virtual void vt09() = 0;
    virtual void vt0A() = 0;
    virtual void vt0B() = 0;
    virtual void vt0C() = 0;
    virtual void vt0D() = 0;
    virtual void vt0E() = 0;
    virtual void vt0F() = 0;
    virtual void vt10() = 0;
    virtual void vt11() = 0;
    virtual void vt12() = 0;
    virtual void vt13() = 0;
    virtual void vt14() = 0;
    virtual void vt15() = 0;
    virtual void vt16() = 0;
    virtual void vt17() = 0;
    virtual void vt18() = 0;
    virtual void vt19() = 0;
    virtual void vt1A() = 0;
    virtual void vt1B() = 0;
    virtual void vt1C() = 0;
    virtual void vt1D() = 0;
    virtual void vt1E() = 0;
    virtual void vt1F() = 0;
    virtual void vt20() = 0;
    virtual void vt21() = 0;
    virtual void vt22() = 0;
    virtual void vt23() = 0;
    virtual void vt24() = 0;
    virtual void vt25() = 0;
    virtual void vt26() = 0;
    virtual void vt27() = 0;
    virtual void vt28() = 0;
    virtual void vt29() = 0;
    virtual void vt2A() = 0;
    virtual void vt2B() = 0;
    virtual void vt2C() = 0;
    virtual void vt2D() = 0;
    virtual void vt2E() = 0;
    virtual void vt2F() = 0;
    virtual void vt30() = 0;
    virtual void vt31() = 0;
    virtual void vt32() = 0;
    virtual void vt33() = 0;
    virtual void vt34() = 0;
    virtual void vt35() = 0;
    virtual void vt36() = 0;
    virtual void vt37() = 0;
    virtual void vt38() = 0;
    virtual void vt39() = 0;
    virtual void vt3A() = 0;
    virtual void vt3B() = 0;
    virtual void vt3C() = 0;
    virtual void vt3D() = 0;
    virtual void vt3E() = 0;
    virtual void vt3F() = 0;
    virtual void vt40() = 0;
    virtual void vt41() = 0;
    virtual void vt42() = 0;
    virtual void vt43() = 0;
    virtual void vt44() = 0;
    virtual void vt45() = 0;
    virtual void vt46() = 0;
    virtual void vt47() = 0;
    virtual void vt48() = 0;
    virtual void vt49() = 0;
    virtual void vt4A() = 0;
    virtual void vt4B() = 0;
    virtual void vt4C() = 0;
    virtual void vt4D() = 0;
    virtual void vt4E() = 0;
    virtual void vt4F() = 0;
    virtual void vt50() = 0;
    virtual void vt51() = 0;
    virtual void vt52() = 0;
    virtual void vt53() = 0;
    // NOTE: MWCC reserves two extra vtable slots for the class's implicit
    // destructor pair before the first declared virtual below.
    // slot 0x158 / 4 = 86: set-mode entry used by func_801F61B0.
    virtual void setMode(int mode) = 0;
    // Fillers up to slot 0x168 / 4 = 90: the map-object value setter invoked
    // on func_80186BC8 results by func_801F6E60.
    virtual void unk5B() = 0;
    virtual void unk5C() = 0;
    virtual void unk5D() = 0;
    virtual void setMapObjValue(f32 value) = 0;
};

namespace cf {

class CfGimmickObject {
public:
    ~CfGimmickObject();
    CfGimmickObject(s32 row, CfGimmickObject** tail, int count,
                    u32* flagWords);

    /* 0x00 */ void* vtable;           // manual vptr (lbl_eu_80534F70)
    /* 0x04 */ f32 field_04[3];        // position basis handed to func_802089BC
    /* 0x10 */ CfGimmickVec3 field_10; // reference point for the colliders
    /* 0x1C */ u8 field_1C[0x30];      // collider A 3x4 matrix dest (func_802089BC)
    /* 0x4C */ u8 gap_4C[0x60 - 0x4C];
    /* 0x60 */ u32 field_60;
    /* 0x64 */ u16 field_64;
    /* 0x66 */ u16 field_66;
    /* 0x68 */ u16 field_68;           // map-object id passed to func_80186BC8
    /* 0x6A */ u16 field_6A;
    /* 0x6C */ u16 field_6C;
    /* 0x6E */ u16 field_6E;
    /* 0x70 */ u8 field_70[4];         // per-LOD CTaskLOD ids (loop uses [0]/[1])
    /* 0x74 */ u32 field_74;           // state flag word
    /* 0x78 */ CfGimmickObjectMgr* field_78;  // area manager (createBattleActor)
    /* 0x7C */ CfGimmickReg field_7C;  // gimmick-object registration slot
    /* 0x80 */ u16 field_80;
    /* 0x82 */ u16 field_82;
    /* 0x84 */ CfGimmickObjectArea field_84[2];  // 2 x 0x10-byte gimmick-area tables
    /* 0xA4 */ CfGimmickObjectStep field_A4[2];  // 2 x 0x10 per-step entries
    /* 0xC4 */ u16 field_C4;
    /* 0xC6 */ u8 gap_C6[0xC9 - 0xC6];
    /* 0xC9 */ u8 field_C9;           // mode bits 5..7 gate the busy-flag raise (func_801F7F24)
    /* 0xCA */ u8 gap_CA[0xF4 - 0xCA];
    /* 0xF4 */ u8 field_F4[0x30];      // collider B 3x4 matrix dest (func_802089BC)
    /* 0x124 */ u8 gap_124[0x138 - 0x124];
    /* 0x138 */ u32 field_138;         // jumptable_eu_80535830 dispatch index (func_801F7F24)
    /* 0x13C */ u16 field_13C;
    /* 0x13E */ u16 field_13E;
    /* 0x140 */ u16 field_140;
    /* 0x142 */ u16 field_142;
    /* 0x144 */ u16 field_144[3];      // u16 id table fired by func_801F8BB8
    /* 0x14A */ u16 field_14A[4];      // u16 table scanned by func_801F72A4
    /* 0x152 */ u16 field_152;         // party-member flag word (func_801F6D8C)
    /* 0x154 */ u16 field_154;
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

// Base ctor of the replicated CfGimmick layout (invoked by name).
extern "C" void __ct__cf_CfGimmick(void* self);

// Bdat column-name bases used by the ctor: lbl_eu_80507B60 holds inline
// column names, lbl_eu_805357E8 a pointer table (see CfGimmick.hpp), and
// the two arrays below hold per-index column-name buffers whose second byte
// the ctor overwrites with the 1-based slot digit ('1'+i).
extern char lbl_eu_80507B60[];
extern char* lbl_eu_80534F00[8];   // area-table column names (2 slots x 8)
extern char* lbl_eu_80534F20[10];  // step-table column names (5 slots x 10)

// Gimmick sub-object initializers (CfGimmick.cpp family; holder receives
// &ctor-local bdat handle slot like the sibling gimmick ctors).
extern "C" void func_80208F34(void* self, void* out, void* bdat, void* holder);
extern "C" void func_80209020(void* self, void* out, void* bdat, void* holder);
extern "C" void func_8020915C(void* self, void* out, void* bdat, void* holder);
extern "C" void func_80209288(void* self, void* out, void* bdat, void* holder);

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
void setLODEnable__8CTaskLODFv(u8 lod, int mode);
void* getScnHandle__Fv(void);
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
// Sound-stop helper (func_801BFED0) is declared in CfGimmick.hpp.
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
f32 getLODDistance__8CTaskLODFv(u8 lod);
f32 getLODLevel__8CTaskLODFv(u8 lod);
void updateLODObject__8CTaskLODFv(u8 lod, f32 f);
void removeLODEntry__8CTaskLODFv(u8 lod, f32 f);
void refreshLOD__8CTaskLODFv(u8 lod, f32 f);
void clearLODEntry__8CTaskLODFv(u8 lod);
void attachLODObject__8CTaskLODFv(u8 lod, int mode);
void detachLODObject__8CTaskLODFv(u8 lod, int mode);
void addLODEntry__8CTaskLODFv(u8 lod, int mode);
void setLODObject__8CTaskLODFv(u8 lod, u16 id);
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
u32 func_8006A33C();
int func_802098EC(u32 mask, cf::CfGimmick* gimmick, const CfGimmickVec3* point,
                  const f32* ang, void* partyId);
void func_80159C04(unsigned int a, int b);
void func_8020974C(unsigned int a, int b);
unsigned int func_801587E8(unsigned short id);
u32 getQueuedFileEventCount__Q22cf13CfGameManagerFv();
u32 getResourceFromTable__Q22cf13CfGameManagerFv(u32 resourceId);
u16 func_80208C48(u16 id, f32* vec);
void func_80193678(int id);
// Step-table / sound helpers (func_801F6780 / func_801F76A8).
void func_80140E00(u32 a, u32 b, u32 c);
void func_8015B25C(u16 id);
void func_8020A6B0(void* reg, const CfGimmickVec3* point, u16 c, f32 d,
                  int e, int g);
extern "C" void setChildV40__(void* obj, void* src);  // matches CfObjectImplMove.hpp (const void* is a distinct type)
int func_801BFABC(int a);
// Single shared flat-name form (extern "C" keeps MWCC from re-mangling;
// see CfObjectImplMove.hpp).
extern "C" u16 playActorSound__Q22cf10CfSoundManFUlUlUlUlf(u32 a, u32 b, u32 c,
                                                         u32 d, f32 f);
u16 func_80208C60(u16 id, f32* pos, f32 d);
void func_801BFF78(int a, u16 b, int c);
CfGimmickSoundSlot* func_801BFAE4(u16 handle);
int func_80195B04(int id);
void func_8007B0C8(int idx);
void func_8020A0F8();
extern "C" void* createBattleActor__Q22cf13CfGameManagerFv(u32 id, u32 mode);  // void* form matches CTaskGameEff.hpp (return-type unity pending repo-wide)
void* getPlayer__Q22cf13CfGameManagerFi(int index);
void func_80199678(void* ctrl, int flag);  // CCtrlMovePC helper (CtrlMoveBase)
void clearPlayerEffect__Q22cf13CfGameManagerFv(void* obj);
}

// ---------------------------------------------------------------------------
// Small-data globals read by this TU (retail @sda21 accesses; global-scope
// variables keep their names without `extern "C"`).
// ---------------------------------------------------------------------------
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
// embedded at +0x8C (address passed to func_80199678). Cast-only view, so the
// controller is kept as an opaque byte block (full class lives in
// CtrlMovePC.hpp, which this header cannot include).
struct CfGimmickMoveSub {
    u8 gap_8C[0x8C];
    u8 ctrl[0x114];   // +0x8C: embedded cf::CCtrlMovePC
};

// Base of a CfObjectMove player (func_800B6BC8 nodes point at base+0x3E9C);
// +0x456C holds the u16 id/bit byte scanned by func_801F72A4.
struct CfGimmickPlayerBase {
    void** vtable;           // +0x00
    u8 gap_456C[0x456C - 0x04];
    u16 field_456C;          // +0x456C
};

// Base of a CfObjectMove player scanned by func_801F7F24's id-collection
// loop; +0x3F04/+0x3F08 are flag words raised before clearPlayerEffect fires.
struct CfGimmickPlayerFlags {
    void** vtable;                     // +0x00
    u8 gap_3F04[0x3F04 - 0x04];
    u32 field_3F04;                    // +0x3F04
    u32 field_3F08;                    // +0x3F08
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

// Type-distinct view of the fight-list header (sentinel pointer also at
// +0x04). Used for the init read in cfCountMatchingPlayers so MWCC keeps it
// separate from the loop-condition read (retail hoists that one alone).
struct CfGimmickListNodeRoot {
    u8 pad[4];
    CfGimmickListNode* root;   // +0x04
};
