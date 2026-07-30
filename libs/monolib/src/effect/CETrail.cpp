// monolib/src/effect/CETrail.cpp
// Trail effect renderer (retail 0x804DADD4..0x804DC6C0).
//
// The retail object has no data sections: class vtables live at absolute DOL
// addresses (lbl_eu_8056FC40/FC4C/FC64) and are referenced through external
// relocations, so the classes are modeled as POD structs with explicit
// vtable pointers and the deleting destructors as free functions.

#include <harness_catalog.h>
#include <revolution/MTX.h>
#include <revolution/GX.h>
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CVec4.hpp"
#include "monolib/util/MemManager.hpp"

// ---------------------------------------------------------------------------
// External vtables / constants / globals (absolute DOL addresses)
// ---------------------------------------------------------------------------
extern "C" {
extern u8 lbl_eu_8056FC40[]; // CETrail secondary interface vtable
extern u8 lbl_eu_8056FC4C[]; // CETrail vtable
extern u8 lbl_eu_8056FC64[]; // reslist<CETrail::POINT> vtable

extern f32 lbl_eu_8066B15C;
extern f32 lbl_eu_8066B158;
extern f32 lbl_eu_8066B178;
extern f64 lbl_eu_8066B180;
extern f32 lbl_eu_8066B188;
extern f32 lbl_eu_8066B198;
extern f32 lbl_eu_8066B19C;
extern f32 lbl_eu_8066B1A0;
extern f32 lbl_eu_8066B1A4;
extern f32 lbl_eu_8066B1A8;
extern f32 lbl_eu_8066B1AC;

struct CETrailHeapInfo {
    u32 m_00;
    mtl::ALLOC_HANDLE m_handle; // 0x4
};
extern CETrailHeapInfo lbl_eu_8065FC18;

struct FogData {
    u32 m_type; // 0x00
    f32 m_startz;
    f32 m_endz;
    f32 m_nearz;
    f32 m_farz; // 0x10
    u8 m_r, m_g, m_b, m_a;
};
struct FogState {
    u32 m_type; // 0x00
    f32 m_startz;
    f32 m_endz;
    f32 m_nearz;
    f32 m_farz;  // 0x10
    u32 m_color; // 0x14
    u8 m_18;
    u8 m_19;
    u16 m_1A;
    u32 m_1C;
    u32 m_20;
    u32 m_24;
    u32 m_28;
    u32 m_2C;
};
extern FogState lbl_eu_8065FCA0;
extern FogState lbl_eu_8065FCD0;

struct CScnFogEnv {
    u8 m_pad00[0x24];
    s32 m_index; // 0x24
};
struct CScnRootEnv {
    u8 m_pad00[0x78];
    CScnFogEnv* m_fogEnv; // 0x78
};
extern CScnRootEnv* lbl_eu_806659B8;
extern s32 lbl_eu_80663B38;
extern s32 lbl_eu_806659BC;
extern u8 lbl_eu_80663B3C;

void __dl__FPv(void*);
void __dla__FPv(void*);
void* __construct_new_array(void* block, void (*ctor)(void*), void (*dtor)(void*), u32 size, u32 count);
}

// ---------------------------------------------------------------------------
// Cross-TU calls (placeholder symbols; extern "C" keeps retail reloc names)
// ---------------------------------------------------------------------------
extern "C" {
void func_804EE558(void* link, void* arg, u32 type, void* data, void* unk);
void func_804EE60C(void* link);
int func_804EEACC(void* link);
void func_804EE8FC(void* link, u32 index);
void func_804F2A8C(void* obj);
void* func_804D5F54(void* self, s16 count);
void func_804D6070(void* vtx);
void func_804D6074(void* trailSeg, void* trail, const void* color, u32 a, u32 b, const void* c, const void* d);
void* func_80494128(void* res);
void func_80494188(void* res);
void func_804C03A0(void* light, u32 mode);
void func_804C0454(void* light, void* arg);
void func_804C07F0(void* light, const void* color);
void func_804C08C8(void* light, u32 mode);
void func_804C09E0(void* light, s32 mode, f32 a, f32 b);
void* func_8048ECD8(void* scene, void* arg, s32 index);
}

