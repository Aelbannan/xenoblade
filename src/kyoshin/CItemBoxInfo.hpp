#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include <nw4r/math/math_types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

class CEventFile;
class CFileHandle;
class CItemImplInstances;

namespace nw4r { namespace lyt { class DrawInfo; class AnimTransform; class ArcResourceAccessor; class Layout; class Pane; } }

// C++-mangled imports from code_80135FDC.cpp: the unmangled identifiers mangle
// to the retail symbols closeFileHandle__FPP11CFileHandle and
// releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor (see CItemBoxLine.hpp).


// Item record shared by the item-box grid/equip/info widgets: packed kind
// word, 16-bit class/price key, tier byte and state/flag byte. This is the
// object handed out by CItem_kindPtr and by CItemImpl::vf2C.
struct CItemBoxItem {
    u32 mWord;   // +0x00
    u16 mFlags;  // +0x04
    u8 mTier;    // +0x06
    u8 mState;   // +0x07
};

// Per-art learn state inside a CItemBoxArtRow: level byte (non-zero once
// learned) and a flag byte whose top bits gate the arts-book sources.
struct CItemBoxArtSlot {
    u8 mLevel;  // +0x00 // name-guess
    u8 mFlags;  // +0x01 // name-guess
};

// 0x49-byte arts row indexed by the character's current row key: one header
// byte followed by 36 art slots indexed by the pc_arts idx.
struct CItemBoxArtRow {
    u8 mHead;                   // +0x00 // name-guess
    CItemBoxArtSlot mArts[36];  // +0x01 // name-guess
};

// Character-param entry returned by CtrlObjectParam_GetCharEntry. The six
// s16 values at +0x1C..+0x27 are the equipped item ids for the six slots
// (the weapon sits at +0x26), and +0x3534 is the counter/arts area. Shared by
// the item-box grid, equip box, info popup and line widgets.
struct CItemBoxCharEntry {
    u8 _00[0x08];
    u32 mBdatTable;          // +0x08 // name-guess: bdat table handle paired with +0xC row key
    u16 mBdatRow;            // +0x0C // name-guess: bdat row key copied into CItemBoxSlotEntry::h
    u8 _0E[0x1C - 0x0E];
    s16 slotIds[6];          // +0x1C..+0x27
    u8 _28[0xE7 - 0x28];
    CItemBoxArtRow mArtRows[2]; // +0xE7 // name-guess: rows selected by GetCurrentRowKey
    u8 _179[0x17C - 0x179];
    cf::CActorParam mParam;  // +0x17C (embedded actor parameter block)
    u8 _3500[0x3534 - 0x34FC]; // +0x3500..+0x3533
    u8 counterArea[0x8A0];   // +0x3534
};

// Opaque item sub-record returned by CItemImpl::vf2C: a u32 at +0 and a
// u16 flag field at +4 whose bits [16..27] select the equip state.
struct CItemBoxSubRecord {
    u32 _00;
    u16 field_04;
    u8 _06[2];
};


// 8-byte colour quad at 0x9C/0xA4: the delta-colour path stages it as two
// u32 words, the selected-colour path reads the alpha s16.
union CItemBoxQuadColorView {
    s16 s[4];
    u32 w[2];
};

struct CItemBoxPagePos {
    f32 x, y, z;
};

/* The page's 0x80-byte text block at +0x0D9, copied as ONE 0x80-byte block by
 * the implicit page copy (retail __as__12CItemBoxInfoFRC12CItemBoxInfo copies
 * 0x15C bytes as 24 B / 0x90 / 48 B / byte / 0x80 and stops at +0x159): the
 * GetItemName output buffer, the twelve s16 values that
 * CItemBoxLine::RefreshPageList writes at page+0x140, and the trailing flag.
 * Packed because the s16 array starts at the odd offset +0x67. */
#pragma pack(push, 1)
struct CItemBoxPageText {
    char mName[0x67];   // 0x00 GetItemName output buffer
    s16 mValues2[12];   // 0x67
    u8 mFlag;           // 0x7F
};
#pragma pack(pop)

/* 0x15C-byte per-page item table embedded in CItemBoxInfo/CItemBoxInfo2
 * (+0xB0), CItemBoxLine (+0x4E0), CEquipItemBox and CMenuGetItemMulti. No
 * user ctor (CItemBoxLine constructs it without a call); the empty inline dtor
 * is kept weak in CMenuGetItemMulti. CItemBoxInfo/CItemBoxInfo2 call the empty
 * Reset() at the end of their ctors (retail 0x801D5ACC). */
