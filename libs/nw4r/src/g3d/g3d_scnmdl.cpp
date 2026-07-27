// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnmdl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void* GetResTexSrt__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFb(void* self, bool enable)
{
    unsigned char* object = static_cast<unsigned char*>(self);
    void* resource = *reinterpret_cast<void**>(object);
    if (resource == 0 || *reinterpret_cast<void**>(object + 0x10) == 0)
        return 0;
    if (enable)
    {
        unsigned int index = *reinterpret_cast<unsigned int*>(object + 0x4);
        unsigned int* flags = *reinterpret_cast<unsigned int**>(static_cast<unsigned char*>(resource) + 0x140);
        flags[index] |= 0x4;
    }
    return *reinterpret_cast<void**>(object + 0x10);
}

void GetResMatChan__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFb(){}

void* GetResGenMode__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFb(void* self, bool enable) {
    struct Model {
        unsigned char pad[0x140];
        unsigned* flags;
    };
    struct Access {
        Model* model;
        unsigned index;
        unsigned char pad[0x10];
        void* resGenMode;
    };
    Access* access = static_cast<Access*>(self);
    if (access->model == 0 || access->resGenMode == 0)
        return 0;
    if (enable)
        access->model->flags[access->index] |= 0x10;
    return access->resGenMode;
}

void GetResMatIndMtxAndScaleEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(){}

void GetResMatTexCoordGenEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(){}

void GetResTevEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(){}

void __ct__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFPQ34nw4r3g3d6ScnMdlUl(){}

void SetVisibilityEx__Q44nw4r3g3d6ScnMdl15CopiedVisAccessFb(){}

void __ct__Q44nw4r3g3d6ScnMdl15CopiedVisAccessFPQ34nw4r3g3d6ScnMdlUl(){}

void Construct__Q34nw4r3g3d6ScnMdlFP12MEMAllocatorPUlQ34nw4r3g3d6ResMdlUli(){}

void ScnMdl_G3DPROC_CALC_MAT__Q34nw4r3g3d6ScnMdlFUlPv(){}

void G3dProc__Q34nw4r3g3d6ScnMdlFUlUlPv(){}

void SetScnObjOption__Q34nw4r3g3d6ScnMdlFUlUl(){}

void GetScnObjOption__Q34nw4r3g3d6ScnMdlCFUlPUl(){}

void InitBuffer__Q34nw4r3g3d6ScnMdlFv(){}

void CleanMatBuffer__Q34nw4r3g3d6ScnMdlFUlUl(){}

void SetAnmObj__Q34nw4r3g3d6ScnMdlFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

void RemoveAnmObj__Q34nw4r3g3d6ScnMdlFPQ34nw4r3g3d6AnmObj(){}

void RemoveAnmObj__Q34nw4r3g3d6ScnMdlFQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

void GetAnmObj__Q34nw4r3g3d6ScnMdlFQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

void GetAnmObj__Q34nw4r3g3d6ScnMdlCFQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

void __dt__Q34nw4r3g3d6ScnMdlFv(){}

void IsDerivedFrom__Q34nw4r3g3d6ScnMdlCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d6ScnMdlCFv(){}

void* GetTypeObj__Q34nw4r3g3d6ScnMdlCFv(void){ return 0; }
