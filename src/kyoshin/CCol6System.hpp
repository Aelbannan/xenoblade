#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "kyoshin/CCol6CheckBat.hpp"
// CCol6Invite.hpp declares func_eu_801651A0 with C++ linkage, which mangles
// the retail-unmangled symbol at call sites (reloc-name drift). Shadow that
// declaration so this unit's extern "C" decl (below) is the only visible one.
#define func_eu_801651A0 func_eu_801651A0_cpponly
#include "kyoshin/CCol6Invite.hpp"
#undef func_eu_801651A0
#include "kyoshin/CScrollBar.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/math/CVec3.hpp"

#include <nw4r/lyt.h>
#include <revolution/GX.h>

// Minimal CScn declaration (same guarded form as CTaskGameEff.hpp/CTaskGameEvt
// /CTaskGamePic): only the render-callback members used by CCol6System's
// Init/Term are needed. Declared with the real class tag so member calls emit
// the retail mangled symbols addRenderCB__4CScnFP10IScnRenderUlUl /
// removeRenderCB__4CScnFP10IScnRender.
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

class CCol6AnimHostView;  // layout anim host view (defined below)

class CCol6Hint : public CProcess {
public:
    CCol6Hint(CProcess* parent);
    virtual ~CCol6Hint();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    /* 0x3C */ u8 mCallbackA[12];          // pointer-to-member-function null (3 words)
    /* 0x48 */ u8 mCallbackB[12];          // pointer-to-member-function null (3 words)
    /* 0x54 */ CCol6AnimHostView* mAnimHost;   // layout anim host (vtable +0x2C/+0x38)
    /* 0x58 */ u32 mField58;               // init 0
    /* 0x5C */ u32 mField5C;               // init 0
    /* 0x60 */ s32 mIndex;                 // init -1
    /* 0x64 */ u8 mFlag64;                 // byte flag, init 0
    /* 0x65 */ u8 mFlag65;                 // byte flag, init 0
    /* 0x66 */ u8 mFlag66;                 // byte flag, init 0
    /* 0x67 */ u8 mActive;                 // byte flag, init 1
    /* 0x68 */ u32 mField68;               // init 0
    /* 0x6C */ u32 mField6C;               // vtable-like ptr
    /* 0x70 */ u32 mField70;               // vtable-like ptr
    /* 0x74 */ u8 mMemRegion[0x10];        // UnkClass_8045F564
    /* 0x84 */ CProcess* mParentPtr;       // stored parent pointer
    /* 0x88 */ CFileHandle* mField88;      // loaded arc file handle
    /* 0x8C */ u32 mField8C;               // ArcResourceAccessor*
    /* 0x90 */ nw4r::lyt::AnimTransform* mField90;  // AnimTransform* (out anim)
    /* 0x94 */ nw4r::lyt::AnimTransform* mField94;  // AnimTransform* (in anim)
    /* 0x98 */ u8 mFlag98;                 // byte flag, init 0
    /* 0x99 */ u8 mState;                  // state machine (0-5), init 0
    /* 0x9C */ u32 mUnk9C[0x20];           // 32-bit entries (Move state 5 indexes by (s8)s)
    /* 0x11C */ u32 mField11C;             // init 0
    /* 0x120 */ u8 mField120;              // init 0
    /* 0x121 */ u8 mField121;              // init 0
    /* 0x122 */ u8 mPad122[2];             // retail pads to 0x124
    /* 0x124 */ u8 mCur18[0x18];           // CCur18
    /* 0x13C */ CScrollBar mScrollBar;
};

// Abstract view into the CCur18 cursor vtable embedded in CCol6System
// (MWCC reserves slots 0/1, so the first user virtuals land at vtable +0x08
// and +0x0C; slot 3 is the per-frame update invoked by Term).
class CCol6CursorView {
public:
    virtual void vf2(int) = 0;  // slot 2 (vtable + 0x08)
    virtual void vf3() = 0;     // slot 3 (vtable + 0x0C)
};

