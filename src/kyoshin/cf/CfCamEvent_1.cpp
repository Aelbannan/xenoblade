// Auto-scaffolded catalog TU for kyoshin/cf/CfCamEvent_1
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/scn/CScnTimeApi.hpp"

#include "kyoshin/cf/CfGameManagerData.hpp"
#include <monolib/math/Random.hpp>
#include "monolib/util/FixStr.hpp"
#include <math.h>

#include "kyoshin/cf/CfCamEvent_1.hpp"
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

// bdat column byte read: the (u8) truncation of the u32 column value, which
// MWCC lowers through a stack temp (stw + lbz) when inlined via a helper
// (same shape as CfGimmickElv.cpp's getCol8/getCol16 helpers).
static inline u8 getCol8(void* table, const char* col, int row) {
    return (u8)getBdatStringColumnValue(table, col, row);
}

// Outlined helpers defined further down in this TU are declared in
// CfCamEvent_1.hpp; declaring them before any use keeps MWCC from inlining
// Word view of a camera-vector triplet: retail copies the slot accessor
// results as raw GPR words (load pair high+low, store low/high, then the
// single), so the snapshot block goes through this POD view instead of
// ml::CVec3::operator= (which would emit an out-of-line __as__ call).
// CamEventVecWords itself is defined in CfCamEvent_1.hpp.

// Memory-resident triplet locals (POD: ml::CVec3 would emit default ctor
// calls). Used by func_800778E4's column reads and func_800784A0's out
// vectors.
struct CamTripletLocals {
    f32 x, y, z;
};

// Raw 0x28-byte vector snapshot handed to func_8007990C. Word-based so no
// CVec3 ctor/operator= calls are emitted - retail fills it with raw GPR
// word copies from the slot accessors.
struct CamEventVecSrcRaw {
    CamEventVecWords v0;   // +0x00
    CamEventVecWords v1;   // +0x0C
    CamEventVecWords v2;   // +0x18
    f32 f_24;              // +0x24
};

