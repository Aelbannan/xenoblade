#pragma once

#include "kyoshin/CSystemWindowApi.hpp"
#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "monolib/work/CProcess.hpp"
#include "kyoshin/CCol6CheckBat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
#include "kyoshin/CCol6Invite.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CBaseCur.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/math/CVec3.hpp"

#include <nw4r/lyt.h>
#include "monolib/device/CDeviceFontInfo.hpp"
#include <revolution/GX.h>

#include "monolib/scn/CScn.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

class UnkClass_80083298;  // cf::CfGameManager::getGameSubManager() result

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
    /* 0x54 */ nw4r::lyt::Layout* mAnimHost;    // layout anim host (SetAnimationEnable/Animate)
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
    /* 0x124 */ CCur18 mCur18;
    /* 0x13C */ CScrollBar mScrollBar;
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
    /* 0xA4 */ u8 mFieldA4;                      // byte flag, set 0x22 by Col6_Win2Interact_378C
    /* 0xA5 */ u8 mPadA5[3];                     // retail pads to 0xA8
    /* 0xA8 */ CCur18 mCur1;
    /* 0xC0 */ CCur18 mCur2;
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

// CMenuFade object (opaque here; getFadeMenu/isFadeActive gate on it).
class CCol6Fade;

// C++-linkage (mangled retail symbols) declared with the unmangled identifier
// so MWCC derives the retail mangled name (moved from code_80135FDC.hpp, which
// this unit no longer includes).

// C-linkage imports (retail symbols are unmangled - keep linkage verbatim)
extern "C" {
// CCur.cpp
void UIWin_CreateSysWin0(char* msg, int a, int b);                         // CUICfManager.cpp
void CtrlRemote_SetSharedBit(u32 destination, u32 value);                      // CfGameManager.cpp
CCol6Pad* getCurrentPad__Q22cf13CfGameManagerFv();                   // cf::CfGameManager
void UIWin_CreateItemMulti(u32, u32, u32, u32, u32, u32, u32, u32, u32);     // CUICfManager.cpp
s32 CItemBlock_getPtr20E8();                                                 // CItemBoxInfo.cpp
void CItemBlock_setCount(s32);                                             // CItemBoxInfo.cpp
void* CItem_findRecByFamily(u16);                                            // CItemBoxInfo.cpp
u32 CItem_sumFamilyByte6(u16);                                              // CItemBoxInfo.cpp
void CItemData_initFromFamily(void*, u16, u32);                                 // CItemBoxInfo.cpp
// CItem_initItemImplInstances is declared extern "C" by
// include/kyoshin/cf/CfGameManager.hpp (pulled in via harness_catalog); a
// local redeclaration here would mangle and trip MWCC 10197.
CCol6Fade* getFadeMenu();                                           // CMenuFade.cpp
int isFadeActive(CCol6Fade* fade);                                    // CMenuFade.cpp (byte result, tested unmasked)
void CUICfManager_queueFadeMenu(u32 a, u32 b, f32 x, f32 y, f32 z);                // CUICfManager.cpp
u32 GetItemMulti_IsActiveFlag();                                                   // code_80135FDC.cpp (any block condition active)
void notifyBattleSystem__Q22cf13CfGameManagerFv(u32, u32, u32, u32, u32);  // CfGameManager.cpp
void notifyCameraManager__Q22cf13CfGameManagerFv();                         // CfGameManager.cpp
int isCameraReady__Q22cf13CfGameManagerFv();                          // CfGameManager.cpp
void lookupEffectForResource__Q22cf13CfGameManagerFv(u32, u32, u32);           // CfGameManager.cpp
void* CfSoundMan_TouchSlotById(u16 handle);                                      // CfSoundMan.cpp
void isEffectReady__Q22cf13CfGameManagerFv(u32, ml::CVec3*, ml::CVec3*, u32, f32); // CfGameManager.cpp
UnkClass_80083298* getGameSubManager__Q22cf13CfGameManagerFv();       // CfGameManager.cpp
extern "C" void Col6FormatBufferString(char* buffer, const char* format, ...); // EU format helper (unmangled retail)
// CScrollBar / CCur18 helpers (retail-unmangled C-linkage names).
// CCur.cpp
void* getCfPadData__Q22cf13CfGameManagerFv();                        // cf::CfGameManager
void* Bdat_GetTable_AA34();                                               // bdat manager
u32 Bdat_GetMaxRow_B1EC(void*);                                            // bdat row count
// CTaskLOD/audio helpers: u8 params (values are passed unmasked by callers).
void activateLOD__8CTaskLODFv(u8);                                  // CTaskLOD.cpp
void deactivateLOD__8CTaskLODFv(u8);                                  // CTaskLOD.cpp
int getLODData__8CTaskLODFv(u8);                                   // CTaskLOD.cpp
void ScnData_FwdB7D9C(void*, u8);                                        // code_80135FDC.cpp
void ScnData_FwdB7DD4(void*, u8);                                        // code_80135FDC.cpp
void forwardMpfCallB__17UnkClass_8047BB54Fv(void*, u8);                 // code_80135FDC.cpp
void forwardMpfCallC__17UnkClass_8047BB54Fv(void*, u8);                 // code_80135FDC.cpp
void __ct__8CProcessFv(CProcess* self);                              // CProcess ctor
extern "C" void __ct__10CScrollBarFUc(void* self, u8 direction);                     // CScrollBar ctor (retail short name)
void __ct__7CSysWinFUc(void* self, int arg);                              // CSysWin ctor (retail short name)
int sprintf(char*, const char*, ...);                                // stdio (CRT)
}

// Layout-build / font helpers whose retail symbols are already mangled
// (buildLayout__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc
// etc.); declare with the full mangled name so the call-site relocs bind
// verbatim (same convention as CItemBoxGrid.hpp / CTalkWindow.hpp).
extern "C" {
CDeviceFontInfo* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
    u32 arg, nw4r::lyt::Layout* layout);
}

// C++-linkage (mangled retail symbols) anim/layout helpers - MUST stay
// outside the extern "C" block so MWCC mangles them (retail reloc names
// advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf / playUISound__FUl).
// NOTE: AnimRewindFrame is NOT in this group - retail keeps the bare unmangled
// symbol, so it is declared extern "C" below.

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
// col6HintBuild's item-box scan).
extern "C" const u32 lbl_eu_80667568;
extern "C" const u16 lbl_eu_8066756C;
extern "C" const u8 lbl_eu_8066756E;
extern "C" const u32 lbl_eu_80667570;
extern "C" const u16 lbl_eu_80667574;
extern "C" const u8 lbl_eu_80667576;
extern "C" u32 CtrlRemote_TouchBitByArg(u32 resourceId);
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__17UnkClass_8045F564Fv(void* self);
extern "C" void CUICfManager_setMessageWindowVisible(u8 enable);                                // message-window show/hide
extern "C" void UIWin_CreateQuestWin(int id, int a, int b);                     // window open request
extern u32 lbl_eu_80663E28;  // .sbss mode bitfield (bit 0x400000 gates hint draws)
extern "C" void __ct__17UnkClass_8045F564Fv(void* self);
