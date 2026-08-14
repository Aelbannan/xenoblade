// Auto-scaffolded catalog TU for kyoshin/cf/CfCamEvent_1
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include <monolib/math/Random.hpp>
#include <math.h>

#include "kyoshin/cf/CfCamEvent_1.hpp"

// bdat column byte read: the (u8) truncation of the u32 column value, which
// MWCC lowers through a stack temp (stw + lbz) when inlined via a helper
// (same shape as CfGimmickElv.cpp's getCol8/getCol16 helpers).
static inline u8 getCol8(void* table, const char* col, int row) {
    return (u8)getBdatStringColumnValue(table, col, row);
}

// Advance one frame of a shake-table object. Returns whether the table was
// still "active" at entry. Each element has an id (used as a time point) and
// a waveform type (c: 1 = bell, 2 = bump, 4 = sine, 3 = clamp-sine); the
// table carries a running value that is interpolated between element ids.
// The same code serves the manager's three shake tables (0x7C / 0x1F4 /
// 0x36C) through the shared CfCamShakeState view layout.
int func_80074F4C(CfCamShakeState* self, int mode) {
    u8 active = self->flag_active;
    self->flag_finish = 0;
    int result = (active != 0) ? 1 : 0;

    // Without an active flag we only proceed if forced and there is data.
    if (!active && (mode == 0 || self->count <= 0))
        goto done;

    {
    int r26 = 0;
    if (lbl_eu_8066641C == self->val) {
        self->flag_finish = 1;
        r26 = 1;
    }

    // Advance the running value by a frame step.
    f32 delta;
    if (CfRes_getD80Flag()) {
        CfRes_getD80Flag();
        delta = func_80496288();
    } else {
        delta = lbl_eu_80666428;
    }

    if (self->flag_active)
        self->val += delta;

    // Hand off to the next element once its id point is reached.
    int nxt = self->index + 1;
    if (nxt < self->count && self->val >= (f32)self->elems[nxt].id) {
        self->flag_finish = 1;
        self->index = (s16)nxt;
    }

    // Snap to the end value and clear the run when it is exceeded.
    if (self->val >= self->end - lbl_eu_8066642C) {
        self->val = self->end;
        self->flag_active = 0;
        if (r26 == 0)
            self->flag_finish = 0;
    }

    // Finished: copy the trailing element's origin into the base anchor.
    // (Retail returns the freshly-computed bool of the reloaded flag here,
    // which is 0 - distinct from the entry `result`. The first element word
    // is loaded with the indexed form, then the pointer is materialised for
    // the remaining two; stores go baseY first, then baseX, then baseZ.)
    u8 act = self->flag_active;
    if (act == 0) {
        int ret = act != 0;
        CfCamShakeElemRaw* e = &self->elems[self->count - 1];
        self->baseY = e->x4;
        self->baseX = e->x0;
        self->baseZ = e->x8;
        return ret;
    }

    // Active run: interpolate between the current element and its neighbors,
    // then shape the result with the waveform selected by the element type.
    // The element pointers are cached up front (retail keeps &elems[cur] /
    // &elems[mid] live across the waveform calls in saved registers).
    int cur = self->index;
    int prev = (cur == 0) ? 0 : (cur - 1);
    int mid = cur + 1;
    int next = (cur < self->count - 2) ? (cur + 2) : (cur + 1);
    CfCamEventElem* eCur = (CfCamEventElem*)&self->elems[cur];
    CfCamEventElem* eMid = (CfCamEventElem*)&self->elems[mid];

    f32 t = lbl_eu_8066641C;
    f32 span = (f32)(s32)(eMid->id - eCur->id);
    if (span > lbl_eu_8066641C)
        t = (self->val - (f32)eCur->id) / span;

    // Shape the interpolation parameter with the selected waveform. Retail
    // recomputes the element address for the `c` read (mulli/add/lhz) and
    // emits the case dispatch with an explicit default branch to the join.
    u16 kind = self->elems[cur].c;
    switch (kind) {
    case 1:
        t = lbl_eu_80666428 +
            SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 *
                (lbl_eu_8066A200 * t - lbl_eu_8066A200));
        break;
    case 2:
        t = lbl_eu_80666418 *
            (lbl_eu_80666428 +
             SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 *
                 (lbl_eu_8066A1F8 * t - lbl_eu_8066A200)));
        break;
    case 4:
        t = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 *
            (lbl_eu_8066A200 * t));
        break;
    case 3:
        if (t > lbl_eu_80666418)
            t = lbl_eu_80666428 - lbl_eu_80666418 *
                SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 *
                    (lbl_eu_8066A1F8 * t));
        else
            t = lbl_eu_80666418 *
                SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 *
                    (lbl_eu_8066A1F8 * t));
        break;
    default:
        break;
    }

    // Fill the base anchor using the selected spline method. The shaped
    // interpolation parameter is passed as the trailing float argument
    // (retail keeps it in f1 across the waveform dispatch).
    if (self->field_0x160 == 1) {
        func_800A41BC((f32*)&self->baseX, (CfCamEventElem*)&self->elems[prev],
                      (CfCamEventElem*)eCur, (CfCamEventElem*)eMid,
                      (CfCamEventElem*)&self->elems[next], t);
    } else if (self->field_0x160 == 0 || self->field_0x160 == 2) {
        func_800A40E8((f32*)&self->baseX, (CfCamEventElem*)eCur,
                      (CfCamEventElem*)eMid, t);
    }
    }

done:
    return result;
}
void func_80082088__Q22cf13CfGameManagerFv(){}
void func_80082060__Q22cf13CfGameManagerFv(){}
void func_80081E90__Q22cf13CfGameManagerFv(int a, int b, int c){}
extern "C" void func_800A3F8C(void*){}
void func_800B24B0(){}

// CfCamEventManager constructor. Clears the field/flag words, walks the two
// 0x178-byte shake units clearing their shared tail fields, then installs
// this as the global cam state, zeroes the 3 slot pointers via memset and
// seeds the camera vectors from CVec3::zero. Finally allocates the 0xC40
// byte slot block (8 elements of 0x188) and stores it at 0x38.
CfCamEventManager::CfCamEventManager() {
    field_0x38 = 0;
    field_0x3C = 0;
    field_0x3E = 0;
    field_0x40 = 0;
    field_0x47 = 0;
    field_0x48 = 0;
    field_0x4C = 0;
    field_0x50 = 0;
    field_0x1DE = 0;
    field_0x1E0 = 0;
    field_0x1E2 = 0;
    field_0x1E4 = 0;
    for (CfCamEventShakeUnit* u = shake; u < &shake[2]; u++) {
        u->field_0x162 = 0;
        u->field_0x164 = 0;
        u->field_0x166 = 0;
        u->field_0x168 = 0;
    }
    lbl_eu_80663DF0 = (CfCamEventGlobal*)this;
    memset(this, 0, 0xc);
    field_0x54 = ml::CVec3::zero;
    field_0x60 = ml::CVec3::zero;
    field_0x6C = ml::CVec3::zero;
    field_0x78 = lbl_eu_80666440;
    field_0x42 = 0;
    field_0x44 = 0;
    field_0x46 = 0;
    void* mem = mtl::MemManager::allocate(0xc40, func_80061FFC());
    if (mem != 0) {
        u8* p = (u8*)mem;
        u8* end = p + 0xc40;
        do {
            func_80240878(p);
            p += 0x188;
        } while (p < end);
        func_80240A64((u8*)mem);
    }
    field_0x38 = (u32)mem;
}

// CfCamEventManager destructor. Releases the three effect slots (virtual
// deleting-dtor dispatch), frees the 0x38 slot block, clears the global cam
// state pointer; MWCC emits the `if (this)` guard and the delete-flag tail
// (operator delete(this)) around this body.
CfCamEventManager::~CfCamEventManager() {
    int i = 0;
    CfCamEventSlot** walk = slots;
    do {
        CfCamEventSlot* p = *walk;
        if (p) {
            if (p) {
                delete p;
                *walk = 0;
            }
            *walk = 0;
        }
        i++;
        walk++;
    } while (i < 3);
    if (field_0x38 != 0) {
        operator delete((void*)field_0x38);
        field_0x38 = 0;
    }
    lbl_eu_80663DF0 = 0;
}

// Release all three effect slots: virtual-delete any non-null slot and clear
// the pointer. The redundant second pointer test mirrors MWCC's `delete`
// expansion (retail emits three beq against one cmpwi).
void func_800754C0(CfCamEventManager* self) {
    for (int i = 0; i < 3; i++) {
        CfCamEventSlot* p = self->slots[i];
        if (p) {
            if (p) {
                delete p;
                self->slots[i] = 0;
            }
            self->slots[i] = 0;
        }
    }
}

void func_80075540(CfCamEventManager* self, u32 idx) {
    u32 n = func_800755BC(self, idx);
    CfCamEventSlot* p = self->slots[n];
    if (p) {
        if (p) {
            delete p;
            self->slots[n] = 0;
        }
        self->slots[n] = 0;
    }
}

void* func_800755B0(void* self, unsigned long idx) {
    return *(void**)((char*)self + (idx << 2));
}

// Categorise an environment/prompt id (r4) into a 0/1/2 bucket. The
// manager pointer in r3 is unused by this helper. extern "C" keeps the
// call reloc name unmangled (retail symbol func_800755BC).
// NOTE: retail lays the four result blocks at the end (checks fall through
// with jump-on-true); Wii/1.1 -O4,p inlines them with jump-on-false, and a
// goto/switch form that reproduces the end layout instead normalizes the
// == 0xb equality into a setnz chain (same version wall as func_8007560C's
// two-cmpi range check).
extern "C" int __declspec(noinline) func_800755BC(CfCamEventManager* /*unused*/, u32 idx) {
    int result = 0;
    if (idx >= 0x10 && idx <= 0x2b) result = 1;
    else if (idx >= 0x8 && idx <= 0xa) result = 2;
    else if (idx <= 1) result = 0;
    else if ((s32)idx == 0xb) result = 2;
    return result;
}

// True when the global cam state exists and is in a "busy" frame range.
// NOTE: canonical shape for the [0x10, 0x2b] two-compare range check
// (cf. code_800B06A4.cpp func_800B67CC). Under the TU default Wii/1.1 MWCC
// folds the range into (u16)(v-0x10)<=0x1b (subi/rlwinm/cmpli); the two-cmpi
// retail form only matches under mw_version="GC/3.0a5.2" (configure.py switch
// pending, read-only this session).
int func_8007560C() {
    CfCamEventGlobal* g = lbl_eu_80663DF0;
    if (g == nullptr) return 0;
    s16 v = g->field_0x3E;
    int result = 0;
    if (v >= 0x10) {
        if (v <= 0x2b) {
            result = 1;
        }
    }
    return result;
}

// True when the global cam state exists and is in the "intro" frame range.
// Same range-check wall as func_8007560C (needs GC/3.0a5.2 for the two-cmpi form).
int func_80075640() {
    CfCamEventGlobal* g = lbl_eu_80663DF0;
    if (g == nullptr) return 0;
    s16 v = g->field_0x3E;
    int result = 0;
    if (v >= 0) {
        if (v <= 1) {
            result = 1;
        }
    }
    return result;
}

// Copy the camera-data block (three f32 triplets at 0x10C/0x118/0x138 and
// the f32 at 0x1E0) from the source object into the manager's 0x54..0x78
// range. Temporaries reproduce retail's load-ascending / store-descending
// schedule (load pair, store high first, then the single).
void func_80075674(CfCamEventManager* dst, CfCamEventCopySrc* src) {
    if (src == 0) return;
    f32 a = src->f_10C;
    f32 b = src->f_110;
    dst->field_0x54.y = b;
    dst->field_0x54.x = a;
    dst->field_0x54.z = src->f_114;
    f32 c = src->f_138;
    f32 d = src->f_13C;
    dst->field_0x60.y = d;
    dst->field_0x60.x = c;
    dst->field_0x60.z = src->f_140;
    f32 e = src->f_118;
    f32 g = src->f_11C;
    dst->field_0x6C.y = g;
    dst->field_0x6C.x = e;
    dst->field_0x6C.z = src->f_120;
    dst->field_0x78 = src->f_1E0;
}

