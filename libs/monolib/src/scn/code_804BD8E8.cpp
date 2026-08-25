// Auto-scaffolded catalog TU for monolib/src/scn/code_804BD8E8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// [.bss] work areas defined at the bottom of this TU.
extern u8 lbl_eu_8065F428[];
extern u8 lbl_eu_8065F8A8[];

namespace ml {
    // Minimal view of ml::CVec3 so we can reference the shared 'zero' vector
    // (retail symbol zero__Q22ml5CVec3) without pulling in the full header,
    // whose PSVECMag declaration clashes with this TU's own declaration.
    struct CVec3 {
        f32 x;
        f32 y;
        f32 z;
    };
    extern CVec3 zero;
}

// Cross-TU imports (monolib/coli code_804B59C8). C linkage keeps the retail
// plain symbol names (MWCC would mangle C++-linkage declarations).
extern "C" u32 func_804BADA0(const char* self);
extern "C" void func_804BA7BC(const char* self, u32 a, u32 b);
extern "C" void func_804BA26C(const char* self, u32 a, u32 b, u32 c, u32 d);

// Cross-TU imports (monolib/coli collision manager entry points).
extern "C" void func_804B9E14(char* state, const f32* offset, u32 flags, int mode);
extern "C" void func_804B877C(char* state, const f32* offset, u32 flags, int mode);
extern "C" void func_804B7804(char* self);
extern "C" f32 PSVECMag(const f32* v);
void operator delete(void* ptr) throw();

extern char lbl_eu_8065F32C[];

// .sdata2 constants (defined in port/data_defs.cpp).
extern const f32 lbl_eu_8066AF44;
extern const f32 lbl_eu_8066AF4C;
extern const f64 lbl_eu_8066AF58;
extern const f32 lbl_eu_8066AF24;
extern const f32 lbl_eu_8066AF60;
extern const f32 lbl_eu_8066AF64;
extern const f32 lbl_eu_8066AF68;
extern const f32 lbl_eu_8066AF6C;
extern const f32 lbl_eu_8066AF70;
extern const f32 lbl_eu_8066AF74;
extern const f32 lbl_eu_8066AF78;
extern const f32 lbl_eu_8066AF7C;
extern const f32 lbl_eu_8066AF38;
extern const f32 lbl_eu_8066AF28;
extern const f32 lbl_eu_8066AF2C;
extern const f32 lbl_eu_8066AF3C;
extern const f32 lbl_eu_8066AF40;
extern const f32 lbl_eu_8066AF48;
extern const f32 lbl_eu_8066AF50;

// [.sbss] state shared with the scn/coli managers.
extern s32 lbl_eu_80665974;
extern s32 lbl_eu_80665978;
extern u32 lbl_eu_80665988;
extern u32 lbl_eu_8066598C;   // per-entry enable bitmask
extern u32 lbl_eu_80665970;  // pointer to the current reference position
extern f32 lbl_eu_80665980;
extern u8 lbl_eu_8066597C;
extern u8 lbl_eu_8066597D;
extern u8 lbl_eu_8066597E;
extern u32 lbl_eu_80665984;

// Defined later in this TU.
s32 func_804BF3EC(f32* v, f32* outLen, s32 flag, f32 scale);
void func_804BEEEC();

// nw4r math/db helpers (retail plain symbols).
float FrSqrt__Q24nw4r4mathFf(float);
void PSVECNormalize(const f32* src, f32* dst);
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// Object holding a pointer to the per-entry scale/pair table at 0x4C.
struct ScnTblObj {
    u8 field_0x00[0x4C];
    f32* field_0x4C;
};

struct ScnRmObj {
    u32 field_0x00;
};

// Destructor for a scene-resource holder object: clear its header word,
// reset the shared mover state and optionally free the object (the scalar
// deleting destructor passes flags > 0 when ownership is released).
void* __dt__804BD8E8(ScnRmObj* obj, s32 flags) {
    if (obj != NULL) {
        obj->field_0x00 = 0;
        func_804B7804(lbl_eu_8065F32C);
        if (flags > 0) {
            ::operator delete(obj);
        }
    }
    return obj;
}

