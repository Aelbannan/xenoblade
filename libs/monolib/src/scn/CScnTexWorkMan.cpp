// Decompiled for monolib/src/scn/CScnTexWorkMan
//
// Texture-work manager: owns a texture-work memory pool (MemManager region),
// the model's per-node world matrices, and walks the model's node tree to
// accumulate the world-space bounds of the node positions (recursive).

#include <harness_catalog.h>
#include <nw4r/db/db_assert.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <revolution/gx.h>
#include "monolib/util.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

// Assert strings referenced by this unit's panic sites (retail .data symbols).
// Sized .sdata strings are declared non-const so MWCC addresses them via r13.
extern char lbl_eu_8056E850[];    // assert source file (line 0x2c asserts)
extern char lbl_eu_8056E834[];    // assert format ("null pointer" family)
extern char lbl_eu_80663910[8];   // assert va arg 1 (sda string)
extern char lbl_eu_806639B8[4];   // assert va arg 2 (sda string)
extern char lbl_eu_8056E820[];    // assert source file (alignment asserts)
extern char lbl_eu_8056E7F8[];    // assert format ("not aligned" family)
extern char lbl_eu_8056E194[];    // assert source file (node-null asserts, line 0x2c)
extern char lbl_eu_8056E178[];    // assert format ("null pointer" family)
extern char lbl_eu_806639BC[4];   // assert va arg 2 (sda string)
extern char lbl_eu_8056E1C8[];    // assert source file (node-id asserts, line 0x38)
extern char lbl_eu_8056E1A8[];    // assert format
extern char lbl_eu_80529678[];    // assert source file (line 0x53 asserts)
extern char lbl_eu_80529658[];    // assert format

// Texture-work object cursor word at +0x28 (read by TexMan_ReleaseArg_02D8).
struct CScnTexWorkManCursor {
    u8 field_0x00[0x28];
    u32 field_0x28; // allocation cursor snapshot
};

// Node-id collection list used by the node-tree walk (array at 0x0, count
// at 0x4).
struct CScnTexWorkManNodeIdList {
    u32* array;  // 0x0
    u32 count;   // 0x4
};

// The retail game's nw4r build keeps the g3d_resnode_ac.h accessor asserts.
// name-guess: unit-local mirrors of the asserting ResNode accessors.
static inline const nw4r::g3d::ResNodeData& ResNodeRefChk(const nw4r::g3d::ResNode& node) {
    if (!node.IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    return *node.ptr();
}

// name-guess: asserting ResNode::ref() mirror (g3d_resnode_ac.h).
static inline const nw4r::g3d::ResNodeData& ResNodeRef(nw4r::g3d::ResNode* node) {
    if (!node->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                        lbl_eu_80663910, lbl_eu_806639B8);
    }
    return *node->ptr();
}

// name-guess: asserting ResCommon::ofs_to_obj<ResNode> mirror.
static inline nw4r::g3d::ResNode ResNodeOfsToObj(const nw4r::g3d::ResNodeData& data, s32 ofs) {
    if (ofs != 0) {
        u8* p = (u8*)&data + ofs;
        if (((u32)p & 3) != 0) {
            nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        }
        return nw4r::g3d::ResNode(p);
    }
    return nw4r::g3d::ResNode(NULL);
}

extern "C" u32 lbl_eu_8056E7D0[];
class __declspec(novtable) CScnTexWorkMan : public CDeviceVICb {
public:
    CScnTexWorkMan(u32 param1, u32 param2, u32 param3);
    virtual ~CScnTexWorkMan();

    /* 0x04 */ u32 field_0x4;
    u8 field_0x8[0xec - 0x8];
    /* 0xec */ ml::CMat34* mEntries; // per-node world matrices (indexed by mtxID)
    u8 field_0xf0[0x146c - 0xf0];
    /* 0x146c */ nw4r::g3d::ResMdlData* mResMdl1;
    u8 field_0x1470[0x147c - 0x1470];
    /* 0x147c */ CScnTexWorkMan* field_0x147c;
};

// Cross-TU / self-recursive helpers with retail unmangled reloc names.
extern "C" void TexMan_ClampBoxes(CScnTexWorkMan* self,
                                  nw4r::g3d::ResNode* pNode,
                                  ml::CVec3* pMin, ml::CVec3* pMax);
