// Decompiled nw4r g3d ScnMdlExpand.
//
// ScnMdlExpand is a ScnGroup that owns a single ScnMdlSimple child
// (mpScnMdl) and keeps a parallel array of model node IDs (mpNodeIDArray),
// one entry per child, in lockstep with the ScnGroup child array.

#include <nw4r/g3d/g3d_draw.h>
#include <nw4r/g3d/g3d_obj.h>
#include <nw4r/g3d/g3d_rtti.h>
#include <nw4r/g3d/g3d_scnmdlsmpl.h>
#include <nw4r/g3d/g3d_scnobj.h>
#include <nw4r/ut/ut_algorithm.h>
#include <nw4r/g3d/g3d_scnmdlexpand.hpp>

#include <algorithm>
#include <cstddef>

namespace nw4r {
namespace g3d {

namespace {

u32 INVALID_NODE_ID[2] = {0xFFFFFFFF, 0x00000000};

} // namespace

ScnMdlExpand* ScnMdlExpand::Construct(MEMAllocator* pAllocator, u32* pSize,
                                      u32 numChildren,
                                      ScnMdlSimple* pScnMdl) {
    u32 scnObjBytes = numChildren * sizeof(ScnObj*);
    u32 ofsNodeID = ut::RoundUp<u32>(scnObjBytes + sizeof(ScnMdlExpand), 4);
    u32 size = ut::RoundUp<u32>(ofsNodeID + scnObjBytes, 4);

    ScnMdlExpand* pObj = NULL;

    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator != NULL) {
        pObj = static_cast<ScnMdlExpand*>(
            detail::AllocFromAllocator(pAllocator, size));

        if (pObj == NULL) {
            return NULL;
        }

        u8* pBytes = static_cast<u8*>(static_cast<void*>(pObj));
        new (pObj) ScnMdlExpand(
            pAllocator,
            static_cast<ScnObj**>(
                static_cast<void*>(pBytes + sizeof(ScnMdlExpand))),
            numChildren, pScnMdl);

        pObj->mpNodeIDArray =
            static_cast<u32*>(static_cast<void*>(pBytes + ofsNodeID));

        for (u32 i = 0; i < numChildren; i++) {
            pObj->mpNodeIDArray[i] = INVALID_NODE_ID[0];
        }

        pScnMdl->G3dProc(G3dObj::G3DPROC_ATTACH_PARENT, 0, pObj);
    }

    return pObj;
}

bool ScnMdlExpand::PushBack(ScnObj* pObj, u32 nodeID) {
    if (!Insert(Size(), pObj)) {
        return false;
    }

    u32 idx = Size() - 1;
    ResMdl mdl = mpScnMdl->GetResMdl();
    u32 numNode = mdl.GetResNodeNumEntries();

    bool success;
    if (nodeID < numNode && idx < Size()) {
        success = true;
        mpNodeIDArray[idx] = nodeID;
    } else {
        success = false;
    }

    if (!success) {
        if (!Empty()) {
            Remove(Size() - 1);
        }
        return false;
    }

    return true;
}

bool ScnMdlExpand::Insert(u32 idx, ScnObj* pObj) {
    u32 invalidNodeID = INVALID_NODE_ID[0];
    bool inserted = ScnGroup::Insert(idx, pObj);
    if (inserted) {
        for (u32 i = Size() - 1; i > idx; i--) {
            mpNodeIDArray[i] = mpNodeIDArray[i - 1];
        }
        mpNodeIDArray[idx] = invalidNodeID;
    }
    return inserted;
}

ScnObj* ScnMdlExpand::Remove(u32 idx) {
    ScnObj* pObj = ScnGroup::Remove(idx);
    if (pObj != NULL) {
        u32 num = Size();
        for (u32 i = idx; i < num; i++) {
            mpNodeIDArray[i] = mpNodeIDArray[i + 1];
        }
    }
    return pObj;
}

bool ScnMdlExpand::Remove(ScnObj* pObj) {
    ScnObj** ppObj = std::find(Begin(), End(), pObj);

    if (ppObj == End()) {
        return false;
    }

    return Remove(static_cast<u32>(std::distance(Begin(), ppObj))) != NULL;
}

ScnMdlExpand::~ScnMdlExpand() {
    if (mpScnMdl != NULL) {
        mpScnMdl->G3dProc(0x10003, 0, this);
    }
}

void ScnMdlExpand::G3dProc(u32 task, u32 param, void* pInfo) {
    if (IsG3dProcDisabled(task)) {
        return;
    }

    switch (task) {
    case G3dObj::G3DPROC_CALC_WORLD: {
        CheckCallback_CALC_WORLD(ScnObj::CALLBACK_TIMING_A, param, pInfo);

        CalcWorldMtx(static_cast<const math::MTX34*>(pInfo), &param);

        CheckCallback_CALC_WORLD(ScnObj::CALLBACK_TIMING_B, param, pInfo);

        if (mpScnMdl != NULL) {
            mpScnMdl->G3dProc(
                task, param,
                const_cast<math::MTX34*>(GetMtxPtr(ScnObj::MTX_WORLD)));
        }

        u32 numChildren = Size();
        for (u32 i = 0; i < numChildren; i++) {
            u32 nodeID = mpNodeIDArray[i];
            u32 childParam = param;

            if (nodeID != INVALID_NODE_ID[0]) {
                ScnObj* pChild = (*this)[i];
                math::MTX34 mtx;

                if (mpScnMdl != NULL) {
                    if (!mpScnMdl->GetScnMtxPos(&mtx, ScnObj::MTX_WORLD,
                                                nodeID)) {
                        childParam |= 1;
                    }
                } else {
                    childParam |= 1;
                }

                pChild->G3dProc(task, childParam, &mtx);
            }
        }

        CheckCallback_CALC_WORLD(ScnObj::CALLBACK_TIMING_C, param, pInfo);
        break;
    }

    case G3dObj::G3DPROC_ATTACH_PARENT:
    case G3dObj::G3DPROC_DETACH_PARENT:
        DefG3dProcScnGroup(task, param, pInfo);
        break;

    case G3dObj::G3DPROC_CHILD_DETACHED:
        if (pInfo == mpScnMdl) {
            mpScnMdl->G3dProc(G3dObj::G3DPROC_DETACH_PARENT, 0, this);
            mpScnMdl = NULL;
        } else {
            DefG3dProcScnGroup(task, param, pInfo);
        }
        break;

    default:
        if (mpScnMdl != NULL) {
            mpScnMdl->G3dProc(task, param, pInfo);
        }
        DefG3dProcScnGroup(task, param, pInfo);
        break;
    }
}

NW4R_G3D_RTTI_DEF(ScnMdlExpand);

bool ScnMdlExpand::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(TYPE_NAME) ? true
         : other == ScnGroup::GetTypeObjStatic() ? true
         : other == ScnObj::GetTypeObjStatic() ? true
         : (other == G3dObj::GetTypeObjStatic());
}

const G3dObj::TypeObj ScnMdlExpand::GetTypeObj() const {
    return TypeObj(TYPE_NAME);
}

const char* ScnMdlExpand::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}

} // namespace g3d
} // namespace nw4r
