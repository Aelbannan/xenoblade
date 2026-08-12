// Decompiled for monolib/src/scn/CScnTexWorkMan
//
// Texture-work manager: owns a texture-work memory pool (MemManager region),
// a per-node entry table, and walks the model's node tree clamping a
// position/scale pair into each node's box corners (recursive).

#include <harness_catalog.h>
#include <nw4r/db/db_assert.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <revolution/gx.h>
#include "monolib/util.hpp"
#include "monolib/math/CVec3.hpp"
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
extern char lbl_eu_80524020[];    // memory-region name passed to MemManager::create

// Shared .sdata2 float constants used by this unit (retail pool).
extern const f32 lbl_eu_8066AA00;  // volume sentinel (nodes without volume)
extern const f32 lbl_eu_8066AA04;  // 1.0f
extern const f32 lbl_eu_8066AA08;  // 0.0f

// Game-fork resource data: the nw4r ResMdlData/ResNodeData layouts in this
// game add a node index at 0x10 and node-dictionary offsets at 0x5c/0x60/0x64
// (both resource types share the extension).
struct CScnTexWorkManResData {
    u8 field_0x00[0x10];
    s32 field_0x10;  // node index into the work-man entry table
    u8 field_0x14[0x5c - 0x14];
    s32 field_0x5c;  // node-dictionary offset (entry-table nodes)
    s32 field_0x60;  // node-dictionary offset (recursion 2)
    s32 field_0x64;  // node-dictionary offset (recursion 1)
};

// 0x30-byte per-node entry: one scalar clamp bound per axis.
struct CScnTexWorkManEntry {
    u8 field_0x00[0x0c];
    f32 field_0x0c;  // x-axis bound
    u8 field_0x10[0x1c - 0x10];
    f32 field_0x1c;  // y-axis bound
    u8 field_0x20[0x2c - 0x20];
    f32 field_0x2c;  // z-axis bound
};

// Node-id collection list used by the node-tree walk (array at 0x0, count
// at 0x4).
struct CScnTexWorkManNodeIdList {
    u32* array;  // 0x0
    u32 count;   // 0x4
};

class CScnTexWorkMan : public CDeviceVICb {
public:
    CScnTexWorkMan(u32 param1, u32 param2, u32 param3);
    virtual ~CScnTexWorkMan();

    /* 0x04 */ u32 field_0x4;
    u8 field_0x8[0xec - 0x8];
    /* 0xec */ CScnTexWorkManEntry* mEntries;
    u8 field_0xf0[0x146c - 0xf0];
    /* 0x146c */ nw4r::g3d::ResMdlData* mResMdl1;
    u8 field_0x1470[0x147c - 0x1470];
    /* 0x147c */ CScnTexWorkMan* field_0x147c;
};

// Cross-TU / self-recursive helpers with retail unmangled reloc names.
extern "C" void func_804903B8(CScnTexWorkMan* self,
                              const nw4r::g3d::ResNode* pNode,
                              ml::CVec3* pos, ml::CVec3* scale);
extern "C" void* func_8049431C(u8* obj, u8* image, u16 w, u16 h, u32 fmt,
                               u8* p6);

// Texture-work memory-pool state (retail .sdata/.sbss).
u32 lbl_eu_806639A8 = 0xFFFFFFFF;  // current mem-handle (INVALID_HANDLE before setup)
u32 lbl_eu_806639AC = 0;           // user-supplied mem-handle
s32 lbl_eu_806658E8 = 0;           // live-instance count (retail bgt: signed)
u32 lbl_eu_806658EC = 0;           // allocatable-region base
u32 lbl_eu_806658F0 = 0;           // allocation cursor
u32 lbl_eu_806658F4 = 0;           // max allocatable size
u32 lbl_eu_806658FC = 0;           // busy/reference counter
u32 lbl_eu_80665900 = 0;           // last allocated texture-work object

CScnTexWorkMan::CScnTexWorkMan(u32 param1, u32 param2, u32 param3) {
    this->field_0x4 = param1;
    lbl_eu_806658E8++;
    if (lbl_eu_806639A8 == mtl::INVALID_HANDLE) {
        lbl_eu_806639A8 = mtl::MemManager::create(param2, param3, lbl_eu_80524020);
        lbl_eu_806639AC = param2;
        lbl_eu_806658F4 = mtl::MemManager::getMaxAllocSize(lbl_eu_806639A8);
        lbl_eu_806658EC = (u32)mtl::MemManager::getMaxAllocData(lbl_eu_806639A8);
        lbl_eu_806658F0 = lbl_eu_806658EC;
        CDeviceVI::entryCb(this);
    }
}

