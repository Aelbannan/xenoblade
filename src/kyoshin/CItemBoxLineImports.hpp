#pragma once

// Retail helper declarations used by CItemBoxLine.cpp (not part of the
// class interface; kept out of CItemBoxLine.hpp so owners can include it).
#include "kyoshin/CItemBoxLine.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// CItemBoxInfo2 layout lives in CItemBoxInfo.hpp (not included: it redeclares
// __dt__17UnkClass_8045F564Fv with a void* arg, clashing with CSysWin.hpp's
// typed form). Only the pointer is used here (dtor dispatch).
struct CItemBoxInfo2;
class CEventFile;   // full layout in monolib/work/CEventFile.hpp (cpp includes it)
class CSysWin;      // full layout in kyoshin/CSysWin.hpp (not included, see above)
class CNumSelect;   // full layout in kyoshin/CNumSelect.hpp (not included, see above)
struct CScrollBar;  // full layout in kyoshin/CScrollBar.hpp (not included, see above)
class CFileHandle;

extern "C" void setPaneVisible(void* pane, u32 a); // set pane visible flag (C-ABI)
// Retail code80135FDC_setVec3 leaves its first arg (a pointer) in r3.
extern "C" void sysWinSwitchKindPane(void* syswin, int kind);   // syswin pane switch (CSysWin.cpp)
extern "C" void sysWinApplyKind(void* syswin, int kind);   // window-kind advance (CSysWin.cpp)

/* 8-byte r3:r4 copy unit (mirrored from CEquipItemBox.hpp; see include note). */
struct CEquipBoxFourShorts {
    s16 a, b, c, d;
};



/* 8-byte animation-color vector (4 shorts) returned by PaneMatGetS16QuadByName. */
struct FourShorts {
    s16 a, b, c, d;
};


/* Tab holder with a trailing FixStr<32> name buffer at +0x94 (ItemBoxLine_FormatTabEntryName). */
struct CIBLTabFormat {
    CIBLTabEntry entries[12]; // +0x00..0x8F
    u16 count;                // +0x90
    u8 pad_92[2];             // +0x92..0x93
    ml::FixStr<32> str94;     // +0x94
};

/* Full tab-format object used by ItemBoxLine_InitTabFormat (init), ItemBoxLine_AllocEntryStock
 * (name dispatch) and the ctor: 12 entries + halfword count + two selector
 * bytes + a FixStr<32> name (mLength at +0xB4) + a FixStr<128> name
 * (mLength at +0x138). The ctor writes the strings via strcpy/strlen,
 * matching the FixStr member layout. */
struct CIBLTabFull {
    CIBLTabEntry entries[12];   // +0x00..0x8F
    u16 count;                  // +0x90
    u8 field92;                 // +0x92 - selector byte (0xD = name-format mode)
    u8 field93;                 // +0x93
    ml::FixStr<32> str94;       // +0x94 (mString[32] + mLength at +0xB4)
    ml::FixStr<128> strB8;      // +0xB8 (mString[128] + mLength at +0x138)
};

/* Minimal vtable view of the embedded cursor object at +0x70. MWCC's vtable
 * prefix (offset-to-top + RTTI) shifts virtual index N to vtable+(N+2)*4, so
 * the 3rd virtual (index 2) sits at vtable+0x10 - the slot ItemBoxLine_PushSyswinCursor
 * dispatches through with the syswin-selection buffer. */
class CIBLCur70View {
public:
    virtual void vf00() = 0;        // index 0 -> +0x08
    virtual void vf01() = 0;        // index 1 -> +0x0C
    virtual void vf02(void* data) = 0; // index 2 -> +0x10 - ItemBoxLine_PushSyswinCursor
};

/* Vtable view of the CSysWin sub-object at +0x350. MWCC prefixes its vtables
 * with offset-to-top + RTTI, so virtual index N sits at vtable+(N+2)*4; the
 * file-load init (ItemBoxLine_LoadFiles) dispatches through the LAST CSysWin virtual
 * (index 32 -> vtable+0x88) with the syswin buffer. */