class CItemBoxPage {
public:
    ~CItemBoxPage() {}

    void Reset();
    char* GetItemName(u8 index);
    void Clear();
    void SetValue(u8 index, s16 value);

    u16 mItemIds[12];          // 0x000
    CItemBoxPagePos mPos[12];  // 0x018
    u8 mBytesA8[12];           // 0x0A8
    u8 mBytesB4[12];           // 0x0B4
    s16 mValues[12];           // 0x0C0
    u8 mSelSlot;               // 0x0D8 selected slot (CItemBoxLine page cursor)
    CItemBoxPageText mText;    // 0x0D9 (0x80 bytes; the page ends at +0x159)
};

// Item detail popup (retail RTTI "CItemBoxInfo", base list IWorkEvent).
class CItemBoxInfo : public IWorkEvent {
public:
    CItemBoxInfo(u8 kind, u16 mode);
    virtual ~CItemBoxInfo();
    virtual bool OnFileEvent(CEventFile* file);

    void LoadFiles();
    void UpdateAnims();
    void DrawLayout(nw4r::lyt::DrawInfo* drawInfo);
    void UnloadFiles();
    u32 GetState();
    void Open();
    void Close();
    void HidePanes(u16 category);
    void SetItem(u16 arg2, void* arg3, u16 arg4);
    void SetNamedText(int index, char* text);
    nw4r::math::VEC3 GetSlotPos(u32 index);
    void SetCounts(u8 arg2, u8 arg3);
    void SetResultPane(bool visible);
    void OpenPhase1();
    void OpenPhase2();
    void ClosePhase1();
    void ClosePhase2();
    struct CItemBoxCompRecord BuildCmpRecord(void* arg3);
    struct CItemBoxSlotFlags BuildSlotFlags(u32 item);
    struct CItemBoxNameRecord BuildNameRec(void* data, void* arg3);
    struct CItemBoxLabelRec GetLabelRec(u32 item);
    struct CItemBoxSelRec BuildSelRec(u32 item);
    struct CItemBoxSlotRecord1 BuildSlotRec(u32 item, void* arg3);
    void RenderKind2(u16 arg2, void* arg3, u32 arg4);
    void RenderDetail(u32 itemId, void* record, u32 arg4);
    void RenderTripleRows(u32 itemId, void* record);
    void RefreshSlotRows(u16 arg2);
    void RenderKind3(u16 arg2, void* arg3, u32 arg4);
    void RenderLabels(u32 arg2);
    void RenderSlotList(u16 arg2, void* arg3);
    void SetupSlotPanes();
    void ResetRowPanes();
    void ClearTripleRows();
    void ApplySelColors();
    void ClearQuadTexts();
    void ClearPairTexts();
    void ClearSlotTexts();
    void StampSlotTex();
    void Activate();
    void SetupDetail(u32 arg2, void* arg3, u32 arg4);
    s32 CalcBonus1(u16 b, s32 arg2, void* d);
    s32 CalcBonus2(u16 b, s32 arg2, void* d);
    u32 GetEffect(u16 lookup_key, u32 category, void* arg3);
    u32 SumGemBonus(void* member, u32 category, void* candidate, s32 slot);
    u32 GetCondStat(void* b, u32 arg2);
    u32 TestBdatFlag(u32 arg1, void* arg2);
    u32 SumTypeCounts(u16 arg2, void* arg3);
    u32 CheckGemChain(u16 lookup_key, void* arg3, void* unused2);

