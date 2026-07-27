#pragma once

#include <types.h>

class CQstLogInfo {
public:
    CQstLogInfo();
    virtual ~CQstLogInfo();
    void OnFileEvent();

    // +0x00: vtable
    void* mUnk20;       // 0x20
    u8 mUnk30;          // 0x30
    u8 mField38;        // 0x38
    u8 mField39;        // 0x39
    u16 mField3A;       // 0x3A
};
