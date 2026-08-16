#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/CBaseCur.hpp"

// Window base class: quest/menu windows are CRTP task objects whose Move/Draw
// behavior is dispatched through the +0x3C/+0x48 pointer-to-member-function
// hooks by the out-of-line CTTask<IUIWindow> specializations (CQuestWindow.cpp).
// __declspec(novtable): the retail ~IUIWindow stores no vptr (the retail
// vtable is a .data object in the split1 unit), so the implicit vptr store
// must be suppressed to match the dtor byte-for-byte.
class __declspec(novtable) IUIWindow : public CTTask<IUIWindow> {
public:
    virtual ~IUIWindow();

    // TODO: add fields
};

// Minimal CScn view (retail CScn member; defined in monolib/src/scn/CScn).
// Term calls removeRenderCB on the scene stored at +0x74. Guarded so the
// kyoshin task headers can be included together in one TU (same minimal
// declaration appears in CTaskGameEff.hpp / CTaskGameEvt.hpp / CTaskGamePic.hpp).
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

// +0x90 object destroyed by Term through its first virtual (vtable +0x8).
class CQuestWinRes {
public:
    virtual ~CQuestWinRes();
};

// Abstract view into the embedded CCur18 vtable (slot 3 at +0x0C is the
// cursor update virtual invoked by Term). Matches CSysWinSelect::CCursor18.
class CCursor18 {
public:
    virtual void vf2(int) = 0;  // slot 2 (0x08)
    virtual void vf3() = 0;     // slot 3 (0x0C)
};

// Full CCur18 vtable view (virtual index 2 -> vtable +0x10 - Move/cursor
// positioning, invoked by func_8012435C with a computed VEC3).
class CCur18View {
public:
    virtual void vf02() = 0;       // index 0 -> +0x08
    virtual void vf03(void*) = 0;  // index 1 -> +0x0C
    virtual void vf04(void*) = 0;  // index 2 -> +0x10 - Move, func_8012435C
};

// Data body of the embedded CCur18 cursor (0x18 bytes). func_8012278C copies
// the non-vtable fields (0x14/0x15 are bytes; 0x16/0x17 are not copied).
struct CCur18Data {
    void* vtable;  // 0x00 (retained, not copied)
    u32   f_04;    // 0x04
    u32   f_08;    // 0x08
    u32   f_0c;    // 0x0c
    u32   f_10;    // 0x10
    u8    f_14;    // 0x14
    u8    f_15;    // 0x15
    u8    f_16;    // 0x16
    u8    f_17;    // 0x17
};

// Abstract view of the object returned by CDeviceFont::func_80452C10; the
// 8th user virtual (vtable+0x24) yields the u32 bound by func_8013676C.
// All-pure, never constructed directly (CPartyState::FontHelper scheme) - a
// real C++ virtual call forces MWCC's canonical r12 vtable dispatch.
class CQuestWinFontView {
public:
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual u32 v7() = 0;  // vtable offset 0x24
};

// Minimal CPad / CfPadData view (offsets read by func_80122C08: pressed
// buttons at +0x4, turbo/edge flags at +0x104).
struct CPadView {
    u8 pad0[0x4];
    u32 field_04;       // +0x04 pressed-button flags
    u8 pad8[0x104 - 0x8];
    u32 field_104;      // +0x104 turbo/edge flags
};