    UnkClass_8045F564 mRegion1;                    // 0x04
    UnkClass_8045F564 mRegion2;                    // 0x14
    CFileHandle* mFileHandle1;                     // 0x24
    CFileHandle* mFileHandle2;                     // 0x28
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;    // 0x2C
    nw4r::lyt::ArcResourceAccessor* mTexArcResAcc; // 0x30
    nw4r::lyt::Layout* mpLayout;                   // 0x34
    nw4r::lyt::AnimTransform* mAnimIn;             // 0x38
    nw4r::lyt::AnimTransform* mAnimInfoIn;         // 0x3C
    /* 0x40 */ nw4r::lyt::Pane* mSlotPane0;
    /* 0x44 */ nw4r::lyt::Pane* mSlotPane1;
    /* 0x48 */ nw4r::lyt::Pane* mSlotPane2;
    /* 0x4C */ nw4r::lyt::Pane* mSlotPane3;
    /* 0x50 */ nw4r::lyt::Pane* mSlotPane4;
    /* 0x54 */ nw4r::lyt::Pane* mSlotPane5;
    /* 0x58 */ nw4r::lyt::Pane* mSlotPane6;
    /* 0x5C */ nw4r::lyt::Pane* mSlotPane7;
    /* 0x60 */ nw4r::lyt::Pane* mSlotPane8;
    /* 0x64 */ nw4r::lyt::Pane* mSlotPane9;
    /* 0x68 */ nw4r::lyt::Pane* mSlotPane10;
    /* 0x6C */ nw4r::lyt::Pane* mSlotPane11;
    /* 0x70 */ nw4r::lyt::Pane* mSlotPane12;
    /* 0x74 */ nw4r::lyt::Pane* mSlotPane13;
    /* 0x78 */ nw4r::lyt::Pane* mSlotPane14;
    /* 0x7C */ nw4r::lyt::Pane* mSlotPane15;
    /* 0x80 */ nw4r::lyt::Pane* mSlotPane16;
    /* 0x84 */ nw4r::lyt::Pane* mSlotPane17;
    /* 0x88 */ nw4r::lyt::Pane* mSlotPane18;
    /* 0x8C */ nw4r::lyt::Pane* mSlotPane19;
    u8 mActive;                                    // 0x90
    s32 mState;                                    // 0x94
    u8 mCurrent;                                   // 0x98
    u8 mVisible;                                   // 0x99
    u8 mKind;                                      // 0x9A
    CItemBoxQuadColorView mColorA;                 // 0x9C
    CItemBoxQuadColorView mColorB;                 // 0xA4
    u16 mMode;                                     // 0xAC
    u8 mFlagAE;                                    // 0xAE
    CItemBoxPage mPage;                            // 0xB0
};

// Shop/exchange item detail popup: same layout as CItemBoxInfo, separate
// class (retail RTTI "CItemBoxInfo2", base list IWorkEvent only).
class CItemBoxInfo2 : public IWorkEvent {
public:
    CItemBoxInfo2(u8 kind, u16 mode);
    virtual ~CItemBoxInfo2();
    virtual bool OnFileEvent(CEventFile* file);

    void LoadFiles();
    void UpdateAnims();
    void DrawLayout(nw4r::lyt::DrawInfo* drawInfo);
    void UnloadFiles();
    u32 GetState();
    void Open();
    void Close();
    void SetItem(u16 arg2, void* arg3, u16 arg4, u32 arg5);
    void SetNamedText(int index, char* text);
    nw4r::math::VEC3 GetSlotPos(u32 index);
    void OpenPhase1();
    void OpenPhase2();
    void ClosePhase1();
    void ClosePhase2();
    struct CItemBoxCompRecord BuildCmpRecord(void* arg3);
    struct CItemBoxSlotFlags BuildSlotFlags(u32 item);
    struct CItemBoxNameRecord BuildNameRec(void* data, void* arg3);
    struct CItemBoxLabelRec GetLabelRec(u32 item);
    struct CItemBoxSelRec BuildSelRec(u32 item);
    struct CItemBoxSlotRecord BuildSlotRec(u32 item, void* arg4);
    void RenderKind2(u16 arg1, void* arg2, u16 arg3);
    void RenderDetail(u16 arg2, void* arg3, u16 arg4);
    void RefreshSlotRows(u16 arg2);
    void RenderKind3(u16 arg2, void* arg3, u16 arg4);
    void RenderLabels(u32 arg2);
    void RenderSlotList(void* arg1, void* arg2);
    void SetupSlotPanes();
    void ResetRowPanes();
    void ClearTripleRows();
    void ApplySelColors();
    void ClearQuadTexts();
    void ClearPairTexts();
    void ClearSlotTexts();
    void StampSlotTex();
    void Activate();
    void SetupDetail(u16 arg2, void* arg3, u16 arg4, u32 arg5);
    s32 CalcBonus1(u16 b, s32 arg2, void* d);
    s32 CalcBonus2(u16 b, s32 arg2, void* d);
    u32 GetCondStat2D(void* arg1);
    u32 GetEffect(u32 b, u32 c, void* d);
    u32 GetCondStatById(u32 arg1, u32 arg2);
    u32 TestBdatFlag(u32 arg1, u32 arg2);
    u32 SumTypeCounts(u16 arg2, void* arg3);
    bool TestEquip(u16 b, void* c);