// Camera aim/position computation. Fills `out` (representing the camera
// aim point) from the source's body/virtual accessors, applying a vertical
// follow/lag that approaches the source's anchor value. Rows mirror the
// retail branch structure.
void func_800756D0(ml::CVec3* out, CinemCamSrc* src) {
    if (src == nullptr) {
        *out = ml::CVec3::zero;
        return;
    }

    CinemAimBody* body = src->vtable->fn_0x12C(src, 100);
    CinemAimBody* anchor = src->vtable->fn_0x128(src);
    if (body != nullptr) {
        out->x = body->x;
        out->y = body->y;
        out->z = body->z;

        if (anchor != nullptr && (src->field_0x64 & 0x4) &&
            func_800AA300(5, 4, 1) == src->field_0x70) {
            f32 d = anchor->y - src->vtable->fn_0xAC(src)->v.y;
            if (d > 0.0f)
                out->y += d;
        }
    }

    CinemVecOut* vo = src->vtable->fn_0xAC(src);
    if (out->y > vo->v.y) {
        if (src->field_0x64 & 0x2) {
            out->y -= 0.25f;
        } else {
            f32 rel = out->y - vo->v.y;
            if (rel >= 1.5f)
                out->y = 0.8f * rel + vo->v.y;
            else
                out->y = out->y - 0.25f;
        }
    } else {
        // Follow: snap to anchor then add the follow offset.
        out->x = vo->v.x;
        out->y = vo->v.y;
        out->z = vo->v.z;
        out->y += 1.5f;
    }
}

// Camera depth/pose solver. Computes two aim vectors from a pair of camera
// sources; b == null skips the paired branch. The retail body is very large
// (paired-single vector math); this is a faithful structural reconstruction.
void func_80075934(ml::CVec3* out1, ml::CVec3* out2, CamCamSrc* a, CamCamSrc* b,
                   ml::CVec3* v1, ml::CVec3* v2, u16 c1, u16 c2, u8 s0, u8 s1) {
    f32 f31 = a->vtable->fn_0x5B4(a);
    if (c1 == 4) {
        out1->set(*v1);
        out2->set(*v2);
        return;
    }

    ml::CVec3 aim1;
    func_800756D0(&aim1, a ? (CinemCamSrc*)&a->voice : (CinemCamSrc*)a);
    ml::CVec3 aim2;
    if (b != 0) {
        ml::CVec3 aimb;
        func_800756D0(&aimb, (CinemCamSrc*)&b->voice);
        CinemVecOut* va = a->voice.vtable->fn_0xAC(&a->voice);
        CinemVecOut* vb = b->voice.vtable->fn_0xAC(&b->voice);
        f32 mul = lbl_eu_80666454 *
            Atan2FIdx__Q24nw4r4mathFff(vb->v.y - va->v.y, vb->v.x - va->v.x);
        f31 *= mul;
        aim1.set(aim1.x + aimb.x, aim1.y + aimb.y, aim1.z + aimb.z);
    }
    out1->set(aim1.x, aim1.y, aim1.z);
    out2->set(f31, aim2.y, aim2.z);
}


// Per-frame camera-event advance. Early-out when inactive; otherwise snapshots
// the global cam table's four aim vectors and forwards them (plus the two
// bit flags from field_0x48) into the shared pose solver.
// Fill a table-element pair from the two aim vectors. When the demo flag
// (global cam table h00) is set, dst1 receives the raw `o2` words and dst2
// receives the yaw/pitch pair derived from the (o1 - o2) difference, with
// the difference magnitude snapped to 1.0 when all components are below the
// epsilon threshold. Otherwise both targets are plain word copies. The
// subtraction is written component-wise so MWCC emits the paired-single
// sub sequence (same shape as the retail bodies).
static inline void camEventAnglePair(const ml::CVec3* o1, const ml::CVec3* o2,
                                     CfCamEventElem* dst1, CfCamEventElem* dst2) {
    if (lbl_eu_80570C90.h00 != 0) {
        ml::CVec3 d;
        d.x = o1->x - o2->x;
        d.y = o1->y - o2->y;
        d.z = o1->z - o2->z;
        dst1->x0 = o2->x;
        dst1->x4 = o2->y;
        dst1->x8 = o2->z;
        f32 len = (f32)sqrt((f64)(d.z * d.z + d.x * d.x));
        dst2->x0 = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.y, len);
        dst2->x4 = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.z, d.x);
        dst2->x8 = lbl_eu_8066641C;
        func_800A3F8C(dst2);
        // The difference is address-taken (PSVECMag), so MWCC reloads its
        // components from the stack after the call - no FPR saves needed.
        if ((f32)__fabs((f64)d.x) <= lbl_eu_8066A208 &&
            (f32)__fabs((f64)d.y) <= lbl_eu_8066A208 &&
            (f32)__fabs((f64)d.z) <= lbl_eu_8066A208)
            dst2->x8 = lbl_eu_8066641C;
        else
            dst2->x8 = PSVECMag(d);
    } else {
        dst1->x0 = o1->x;
        dst1->x4 = o1->y;
        dst1->x8 = o1->z;
        dst2->x0 = o2->x;
        dst2->x4 = o2->y;
        dst2->x8 = o2->z;
    }
}

// Per-frame camera-event advance. Snapshot the global cam table's four aim
// vectors, scan tab0 for the first element whose marker word is set, then
// run the shared pose solver over the matching cam pair and reshape the
// table elements into aim angles. Returns 1 when active.
int func_800762A0(CfCamEventManager* self) {
    if ((self->field_0x48 & 0x9) == 0) return 1;

    ml::CVec3 v[4];
    v[0].set(lbl_eu_80570C90.f1C, lbl_eu_80570C90.f20, lbl_eu_80570C90.f24);
    v[1].set(lbl_eu_80570C90.f28, lbl_eu_80570C90.f2C, lbl_eu_80570C90.f30);
    v[2].set(lbl_eu_80570C90.f34, lbl_eu_80570C90.f38, lbl_eu_80570C90.f3C);
    v[3].set(lbl_eu_80570C90.f40, lbl_eu_80570C90.f44, lbl_eu_80570C90.f48);
    u16 h0A = lbl_eu_80570C90.h0A;
    u16 h0C = lbl_eu_80570C90.h0C;
    u16 h0E = lbl_eu_80570C90.h0E;
    u16 h10 = lbl_eu_80570C90.h10;

    // Scan for the first tab0 element with a nonzero marker word.
    int idx = 0;
    for (int i = 0; i < self->field_0x1E2; i++) {
        if (self->tab0.elems[i].d != 0) break;
        idx++;
    }

    void* hA = func_800B708C__Fi((int)self->field_0x10);
    void* hB = func_800B708C__Fi((int)self->field_0x24);
    void* sa = func_8016FE34(hA);
    void* sb = func_8016FE34(hB);
    if (hA == 0 || hB == 0) return 0;

    ml::CVec3 o1, o2;
    if (idx == self->field_0x1E2) {
        // No marked element: rework the last one.
        idx--;
        if (idx < 0) return 1;
        func_80075934(&o1, &o2, (CamCamSrc*)sa, (CamCamSrc*)sb, &v[2], &v[3],
                      h0E, h10,
                      (u8)((self->field_0x48 & 2) != 0),
                      (u8)((self->field_0x48 & 0x10000) != 0));
        camEventAnglePair(&o1, &o2, &self->tab0.elems[idx],
                          &self->shake[0].u.elems[idx]);
        return 1;
    }

    if (self->field_0x48 & 0x1) {
        func_80075934(&o1, &o2, (CamCamSrc*)sa, (CamCamSrc*)sb, &v[0], &v[1],
                      h0A, h0C,
                      (u8)((self->field_0x48 & 2) != 0),
                      (u8)((self->field_0x48 & 0x10000) != 0));
        camEventAnglePair(&o1, &o2, &self->tab0.elems[idx],
                          &self->shake[0].u.elems[idx]);
        idx++;
    }
    func_80075934(&o1, &o2, (CamCamSrc*)sa, (CamCamSrc*)sb, &v[2], &v[3],
                  h0E, h10,
                  (u8)((self->field_0x48 & 2) != 0),
                  (u8)((self->field_0x48 & 0x10000) != 0));
    camEventAnglePair(&o1, &o2, &self->tab0.elems[idx],
                      &self->shake[0].u.elems[idx]);
    idx++;
    if (self->field_0x48 & 0x40) {
        CfCamEventSlotObj* s0 = (CfCamEventSlotObj*)self->slots[0];
        ml::CVec3* va = s0->vtable->fn_0x34(s0);
        ml::CVec3* vb = s0->vtable->fn_0x10(s0);
        camEventAnglePair(vb, va, &self->tab0.elems[idx],
                          &self->shake[0].u.elems[idx]);
    }

    // Shake the follow elements toward their predecessors when the demo
    // flag is up and the unit's own flag is armed.
    if (lbl_eu_80570C90.h00 != 0 && self->shake[0].field_0x163 != 0) {
        s16 count = self->shake[0].field_0x166;
        CfCamEventElem* e = self->shake[0].u.elems + 1;
        for (int i = 1; i < count; i++, e++) {
            f32 cur = e->x4;
            f32 prev = (e - 1)->x4;
            f32 d = cur - prev;
            if ((f32)__fabs((f64)d) >= lbl_eu_8066A1F8) {
                if (d > lbl_eu_8066641C)
                    e->x4 = cur - lbl_eu_8066A1FC;
                else
                    e->x4 = cur + lbl_eu_8066A1FC;
            }
        }
    }
    return 1;
}


// Bitmask capability check: returns 1 iff 'flags' grants the given capability.
int func_80076C08(int type_, int state, int flags) {
    if (flags == 0) return 1;
    if ((flags & 1) && type_ == 0 && state != 0) return 1;
    if ((flags & 2) && type_ == 0 && state == 0) return 1;
    if ((flags & 4) && (type_ & 1)) return 1;
    if ((flags & 8) && (type_ & 2)) return 1;
    if ((flags & 16) && (type_ & 3)) return 1;
    if ((flags & 32) && (type_ == 4 || (type_ == 6 || type_ == 7))) return 1;
    if ((flags & 64) && type_ == 5) return 1;
    return 0;
}

// Returns 1 when 'flags' bit n pairs with 'type_' == n+1.
int func_80076CE4(int type_, int flags) {
    if (flags == 0) return 1;
    if ((flags & 1) && type_ == 1) return 1;
    if ((flags & 2) && type_ == 2) return 1;
    if ((flags & 4) && type_ == 3) return 1;
    if ((flags & 8) && type_ == 4) return 1;
    if ((flags & 16) && type_ == 5) return 1;
    if ((flags & 32) && type_ == 6) return 1;
    return 0;
}

// Weighted-random row picker over the current bdat table (type_ 0x2b forces
// an early 0). Reads the cam source's body state (or defaults when the source
// equals `other`), scans table rows whose type column matches type_, checks
// two capability masks, then walks the runtime-named columns 1..4 subtracting
// each column's byte weight from a rand(100) draw until one is hit. Stores the
// winning row and column byte into *outRow / *outCol and returns 1.
int func_80076D8C(int unused, int type_, CamEventSrc* src, CamEventTargetInfo* other,
                  u32* outRow, u32* outCol) {
    func_8003AA34();
    void* g = lbl_eu_80664164;
    *outRow = 0;
    *outCol = 0;
    if (type_ == 0x2b) return 0;

    CamEventBody* body = src->vtable->fn_0x298(src);
    int state = 1;
    u16 typeB = 0;
    u32 typeA = 2;
    if ((const void*)src != (const void*)other) {
        u32 v = body->field_0x50;
        if (v == 0) return 0;
        state = (body->field_0x60 <= lbl_eu_8066645C) ? 1 : 0;
        typeB = body->field_0x5E;
        typeA = other->field_0x15E4;
    }

    int rowBase = (int)func_8003B41C(g);
    int rowEnd = rowBase + (int)func_8003B1EC(g);
    char* buf2 = (char*)lbl_eu_80527638;    // inner-loop column buffer
    const char* colBase = (const char*)lbl_eu_804FB5D0; // base of the type-column names
    u8 c0, c1, c2, w, w2;                   // per-row column bytes
    for (int row = rowBase; row < rowEnd; row++) {
        c0 = getCol8(g, (const char*)lbl_eu_804FB5D0, row);
        if (type_ != c0) continue;
        c1 = getCol8(g, colBase + 8, row);
        if (func_80076CE4((int)typeA, c1) == 0) continue;
        c2 = getCol8(g, colBase + 0x13, row);
        if (func_80076C08((int)typeB, state, c2) == 0) continue;

        u32 rem = (u32)ml::math::mtRand(100);
        for (int i = 1; i <= 4; i++) {
            u8 digit = (u8)(i + 0x30);
            lbl_eu_80661BB8[4] = digit;
            buf2[8] = digit;
            w = getCol8(g, (const char*)lbl_eu_80661BB8, row);
            if (rem < w) {
                w2 = getCol8(g, buf2, row);
                *outCol = w2;
                *outRow = row;
                return 1;
            }
            rem -= w;
        }
    }
    return 0;
}