// Re-seat a cam slot. Categorises `first` into a 0/1/2 slot index via
// func_800755BC; when the slot at that index does not already hold the
// current cam state the old slot is deleted and a new one constructed from
// `second` (with the pose returned by func_80496264 as its second ctor
// arg). Then every slot is handed off to func_8006BC1C and the freshly
// seated slot woken via func_8006BBF4.
// Category 0 forwards the outgoing slot's vectors through func_8006D450;
// category 1 rebuilds a cam table from `sixth` (or falls back to
// func_800778E4) and runs the shared follow-cam setup func_80076F88.
// NOTE: the fourth func_80077F20 call passes `first` as its source argument
// (retail quirk - the source argument is the category id, not the voice).
CfCamEventObj* func_800784A0(u32 first, CfCamEventObj* second,
                              CfCamEventObj* third, CfCamEventObj* fourth,
                              CfCamEventObj* fifth,
                              CfCamDataTable* sixth, CfCamEventObj* seventh) {
    CfCamEventManager* mgr = (CfCamEventManager*)lbl_eu_80663DF0;
    if (mgr == 0) return 0;

    // Retail reads the current state word twice: a throwaway temp picks the
    // outgoing slot, then the state is re-loaded after the categorize call.
    CfCamEventSlot* cur = mgr->slots[mgr->field_0x3C];
    int idx = func_800755BC(mgr, first);
    CfCamEventManager* m = (CfCamEventManager*)lbl_eu_80663DF0;
    s16 oldState = m->field_0x3C;

    // Snapshot the outgoing slot's four camera vectors into the manager
    // before it gets replaced (retail duplicates this block per branch).
    if (first == 0) {
        if (oldState != 0 && cur != 0) {
            CfCamEventManager* m = (CfCamEventManager*)lbl_eu_80663DF0;
            CamEventVecWords* v =
                (CamEventVecWords*)((CfCamEventSlotObj*)cur)->vtable->fn_0x10(cur);
            CamEventVecWords* d = (CamEventVecWords*)&m->field_0x54;
            u32 vy = v->w[1];
            u32 vx = v->w[0];
            d->w[0] = vx;
            d->w[1] = vy;
            d->w[2] = v->w[2];
            v = (CamEventVecWords*)((CfCamEventSlotObj*)cur)->vtable->fn_0x34(cur);
            d = (CamEventVecWords*)&m->field_0x60;
            vy = v->w[1];
            vx = v->w[0];
            d->w[0] = vx;
            d->w[1] = vy;
            d->w[2] = v->w[2];
            v = (CamEventVecWords*)((CfCamEventSlotObj*)cur)->vtable->fn_0x1C(cur);
            d = (CamEventVecWords*)&m->field_0x6C;
            vy = v->w[1];
            vx = v->w[0];
            d->w[0] = vx;
            d->w[1] = vy;
            d->w[2] = v->w[2];
            m->field_0x78 = ((CfCamEventSlotObj*)cur)->vtable->fn_0x58(cur);
        }
    } else {
        if (cur != 0) {
            CfCamEventManager* m = (CfCamEventManager*)lbl_eu_80663DF0;
            CamEventVecWords* v =
                (CamEventVecWords*)((CfCamEventSlotObj*)cur)->vtable->fn_0x10(cur);
            CamEventVecWords* d = (CamEventVecWords*)&m->field_0x54;
            u32 vy = v->w[1];
            u32 vx = v->w[0];
            d->w[0] = vx;
            d->w[1] = vy;
            d->w[2] = v->w[2];
            v = (CamEventVecWords*)((CfCamEventSlotObj*)cur)->vtable->fn_0x34(cur);
            d = (CamEventVecWords*)&m->field_0x60;
            vy = v->w[1];
            vx = v->w[0];
            d->w[0] = vx;
            d->w[1] = vy;
            d->w[2] = v->w[2];
            v = (CamEventVecWords*)((CfCamEventSlotObj*)cur)->vtable->fn_0x1C(cur);
            d = (CamEventVecWords*)&m->field_0x6C;
            vy = v->w[1];
            vx = v->w[0];
            d->w[0] = vx;
            d->w[1] = vy;
            d->w[2] = v->w[2];
            m->field_0x78 = ((CfCamEventSlotObj*)cur)->vtable->fn_0x58(cur);
        }
    }

    // Rebuild the target slot unless the current cam state already sits in
    // it with a live object.
    if (!(((CfCamEventManager*)lbl_eu_80663DF0)->field_0x3C == idx &&
          ((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx] != 0)) {
        if (((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx] == 0) {
            CfCamEventManager* g = (CfCamEventManager*)lbl_eu_80663DF0;
            g->tab0.flag_active = 0;
            g->shake[0].field_0x162 = 0;
            g->shake[1].field_0x162 = 0;
            CfCamEventManager* g2 = (CfCamEventManager*)lbl_eu_80663DF0;
            CfCamEventSlot* p = g2->slots[idx];
            if (p != 0) {
                if (p != 0) {
                    if (p != 0) {
                        delete p;
                    }
                    g2->slots[idx] = 0;
                }
                g2->slots[idx] = 0;
            }
            g2->field_0x47 = 0;
            CfCamEventObj* pose = (CfCamEventObj*)func_80496264((void*)CfRes_getD80Flag(), 0);
            if (idx == 0) {
                g2->slots[idx] = (CfCamEventSlot*)func_80074CEC(second, pose);
            } else if (idx == 2) {
                g2->slots[idx] = (CfCamEventSlot*)__ct__8006B310(second, pose);
            } else if (idx == 1) {
                g2->slots[idx] = (CfCamEventSlot*)__ct__8006B310(second, pose);
            }
        }
        ((CfCamEventManager*)lbl_eu_80663DF0)->field_0x3C = (s16)idx;
    }

    // Hand every slot off to the shared cam-event cleanup, then wake the
    // freshly-seated slot.
    {
        CfCamEventManager* g = (CfCamEventManager*)lbl_eu_80663DF0;
        for (int i = 0; i < 3; i++) {
            if (g->slots[i] != 0) func_8006BC1C(g->slots[i], 0x8000);
        }
        func_8006BBF4(((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx],
                      0x8000, 1);
    }

    if (cur != 0) {
        ((CfCamEventManager*)lbl_eu_80663DF0)->field_0x46 = 0;
        if (idx == 0) {
            CfCamEventSlotObj* s0 = (CfCamEventSlotObj*)
                ((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx];
            if (s0 != 0) {
                s0->vtable->fn_0x5C(s0, second);
            }
            // Feed the outgoing slot's vectors into the new slot; the
            // trailing three arguments select blend-vs-hold mode.
            CfCamEventSlotObj* cs = (CfCamEventSlotObj*)cur;
            f32 f = cs->vtable->fn_0x58(cs);
            ml::CVec3* p34 = cs->vtable->fn_0x34(cs);
            ml::CVec3* p10 = cs->vtable->fn_0x10(cs);
            if (third != 0) {
                func_8006D450(
                    ((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx],
                    p10, p34, f, 0, third, seventh);
            } else {
                func_8006D450(
                    ((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx],
                    p10, p34, f, fourth, 0, 0);
            }
        } else if (idx == 1) {
            CfCamEventObj* result;
            if (sixth != 0) {
                // Stack temporaries declared low-to-high to mirror retail's
                // stack map (out vectors at 0x08..0x2C, table at 0x38).
                // Plain POD triplets: ml::CVec3 locals would emit default
                // ctor calls that retail does not have.
                CamTripletLocals toutD;
                CamTripletLocals toutC;
                CamTripletLocals toutB;
                CamTripletLocals toutA;
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

                // Resolve the follow object: the source's voice sub-object,
                // falling back to player 1.
                CfCamEventObj* player = 0;
                if (second != 0) {
                    CfCamEventObj* v =
                        (CfCamEventObj*)((CamEventVoice*)second)->vtable->fn_0x4C(second);
                    if (v != 0) player = (CfCamEventObj*)func_800B708C__Fi((int)v);
                    if (player == 0)
                        player = (CfCamEventObj*)getPlayer__Q22cf13CfGameManagerFi(1);
                }

                u16 c1 = (u16)func_80078400(sixth->h0A, sixth->h0C);
                u16 c2 = (u16)func_80078400(sixth->h0E, sixth->h10);
                func_80077F20(&toutA, second, player, c1,
                              sixth->h0A, &sixth->f1C);
                tbl.f1C = toutA.x;
                tbl.f20 = toutA.y;
                tbl.f24 = toutA.z;
                func_80077F20(&toutB, second, player, c1,
                              sixth->h0C, &sixth->f28);
                tbl.f28 = toutB.x;
                tbl.f2C = toutB.y;
                tbl.f30 = toutB.z;
                func_80077F20(&toutC, second, player, c2,
                              sixth->h0E, &sixth->f34);
                tbl.f34 = toutC.x;
                tbl.f38 = toutC.y;
                tbl.f3C = toutC.z;
                // Retail quirk: this call passes `first`, not `second`.
                func_80077F20(&toutD, (CfCamEventObj*)first, player, c2,
                              sixth->h10, &sixth->f40);
                tbl.f40 = toutD.x;
                tbl.f44 = toutD.y;
                tbl.f48 = toutD.z;
                result = (CfCamEventObj*)func_80076F88((CfCamEventManager*)lbl_eu_80663DF0,
                                       first, second, &tbl);
            } else {
                result = (CfCamEventObj*)func_800778E4((CfCamEventManager*)lbl_eu_80663DF0,
                                       first, second, (u32)fifth);
            }
            if (result == 0 && oldState != idx) {
                func_8006BC1C(
                    ((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx], 0x8000);
                func_8006BBF4(
                    ((CfCamEventManager*)lbl_eu_80663DF0)->slots[0], 0x8000, 1);
                idx = 0;
                first = 0;
                ((CfCamEventManager*)lbl_eu_80663DF0)->field_0x3C = 0;
            }
        }
    }

    ((CfCamEventManager*)lbl_eu_80663DF0)->field_0x3E = (s16)first;
    return (CfCamEventObj*)((CfCamEventManager*)lbl_eu_80663DF0)->slots[idx];
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
    int startedFinished = 0;
    if (lbl_eu_8066641C == self->val) {
        self->flag_finish = 1;
        startedFinished = 1;
    }

    // Advance the running value by a frame step.
    f32 delta;
    if (CfRes_getD80Flag()) {
        CfRes_getD80Flag();
        delta = func_80496288(lbl_eu_80663E14);
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
        if (startedFinished == 0)
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
void func_80082088__Q22cf13CfGameManagerFv(int idx, void* vecA, void* vecB, int mode, f32 val){}
void func_80082008__Q22cf13CfGameManagerFv(int a, u8 b, int c, int d, int e){}
void func_80081E90__Q22cf13CfGameManagerFv(int a, int b, int c){}
// Vector-normalize helper; the body is provided by this TU (declared in
// CfCamEvent_1.hpp). Callers pass raw triplet/element storage, so the
// call sites view it through the CVec3 parameter type.
extern void func_800A3F8C(ml::CVec3*) {}
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
    tab0.flag_active = 0;
    tab0.flag_finish = 0;
    tab0.count = 0;
    tab0.field_0x168 = 0;
    for (CfCamEventShakeUnit* u = shake; u < &shake[2]; u++) {
        u->field_0x162 = 0;
        u->field_0x164 = 0;
        u->field_0x166 = 0;
        u->field_0x168 = 0;
    }
    lbl_eu_80663DF0 = (CfCamEventGlobal*)this;
    memset(this, 0, 0xc);
    // Word-view copies: ml::CVec3 assignment emits an out-of-line __as__ call
    // and member-wise float copies emit lfs/stfs chains; retail inlines these
    // as raw GPR word copies from the zero global.
    // OPEN ITEM (us-80075c34, 37.6%): retail inline-expands the three 12-byte
    // zero-vector copies as lwzu/lwz GPR loads + stw stores; every source shape
    // tried (CVec3 operator= [outlined __as__], member-wise floats [lfs/stfs
    // chain], POD word-struct assign [outlined __as__], memcpy [bl memcpy call])
    // fails to inline. Header operator= on ml::CVec3 also stayed out-of-line.
    // Next angles: hand-rolled u32* copy loop, per-field u32 reads from an
    // extern "C" zero view, or checking how other matched TUs fill CVec3 fields.
    memcpy(&field_0x54, &ml::CVec3::zero, 0xC);
    memcpy(&field_0x60, &ml::CVec3::zero, 0xC);
    memcpy(&field_0x6C, &ml::CVec3::zero, 0xC);
    field_0x78 = lbl_eu_80666440;
    field_0x42 = 0;
    field_0x44 = 0;
    field_0x46 = 0;
    // Declared end-first: MWCC colors these locals in reverse declaration
    // order (p -> r28, mem -> r29, end -> r30), matching retail.
    u8* end;
    u8* mem;
    u8* p;
    mem = (u8*)mtl::MemManager::allocate(0xc40, func_80061FFC());
    if (mem != 0) {
        end = mem + 0xc40;
        p = mem;
        do {
            func_80240878(p);
            p += 0x188;
        } while (p < end);
        func_80240A64(mem);
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
// manager pointer in r3 is unused by this helper. The declaration in
// CfCamEvent_1.hpp keeps the call reloc name unmangled (retail symbol
// func_800755BC).
// NOTE: retail lays the four result blocks at the end (checks fall through
// with jump-on-true); Wii/1.1 -O4,p inlines them with jump-on-false, and a
// goto/switch form that reproduces the end layout instead normalizes the
// == 0xb equality into a setnz chain (same version wall as func_8007560C's
// two-cmpi range check).
// Retail calls these outlined from func_800784A0; dont_inline keeps IPA from
// folding their bodies into the caller (repo convention, cf. CfGameManager).
#pragma dont_inline on
int func_800755BC(CfCamEventManager* /*unused*/, int idx) {
    // Retail lowers these as subi/cmpli unsigned range checks that jump to
    // end-of-function result blocks (jump-on-true).
    if ((u32)(idx - 0x10) <= 27) return 1;
    if ((u32)(idx - 8) <= 2) return 2;
    if ((u32)idx <= 1) return 0;
    if (idx == 11) return 2;
    return 0;
}
#pragma dont_inline reset

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
    // Retail copies the triplets as raw GPR words (lwz/stw pairs), not
    // per-float lfs/stfs - use u32 word views on both sides.
    // Paired word temps reproduce retail's lwz-pair/stw-pair GPR copy shape.
    // OPEN ITEM (us-80076010, 91.3%): retail loads lo THEN hi (lo->r5,
    // hi->r0); every declaration order tried gives either lo-first-load with
    // swapped colors or hi-first-load with matching colors - the load order
    // is bound to the color assignment in MWCC. Witness rejected the reorder
    // (slot-2 bits differ), so this needs the true source idiom: likely a
    // struct copy of a 3-word view that MWCC inline-expands.
    // Named-member word reads with hi born first (retail colors hi->r0);
    // MWCC then schedules the loads by ascending source address.
    u32 hi = src->w_110;
    u32 lo = src->w_10C;
    dst->field_0x54.w[1] = hi;
    dst->field_0x54.w[0] = lo;
    dst->field_0x54.w[2] = src->w_114;
    lo = src->w_138;
    hi = src->w_13C;
    dst->field_0x60.w[1] = hi;
    dst->field_0x60.w[0] = lo;
    dst->field_0x60.w[2] = src->w_140;
    lo = src->w_118;
    hi = src->w_11C;
    dst->field_0x6C.w[1] = hi;
    dst->field_0x6C.w[0] = lo;
    dst->field_0x6C.w[2] = src->w_120;
    dst->field_0x78 = src->f_1E0;
}

// Camera aim/position computation. Fills `out` (the camera aim point) from
// the source's body accessor, optionally applying a vertical snap toward the
// anchor accessor, then clamps/follows against the voice's current vector.
// Body/anchor coordinates are staged through locals so MWCC emits the
// retail stfs-to-stack + GPR block-copy schedule; the follow-snap block is
// textually duplicated because retail emits two identical copies.
void func_800756D0(ml::CVec3* out, CinemCamSrc* src) {
    CinemAimBody* body;
    CinemAimBody* anchor;

    if (src == nullptr)
        goto zero_init;

    body = ((CinemCamSrcI*)src)->getBody(100);
    anchor = ((CinemCamSrcI*)src)->getAnchor();

    // Two independent ifs (no shared diamond join): keeps retail's
    // duplicated follow-snap block from being tail-merged.
    if (body != nullptr) {
        // Named float temps pin the register allocation to retail's
        // right-to-left schedule (z -> f0, y -> f1, x -> f2). All vector
        // staging is member-wise on POD views: ml::CVec3 methods emit
        // out-of-line __ct__/set/__as__ calls that retail inlined.
        CamTripletLocals pos;
        f32 pz = body->z;
        f32 py = body->y;
        f32 px = body->x;
        pos.x = px;
        pos.y = py;
        pos.z = pz;
        ((u32*)out)[0] = ((u32*)&pos)[0];
        ((u32*)out)[1] = ((u32*)&pos)[1];
        ((u32*)out)[2] = ((u32*)&pos)[2];

        if (anchor != nullptr && (src->field_0x64 & 4) &&
            src->field_0x70 == func_800AA300(5, 4, 1)) {
            CamTripletLocals av;
            f32 az = anchor->z;
            f32 ay = anchor->y;
            f32 ax = anchor->x;
            av.x = ax;
            av.y = ay;
            av.z = az;
            f32 d = av.y - ((CinemCamSrcI*)src)->getVecOut()->v.y;
            if (d > lbl_eu_8066641C)
                out->y += d;
        }

        CinemVecOut* vo = ((CinemCamSrcI*)src)->getVecOut();
        if (out->y > vo->v.y) {
            if (src->field_0x64 & 2) {
                out->y -= lbl_eu_80666444;
            } else {
                f32 rel = out->y - vo->v.y;
                if (rel >= lbl_eu_80666448)
                    out->y = lbl_eu_8066644C * rel + vo->v.y;
                else
                    out->y = out->y - lbl_eu_80666444;
            }
        } else {
            // Below the follow line: snap to it plus the follow offset.
            ((u32*)out)[0] = ((u32*)&vo->v)[0];
            ((u32*)out)[1] = ((u32*)&vo->v)[1];
            ((u32*)out)[2] = ((u32*)&vo->v)[2];
            out->y += lbl_eu_80666448;
        }
    } else {
        // No body: snap straight to the voice vector plus follow offset.
        CinemVecOut* vo = ((CinemCamSrcI*)src)->getVecOut();
        ((u32*)out)[0] = ((u32*)&vo->v)[0];
        ((u32*)out)[1] = ((u32*)&vo->v)[1];
        ((u32*)out)[2] = ((u32*)&vo->v)[2];
        out->y += lbl_eu_80666448;
    }
    return;

zero_init:
    // Retail inline-expands this as lwzu/lwz GPR copies from the zero global.
    ((u32*)out)[0] = ((u32*)&ml::CVec3::zero)[0];
    ((u32*)out)[1] = ((u32*)&ml::CVec3::zero)[1];
    ((u32*)out)[2] = ((u32*)&ml::CVec3::zero)[2];
}

// Camera depth/pose solver for the event cam. Computes a blended aim point
// from one or two camera sources:
//  - c1==4 short-circuits to raw vector copies;
//  - otherwise both sources are run through func_800756D0, summed and scaled
//    by a yaw term derived from Atan2FIdx of the two voices' position delta
//    (or re-queried per-source when the sources differ / match c1/c2 pairs);
//  - the global cam-table flag word (0x4C) then shapes two working copies of
//    the v1/v2 vectors (dead-end stack temps in retail);
//  - finally out1/out2 are produced by func_80074010 blends selected by
//    c1/c2, with demo-mode y-corrections applied at the tail.
void func_80075934(ml::CVec3* out1, ml::CVec3* out2, CamCamSrc* a, CamCamSrc* b,
                   ml::CVec3* v1, ml::CVec3* v2, u16 c1, u16 c2, u8 s0, u8 s1) {
    f32 scale = a->vtable->fn_0x5B4(a);
    if (c1 == 4) {
        // Word copies (retail uses GPR loads/stores, not vector moves).
        CamEventVecWords* sw1 = (CamEventVecWords*)v1;
        u32 t1 = sw1->w[1];
        u32 t0 = sw1->w[0];
        CamEventVecWords* dw1 = (CamEventVecWords*)out1;
        dw1->w[0] = t0;
        dw1->w[1] = t1;
        dw1->w[2] = sw1->w[2];
        CamEventVecWords* sw2 = (CamEventVecWords*)v2;
        u32 u1 = sw2->w[1];
        u32 u0 = sw2->w[0];
        CamEventVecWords* dw2 = (CamEventVecWords*)out2;
        dw2->w[0] = u0;
        dw2->w[1] = u1;
        dw2->w[2] = sw2->w[2];
        return;
    }

    // Blended aim accumulator; func_800756D0 fills it from source a.
    CamTripletLocals aimA; // retail stack slot 0x1E8
    func_800756D0((ml::CVec3*)&aimA,
                  a ? (CinemCamSrc*)&a->voice : (CinemCamSrc*)a);

    CamTripletLocals anchB; // b's follow limits snapshot (retail 0x1F4)
    if (b != 0) {
        CamTripletLocals aimB; // retail stack slot 0x194
        func_800756D0((ml::CVec3*)&aimB, (CinemCamSrc*)&b->voice);

        // Sum the two aims, then scale by the constant yaw factor.
        CamTripletLocals sum; // retail stack slot 0x110
        sum.x = aimB.x + aimA.x;
        sum.y = aimB.y + aimA.y;
        sum.z = aimB.z + aimA.z;
        CamTripletLocals sc; // retail stack slot 0x104
        sc.x = sum.x * lbl_eu_80666454;
        sc.y = sum.y * lbl_eu_80666454;
        sc.z = sum.z * lbl_eu_80666454;
        aimA.x = sc.x;
        aimA.y = sc.y;
        aimA.z = sc.z;

        CinemVecOut* va = (CinemVecOut*)((CamEventVoice*)&a->voice)->vtable->fn_0xAC(&a->voice);
        CinemVecOut* vb = (CinemVecOut*)((CamEventVoice*)&b->voice)->vtable->fn_0xAC(&b->voice);
        CamTripletLocals diff; // retail stack slot 0xF8
        diff.x = vb->v.x - va->v.x;
        diff.y = vb->v.y - va->v.y;
        diff.z = vb->v.z - va->v.z;
        scale = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(diff.x, diff.z);

        if (a != b) scale = a->vtable->fn_0x5B4(a);
        if (c1 == 1 && c2 == 1) {
            scale = a->vtable->fn_0x5B4(a);
        } else if (c1 == 2 && c2 == 2) {
            scale = b->vtable->fn_0x5B4(b);
        }

        anchB.x = b->f_44D8;
        anchB.y = b->f_44DC;
        anchB.z = b->f_44E0;
    }

    // Working copies of both input vectors; s1 mirrors x of the second.
    CamTripletLocals p1; // retail stack slot 0x1D0
    p1.x = v1->x; p1.y = v1->y; p1.z = v1->z;
    CamTripletLocals p2; // retail stack slot 0x1C4
    p2.x = v2->x; p2.y = v2->y; p2.z = v2->z;
    if (s1 != 0) p1.x = -p1.x;

    // Flag-driven shaping of the working vectors (dead-end temps in retail,
    // kept as explicit locals so the flag reads/writes stay observable).
    if (c1 == 1) {
        CfCamEventGlobal* g = lbl_eu_80663DF0;
        u32 flags = g->field_0x4C;
        CamTripletLocals w;
        w.x = p1.x; w.y = p1.y; w.z = p1.z;
        if (flags & 3) {
            f32 lim = a->f_44D8;
            if (__fabs(w.x) < lim) {
                if (w.x >= lbl_eu_8066641C) w.x += lim;
                else w.x -= lim;
            }
        }
        flags = g->field_0x4C;
        if (flags & 5) w.y = lbl_eu_8066641C;
        if (flags & 8) w.y -= lbl_eu_80666450 * a->f_44DC;
    } else if (c2 == 1) {
        CfCamEventGlobal* g = lbl_eu_80663DF0;
        u32 flags = g->field_0x4C;
        CamTripletLocals w;
        w.x = p2.x; w.y = p2.y; w.z = p2.z;
        if (flags & 3) {
            f32 lim = a->f_44D8;
            if (__fabs(w.x) < lim) {
                if (w.x >= lbl_eu_8066641C) w.x += lim;
                else w.x -= lim;
            }
        }
        flags = g->field_0x4C;
        if (flags & 5) w.y = lbl_eu_8066641C;
        if (flags & 8) w.y -= lbl_eu_80666450 * a->f_44DC;
    } else if (c1 == 2) {
        CfCamEventGlobal* g = lbl_eu_80663DF0;
        u32 flags = g->field_0x4C;
        CamTripletLocals w;
        w.x = p1.x; w.y = p1.y; w.z = p1.z;
        if (flags & 3) {
            f32 lim = anchB.x;
            if (__fabs(w.x) < lim) {
                if (w.x >= lbl_eu_8066641C) w.x += lim;
                else w.x -= lim;
            }
        }
        flags = g->field_0x4C;
        if (flags & 5) w.y = lbl_eu_8066641C;
        if (flags & 8) w.y -= lbl_eu_80666450 * anchB.y;
    } else if (c2 == 2) {
        CfCamEventGlobal* g = lbl_eu_80663DF0;
        u32 flags = g->field_0x4C;
        CamTripletLocals w;
        w.x = p2.x; w.y = p2.y; w.z = p2.z;
        if (flags & 3) {
            f32 lim = anchB.x;
            if (__fabs(w.x) < lim) {
                if (w.x >= lbl_eu_8066641C) w.x += lim;
                else w.x -= lim;
            }
        }
        flags = g->field_0x4C;
        if (flags & 5) w.y = lbl_eu_8066641C;
        if (flags & 8) w.y -= lbl_eu_80666450 * anchB.y;
    }

    // Build out1 from c1.
    if (c1 == 1 || c1 == 2) {
        CamCamSrc* src = (c1 == 1) ? a : b;
        CamTripletLocals in;  // retail 0xE0 / 0xC8
        CamTripletLocals res; // retail 0xEC / 0xD4
        CinemCamSrc* v = src ? (CinemCamSrc*)&src->voice : (CinemCamSrc*)src;
        if (v != 0) {
            func_800756D0((ml::CVec3*)&in, v);
            func_80074010(&res, &in, scale, &p1);
        } else {
            res.x = p1.x; res.y = p1.y; res.z = p1.z;
        }
        CamEventVecWords* rw = (CamEventVecWords*)&res;
        CamEventVecWords* ow = (CamEventVecWords*)out1;
        u32 ry = rw->w[1];
        u32 rx = rw->w[0];
        ow->w[0] = rx;
        ow->w[1] = ry;
        ow->w[2] = rw->w[2];
    } else if (c1 == 3) {
        CamTripletLocals res; // retail 0x140
        func_80074010(&res, &aimA, scale, &p1);
        CamEventVecWords* rw = (CamEventVecWords*)&res;
        CamEventVecWords* ow = (CamEventVecWords*)out1;
        u32 ry = rw->w[1];
        u32 rx = rw->w[0];
        ow->w[0] = rx;
        ow->w[1] = ry;
        ow->w[2] = rw->w[2];
    }

    // Build out2 from c2.
    if (c2 == 1 || c2 == 2) {
        CamCamSrc* src = (c2 == 1) ? a : b;
        CamTripletLocals in;  // retail 0xB0 / 0x98
        CamTripletLocals res; // retail 0xBC / 0xA4
        CinemCamSrc* v = src ? (CinemCamSrc*)&src->voice : (CinemCamSrc*)src;
        if (v != 0) {
            func_800756D0((ml::CVec3*)&in, v);
            func_80074010(&res, &in, scale, &p2);
        } else {
            res.x = p2.x; res.y = p2.y; res.z = p2.z;
        }
        CamEventVecWords* rw = (CamEventVecWords*)&res;
        CamEventVecWords* ow = (CamEventVecWords*)out2;
        u32 ry = rw->w[1];
        u32 rx = rw->w[0];
        ow->w[0] = rx;
        ow->w[1] = ry;
        ow->w[2] = rw->w[2];
    } else if (c2 == 3) {
        CamTripletLocals res; // retail 0x11C
        func_80074010(&res, &aimA, scale, &p2);
        CamEventVecWords* rw = (CamEventVecWords*)&res;
        CamEventVecWords* ow = (CamEventVecWords*)out2;
        u32 ry = rw->w[1];
        u32 rx = rw->w[0];
        ow->w[0] = rx;
        ow->w[1] = ry;
        ow->w[2] = rw->w[2];
    }

    // s0 forces the second output's pitch onto the first.
    if (s0 != 0) out2->y = out1->y;

    // Demo-mode pitch correction between the two voices' current positions.
    if (c1 == 1 && c2 == 1 && func_800FE68C() != 0) {
        void* sel = func_800FE68C();
        void* handle = func_800B708C__Fi((int)*(u32*)((u8*)sel + 0x90E4));
        if (handle != 0) {
            f32 ya = ((CinemVecOut*)((CamEventVoice*)&a->voice)->vtable->fn_0xAC(&a->voice))->v.y;
            CinemVecOut* vb = (CinemVecOut*)((CamEventVoice*)handle)
                                  ->vtable->fn_0xAC(handle);
            if (vb->v.y - ya >= lbl_eu_80666428) {
                if (func_804BE398((float*)out2, 0x4044a05, 0, 0,
                                  lbl_eu_80666448, lbl_eu_8066641C)) {
                    CamTripletLocals t; // retail 0x1B8
                    func_804BE4B4((float*)&t, 0);
                    out2->y = t.y + lbl_eu_80666428;
                } else {
                    out2->y = out2->y + lbl_eu_80666428;
                }
            }
        }
    }
    if (func_804BE348(out2, out1, 0x4044a03, 0)) {
        out1->y = out1->y + lbl_eu_80666428;
    }
}


// Fill a table-element pair from the two aim vectors. When the demo flag
// (global cam table h00) is set, dst1 receives the raw `o2` words and dst2
// receives the yaw/pitch pair derived from the (o1 - o2) difference, with
// the difference magnitude snapped to 1.0 when all components are below the
// epsilon threshold. Otherwise both targets are plain word copies. Fully
// inlined at every call site (retail emits four copies of this body).
static inline void camEventAnglePair(ml::CVec3* o1, ml::CVec3* o2,
                                     CfCamEventElem* dst1, CfCamEventElem* dst2) {
    if (lbl_eu_80570C90.h00 != 0) {
        CamTripletLocals d;
        d.x = o1->x - o2->x;
        d.y = o1->y - o2->y;
        d.z = o1->z - o2->z;
        // Word-copy the aim point into the table element.
        CamEventVecWords* srcW = (CamEventVecWords*)o2;
        CamEventVecWords* dstW = (CamEventVecWords*)dst1;
        u32 wy = srcW->w[1];
        u32 wx = srcW->w[0];
        dstW->w[0] = wx;
        dstW->w[1] = wy;
        dstW->w[2] = srcW->w[2];
        f32 len = (f32)sqrt((f64)(d.x * d.x + d.z * d.z));
        dst2->x0 = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.y, len);
        dst2->x4 = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.z, d.x);
        dst2->x8 = lbl_eu_8066641C;
        func_800A3F8C((ml::CVec3*)dst2);
        // Second stack copy of the difference: PSVECMag takes its address,
        // so the epsilon checks below read back through memory.
        ml::CVec3 dm;
        dm.x = d.x;
        dm.y = d.y;
        dm.z = d.z;
        f32 mag;
        if ((f32)__fabs((f64)dm.x) <= lbl_eu_8066A208 &&
            (f32)__fabs((f64)dm.y) <= lbl_eu_8066A208 &&
            (f32)__fabs((f64)dm.z) <= lbl_eu_8066A208)
            mag = lbl_eu_8066641C;
        else
            mag = PSVECMag(dm);
        dst2->x8 = mag;
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
    for (int i = 0; i < self->tab0.count; i++) {
        if (self->tab0.elems[i].d != 0) break;
        idx++;
    }

    void* hA = func_800B708C__Fi((int)self->field_0x10);
    void* hB = func_800B708C__Fi((int)self->field_0x24);
    void* sa = func_8016FE34(hA);
    void* sb = func_8016FE34(hB);
    if (hA == 0 || hB == 0) return 0;

    ml::CVec3 o1, o2;
    if (idx == self->tab0.count) {
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
// Declared in CfCamEvent_1.hpp; retail calls it with a bare unmangled bl.
int func_80076D8C(int unused, int type_, CamEventSrc* src, CamEventTargetInfo* other,
                  u32* outRow, u32* outCol) {
    char* buf2;                             // inner-loop column buffer
    const char* colBase;                    // base of the type-column names
    char* digitBuf;                         // runtime-built weight-column name
    void* g;
    int state;
    u16 typeB;
    u32 typeA;
    int rowEnd;
    int row;
    u8 c0, c1, c2, w, w2;                   // per-row column bytes

    func_8003AA34();
    g = lbl_eu_80664164;
    *outRow = 0;
    *outCol = 0;
    if (type_ == 0x2b) return 0;

    CamEventBody* body = src->vtable->fn_0x298(src);
    state = 1;
    typeB = 0;
    typeA = 2;
    if ((const void*)src != (const void*)other) {
        u32 v = body->field_0x50;
        if (v == 0) return 0;
        state = (body->field_0x60 <= lbl_eu_8066645C) ? 1 : 0;
        typeB = body->field_0x5E;
        typeA = other->field_0x15E4;
    }

    row = (int)func_8003B41C(g);
    rowEnd = row + (int)func_8003B1EC(g);
    buf2 = (char*)lbl_eu_80527638;
    colBase = (const char*)lbl_eu_804FB5D0;
    digitBuf = lbl_eu_80661BB8;
    for (; row < rowEnd; row++) {
        c0 = getCol8(g, (const char*)lbl_eu_804FB5D0, row);
        if (type_ != c0) continue;
        c1 = getCol8(g, colBase + 8, row);
        if (func_80076CE4((int)typeA, c1) == 0) continue;
        c2 = getCol8(g, colBase + 0x13, row);
        if (func_80076C08((int)typeB, state, c2) == 0) continue;

        u32 rem = (u32)ml::math::mtRand(100);
        for (int i = 1; i <= 4; i++) {
            u8 digit = (u8)(i + 0x30);
            digitBuf[4] = digit;
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
// __declspec(noinline): retail calls it outlined from func_800784A0.
#pragma dont_inline on
void* func_80076F88(CfCamEventManager* self, int unk34,
                                void* srcArg, CfCamDataTable* cam) {
    CamEventSrc* src = (CamEventSrc*)func_8016FE34(srcArg);
    u32 field_0x3F10 = (u32)src->field_0x3F10;
    CamEventBody* body = src->vtable->fn_0x298(src);
    void* p31 = body->field_0x04;
    // Pointer selects written init + conditional overwrite so MWCC emits
    // retail's mr/beq/addi shape instead of a branch-around ternary.
    void* followObj = src;
    if (src != 0) followObj = &src->voice;
    if (p31 == 0) {
        followObj = func_800BBC0C(func_800B708C__Fi((int)field_0x3F10));
    }
    void* srcSel = src;
    if (src != 0) srcSel = &src->voice;
    void* srcVoice = func_8016FE34(srcSel);
    int otherVoice = (int)func_8016FE34(followObj);

    // "Band" check: any of the four cam halfwords selects waveform 2/3.
    // Single short-circuit chain so MWCC lays out the beq-to-block /
    // fall-through / final-bgt-past-body shape retail uses.
    if (cam->h0A == 2 || cam->h0A == 3 ||
        cam->h0C == 2 || cam->h0C == 3 ||
        cam->h0E == 2 || cam->h0E == 3 ||
        (u16)(cam->h10 - 2) <= 1) {
        if (p31 == 0) {
        void* v = src->voice.vtable->fn_0x4C(&src->voice);
        followObj = func_800BBC0C(func_800B708C__Fi((int)v));
        if (unk34 == 0x2B) {
            void* p0 = getPlayer__Q22cf13CfGameManagerFi(0);
            if (p0 == 0) {
                followObj = 0;
            } else {
                followObj = 0;
                void* q = ((CamEventVoice*)p0)->vtable->fn_0x4C(p0);
                if (q != 0) followObj = func_800BBC0C(func_800B708C__Fi((int)q));
                if (followObj == 0) followObj = getPlayer__Q22cf13CfGameManagerFi(1);
            }
            self->field_0x34 = (u32)followObj;
        }
        if (followObj == 0) return 0;
        otherVoice = (int)func_8016FE34(followObj);
        }
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

    void* voice = src;
    if (src != 0) voice = &src->voice;

    // Reset the manager's three shake groups (id 1, default speeds).
    {
        u32 flags = self->field_0x50 & 0xFFFEFFFFu;
        self->field_0x40 = 0;
        self->field_0x50 = flags;
        self->tab0.field_0x160 = 1;
        self->tab0.count = 0;
        self->tab0.field_0x168 = 0;
        self->tab0.field_0x16C = lbl_eu_80666418;
        self->tab0.field_0x170 = lbl_eu_8066641C;
        self->tab0.field_0x174 = lbl_eu_8066641C;
        self->tab0.flag_active = 0;
        self->tab0.flag_finish = 0;
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
    if (voice != 0) {
        aim->field_0x0C = voice;
        aim->field_0x10 = ((CamEventAimObj*)voice)->field_0x74;
        CamTripletLocals tmp;
        func_800756D0((ml::CVec3*)&tmp, (CinemCamSrc*)voice);
        // Word-copy the out vector (retail uses GPR loads/stores here).
        CamEventVecWords* tw = (CamEventVecWords*)&tmp;
        CamEventVecWords* d14 = (CamEventVecWords*)&aim->field_0x14;
        u32 wy = tw->w[1];
        u32 wx = tw->w[0];
        d14->w[1] = wy;
        d14->w[0] = wx;
        d14->w[2] = tw->w[2];
    } else {
        aim->field_0x0C = 0;
        aim->field_0x10 = 0;
    }
    if (followObj != 0) {
        aim->field_0x20 = followObj;
        aim->field_0x24 = ((CamEventAimObj*)followObj)->field_0x74;
        CamTripletLocals tmp;
        func_800756D0((ml::CVec3*)&tmp, (CinemCamSrc*)followObj);
        CamEventVecWords* tw = (CamEventVecWords*)&tmp;
        CamEventVecWords* d28 = (CamEventVecWords*)&aim->field_0x28;
        u32 wy = tw->w[1];
        u32 wx = tw->w[0];
        d28->w[1] = wy;
        d28->w[0] = wx;
        d28->w[2] = tw->w[2];
    } else {
        aim->field_0x20 = 0;
        aim->field_0x24 = 0;
    }

    // Push the follow state into the manager's follow-cam object.
    void* obj4 = self->slots[1];
    self->field_0x34 = (u32)followObj;
    ((CamEventMgrSub*)obj4)->vtable->fn_0x40(obj4, (void*)unk34);
    ((CamEventMgrSub*)obj4)->vtable->fn_0x5C(obj4, voice);

    self->field_0x48 = cam->h02;
    self->field_0x4C = cam->h04;
    void* blk = ((CfCamEventSlotView*)self->slots[0])->field_0x0C;
    void* va = ((CamEventVoice*)followObj)->vtable->fn_0xAC(followObj);
    void* vb = src->voice.vtable->fn_0xAC(&src->voice);
    if (func_800A4050((u8*)blk + 0x10C, vb, va) != 0 &&
        (self->field_0x48 & 0x10) == 0) {
        self->field_0x48 |= 0x10000;
    }

    // Dispatch on the state word bits: 0x4 selects the manager's own aim
    // vectors, 0x20 copies the slot's four vectors into a local block.
    int stateArg = 0;
    if (self->field_0x48 & 0x4) {
        func_8007990C(self, 0, 1, (CamEventVecSrc*)&self->field_0x54, 0);
        stateArg = 0x1E;
    } else if (self->field_0x48 & 0x20) {
        CamEventVecSrcRaw tmp;
        if (self->slots[0] != 0) {
            CfCamEventSlotObj* s = (CfCamEventSlotObj*)self->slots[0];
            // Per-vector raw word copy (load pair, store high first).
            CamEventVecWords* v =
                (CamEventVecWords*)s->vtable->fn_0x10(s);
            u32 b = v->w[1];
            u32 a = v->w[0];
            tmp.v0.w[1] = b;
            tmp.v0.w[0] = a;
            tmp.v0.w[2] = v->w[2];
            v = (CamEventVecWords*)s->vtable->fn_0x34(s);
            b = v->w[1];
            a = v->w[0];
            tmp.v1.w[1] = b;
            tmp.v1.w[0] = a;
            tmp.v1.w[2] = v->w[2];
            v = (CamEventVecWords*)s->vtable->fn_0x1C(s);
            b = v->w[1];
            a = v->w[0];
            tmp.v2.w[1] = b;
            tmp.v2.w[0] = a;
            tmp.v2.w[2] = v->w[2];
            tmp.f_24 = s->vtable->fn_0x58(s);
        }
        func_8007990C(self, 0, 1, (CamEventVecSrc*)&tmp, 0);
        stateArg = 0x1E;
    }

    CamTripletLocals o1, o2;
    if ((self->field_0x48 & 0x4) == 0) {
        func_80075934((ml::CVec3*)&o1, (ml::CVec3*)&o2, (CamCamSrc*)srcVoice,
                      (CamCamSrc*)otherVoice,
                      (ml::CVec3*)&cam->f1C, (ml::CVec3*)&cam->f28,
                      cam->h0A, cam->h0C,
                      (u8)((self->field_0x48 >> 1) & 1),
                      (u8)((self->field_0x48 >> 16) & 1));
        func_80078D08(self, stateArg, (ml::CVec3*)&o1, (ml::CVec3*)&o2, cam->h08,
                      0, cam->f14);
        stateArg += cam->h06;
    }
    func_80075934((ml::CVec3*)&o1, (ml::CVec3*)&o2, (CamCamSrc*)srcVoice,
                  (CamCamSrc*)otherVoice,
                  (ml::CVec3*)&cam->f34, (ml::CVec3*)&cam->f40,
                  cam->h0E, cam->h10,
                  (u8)((self->field_0x48 >> 1) & 1),
                  (u8)((self->field_0x48 >> 16) & 1));
    func_80078D08(self, (u16)stateArg, (ml::CVec3*)&o1, (ml::CVec3*)&o2, 0, 0,
                  cam->f18);
    func_80078D08(self, (u16)stateArg, (ml::CVec3*)&o1, (ml::CVec3*)&o2, 4, 0,
                  cam->f18);
    if (self->field_0x48 & 0x40) {
        CamEventVecSrcRaw tmp2;
        if (self->slots[0] != 0) {
            CfCamEventSlotObj* s = (CfCamEventSlotObj*)self->slots[0];
            CamEventVecWords* v =
                (CamEventVecWords*)s->vtable->fn_0x10(s);
            u32 b = v->w[1];
            u32 a = v->w[0];
            tmp2.v0.w[1] = b;
            tmp2.v0.w[0] = a;
            tmp2.v0.w[2] = v->w[2];
            v = (CamEventVecWords*)s->vtable->fn_0x34(s);
            b = v->w[1];
            a = v->w[0];
            tmp2.v1.w[1] = b;
            tmp2.v1.w[0] = a;
            tmp2.v1.w[2] = v->w[2];
            v = (CamEventVecWords*)s->vtable->fn_0x1C(s);
            b = v->w[1];
            a = v->w[0];
            tmp2.v2.w[1] = b;
            tmp2.v2.w[0] = a;
            tmp2.v2.w[2] = v->w[2];
            tmp2.f_24 = s->vtable->fn_0x58(s);
        }
        func_8007990C(self, (u16)(stateArg + 0x1E), 0, (CamEventVecSrc*)&tmp2, 0);
    }

    // Shake tail (same busy/flag logic as func_80079B34).
    self->tab0.flag_active = 1;
    self->tab0.field_0x163 = 0;

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
#pragma dont_inline reset

// Union temp reproducing MWCC's 0x4330-magic int->double conversion slots.
// Hand-built (like CItemBoxGrid's converters) so the pool references are the
// named sdata2 magics lbl_eu_80666420/80666438 instead of anonymous @-labels;
// retail alternates two of these slots per conversion site.
union CamConvTmp {
    u32 w[2];
    f64 d;
};

// Bit-cast for word-copied vector components: lets base-anchor stores go
// through the same GPR words the element fill loaded (retail never does
// float round-trips here).
union CamWordBits {
    u32 w;
    f32 f;
};

// Build the cam-event follow state from a bdat row: resolve the source's
// follow target, pick a table row (or take the caller override), fill a
// local CfCamDataTable from the row's byte/angle columns, run the shared
// pose setup (func_80076F88), then patch the cam-table entry selected by
// the row's speed columns. Returns 1 on success.
// __declspec(noinline): retail calls it outlined from func_800784A0.
#pragma dont_inline on
void* func_800778E4(
    CfCamEventManager* self, int unk34, void* srcArg,
    u32 rowOverride) {
    // Declared low-to-high in retail's stack map: MWCC gives the first
    //-declared local the lowest address.
    s32 colVal[14];   // 0x08..0x3C
    u32 row;          // 0x40 - filled via the picker's 6th arg / override
    u32 colTmp;       // 0x44 - picker's 5th arg, unused afterwards
    CamTripletLocals t4;
    CamTripletLocals t3;
    CamTripletLocals t2;
    CamTripletLocals t1;
    CfCamDataTable tbl;
    CamConvTmp convA;
    CamConvTmp convB;
    void* bdat;
    const char* colBase;

    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    CamEventSrc* src = (CamEventSrc*)func_8016FE34(srcArg);
    u32 field3F10 = (u32)src->field_0x3F10;
    CamEventBody* body = src->vtable->fn_0x298(src);
    void* follow = body->field_0x04;
    if (follow == 0)
        follow = (void*)field3F10;
    CamEventTargetInfo* other =
        (CamEventTargetInfo*)func_800BBC0C(func_800B708C__Fi((int)follow));
    if (other == 0)
        return 0;

    CinemCamSrc* voice =
        (src != 0) ? (CinemCamSrc*)&src->voice : (CinemCamSrc*)src;
    CamEventSrc* voiceSrc = (CamEventSrc*)func_8016FE34(voice);
    CamEventTargetInfo* otherArg = (CamEventTargetInfo*)func_8016FE34(other);

    // NOTE: retail reads the winning row back from the SIXTH argument slot,
    // so the row target goes last here (the fifth is never read again).
    int hit = func_80076D8C((int)self, unk34, voiceSrc, otherArg, &colTmp, &row);
    if (rowOverride != 0) {
        row = rowOverride;
    } else if (hit == 0 || row == 0) {
        return 0;
    }

    // Fill the local cam table from the bdat row. Byte columns go into the
    // halfword flags; angle columns are converted to floats (unsigned for
    // f14/f18, signed for the four aim triplets).
    // Retail materialises the bdat pointer and column-name base only here,
    // after the row has been picked.
    bdat = lbl_eu_80664168;
    colBase = (const char*)lbl_eu_804FB5D0;
    colVal[0] = getBdatStringColumnValue(bdat, colBase + 0x19, row);
    tbl.h00 = colVal[0];
    colVal[1] = getBdatStringColumnValue(bdat, colBase + 0x21, row);
    tbl.h02 = colVal[1];
    colVal[2] = getBdatStringColumnValue(bdat, colBase + 0x28, row);
    tbl.h04 = colVal[2];
    colVal[3] = getBdatStringColumnValue(bdat, colBase + 0x30, row);
    tbl.h06 = colVal[3];
    colVal[4] = getBdatStringColumnValue(bdat, colBase + 0x35, row);
    tbl.h08 = colVal[4];
    colVal[5] = getBdatStringColumnValue(bdat, colBase + 0x3a, row);
    tbl.h0A = colVal[5];
    colVal[6] = getBdatStringColumnValue(bdat, colBase + 0x47, row);
    tbl.h0C = colVal[6];
    colVal[7] = getBdatStringColumnValue(bdat, colBase + 0x55, row);
    tbl.h0E = colVal[7];
    colVal[8] = getBdatStringColumnValue(bdat, colBase + 0x62, row);
    tbl.h10 = colVal[8];
    colVal[9] = getBdatStringColumnValue(bdat, colBase + 0x70, row);
    convA.w[1] = (u32)(u8)colVal[9];
    tbl.f14 = (f32)(convA.d - lbl_eu_80666420);
    colVal[10] = getBdatStringColumnValue(bdat, colBase + 0x76, row);
    convA.w[1] = (u32)(u8)colVal[10];
    tbl.f18 = (f32)(convA.d - lbl_eu_80666420);

    convA.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0x7c, row) ^ 0x80000000u;
    t1.x = (f32)(convA.d - lbl_eu_80666438);
    convB.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0x83, row) ^ 0x80000000u;
    t1.y = (f32)(convB.d - lbl_eu_80666438);
    convA.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0x8a, row) ^ 0x80000000u;
    t1.z = (f32)(convA.d - lbl_eu_80666438);
    tbl.f1C = t1.x;
    tbl.f20 = t1.y;
    tbl.f24 = t1.z;

    convA.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0x91, row) ^ 0x80000000u;
    t2.x = (f32)(convA.d - lbl_eu_80666438);
    convB.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0x99, row) ^ 0x80000000u;
    t2.y = (f32)(convB.d - lbl_eu_80666438);
    convA.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0xa1, row) ^ 0x80000000u;
    t2.z = (f32)(convA.d - lbl_eu_80666438);
    tbl.f28 = t2.x;
    tbl.f2C = t2.y;
    tbl.f30 = t2.z;

    convA.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0xa9, row) ^ 0x80000000u;
    t3.x = (f32)(convA.d - lbl_eu_80666438);
    convB.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0xb0, row) ^ 0x80000000u;
    t3.y = (f32)(convB.d - lbl_eu_80666438);
    convA.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0xb7, row) ^ 0x80000000u;
    t3.z = (f32)(convA.d - lbl_eu_80666438);
    tbl.f34 = t3.x;
    tbl.f38 = t3.y;
    tbl.f3C = t3.z;

    convA.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0xbe, row) ^ 0x80000000u;
    t4.x = (f32)(convA.d - lbl_eu_80666438);
    convB.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0xc6, row) ^ 0x80000000u;
    t4.y = (f32)(convB.d - lbl_eu_80666438);
    convA.w[1] = (u32)getBdatStringColumnValue(bdat, colBase + 0xce, row) ^ 0x80000000u;
    t4.z = (f32)(convA.d - lbl_eu_80666438);
    tbl.f40 = t4.x;
    tbl.f44 = t4.y;
    tbl.f48 = t4.z;

    if (func_80076F88(self, unk34, srcArg, &tbl) == 0)
        return 0;

    // Patch the cam-table entry picked by this row's speed columns.
    colVal[11] = getBdatStringColumnValue(bdat, colBase + 0xd6, row);
    u8 flag = (u8)colVal[11];
    colVal[12] = getBdatStringColumnValue(bdat, colBase + 0xdf, row);
    colVal[13] = getBdatStringColumnValue(bdat, colBase + 0xea, row);
    u16 hEA = (u16)colVal[13];
    if (flag != 0 && hEA != 0) {
        CamEventTableEntry* e = &lbl_eu_805273C8[(u16)(flag - 1)];
        u16 hDF = (u16)colVal[12];
        convA.w[1] = (u32)hDF;
        convB.w[1] = (u32)hEA;
        e->f_1C = (f32)((f32)(convA.d - lbl_eu_80666420) / lbl_eu_80666460);
        f32 t = (f32)(convB.d - lbl_eu_80666420) / lbl_eu_80666460;
        e->f_20 = t;
        e->f_24 = lbl_eu_80666464 * ((f32)(convB.d - lbl_eu_80666420) /
                                     lbl_eu_80666460);
        e->f_28 = lbl_eu_80666464 * t;
        CfCamEventGlobal* g = lbl_eu_80663DF0;
        if (g != 0 && g->field_0x38 != 0) {
            func_80240C98(g->field_0x38, (int)e, 0);
        }
    }
    return (void*)1;
}

// Compile-only stub; retail calls it outlined (dont_inline keeps the bl).
#pragma dont_inline on
void func_80077F20(void* out, void* a,
                                                     void* b, u16 c, u16 d,
                                                     void* e) {}
#pragma dont_inline reset

// Maps an (action, parameter) pair to a campaign-state id; default 5.
#pragma dont_inline on
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
#pragma dont_inline reset

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
    func_800784A0(idx, (CfCamEventObj*)param, 0, 0, 0, 0, 0);
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
    self->field_0x40 = 0;
    self->field_0x50 &= 0xFFFEFFFFu;
    self->tab0.field_0x160 = (u16)second;
    self->tab0.count = 0;
    self->tab0.field_0x168 = 0;
    self->tab0.field_0x16C = f18;
    self->tab0.field_0x170 = f1C;
    self->tab0.field_0x174 = f1C;
    self->tab0.flag_active = 0;
    self->tab0.flag_finish = 0;
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
    // Keep the state id in an int after the lha so the busy-range compare
    // below stays as two signed cmpi (an s16 local would let MWCC fold the
    // [0x10,0x2b] test into a subi/rlwinm/cmpli trio).
    int st = self->field_0x3E;
    if (st == 9) {
        // Scale the +0x18 triplet into a temp, append it (p5 stays the raw
        // source pointer).
        ml::CVec3 tmp;
        // Compound-scale per component: keeps MWCC's lfs/fmuls/stfs
        // per-component schedule closest to retail.
        tmp.x = c->v2.x;
        tmp.y = c->v2.y;
        tmp.z = c->v2.z;
        tmp.x *= lbl_eu_8066A20C;
        tmp.y *= lbl_eu_8066A20C;
        tmp.z *= lbl_eu_8066A20C;
        func_80078D08(self, a, (ml::CVec3*)c, &tmp, b, 0, c->f_24);
    } else if (st == 8) {
        // Copy both aim vectors, then (when the follow cam exists) subtract
        // its aim vector from each.
        ml::CVec3 v1;
        ml::CVec3 v2;
        v1.x = c->v0.x;
        v1.y = c->v0.y;
        v1.z = c->v0.z;
        v2.x = c->v1.x;
        v2.y = c->v1.y;
        v2.z = c->v1.z;
        if (self->field_0x34 != 0) {
            CinemCamSrc* src34 = (CinemCamSrc*)(void*)self->field_0x34;
            // Named locals pin the register-asm VEC3Sub arguments into
            // materialised address registers (retail's addi r4/r5 schedule).
            nw4r::math::VEC3* pv1 = (nw4r::math::VEC3*)&v1;
            nw4r::math::VEC3* pv2 = (nw4r::math::VEC3*)&v2;
            const nw4r::math::VEC3* pd =
                (const nw4r::math::VEC3*)&src34->vtable->fn_0xAC(src34)->v;
            nw4r::math::VEC3Sub(pv1, pv1, pd);
            nw4r::math::VEC3Sub(pv2, pv2, pd);
        }
        func_80078D08(self, a, &v1, &v2, b, 0, c->f_24);
    } else if (st == 0xA) {
        ml::CVec3 v1;
        ml::CVec3 v2;
        v1.x = c->v0.x;
        v1.y = c->v0.y;
        v1.z = c->v0.z;
        v2.x = c->v1.x;
        v2.y = c->v1.y;
        v2.z = c->v1.z;
        if (self->field_0x34 != 0) {
            func_80074230(&v1, &v2);
        }
        func_80078D08(self, a, &v1, &v2, b, 0, c->f_24);
    } else {
        // Default: only append while the global cam state is in its busy
        // frame range.
        int on = 0;
        if (lbl_eu_80663DF0 != 0) {
            int g = lbl_eu_80663DF0->field_0x3E;
            if (g >= 0x10 && g <= 0x2b) on = 1;
        }
        if (on != 0) {
            func_80078D08(self, a, (ml::CVec3*)c, &c->v1, b, d, c->f_24);
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
    // Retail zero-extends the truncated sum into a u32 (clrlwi r31); the
    // element stores narrow it back to a halfword.
    u32 id = (u16)(add + self->field_0x40);
    // Two hand-built 0x4330-magic int->double conversion slots (retail stack
    // 0xc8/0xd0), both pre-stored at entry; each append tail alternates
    // between them.
    CamConvTmp convB;
    CamConvTmp convA;
    convB.w[0] = 0x43300000;
    convA.w[0] = 0x43300000;
    if (p1 <= lbl_eu_8066A208)
        self->field_0x50 |= 0x10000;

    s16 state = self->field_0x3E;
    if (state == 9) {
        // Scale the r6 aim vector and normalize it; all three tables get it.
        CamTripletLocals sv;
        f32 k = lbl_eu_8066A210;
        f32 sx = r6->x * k;
        f32 sy = r6->y * k;
        f32 sz = r6->z * k;
        sv.x = sx;
        sv.y = sy;
        sv.z = sz;
        func_800A3F8C((ml::CVec3*)&sv);

        // table0 (0x7C) - elements start at the table base; count at 0x1E2.
        // Retail stores the raw p5 words here; the scaled vector goes to
        // table1 (that is why p5 lives in a saved register across the
        // normalize call).
        s16 cnt0 = self->tab0.count;
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
            self->tab0.count = cnt0 + 1;
            convA.w[1] = id;
            self->tab0.field_0x174 = (f32)(convA.d - lbl_eu_80666420);
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
            convB.w[1] = id;
            self->shake[0].field_0x174 = (f32)(convB.d - lbl_eu_80666420);
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
            convA.w[1] = id;
            self->shake[1].field_0x174 = (f32)(convA.d - lbl_eu_80666420);
        }
    } else if (state == 8) {
        if (self->field_0x47 != 0) {
            // One-shot path: raw pair, d word cleared. NOTE: this branch's
            // three tails use the conversion slots in reverse order (B/A/B).
            s16 cnt0 = self->tab0.count;
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
                self->tab0.count = cnt0 + 1;
                convB.w[1] = id;
                self->tab0.field_0x174 = (f32)(convB.d - lbl_eu_80666420);
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
                convA.w[1] = id;
                self->shake[0].field_0x174 = (f32)(convA.d - lbl_eu_80666420);
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
                convB.w[1] = id;
                self->shake[1].field_0x174 = (f32)(convB.d - lbl_eu_80666420);
            }
        } else {
            // One-shot clear path: table0 gets the raw r6 vector, table1 the
            // yaw/pitch angles of the (p5 - r6) difference.
            CamTripletLocals d;
            d.x = p5->x - r6->x;
            d.y = p5->y - r6->y;
            d.z = p5->z - r6->z;
            f32 len = (f32)sqrt((f64)(d.z * d.z + d.x * d.x));
            CamTripletLocals out;
            out.x = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.y, len);
            out.y = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
            out.z = lbl_eu_8066641C;
            func_800A3F8C((ml::CVec3*)&out);
            if ((f32)__fabs((f64)d.x) <= lbl_eu_8066A208 &&
                (f32)__fabs((f64)d.y) <= lbl_eu_8066A208 &&
                (f32)__fabs((f64)d.z) <= lbl_eu_8066A208)
                out.z = lbl_eu_8066641C;
            else
                out.z = PSVECMag((const Vec*)&d);

            s16 cnt0 = self->tab0.count;
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
                self->tab0.count = cnt0 + 1;
                convA.w[1] = id;
                self->tab0.field_0x174 = (f32)(convA.d - lbl_eu_80666420);
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
                convB.w[1] = id;
                self->shake[0].field_0x174 = (f32)(convB.d - lbl_eu_80666420);
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
                convA.w[1] = id;
                self->shake[1].field_0x174 = (f32)(convA.d - lbl_eu_80666420);
            }
        }
    } else if (state == 0xA) {
        // Same angle pair as state 8's clear path.
        CamTripletLocals d;
        d.x = p5->x - r6->x;
        d.y = p5->y - r6->y;
        d.z = p5->z - r6->z;
        f32 len = (f32)sqrt((f64)(d.z * d.z + d.x * d.x));
        CamTripletLocals out;
        out.x = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.y, len);
        out.y = lbl_eu_80666454 * Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
        out.z = lbl_eu_8066641C;
        func_800A3F8C((ml::CVec3*)&out);
        if ((f32)__fabs((f64)d.x) <= lbl_eu_8066A208 &&
            (f32)__fabs((f64)d.y) <= lbl_eu_8066A208 &&
            (f32)__fabs((f64)d.z) <= lbl_eu_8066A208)
            out.z = lbl_eu_8066641C;
        else
            out.z = PSVECMag((const Vec*)&d);

        s16 cnt0 = self->tab0.count;
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
            self->tab0.count = cnt0 + 1;
            convA.w[1] = id;
            self->tab0.field_0x174 = (f32)(convA.d - lbl_eu_80666420);
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
            convB.w[1] = id;
            self->shake[0].field_0x174 = (f32)(convB.d - lbl_eu_80666420);
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
            convA.w[1] = id;
            self->shake[1].field_0x174 = (f32)(convA.d - lbl_eu_80666420);
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
            CamTripletLocals t0, t1;
            if (lbl_eu_80570C90.h00 != 0) {
                CamTripletLocals d;
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
                func_800A3F8C((ml::CVec3*)&t1);
                if ((f32)__fabs((f64)d.x) <= lbl_eu_8066A208 &&
                    (f32)__fabs((f64)d.y) <= lbl_eu_8066A208 &&
                    (f32)__fabs((f64)d.z) <= lbl_eu_8066A208)
                    t1.z = lbl_eu_8066641C;
                else
                    t1.z = PSVECMag((const Vec*)&d);
            } else {
                t0.x = p5->x;
                t0.y = p5->y;
                t0.z = p5->z;
                t1.x = r6->x;
                t1.y = r6->y;
                t1.z = r6->z;
            }
            s16 cnt0 = self->tab0.count;
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
                self->tab0.count = cnt0 + 1;
                convA.w[1] = id;
                self->tab0.field_0x174 = (f32)(convA.d - lbl_eu_80666420);
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
                convB.w[1] = id;
                self->shake[0].field_0x174 = (f32)(convB.d - lbl_eu_80666420);
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
                e->d = (u16)p8;
                if (cnt2 == 0) {
                    self->shake[1].u.tab.baseX = c2[0];
                    self->shake[1].u.tab.baseY = c2[1];
                    self->shake[1].u.tab.baseZ = c2[2];
                }
                self->shake[1].field_0x166 = cnt2 + 1;
                convA.w[1] = id;
                self->shake[1].field_0x174 = (f32)(convA.d - lbl_eu_80666420);
            }
        }
    }
}