// Begin a registration pass over the scene resources. select picks between
// the default matrix configuration pass (func_804B9E14) and the linked-
// object/default-table swapping pass (func_804B877C).
// Begin a registration pass over the scene resources.
//
// Sets up the shared crossing state (anchor position, thresholds, flags),
// quantizes the movement vector via func_804BF3EC, then walks every enabled
// resource entry (push pass + resolve pass), recording height crossings into
// the history table and finally re-resolving the ground height against the
// recorded data.
// NOTE: partial reconstruction - the tail of retail is not fully visible in
// the reference listing; control flow here follows the visible structure.
s32 func_804BD94C(ml::CVec3* pos, ml::CVec3* anchor, f32 t1, f32 t2, f32 t3,
                  f32 t4, f32 t5, s32 a8, void* a9, s32 a10) {
    // Local layout views (the shared structs are declared further down the TU).
    struct EntL {
        u8 pad0[0x0C];
        f32 field_0x0C;
        f32 field_0x10;
        f32 field_0x14;
        u8 pad1[0x24 - 0x18];
    };
    struct WorkL {
        EntL entries[32];   // lbl_eu_8065F428
        f32 anchor[3];      // lbl_eu_8065F8A8
    };
    WorkL* w = (WorkL*)lbl_eu_8065F428;
    // Retail calls this helper with the entry index; the TU-local definition
    // is declared without parameters, so go through a typed pointer.
    s32 (*recordCrossing)(s32) = (s32 (*)(s32))&func_804BEEEC;

    // Publish the anchor and thresholds into the shared state block.
    w->anchor[0] = anchor->x;
    w->anchor[1] = anchor->y;
    w->anchor[2] = anchor->z;
    lbl_eu_80665980 = t4;
    lbl_eu_80665984 = t1;
    lbl_eu_8066597C = 0;
    lbl_eu_8066597D = 0;
    lbl_eu_8066597E = 0;

    // Working copy of the current position, offset by the second threshold.
    ml::CVec3 cur;
    cur.x = pos->x;
    cur.y = pos->y;
    cur.z = pos->z;
    ml::CVec3 base;
    base.x = pos->x;
    base.y = pos->y + t2;
    base.z = pos->z;

    // Quantize the step length of the working vector.
    f32 stepLen;
    s32 steps = func_804BF3EC(&cur.x, &stepLen, a10 != 0 ? 1 : 0, t5);
    (void)steps;

    f32 dy = t1 * t1;
    f32 moveY = t1 + t5;
    const f32 zero = lbl_eu_8066AF24;
    const f32 eps = lbl_eu_8066AF28;
    s32 crossed = 0;

    // Pass 1: walk every enabled entry, accumulate crossings.
    for (s32 i = 0; i < (s32)lbl_eu_80665988; ++i) {
        EntL* e = &w->entries[i];
        e->field_0x0C += cur.x;
        e->field_0x10 += cur.y;
        e->field_0x14 += cur.z;
        e->field_0x10 += moveY;
        func_804B9E14(lbl_eu_8065F32C, &base.x, 0, 1);
        if (lbl_eu_80665988 == 0) {
            continue;
        }
        s32 count = 0;
        for (s32 j = 0; j < (s32)lbl_eu_80665988; ++j) {
            if (!((1 << j) & lbl_eu_8066598C)) {
                continue;
            }
            // Distance from the tracked point to entry j's position.
            f32 dx = base.x - w->entries[j].field_0x0C;
            f32 dyy = base.y - w->entries[j].field_0x10;
            f32 dz2 = base.z - w->entries[j].field_0x14;
            f32 distSq = dx * dx + dyy * dyy + dz2 * dz2;
            if (distSq >= dy) {
                continue;
            }
            if (distSq <= zero) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 inv = (distSq <= zero) ? zero : distSq * FrSqrt__Q24nw4r4mathFf(distSq);
            f32 scale = eps / inv;
            if (a10 != 0) {
                w->entries[j].field_0x0C += (t1 - scale) * dx;
                w->entries[j].field_0x10 += (t1 - scale) * dyy;
                w->entries[j].field_0x14 += (t1 - scale) * dz2;
                base.x += scale * dx / t1;
                base.y += scale * dyy / t1;
                base.z += scale * dz2 / t1;
            } else {
                w->entries[j].field_0x10 -= scale * dyy;
                lbl_eu_8066597D = 1;
            }
            ++count;
        }
        if (crossed == 0) {
            crossed = recordCrossing(i);
        }
    }

    // Pass 2: resolve remaining collisions against the recorded history.
    for (s32 i = 0; i < (s32)lbl_eu_80665988; ++i) {
        if (!((1 << i) & lbl_eu_8066598C)) {
            continue;
        }
        EntL* e = &w->entries[i];
        f32 dx = base.x - e->field_0x0C;
        f32 dyy = base.y - e->field_0x10;
        f32 dz2 = base.z - e->field_0x14;
        if (base.y > e->field_0x10) {
            f32 distSq = dx * dx + dyy * dyy + dz2 * dz2;
            if (distSq >= dy) {
                continue;
            }
            if (distSq <= zero) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 inv = (distSq <= zero) ? zero : distSq * FrSqrt__Q24nw4r4mathFf(distSq);
            f32 scale = eps / inv;
            f32 push = scale * (t1 - scale);
            w->entries[i].field_0x0C += push * dx;
            w->entries[i].field_0x10 += push * dyy;
            w->entries[i].field_0x14 += push * dz2;
            lbl_eu_8066597D = 1;
            if (crossed == 0) {
                crossed = recordCrossing(i);
            }
        } else {
            f32 distSq = dx * dx + dz2 * dz2;
            if (distSq >= dy) {
                continue;
            }
            if (distSq <= zero) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 inv = (distSq <= zero) ? zero : distSq * FrSqrt__Q24nw4r4mathFf(distSq);
            f32 scale = eps / inv;
            w->entries[i].field_0x10 += scale * dyy;
            lbl_eu_8066597D = 1;
        }
    }

    // Final vertical clamp: find the highest entry surface below the point.
    f32 bestY = base.y;
    s32 found = 0;
    for (s32 i = 0; i < (s32)lbl_eu_80665988; ++i) {
        EntL* e = &w->entries[i];
        if (e->field_0x10 < zero && e->field_0x10 > pos->y && e->field_0x10 < bestY) {
            bestY = e->field_0x10;
            found = 1;
        }
    }

    s32 result = crossed;
    if (found != 0 && bestY - base.y <= lbl_eu_8066AF2C * t2) {
        f32 lowest = bestY - lbl_eu_8066AF2C * t2;
        f32 hi = 0.0f;
        s32 seen = 0;
        for (s32 i = 0; i < (s32)lbl_eu_80665988; ++i) {
            EntL* e = &w->entries[i];
            if (e->field_0x10 > zero && base.y - e->field_0x10 > lowest) {
                continue;
            }
            if (e->field_0x10 < hi || seen == 0) {
                hi = e->field_0x10;
                seen = 1;
            }
            if (e->field_0x10 > t4) {
                result = 1;
            }
        }
        if (seen != 0) {
            w->anchor[1] = hi;
        }
    }
    return result;
}

