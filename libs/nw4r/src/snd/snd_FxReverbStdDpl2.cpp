// Auto-scaffolded catalog TU for nw4r/src/snd/snd_FxReverbStdDpl2
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Forward declarations for functions called by AssignWorkBuffer/ReleaseWorkBuffer
extern "C" void CreateHeap__Q44nw4r3snd6detail8AxfxImplFPvUl(void*, u32);
extern "C" void DestroyHeap__Q44nw4r3snd6detail8AxfxImplFv(void*);

extern "C" void __ct__Q34nw4r3snd15FxReverbStdDpl2Fv() {}

extern "C" void GetRequiredMemSize__Q34nw4r3snd15FxReverbStdDpl2Fv() {}

extern "C" void AssignWorkBuffer__Q34nw4r3snd15FxReverbStdDpl2FPvUl(void* self, u32 size) { ((void(*)(void*, u32))CreateHeap__Q44nw4r3snd6detail8AxfxImplFPvUl)((char*)self + 0x14, size); }

extern "C" void ReleaseWorkBuffer__Q34nw4r3snd15FxReverbStdDpl2Fv(void* self) { ((void(*)(void*))DestroyHeap__Q44nw4r3snd6detail8AxfxImplFv)((char*)self + 0x14); }

extern "C" void StartUp__Q34nw4r3snd15FxReverbStdDpl2Fv() {}

extern "C" void Shutdown__Q34nw4r3snd15FxReverbStdDpl2Fv() {}

extern "C" void SetParam__Q34nw4r3snd15FxReverbStdDpl2FRCQ44nw4r3snd6detail16FxReverbStdParam() {}

extern "C" void UpdateBuffer__Q34nw4r3snd15FxReverbStdDpl2FiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode() {}

extern "C" void OnChangeOutputMode__Q34nw4r3snd15FxReverbStdDpl2Fv() {}