// Advance each element of the shake table toward its predecessor by 2*PI once
// the gap reaches PI. Written inline in each path below (retail keeps several
// copies of the loop).
void func_80079B34(CfCamEventManager* self) {
    self->tab0.flag_active = 1;
    self->tab0.field_0x163 = 0;

    // Degenerate advance over tab0 kept from the original source (the same
    // broken pointer-vs-count comparison as func_80079D6C): the table pointer
    // is compared against its own s16 count word, so the loop never iterates
    // at runtime - retail still emits the body.
    {
        CfCamEventElem* e = (CfCamEventElem*)self;
        f32 step = lbl_eu_8066A1FC;
        while ((s32)e < (s32)self->tab0.count) {
            f32 cur = e->x4;
            f32 d = cur - (e - 1)->x4;
            f32 ad = (f32)__fabs((f64)d);
            if (ad >= step) {
                if (ad > step)
                    e->x4 = cur - step;
                else
                    e->x4 = cur + step;
            }
        }
    }

    // Whether the global cam manager is inside its busy frame range.
    // Keep the state id in an int so the [0x10,0x2b] test stays as two cmpi.
    int busy = 0;
    if (lbl_eu_80663DF0 != nullptr) {
        int g = lbl_eu_80663DF0->field_0x3E;
        if (g >= 0x10 && g <= 0x2b) busy = 1;
    }

    if (busy) {
        // Demo-mode flag drives the shake update.
        self->shake[0].field_0x162 = 1;
        u8 on = (lbl_eu_80570C90.h00 != 0);
        self->shake[0].field_0x163 = on;
        if (on) {
            s16 count = self->shake[0].field_0x166;
            CfCamEventElem* e = self->shake[0].u.elems + 1;
            for (int i = 1; i < count; i++, e++) {
                f32 cur = e->x4;
                f32 d = cur - (e - 1)->x4;
                if ((f32)__fabs((f64)d) >= lbl_eu_8066A1F8) {
                    if (d > lbl_eu_8066641C)
                        e->x4 = cur - lbl_eu_8066A1FC;
                    else
                        e->x4 = cur + lbl_eu_8066A1FC;
                }
            }
        }
    } else {
        // When not busy, field_0x47 decides whether to shake; the advance
        // loop is shared between the two arms (retail duplicates its body).
        if (self->field_0x47) {
            self->shake[0].field_0x162 = 1;
            self->shake[0].field_0x163 = 0;
        } else {
            self->shake[0].field_0x162 = 1;
            self->shake[0].field_0x163 = 1;
        }
        s16 count = self->shake[0].field_0x166;
        CfCamEventElem* e = self->shake[0].u.elems + 1;
        for (int i = 1; i < count; i++, e++) {
            f32 cur = e->x4;
            f32 d = cur - (e - 1)->x4;
            if ((f32)__fabs((f64)d) >= lbl_eu_8066A1F8) {
                if (d > lbl_eu_8066641C)
                    e->x4 = cur - lbl_eu_8066A1FC;
                else
                    e->x4 = cur + lbl_eu_8066A1FC;
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
// retail body is a degenerate while-loop: the element pointer is used both
// as the element base (previous element one stride back at -0x10) and as
// the base for the table count at +0x4D2, and it never advances.
// NOTE: retail consumes `step` from f0 (no prologue homes the incoming f1
// param); MWCC keeps a float param in f1 in every tested source shape, so
// the d-chain/step FPR colors stay transposed (documented open item).
void func_80079D6C(f32 step, CfCamEventShakeElem* e) {
    while ((s32)e < (s32)e->count) {
        f32 d = (f32)__fabs((f64)(e->x4 - ((CfCamEventElem*)e - 1)->x4));
        if (d >= step) {
            if (d > step)
                e->x4 -= step;
            else
                e->x4 += step;
        }
    }
}

// NOTE: retail materializes the sub pointer (addi r3,r3,0x1f4) and keeps the
// three checks branchy with separate return blocks. Advancing a byte pointer
// in-source pins the addi so the optimizer cannot refold it into the load
// displacements.
bool func_80079DBC(CfCamEventManager* manager) {
    // OPEN ITEM (us-8007a758): retail materializes base+0x1F4 via `addi r3,r3,500`
    // before the 2nd/3rd byte tests; every source shape tried so far (raw u8*
    // indexing, member access shake[i].field_0x162, tab0-strided array t[0..2])
    // folds to direct displacements instead. Next angles: sub-object cast
    // (this-adjustment addi), helper returning &shake[i], loop unrolled by -O4,p.
    if (manager->tab0.flag_active != 0) return true;
    if (manager->shake[0].field_0x162 != 0) return true;
    return manager->shake[1].field_0x162 != 0;
}

// One composition step of the fixed 3x3 recurrence used by the func_80079E04
// rotation body. Folds a fresh sin/cos pair into the running block; written
// inline twice per branch exactly as retail duplicates it.

// Per-frame cam-event update (state machine). Resolves the game manager via
// dynamic_cast and checks its busy flag and the frame-speed value before
// dispatching on the cam-state word (0x3E). States 8/0xA compose a pair of
// fixed 3x3 recurrences from sin/cos of the table-base axis and push the
// resulting offset vectors into the dynamic object's setters; state 9 just
// renormalizes the unit-0 base anchor. Returns whether either shake table
// finished (or the delegate's result when busy).
int func_80079E04(CfCamEventManager* self) {
    UnkClass_800821F8* gm = func_800821F8__Q22cf13CfGameManagerFv();
    CfDynMgr* dyn = (CfDynMgr*)__dynamic_cast(gm, 0, (const void*)&lbl_eu_80661B00,
                                              (const void*)&lbl_eu_80661B30, 0);
    if (dyn == 0) return 0;
    if (dyn->m_field04 & 0x04000000) return 0;

    CfRes_getD80Flag();
    f32 val = func_80496288(lbl_eu_80663E14);
    // cror eq,lt,eq / bne: proceed only when val <= k.
    if (!(val <= lbl_eu_8066641C)) return 0;

    if (self->tab0.flag_active == 0) {
        // Flag 0x1DE clear: busy range still hands off to func_8007AA4C;
        // otherwise the one-shot flag 0x46 is released via the game manager.
        if (lbl_eu_80663DF0 != 0) {
            s16 g = lbl_eu_80663DF0->field_0x3E;
            if (g >= 0x10 && g <= 0x2b) {
                if (self->field_0x48 & 1) {
                    func_8007AA4C(self);
                }
                return 0;
            }
        }
        if (self->field_0x46 != 0) {
            func_80081E90__Q22cf13CfGameManagerFv(0, 0, self->field_0x44);
            self->field_0x46 = 0;
        }
        return 0;
    }

    // Unit-1 active flag (0x4CE); survives to the tail scalar push.
    int flag = (self->shake[1].field_0x162 != 0);

    // Busy early-out: hand the whole advance to func_8007AA4C.
    if (lbl_eu_80663DF0 != 0) {
        s16 g = lbl_eu_80663DF0->field_0x3E;
        if (g >= 0x10 && g <= 0x2b) return func_8007AA4C(self);
    }

    // Advance the three shake tables.
    CfCamEventShakeUnit* u0 = &self->shake[0];
    func_80074F4C((CfCamShakeState*)&self->tab0, 0);
    func_80074F4C((CfCamShakeState*)u0, 0);
    func_80074F4C((CfCamShakeState*)&self->shake[1], 0);

    // Busy word: either unit 0's finish flag or the table-0 finish flag.
    int result = (u0->field_0x164 != 0) || (self->tab0.flag_finish != 0);

    CfCamAdvObj* adv = (CfCamAdvObj*)dyn;
    s16 state = self->field_0x3E;
    if (state == 9) {
        // Normalize the unit-0 base anchor, then push it (and the table-0
        // base anchor) into the dynamic object's vector setters.
        CamTripletLocals v;
        v.x = u0->u.tab.baseX;
        v.y = u0->u.tab.baseY;
        v.z = u0->u.tab.baseZ;
        func_800A3F8C((ml::CVec3*)&v);
        adv->vtable->fn_0x14(adv, &self->tab0.baseX);
        adv->vtable->fn_0x4C(adv, &v);
    } else if (state == 8) {
        if (self->field_0x47 != 0) {
                // One-shot: table-0 base into slot 2, unit-0 base into slot 1.
                CamTripletLocals a, b;
                a.x = self->tab0.baseX;
                a.y = self->tab0.baseY;
                a.z = self->tab0.baseZ;
                b.x = u0->u.tab.baseX;
                b.y = u0->u.tab.baseY;
                b.z = u0->u.tab.baseZ;
                adv->vtable->fn_0x14(adv, &b);
                adv->vtable->fn_0x64(adv, &a);
        } else {
            // Rotate the table-0 base anchor by the fixed two-step float
            // recurrence built from sin/cos of (k*1, unit-0 baseX, unit-0
            // baseY); the offset magnitude is |unit-0 baseZ|. Each step folds
            // a fresh sin/cos pair into the running 9-float block; the base
            // pair (s1,c1) stays live across both steps.
            f32 Vx = self->tab0.baseX;
            f32 Vy = self->tab0.baseY;
            f32 Vz = self->tab0.baseZ;
            f32 k1 = lbl_eu_8066641C;
            f32 half = lbl_eu_80666428;
            f32 ax = u0->u.tab.baseX;
            f32 ay = u0->u.tab.baseY;
            f32 mag = (f32)__fabs((f64)u0->u.tab.baseZ);
            f32 s1 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * k1);
            f32 c1 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * k1);
            CamEventMtx m;
            m.m[0] = c1; m.m[1] = -s1; m.m[2] = k1;
            m.m[3] = s1; m.m[4] = c1; m.m[5] = k1;
            m.m[6] = k1; m.m[7] = k1; m.m[8] = half;
            f32 sn = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ax);
            f32 cs = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ax);
            CamEventMtx n1;
            n1.m[8] = k1 * m.m[2] + sn * k1 + cs * m.m[8];
            n1.m[7] = k1 * m.m[1] + sn * c1 + cs * m.m[7];
            n1.m[6] = k1 * m.m[0] + sn * s1 + cs * m.m[6];
            n1.m[2] = k1 * m.m[8] + (half * m.m[2] + k1 * m.m[5]);
            n1.m[1] = half * m.m[1] + k1 * m.m[4];
            n1.m[0] = k1 * m.m[6] + (half * m.m[0] + sn * s1);
            n1.m[4] = cs * m.m[4] + k1 * m.m[1] - sn * m.m[7];
            n1.m[3] = k1 * m.m[0] + cs * s1 - sn * m.m[6];
            n1.m[5] = cs * m.m[5] - sn * m.m[8];
            f32 s3 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ay);
            f32 c3 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ay);
            CamEventMtx n2;
            n2.m[8] = k1 * n1.m[2] + s3 * k1 + c3 * n1.m[8];
            n2.m[7] = k1 * n1.m[1] + s3 * c1 + c3 * n1.m[7];
            n2.m[6] = k1 * n1.m[0] + s3 * s1 + c3 * n1.m[6];
            n2.m[2] = k1 * n1.m[8] + (half * n1.m[2] + k1 * n1.m[5]);
            n2.m[1] = half * n1.m[1] + k1 * n1.m[4];
            n2.m[0] = k1 * n1.m[6] + (half * n1.m[0] + s3 * s1);
            n2.m[4] = c3 * n1.m[4] + k1 * n1.m[1] - s3 * n1.m[7];
            n2.m[3] = k1 * n1.m[0] + c3 * s1 - s3 * n1.m[6];
            n2.m[5] = c3 * n1.m[5] - s3 * n1.m[8];
            f32 nmag = -mag;
            f32 mx = nmag * n2.m[2] + (half * n2.m[1] + half * n2.m[0]);
            f32 my = nmag * n2.m[5] + (half * n2.m[4] + half * n2.m[3]);
            f32 mz = nmag * n2.m[8] + (half * n2.m[7] + half * n2.m[6]);
            f32 o1x = Vx - mx;
            f32 o1y = Vy + my;
            f32 o1z = Vz - mz;
            CamTripletLocals out1;
            out1.x = o1x;
            out1.y = o1y;
            out1.z = o1z;
            CamTripletLocals out2;
            out2.x = Vx;
            out2.y = Vy;
            out2.z = Vz;
            if (self->field_0x34 != 0) {
                CinemCamSrc* src = (CinemCamSrc*)(void*)self->field_0x34;
                ml::CVec3* rv = &src->vtable->fn_0xAC(src)->v;
                out1.x += rv->x;
                out1.y += rv->y;
                out1.z += rv->z;
                out2.x += rv->x;
                out2.y += rv->y;
                out2.z += rv->z;
            }
            adv->vtable->fn_0x14(adv, &out1);
            adv->vtable->fn_0x64(adv, &out2);
        }
    } else if (state == 0xA) {
        // Same recurrence as state 8's clear path (retail duplicates it).
        f32 Vx = self->tab0.baseX;
        f32 Vy = self->tab0.baseY;
        f32 Vz = self->tab0.baseZ;
        f32 k1 = lbl_eu_8066641C;
        f32 half = lbl_eu_80666428;
        f32 ax = u0->u.tab.baseX;
        f32 ay = u0->u.tab.baseY;
        f32 mag = (f32)__fabs((f64)u0->u.tab.baseZ);
        f32 s1 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * k1);
        f32 c1 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * k1);
        CamEventMtx m;
        m.m[0] = c1; m.m[1] = -s1; m.m[2] = k1;
        m.m[3] = s1; m.m[4] = c1; m.m[5] = k1;
        m.m[6] = k1; m.m[7] = k1; m.m[8] = half;
        f32 sn = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ax);
        f32 cs = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ax);
        CamEventMtx n1;
        n1.m[8] = k1 * m.m[2] + sn * k1 + cs * m.m[8];
        n1.m[7] = k1 * m.m[1] + sn * c1 + cs * m.m[7];
        n1.m[6] = k1 * m.m[0] + sn * s1 + cs * m.m[6];
        n1.m[2] = k1 * m.m[8] + (half * m.m[2] + k1 * m.m[5]);
        n1.m[1] = half * m.m[1] + k1 * m.m[4];
        n1.m[0] = k1 * m.m[6] + (half * m.m[0] + sn * s1);
        n1.m[4] = cs * m.m[4] + k1 * m.m[1] - sn * m.m[7];
        n1.m[3] = k1 * m.m[0] + cs * s1 - sn * m.m[6];
        n1.m[5] = cs * m.m[5] - sn * m.m[8];
        f32 s3 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ay);
        f32 c3 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * ay);
        CamEventMtx n2;
        n2.m[8] = k1 * n1.m[2] + s3 * k1 + c3 * n1.m[8];
        n2.m[7] = k1 * n1.m[1] + s3 * c1 + c3 * n1.m[7];
        n2.m[6] = k1 * n1.m[0] + s3 * s1 + c3 * n1.m[6];
        n2.m[2] = k1 * n1.m[8] + (half * n1.m[2] + k1 * n1.m[5]);
        n2.m[1] = half * n1.m[1] + k1 * n1.m[4];
        n2.m[0] = k1 * n1.m[6] + (half * n1.m[0] + s3 * s1);
        n2.m[4] = c3 * n1.m[4] + k1 * n1.m[1] - s3 * n1.m[7];
        n2.m[3] = k1 * n1.m[0] + c3 * s1 - s3 * n1.m[6];
        n2.m[5] = c3 * n1.m[5] - s3 * n1.m[8];
        f32 nmag = -mag;
        f32 mx = nmag * n2.m[2] + (half * n2.m[1] + half * n2.m[0]);
        f32 my = nmag * n2.m[5] + (half * n2.m[4] + half * n2.m[3]);
        f32 mz = nmag * n2.m[8] + (half * n2.m[7] + half * n2.m[6]);
        f32 o1x = Vx - mx;
        f32 o1y = Vy + my;
        f32 o1z = Vz - mz;
        CamTripletLocals out1;
        out1.x = o1x;
        out1.y = o1y;
        out1.z = o1z;
        CamTripletLocals out2;
        out2.x = Vx;
        out2.y = Vy;
        out2.z = Vz;
        // With a source object present the pair goes through the
        // pointer-taking setters instead.
        if (self->field_0x34 != 0) {
            adv->vtable->fn_0x68(adv, (void*)self->field_0x34, &out1, 0);
            adv->vtable->fn_0x6C(adv, (void*)self->field_0x34, &out2, 0);
        } else {
            adv->vtable->fn_0x14(adv, &out1);
            adv->vtable->fn_0x64(adv, &out2);
        }
    }

    // One-shot active flag: push the unit-1 base value into the dynamic
    // object's scalar setter.
    if (flag) {
        adv->vtable->fn_0x3C(adv, self->shake[1].u.tab.baseX);
    }
    return result;
}


