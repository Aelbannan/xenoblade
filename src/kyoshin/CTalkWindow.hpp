#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/IUIWindow.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include <nw4r/lyt.h>
#include <nw4r/math/math_types.h>
#include <nw4r/ut/ut_TagProcessorBase.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
#include "kyoshin/CUICfManager.hpp"

class CScn;

/*
 * CTalkWindow - the message/talk window process (created by
 * TalkWin_Create_CC78 on the work heap and registered as a CProcess under the
 * caller's parent).
 *
 * Real MI class: retail __vt__11CTalkWindow (0x8052DFA8) is the composite
 * table with the +0x24 (IWorkEvent) and +0xAC (IScnRender) sub-vtables and the
 * trailing override-pointer block, and the 0x40 base list at 0x8052E068 lists
 * IUIWindow (-> CTTask<IUIWindow> -> CProcess -> CChildListNode ->
 * CDoubleListNode), IWorkEvent and IScnRender. The +0x6C/+0x70 adjusting
 * thunks and the RTTI record are compiler output.
 *
 *   0x00  IUIWindow base (CProcess header, Move/Draw pmf hooks at +0x3C/+0x48,
 *         layout/state fields at +0x54..+0x68)
 *   0x6C  IWorkEvent base
 *   0x70  IScnRender base (render callback registered with the scene)
 *   0x74  mScene      - owner scene
 *   0x78  mMemRegion  - embedded layout-build scratch region (0x10)
 *   0x88  field_88..field_98 - the five layout animations
 *   0x9C  field_9C    - message-buffer pointer (ctor arg)
 *   0xA0  field_A0    - "default animation set" flag (ctor arg)
 *   0xA1  field_A1    - side flag (ctor arg)
 *   0xA4  field_A4    - page index (into the page-pane-name table)
 *   0xA8  field_A8    - page-position table index
 *   0xAC  field_AC
 *   0xB0  field_B0    - window state (1..5 dispatch in Move)
 *   0xB4  field_B4    - talk-enable flag (ctor arg)
 */
class CTalkWindow : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    CTalkWindow(CScn* scene, u32 sourceId, u8* msg, u32 a0, u32 a1, u32 b4);
    virtual ~CTalkWindow();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void Draw();
    virtual void cbRenderBefore(CScn* scene);

    CScn* mScene;                       // 0x74 owning scene
    UnkClass_8045F564 mMemRegion;       // 0x78 embedded layout-build region
    nw4r::lyt::AnimTransform* field_88; // 0x88 intro animation
    nw4r::lyt::AnimTransform* field_8C; // 0x8C page-in animation
    nw4r::lyt::AnimTransform* field_90; // 0x90 page animation
    nw4r::lyt::AnimTransform* field_94; // 0x94 accept animation
    nw4r::lyt::AnimTransform* field_98; // 0x98 page-out animation
    u8*   field_9C;                     // 0x9C message-buffer pointer
    u8    field_A0;                     // 0xA0 default-animation-set flag
    u8    field_A1;                     // 0xA1 side flag
    u8    _A2[2];                       // 0xA2
    u32   field_A4;                     // 0xA4 page index
    u32   field_A8;                     // 0xA8 page-position index
    u32   field_AC;                     // 0xAC
    u32   field_B0;                     // 0xB0 window state
    u8    field_B4;                     // 0xB4 talk-enable flag
    u8    _B5[3];                       // 0xB5
}; // size: 0xB8

// Page-pane names indexed by CTalkWindow::field_A4 (retail .data +0x00).
// The two short literals are also used directly by Init, which is why they
// appear in the .sdata2 pool as well as here.
extern const char* TalkWinPagePaneName[4];

// Page-position table filled by the TU static initializer (retail .bss 0xC0).
extern nw4r::math::VEC3 TalkWinPagePos[16];

