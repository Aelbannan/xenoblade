// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnmdlexpand
// Replace stubs with high-level C/C++ during decomp.

#include <nw4r/g3d.h>
#include <nw4r/g3d/g3d_scnmdlexpand.hpp>

#include <algorithm>
#include <cstddef>

// lbl_* are the ONLY symbols allowed with extern "C".

// Retail data imports (defined in nw4r_data.o; the shared header does not
// declare them, so they live here).
extern char lbl_eu_805697E8; // ScnMdlExpand vtable
extern u32 lbl_eu_806634A8;  // invalid node-ID marker (0xFFFFFFFF)

// The shared g3d_scnmdl.h only declares the ScnMdlExpand class shell (no tail
// fields, no Insert/Remove overrides), so the retail layout is expressed here:
// ScnMdlExpand = ScnGroup + mpScnMdl (owner model) + mpNodeIDArray (node ID
// per child, kept in lockstep with the ScnGroup child array).
namespace nw4r {
namespace g3d {

class ScnMdlExpandLayout : public ScnGroup {
public:
    ScnMdlSimple* mpScnMdl; // at 0xE8 - owner model
    u32* mpNodeIDArray;     // at 0xEC - node ID per child
};

// vtable slot at offset 0 (destructor only).
class ScnMdlExpandVtableSlot {
public:
    u32 field_0x0; // vtable pointer
};

// Target 1: destructor
// Detaches this expand object from its owner model, then destroys the
// ScnGroup base.
ScnMdlExpand* __dt__Q34nw4r3g3d12ScnMdlExpandFv(ScnMdlExpand* self) {
    if (self != NULL) {
        ScnMdlExpandVtableSlot* vt = reinterpret_cast<ScnMdlExpandVtableSlot*>(self);
        ScnMdlExpandLayout* lay = reinterpret_cast<ScnMdlExpandLayout*>(self);
        ScnMdlSimple* mdl = lay->mpScnMdl;
        vt->field_0x0 = reinterpret_cast<u32>(&lbl_eu_805697E8);
        if (mdl != NULL) {
            mdl->G3dProc(G3dObj::G3DPROC_DETACH_PARENT, 0, self);
        }
        static_cast<ScnGroup*>(self)->~ScnGroup();
    }
    return self;
}

// Target 5: Insert
// Inserts pObj into the ScnGroup child array and shifts the node-ID array to
// keep the two arrays in lockstep; the new slot gets the invalid node-ID
// marker (lbl_eu_806634A8 = 0xFFFFFFFF).
bool Insert__Q34nw4r3g3d12ScnMdlExpandFUlPQ34nw4r3g3d6ScnObj(ScnMdlExpand* self, u32 idx,
                                                            ScnObj* pObj) {
    ScnMdlExpandLayout* lay = reinterpret_cast<ScnMdlExpandLayout*>(self);
    bool success = static_cast<ScnGroup*>(self)->ScnGroup::Insert(idx, pObj);
    if (success) {
        u32 num = lay->Size();
        for (u32 i = num - 1; i > idx; i--) {
            lay->mpNodeIDArray[i] = lay->mpNodeIDArray[i - 1];
        }
        lay->mpNodeIDArray[idx] = lbl_eu_806634A8; // invalid node ID
    }
    return success;
}

// Target 4: Remove(u32)
// Removes the ScnObj at idx (ScnGroup::Remove) and shifts the node-ID array
// left so it stays in lockstep with the child array.
ScnObj* Remove__Q34nw4r3g3d12ScnMdlExpandFUl(ScnMdlExpand* self, u32 idx) {
    ScnMdlExpandLayout* lay = reinterpret_cast<ScnMdlExpandLayout*>(self);
    ScnObj* pObj = static_cast<ScnGroup*>(self)->ScnGroup::Remove(idx);
    if (pObj != NULL && idx < lay->Size()) {
        u32 num = lay->Size();
        for (u32 i = idx; i < num; i++) {
            lay->mpNodeIDArray[i] = lay->mpNodeIDArray[i + 1];
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

    if (ppObj != lay->End()) {
        return lay->Remove(std::distance(lay->Begin(), ppObj)) != NULL;
    }

    return false;
}

// Target 3: PushBack
// Appends pObj (virtual Insert) and records its model node ID. If the node ID
// is out of range the push is rolled back.
bool PushBack__Q34nw4r3g3d12ScnMdlExpandFPQ34nw4r3g3d6ScnObjUl(ScnMdlExpand* self, ScnObj* pObj,
                                                              u32 nodeID) {
    ScnMdlExpandLayout* lay = reinterpret_cast<ScnMdlExpandLayout*>(self);
    if (!lay->Insert(lay->Size(), pObj)) {
        return false;
    }

    ResMdl mdl = lay->mpScnMdl->GetResMdl();
    u32 idx = lay->Size() - 1;
    u32 numNode = mdl.GetResNodeNumEntries();

    bool success = false;
    if (nodeID < numNode && idx < lay->Size()) {
        lay->mpNodeIDArray[idx] = nodeID;
        success = true;
    }

    if (!success) {
        if (!lay->Empty()) {
            lay->Remove(lay->Size() - 1);
        }
        return false;
    }

    return true;
}

// Remaining stubs
void Construct__Q34nw4r3g3d12ScnMdlExpandFP12MEMAllocatorPUlUlPQ34nw4r3g3d12ScnMdlSimple() {}
void G3dProc__Q34nw4r3g3d12ScnMdlExpandFUlUlPv() {}

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