// ---------------------------------------------------------------------------
// Quest window (retail sizeof 0xEC; the factory allocates 0xEC on the work
// heap). Plain non-polymorphic struct (CTalkWindow / CSimpleEveTalkWin idiom):
// the retail US/EU vtable lives in the split1 .data unit (lbl_eu_8052D0F0),
// so the vptr at +0x10 is a plain field written by the ctor/dtor at source
// level - a C++ virtual class would emit a local __vt__12CQuestWindow and
// drift the dtor's vptr-store reloc.
// ---------------------------------------------------------------------------
struct CQuestWindow {
    u8  pad0[0x10];                        // 0x00 CProcess head
    u32 mVtable;                           // 0x10 window composite vtable slot
    u8  pad14[0x3C - 0x14];                // 0x14 CProcess tail
    CTTask<IUIWindow>::MoveFunc mMoveFunc;  // 0x3C Move ptmf hook (12B)
    CTTask<IUIWindow>::DrawFunc mDrawFunc;  // 0x48 Draw ptmf hook (12B)
    nw4r::lyt::Layout* mpLayout;           // 0x54 layout (cbRenderBefore)
    u32 field_58;                          // 0x58
    u32 field_5C;                          // 0x5C
    s32 field_60;                          // 0x60 (ctor init -1)
    u8  field_64;                          // 0x64
    u8  field_65;                          // 0x65
    u8  field_66;                          // 0x66
    u8  field_67;                          // 0x67 (ctor init 1)
    u32 field_68;                          // 0x68
    u32 field_6C;                          // 0x6C IWorkEvent dispatch slot
    u32 field_70;                          // 0x70 IScnRender dispatch slot
    CScn* mpScn;                           // 0x74 owning scene (removeRenderCB receiver)
    UnkClass_8045F564 mMemRegion;          // 0x78 embedded scratch region
    CFileHandle* field_0x88;               // 0x88 file handle (readFile)
    CFileHandle* field_0x8C;               // 0x8C file handle (readCommonArchiveFile)
    CQuestWinRes* field_0x90;              // 0x90 object released by Term (virtual dtor)
    nw4r::lyt::ArcResourceAccessor* field_0x94; // 0x94 arc accessor (Term release)
    nw4r::lyt::AnimTransform* mAnimA;      // 0x98
    nw4r::lyt::AnimTransform* mAnimB;      // 0x9C
    u8  mCursor[0x18];                     // 0xA0 CCur18 cursor storage
    u32 field_B8;                          // 0xB8 quest id (Init table index)
    u8  field_0xBC;                        // 0xBC window state byte
    u8  padBD[0xC0 - 0xBD];                // 0xBD
    s32 field_0xC0;                        // 0xC0 cursor draw gate (signed)
    u32 field_0xC4;                        // 0xC4
    u8  field_0xC8;                        // 0xC8 flag byte (ctor arg3)
    u32 field_0xCC;                        // 0xCC
    u32 field_0xD0;                        // 0xD0
    u16 field_0xD4;                        // 0xD4
    u16 field_0xD6;                        // 0xD6
    u16 field_0xD8;                        // 0xD8
    u8  field_0xDA;                        // 0xDA
    u32 field_0xDC;                        // 0xDC
    u32 field_0xE0;                        // 0xE0
    u8  field_0xE4;                        // 0xE4
    u32 field_0xE8;                        // 0xE8 enabled pad flags (Term/ctor)

    void Init();
    void Term();
    void cbRenderBefore();
    void OnFileEvent();
};

// Quest-window object layout as seen by the CQuestWindow-unit free functions
// (func_80122654 / func_80122BB0 / func_8012429C / func_801242FC). Mirrors
// IUIWindow: the CTTask<IUIWindow> base occupies 0x00..0x54 (CProcess storage
// plus the two 12-byte pointer-to-member-function Move/Draw hooks at 0x3C and
// 0x48), followed by the quest-window fields recovered from the retail asm.
struct QuestWinObj {
    u8 pad0[0x3C];                           // 0x00 CTTask<IUIWindow> head
    CTTask<IUIWindow>::MoveFunc mMoveFunc;   // 0x3C Move ptmf hook (12B)
    CTTask<IUIWindow>::DrawFunc mDrawFunc;   // 0x48 Draw ptmf hook (12B)
    nw4r::lyt::Layout* mpLayout;             // 0x54 layout (func_8012429C/FC)
    u8 pad58[0x60 - 0x58];                   // 0x58
    u32 field_0x60;                          // 0x60 (func_80122C08 writes c0+1)
    u8 field_64;                             // 0x64 (func_80122EF8 sets 1)
    u8 pad65[0x88 - 0x65];                   // 0x65
    u32 field_0x88;                          // 0x88 idle gate (func_80122654)
    u32 field_0x8C;                          // 0x8C idle gate (func_80122654)
    u8 pad90[0x98 - 0x90];                   // 0x90
    nw4r::lyt::AnimTransform* mAnimA;        // 0x98
    nw4r::lyt::AnimTransform* mAnimB;        // 0x9C
    CBaseCur mCursor;                        // 0xA0 (sizeof 0x18, aligns to 4)
    u32 field_B8;                            // 0xB8 quest id (func_80122EF8)
    u8 field_0xBC;                           // 0xBC window state byte
    u8 padBD[0xC0 - 0xBD];                   // 0xBD
    s32 field_0xC0;                          // 0xC0 cursor gate (func_8012435C / 80122C08)
    u32 field_0xC4;                          // 0xC4
    u8 field_0xC8;                           // 0xC8 flag byte
    u32 field_0xCC;                          // 0xCC
    u32 field_0xD0;                          // 0xD0
    u16 field_0xD4;                          // 0xD4
    u16 field_0xD6;                          // 0xD6
    u16 field_0xD8;                          // 0xD8
    u8 field_0xDA;                           // 0xDA
    u32 field_0xDC;                          // 0xDC
    u32 field_0xE0;                          // 0xE0
    u8 field_0xE4;                           // 0xE4
    u32 field_0xE8;                          // 0xE8
};

