#pragma once

#include <types.h>

#include <nw4r/lyt.h>

#include <monolib/scn/IScnRender.hpp>
#include <monolib/work/CProcess.hpp>

#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp" // CTitleAHelp (0x38 subobject at menu +0x80)

class CScn;

// Minimal CTaskGame decl (retail global-namespace class; symbols
// getInstance__9CTaskGameFv / func_800426F0__9CTaskGameFv).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// 8-byte award list entry (array element; zeroed by func_802706C4 and freed by
// __dt__802706D4).
struct CPlayAwardEntry {
    u32 word0;
    u32 word1;
};

// Layout of the entry-array region (CPlayAwardList + 0x94): 512 entries
// (0x1000 bytes) followed by the trailing state fields at +0x1000..+0x100F
// (initialised by func_80270654; page index for entry lookup at +0x100A).
struct CPlayAwardEntryArray {
    CPlayAwardEntry mEntries[0x200]; // 0x00-0xFFF
    u8 field_1000;                   // 0x1000
    u8 field_1001;                   // 0x1001
    u8 _1002[0x1004 - 0x1002];       // 0x1002-0x1003
    f32 field_1004;                  // 0x1004
    u8 field_1008[2];                // 0x1008-0x1009: per-page entry counts (field_1008[page])
    u8 mPageIndex;                   // 0x100A: page index for entry lookup (<< 11)
    u8 _100B;                        // 0x100B
    s16 field_100C;                  // 0x100C (s16; func_80270F74 reads it via a u16 cast)
    s16 field_100E;                  // 0x100E (s16; func_80270F74 reads it via a u16 cast)
};

// Mirror of the CScrollBar layout (0x40 bytes) for the raw body copy in the
// CPlayAwardList ctor (same struct as CSortMenu.hpp; CScrollBar.hpp cannot be
// co-included with CTitleAHelp.hpp here).
struct CScrollBarData {
    void* mVtbl;         // +0x00
    u32 mMemRegion[4];   // +0x04 (UnkClass_8045F564)
    void* mFileHandle;   // +0x14
    void* mAccessor;     // +0x18
    void* mLayout;       // +0x1C
    void* mAnimTrans;    // +0x20
    u8 mReady;           // +0x24
    u8 mVisible;         // +0x25
    u8 mState;           // +0x26
    u8 mActive;          // +0x27
    f32 mAnimOffset;     // +0x28
    f32 mScrollPosY;     // +0x2C
    f32 mScrollRatio;    // +0x30
    f32 mThumbHeight;    // +0x34
    f32 mContentHeight;  // +0x38
    u8 mDirection;       // +0x3C
};

// Minimal mirror of cf::CfPadData (see kyoshin/cf/CfPadData.hpp; the real
// struct pulls monolib/core.hpp which this TU's include set avoids).
struct CPlayAwardPadData {
    u32 mPadHeldFlags;          // +0x00: CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // +0x04: CPad::mPressedButtonFlags
    u8 _pad[0xF8 - 0x08];       // +0x08-0xF7: rest of CPad
    u32 mHeldButtonFlags;       // +0xF8
    u32 mPrevHeldButtonFlags;   // +0xFC
    u32 mPressedButtonFlags;    // +0x100
    u32 mTurboPressButtonFlags; // +0x104
};

// Award texture object returned by ArcResourceAccessor::GetResource("timg")
// (CQstLogList's CQstLogListMsgObj pattern): chain pointer at +0x08, the
// chain's coords at +0x00 (column u16 at +0x00, row u16 at +0x02).
struct CPlayAwardMsgChain {
    struct CPlayAwardCoords* pCoords; // +0x00
};
struct CPlayAwardCoords {
    u16 col; // +0x00
    u16 row; // +0x02
};
struct CPlayAwardMsgObj {
    u32 field_0;                // +0x00
    u32 field_4;                // +0x04
    CPlayAwardMsgChain* chain;  // +0x08
};

