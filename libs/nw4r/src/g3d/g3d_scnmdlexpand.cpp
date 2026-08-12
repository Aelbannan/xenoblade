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
extern "C" void __dt__Q34nw4r3g3d8ScnGroupFv(void* self, int freeFlag);

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
