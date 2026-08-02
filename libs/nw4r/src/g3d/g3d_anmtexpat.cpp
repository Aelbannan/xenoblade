// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_anmtexpat
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

bool AnmObjTexPat::TestExistence(u32 idx) const {
    return (mpBinding[idx] & (BINDING_INVALID | BINDING_UNDEFINED)) == 0;
}

f32 AnmObjTexPatRes::GetFrame() const {
    return GetFrm();
}

} // namespace g3d
} // namespace nw4r

void Release__Q34nw4r3g3d12AnmObjTexPatFv(){}

int Attach__Q34nw4r3g3d12AnmObjTexPatFiPQ34nw4r3g3d15AnmObjTexPatRes(void) { return 0x0; }

int Detach__Q34nw4r3g3d12AnmObjTexPatFi(void) { return 0x0; }

void DetachAll__Q34nw4r3g3d12AnmObjTexPatFv(void) {}

void __dt__Q34nw4r3g3d12AnmObjTexPatFv(){}

void Construct__Q34nw4r3g3d15AnmObjTexPatResFP12MEMAllocatorPUlQ34nw4r3g3d12ResAnmTexPatQ34nw4r3g3d6ResMdlb(){}

void SetFrame__Q34nw4r3g3d15AnmObjTexPatResFf(){}

void SetUpdateRate__Q34nw4r3g3d15AnmObjTexPatResFf(){}

float GetUpdateRate__Q34nw4r3g3d15AnmObjTexPatResCFv(void *self) { return *(float *)((char *)self + 28); }

void UpdateFrame__Q34nw4r3g3d15AnmObjTexPatResFv(){}

void Bind__Q34nw4r3g3d15AnmObjTexPatResFQ34nw4r3g3d6ResMdl(){}

void GetResult__Q34nw4r3g3d15AnmObjTexPatResFPQ34nw4r3g3d15TexPatAnmResultUl(){}

void G3dProc__Q34nw4r3g3d15AnmObjTexPatResFUlUlPv(){}

void ApplyTexPatAnmResult__Q24nw4r3g3dFQ34nw4r3g3d9ResTexObjQ34nw4r3g3d10ResTlutObjPCQ34nw4r3g3d15TexPatAnmResult(){}

void IsDerivedFrom__Q34nw4r3g3d15AnmObjTexPatResCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void IsDerivedFrom__Q34nw4r3g3d12AnmObjTexPatCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d15AnmObjTexPatResCFv(){}

extern const char lbl_eu_8051D578[];
void* GetTypeObj__Q34nw4r3g3d15AnmObjTexPatResCFv(void) { return (void*)lbl_eu_8051D578; }

void __dt__Q34nw4r3g3d15AnmObjTexPatResFv(){}

void GetTypeName__Q34nw4r3g3d12AnmObjTexPatCFv(){}

extern "C" const char lbl_eu_8051D560[];
extern "C" void* GetTypeObj__Q34nw4r3g3d12AnmObjTexPatCFv(void) { return (void*)lbl_eu_8051D560; }