// Mirror of nw4r::lyt::Pane's protected mSize region (cf. CSortMenu.hpp).
struct CPlayAwardPaneSize {
    u8 pad[0x4C];  // +0x00-0x4B
    f32 width;     // +0x4C
    f32 height;    // +0x50
};

// View into a nw4r::lyt::Pane's trailing user-data region: OnFileEvent writes
// the tag-processor pointer (field_0x2C) into the pane at +0xF8 after binding
// each award pane's name.
struct CPlayAwardPaneUser {
    u8 pad[0xF8];   // +0x00-0xF7
    u32 field_F8;   // +0xF8
};

// Field view of the opaque CCur18 cursor sub-object (CBaseCur layout +
// padding, same as CQstLogList.hpp). Used to copy a stack-constructed CCur18
// into the embedded mCursor region without touching the +0x00 vtable pointer
// (retail copies +0x04..+0x15).
struct CCur18Data {
    void* mVtbl;    // +0x00 - not copied
    void* field_4;  // +0x04
    void* field_8;  // +0x08
    void* field_C;  // +0x0C
    void* field_10; // +0x10
    u8 field_14;    // +0x14
    u8 field_15;    // +0x15
};

// Abstract view into the embedded CCur18 cursor vtable (same scheme as
// CMenuGameClear/CSysWinSave): MWCC prepends 2 entries (offset-to-top + RTTI),
// so vf3 lands at vtable +0x0C (teardown virtual) and vf4 at +0x10 (cursor
// Move virtual taking a VEC3*). Pure virtual so MWCC emits no vtable.
class CCursor18 {
public:
    virtual void vf2() = 0;                     // slot 2 (+0x08)
    virtual void vf3() = 0;                     // slot 3 (+0x0C)
    virtual void vf4(const nw4r::math::VEC3*) = 0; // slot 4 (+0x10): Move
};

// View into the object returned by CDeviceFont::func_80452C10 (cf.
// CMenuBattleCommu.hpp): with MWCC's 2-entry vtable prefix the 8th virtual
// lands at +0x24 and returns the font handle passed to func_8013676C.
// Never instantiated here, so no vtable is emitted.
class CPlayAwardFontObj {
public:
    virtual void vf08();              // +0x08
    virtual void vf0C();              // +0x0C
    virtual void vf10();              // +0x10
    virtual void vf14();              // +0x14
    virtual void vf18();              // +0x18
    virtual void vf1C();              // +0x1C
    virtual void vf20();              // +0x20
    virtual u32 getFontHandle();      // +0x24
};

// Object behind CPlayAwardList::field_0x2C: only observed uses are `delete`
// through the virtual dtor (vtable +0x08) and passing the pointer bits as the
// second award text attr (func_80271680). Never constructed here, so the dtor
// stays undefined and no vtable is emitted.
class CPlayAwardAttrObj {
public:
    virtual ~CPlayAwardAttrObj();
};

class CPlayAwardList : public IWorkEvent {
public:
    CPlayAwardList();
    virtual ~CPlayAwardList();
    bool OnFileEvent(CEventFile* event);