CScnTexWorkMan::~CScnTexWorkMan() {
    lbl_eu_806658E8--;
    if (lbl_eu_806658E8 <= 0) {
        mtl::MemManager::erase(lbl_eu_806639A8);
        lbl_eu_806639A8 = mtl::INVALID_HANDLE;
        CDeviceVI::removeCb(this);
    }
}

// Resolve a ResMdl's node dictionary (offset field at 0x60) to a ResNode,
// with the nw4r null/alignment asserts.
extern "C" nw4r::g3d::ResNode func_804909A8(const nw4r::g3d::ResNode* mdl) {
    if (mdl->ptr() == NULL) {
        nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                        lbl_eu_80663910, lbl_eu_806639B8);
    }
    const CScnTexWorkManResData* data2 =
        (const CScnTexWorkManResData*)mdl->ptr();
    nw4r::g3d::ResNode node;
    if (data2->field_0x60 != 0) {
        node = nw4r::g3d::ResNode((u8*)data2 + data2->field_0x60);
        if (((u32)node.ptr() & 3) != 0) {
            nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        }
    } else {
        node = nw4r::g3d::ResNode(NULL);
    }
    return node;
}

// Initialize the position/scale defaults and clamp them against the model's
// node boxes.
extern "C" int func_80490A44(CScnTexWorkMan* self, ml::CVec3* pos,
                             ml::CVec3* scale) {
    nw4r::g3d::ResMdl mdl = nw4r::g3d::ResMdl(self->mResMdl1);
    if (mdl.GetResNode(0).IsValid() == false) {
        return 0;
    }
    pos->x = lbl_eu_8066AA04;
    pos->y = lbl_eu_8066AA04;
    pos->z = lbl_eu_8066AA04;
    scale->x = lbl_eu_8066AA08;
    scale->y = lbl_eu_8066AA08;
    scale->z = lbl_eu_8066AA08;
    CScnTexWorkMan* work = self->field_0x147c;
    nw4r::g3d::ResNode node = mdl.GetResNode(0);
    func_804903B8(work, &node, pos, scale);
    return 1;
}

// Reserve texture-work memory from the pool and build a texture-work object.
extern "C" void* func_80490208(void* p, u16 w, u16 h, u32 fmt) {
    if (lbl_eu_806658FC != 0) {
        return 0;
    }
    u32 size = GXGetTexBufferSize(w, h, fmt, 0, 0);
    u32 cursor = lbl_eu_806658F0;
    u32 mask = cursor & 0x1F;
    u32 aligned = mask != 0 ? cursor + 0x20 - mask : cursor;
    u32 newCursor = aligned + size;
    lbl_eu_806658F0 = newCursor;
    if (newCursor > lbl_eu_806658EC + lbl_eu_806658F4) {
        lbl_eu_806658F0 = cursor;
        return 0;
    }
    u8* obj = (u8*)mtl::MemManager::allocate(
        0x2c, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // func_8049431C returns its first argument, so r3 keeps the object
        // across the call in both paths.
        obj = (u8*)func_8049431C(obj, (u8*)aligned, w, h, fmt, (u8*)cursor);
    }
    lbl_eu_80665900 = (u32)obj;
    return obj;
}

// Clamp one component vector against the box's three bounds, one component
// per axis (pos is max-clamped, scale is min-clamped). The bounds triple is
// rebuilt per axis in the unrolled loop (retail re-loads all three per axis).
static void clampVecMax(f32* v, const CScnTexWorkManEntry* entry) {
    for (int i = 0; i < 3; i++) {
        f32 tri[3] = {entry->field_0x0c, entry->field_0x1c, entry->field_0x2c};
        if (v[i] > tri[i]) {
            v[i] = tri[i];
        }
    }
}
static void clampVecMin(f32* v, const CScnTexWorkManEntry* entry) {
    for (int i = 0; i < 3; i++) {
        f32 tri[3] = {entry->field_0x0c, entry->field_0x1c, entry->field_0x2c};
        if (v[i] < tri[i]) {
            v[i] = tri[i];
        }
    }
}

