#pragma once

#include <types.h>
#include <nw4r/lyt.h>

class CQstLogInfo {
public:
    CQstLogInfo();
    virtual ~CQstLogInfo();
    void OnFileEvent();

    u8 func_802296D0();
    u8 func_802296D8();
    void func_80229768(u16 val);
    void func_80229600();
    void func_80229770();
    void func_802297BC();
    void func_80229808();

    // +0x00: vtable
    void* mUnk20;                             // 0x20
    nw4r::lyt::AnimTransform* field_0x24;     // 0x24
    nw4r::lyt::AnimTransform* field_0x28;     // 0x28
    u8 mUnk30;                                // 0x30
    u32 field_0x34;                           // 0x34
    u8 mField38;                              // 0x38
    u8 mField39;                              // 0x39
    u16 mField3A;                             // 0x3A
};
