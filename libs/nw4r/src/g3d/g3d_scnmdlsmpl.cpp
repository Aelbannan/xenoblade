// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnmdlsmpl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ScnMdlSmpl_CalcPosture__Q34nw4r3g3d12ScnMdlSimpleFUlPCQ34nw4r4math5MTX34(){}

void ScnMdlSmpl_G3DPROC_GATHER_SCNOBJ__Q34nw4r3g3d12ScnMdlSimpleFUlPQ34nw4r3g3d13IScnObjGather(){}

void ScnMdlSmpl_G3DPROC_CALC_VIEW__Q34nw4r3g3d12ScnMdlSimpleFUlPCQ34nw4r4math5MTX34(){}

void ExecCallback_DRAW_OPA__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv(void) {}

void ExecCallback_DRAW_XLU__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv(void) {}

void G3dProc__Q34nw4r3g3d12ScnMdlSimpleFUlUlPv(){}

void SetScnObjOption__Q34nw4r3g3d12ScnMdlSimpleFUlUl(){}

void GetScnObjOption__Q34nw4r3g3d12ScnMdlSimpleCFUlPUl(){}

void GetScnMtxPos__Q34nw4r3g3d12ScnMdlSimpleCFPQ34nw4r4math5MTX34Q44nw4r3g3d6ScnObj13ScnObjMtxTypeUl(){}

void SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

void RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObj(){}

void RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

void GetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

void GetAnmObj__Q34nw4r3g3d12ScnMdlSimpleCFQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

void UpdateFrame__Q34nw4r3g3d12ScnMdlSimpleFv(){}

void EnableScnMdlCallbackTiming__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d6ScnObj6Timing(){}

void GetViewPosMtxArray__Q34nw4r3g3d12ScnMdlSimpleFv(){}

void* GetViewNrmMtxArray__Q34nw4r3g3d12ScnMdlSimpleFv(void* r3) {
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

void GetViewTexMtxArray__Q34nw4r3g3d12ScnMdlSimpleFv(){}

void __ct__Q34nw4r3g3d12ScnMdlSimpleFP12MEMAllocatorQ34nw4r3g3d6ResMdlPQ34nw4r4math5MTX34PUlPQ34nw4r4math5MTX34PQ34nw4r4math5MTX33PQ34nw4r4math5MTX34ii(){}

void __dt__Q34nw4r3g3d12ScnMdlSimpleFv(){}

void __dt__Q34nw4r3g3d18ICalcWorldCallbackFv(){}

void IsDerivedFrom__Q34nw4r3g3d12ScnMdlSimpleCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d12ScnMdlSimpleCFv(){}

extern "C" const char lbl_eu_8051D7A8[];
extern "C" void* GetTypeObj__Q34nw4r3g3d12ScnMdlSimpleCFv(void) { return (void*)lbl_eu_8051D7A8; }
