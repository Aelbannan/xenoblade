#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

class CEventFile;
class CFileHandle;
class CItemImplInstances;

namespace nw4r { namespace lyt { class DrawInfo; class AnimTransform; class ArcResourceAccessor; class Layout; class Pane; } }
namespace nw4r { namespace math { struct VEC3; } }

// C++-mangled imports from code_80135FDC.cpp: the unmangled identifiers mangle
// to the retail symbols closeFileHandle__FPP11CFileHandle and
// releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor (see CItemBoxLine.hpp).
void closeFileHandle(CFileHandle**);
void releaseArcResourceAccessor(nw4r::lyt::ArcResourceAccessor*);


// Opaque item sub-record returned by CItemImpl::vf2C: a u32 at +0 and a
// u16 flag field at +4 whose bits [16..27] select the equip state.
struct CItemBoxSubRecord {
    u32 _00;
    u16 field_04;
    u8 _06[2];
};


// 8-byte colour quad at 0x9C/0xA4 in CItemBoxInfoState: the delta-colour path
// stages it as two u32 words, the selected-colour path reads the alpha s16.
union CItemBoxQuadColorView {
    s16 s[4];
    u32 w[2];
};

struct CItemBoxInfoState {
    u8 _00[0x04];
    UnkClass_8045F564 memRegion1;   // 0x04
    UnkClass_8045F564 memRegion2;   // 0x14
    CFileHandle* fileHandle1;       // 0x24
    CFileHandle* fileHandle2;       // 0x28
    nw4r::lyt::ArcResourceAccessor* arcResourceAccessor;     // 0x2C - nw4r::lyt::ArcResourceAccessor
    nw4r::lyt::ArcResourceAccessor* resource;                // 0x30 - second ArcResourceAccessor (released by releaseArcResourceAccessor)
    nw4r::lyt::Layout* layout;
    nw4r::lyt::AnimTransform* animTransform1;   // 0x38 - nw4r::lyt::AnimTransform
    nw4r::lyt::AnimTransform* animTransform2;   // 0x3C - nw4r::lyt::AnimTransform
    nw4r::lyt::Pane* slotPanes[20];  // 0x40..0x8C - cached selection-row panes
                                     // (filled by CItemBoxInfo2::OnFileEvent)
    u8 active;
    u8 _91[3];
    s32 state;
    u8 current;
    u8 visible;
    u8 field_9A;                  // 0x9A: ctor arg2 (item-box kind)
    u8 _9B;
    CItemBoxQuadColorView quadA;  // 0x9C: colour pair (w[2] / s[3] = alpha)
    CItemBoxQuadColorView quadB;  // 0xA4
    u16 field_AC;                 // 0xAC: ctor arg3
    u8 field_AE;                  // 0xAE
    u8 _AF;
    u8 _B0[0x10];                 // 0xB0..0xBF: per-slot item values (raw view)
    s16 values[12];
};

struct CItemBoxInfo {
    CItemBoxInfoState state;
    CItemBoxInfo(u8 arg2, u16 arg3);
    ~CItemBoxInfo();
    bool OnFileEvent(CEventFile*);

    u8 getItemBoxState();
    void advanceItemBoxState();
    void setItemBoxIndex(unsigned char index, short value);
};

// Free-function helper (retail: tryActivateItemBox__FP12CItemBoxInfo)
void tryActivateItemBox(CItemBoxInfo* info);

// Recovered free functions (retail: getItemBoxState__FP12CItemBoxInfo)
u8 getItemBoxState(CItemBoxInfo* self);

struct CItemBoxInfo2 {
    CItemBoxInfoState state;
    ~CItemBoxInfo2();
    bool OnFileEvent(CEventFile*);

    void drawItemBox2Layout(nw4r::lyt::DrawInfo* drawInfo);
    void advanceItemBox2State();
};

u8 getItemBox2State(CItemBoxInfo2* self);

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
    u8 _0E[2];     // 0x0E
    f32 f38;       // 0x10
    s16 s3C;       // 0x14
    u8 _16[7];     // 0x16..0x1C
    u8 flags1[8];  // 0x1D..0x24
    u8 flags2[8];  // 0x24..0x2B
    u8 flags3[8];  // 0x2B..0x32
    u8 _32[2];     // 0x32..0x33
};


