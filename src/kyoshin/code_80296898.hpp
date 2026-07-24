#pragma once

#include <types.h>

/// 64-byte game configuration structure.
/// Frame timing, flag values, and per-slot settings are initialized to defaults
/// in init() and can be overwritten via func_80296AE8.
class Class_80296898{
public:
    // 0x00-0x0E: slot configuration flags (first 8 bytes initialized to 1/2)
    u8 mSlotFlags[0x0F - 0x00];
    // 0x0F: frame counter for VI timing (default 10)
    u8 mFrameCount;
    // 0x10-0x3F: additional configuration values
    u8 mConfigData[0x40 - 0x10];

    Class_80296898(){
        init();
    }

    void init();
    static Class_80296898* getInstance();
};

Class_80296898* func_80296A04(Class_80296898* obj);
void func_80296AE8(u8* src);