// 12-byte Move-hook ptmf copied into +0x3C by func_80122654 (retail .data).
extern CTTask<IUIWindow>::MoveFunc lbl_eu_8052D074;

// 12-byte initial Move-hook ptmf copied into +0x3C by the ctor (retail .data).
extern u32 lbl_eu_8052D068[3];

// Null pointer-to-member-function constant copied into the Move/Draw hooks.
extern u32 __ptmf_null[3];

// IUIWindow base vtable (written at +0x10 first, then overwritten by the
// CQuestWindow composite vtable lbl_eu_8052D0F0).
extern char lbl_eu_8052D238[];

// Window-count .sbss counter (incremented by the ctor, decremented by Term).
extern u32 lbl_eu_80663FD0;

// Quest-name table indexed by func_80138138(field_B8) then passed to
// func_801361E8 (func_80122EF8).
extern u32 lbl_eu_80573D18[];

// Quest-name table indexed by func_80138138(field_B8) then passed to getFP
// (func_8012278C stores the file pointer at +0xD0).
extern const char* lbl_eu_8052CFF4[];

// func_801231C4 jump-table item-name pointers (.sdata): passed as the first
// arg of func_8013639C for the quest-slot name switch (cases 1, 3 and 4).
extern u32 lbl_eu_806640D8;
extern u32 lbl_eu_806640EC;
extern u32 lbl_eu_80664104;

// Shared string table (func_8012435C pane names, func_80122EF8 messages).
extern char lbl_eu_804FEC84[];

// Cursor-position pane-name tables indexed by field_0xC0 (func_8012435C):
// each entry's 4 bytes spell the pane name passed to FindPaneByName. Non-const
// with a fixed small size keeps the indexed loads sda21-based (incomplete or
// const arrays go to .rodata/.data with absolute addressing).
extern f32 lbl_eu_80667178[2];
extern f32 lbl_eu_8066717C[2];

// func_80122EF8 loop scaling / rounding constants (.sdata2).
extern const f32 lbl_eu_8066713C;
extern const f32 lbl_eu_8066714C;
extern const f64 lbl_eu_80667150;
extern const f64 lbl_eu_80667158;
extern const f64 lbl_eu_80667160;

// func_80122EF8 5-byte window-name patch table: the leading word holds float
// bits (read as raw bytes into a stack buffer) plus a trailing byte.
extern const u32 lbl_eu_80667144;
extern const u8 lbl_eu_80667148;

// Anim-completion sentinel float (func_80122BB0 waits on mAnimA against it).
// `const` hoists the sdata2 load above the frame stores (CArtsInfo pattern).
extern const f32 lbl_eu_80667140;

// Quest-window vtable (.data, split1 unit) - written to +0x10 by the ctor and
// re-written by the dtor (with the +0x24/+0xAC IWorkEvent/IScnRender slots).
extern char lbl_eu_8052D0F0[];

