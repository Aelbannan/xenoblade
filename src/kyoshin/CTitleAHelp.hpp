#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "revolution/gx/GXTypes.h"

#include <nw4r/lyt.h>

class CTitleAHelp : public IWorkEvent {
public:
    CTitleAHelp(char*, u8);
    virtual ~CTitleAHelp();
    void CTitleAHelp_load();
    void updateHelp();
    void drawHelp(nw4r::lyt::DrawInfo*);
    void teardown();
    u8 isInitialized();
    u8 isActive();
    int isIdle();  // retail callers test r3 with cmpwi (no u8 mask)
    void func_801C412C();
    void beginClose();
    void reopenFromClose();
    void markReplayClose();
    void setNameText(char*);
    // retail keeps func_801C41E8 unmangled (C linkage): free function in .cpp
    void func_801C4654(u32);
    void setInfoText(char*);
    void func_801C46DC(u32);
    void setActive(u8);
    virtual bool OnFileEvent(CEventFile* pEventFile) override;

    UnkClass_8045F564 unk4;
    CFileHandle* mFileHandle;                             //0x14
    nw4r::lyt::ArcResourceAccessor* mArcResourceAccessor; //0x18
    nw4r::lyt::Layout* mLayout;                           //0x1c
    nw4r::lyt::AnimTransform* mAnimTrans20;
    nw4r::lyt::AnimTransform* mAnimTrans24;
    u8 unk28;
    s32 unk2c;
    char* mName; //0x30
    u8 unk34;
    u8 unk35;
    u8 unk36;
    u8 unk37;
    CTitleAHelp();
};
