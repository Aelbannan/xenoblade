// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/mix/mix
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void __MIXSetPan() {}

void __MIXGetVolume() {}

void MIXInit() {}

void MIXQuit() {}

void MIXInitChannel() {}

void MIXReleaseChannel(void* arg) {
    extern void* __MIXChannel;
    int channelIndex = *(int*)((char*)arg + 0x18);
    char* base = (char*)__MIXChannel;
    *(int*)(base + channelIndex * 0x70) = 0;
}

void MIXSetInput() {}

void MIXSetPan() {}

void MIXSetFader() {}

void MIXUpdateSettings() {}
