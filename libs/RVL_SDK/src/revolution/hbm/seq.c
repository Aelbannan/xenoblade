// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/hbm/seq
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void __HBMSEQInitTracks__FP15_HBMSEQSEQUENCEPUci() {}

void __HBMSEQReadHeader__FP15_HBMSEQSEQUENCEPUc() {}

void HBMSEQInit() {}

void HBMSEQQuit() {}

void HBMSEQRunAudioFrame() {}

void HBMSEQAddSequence() {}

void HBMSEQRemoveSequence() {}

void HBMSEQSetState() {}

extern "C" u32 HBMSEQGetState(void* self) { return *(u32*)((u8*)self + 0x4); }

extern "C" void HBMSYNSetMasterVolume(void*);
extern "C" void HBMSEQSetVolume(void* self) { HBMSYNSetMasterVolume((char*)self + 0x14); }