// Begin a registration pass over the scene resources.
s32 func_804BE2E8(const f32* offset, u32 flags, int mode, int select) {
    if (select != 0) {
        func_804B9E14(lbl_eu_8065F32C, offset, flags, mode);
    } else {
        func_804B877C(lbl_eu_8065F32C, offset, flags, mode);
    }
    return lbl_eu_80665988;
}

void func_804BE348(){}

void func_804BE398(){}

void func_804BE3E0(u32 a1, u32 a2, u32 a3, u32 a4) {
    func_804BA26C(lbl_eu_8065F32C, a1, a2, a3, a4);
}

void func_804BE408(){}

// 12-byte header block copied out of a ScnResourceEntry.
struct ScnResHead {
    u32 field_0x00;  // 0x00
    u32 field_0x04;  // 0x04
    u32 field_0x08;  // 0x08
};

struct ScnResourceEntry {
    ScnResHead head;             // 0x00
    u8 field_0x0C[0x18 - 0x0C];  // 0x0C
    void* field_0x18;            // 0x18 - payload pointer
    u32 field_0x1C;              // 0x1C
    int value;                   // 0x20
};

// Float view of the 0x24-byte resource entry.
struct ScnEntryF {
    u8 field_0x00[0x0C];
    f32 field_0x0C;              // 0x0C
    f32 field_0x10;              // 0x10
    f32 field_0x14;              // 0x14
    u8 field_0x18[0x24 - 0x18];
};