cf::CActorParam_UnkStruct1* cf::CActorParam::CActorParam_UnkVirtualFunc129() {
    return (cf::CActorParam_UnkStruct1*)((u8*)this + *(u8*)((u8*)this + 0x3354) * 0xbc + 0x2a84);
}

// Set up the follow-cam state for one frame. Resolves the source object's
// voice and follow target, copies the cam table into the global, resets the
// manager's shake groups, and feeds the two aim pairs into the shared pose
// solver (func_80075934 / func_80078D08). The shake-table tail duplicates
// func_80079B34's busy/flag logic. Returns 1 on success.
extern "C" void* func_80076F88(CfCamEventManager* self, int unk34,
                                void* srcArg, CfCamDataTable* cam) {
    CamEventSrc* src = (CamEventSrc*)func_8016FE34(srcArg);
    u32 field_0x3F10 = (u32)src->field_0x3F10;
    CamEventBody* body = src->vtable->fn_0x298(src);
    void* p31 = body->field_0x04;
    void* r29 = (src != 0) ? (void*)&src->voice : (void*)src;
    if (p31 == 0) {
        r29 = func_800BBC0C(func_800B708C__Fi((int)field_0x3F10));
    }
    void* r28 = func_8016FE34((src != 0) ? (void*)&src->voice : (void*)src);
    int r27 = (int)func_8016FE34(r29);

    // "Band" check: any of the four cam halfwords selects waveform 2/3.
    int band = 0;
    if (cam->h0A == 2 || cam->h0A == 3) band = 1;
    if (cam->h0C == 2 || cam->h0C == 3) band = 1;
    if (cam->h0E == 2 || cam->h0E == 3) band = 1;
    if ((u16)(cam->h10 + 0xFFFE) <= 1) band = 1;

    if (band && p31 == 0) {
        void* v = src->voice.vtable->fn_0x4C(&src->voice);
        r29 = func_800BBC0C(func_800B708C__Fi((int)v));
        if (unk34 == 0x2B) {
            void* p0 = getPlayer__Q22cf13CfGameManagerFi(0);
            if (p0 == 0) {
                r29 = 0;
            } else {
                r29 = 0;
                void* q = ((CamEventVoice*)p0)->vtable->fn_0x4C(p0);
                if (q != 0) r29 = func_800BBC0C(func_800B708C__Fi((int)q));
                if (r29 == 0) r29 = getPlayer__Q22cf13CfGameManagerFi(1);
            }
            self->field_0x34 = (u32)r29;
        }
        if (r29 == 0) return 0;
        r27 = (int)func_8016FE34(r29);
    }

    // Copy the cam table into the global (pair loads / store-descend).
    lbl_eu_80570C90.h00 = cam->h00;
    lbl_eu_80570C90.h02 = cam->h02;
    lbl_eu_80570C90.h04 = cam->h04;
    lbl_eu_80570C90.h06 = cam->h06;
    lbl_eu_80570C90.h08 = cam->h08;
    lbl_eu_80570C90.h0A = cam->h0A;
    lbl_eu_80570C90.h0C = cam->h0C;
    lbl_eu_80570C90.h0E = cam->h0E;
    lbl_eu_80570C90.h10 = cam->h10;
    lbl_eu_80570C90.f14 = cam->f14;
    lbl_eu_80570C90.f18 = cam->f18;
    f32 q1 = cam->f1C;
    f32 q2 = cam->f20;
    lbl_eu_80570C90.f20 = q2;
    lbl_eu_80570C90.f1C = q1;
    lbl_eu_80570C90.f24 = cam->f24;
    f32 q3 = cam->f28;
    f32 q4 = cam->f2C;
    lbl_eu_80570C90.f2C = q4;
    lbl_eu_80570C90.f28 = q3;
    lbl_eu_80570C90.f30 = cam->f30;
    f32 q5 = cam->f34;
    f32 q6 = cam->f38;
    lbl_eu_80570C90.f38 = q6;
    lbl_eu_80570C90.f34 = q5;
    lbl_eu_80570C90.f3C = cam->f3C;
    f32 q7 = cam->f40;
    f32 q8 = cam->f44;
    lbl_eu_80570C90.f44 = q8;
    lbl_eu_80570C90.f40 = q7;
    lbl_eu_80570C90.f48 = cam->f48;

    void* r31 = (src != 0) ? (void*)&src->voice : (void*)src;

    // Reset the manager's three shake groups (id 1, default speeds).
    {
        u32 flags = self->field_0x50 & 0xFFFEFFFFu;
        self->field_0x40 = 0;
        self->field_0x50 = flags;
        self->field_0x1DC = 1;
        self->field_0x1E2 = 0;
        self->field_0x1E4 = 0;
        self->field_0x1E8 = lbl_eu_80666418;
        self->field_0x1EC = lbl_eu_8066641C;
        self->field_0x1F0 = lbl_eu_8066641C;
        self->field_0x1DE = 0;
        self->field_0x1E0 = 0;
        self->shake[0].field_0x160 = 1;
        self->shake[0].field_0x166 = 0;
        self->shake[0].field_0x168 = 0;
        self->shake[0].field_0x16C = lbl_eu_80666418;
        self->shake[0].field_0x170 = lbl_eu_8066641C;
        self->shake[0].field_0x174 = lbl_eu_8066641C;
        self->shake[0].field_0x162 = 0;
        self->shake[0].field_0x164 = 0;
        self->shake[1].field_0x160 = 1;
        self->shake[1].field_0x166 = 0;
        self->shake[1].field_0x168 = 0;
        self->shake[1].field_0x16C = lbl_eu_80666418;
        self->shake[1].field_0x170 = lbl_eu_8066641C;
        self->shake[1].field_0x174 = lbl_eu_8066641C;
        self->shake[1].field_0x162 = 0;
        self->shake[1].field_0x164 = 0;
        self->field_0x3E = (s16)unk34;
    }

    // Seed the aim vectors from the source voice (or clear them).
    CfCamEventAimRegion* aim = (CfCamEventAimRegion*)self;
    if (r31 != 0) {
        aim->field_0x0C = r31;
        aim->field_0x10 = ((CamEventAimObj*)r31)->field_0x74;
        ml::CVec3 tmp;
        func_800756D0(&tmp, (CinemCamSrc*)r31);
        f32 tx = tmp.x;
        f32 ty = tmp.y;
        aim->field_0x18 = ty;
        aim->field_0x14 = tx;
        aim->field_0x1C = tmp.z;
    } else {
        aim->field_0x0C = 0;
        aim->field_0x10 = 0;
    }
    if (r29 != 0) {
        aim->field_0x20 = r29;
        aim->field_0x24 = ((CamEventAimObj*)r29)->field_0x74;
        ml::CVec3 tmp;
        func_800756D0(&tmp, (CinemCamSrc*)r29);
        f32 tx = tmp.x;
        f32 ty = tmp.y;
        aim->field_0x2C = ty;
        aim->field_0x28 = tx;
        aim->field_0x30 = tmp.z;
    } else {
        aim->field_0x20 = 0;
        aim->field_0x24 = 0;
    }

    // Push the follow state into the manager's follow-cam object.
    void* obj4 = self->slots[1];
    self->field_0x34 = (u32)r29;
    ((CamEventMgrSub*)obj4)->vtable->fn_0x40(obj4, (void*)unk34);
    ((CamEventMgrSub*)obj4)->vtable->fn_0x5C(obj4, r31);

    self->field_0x48 = cam->h02;
    self->field_0x4C = cam->h04;
    void* blk = ((CfCamEventSlotView*)self->slots[0])->field_0x0C;
    void* va = ((CamEventVoice*)r29)->vtable->fn_0xAC(r29);
    void* vb = src->voice.vtable->fn_0xAC(&src->voice);
    if (func_800A4050((u8*)blk + 0x10C, vb, va) != 0 &&
        (self->field_0x48 & 0x10) == 0) {
        self->field_0x48 |= 0x10000;
    }

    // Dispatch on the state word bits: 0x4 selects the manager's own aim
    // vectors, 0x20 copies the slot's four vectors into a local block.
    int r29i = 0;
    if (self->field_0x48 & 0x4) {
        func_8007990C(self, 0, 1, (CamEventVecSrc*)&self->field_0x54, 0);
        r29i = 0x1E;
    } else if (self->field_0x48 & 0x20) {
        CamEventVecSrc tmp;
        if (self->slots[0] != 0) {
            CfCamEventSlotObj* s = (CfCamEventSlotObj*)self->slots[0];
            tmp.v0 = *s->vtable->fn_0x10(s);
            tmp.v1 = *s->vtable->fn_0x34(s);
            tmp.v2 = *s->vtable->fn_0x1C(s);
            tmp.f_24 = s->vtable->fn_0x58(s);
        }
        func_8007990C(self, 0, 1, &tmp, 0);
        r29i = 0x1E;
    }

    ml::CVec3 o1, o2;
    if ((self->field_0x48 & 0x4) == 0) {
        func_80075934(&o1, &o2, (CamCamSrc*)r28, (CamCamSrc*)r27,
                      (ml::CVec3*)&cam->f1C, (ml::CVec3*)&cam->f28,
                      cam->h0A, cam->h0C,
                      (u8)((self->field_0x48 >> 1) & 1),
                      (u8)((self->field_0x48 >> 16) & 1));
        func_80078D08(self, r29i, &o1, &o2, cam->h08, 0, cam->f14);
        r29i += cam->h06;
    }
    func_80075934(&o1, &o2, (CamCamSrc*)r28, (CamCamSrc*)r27,
                  (ml::CVec3*)&cam->f34, (ml::CVec3*)&cam->f40,
                  cam->h0E, cam->h10,
                  (u8)((self->field_0x48 >> 1) & 1),
                  (u8)((self->field_0x48 >> 16) & 1));
    func_80078D08(self, (u16)r29i, &o1, &o2, 0, 0, cam->f18);
    func_80078D08(self, (u16)r29i, &o1, &o2, 4, 0, cam->f18);
    if (self->field_0x48 & 0x40) {
        CamEventVecSrc tmp2;
        if (self->slots[0] != 0) {
            CfCamEventSlotObj* s = (CfCamEventSlotObj*)self->slots[0];
            tmp2.v0 = *s->vtable->fn_0x10(s);
            tmp2.v1 = *s->vtable->fn_0x34(s);
            tmp2.v2 = *s->vtable->fn_0x1C(s);
            tmp2.f_24 = s->vtable->fn_0x58(s);
        }
        func_8007990C(self, (u16)(r29i + 0x1E), 0, &tmp2, 0);
    }

    // Shake tail (same busy/flag logic as func_80079B34).
    self->field_0x1DE = 1;
    self->field_0x1DF = 0;

    int busy = 0;
    if (lbl_eu_80663DF0 != 0) {
        s16 g = lbl_eu_80663DF0->field_0x3E;
        if (g >= 0x10 && g <= 0x2b) busy = 1;
    }

    if (busy) {
        self->shake[0].field_0x162 = 1;
        u16 demo = lbl_eu_80570C90.h00;
        u8 on = (demo != 0);
        self->shake[0].field_0x163 = on;
        if (on) {
            s16 count = self->shake[0].field_0x166;
            CfCamEventElem* e = self->shake[0].u.elems + 1;
            for (int i = 1; i < count; i++, e++) {
                f32 cur = e->x4;
                f32 prev = (e - 1)->x4;
                f32 d = cur - prev;
                if ((f32)__fabs((f64)d) >= lbl_eu_8066A1F8) {
                    if (d > lbl_eu_8066641C)
                        e->x4 = cur - lbl_eu_8066A1FC;
                    else
                        e->x4 = cur + lbl_eu_8066A1FC;
                }
            }
        }
    } else {
        if (self->field_0x47) {
            self->shake[0].field_0x162 = 1;
            self->shake[0].field_0x163 = 0;
        } else {
            self->shake[0].field_0x162 = 1;
            self->shake[0].field_0x163 = 1;
            s16 count = self->shake[0].field_0x166;
            CfCamEventElem* e = self->shake[0].u.elems + 1;
            for (int i = 1; i < count; i++, e++) {
                f32 cur = e->x4;
                f32 prev = (e - 1)->x4;
                f32 d = cur - prev;
                if ((f32)__fabs((f64)d) >= lbl_eu_8066A1F8) {
                    if (d > lbl_eu_8066641C)
                        e->x4 = cur - lbl_eu_8066A1FC;
                    else
                        e->x4 = cur + lbl_eu_8066A1FC;
                }
            }
        }
    }

    if (self->field_0x50 & 0x10000) {
        self->shake[1].field_0x162 = 0;
    } else {
        self->shake[1].field_0x162 = 1;
        self->shake[1].field_0x163 = 0;
    }
    return (void*)1;
}