// CCur18 vtable view for CCol6Hint::Move: the +0x10 slot (virtual index 2)
// is the position-set virtual invoked with the func_80137924 result.
class CCol6Cur18View {
public:
    virtual void vf02() = 0;                      // +0x08
    virtual void vf03(void*) = 0;                 // +0x0C
    virtual void vf04(nw4r::math::VEC3*) = 0;     // +0x10 - Move state 2
};

// Layout-animation host (the object behind CCol6Hint::mField54): its vtable
// carries the anim-swap setter at +0x2C (virtual index 9) and the per-frame
// update at +0x38 (virtual index 12); its +0x10 data slot is the Pane whose
// vtable +0x3C virtual (index 13) sets label text. Virtuals are declared
// FIRST so MWCC places the vptr at +0 (a data member before the virtuals
// would push the vptr to +0x10 and break the dispatch).
class CCol6AnimHostView {
public:
    virtual void v00(int) = 0;  // +0x08 - Term releases the host with arg 1
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void setAnim(nw4r::lyt::AnimTransform* anim, int index) = 0;  // +0x2C
    virtual void v0A() = 0;
    virtual void v0B() = 0;
    virtual void update(int) = 0;  // +0x38

    u8 _04[0x0C];                 // +0x04..0x0F
    nw4r::lyt::Pane* mPane10;     // +0x10 - text pane (vtable +0x3C setter)
};

// Pane vtable view exposing the label-text setter at +0x3C (virtual index 13).
class CCol6PaneView {
public:
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v0A() = 0;
    virtual void v0B() = 0;
    virtual void v0C() = 0;
    virtual nw4r::lyt::Pane* setText(const char* text, int flag) = 0;  // +0x3C
};

// cf::CPad view: only the pressed-button flags word this unit reads.
struct CCol6Pad {
    u32 mHeldButtonFlags;       // 0x00
    u32 mPressedButtonFlags;    // 0x04
};

// cf::CfPadData view for getCfPadData: only the two flag words the Move-style
// functions read (held buttons at +0x04, short-press flags at +0x104).
struct CCol6PadData {
    u8  _00[0x4];
    u32 mField04;      // +0x04 held-button flags
    u8  _08[0x104 - 8];
    u32 mField104;     // +0x104 short-press flags
};

// nw4r::lyt::Layout view exposing the +0x10 root-pane slot (the retail Layout
// stores the root pane at +0x10; the SDK header keeps it private).
struct CCol6LayoutView {
    void* vtable;                 // 0x00
    u8 _04[0x0C];                 // 0x04..0x0F
    nw4r::lyt::Pane* mpRootPane;  // 0x10
};

// Data body of the embedded CCur18 cursor (0x18 bytes). func_8015DD4C copies
// the non-vtable fields (f_04..f_15) from a stack temp into the embedded
// cursor at +0x124 (same shape as CSysWinSave::Init).
struct CCol6Cur18Data {
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

// View into the object returned by CDeviceFont::func_80452C10: vtable+0x24
// (virtual index 7, no args) yields the u32 bound into the layout's font pane
// by func_8013676C. All-pure so no vtable is emitted.
class CCol6FontView {
public:
    virtual void vf0() = 0; // index 0 -> +0x08
    virtual void vf1() = 0; // index 1 -> +0x0C
    virtual void vf2() = 0; // index 2 -> +0x10
    virtual void vf3() = 0; // index 3 -> +0x14
    virtual void vf4() = 0; // index 4 -> +0x18
    virtual void vf5() = 0; // index 5 -> +0x1C
    virtual void vf6() = 0; // index 6 -> +0x20
    virtual u32 vf7() = 0;  // index 7 -> +0x24
};

// Byte-at-+0xBB view of a pane (mFlag in the game's nw4r layout; the SDK
// header's member offsets do not match retail).
struct CCol6PaneFlagView {
    u8 _00[0xBB];
    u8 mFlag;   // 0xBB
};

// Cast-only view of the object returned by CItem_initItemImplInstances: MWCC
// reserves vtable slots 0/1, so the first user virtuals land at +0x08/+0x0C
// and slot 4 at +0x10 is the per-item sync callback invoked by func_80160EE4.
class CCol6ItemInstancesView {
public:
    virtual void vf2(int) = 0;         // vtable + 0x08
    virtual void vf3() = 0;            // vtable + 0x0C
    virtual void vf4(void* item) = 0;  // vtable + 0x10 (called with the item)
};

class CCol6System : public CProcess {
public:
    CCol6System();
    virtual ~CCol6System();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    // Cursor/window position sync; MWCC inlines this into every Move() call site.
    void updateCursorPos();

