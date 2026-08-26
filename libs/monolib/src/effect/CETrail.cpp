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
#include "monolib/effect/CETrail.hpp"

// ---------------------------------------------------------------------------
// External vtables / constants / globals (absolute DOL addresses)
// ---------------------------------------------------------------------------
extern "C" {
extern u32 lbl_eu_8056FC40[]; // CETrail secondary interface vtable
extern u32 lbl_eu_8056FC4C[]; // CETrail vtable
extern u32 lbl_eu_8056FC64[]; // reslist<CETrail::POINT> vtable

extern const f32 lbl_eu_8066B15C;
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
extern u32 lbl_eu_8065FC18[];

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
FogState lbl_eu_8065FCA0;
FogState lbl_eu_8065FCD0;

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
extern "C" s32 lbl_eu_806659BC;
extern u8 lbl_eu_80663B3C[4];

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
// Scene-root accessor (see CScnMem.cpp); single-argument signature keeps
// MWCC's IPA register info intact so r4/r5 constants survive across the calls.
void* func_8048ECD8(void* scene);
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
    CETrailNode* cur = t->m_head->m_next;
    while (cur != t->m_head) {
        CETrailNode* old = cur;
        cur = old->m_next;
        old->m_next = nullptr;
    }
    t->m_head->m_next = t->m_head;
    t->m_head->m_prev = t->m_head;
}