extern "C" nw4r::g3d::ResNode TexMan_NodeFromNode_090C(nw4r::g3d::ResNode* node);
extern "C" nw4r::g3d::ResNode TexMan_NodeFromMdl_09A8(nw4r::g3d::ResNode* node);
extern "C" void* TexObj_InitExt(u8* obj, u8* image, u16 w, u16 h, u32 fmt,
                               u8* p6);

// Texture-work memory-pool state (retail .sdata/.sbss).
extern u32 lbl_eu_806639A8;  // memory handle (owned by auto_09 .sdata split)
extern u32 sTexWorkMemPoolSize;  // memory-pool size (owned by auto_09 .sdata split)
s32 sTexWorkInstanceCount = 0;           // live-instance count (retail bgt: signed)
u32 sTexWorkRegionBase = 0;           // allocatable-region base
u32 sTexWorkAllocCursor = 0;           // allocation cursor
u32 sTexWorkMaxAllocSize = 0;           // max allocatable size
u32 lbl_eu_806658FC = 0;           // busy/reference counter
u32 sTexWorkLastObject = 0;           // last allocated texture-work object

CScnTexWorkMan::CScnTexWorkMan(u32 param1, u32 param2, u32 param3) {
    this->field_0x4 = param1;
    sTexWorkInstanceCount++;
    if (lbl_eu_806639A8 == mtl::INVALID_HANDLE) {
        lbl_eu_806639A8 = mtl::MemManager::create(param2, param3, "CScnTexWorkMan");
        sTexWorkMemPoolSize = param2;
        sTexWorkMaxAllocSize = mtl::MemManager::getMaxAllocSize(lbl_eu_806639A8);
        sTexWorkRegionBase = (u32)mtl::MemManager::getMaxAllocData(lbl_eu_806639A8);
        sTexWorkAllocCursor = sTexWorkRegionBase;
        CDeviceVI::entryCb(this);
    }
}

CScnTexWorkMan::~CScnTexWorkMan() {
    *(void**)this = (void*)lbl_eu_8056E7D0;
    sTexWorkInstanceCount--;
    if (sTexWorkInstanceCount <= 0) {
        mtl::MemManager::erase(lbl_eu_806639A8);
        lbl_eu_806639A8 = mtl::INVALID_HANDLE;
        CDeviceVI::removeCb(this);
    }
}

// Resolve a ResMdl's node dictionary (offset field at 0x60) to a ResNode,
// with the nw4r null/alignment asserts.
extern "C" nw4r::g3d::ResNode TexMan_NodeFromMdl_09A8(nw4r::g3d::ResNode* node) {
    const nw4r::g3d::ResNodeData& data = ResNodeRef(node);
    return ResNodeOfsToObj(data, data.toChildNode);
}

// Initialize the position/scale defaults and clamp them against the model's
// node boxes.
extern "C" int TexMan_GetDefaultXf_0A44(CScnTexWorkMan* self, ml::CVec3* pos,
                             ml::CVec3* scale) {
    nw4r::g3d::ResMdl mdl = nw4r::g3d::ResMdl(self->mResMdl1);
    if (mdl.GetResNode(0).IsValid() == false) {
        return 0;
    }
    pos->x = 1000000.0f;
    pos->y = 1000000.0f;
    pos->z = 1000000.0f;
    scale->x = (-1000000.0f);
    scale->y = (-1000000.0f);
    scale->z = (-1000000.0f);
    CScnTexWorkMan* work = self->field_0x147c;
    nw4r::g3d::ResNode node = mdl.GetResNode(0);
    TexMan_ClampBoxes(work, &node, pos, scale);
    return 1;
}

// Reserve texture-work memory from the pool and build a texture-work object.
extern "C" void* TexMan_AllocBuffer_0208(void* p, u16 w, u16 h, u32 fmt) {
    if (lbl_eu_806658FC != 0) {
        return 0;
    }
    u32 size = GXGetTexBufferSize(w, h, fmt, 0, 0);
    u32 cursor = sTexWorkAllocCursor;
    u32 mask = cursor & 0x1F;
    u32 aligned = mask != 0 ? cursor + 0x20 - mask : cursor;
    u32 newCursor = aligned + size;
    sTexWorkAllocCursor = newCursor;
    if (newCursor > sTexWorkRegionBase + sTexWorkMaxAllocSize) {
        sTexWorkAllocCursor = cursor;
        return 0;
    }
    u8* obj = (u8*)mtl::MemManager::allocate(
        0x2c, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // TexObj_InitExt returns its first argument, so r3 keeps the object
        // across the call in both paths.
        obj = (u8*)TexObj_InitExt(obj, (u8*)aligned, w, h, fmt, (u8*)cursor);
    }
    sTexWorkLastObject = (u32)obj;
    return obj;
}