// Walk the node tree from pNode, clamping pos/scale into every entry box.
extern "C" void func_804903B8(CScnTexWorkMan* self,
                              const nw4r::g3d::ResNode* pNode,
                              ml::CVec3* pos, ml::CVec3* scale) {
    nw4r::g3d::ResNode node1;
    bool valid = (pNode->ptr() != NULL);
    if (valid) {
        if (pNode->IsValid() == false) {
            nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                            lbl_eu_80663910, lbl_eu_806639B8);
        }
        const CScnTexWorkManResData* data =
            (const CScnTexWorkManResData*)pNode->ptr();
        if (data->field_0x5c != 0) {
            node1 = nw4r::g3d::ResNode((u8*)data + data->field_0x5c);
            if (((u32)node1.ptr() & 3) != 0) {
                nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
            }
        } else {
            node1 = nw4r::g3d::ResNode(NULL);
        }
        valid = node1.IsValid();
    }
    if (valid) {
        if (pNode->IsValid() == false) {
            nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                            lbl_eu_80663910, lbl_eu_806639B8);
        }
        const CScnTexWorkManResData* data =
            (const CScnTexWorkManResData*)pNode->ptr();
        if (data->field_0x5c != 0) {
            node1 = nw4r::g3d::ResNode((u8*)data + data->field_0x5c);
            if (((u32)node1.ptr() & 3) != 0) {
                nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
            }
        } else {
            node1 = nw4r::g3d::ResNode(NULL);
        }
        if (node1.IsValid() == false) {
            nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
        }
        s32 idx1 = node1.ptr() != NULL
                       ? ((const CScnTexWorkManResData*)node1.ptr())->field_0x10
                       : 0;
        CScnTexWorkManEntry* entry1 = &self->mEntries[idx1];
        if (pNode->IsValid() == false) {
            nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
        }
        s32 idx2 = pNode->ptr() != NULL
                       ? ((const CScnTexWorkManResData*)pNode->ptr())->field_0x10
                       : 0;
        CScnTexWorkManEntry* entry2 = &self->mEntries[idx2];
        clampVecMax((f32*)pos, entry1);
        clampVecMax((f32*)pos, entry2);
        clampVecMin((f32*)scale, entry1);
        clampVecMin((f32*)scale, entry2);
    }
    if (pNode->IsValid()) {
        if (pNode->IsValid() == false) {
            nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                            lbl_eu_80663910, lbl_eu_806639B8);
        }
        const CScnTexWorkManResData* data =
            (const CScnTexWorkManResData*)pNode->ptr();
        nw4r::g3d::ResNode node2;
        if (data->field_0x64 != 0) {
            node2 = nw4r::g3d::ResNode((u8*)data + data->field_0x64);
            if (((u32)node2.ptr() & 3) != 0) {
                nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
            }
        } else {
            node2 = nw4r::g3d::ResNode(NULL);
        }
        func_804903B8(self, &node2, pos, scale);
        if (pNode->IsValid() == false) {
            nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                            lbl_eu_80663910, lbl_eu_806639B8);
        }
        const CScnTexWorkManResData* data2 =
            (const CScnTexWorkManResData*)pNode->ptr();
        nw4r::g3d::ResNode node3;
        if (data2->field_0x60 != 0) {
            node3 = nw4r::g3d::ResNode((u8*)data2 + data2->field_0x60);
            if (((u32)node3.ptr() & 3) != 0) {
                nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
            }
        } else {
            node3 = nw4r::g3d::ResNode(NULL);
        }
        func_804903B8(self, &node3, pos, scale);
    }
}

// ---- scaffold stubs (unmatched functions, kept as-is) ----

extern "C" void __dt__804943A0(void* self, int flags);

// Release path: while the busy counter is clear, if the arg matches the last
// allocated work object, publish its +0x28 word into the allocation cursor;
// then delete the arg when non-null (retail tail-calls __dt__804943A0). The
// arg is the SECOND parameter (retail compares r4; r3 self is unused).
extern "C" void func_804902D8(void* self, void* arg) {
    if (lbl_eu_806658FC != 0) return;
    if (lbl_eu_80665900 == (u32)arg) {
        lbl_eu_806658F0 = *(u32*)((u8*)arg + 0x28);
    }
    if (arg != 0) {
        __dt__804943A0(arg, 1);
    }
}