// Talk-window views of the talk-source objects (func_800BBC0C result). The
// character voice sub-object at +0x98 carries play at vtable 0x58 (same
// slot as TagMemberObj::v58 in CTagProcessor.hpp); the message-state object
// at +0xC4 carries the page flags and is fed to getAnimModelId / func_8004B9D4.
class CTalkWinVoice {
public:
    virtual ~CTalkWinVoice();  // 0x00 (dtor at 0x08)
    virtual void v0C(); // 0x0C
    virtual void v10(); // 0x10
    virtual void v14(); // 0x14
    virtual void v18(); // 0x18
    virtual void v1C(); // 0x1C
    virtual void v20(); // 0x20
    virtual void v24(); // 0x24
    virtual void v28(); // 0x28
    virtual void v2C(); // 0x2C
    virtual void v30(); // 0x30
    virtual void v34(); // 0x34
    virtual void v38(); // 0x38
    virtual void v3C(); // 0x3C
    virtual void v40(); // 0x40
    virtual void v44(); // 0x44
    virtual void v48(); // 0x48
    virtual void v4C(); // 0x4C
    virtual void v50(); // 0x50
    virtual void v54(); // 0x54
    virtual void play(int flag, int priority); // 0x58
};

struct CTalkWinTalkC4 {
    u8  _00[0x270];      // 0x00
    u32 field_270;       // 0x270 (talk-stop flag bit 0x80)
};

struct CTalkWinTalkSrc {
    u8  _00[0x98];                    // 0x00 vtable + data
    CTalkWinVoice* mVoice;            // 0x98 character voice
    u8  _9C[0xC4 - 0x9C];             // 0x9C
    CTalkWinTalkC4* mTalkC4;          // 0xC4 message state
};

// Talk-window anim frame slot: the retail talk-window AnimTransform variant
// keeps the current frame at +0x10 (the nw4r header does not declare it).
struct CTalkAnimFrame {
    u8  _00[0x10];   // 0x00 vtable + resource data
    f32 mFrame;      // 0x10
};

// Position components read from the talk-source CfObject_findNodeMatrix(name)
// result
// (0x10-byte stride: +0x0C / +0x1C / +0x2C).
struct CTalkWinPosObj {
    u8  _00[0x0C];
    f32 field_0x0C; // +0x0C
    u8  _10[0x1C - 0x10];
    f32 field_0x1C; // +0x1C
    u8  _20[0x2C - 0x20];
    f32 field_0x2C; // +0x2C
};

// findObjectById(id) result (talk-source actor) for Init / updateTalkWindow.
// The actor is a cf::CfObject: its virtuals carry the dispatched slots -
// 0x40 talk text (CObjectParam_getParamPtr), 0xAC position fallback
// (CfObject_getPosVector), 0x120 named-position lookup
// (CfObject_findNodeMatrix) and 0x158 voice stop
// (setPointEnabled). Call sites cast to cf::CfObject and call
// those directly; no TU-local view is emitted. Field +0x8C carries the
// case-4 message id.
struct CTalkActorId {
    u8  _00[0x8C];
    u16 msgId8C;                       // 0x8C message id (case 4)
};

// CTagProcessor field view for Init's early-init switch: vtable+0x14 is the
// message-setup call (retail word: TagProcPreprocessMessage, same (src, a, b, c) arity
// as its free-function signature), +0x814..+0x81C the mode/flag/message
// fields. Deriving from the real nw4r base puts Proc at 0x14 with no filler.
class CTalkWinTagProc : public nw4r::ut::TagProcessorBase<wchar_t> {
public:
    virtual u16* Proc(const void* src, f32 a, f32 b, u32 c);  // 0x14
    u8  _04[0x814 - 0x04];   // 0x04..0x813 (vptr at 0x0)
    u8  field_0x814;                   // 0x814 switch type (0..8)
    u8  field_0x815;                   // 0x815 option bits (Init reads bits 0/1/2)
    u8  field_0x816;                   // 0x816 page index -> field_A4
    u8  field_0x817;                   // 0x817 page-loop flag -> field_A8
    u16 field_0x818;                   // 0x818 message id (cases 1/5)
    u8  field_0x81A;                   // 0x81A case-0 flag
    u8  _81B;                          // 0x81B
    f32 field_0x81C;                   // 0x81C case-0 scale
};