// 0xA4-byte item-name record written by buildItemBoxNameRec (copied out as one
// unit): header word, count byte, label string pointer, 0x10-byte name
// buffer, two state bytes, an equip-state colour and a 0x20-byte text buffer
// used by the SJIS-aware width scan; the 0xA4 copy drags in the unused tail.
struct CItemBoxNameRecord {
    u32 _00;        // 0x00
    u8 count;       // 0x04
    u8 _05[3];      // 0x05
    u32 str;        // 0x08
    char name[0x10];// 0x0C..0x1B
    u8 e0;          // 0x1C
    u8 e1;          // 0x1D
    u8 _1E[2];      // 0x1E
    u32 color;      // 0x20
    char text[0x20];// 0x24..0x43
    u8 _44[0x60];   // 0x44..0xA3
};

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
struct CItemBoxNameRecord2 {
    u8 count;        // 0x00
    u8 _01[3];       // 0x01
    u32 str;         // 0x04
    char name[0x10]; // 0x08..0x17
    u8 e0;           // 0x18
    u8 e1;           // 0x19
    u8 _1A[2];       // 0x1A
    u32 color;       // 0x1C
    char text[0x20]; // 0x20
    u8 _40[0x64];    // 0x40..0xA3
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

extern "C" char lbl_eu_805063BC[];
extern "C" char lbl_eu_8050634C[];
extern "C" char lbl_eu_80506380[];extern "C" void LayoutSetTextBoxFmtValue(nw4r::lyt::Layout*, const char*, const char*, u32);
extern "C" int Counter_TestBit(void*, u32);extern "C" u32 IdTable_SumValues(void*, u32);
extern "C" u32 AnimRewindFrame(nw4r::lyt::AnimTransform*, float);
extern "C" void* CtrlObjectParam_GetCharEntry(u32);
extern "C" void setPaneVisible(void*, u32);
extern "C" u32 BdatGetU8Direct(u32, const char*, u32);
extern "C" u16 BdatGetItemId(u32);
extern "C" u32 BdatGetItemType(u32);
extern "C" u32 LookupItemTypeTable(void*);
extern "C" void clearItemBoxPairTexts(CItemBoxInfo*);
extern "C" void resetItemBoxRowPanes(CItemBoxInfo*);
extern "C" CItemBoxLabelRec getItemBoxLabelRec(CItemBoxInfo*, u32);
extern "C" void SplitU32ToS16s(void*, u32);
extern "C" void setGXColorS10(void*, u32, u32, u32, u32);
extern "C" void __as__11_GXColorS10FRC11_GXColorS10(void* dst, const void* src);
extern "C" void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern "C" char lbl_eu_8052E590[];
extern "C" char lbl_eu_8052E568[];
extern "C" void openItemBoxPhase1(CItemBoxInfo*);
extern "C" void closeItemBoxPhase1(CItemBoxInfo*);
extern "C" void openItemBox2Phase1(CItemBoxInfo2*);
extern "C" void clearItemBox2PairTexts(CItemBoxInfo2*);
extern "C" void resetItemBox2RowPanes(CItemBoxInfo2*);
extern "C" CItemBoxLabelRec getItemBox2LabelRec(CItemBoxInfo2*, u32);
extern "C" u32 getItemBoxCondStat2D(void*, void*);
extern "C" u32 getItemBoxCondStat(void*, void*, u32);
extern "C" u32 getItemBoxEffect(void*, void*, u32, void*);
extern "C" void openItemBox2Phase2(CItemBoxInfo2*);
extern "C" void closeItemBox2Phase1(CItemBoxInfo2*);
extern "C" void closeItemBox2Phase2(CItemBoxInfo2*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" void* lbl_eu_80534B28[];
extern "C" void* lbl_eu_80534A90[];
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void copyVEC3(void*, const void*);
// C-linkage position-set helper (retail-unmangled ScalePaneWorldPos; same
// signature as CSysWin.hpp / CMenuPlayAward.hpp).
extern "C" void ScalePaneWorldPos(void*, void*, void*, void*);
// More flat retail (unmangled) helpers used by the item-box renderers.
extern "C" void PaneSetVtxColorByName(nw4r::lyt::Layout*, const char*, u32);
extern "C" char* BdatTouchStringCell(char*, char*, u32);
extern "C" void PaneMatSetTevColorsByName(nw4r::lyt::Layout*, char*, void*, void*);
extern "C" void PaneSetTexPaletteByName(nw4r::lyt::Layout*, const char*, u32);
extern "C" u32 func_801392C0();
extern "C" int GetCollectedFlagByte(u8);
extern "C" f32 BlendFloatAvgScale(u32 a, u32 b, u32 c, f32 d);
extern "C" void clearItemBox2SlotTexts(CItemBoxInfo2*);
extern "C" void setupItemBox2Detail(CItemBoxInfo2*, u16, void*, u16, u32);
// cf/CfGameManager.hpp (pulled in via harness_catalog) already declares the
// singleton no-arg form of this import at global scope; MWCC rejects a second,
// differently-prototyped extern "C" declaration as an illegal overload. Keep
// the item-pointer form (same unmangled symbol) in a namespace and shim the
// call sites through it.
namespace itemimplshim {
extern "C" void* CItem_initItemImplInstances(void*);
}
#define CItem_initItemImplInstances(item) itemimplshim::CItem_initItemImplInstances(item)
extern "C" void setItemBoxVtxColor(void*, u32, const void*);
extern "C" void applyItemBoxSelColors(CItemBoxInfo*);
// 9-byte flag-record builder: (out, unused, data) - param2 is never read in
// retail (callers pass the item-box pointer through it).
extern "C" CItemBoxSelRec buildItemBoxSelRec(CItemBoxInfo*, u32);
extern "C" void renderItemBox2SlotList(CItemBoxInfo2*, void*, void*);
extern "C" u8 BdatGetU8ByTableKey(const void*, const void*, u32);
extern "C" u32 CtrlObjectParam_GetCurrentRowKey();
extern "C" void initItemBoxVecTable(void*);
extern "C" int IsSkillItem(void*);
extern "C" u32 isItemBoxType9Flagged(void*);
extern "C" void renderItemBoxDetail(CItemBoxInfo*, u32, void*, u32);
extern "C" void renderItemBoxKind2(CItemBoxInfo*, u16, void*, u32);
extern "C" void renderItemBoxKind3(CItemBoxInfo*, u16, void*, u32);

// 0xA8-byte detail record built by buildItemBoxNameRec for renderItemBoxKind3; only the
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

extern "C" void renderItemBoxLabels(CItemBoxInfo*, u32);
extern "C" void refreshItemBoxSlotRows(CItemBoxInfo*, u16);
extern "C" void setupItemBoxSlotPanes(CItemBoxInfo*);
extern "C" void clearItemBoxTripleRows(CItemBoxInfo*);
extern "C" CItemBoxSlotFlags buildItemBox2SlotFlags(void*, u32);
extern "C" void clearItemBoxQuadTexts(CItemBoxInfo*);
extern "C" void renderItemBox2Kind2(CItemBoxInfo2*, u16, void*, u16);
extern "C" void renderItemBox2Kind3(CItemBoxInfo2*, u16, void*, u16);
extern "C" void renderItemBox2Detail(CItemBoxInfo2*, u16, void*, u16);
extern "C" void renderItemBox2Labels(CItemBoxInfo2*, u32);
extern "C" void refreshItemBox2SlotRows(CItemBoxInfo2*, u16);
extern "C" void setupItemBox2SlotPanes(CItemBoxInfo2*);
extern "C" void clearItemBox2TripleRows(CItemBoxInfo2*);
extern "C" void applyItemBox2SelColors(CItemBoxInfo2*);
extern "C" void clearItemBox2QuadTexts(CItemBoxInfo2*);
extern "C" void clearItemBoxSlotTexts(CItemBoxInfo*);
extern "C" void stampItemBoxSlotTex(CItemBoxInfo*);
extern "C" void stampItemBox2SlotTex(CItemBoxInfo2*);
extern "C" void renderItemBox2ByKind(CItemBoxInfo2*, u16, void*, u16, u32);
extern "C" void renderItemBoxSlotList(CItemBoxInfo*, u16, void*);

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

// Unmangled retail imports (plain symbol names in the retail binary).
extern "C" void buildItemBox2NameRec(void*, void*, void*, void*);
extern "C" void LayoutSetTextBoxText(nw4r::lyt::Layout*, const char*, const char*, u32);
extern "C" char* BuildItemNameStr(u32);
extern "C" CItemBoxSelRec buildItemBox2SelTable(CItemBoxInfo2*, u32);
extern "C" void* CItemBlock_getPtr20E8();
extern "C" void PaneSetTexPalette(void*, u32);
extern "C" void PaneSetVtxColorAll(void*, u32);
extern "C" u16 BdatGetU16Direct(const void*, const void*, int);
extern "C" char* MakeTplNameSysFile(u32);
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();

// Layout-build helpers for OnFileEvent (retail mangled C++ symbols; keep
// signature shapes verbatim so the mangled names line up).
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void ApplyFontToTxtPanes(nw4r::lyt::Pane*, void*);
extern "C" char* CUICfManager_getPackedFont9C(void);
extern "C" char* GetBdatNameByNumId(const char* msg);
void buildLayout(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void bindLayoutAnimTransform(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
void setLayoutTextBoxFont(nw4r::lyt::Layout*, char*, u32);

extern "C" u32 MapSexFlagGroup(u16);
extern u32 lbl_eu_80664098;

// 8-byte GXColorS10 returned by value in r3:r4 from PaneMatGetS16Quad.
struct CItemBoxGXColor {
    s16 r, g, b, a;
};
// Pane/material vertex-colour getter (flat retail symbol; returns in r3:r4).
extern "C" CItemBoxGXColor PaneMatGetS16Quad(void* obj, int index);


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
extern "C" CItemBoxSlotRecord buildItemBox2SlotRec(CItemBoxInfo2*, u32, void*);

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
extern "C" CItemBoxSlotRecord1 buildItemBoxSlotRec(CItemBoxInfo*, u32, void*);

// Word-array view: assigning the union copies the whole object as one block
// (reproduces the retail mtctr lwzu/stwu pair loop) instead of expanding
// field-by-field.
union CItemBoxSlotRecAny1 {
    CItemBoxSlotRecord1 rec;
    u32 w[11];
};