void func_80490310__14CScnTexWorkManFv(void) {}

int lbl_eu_806658F8;

void func_80490314() {
    if (lbl_eu_806658FC == 0) {
        lbl_eu_806658F8 = lbl_eu_806658F0;
    }
}

void func_8049032C() {
    if (lbl_eu_806658FC != 0) return;
    lbl_eu_806658F0 = lbl_eu_806658F8;
    lbl_eu_806658F8 = 0;
}

void func_8049034C(){}

void func_8049090C(){}

extern "C" nw4r::g3d::ResNode func_80490AF4(void* self, const char* name) {
    nw4r::g3d::ResMdl mdl = *(nw4r::g3d::ResMdl*)((u8*)self + 0x146C);
    return mdl.GetResNode(name);
}

// Resolve a ResNode's next sibling (0x64 offset) - retail helper (stub).
nw4r::g3d::ResNode func_8049090C(const nw4r::g3d::ResNode* pNode) {
    return nw4r::g3d::ResNode();
}

// Count every node whose bounding volume (volume_min/volume_max, 0x44..0x58)
// is not the all-sentinel degenerate box, walking the tree as: node itself,
// then the node's next sibling and its child. Each component check is guarded
// by the retail node-null assert, and the six checks share one count tail.
void func_80490B20(nw4r::g3d::ResNode* pNode, int* pCounter) {
    if (pNode->ptr() == NULL) {
        return;
    }
    if (pNode->ptr() == NULL) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (pNode->ptr()->volume_min.x != lbl_eu_8066AA00) goto L_count;
    if (pNode->ptr() == NULL) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (pNode->ptr()->volume_min.y != lbl_eu_8066AA00) goto L_count;
    if (pNode->ptr() == NULL) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (pNode->ptr()->volume_min.z != lbl_eu_8066AA00) goto L_count;
    if (pNode->ptr() == NULL) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (pNode->ptr()->volume_max.x != lbl_eu_8066AA00) goto L_count;
    if (pNode->ptr() == NULL) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (pNode->ptr()->volume_max.y != lbl_eu_8066AA00) goto L_count;
    if (pNode->ptr() == NULL) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (pNode->ptr()->volume_max.z == lbl_eu_8066AA00) goto L_ok;
L_count:
    (*pCounter)++;
L_ok:
    if (pNode->ptr() == NULL) {
        nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                        lbl_eu_80663910, lbl_eu_806639B8);
    }
    nw4r::g3d::ResNode sib;
    if (pNode->ptr()->toNextSibling != 0) {
        sib = nw4r::g3d::ResNode((u8*)pNode->ptr() + pNode->ptr()->toNextSibling);
        if (((u32)sib.ptr() & 3) != 0) {
            nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        }
    } else {
        sib = nw4r::g3d::ResNode(NULL);
    }
    if (sib.ptr() != NULL) {
        if (sib.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != sib.ptr()->volume_min.x) goto L_countSib;
        if (sib.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != sib.ptr()->volume_min.y) goto L_countSib;
        if (sib.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != sib.ptr()->volume_min.z) goto L_countSib;
        if (sib.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != sib.ptr()->volume_max.x) goto L_countSib;
        if (sib.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != sib.ptr()->volume_max.y) goto L_countSib;
        if (sib.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 == sib.ptr()->volume_max.z) goto L_okSib;
L_countSib:
        (*pCounter)++;
L_okSib:
        nw4r::g3d::ResNode sib2 = func_8049090C(&sib);
        func_80490B20(&sib2, pCounter);
        nw4r::g3d::ResNode sib3 = func_804909A8(&sib);
        func_80490B20(&sib3, pCounter);
    }
    if (pNode->ptr() == NULL) {
        nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                        lbl_eu_80663910, lbl_eu_806639B8);
    }
    nw4r::g3d::ResNode child;
    if (pNode->ptr()->toChildNode != 0) {
        child = nw4r::g3d::ResNode((u8*)pNode->ptr() + pNode->ptr()->toChildNode);
        if (((u32)child.ptr() & 3) != 0) {
            nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        }
    } else {
        child = nw4r::g3d::ResNode(NULL);
    }
    if (child.ptr() != NULL) {
        if (child.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != child.ptr()->volume_min.x) goto L_countChild;
        if (child.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != child.ptr()->volume_min.y) goto L_countChild;
        if (child.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != child.ptr()->volume_min.z) goto L_countChild;
        if (child.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != child.ptr()->volume_max.x) goto L_countChild;
        if (child.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != child.ptr()->volume_max.y) goto L_countChild;
        if (child.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 == child.ptr()->volume_max.z) goto L_okChild;
L_countChild:
        (*pCounter)++;
L_okChild:
        nw4r::g3d::ResNode child2 = func_8049090C(&child);
        func_80490B20(&child2, pCounter);
        nw4r::g3d::ResNode child3 = func_804909A8(&child);
        func_80490B20(&child3, pCounter);
    }
}