// name-guess: asserting ResNode::GetParentNode() mirror.
static inline nw4r::g3d::ResNode ResNodeGetParent(nw4r::g3d::ResNode* node) {
    const nw4r::g3d::ResNodeData& data = ResNodeRef(node);
    return ResNodeOfsToObj(data, data.toParentNode);
}

// name-guess: the node's world matrix (entry table indexed by the node's mtxID).
static inline ml::CMat34& NodeWorldMtx(CScnTexWorkMan* self, const nw4r::g3d::ResNode& node) {
    if (!node.IsValid()) {
        nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
    }
    return self->mEntries[node.GetMtxID()];
}

// Walk the node tree from pNode, growing the [pMin, pMax] box by the world
// translation of every node that has a parent (and of that parent).
extern "C" void TexMan_ClampBoxes(CScnTexWorkMan* self, nw4r::g3d::ResNode* pNode,
                                  ml::CVec3* pMin, ml::CVec3* pMax) {
    if (pNode->IsValid() && ResNodeGetParent(pNode).IsValid()) {
        nw4r::g3d::ResNode parent = ResNodeGetParent(pNode);
        ml::CMat34& parentMtx = NodeWorldMtx(self, parent);
        ml::CMat34& nodeMtx = NodeWorldMtx(self, *pNode);
        if (pMin->x > parentMtx.getTranslation().x) pMin->x = parentMtx.getTranslation().x;
        if (pMin->y > parentMtx.getTranslation().y) pMin->y = parentMtx.getTranslation().y;
        if (pMin->z > parentMtx.getTranslation().z) pMin->z = parentMtx.getTranslation().z;
        if (pMin->x > nodeMtx.getTranslation().x) pMin->x = nodeMtx.getTranslation().x;
        if (pMin->y > nodeMtx.getTranslation().y) pMin->y = nodeMtx.getTranslation().y;
        if (pMin->z > nodeMtx.getTranslation().z) pMin->z = nodeMtx.getTranslation().z;
        if (pMax->x < parentMtx.getTranslation().x) pMax->x = parentMtx.getTranslation().x;
        if (pMax->y < parentMtx.getTranslation().y) pMax->y = parentMtx.getTranslation().y;
        if (pMax->z < parentMtx.getTranslation().z) pMax->z = parentMtx.getTranslation().z;
        if (pMax->x < nodeMtx.getTranslation().x) pMax->x = nodeMtx.getTranslation().x;
        if (pMax->y < nodeMtx.getTranslation().y) pMax->y = nodeMtx.getTranslation().y;
        if (pMax->z < nodeMtx.getTranslation().z) pMax->z = nodeMtx.getTranslation().z;
    }
    if (pNode->IsValid()) {
        nw4r::g3d::ResNode sib = TexMan_NodeFromNode_090C(pNode);
        TexMan_ClampBoxes(self, &sib, pMin, pMax);
        nw4r::g3d::ResNode child = TexMan_NodeFromMdl_09A8(pNode);
        TexMan_ClampBoxes(self, &child, pMin, pMax);
    }
}

// ---- scaffold stubs (unmatched functions, kept as-is) ----

extern "C" void __dt__804943A0(void* self, int flags);

// Release path: while the busy counter is clear, if the arg matches the last
// allocated work object, publish its +0x28 word into the allocation cursor;
// then delete the arg when non-null (retail tail-calls __dt__804943A0). The
// arg is the SECOND parameter (retail compares r4; r3 self is unused).
extern "C" void TexMan_ReleaseArg_02D8(void* self, void* arg) {
    if (lbl_eu_806658FC != 0) return;
    if (sTexWorkLastObject == (u32)arg) {
        sTexWorkAllocCursor = ((CScnTexWorkManCursor*)arg)->field_0x28;
    }
    if (arg != 0) {
        __dt__804943A0(arg, 1);
    }
}