    /* 0x3C */ u8 mCallbackA[12];               // pointer-to-member-function null (3 words)
    /* 0x48 */ u8 mCallbackB[12];               // pointer-to-member-function null (3 words)
    /* 0x54 */ nw4r::lyt::Layout* mpLayout;     // layout (released in Term)
    /* 0x58 */ u32 mField58;                    // init 0
    /* 0x5C */ u32 mField5C;                    // init 0
    /* 0x60 */ s32 mIndex;                      // init -1
    /* 0x64 */ u8 mFlag64;                      // byte flag, init 0
    /* 0x65 */ u8 mFlag65;                      // byte flag, init 0
    /* 0x66 */ u8 mFlag66;                      // byte flag, init 0
    /* 0x67 */ u8 mFlag67;                      // byte flag, init 1
    /* 0x68 */ u32 mField68;                    // init 0
    /* 0x6C */ u32 mField6C;                    // vtable-like ptr
    /* 0x70 */ u8 mScnRender[4];                // IScnRender vptr slot (this-adjust target)
    /* 0x74 */ u8 mMemRegion[0x10];             // UnkClass_8045F564 scratch region
    /* 0x84 */ CScn* mScn;                      // owning scene (render-CB receiver)
    /* 0x88 */ CFileHandle* mFileHandle;        // loaded arc file handle
    /* 0x8C */ nw4r::lyt::ArcResourceAccessor* mArcAccessor; // arc accessor (Term release)
    /* 0x90 */ nw4r::lyt::AnimTransform* mAnim90;  // layout anims (setAnim targets)
    /* 0x94 */ nw4r::lyt::AnimTransform* mAnim94;
    /* 0x98 */ nw4r::lyt::AnimTransform* mAnim98;
    /* 0x9C */ nw4r::lyt::AnimTransform* mAnim9C;
    /* 0xA0 */ u8 mFlagA0;                      // layout-2 ready flag (cleared in Term)
    /* 0xA1 */ u8 mFlagA1;                      // draw-gate flag (cbRenderBefore)
    /* 0xA2 */ u16 mFieldA2;                    // halfword (retail ctor: sth 0)
    /* 0xA4 */ u8 mFieldA4;                      // byte flag, set 0x22 by func_8016378C
    /* 0xA5 */ u8 mPadA5[3];                     // retail pads to 0xA8
    /* 0xA8 */ u8 mCur1[0x18];                  // CCur18 cursor 1
    /* 0xC0 */ u8 mCur2[0x18];                  // CCur18 cursor 2
    /* 0xD8 */ u8 mSysWin1[0x3C];               // CSysWin window 1
    /* 0x114 */ u8 mSysWin2[0x3C];              // CSysWin window 2
    /* 0x150 */ ml::CVec3 mCamPos;              // camera position (CfGameManager camera API)
    /* 0x15C */ ml::CVec3 mCamLookAt;           // camera look-at
    /* 0x168 */ ml::CVec3 mVec168;              // ctor init (f,f,f)
    /* 0x174 */ ml::CVec3 mVec174;              // ctor init (f,f,f)
    /* 0x180 */ f32 mField180;                  // ctor init lbl_eu_80667564
    /* 0x184 */ u8 mField184;                   // ctor init 0
    /* 0x188 */ ml::CVec3 mVec188;              // ctor init (f,f,f)
    /* 0x194 */ ml::CVec3 mVec194;              // ctor init (f,f,f)
    /* 0x1A0 */ ml::CVec3 mVec1A0;              // ctor init (f,f,f)
    /* 0x1AC */ ml::CVec3 mVec1AC;              // ctor init (f,f,f)
    /* 0x1B8 */ f32 mField1B8;                  // ctor init lbl_eu_80667564
    /* 0x1BC */ u8 mField1BC;                   // ctor init 0
};

// Data body of the embedded CSysWin (0x3C bytes). CCol6System::Init copies the
// non-vtable fields only (skips the vptr at +0 and the +0x29..0x2B padding), so
// MWCC emits the exact lwz/stw + lbz/stb sequence retail shows.
struct CSysWinData {
    void* vtable;  // 0x00 (retained from construction, not copied)
    u32   f_04;    // 0x04
    u32   f_08;    // 0x08
    u32   f_0c;    // 0x0c
    u32   f_10;    // 0x10
    u32   f_14;    // 0x14
    u32   f_18;    // 0x18
    u32   f_1c;    // 0x1c
    u32   f_20;    // 0x20
    u32   f_24;    // 0x24 (copied as a word)
    u8    f_28;    // 0x28 (copied as a byte)
    u8    f_29;    // 0x29 (not copied)
    u8    f_2a;    // 0x2a (not copied)
    u8    f_2b;    // 0x2b (not copied)
    u32   f_2c;    // 0x2c
    u32   f_30;    // 0x30
    u8    f_34;    // 0x34
    u8    f_35;    // 0x35
    u8    f_36;    // 0x36
    u8    f_37;    // 0x37
    u8    f_38;    // 0x38
    u8    f_39;    // 0x39
};

// CSysWin vtable view exposing the layout-build virtual at +0x88 (slot 34)
// invoked by CCol6System::Init. Leading pure virtuals occupy inherited slots
// so the target lands at virtual index 32 (-> vtable +0x88 after the RTTI
// prefix). Same shape as CSysWinSave.hpp / CSysWinSelect.hpp.
class CSysWinView {
public:
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v0A() = 0;
    virtual void v0B() = 0;
    virtual void v0C() = 0;
    virtual void v0D() = 0;
    virtual void v0E() = 0;
    virtual void v0F() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v1A() = 0;
    virtual void v1B() = 0;
    virtual void v1C() = 0;
    virtual void v1D() = 0;
    virtual void v1E() = 0;
    virtual void v1F() = 0;
    virtual void v20() = 0;  // index 32 -> +0x88 - Init
};

// Minimal view of the object returned by cf::CfGameManager::func_80083298:
// only the +0xF0 subobject (UnkClass_8047BB54) is accessed in this unit.
struct UnkClass_80083298View {
    u8 field_0x00[0xF0];
    u8 field_0xF0[0x14];
};

// Minimal CTaskGame import (retail symbols getInstance__9CTaskGameFv /
// func_800426F0__9CTaskGameFv; chained-call form per CUICfManager.cpp).
class CTaskGame;
extern "C" CTaskGame* getInstance__9CTaskGameFv();
extern "C" bool func_800426F0__9CTaskGameFv(CTaskGame* self);

// CMenuFade object (opaque here; func_80113E1C/func_80113E24 gate on it).
class CCol6Fade;

// C++-linkage (mangled retail symbols) declared with the unmangled identifier
// so MWCC derives the retail mangled name (moved from code_80135FDC.hpp, which
// this unit no longer includes).
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void func_80137250(nw4r::lyt::DrawInfo* pDrawInfo);
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);