// 12-byte position record kept in the crossing history table.
struct Rec12 {
    f32 x;
    f32 y;
    f32 z;
};

// Overlay spanning the resource table, anchor vector and history table so
// the walk below can use a single base register like retail does.
struct ScnWork {
    ScnEntryF entries[32];  // 0x000 (lbl_eu_8065F428)
    f32 anchor[3];          // 0x480 (lbl_eu_8065F8A8)
    u8 pad[0x498 - 0x48C];  // 0x48C
    Rec12 recs[32];         // 0x498 (lbl_eu_8065F8C0)
};

void func_804BE458(u32 a1, u32 a2) {
    func_804BA7BC(lbl_eu_8065F32C, a1, a2);
}

void func_804BE470(void* a1, void* a2, void* a3, void* a4, void* a5) {
    extern void func_804BA8B4(void*, void*, void*, void*, void*, void*);
    extern char lbl_eu_8065F32C[];
    func_804BA8B4(lbl_eu_8065F32C, a1, a2, a3, a4, a5);
}

u32 func_804BE4A0() {
    return func_804BADA0(lbl_eu_8065F32C);
}

u32 func_804BE4AC(void) {
    extern u32 lbl_eu_80665988;
    return lbl_eu_80665988;
}

// Copy the 12-byte header of resource entry [index] into dst.
void func_804BE4B4(ScnResHead* dst, int index) {
    extern unsigned char lbl_eu_8065F428[];
    ScnResourceEntry* entries = (ScnResourceEntry*)lbl_eu_8065F428;
    u32 head0 = entries[index].head.field_0x00;
    dst->field_0x04 = entries[index].head.field_0x04;
    dst->field_0x00 = head0;
    dst->field_0x08 = entries[index].head.field_0x08;
}

void func_804BE4E0(){}

extern "C" { extern unsigned char lbl_eu_8065F428[]; }
extern "C" void* func_804BE50C(u32 idx) { return (void*)((char*)lbl_eu_8065F428 + idx * 0x24); }

void* func_804BE520(int index) {
    extern unsigned char lbl_eu_8065F428[];
    return (void*)(lbl_eu_8065F428 + index * sizeof(ScnResourceEntry) + 0x0c);
}

extern "C" void func_804BE62C();
extern "C" void func_804BE538(void) { func_804BE62C(); }

// Fetch the scale/pair table row referenced by resource entry [index].
s32 func_804BE53C(f32* dst, s32 index) {
    s32 ofs = index * 0x24;
    const u8* info = *(const u8* const*)(lbl_eu_8065F428 + ofs + 0x18);
    u8 code = info[0x0F];
    if (code != 0) {
        s32 sel = code - 1;
        ScnTblObj* obj = (ScnTblObj*)lbl_eu_8065F32C;
        dst[0] = obj->field_0x4C[sel * 2];
        dst[1] = lbl_eu_8066AF24;
        dst[2] = obj->field_0x4C[sel * 2 + 1];
        return 1;
    }
    return 0;
}

extern "C" int func_804BEE54(u32 flags);
extern "C" int func_804BE5A0(u32 flags) { return func_804BEE54(flags); }

extern "C" void func_804BEEAC();
extern "C" void func_804BE5A4(void) { func_804BEEAC(); }

extern "C" void func_804BEDFC();
extern "C" void func_804BE5A8(void) { func_804BEDFC(); }

int func_804BE5AC() {
    return func_804BEE54(0x8000);
}

extern u8 lbl_eu_8066597C;
extern u8 lbl_eu_8066597D;
u8 func_804BE5B8() { return lbl_eu_8066597C; }

u8 func_804BE5C0() { return lbl_eu_8066597D; }

void func_804BE5C8(){}

int func_804BE604(int index) {
    extern unsigned char lbl_eu_8065F428[];
    ScnResourceEntry* entries = (ScnResourceEntry*)lbl_eu_8065F428;
    return entries[index].value != 0;
}

extern "C" void func_804BF3B4();
extern "C" void func_804BE628(void) { func_804BF3B4(); }