    UnkClass_8045F564 mRegion1;                    // 0x04
    UnkClass_8045F564 mRegion2;                    // 0x14
    CFileHandle* mFileHandle1;                     // 0x24
    CFileHandle* mFileHandle2;                     // 0x28
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;    // 0x2C
    nw4r::lyt::ArcResourceAccessor* mTexArcResAcc; // 0x30
    nw4r::lyt::Layout* mpLayout;                   // 0x34
    nw4r::lyt::AnimTransform* mAnimIn;             // 0x38
    nw4r::lyt::AnimTransform* mAnimInfoIn;         // 0x3C
    /* 0x40 */ nw4r::lyt::Pane* mSlotPane0;
    /* 0x44 */ nw4r::lyt::Pane* mSlotPane1;
    /* 0x48 */ nw4r::lyt::Pane* mSlotPane2;
    /* 0x4C */ nw4r::lyt::Pane* mSlotPane3;
    /* 0x50 */ nw4r::lyt::Pane* mSlotPane4;
    /* 0x54 */ nw4r::lyt::Pane* mSlotPane5;
    /* 0x58 */ nw4r::lyt::Pane* mSlotPane6;
    /* 0x5C */ nw4r::lyt::Pane* mSlotPane7;
    /* 0x60 */ nw4r::lyt::Pane* mSlotPane8;
    /* 0x64 */ nw4r::lyt::Pane* mSlotPane9;
    /* 0x68 */ nw4r::lyt::Pane* mSlotPane10;
    /* 0x6C */ nw4r::lyt::Pane* mSlotPane11;
    /* 0x70 */ nw4r::lyt::Pane* mSlotPane12;
    /* 0x74 */ nw4r::lyt::Pane* mSlotPane13;
    /* 0x78 */ nw4r::lyt::Pane* mSlotPane14;
    /* 0x7C */ nw4r::lyt::Pane* mSlotPane15;
    /* 0x80 */ nw4r::lyt::Pane* mSlotPane16;
    /* 0x84 */ nw4r::lyt::Pane* mSlotPane17;
    /* 0x88 */ nw4r::lyt::Pane* mSlotPane18;
    /* 0x8C */ nw4r::lyt::Pane* mSlotPane19;
    u8 mActive;                                    // 0x90
    s32 mState;                                    // 0x94
    u8 mCurrent;                                   // 0x98
    u8 mVisible;                                   // 0x99
    u8 mKind;                                      // 0x9A
    CItemBoxQuadColorView mColorA;                 // 0x9C
    CItemBoxQuadColorView mColorB;                 // 0xA4
    u16 mMode;                                     // 0xAC
    CItemBoxPage mPage;                            // 0xB0
};

struct CItemBoxInfoEntry {
    u16 itemId;
    u16 _02;
    u32 value;
    u8 state;

};

// 8-byte color (4 s16 channels) used by the item-box layout renderer.
// Colour quad used by the item-box layout renderer. The default ctor is
// zero-initializing; stack locals are built member-wise at their use sites.
struct CItemBoxQuadColor {
    s16 r, g, b, a;
    CItemBoxQuadColor() : r(0), g(0), b(0), a(0) {}
    CItemBoxQuadColor(s16 _r, s16 _g, s16 _b, s16 _a) : r(_r), g(_g), b(_b), a(_a) {}
};

// 4 selection/vertex colours (0x20 bytes) built per-slot in renderItemBox2Kind2.
struct CItemBoxQuad {
    CItemBoxQuadColor col[4];
};

// POD views without ctors: renderItemBoxDetail builds its colour pair member-wise
// at the use site (retail shows bare sth/stw sequences, no ctor calls).
struct CItemBoxColorPOD {
    s16 r, g, b, a;
};
struct CItemBoxQuadPOD {
    CItemBoxColorPOD col[4];
};

// 0x1C-byte per-slot comparison record returned by buildItemBoxSlotFlags:
// three s16 stat values (read sign-extended by lha) plus three 7-slot byte
// banks (equipable flag, then the two stat-comparison results 0/1/2).
struct CItemBoxSlotFlags {
    s16 v[3];        // 0x00
    u8 equipable[7]; // 0x06
    u8 cmp0[7];      // 0x0D
    u8 cmp1[7];      // 0x14
    u8 _1B;          // 0x1B
};

