#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"

class CEventFile;

namespace nw4r {
namespace lyt {
    class Pane;
    class Layout;
    class AnimTransform;
    class ArcResourceAccessor;
}
namespace math {
    struct VEC3;
}
}

class CArtsInfo {
public:
    CArtsInfo();
    // Virtual in retail; keeps the implicit vtable pointer slot at 0x00 so
    // member offsets match (the free-function ctor emits no extra vtable store).
    virtual ~CArtsInfo();
    int OnFileEvent(CEventFile* event);
    u8 getField48();
    u8 getField49();
    void setField54(u8 val);
    void setField55(u8 val);
    void setField56(u8 val);
    void setField58(u16 val);
    u32 getField5A();
    int isField44GE6();
    void initialize();

    // vtable pointer at 0x00 (implicit)
    /* 0x04 */ UnkClass_8045F564 mMemRegion;
    /* 0x14 */ int field_0x14;  // file handle 1
    /* 0x18 */ int field_0x18;  // file handle 2
    /* 0x1C */ int field_0x1C;  // arc resource accessor
    /* 0x20 */ nw4r::lyt::Layout* mpLayout1;
    /* 0x24 */ nw4r::lyt::AnimTransform* mpAnimTrans1;
    /* 0x28 */ nw4r::lyt::AnimTransform* mpAnimTrans2;
    /* 0x2C */ nw4r::lyt::AnimTransform* mpAnimTrans3;
    /* 0x30 */ nw4r::lyt::AnimTransform* mpAnimTrans4;
    /* 0x34 */ nw4r::lyt::Layout* mpLayout2;
    /* 0x38 */ nw4r::lyt::AnimTransform* mpAnimTrans5;
    /* 0x3C */ nw4r::lyt::AnimTransform* mpAnimTrans6;
    /* 0x40 */ u8 field_0x40;
    /* 0x41 */ u8 _41[3];      // padding
    /* 0x44 */ int field_0x44;  // state machine
    /* 0x48 */ u8 field_0x48;
    /* 0x49 */ u8 field_0x49;
    /* 0x4A */ u8 _4A[2];      // padding
    /* 0x4C */ int field_0x4C;
    /* 0x50 */ int field_0x50;
    /* 0x54 */ u8 field_0x54;
    /* 0x55 */ u8 field_0x55;
    /* 0x56 */ u8 field_0x56;
    /* 0x57 */ u8 _57;          // padding
    /* 0x58 */ u16 field_0x58;
    /* 0x5A */ s8 field_0x5A;
    /* 0x5B */ u8 _5B;          // padding
    /* 0x5C */ u8 mCursor[0x18]; // CCur18 embedded cursor
};

// Async file event handed to IWorkEvent callbacks by CDeviceFile (global
// ::CEventFile, full layout in monolib/work/CEventFile.hpp).
struct CEventFile {
    s32 field_00;          // 0x00 event type
    CFileHandle* field_04; // 0x04 file handle
};


// Overlay structs for the 'timg' message resource consumed by OnFileEvent:
// the resource object holds a chain whose first field points at the u16
// row/column pair used to size the message panes.
struct CArtsCoords {
    u16 col; // 0x0
    u16 row; // 0x2
};
struct CArtsMsgChain {
    CArtsCoords* pCoords; // 0x0
};
struct CArtsMsgObj {
    u8 gap[0x8];
    CArtsMsgChain* chain; // 0x8
};

// Pane overlays used by OnFileEvent: message-pane size floats (+0x4C/+0x50)
// and an anchor-pane position triple (+0x2C..+0x36) read/written through a
// u32/float punning union (retail moves the values GPR->FPR via the stack).
union CArtsFpu {
    u32 u;
    float f;
};
struct CArtsPaneSize {
    u8 gap[0x4C];
    CArtsFpu width;  // +0x4C
    CArtsFpu height; // +0x50
};
struct CArtsPanePos {
    CArtsFpu x; // +0x2C
    CArtsFpu y; // +0x30
    CArtsFpu z; // +0x34
};

// 4x s16 colour records shared through the small-data area (sinit'd by
// sinit_8023BC8C).
struct CArtsQuadColor {
    s16 c0;
    s16 c2;
    s16 c4;
    s16 c6;
};

// 8-byte pane colour pair returned in r3/r4 by func_801397AC.
struct CArtsColorPair {
    u32 lo;
    u32 hi;
};







// +0x17C: vptr of the embedded stats object (real owner: cf::CActorParam;
// +0x200 is CActorParam_UnkVirtualFunc91, +0x224 UnkVirtualFunc100).

