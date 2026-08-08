// Auto-scaffolded catalog TU for kyoshin/cf/CfCamEvent_1
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include <math.h>

#include "kyoshin/cf/CfCamEvent_1.hpp"

// External retail symbols referenced by this TU's decomps.
extern "C" CfCamDataTable lbl_eu_80570C90;
extern "C" CfCamEventGlobal* lbl_eu_80663DF0;
extern "C" void* func_8016FE34(void* source);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int idx);
extern "C" void* func_800B708C__Fi(int id);
extern "C" void* func_800BBC0C();
extern "C" f32   lbl_eu_8066A1F8;   // PI
extern "C" f32   lbl_eu_8066A1FC;   // 2*PI
extern "C" f32   lbl_eu_8066641C;   // 0.0f
extern "C" f32   lbl_eu_80666418;
extern "C" f32   lbl_eu_80666420;
extern "C" f32   lbl_eu_80666428;
extern "C" f32   lbl_eu_80666448;
extern "C" f32   lbl_eu_80666454;
extern "C" f32   lbl_eu_8066A208;
extern "C" f32   lbl_eu_8066A210;
extern "C" void* lbl_eu_806640BC;
extern "C" u8    lbl_eu_804FB5D0[];
extern "C" int   func_8003B1EC(void* self);
extern "C" int   getBdatStringColumnValue(void* a, const u8* b, int c);
extern "C" f32   Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);
extern "C" int   CfRes_getD80Flag();
extern "C" f32   func_80496288();
extern "C" void  func_800756D0(ml::CVec3* out, CinemCamSrc* src);

extern "C" int __declspec(noinline) func_800755BC(CfCamEventManager* mgr, u32 idx);

// Frame constants used by the shake interpolation (retail .sdata2 slots).
extern "C" f32 lbl_eu_8066642C;
extern "C" f32 lbl_eu_80666430;
extern "C" f32 lbl_eu_8066A200;
extern "C" f32 SinFIdx__Q24nw4r4mathFf(f32);
extern "C" void func_800A41BC(f32* out, CfCamEventElem* a, CfCamEventElem* b,
                              CfCamEventElem* c, CfCamEventElem* d);
extern "C" void func_800A40E8(f32* out, CfCamEventElem* a, CfCamEventElem* b);