extern "C" void onViAfterDrawDone__14CScnTexWorkManFv(void) {}

int sTexWorkSavedCursor;

extern "C" void TexMan_SnapCursor_0314() {
    if (lbl_eu_806658FC == 0) {
        sTexWorkSavedCursor = sTexWorkAllocCursor;
    }
}

extern "C" void TexMan_RestoreCursor_032C() {
    if (lbl_eu_806658FC != 0) return;
    sTexWorkAllocCursor = sTexWorkSavedCursor;
    sTexWorkSavedCursor = 0;
}

// Camera resolver in CScn.cpp (retail also receives the view in r4).
extern "C" void* Scn_GetCamList(void* scene, void* view);

// Apply the camera's perspective (matrix at cam+0x9c, fov/near/far at
// cam+0x1e0/0x1ec/0x1f0) to the given CDrawGX; falls back to the current view.
// Camera perspective payload read by TexMan_ApplyCamPersp_034C: view matrix
// at +0x9c, near/mid/far planes at +0x1e0/+0x1ec/+0x1f0.
struct CScnTexWorkManCamPersp {
    u8 field_0x00[0x9c];
    ml::CMat34 field_0x9c;
    u8 field_0xcc[0x1e0 - 0xcc];
    f32 field_0x1e0; // near
    u8 field_0x1e4[0x1ec - 0x1e4];
    f32 field_0x1ec; // mid
    f32 field_0x1f0; // far
};

extern "C" int TexMan_ApplyCamPersp_034C(CScnTexWorkMan* scene, CDrawGX* gx, CView* view) {
    if (view == NULL) {
        view = CView::getCurrentView();
    }
    const CScnTexWorkManCamPersp* cam =
        (const CScnTexWorkManCamPersp*)Scn_GetCamList(scene, view);
    // Sequenced to match retail's load order (far, mat addr, mid, near).
    const f32 valFar = cam->field_0x1f0;
    const ml::CMat34& mat = cam->field_0x9c;
    const f32 valMid = cam->field_0x1ec;
    const f32 valNear = cam->field_0x1e0;
    gx->setPerspective(mat, valNear, valMid, valFar);
    return 1;
}

extern "C" nw4r::g3d::ResNode TexMan_FindNode_0AF4(void* self, const char* name) {
    nw4r::g3d::ResMdl mdl = nw4r::g3d::ResMdl(((CScnTexWorkMan*)self)->mResMdl1);
    return mdl.GetResNode(name);
}

// Resolve a ResNode's next sibling (offset field at 0x64) with the nw4r
// null/alignment asserts.
extern "C" nw4r::g3d::ResNode TexMan_NodeFromNode_090C(nw4r::g3d::ResNode* node) {
    const nw4r::g3d::ResNodeData& data = ResNodeRef(node);
    return ResNodeOfsToObj(data, data.toNextSibling);
}

// Count every node with a non-degenerate bounding volume in the subtree.
extern "C" void TexMan_CountSubtreeNodes(nw4r::g3d::ResNode node, int* pCounter) {
    if (node.IsValid()) {
        if (0.0f != ResNodeRefChk(node).volume_min.x || 0.0f != ResNodeRefChk(node).volume_min.y ||
            0.0f != ResNodeRefChk(node).volume_min.z || 0.0f != ResNodeRefChk(node).volume_max.x ||
            0.0f != ResNodeRefChk(node).volume_max.y || 0.0f != ResNodeRefChk(node).volume_max.z) {
            (*pCounter)++;
        }
        TexMan_CountSubtreeNodes(TexMan_NodeFromNode_090C(&node), pCounter);
        TexMan_CountSubtreeNodes(TexMan_NodeFromMdl_09A8(&node), pCounter);
    }
}

extern "C" int TexMan_CountRootNodes(nw4r::g3d::ResNode* pNode) {
    int counter = 0;
    TexMan_CountSubtreeNodes(*pNode, &counter);
    return counter;
}