extern "C" __declspec(noinline) void* func_800778E4(void* self, u32 a,
                                                       void* b, void* c) { return 0; }

extern "C" __declspec(noinline) void func_80077F20(void* out, void* a,
                                                     void* b, u16 c, u16 d,
                                                     void* e) {}

// Maps an (action, parameter) pair to a campaign-state id; default 5.
extern "C" int func_80078400(int action, int param) {
    int result = 5;
    if (action == 1 && param == 1) { result = 1; goto done; }
    if (action == 2 && param == 2) { result = 2; goto done; }
    if (action == 3 && param == 3) { result = 3; goto done; }
    if (action == 1 && param == 2) { result = 5; goto done; }
    if (action == 2 && param == 1) { result = 6; goto done; }
    if (action == 3) { result = 3; goto done; }
    if (action == 4) { result = 4; }
done:
    return result;
}

// Copy a slot's four camera-vector accessor results into the manager's
// 0x54..0x78 range. The vector words are copied as bit patterns (GPR
// load-pair / store-descend schedule, same shape as func_80075674).
// Written inline twice below because the retail body duplicates it; a
// shared helper would alter the register allocation.

// Re-seat a cam slot. Categorises `first` into a 0/1/2 slot index; when the
// slot at that index does not already hold the current cam state the old
// slot is deleted and a new one constructed from `second` (with the pose
// returned by func_80496264 as its second ctor arg). Then the manager's
// three slots are handed off, and for category 1 the cam table `sixth` is
// copied into a local, reshaped via func_80077F20 and pushed into the
// shared pose solver (func_80076F88 / func_800778E4). Returns slots[idx].
extern "C" __declspec(noinline) void* func_800784A0(u32 first, void* second,
                                                      void* third, void* fourth,
                                                      void* fifth,
                                                      CfCamDataTable* sixth,
                                                      void* seventh) {
    CfCamEventManager* mgr = (CfCamEventManager*)lbl_eu_80663DF0;
    if (mgr == 0) return 0;

    CfCamEventSlot* cur = mgr->slots[mgr->field_0x3C];
    int idx = func_800755BC(mgr, first);
    CfCamEventManager* m = (CfCamEventManager*)lbl_eu_80663DF0;
    int old = m->field_0x3C;

    if (first == 0) {
        if (old != 0 && cur != 0) {
            m->field_0x54 = *((CfCamEventSlotObj*)cur)->vtable->fn_0x10(cur);
            m->field_0x60 = *((CfCamEventSlotObj*)cur)->vtable->fn_0x34(cur);
            m->field_0x6C = *((CfCamEventSlotObj*)cur)->vtable->fn_0x1C(cur);
            m->field_0x78 = ((CfCamEventSlotObj*)cur)->vtable->fn_0x58(cur);
        }
    } else {
        if (cur != 0) {
            m->field_0x54 = *((CfCamEventSlotObj*)cur)->vtable->fn_0x10(cur);
            m->field_0x60 = *((CfCamEventSlotObj*)cur)->vtable->fn_0x34(cur);
            m->field_0x6C = *((CfCamEventSlotObj*)cur)->vtable->fn_0x1C(cur);
            m->field_0x78 = ((CfCamEventSlotObj*)cur)->vtable->fn_0x58(cur);
        }
    }

    // Re-fetch the manager: the copy blocks called through vtables, so the
    // global may have been modified (retail reloads before this test and
    // reuses the loaded pointer for the flag stores below).
    CfCamEventManager* gm = (CfCamEventManager*)lbl_eu_80663DF0;
    if (gm->field_0x3C != idx || gm->slots[idx] == 0) {
        gm->field_0x1DE = 0;
        gm->shake[0].field_0x162 = 0;
        gm->shake[1].field_0x162 = 0;
        CfCamEventManager* g = (CfCamEventManager*)lbl_eu_80663DF0;
        CfCamEventSlot* p = g->slots[idx];
        if (p) {
            if (p) {
                if (p) {
                    delete p;
                    g->slots[idx] = 0;
                }
                g->slots[idx] = 0;
            }
        }
        g->field_0x47 = 0;
        void* pose = func_80496264((void*)CfRes_getD80Flag(), 0);
        if (idx == 0) {
            g->slots[idx] = (CfCamEventSlot*)func_80074CEC(second, pose);
        } else if (idx == 2) {
            g->slots[idx] = (CfCamEventSlot*)__ct__8006B310(second, pose);
        } else if (idx == 1) {
            g->slots[idx] = (CfCamEventSlot*)__ct__8006B310(second, pose);
        }
        g->field_0x3C = (s16)idx;
    }

    // Hand every slot off to the shared cam-event cleanup, then wake the
    // freshly-seated slot.
    {
        CfCamEventManager* g = (CfCamEventManager*)lbl_eu_80663DF0;
        for (int i = 0; i < 3; i++) {
            if (g->slots[i] != 0) func_8006BC1C(g->slots[i], 0x8000);
        }
        func_8006BBF4(((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx], 0x8000, 1);
    }

    if (cur != 0) {
        ((CfCamEventManager*)lbl_eu_80663DF0)->field_0x46 = 0;
        if (idx == 0) {
            CfCamEventSlotObj* s0 =
                (CfCamEventSlotObj*)((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx];
            if (s0 != 0) {
                s0->vtable->fn_0x5C(s0, second);
            }
            if (third != 0) {
                CfCamEventSlotObj* cs = (CfCamEventSlotObj*)cur;
                f32 f = cs->vtable->fn_0x58(cs);
                void* p34 = cs->vtable->fn_0x34(cs);
                void* p10 = cs->vtable->fn_0x10(cs);
                func_8006D450(((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx],
                              p10, p34, f, 0, third, seventh);
            } else {
                CfCamEventSlotObj* cs = (CfCamEventSlotObj*)cur;
                f32 f = cs->vtable->fn_0x58(cs);
                void* p34 = cs->vtable->fn_0x34(cs);
                void* p10 = cs->vtable->fn_0x10(cs);
                func_8006D450(((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx],
                              p10, p34, f, fourth, 0, 0);
            }
        } else if (idx == 1) {
            void* result;
            if (sixth != 0) {
                CfCamDataTable tbl;
                tbl.h00 = sixth->h00;
                tbl.h02 = sixth->h02;
                tbl.h04 = sixth->h04;
                tbl.h06 = sixth->h06;
                tbl.h08 = sixth->h08;
                tbl.h0A = sixth->h0A;
                tbl.h0C = sixth->h0C;
                tbl.h0E = sixth->h0E;
                tbl.h10 = sixth->h10;
                tbl.f14 = sixth->f14;
                tbl.f18 = sixth->f18;
                f32 q1 = sixth->f1C;
                f32 q2 = sixth->f20;
                tbl.f20 = q2;
                tbl.f1C = q1;
                tbl.f24 = sixth->f24;
                f32 q3 = sixth->f28;
                f32 q4 = sixth->f2C;
                tbl.f2C = q4;
                tbl.f28 = q3;
                tbl.f30 = sixth->f30;
                f32 q5 = sixth->f34;
                f32 q6 = sixth->f38;
                tbl.f38 = q6;
                tbl.f34 = q5;
                tbl.f3C = sixth->f3C;
                f32 q7 = sixth->f40;
                f32 q8 = sixth->f44;
                tbl.f44 = q8;
                tbl.f40 = q7;
                tbl.f48 = sixth->f48;

                // Resolve the follow object: the player's voice sub-object,
                // falling back to player 1.
                void* player;
                if (second == 0) {
                    player = 0;
                } else {
                    player = 0;
                    void* v = ((CamEventVoice*)second)->vtable->fn_0x4C(second);
                    if (v != 0) player = func_800B708C__Fi((int)v);
                    if (player == 0) player = getPlayer__Q22cf13CfGameManagerFi(1);
                }

                fourth = (void*)func_80078400(sixth->h0A, sixth->h0C);
                third = (void*)func_80078400(sixth->h0E, sixth->h10);
                ml::CVec3 t1, t2, t3, t4;
                func_80077F20(&t1, second, player, (u16)(u32)fourth, sixth->h0A,
                              &sixth->f1C);
                tbl.f1C = t1.x;
                tbl.f20 = t1.y;
                tbl.f24 = t1.z;
                func_80077F20(&t2, second, player, (u16)(u32)fourth, sixth->h0C,
                              &sixth->f28);
                tbl.f28 = t2.x;
                tbl.f2C = t2.y;
                tbl.f30 = t2.z;
                func_80077F20(&t3, second, player, (u16)(u32)third, sixth->h0E,
                              &sixth->f34);
                tbl.f34 = t3.x;
                tbl.f38 = t3.y;
                tbl.f3C = t3.z;
                func_80077F20(&t4, second, player, (u16)(u32)third, sixth->h10,
                              &sixth->f40);
                tbl.f40 = t4.x;
                tbl.f44 = t4.y;
                tbl.f48 = t4.z;
                result = func_80076F88((CfCamEventManager*)lbl_eu_80663DF0,
                                       first, second, &tbl);
            } else {
                result = func_800778E4((CfCamEventManager*)lbl_eu_80663DF0,
                                       first, second, fifth);
            }
            if (result == 0 && old != idx) {
                func_8006BC1C(((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx], 0x8000);
                func_8006BBF4(((CfCamEventManager*)lbl_eu_80663DF0)->slots[0], 0x8000, 1);
                idx = 0;
                first = 0;
                ((CfCamEventManager*)lbl_eu_80663DF0)->field_0x3C = 0;
            }
        }
    }

    ((CfCamEventManager*)lbl_eu_80663DF0)->field_0x3E = (s16)first;
    return ((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx];
}

void func_80078B60(CfCamEventManager* self, u32 idx, u32 param) {
    u32 n = (u32)func_800755BC(self, idx);
    CfCamEventSlot* p = self->slots[n];
    if (p) {
        if (p) {
            delete p;
            self->slots[n] = 0;
        }
        self->slots[n] = 0;
    }
    func_800784A0(idx, (void*)param, 0, 0, 0, 0, 0);
}

// Reset the cam-event manager: clear the low halfword of the flags word,
// re-seed the three shake-table groups (manager 0x1DC group + the two shake
// units) with the given id/float constants, then forward the parameters to
// the cam-data setup helper when the game manager is alive.
void func_80078C08(CfCamEventManager* self, u32 first, u32 second,
                   u32 third, u32 fourth, u32 fifth) {
    func_8008212C__Q22cf13CfGameManagerFv(first);
    f32 f1C = lbl_eu_8066641C;
    f32 f18 = lbl_eu_80666418;
    u32 flags = self->field_0x50 & 0xFFFF0000u;
    self->field_0x40 = 0;
    self->field_0x50 = flags;
    self->field_0x1DC = (u16)second;
    self->field_0x1E2 = 0;
    self->field_0x1E4 = 0;
    self->field_0x1E8 = f18;
    self->field_0x1EC = f1C;
    self->field_0x1F0 = f1C;
    self->field_0x1DE = 0;
    self->field_0x1E0 = 0;
    self->shake[0].field_0x160 = (u16)second;
    self->shake[0].field_0x166 = 0;
    self->shake[0].field_0x168 = 0;
    self->shake[0].field_0x16C = f18;
    self->shake[0].field_0x170 = f1C;
    self->shake[0].field_0x174 = f1C;
    self->shake[0].field_0x162 = 0;
    self->shake[0].field_0x164 = 0;
    self->shake[1].field_0x160 = (u16)second;
    self->shake[1].field_0x166 = 0;
    self->shake[1].field_0x168 = 0;
    self->shake[1].field_0x16C = f18;
    self->shake[1].field_0x170 = f1C;
    self->shake[1].field_0x174 = f1C;
    self->shake[1].field_0x162 = 0;
    self->shake[1].field_0x164 = 0;
    self->field_0x3E = (s16)first;
    self->field_0x34 = fifth;
    UnkClass_800821F8* gm = func_800821F8__Q22cf13CfGameManagerFv();
    if (third != 0 && gm != 0) {
        func_8007990C(self, 0, fourth, (CamEventVecSrc*)&self->field_0x54, 0);
        self->field_0x40 = (u16)third;
    }
}

// Camera-event state machine: dispatches on the manager's cam-state id (0x3E).
// State 9 scales the +0x18 triplet into a temp and appends it; states 8 / 0xA
// subtract the +0x34 source's aim vector (PS vector sub) or run func_80074230
// on the two +0x00/+0x0C aim vectors; the default state appends the raw pair
// when the global cam state is in its busy frame range [0x10, 0x2b].
void func_8007990C(CfCamEventManager* self, u32 a, u32 b, CamEventVecSrc* c, u32 d) {
    s16 st = self->field_0x3E;
    if (st == 9) {
        ml::CVec3 tmp;
        tmp.x = c->v2.x * lbl_eu_8066A20C;
        tmp.y = c->v2.y * lbl_eu_8066A20C;
        tmp.z = c->v2.z * lbl_eu_8066A20C;
        func_80078D08(self, a, (ml::CVec3*)c, &tmp, b, 0, c->f_24);
    } else if (st == 8) {
        ml::CVec3 v1, v2;
        v1.x = c->v0.x; v1.y = c->v0.y; v1.z = c->v0.z;
        v2.x = c->v1.x; v2.y = c->v1.y; v2.z = c->v1.z;
        if (self->field_0x34 != 0) {
            CinemCamSrc* src34 = (CinemCamSrc*)(void*)self->field_0x34;
            CinemVecOut* sv = src34->vtable->fn_0xAC(src34);
            v1 -= sv->v;
            v2 -= sv->v;
        }
        func_80078D08(self, a, &v1, &v2, b, 0, c->f_24);
    } else if (st == 0xA) {
        ml::CVec3 v1, v2;
        v1.x = c->v0.x; v1.y = c->v0.y; v1.z = c->v0.z;
        v2.x = c->v1.x; v2.y = c->v1.y; v2.z = c->v1.z;
        if (self->field_0x34 != 0) {
            func_80074230(&v1, &v2);
        }
        func_80078D08(self, a, &v1, &v2, b, 0, c->f_24);
    } else {
        int busy = 0;
        if (lbl_eu_80663DF0 != 0) {
            s16 g = lbl_eu_80663DF0->field_0x3E;
            if (g >= 0x10 && g <= 0x2b) busy = 1;
        }
        if (busy) {
            func_80078D08(self, a, (ml::CVec3*)c, (ml::CVec3*)&c->v1, b, d, c->f_24);
        }
    }
}

// Insert a scaled aim vector into the manager's three shake tables (0x7C,
// 0x1F4, 0x36C), each capped at 16 entries. The cam-state word (0x3E)
// selects the fill pattern:
//   state 9      - r6 scaled by lbl_eu_8066A210 and normalized, stored into
//                  all three tables (table2 receives the (p1,1,1) constant
//                  triplet instead);
//   state 8 + 47 - raw pair: table0 <- r6, table1 <- p5, d word cleared;
//   state 8/-0xA - angle pair: table0 <- r6, table1 <- the yaw/pitch angles
//                  of the (p5 - r6) difference, d word cleared;
//   anything else- only while the global cam state is in the busy range
//                  [0x10, 0x2b]: demo mode (global table h00) swaps the
//                  angle vector into table1, otherwise table0 <- p5 and
//                  table1 <- r6.
// Every appended element shares the id halfword (add + field_0x40) and the
// p7/p8 word fields. The angle-pair body is duplicated below exactly as
// retail duplicates it (a helper would add a call).
void func_80078D08(CfCamEventManager* self, int add, ml::CVec3* p5, ml::CVec3* r6,
                   int p7, int p8, f32 p1) {
    u16 id = (u16)(add + self->field_0x40);
    if (p1 <= lbl_eu_8066A208)
        self->field_0x50 |= 0x10000;

    s16 state = self->field_0x3E;
    if (state == 9) {
        // Scale the r6 aim vector and normalize it; all three tables get it.
        ml::CVec3 sv;
        f32 k = lbl_eu_8066A210;
        sv.x = r6->x * k;
        sv.y = r6->y * k;
        sv.z = r6->z * k;
        func_800A3F8C(&sv);

        // table0 (0x7C) - elements start at the table base; count at 0x1E2.
        // Retail stores the raw p5 words here; the scaled vector goes to
        // table1 (that is why p5 lives in a saved register across the
        // normalize call).
        s16 cnt0 = self->field_0x1E2;
        if (cnt0 < 0x10) {
            CfCamEventElem* e = &self->tab0.elems[cnt0];
            e->x0 = p5->x;
            e->x4 = p5->y;
            e->x8 = p5->z;
            e->id = id;
            e->c = (u16)p7;
            e->d = (u16)p8;
            if (cnt0 == 0) {
                self->tab0.baseX = p5->x;
                self->tab0.baseY = p5->y;
                self->tab0.baseZ = p5->z;
            }
            self->field_0x1E2 = cnt0 + 1;
            self->field_0x1F0 = (f32)id;
        }

        // table1 (0x1F4) - shake unit 0 element array
        s16 cnt1 = self->shake[0].field_0x166;
        if (cnt1 < 0x10) {
            CfCamEventElem* e = &self->shake[0].u.elems[cnt1];
            e->x0 = sv.x;
            e->x4 = sv.y;
            e->x8 = sv.z;
            e->id = id;
            e->c = (u16)p7;
            e->d = (u16)p8;
            if (cnt1 == 0) {
                // Retail stores baseY first, then baseX, then baseZ here.
                f32 bx = sv.x;
                f32 by = sv.y;
                self->shake[0].u.tab.baseY = by;
                self->shake[0].u.tab.baseX = bx;
                self->shake[0].u.tab.baseZ = sv.z;
            }
            self->shake[0].field_0x166 = cnt1 + 1;
            self->shake[0].field_0x174 = (f32)id;
        }

        // table2 (0x36C): stores the (p1, 1, 1) constant triplet. Retail
        // materializes the triple in a stack temp, then GPR-copies it.
        s16 cnt2 = self->shake[1].field_0x166;
        f32 c2[3] = { p1, lbl_eu_8066641C, lbl_eu_8066641C };
        if (cnt2 < 0x10) {
            CfCamEventElem* e = &self->shake[1].u.tab.elems[cnt2];
            e->x0 = c2[0];
            e->x4 = c2[1];
            e->x8 = c2[2];
            e->id = id;
            e->c = (u16)p7;
            e->d = 0;
            if (cnt2 == 0) {
                self->shake[1].u.tab.baseX = c2[0];
                self->shake[1].u.tab.baseY = c2[1];
                self->shake[1].u.tab.baseZ = c2[2];
            }
            self->shake[1].field_0x166 = cnt2 + 1;
            self->shake[1].field_0x174 = (f32)id;
        }
    } else if (state == 8) {
        if (self->field_0x47 != 0) {
            // One-shot path: raw pair, d word cleared.
            s16 cnt0 = self->field_0x1E2;
            if (cnt0 < 0x10) {
                CfCamEventElem* e = &self->tab0.elems[cnt0];
                e->x0 = r6->x;
                e->x4 = r6->y;
                e->x8 = r6->z;
                e->id = id;
                e->c = (u16)p7;
                e->d = 0;
                if (cnt0 == 0) {
                    self->tab0.baseX = r6->x;
                    self->tab0.baseY = r6->y;
                    self->tab0.baseZ = r6->z;
                }
                self->field_0x1E2 = cnt0 + 1;
                self->field_0x1F0 = (f32)id;
            }
            s16 cnt1 = self->shake[0].field_0x166;
            if (cnt1 < 0x10) {
                CfCamEventElem* e = &self->shake[0].u.elems[cnt1];
                e->x0 = p5->x;
                e->x4 = p5->y;
                e->x8 = p5->z;
                e->id = id;
                e->c = (u16)p7;
                e->d = 0;
                if (cnt1 == 0) {
                    self->shake[0].u.tab.baseX = p5->x;
                    self->shake[0].u.tab.baseY = p5->y;
                    self->shake[0].u.tab.baseZ = p5->z;
                }
                self->shake[0].field_0x166 = cnt1 + 1;
                self->shake[0].field_0x174 = (f32)id;
            }
            s16 cnt2 = self->shake[1].field_0x166;
            f32 c2[3] = { p1, lbl_eu_8066641C, lbl_eu_8066641C };
            if (cnt2 < 0x10) {
                CfCamEventElem* e = &self->shake[1].u.tab.elems[cnt2];
                e->x0 = c2[0];
                e->x4 = c2[1];
                e->x8 = c2[2];
                e->id = id;
                e->c = (u16)p7;
                e->d = 0;
                if (cnt2 == 0) {
                    self->shake[1].u.tab.baseX = c2[0];
                    self->shake[1].u.tab.baseY = c2[1];
                    self->shake[1].u.tab.baseZ = c2[2];
                }
                self->shake[1].field_0x166 = cnt2 + 1;
                self->shake[1].field_0x174 = (f32)id;
            }
        } else {
            // One-shot clear path: table0 gets the raw r6 vector, table1 the
            // yaw/pitch angles of the (p5 - r6) difference.
            ml::CVec3 d;
            d.x = p5->x - r6->x;
            d.y = p5->y - r6->y;
            d.z = p5->z - r6->z;
            f32 len = (f32)sqrt((f64)(d.z * d.z + d.x * d.x));
            ml::CVec3 out;
            out.x = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.y, len);
            out.y = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
            out.z = lbl_eu_8066641C;
            func_800A3F8C(&out);
            if ((f32)__fabs((f64)d.x) <= lbl_eu_8066A208 &&
                (f32)__fabs((f64)d.y) <= lbl_eu_8066A208 &&
                (f32)__fabs((f64)d.z) <= lbl_eu_8066A208)
                out.z = lbl_eu_8066641C;
            else
                out.z = PSVECMag(d);

            s16 cnt0 = self->field_0x1E2;
            if (cnt0 < 0x10) {
                CfCamEventElem* e = &self->tab0.elems[cnt0];
                e->x0 = r6->x;
                e->x4 = r6->y;
                e->x8 = r6->z;
                e->id = id;
                e->c = (u16)p7;
                e->d = 0;
                if (cnt0 == 0) {
                    self->tab0.baseX = r6->x;
                    self->tab0.baseY = r6->y;
                    self->tab0.baseZ = r6->z;
                }
                self->field_0x1E2 = cnt0 + 1;
                self->field_0x1F0 = (f32)id;
            }
            s16 cnt1 = self->shake[0].field_0x166;
            if (cnt1 < 0x10) {
                CfCamEventElem* e = &self->shake[0].u.elems[cnt1];
                e->x0 = out.x;
                e->x4 = out.y;
                e->x8 = out.z;
                e->id = id;
                e->c = (u16)p7;
                e->d = 0;
                if (cnt1 == 0) {
                    // Retail stores baseY first, then baseX, then baseZ here.
                    f32 bx = out.x;
                    f32 by = out.y;
                    self->shake[0].u.tab.baseY = by;
                    self->shake[0].u.tab.baseX = bx;
                    self->shake[0].u.tab.baseZ = out.z;
                }
                self->shake[0].field_0x166 = cnt1 + 1;
                self->shake[0].field_0x174 = (f32)id;
            }
            s16 cnt2 = self->shake[1].field_0x166;
            f32 c2[3] = { p1, lbl_eu_8066641C, lbl_eu_8066641C };
            if (cnt2 < 0x10) {
                CfCamEventElem* e = &self->shake[1].u.tab.elems[cnt2];
                e->x0 = c2[0];
                e->x4 = c2[1];
                e->x8 = c2[2];
                e->id = id;
                e->c = (u16)p7;
                e->d = 0;
                if (cnt2 == 0) {
                    self->shake[1].u.tab.baseX = c2[0];
                    self->shake[1].u.tab.baseY = c2[1];
                    self->shake[1].u.tab.baseZ = c2[2];
                }
                self->shake[1].field_0x166 = cnt2 + 1;
                self->shake[1].field_0x174 = (f32)id;
            }
        }
    } else if (state == 0xA) {
        // Same angle pair as state 8's clear path.
        ml::CVec3 d;
        d.x = p5->x - r6->x;
        d.y = p5->y - r6->y;
        d.z = p5->z - r6->z;
        f32 len = (f32)sqrt((f64)(d.z * d.z + d.x * d.x));
        ml::CVec3 out;
        out.x = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.y, len);
        out.y = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
        out.z = lbl_eu_8066641C;
        func_800A3F8C(&out);
        if ((f32)__fabs((f64)d.x) <= lbl_eu_8066A208 &&
            (f32)__fabs((f64)d.y) <= lbl_eu_8066A208 &&
            (f32)__fabs((f64)d.z) <= lbl_eu_8066A208)
            out.z = lbl_eu_8066641C;
        else
            out.z = PSVECMag(d);

        s16 cnt0 = self->field_0x1E2;
        if (cnt0 < 0x10) {
            CfCamEventElem* e = &self->tab0.elems[cnt0];
            e->x0 = r6->x;
            e->x4 = r6->y;
            e->x8 = r6->z;
            e->id = id;
            e->c = (u16)p7;
            e->d = 0;
            if (cnt0 == 0) {
                self->tab0.baseX = r6->x;
                self->tab0.baseY = r6->y;
                self->tab0.baseZ = r6->z;
            }
            self->field_0x1E2 = cnt0 + 1;
            self->field_0x1F0 = (f32)id;
        }
        s16 cnt1 = self->shake[0].field_0x166;
        if (cnt1 < 0x10) {
            CfCamEventElem* e = &self->shake[0].u.elems[cnt1];
            e->x0 = out.x;
            e->x4 = out.y;
            e->x8 = out.z;
            e->id = id;
            e->c = (u16)p7;
            e->d = 0;
            if (cnt1 == 0) {
                // Retail stores baseY first, then baseX, then baseZ here.
                f32 bx = out.x;
                f32 by = out.y;
                self->shake[0].u.tab.baseY = by;
                self->shake[0].u.tab.baseX = bx;
                self->shake[0].u.tab.baseZ = out.z;
            }
            self->shake[0].field_0x166 = cnt1 + 1;
            self->shake[0].field_0x174 = (f32)id;
        }
        s16 cnt2 = self->shake[1].field_0x166;
        f32 c2[3] = { p1, lbl_eu_8066641C, lbl_eu_8066641C };
        if (cnt2 < 0x10) {
            CfCamEventElem* e = &self->shake[1].u.tab.elems[cnt2];
            e->x0 = c2[0];
            e->x4 = c2[1];
            e->x8 = c2[2];
            e->id = id;
            e->c = (u16)p7;
            e->d = 0;
            if (cnt2 == 0) {
                self->shake[1].u.tab.baseX = c2[0];
                self->shake[1].u.tab.baseY = c2[1];
                self->shake[1].u.tab.baseZ = c2[2];
            }
            self->shake[1].field_0x166 = cnt2 + 1;
            self->shake[1].field_0x174 = (f32)id;
        }
    } else {
        // Default states: only append while the global cam state is inside
        // its busy frame range; demo mode swaps the angle vector into table1.
        int busy = 0;
        if (lbl_eu_80663DF0 != 0) {
            s16 g = lbl_eu_80663DF0->field_0x3E;
            if (g >= 0x10 && g <= 0x2b) busy = 1;
        }
        if (busy) {
            ml::CVec3 t0, t1;
            if (lbl_eu_80570C90.h00 != 0) {
                ml::CVec3 d;
                d.x = p5->x - r6->x;
                d.y = p5->y - r6->y;
                d.z = p5->z - r6->z;
                t0.x = r6->x;
                t0.y = r6->y;
                t0.z = r6->z;
                f32 len = (f32)sqrt((f64)(d.z * d.z + d.x * d.x));
                t1.x = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.y, len);
                t1.y = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
                t1.z = lbl_eu_8066641C;
                func_800A3F8C(&t1);
                if ((f32)__fabs((f64)d.x) <= lbl_eu_8066A208 &&
                    (f32)__fabs((f64)d.y) <= lbl_eu_8066A208 &&
                    (f32)__fabs((f64)d.z) <= lbl_eu_8066A208)
                    t1.z = lbl_eu_8066641C;
                else
                    t1.z = PSVECMag(d);
            } else {
                t0.x = p5->x;
                t0.y = p5->y;
                t0.z = p5->z;
                t1.x = r6->x;
                t1.y = r6->y;
                t1.z = r6->z;
            }
            s16 cnt0 = self->field_0x1E2;
            if (cnt0 < 0x10) {
                CfCamEventElem* e = &self->tab0.elems[cnt0];
                e->x0 = t0.x;
                e->x4 = t0.y;
                e->x8 = t0.z;
                e->id = id;
                e->c = (u16)p7;
                e->d = (u16)p8;
                if (cnt0 == 0) {
                    self->tab0.baseX = t0.x;
                    self->tab0.baseY = t0.y;
                    self->tab0.baseZ = t0.z;
                }
                self->field_0x1E2 = cnt0 + 1;
                self->field_0x1F0 = (f32)id;
            }
            s16 cnt1 = self->shake[0].field_0x166;
            if (cnt1 < 0x10) {
                CfCamEventElem* e = &self->shake[0].u.elems[cnt1];
                e->x0 = t1.x;
                e->x4 = t1.y;
                e->x8 = t1.z;
                e->id = id;
                e->c = (u16)p7;
                e->d = (u16)p8;
                if (cnt1 == 0) {
                    // Retail stores baseY first, then baseX, then baseZ here.
                    f32 bx = t1.x;
                    f32 by = t1.y;
                    self->shake[0].u.tab.baseY = by;
                    self->shake[0].u.tab.baseX = bx;
                    self->shake[0].u.tab.baseZ = t1.z;
                }
                self->shake[0].field_0x166 = cnt1 + 1;
                self->shake[0].field_0x174 = (f32)id;
            }
            s16 cnt2 = self->shake[1].field_0x166;
            ml::CVec3 c2(p1, lbl_eu_8066641C, lbl_eu_8066641C);
            if (cnt2 < 0x10) {
                CfCamEventElem* e = &self->shake[1].u.tab.elems[cnt2];
                e->x0 = c2.x;
                e->x4 = c2.y;
                e->x8 = c2.z;
                e->id = id;
                e->c = (u16)p7;
                e->d = (u16)p8;
                if (cnt2 == 0) {
                    self->shake[1].u.tab.baseX = c2.x;
                    self->shake[1].u.tab.baseY = c2.y;
                    self->shake[1].u.tab.baseZ = c2.z;
                }
                self->shake[1].field_0x166 = cnt2 + 1;
                self->shake[1].field_0x174 = (f32)id;
            }
        }
    }
}


// Advance each element of the shake table toward its predecessor by 2*PI once
// the gap reaches PI. Written inline in both branches below (retail keeps two
// copies of the loop).
void func_80079B34(CfCamEventManager* self) {
    self->field_0x1DE = 1;
    self->field_0x1DF = 0;

    // Whether the global cam manager is inside its busy frame range.
    int busy = 0;
    if (lbl_eu_80663DF0 != nullptr) {
        s16 g = lbl_eu_80663DF0->field_0x3E;
        if (g >= 0x10 && g <= 0x2b) busy = 1;
    }

    if (busy) {
        // Demo-mode flag drives the shake update.
        self->shake[0].field_0x162 = 1;
        u16 demo = lbl_eu_80570C90.h00;
        u8 on = (demo != 0);
        self->shake[0].field_0x163 = on;
        if (on) {
            s16 count = self->shake[0].field_0x166;
            CfCamEventElem* e = self->shake[0].u.elems + 1;
            for (int i = 1; i < count; i++, e++) {
                f32 cur = e->x4;
                f32 prev = (e - 1)->x4;
                f32 d = cur - prev;
                if (fabsf(d) >= lbl_eu_8066A1F8) {
                    if (d > lbl_eu_8066641C)
                        e->x4 = cur - lbl_eu_8066A1FC;
                    else
                        e->x4 = cur + lbl_eu_8066A1FC;
                }
            }
        }
    } else {
        // When not busy, field_0x47 decides whether to shake.
        if (self->field_0x47) {
            self->shake[0].field_0x162 = 1;
            self->shake[0].field_0x163 = 0;
        } else {
            self->shake[0].field_0x162 = 1;
            self->shake[0].field_0x163 = 1;
            s16 count = self->shake[0].field_0x166;
            CfCamEventElem* e = self->shake[0].u.elems + 1;
            for (int i = 1; i < count; i++, e++) {
                f32 cur = e->x4;
                f32 prev = (e - 1)->x4;
                f32 d = cur - prev;
                if (fabsf(d) >= lbl_eu_8066A1F8) {
                    if (d > lbl_eu_8066641C)
                        e->x4 = cur - lbl_eu_8066A1FC;
                    else
                        e->x4 = cur + lbl_eu_8066A1FC;
                }
            }
        }
    }

    if (self->field_0x50 & 0x10000) {
        self->shake[1].field_0x162 = 0;
    } else {
        self->shake[1].field_0x162 = 1;
        self->shake[1].field_0x163 = 0;
    }
}

// Advance one shake-table element toward its predecessor by `step`. The
// retail body is a degenerate while-loop: the first argument is used both as
// the element pointer (previous element one stride back at -0x10) and as the
// base for the table count at +0x4D2, and it never advances.
void func_80079D6C(f32 step, CfCamEventShakeElem* e) {
    while ((s32)e < (s32)e->count) {
        f32 cur = e->x4;
        f32 prev = ((CfCamEventElem*)e - 1)->x4;
        f32 d = (f32)__fabs((f64)(cur - prev));
        if (d >= step) {
            if (d > step)
                e->x4 = cur - step;
            else
                e->x4 = cur + step;
        }
    }
}

// True while any of three "active" flags are set on the manager.
// NOTE: retail materializes the sub pointer (addi r3,r3,0x1f4) and keeps the
// three checks branchy; Wii/1.1 -O4,p folds the addi into the displacements
// and normalizes the tail return to the neg/or/rlwinm bool idiom (same
// version wall as func_8007560C's two-cmpi range check).
bool func_80079DBC(CfCamEventManager* manager) {
    if (manager->field_0x1DE != 0) return true;
    CfCamEventShakeUnit* sub = &manager->shake[0];
    if (sub->field_0x162 != 0) return true;
    if (sub[1].field_0x162 != 0) return true;
    return false;
}

// Per-frame cam-event update (state machine). Resolves the game manager via
// dynamic_cast and checks its busy flag and the frame-speed value before
// dispatching on the cam-state word (0x3E). The main path (flag 0x1DE set)
// advances the three shake tables, then feeds the manager's aim/base vectors
// through the dynamic-cast object's vector setters; states 8/0xA compose a
// pair of rotations (sin/cos of the table-base axis) around the source
// vector. Returns the busy/flag word (r29).
int func_80079E04(CfCamEventManager* self) {
    void* gm = func_800821F8__Q22cf13CfGameManagerFv();
    CfDynMgr* dyn = (CfDynMgr*)__dynamic_cast(gm, 0, (const void*)&lbl_eu_80661B00,
                                              (const void*)&lbl_eu_80661B30, 0);
    if (dyn == 0) return 0;
    if (dyn->m_field04 & 0x04000000) return 0;

    CfRes_getD80Flag();
    f32 val = func_80496288();
    if (val <= 0.0f) return 0;

    int result = 0;
    if (self->field_0x1DE != 0) {
        // r31 = the unit-1 active flag (0x4CE); survives to the tail call.
        int flag = (self->shake[1].field_0x162 != 0);

        // Busy early-out: hand the whole advance to func_8007AA4C.
        int busy = 0;
        if (lbl_eu_80663DF0 != 0) {
            s16 g = lbl_eu_80663DF0->field_0x3E;
            if (g >= 0x10 && g <= 0x2b) busy = 1;
        }
        if (busy) return func_8007AA4C(self);

        // Advance the three shake tables.
        func_80074F4C((CfCamShakeState*)&self->tab0, 0);
        func_80074F4C((CfCamShakeState*)&self->shake[0], 0);
        func_80074F4C((CfCamShakeState*)&self->shake[1], 0);

        // Busy word: either the manager's own flag or unit 0's finish flag.
        result = (self->shake[0].field_0x164 != 0) || (self->field_0x1E0 != 0);

        CfCamAdvObj* adv = (CfCamAdvObj*)dyn;
        s16 state = self->field_0x3E;
        if (state == 9) {
            // Normalize the unit-0 base anchor, then push it (and the table-0
            // base anchor) into the dynamic object's vector setters.
            ml::CVec3 v;
            v.x = self->shake[0].u.tab.baseX;
            v.y = self->shake[0].u.tab.baseY;
            v.z = self->shake[0].u.tab.baseZ;
            func_800A3F8C(&v);
            adv->vtable->fn_0x14(adv, &self->tab0.baseX);
            adv->vtable->fn_0x4C(adv, &v);
        } else if (state == 8) {
            if (self->field_0x47 != 0) {
                // One-shot: table-0 base into slot 2, unit-0 base into slot 1.
                ml::CVec3 a, b;
                a.x = self->tab0.baseX;
                a.y = self->tab0.baseY;
                a.z = self->tab0.baseZ;
                b.x = self->shake[0].u.tab.baseX;
                b.y = self->shake[0].u.tab.baseY;
                b.z = self->shake[0].u.tab.baseZ;
                adv->vtable->fn_0x14(adv, &b);
                adv->vtable->fn_0x64(adv, &a);
            } else {
                // Rotate the table-0 base anchor by the two-axis rotation
                // composed from the unit-0 base (the retail fuses the 3x3
                // products into FPR chains; this is the equivalent C form).
                ml::CVec3 V;
                V.x = self->tab0.baseX;
                V.y = self->tab0.baseY;
                V.z = self->tab0.baseZ;
                f32 mz = (f32)__fabs((f64)self->shake[0].u.tab.baseZ);
                f32 ax = self->shake[0].u.tab.baseX;
                f32 ay = self->shake[0].u.tab.baseY;
                f32 one = lbl_eu_8066641C;
                f32 s1 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * one);
                f32 c1 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * one);
                f32 s2 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ax);
                f32 c2 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ax);
                // Base matrix (unit/0.5 fills as in the retail).
                f32 m00 = c1, m01 = -s1, m02 = one, m11 = c1, m12 = one,
                    m21 = one, m22 = lbl_eu_80666428;
                // M = R(s2,c2) * base, expanded component-wise.
                f32 M00 = c2 * s2 * m12 + m00 + s2 * s1;
                f32 M01 = c2 * m21 + m01 + s2 * m11;
                f32 M02 = c2 * m22 + m02 + s2 * m12;
                f32 M10 = s2 * m12 + lbl_eu_80666428 * m00 + s1;
                f32 M11 = m21 + lbl_eu_80666428 * m01 + m11;
                f32 M12 = m22 + lbl_eu_80666428 * m02 + m12;
                f32 M20 = -s2 * s2 * m12 + m00 + c2 * s1;
                f32 M21 = -s2 * m21 + m01 + c2 * m11;
                f32 M22 = -s2 * m22 + m02 + c2 * m12;

                // Second composition around the Y axis, then scale the
                // result by the base magnitude to get the offset vector m.
                f32 s3 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ay);
                f32 c3 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ay);
                f32 t00 = c3 * s3 * M12 + M00 + s3 * M10;
                f32 t01 = c3 * M21 + M01 + s3 * M11;
                f32 t02 = c3 * M22 + M02 + s3 * M12;
                f32 t10 = s3 * M12 + lbl_eu_80666428 * M00 + M10;
                f32 t11 = M21 + lbl_eu_80666428 * M01 + M11;
                f32 t12 = M22 + lbl_eu_80666428 * M02 + M12;
                f32 t20 = -s3 * s3 * M12 + M00 + c3 * M10;
                f32 t21 = -s3 * M21 + M01 + c3 * M11;
                f32 t22 = -s3 * M22 + M02 + c3 * M12;

                ml::CVec3 m;
                m.x = mz * t00 + t20;
                m.y = mz * t01 + t21;
                m.z = mz * t02 + t22;

                ml::CVec3 out1, out2;
                out1.x = V.x - m.x;
                out1.y = V.y + m.y;
                out1.z = V.z - m.z;
                out2 = V;
                if (self->field_0x34 != 0) {
                    CinemCamSrc* src = (CinemCamSrc*)(void*)self->field_0x34;
                    ml::CVec3* rv = &src->vtable->fn_0xAC(src)->v;
                    out1.x += rv->x;
                    out1.y += rv->y;
                    out1.z += rv->z;
                    out2.x += rv->x;
                    out2.y += rv->y;
                    out2.z += rv->z;
                    adv->vtable->fn_0x68(adv, (void*)self->field_0x34, &out1, 0);
                    adv->vtable->fn_0x6C(adv, (void*)self->field_0x34, &out2, 0);
                } else {
                    adv->vtable->fn_0x14(adv, &out1);
                    adv->vtable->fn_0x64(adv, &out2);
                }
            }
        } else if (state == 0xA) {
            // Same rotation composition as state 8's clear path.
            ml::CVec3 V;
            V.x = self->tab0.baseX;
            V.y = self->tab0.baseY;
            V.z = self->tab0.baseZ;
            f32 mz = (f32)__fabs((f64)self->shake[0].u.tab.baseZ);
            f32 ax = self->shake[0].u.tab.baseX;
            f32 ay = self->shake[0].u.tab.baseY;
            f32 one = lbl_eu_8066641C;
            f32 s1 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * one);
            f32 c1 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * one);
            f32 s2 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ax);
            f32 c2 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ax);
            f32 m00 = c1, m01 = -s1, m02 = one, m11 = c1, m12 = one,
                m21 = one, m22 = lbl_eu_80666428;
            f32 M00 = c2 * s2 * m12 + m00 + s2 * s1;
            f32 M01 = c2 * m21 + m01 + s2 * m11;
            f32 M02 = c2 * m22 + m02 + s2 * m12;
            f32 M10 = s2 * m12 + lbl_eu_80666428 * m00 + s1;
            f32 M11 = m21 + lbl_eu_80666428 * m01 + m11;
            f32 M12 = m22 + lbl_eu_80666428 * m02 + m12;
            f32 M20 = -s2 * s2 * m12 + m00 + c2 * s1;
            f32 M21 = -s2 * m21 + m01 + c2 * m11;
            f32 M22 = -s2 * m22 + m02 + c2 * m12;

            f32 s3 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ay);
            f32 c3 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ay);
            f32 t00 = c3 * s3 * M12 + M00 + s3 * M10;
            f32 t01 = c3 * M21 + M01 + s3 * M11;
            f32 t02 = c3 * M22 + M02 + s3 * M12;
            f32 t10 = s3 * M12 + lbl_eu_80666428 * M00 + M10;
            f32 t11 = M21 + lbl_eu_80666428 * M01 + M11;
            f32 t12 = M22 + lbl_eu_80666428 * M02 + M12;
            f32 t20 = -s3 * s3 * M12 + M00 + c3 * M10;
            f32 t21 = -s3 * M21 + M01 + c3 * M11;
            f32 t22 = -s3 * M22 + M02 + c3 * M12;

            ml::CVec3 m;
            m.x = mz * t00 + t20;
            m.y = mz * t01 + t21;
            m.z = mz * t02 + t22;

            ml::CVec3 out1, out2;
            out1.x = V.x - m.x;
            out1.y = V.y + m.y;
            out1.z = V.z - m.z;
            out2 = V;
            if (self->field_0x34 != 0) {
                CinemCamSrc* src = (CinemCamSrc*)(void*)self->field_0x34;
                ml::CVec3* rv = &src->vtable->fn_0xAC(src)->v;
                out1.x += rv->x;
                out1.y += rv->y;
                out1.z += rv->z;
                out2.x += rv->x;
                out2.y += rv->y;
                out2.z += rv->z;
                adv->vtable->fn_0x68(adv, (void*)self->field_0x34, &out1, 0);
                adv->vtable->fn_0x6C(adv, (void*)self->field_0x34, &out2, 0);
            } else {
                adv->vtable->fn_0x14(adv, &out1);
                adv->vtable->fn_0x64(adv, &out2);
            }
        }

        // One-shot active flag: push the unit-1 base magnitude into the
        // dynamic object's scalar setter.
        if (flag) {
            adv->vtable->fn_0x3C(adv, self->shake[1].u.tab.baseX);
        }
    } else {
        // Flag 0x1DE clear: busy range still hands off to func_8007AA4C;
        // otherwise the one-shot flag 0x46 is cleared via the game manager.
        int busy = 0;
        if (lbl_eu_80663DF0 != 0) {
            s16 g = lbl_eu_80663DF0->field_0x3E;
            if (g >= 0x10 && g <= 0x2b) busy = 1;
        }
        if (busy) {
            if (self->field_0x48 & 1) {
                func_8007AA4C(self);
            }
        } else {
            if (self->field_0x46 != 0) {
                func_80081E90__Q22cf13CfGameManagerFv(0, 0, self->field_0x44);
                self->field_0x46 = 0;
            }
        }
    }
    return result;
}


