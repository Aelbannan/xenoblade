// Auto-scaffolded catalog TU for monolib/src/effect/code_804CC2B8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <revolution/gx/GXLight.h>
#include <revolution/MTX.h>
#include "monolib/math/CVec3.hpp"

// Float constant 0.0f, shared across this TU
extern f32 lbl_eu_8066B0DC;

// Placer color globals and a shared material-color constant (retail C names).
extern "C" s32 lbl_eu_806659A8;
extern "C" s32 lbl_eu_806659AC;
extern "C" s32 lbl_eu_806659B0;
extern "C" s32 lbl_eu_806659B4;
extern "C" u32 lbl_eu_8066B124;

// Effect struct — partial layout; only fields accessed by matching functions
// are declared. The struct is large (~0x338 bytes).
struct EffectStruct {
    s16 field_0x00;
    s16 field_0x02;
    s16 field_0x04;
    u16 field_0x06; // flags/bitfield
    u32 field_0x08; // pointer (signalled object)
    u32 field_0x0c;
    u32 field_0x10; // also used as float via type punning
    u32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    u8 pad_0x20[0x328 - 0x20];
    void* field_0x328;
    void* field_0x32c;
    void* field_0x330;
    void* field_0x334;
    ~EffectStruct();
};

// Cross-TU destructor/free helpers used by EffectStruct::~EffectStruct().
extern "C" void func_80495E84(void* p);
extern "C" void __dt__804D80F0(void* p, int flag);
extern "C" void __dl__FPv(void* p);

EffectStruct::~EffectStruct() {
    if (field_0x32c) {
        func_80495E84(*(void**)((u8*)field_0x08 + 0x10));
        field_0x32c = 0;
    }
    if (field_0x328) {
        if (field_0x328) {
            void** vt = *(void***)((u8*)field_0x328 + 0x184);
            ((void (*)(void*, int))vt[1])(field_0x328, 1);
        }
        field_0x328 = 0;
    }
    if (field_0x330) {
        __dt__804D80F0(field_0x330, 1);
        field_0x330 = 0;
    }
    if (field_0x334) {
        __dl__FPv(field_0x334);
        field_0x334 = 0;
    }
    field_0x00 = -1;
    field_0x02 = -1;
    field_0x04 = -1;
    field_0x06 = 0;
}

// Node used by the firework/placer list helpers (targets 8-10). Overlaps the
// EffectStruct ranges, but several fields are read sign-extended (lha) here.
struct EffectNode {
    s16 field_0x00;
    s16 field_0x02;
    s16 field_0x04;
    s16 field_0x06;
    s16 field_0x08;
    s16 field_0x0a;
    s16 field_0x0c;
    u8 pad_0x0e[0x18 - 0x0e];
    u32 field_0x18;
};

// Target 6: object pointed to by a list owner (field_0x14 at 0x14).
struct EffectInfo {
    u8 pad_0x00[0x1c];
    u16 field_0x1c;
};

struct ListOwner {
    u8 pad_0x00[0x14];
    EffectInfo* field_0x14;
};

// Target 3: position/transform layout.
struct EffPos {
    u8 pad_0x00[0xac];
    Vec field_0xac;          // 0xac,0xb0,0xb4
    Vec field_0xb8;          // 0xb8,0xbc,0xc0
    u8 pad_0xc4[0x130 - 0xc4];
    Mtx field_0x130;
};

// Target 2: singly-linked node and list-control layout.
struct Node2 {
    Node2* field_0x00;       // next
    Node2* field_0x04;
};

struct Node2Control {
    u8 pad_0x00[0x04];
    Node2* field_0x04;
};

// Cross-TU firework/placer helpers (retail C-linkage symbols).
extern "C" EffectNode* func_804E0114(s32 index);
extern "C" void func_804E0098(s16 index);
extern "C" s32 func_804DFFA8(s32 index);
extern "C" u32 func_804E0104(void);
extern "C" s32 func_804CDF20(void* self, void* b, void* c, f32* out);

void func_804CD0A4(EffectStruct* self);

// Larger scene/effect layout used by targets 3, 8, 9, 10. Only fields touched
// by the reconstructed functions are declared.
struct SceneSubObj {
    u32 field_0x00;              // 0x00
    u8 pad_0x04[0xec - 0x04];
    u8* field_0xec;              // 0xec
    u8 pad_0xf0[0xf8 - 0xf0];
    u32 field_0xf8;              // 0xf8
};