// Quest-name table indexed by func_80138138(field_B8) (Init).
extern const char* lbl_eu_8052CF80[];

// Window arc path pointer (.sdata) fed to CDeviceFile::readFile (Init).
extern const char* lbl_eu_80661EB8;

// CfPadTask teardown float (.sdata2) passed to func_801C1B94 in the dtor.
// `const` hoists the sdata2 load above the frame stores (CArtsInfo pattern).
extern const f32 lbl_eu_80667138;

// cbRenderBefore busy flag (.sbss; bit 0x200000 gates the draw).
extern u32 lbl_eu_80663E28;

// nw4r::lyt::Pane flag byte at +0xBB (mFlag; bit0 = visible). func_80124270
// (the quest-window pane toggle) clears bit 0 and ORs in the caller's flag
// byte, mirroring SetVisible without the branchy nw4r SetBit helper.
struct PaneFlagRef {
    u8 pad[0xBB]; // +0x00..0xBA
    u8 mFlag;     // +0xBB
};

// C-ABI imports for the quest-window free functions. These retail symbols are
// unmangled, so they must stay C linkage to stop MWCC appending a
// __F<argtypes> suffix to the call reloc names.
extern "C" {
extern "C" void func_801D216C(void*, int); // cursor visibility setter (CCur)
u32 func_80293C10();                       // party-change notice screen active?
u32 func_80192BD0();                       // party-state screen active?
void func_801D202C(void* cur);             // cursor per-frame update (CCur)
void func_801D20B0(void*, void*); // cursor draw
void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self);
void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self, int flags);
extern "C" int func_800A9D90(void);        // common-archive handle
u32 func_80138138(u32 questId);             // quest id -> name-table index
void func_8008294C__Q22cf13CfGameManagerFv(bool enable); // cf pad enable/disable
void* __ct__CQuestWindow(CQuestWindow* self, u32 arg1, u32 arg2, u32 arg3);
void* __dt__12CQuestWindowFv(CQuestWindow* self, int flags);
void __dt__6CCur18Fv(void* self, int flags);
void __dt__17UnkClass_8045F564Fv(void* self, int flags);
void __dt__9IUIWindowFv(void* self, int flags);

// CProcess base ctor (retail symbol is the mangled C++ name; bind literally).
void __ct__8CProcessFv(CProcess* self);
void __ct__CCur18(void* cursor, void* accessor);
void __ct__17UnkClass_8045F564Fv(void* self);

// Scratch-region RAII guard C-ABI ct/dt (func_8012278C uses an explicit
// 8-byte stack buffer, CQstLogList/CSaveLoad idiom).
void __ct__14Class_8045F858FP17UnkClass_8045F564(void* self, void* base);
void __dt__14Class_8045F858Fv(void* self, int dealloc);

// CTagProcessor ctor (retail symbol is unmangled __ct__CTagProcessor; the
// C++ class ctor would mangle to __ct__11CTagProcessorFv).
class CTagProcessor;
void* __ct__CTagProcessor(void*);

// CLibLayout arc accessor factory (func_8012278C window-arc branch).
// getAllocHandle__10CLibLayoutFv is declared in code_80135FDC.hpp.
nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();

// CfGameManager C-ABI imports (pre-mangled retail names; some retail symbols
// are Fv-mangled but receive a vestigial register arg - keep the arg for the
// call sites that set it, e.g. func_80086F9C(-1)).
int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
u32 getEnabledInputFlags__Q22cf13CfGameManagerFv();
void enablePadFlags__Q22cf13CfGameManagerFUlb(u32 enableFlags, bool enable);
void* getCurrentPad__Q22cf13CfGameManagerFv();
void* getCfPadData__Q22cf13CfGameManagerFv();
void* getPlayer__Q22cf13CfGameManagerFi(int index);
u32 func_8008235C__Q22cf13CfGameManagerFv(u32);