// Triangle descriptor referenced by the resource-entry header words.
struct ScnTriInfo {
    u8 field_0x00[0x04];
    u16 idxA;         // 0x04
    u16 field_0x06;
    u16 idxB;         // 0x08
    u8 field_0x0A[2];
    u8 idxC;          // 0x0C
    u8 idxD;          // 0x0D
    u8 idxE;          // 0x0E
    u8 field_0x0F;
    u16 vtxIdx;       // 0x10 - index (x6 bytes) into the s16 vertex buffer
};

// Entry-header pointer view: word 0x18 is the triangle info, 0x1C a matrix.
struct ScnEntryPtrs {
    u8 field_0x00[0x18];
    ScnTriInfo* tri;  // 0x18
    f32* mtx;         // 0x1C
};

// Collision-manager state object (lbl_eu_8065F32C) array pointers.
struct ScnStateObj2 {
    u8 field_0x00[0x18];
    ml::CVec3* vertsA;   // 0x18
    s16* shortVerts;     // 0x1C
    u8 field_0x20[0x24 - 0x20];
    ml::CVec3* vertsB;   // 0x24
};

void PSMTXMultVec(const f32* m, const f32* src, f32* dst);

// Project the tracked height position onto the triangle of resource entry 0,
// resolving the closest point across the three edge pairs. Returns 1 when a
// projected position was written to out, 0 when no data is active.
s32 func_804BE62C(ml::CVec3* out) {
    if (lbl_eu_80665988 == 0) {
        return 0;
    }

    ScnEntryPtrs* e = (ScnEntryPtrs*)lbl_eu_8065F428;
    ScnTriInfo* info = e->tri;
    ScnStateObj2* st = (ScnStateObj2*)lbl_eu_8065F32C;

    // Vertex 0 lives in an s16 buffer; convert via double scaling.
    const s16* sv = (const s16*)((char*)st->shortVerts + info->vtxIdx * 6);
    ml::CVec3 pos;
    pos.x = (f32)((f64)sv[0] * lbl_eu_8066AF38);
    pos.y = (f32)((f64)sv[1] * lbl_eu_8066AF38);
    pos.z = (f32)((f64)sv[2] * lbl_eu_8066AF38);

    // Optional world matrix applied to the converted point.
    ml::CVec3 xf;
    const ml::CVec3* q = &pos;
    if (e->mtx != NULL) {
        PSMTXMultVec(e->mtx, &pos.x, &xf.x);
        q = &xf;
    }

    // Pick the two projection axes from where q sits between the slab
    // constants (dominant-axis style selection).
    s32 ca, cb;
    if (q->y >= lbl_eu_8066AF3C) {
        ca = 0;
        cb = 2;
    } else if (q->y <= lbl_eu_8066AF40) {
        ca = 2;
        cb = 1;
    } else {
        ca = 0;
        cb = 1;
    }

    const ml::CVec3* va = &st->vertsA[info->idxA];
    const ml::CVec3* vb = &st->vertsA[info->idxB];
    const ml::CVec3* vc = &st->vertsB[info->idxC];
    const ml::CVec3* vd = &st->vertsB[info->idxD];
    const ml::CVec3* ve = &st->vertsB[info->idxE];

    // Component deltas of each triangle corner against q on both axes.
    f32 ax[3] = { va->x, va->y, va->z };
    f32 bx[3] = { vb->x, vb->y, vb->z };
    f32 cx[3] = { vc->x, vc->y, vc->z };
    f32 dx[3] = { vd->x, vd->y, vd->z };
    f32 ex[3] = { ve->x, ve->y, ve->z };
    f32 qx[3] = { q->x, q->y, q->z };

    f32 e0a = ax[ca] - qx[ca];
    f32 e0b = bx[ca] - qx[ca];
    f32 e1a = cx[ca] - qx[ca];
    f32 e1b = dx[ca] - qx[ca];
    f32 dotA = e0a * e1a;
    f32 dotB = e0b * e1b;

    ml::CVec3 r;
    // Edge AB region: barycentric solve along the first axis pair.
    if (dotA <= lbl_eu_8066AF44 && dotA >= lbl_eu_8066AF48) {
        f32 ex0 = (ax[cb] - qx[cb]) < 0.0f ? -(ax[cb] - qx[cb]) : (ax[cb] - qx[cb]);
        f32 ex1 = (cx[cb] - qx[cb]) < 0.0f ? -(cx[cb] - qx[cb]) : (cx[cb] - qx[cb]);
        f32 sum = ex0 + ex1;
        if (sum != lbl_eu_8066AF44) {
            f32 t = lbl_eu_8066AF4C / sum;
            f32 u = ex0 - ex0 * t;
            f32 v = ex1 - ex1 * t;
            r.x = bx[0] * u + dx[0] * v;
            r.y = bx[1] * u + dx[1] * v;
            r.z = bx[2] * u + dx[2] * v;
        } else {
            if (ex0 > ex1) {
                vc = ve;
            }
            r = *vc;
        }
        *out = r;
        return 1;
    }

    // Edge AC region.
    if (dotB <= lbl_eu_8066AF44 && dotB >= lbl_eu_8066AF48) {
        f32 ex0 = (bx[cb] - qx[cb]) < 0.0f ? -(bx[cb] - qx[cb]) : (bx[cb] - qx[cb]);
        f32 ex1 = (dx[cb] - qx[cb]) < 0.0f ? -(dx[cb] - qx[cb]) : (dx[cb] - qx[cb]);
        f32 sum = ex0 + ex1;
        if (sum != lbl_eu_8066AF44) {
            f32 t = lbl_eu_8066AF4C / sum;
            f32 u = ex0 - ex0 * t;
            f32 v = ex1 - ex1 * t;
            r.x = cx[0] * u + ve->x * v;
            r.y = cx[1] * u + ve->y * v;
            r.z = cx[2] * u + ve->z * v;
        } else {
            if (ex0 > ex1) {
                vd = vc;
            }
            r = *vd;
        }
        *out = r;
        return 1;
    }

    // Interior / third-edge fallback.
    if (cb < 2) {
        f32 ex0 = (cx[ca] - qx[ca]) < 0.0f ? -(cx[ca] - qx[ca]) : (cx[ca] - qx[ca]);
        f32 ex1 = (ex[ca] - qx[ca]) < 0.0f ? -(ex[ca] - qx[ca]) : (ex[ca] - qx[ca]);
        f32 sum = ex0 + ex1;
        if (sum != lbl_eu_8066AF44) {
            f32 t = lbl_eu_8066AF4C / sum;
            f32 u = ex0 - ex0 * t;
            f32 v = ex1 - ex1 * t;
            r.x = bx[0] * u + cx[0] * v;
            r.y = bx[1] * u + cx[1] * v;
            r.z = bx[2] * u + cx[2] * v;
        } else {
            if (ex0 > ex1) {
                vb = vd;
            }
            r = *vb;
        }
        *out = r;
        return 1;
    }

    // Final plane distance test against the transformed point.
    f32 t0 = (qx[2] - ax[2]) < 0.0f ? -(qx[2] - ax[2]) : (qx[2] - ax[2]);
    f32 t1 = (qx[1] - ax[1]) < 0.0f ? -(qx[1] - ax[1]) : (qx[1] - ax[1]);
    f32 dist = t0 + t1;
    if (dist <= lbl_eu_8066AF50) {
        if (qx[1] <= qx[2]) {
            r.x = ax[0];
            r.y = ax[1];
            r.z = ax[2];
        } else {
            r.x = bx[0];
            r.y = bx[1];
            r.z = bx[2];
        }
    } else {
        f32 t = lbl_eu_8066AF4C / dist;
        f32 u = (qx[2] - ax[2]) - (qx[2] - ax[2]) * t;
        f32 v = (qx[1] - ax[1]) - (qx[1] - ax[1]) * t;
        r.x = ax[0] * u + bx[0] * v;
        r.y = ax[1] * u + bx[1] * v;
        r.z = ax[2] * u + bx[2] * v;
    }
    *out = r;
    return 1;
}