extern "C" void TexMan_CollectNodeIds(nw4r::g3d::ResNode node, CScnTexWorkManNodeIdList* pList) {
    if (node.IsValid()) {
        if (0.0f != ResNodeRefChk(node).volume_min.x || 0.0f != ResNodeRefChk(node).volume_min.y ||
            0.0f != ResNodeRefChk(node).volume_min.z || 0.0f != ResNodeRefChk(node).volume_max.x ||
            0.0f != ResNodeRefChk(node).volume_max.y || 0.0f != ResNodeRefChk(node).volume_max.z) {
            if (!node.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
            }
            pList->array[pList->count++] = node.GetID();
        }
        TexMan_CollectNodeIds(TexMan_NodeFromNode_090C(&node), pList);
        TexMan_CollectNodeIds(TexMan_NodeFromMdl_09A8(&node), pList);
    }
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace TWMBlob {
extern "C" void __dt__14CScnTexWorkManFv();
// onViAfterDrawDone__14CScnTexWorkManFv is defined (extern "C") earlier
// in this TU; the unqualified use below resolves to it.
extern "C" void viBeforeDrawDone__11CDeviceVICbFv();
extern "C" void viBeginFrame__11CDeviceVICbFv();
}
extern "C" u32 __RTTI__14CScnTexWorkMan; extern "C" u32 __RTTI__11CDeviceVICb;

// lbl_eu_8056E7D0 (24B) vtable/ptrs
extern "C" u32 lbl_eu_8056E7D0[6] = {
(u32)&__RTTI__14CScnTexWorkMan, 0x00000000, (u32)&TWMBlob::__dt__14CScnTexWorkManFv, (u32)&TWMBlob::viBeforeDrawDone__11CDeviceVICbFv, (u32)&onViAfterDrawDone__14CScnTexWorkManFv, (u32)&TWMBlob::viBeginFrame__11CDeviceVICbFv,
};
// lbl_eu_8056E7E8 (16B) vtable/ptrs
extern "C" u32 lbl_eu_8056E7E8[4] = {
    (u32)&__RTTI__11CDeviceVICb, 0x00000000, 0x00000000, 0x00000000,
};
// lbl_eu_8056E7F8 (38B) string/bytes
extern "C" char lbl_eu_8056E7F8[40] = { 0x4E, 0x57, 0x34, 0x52, 0x3A, 0x46, 0x61, 0x69, 0x6C, 0x65, 0x64, 0x20, 0x61, 0x73, 0x73, 0x65, 0x72, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x21, 0x28, 0x28, 0x75, 0x33, 0x32, 0x29, 0x70, 0x20, 0x26, 0x20, 0x30, 0x78, 0x33, 0x29, 0x00, 0x00, 0x00, };
// lbl_eu_8056E820 (17B) string/bytes
extern "C" char lbl_eu_8056E820[20] = { 0x67, 0x33, 0x64, 0x5F, 0x72, 0x65, 0x73, 0x6E, 0x6F, 0x64, 0x65, 0x5F, 0x61, 0x63, 0x2E, 0x68, 0x00, 0x00, 0x00, 0x00, };
// lbl_eu_8056E834 (26B) string/bytes
extern "C" char lbl_eu_8056E834[28] = { 0x25, 0x73, 0x3A, 0x3A, 0x25, 0x73, 0x3A, 0x20, 0x4F, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x20, 0x6E, 0x6F, 0x74, 0x20, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x2E, 0x00, 0x00, 0x00, };
// lbl_eu_8056E850 (17B) string/bytes
extern "C" char lbl_eu_8056E850[24] = { 0x67, 0x33, 0x64, 0x5F, 0x72, 0x65, 0x73, 0x6E, 0x6F, 0x64, 0x65, 0x5F, 0x61, 0x63, 0x2E, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

// [.rodata] 0x80524010-0x80524030 (32B)
extern "C" __declspec(align(4)) const char lbl_eu_80524010[0x10] = {
    0x43,0x53,0x63,0x6E,0x54,0x65,0x78,0x57,0x6F,0x72,0x6B,0x4D,0x61,0x6E,0x00,0x00,
};

// [.sbss] 0x806658E8-0x80665910 (40B) zero-fill. The live-count / region
// state labels (806658E8..80665900) are defined above; add the missing retail
// slots to reach the 40B block.
u32 lbl_eu_80665904;
u32 lbl_eu_80665908[2];
// (retired DECOMP_FORCEACTIVE keep-alives: 80524020 is referenced by
// MemManager::create above; 80524010 is anchored by CScnItemCamera.cpp's
// sd table -- section-defined globals survive linking without text refs.)
