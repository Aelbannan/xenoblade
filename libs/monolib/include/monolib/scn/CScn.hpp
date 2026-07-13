#pragma once

#include "monolib/work.hpp"

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

// IScnRender draw callback shape: vt+0xC(cb, scn).
struct IScnRenderDraw {
    virtual ~IScnRenderDraw() {}
    virtual void onRender(CScn* scn) = 0;
};

class CScn : public CTTask<CScn>, public IWorkEvent{
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
    virtual void Draw();

    //0x000: vtable 1 (CTTask)
    //0x000-054: CTTask
    //0x054: vtable 2 (IWorkEvent)
    u8 unk58[0x68 - 0x058]; //0x058
    UnkScn68* mCamWork; //0x068
    u8 unk6C[0x8C - 0x06C]; //0x06C
    UnkScn8C* mUnk8C; //0x08C
    u8 unk90[0x0B4 - 0x090]; //0x090
    ICulling* unkB4; //0x0B4
    u32 unkB8; //0x0B8
    ScnRenderCB mRenderCBs[64]; //0x0BC
    u32 mRenderCBCount; //0x3BC
    char unk3C0[0x3E4 - 0x3C0]; //0x3C0
    u8 unk_3E4; //0x3E4
    u8 unk_3E5; //0x3E5
    u8 unk_3E6; //0x3E6
    u8 unk_3E7; //0x3E7
    u8 unk_3E8; //0x3E8
    u8 unk_3E9; //0x3E9
    u8 unk_3EA[0x3EC - 0x3EA]; //0x3EA
}; // size = 0x3EC