// C-linkage imports (retail symbols are unmangled - keep linkage verbatim)
extern "C" {
u32 CSysWin_isReady(void* syswin);                                   // CSysWin.cpp
int CSysWin_isActive(void* syswin);                                   // CSysWin.cpp
int CSysWin_getUnk34(void* syswin);                                   // CSysWin.cpp
void func_8022B7C8(void* syswin, nw4r::lyt::DrawInfo* drawInfo);     // CSysWin.cpp
void func_8022B7F4(void* syswin);                                    // CSysWin.cpp
void func_8022B8E4(void* syswin);                                    // CSysWin.cpp
void func_801D20B0(void*, void*);     // CCur.cpp
void func_8013D55C(char* msg, int a, int b);                         // CUICfManager.cpp
void func_8009D018(u32 destination, u32 value);                      // CfGameManager.cpp
CCol6Pad* getCurrentPad__Q22cf13CfGameManagerFv();                   // cf::CfGameManager
int func_80086F9C__Q22cf13CfGameManagerFv(int arg);                  // cf::CfGameManager
void func_8013E2E0(u32, u32, u32, u32, u32, u32, u32, u32, u32);     // CUICfManager.cpp
u8 func_8013600C(const void*, const void*, u32);                     // code_80135FDC.cpp
s32 func_801571FC();                                                 // CItemBoxInfo.cpp
void func_80157184(s32);                                             // CItemBoxInfo.cpp
void* func_801587E8(u16);                                            // CItemBoxInfo.cpp
u32 func_80158068(u16);                                              // CItemBoxInfo.cpp
void func_80158118(void*, u16, u32);                                 // CItemBoxInfo.cpp
// CItem_initItemImplInstances is declared extern "C" by
// include/kyoshin/cf/CfGameManager.hpp (pulled in via harness_catalog); a
// local redeclaration here would mangle and trip MWCC 10197.
u32 func_80124B78();                                                  // CHelp_CloseSysMenu.cpp
CCol6Fade* func_80113E1C();                                           // CMenuFade.cpp
int func_80113E24(CCol6Fade* fade);                                    // CMenuFade.cpp (byte result, tested unmasked)
void func_80135464(u32 a, u32 b, f32 x, f32 y, f32 z);                // CUICfManager.cpp
u32 func_801B481C();                                                   // code_80135FDC.cpp (any block condition active)
u16 func_8013606C(const void*, const void*, u32);                     // code_80135FDC.cpp
char* func_80136190(const void*, const void*, int);                   // code_80135FDC.cpp
void func_80082008__Q22cf13CfGameManagerFv(u32, u32, u32, u32, u32);  // CfGameManager.cpp
void func_80082060__Q22cf13CfGameManagerFv();                         // CfGameManager.cpp
int func_80082104__Q22cf13CfGameManagerFv();                          // CfGameManager.cpp
void func_80081E90__Q22cf13CfGameManagerFv(u32, u32, u32);           // CfGameManager.cpp
void* func_801BFAE4(u16 handle);                                      // CfSoundMan.cpp
void func_80082088__Q22cf13CfGameManagerFv(u32, ml::CVec3*, ml::CVec3*, u32, f32); // CfGameManager.cpp
UnkClass_80083298View* func_80083298__Q22cf13CfGameManagerFv();       // CfGameManager.cpp
extern "C" u16 func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(u32, u32, u32, u32, f32); // CfSoundMan.cpp (returns sound handle)
extern "C" void func_eu_801651A0(char* buffer, const char* format, ...); // EU format helper (unmangled retail)
void func_8022B9B4(void* syswin, const char* msg, int flag);          // CSysWin.cpp
void func_8022BFC8(void* syswin, int flag);                           // CSysWin.cpp
void func_8022B8B8(void* syswin);                                     // CSysWin.cpp
void func_8022B748(void* syswin);                                     // CSysWin.cpp (per-frame update)
char* func_8013639C(const void*, const void*, int);                   // code_80135FDC.cpp
int func_8013BE50();                                                  // code_80135FDC.cpp
// CScrollBar / CCur18 helpers (retail-unmangled C-linkage names).
int CScrollBar_isVisible(void* scrollbar);                             // CScrollBar.cpp
int func_801F3668(void* scrollbar);                                    // CScrollBar.cpp
void func_801F3670(void* scrollbar, ml::CVec3* vec);                 // CScrollBar.cpp
void func_801F36BC(void* scrollbar, int count, u32 value);           // CScrollBar.cpp
void func_801F367C(void* scrollbar);                                 // CScrollBar.cpp
void func_801F3540(void* scrollbar);                                 // CScrollBar.cpp
void func_801F369C(void* scrollbar);                                 // CScrollBar.cpp (layout rebuild)
void func_801F3850(void* scrollbar, u16 value);                     // CScrollBar.cpp (thumb position)
void func_801D216C(void* cursor, int arg);                            // CCur.cpp
void func_801D202C(void* cursor);                                     // CCur.cpp
void* func_801355F4();                                                // CCur.cpp (vtable source for __ct__CCur18)
void* getCfPadData__Q22cf13CfGameManagerFv();                        // cf::CfGameManager
u8 func_801361E8(const void*, const char*, u8);                      // code_80135FDC.cpp
void func_80136B4C(nw4r::lyt::Layout*, const char*, const char*, u32); // code_80135FDC.cpp
u32 func_801355BC();                                                    // CUICfManager.cpp (font value)
s16 func_80136130(const void*, const void*, u32);                     // code_80135FDC.cpp (s16-keyed lookup); u32 matches the definition verbatim
void func_8013676C(nw4r::lyt::Pane*, u32);                           // code_80135FDC.cpp
void func_801392E4(u16);                                             // CItemBoxInfo.cpp
void func_80139358(u16);                                             // CItemBoxInfo.cpp
char* func_801394D4(u16);                                            // CItemBoxInfo.cpp
void* func_8003AA34();                                               // bdat manager
void* getFP__FPCc(const char*);                                      // bdat file pointer
u32 func_8003B1EC(void*);                                            // bdat row count
void func_8022C1B4(u8* out, void* syswin, u8 sel);                   // CSysWin.cpp
void func_8022BF6C(void* syswin, void* a, void* b);                  // CSysWin.cpp
extern "C" void func_80137924(nw4r::math::VEC3* out, nw4r::lyt::Pane* a,
                   nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);          // code_80135FDC.cpp
// CTaskLOD/audio helpers: u8 params (values are passed unmasked by callers).
void func_80462D04__8CTaskLODFv(u8);                                  // CTaskLOD.cpp
void func_80462D5C__8CTaskLODFv(u8);                                  // CTaskLOD.cpp
int func_80462E1C__8CTaskLODFv(u8);                                   // CTaskLOD.cpp
void* func_804BC9EC__Fv(void);                                        // code_80135FDC.cpp
void func_804BCC30(void*, u8);                                        // code_80135FDC.cpp
void func_804BCC3C(void*, u8);                                        // code_80135FDC.cpp
void func_8047BD8C__17UnkClass_8047BB54Fv(void*, u8);                 // code_80135FDC.cpp
void func_8047BD94__17UnkClass_8047BB54Fv(void*, u8);                 // code_80135FDC.cpp
void __ct__8CProcessFv(CProcess* self);                              // CProcess ctor
void __ct__CScrollBar(void* self, u8 direction);                     // CScrollBar ctor (retail short name)
void __ct__CSysWin(void* self, int arg);                              // CSysWin ctor (retail short name)
void __ct__CCur18(void* self, void* arg);                            // CCur18 ctor (retail short name)
void func_801F34F4(CScrollBar* self);                                // CScrollBar arc read (retail short name)
int sprintf(char*, const char*, ...);                                // stdio (CRT)
}