// Stat block returned by the vtable[0x224] dispatch.
struct CArtsStatBlock {
    u8 _00[0x10];
    float field_0x10;                   // +0x10
    u8 _14[0x08];                       // 0x14..0x1B
    s16 field_0x1C;                     // +0x1C
    s16 field_0x1E;                     // +0x1E
    s16 field_0x20;                     // +0x20
};

// Character-data object returned by func_8009EC9C (see CPartyCharData).
struct CArtsCharData {
    u8 _00[0x1C];
    s16 field_0x1C;                     // +0x1C
    s16 field_0x1E;                     // +0x1E
    s16 field_0x20;                     // +0x20
    s16 field_0x22;                     // +0x22
    s16 field_0x24;                     // +0x24
    s16 field_0x26;                     // +0x26 (weapon id; -1 = none)
    u8 _28[0x17C - 0x28];               // 0x28..0x17B
    u8 stats[4];                        // +0x17C (vptr; see note above)
};

// (List entries come from func_80157C4C as CItemExt*; see kyoshin/cf/CItem.hpp.)

// Sub-entry returned by the slot-info virtual (vtable +0x2C). The arts list
// loop reads an id field at +0x4 (bits 4..15 of the u16) and a value at
// +0x0 (bits 10..20 of the u32).
struct CArtsItemInfo {
    u32 field_0x0;                      // +0x00
    u16 field_0x4;                      // +0x04
};


// Six-byte item slot-type flags for the arts list loop (func_80236E6C): a
// u32 + u16 loaded from the sdata objects at 0x80668688/0x8066868C, then
// byte-indexed per loop iteration. The union gives the byte view without
// pointer arithmetic.
union CArtsSlotFlags {
    struct {
        u32 field_0x0;                  // +0x00
        u16 field_0x4;                  // +0x04
    } w;
    u8 b[6];
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// func_eu_80136F90 is defined in code_80135FDC.cpp (C linkage);
// func_eu_8023D490 is a same-unit retail symbol (unmangled, C linkage).
extern "C" void func_eu_80136F90(char* str);
extern "C" void func_eu_8023D490(CArtsInfo*, u32, char*);
extern "C" int getLanguage__9CDeviceSCFv();
extern "C" s8 BdatGetS8ByTableKey(const void*, const char*, u8); // BDAT string -> s8
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u32 KyoshinHeap_GetField44();
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void getEntry__5CBdatFUl(u32);
extern "C" void closeFileHandle__FPP11CFileHandle(void*);
extern "C" void releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void deleteRegion__17UnkClass_8045F564Fv(void*);
extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__CCur18(void*, void*);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dl__FPv(void*);
extern "C" u32 advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(nw4r::lyt::AnimTransform*, float);
extern "C" u32 AnimRewindFrame(nw4r::lyt::AnimTransform*, float);
extern "C" void LayoutSetTextBoxFmtValue(nw4r::lyt::Layout*, char*, char*, u32);
extern "C" void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);
extern "C" char* BdatTouchStringCell(const void*, const void*, int); // caller-tuned (see code_80135FDC.hpp)
extern "C" char* BdatGetPtrDirect(const void*, const void*, int); // BDAT row string lookup
extern "C" void playUISound__FUl(u32);
extern "C" u8 BdatGetU8Direct(u32, const char*, u32);
// BdatGetU16ByTableKey/8013600C take byte-keyed lookups: retail masks the 3rd arg
// to 8 bits at the call site, so the param is u8.
extern "C" u16 BdatGetU16ByTableKey(const void*, const void*, u8);
extern "C" u8 BdatGetU8ByTableKey(const void*, const void*, u32);
extern "C" s16 BdatGetS16ByTableKey(const void*, const void*, u32); // u32: matches defining TU code_80135FDC.cpp verbatim
extern "C" void SplitU32ToS16s(void*, u32);
extern "C" void setGXColorS10(void*, s16, s16, s16, s16); // colour init
extern "C" void PaneMatSetTevColorsByName(void*, void*, void*, void*);
extern "C" void func_80137924(void*, void*, void*, void*);
extern "C" void* func_8009EC9C(u32);
extern "C" u32 CtrlObjectParam_GetCurrentRowKey(void*); // character-data category
// Shared retail declaration (same signature as CItemBoxGrid.hpp - MWCC
// rejects differing extern "C" redeclarations as illegal overloading).
extern "C" u32 CtrlObjectParam_GetArtsDataWord(void*);
extern "C" u16 BdatGetItemId(u32);
// Full-width return: retail callers keep the raw result in a register and
// narrow explicitly ((u16) casts); a u16 decl forces an early clrlwi.
extern "C" u32 BdatGetU16Direct(const void*, const void*, int);
// (func_80157C4C is declared by kyoshin/cf/CItem.hpp: the real 2-arg
// CItemExt* form. Call sites pass the slot id that retail leaves in r4.)
extern "C" void func_801D202C(void*);
extern "C" void Cur_DrawLayout(void*, void*);
extern "C" void Cur_SetVisible(void*, int);
extern "C" int sprintf(char*, const char*, ...);

