// Auto-scaffolded catalog TU for nw4r/src/snd/snd_FxDelayDpl2
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Forward declarations for functions called by AssignWorkBuffer/ReleaseWorkBuffer
extern "C" void CreateHeap__Q44nw4r3snd6detail8AxfxImplFPvUl(void*, u32);
extern "C" void DestroyHeap__Q44nw4r3snd6detail8AxfxImplFv(void*);

void __ct__Q34nw4r3snd11FxDelayDpl2Fv(){}

void GetRequiredMemSize__Q34nw4r3snd11FxDelayDpl2Fv(){}

extern "C" void AssignWorkBuffer__Q34nw4r3snd11FxDelayDpl2FPvUl(void* self, u32 size) { ((void(*)(void*, u32))CreateHeap__Q44nw4r3snd6detail8AxfxImplFPvUl)((char*)self + 0x14, size); }

extern "C" void ReleaseWorkBuffer__Q34nw4r3snd11FxDelayDpl2Fv(void* self) { ((void(*)(void*))DestroyHeap__Q44nw4r3snd6detail8AxfxImplFv)((char*)self + 0x14); }

void StartUp__Q34nw4r3snd11FxDelayDpl2Fv(){}

void Shutdown__Q34nw4r3snd11FxDelayDpl2Fv(){}

void SetParam__Q34nw4r3snd11FxDelayDpl2FRCQ44nw4r3snd6detail12FxDelayParam(){}

void UpdateBuffer__Q34nw4r3snd11FxDelayDpl2FiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode(){}

void OnChangeOutputMode__Q34nw4r3snd11FxDelayDpl2Fv(){}