// Layout-build / font helpers whose retail symbols are already mangled
// (func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc
// etc.); declare with the full mangled name so the call-site relocs bind
// verbatim (same convention as CItemBoxGrid.hpp / CTalkWindow.hpp).
extern "C" {
void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
    nw4r::lyt::Layout** ppLayout, nw4r::lyt::ArcResourceAccessor* accessor,
    const char* arcName);
void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
    nw4r::lyt::Layout* layout, nw4r::lyt::AnimTransform** ppAnim,
    nw4r::lyt::ArcResourceAccessor* accessor, char* animName);
void func_80136910__FPQ34nw4r3lyt6LayoutPcUc(nw4r::lyt::Layout* layout,
                                            char* paneName, u8 value);
void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
    u32 arg, nw4r::lyt::Layout* layout);
}

// C++-linkage (mangled retail symbols) anim/layout helpers - MUST stay
// outside the extern "C" block so MWCC mangles them (retail reloc names
// func_80137444__FPQ34nw4r3lyt13AnimTransformf / func_80138078__FUl).
// NOTE: func_80137510 is NOT in this group - retail keeps the bare unmangled
// symbol, so it is declared extern "C" below.
void func_801368C0(nw4r::lyt::Layout* layout, char* paneName, u32 value); // code_80135FDC.cpp (mangled __FPQ34nw4r3lyt6LayoutPcUl)
int func_80137444(nw4r::lyt::AnimTransform* anim, float frame);
extern "C" int func_80137510(nw4r::lyt::AnimTransform* anim, float frame);
void func_80138078(u32 soundId);