struct EffectScene {
    u8 pad_0x00[0x06];
    u16 field_0x06;              // flags
    u8 pad_0x08[0x0c - 0x08];
    SceneSubObj* field_0x0c;     // pointer to a sub-object
    u8 pad_0x10[0x1c - 0x10];
    f32 field_0x1c;
    u8 pad_0x20[0xc4 - 0x20];
    Vec field_0xc4_vec;          // target 9 input vector (0xc4,0xc8,0xcc)
    u8 pad_0xd0[0xf4 - 0xd0];
    Mtx field_0xf4;              // target 8 matrix
    u8 pad_0x124[0x160 - 0x124];
    Mtx field_0x160;             // target 9 matrix (ends at 0x190)
    Vec field_0x190_vec;         // target 10 CVec3 out (0x190,0x194,0x198)
    u8 pad_0x19c[0x21c - 0x19c];
    Vec field_0x21c_vec;         // target 8 input vector
};

void func_804CC800(void* self) { *(u32*)((u8*)self + 0x0) = 0; }

void func_804CCF84(){}

void func_804CD0CC(){}

void func_804CDE50(EffectScene* self, void* p2, void* p3) {
    SceneSubObj* sub = self->field_0x0c;
    u32 cls = sub->field_0x00;
    if ((u32)(cls - 9) <= 2) return;
    u8* fo = (u8*)sub->field_0xf8;
    if (!fo) return;
    if (!*(s8*)(fo - 0xd)) return;
    Vec out;
    PSMTXMultVec(self->field_0xf4, &self->field_0x21c_vec, &out);
    Vec res;
    res.x = out.x;
    res.y = out.y;
    res.z = out.z;
    if (!func_804CDF20((void*)self, p2, p3, (f32*)&res)) return;
    f32 z = lbl_eu_8066B0DC;
    u32 fl = self->field_0x06 & (u32)~0x8000;
    self->field_0x06 = (u16)fl;
    self->field_0x1c = z;
}

void func_804CDF20(){}

void func_804CDD78(EffectScene* self, void* p2, void* p3) {
    if (self->field_0x06 & 0x0100) return;
    Vec out;
    PSMTXMultVec(self->field_0x160, &self->field_0xc4_vec, &out);
    Vec res;
    res.x = out.x;
    res.y = out.y;
    res.z = out.z;
    if (!func_804CDF20((void*)self, p2, p3, (f32*)&res)) return;
    self->field_0x06 = (u16)(self->field_0x06 | 0x300);
    u32* sub = (u32*)self->field_0x0c;
    u8* fo = (u8*)(sub[0x3b]);
    if (!fo) return;
    if (!*(s8*)(fo - 0xa)) return;
    f32 z = lbl_eu_8066B0DC;
    u32 fl = self->field_0x06 & (u32)~0x8000;
    self->field_0x06 = (u16)fl;
    self->field_0x1c = z;
}

void func_804CE264(){}

u32 func_804CE378(void* self) { return *(u32*)((u8*)self + 0x4); }

u32 func_804CE380(void* self) { return *(u32*)((u8*)self + 0x5c); }

void func_804CE388(){}

// Bounds constants for the target-10 direction-normalization guard.
extern "C" f32 lbl_eu_8066B108;
extern "C" f32 lbl_eu_8066B10C;

void func_804CE160(EffectScene* self, const ml::CVec3& a, const ml::CVec3& b) {
    ml::CVec3 delta = a - b;
    ml::CVec3 out = delta;
    // If every component already lies inside [c1, c2], the direction is safe and
    // needs no re-normalization.
    f32 c1 = lbl_eu_8066B108;
    f32 c2 = lbl_eu_8066B10C;
    if (out.x < c1 || c2 < out.x || out.y < c1 || c2 < out.y || out.z < c1 || c2 < out.z) {
        if (out.x * out.x + out.y * out.y + out.z * out.z == 0.0f) {
            out = ml::CVec3::zero;
        } else {
            PSVECNormalize((const Vec*)&out, (Vec*)&out);
        }
    }
    self->field_0x190_vec.x = out.x;
    self->field_0x190_vec.y = out.y;
    self->field_0x190_vec.z = out.z;
}

void func_804CE418(EffPos* self, Vec* out) {
    Vec src = self->field_0xac;
    out->x = src.x;
    out->y = src.y;
    out->z = src.z;
    Vec res;
    PSMTXMultVec(self->field_0x130, out, &res);
    out->x = res.x + self->field_0xb8.x;
    out->y = res.y + self->field_0xb8.y;
    out->z = res.z + self->field_0xb8.z;
}

void func_804CE4C0(){}

void func_804CE79C(){}

void func_804CE9A4(){}

void func_804CF700(){}

void func_804CFBC8(){}