namespace nw4r {
namespace g3d {
struct ScnRoot {
    FogData* GetFog(int index);
};
} // namespace g3d
} // namespace nw4r

// ---------------------------------------------------------------------------
// CETrail types
// ---------------------------------------------------------------------------

// Trail point payload (0x1C)
struct CETrailPoint {
    ml::CVec3 m_posA; // 0x00
    ml::CVec3 m_posB; // 0x0C
    f32 m_age;        // 0x18
};

// Intrusive list node wrapping a point (0x24)
struct CETrailNode {
    CETrailNode* m_next; // 0x00
    CETrailNode* m_prev; // 0x04
    CETrailPoint m_item; // 0x08
};

// GPU vertex (0x1C)
struct CETrailVertex {
    f32 m_x;     // 0x00
    f32 m_y;     // 0x04
    f32 m_z;     // 0x08
    f32 m_u;     // 0x0C
    f32 m_v;     // 0x10
    u8 m_shade;  // 0x18
    u8 m_pad[3]; // 0x19
};

// Bone/link attachment (0x48)
struct CETrailLink {
    u8 m_pad00[0x24];
    f32 m_mtx00; // 0x24
    u8 m_pad28[0xC];
    f32 m_mtx10; // 0x34
    u8 m_pad38[0xC];
    f32 m_mtx20; // 0x44
};

// Small inline helper slots (0xC each)
struct CETrailSlot {
    u32 m_a;
    u32 m_b;
    u32 m_c;
    void init() {}
};

struct CETrail {
    void* m_vtable;           // 0x000
    CETrailNode* m_head;      // 0x004 -> &m_startNode
    CETrailNode m_startNode;  // 0x008
    CETrailNode* m_list;      // 0x02C
    u32 m_capacity;           // 0x030
    u8 m_ownsList;            // 0x034
    u8 m_pad035[3];           // 0x035
    s16 m_segCount;           // 0x038
    u8 m_pad03A[2];           // 0x03A
    CETrailVertex* m_verts;   // 0x03C
    void* m_buffer;           // 0x040
    CETrailSlot m_slots0[2];  // 0x044
    CETrailSlot m_slots1[2];  // 0x05C
    CETrailSlot m_slots2[2];  // 0x074
    CETrailSlot m_slots3[2];  // 0x08C
    s32 m_numVerts;           // 0x0A4
    f32 m_texU0;              // 0x0A8
    f32 m_texV0;              // 0x0AC
    f32 m_texU1;              // 0x0B0
    f32 m_texV1;              // 0x0B4
    f32 m_texU0Scale;         // 0x0B8
    f32 m_texV0Scale;         // 0x0BC
    f32 m_texU1Scale;         // 0x0C0
    f32 m_texV1Scale;         // 0x0C4
    CETrailLink m_link0;      // 0x0C8
    CETrailLink m_link1;      // 0x110
    u8 m_158;                 // 0x158
    u8 m_pad159[3];           // 0x159
    u32 m_15C;                // 0x15C
    u32 m_mode;               // 0x160
    ml::CVec4 m_color;        // 0x164
    ml::CVec4 m_scale;        // 0x174
    void* m_vtable2;          // 0x184
};

// Light wrapper used by func_804D807C/__dt__804D80F0/func_804D8160
struct CLight {
    u8 m_pad00[0x30];
    u32 m_flags; // 0x30
    f32 m_3C;    // 0x3C
};

struct CResHolder {
    u8 m_pad00[0x5C];
    void* m_res; // 0x5C
};

struct CETrailLight {
    CResHolder* m_parent; // 0x0
    CLight* m_light;      // 0x4
};

struct CETrailLightParam {
    f32 m_r;         // 0x0
    f32 m_g;         // 0x4
    f32 m_b;         // 0x8
    f32 m_intensity; // 0xC
};