#pragma push
#pragma auto_inline off
extern "C" void func_804BEDFC(){}
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" int func_804BEE54(u32 flags) { return 0; }
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void func_804BEEAC(){}
#pragma pop

// Dissolved monolibdata2 sdata slot: word 0 is the func_804BF274 pointer
// called by func_804BEEEC (retail loads the data word, not the address).
extern "C" u32 lbl_eu_80663AD8[2];
extern "C" u32 lbl_eu_80663AE0[2];

void func_804BEEEC() {
    ((void (*)())*(u32*)&lbl_eu_80663AD8)();
}

// Validate the height-crossing direction for resource entry [idx]: build the
// normalized offset between the tracked position and the entry position, run
// the threshold ladder, project onto the crossing-history table and flag the
// ground/anchor crossings.
s32 func_804BEEF8(s32 idx) {
    ScnWork* w = (ScnWork*)lbl_eu_8065F428;
    // Indexed from element 0 like retail (addi 0 + add addressing shape).
    ScnEntryF* e = &w->entries[0] + idx;
    // Active position: the entry position itself when the entry is disabled,
    // otherwise the (normalized) offset direction computed below.
    ml::CVec3* pos = (ml::CVec3*)&e->field_0x0C;
    ml::CVec3 dir;
    s32 enabled = 0;

    if ((1 << idx) & lbl_eu_8066598C) {
        enabled = 1;
        ml::CVec3* cur = (ml::CVec3*)lbl_eu_80665970;
        dir.x = cur->x - pos->x;
        dir.y = cur->y - pos->y;
        dir.z = cur->z - pos->z;
        if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f) {
            dir.x = 0.0f;
            dir.y = 1.0f;
            dir.z = 0.0f;
        } else {
            f32 mag2 = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
            if (mag2 == 0.0f) {
                dir = ml::zero;
            } else {
                PSVECNormalize(&dir.x, &dir.x);
            }
        }
        pos = &dir;
    }

    if (pos->y >= lbl_eu_8066AF60) {
        if (pos->y < lbl_eu_80665980) {
            if (enabled) {
                if (dir.x * dir.x + dir.z * dir.z <= lbl_eu_8066AF64) {
                    lbl_eu_8066597E = 1;
                } else {
                    lbl_eu_80665978++;
                }
            } else {
                lbl_eu_80665978++;
                if (pos->y >= lbl_eu_8066AF68) {
                    return 0;
                }
            }

            if (lbl_eu_8066597E != 0) {
                if (lbl_eu_80665974 >= 1) {
                    return 1;
                }
            } else {
                s32 n = lbl_eu_80665974;
                Rec12* slot = &w->recs[n];
                slot->x = dir.x;
                slot->z = dir.z;
                f32 mag2 = dir.x * dir.x + dir.z * dir.z;
                if (mag2 != 0.0f) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 inv = (mag2 <= 0.0f) ? lbl_eu_8066AF4C : mag2 * FrSqrt__Q24nw4r4mathFf(mag2);
                f32 scale = lbl_eu_8066AF4C / inv;
                slot->x *= scale;
                slot->z *= scale;
                // Project the new crossing against every previous one.
                if (lbl_eu_80665978 != 0) {
                    for (s32 i = 0; i < lbl_eu_80665978; ++i) {
                        if (slot->x * w->recs[i].x + slot->z * w->recs[i].z < lbl_eu_8066AF74) {
                            return 1;
                        }
                    }
                }
            }
            lbl_eu_80665974++;
        }
    }

    if (lbl_eu_8066597C == 0) {
        bool checkAnchor = false;
        if (pos->y < lbl_eu_80665980) {
            checkAnchor = true;
        } else if (enabled && !(pos->y > lbl_eu_8066AF78)) {
            checkAnchor = true;
        }
        if (checkAnchor) {
            f32 t = w->anchor[2] * pos->z + w->anchor[0] * pos->x;
            if (t < lbl_eu_8066AF44) {
                lbl_eu_8066597C = 1;
            }
        }
    }
    return 0;
}