// Split1 .rodata string pool (arc paths / layout region names / format strings).
extern char lbl_eu_80502050[];

// Data symbols for the check-bat/hint factories (defined in CCol6System.cpp /
// CCol6Invite.cpp as common symbols; plain global-scope externs emit the same
// unmangled symbols).
extern u32 __ptmf_null[3];      // null pointer-to-member-function constant
extern u32 lbl_eu_80664098;     // invite message-buffer base (.sdata)
extern char lbl_eu_8052D238[];  // CProcess temp vtable (.data)
extern char lbl_eu_8053001C[];  // CCol6System composite vtable (.data)
extern char lbl_eu_8053011C[];  // CCol6Hint composite vtable (.data)
extern char lbl_eu_8053021C[];  // CCol6CheckBat composite vtable (.data)
extern char* lbl_eu_80662358;   // .sdata arc-path pointer

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" const u32 lbl_eu_80667540;
extern "C" const u8 lbl_eu_80667544;
extern "C" const u32 lbl_eu_80667548;
extern "C" const u8 lbl_eu_8066754C;
extern "C" const u32 lbl_eu_80667550;
extern "C" const u8 lbl_eu_80667554;
extern "C" const f32 lbl_eu_80667558;
extern "C" const f32 lbl_eu_8066755C;
extern "C" const f32 lbl_eu_80667560;
extern "C" const f32 lbl_eu_80667564;
extern "C" const f32 lbl_eu_80667578;
extern "C" const f64 lbl_eu_80667580;  // 2^52 magic constant for (f32)(s16) conversions
// Two 7-byte count-threshold tables (u32 + u16 + u8, read as byte arrays by
// func_8015DD4C's item-box scan).
extern "C" const u32 lbl_eu_80667568;
extern "C" const u16 lbl_eu_8066756C;
extern "C" const u8 lbl_eu_8066756E;
extern "C" const u32 lbl_eu_80667570;
extern "C" const u16 lbl_eu_80667574;
extern "C" const u8 lbl_eu_80667576;
extern "C" u32 func_8009CF8C(u32 resourceId);
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__17UnkClass_8045F564Fv(void* self);
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(void* drawInfo);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(void* drawInfo, int flags);
extern "C" void func_801F35DC(CScrollBar* scrollbar);                    // scrollbar destroy
extern "C" void func_801F35B0(void* scrollbar, nw4r::lyt::DrawInfo* di); // scrollbar draw
extern "C" void func_80135998(u8 enable);                                // message-window show/hide
extern "C" void func_8013DA60(int id, int a, int b);                     // window open request
extern u32 lbl_eu_80663E28;  // .sbss mode bitfield (bit 0x400000 gates hint draws)
extern "C" void __ct__17UnkClass_8045F564Fv(void* self);