// ---------------------------------------------------------------------------
// List helpers (inlined into users, mirroring reslist<CETrail::POINT>)
// ---------------------------------------------------------------------------
static u32 trailListSize(CETrail* t) {
    CETrailNode* head = t->m_head;
    CETrailNode* cur = head->m_next;
    u32 n = 0;
    while (cur != head) {
        cur = cur->m_next;
        n++;
    }
    return n;
}

static void trailClearList(CETrail* t) {
    CETrailNode* head = t->m_head;
    CETrailNode* cur = head->m_next;
    while (cur != head) {
        CETrailNode* next = cur->m_next;
        cur->m_next = nullptr;
        cur = next;
    }
    head->m_next = head;
    head->m_prev = head;
}

// ---------------------------------------------------------------------------
// func_804D70A0: POINT node constructor (array element init)
// ---------------------------------------------------------------------------
extern "C" void func_804D70A0(CETrailNode* node);

// ---------------------------------------------------------------------------
// __dt__804D6C60: CETrail constructor (retail placeholder name)
// ---------------------------------------------------------------------------
extern "C" void* __dt__804D6C60(CETrail* t, u32 count, s16 segCount, void* linkArg,
                                u8* dataA, u8* dataB, u8 flag158, u32 id15C) {
    t->m_vtable2 = lbl_eu_8056FC40;
    t->m_vtable = lbl_eu_8056FC64;

    ml::CVec3 za = ml::CVec3::zero;
    t->m_startNode.m_item.m_posA = za;
    ml::CVec3 zb = ml::CVec3::zero;
    t->m_startNode.m_item.m_posB = zb;
    t->m_startNode.m_item.m_age = lbl_eu_8066B15C;
    t->m_list = nullptr;
    t->m_capacity = 0;
    t->m_ownsList = false;
    t->m_head = &t->m_startNode;
    t->m_startNode.m_next = &t->m_startNode;
    t->m_startNode.m_prev = &t->m_startNode;
    t->m_vtable = lbl_eu_8056FC4C;

    for (CETrailSlot* p = &t->m_slots0[0]; p < &t->m_slots0[2]; p++) {
        p->init();
    }
    for (CETrailSlot* p = &t->m_slots1[0]; p < &t->m_slots1[2]; p++) {
        p->init();
    }
    for (CETrailSlot* p = &t->m_slots2[0]; p < &t->m_slots2[2]; p++) {
        p->init();
    }
    for (CETrailSlot* p = &t->m_slots3[0]; p < &t->m_slots3[2]; p++) {
        p->init();
    }

    t->m_buffer = nullptr;
    t->m_verts = nullptr;

    func_804F2A8C((u8*)&t->m_link0 + 0xC);
    func_804F2A8C((u8*)&t->m_link1 + 0xC);

    if ((s32)count < 2) {
        count = 2;
    }

    mtl::ALLOC_HANDLE handle = lbl_eu_8065FC18.m_handle;
    void* block = mtl::MemManager::allocate_array(count * 0x24 + 0x10, handle);
    t->m_list = (CETrailNode*)__construct_new_array(block, (void (*)(void*))func_804D70A0, nullptr, 0x24, count);

    for (u32 i = 0; i < count; i++) {
        t->m_list[i].m_next = nullptr;
    }
    t->m_capacity = count;

    trailClearList(t);

    if (t->m_verts != nullptr) {
        delete[] t->m_verts;
        t->m_verts = nullptr;
    }

    t->m_segCount = segCount;

    if (t->m_buffer != nullptr) {
        __dla__FPv(t->m_buffer);
        t->m_buffer = nullptr;
    }

    t->m_buffer = func_804D5F54(&t->m_buffer, (s16)segCount);

    u32 need = ((u32)segCount * 0x1C) * (count * 2);
    CETrailVertex* verts;
    if (need <= mtl::MemManager::getMaxAllocSize(lbl_eu_8065FC18.m_handle)) {
        u32 n2 = segCount * count * 2;
        void* block2 = mtl::MemManager::allocate_array(n2 * 0x1C + 0x10, lbl_eu_8065FC18.m_handle);
        verts = (CETrailVertex*)__construct_new_array(block2, (void (*)(void*))func_804D6070, nullptr, 0x1C, n2);
    } else {
        verts = nullptr;
    }
    if (verts == nullptr) {
        verts = nullptr;
    }
    t->m_verts = verts;

    void* payloadA;
    if (dataA != nullptr) {
        payloadA = dataA - 8;
    } else {
        payloadA = nullptr;
    }
    u32 typeA;
    if (dataA != nullptr) {
        typeA = dataA[-0x10];
    } else {
        typeA = 0;
    }
    func_804EE558(&t->m_link0, linkArg, typeA, payloadA, nullptr);

    if (dataB != nullptr) {
        func_804EE558(&t->m_link1, linkArg, dataB[0], dataB + 2, nullptr);
    } else {
        func_804EE558(&t->m_link1, linkArg, 0, nullptr, nullptr);
    }

    t->m_158 = flag158;
    t->m_15C = id15C;

    u32 mode;
    if (dataB == nullptr) {
        mode = 0;
    } else {
        u32 type = (dataA != nullptr) ? dataA[-0xE] : 0;
        mode = (type == 7) ? 2 : 1;
    }
    t->m_mode = mode;

    t->m_scale = ml::CVec4::zero;
    t->m_color = ml::CVec4::zero;

    return t;
}