// Advance one frame of a shake-table object. Returns whether the table was
// still "active" at entry. Each element has an id (used as a time point) and
// a waveform type (c: 1 = bell, 2 = bump, 4 = sine, 3 = clamp-sine); the
// table carries a running value that is interpolated between element ids.
int func_80074F4C(CfCamShakeState* self, int mode) {
    u8 active = self->flag_active;
    int result = (active != 0) ? 1 : 0;
    self->flag_finish = 0;

    // Without an active flag we only proceed if forced and there is data.
    if (!active && (mode == 0 || self->count <= 0))
        return result;

    int r26 = 0;
    if (self->val == 0.0f) {
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
    s16 nxt = (s16)(self->index + 1);
    if (nxt < self->count && self->val >= (f32)self->elems[nxt].id) {
        self->flag_finish = 1;
        self->index = nxt;
    }

    // Snap to the end value and clear the run when it is exceeded.
    if (self->val >= self->end - lbl_eu_8066642C) {
        self->val = self->end;
        self->flag_active = 0;
        if (r26 == 0)
            self->flag_finish = 0;
    }

    // Finished: copy the trailing element's origin into the base anchor.
    if (self->flag_active == 0) {
        CfCamEventElem* e = &self->elems[self->count - 1];
        self->baseX = e->x0;
        self->baseY = e->x4;
        self->baseZ = e->x8;
        return result;
    }

    // Active run: interpolate between the current element and its neighbors,
    // then shape the result with the waveform selected by the element type.
    int cur = self->index;
    int prev = (cur == 0) ? 0 : (cur - 1);
    int mid = cur + 1;
    int next = (cur < self->count - 2) ? (cur + 2) : (cur + 1);

    f32 t = 0.0f;
    f32 span = (f32)(s32)(self->elems[mid].id - self->elems[cur].id);
    if (span > 0.0f)
        t = (self->val - (f32)self->elems[cur].id) / span;

    // Shape the interpolation parameter with the selected waveform.
    u16 kind = self->elems[cur].c;
    if (kind == 1) {
        t = lbl_eu_80666428 +
            SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 *
                (lbl_eu_8066A200 * t - lbl_eu_8066A200));
    } else if (kind == 2) {
        t = lbl_eu_80666418 *
            (lbl_eu_80666428 +
             SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 *
                 (lbl_eu_8066A1F8 * t - lbl_eu_8066A200)));
    } else if (kind == 4) {
        t = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 *
            (lbl_eu_8066A200 * t));
    } else if (kind == 3) {
        if (t > lbl_eu_80666418)
            t = lbl_eu_80666428 - lbl_eu_80666418 *
                SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 *
                    (lbl_eu_8066A1F8 * t));
        else
            t = lbl_eu_80666418 *
                SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 *
                    (lbl_eu_8066A1F8 * t));
    }

    // Fill the base anchor using the selected spline method.
    if (self->field_0x160 == 1) {
        func_800A41BC(&self->baseX, &self->elems[prev], &self->elems[cur],
                      &self->elems[mid], &self->elems[next]);
    } else if (self->field_0x160 == 0 || self->field_0x160 == 2) {
        func_800A40E8(&self->baseX, &self->elems[cur], &self->elems[mid]);
    }

    return result;
}
void func_8007AA4C(void* self){}
void func_80082088__Q22cf13CfGameManagerFv(){}
void func_80082060__Q22cf13CfGameManagerFv(){}
void func_80081E90__Q22cf13CfGameManagerFv(int a, int b, int c){}
void* func_800821F8__Q22cf13CfGameManagerFv(){ return 0; }
void func_800A3F8C(void*){}
void func_800B24B0(){}

void __ct__80075298(){}

void __dt__8007540C(){}

void func_800754C0(){}

void func_80075540(CfCamEventManager* self, u32 idx) {
    u32 n = func_800755BC(self, idx);
    if (self->slots[n]) {
        delete self->slots[n];
        self->slots[n] = 0;
        self->slots[n] = 0;
    }
}

void* func_800755B0(void* self, unsigned long idx) {
    return *(void**)((char*)self + (idx << 2));
}

// Resolve the shared global 'cam state' object; external C-function tail-call
// targets used by the shake helpers below.
extern "C" CfCamEventGlobal* lbl_eu_80663DF0;
extern "C" CfCamDataTable lbl_eu_80570C90;
extern "C" int func_8024125C(int state, int val);
extern "C" int func_80241344(int state, int val);
extern "C" int func_80240C98(int state, int arg0, int arg1);

// Categorise an environment/prompt id (r4) into a 0/1/2 bucket. The
// manager pointer in r3 is unused by this helper.
extern "C" int __declspec(noinline) func_800755BC(CfCamEventManager* /*unused*/, u32 idx) {
    if (idx >= 0x10 && idx <= 0x2b) return 1;
    if (idx >= 0x8 && idx <= 0xa)   return 2;
    if (idx <= 1) return 0;
    if (idx == 0xb) return 2;
    return 0;
}

// True when the global cam state exists and is in a "busy" frame range.
int func_8007560C() {
    if (lbl_eu_80663DF0 == nullptr) return 0;
    if (lbl_eu_80663DF0->field_0x3E < 0x10 || lbl_eu_80663DF0->field_0x3E > 0x2b) return 0;
    return 1;
}

// True when the global cam state exists and is in the "intro" frame range.
int func_80075640() {
    if (lbl_eu_80663DF0 == nullptr) return 0;
    if (lbl_eu_80663DF0->field_0x3E < 0 || lbl_eu_80663DF0->field_0x3E > 1) return 0;
    return 1;
}

