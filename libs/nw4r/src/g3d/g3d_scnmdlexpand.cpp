// Decompiled nw4r g3d ScnMdlExpand.
//
// ScnMdlExpand is a ScnGroup that owns a single ScnMdlSimple child
// (mpScnMdl) and keeps a parallel array of model node IDs (mpNodeIDArray),
// one entry per child, in lockstep with the ScnGroup child array.

#include <nw4r/g3d.h>
#include <nw4r/g3d/g3d_scnmdlexpand.hpp>

#include <algorithm>
#include <cstddef>

// Retail data imports (defined in nw4r_data.o; the shared header does not
// declare them, so they live here).
extern char lbl_eu_805697E8[]; // ScnMdlExpand vtable
extern u32 lbl_eu_806634A8;  // invalid node-ID marker (0xFFFFFFFF)
namespace nw4r {
namespace g3d {

// The shared g3d_scnmdl.h only declares the ScnMdlExpand class shell (no tail
// fields, no Insert/Remove overrides), so the retail layout is expressed here:
// ScnMdlExpand = ScnGroup + mpScnMdl (owner model) + mpNodeIDArray (node ID
// per child, kept in lockstep with the ScnGroup child array).
class ScnMdlExpandLayout : public ScnGroup {
public:
    ScnMdlSimple* mpScnMdl; // at 0xE8 - owner model
    u32* mpNodeIDArray;     // at 0xEC - node ID per child
};

// vtable slot at offset 0 (the ScnMdlExpand vtable data lives in nw4r_data.o;
// the destructor resets this pointer manually because the class shell in the
// shared header has no member declarations here).
class ScnMdlExpandVtblSlot {
public:
    u32 field_0x0; // vtable pointer
};

// Target 1: destructor
// Detaches this expand object from its owner model (vtable reset + G3dProc
// DETACH_PARENT), then the implicit ScnGroup base destruction runs (retail
// li r4,0 before bl __dt__ScnGroup; MWCC's implicit base call uses flag 0).
ScnMdlExpand::~ScnMdlExpand() {
    *(u32*)this = reinterpret_cast<u32>(lbl_eu_805697E8);
    ScnMdlSimple* mdl = *(ScnMdlSimple**)((char*)this + 0xE8);
    if (mdl != NULL) {
        mdl->G3dProc(0x10003, 0, this);
    }
}

// Target 5: Insert
// Inserts pObj into the ScnGroup child array and shifts the node-ID array to
// keep the two arrays in lockstep; the new slot gets the invalid node-ID
// marker (0xFFFFFFFF).
bool Insert__Q34nw4r3g3d12ScnMdlExpandFUlPQ34nw4r3g3d6ScnObj(ScnMdlExpand* self, u32 idx,
                                                            ScnObj* pObj) {
    u32 invalidNodeID = lbl_eu_806634A8; // invalid node-ID marker
    bool inserted = self->ScnGroup::Insert(idx, pObj);
    if (inserted) {
        for (u32 i = reinterpret_cast<ScnMdlExpandLayout*>(self)->Size() - 1; i > idx;
             i--) {
            reinterpret_cast<ScnMdlExpandLayout*>(self)->mpNodeIDArray[i] =
                reinterpret_cast<ScnMdlExpandLayout*>(self)->mpNodeIDArray[i - 1];
        }
        reinterpret_cast<ScnMdlExpandLayout*>(self)->mpNodeIDArray[idx] = invalidNodeID;
    }
    return inserted;
}

// Target 4: Remove(u32)
// Removes the ScnObj at idx (ScnGroup::Remove) and shifts the node-ID array
// left so it stays in lockstep with the child array.
ScnObj* Remove__Q34nw4r3g3d12ScnMdlExpandFUl(ScnMdlExpand* self, u32 idx) {
    ScnObj* pObj = self->ScnGroup::Remove(idx);
    if (pObj != NULL) {
        u32 num = reinterpret_cast<ScnMdlExpandLayout*>(self)->Size();
        for (u32 i = idx; i < num; i++) {
            reinterpret_cast<ScnMdlExpandLayout*>(self)->mpNodeIDArray[i] =
                reinterpret_cast<ScnMdlExpandLayout*>(self)->mpNodeIDArray[i + 1];
        }
    }
    return pObj;
}

// Target 2: Remove(ScnObj*)
// Finds pObj in the child array, then removes it by index (virtual dispatch,
// so ScnMdlExpand::Remove(u32) runs).
bool Remove__Q34nw4r3g3d12ScnMdlExpandFPQ34nw4r3g3d6ScnObj(ScnMdlExpand* self, ScnObj* pObj) {
    ScnMdlExpandLayout* lay = reinterpret_cast<ScnMdlExpandLayout*>(self);
    ScnObj** ppObj = std::find(lay->Begin(), lay->End(), pObj);

    if (ppObj == lay->End()) {
        return false;
    }

    return lay->Remove(std::distance(lay->Begin(), ppObj)) != NULL;
}

// Target 3: PushBack
// Appends pObj (virtual Insert) and records its model node ID. If the node ID
// is out of range the push is rolled back.
bool PushBack__Q34nw4r3g3d12ScnMdlExpandFPQ34nw4r3g3d6ScnObjUl(ScnMdlExpand* self,
                                                              ScnObj* pObj, u32 nodeID) {
    if (!reinterpret_cast<ScnMdlExpandLayout*>(self)->Insert(
            reinterpret_cast<ScnMdlExpandLayout*>(self)->Size(), pObj)) {
        return false;
    }

    u32 idx = reinterpret_cast<ScnMdlExpandLayout*>(self)->Size() - 1;
    ResMdl mdl = reinterpret_cast<ScnMdlExpandLayout*>(self)->mpScnMdl->GetResMdl();
    u32 numNode = mdl.GetResNodeNumEntries();

    bool success;
    if (nodeID < numNode &&
        idx < reinterpret_cast<ScnMdlExpandLayout*>(self)->Size()) {
        success = true;
        reinterpret_cast<ScnMdlExpandLayout*>(self)->mpNodeIDArray[idx] = nodeID;
    } else {
        success = false;
    }

    if (!success) {
        if (!reinterpret_cast<ScnMdlExpandLayout*>(self)->Empty()) {
            reinterpret_cast<ScnMdlExpandLayout*>(self)->Remove(
                reinterpret_cast<ScnMdlExpandLayout*>(self)->Size() - 1);
        }
        return false;
    }

    return true;
}

// Target: ScnMdlExpand::Construct
// Single backing allocation laid out as: object (0xF0), child pointer array,
// node ID array. The vtable is patched manually because only the ScnGroup base
// constructor runs on the raw block.
// Static member ScnMdlExpand::Construct (the retail mangled name is spelled
// literally because the shared header cannot host the declaration).
nw4r::g3d::ScnMdlExpand*
Construct__Q34nw4r3g3d12ScnMdlExpandFP12MEMAllocatorPUlUlPQ34nw4r3g3d12ScnMdlSimple(
    MEMAllocator* pAllocator, u32* pSize, u32 numChildren,
    nw4r::g3d::ScnMdlSimple* pScnMdl) {
    using namespace nw4r::g3d;
    typedef ScnMdlExpandLayout Expand;

    u32 scnObjBytes = numChildren * sizeof(ScnObj*);
    u32 ofsNodeID = ut::RoundUp<u32>(scnObjBytes + sizeof(Expand), 4);
    u32 size = ut::RoundUp<u32>(ofsNodeID + scnObjBytes, 4);

    u32 i;
    Expand* pObj = NULL;

    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator != NULL) {
        pObj = reinterpret_cast<Expand*>(detail::AllocFromAllocator(pAllocator, size));

        if (pObj == NULL) {
            return NULL;
        }

        // Construct the ScnGroup base over the block; the child pointer
        // array sits right behind the object.
        new (pObj) ScnGroup(
            pAllocator,
            reinterpret_cast<ScnObj**>(reinterpret_cast<u8*>(pObj) +
                                       sizeof(Expand)),
            numChildren);

        // Patch in the ScnMdlExpand vtable (base ctor stored ScnGroup's).
        *(u32*)pObj = reinterpret_cast<u32>(lbl_eu_805697E8);
        pObj->mpScnMdl = pScnMdl;
        pObj->mpNodeIDArray = reinterpret_cast<u32*>(reinterpret_cast<u8*>(pObj) + ofsNodeID);

        // All node IDs start as the invalid marker; the global cannot be
        // cached across iterations because the stores may alias it.
        for (i = 0; i < numChildren; i++) {
            pObj->mpNodeIDArray[i] = lbl_eu_806634A8;
        }

        // Attach the freshly made group as a child of its owner model.
        pScnMdl->G3dProc(G3dObj::G3DPROC_ATTACH_PARENT, 0, pObj);
    }