    // Layout (IWorkEvent base at 0x00-0x03, then members):
    // 0x04-0x13: UnkClass_8045F564 layout memory region
    // 0x14/0x18: CDeviceFile read handles (readFile / readCommonArchiveFile)
    // 0x20:      Layout* render target (drawn by func_80270E04)
    // 0x24:      AnimTransform* reverse-advance target (func_80271574/80271480)
    // 0x28:      AnimTransform* reverse-advance target (func_80271528)
    // 0x30-0x47: cursor region (drawn via func_801D20B0)
    // 0x48-0x87: CScrollBar region (0x40, opaque)
    // 0x88:      render gate (nonzero enables func_80270E04) | 0x89: state byte
    // 0x8A/0x8B: flag bytes
    // 0x8C/0x8E: s16 scroll position fields | 0x90: page byte | 0x94: entry array
    UnkClass_8045F564 mMemRegion;           // 0x04-0x13: layout memory region
    CFileHandle* mFileHandle;               // 0x14: readFile handle
    CFileHandle* mFileHandle2;              // 0x18: readCommonArchiveFile handle
    nw4r::lyt::ArcResourceAccessor* mArcAccessor1C; // 0x1C: layout arc accessor (freed by func_80270E64)
    nw4r::lyt::Layout* mLayout20;           // 0x20: layout drawn by func_80270E04
    nw4r::lyt::AnimTransform* mAnimTrans24; // 0x24
    nw4r::lyt::AnimTransform* mAnimTrans28; // 0x28
    u32 field_0x2C;                         // 0x2C: second award text color/attr (func_80271680)
    u8 mCursor[0x18];                       // 0x30-0x47: cursor region (func_801D20B0)
    u8 mScrollBar[0x40];                    // 0x48-0x87 (CScrollBar region)
    u8 field_0x88;                          // 0x88: render gate checked by func_80270E04
    u8 field_0x89;                          // 0x89: state byte (5/0/2 on transitions)
    u8 field_0x8A;                          // 0x8A: checked by func_80270F28
    u8 field_0x8B;                          // 0x8B: some state/flag byte
    s16 field_0x8C;                         // 0x8C: scroll position (halfword)
    s16 field_0x8E;                         // 0x8E: scroll offset (halfword)
    s8 field_0x90;                          // 0x90: page byte used by func_80271468
    u8 _pad91[0x94 - 0x91];                 // 0x91-0x93
    CPlayAwardEntryArray mEntryArray;       // 0x94: 512-entry array + state fields
};

class CMenuPlayAward : public CProcess {
public:
    CMenuPlayAward();
    virtual ~CMenuPlayAward();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // Layout (CProcess base at 0x00-0x3B, then members). Menu-screen pattern
    // (cf. CMenuTutorialList): ptmf callback slots at 0x3C-0x53, state byte
    // at 0x54, IScnRender subobject at 0x58, CScn* at 0x5C, CBgTex at
    // 0x60-0x7F, CTitleAHelp at 0x80-0xB7, CPlayAwardList at 0xB8,
    // help-display byte at 0x1160.
    u32 ptmf0[3];                                              // 0x3C-0x47: null PMF callback slot group 1
    u32 ptmf1[3];                                              // 0x48-0x53: null PMF callback slot group 2
    u8 mField54;                                               // 0x54: state byte (set to 1 by func_802705F4)
    u8 mField55;                                               // 0x55
    u8 _pad56[0x58 - 0x56];                                    // 0x56-0x57
    IScnRender mIScnRender;                                    // 0x58-0x5B: render-callback subobject (vptr)
    CScn* mScene;                                              // 0x5C: owning scene
    CBgTex mBgTex;                                             // 0x60-0x7F
    CTitleAHelp mTitleAHelp;                                   // 0x80-0xB7
    CPlayAwardList mPlayAwardList;                             // 0xB8
    f32 mField115C;                                            // 0x115C: float state (ctor: lbl_eu_80668998)
    u8 mField1160;                                             // 0x1160: help-display byte (set to 2 by func_80270404)
    u8 _pad1161[0x1164 - 0x1161];                              // 0x1161-0x1163: pad to total 0x1164
};

// MWCC runtime operator delete (freeing dtor helper).
extern "C" void* __dl__FPv(void*);

// .sdata2 float pool: 1.0f -- animation frame step for the award list
// open/close transitions (func_80271528 / func_80271574 / func_80271480).
extern const float lbl_eu_806689C0;

// .sdata2 float pool: entry-array init constant (func_80270654) and the
// scrollbar init vector for func_80270F74 (x/y at 806689B8/BC, z at 806689A8).
extern const float lbl_eu_806689A8;
extern const float lbl_eu_806689B8;
extern const float lbl_eu_806689BC;

// .sdata2 float pool: award-list menu frame-step (8066899C) and clamp
// (806689A0) for the mField115C accumulator in func_80270454.
extern const float lbl_eu_8066899C;
extern const float lbl_eu_806689A0;