class CSysWinVtblView {
public:
    virtual void vf00() = 0;        // index 0 -> +0x08
    virtual void vf01() = 0;        // index 1 -> +0x0C
    virtual void vf02() = 0;        // index 2 -> +0x10
    virtual void vf03() = 0;        // index 3 -> +0x14
    virtual void vf04() = 0;        // index 4 -> +0x18
    virtual void vf05() = 0;        // index 5 -> +0x1C
    virtual void vf06() = 0;        // index 6 -> +0x20
    virtual void vf07() = 0;        // index 7 -> +0x24
    virtual void vf08() = 0;        // index 8 -> +0x28
    virtual void vf09() = 0;        // index 9 -> +0x2C
    virtual void vf10() = 0;        // index 10 -> +0x30
    virtual void vf11() = 0;        // index 11 -> +0x34
    virtual void vf12() = 0;        // index 12 -> +0x38
    virtual void vf13() = 0;        // index 13 -> +0x3C
    virtual void vf14() = 0;        // index 14 -> +0x40
    virtual void vf15() = 0;        // index 15 -> +0x44
    virtual void vf16() = 0;        // index 16 -> +0x48
    virtual void vf17() = 0;        // index 17 -> +0x4C
    virtual void vf18() = 0;        // index 18 -> +0x50
    virtual void vf19() = 0;        // index 19 -> +0x54
    virtual void vf20() = 0;        // index 20 -> +0x58
    virtual void vf21() = 0;        // index 21 -> +0x5C
    virtual void vf22() = 0;        // index 22 -> +0x60
    virtual void vf23() = 0;        // index 23 -> +0x64
    virtual void vf24() = 0;        // index 24 -> +0x68
    virtual void vf25() = 0;        // index 25 -> +0x6C
    virtual void vf26() = 0;        // index 26 -> +0x70
    virtual void vf27() = 0;        // index 27 -> +0x74
    virtual void vf28() = 0;        // index 28 -> +0x78
    virtual void vf29() = 0;        // index 29 -> +0x7C
    virtual void vf30() = 0;        // index 30 -> +0x80
    virtual void vf31() = 0;        // index 31 -> +0x84
    virtual void vf32() = 0;        // index 32 -> +0x88 - ItemBoxLine_LoadFiles tail dispatch
};





/* Color-source object returned by the nameplate pane's +0x68 virtual: the two
 * GXColorS10 words live at +0x10/+0x14 (copied into the +0x396 field). */
struct CIBLColorSrc {
    u8 pad_00[0x10];   // +0x00
    GXColorS10 color;  // +0x10
};

/* Vtable view of the font-info object returned by CDeviceFont::getFontInfo:
 * virtual index 7 (vtable+0x24) yields the pane data bound via ApplyFontToTxtPanes
 * (same scheme as COption::OnFileEvent). */
class CItemBoxFontView {
public:
    virtual void vf0() = 0;        // index 0 -> +0x08
    virtual void vf1() = 0;        // index 1 -> +0x0C
    virtual void vf2() = 0;        // index 2 -> +0x10
    virtual void vf3() = 0;        // index 3 -> +0x14
    virtual void vf4() = 0;        // index 4 -> +0x18
    virtual void vf5() = 0;        // index 5 -> +0x1C
    virtual void vf6() = 0;        // index 6 -> +0x20
    virtual void* vf7() = 0;       // index 7 -> +0x24 - font pane data
};

/* Minimal vtable view of the custom nameplate pane at pool+0x761: virtual
 * index 24 (vtable+0x68) returns the CIBLColorSrc colour source. */
class CIBLNameplateView {
public:
    virtual void vf00() = 0;            // index 0 -> +0x08
    virtual void vf01() = 0;            // index 1 -> +0x0C
    virtual void vf02() = 0;            // index 2 -> +0x10
    virtual void vf03() = 0;            // index 3 -> +0x14
    virtual void vf04() = 0;            // index 4 -> +0x18
    virtual void vf05() = 0;            // index 5 -> +0x1C
    virtual void vf06() = 0;            // index 6 -> +0x20
    virtual void vf07() = 0;            // index 7 -> +0x24
    virtual void vf08() = 0;            // index 8 -> +0x28
    virtual void vf09() = 0;            // index 9 -> +0x2C
    virtual void vf10() = 0;            // index 10 -> +0x30
    virtual void vf11() = 0;            // index 11 -> +0x34
    virtual void vf12() = 0;            // index 12 -> +0x38
    virtual void vf13() = 0;            // index 13 -> +0x3C
    virtual void vf14() = 0;            // index 14 -> +0x40
    virtual void vf15() = 0;            // index 15 -> +0x44
    virtual void vf16() = 0;            // index 16 -> +0x48
    virtual void vf17() = 0;            // index 17 -> +0x4C
    virtual void vf18() = 0;            // index 18 -> +0x50
    virtual void vf19() = 0;            // index 19 -> +0x54
    virtual void vf20() = 0;            // index 20 -> +0x58
    virtual void vf21() = 0;            // index 21 -> +0x5C
    virtual void vf22() = 0;            // index 22 -> +0x60
    virtual void vf23() = 0;            // index 23 -> +0x64
    virtual CIBLColorSrc* vf24() = 0;   // index 24 -> +0x68 - colour source
};


