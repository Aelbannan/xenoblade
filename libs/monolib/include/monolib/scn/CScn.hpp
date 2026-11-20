#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"

// Canonical CScn definition. Every TU that needs CScn (kyoshin render-callback
// users, monolib scene managers, CScnNw4r) includes this header; the CScn TU
// adds its private helpers in libs/monolib/src/scn/CScn.hpp on top of it.
// Kept lean on purpose (no umbrella includes, no C-linkage imports) so that any
// TU can include it without dragging in unrelated declarations.

class IScnRender;
class ICulling;
class CScnItemPool;
class CScnFilterMan;
class CScnFadeMan;
class CScnEnvLgtCtrl;
struct CScnFrame;

struct ScnRenderCB {
    IScnRender* cb; //0x0
    u32 prio; //0x4
    u8 flag; //0x8
}; // size = 0xC

// Camera-work blob at CScn+0x68 (CScnCameraMan-related); Draw reads +0x34 via lwz+extsh.
struct UnkScn68 {
    u8 unk00[0x34];
    s32 unk34; //0x34
};

// Object at CScn+0x8C; Draw calls vf+0x18 / vf+0x1C (RTTI vtable).
struct UnkScn8C {
    virtual ~UnkScn8C() {}
    virtual void vf0C() = 0;
    virtual void vf10() = 0;
    virtual void vf14() = 0;
    virtual void vf18() = 0;
    virtual void vf1C() = 0;
};

class CScn : public CTTask<CScn>, public IWorkEvent{
public:
    // Retail: __dt__4CScnFv (0x80499E30) is out of line; declaring it here
    // keeps MWCC from synthesizing an inline destructor in user TUs.
    CScn();
    virtual ~CScn();
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
    void initSubsystems(u32 a, u32 b, void* str, u32 d);
    virtual void Init();
    void Term();
    virtual void Move();
    virtual void Draw();
    // IWorkEvent overrides (secondary base): MWCC appends their real slots
    // to the end of __vt__4CScn in this declaration order.
    virtual bool WorkEvent4();
    virtual bool WorkEvent3(UNKTYPE* r4);
    // 0x8049A068: tail-calls mUnk8C's vtable+0x2C virtual, yielding the
    // scene's heap handle (an mtl::ALLOC_HANDLE, i.e. u32).
    u32 getAllocHandle();

    //0x000: vtable 1 (CTTask)
    //0x000-054: CTTask
    //0x054: vtable 2 (IWorkEvent)
    u32 unk58; //0x058
    void* unk5C; //0x05C subsystem released by Term
    CScnItemPool* mItemPool; //0x060 (CScnIdMan reads sub-pool 5)
    void* unk64; //0x064 subsystem released by Term
    UnkScn68* mCamWork; //0x068
    void* unk6C; //0x06C subsystem released by Term
    CScnFilterMan* mFilterMan; //0x070
    void* unk74; //0x074 subsystem released by Term
    void* unk78; //0x078 subsystem released by Term
    CScnEnvLgtCtrl* mLgtCtrl; //0x07C light control (destroyed directly by Term)
    CScnFadeMan* mFadeMan; //0x080
    CScnFrame* mFrame84; //0x084 scene frame (mElapsed/mStep read by Scn_GetFrameDelta/SetTimeScale)
    void* unk88; //0x088 scene-root subobject destroyed by ~CScn
    UnkScn8C* mUnk8C; //0x08C
    u32 unk90; //0x090
    u32 unk94; //0x094 (ctor leaves it uninitialized)
    f32 unk98; //0x098 scene time scale
    u16 unk9C; //0x09C
    u16 unk9E[10]; //0x09E
    u16 unkB2; //0x0B2
    ICulling* unkB4; //0x0B4
    u8 unkB8; //0x0B8 (ctor stores a byte)
    ScnRenderCB mRenderCBs[64]; //0x0BC
    u32 mRenderCBCount; //0x3BC
    char unk3C0[0x20]; //0x3C0
    u32 unk3E0; //0x3E0
    u8 unk_3E4; //0x3E4
    u8 unk_3E5; //0x3E5
    u8 unk_3E6; //0x3E6
    u8 unk_3E7; //0x3E7
    u8 unk_3E8; //0x3E8
    u8 unk_3E9; //0x3E9 PAL-fix flag (Scn_GetFrameDelta/Scn_SetPalFixFlag)
    u8 unk_3EA[0x3EC - 0x3EA]; //0x3EA
}; // size = 0x3EC


// Thin owner-API header for monolib/src/scn/CScn (retail Scn_GetFrameDelta,
// defined in CScn.cpp; canonical declaration also appears in
// monolib/core/code_804E36DC.hpp). Scene delta-time query: PAL consoles
// without the speed fix run at 1.2x.

class CScn;

// Real C++ entry points defined in monolib/src/scn/CScn.cpp.
f32 Scn_GetFrameDelta(void* view);
void Scn_SetTimeScale(CScn* self, float value);