// Word-array view: assigning the union copies the whole 0x1C-byte object as
// one block, reproducing the retail mtctr 8-byte-pair copy loop.
union CItemBoxSlotFlagsAny {
    CItemBoxSlotFlags rec;
    u32 w[7];
};

// 0x34-byte per-slot comparison record built by buildItemBoxCmpRecord / buildItemBox2CmpRecord
// and copied out as one unit. The three flag banks at +0x1D/+0x24/+0x2B hold
// per-slot {1,2,0} comparison results (candidate vs current item values); the
// word0 float is the BlendFloatAvgScale result and word4 the name ratio.
struct CItemBoxCompRecord {
    f32 f28;       // 0x00 (BlendFloatAvgScale result)
    s16 s2C;       // 0x04
    s16 s2E;       // 0x06
    s16 s30;       // 0x08
    s16 s32;       // 0x0A
    s16 s34;       // 0x0C
    f32 f38;       // 0x10
    s16 s3C;       // 0x14
    // 0x16: per-slot rows (7 slots): [0] slot-selected flag, [1..3] the
    // {1,2,0} comparison results for f28 / s30 / s32.
    u8 flags[4][7];
};


// 0xA4-byte item-name record written by buildItemBoxNameRec (copied out as one
// unit): header word, count byte, label string pointer, 0x10-byte name
// buffer, two state bytes, an equip-state colour and a 0x20-byte text buffer
// used by the SJIS-aware width scan; the 0xA4 copy drags in the unused tail.


// Stack copy of the static label-pointer table (lbl_eu_8050634C): a header
// word precedes the four char* entries; padded so the 0x1C-byte move stays a
// 3-pair mtctr loop + 4-byte tail.
struct CItemBoxLabelTable {
    u32 hdr;         // 0x00
    char* labels[4]; // 0x04
    u32 tail[2];     // 0x14..0x1B
};

// 0xA4-byte item-name record written by buildItemBox2NameRec (ItemBox2 twin of
// buildItemBoxNameRec, copied out as one unit): unlike CItemBoxNameRecord the
// count byte sits at +0 (no u32 prefix), so str is at +4 and the name buffer
// at +8. The two state bytes at +0x18/+0x19 and the equip colour at +0x1C
// sit inside the tail of the name buffer (retail overwrites them after the
// name sprintf), and the text buffer is at +0x20. The remaining +0x40..0xA3
// bytes are copied out but unused.
struct CItemBoxNameRecord {
    u8 count;        // 0x00
    u8 _01[3];       // 0x01
    u32 str;         // 0x04
    char name[0x10]; // 0x08..0x17
    u8 e0;           // 0x18
    u8 e1;           // 0x19
    u8 _1A[2];       // 0x1A
    u32 color;       // 0x1C
    char text[0x20]; // 0x20
    u8 _40[0x60];    // 0x40..0x9F
};