/* Page-data region read/written by ItemBoxLine_RefreshPageList. It overlaps the opaque
 * info2/num-select/scrollbar/syswin byte regions of CItemBoxLine; only the
 * named fields are used. Cast from &mInfo2D0[0xB0] (self+0x180). */
struct CIBLPageData {
    u16 pageWords180[12];       // +0x00 (self+0x180)
    CIBLVec3 pageVec198[12];    // +0x18 (self+0x198)
    u8 pageFlags228[12];        // +0xA8 (self+0x228)
    u8 pageFlags234[12];        // +0xB4 (self+0x234)
    u8 pad_C0[0xD8 - 0xC0];     // 0xC0..0xD7
    u8 pageSlot258;             // +0xD8 (self+0x258) - selected page slot (0..11, 8 if slot 8 occupied)
    u8 pad_D9[0x140 - 0xD9];    // 0xD9..0x13F
    s16 pageShorts2C0[12];      // +0x140 (self+0x2C0)
    u8 pad_158[0x360 - 0x158];  // 0x158..0x35F
    u16 pageWords4E0[12];       // +0x360 (self+0x4E0)
    CIBLVec3 pageVecs4F8[12];   // +0x378 (self+0x4F8) - written by ItemBoxLine_BuildPageData
    u8 pageBytes588[12];        // +0x408 (self+0x588)
    u8 pageBytes594[12];        // +0x414 (self+0x594)
    s16 pageShorts5A0[12];      // +0x420 (self+0x5A0)
};

// ---------------------------------------------------------------------------
// C-linkage imports - moved verbatim from CItemBoxLine.cpp. The names ARE the
// retail linker symbols; do NOT rename or "fix" the mangled forms.
// ---------------------------------------------------------------------------

// CIBLTabCur vtable (.data)
extern "C" void* lbl_eu_80534D80[];

// CBaseCur ctor/dtor + operator delete (defined in CCur unit).
extern "C" void __ct__8CBaseCurFv(CBaseCur* _this);
extern "C" void* __dt__8CBaseCurFv(CBaseCur* _this, int flags);

// Sub-object destructors invoked by ~CItemBoxLine (retail C-ABI names; the
// member-class dtors live in their own TUs as extern "C" free functions).
extern "C" void __dt__7CSysWinFv(CSysWin*, int);
extern "C" void __dt__10CScrollBarFv(CScrollBar*, int);
extern "C" void __dt__10CNumSelectFv(CNumSelect*, int);
extern "C" void __dt__13CItemBoxInfo2Fv(CItemBoxInfo2*, int);
extern "C" void __dt__7CSubCurFv(CBaseCur*, int);