// .sdata2 double pool: 2^52 (0x4330000000000000) -- unsigned->float conversion
// constant for the award texture row/column sizes (func_802717F8). The lfd
// reloc binds a TU-local pool entry (@N) that cannot be named in source
// (makecrystal precedent); the value matches the retail symbol.
extern const double lbl_eu_806689B0;

// Menu-open singleton (0 = closed; cleared by Term, checked by func_80270308).
extern unsigned long lbl_eu_806648A0;

// Loaded-BDAT pointer for the award tables (stored by OnFileEvent branch 2
// from getFP; read by func_80270770's sort/list build).
extern void* lbl_eu_806648A8;

// Second string pool (split .rodata): title/help arc paths used by Init.
extern char lbl_eu_8050E7A0[];

// .sdata2 float pool: award-list page-progress scale for func_80270770's
// field_1004 ratio (const * field_1000 / field_1001).
extern const float lbl_eu_806689AC;

// Render-gate mode bitfield (.sbss; bit 21 = task busy, gates cbRenderBefore).
extern u32 lbl_eu_80663E28;

// Shared string pool (split .rodata): award-list arc file paths at +0x5A / +0x74.
extern char lbl_eu_8050E7C0[];

// CProcess primary vtable (pre-PMF copy) / CMenuPlayAward composite vtable
// and the null PMF callback slots (ctor vtable fixups; same scheme as
// CMenuTutorialList.hpp). MWCC does not mangle global-scope data names.
extern char lbl_eu_8052BF70[];
extern char lbl_eu_80537BB0[];
extern char lbl_eu_80537C20[];   // CPlayAwardList vtable (stored at +0x00 by its ctor)
extern u32 __ptmf_null[3];

// .sdata2 float pool: menu init float (ctor stores to +0x115C).
extern const float lbl_eu_80668998;

// MEM2 alloc-handle query (retail-unmangled; C linkage so the zero-arg name
// stays bare instead of mangling to func_800A9D90__Fv).
extern "C" int func_800A9D90();

// Scene-active query (retail-unmangled; gates cbRenderBefore draws).
extern "C" int func_8013BE50();

// DrawInfo layout setup (retail MANGLED name func_80137250__FPQ34nw4r3lyt8DrawInfo;
// declared with C++ linkage so MWCC re-derives the mangled reloc).
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// Layout DrawInfo ctor/dtor (retail keeps the mangled names verbatim). A
// class-typed local would make MWCC virtual-dispatch the dtor, so cbRenderBefore
// uses a plain byte buffer with these direct C-linkage calls (CSysWinSave idiom).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(u8* self);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self, int flags);