static void trailClearListDerived(CETrail* t) {
    CETrailNode* head = t->m_head;
    CETrailNode* cur = head->m_next;
    while (cur != head) {
        CETrailNode* old = cur;
        cur = old->m_next;
        old->m_next = nullptr;
    }
    head->m_next = head;
    head->m_prev = head;
}
// ---------------------------------------------------------------------------
// __dt__804D6C60: CETrail constructor (retail placeholder name)
// ---------------------------------------------------------------------------
extern "C" void* __dt__804D6C60(CETrail* t, u32 count, s16 segCount, void* linkArg,
                                u8* dataA, u8* dataB, u8 flag158, u32 id15C) {
    t->m_vtable2 = (void*)lbl_eu_8056FC40;
    t->m_vtable = (void*)lbl_eu_8056FC64;

    ml::CVec3 za;
    za.x = ml::CVec3::zero.x;
    za.y = ml::CVec3::zero.y;
    za.z = ml::CVec3::zero.z;
    ml::CVec3 zb;
    zb.x = ml::CVec3::zero.x;
    zb.y = ml::CVec3::zero.y;
    zb.z = ml::CVec3::zero.z;
    t->m_startNode.m_item.m_posA = za;
    t->m_startNode.m_item.m_posB = zb;
    t->m_startNode.m_item.m_age = lbl_eu_8066B15C;
    t->m_list = nullptr;
    t->m_capacity = 0;
    t->m_ownsList = false;
    t->m_head = &t->m_startNode;
    t->m_startNode.m_next = &t->m_startNode;
    t->m_startNode.m_prev = &t->m_startNode;
    t->m_vtable = (void*)lbl_eu_8056FC4C;

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

    func_804F2A8C((char*)&t->m_link0 + 0xC);
    func_804F2A8C((char*)&t->m_link1 + 0xC);

    if ((s32)count < 2) {
        count = 2;
    }

    // MemManager handle is re-read from the global at every use (never cached).
    void* block = mtl::MemManager::allocate_array(count * 0x24 + 0x10, *(u32*)&lbl_eu_8065FC18[1]);
    t->m_list = (CETrailNode*)__construct_new_array(block, (void (*)(void*))func_804D70A0, nullptr, 0x24, count);

    for (u32 i = 0; i < count; i++) {
        t->m_list[i].m_next = nullptr;
    }
    t->m_capacity = count;

    // Inline clear: head pointer is re-read through t every iteration.
    {
        CETrailNode* cur = t->m_head->m_next;
        while (cur != t->m_head) {
            CETrailNode* old = cur;
            cur = old->m_next;
            old->m_next = nullptr;
        }
        t->m_head->m_next = t->m_head;
        t->m_head->m_prev = t->m_head;
    }

    // Nested identical null-tests survive in retail codegen.
    if (t->m_verts != nullptr) {
        if (t->m_verts != nullptr) {
            __dla__FPv((char*)t->m_verts - 16);
        }
        t->m_verts = nullptr;
    }

    if (t->m_buffer != nullptr) {
        __dla__FPv(t->m_buffer);
        t->m_buffer = nullptr;
    }

    t->m_segCount = segCount;

    if (t->m_buffer != nullptr) {
        __dla__FPv(t->m_buffer);
        t->m_buffer = nullptr;
    }

    t->m_buffer = func_804D5F54(&t->m_buffer, (s16)segCount);

    u32 need = ((u32)segCount * 0x1C) * (count * 2);
    CETrailVertex* verts;
    if (need <= mtl::MemManager::getMaxAllocSize(*(u32*)&lbl_eu_8065FC18[1])) {
        u32 n2 = segCount * count * 2;
        void* block2 = mtl::MemManager::allocate_array(n2 * 0x1C + 0x10, *(u32*)&lbl_eu_8065FC18[1]);
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
// POINT node ctor: build a zero position via the copy ctor (per-component
// float stores), assign it to m_posB, then repeat for m_posA, then the age.
extern "C" void func_804D70A0(CETrailNode* node) {
    ml::CVec3 posA, posB;
    posB.set(ml::CVec3::zero);
    posA.set(ml::CVec3::zero);
    node->m_item.m_posA = posA;
    node->m_item.m_posB = posB;
    node->m_item.m_age = lbl_eu_8066B15C;
}

// ---------------------------------------------------------------------------
// __dt___reslist_base_CETrail_POINT: reslist base deleting destructor
// ---------------------------------------------------------------------------
extern "C" CETrail* __dt___reslist_base_CETrail_POINT(CETrail* t, int deleting) {
    if (t != nullptr) {
        t->m_vtable = (void*)lbl_eu_8056FC64;
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

// Base-dtor body without the deleting call; the derived deleting dtor inlines
// this whole shape (including its own null check) in retail.
static void reslistBaseCleanup(CETrail* t) {
    if (t != nullptr) {
        t->m_vtable = (void*)lbl_eu_8056FC64;
        CETrailNode* old;
        CETrailNode* cur = t->m_head->m_next;
        while (cur != t->m_head) {
            old = cur;
            cur = old->m_next;
            old->m_next = 0;
        }
        t->m_head->m_next = t->m_head;
        t->m_head->m_prev = t->m_head;
        if (!t->m_ownsList && t->m_list != nullptr) {
            delete[] t->m_list;
            t->m_list = nullptr;
        }
    }
}

// ---------------------------------------------------------------------------
// __dt__reslist_CETrail_POINT: reslist deleting destructor
// ---------------------------------------------------------------------------
extern "C" CETrail* __dt__reslist_CETrail_POINT(CETrail* t, int deleting) {
    if (t != nullptr) {
        reslistBaseCleanup(t);
        if (deleting > 0) {
            __dl__FPv(t);
        }
    }
    return t;
}

// Derived-half cleanup, spelled as a helper: when MWCC inlines it, the
// callee's own null-test survives next to the caller's, giving retail's
// doubled `beq` after the m_capacity store.
static inline void reslistDerivedCleanup(CETrail* t) {
    // && form on a register value keeps retail's duplicated null-test branch
    if (t != nullptr && t != nullptr) {
        t->m_vtable = (void*)lbl_eu_8056FC64;
        CETrailNode* cur = t->m_head->m_next;
        while (cur != t->m_head) {
            CETrailNode* old = cur;
            cur = old->m_next;
            old->m_next = nullptr;
        }
        t->m_head->m_next = t->m_head;
        t->m_head->m_prev = t->m_head;
        if (t->m_ownsList == 0 && t->m_list != nullptr) {
            if (t->m_list != nullptr) {
                __dla__FPv((char*)t->m_list - 16);
            }
            t->m_list = nullptr;
        }
    }
}

// ---------------------------------------------------------------------------
// __dt__7CETrailFv: CETrail deleting destructor
// ---------------------------------------------------------------------------
extern "C" CETrail* __dt__7CETrailFv(CETrail* t, int deleting) {
    if (t != nullptr) {
        t->m_vtable2 = (void*)lbl_eu_8056FC40;

        if (t->m_verts != nullptr) {
            // retail re-tests the pointer before the array-deleter call
            if (t->m_verts != nullptr) {
                __dla__FPv((char*)t->m_verts - 16);
            }
            t->m_verts = nullptr;
        }
        if (t->m_buffer != nullptr) {
            __dla__FPv(t->m_buffer);
            t->m_buffer = nullptr;
        }

        trailClearList(t);
        if (t->m_ownsList == 0 && t->m_list != nullptr) {
            if (t->m_list != nullptr) {
                __dla__FPv((char*)t->m_list - 16);
            }
            t->m_list = nullptr;
        }
        t->m_capacity = 0;

        reslistDerivedCleanup(t);

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
// Builds the integer->double bit pattern (0x43300000 exponent slot) directly,
// so the only fsubs is the explicit lbl_eu_8066B180 offset like retail.
union IntToF64 {
    f64 f;
    u32 u[2];
};

extern "C" void func_804D7434(CETrail* t, s32 mode, const u8* color) {
    // Two persistent int->double bit-cast slots; retail pre-stores both
    // 0x43300000 headers once in the prologue and only rewrites the low word.
    IntToF64 convShade;
    IntToF64 convGate;
    convGate.u[0] = 0x43300000u;
    convShade.u[0] = 0x43300000u;

    // Gate: valid vertex buffer, at least two counted nodes (the rotated
    // while counts the head sentinel once), and non-zero alpha.
    bool draw;
    if (t->m_verts != nullptr) {
        CETrailNode* cur = t->m_head->m_next;
        u32 n = 0;
        CETrailNode* head = t->m_head;
        while (cur != head) {
            cur = cur->m_next;
            n++;
        }
        if (n >= 2) {
            // Alpha through the integer->double round-trip, compared > 0.
            convGate.u[1] = color[3];
            draw = (convGate.f - lbl_eu_8066B180) > lbl_eu_8066B15C;
        } else {
            draw = false;
        }
    } else {
        draw = false;
    }
    if (!draw) {
        return;
    }

    func_804D6074(&t->m_segCount, t, color, t->m_mode == 2, mode != 0, &t->m_color, &t->m_scale);

    mtl::MemManager::func_80434A4C(false);

    switch (mode) {
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
            convShade.u[1] = v->m_shade;
            WGPIPE.uc = (u8)(s32)(convShade.f - lbl_eu_8066B180);
        }
        break;
    }
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
            convShade.u[1] = v->m_shade;
            WGPIPE.uc = (u8)(s32)(convShade.f - lbl_eu_8066B180);
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
            convShade.u[1] = v->m_shade;
            WGPIPE.uc = (u8)(s32)(convShade.f - lbl_eu_8066B180);
            WGPIPE.f = t->m_texU0Scale * (t->m_texU0 + v->m_u);
            WGPIPE.f = t->m_texV0Scale * (t->m_texV0 + v->m_v);
            WGPIPE.f = t->m_texU1Scale * (t->m_texU1 + v->m_u);
            WGPIPE.f = t->m_texV1Scale * (t->m_texV1 + v->m_v);
        }
        break;
    }
    }

    mtl::MemManager::func_80434A4C(true);
}

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
    default: {
        ml::CVec3 off;
        off.x = lbl_eu_8066B15C;
        off.y = k * f;
        off.z = lbl_eu_8066B15C;
        f32 c = lbl_eu_8066B188;
        ml::CVec3 offc = off * c;
        ml::CVec3 raw1;
        PSMTXMultVec(*M, offc, raw1);
        ml::CVec3 v1 = raw1;
        ml::CVec3 raw2;
        PSMTXMultVec(*M, off, raw2);
        ml::CVec3 v2 = raw2;
        func_804D7B28(t, &v2, &v1, color, scale);
        break;
    }
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

        // Retail computes each sum/diff through the nw4r paired-single
        // VEC3Add/VEC3Sub kernels, then copies the temp into the named local.
        ml::CVec3 sum0 = pos0 + off;
        ml::CVec3 diff0 = pos0 - off;
        func_804D7B28(t, &sum0, &diff0, color, scale);

        ml::CVec3 sum1 = pos1 + off;
        ml::CVec3 diff1 = pos1 - off;
        func_804D7B28(t, &sum1, &diff1, color, scale);
        break;
    }
    }
}