// --- external retail helpers (C-linkage or literal mangled symbols) ---
// OnFileEvent helpers (external retail symbols, plain C-ABI names).
extern "C" void* Bdat_GetTable_AA34();                           // bdat manager reset (no args)
extern "C" void* __ct__CTagProcessor(void*);               // tag-processor ctor (self)
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
void setLayoutTextBoxFont(nw4r::lyt::Layout*, char*, u32);         // pane text bind (mangled retail name)
extern "C" char* CUICfManager_getPackedFont9C(void);                      // line text string
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();  // cursor accessor (CCur unit)
extern "C" void MenuShopSellCopyCursor(void*, void*);               // copy stack cursor temp into member
// Cur_ShowTwoSubPanes (subcur pane visibility) - 2-arg form used by OnFileEvent;
// ItemBoxLine_ResetCursorB8 still calls it through a 1-arg cast to keep the 8-byte tail.
extern "C" void Cur_ShowTwoSubPanes(void*, u32);
extern "C" u32 getItemBox2State__FP13CItemBoxInfo2(void*);
extern "C" u32 NumSel_GetActiveFlag_B020(void*);          // num-select busy flag (+0x2C)
extern "C" u32 NumSel_GetField2D_B018(void*);          // num-select busy flag (+0x2D)
extern "C" int CSysWin_getUnk34(void*);
extern "C" void advanceItemBox2State__FP13CItemBoxInfo2(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" int CSysWin_isActive(void*);
extern "C" void Cur_SetVisible(void*, u8);
extern "C" void sysWinGetPaneScreenPos(void*, void*, u8);
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void calcItemBox2PaneVec(void*, void*, unsigned int);
extern "C" u32 BdatGetItemType(u32);
extern "C" u16 BdatGetItemId(u32);
extern "C" void CItem_thunkAllocRecord(u32, u32);
extern "C" void* CItemBlock_getPtr20E8(void);
extern "C" void CItemBlock_setCount(s32);
extern "C" void NumSel_SetCaption_B030(void*, void*);
extern "C" void NumSel_SetCaptionNum_B04C(void*, u8);
extern "C" void NumSel_FormatPicName_B064(void*, unsigned int);
extern "C" void NumSel_EnterState1Close_B0D4(void*);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv(void);
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" int KyoshinHeap_GetField44(void);
extern "C" void loadItemBox2Files(void*);
extern "C" void NumSel_LoadArchive_AE8C(void*);
// per-frame update helpers (external retail symbols, same merged unit)
extern "C" void Cur_Update(void*);            // cursor per-frame update (CCur)
extern "C" void updateItemBox2Anims(void*);            // item-box info2 per-frame update
extern "C" void NumSel_DispatchState_AED4(void*);            // num-select per-frame update
extern "C" void sysWinDispatchPhase(void*);            // syswin per-frame update
extern "C" void startItemBox2Open(void*);                // info2 state open
extern "C" void setItemBox2NamedText(void*, char*, char*);  // info2 tab-name set
extern "C" u8 ItemBoxLine_GetTabEntryLock(void*, unsigned int);   // tab entry byte accessor (external overload)
extern "C" u8 ItemBoxLine_GetTabEntryReady(void*, unsigned int);    // tab entry byte accessor (external overload)
extern "C" int ItemBoxLine_ProbeNameSlot(void*, unsigned int);   // name-dispatch lookup (external overload)
// nav-state dispatch helpers (external overloads of the C++ bodies in this TU)
extern "C" void* lbl_eu_80664104;
extern "C" void* lbl_eu_806640A8;
extern "C" void* lbl_eu_806640EC;
extern "C" void* lbl_eu_806640D8;
extern "C" void drawItemBox2Layout__FP13CItemBoxInfo2PQ34nw4r3lyt8DrawInfo(void*, void*);
extern "C" void Cur_DrawLayout(void*, void*);
extern "C" void NumSel_DrawLayout_AF7C(void*, void*);
extern "C" void sysWinDrawLayout(void*, void*);
extern "C" int Cur_BothSubPanesOn(CBaseCur*);

// Forward declarations for sibling functions resolved at link via the symbol map
// (retail treats these as external relocs, not same-TU symbols). The C++
// definitions in this TU mangle to ItemBoxLine_RefreshPageList__FP12CItemBoxLine etc.; these
// C-linkage overloads keep the HEAD callers' (void*) calls external/unmangled.

// Layout/anim builders (code_80135FDC unit) used by ItemBoxLine_BuildLineLayout. The
// unmangled C++ forms mangle to the retail symbols
// (buildLayout__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc /
// bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc).
void buildLayout(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void bindLayoutAnimTransform(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
// Cursor deactivation tail handler (CCur unit) - genuine C-ABI helper.
extern "C" void Cur_DeactivateTail(void*);

extern "C" char lbl_eu_805071B0[]; // rodata string pool
extern const float lbl_eu_80668114; // .sdata2 animation-completion threshold (lfs-hoist: const routes it to readonly sdata2)
extern const float lbl_eu_80668124; // .sdata2 tab-slot float divider (page-slot -> page index conversion)
extern const float lbl_eu_806680F8; // .sdata2 tab-pane placement z (setVec3)
extern const float lbl_eu_8066811C; // .sdata2 tab-pane placement x
extern const float lbl_eu_80668120; // .sdata2 tab-pane placement y

// ItemBoxLine_ShowSlotCounts: .sdata2 name/ratio constants (divisor + multipliers).
extern const float lbl_eu_80668128;
extern const float lbl_eu_8066812C;
extern const float lbl_eu_80668130;
// u8->float / int->float conversion magics (MWCC emits its own TU-local pool
// constants for these; the retail names are kept for reference).
extern const double lbl_eu_80668108;
extern const double lbl_eu_80668100;

// ItemBoxLine_ShowSlotCounts: character-data / item helpers (C-ABI).
extern "C" u32 CtrlObjectParam_GetArtsDataWord(void*);
extern "C" float GetFloatTableEntry(unsigned int);

// ItemBoxLine_BuildTabEntries: tab-format / item-name helpers + rodata tables.
extern const char lbl_eu_80506D90[];        // rodata name/format tables
// ItemBoxLine_FormatDisplayName: language selector (CDeviceS static getter, C-ABI).
extern "C" u8 getLanguage__9CDeviceSCFv();
extern u32 lbl_eu_80664098;
extern u32 lbl_eu_80664184;                 // name-index byte (sda21)

extern "C" u32 MapValueToRank6(unsigned int);
extern "C" u16 CtrlRemote_TouchBitByArg(u32);
extern "C" u32 ItemBoxLine_CalcCountKey(void*, unsigned int);

// Tab-accessor C-linkage overloads: the typed C++ bodies live in this TU
// (their mangled names map to the retail symbols); these C-linkage
// declarations keep the cross-.o calls (retail external relocs) external
// instead of inlining the local body.
extern "C" void* ItemBoxLine_FormatTabEntryName(void*, unsigned int);
extern "C" u32 ItemBoxLine_GetTabEntryValue(void*, unsigned int);
extern "C" u16 ItemBoxLine_GetTabEntryItem(CIBLTab*, unsigned int);
extern "C" u8 ItemBoxLine_ProbePageSlot(void*, unsigned int);

// Sibling tab-dispatch helpers in this merged unit (external relocs in retail).
extern "C" void BuildSlotItemLists(unsigned int);
extern "C" void PaneMatSetTevColorsByName(nw4r::lyt::Layout*, char*, void*, void*);
extern "C" char* getErrMesText16(void);
extern "C" char* getErrMesText15(void);
// Syswin content-setter helpers (defined in CSysWin.cpp) + item-name provider
// (defined in CItemBoxInfo.cpp): C-linkage so calls bind to the retail names.
extern "C" void sysWinSetKindContent(void*, u32, int);
extern "C" void sysWinOpenPhase1(void*);
extern "C" u32 formatItemBoxName(void*, u32);

// Layout/anim builders + text/table helpers declared by the code_80135FDC unit.
// setLayoutTextBoxNumber is a C++ function (mangled retail name); the text helpers are
// C-ABI (plain retail names).
extern "C" void setLayoutTextBoxNumber__FPQ34nw4r3lyt6LayoutPcUc(nw4r::lyt::Layout*, char*, u32);

// Item-box-line refresh helpers (external retail symbols).
extern "C" void ItemBoxLine_BuildTabEntries(CIBLTab*, unsigned char, unsigned short, unsigned char);

extern "C" void sysWinAdvancePhase3(void*);
extern "C" int NumSel_GetField2E_B028(void*);
extern "C" void NumSel_ShowPaneB_B314(CNumSelect* self); // num-select display step
extern "C" void NumSel_EnterState3Open_B178(void*);

// --- teardown helpers called by ItemBoxLine_UnloadFiles (retail plain names) ---
extern "C" void NumSel_Teardown_AF9C(CNumSelect*);          // num-select destroy
// C++ mangling reproduces the retail names closeFileHandle__FPP11CFileHandle /
// releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor.
void closeFileHandle(CFileHandle**);
void releaseArcResourceAccessor(nw4r::lyt::ArcResourceAccessor*);
extern "C" void closeItemBox2(CItemBoxInfo2*);           // info2 destroy
extern "C" void sysWinTermLayout(CSysWin*);             // syswin destroy
// Tab-page helpers used by ItemBoxLine_RefreshPageList (retail plain names).
extern "C" void CopyTabSlotVec(CIBLVec3*, const u16*, u32);
extern "C" void copyVEC3(void*, const void*);
extern "C" u16 ArrayGet12(const u16*, u8);
extern "C" u8 ItemBoxGetTab(const u8*, unsigned int);   // same-TU body kept external
extern "C" char* ItemBoxLine_FormatDisplayName(CIBLTabFull*, u16);    // tab-format setter (external call shape)

// code_80135FDC string/table helpers (retail unmangled C-ABI names).
// KEEP (blockers-A 2026-10-23): this TU models every BDAT table as a u32 handle
// (LookupItemTypeTable, lbl_eu_8066464C/806640F4/...), so it keeps the u32-table
// view of BdatGetU16Direct instead of kyoshin/code_80135FDC_Bdat.hpp until those
// table globals are retyped.
extern "C" char* MakeTplNameSysFile(u32);

// .sbss item-kind owner tables (u32 values; SDA-accessed).
extern u32 lbl_eu_806640F4;
extern u32 lbl_eu_806640F8;

// ItemBoxLine_ProbeAvailability: item-kind table base (character-data owner table).
extern u32 lbl_eu_80664110;

// ItemBoxLine_ProbePageSlot: character-data lookup helpers (C-ABI).
extern "C" u32 CItemBlock_countEmpty(u8);
extern "C" u32 CItem_sumFamilyByte6(unsigned int);

// ItemBoxLine_ProbeAvailability: string/kind-table helpers (C-ABI).
extern "C" void* CtrlObjectParam_GetCharEntry(u32);
extern "C" u32 CtrlObjectParam_GetCurrentRowKey();

// sinit_801F32EC: static color-object ctor helpers (C-ABI).
extern "C" void SplitU32ToS16s(void*, u32);
extern "C" FourShorts PaneMatGetS16QuadByName(void*, void*, u32);  // animation color fetch
// GXColorS10 copy (retail __as__11_GXColorS10FRC11_GXColorS10)
extern "C" void setGXColorS10(void*, u32, u32, u32, u32); // RGB color builder

// ItemBoxLine_CompactTabList: .sbss item-box guard words (SDA-accessed, compared to 0).
extern u32 lbl_eu_80664648;
extern u32 lbl_eu_8066464C;
extern u32 lbl_eu_80664650;

// sinit_801F32EC: the fourteen 8-byte .sbss color objects, constructed in
// retail call order (SplitU32ToS16s resets, setGXColorS10 sets RGBA).
extern u8 lbl_eu_806645D8[8];
extern u8 lbl_eu_806645E0[8];
extern u8 lbl_eu_806645E8[8];
extern u8 lbl_eu_806645F0[8];
extern u8 lbl_eu_806645F8[8];
extern u8 lbl_eu_80664600[8];
extern u8 lbl_eu_80664608[8];
extern u8 lbl_eu_80664610[8];
extern u8 lbl_eu_80664618[8];
extern u8 lbl_eu_80664620[8];
extern u8 lbl_eu_80664628[8];
extern u8 lbl_eu_80664630[8];
extern u8 lbl_eu_80664638[8];
extern u8 lbl_eu_80664640[8];

// ItemBoxLine_RebuildTabList: per-slot tab dispatch (C-ABI; defined in this TU).
// Retail helpers whose CItemBoxInfo.hpp declarations use other parameter types
// (CItemBoxInfo.hpp's forms are renamed at its include above).
extern "C" FourShorts PaneMatGetS16Quad(void*, u32);                   // pane colour getter (r3:r4 pair)
extern "C" u32 LookupItemTypeTable(u32);
extern "C" void renderItemBox2ByKind(void*, unsigned short, void*, unsigned short, unsigned int); // info2 tab-data push
extern "C" void* BuildItemNameStr(u32);
extern "C" void BlendFloatAvgScale(u16, u16, u32, float);
extern "C" int Counter_TestBit(u32, u32);
extern "C" u32 GetCollectedFlagByte(u32);
extern "C" GXColorS10* __as__11_GXColorS10FRC11_GXColorS10(GXColorS10* self, const GXColorS10* src);
extern "C" void NumSel_ShowPaneB_B314(CNumSelect* self); // num-select display step
extern "C" void NumSel_Teardown_AF9C(CNumSelect*);          // num-select destroy
extern "C" void sysWinTermLayout(CSysWin*);             // syswin destroy

