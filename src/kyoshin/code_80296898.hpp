#pragma once

#include <types.h>

/// 64-byte game configuration structure.
/// Frame timing, flag values, and per-slot settings are initialized to defaults
/// in init() and can be overwritten via updateConfig.
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

Class_80296898* initInstance(Class_80296898* obj);
void updateConfig(u8* src);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// External function referenced by both init() and updateConfig.
// Takes a boolean (0 or 1) derived from mConfigData[0x0C].
extern "C" void func_eu_802B14F8(u8);