// ---------------------------------------------------------------------------
// func_804D7B28: append a trail point (reslist<POINT> method shapes)
// ---------------------------------------------------------------------------
// CETrail embeds a reslist<CETrail::POINT>-shaped header, and the retail body
// is built from inlined reslist method templates. Each helper below mirrors
// the corresponding reslist.hpp member INCLUDING its local declaration order,
// because MWCC's register/slot coloring for the inlined loops follows it.

// Iterator view mirroring _reslist_iterator<POINT> (by-ref operator forms
// force MWCC to home the iterator objects to frame slots like retail).
struct TrailIterator {
    CETrailNode* m_node;
    explicit TrailIterator(CETrailNode* n) : m_node(n) {}
    TrailIterator& operator++() {
        m_node = m_node->m_next;
        return *this;
    }
    bool operator!=(const TrailIterator& rhs) const { return m_node != rhs.m_node; }
};

static u32 trailSize(CETrailNode* head) {
    u32 length = 0;

    TrailIterator first(head->m_next);
    TrailIterator last(head);

    while (first != last) {
        length++;
        ++first;
    }

    return length;
}

static void trailPopFront(CETrailNode* head) {
    CETrailNode* prevNode;
    CETrailNode* nextNode;
    CETrailNode* frontNode;

    frontNode = head->m_next;
    nextNode = frontNode->m_next;
    prevNode = frontNode->m_prev;
    prevNode->m_next = nextNode;
    nextNode->m_prev = prevNode;
    frontNode->m_next = nullptr;
}