// Target 6: us-804d4308 - reset two placer color globals, set channel 0 material
// color from a shared GXColor constant, then reset two more globals to -1.
void func_804D0194() {
    GXColor color;
    *(u32*)&color = lbl_eu_8066B124;
    lbl_eu_806659A8 = -1;
    lbl_eu_806659AC = -1;
    reinterpret_cast<void (*)(GXChannelID, const GXColor*)>(GXSetChanMatColor)(
        GX_COLOR0A0, &color);
    lbl_eu_806659B0 = -1;
    lbl_eu_806659B4 = -1;
}

void func_804D01E0(){}

void func_804D0AB4(){}

void func_804D189C(){}

void func_804D20EC(){}

void func_804D2690(){}

void func_804D2B60(){}

void func_804D3098(){}

void func_804D361C(){}

void func_804D3B14(){}

void func_804D3DB0(){}

// Target 5 initializer layout: 0x18 is written as a raw int here, though other
// functions read 0x18 as a float (type-punned offset).
struct EffectInit {
    s16 field_0x00;
    s16 field_0x02;
    s16 field_0x04;
    s16 field_0x06;
    s16 field_0x08;
    s16 field_0x0a;
    s16 field_0x0c;
    u8 pad_0x0e[0x10 - 0x0e];
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
};

// Target 5: us-804d80d4
void func_804D3F60(EffectInit* self) {
    self->field_0x06 = -1;
    self->field_0x08 = -1;
    self->field_0x0a = 0;
    self->field_0x00 = 0;
    self->field_0x02 = 0;
    self->field_0x04 = 0;
    self->field_0x0c = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x18 = 0;
}

// Target 10: us-804d8108 - walk the node chain by index, releasing each node,
// then clear the head list.
void func_804D3F94(EffectNode* self) {
    s32 idx = self->field_0x06;
    while (idx >= 0) {
        s32 old = idx;
        EffectNode* node = func_804E0114(idx);
        idx = node->field_0x02;
        func_804E0098((s16)old);
    }
    self->field_0x06 = -1;
    self->field_0x08 = -1;
    self->field_0x0a = 0;
    self->field_0x0c = 0;
    self->field_0x18 = 0;
}

void func_804D401C(){}

void func_804D4144(EffectNode* self) {
    if (self->field_0x0a <= 0) return;
    EffectNode* node;
    if (self->field_0x06 < 0) {
        node = 0;
    } else {
        node = func_804E0114(self->field_0x06);
        self->field_0x00 = -1;
        self->field_0x02 = self->field_0x06;
        self->field_0x04 = node->field_0x02;
        self->field_0x0c = 0;
    }
    while (node) {
        func_804CD0A4((EffectStruct*)(void*)node);
        if (self->field_0x04 >= 0) {
            node = func_804E0114(self->field_0x04);
            self->field_0x00 = self->field_0x02;
            self->field_0x02 = self->field_0x04;
            self->field_0x04 = node->field_0x02;
            self->field_0x0c += 1;
        } else {
            node = 0;
        }
    }
}

// Target 6: scan node slots for one matching the pattern in r28; return 0 if a
// candidate lacks the "fixed" bit, else 1. Flag map: 0x2000 gate, 0x8000 slot
// busy, 0x0800 already-linked.
s32 func_804D41F8(ListOwner* self, const EffectNode* target) {
    if (!(self->field_0x14->field_0x1c & 0x2000)) return 1;
    s32 result = 1;
    s32 i = 0;
    while ((s16)i < (s32)func_804E0104()) {
        EffectNode* node = func_804E0114(i);
        if (node) {
            u32 nf = node->field_0x06;
            if (nf & 0x8000) {
                if (node->field_0x04 == target->field_0x00) {
                    if (nf & 0x0800) {
                        node->field_0x04 = -1;
                    } else {
                        result = 0;
                    }
                }
            }
        }
        i++;
    }
    return result;
}

void func_804D42B8(){}

void func_804D513C(){}

void func_804D5764(){}

// Target 8: us-804d9ebc - attach node list entry by index.
EffectNode* func_804D5D48(EffectNode* self) {
    s32 idx = self->field_0x06;
    EffectNode* node = 0;
    if (idx >= 0) {
        node = func_804E0114(idx);
        self->field_0x00 = -1;
        self->field_0x02 = self->field_0x06;
        self->field_0x04 = node->field_0x02;
        self->field_0x0c = 0;
    }
    return node;
}

