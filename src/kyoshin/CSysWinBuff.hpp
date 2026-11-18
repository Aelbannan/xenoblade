#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/IUIWindow.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

class CFileHandle;
class CScn;
class CEventFile;
class CSysWinBuff;

// Global data imports (MWCC does not mangle global-scope data names).
// (singleton/word moved to CSysWinBuff.cpp as single global defs to dodge
// a MWCC-1.1 decl-merging fault (10322) on these two.)
extern u32 lbl_eu_806640E0;           // message-table word read by Move's label lookup

// C-ABI helper imports with retail verbatim (unmangled) names; the defining
// TUs (CSysWin.cpp / monolib / code_80135FDC.cpp) emit these flat symbols.
extern "C" {
void __ct__8CProcessFv(CProcess* self);
void __dt__8CProcessFv(CProcess* self, int flags);
void __ct__7CSysWinFUc(void* syswin, int arg);
void __dt__7CSysWinFv(void* syswin, int flags);
void Bdat_GetTable_AA34();
s32 MenuTutorialIsCreated();
void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);

// CDeviceFile common-archive loader + work-memory handle query (unmangled
// retail C symbols; declared with C linkage so the call reloc keeps the
// verbatim names, per the repo-wide convention).
void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
    u32 handle, const char* path, void* workEvent, int a, int b);
u32 KyoshinHeap_GetField44();
}

// CSysWinBuff::OnFileEvent's body lives in the .cpp (retail keeps it out of
// line at 0x80276F08: the IWorkEvent sub-vtable slot holds the compiler's
// +0x6C adjusting thunk, which tail-jumps straight into it).
//

/*
 * System window "buff" process (singleton, created by create__11CSysWinBuffFv).
 *
 * Real MI class: IUIWindow at +0x00 (0x6C, brings the CTTask<IUIWindow> Move/
 * Draw ptmf hooks at +0x3C/+0x48 plus the layout/state members from +0x54),
 * IWorkEvent at +0x6C and IScnRender at +0x70 - so retail's
 * __vt__11CSysWinBuff (lbl_eu_80537DC0) carries the +0x24 (IWorkEvent) and
 * +0xAC (IScnRender) sub-vtables and the base list (lbl_eu_80537E84) names
 * IScnRender@0x70 / IWorkEvent@0x6C exactly as the compiler emits them.
 *
 * Layout (0xBC bytes, the size the factory allocates):
 *   0x00  IUIWindow base
 *   0x6C  IWorkEvent base
 *   0x70  IScnRender base (long-lived render callback subobject)
 *   0x74  mFileHandle - loaded System.arc handle
 *   0x78  mScene      - owning scene
 *   0x7C  mSysWin     - embedded CSysWin widget (0x3C)
 *   0xB8  mFlagB8     - state flag byte
 *   0xBA  mArgBA      - window-kind argument
 */
class CSysWinBuff : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    // Single-use ctor: retail has no out-of-line __ct__11CSysWinBuff symbol, the
    // whole body (CProcess ctor, CTTask<IUIWindow> vptr + null ptmf hooks,
    // IUIWindow state, composite + sub-vptrs, embedded CSysWin) is inlined into
    // the create() factory, so the body lives here.
    CSysWinBuff(CScn* scene, u16 arg)
        : mFileHandle(NULL), mScene(scene), mSysWin(0), mFlagB8(0), mArgBA(arg) {
        setPresentationFlag__Q22cf13CfGameManagerFv(true);
        code80135FDC_postIncByte_64080();
    }
    virtual ~CSysWinBuff();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    static CSysWinBuff* getInstance();

    CFileHandle* mFileHandle;  // 0x74 loaded System.arc handle
    CScn* mScene;              // 0x78 owning scene
    CSysWin mSysWin;           // 0x7C CSysWin storage (0x3C)
    u8 mFlagB8;                // 0xB8 state flag byte
    u16 mArgBA;                // 0xBA window-kind argument (u16)
};

// Abstract view into the embedded CSysWin vtable: MWCC adds 2 implicit entries
// (offset-to-top / RTTI) before the first user virtual, so virtual index 32
// lands at vtable +0x88 (the layout-build dispatch Init performs). Pure
// abstract -> MWCC emits no vtable for this view. Same idiom as
// CSysWinSave::CSysWinView / CCollepedia::CSysWinProxy.
struct CSysWinView {
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
    virtual void v20() = 0;  // virtual index 32 -> vtable +0x88 - Init layout build
};

// Data body of the embedded CSysWin (0x3C bytes). Init rebuilds the storage by
// constructing a stack temp and memberwise-copying every field except the
// vtable word (+0) and the 0x29-0x2B padding, so MWCC emits the exact
// lwz/stw + lbz/stb sequence retail shows (CSysWinSave's copy also skips the
// vtable; this one additionally skips the 3 pad bytes retail leaves alone).
struct CSysWinDataBuff {
    u32 vtable;    // 0x00 (retained from construction, not copied)
    u32 f_04;      // 0x04 (UnkClass_8045F564 mem region, 4 words)
    u32 f_08;      // 0x08
    u32 f_0c;      // 0x0c
    u32 f_10;      // 0x10
    u32 f_14;      // 0x14 (mFileHandle)
    u32 f_18;      // 0x18 (mTagProcessor)
    u32 f_1c;      // 0x1c (mArcAccessor)
    u32 f_20;      // 0x20 (mLayout)
    u32 f_24;      // 0x24 (mAnimTrans)
    u8  f_28;      // 0x28 (ready flag)
    // 0x29-0x2B padding (not copied)
    u32 f_2c;      // 0x2c
    u32 f_30;      // 0x30
    u8  f_34;      // 0x34
    u8  f_35;      // 0x35
    u8  f_36;      // 0x36
    u8  f_37;      // 0x37
    u8  f_38;      // 0x38
    u8  f_39;      // 0x39
};

// C++-linkage helpers (MWCC mangles the plain names to the retail forms
// closeFileHandle__FPP11CFileHandle / SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo).