// nw4r Pane trailing-region view: the talk-window layout panes carry a flag
// byte at +0xBB (mFlag) and, for the 0xf1 pane, a tag-processor pointer at
// +0xF8. Slots 0x74/0x78 are the TextBox string-buffer alloc/free pair
// (retail nw4r version places them there), dispatched by Init (results
// discarded). Never instantiated - cast view only.
class CTalkWinPane {
public:
    virtual ~CTalkWinPane();           // 0x00 (dtor at 0x08)
    virtual void v0C();                // 0x0C
    virtual void v10();                // 0x10
    virtual void v14();                // 0x14
    virtual void v18();                // 0x18
    virtual void v1C();                // 0x1C
    virtual void v20();                // 0x20
    virtual void v24();                // 0x24
    virtual void v28();                // 0x28
    virtual void v2C();                // 0x2C
    virtual void v30();                // 0x30
    virtual void v34();                // 0x34
    virtual void v38();                // 0x38
    virtual void v3C();                // 0x3C
    virtual void v40();                // 0x40
    virtual void v44();                // 0x44
    virtual void v48();                // 0x48
    virtual void v4C();                // 0x4C
    virtual void v50();                // 0x50
    virtual void v54();                // 0x54
    virtual void v58();                // 0x58
    virtual void v5C();                // 0x5C
    virtual void v60();                // 0x60
    virtual void v64();                // 0x64
    virtual void v68();                // 0x68
    virtual void v6C();                // 0x6C
    virtual void v70();                // 0x70
    virtual void AllocStringBuffer(u16 len);  // 0x74
    virtual void FreeStringBuffer();          // 0x78
    u8  _04[0xBB - 0x04];    // 0x04..0xBA (vptr at 0x0)
    u8  mFlag;                         // 0xBB
    u8  _BC[0xF8 - 0xBC];    // 0xBC..0xF7
    u32 field_F8;                      // 0xF8 tag-processor pointer
};

// getFontInfo(1, layout) result view: vtable+0x24 returns the font handle
// handed to ApplyFontToTxtPanes.
class CTalkFontObj {
public:
    virtual ~CTalkFontObj();           // 0x00 (dtor at 0x08)
    virtual void v0C();                // 0x0C
    virtual void v10();                // 0x10
    virtual void v14();                // 0x14
    virtual void v18();                // 0x18
    virtual void v1C();                // 0x1C
    virtual void v20();                // 0x20
    virtual u32 getFontHandle();       // 0x24
};

// Opaque Scn_FindCamItem(scene, -1) pose block passed to CamMan_ProjectToScreen.
struct CTalkWinPose;

// C-ABI function imports: these retail symbols are unmangled (or carry a
// pre-mangled name), so they must be declared `extern "C"` to stop MWCC
// appending a __F<argtypes> suffix to the emitted reloc names.
extern "C" {
void __ct__8CProcessFv(CProcess* self);
void __dt__8CProcessFv(CProcess* self, int flags);
void updateTalkWindow(CTalkWindow* self);
void driveTalkIntro(CTalkWindow* self);
void driveTalkMessage(CTalkWindow* self);
void layoutTalkPanes(CTalkWindow* self);
CTalkWinTalkSrc* func_800BBC0C();
int getAnimModelId(CTalkWinTalkC4* talkC4);
void func_8004B9D4(CTalkWinTalkC4* talkC4, int a, int b, int c, int d);
int TagProcPumpMessage(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                  nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);
int TagProcStepMessage(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                  nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);
void TagProcResetPage(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                   nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);
void CUICfManager_queueFadeMenu(u8 a, int b, float c, float d, float e);
void UIWin_CreateQuestWin(u16 id, int a, int b);
void UIWin_CreateShopWin(u16 id);
void UIWin_CreateBEDE0Win(u16 id);
void TagProcRenderPage(void* tagProc, nw4r::lyt::Pane* a, nw4r::lyt::Pane* b,
                    nw4r::lyt::Pane* c, int flag);
void* __ct__CTagProcessor(void* self);
CTalkWinPose* Scn_FindCamItem(CScn* scene, int index);
void CamMan_ProjectToScreen(nw4r::math::VEC3* out, CTalkWinPose* pose,
                   const nw4r::math::VEC3* in);
}

// C++-linkage imports (retail emits the mangled forms).
void* findObjectById(int id);

#include "monolib/scn/CScn.hpp"

#include "kyoshin/CTaskGame.hpp"

// Global data imports (MWCC does not mangle global-scope data names).
extern u32 gmPauseFlags;    // cbRenderBefore flag (.sbss)
extern u32 __ptmf_null[3];     // null pointer-to-member-function constant

// Talk-window global state (.sbss 0x80664040..0x80664048).
extern u8 gTalkWinPageModeFlag; // page-mode flag (0 = n-side, 1 = a-side)
extern u32 gTalkWinLiveCount;  // live talk-window counter