// Target 9: us-804d9f20 - step to next node slot.
EffectNode* func_804D5DAC(EffectNode* self) {
    s32 cur = self->field_0x04;
    EffectNode* node = 0;
    if (cur >= 0) {
        self->field_0x00 = self->field_0x02;
        self->field_0x02 = cur;
        node = func_804E0114(cur);
        self->field_0x04 = node->field_0x02;
        self->field_0x0c = self->field_0x0c + 1;
    }
    return node;
}

void func_804D5E10(EffectNode* self, s32 index) {
    s32 v = func_804DFFA8(index);
    if ((s16)v < 0) return;
    func_804E0114((s16)v);
    if (self->field_0x06 >= 0) {
        EffectNode* last = func_804E0114(self->field_0x08);
        last->field_0x02 = (s16)v;
        self->field_0x08 = (s16)v;
    } else {
        self->field_0x06 = (s16)v;
        self->field_0x08 = (s16)v;
    }
    self->field_0x0a += 1;
}

void func_804D5E90(EffectNode* self) {
    if (self->field_0x02 == self->field_0x08) {
        self->field_0x08 = self->field_0x00;
    }
    if (self->field_0x00 < 0) {
        EffectNode* node = func_804E0114(self->field_0x02);
        s16 next = node->field_0x02;
        self->field_0x04 = next;
        self->field_0x06 = next;
        func_804E0098(self->field_0x02);
        self->field_0x02 = -1;
    } else {
        EffectNode* nodeA = func_804E0114(self->field_0x02);
        EffectNode* nodeB = func_804E0114(self->field_0x00);
        nodeB->field_0x02 = nodeA->field_0x02;
        self->field_0x04 = nodeA->field_0x02;
        func_804E0098(self->field_0x02);
        self->field_0x02 = self->field_0x00;
    }
    if (self->field_0x0a > 0) self->field_0x0a -= 1;
}

void func_804D5F54(){}

void func_804D6070(void) {}

void func_804D6074(){}

s32 func_804D6BC0(void* unused, Node2Control* c, Node2** p5, Node2** p6,
               Node2** p7, Node2** p8, Node2** p9) {
    *p6 = *p5;
    *p6 = (*p5)->field_0x04;
    *p7 = *p5;
    *p8 = *p5;
    *p8 = (*p5)->field_0x00;
    *p9 = *p5;
    *p9 = (*p5)->field_0x00;
    *p9 = (*p5)->field_0x00->field_0x00;
    Node2* e = c->field_0x04;
    if (*p7 == e->field_0x00) *p6 = *p7;
    if (*p8 == e) {
        *p8 = *p7;
        *p9 = *p7;
        return 1;
    }
    if (*p9 == e) *p9 = *p8;
    return 0;
}

// Target 1: us-804d8184 — simple setter: stores two params at offsets 0x10/0x14
void func_804D4010(EffectStruct* self, u32 a, u32 b) {
    self->field_0x10 = a;
    self->field_0x14 = b;
}

// Target 2: us-804d22b4 — if field_0x1c <= 0.0f, clear bit 15 (0x8000) of field_0x06
// SOFT-CAP: MWCC optimizes wrapping mask 0xFFFF7FFF (rlwinm 17,15) to
// non-wrapping 0x00007FFF (rlwinm 17,31) because upper bits are zero after lhz.
// Semantically equivalent — same result for all inputs.
void func_804CE140(EffectStruct* self) {
    if (lbl_eu_8066B0DC < self->field_0x1c) return;
    u32 tmp = self->field_0x06;
    tmp &= ~0x8000;
    self->field_0x06 = (u16)tmp;
}

// Target 3: us-804d1218 — if bits 0x0800 and 0x8000 of field_0x06 are set, add field_0x18 to field_0x10
void func_804CD0A4(EffectStruct* self) {
    u32 flags = self->field_0x06;
    if (!((flags >> 11) & 1)) return;
    if (!((flags >> 15) & 1)) return;
    // field_0x10 is stored as u32 but used as float here
    *(f32*)&self->field_0x10 += self->field_0x18;
}

// Target 4: us-804d0414 — init: set first three fields to -1, rest to 0
void func_804CC2B8(EffectStruct* self) {
    self->field_0x00 = -1;
    self->field_0x02 = -1;
    self->field_0x04 = -1;
    self->field_0x06 = 0;
    self->field_0x328 = 0;
    self->field_0x32c = 0;
    self->field_0x330 = 0;
    self->field_0x334 = 0;
}

// Target 5: us-804d255c — if field_0x328 != 0 and field_0x18 > 0.0f, set bit 6 (0x40) of field_0x06
void func_804CE3E8(EffectStruct* self) {
    if (self->field_0x328 == 0) return;
    if (self->field_0x18 <= lbl_eu_8066B0DC) return;
    self->field_0x06 |= 0x40;
}