// ---------------------------------------------------------------------------
// func_804D70A0: POINT node constructor
// ---------------------------------------------------------------------------
extern "C" void func_804D70A0(CETrailNode* node) {
    ml::CVec3 za = ml::CVec3::zero;
    node->m_item.m_posA = za;
    ml::CVec3 zb = ml::CVec3::zero;
    node->m_item.m_posB = zb;
    node->m_item.m_age = lbl_eu_8066B15C;
}

// ---------------------------------------------------------------------------
// __dt___reslist_base_CETrail_POINT: reslist base deleting destructor
// ---------------------------------------------------------------------------
extern "C" CETrail* __dt___reslist_base_CETrail_POINT(CETrail* t, int deleting) {
    if (t != nullptr) {
        t->m_vtable = lbl_eu_8056FC64;
        trailClearList(t);
        if (!t->m_ownsList && t->m_list != nullptr) {
            delete[] t->m_list;
            t->m_list = nullptr;
        }
        if (deleting > 0) {
            __dl__FPv(t);
        }
    }
    return t;
}

// ---------------------------------------------------------------------------
// __dt__reslist_CETrail_POINT: reslist deleting destructor
// ---------------------------------------------------------------------------
extern "C" CETrail* __dt__reslist_CETrail_POINT(CETrail* t, int deleting) {
    if (t != nullptr) {
        if (t != nullptr) {
            t->m_vtable = lbl_eu_8056FC64;
            trailClearList(t);
            if (!t->m_ownsList && t->m_list != nullptr) {
                delete[] t->m_list;
                t->m_list = nullptr;
            }
        }
        if (deleting > 0) {
            __dl__FPv(t);
        }
    }
    return t;
}

// ---------------------------------------------------------------------------
// __dt__7CETrailFv: CETrail deleting destructor
// ---------------------------------------------------------------------------
extern "C" CETrail* __dt__7CETrailFv(CETrail* t, int deleting) {
    if (t != nullptr) {
        t->m_vtable2 = lbl_eu_8056FC40;

        if (t->m_verts != nullptr) {
            delete[] t->m_verts;
            t->m_verts = nullptr;
        }
        if (t->m_buffer != nullptr) {
            __dla__FPv(t->m_buffer);
            t->m_buffer = nullptr;
        }

        trailClearList(t);
        if (!t->m_ownsList && t->m_list != nullptr) {
            delete[] t->m_list;
            t->m_list = nullptr;
        }
        t->m_capacity = 0;

        if (t != nullptr) {
            t->m_vtable = lbl_eu_8056FC64;
            trailClearList(t);
            if (!t->m_ownsList && t->m_list != nullptr) {
                delete[] t->m_list;
                t->m_list = nullptr;
            }
        }

        if (deleting > 0) {
            __dl__FPv(t);
        }
    }
    return t;
}