// Item-implementation object for an entry (real owner: CItemImpl in
// kyoshin/cf/CItem.hpp; slots +0x2C/+0x30/+0x40/+0x54/+0x90).
// (CItem_initItemImplInstances is declared by kyoshin/cf/CItem.hpp.)

// Imports from the IdTable_SumValues accessor family (code_8025FB10.cpp).
// Retail call sites bind to the unmangled names, so keep C linkage
// (same style as CItemBoxGrid.hpp).
extern "C" int Counter_TestBit(void*, u32);
// int-return form: single winning decl shared with chain/CChainActorList.hpp.
extern "C" int IdTable_SumValues(void*, u32);

// Float -> s32 conversion helper used by the arts-grid percentage functions
// (func_8023916C): takes the scaled float in f1 and returns the truncated
// integer in r3.
extern "C" s32 RoundHalfAway0(float);

// Float constant in the small data area (sda21-accessed via lfs, promoted to
// double for the arts-info sprintf vararg).
extern float lbl_eu_806686A0;

// Double constants in the small data area (sda21-accessed via lfd, used as
// vararg doubles in the arts-info sprintf calls).
extern double lbl_eu_806686C0;
extern double lbl_eu_806686C8;

// Double constant in the small data area (sda21-accessed via lfd): the
// 2^52 + 2^31 bias (4503601774854144.0) subtracted in MWCC's s32->float
// conversion trick.
extern const double lbl_eu_80668698;

// Double constant 2^52 (4503599627370496.0) in the small data area
// (sda21-accessed via lfd): the bias subtracted after the 0x4330 magic for
// MWCC's u32->float conversion.
extern double lbl_eu_806686A8;

// Float constant in the small data area (sda21-accessed via lfs) scaling the
// converted double in func_8023A460.
extern float lbl_eu_806686A4;

// Float constants in the small data area (sda21-accessed via lfs): scaling
// factors for the arts-level percentage computations in CArtsInfo_CalcDmg52.
extern float lbl_eu_80668690;
extern float lbl_eu_80668694;

// Float scale for the arts-level percentage computation in func_80239FC4
// (percentage = 806686BC * level + base, sda21-accessed via lfs).
extern float lbl_eu_806686BC;

// Float constants in the small data area (sda21-accessed via lfs): fixed
// values for the arts-type switch in func_80237E24 (field_0x55 0x68/0x69/0x70
// overrides the first grid value).
extern float lbl_eu_806686B0;
extern float lbl_eu_806686B4;
extern float lbl_eu_806686B8;

// Item slot-type flags for the func_80236E6C list loop (6 bytes: a u32
// followed by a u16 at 0x8066868C). Loaded as lwz + lhz, byte-indexed via
// the CArtsSlotFlags union.
extern u32 lbl_eu_80668688;
extern u16 lbl_eu_8066868C;

// Arts bdat file pointer used by func_80236E6C's final row lookup
// (BdatGetU16Direct, value passed as the first argument).
// void* (not u32): shared with kyoshin/cf/CItem.hpp (MWCC 10197).
extern void* lbl_eu_806640D8;

// --- CArtsInfo::OnFileEvent imports (retail C-linkage names) ---
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv(); // already above? keep single decl
extern "C" void setMemInitFlag__Q23mtl10MemManagerFb(bool);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void func_8013676C(nw4r::lyt::Pane*, u32);
extern "C" char* CUICfManager_getPackedFont9C(void);
extern "C" void setLayoutTextBoxFont(nw4r::lyt::Layout*, char*, u32);
extern "C" void buildLayout(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
extern "C" void bindLayoutAnimTransform(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int);
extern "C" char* MakeTplNameSysFile(u32);
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();
extern "C" void PaneSetTexPaletteByName(nw4r::lyt::Layout*, const char*, u32);
extern "C" void setPaneVisible(void*, u32);
extern "C" void setBdatEntry__5CBdatFUlPv(u32, void*);
extern "C" void* Bdat_GetTable_AA34();
extern "C" void* getFP__FPCc(const char* name);
extern "C" void func_8023B430(CArtsInfo* self);
extern "C" CArtsColorPair func_801397AC(void* pane, int idx);
extern "C" void CopyVec4s(CArtsQuadColor* dst, const CArtsColorPair* src);

// Float constant in the small data area (sda21-accessed via lfs): the anchor
// pane x-translate written by OnFileEvent.
extern float lbl_eu_806686D0;