// Same tree walk as func_80490B20, but keeps the count in a local and returns
// it: used for the model root node.
int func_80491158(nw4r::g3d::ResNode* pNode) {
    int counter = 0;
    const nw4r::g3d::ResNodeData* node = pNode->ptr();
    if (node != NULL) {
        if (node == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != node->volume_min.x) goto L_count;
        if (node == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != node->volume_min.y) goto L_count;
        if (node == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != node->volume_min.z) goto L_count;
        if (node == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != node->volume_max.x) goto L_count;
        if (node == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != node->volume_max.y) goto L_count;
        if (node == NULL) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 == node->volume_max.z) goto L_ok;
L_count:
        counter++;
L_ok:
        if (node == NULL) {
            nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                            lbl_eu_80663910, lbl_eu_806639B8);
        }
        nw4r::g3d::ResNode sib;
        if (node->toNextSibling != 0) {
            sib = nw4r::g3d::ResNode((u8*)node + node->toNextSibling);
            if (((u32)sib.ptr() & 3) != 0) {
                nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
            }
        } else {
            sib = nw4r::g3d::ResNode(NULL);
        }
        if (sib.ptr() != NULL) {
            if (!sib.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 != sib.ptr()->volume_min.x) goto L_countSib;
            if (!sib.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 != sib.ptr()->volume_min.y) goto L_countSib;
            if (!sib.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 != sib.ptr()->volume_min.z) goto L_countSib;
            if (!sib.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 != sib.ptr()->volume_max.x) goto L_countSib;
            if (!sib.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 != sib.ptr()->volume_max.y) goto L_countSib;
            if (!sib.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 == sib.ptr()->volume_max.z) goto L_okSib;
L_countSib:
            counter++;
L_okSib:
            nw4r::g3d::ResNode sib2 = func_8049090C(&sib);
            func_80490B20(&sib2, &counter);
            nw4r::g3d::ResNode sib3 = func_804909A8(&sib);
            func_80490B20(&sib3, &counter);
        }
        if (node == NULL) {
            nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                            lbl_eu_80663910, lbl_eu_806639B8);
        }
        nw4r::g3d::ResNode child;
        if (node->toChildNode != 0) {
            child = nw4r::g3d::ResNode((u8*)node + node->toChildNode);
            if (((u32)child.ptr() & 3) != 0) {
                nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
            }
        } else {
            child = nw4r::g3d::ResNode(NULL);
        }
        if (child.ptr() != NULL) {
            if (!child.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 != child.ptr()->volume_min.x) goto L_countChild;
            if (!child.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 != child.ptr()->volume_min.y) goto L_countChild;
            if (!child.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 != child.ptr()->volume_min.z) goto L_countChild;
            if (!child.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 != child.ptr()->volume_max.x) goto L_countChild;
            if (!child.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 != child.ptr()->volume_max.y) goto L_countChild;
            if (!child.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_806639BC);
            }
            if (lbl_eu_8066AA00 == child.ptr()->volume_max.z) goto L_okChild;
L_countChild:
            counter++;
L_okChild:
            nw4r::g3d::ResNode child2 = func_8049090C(&child);
            func_80490B20(&child2, &counter);
            nw4r::g3d::ResNode child3 = func_804909A8(&child);
            func_80490B20(&child3, &counter);
        }
    }
    return counter;
}