// Retail-unmangled free functions of this unit (US retail strips the
// mangling). C linkage keeps call relocs on the bare retail names;
// __declspec(noinline) keeps the same-TU calls as real `bl` instructions.
// Return types are int (not u8) so callers compare with cmpwi directly.
extern "C" int __declspec(noinline) func_80270F6C(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_802715C0(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_80271620(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_80271730(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_80270E64(CPlayAwardList* self);
extern "C" int __declspec(noinline) func_80270F28(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_80270F74(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_80271480(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_802714D4(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_80271528(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_80271574(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_80271680(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_802713BC(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_802710D4(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_80271300(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_80271190(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_802719F8(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_802717F8(CPlayAwardList* self);
extern "C" void func_80270770(CPlayAwardEntryArray* self);
extern "C" void __declspec(noinline) func_80270E04(CPlayAwardList* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" __declspec(noinline) u8* func_80270AEC(CPlayAwardEntryArray* self, int param);

// Menu phase handlers + per-frame list update (retail-unmangled names; same
// C-linkage scheme as the rest of this unit -- keeps call relocs on the bare
// retail names).
extern "C" void func_8027038C(CMenuPlayAward* self);
extern "C" void func_80270404(CMenuPlayAward* self);
extern "C" void func_80270454(CMenuPlayAward* self);
extern "C" void func_802705F4(CMenuPlayAward* self);
extern "C" void func_80270D64(CPlayAwardList* self);
extern "C" void func_80270CEC(CPlayAwardList* self);
extern "C" void func_80270AD8(CPlayAwardEntry* dst, const CPlayAwardEntry* src);
extern "C" void func_80271260(CPlayAwardList* self);
extern "C" s32 __declspec(noinline) func_80271468(CPlayAwardList* self);

// Award-list state handlers / helpers (retail-unmangled names; see above).
extern "C" void __declspec(noinline) func_80271070(CPlayAwardList* self);
extern "C" __declspec(noinline) CPlayAwardEntryArray* func_80270654(CPlayAwardEntryArray* self);

// Menu factory (retail-unmangled): allocate + construct the singleton menu,
// register it as a child of `parent`, return the stored instance.
extern "C" __declspec(noinline) CMenuPlayAward* func_80270308(CProcess* parent, CScn* scene);

// Retail constructor symbol (unmangled global in US). Free-function form so
// the factory (func_80270308) emits a real bl to the bare retail symbol;
// returns `this` in r3 like retail.
extern "C" CMenuPlayAward* __ct__CMenuPlayAward(CMenuPlayAward* self, CScn* scene);

// Embedded-widget ctors called by the menu ctor (US retail strips these
// manglings; C-ABI wrappers, same scheme as CMenuTutorialList.hpp).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
extern "C" __declspec(noinline) CPlayAwardList* __ct__CPlayAwardList(CPlayAwardList* self, int arg);

// Title/help bar idle query -- free-function form of CTitleAHelp::isIdle()
// (retail reloc is the mangled member name; declared with int return so the
// caller's `!= 0` test compiles to a direct cmpwi, cf. CMenuTutorialList.hpp).
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);

// CBgTex/CTitleAHelp widget helpers (retail-unmangled callee names; US retail
// strips the mangling for these widget helpers, cf. CMenuTutorialList.hpp).
// Return types are int (not u8) so callers compare with cmpwi directly.
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" int func_801C3E34(CBgTex* self);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);

// Per-frame widget updates (retail-unmangled; called by CMenuPlayAward::Move).
extern "C" void func_801C3D54(CBgTex* self);
extern "C" void func_801C3FF0(CTitleAHelp* self);

// Title/help bar close + mode-set helpers (func_80270454).
extern "C" void func_801C414C(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);

// CfGameManager controller-type query + save/skip busy queries (retail
// pre-mangled / unmangled C symbols; same scheme as CMenuTutorialList.hpp).
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
extern "C" int func_800FEDF8();
extern "C" void func_800FF914();

// Shared layout-arc resource manager (retail-unmangled; ArcResourceAccessor
// with GetResource at vtable +0x0C, used by func_802717F8).
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();

// CfGameManager controller-type query (retail pre-mangled name; declared
// under C linkage so calls bind the literal identifier).
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u8 enable);

// CScrollBar show request (retail-unmangled import). The member declaration in
// CScrollBar.hpp would mangle the call reloc (func_801F367C__10CScrollBarFv),
// and CSortMenu.hpp cannot be co-included with CTitleAHelp.hpp (its extern
// UnkClass_8045F564 ctor conflicts with the member ctor CTitleAHelp pulls in).
extern "C" void func_801F367C(u8* scrollBar);

// Scroll-bar init (retail-unmangled; 2nd arg is a 3-float vector, cf. CMapSel).
extern "C" void func_801F3670(u8* scrollBar, const float* vec);

// CScrollBar / CCur18 embedded-widget ctors (retail-unmangled names).
extern "C" void __ct__CScrollBar(u8* scrollBar, int arg);
extern "C" void __ct__CCur18(u8* cursor, void* arg);

// CScrollBar draw + cursor draw imports (retail-unmangled names; same reason
// as func_801F367C -- the CScrollBar.hpp member decl would mangle them).
extern "C" void func_801F35B0(u8* scrollBar, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801D20B0(u8* cursor, nw4r::lyt::DrawInfo* drawInfo);

// Cursor/scrollbar helpers used by the close sequence (func_80271070).
extern "C" void func_801D216C(u8* cursor, u8 val);
extern "C" void func_801F369C(u8* scrollBar);

// Layout text-binding helper (retail-unmangled; 3rd arg is the raw text word).
extern "C" void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);

// Pane-position translation helper (retail-unmangled): resolve a position in
// pane1's space into pane2/root space (cf. CSelShopWin/CSysWin usage).
extern "C" void func_80137924(nw4r::math::VEC3*, nw4r::lyt::Pane*, nw4r::lyt::Pane*, nw4r::lyt::Pane*);

// Scroll-bar layout load + per-frame update/draw helpers, and the cursor
// update helper (retail-unmangled names; CScrollBar.hpp cannot be included).
extern "C" void func_801F34F4(u8* scrollBar);
extern "C" void func_801F35DC(u8* scrollBar);
extern "C" void func_801F3540(u8* scrollBar);
extern "C" void func_801F36BC(u8* scrollBar, int, int);
extern "C" void func_801F3850(u8* scrollBar, u16 val);
extern "C" void func_801D202C(u8* cursor);

// Embedded-widget destructors (retail mangled __dt__ names; the widget
// storages are opaque u8 arrays so the dtor calls are C-ABI imports).
extern "C" void __dt__10CScrollBarFv(u8* scrollBar, int dealloc);
extern "C" void __dt__6CCur18Fv(u8* cursor, int dealloc);

// Class_8045F858 scratch-region RAII guard used by OnFileEvent around the
// layout build (retail C-linkage names; the class itself is declared in
// monolib/lib/UnkClass_8045F564.hpp).
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
extern "C" void __dt__14Class_8045F858Fv(void*, int);

// 4-word mem-region copy helper used by Init to copy temp widget bodies into
// the embedded members (retail-unmangled C symbol).
extern "C" void __ct__UnkClass_8011C974(void* dst, void* src);

// OnFileEvent / Init layout-build imports (retail-unmangled C symbols).
extern "C" void* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void func_80434A4C__Q23mtl10MemManagerFb(bool);
extern "C" void* __ct__CTagProcessor(void*);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" u32 func_801355BC();
extern "C" void func_8003AA78__5CBdatFUlPv(u32, void*);
extern "C" u32 func_8003B1EC(void*);
extern "C" u32 func_8009CF8C(u32);
extern "C" u16 func_80136254(const void*, const void*, int);
extern "C" u8 code80135FDC_getByte_6407F();
extern "C" void func_801C3C14(CBgTex* self);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);

// Award-list array element ctor/dtor + MWCC array helpers (retail-unmangled
// names). Declaring the element dtor with C linkage keeps the
// __construct_array/__destroy_arr element-dtor relocs bound to the bare
// retail names (cf. CMenuQstCnt's __dt__80227030).
extern "C" void func_802706C4(CPlayAwardEntry* self);
extern "C" void* __dt__802706D4(CPlayAwardEntry* self, int mode);
extern "C" void __construct_array(void*, void*, void*, u32, u32);
extern "C" void __destroy_arr(void*, void*, int, int);

// Widget/base destructors called by the member dtor (US retail keeps the
// mangled __dt__ names; C-ABI wrappers for the embedded widget storages).
// __declspec(noinline) on the CPlayAwardList dtor keeps the same-TU empty
// member stub from being inlined into the calls.
extern "C" __declspec(noinline) CPlayAwardList* __dt__14CPlayAwardListFv(CPlayAwardList* self, int dealloc);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int dealloc);
extern "C" void __dt__6CBgTexFv(CBgTex* self, int dealloc);
extern "C" void __dt__800FED0C(CProcess* self, int flags);

// IScnRender-vtable thunk targets: the C++ members are emitted under their
// mangled names; C linkage keeps the verbatim retail symbols (so the
// func_80270644 / func_8027064C thunks bind the literal identifiers).
extern "C" void cbRenderBefore__14CMenuPlayAwardFv(CMenuPlayAward* self);