    return reinterpret_cast<ScnMdlExpand*>(pObj);
}
// Access shims: the shared headers keep these helpers protected, so re-expose
// them through zero-size derived hooks (never instantiated).
class ScnObjHook : public ScnObj {
public:
    using ScnObj::IsG3dProcDisabled;
    using ScnObj::CheckCallback_CALC_WORLD;
};

class ScnGroupHook : public ScnGroup {
public:
    using ScnGroup::DefG3dProcScnGroup;
};

// ScnMdlExpand::G3dProc
// Dispatches like ScnGroup::DefG3dProcScnGroup, but the CALC_WORLD pass resolves
// each child's world matrix from the owner model via its recorded node ID
// (children whose node ID is still the invalid marker are skipped entirely).
// CHILD_DETACHED additionally drops the owner-model link when the model itself
// is the object being detached.
// pInfo carries a pass-specific payload: const math::MTX34* for CALC_WORLD,
// the detached child (ScnObj*) otherwise.
void G3dProc__Q34nw4r3g3d12ScnMdlExpandFUlUlPv(ScnMdlExpand* self, u32 task,
                                               u32 param, G3dObj* pInfo) {
    ScnMdlExpandLayout* lay = reinterpret_cast<ScnMdlExpandLayout*>(self);
    ScnObjHook* hook = reinterpret_cast<ScnObjHook*>(self);

    if (hook->IsG3dProcDisabled(task)) {
        return;
    }

    switch (task) {
    case G3dObj::G3DPROC_CALC_WORLD: {
        hook->CheckCallback_CALC_WORLD(ScnObj::CALLBACK_TIMING_A, param, pInfo);

        lay->CalcWorldMtx(reinterpret_cast<const math::MTX34*>(pInfo), &param);

        hook->CheckCallback_CALC_WORLD(ScnObj::CALLBACK_TIMING_B, param, pInfo);

        // Recurse into the owner model with our world matrix.
        if (lay->mpScnMdl != NULL) {
            lay->mpScnMdl->G3dProc(
                task, param,
                const_cast<math::MTX34*>(lay->GetMtxPtr(ScnObj::MTX_WORLD)));
        }

        u32 numChildren = lay->Size();
        for (u32 i = 0; i < numChildren; i++) {
            u32 nodeID = lay->mpNodeIDArray[i];
            u32 childParam = param;

            if (nodeID != lbl_eu_806634A8) {
                ScnObj* pChild = (*lay)[i];
                math::MTX34 mtx;

                if (lay->mpScnMdl != NULL) {
                    // No matrix for this node -> mark the world mtx invalid.
                    if (!lay->mpScnMdl->GetScnMtxPos(&mtx, ScnObj::MTX_WORLD,
                                                     nodeID)) {
                        childParam |= 1;
                    }
                } else {
                    childParam |= 1;
                }

                pChild->G3dProc(task, childParam, &mtx);
            }
        }

        hook->CheckCallback_CALC_WORLD(ScnObj::CALLBACK_TIMING_C, param, pInfo);
        break;
    }

    case G3dObj::G3DPROC_ATTACH_PARENT:
    case G3dObj::G3DPROC_DETACH_PARENT:
        reinterpret_cast<ScnGroupHook*>(self)->DefG3dProcScnGroup(task, param,
                                                                  pInfo);
        break;

    case G3dObj::G3DPROC_CHILD_DETACHED:
        if (pInfo == lay->mpScnMdl) {
            // The owner model is going away: notify and drop the link.
            lay->mpScnMdl->G3dProc(G3dObj::G3DPROC_DETACH_PARENT, 0, self);
            lay->mpScnMdl = NULL;
        } else {
            reinterpret_cast<ScnGroupHook*>(self)->DefG3dProcScnGroup(task,
                                                                      param,
                                                                      pInfo);
        }
        break;

    default:
        if (lay->mpScnMdl != NULL) {
            lay->mpScnMdl->G3dProc(task, param, pInfo);
        }
        reinterpret_cast<ScnGroupHook*>(self)->DefG3dProcScnGroup(task, param,
                                                                  pInfo);
        break;
    }
}

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("ScnMdlExpand")> lbl_eu_8051D7D0 = {sizeof("ScnMdlExpand"), "ScnMdlExpand"};

bool ScnMdlExpand::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D7D0) ? true
         : other == TypeObj(lbl_eu_8051D788) ? true
         : other == TypeObj(lbl_eu_8051D768) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj ScnMdlExpand::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D7D0);
}

const char* ScnMdlExpand::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
} // namespace nw4r