// ---------------------------------------------------------------------------
// func_804D73FC: release both link attachments
// ---------------------------------------------------------------------------
extern "C" void func_804D73FC(CETrail* t) {
    func_804EE60C(&t->m_link0);
    func_804EE60C(&t->m_link1);
}

// ---------------------------------------------------------------------------
// func_804D7434: draw the trail strip
// ---------------------------------------------------------------------------
extern "C" void func_804D7434(CETrail* t, s32 mode, const u8* color) {
    bool draw = false;
    if (t->m_verts != nullptr && trailListSize(t) >= 2) {
        f32 alpha = (f32)color[3];
        draw = alpha > lbl_eu_8066B15C;
    }
    if (!draw) {
        return;
    }

    func_804D6074(&t->m_segCount, t, color, t->m_mode != 2, mode < 0, &t->m_color, &t->m_scale);

    mtl::MemManager::func_80434A4C(false);

    switch (mode) {
    case 1: {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT2, (u16)t->m_numVerts);
        for (s16 i = 0; i < t->m_numVerts; i++) {
            CETrailVertex* v = &t->m_verts[i];
            WGPIPE.f = v->m_x;
            WGPIPE.f = v->m_y;
            WGPIPE.f = v->m_z;
            WGPIPE.uc = color[0];
            WGPIPE.uc = color[1];
            WGPIPE.uc = color[2];
            WGPIPE.uc = (u8)(f32)v->m_shade;
            WGPIPE.f = t->m_texU0Scale * (t->m_texU0 + v->m_u);
            WGPIPE.f = t->m_texV0Scale * (t->m_texV0 + v->m_v);
        }
        break;
    }
    case 2: {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT2, (u16)t->m_numVerts);
        for (s16 i = 0; i < t->m_numVerts; i++) {
            CETrailVertex* v = &t->m_verts[i];
            WGPIPE.f = v->m_x;
            WGPIPE.f = v->m_y;
            WGPIPE.f = v->m_z;
            WGPIPE.uc = color[0];
            WGPIPE.uc = color[1];
            WGPIPE.uc = color[2];
            WGPIPE.uc = (u8)(f32)v->m_shade;
            WGPIPE.f = t->m_texU0Scale * (t->m_texU0 + v->m_u);
            WGPIPE.f = t->m_texV0Scale * (t->m_texV0 + v->m_v);
            WGPIPE.f = t->m_texU1Scale * (t->m_texU1 + v->m_u);
            WGPIPE.f = t->m_texV1Scale * (t->m_texV1 + v->m_v);
        }
        break;
    }
    default: {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT2, (u16)t->m_numVerts);
        for (s16 i = 0; i < t->m_numVerts; i++) {
            CETrailVertex* v = &t->m_verts[i];
            WGPIPE.f = v->m_x;
            WGPIPE.f = v->m_y;
            WGPIPE.f = v->m_z;
            WGPIPE.uc = color[0];
            WGPIPE.uc = color[1];
            WGPIPE.uc = color[2];
            WGPIPE.uc = (u8)(f32)v->m_shade;
        }
        break;
    }
    }

    mtl::MemManager::func_80434A4C(true);
}

// ---------------------------------------------------------------------------
// func_804D7B28: append/update one trail segment
// ---------------------------------------------------------------------------
extern "C" void func_804D7B28(CETrail* t, const ml::CVec3* posA, const ml::CVec3* posB,
                              const ml::CVec4* color, const ml::CVec4* scale);

