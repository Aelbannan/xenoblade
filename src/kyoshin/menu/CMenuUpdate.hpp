#pragma once
#include "kyoshin/cf/CfCamSymbols.hpp"
#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Forward declarations
namespace nw4r { namespace lyt {
    class Layout;
    class AnimTransform;
    class DrawInfo;
    class ArcResourceAccessor;
}}

class CProcess;
class CScn;

// Object returned by Scn_QueryUnk80State (frame-timing provider)
struct Unk_8049603C {
    u8 _00[0xC];
    f32 field_C;
};

// View of the object returned by CDeviceFont::getFontInfo: vtable slot 9
// (offset 0x24) yields the handle bound via ApplyFontToTxtPanes. Never instantiated,
// so no vtable is emitted; the genuine virtual call makes MWCC emit the retail
// r12 dispatch chain.
class CMenuUpdateFontView {
public:
    virtual void vf0() = 0;
    virtual void vf1() = 0;
    virtual void vf2() = 0;
    virtual void vf3() = 0;
    virtual void vf4() = 0;
    virtual void vf5() = 0;
    virtual void vf6() = 0;
    virtual u32 getHandle() = 0; // +0x24
};

// "timg" texture object returned by the resource accessor; the pixel
// dimensions are reached through a two-level chain (obj+8 -> dims).
struct CMenuUpdateTexDims {
    u16 mW; // 0x00
    u16 mH; // 0x02
};
struct CMenuUpdateTexChain {
    CMenuUpdateTexDims* mDims; // 0x00
};
struct CMenuUpdateTexObj {
    u8 _00[8];
    CMenuUpdateTexChain* mChain; // 0x08
};

// 16-byte parameter struct used for window entries
struct CMenuUpdate_8014274C {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;

    void __ct__8014274C() {
        field_0 = 0;
        field_4 = 0;
        field_8 = 0;
        field_C = 0;
    }

    void init_8014274C(u32 v0, u32 v1, u32 v2, u32 v3) {
        field_0 = v0;
        field_4 = v1;
        field_8 = v2;
        field_C = v3;
    }
};

// Minimal view of the pad state returned by CfGameManager::getCurrentPad()
// (pressed-button word lives at +0x04).
struct CMenuUpdatePadData {
    u32 mHeldButtonFlags;    // +0x00
    u32 mPressedButtonFlags; // +0x04
};

// Window bases: the real monolib interfaces plus the canonical IUIWindow
// (kyoshin/IUIWindow.hpp). This TU derives from all three; MWCC emits the
// IWorkEvent (+0x6C) and IScnRender (+0x70) sub-vtables itself.
#include "kyoshin/IUIWindow.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CUICfManager.hpp"

class CMenuUpdate : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    // IUIWindow at +0x00 (size 0x6C)
    // IWorkEvent at +0x6C (size 0x04)
    // IScnRender at +0x70 (size 0x04)
    CScn* mScene; // 0x74
    UnkClass_8045F564 mMemRegion; // 0x78 (size 0x10)
    nw4r::lyt::AnimTransform* mAnim1; // 0x88
    nw4r::lyt::AnimTransform* mAnim2; // 0x8C
    nw4r::lyt::AnimTransform* mActiveAnim; // 0x90
    CMenuUpdate_8014274C mEntries[8]; // 0x94 - 0x113 (8 * 0x10 = 0x80)
    s32 mState; // 0x114
    s32 mSubState; // 0x118
    s32 mSubType; // 0x11C
    s32 mCounter; // 0x120
    u32 mMode; // 0x124, default 2
    f32 mFloat128; // 0x128
    f32 mFloat12C; // 0x12C

    CMenuUpdate(CScn* scene, u32 type, u32 arg1, u32 arg2, u32 arg3);
    virtual ~CMenuUpdate();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();
    void UpsertEntry(int type, u32 arg1, u32 arg2, u32 arg3);

    // Accessors
    u32 getField10() { return *(u32*)((u8*)this + 0x10); }
    void setField67(u8 val) { mField67 = val; }
    u8 getField64() { return mField64; }
    void setField64(u8 val) { mField64 = val; }
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
// One-arg form: retail emits 'li r3,1; bl' with no r4 setup - the second
// (Layout*) argument arrives as the &mAnim2 pointer the preceding
// bindLayoutAnimTransform call left in r4.
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dt__14Class_8045F858Fv(void*, int);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
extern "C" void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
extern "C" void* getInstance__9CTaskGameFv();
extern "C" bool isFlag01Set__9CTaskGameFv(void*);
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(void*, void*, u32, u32);
extern "C" void removeRenderCB__4CScnFP10IScnRender(void*, void*);
// One-arg form: retail emits 'li r3,1; bl' with no r4 setup - the second
// (Layout*) argument arrives as the &mAnim2 pointer the preceding
// bindLayoutAnimTransform call left in r4.
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32);
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* getWorkMem__17CWorkThreadSystemFv();
extern "C" void Regist__8CProcessFP8CProcessb(void*, void*, bool);
// BdatGetPtrDirect: declared with char* return in code_80135FDC.hpp
// BdatTouchStringCell: declared with char* return in code_80135FDC.hpp (caller-tuned)
extern "C" bool CUICfManager_hasInUseSlot();
extern "C" void UIWin_CreateSysWinBuff(u32);
extern "C" bool hasQuestWindow();
extern "C" bool QstCnt_HasInstance_6B94();
extern "C" void writePanePos(void*, void*);

extern "C" bool isKizunaTalkActive();
extern "C" bool SkipTimer_IsActiveFlag();
// Scn_QueryUnk80State: declared once in libs/monolib/src/scn/CScn_8049603C.hpp.

extern "C" void* CfRes_getE14();
extern "C" void CtrlRemote_SetSharedBit(u32, u32);
extern "C" void format__Q22ml10FixStr_32_FPCce(void*, const char*, ...);
extern "C" bool isSceneLoading__Q22cf13CfGameManagerFv();
extern "C" bool isSceneReadyForInput__Q22cf13CfGameManagerFv();

// C-linkage helpers for thunks
extern "C" void* MenuUpdate_SpawnProcessForMode(void* self, u32 r4, int r5, int r6, int r7, int r8);
extern "C" void MenuUpdate_MarkByte15Ready(void* self);
extern "C" void* getGlobalA10();
extern "C" int getGlobal0E0();
extern "C" void MenuUpdate_SignalGlobalField64();
extern "C" void MenuUpdate_FormatNotifyText(void* self, void* r4, void* r5);
extern "C" void* noop_80142D5C(void* self);
extern "C" void MenuUpdate_RebindLayout(void* self);
extern "C" int MenuUpdate_CheckBootGates(void* self);
extern "C" void MenuUpdate_EnterMode1Chime(void* self);
extern "C" void MenuUpdate_MaintainActive(void* self);
extern "C" f32 MenuUpdate_QueryUnk80Float();
extern "C" void MenuUpdate_GuardFirstEntry(void* self);