static s32 trailFindFirstEmptySlotIndex(CETrail* t) {
    s32 i = 0;

    while (i < t->m_capacity) {
        if (t->m_list[i].m_next == nullptr) {
            break;
        }
        i++;
    }

    return i;
}

static void trailSetItem(CETrailNode* node, const CETrailPoint& value) {
    CETrailPoint* ptr = &node->m_item;
    if (ptr != nullptr) {
        try {
            *ptr = value;
        } catch (...) {
            throw;
        }
    }
}

static void trailPushBack(CETrailNode* head, CETrail* t, const CETrailPoint& item) {
    s32 i = trailFindFirstEmptySlotIndex(t);

    CETrailNode* temp = &t->m_list[i];

    trailSetItem(temp, item);
    temp->m_next = head;
    temp->m_prev = head->m_prev;
    head->m_prev->m_next = temp;
    head->m_prev = temp;
}

extern "C" void func_804D7B28(CETrail* t, const ml::CVec3* posA, const ml::CVec3* posB,
                              const ml::CVec4* color, const ml::CVec4* scale) {
    // Distance between posA and the list's first point (front(), like retail)
    f32 dist;
    if (trailSize(t->m_head) != 0) {
        ml::CVec3 d = *posA - t->m_head->m_next->m_item.m_posA;
        dist = PSVECMag(d);
    } else {
        dist = lbl_eu_8066B15C;
    }

    // Trail would not advance: drop the oldest point
    if (trailSize(t->m_head) != 0 && dist == lbl_eu_8066B15C) {
        trailPopFront(t->m_head);
        return;
    }

    // Make room when at capacity
    if (t->m_capacity <= trailSize(t->m_head)) {
        trailPopFront(t->m_head);
    }

    CETrailPoint item;
    item.m_posA = *posA;
    item.m_posB = *posB;
    item.m_age = lbl_eu_8066B15C;
    trailPushBack(t->m_head, t, item);

    // Age every point by walking back from the last node; the loop exits on
    // the front node (head->next re-read each iteration like retail).
    f32 frontAge;
    if (trailSize(t->m_head) >= 2) {
        CETrailNode* cur = t->m_head->m_prev;
        while (true) {
            cur->m_item.m_age += dist;
            if (cur == t->m_head->m_next) {
                break;
            }
            cur = cur->m_prev;
        }
        frontAge = t->m_head->m_next->m_item.m_age;
    } else {
        frontAge = lbl_eu_8066B15C;
    }

    t->m_color = *color;

    if (scale->x > lbl_eu_8066B15C) {
        t->m_scale.x = frontAge / scale->x;
    } else {
        t->m_scale.z = lbl_eu_8066B158;
        t->m_scale.x = lbl_eu_8066B158;
    }

    if (scale->y > lbl_eu_8066B15C) {
        ml::CVec3 d = *posB - *posA;
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
        ml::CVec3 d = *posB - *posA;
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
    self->m_light = (CLight*)func_80494128(parent->m_res);
    if (self->m_light == nullptr) {
        return self;
    }
    func_804C03A0(self->m_light, 3);
    self->m_light->m_flags = (self->m_light->m_flags & ~0xFu) | 0xFu;
    func_804C08C8(self->m_light, 0);
    return self;
}

// ---------------------------------------------------------------------------
// __dt__804D80F0: light attachment deleting destructor
// ---------------------------------------------------------------------------
struct CETrailLightDtor {
    CResHolder* m_parent;
    CLight* m_light;
};

// __dt__804D80F0: light attachment deleting destructor.
// Known residual: MWCC colors the m_light temp r0 in every hand-written
// variant tried; retail reuses the freed arg register r4 (2 pure reg-swaps).
extern "C" CETrailLightDtor* __dt__804D80F0(CETrailLightDtor* self, int deleting) {
    if (self != nullptr) {
        if (self->m_light != nullptr) {
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
// Light color payload built on the stack for func_804C07F0
struct LightColor {
    f32 r, g, b, a;
};

void func_804D8160(CETrailLight* self, void* arg, s32 mode, CETrailLightParam* p, f32 f1, f32 f2) {
    // NOTE: self->m_light is re-read for every call (never cached in a local)
    // so MWCC keeps it in a volatile register like retail.
    if (self->m_light == nullptr) {
        return;
    }

    if (p->m_intensity <= lbl_eu_8066B198) {
        func_804C08C8(self->m_light, 0);
        return;
    }

    // Clamp the fade parameter
    if (f2 < lbl_eu_8066B19C) {
        f2 = lbl_eu_8066B19C;
    }
    if (lbl_eu_8066B1A0 < f2) {
        f2 = lbl_eu_8066B1A0;
    }

    // Clamp the light index through a float round-trip (retail does the same);
    // the int conversion stays inline at the call site so its stack round-trip
    // lands between the C08C8 and C09E0 calls like retail.
    f32 fm = (f32)mode;
    if (fm < lbl_eu_8066B1A4) {
        fm = lbl_eu_8066B1A4;
    }
    if (lbl_eu_8066B1A8 < fm) {
        fm = lbl_eu_8066B1A8;
    }
    s32 m = (s32)fm;

    func_804C08C8(self->m_light, 1);
    func_804C0454(self->m_light, arg);
    func_804C09E0(self->m_light, m, f1, f2);

    LightColor out;
    self->m_light->m_3C = lbl_eu_8066B1AC;
    out.b = p->m_b * p->m_intensity;
    out.g = p->m_g * p->m_intensity;
    out.r = p->m_r * p->m_intensity;
    out.a = lbl_eu_8066B1A4;
    func_804C07F0(self->m_light, &out);
}

// ---------------------------------------------------------------------------
// func_804D82DC: snapshot the scene fog state
// ---------------------------------------------------------------------------
// func_804D82DC: snapshot the scene fog state
void func_804D82DC(CScnRootEnv* scene) {
    // -1 stays live in a register across both lookups; 0 is rematerialized.
    s32 invalid = -1;
    lbl_eu_806659B8 = scene;
    lbl_eu_80663B38 = invalid;
    lbl_eu_806659BC = 0;
    lbl_eu_80663B3C[0] = 1;

    nw4r::g3d::ScnRoot* root = (nw4r::g3d::ScnRoot*)func_8048ECD8(scene);
    *(FogData*)&lbl_eu_8065FCA0 = *root->GetFog(0);

    // Retail re-reads the global for the second lookup.
    nw4r::g3d::ScnRoot* root2 = (nw4r::g3d::ScnRoot*)func_8048ECD8(lbl_eu_806659B8);
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


// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.bss] 0x8065FD00-0x8065FE30 (304B): unknown zero-fill object.
__declspec(align(8)) u8 lbl_eu_8065FD00[0x130];

// [.sbss] 0x806659BC-0x806659C8 (12B): retail tail global (8B span).
extern "C" __declspec(align(4)) s32 lbl_eu_806659BC = 0;
extern "C" __declspec(align(4)) u32 lbl_eu_806659C0 = 0;
extern "C" __declspec(align(4)) u32 lbl_eu_806659C4 = 0;

// [.sdata] 0x80663B20-0x80663B40 (32B): typeinfo pairs + counters.
extern "C" u32 lbl_eu_8066B190;
extern "C" __declspec(align(8)) const char lbl_eu_80524530[0x18];
extern "C" __declspec(align(8)) const char lbl_eu_80524548[0x20];
extern "C" u32 lbl_eu_8056FC58[3];
extern "C" u32 lbl_eu_80663B20[2] = { (u32)&lbl_eu_8066B190, 0x00000000 };
extern "C" u32 lbl_eu_80663B28[2] = { (u32)&lbl_eu_80524530, (u32)&lbl_eu_8056FC58 };
extern "C" u32 lbl_eu_80663B30[2] = { (u32)&lbl_eu_80524548, 0x00000000 };
extern "C" { s32 lbl_eu_80663B38 = -1; u8 lbl_eu_80663B3C[4] = { 1, 0, 0, 0 }; }  // 3B zero tail shares the initialized word (PROGBITS .sdata)
// 3B zero tail at +0x1D (kept PROGBITS .sdata via explicit section decl)

// [.rodata] 0x80524530-0x80524568 (56B): template typeinfo names.
extern "C" __declspec(align(8)) const char lbl_eu_80524530[0x18] = { 0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x3C,0x43,0x45,0x54,0x72,0x61,0x69,0x6C,0x3A,0x3A,0x50,0x4F,0x49,0x4E,0x54,0x3E,0x00 };
extern "C" __declspec(align(8)) const char lbl_eu_80524548[0x20] = { 0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x43,0x45,0x54,0x72,0x61,0x69,0x6C,0x3A,0x3A,0x50,0x4F,0x49,0x4E,0x54,0x3E,0x00,0x00,0x00 };  // +2B tail pad

// [.data] 0x8056FC40-0x8056FC70 (48B): four 12B vtables. The reslist dtors
// are splitter-shortened template names (foreign weak definitions).
extern "C" u32 lbl_eu_8056FC40[3] = { (u32)&lbl_eu_80663B20, 0x00000000, (u32)&__dt__7CETrailFv };
extern "C" u32 lbl_eu_8056FC4C[3] = { (u32)&lbl_eu_80663B28, 0x00000000, (u32)&__dt__reslist_CETrail_POINT };
extern "C" u32 lbl_eu_8056FC58[3] = { (u32)&lbl_eu_80663B30, 0x00000000, 0x00000000 };
extern "C" u32 lbl_eu_8056FC64[3] = { (u32)&lbl_eu_80663B30, 0x00000000, (u32)&__dt___reslist_base_CETrail_POINT };