// 9-byte selection record returned by buildItemBoxSelRec: sel[0] is the
// category flag, sel[1..7] the per-slot "selected" flags; flag2 also selects
// the item-box row-height label in refreshItemBoxSlotRows.
struct CItemBoxSelRec {
    u8 category;       // 0x00
    bool selected[7];  // 0x01 per-slot "selected" flags
    u8 flag2;          // 0x08
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Out-record built by getItemBoxLabelRec / getItemBox2LabelRec: {u8, ptr, u8, ptr}.
// Returned by value (hidden result pointer in r3); the struct copy is the
// retail four-word lwz/stw block.
struct CItemBoxLabelRec {
    u8 a;
    u8 pad0[3];
    char* b;
    u8 c;
    u8 pad1[3];
    char* d;
};

// 0xA4-byte body (offset 4..0xA7) is copied out whole by the callers (retail
// mtctr 8-byte-pair copy loop over 20 iterations).
struct CItemBoxDetailBody {
    char* text;      // 0x00 - description text pointer
    u8 kind;         // 0x04 - compared against 0x30
    u8 _05[3];
    u8 _08[0x10];
    u8 flag18;       // 0x18 - caption selector (3/4 select the count label)
    u8 _19[0x8B];    // pad to 0xA4
};
struct CItemBoxDetailRec {
    u32 _00;
    CItemBoxDetailBody body;
};

// Six per-slot equip s16 values staged from the lookup record inside the
// renderItemBoxKind3 slot loop ([0] = record+0x26 field, [1..5] = +0x1c..+0x24).
union CItemBoxSlotVals {
    struct {
        s16 v26;
        s16 v1C;
        s16 v1E;
        s16 v20;
        s16 v22;
        s16 v24;
    };
    s16 s[6];
};


// 0xA0-byte label record written by buildItemBox2NameRec and consumed by
// renderItemBox2Kind3 (CItemBoxNameRecord2 minus its unused 4-byte tail).
struct CItemBoxInfo2LabelRec {
    u32 count;       // 0x00
    char* str;       // 0x04 - pane text pointer for the fixed label
    char name[0x10]; // 0x08..0x17 (name[0] checked against '0', used as %s)
    u8 e0;           // 0x18 equip-state flag
    u8 e1;           // 0x19
    u8 _1A[2];
    u32 color;       // 0x1C
    char text[0x20]; // 0x20
    u8 _40[0x60];    // 0x40..0x9F
};

// Per-slot equip-value entry staged in renderItemBox2Kind3: the table handle pair
// comes from two .sdata2 constants, the six s16 values from the slot's item
// table record.
struct CItemBoxSlotEntry {
    u32 tbl;       // 0x00
    u16 h;         // 0x04
    u8 _06[2];
    s16 vals[6];   // 0x08..0x13 ([0] = +0x26 field, [1..5] = +0x1c..+0x24)
};

// Six-byte equip-slot kind table {2,4,5,6,7,8} staged once before the slot
// loops of renderItemBoxKind3/renderItemBox2Kind3 (MWCC pools the initializer
// in .sdata2 and copies it as a word + halfword).
// Packed to its 6 bytes so the pooled initializer keeps 4-byte alignment.
#pragma pack(push, 2)
union CItemBoxTagStage {
    u8 kinds[6];
    struct {
        u32 tbl;
        u16 h;
    };
};
#pragma pack(pop)

// POD (ctor-less) 8-byte colours used by refreshItemBox2SlotRows: the s16 view builds
// the alpha-only defaults with individual halfword stores, the u32 view lets
// the selected-item pair move as word loads/stores (retail stw/lwz shape).
struct CItemBoxColorRaw {
    s16 r, g, b, a;
};
struct CItemBoxColorBlock {
    u32 w[2];
};

// Halfword/word dual view of one 8-byte colour: built via the s16 members,
// moved as whole words (retail lwz/stw shape).
union CItemBoxColorAny {
    CItemBoxColorRaw c;
    CItemBoxColorBlock w;
};
// 0x10-byte POD vertex-colour quad built per-slot in renderItemBoxKind3 from
// info+0x9C..0xA8 and consumed pairwise by setItemBoxVtxColor.
struct CItemBoxVertexColors {
    u32 w[4];
};

 // Pane/material vertex-colour getter (flat retail symbol; returns in r3:r4).


// renderers): count byte, string pointer, per-slot text pointers, a 4-byte
// gap, per-slot byte values, and a tail whose byte 1 doubles as the build
// counter. Whole-struct copies of this size compile to the retail mtctr
// 8-byte-pair copy loop.
struct CItemBoxSlotRecord {
    u8 count;      // 0x00
    u8 _01[3];     // 0x01
    u32 str;       // 0x04
    u32 text[4];   // 0x08
    u8 _18[4];     // 0x18
    u8 vals[4];    // 0x1C
    u8 tail[4];    // 0x20 (tail[1] = counter at 0x21)
};

// 0x2C-byte item-box slot record with per-slot pair counts (built by
// buildItemBoxSlotRec, copied out as one unit): count byte, string pointer,
// per-slot text pointers, per-slot byte values, a counter byte at 0x21, and
// two u16 pair counts at 0x22.
struct CItemBoxSlotRecord1 {
    u8 count;      // 0x00
    u8 _01[3];     // 0x01
    u32 str;       // 0x04
    u32 text[4];   // 0x08
    u8 _18[4];     // 0x18
    u8 vals[4];    // 0x1C
    u8 _20;        // 0x20
    u8 counter;    // 0x21 (build counter)
    u16 counts[4]; // 0x22 (indexed up to counter-1)
    u8 _26[2];     // 0x2A - pad to 0x2C
};

// Word-array view: assigning the union copies the whole object as one block
// (reproduces the retail mtctr lwzu/stwu pair loop) instead of expanding
// field-by-field.
union CItemBoxSlotRecAny1 {
    CItemBoxSlotRecord1 rec;
    u32 w[11];
};

