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
    u16 field_100C;                  // 0x100C
    u16 field_100E;                  // 0x100E
};

class CPlayAwardList : public IWorkEvent {
public:
    CPlayAwardList();
    virtual ~CPlayAwardList();
    void OnFileEvent();

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
    u8 _pad1C[0x20 - 0x1C];                 // 0x1C-0x1F
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
    u8 _pad3C[0x54 - 0x3C];                                    // 0x3C-0x53
    u8 mField54;                                               // 0x54: state byte (set to 1 by func_802705F4)
    u8 _pad55[0x58 - 0x55];                                    // 0x55-0x57
    IScnRender mIScnRender;                                    // 0x58-0x5B: render-callback subobject (vptr)
    CScn* mScene;                                              // 0x5C: owning scene
    CBgTex mBgTex;                                             // 0x60-0x7F
    CTitleAHelp mTitleAHelp;                                   // 0x80-0xB7
    CPlayAwardList mPlayAwardList;                             // 0xB8
    u8 _padAfterList[0x1160 - (0xB8 + sizeof(CPlayAwardList))]; // pad to 0x1160
    u8 mField1160;                                             // 0x1160: help-display byte (set to 2 by func_80270404)
};

// MWCC runtime operator delete (freeing dtor helper).
extern "C" void* __dl__FPv(void*);

// .sdata2 float pool: 1.0f -- animation frame step for the award list
// open/close transitions (func_80271528 / func_80271574 / func_80271480).
extern const float lbl_eu_806689C0;

// .sdata2 float pool: entry-array init constant (func_80270654).
extern const float lbl_eu_806689A8;

// Menu-open singleton (0 = closed; cleared by Term, checked by func_80270308).
extern unsigned long lbl_eu_806648A0;

// Render-gate mode bitfield (.sbss; bit 21 = task busy, gates cbRenderBefore).
extern u32 lbl_eu_80663E28;

// Shared string pool (split .rodata): award-list arc file paths at +0x5A / +0x74.
extern char lbl_eu_8050E7C0[];

// MEM2 alloc-handle query (retail-unmangled; C linkage so the zero-arg name
// stays bare instead of mangling to func_800A9D90__Fv).
extern "C" int func_800A9D90();

// Scene-active query (retail-unmangled; gates cbRenderBefore draws).
extern "C" int func_8013BE50();

// DrawInfo layout setup (retail MANGLED name func_80137250__FPQ34nw4r3lyt8DrawInfo;
// declared with C++ linkage so MWCC re-derives the mangled reloc).
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

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
extern "C" void __declspec(noinline) func_802717F8(CPlayAwardList* self);
extern "C" void __declspec(noinline) func_80270E04(CPlayAwardList* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" __declspec(noinline) u8* func_80270AEC(CPlayAwardEntryArray* self, int param);

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

// CfGameManager controller-type query (retail pre-mangled name; declared
// under C linkage so calls bind the literal identifier).
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u8 enable);

// CScrollBar show request (retail-unmangled import). The member declaration in
// CScrollBar.hpp would mangle the call reloc (func_801F367C__10CScrollBarFv),
// and CSortMenu.hpp cannot be co-included with CTitleAHelp.hpp (its extern
// UnkClass_8045F564 ctor conflicts with the member ctor CTitleAHelp pulls in).
extern "C" void func_801F367C(u8* scrollBar);

// CScrollBar draw + cursor draw imports (retail-unmangled names; same reason
// as func_801F367C -- the CScrollBar.hpp member decl would mangle them).
extern "C" void func_801F35B0(u8* scrollBar, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801D20B0(u8* cursor, nw4r::lyt::DrawInfo* drawInfo);

// Cursor/scrollbar helpers used by the close sequence (func_80271070).
extern "C" void func_801D216C(u8* cursor, u8 val);
extern "C" void func_801F369C(u8* scrollBar);

// Layout text-binding helper (retail-unmangled; 3rd arg is the raw text word).
extern "C" void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);

// Scroll-bar layout load + per-frame update/draw helpers, and the cursor
// update helper (retail-unmangled names; CScrollBar.hpp cannot be included).
extern "C" void func_801F34F4(u8* scrollBar);
extern "C" void func_801F3540(u8* scrollBar);
extern "C" void func_801F36BC(u8* scrollBar, int, int);
extern "C" void func_801F3850(u8* scrollBar, u16 val);
extern "C" void func_801D202C(u8* cursor);

// Embedded-widget destructors (retail mangled __dt__ names; the widget
// storages are opaque u8 arrays so the dtor calls are C-ABI imports).
extern "C" void __dt__10CScrollBarFv(u8* scrollBar, int dealloc);
extern "C" void __dt__6CCur18Fv(u8* cursor, int dealloc);

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
