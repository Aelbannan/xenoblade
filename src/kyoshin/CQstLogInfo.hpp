#pragma once

#include <types.h>
#include <nw4r/lyt.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void*, void*, int, int);

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
    void func_802298A0();
    void func_80229900();

    // +0x00: vtable (implicit)
    u8 _04[0x1C];                             // 0x04 - padding to 0x1F
    nw4r::lyt::Layout* mUnk20;                             // 0x20
    nw4r::lyt::AnimTransform* field_0x24;     // 0x24
    nw4r::lyt::AnimTransform* field_0x28;     // 0x28
    u8 _2C[4];                                // 0x2C - padding to 0x2F
    u8 mUnk30;                                // 0x30
    u8 _31[3];                                // 0x31 - padding to 0x33
    u32 field_0x34;                           // 0x34
    u8 mField38;                              // 0x38
    u8 mField39;                              // 0x39
    u16 mField3A;                             // 0x3A
};
