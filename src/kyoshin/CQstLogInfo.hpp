#pragma once

#include <types.h>
#include "nw4r/lyt.h"

class CQstLogInfo {
public:
    CQstLogInfo();
    virtual ~CQstLogInfo();
    void OnFileEvent();

    u8 func_802296D0();
    u8 func_802296D8();
    void func_80229768(u16 val);
    void func_80229600(nw4r::lyt::DrawInfo* drawInfo);
    void func_802296E0();
    void func_80229724();
    // +0x00: vtable (implicit)
    u8 _04[0x20 - 0x04];              // 0x04-0x1F
    nw4r::lyt::Layout* mUnk20;        // 0x20
    u8 _24[0x30 - 0x24];              // 0x24-0x2F
    u8 mUnk30;                         // 0x30
    u8 _31[0x34 - 0x31];              // 0x31-0x33
    int mUnk34;                        // 0x34: state (0, 1, 3, 4)
    u8 mField38;                       // 0x38
    u8 mField39;                       // 0x39
    u16 mField3A;                      // 0x3A
};

void func_802298A0(CQstLogInfo*);
void func_80229900(CQstLogInfo*);
