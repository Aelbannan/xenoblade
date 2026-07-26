// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnmdlsmpl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" void ScnMdlSmpl_CalcPosture__Q34nw4r3g3d12ScnMdlSimpleFUlPCQ34nw4r4math5MTX34() {}

extern "C" void ScnMdlSmpl_G3DPROC_GATHER_SCNOBJ__Q34nw4r3g3d12ScnMdlSimpleFUlPQ34nw4r3g3d13IScnObjGather() {}

extern "C" void ScnMdlSmpl_G3DPROC_CALC_VIEW__Q34nw4r3g3d12ScnMdlSimpleFUlPCQ34nw4r4math5MTX34() {}

extern "C" void ExecCallback_DRAW_OPA__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv(void) {}

extern "C" void ExecCallback_DRAW_XLU__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv(void) {}

extern "C" void G3dProc__Q34nw4r3g3d12ScnMdlSimpleFUlUlPv() {}

extern "C" void SetScnObjOption__Q34nw4r3g3d12ScnMdlSimpleFUlUl() {}

extern "C" void GetScnObjOption__Q34nw4r3g3d12ScnMdlSimpleCFUlPUl() {}

extern "C" void GetScnMtxPos__Q34nw4r3g3d12ScnMdlSimpleCFPQ34nw4r4math5MTX34Q44nw4r3g3d6ScnObj13ScnObjMtxTypeUl() {}

extern "C" void SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType() {}

extern "C" void RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObj() {}

extern "C" void RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d12ScnMdlSimple10AnmObjType() {}

extern "C" void GetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d12ScnMdlSimple10AnmObjType() {}

extern "C" void GetAnmObj__Q34nw4r3g3d12ScnMdlSimpleCFQ44nw4r3g3d12ScnMdlSimple10AnmObjType() {}

extern "C" void UpdateFrame__Q34nw4r3g3d12ScnMdlSimpleFv() {}

extern "C" void EnableScnMdlCallbackTiming__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d6ScnObj6Timing() {}

extern "C" void GetViewPosMtxArray__Q34nw4r3g3d12ScnMdlSimpleFv() {}

extern "C" void* GetViewNrmMtxArray__Q34nw4r3g3d12ScnMdlSimpleFv(void* r3) {
    void* r5 = *(void**)((char*)r3 + 0xf8);
    if (r5 == nullptr) {
        return nullptr;
    }
    unsigned short r0 = *(unsigned short*)((char*)r3 + 0x102);
    unsigned char r4 = *(unsigned char*)((char*)r3 + 0x101);
    unsigned int r3_val = r0 * 0x24;
    unsigned int aligned = (r3_val + 0x1f) & ~0x1f;
    unsigned int offset = r4 * aligned;
    return (void*)((char*)r5 + offset);
}

extern "C" void GetViewTexMtxArray__Q34nw4r3g3d12ScnMdlSimpleFv() {}

extern "C" void __ct__Q34nw4r3g3d12ScnMdlSimpleFP12MEMAllocatorQ34nw4r3g3d6ResMdlPQ34nw4r4math5MTX34PUlPQ34nw4r4math5MTX34PQ34nw4r4math5MTX33PQ34nw4r4math5MTX34ii() {}

extern "C" void __dt__Q34nw4r3g3d12ScnMdlSimpleFv() {}

extern "C" void __dt__Q34nw4r3g3d18ICalcWorldCallbackFv() {}

extern "C" void IsDerivedFrom__Q34nw4r3g3d12ScnMdlSimpleCFQ44nw4r3g3d6G3dObj7TypeObj() {}

extern "C" void GetTypeName__Q34nw4r3g3d12ScnMdlSimpleCFv() {}

extern "C" void* GetTypeObj__Q34nw4r3g3d12ScnMdlSimpleCFv(void) { return 0; }