// ---------------------------------------------------------------------------
// func_804D77E4: update trail geometry from link matrices
// ---------------------------------------------------------------------------
extern "C" void func_804D77E4(CETrail* t, const Mtx* M, const ml::CVec4* color, const ml::CVec4* scale, f32 f) {
    if (func_804EEACC(&t->m_link0) == 0) {
        return;
    }
    if (func_804EEACC(&t->m_link1) == 0) {
        return;
    }

    f32 k = lbl_eu_8066B178;

    switch (t->m_mode) {
    case 1: {
        func_804EE8FC(&t->m_link0, t->m_15C);
        ml::CVec3 pos0;
        pos0.x = t->m_link0.m_mtx00;
        pos0.y = t->m_link0.m_mtx10;
        pos0.z = t->m_link0.m_mtx20;
        func_804EE8FC(&t->m_link1, t->m_15C);
        ml::CVec3 pos1;
        pos1.x = t->m_link1.m_mtx00;
        pos1.y = t->m_link1.m_mtx10;
        pos1.z = t->m_link1.m_mtx20;
        func_804D7B28(t, &pos0, &pos1, color, scale);
        break;
    }
    case 2: {
        ml::CVec3 off;
        if (t->m_158 == 0) {
            off.x = lbl_eu_8066B15C;
            off.y = k * f;
            off.z = lbl_eu_8066B15C;
        } else {
            off.x = k * f;
            off.y = lbl_eu_8066B15C;
            off.z = lbl_eu_8066B15C;
        }
        func_804EE8FC(&t->m_link0, t->m_15C);
        ml::CVec3 pos0;
        pos0.x = t->m_link0.m_mtx00;
        pos0.y = t->m_link0.m_mtx10;
        pos0.z = t->m_link0.m_mtx20;
        func_804EE8FC(&t->m_link1, t->m_15C);
        ml::CVec3 pos1;
        pos1.x = t->m_link1.m_mtx00;
        pos1.y = t->m_link1.m_mtx10;
        pos1.z = t->m_link1.m_mtx20;

        ml::CVec3 sum0;
        sum0.x = pos0.x + off.x;
        sum0.y = pos0.y + off.y;
        sum0.z = pos0.z + off.z;
        ml::CVec3 diff0;
        diff0.x = pos0.x - off.x;
        diff0.y = pos0.y - off.y;
        diff0.z = pos0.z - off.z;
        func_804D7B28(t, &sum0, &diff0, color, scale);

        ml::CVec3 sum1;
        sum1.x = pos1.x + off.x;
        sum1.y = pos1.y + off.y;
        sum1.z = pos1.z + off.z;
        ml::CVec3 diff1;
        diff1.x = pos1.x - off.x;
        diff1.y = pos1.y - off.y;
        diff1.z = pos1.z - off.z;
        func_804D7B28(t, &sum1, &diff1, color, scale);
        break;
    }
    default: {
        ml::CVec3 off;
        off.x = lbl_eu_8066B15C;
        off.y = k * f;
        off.z = lbl_eu_8066B15C;
        f32 c = lbl_eu_8066B188;
        ml::CVec3 offc;
        offc.x = off.x * c;
        offc.y = off.y * c;
        offc.z = off.z * c;
        ml::CVec3 v1;
        PSMTXMultVec(*M, offc, v1);
        ml::CVec3 v2;
        PSMTXMultVec(*M, off, v2);
        func_804D7B28(t, &v2, &v1, color, scale);
        break;
    }
    }
}

