#pragma once

#include <types.h>
#include <nw4r/g3d/g3d_obj.h>
#include <nw4r/g3d/g3d_scnmdlsmpl.h>
#include <nw4r/g3d/g3d_scnobj.h>

// C-linkage import retained from the original private header.

namespace nw4r {
namespace g3d {

class ScnMdlExpand : public ScnGroup {
public:
    static ScnMdlExpand* Construct(MEMAllocator* pAllocator, u32* pSize,
                                   u32 numChildren, ScnMdlSimple* pScnMdl);

    ScnMdlExpand(MEMAllocator* pAllocator, ScnObj** ppObj, u32 capacity,
                 ScnMdlSimple* pScnMdl)
        : ScnGroup(pAllocator, ppObj, capacity), mpScnMdl(pScnMdl) {}

    virtual ~ScnMdlExpand();

    virtual void G3dProc(u32 task, u32 param, void* pInfo);
    virtual bool Insert(u32 idx, ScnObj* pObj);
    virtual ScnObj* Remove(u32 idx);
    virtual bool Remove(ScnObj* pObj);

    bool PushBack(ScnObj* pObj, u32 nodeID);

    NW4R_G3D_RTTI_DECL_DERIVED(ScnMdlExpand, ScnGroup);

private:
    ScnMdlSimple* mpScnMdl;
    u32* mpNodeIDArray;
};

} // namespace g3d
} // namespace nw4r
