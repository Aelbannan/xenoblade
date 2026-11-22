#include "kyoshin/code_80296898.hpp"

#include <cstring>

// Singleton storage. Defining it here (with non-trivial ctor) makes MWCC emit
// the auto `__sinit_\code_80296898_cpp` static initializer that tail-calls
// init() on this object before main() runs.
Class_80296898 gGameConfig; // name-guess: singleton returned by getInstance()

Class_80296898* initInstance(Class_80296898* obj){
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

    // Configuration data (mConfigData starts at struct offset 0x10)
    mConfigData[0x00] = 1;
    mConfigData[0x01] = 0;
    mConfigData[0x0C] = 0;
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

    // Retail reads from the global singleton at offset 0x1C (mConfigData[0x0C])
    setErrMesLanguage(gGameConfig.mConfigData[0x0C] != 0);
}



Class_80296898* Class_80296898::getInstance(void) {
    return &gGameConfig;
}


// Overwrite the full configuration from external source.
// Resets mFrameCount to 10 if the incoming data had it at 0.
void updateConfig(u8* src){
    std::memcpy(gGameConfig.mSlotFlags, src, 0x40);

    if(gGameConfig.mFrameCount == 0){
        gGameConfig.mFrameCount = 0xA;
    }

    setErrMesLanguage(gGameConfig.mConfigData[0x0C] != 0);
}