extern "C" void func_804D7B28(CETrail* t, const ml::CVec3* posA, const ml::CVec3* posB,
                              const ml::CVec4* color, const ml::CVec4* scale) {
    CETrailNode* head = t->m_head;

    // Distance between posA and the newest point
    f32 dist;
    {
        u32 n = 0;
        CETrailNode* cur = head->m_next;
        while (cur != head) {
            cur = cur->m_next;
            n++;
        }
        if (n != 0) {
            CETrailNode* newest = head->m_prev;
            ml::CVec3 d;
            d.x = posA->x - newest->m_item.m_posA.x;
            d.y = posA->y - newest->m_item.m_posA.y;
            d.z = posA->z - newest->m_item.m_posA.z;
            dist = PSVECMag(d);
        } else {
            dist = lbl_eu_8066B15C;
        }
    }

    // Drop the oldest point when the trail would not advance
    {
        u32 n = 0;
        CETrailNode* cur = head->m_next;
        while (cur != head) {
            cur = cur->m_next;
            n++;
        }
        if (n != 0 && dist == lbl_eu_8066B15C) {
            CETrailNode* oldest = head->m_next;
            CETrailNode* prev = oldest->m_prev;
            CETrailNode* next = oldest->m_next;
            prev->m_next = next;
            next->m_prev = prev;
            oldest->m_next = nullptr;
            return;
        }
    }

    // Make room when at capacity
    {
        u32 n = 0;
        CETrailNode* cur = head->m_next;
        while (cur != head) {
            cur = cur->m_next;
            n++;
        }
        if (t->m_capacity <= n) {
            CETrailNode* oldest = head->m_next;
            CETrailNode* prev = oldest->m_prev;
            CETrailNode* next = oldest->m_next;
            prev->m_next = next;
            next->m_prev = prev;
            oldest->m_next = nullptr;
        }
    }

    // Build the new point
    CETrailPoint item;
    item.m_posA = *posA;
    item.m_posB = *posB;
    item.m_age = lbl_eu_8066B15C;

    // Find a free slot
    u32 i = 0;
    while (i < t->m_capacity) {
        if (t->m_list[i].m_next == nullptr) {
            break;
        }
        i++;
    }

    CETrailNode* slot = &t->m_list[i];
    slot->m_item = item;
    slot->m_next = head;
    slot->m_prev = head->m_prev;
    head->m_prev->m_next = slot;
    head->m_prev = slot;

    // Age existing points
    f32 frontAge;
    {
        u32 n = 0;
        CETrailNode* cur = head->m_next;
        while (cur != head) {
            cur = cur->m_next;
            n++;
        }
        if (n >= 2) {
            cur = head->m_prev->m_prev;
            while (true) {
                cur->m_item.m_age += dist;
                CETrailNode* oldest = head->m_next;
                if (cur == oldest) {
                    frontAge = oldest->m_item.m_age;
                    break;
                }
                cur = cur->m_prev;
            }
        } else {
            frontAge = lbl_eu_8066B15C;
        }
    }

    t->m_color = *color;

    if (scale->x > lbl_eu_8066B15C) {
        t->m_scale.x = frontAge / scale->x;
    } else {
        t->m_scale.w = lbl_eu_8066B158;
        t->m_scale.x = lbl_eu_8066B158;
    }

    if (scale->y > lbl_eu_8066B15C) {
        ml::CVec3 d;
        d.x = posB->x - posA->x;
        d.y = posB->y - posA->y;
        d.z = posB->z - posA->z;
        t->m_scale.y = PSVECMag(d) / scale->y;
    } else {
        t->m_scale.y = lbl_eu_8066B158;
    }

    if (scale->z > lbl_eu_8066B15C) {
        t->m_scale.z = frontAge / scale->z;
    } else {
        t->m_scale.z = lbl_eu_8066B158;
    }

    if (scale->w > lbl_eu_8066B15C) {
        ml::CVec3 d;
        d.x = posB->x - posA->x;
        d.y = posB->y - posA->y;
        d.z = posB->z - posA->z;
        t->m_scale.w = PSVECMag(d) / scale->w;
    } else {
        t->m_scale.w = lbl_eu_8066B158;
    }
}

