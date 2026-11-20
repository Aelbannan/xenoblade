#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/core/CException.hpp" // IGameException
#include "monolib/util/FixStr.hpp"

// Canonical CTaskGame class definition (header-consolidation pilot 2026-10-23).
// Kept lean so every kyoshin TU can include it; the CTaskGame TU's private
// retail-name imports live in kyoshin/CTaskGameImports.hpp.

class CView;
class CWorkThread;
class CScnNw4r;
class CTaskGameEvt;

// Object stored in the four virtual-delete slots at CTaskGame +0x174..+0x184
// (Term deletes each non-null entry through its vtable deleting-dtor slot).
// Declaration-only dtor: no vtable is emitted in this TU.
class CTaskGameSlotObj {
public:
    virtual ~CTaskGameSlotObj();
};

class ITitleMenu{
public:
    virtual ~ITitleMenu(){}
    virtual void ITitleMenu__UnkVirtualFunc1() = 0;
};

class IErrMesWinSel{
public:
    virtual ~IErrMesWinSel(){}
    virtual void IErrMesWinSel__UnkVirtualFunc1() = 0;
};

struct UnkClass_8004041C{
    void CTaskGame_setVisionParam(u8 r4, float f1, int r5, u32 r6, u8 r7, u32 r8, u32 r9);

    u8 unk0;
    volatile float unk4; /* 0x04 */
    int unk8; /* 0x08 */
    u32 unkC; /* 0x0C */
    u8 unk10; /* 0x10 */
    volatile u32 unk14; /* 0x14 */
    u32 unk18; /* 0x18 */
};


class CTaskGame : public CTTask<CTaskGame>,
                  public IWorkEvent,
                  public IScnRender,
                  public IGameException,
                  public ITitleMenu,
                  public IErrMesWinSel {
public:
    CTaskGame(CView* pView, CWorkThread* pThread, int r6);
    virtual ~CTaskGame();

    static CTaskGame* getInstance();
    static u32 isFlag2000Set(); // isUnk68Bit13Set: (spInstance->unk68 >> 13) & 1
    virtual void Init();

    // Retail CTaskGame_isMoveHook compares the inherited CTTask move-hook ptmf against
    // a static ptmf pool entry (lbl_eu_80525850). Passed by value so MWCC
    // materializes the pool entry on the stack before __ptmf_cmpr.
    bool isMoveFunc(CTTask<CTaskGame>::MoveFunc f) const {
        return mMoveFunc == f;
    }

    // mMoveFunc is protected in CTTask; the pool-swap helpers assign the
    // whole 12-byte ptmf through this inline member.
    void setMoveFunc(CTTask<CTaskGame>::MoveFunc f) { mMoveFunc = f; }
    void setMoveFuncFrom(const CTTask<CTaskGame>::MoveFunc* src) { mMoveFunc = *src; }

    void setLoadingCaption(u16 r4, u16 r5, const char* r6, s16 r7);
    static bool isFlag01Set();
    void setInitFlag();
    void requestGameExit();
    static CTaskGame* create(CView* pView, CWorkThread* pThread, int r5);
    static s32 isMoveFuncActive();

    virtual void Term();
    void stub_80040A3C();
    void stub_80042720();
    void setFlag_1000(int value);
    virtual void ITitleMenu__UnkVirtualFunc1();
    virtual void IErrMesWinSel__UnkVirtualFunc1();
    virtual bool gameExceptionCB(u32 r4);

    CScnNw4r* getScene() const {
        return unk74;
    }

    //0x0: vtable (CTTask)
    //0x4-3C: CProcess
    //0x3C-54: CTTask
    //0x54: vtable 2 (IWorkEvent)
    //0x58: vtable 3 (IScnRender)
    //0x5C: vtable 4 (IGameException)
    //0x60: vtable 5 (ITitleMenu)
    //0x64: vtable 6 (IErrMesWinSel)
    u32 unk68; /* 0x68 */
    CWorkThread* unk6C; /* 0x6C */
    CView* unk70; /* 0x70 */
    CScnNw4r* unk74; /* 0x74 */
    u32 unk78; /* 0x78 */
    u32 unk7C; /* 0x7C */
    u16 unk80; /* 0x80 */
    u16 unk82; /* 0x82 */
    s16 unk84;   // mission/save id; retail CTaskGame_moveStartMission sign-extends it (lha) /* 0x84 */
    u16 unk86; /* 0x86 */
    u16 unk88; /* 0x88 */
    s16 unk8A;   // retail CTaskGame_moveStartMission sign-extends it (lha) into the format vararg /* 0x8A */
    s16 unk8C;   // retail lha/sth/extsh (cbRenderBefore frame counter) /* 0x8C */
    s16 unk8E; /* 0x8E */
    u8 unk90; /* 0x90 */
    u8 unk91[0xA0 - 0x91]; /* 0x91 */
    u32 unkA0; /* 0xA0 */
    ml::FixStr<32> unkA4; /* 0xA4 */
    u8 unkC8; /* 0xC8 */
    u8 unkC9[0xCC - 0xC9]; /* 0xC9 */
    u32 unkCC; /* 0xCC */
    u32 unkD0; /* 0xD0 */
    CTaskGameEvt* unkD4; /* 0xD4 */
    int unkD8; /* 0xD8 */
    u32 unkDC; /* 0xDC */
    float unkE0; /* 0xE0 */
    u32 unkE4; /* 0xE4 */
    int unkE8; /* 0xE8 */
    u32 unkEC; /* 0xEC */
    u32 unkF0; /* 0xF0 */
    u32 unkF4; /* 0xF4 */
    int unkF8; /* 0xF8 */
    u32 unkFC; /* 0xFC */
    u32 unk100; /* 0x100 */
    u8 unk104; /* 0x104 */
    u8 unk105[0x124 - 0x105]; /* 0x105 */
    u32 unk124; /* 0x124 */
    u32 unk128; /* 0x128 */
    u8 unk12C[0x130 - 0x12C]; /* 0x12C */
    u8 unk130; /* 0x130 */
    u8 unk131[0x170 - 0x131]; /* 0x131 */
    u32 unk170; /* 0x170 */
    CTaskGameSlotObj* unk174[5]; /* 0x174 */
    u8 unk188; /* 0x188 */
    u8 unk189[0x18C - 0x189]; //padding? /* 0x189 */
    UnkClass_8004041C unk18C; /* 0x18C */

protected:
    CTaskGame();
    void cbRenderBefore();
    void OnFileEvent();
    void Draw();
};

// Flat retail-name CRI streaming helper defined in CTaskGame.cpp. Callers in
// other TUs (e.g. CCollepedia) use this one canonical declaration.
extern "C" void CTaskGame_openStream(u32 a1, const char* path, u32 a3, u32 a4,
                              u32 a5, u32 a6, float volume);