// Busy-range cam-event advance (called from func_80079E04). Advances the
// three shake tables, then pushes the current aim pair into the dynamic
// object obtained from the game manager: in demo mode (global cam table
// h00 set) the pair is composed from the same fixed 3x3 recurrence used by
// func_80079E04 - base matrix from sin/cos of (k*1), stepped by the unit-0
// base axis (baseX, then baseY), with the unit-0 baseZ magnitude folded in -
// otherwise the raw table/unit base anchors are forwarded. Returns whether
// either shake table finished.
int func_8007AA4C(CfCamEventManager* self) {
    int flag = self->field_0x48 & 1;
    int ret = func_800762A0(self);
    if (flag != 0 && ret == 0) return 0;

    func_80074F4C((CfCamShakeState*)&self->tab0, flag);
    func_80074F4C((CfCamShakeState*)&self->shake[0], flag);
    func_80074F4C((CfCamShakeState*)&self->shake[1], flag);

    f32 wx = self->tab0.baseX;
    f32 wy = self->tab0.baseY;
    f32 wz = self->tab0.baseZ;

    // vecB defaults to the table-0 base anchor (staged first, like retail).
    ml::CVec3 vB;
    vB.set(wx, wy, wz);
    ml::CVec3 vA;

    if (lbl_eu_80570C90.h00 != 0) {
        // Demo mode: fold the unit-0 baseZ magnitude, force z to 1, and run
        // the two-step rotation over the unit-0 base axis.
        f32 mag = (f32)__fabs((f64)self->shake[0].u.tab.baseZ);
        self->shake[0].u.tab.baseZ = lbl_eu_8066641C;
        f32 bx = self->shake[0].u.tab.baseX;
        f32 by = self->shake[0].u.tab.baseY;

        f32 s1 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * lbl_eu_8066641C);
        f32 c1 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * lbl_eu_8066641C);
        f32 s2 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * bx);
        f32 c2 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * bx);
        CamEventMtx a;
        a.m[0] = c1; a.m[1] = -s1; a.m[2] = lbl_eu_8066641C;
        a.m[3] = s1; a.m[4] = c1; a.m[5] = lbl_eu_8066641C;
        a.m[6] = lbl_eu_8066641C; a.m[7] = lbl_eu_8066641C; a.m[8] = lbl_eu_80666428;
        // Step the matrix in place through scalar temporaries (the macro's
        // two-matrix form costs stack slots retail does not spend).
        {
            f32 t6 = a.m[0] + s2 * a.m[1] + c2 * a.m[2];
            f32 t7 = a.m[3] + s2 * a.m[4] + c2 * a.m[5];
            f32 t8 = a.m[2] + s2 * a.m[5] + c2 * a.m[8];
            f32 t3 = a.m[0] + c2 * a.m[1] - s2 * a.m[6];
            f32 t4 = a.m[1] + c2 * a.m[4] - s2 * a.m[7];
            f32 t5 = a.m[2] + c2 * a.m[5] - s2 * a.m[8];
            f32 t0 = a.m[6] + a.m[0] + a.m[1];
            f32 t1 = a.m[7] + a.m[1] + a.m[4];
            f32 t2 = a.m[8] + a.m[2] + a.m[5];
            a.m[8] = t8; a.m[7] = t7; a.m[6] = t6;
            a.m[2] = t2; a.m[1] = t1; a.m[0] = t0;
            a.m[5] = t5; a.m[4] = t4; a.m[3] = t3;
        }
        f32 s3 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * by);
        f32 c3 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80666430 * by);
        // Reuse the same matrix storage for the second step (the stepped
        // values from step one are no longer needed).
        a.m[0] = c3; a.m[1] = -s3; a.m[2] = lbl_eu_8066641C;
        a.m[3] = s3; a.m[4] = c3; a.m[5] = lbl_eu_8066641C;
        a.m[6] = lbl_eu_8066641C; a.m[7] = lbl_eu_8066641C; a.m[8] = lbl_eu_80666428;
        {
            f32 t6 = a.m[0] + s3 * a.m[1] + c3 * a.m[2];
            f32 t7 = a.m[3] + s3 * a.m[4] + c3 * a.m[5];
            f32 t8 = a.m[2] + s3 * a.m[5] + c3 * a.m[8];
            f32 t3 = a.m[0] + c3 * a.m[1] - s3 * a.m[6];
            f32 t4 = a.m[1] + c3 * a.m[4] - s3 * a.m[7];
            f32 t5 = a.m[2] + c3 * a.m[5] - s3 * a.m[8];
            f32 t0 = a.m[6] + a.m[0] + a.m[1];
            f32 t1 = a.m[7] + a.m[1] + a.m[4];
            f32 t2 = a.m[8] + a.m[2] + a.m[5];
            a.m[8] = t8; a.m[7] = t7; a.m[6] = t6;
            a.m[2] = t2; a.m[1] = t1; a.m[0] = t0;
            a.m[5] = t5; a.m[4] = t4; a.m[3] = t3;
        }
        f32 mx = lbl_eu_8066641C * a.m[1] + lbl_eu_8066641C * a.m[0]
            - mag * a.m[2];
        f32 my = lbl_eu_8066641C * a.m[4] + lbl_eu_8066641C * a.m[3]
            - mag * a.m[5];
        f32 mz = lbl_eu_8066641C * a.m[7] + lbl_eu_8066641C * a.m[6]
            - mag * a.m[8];
        vA.set(wx - mx, wy + my, wz - mz);
    } else {
        // Plain mode: forward both base anchors unchanged.
        vA = vB;
        vB.set(self->shake[0].u.tab.baseX, self->shake[0].u.tab.baseY,
               self->shake[0].u.tab.baseZ);
    }

    UnkClass_800821F8* gm = func_800821F8__Q22cf13CfGameManagerFv();
    CfCamAdvObj* adv =
        (CfCamAdvObj*)__dynamic_cast(gm, 0, (const void*)&lbl_eu_80661B00,
                                     (const void*)&lbl_eu_80661B30, 0);
    adv->vtable->fn_0x14(adv, &vA);
    adv->vtable->fn_0x64(adv, &vB);
    adv->vtable->fn_0x3C(adv, self->shake[1].u.tab.baseX);

    // Finished when either shake table raised its finish flag.
    return (self->tab0.flag_finish != 0) || (self->shake[0].field_0x164 != 0);
}