// Record a height crossing for resource entry [index] into the history
// table and project it against previously recorded positions. Returns 1
// when the projected position dips below the active threshold.
s32 func_804BF274(s32 index) {
    ScnWork* w = (ScnWork*)lbl_eu_8065F428;
    const f32 thrMin = lbl_eu_8066AF60;
    ScnEntryF* e = (ScnEntryF*)(lbl_eu_8065F428 + index * 0x24);
    f32 y = e->field_0x10;
    if (y >= thrMin) {
        if (y >= lbl_eu_8066AF6C) {
            lbl_eu_80665978++;
        }
        // Upper-threshold flag is captured before the walk; the loop picks
        // the comparison limit based on the crossing counter.
        bool geUpper = y >= lbl_eu_8066AF70;
        s32 n = lbl_eu_80665974;
        Rec12* slot = &w->recs[n];
        // The projection math rounds the coordinates to single precision
        // while the history record keeps the raw field values.
        double xd = e->field_0x0C;
        double zd = e->field_0x14;
        slot->x = e->field_0x0C;
        slot->z = e->field_0x14;
        s32 cnt = lbl_eu_80665978;
        const f32 limHi = lbl_eu_8066AF7C;
        const f32 limLo = lbl_eu_8066AF74;
        const Rec12* it = w->recs;
        for (s32 i = n; i > 0; --i) {
            f32 v = (f32)xd * it->x + (f32)zd * it->z;
            if (cnt != 0) {
                if (v < limLo) {
                    return 1;
                }
            } else if (geUpper && v < limHi) {
                return 1;
            }
            ++it;
        }
        lbl_eu_80665974++;
    }
    if (lbl_eu_8066597C == 0 && e->field_0x10 < lbl_eu_80665980) {
        f32 t = w->anchor[2] * e->field_0x14 + w->anchor[0] * e->field_0x0C;
        if (t < lbl_eu_8066AF44) {
            lbl_eu_8066597C = 1;
        }
    }
    return 0;
}