// ---------------------------------------------------------------------------
// func_804D807C: light attachment init
// ---------------------------------------------------------------------------
extern "C" CETrailLight* func_804D807C(CETrailLight* self, CResHolder* parent) {
    self->m_parent = parent;
    CLight* light = (CLight*)func_80494128(parent->m_res);
    self->m_light = light;
    if (light != nullptr) {
        func_804C03A0(light, 3);
        light = self->m_light;
        light->m_flags = (light->m_flags & ~0xFu) | 0xFu;
        func_804C08C8(self->m_light, 0);
    }
    return self;
}

// ---------------------------------------------------------------------------
// __dt__804D80F0: light attachment deleting destructor
// ---------------------------------------------------------------------------
extern "C" CETrailLight* __dt__804D80F0(CETrailLight* self, int deleting) {
    if (self != nullptr) {
        CLight* light = self->m_light;
        if (light != nullptr) {
            func_80494188(self->m_parent->m_res);
            self->m_light = nullptr;
        }
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// func_804D8160: drive the attached light
// ---------------------------------------------------------------------------
extern "C" void func_804D8160(CETrailLight* self, void* arg, s32 mode, const CETrailLightParam* p, f32 f1, f32 f2) {
    CLight* light = self->m_light;
    if (light == nullptr) {
        return;
    }

    if (p->m_intensity <= lbl_eu_8066B198) {
        func_804C08C8(light, 0);
        return;
    }

    if (f2 < lbl_eu_8066B19C) {
        f2 = lbl_eu_8066B19C;
    }
    if (lbl_eu_8066B1A0 < f2) {
        f2 = lbl_eu_8066B1A0;
    }

    f32 ff = (f32)mode;
    if (ff < lbl_eu_8066B1A4) {
        ff = lbl_eu_8066B1A4;
    }
    if (lbl_eu_8066B1A8 < ff) {
        ff = lbl_eu_8066B1A8;
    }
    s32 m = (s32)ff;

    func_804C08C8(light, 1);
    func_804C0454(light, arg);
    func_804C09E0(light, m, f1, f2);

    light->m_3C = lbl_eu_8066B1AC;

    struct LightColor {
        f32 r, g, b, a;
    } out;
    out.r = p->m_r * p->m_intensity;
    out.g = p->m_g * p->m_intensity;
    out.b = p->m_b * p->m_intensity;
    out.a = lbl_eu_8066B1A4;
    func_804C07F0(light, &out);
}

// ---------------------------------------------------------------------------
// func_804D82DC: snapshot the scene fog state
// ---------------------------------------------------------------------------
extern "C" void func_804D82DC(CScnRootEnv* scene) {
    lbl_eu_806659B8 = scene;
    lbl_eu_80663B38 = -1;
    lbl_eu_806659BC = 0;
    lbl_eu_80663B3C = 1;

    nw4r::g3d::ScnRoot* root = (nw4r::g3d::ScnRoot*)func_8048ECD8(lbl_eu_806659B8, nullptr, -1);
    *(FogData*)&lbl_eu_8065FCA0 = *root->GetFog(0);

    nw4r::g3d::ScnRoot* root2 = (nw4r::g3d::ScnRoot*)func_8048ECD8(lbl_eu_806659B8, &lbl_eu_8065FCA0, -1);
    *(FogData*)&lbl_eu_8065FCD0 = *root2->GetFog(0);
}

// ---------------------------------------------------------------------------
// func_804D83D0: per-frame fog restore
// ---------------------------------------------------------------------------
extern "C" void func_804D83D0(void) {
    s32 saved = lbl_eu_80663B38;
    s32 cur = lbl_eu_806659B8->m_fogEnv->m_index;

    if (saved == -1) {
        lbl_eu_8065FCA0 = lbl_eu_8065FCD0;
        lbl_eu_806659BC = 0;
        lbl_eu_80663B38 = cur;
        saved = cur;
    }

    if (saved != cur) {
        lbl_eu_8065FCA0 = lbl_eu_8065FCD0;
        lbl_eu_806659BC = 30;
        lbl_eu_80663B38 = cur;
    }

    if (lbl_eu_806659BC > 0) {
        lbl_eu_806659BC = lbl_eu_806659BC - 1;
    }
}