// Same tree walk as func_80490B20, but non-degenerate nodes are collected by
// id (node->id at 0xc, or 0 for a null node) into pList instead of counted.
void func_80491764(nw4r::g3d::ResNode* pNode, CScnTexWorkManNodeIdList* pList) {
    if (pNode->ptr() == NULL) {
        return;
    }
    if (!pNode->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (lbl_eu_8066AA00 != pNode->ptr()->volume_min.x) goto L_mismatch;
    if (!pNode->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (lbl_eu_8066AA00 != pNode->ptr()->volume_min.y) goto L_mismatch;
    if (!pNode->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (lbl_eu_8066AA00 != pNode->ptr()->volume_min.z) goto L_mismatch;
    if (!pNode->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (lbl_eu_8066AA00 != pNode->ptr()->volume_max.x) goto L_mismatch;
    if (!pNode->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (lbl_eu_8066AA00 != pNode->ptr()->volume_max.y) goto L_mismatch;
    if (!pNode->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_806639BC);
    }
    if (lbl_eu_8066AA00 == pNode->ptr()->volume_max.z) goto L_ok;
L_mismatch:
    if (!pNode->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
    }
    {
        const nw4r::g3d::ResNodeData* n = pNode->ptr();
        pList->array[pList->count++] = (n != NULL ? n->id : 0);
    }
L_ok:
    if (!pNode->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                        lbl_eu_80663910, lbl_eu_806639B8);
    }
    const nw4r::g3d::ResNodeData* node = pNode->ptr();
    nw4r::g3d::ResNode sib;
    if (node->toNextSibling != 0) {
        sib = nw4r::g3d::ResNode((u8*)node + node->toNextSibling);
        if (((u32)sib.ptr() & 3) != 0) {
            nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        }
    } else {
        sib = nw4r::g3d::ResNode(NULL);
    }
    if (sib.ptr() != NULL) {
        if (!sib.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != sib.ptr()->volume_min.x) goto L_mismatchSib;
        if (!sib.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != sib.ptr()->volume_min.y) goto L_mismatchSib;
        if (!sib.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != sib.ptr()->volume_min.z) goto L_mismatchSib;
        if (!sib.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != sib.ptr()->volume_max.x) goto L_mismatchSib;
        if (!sib.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != sib.ptr()->volume_max.y) goto L_mismatchSib;
        if (!sib.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 == sib.ptr()->volume_max.z) goto L_okSib;
L_mismatchSib:
        if (!sib.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
        }
        {
            const nw4r::g3d::ResNodeData* n = sib.ptr();
            pList->array[pList->count++] = (n != NULL ? n->id : 0);
        }
L_okSib:
        nw4r::g3d::ResNode sib2 = func_8049090C(&sib);
        func_80491764(&sib2, pList);
        nw4r::g3d::ResNode sib3 = func_804909A8(&sib);
        func_80491764(&sib3, pList);
    }
    if (!pNode->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                        lbl_eu_80663910, lbl_eu_806639B8);
    }
    node = pNode->ptr();
    nw4r::g3d::ResNode child;
    if (node->toChildNode != 0) {
        child = nw4r::g3d::ResNode((u8*)node + node->toChildNode);
        if (((u32)child.ptr() & 3) != 0) {
            nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        }
    } else {
        child = nw4r::g3d::ResNode(NULL);
    }
    if (child.ptr() != NULL) {
        if (!child.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != child.ptr()->volume_min.x) goto L_mismatchChild;
        if (!child.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != child.ptr()->volume_min.y) goto L_mismatchChild;
        if (!child.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != child.ptr()->volume_min.z) goto L_mismatchChild;
        if (!child.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != child.ptr()->volume_max.x) goto L_mismatchChild;
        if (!child.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 != child.ptr()->volume_max.y) goto L_mismatchChild;
        if (!child.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_806639BC);
        }
        if (lbl_eu_8066AA00 == child.ptr()->volume_max.z) goto L_okChild;
L_mismatchChild:
        if (!child.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
        }
        {
            const nw4r::g3d::ResNodeData* n = child.ptr();
            pList->array[pList->count++] = (n != NULL ? n->id : 0);
        }
L_okChild:
        nw4r::g3d::ResNode child2 = func_8049090C(&child);
        func_80491764(&child2, pList);
        nw4r::g3d::ResNode child3 = func_804909A8(&child);
        func_80491764(&child3, pList);
    }
}