// C-ABI UI / misc imports used by Term, func_80122EF8, func_80122C08.
u8 func_8013B980();
u32 func_801B481C();
void func_80135550();
u8 code80135FDC_getByte_64080();
void code80135FDC_postIncByte_64080();
void code80135FDC_setByte_6405B();
void func_8003AA8C__5CBdatFUl(u32);
void func_80137924(void*, void*, void*, void*);
void func_80138078__FUl(u32);
void func_8013B428__FUl(u32);
void func_8009D018(u32 owner, u32 flag);
u32 func_8009CF8C(u32 resourceId);
void func_8013D55C(char* msg, int a, int b);
int func_8015D310();
void func_8015D3A0();
void func_8013E030();
void func_8013E2E0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
void* func_800451D8(u32 cls, void* param);
u32 func_800A32BC();
int func_8026178C(void* data, u32 flag);
u32 func_8025FB10(void* data, u32 flag);
void func_800A21F8(void* data, u16 value, int a, int b);
char* func_801571FC();
void func_80157184(s32 value);

// CUICfManager font helpers (func_8012278C binds the font and rebuilds the
// cursor). func_801355BC returns the font string, func_801355F4 the shared
// arc accessor.
extern "C" void* func_801355BC(void);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();

// BDAT archive attach helpers (func_8012278C common-archive branch).
void func_8003AA78__5CBdatFUlPv(u32, void*);
extern "C" void func_8003AA34(void*);

// Quest-content helpers (func_801231C4). The *_E4/58/2C functions return raw
// u32 values that the caller masks explicitly with & 0xFFFF (retail keeps the
// unmasked value in a callee-saved register between the call and the mask).
extern "C" u16 func_80136254(const void*, const void*, int);
u32 func_801392E4(u32);
extern "C" u16 func_80139358(u32);
u32 func_8013732C(u32);
void* func_80138DA4(const char*);
u32 func_8009ECF0();
void func_8009ECFC(u16);
void func_8013DB6C(u32, u32, u32, u32);

// Pane visible-bit toggle (clears bit 0 of the pane +0xBB flag byte and ORs
// the caller's flag byte; func_801231C4 pane show/hide calls).
extern "C" void func_80124270(void*, u32);

// Copy 2 floats from src to self+0x4C (func_801231C4 cursor-pane placement).
void func_80124288(u8* self, const float* src);
}

// Same-TU quest-window free functions. The retail names are unmangled, so the
// calls from func_801226C8 keep C linkage (the definitions below inherit it).
// noinline: func_8012435C/80122C08/80122EF8 are still empty stubs; without it
// MWCC inlines their empty bodies into func_801226C8's switch and collapses
// the case-2/3 dispatch (retail keeps the calls).
extern "C" {
void func_801226C8(QuestWinObj* self);
void func_80122BB0(QuestWinObj* self);
void func_8012429C(QuestWinObj* self);
__attribute__((noinline)) void func_80122C08(QuestWinObj* self);
__attribute__((noinline)) void func_80122EF8(QuestWinObj* self);
__attribute__((noinline)) void func_8012435C(QuestWinObj* self);
CQuestWindow* func_80122B2C(CProcess* parent, u32 arg1, u32 arg2, u32 arg3);
// CQuestWindow::OnFileEvent tail-calls func_8012278C with this - 0x6C; the
// event's file handle selects the window-arc (field_0x88) or common-archive
// (field_0x8C) build branch. func_801231C4 refreshes the window content after
// either branch completes.
bool func_8012278C(CQuestWindow* self, CEventFile* event);
// noinline: func_801231C4 is still an empty stub; without it MWCC inlines its
// empty body into func_8012278C's two call sites (retail keeps the calls).
__attribute__((noinline)) void func_801231C4(CQuestWindow* self);
}

// Minimal CTaskGame decl (retail global-namespace class, symbols
// getInstance__9CTaskGameFv / func_800426F0__9CTaskGameFv).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// Quest-window state record behind the retail .sbss singleton
// lbl_eu_80663FD4; only the +0xDA byte is read by func_8012246C.
struct QuestWindowState {
    u8 pad[0xDA];  // +0x00..0xD9
    u8 field_0xDA; // +0xDA
};

// Retail .sbss pointer (global-scope data symbols are not mangled by MWCC).
extern QuestWindowState* lbl_eu_80663FD4;