#pragma push
#pragma auto_inline off
extern "C" void func_804BF3B4(){}
#pragma pop

// Quantize the vector length: snap v onto a grid derived from 'scale' and
// report the step count through the return value (1 = zero vector, 2 =
// sub-step clamped, else the quantized step count).
s32 func_804BF3EC(f32* v, f32* outLen, s32 flag, f32 scale) {
    s32 result = 1;
    outLen[0] = lbl_eu_8066AF4C;
    if (v[0] == lbl_eu_8066AF44 && v[1] == lbl_eu_8066AF44 && v[2] == lbl_eu_8066AF44) {
        return result;
    }
    f32 mag = PSVECMag(v);
    if (mag != lbl_eu_8066AF44) {
        s32 q = (s32)(mag / scale);
        if (q == 0) {
            if (flag != 0) {
                outLen[0] = lbl_eu_8066AF6C;
                v[0] *= lbl_eu_8066AF6C;
                v[1] *= lbl_eu_8066AF6C;
                v[2] *= lbl_eu_8066AF6C;
                result = 2;
            }
        } else {
            u32 steps = q + 1;
            if (flag != 0) {
                steps <<= 1;
            }
            f32 invMag = lbl_eu_8066AF4C / mag;
            outLen[0] = invMag;
            f32 t = mag / (f32)((f64)steps - lbl_eu_8066AF58);
            v[0] = v[0] * invMag * t;
            v[1] = v[1] * invMag * t;
            v[2] = v[2] * invMag * t;
            result = steps;
        }
    }
    return result;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804BF540(){}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// func_804BF274 is defined in this TU above (plain C++ linkage, unmangled name)
extern "C" u32 lbl_eu_8066AFC4;   // .sdata2 string

// [.sdata] 0x80663AD8-0x80663AE8 (16B)
extern "C" u32 lbl_eu_80663AD8[2] = { (u32)&func_804BF274, 0x00000000 };
extern "C" u32 lbl_eu_80663AE0[2] = { (u32)&lbl_eu_8066AFC4, 0x00000000 };

// [.bss] 0x8065F418-0x8065FA40 (0x628 = 1576B) zero-fill
u8 lbl_eu_8065F418[16];
u8 lbl_eu_8065F428[1152];
u8 lbl_eu_8065F8A8[24];
u8 lbl_eu_8065F8C0[384];

// [.sbss] 0x8066597C-0x806659A0 (36B) zero-fill. Each global <=8B stays in
// .sbss; all 4-aligned. (Retail .sbss sh_addralign is 4; MWCC emits 8 here --
// needs UNIT_RULES set_data_align=(('.sbss',4)) to fix in postprocess.)
u8 lbl_eu_8066597C;
u8 lbl_eu_8066597D;
u8 lbl_eu_8066597E;
u8 lbl_eu_8066597F;
f32 lbl_eu_80665980;
u32 lbl_eu_80665984;
u32 lbl_eu_80665988;
u32 lbl_eu_8066598C;
u32 lbl_eu_80665990;
u32 lbl_eu_80665994;
u32 lbl_eu_80665998;
u32 lbl_eu_8066599C;