int func_8007AA4C(CfCamEventManager* self) { return 0; }

extern "C" void func_8007B030(u8* self) {
    *(u8*)((u8*)self + 0x1de) = 0;
    *(u8*)((u8*)self + 0x356) = 0;
    *(u8*)((u8*)self + 0x4ce) = 0;
}

// Forward shake state to the shared handler if the global cam state is alive.
int func_8007B044(int arg0, int arg1) {
    CfCamEventGlobal* g = lbl_eu_80663DF0;
    if (g != 0 && g->field_0x38 != 0) goto docall;
    return 0;
docall:
    return func_80240C98(g->field_0x38, arg0, arg1);
}

int func_8007B078(int val) {
    CfCamEventGlobal* g = lbl_eu_80663DF0;
    if (g == nullptr) return val;
    u32 state = g->field_0x38;
    if (state == 0) return state;  // r3 already holds 0 here
    return func_8024125C(state, val);
}

int func_8007B0A0(int val) {
    CfCamEventGlobal* g = lbl_eu_80663DF0;
    if (g == nullptr) return val;
    u32 state = g->field_0x38;
    if (state == 0) return state;  // r3 already holds 0 here
    return func_80241344(state, val);
}

// Initialize camera-event slot from the Bdat table. Walks down a chain of
// column probes (0Xf4/0Xfa -> 0X100 -> 0X106 -> 0X10d -> 0X114); the deepest
// non-empty one gates the rest. Heavily elided in retail so the body here
// captures the probe chain and the routing tail.
void func_8007B0C8(int idx) {
    if (lbl_eu_80663DF0 == 0) return;
    void* mgr = lbl_eu_806640BC;
    if (idx < 1) return;
    if (idx > func_8003B1EC(mgr)) return;

    int b = 0, c = 0, d = 0, e = 0, f = 0;
    if (getBdatStringColumnValue(mgr, (const char*)&lbl_eu_804FB5D0[0xf4], idx) == 0 &&
        getBdatStringColumnValue(mgr, (const char*)&lbl_eu_804FB5D0[0xfa], idx) == 0)
        f = 1;
    if (f && getBdatStringColumnValue(mgr, (const char*)&lbl_eu_804FB5D0[0x100], idx) == 0) e = 1;
    if (e && getBdatStringColumnValue(mgr, (const char*)&lbl_eu_804FB5D0[0x106], idx) == 0) d = 1;
    if (d && getBdatStringColumnValue(mgr, (const char*)&lbl_eu_804FB5D0[0x10d], idx) == 0) c = 1;
    if (c && getBdatStringColumnValue(mgr, (const char*)&lbl_eu_804FB5D0[0x114], idx) == 0) b = 1;

    if (b) {
        u32 r40 = getBdatStringColumnValue(mgr, (const char*)&lbl_eu_804FB5D0[0x11b], idx);
        u32 r3c = getBdatStringColumnValue(mgr, (const char*)&lbl_eu_804FB5D0[0x126], idx);
        u32 r38 = getBdatStringColumnValue(mgr, (const char*)&lbl_eu_804FB5D0[0x132], idx);
        u8 v = (u8)r40;
        if (v && (((v + 6) & 0xffff) != 0)) {
            u16 h38 = (u16)r38;
            if (h38 != 0) {
                func_80082088__Q22cf13CfGameManagerFv();
            } else {
                func_80082088__Q22cf13CfGameManagerFv();
            }
        }
    }
    func_80082060__Q22cf13CfGameManagerFv();
}