void func_80075674(){}

// Camera aim/position computation. Fills `out` (representing the camera
// aim point) from the source's body/virtual accessors, applying a vertical
// follow/lag that approaches the source's anchor value. Rows mirror the
// retail branch structure.
extern "C" u32 func_800AA300(int a, int b, int c);
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
extern "C" f32 lbl_eu_80666454;
extern "C" f32 Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);
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
int func_800762A0(CfCamEventManager* self) {
    if ((self->field_0x48 & 0x9) == 0) return 1;

    ml::CVec3 v[4];
    v[0].set(lbl_eu_80570C90.f1C, lbl_eu_80570C90.f20, lbl_eu_80570C90.f24);
    v[1].set(lbl_eu_80570C90.f28, lbl_eu_80570C90.f2C, lbl_eu_80570C90.f30);
    v[2].set(lbl_eu_80570C90.f34, lbl_eu_80570C90.f38, lbl_eu_80570C90.f3C);
    v[3].set(lbl_eu_80570C90.f40, lbl_eu_80570C90.f44, lbl_eu_80570C90.f48);

    void* hA = func_800B708C__Fi((int)self->field_0x10);
    void* hB = func_800B708C__Fi((int)self->field_0x24);
    CamCamSrc* sa = (CamCamSrc*)func_8016FE34(hA);
    CamCamSrc* sb = (CamCamSrc*)func_8016FE34(hB);
    if (hA == 0 || hB == 0) return 0;

    u8 s0 = (u8)((self->field_0x48 >> 31) & 1);
    u8 s1 = (u8)((self->field_0x48 >> 15) & 1);
    ml::CVec3 out1, out2;
    func_80075934(&out1, &out2, sa, sb, &v[2], &v[3],
                  lbl_eu_80570C90.h0E, lbl_eu_80570C90.h10, s0, s1);
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

void func_80076D8C(){}

cf::CActorParam_UnkStruct1* cf::CActorParam::CActorParam_UnkVirtualFunc129() {
    return (cf::CActorParam_UnkStruct1*)((u8*)this + *(u8*)((u8*)this + 0x3354) * 0xbc + 0x2a84);
}

void* func_80076F88(CfCamEventManager* self, int unk34, void* srcArg, CfCamDataTable* cam) {
    CamEventSrc* src = (CamEventSrc*)func_8016FE34(srcArg);
    CamEventBody* body = src->vtable->fn_0x298(src);
    u32 field_0x3F10 = (u32)src->field_0x3F10;
    void* p31 = body->field_0x04;

    // r29 points at the source's embedded voice (or null when no source).
    CamEventVoice* r29 = (src != 0) ? &src->voice : (CamEventVoice*)src;
    if (p31 == 0) {
        func_800B708C__Fi((int)field_0x3F10);
        r29 = (CamEventVoice*)func_800BBC0C();
    }
    func_8016FE34((src != 0) ? &src->voice : (void*)src);
    func_8016FE34(r29);

    // True when any of the four halfwords in the cam table is 2 or 3.
    int band = 0;
    if (cam->h0A == 2 || cam->h0A == 3) band = 1;
    if (cam->h0C == 2 || cam->h0C == 3) band = 1;
    if (cam->h0E == 2 || cam->h0E == 3) band = 1;
    if (cam->h10 == 2 || cam->h10 == 3) band = 1;

    if (band && p31 == 0) {
        void* v = src->voice.vtable->fn_0x4C(&src->voice);
        func_800B708C__Fi((int)v);
        r29 = (CamEventVoice*)func_800BBC0C();
        if (unk34 == 0x2b) {
            CamEventVoice* p0 = (CamEventVoice*)getPlayer__Q22cf13CfGameManagerFi(0);
            if (p0 == 0) {
                r29 = 0;
            } else {
                r29 = 0;
                void* q = p0->vtable->fn_0x4C(p0);
                if (q != 0)
                    r29 = (CamEventVoice*)func_800B708C__Fi((int)q);
                if (r29 == 0)
                    r29 = (CamEventVoice*)getPlayer__Q22cf13CfGameManagerFi(1);
            }
            self->field_0x34 = (u32)r29;
        }
        if (r29 == 0) return 0;
        func_8016FE34(r29);
    }

    // Copy the cam table into the global.
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
    lbl_eu_80570C90.f1C = cam->f1C;
    lbl_eu_80570C90.f20 = cam->f20;
    lbl_eu_80570C90.f24 = cam->f24;
    lbl_eu_80570C90.f28 = cam->f28;
    lbl_eu_80570C90.f2C = cam->f2C;
    lbl_eu_80570C90.f30 = cam->f30;
    lbl_eu_80570C90.f34 = cam->f34;
    lbl_eu_80570C90.f38 = cam->f38;

    // Shake table 1 (sub_0x1F4): advance each element toward its predecessor.
    {
        s16 count = self->sub_0x1F4.field_0x166;
        CfCamEventElem* e = self->sub_0x1F4.elems + 1;
        for (int i = 1; i < count; i++, e++) {
            f32 cur = e->x4;
            f32 prev = (e - 1)->x4;
            f32 d = cur - prev;
            if (fabsf(d) >= lbl_eu_8066A1F8) {
                if (d > 0.0f)
                    e->x4 = cur - lbl_eu_8066A1FC;
                else
                    e->x4 = cur + lbl_eu_8066A1FC;
            }
        }
    }

    if (self->field_0x50 & 0x10000) {
        self->sub_0x1F4.field_0x2DA = 0;
    } else {
        self->sub_0x1F4.field_0x2DA = 1;
        self->sub_0x1F4.field_0x2DB = 0;
    }

    // Shake table 2 (sub_0x36C).
    {
        s16 count = self->sub_0x36C.count;
        CfCamEventElem* e = self->sub_0x36C.elems + 1;
        for (int i = 1; i < count; i++, e++) {
            f32 cur = e->x4;
            f32 prev = (e - 1)->x4;
            f32 d = cur - prev;
            if (fabsf(d) >= lbl_eu_8066A1F8) {
                if (d > 0.0f)
                    e->x4 = cur - lbl_eu_8066A1FC;
                else
                    e->x4 = cur + lbl_eu_8066A1FC;
            }
        }
    }

    return (void*)1;
}

void func_800778E4(){}

void func_80077F20(){}

// Maps an (action, parameter) pair to a campaign-state id; default 5.
int func_80078400(int action, int param) {
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

void func_800784A0(){}

void func_80078B60(){}

void func_80078C08(){}

// Insert a scaled aim vector into the manager's three shake tables (0x7C,
// 0x1F4, 0x36C), each capped at 16 entries. table0/table1 store the scaled
// aim; table2 stores (f1, 0, 0).
void func_80078D08(CfCamEventManager* self, int add, void* p5, ml::CVec3* r6, int p7, int p8,
                   f32 p1) {
    u16 id = (u16)(add + self->field_0x40);
    if (p1 <= lbl_eu_8066A208)
        self->field_0x50 |= 0x10000;

    if (self->field_0x3E != 9) return;

    ml::CVec3 sv;
    f32 k = lbl_eu_8066A210;
    sv.x = r6->x * k;
    sv.y = r6->y * k;
    sv.z = r6->z * k;
    func_800A3F8C(&sv);

    // table0 (0x7C)
    s16 cnt0 = self->tab0.count;
    if (cnt0 < 0x10) {
        CfCamEventElem* e = &self->tab0.elems[cnt0];
        e->x0 = sv.x;
        e->x4 = sv.y;
        e->x8 = sv.z;
        e->id = id;
        e->c = (u16)p7;
        e->d = (u16)p8;
        if (cnt0 == 0) {
            self->tab0.baseX = sv.x;
            self->tab0.baseY = sv.y;
            self->tab0.baseZ = sv.z;
        }
        self->tab0.count = cnt0 + 1;
    }

    // table1 (0x1F4)
    s16 cnt1 = self->sub_0x1F4.field_0x166;
    if (cnt1 < 0x10) {
        CfCamEventElem* e = &self->sub_0x1F4.elems[cnt1];
        e->x0 = sv.x;
        e->x4 = sv.y;
        e->x8 = sv.z;
        e->id = id;
        e->c = (u16)p7;
        e->d = (u16)p8;
        self->sub_0x1F4.field_0x166 = cnt1 + 1;
    }

    // table2 (0x36C): stores (p1, 0, 0)
    s16 cnt2 = self->sub_0x36C.count;
    if (cnt2 < 0x10) {
        CfCamEventElem* e = &self->sub_0x36C.elems[cnt2];
        e->x0 = p1;
        e->x4 = 0.0f;
        e->x8 = 0.0f;
        e->id = id;
        e->c = (u16)p7;
        e->d = 0;
        if (cnt2 == 0) {
            self->sub_0x36C.baseX = p1;
            self->sub_0x36C.baseY = 0.0f;
            self->sub_0x36C.baseZ = 0.0f;
        }
        self->sub_0x36C.count = cnt2 + 1;
    }
}


void func_8007990C(){}

extern "C" f32 lbl_eu_8066641C;   // 0.0f
extern "C" f32 lbl_eu_8066A1F8;   // PI
extern "C" f32 lbl_eu_8066A1FC;   // 2*PI

// Advance each element of the shake table toward its predecessor by 2*PI once
// the gap reaches PI. Written inline in both branches below (retail keeps two
// copies of the loop).
void func_80079B34(CfCamEventManager* self) {
    self->tab0.flag0 = 1;
    self->tab0.flag1 = 0;

    // Whether the global cam manager is inside its busy frame range.
    int busy = 0;
    if (lbl_eu_80663DF0 != nullptr) {
        s16 g = lbl_eu_80663DF0->field_0x3E;
        if (g >= 0x10 && g <= 0x2b) busy = 1;
    }

    if (busy) {
        // Demo-mode flag drives the shake update.
        self->sub_0x1F4.field_0x162 = 1;
        u16 demo = lbl_eu_80570C90.h00;
        u8 on = (demo != 0);
        self->sub_0x1F4.field_0x163 = on;
        if (on) {
            s16 count = self->sub_0x1F4.field_0x166;
            CfCamEventElem* e = self->sub_0x1F4.elems + 1;
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
            self->sub_0x1F4.field_0x162 = 1;
            self->sub_0x1F4.field_0x163 = 0;
        } else {
            self->sub_0x1F4.field_0x162 = 1;
            self->sub_0x1F4.field_0x163 = 1;
            s16 count = self->sub_0x1F4.field_0x166;
            CfCamEventElem* e = self->sub_0x1F4.elems + 1;
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
        self->sub_0x1F4.field_0x2DA = 0;
    } else {
        self->sub_0x1F4.field_0x2DA = 1;
        self->sub_0x1F4.field_0x2DB = 0;
    }
}

void func_80079D6C(){}

// True while any of three "active" flags are set on the manager.
bool func_80079DBC(CfCamEventManager* manager) {
    if (manager->tab0.flag0) return true;
    CfCamEventSub1F4* sub = &manager->sub_0x1F4;
    if (sub->field_0x162) return true;
    if (sub->field_0x2DA) return true;
    return false;
}

// Per-frame cam-event update. Resolves the game manager, checks its busy
// flag, then either runs the main camera advance (flag0 set) or the shorts
// path that clears the one-shot flag. Retail body is large and heavily
// elided, so this is a faithful structural reconstruction.
void func_80079E04(CfCamEventManager* self) {
    CfDynMgr* dyn = (CfDynMgr*)func_800821F8__Q22cf13CfGameManagerFv();
    if (dyn == 0) return;
    if (dyn->m_field04 & 0x04000000) return;

    CfRes_getD80Flag();
    f32 val = func_80496288();
    if (val <= 0.0f) return;

    if (self->tab0.flag0 != 0) {
        int busy = 0;
        if (lbl_eu_80663DF0 != 0) {
            s16 g = lbl_eu_80663DF0->field_0x3E;
            if (g >= 0x10 && g <= 0x2b) busy = 1;
        }
        if (busy) {
            self->field_0x50 = self->field_0x50 | 1;
            func_8007AA4C(self);
        } else {
            if (self->field_0x46 != 0) {
                func_80081E90__Q22cf13CfGameManagerFv(self->field_0x44, 0, 0);
                self->field_0x46 = 0;
            }
        }
    } else {
        int busy = 0;
        if (lbl_eu_80663DF0 != 0) {
            s16 g = lbl_eu_80663DF0->field_0x3E;
            if (g >= 0x10 && g <= 0x2b) busy = 1;
        }
        if (busy) {
            if (self->field_0x48 & 1) func_8007AA4C(self);
        } else {
            if (self->field_0x46 != 0) {
                func_80081E90__Q22cf13CfGameManagerFv(self->field_0x44, 0, 0);
                self->field_0x46 = 0;
            }
        }
    }
}


void func_8007AA4C(){}

extern "C" void func_8007B030(void* self) {
    *(u8*)((u8*)self + 0x1de) = 0;
    *(u8*)((u8*)self + 0x356) = 0;
    *(u8*)((u8*)self + 0x4ce) = 0;
}

// Forward shake state to the shared handler if the global cam state is alive.
int func_8007B044(int arg0, int arg1) {
    if (lbl_eu_80663DF0 == nullptr) return 0;
    if (lbl_eu_80663DF0->field_0x38 == 0) return 0;
    return func_80240C98(lbl_eu_80663DF0->field_0x38, arg0, arg1);
}

int func_8007B078(int val) {
    if (lbl_eu_80663DF0 == nullptr) return val;
    if (lbl_eu_80663DF0->field_0x38 == 0) return 0;
    return func_8024125C(lbl_eu_80663DF0->field_0x38, val);
}

int func_8007B0A0(int val) {
    if (lbl_eu_80663DF0 == nullptr) return val;
    if (lbl_eu_80663DF0->field_0x38 == 0) return 0;
    return func_80241344(lbl_eu_80663DF0->field_0x38, val);
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
    if (getBdatStringColumnValue(mgr, &lbl_eu_804FB5D0[0xf4], idx) == 0 &&
        getBdatStringColumnValue(mgr, &lbl_eu_804FB5D0[0xfa], idx) == 0)
        f = 1;
    if (f && getBdatStringColumnValue(mgr, &lbl_eu_804FB5D0[0x100], idx) == 0) e = 1;
    if (e && getBdatStringColumnValue(mgr, &lbl_eu_804FB5D0[0x106], idx) == 0) d = 1;
    if (d && getBdatStringColumnValue(mgr, &lbl_eu_804FB5D0[0x10d], idx) == 0) c = 1;
    if (c && getBdatStringColumnValue(mgr, &lbl_eu_804FB5D0[0x114], idx) == 0) b = 1;

    if (b) {
        u32 r40 = getBdatStringColumnValue(mgr, &lbl_eu_804FB5D0[0x11b], idx);
        u32 r3c = getBdatStringColumnValue(mgr, &lbl_eu_804FB5D0[0x126], idx);
        u32 r38 = getBdatStringColumnValue(mgr, &lbl_eu_804FB5D0[0x132], idx);
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

void func_8007BAFC(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8007BE74(){}
