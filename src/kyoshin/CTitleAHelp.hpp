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
    void func_801C3FF0();
    void func_801C4080(nw4r::lyt::DrawInfo*);
    void func_801C40A0();
    u8 func_801C4114();
    u8 func_801C411C();
    u8 isIdle();
    void func_801C412C();
    void func_801C414C();
    void func_801C416C();
    void func_801C4198();
    void func_801C41C0(char*);
    // retail keeps func_801C41E8 unmangled (C linkage): free function in .cpp
    void func_801C4654(u32);
    void func_801C46B4(char*);
    void func_801C46DC(u32);
    void func_801C473C(u8);
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

// View into the object returned by CDeviceFont::func_80452C10: vtable+0x24
// (slot 9, no args) yields the u32 font handle bound by func_8013676C.
// All-pure so no vtable is emitted.
class CTitleAHelpFontView {
public:
    virtual void vf0() = 0; // +0x08
    virtual void vf1() = 0; // +0x0C
    virtual void vf2() = 0; // +0x10
    virtual void vf3() = 0; // +0x14
    virtual void vf4() = 0; // +0x18
    virtual void vf5() = 0; // +0x1C
    virtual void vf6() = 0; // +0x20
    virtual u32 vf7() = 0;  // +0x24
};

bool func_801C4648(nw4r::lyt::Pane*);

// One row of the title-A help table: 7 u16 cells indexed by button id.
struct HelpRow {
    u16 cell[7];
};

extern HelpRow lbl_eu_80533E60[];

// Global (retail-unmangled) helper returning the UI layout resource accessor.
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();

// Retail-unmangled CfGameManager static helper (takes a selector mask).
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int selector);

// Vector copy helper (defined in CTagProcessor.cpp).
extern "C" void copyVEC3(void* dst, const void* src);