u8 func_8007BAE4() {
    u8 result = 0;
    if (lbl_eu_80663DF0) {
        result = lbl_eu_80663DF0->field_0x46;
    }
    return result;
}

// Per-frame cam-event advance (state machine). Dispatches the slot's
// vtable+0x20 update, then - when the cam update returned true, the state is
// 2 and the global flag is clear - validates two camera vector squared-
// lengths: each is clamped to [lbl_eu_80666468, 1.0], range-checked (with an
// nw4r warning on violation), acos'd and compared against a threshold; when
// both angles are within the threshold r30 is cleared. Retail computes the
// squared lengths with paired-single dot products over a 12-word block copy.
void func_8007BAFC(CfCamEventManager* self) {
    if (self->field_0x3C == 1) {
        CfCamEventSlotView* s0 = (CfCamEventSlotView*)self->slots[0];
        if (s0 != 0) {
            s0->vtable->fn_0x20(s0);
        }
    }

    int state = self->field_0x3C;
    CfCamEventSlotView* slot = (CfCamEventSlotView*)self->slots[state];
    if (slot == 0) return;

    u32 flag = 0;
    if (lbl_eu_80663DF0 != 0) flag = lbl_eu_80663DF0->field_0x46;

    int r30 = func_80079E04(self);
    if (r30 != 0 && state == 2 && flag == 0) {
        CamEventVecBlock* blk = (CamEventVecBlock*)slot->field_0x0C;
        slot->vtable->fn_0x20(slot);

        // Squared lengths of the two camera triplets (retail: PS dots).
        f32 f31 = blk->f_9C * blk->f_9C + blk->f_A0 * blk->f_A0 +
                  blk->f_A4 * blk->f_A4;
        f32 f30 = blk->f_AC * blk->f_AC + blk->f_B0 * blk->f_B0 +
                  blk->f_B4 * blk->f_B4;

        // Clamp f31 to [lbl_eu_80666468, 1.0], then validate the range.
        if (f31 < lbl_eu_80666468) {
            f31 = lbl_eu_80666468;
        } else if (f31 > lbl_eu_80666428) {
            f31 = lbl_eu_80666428;
        }
        int ok = 0;
        if (f31 <= lbl_eu_80666428 && f31 >= lbl_eu_80666468) ok = 1;
        if (ok == 0) {
            Warning__Q24nw4r2dbFPCciPCce(lbl_eu_805262F0, 0xef, lbl_eu_805262C8);
        }

        f32 a1 = acosf(f31);
        if (a1 <= lbl_eu_8066646C * lbl_eu_8066A210) {
            // Clamp f30 to [lbl_eu_80666468, 1.0], then validate the range.
            if (f30 < lbl_eu_80666468) {
                f30 = lbl_eu_80666468;
            } else if (f30 > lbl_eu_80666428) {
                f30 = lbl_eu_80666428;
            }
            int ok2 = 0;
            if (f30 <= lbl_eu_80666428 && f30 >= lbl_eu_80666468) ok2 = 1;
            if (ok2 == 0) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_805262F0, 0xef, lbl_eu_805262C8);
            }

            f32 a2 = acosf(f30);
            if (a2 <= lbl_eu_8066646C * lbl_eu_8066A210) {
                r30 = 0;
            }
        }
    } else {
        slot->vtable->fn_0x20(slot);
    }

    if (r30 != 0) {
        func_80085878__Q22cf13CfGameManagerFv();
        func_8016FD84(lbl_eu_80666470, lbl_eu_80666428);
    }

    if (self->field_0x38 != 0) {
        func_80240AAC(self->field_0x38);
        func_80240B10(self->field_0x38, slot->field_0x0C);
    }
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static initializer: fills a local prototype array and the global cam-table
// (0x805273C8, 12 entries x 0x34 with two aim vectors each) with the same
// 24 triplets. The local copy is discarded (stack frame 0x130 = 0x120 local
// + linkage); MWCC hoists the 12 sdata2 constants into f5..f11 / f0..f4.
void sinit_8007BE74() {
    ml::CVec3 local[24];
    local[0].set(lbl_eu_80666458, lbl_eu_80666458, lbl_eu_8066641C);
    lbl_eu_805273C8[0].v0.set(lbl_eu_80666458, lbl_eu_80666458, lbl_eu_8066641C);
    local[1].set(lbl_eu_80666458, lbl_eu_80666458, lbl_eu_8066641C);
    lbl_eu_805273C8[0].v1.set(lbl_eu_80666458, lbl_eu_80666458, lbl_eu_8066641C);
    local[2].set(lbl_eu_80666474, lbl_eu_80666474, lbl_eu_8066641C);
    lbl_eu_805273C8[1].v0.set(lbl_eu_80666474, lbl_eu_80666474, lbl_eu_8066641C);
    local[3].set(lbl_eu_80666474, lbl_eu_80666474, lbl_eu_8066641C);
    lbl_eu_805273C8[1].v1.set(lbl_eu_80666474, lbl_eu_80666474, lbl_eu_8066641C);
    local[4].set(lbl_eu_80666478, lbl_eu_80666478, lbl_eu_8066641C);
    lbl_eu_805273C8[2].v0.set(lbl_eu_80666478, lbl_eu_80666478, lbl_eu_8066641C);
    local[5].set(lbl_eu_80666478, lbl_eu_80666478, lbl_eu_8066641C);
    lbl_eu_805273C8[2].v1.set(lbl_eu_80666478, lbl_eu_80666478, lbl_eu_8066641C);
    local[6].set(lbl_eu_8066647C, lbl_eu_8066647C, lbl_eu_80666478);
    lbl_eu_805273C8[3].v0.set(lbl_eu_8066647C, lbl_eu_8066647C, lbl_eu_80666478);
    local[7].set(lbl_eu_8066647C, lbl_eu_8066647C, lbl_eu_80666478);
    lbl_eu_805273C8[3].v1.set(lbl_eu_8066647C, lbl_eu_8066647C, lbl_eu_80666478);
    local[8].set(lbl_eu_80666480, lbl_eu_80666480, lbl_eu_80666484);
    lbl_eu_805273C8[4].v0.set(lbl_eu_80666480, lbl_eu_80666480, lbl_eu_80666484);
    local[9].set(lbl_eu_80666480, lbl_eu_80666480, lbl_eu_80666484);
    lbl_eu_805273C8[4].v1.set(lbl_eu_80666480, lbl_eu_80666480, lbl_eu_80666484);
    local[10].set(lbl_eu_80666480, lbl_eu_80666480, lbl_eu_80666484);
    lbl_eu_805273C8[5].v0.set(lbl_eu_80666480, lbl_eu_80666480, lbl_eu_80666484);
    local[11].set(lbl_eu_80666480, lbl_eu_80666480, lbl_eu_80666484);
    lbl_eu_805273C8[5].v1.set(lbl_eu_80666480, lbl_eu_80666480, lbl_eu_80666484);
    local[12].set(lbl_eu_8066641C, lbl_eu_80666458, lbl_eu_8066641C);
    lbl_eu_805273C8[6].v0.set(lbl_eu_8066641C, lbl_eu_80666458, lbl_eu_8066641C);
    local[13].set(lbl_eu_80666478, lbl_eu_80666458, lbl_eu_8066641C);
    lbl_eu_805273C8[6].v1.set(lbl_eu_80666478, lbl_eu_80666458, lbl_eu_8066641C);
    local[14].set(lbl_eu_80666458, lbl_eu_80666474, lbl_eu_80666458);
    lbl_eu_805273C8[7].v0.set(lbl_eu_80666458, lbl_eu_80666474, lbl_eu_80666458);
    local[15].set(lbl_eu_80666488, lbl_eu_80666478, lbl_eu_8066641C);
    lbl_eu_805273C8[7].v1.set(lbl_eu_80666488, lbl_eu_80666478, lbl_eu_8066641C);
    local[16].set(lbl_eu_80666478, lbl_eu_80666458, lbl_eu_80666458);
    lbl_eu_805273C8[8].v0.set(lbl_eu_80666478, lbl_eu_80666458, lbl_eu_80666458);
    local[17].set(lbl_eu_8066648C, lbl_eu_8066647C, lbl_eu_80666478);
    lbl_eu_805273C8[8].v1.set(lbl_eu_8066648C, lbl_eu_8066647C, lbl_eu_80666478);
    local[18].set(lbl_eu_8066641C, lbl_eu_80666458, lbl_eu_8066641C);
    lbl_eu_805273C8[9].v0.set(lbl_eu_8066641C, lbl_eu_80666458, lbl_eu_8066641C);
    local[19].set(lbl_eu_80666478, lbl_eu_80666474, lbl_eu_8066641C);
    lbl_eu_805273C8[9].v1.set(lbl_eu_80666478, lbl_eu_80666474, lbl_eu_8066641C);
    local[20].set(lbl_eu_80666478, lbl_eu_80666474, lbl_eu_80666458);
    lbl_eu_805273C8[10].v0.set(lbl_eu_80666478, lbl_eu_80666474, lbl_eu_80666458);
    local[21].set(lbl_eu_80666490, lbl_eu_80666464, lbl_eu_8066647C);
    lbl_eu_805273C8[10].v1.set(lbl_eu_80666490, lbl_eu_80666464, lbl_eu_8066647C);
    local[22].set(lbl_eu_80666474, lbl_eu_80666480, lbl_eu_80666458);
    lbl_eu_805273C8[11].v0.set(lbl_eu_80666474, lbl_eu_80666480, lbl_eu_80666458);
    local[23].set(lbl_eu_80666428, lbl_eu_8066647C, lbl_eu_8066647C);
    lbl_eu_805273C8[11].v1.set(lbl_eu_80666428, lbl_eu_8066647C, lbl_eu_8066647C);
}
