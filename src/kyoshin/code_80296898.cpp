#include "kyoshin/code_80296898.hpp"

#include <cstring>

static Class_80296898 lbl_8057A9C8;

Class_80296898* func_80296A04(Class_80296898* obj){
    obj->init();
    return obj;
}

void Class_80296898::init(){
    std::memset(mSlotFlags, 0, 64);

    // Slot configuration: values 1/2 likely indicate player/team assignments
    mSlotFlags[0x0] = 1;
    mSlotFlags[0x1] = 1;
    mSlotFlags[0x2] = 2;
    mSlotFlags[0x3] = 2;
    mSlotFlags[0x4] = 2;
    mSlotFlags[0x5] = 1;
    mSlotFlags[0x6] = 1;
    mSlotFlags[0x7] = 1;

    // Frame timing default
    mFrameCount = 10;

    // Configuration data at specific offsets
    mConfigData[0x00] = 1;
    mConfigData[0x01] = 0;
    mConfigData[0x10] = 1;
    mConfigData[0x11] = 1;
    mConfigData[0x12] = 0;
    mConfigData[0x13] = 0;
    mConfigData[0x14] = 1;
    mConfigData[0x15] = 1;
    mConfigData[0x20] = 1;
    mConfigData[0x22] = 1;
    mConfigData[0x21] = 0;
    mConfigData[0x28] = 3;
    mConfigData[0x29] = 3;
    mConfigData[0x2A] = 5;
}

extern "C" Class_80296898 lbl_eu_805772C8;

Class_80296898* Class_80296898::getInstance(void) {
    return &lbl_eu_805772C8;
}

// Overwrite the full configuration from external source.
// Resets mFrameCount to 10 if the incoming data had it at 0.
void func_80296AE8(u8* src){
    std::memcpy(lbl_8057A9C8.mSlotFlags, src, 0x40);

    if(lbl_8057A9C8.mFrameCount == 0){
        lbl_8057A9C8.mFrameCount = 0xA;
    }
}