extern void func_8007B030(u8* self) {
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

// bdat column reads as function-like macros: MWCC ignores inline hints for
// these helpers under the TU flags, and retail emits the truncated read
// (call + stw/lbz or stw/lhz stack temp) directly at each site.
#define getColH(table, col, row) ((u16)getBdatStringColumnValue(table, col, row))
#define getColB(table, col, row) ((u8)getBdatStringColumnValue(table, col, row))

// Snapshot a cam slot's four camera vectors into the raw 0x28-byte block
// layout retail builds on the stack before calling func_8007990C.
// Accessor order (0x10 / 0x34 / 0x1C / 0x58) matches retail.
#define slotSnapRaw(dst, slot) \
    do { \
        CfCamEventSlotObj* s = (CfCamEventSlotObj*)(slot); \
        CamEventVecWords* v = (CamEventVecWords*)s->vtable->fn_0x10(s); \
        (dst).v0.w[1] = v->w[1]; (dst).v0.w[0] = v->w[0]; (dst).v0.w[2] = v->w[2]; \
        v = (CamEventVecWords*)s->vtable->fn_0x34(s); \
        (dst).v1.w[1] = v->w[1]; (dst).v1.w[0] = v->w[0]; (dst).v1.w[2] = v->w[2]; \
        v = (CamEventVecWords*)s->vtable->fn_0x1C(s); \
        (dst).v2.w[1] = v->w[1]; (dst).v2.w[0] = v->w[0]; (dst).v2.w[2] = v->w[2]; \
        (dst).f_24 = s->vtable->fn_0x58(s); \
    } while (0)

// Initialize camera-event state from the Bdat table row `idx`.
//
// Two paths:
// 1. When the whole probe chain (columns 0xf4..0x114) is empty, a cam-table
//    entry at lbl_eu_805273C8[(u16)(col_11b + 5)] is seeded from columns
//    0x126/0x132 (angle / duration, divided by the cam-table speed divisor
//    and scaled by the sdata2 factor) and the shared handler is notified.
// 2. Otherwise the event-cam mode is programmed (func_8008212C/80082008),
//    an optional slot snapshot is fed to func_8007990C, and up to six
//    per-frame rows are walked: runtime-built column names (ml::FixStr<16>
//    + format) feed two aim triplets through func_80082088, and each row's
//    follow column seeds another cam-table entry.
void func_8007B0C8(int idx) {
    if (lbl_eu_80663DF0 == 0) return;
    void* mgr = lbl_eu_806640BC;
    if (idx < 1) return;
    if (idx > func_8003B1EC(mgr)) return;

    const char* colBase = (const char*)lbl_eu_804FB5D0;

    // Probe chain: each deeper column is only read while all shallower ones
    // came back empty; `b` set means every probe column was empty.
    int b = 0, c = 0, d = 0, e = 0, f = 0;
    if (getBdatStringColumnValue(mgr, colBase + 0xf4, idx) == 0 &&
        getBdatStringColumnValue(mgr, colBase + 0xfa, idx) == 0)
        f = 1;
    if (f && getBdatStringColumnValue(mgr, colBase + 0x100, idx) == 0) e = 1;
    if (e && getBdatStringColumnValue(mgr, colBase + 0x106, idx) == 0) d = 1;
    if (d && getBdatStringColumnValue(mgr, colBase + 0x10d, idx) == 0) c = 1;
    if (c && getBdatStringColumnValue(mgr, colBase + 0x114, idx) == 0) b = 1;

    if (b) {
        // Seed a single cam-table entry from the deep columns.
        u8 v = getColB(mgr, colBase + 0x11b, idx);
        u16 h3c = getColH(mgr, colBase + 0x126, idx);
        u16 h38 = getColH(mgr, colBase + 0x132, idx);
        // Retail quirk: v == 0 and v == 0xfffa both bail out.
        if (v == 0 || (u16)(v + 6) == 0 || h38 == 0) return;

        CamEventTableEntry* entry = &lbl_eu_805273C8[(u16)(v + 5)];
        entry->f_1C = (f32)h3c / lbl_eu_80666460;
        entry->f_20 = (f32)h38 / lbl_eu_80666460;
        f32 t = entry->f_20;
        entry->f_24 = lbl_eu_80666464 * t;
        entry->f_28 = lbl_eu_80666464 * t;

        CfCamEventGlobal* g = lbl_eu_80663DF0;
        if (g == 0) return;
        if (g->field_0x38 == 0) return;
        func_80240C98(g->field_0x38, (int)entry, 0);
        return;
    }

    // Program the event-cam mode from column 0x19 (mode 2 forces the flag).
    func_8008212C__Q22cf13CfGameManagerFv(8);
    u8 mode = getColB(mgr, colBase + 0x19, idx);
    ((CfCamEventManager*)lbl_eu_80663DF0)->field_0x47 = 0;
    if (mode == 2) ((CfCamEventManager*)lbl_eu_80663DF0)->field_0x47 = 1;
    func_80082008__Q22cf13CfGameManagerFv(8, mode, 0, 0, 0);

    // Mirror the dynamic-cast target's "busy" flag byte.
    u8 dynFlag = (getColB(mgr, colBase + 0x13e, idx) != 0);
    UnkClass_800821F8* gm = func_800821F8__Q22cf13CfGameManagerFv();
    void* casted = __dynamic_cast(gm, 0, (const void*)&lbl_eu_80661B00,
                                  (const void*)&lbl_eu_80661B30, 0);
    if (casted != 0)
        ((CfCamDynObj*)casted)->field_0x294 = dynFlag;

    u32 angle = 0;
    if (getColB(mgr, colBase + 0x148, idx) == 1) {
        CfCamEventManager* m = (CfCamEventManager*)lbl_eu_80663DF0;
        CfCamEventSlot* slot0 = m->slots[0];
        if (slot0 != 0) {
            // Snapshot the current slot vectors and seed the pose solver
            // with them at table index 1.
            CamEventVecSrcRaw snap;
            slotSnapRaw(snap, slot0);
            func_8007990C(m, 0, 1, (CamEventVecSrc*)&snap, 0);
        }
        angle = getColH(mgr, colBase + 0x157, idx);
    }

    // Runtime-built column-name buffers. A single POD struct of 11 raw
    // FixStr-shaped slots so every access folds to a fixed sp offset
    // (retail never materialises pointers to them) and no implicit ctor is
    // generated; cleared field-by-field like retail.
    struct ColNameRaw { char s[16]; int len; };
    struct {
        ColNameRaw a, b, c, d, e, f2, g, h, i2, j, k;
    } nm;
    // Cast helper: view a raw slot as the real FixStr<16> for format().
    #define COLN(m) (*(ml::FixStr<16>*)&nm.m)
    nm.a.s[0] = 0; nm.a.len = 0;
    nm.b.s[0] = 0; nm.b.len = 0;
    nm.c.s[0] = 0; nm.c.len = 0;
    nm.d.s[0] = 0; nm.d.len = 0;
    nm.e.s[0] = 0; nm.e.len = 0;
    nm.f2.s[0] = 0; nm.f2.len = 0;
    nm.g.s[0] = 0; nm.g.len = 0;
    nm.h.s[0] = 0; nm.h.len = 0;
    nm.i2.s[0] = 0; nm.i2.len = 0;
    nm.j.s[0] = 0; nm.j.len = 0;
    nm.k.s[0] = 0; nm.k.len = 0;

    // The two aim triplets live across iterations in stack memory (retail
    // keeps them at sp+0x50 / sp+0x44); arrays force memory residency so
    // MWCC cannot cache them in FP registers across the blend calls.
    f32 tripA[3];
    f32 tripB[3];
    f32 lift = 0;

    // Walk the six per-frame rows. Loop exits early once row i (>1) has a
    // zero first-column value.
    for (int i = 1; i <= 6; i++) {
        COLN(a).format(colBase + 0x162, i);
        COLN(b).format(colBase + 0x169, i);
        COLN(c).format(colBase + 0x170, i);
        COLN(d).format(colBase + 0x177, i);
        COLN(e).format(colBase + 0x17f, i);
        COLN(f2).format(colBase + 0x187, i);
        COLN(g).format(colBase + 0x18f, i);
        COLN(h).format(colBase + 0x195, i);
        COLN(i2).format(colBase + 0x19f, i);
        COLN(j).format(colBase + 0x1ab, i);
        COLN(k).format(colBase + 0x1b8, i);

        u16 colA = getColH(mgr, COLN(h).mString, idx);
        int haveNext = 1;
        if (i < 6) {
            // Peek at row i+1's follow column to pick the blend mode.
            COLN(h).format(colBase + 0x195, i + 1);
            u16 colNext = getColH(mgr, COLN(h).mString, idx);
            haveNext = (colNext == 0);
        }
        if (i > 1 && colA == 0) break;

        // Two scaled aim triplets (sdata2 factor x signed halfword column).
        tripA[0] = lbl_eu_80666458 * (f32)(s16)getColH(mgr, COLN(a).mString, idx);
        tripA[1] = lbl_eu_80666458 * (f32)(s16)getColH(mgr, COLN(b).mString, idx);
        tripA[2] = lbl_eu_80666458 * (f32)(s16)getColH(mgr, COLN(c).mString, idx);
        tripB[0] = lbl_eu_80666458 * (f32)(s16)getColH(mgr, COLN(d).mString, idx);
        tripB[1] = lbl_eu_80666458 * (f32)(s16)getColH(mgr, COLN(e).mString, idx);
        tripB[2] = lbl_eu_80666458 * (f32)(s16)getColH(mgr, COLN(f2).mString, idx);
        u8 colAc = getColB(mgr, COLN(g).mString, idx);
        lift = (f32)colAc;
        int nextMode = (haveNext != 0) ? 4 : 0;

        // First frame of mode-1 runs an extra blend at mode 0.
        if (mode == 1 && i == 1) {
            func_80082088__Q22cf13CfGameManagerFv((u32)(u16)angle,
                                                  tripA, tripB, 0, lift);
            angle += 1;
        }
        func_80082088__Q22cf13CfGameManagerFv((u32)(u16)angle,
                                              tripA, tripB, nextMode, lift);

        // Optional per-row cam-table entry (follow column w70 -> index,
        // w5c -> duration).
        u8 w84 = getColB(mgr, COLN(i2).mString, idx);
        u16 w70 = getColH(mgr, COLN(j).mString, idx);
        u16 w5c = getColH(mgr, COLN(k).mString, idx);
        u16 nextIdx = angle + w70;
        if (w84 != 0 && (u16)(w84 + 6) != 0 && w5c != 0) {
            CamEventTableEntry* e2 = &lbl_eu_805273C8[(u16)(w84 + 5)];
            e2->f_1C = (f32)nextIdx / lbl_eu_80666460;
            e2->f_20 = (f32)w5c / lbl_eu_80666460;
            f32 t2 = e2->f_20;
            e2->f_24 = lbl_eu_80666464 * t2;
            e2->f_28 = lbl_eu_80666464 * t2;
            CfCamEventGlobal* g = lbl_eu_80663DF0;
            if (g != 0 && g->field_0x38 != 0)
                func_80240C98(g->field_0x38, (int)e2, 0);
        }
        angle += colA;
    }

    // Tail: program the final blend from the last row's values.
    u8 col1c5 = getColB(mgr, colBase + 0x1c5, idx);
    CfCamEventManager* g = (CfCamEventManager*)lbl_eu_80663DF0;
    g->field_0x42 = (col1c5 != 0);
    g->field_0x44 = getColH(mgr, colBase + 0x1d2, idx);
    g->field_0x46 = 1;
    if (g->field_0x42 != 0 && g->field_0x44 != 0) {
        func_80082088__Q22cf13CfGameManagerFv((u32)(u16)angle,
                                              tripA, tripB, 4, lift);
        CfCamEventManager* m = (CfCamEventManager*)lbl_eu_80663DF0;
        CfCamEventSlot* slot0 = m->slots[0];
        angle += m->field_0x44;
        if (slot0 != 0) {
            // Feed the freshly blended vectors into the pose solver.
            CamEventVecSrcRaw snap;
            slotSnapRaw(snap, slot0);
            func_8007990C(m, (u16)angle, 4, (CamEventVecSrc*)&snap, 0);
        }
    } else {
        func_80082088__Q22cf13CfGameManagerFv((u32)(u16)angle,
                                              tripA, tripB, 0, lift);
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

    CfCamEventSlotView* slot = (CfCamEventSlotView*)self->slots[self->field_0x3C];
    if (slot == 0) return;

    u8 flag = 0;
    if (lbl_eu_80663DF0 != 0) flag = lbl_eu_80663DF0->field_0x46;

    int updateResult = func_80079E04(self);
    if (updateResult != 0) {
        if (self->field_0x3C == 2 && flag == 0) {
            // Snapshot the slot's camera-vector block before the update call;
            // the squared lengths below are recomputed from the snapshot.
            CamEventData data = *(CamEventData*)slot->field_0x0C;
            slot->vtable->fn_0x20(slot);

            ml::CVec3 a = data.v0;
            ml::CVec3 b = data.v1;

            // Squared lengths of the two camera triplets.
            f32 f31 = a.x * a.x + a.y * a.y + a.z * a.z;
            f32 f30 = b.x * b.x + b.y * b.y + b.z * b.z;

            // Clamp f31 to [lbl_eu_80666468, lbl_eu_80666428], then validate
            // the range (nw4r warning on violation).
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

            // Retail only re-validates the second triplet when the first
            // angle EXCEEDS the threshold; r30 clears only when both angles
            // are above it.
            f32 a1 = acos(f31);
            if (a1 > lbl_eu_8066646C * lbl_eu_8066A210) {
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

                f32 a2 = acos(f30);
                if (a2 <= lbl_eu_8066646C * lbl_eu_8066A210) {
                    updateResult = 0;
                }
            }
        } else {
            slot->vtable->fn_0x20(slot);
        }
    }

    if (updateResult != 0) {
        func_80085878__Q22cf13CfGameManagerFv();
        func_8016FD84(lbl_eu_80666470, lbl_eu_80666428);
    }

    if (self->field_0x38 != 0) {
        func_80240AAC(self->field_0x38);
        // Retail reloads the slot pointer and its vector block here.
        CfCamEventSlotView* tail =
            (CfCamEventSlotView*)self->slots[self->field_0x3C];
        func_80240B10(self->field_0x38, tail->field_0x0C);
    }
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static initializer: fills a discarded local prototype array (back to
// front, local[23] first) and the global cam-table (0x805273C8, 12 entries
// x 0x34 with two aim vectors each) front to back, one interleaved pair of
// triplets at a time. Members are assigned x,y,z directly (not via set())
// so MWCC allocates the pool constants in retail's first-use order.
void sinit_8007BE74() {
    // Discarded prototype vectors, declared front-to-back. POD view: an
    // ml::CVec3 array would emit a default-ctor loop retail doesn't have.
    CamPrototypeVec local[24];
    local[23].x = lbl_eu_80666458;
    local[23].y = lbl_eu_80666458;
    local[23].z = lbl_eu_8066641C;
    lbl_eu_805273C8[0].v0.x = lbl_eu_80666458;
    lbl_eu_805273C8[0].v0.y = lbl_eu_80666458;
    lbl_eu_805273C8[0].v0.z = lbl_eu_8066641C;
    local[22].x = lbl_eu_80666458;
    local[22].y = lbl_eu_80666458;
    local[22].z = lbl_eu_8066641C;
    lbl_eu_805273C8[0].v1.x = lbl_eu_80666458;
    lbl_eu_805273C8[0].v1.y = lbl_eu_80666458;
    lbl_eu_805273C8[0].v1.z = lbl_eu_8066641C;
    local[21].x = lbl_eu_80666474;
    local[21].y = lbl_eu_80666474;
    local[21].z = lbl_eu_8066641C;
    lbl_eu_805273C8[1].v0.x = lbl_eu_80666474;
    lbl_eu_805273C8[1].v0.y = lbl_eu_80666474;
    lbl_eu_805273C8[1].v0.z = lbl_eu_8066641C;
    local[20].x = lbl_eu_80666474;
    local[20].y = lbl_eu_80666474;
    local[20].z = lbl_eu_8066641C;
    lbl_eu_805273C8[1].v1.x = lbl_eu_80666474;
    lbl_eu_805273C8[1].v1.y = lbl_eu_80666474;
    lbl_eu_805273C8[1].v1.z = lbl_eu_8066641C;
    local[19].x = lbl_eu_80666478;
    local[19].y = lbl_eu_80666478;
    local[19].z = lbl_eu_8066641C;
    lbl_eu_805273C8[2].v0.x = lbl_eu_80666478;
    lbl_eu_805273C8[2].v0.y = lbl_eu_80666478;
    lbl_eu_805273C8[2].v0.z = lbl_eu_8066641C;
    local[18].x = lbl_eu_80666478;
    local[18].y = lbl_eu_80666478;
    local[18].z = lbl_eu_8066641C;
    lbl_eu_805273C8[2].v1.x = lbl_eu_80666478;
    lbl_eu_805273C8[2].v1.y = lbl_eu_80666478;
    lbl_eu_805273C8[2].v1.z = lbl_eu_8066641C;
    local[17].x = lbl_eu_8066647C;
    local[17].y = lbl_eu_8066647C;
    local[17].z = lbl_eu_80666478;
    lbl_eu_805273C8[3].v0.x = lbl_eu_8066647C;
    lbl_eu_805273C8[3].v0.y = lbl_eu_8066647C;
    lbl_eu_805273C8[3].v0.z = lbl_eu_80666478;
    local[16].x = lbl_eu_8066647C;
    local[16].y = lbl_eu_8066647C;
    local[16].z = lbl_eu_80666478;
    lbl_eu_805273C8[3].v1.x = lbl_eu_8066647C;
    lbl_eu_805273C8[3].v1.y = lbl_eu_8066647C;
    lbl_eu_805273C8[3].v1.z = lbl_eu_80666478;
    local[15].x = lbl_eu_80666480;
    local[15].y = lbl_eu_80666480;
    local[15].z = lbl_eu_80666484;
    lbl_eu_805273C8[4].v0.x = lbl_eu_80666480;
    lbl_eu_805273C8[4].v0.y = lbl_eu_80666480;
    lbl_eu_805273C8[4].v0.z = lbl_eu_80666484;
    local[14].x = lbl_eu_80666480;
    local[14].y = lbl_eu_80666480;
    local[14].z = lbl_eu_80666484;
    lbl_eu_805273C8[4].v1.x = lbl_eu_80666480;
    lbl_eu_805273C8[4].v1.y = lbl_eu_80666480;
    lbl_eu_805273C8[4].v1.z = lbl_eu_80666484;
    local[13].x = lbl_eu_80666480;
    local[13].y = lbl_eu_80666480;
    local[13].z = lbl_eu_80666484;
    lbl_eu_805273C8[5].v0.x = lbl_eu_80666480;
    lbl_eu_805273C8[5].v0.y = lbl_eu_80666480;
    lbl_eu_805273C8[5].v0.z = lbl_eu_80666484;
    local[12].x = lbl_eu_80666480;
    local[12].y = lbl_eu_80666480;
    local[12].z = lbl_eu_80666484;
    lbl_eu_805273C8[5].v1.x = lbl_eu_80666480;
    lbl_eu_805273C8[5].v1.y = lbl_eu_80666480;
    lbl_eu_805273C8[5].v1.z = lbl_eu_80666484;
    local[11].x = lbl_eu_8066641C;
    local[11].y = lbl_eu_80666458;
    local[11].z = lbl_eu_8066641C;
    lbl_eu_805273C8[6].v0.x = lbl_eu_8066641C;
    lbl_eu_805273C8[6].v0.y = lbl_eu_80666458;
    lbl_eu_805273C8[6].v0.z = lbl_eu_8066641C;
    local[10].x = lbl_eu_80666478;
    local[10].y = lbl_eu_80666458;
    local[10].z = lbl_eu_8066641C;
    lbl_eu_805273C8[6].v1.x = lbl_eu_80666478;
    lbl_eu_805273C8[6].v1.y = lbl_eu_80666458;
    lbl_eu_805273C8[6].v1.z = lbl_eu_8066641C;
    local[9].x = lbl_eu_80666458;
    local[9].y = lbl_eu_80666474;
    local[9].z = lbl_eu_80666458;
    lbl_eu_805273C8[7].v0.x = lbl_eu_80666458;
    lbl_eu_805273C8[7].v0.y = lbl_eu_80666474;
    lbl_eu_805273C8[7].v0.z = lbl_eu_80666458;
    local[8].x = lbl_eu_80666488;
    local[8].y = lbl_eu_80666478;
    local[8].z = lbl_eu_8066641C;
    lbl_eu_805273C8[7].v1.x = lbl_eu_80666488;
    lbl_eu_805273C8[7].v1.y = lbl_eu_80666478;
    lbl_eu_805273C8[7].v1.z = lbl_eu_8066641C;
    local[7].x = lbl_eu_80666478;
    local[7].y = lbl_eu_80666458;
    local[7].z = lbl_eu_80666458;
    lbl_eu_805273C8[8].v0.x = lbl_eu_80666478;
    lbl_eu_805273C8[8].v0.y = lbl_eu_80666458;
    lbl_eu_805273C8[8].v0.z = lbl_eu_80666458;
    local[6].x = lbl_eu_8066648C;
    local[6].y = lbl_eu_8066647C;
    local[6].z = lbl_eu_80666478;
    lbl_eu_805273C8[8].v1.x = lbl_eu_8066648C;
    lbl_eu_805273C8[8].v1.y = lbl_eu_8066647C;
    lbl_eu_805273C8[8].v1.z = lbl_eu_80666478;
    local[5].x = lbl_eu_8066641C;
    local[5].y = lbl_eu_80666458;
    local[5].z = lbl_eu_8066641C;
    lbl_eu_805273C8[9].v0.x = lbl_eu_8066641C;
    lbl_eu_805273C8[9].v0.y = lbl_eu_80666458;
    lbl_eu_805273C8[9].v0.z = lbl_eu_8066641C;
    local[4].x = lbl_eu_80666478;
    local[4].y = lbl_eu_80666474;
    local[4].z = lbl_eu_8066641C;
    lbl_eu_805273C8[9].v1.x = lbl_eu_80666478;
    lbl_eu_805273C8[9].v1.y = lbl_eu_80666474;
    lbl_eu_805273C8[9].v1.z = lbl_eu_8066641C;
    local[3].x = lbl_eu_80666478;
    local[3].y = lbl_eu_80666474;
    local[3].z = lbl_eu_80666458;
    lbl_eu_805273C8[10].v0.x = lbl_eu_80666478;
    lbl_eu_805273C8[10].v0.y = lbl_eu_80666474;
    lbl_eu_805273C8[10].v0.z = lbl_eu_80666458;
    local[2].x = lbl_eu_80666490;
    local[2].y = lbl_eu_80666464;
    local[2].z = lbl_eu_8066647C;
    lbl_eu_805273C8[10].v1.x = lbl_eu_80666490;
    lbl_eu_805273C8[10].v1.y = lbl_eu_80666464;
    lbl_eu_805273C8[10].v1.z = lbl_eu_8066647C;
    local[1].x = lbl_eu_80666474;
    local[1].y = lbl_eu_80666480;
    local[1].z = lbl_eu_80666458;
    lbl_eu_805273C8[11].v0.x = lbl_eu_80666474;
    lbl_eu_805273C8[11].v0.y = lbl_eu_80666480;
    lbl_eu_805273C8[11].v0.z = lbl_eu_80666458;
    local[0].x = lbl_eu_80666428;
    local[0].y = lbl_eu_8066647C;
    local[0].z = lbl_eu_8066647C;
    lbl_eu_805273C8[11].v1.x = lbl_eu_80666428;
    lbl_eu_805273C8[11].v1.y = lbl_eu_8066647C;
    lbl_eu_805273C8[11].v1.z = lbl_eu_8066647C;
}
