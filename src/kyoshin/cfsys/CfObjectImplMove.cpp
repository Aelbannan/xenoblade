// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplMove
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp" // func_800BE12C (owner decl)
#include "libs/monolib/src/scn/CScn_8049603C.hpp" // func_8049603C (single owner decl)
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cfsys/CfObjectImplMove.hpp"
#include <nw4r/math.h>
#include <revolution/mtx/mtxvec.h>
#include <revolution/mtx/quat.h>
#include <math.h>
#include <stdlib.h>
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Variant func_804BE398 ABI (r7 ptr + f1 float) used by func_800CF064 /
// func_800CF810 ground probes; retail marshals args that way there (see
// those sites below). The header decl stays canonical.
typedef int (*BE398PtrF32)(void*, u32, u32, u32, void*, f32);

void func_800CEE80(CfObjectImplMoveObj* self);
void func_800CF064(CfObjectImplMoveObj* self, CfMoveContact* param);
void func_800CF810(CfObjectImplMoveObj* self, CfMoveContact* param);

void func_800CA948(CfObjectImplMoveObj* self) {
    // Init helper: -1 at 0x20, 0 at 0x1c, shared float constant at 0x24.
    self->field_0x20 = 0xFFFFFFFF;
    self->field_0x1C = 0;
    self->field_0x24 = lbl_eu_80666C60;
}

// Move-state update: when an actor is bound, run the four dispatch virtuals
// (0xd4-0xe0); then, if the driver's flag word (bit 0x2 or bit 0x80) is set,
// chain vfE4 -> slot 0x10; finally run func_802A0E08 on the embedded buffer
// when the status word and flag bit 0x2 are both live.
void func_800CA964(CfObjectImplMoveObj* self) {
    if (self->field_0x18 != nullptr) {
        self->vfD4();
        self->vfD8();
        self->vfDC();
        self->vfE0();
    }
    u32 flags = self->mSubObj->field_0x64;
    if ((flags & 2) != 0 || (flags & 0x80) != 0) {
        self->vfE4()->e10();
    }
    if (self->mSubObj->field_0x90 != 0 && (self->mSubObj->field_0x64 & 2) != 0) {
        func_802A0E08(self->field_0x28);
    }
}

// Full reset: unregister this object's callback source on the bound actor,
// tear down effects, run the buffer cleanup, and clear the attached
// flag-word object (+0xb0 word zeroed, bit 0x40 set, pointer released).
void func_800CAA44(CfObjectImplMoveObj* self) {
    CfActorObj* actor = self->field_0x18;
    if (actor != 0 && actor->sub.field_98 != 0) {
        void* src = self;
        if (self != 0) {
            src = &self->field_0x0C;
        }
        func_80482AD4(actor->sub.field_98, src);
    }
    if (self->vfE4() != 0) {
        func_8015BD94(self->vfE4());
    }
    func_802A0FE8(&self->field_0x28[0]);
    if (self->field_0x1C != 0) {
        self->field_0x1C->field_0xB0 = 0;
        self->field_0x1C->field_0x68 |= 0x40;
        self->field_0x1C = 0;
    }
}

void func_800CAB00(CfObjectImplMoveObj* self) {
    // Dispatch the sub-object's event id to the callback source (this+0xc).
    unsigned int id = self->mSubObj->field_0x98;
    if (id == 0) {
        return;
    }
    void* src = self;
    if (self != 0) {
        src = &self->field_0x0C;
    }
    func_80482AB8(id, src);
}

void func_800CAB2C(void) {}

// Move-start dispatcher: plays the entry sound for ids 0x111/0x112, then
// runs the per-id jump-table body (embedded-handler teardown, driver state
// resets, and move-state requests), and finally forwards the param through
// the presentation gate. Case bodies are laid out in retail's emission order
// (0x10, 0xf, 9/0xb, 0xa/0xc-0xe) so MWCC's jump table and body placement
// match.
void func_800CAB30(CfObjectImplMoveObj* self, CfMoveEvtParam* param) {
    u16 id = param->field_C;
    func_800CB21C(self, id);
    if (id == 0x111) {
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1c8, 0, 0,
            lbl_eu_80666C64);
    }
    if (id == 0x112) {
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1c7, 0, 0,
            lbl_eu_80666C64);
    }
    switch (id) {
    case 0x10:
        // Release embedded handler objects 0xef/0xf0/0xf1, then request
        // move state 0xb.
        if (func_80148778(self->field_0x18->field_08, 0xef) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xef);
        if (func_80148778(self->field_0x18->field_08, 0xf0) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf0);
        if (func_80148778(self->field_0x18->field_08, 0xf1) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf1);
        func_800BE12C((u8*)&self->field_0x18->sub, 0xb, 0, -1, 1);
        break;
    case 6:
        // Retail body for id 6 sits between the case-0xf tail and the exit
        // gate; reconstructed as a registration-probe body.
        {
            CfActorObj* a6 = self->field_0x18;
            u32 v6 = a6->field_04->b30()->field_0;
            if (func_80174C98(a6, &v6, 0x805) == 0)
                func_800BE12C((u8*)&self->field_0x18->sub, 0xb, 0, -1, 1);
        }
        break;
    case 0xf:
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x194, 0, 0,
            lbl_eu_80666C64);
        // Reset driver state and re-init the move buffer (retail reloads
        // self->field_0x18 for each of these stores).
        self->field_0x18->mst.field_214 = 0;
        self->field_0x18->mst.field_210 = 0;
        func_8014AE00(self->field_0x18->mst.buf);
        self->field_0x18->field_3E98 = 0;
        self->field_0x18->mst.field_8 &= ~0x10;
        // Retail caches the actor in r29 for exactly this probe pair.
        {
            CfActorObj* actor = self->field_0x18;
            u32 v805 = actor->field_04->b30()->field_0;
            if (func_80174C98(actor, &v805, 0x805) == 0) {
                // Re-request the base move state unless the c4 object
                // reports 5.
                if (((CfMoveC4Obj*)self->field_0x18->sub.field_C4)->field_374 != 5) {
                    func_800BE12C((u8*)&self->field_0x18->sub, 0xc, 0, -1, 1);
                    if (((CfMoveC4Obj*)self->field_0x18->sub.field_C4)->field_374 != 5)
                        func_800BE12C((u8*)&self->field_0x18->sub, 0x31, 0, -1, 1);
                }
            } else {
                func_8014B2DC(self->field_0x18->mst.buf);
            }
        }
        // Release handlers 0xee..0x112.
        if (func_80148778(self->field_0x18->field_08, 0xee) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xee);
        if (func_80148778(self->field_0x18->field_08, 0xf4) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf4);
        if (func_80148778(self->field_0x18->field_08, 0xef) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xef);
        if (func_80148778(self->field_0x18->field_08, 0xf0) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf0);
        if (func_80148778(self->field_0x18->field_08, 0xf1) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf1);
        if (func_80148778(self->field_0x18->field_08, 0x111) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0x111);
        if (func_80148778(self->field_0x18->field_08, 0x112) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0x112);
        // Gate word at +0x1528 feeds both the test and the dispatch argument.
        if (((CfMoveHandlerWide*)self->field_0x18->field_08)->field_0x1528 != 0)
            ((CfMoveHandlerWide*)self->field_0x18->field_08)->w20(
                ((CfMoveHandlerWide*)self->field_0x18->field_08)->field_0x1528);
        break;
    case 9:
    case 0xb:
        // Registration probe 0x802: on success push command 0x100 into the
        // embedded sub-object (retail caches the actor for the probe pair).
        {
            CfActorObj* a9 = self->field_0x18;
            u32 v802 = a9->field_04->b30()->field_0;
            if (func_80174C98(a9, &v802, 0x802) != 0)
                self->field_0x18->sub.vfn00(0x100);
        }
        break;
    case 0xa:
    case 0xc:
    case 0xd:
    case 0xe:
        // Clear the request word and presentation bit, tear down the buffer,
        // then run the sub-object teardown entry point.
        self->field_0x18->field_3E98 = 0;
        self->field_0x18->mst.field_8 &= ~0x10;
        func_8014B2DC(self->field_0x18->mst.buf);
        self->field_0x18->sub.f03();
        if (id == 0xc) {
            // Bit 1 of the +0x3f00 flag word selects which registration
            // probe (both id 0xa) gates the 0x31 request; retail emits both
            // arms as separate bodies.
            CfActorObj* ac = self->field_0x18;
            if ((ac->sub.field_64 & 2) != 0) {
                u32 vA = ac->field_04->b30()->field_0;
                if (func_80174C98(ac, &vA, 0xa) == 0)
                    func_800BE12C((u8*)&self->field_0x18->sub, 0x31, 0, -1, 1);
            } else {
                u32 vB = ac->field_04->b30()->field_0;
                if (func_80174C98(ac, &vB, 0xa) == 0)
                    func_800BE12C((u8*)&self->field_0x18->sub, 0x31, 0, -1, 1);
            }
        } else {
            CfActorObj* ae = self->field_0x18;
            u32 v805b = ae->field_04->b30()->field_0;
            if (func_80174C98(ae, &v805b, 0x805) == 0)
                func_800BE12C((u8*)&self->field_0x18->sub, 0x31, 0, -1, 1);
        }
        // Release handlers 0xee..0xf8.
        if (func_80148778(self->field_0x18->field_08, 0xee) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xee);
        if (func_80148778(self->field_0x18->field_08, 0xf4) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf4);
        if (func_80148778(self->field_0x18->field_08, 0xef) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xef);
        if (func_80148778(self->field_0x18->field_08, 0xf0) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf0);
        if (func_80148778(self->field_0x18->field_08, 0xf1) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf1);
        if (func_80148778(self->field_0x18->field_08, 0xeb) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xeb);
        if (func_80148778(self->field_0x18->field_08, 0xf8) != 0)
            ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf8);
        break;
    }
    // Presentation tail: forward the param unless both gate bits are set.
    if (param->field_2E == 0 || (param->field_30 & 2) == 0) {
        func_801A891C(self->field_0x18, (int)param);
    }
}

void func_800CB21C(CfObjectImplMoveObj* self, u32 id) {
    // Tail-call dispatch: each case issues an embedded sub-object virtual call
    // (vtable 0x204) and returns; retail compiles each case to a bctr tail call.
    // Case order is retail's body emission order (MWCC emits in source order).
    switch (id) {
    case 9:
        self->field_0x18->sub.vfn204(0x1f, 0, -1, 0, 0);
        break;
    case 6:
        self->field_0x18->sub.vfn204(0x28, 0, -1, 0, 0);
        break;
    case 12:
        self->field_0x18->sub.vfn204(0x29, 0, -1, 0, 0);
        break;
    case 11:
        self->field_0x18->sub.vfn204(0x2a, 0, -1, 0, 0);
        break;
    case 0xce:
        self->field_0x18->sub.vfn204(0x1c, 0, -1, 0, 0);
        break;
    case 0xcf:
        self->field_0x18->sub.vfn204(0x1e, 0, -1, 0, 0);
        break;
    case 0xd0:
        self->field_0x18->sub.vfn204(0x1e, 0, -1, 0, 0);
        break;
    case 0xd1:
        self->field_0x18->sub.vfn204(0x1d, 0, -1, 0, 0);
        break;
    case 15:
        self->field_0x18->sub.vfn204(0x1, 0, -1, 0, 0);
        break;
    case 0x111:
        self->field_0x18->sub.vfn204(0x20, 0, -1, 0, 0);
        break;
    case 0x112:
        self->field_0x18->sub.vfn204(0x21, 0, -1, 0, 0);
        break;
    }
}

// Move-event dispatcher: runs the func_800CB9AC pre-dispatch with the event
// id, then routes per id. Cases 9/0xb-0xe share the battle-gated request
// build (case 0xb first raises the actor's 0x40 presentation mask); case 0xa
// only refreshes via vfB4; case 0xf adds the 0x194 sound request and a
// conditional 0x31 move-state request behind three handler probes; case 0x10
// re-registers handlers behind a 0x30 request; ids 0x33/0xf7 discard eight
// vf29C results while clearing flag-bit groups of the actor's +0x74 word.
void func_800CB454(CfObjectImplMoveObj* self, CfMoveEvtParam* param) {
    // Retail reloads the id halfword from the parameter block for every
    // comparison (param stays cached in r31), so no local copy is kept.
    func_800CB9AC(self, param->field_C);
    switch (param->field_C) {
    case 0xa:
        self->field_0x18->f43();
        break;
    case 0xb:
        func_80174C24(self->field_0x18, 0x40);
        // fallthrough
    case 9:
    case 0xc: {
        self->field_0x18->f43();
        getInstance__Q22cf13CfGameManagerFv();
        if (func_8006EF04(0x04000000))
            break;
        CfMoveBMId* bmId =
            (CfMoveBMId*)func_800EA444(getInstance__Q22cf14CBattleManagerFv());
        if (bmId == nullptr ||
            self->field_0x18->sub.field_74 != bmId->field_0)
            break;
        void* src = func_800F477C();
        CfMoveAcReq req;
        memset(&req.at4.raw, 0, 0xe);
        memset(&req, 0, sizeof(req));
        if (src != nullptr) {
            req.at4.body.field_D = 0x5a;
            req.field_12 = ((u8*)src)[0x77];
            req.field_14 = lbl_eu_80666C60;
            req.at4.body.field_10 = 0;
            req.at4.body.field_6 = 6;
        } else {
            req.at4.body.field_D = 4;
            req.field_12 = 0xFFFF;
            req.field_14 = lbl_eu_80666C60;
            req.at4.body.field_10 = 0;
            req.at4.body.field_6 = 6;
        }
        func_8014AC38(self->field_0x18->mst.buf, &req);
        break;
    }
    case 0x10: {
        // Handler re-registration: all three probes must come back empty;
        // then request state 0x30 unless the driver flag (or its +0x4ec
        // mirror on the +0xc4 object) blocks it.
        u32 v1a = self->field_0x18->field_04->b30()->field_0;
        if (func_80174C98(self->field_0x18, &v1a, 0x1a) != 0)
            break;
        u32 v19 = self->field_0x18->field_04->b30()->field_0;
        if (func_80174C98(self->field_0x18, &v19, 0x19) != 0)
            break;
        if (self->field_0x18->f173() != 0)
            break;
        CfEmbeddedSubObj_3E9C* sub = &self->field_0x18->sub;
        if ((sub->field_64 & 2) == 0) {
            func_800BE12C((u8*)sub, 0x30, 0, -1, 1);
        } else if ((((CfMoveC4Obj*)sub->field_C4)->field_4EC & 2) == 0) {
            func_800BE12C((u8*)sub, 0x30, 0, -1, 1);
        }
        break;
    }
    case 0xf: {
        self->field_0x18->f43();
        func_801BFE8C(0, 0x194, 0);
        getInstance__Q22cf13CfGameManagerFv();
        if (!func_8006EF04(0x04000000)) {
            CfMoveBMId* bmId = (CfMoveBMId*)func_800EA444(
                getInstance__Q22cf14CBattleManagerFv());
            if (bmId != nullptr &&
                self->field_0x18->sub.field_74 == bmId->field_0) {
                void* src = func_800F477C();
                CfMoveAcReq req;
                memset(&req.at4.raw, 0, 0xe);
                memset(&req, 0, sizeof(req));
                if (src != nullptr) {
                    req.at4.body.field_D = 0x5a;
                    req.field_12 = ((u8*)src)[0x77];
                    req.field_14 = lbl_eu_80666C60;
                    req.at4.body.field_10 = 0;
                    req.at4.body.field_6 = 6;
                } else {
                    req.at4.body.field_D = 4;
                    req.field_12 = 0xFFFF;
                    req.field_14 = lbl_eu_80666C60;
                    req.at4.body.field_10 = 0;
                    req.at4.body.field_6 = 6;
                }
                func_8014AC38(self->field_0x18->mst.buf, &req);
            }
        }
        // Handler probes (ids 0x805/0x1a/0x19), then a conditional 0x31
        // move-state request gated on the driver flag word.
        u32 v805 = self->field_0x18->field_04->b30()->field_0;
        if (func_80174C98(self->field_0x18, &v805, 0x805) != 0)
            break;
        u32 v1a = self->field_0x18->field_04->b30()->field_0;
        if (func_80174C98(self->field_0x18, &v1a, 0x1a) != 0)
            break;
        u32 v19 = self->field_0x18->field_04->b30()->field_0;
        if (func_80174C98(self->field_0x18, &v19, 0x19) != 0)
            break;
        if (self->field_0x18->f173() != 0)
            break;
        CfEmbeddedSubObj_3E9C* subF = &self->field_0x18->sub;
        if ((subF->field_64 & 2) != 0)
            func_800BE12C((u8*)subF, 0x31, 0, -1, 1);
        break;
    }
    case 0x33:
        // Eight vf29C dispatches whose results are discarded; each pass
        // clears bit mask 0x8000 of the actor's own +0x74 word.
        for (int i = 0; i < 8; i++) {
            self->field_0x18->vf29C(i);
            self->field_0x18->field_74 &= 0xFFFF7FFF;
        }
        break;
    case 0xf7:
        // Same sweep clearing bit mask 0x00c0 instead.
        for (int i = 0; i < 8; i++) {
            self->field_0x18->vf29C(i);
            self->field_0x18->field_74 &= 0xFFFFFF3F;
        }
    }
    // Presentation tail: forward the param unless both gate bits are set.
    if (param->field_2E == 0 || (param->field_30 & 2) != 0) {
        func_801A891C(self->field_0x18, (int)param);
    }
}

// Presentation event gate: the embedded handler at actor +0x08 must accept
// the id before the vf8C presentation virtual is dispatched.
void func_800CB94C(CfObjectImplMoveObj* self, u32 id) {
    if (func_80148778(self->field_0x18->field_08, id) == 0) {
        self->vf94(id);
    }
}

void func_800CB9AC(void* selfV, u32 id) {
    CfObjectImplMoveObj* self = (CfObjectImplMoveObj*)selfV;
    // Presentation/event gate: when the 0x04000000 event flag is set, skip all
    // move dispatch. Otherwise map the incoming id to an embedded sub-object
    // command (vtable 0x20c). Case order is retail's body emission order.
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04(0x04000000)) {
        return;
    }
    switch (id) {
    case 9:
        self->field_0x18->sub.vfn20C(0x1f);
        break;
    case 6:
        self->field_0x18->sub.vfn20C(0x28);
        break;
    case 12:
        self->field_0x18->sub.vfn20C(0x29);
        break;
    case 11:
        self->field_0x18->sub.vfn20C(0x2a);
        break;
    case 0xce:
        // Event 0xce is gated on the +0x8 sub-object's handler accepting it;
        // only then issue command 0x1c.
        if (func_80148778((u8*)self->field_0x18 + 8, 0xce) == 0) {
            self->field_0x18->sub.vfn20C(0x1c);
        }
        break;
    case 0xcf:
        self->field_0x18->sub.vfn20C(0x1e);
        break;
    case 0xd0:
        self->field_0x18->sub.vfn20C(0x1e);
        break;
    case 0xd1:
        self->field_0x18->sub.vfn20C(0x1d);
        break;
    case 15:
        self->field_0x18->sub.vfn20C(0x1);
        break;
    case 0x111:
        self->field_0x18->sub.vfn20C(0x20);
        break;
    case 0x112:
        self->field_0x18->sub.vfn20C(0x21);
        break;
    case 0x113:
    case 0x114:
    case 0x115:
    case 0x116:
        self->field_0x18->sub.vfn20C(0x26);
        break;
    }
}

// Move-event handler. Gates on the presentation flag and a float probe, then
// either rebinds the embedded handler (when the current source is missing or
// the found actor accepts event 0xf8), or runs the filtered enum sweep that
// repositions / clears move effects, and finally fires the 0x200 request when
// actor id 5 is registered and its float counter exceeds the threshold.
void func_800CBBD8(CfObjectImplMoveObj* self) {
    getInstance__Q22cf13CfGameManagerFv();
    if (!func_8006EF04(0x400) && func_80496288(lbl_eu_80663E14) != lbl_eu_80666C60) {
        void* src = self->field_0x18->sub.vfn13();
        if (src == nullptr) {
            // No source bound: only reset when the probe stays empty.
            if (self->field_0x18->sub.vfn13() == nullptr) {
                func_80174B4C(self->field_0x18, 0x80000);
            }
        } else {
            CfActorObj* found = (CfActorObj*)func_8016FE34(
                func_800B708C((int)self->field_0x18->sub.vfn13()));
            if (found == nullptr || found->f173() != 0 ||
                func_80148778(found->field_08, 0xf8) != 0) {
                self->vf70(nullptr);
                func_80174B4C(self->field_0x18, 0x80000);
            }
        }
    }
    // Main sweep block. Retail reloads field_0x18 for every check group.
    CfActorObj* actor = self->field_0x18;
    u32 v6 = actor->field_04->b30()->field_0;
    bool any = func_80174C98(actor, &v6, 6) != 0;
    if (!any) {
        u32 vA = actor->field_04->b30()->field_0;
        if (func_80174C98(actor, &vA, 0x806) == 0)
            return;
    }
    u32 vB = actor->field_04->b30()->field_0;
    if (func_80174C98(actor, &vB, 0x40000000) == 0) {
        u32 vC = actor->field_04->b30()->field_0;
        if (func_80174C98(actor, &vC, 0x800000) == 0)
            return;
    }
    CfMoveEnumHolder holder;
    func_80043D90(&holder);
    func_80043F18(&holder);
    func_800F4A98(func_80043F18(&holder), 0x20, 0x800);
    int handle = self->vf48();
    func_800F6ED0(func_80043F18(&holder), (void*)handle);
    if (((CfMoveEnumList*)func_80043F18(&holder))->field_620 != 0) {
        // Effects exist: clear the driver state and re-register the sub-object.
        self->field_0x18->mst.field_214 = 0;
        self->field_0x18->mst.field_210 = 0;
        func_8014B2DC(self->field_0x18->mst.buf);
        func_800BE12C((u8*)&self->field_0x18->sub, 0x31, 0, -1, 1);
    } else {
        self->vf70(func_800F6E08(func_80043F18(&holder)));
        void* entry = func_800F6EAC(func_80043F18(&holder), 0);
        void* arg = entry;
        if (entry != nullptr)
            arg = (u8*)entry - 0x3e9c;
        self->field_0x18->vf2C4(arg, lbl_eu_80666C60, lbl_eu_80666C60,
            lbl_eu_80666C60);
        CfActorObj* act = self->field_0x18;
        u32 vD = act->field_04->b30()->field_0;
        if (func_80174C98(act, &vD, 0x806) != 0) {
            func_8014B2DC(act->mst.buf);
        } else {
            func_80174B4C(act, 5);
            act->f138();
            int count = ((CfMoveEnumList*)func_80043F18(&holder))->field_620;
            for (int i = 0; i < count; i++) {
                func_8016FE34(
                    func_800F6EAC(func_80043F18(&holder), i));
            }
        }
    }
    __dt__80043E88(&holder, -1);
    actor = self->field_0x18;
    u32 vE = actor->field_04->b30()->field_0;
    if (func_80174C98(actor, &vE, 5) != 0) {
        if (actor->f139()->field_0 > lbl_eu_80666C60) {
            self->field_0x18->sub.vfn00(0x200);
        }
    }
}

void func_800CC01C(void) {}

// Move-effect update driver. Gates on the bound event object (+0x3F60), the
// presentation probe, elapsed-time thresholds and global mode bits, then
// resolves the handler object and drives the effect attach / reposition path.
// NOTE: a middle block of the retail routine was not available during this
// pass; the reconstructed flow covers the gate chain, handler resolution and
// the visible tail.
void func_800CC020(CfObjectImplMoveObj* self) {
    CfActorObj* actor = self->field_0x18;
    CfMoveEvt60* evt = (CfMoveEvt60*)actor->sub.field_C4;      // actor +0x3F60
    if (evt == nullptr)
        return;
    getInstance__Q22cf13CfGameManagerFv();
    if (func_80496288(lbl_eu_80663E14) == lbl_eu_80666C60)
        return;
    f32 t = evt->field_4F8;
    if (!(t > lbl_eu_80666C60) || evt->field_4AC == 8)
        return;
    self->vfD8();
    if ((actor->sub.field_64 & 2) == 0)
        return;
    u32 mode = lbl_eu_80663E24;
    // Bits 6 / 21 of the global mode word suppress this effect entirely.
    if (((mode & 0x40) | (mode & 0x200000)) != 0 || !(t > lbl_eu_80666C6C))
        return;
    t = evt->field_4FC;
    void* pos = actor->sub.f41();                              // slot 0xac
    CfMoveVec3f vec;
    vec.x = *(f32*)&((u32*)pos)[0];
    vec.y = *(f32*)&((u32*)pos)[1];
    vec.z = *(f32*)&((u32*)pos)[2];
    if (!(t - vec.y < lbl_eu_80666C70))
        return;
    // TODO(us-800cca54): the middle attach/reposition block of the retail
    // routine was elided from this pass's reference; the gate chain above is
    // complete, the effect dispatch below follows the visible tail.
    vec.y = lbl_eu_80666C74 + t;
    if (self->field_0x1C == nullptr) {
        self->field_0x1C =
            (CfMoveField1CObj*)func_8008187C__Q22cf13CfGameManagerFv(0x16);
        if (self->field_0x1C != nullptr)
            self->field_0x1C->field_0xB0 = (u32)&self->field_0x0C;
    }
    if (self->field_0x1C != nullptr) {
        ((CfMoveEffObj*)self->field_0x18)->vfn9C(&vec);
    }
}

// Battle-entry hook: when the incoming id matches the bound actor's battle
// id (+0x3F60), fire the vf64 entry virtual and hand the actor to the battle
// manager's party-chain helper.
void func_800CC5DC(CfObjectImplMoveObj* self, u32 battleId) {
    CfActorObj* actor = self->field_0x18;
    if (actor != 0 && battleId == actor->sub.field_C4) {
        self->vf64();
        func_800E1B5C(getInstance__Q22cf14CBattleManagerFv(), self->field_0x18);
    }
}

void func_800CC638(CfObjectImplMoveObj* self, u32 id, CfMoveParam* param) {
    // Move-request driver: gate on the driver's id, then resolve the move
    // object from the request mode, reposition it for the 0x65/0x66 event
    // pair, and finally attach partner objects / fire the count effect.
    // The driver/mode are re-read from memory at each use (retail reloads
    // 0x14(r28) / 0xa(r29)), so no locals are cached across calls.
    if (id != self->mSubObj->mSomeId) {
        return;
    }
    CfMoveEventObj* evt = (CfMoveEventObj*)self->mSubObj->field_0x98;
    if (evt != nullptr && evt->f17() > lbl_eu_80666C88) {
        return;
    }
    u8 mode = param->field_0xA;    // scratch r3 (born first, dies before calls)
    u8 type7;                       // saved r31 (1st declared local)
    CfMoveEffObj* obj = nullptr;    // saved r30 (2nd)
    u8 bit7;                        // saved r27 (3rd)
    type7 = param->field_0xE & 0x7f;   // low 7 bits: effect type
    bit7 = (param->field_0xE >> 7) & 1;  // bit 7: flag passed to vfn194
    if (mode == 0) {
        obj = (CfMoveEffObj*)func_8008187C__Q22cf13CfGameManagerFv(param->field_0xC);
    } else if (mode == 2) {
        obj = (CfMoveEffObj*)func_800817BC__Q22cf13CfGameManagerFv(param->field_0xC, 0);
    } else if (mode == 1) {
        obj = (CfMoveEffObj*)self->mSubObj->vfn220(param->field_0xC);
    }
    if (obj == nullptr) {
        return;
    }
    if (param->field_0xA == 0 && param->field_0xC >= 0x65 && param->field_0xC <= 0x66) {
        // Reposition: pull the driver's position objects and hand the merged
        // vector to the move object's slot 0x9C. The posObj fields are copied
        // as words (float -> u32 bit-cast), matching retail stfs/lwz/stw.
        CfMovePosObj* posObj = (CfMovePosObj*)self->mSubObj->vfn128();
        CfMoveVec3f* acVec = (CfMoveVec3f*)self->mSubObj->vfnAC();
        CfMoveVec3f pos = *acVec;
        if (posObj != nullptr) {
            CfMoveFloatBits b0, b1, b2;
            b0.f = posObj->field_0xC;
            b1.f = posObj->field_0x1C;
            b2.f = posObj->field_0x2C;
            *(u32*)&pos.x = b0.w;
            *(u32*)&pos.y = b1.w;
            *(u32*)&pos.z = b2.w;
        }
        obj->vfn9C(&pos);
        return;
    }
    // Common path: attach to the driver, pass the flag, then dispatch by
    // effect type (bitmask {1,4,5,6} or {0x85,0x86} reaches the actor id).
    func_800ACF78(obj, self->mSubObj, 0);
    obj->vfn194(bit7);
    if (self->mSubObj->field_0x98 != 0) {
        func_800ACEF8(obj, (u8*)self->mSubObj->field_0x98 + 0x304);
    }
    u32 t = type7 + 0xff;
    int cond = 0;
    if ((u8)t <= 5 && ((1u << t) & 0x39) != 0) {
        cond = 1;
    }
    int keep = 1;
    if (cond == 0 && (u8)(type7 + 0x7b) > 1) {
        keep = 0;
    }
    if (keep != 0) {
        func_800ACFD8(obj, func_800B708C((int)self->field_0x18->vf298()->field_0x4));
    }
    if (type7 == 2 || type7 == 5 || type7 == 0x85) {
        if (self->mSubObj->field_0x6F8 != 0) {
            func_800ACF78(obj, nullptr,
                (u32)((CfMoveDriverSubObj*)self->mSubObj->field_0x6F8)->vfnA8());
        }
    } else if (type7 == 3 || type7 == 6 || type7 == 0x86) {
        if (self->mSubObj->field_0x6FC != 0) {
            func_800ACF78(obj, nullptr,
                (u32)((CfMoveDriverSubObj*)self->mSubObj->field_0x6FC)->vfnA8());
        }
    }
    if (param->field_0xF != 0 && obj != 0 && obj->field_0x94 != 0) {
        // u8 count -> f32 via the shared 0x43300000 magic double.
        CfMoveF64Conv conv;
        conv.w[1] = (u32)param->field_0xF;
        conv.w[0] = 0x43300000;
        func_804E3CDC(obj->field_0x94, (f32)(conv.d - lbl_eu_80666C90), lbl_eu_80666C64);
    }
}

// Move-event dispatcher (id pre-filtered against mSomeId by the caller
// contract). Switches on the request mode byte (+0xa of the parameter block):
// most modes drive the pair of partner objects at driver +0x6f8 / +0x6fc via
// embedded-sub-object commands, two entries forward the raw block to the
// contact handlers.
// NOTE: the parameter-block view below mirrors the raw retail layout: mode
// byte +0xa, flag byte +0xb, float +0xc (loaded with lfs) and an ASCII digit
// string at +0x1c fed to atoi.
struct CfMoveReqParam {
    u8 _00_09[0xa];
    u8 field_A;
    u8 field_B;
    f32 field_C_f;                // 0x0c (float-loaded)
    u8 _10_1B[0xc];
    char field_1C[8];             // 0x1c (ASCII digit)
};

void func_800CC964(CfObjectImplMoveObj* self, u32 id, CfMoveReqParam* param) {
    if (id != ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->field_C4) {
        return;
    }
    // Case order follows the retail jump-table body emission order (one body
    // per case; retail re-reads self->mSubObj around every dispatch).
    switch (param->field_A) {
    case 0:
        func_800CF064(self, (CfMoveContact*)param);
        break;
    case 1:
        func_800CF810(self, (CfMoveContact*)param);
        break;
    case 2: {
        // Only the first player's own driver plays the jingle.
        if (getPlayer__Q22cf13CfGameManagerFi(0) != (void*)self->mSubObj) {
            return;
        }
        int snd = 0xd0;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            snd = atoi(&param->field_1C[0]);
        }
        if (lbl_eu_80663EF0 != 0) {
            return;
        }
        u32 handle = ((CfObjectImplMoveSubObj*)self->mSubObj)->field_0x74;
        f32 vol = ((f32*)func_8049603C((CScn*)lbl_eu_80663E14))[3];
        func_801BFE20(0, snd, (u8*)(uintptr_t)handle,
            lbl_eu_80666C64 - vol, lbl_eu_80666C98);
        break;
    }
    case 3: {
        int digit = -1;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            digit = atoi(&param->field_1C[0]);
        }
        if (((CfEmbeddedSubObj_3E9C*)self->mSubObj)->field_6F8 != 0 &&
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn210(0xa) == 0) {
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0xa, 0, digit, 0, 0);
        }
        if (((CfEmbeddedSubObj_3E9C*)self->mSubObj)->field_6FC != 0 &&
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn210(0xb) == 0) {
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0xb, 0, digit, 0, 0);
        }
        break;
    }
    case 4: {
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            atoi(&param->field_1C[0]);
        }
        if (((CfEmbeddedSubObj_3E9C*)self->mSubObj)->field_6F8 != 0) {
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0xa);
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0xc);
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0xe);
        }
        if (((CfEmbeddedSubObj_3E9C*)self->mSubObj)->field_6FC != 0) {
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0xb);
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0xd);
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0xf);
        }
        // When the actor's 0x3f28 state halfword is 1, raise command 0x27 too.
        CfActorObj* actor = self->field_0x18;
        if (actor->sub.field_0x8C == 1) {
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0x27);
        }
        break;
    }
    case 5: {
        int digit = -1;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            digit = atoi(&param->field_1C[0]);
        }
        if (((CfEmbeddedSubObj_3E9C*)self->mSubObj)->field_6F8 != 0) {
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0xc, 0, digit, 0, 0);
        }
        if (((CfEmbeddedSubObj_3E9C*)self->mSubObj)->field_6FC != 0) {
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0xd, 0, digit, 0, 0);
        }
        break;
    }
    case 6: {
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            atoi(&param->field_1C[0]);
        }
        if (((CfEmbeddedSubObj_3E9C*)self->mSubObj)->field_6F8 != 0) {
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0xc);
        }
        if (((CfEmbeddedSubObj_3E9C*)self->mSubObj)->field_6FC != 0) {
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0xd);
        }
        break;
    }
    case 7: {
        int digit = -1;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            digit = atoi(&param->field_1C[0]);
        }
        f32 val = param->field_C_f;
        if (((CfEmbeddedSubObj_3E9C*)self->mSubObj)->field_6F8 != 0) {
            if (val != lbl_eu_80666C60) {
                ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0xe, 0, digit, 0, 0);
            }
            if (val == lbl_eu_80666C70) {
                ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0xa);
            }
        }
        if (((CfEmbeddedSubObj_3E9C*)self->mSubObj)->field_6FC != 0) {
            if (val != lbl_eu_80666C60) {
                ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0xf, 0, digit, 0, 0);
            }
            if (val == lbl_eu_80666C70) {
                ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0xb);
            }
        }
        break;
    }
    case 8: {
        // Like case 7 but the non-default volume probe drives the partner
        // object's own 0x20c slot, and the digit argument is fixed at -1.
        CfEmbeddedSubObj_3E9C* drv = (CfEmbeddedSubObj_3E9C*)self->mSubObj;
        f32 val = param->field_C_f;
        if (drv->field_6F8 != 0) {
            ((CfEmbeddedSubObj_3E9C*)drv->field_6F8)->vfn20C(0xe);
            if (val == lbl_eu_80666C70) {
                ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0xa, 0, -1, 0, 0);
            }
        }
        if (drv->field_6FC != 0) {
            ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn20C(0xf);
            if (val == lbl_eu_80666C70) {
                ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0xb, 0, -1, 0, 0);
            }
        }
        break;
    }
    case 9: {
        // Actor-id request: resolve the actor through the vf298 block first,
        // then parse the digit argument.
        u32 aid = (u32)func_800B708C((int)self->field_0x18->vf298()->field_0x4);
        int digit = -1;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            digit = atoi(&param->field_1C[0]);
        }
        ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0x15, aid, digit, 0,
            param->field_B);
        break;
    }
    case 0xa: {
        int digit = -1;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            digit = atoi(&param->field_1C[0]);
        }
        ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0x16, 0, digit, 0,
            param->field_B);
        break;
    }
    case 0xb: {
        int digit = -1;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            digit = atoi(&param->field_1C[0]);
        }
        ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0x17, 0, digit, 0,
            param->field_B);
        break;
    }
    case 0xc: {
        u32 aid = (u32)func_800B708C((int)self->field_0x18->vf298()->field_0x4);
        int digit = -1;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            digit = atoi(&param->field_1C[0]);
        }
        ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0x18, aid, digit, 0,
            param->field_B);
        break;
    }
    case 0xd: {
        int digit = -1;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            digit = atoi(&param->field_1C[0]);
        }
        ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0x19, 0, digit, 0,
            param->field_B);
        break;
    }
    case 0xe: {
        // Partner objects receive the raw 0x208 slot dispatch with 0x19.
        CfEmbeddedSubObj_3E9C* drv = (CfEmbeddedSubObj_3E9C*)self->mSubObj;
        if (drv->field_6F8 != 0) {
            ((CfEmbeddedSubObj_3E9C*)drv->field_6F8)->f128(0x19);
        }
        if (drv->field_6FC != 0) {
            ((CfEmbeddedSubObj_3E9C*)drv->field_6FC)->f128(0x19);
        }
        break;
    }
    case 0xf: {
        int digit = -1;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            digit = atoi(&param->field_1C[0]);
        }
        ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0x1a, 0, digit, 0,
            param->field_B);
        break;
    }
    case 0x16: {
        // Battle-gated 0x27 request: skipped while a battle intro counter is
        // running (CBattleManager +0x20c8).
        CBattleManagerCcGate* bm =
            (CBattleManagerCcGate*)getInstance__Q22cf14CBattleManagerFv();
        if (bm != nullptr && bm->field_20C8 != 0) {
            break;
        }
        int digit = -1;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            digit = atoi(&param->field_1C[0]);
        }
        ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0x27, 0, digit, 0,
            param->field_B);
        break;
    }
    case 0x17:
    case 0x18: {
        int digit = -1;
        if ((u32)((u8)param->field_1C[0] - 0x30) <= 9) {
            digit = atoi(&param->field_1C[0]);
        }
        ((CfEmbeddedSubObj_3E9C*)self->mSubObj)->vfn204(0x2b, 0, digit, 0,
            param->field_B);
        break;
    }
    default:
        break;
    }
}

void func_800CD268(){}

// Camera-shake trigger: when the bound move event object exists, measure the
// distance between the actor's target position and the scene pose position;
// if it is within the threshold (param override, or base constant x event
// scale), fade a shake scale from 1 down to 0 across the threshold band and
// fire the camera shake with the parameter block's vectors scaled by it.
void func_800CD460(CfObjectImplMoveObj* self, CfMoveCd460Target* target,
                   CfMoveCd460Arg* param) {
    CfMoveEvt60* evt = (CfMoveEvt60*)self->field_0x18->sub.field_98;
    if (evt == nullptr) {
        return;
    }

    CfMoveCd460Pose* pose = (CfMoveCd460Pose*)func_80496264(lbl_eu_80663E14, -1);

    // Position delta between the actor and the scene pose: VEC3Sub lowers to
    // the inlined PS kernel; the components are then re-stored through a
    // stack-slot pointer whose address feeds PSVECMag.
    CfMoveVec3f magIn;
    CfMoveVec3f diff;
    Vec* pMag = (Vec*)&magIn;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                        (nw4r::math::VEC3*)&target->pos,
                        (nw4r::math::VEC3*)&pose->pos);
    pMag->x = diff.x; pMag->y = diff.y; pMag->z = diff.z;
    f32 dist = PSVECMag(pMag);

    f32 thresh = param->field_40;
    if (lbl_eu_80666C60 == thresh) {
        CfMoveEvt60* cur = (CfMoveEvt60*)self->field_0x18->sub.field_98;
        thresh = lbl_eu_80666C68 * cur->field_2E8;
    }
    if (dist > thresh) {
        return;
    }

    // Falloff: full shake at 0 distance, fading to 0 at the threshold edge.
    // Written without intermediates so MWCC fuses (dist - C9C*thresh) into
    // the retail fnmsubs.
    f32 scale = lbl_eu_80666C64;
    if (lbl_eu_80666C9C * thresh <= dist) {
        scale = lbl_eu_80666C64 -
                (dist - lbl_eu_80666C9C * thresh) /
                    (lbl_eu_80666C88 * thresh);
    }

    CfMoveCd460Shake shake;
    memcpy(&shake, &param->shake, sizeof(shake));
    // VEC3Scale is the inlined PS kernel behind retail's ps_muls0 pairs.
    nw4r::math::VEC3Scale((nw4r::math::VEC3*)&shake.vecA,
                          (nw4r::math::VEC3*)&shake.vecA, scale);
    nw4r::math::VEC3Scale((nw4r::math::VEC3*)&shake.vecB,
                          (nw4r::math::VEC3*)&shake.vecB, scale);
    func_8007B044(&shake, 0);
}

void func_800BE824(void*, unsigned int);

void CfObjectImplMoveData::func_800CD5C0(unsigned int a, unsigned int b) {
    CfObjectImplMoveSubObj* inner = (CfObjectImplMoveSubObj*)this->mSubObj;
    if (a == inner->mSomeId) {
        func_800BE824(inner, b);
    }
}

// Move-state transition driver (shared entry): gates on the bound move id,
// toggles the 0x1000 state bits on the driver sub-object, runs the
// chainId==2 vf29C presentation chain, executes the per-kind body, then
// forwards the transition to the partner actor linked at sub.field_71C.
void func_800CD5DC(CfObjectImplMoveObj* self, u32 id, u32 kind, u32,
                   u32 chainId, u32 extraParam) {
    if (id != self->mSubObj->mSomeId) {
        return;
    }
    if (self->mSubObj->f07(0x1000) != 0 && kind != 3) {
        self->mSubObj->f06(0x1000);
        if (self->mSubObj->f01(1) != 0) {
            self->mSubObj->f02(1);
            self->mSubObj->f04(1);
        }
    }
    if (kind == 3) {
        self->mSubObj->f04(0x1000);
        self->mSubObj->f02(4);
    }
    CfActorObj* actor = self->field_0x18;
    if (actor == nullptr) {
        return;
    }
    if (chainId == 2) {
        // Presentation chain: walk the actor's vf29C item list; when a chained
        // item carries a live state halfword, raise it on this object.
        if (actor->vf29C(0)->field_0x50 != nullptr) {
            if (((CfMoveVf29CItem*)actor->vf29C(0)->field_0x50)->field_0x48 != 0) {
                self->vf98(
                    ((CfMoveVf29CItem*)actor->vf29C(0)->field_0x50)->field_0x48);
            }
        }
        if ((actor->vf29C(0)->field_0x78 & 0x400) != 0) {
            if (actor->vf2A8() == nullptr) {
                func_800CB9AC(
                    self,
                    (u32)((CfMoveVf29CItem*)actor->vf29C(0)->field_0x50)->field_0x48);
            }
        }
        actor->sub.vfn90(lbl_eu_80666C64);
        if (kind == 2 || kind == 0x11) {
            getInstance__Q22cf13CfGameManagerFv();
            if (!func_8006EF04(0x400)) {
                actor->vf2AC();
            }
        } else {
            getInstance__Q22cf13CfGameManagerFv();
            if (!func_8006EF04(0x400)) {
                if (kind == 0x2d) {
                    u32 prev = actor->vf2A4()->field_48;
                    if (actor->vf2A0()->field_48 == prev) {
                        actor->vf2B0();
                    }
                } else {
                    actor->vf2B0();
                }
            }
        }
    }
    if (kind == 0x31) {
        if (func_800EA444(getInstance__Q22cf14CBattleManagerFv()) == nullptr) {
            actor->vf2B0();
        } else if ((((CfMoveBM824*)func_800EA444(getInstance__Q22cf14CBattleManagerFv()))
                        ->field_824 &
                       0x800) == 0) {
            actor->vf2B0();
        }
    }
    switch (kind) {
    case 3:
    case 4:
    case 5:
    case 6:
    case 0xc:
    case 0xf:
    case 0x11:
    case 0x17:
    case 0x1b:
    case 0x1e:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2a:
    case 0x2b:
    case 0x2c:
    case 0x2d:
    case 0x2e:
    case 0x2f:
    case 0x30: {
        // Shared interrupt/cancel body: battle-gated vision refresh, then a
        // sweep over the bound event object's state word.
        self->field_0x18->sub.vfn204(0, 0, -1, 0, 0);
        CfActorObj* ac1 = self->field_0x18;
        if (func_800EA444(getInstance__Q22cf14CBattleManagerFv()) != nullptr &&
            ((CfMoveBMId*)func_800EA444(getInstance__Q22cf14CBattleManagerFv()))
                    ->field_0 == ac1->sub.field_74 &&
            func_80148778(ac1->field_08, 0xf) == 0 &&
            func_80148778(ac1->field_08, 9) == 0 &&
            func_80148778(ac1->field_08, 0xc) == 0 &&
            func_80148778(ac1->field_08, 0xb) == 0 &&
            (((CfMoveBM824*)func_800EA444(getInstance__Q22cf14CBattleManagerFv()))
                 ->field_824 &
                0x800) == 0) {
            func_800F449C(func_800EA444(getInstance__Q22cf14CBattleManagerFv()));
        }
        CfActorObj* ac2 = self->field_0x18;
        CfMoveC4Obj* evt = (CfMoveC4Obj*)ac2->sub.field_C4;
        u32 t = (evt != nullptr) ? evt->field_374 : 0;
        if (t >= 0xb) {
            t = (evt != nullptr) ? evt->field_374 : 0;
            if (t <= 0xf) {
                t = (evt != nullptr) ? evt->field_374 : 0;
                if (t == 0xf && ac2->sub.field_0x8C >= 0x9c5 &&
                    ac2->sub.field_0x8C <= 0x9c7) {
                    func_80174B4C(ac2, 0x10);
                    goto tail;
                }
                if (func_800DA06C(getInstance__Q22cf14CBattleManagerFv(), ac2) == 0) {
                    CfActorObj* ac3 = self->field_0x18;
                    if ((ac3->field_3374 & 8) == 0) {
                        u32 v1d = ac3->field_04->b30()->field_0;
                        if (func_80174C98(ac3, &v1d, 0x1d) == 0) {
                            CfActorObj* ac4 = self->field_0x18;
                            u32 v8 = ac4->field_04->b30()->field_0;
                            if (func_80174C98(ac4, &v8, 8) == 0) {
                                CfActorObj* ac5 = self->field_0x18;
                                ac5->sub.vfn00(0x200);
                                func_800D9978(getInstance__Q22cf14CBattleManagerFv(),
                                    self->field_0x18);
                            }
                        }
                    }
                }
                self->field_0x18->sub.vfn20C(0x100);
                func_80174B4C(self->field_0x18, 6);
                goto tail;
            }
        }
        t = (evt != nullptr) ? evt->field_374 : 0;
        if (t == 4) {
            func_80174B4C(self->field_0x18, 0x17);
            goto tail;
        }
        t = (evt != nullptr) ? evt->field_374 : 0;
        if (t == 5) {
            func_80174B4C(self->field_0x18, 0x15);
            goto tail;
        }
        t = (evt != nullptr) ? evt->field_374 : 0;
        if (t == 1) {
            func_80174B4C(self->field_0x18, 0x11);
            goto tail;
        }
        t = (evt != nullptr) ? evt->field_374 : 0;
        if (t == 0xa) {
            func_80174B4C(self->field_0x18, 0x10);
            goto tail;
        }
        {
            CfActorObj* ac6 = self->field_0x18;
            u32 v1 = ac6->field_04->b30()->field_0;
            if (func_80174C98(ac6, &v1, 1) != 0) {
                goto tail;
            }
            CfActorObj* ac7 = self->field_0x18;
            u32 v2 = ac7->field_04->b30()->field_0;
            if (func_80174C98(ac7, &v2, 2) != 0) {
                goto tail;
            }
            self->field_0x18->sub.vfn20C(0x200);
            func_80174B4C(self->field_0x18, 3);
        }
        break;
    }
    case 7:
        func_80174B4C(self->field_0x18, 7);
        self->field_0x18->sub.vfn20C(0x100);
        self->field_0x18->sub.vfn20C(0x200);
        self->field_0x18->sub.vfn00(4);
        func_800D9978(getInstance__Q22cf14CBattleManagerFv(), self->field_0x18);
        break;
    case 0x1f:
        func_80174B4C(self->field_0x18, 0x1f);
        break;
    case 8: {
        // Full teardown: clear counters, reset the move buffer state and hand
        // the actor back to the battle/voice managers.
        func_80174B4C(self->field_0x18, 8);
        self->field_0x18->sub.vfn204(0, 0, -1, 0, 0);
        self->field_0x18->sub.vfn204(2, 0, -1, 0, 0);
        self->field_0x18->sub.vfn20C(0x200);
        self->field_0x18->sub.vfn20C(0x100);
        CfActorObj* a4 = self->field_0x18;
        a4->mst.field_214 = 0;
        a4->mst.field_210 = 0;
        self->vf70();
        func_800D9CA0(getInstance__Q22cf14CBattleManagerFv(), self->field_0x18);
        func_802A2D0C(self->field_0x18);
        break;
    }
    case 0x10:
        func_80174B4C(self->field_0x18, 0x10);
        self->field_0x18->sub.vfn204(0, 0, -1, 0, 0);
        break;
    case 0x16:
        self->field_0x18->sub.f03();
        self->field_0x18->sub.vfn204(0, 0, -1, 0, 0);
        self->field_0x18->sub.vfn204(2, 0, -1, 0, 0);
        func_80174B4C(self->field_0x18, 0x16);
        break;
    case 0x15:
        self->field_0x18->sub.f03();
        func_80174B4C(self->field_0x18, 0x15);
        break;
    case 0x18:
        func_80174B4C(self->field_0x18, 0x18);
        break;
    case 0x19: {
        // Release the presentation handler set unless either flag-word bit 30
        // pair is live, in which case refresh vision first.
        self->field_0x18->sub.f03();
        CfActorObj* a9 = self->field_0x18;
        CfMoveC4Obj* e9 = (CfMoveC4Obj*)a9->sub.field_C4;
        if ((a9->sub.field_64 & 2) == 0 || (e9->field_4EC & 2) == 0) {
            func_8004CEF8(e9, 0xb);
            func_80174B4C(self->field_0x18, 0x19);
        }
        ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0x10);
        ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xef);
        ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf0);
        ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf1);
        break;
    }
    case 0x1a: {
        self->field_0x18->sub.f03();
        CfActorObj* aa = self->field_0x18;
        CfMoveC4Obj* ea = (CfMoveC4Obj*)aa->sub.field_C4;
        if ((aa->sub.field_64 & 2) == 0 || (ea->field_4EC & 2) == 0) {
            func_8004CEF8(ea, 0xb);
            func_80174B4C(self->field_0x18, 0x1a);
        }
        ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0x10);
        ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf);
        ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xef);
        ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf0);
        ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf1);
        break;
    }
    case 0x1c:
    case 0x31: {
        // Reset body: clear both request words and the embedded handlers,
        // then re-arm the move buffer bookkeeping.
        func_8004CEF8((CfMoveC4Obj*)((CfActorObj*)self->field_0x18)->sub.field_C4,
            0xb);
        self->field_0x18->sub.vfn204(0, 0, -1, 0, 0);
        self->field_0x18->sub.vfn204(1, 0, -1, 0, 0);
        self->field_0x18->sub.vfn204(2, 0, -1, 0, 0);
        self->field_0x18->sub.f03();
        CfActorObj* ab = self->field_0x18;
        ab->mst.field_214 = 0;
        ab->mst.field_210 = 0;
        func_80174B4C(self->field_0x18, 0x1c);
        self->field_0x18->field_3E98 = 0;
        self->field_0x18->mst.field_8 &= ~0x10;
        break;
    }
    case 0x1d:
        self->field_0x18->field_04->b20(0x200000);
        self->field_0x18->field_04->b20(0x400000);
        func_80174B4C(self->field_0x18, 0x1d);
        break;
    case 9:
    case 0xa:
    case 0xb: {
        // Knockback-strength scaling: the registered probe gates the float
        // dance driven by the slot-0x218 signed halfword.
        self->field_0x18->sub.vfn204(0, 0, -1, 0, 0);
        func_800DA0A4(getInstance__Q22cf14CBattleManagerFv(), self->field_0x18,
            extraParam);
        if (self->field_0x18->vf298()->field_48 >= 0x10) {
            func_80174B4C(self->field_0x18, 0xa);
        } else if (self->field_0x18->vf298()->field_48 >= 7) {
            func_80174B4C(self->field_0x18, 0xb);
        } else {
            func_80174B4C(self->field_0x18, 9);
        }
        CfActorObj* aT = self->field_0x18;
        u32 v9 = aT->field_04->b30()->field_0;
        if (func_80174C98(aT, &v9, 9) == 0) {
            f32 val = ((CfVf218Result*)self->field_0x18->vf218())->field_28;
            if (val > lbl_eu_80666C60) {
                self->field_0x18->sub.vfn90(lbl_eu_80666C64 + val / lbl_eu_80666C78);
            } else {
                self->field_0x18->sub.vfn90(lbl_eu_80666C64 /
                    (lbl_eu_80666C64 + (-val / lbl_eu_80666C78)));
            }
            if (self->field_0x18->sub.vfn94() == lbl_eu_80666C60) {
                self->field_0x18->sub.vfn90(lbl_eu_80666C64);
            }
        }
        break;
    }
    case 0xd:
    case 0xe: {
        CfActorObj* ad = self->field_0x18;
        u32 v802 = ad->field_04->b30()->field_0;
        if (func_80174C98(ad, &v802, 0x802) != 0) {
            func_80174B4C(self->field_0x18, 0xe);
            self->field_0x18->sub.vfn00(0x100);
            goto tail;
        }
        CfActorObj* ae = self->field_0x18;
        u32 v805 = ae->field_04->b30()->field_0;
        if (func_80174C98(ae, &v805, 0x805) != 0) {
            self->field_0x18->sub.vfn00(0x100);
        }
        func_80174B4C(self->field_0x18, 0xd);
        break;
    }
    case 0x12: {
        CfActorObj* af = self->field_0x18;
        u32 v802 = af->field_04->b30()->field_0;
        if (func_80174C98(af, &v802, 0x802) != 0) {
            self->field_0x18->sub.vfn00(0x100);
        }
        func_80174B4C(self->field_0x18, 0x12);
        break;
    }
    case 0x13: {
        CfActorObj* ag = self->field_0x18;
        u32 v802 = ag->field_04->b30()->field_0;
        if (func_80174C98(ag, &v802, 0x802) != 0) {
            self->field_0x18->sub.vfn00(0x100);
        }
        func_80174B4C(self->field_0x18, 0x13);
        break;
    }
    case 0x14: {
        CfActorObj* ah = self->field_0x18;
        u32 v802 = ah->field_04->b30()->field_0;
        if (func_80174C98(ah, &v802, 0x802) != 0) {
            self->field_0x18->sub.vfn00(0x100);
        }
        func_80174B4C(self->field_0x18, 0x14);
        break;
    }
    default:
        break;
    }
tail:
    // Partner forward: when this actor links a partner (sub.field_71C), swap
    // the bound move objects through slot 0x110 and replay the transition on
    // the partner actor; otherwise clear the link.
    {
        CfActorObj* aT2 = self->field_0x18;
        if (aT2->sub.field_71C != 0) {
            CfActorObj* other =
                (CfActorObj*)func_8016FE34(func_800B708C((int)aT2->sub.field_71C));
            if (other != nullptr && other->sub.field_C4 != 0) {
                void* moved = aT2->sub.vfn110()->field_0x18;
                other->sub.vfn110()->field_0x18 = moved;
                void* xfer = aT2->vf5cc();
                other->vf5c8(xfer);
                CfMoveC4Obj* oEvt = (CfMoveC4Obj*)other->sub.field_C4;
                CfMoveC4Obj* sEvt = (CfMoveC4Obj*)aT2->sub.field_C4;
                func_8004CEF8(oEvt, (sEvt != nullptr) ? sEvt->field_374 : 0);
                func_800BE12C((u8*)&other->sub, kind, 0, -1, 1);
                aT2->vf5d0();
            } else {
                aT2->sub.field_71C = 0;
            }
        }
    }
}

// Enum-list effect sweep: wake both field_04 channels, build a filtered enum
// list (type 0x20) bound to this object's vf48 handle, then re-position each
// listed effect at the shared constant vector and hand the list head to
// vf70 before tearing the holder down.
void func_800CE544(CfObjectImplMoveObj* self) {
    self->field_0x18->field_04->b20(0x400000);
    self->field_0x18->field_04->b20(0x800000);
    CfMoveEnumHolder holder;
    func_80043D90(&holder);
    func_800F4A98(func_80043F18(&holder), 0x20, 0);
    u32 handle = self->vf48();
    func_800F6ED0(func_80043F18(&holder), (void*)handle);
    // The count is re-read through func_80043F18 on every use (retail makes
    // a fresh call each time).
    if (((CfMoveEnumList*)func_80043F18(&holder))->field_620 != 0) {
        for (u32 i = 0; i < ((CfMoveEnumList*)func_80043F18(&holder))->field_620; i++) {
            void* entry =
                func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
            self->field_0x18->vf2C4(entry, lbl_eu_80666C60, lbl_eu_80666C60,
                lbl_eu_80666C60);
        }
    }
    self->vf70(func_800F6E08(func_80043F18(&holder)));
    self->vf80();
    __dt__80043E88(&holder, -1);
}

// Move-state reset driver: probes the actor's vf29C item chain (up to three
// calls, each gated on the chained item's +0x50 pointer / +0x48 halfword) to
// raise the presentation virtual or the func_800CB9AC event dispatch, then
// releases the embedded handlers (0xf via slot 0x20, 0x100 via slot 0x30),
// clears the move state bits and resets the driver bookkeeping fields.
void func_800CE6A0(CfObjectImplMoveObj* self) {
    CfMoveVf29CItem* item = self->field_0x18->vf29C(0);
    if (item->field_0x50 != nullptr) {
        CfMoveVf29CItem* item2 = self->field_0x18->vf29C(0);
        if (((CfMoveVf29CItem*)item2->field_0x50)->field_0x48 != 0) {
            // Chained item's state halfword is live: forward it as an event id.
            CfMoveVf29CItem* item3 = self->field_0x18->vf29C(0);
            self->vf98(((CfMoveVf29CItem*)item3->field_0x50)->field_0x48);
        }
    }
    item = self->field_0x18->vf29C(0);
    if ((item->field_0x78 & 0x400) != 0) {
        // Only when the actor's 0x2a8 probe is empty, re-raise the event.
        if (self->field_0x18->vf2A8() == nullptr) {
            CfMoveVf29CItem* item4 = self->field_0x18->vf29C(0);
            func_800CB9AC(self,
                ((CfMoveVf29CItem*)item4->field_0x50)->field_0x48);
        }
    }
    // Release the embedded handler for ids 0xf (slot 0x20) and 0x100
    // (slot 0x30); retail uses lwzu onto actor+8 for each dispatch.
    ((CfMoveHandler8*)self->field_0x18->field_08)->h20(0xf);
    ((CfMoveHandler8*)self->field_0x18->field_08)->h30(0x100);
    self->field_0x18->field_04->b20(0x00400000);
    self->field_0x18->field_04->b20(0x00800000);
    self->field_0x18->sub.vfn14(nullptr);
    func_80174B4C(self->field_0x18, 0x08000000);
    func_80174B4C(self->field_0x18, 0x10000000);
    self->field_0x18->field_3E98 = 0;
    self->field_0x18->mst.field_8 &= ~0x10;
    CfActorMstBlock* mst = &self->field_0x18->mst;
    mst->field_afc = 1;
    mst->field_4 = 1;
    func_8014B2DC(mst->buf);
    CfActorObj* actor = self->field_0x18;
    actor->mst.field_214 = 0;
    actor->mst.field_210 = 0;
    self->field_0x18->vf2B0();
}

void func_800CE8AC(CfObjectImplMoveObj* self) {
    // Virtual dispatch on the sub-object embedded at +0x3e9c of the actor
    // object (vtable slot 0x4c), then chain the result through
    // func_800B708C (actor id lookup) into func_8016FE34.
    func_8016FE34(func_800B708C(
        (int)self->field_0x18->sub.vfn13()));
}

// Handler rebind: validate the actor against two registered handlers (ids
// 0xa/0xb), bail under the game manager presentation mask, then - unless the
// embedded handler already resolves to param - rebind it via func_802A31AC,
// refresh the handler slots, and forward the parameter to vf84.
void func_800CE8E4(CfObjectImplMoveObj* self, void* param) {
    // Retail reloads self->field_0x18 (actor) from memory before every use.
    CfActorObj* actor = self->field_0x18;
    u32 valA = actor->field_04->b30()->field_0;
    if (func_80174C98(actor, &valA, 0xa) != 0) {
        return;
    }
    actor = self->field_0x18;
    u32 valB = actor->field_04->b30()->field_0;
    if (func_80174C98(actor, &valB, 0xb) != 0) {
        return;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04(0x04000000)) {
        return;
    }
    if (self->field_0x18->sub.vfn13() == param) {
        return;
    }
    func_802A31AC(self->field_0x18, self->field_0x18->sub.vfn13(), param);
    self->field_0x18->sub.vfn14(param);
    if (param != nullptr) {
        self->field_0x18->sub.vfn00(4);
    }
    self->vf84(param);
}

// Battle roster sync: wake both field_04 sub-object channels (0x40/0x80),
// resolve this object's actor via the id lookup chain, then copy the battle
// state's per-slot handles into the actor's vf29C item list.
void func_800CEA34(CfObjectImplMoveObj* self) {
    self->field_0x18->field_04->b20(0x400000);
    self->field_0x18->field_04->b20(0x800000);
    // The embedded handler probe result feeds straight into the actor-id lookup.
    if (func_800B708C((int)self->field_0x18->sub.vfn13()) == 0) {
        return;
    }
    void* bmRes = func_800EA444(getInstance__Q22cf14CBattleManagerFv());
    if (bmRes == nullptr) {
        return;
    }
    void* found = func_8016FE34(func_800B708C((int)((CfMoveBMId*)bmRes)->field_0));
    if (found != (void*)self->field_0x18) {
        return;
    }
    int count = 0;
    CfMoveBattleState* state =
        (CfMoveBattleState*)((CfActorObj*)found)->vf298()->field_0x50;
    if (state == nullptr) {
        return;
    }
    int kind = state->field_0x43;
    if (kind == 1) {
        count = state->field_0x44;
    }
    if (kind == 2) {
        count = 1;
    }
    for (int i = 0; i < count; i++) {
        CfMoveVf29CItem* item = ((CfActorObj*)found)->vf29C(i);
        item->field_0x4 = ((CfActorObj*)self->field_0x18)->sub.vfn13();
    }
}

// Rebind the driver sub-object: store the request word at +0x04, resolve the
// source object via func_800BBC0C into +0x14, then derive the actor pointer
// back from it (source - 0x3e9c) when its slot-0x200 probe succeeds.
void func_800CEB68(CfObjectImplMoveObj* self, u8* param) {
    self->field_0x04 = param;
    CfObjectImplMoveSubObj* src = (CfObjectImplMoveSubObj*)func_800BBC0C(param);
    self->mSubObj = src;
    if (src->vfn200() != 0) {
        if (param != 0) {
            param -= 0x3e9c;
        }
        self->field_0x18 = (CfActorObj*)param;
    } else {
        self->field_0x18 = 0;
    }
}

// Move-start dispatcher: when the driver flag word bit 0x4 is clear or the
// global presentation word has bit 0x40000 set, start with enable=0 instead of
// 1; then, if an event object is bound, push the resulting active state into
// its slot-0x88 callback.
void func_800CEBE0(CfObjectImplMoveObj* self) {
    if ((self->mSubObj->field_0x64 & 4) != 0 && (lbl_eu_80663E24 & 0x40000) == 0) {
        self->vf30(0x100, 1);
        if (self->mSubObj->field_0x98 == 0) {
            return;
        }
        if (self->vf40(0x200) != 0) {
            ((CfMoveEventObj*)self->mSubObj->field_0x98)->vfn88(1);
        } else {
            ((CfMoveEventObj*)self->mSubObj->field_0x98)->vfn88(self->vf40(0x100));
        }
    } else {
        self->vf30(0x100, 0);
        if (self->mSubObj->field_0x98 == 0) {
            return;
        }
        if (self->vf40(0x200) != 0) {
            ((CfMoveEventObj*)self->mSubObj->field_0x98)->vfn88(1);
        } else {
            ((CfMoveEventObj*)self->mSubObj->field_0x98)->vfn88(self->vf40(0x100));
        }
    }
}

// Effect-state sync: run the driver's mode query (vf30), bail when no event
// object is bound, then forward the resolved state to the event object's
// slot 0x88 - a literal 1 when vf40(0x200) reports active, otherwise the
// result of the fallback query vf40(0x100).
void func_800CED64(CfObjectImplMoveObj* self, u32 param) {
    self->vf30(0x100, param);
    if (self->mSubObj->field_0x98 == 0) {
        return;
    }
    if (self->vf40(0x200) != 0) {
        ((CfMoveEventObj*)self->mSubObj->field_0x98)->vfn88(1);
    } else {
        u32 state = self->vf40(0x100);
        ((CfMoveEventObj*)self->mSubObj->field_0x98)->vfn88(state);
    }
}

// When the +0x18 actor and its +0x3F2C flag are present, run the vfE4 and
// vf14 virtuals (retail dispatches 0xE4 first, then 0x14).
void func_800CEE28(CfObjectImplMoveObj* self) {
    if (self->field_0x18 == 0)
        return;
    if (*(u32*)((u8*)self->field_0x18 + 0x3F2C) == 0)
        return;
    self->vfE4()->e14();
}

void func_800CEE7C(void) {}

// Impact/land presentation driver: resolves an effect object through the
// bound move event (slot 0x44 refreshes the cached handle at +0x20 when it is
// unset, slot 0x40 queries the active effect), falls back to the event's
// +0x14ac object, builds the presentation vector, picks effect/sound ids from
// the move timers, then drives the effect manager, the attach helper and the
// positional sound.
void func_800CEE80(CfObjectImplMoveObj* self) {
    if (self->mSubObj->field_0x98 == nullptr) {
        return;
    }
    // Retail performs this dead global reload; keep it (volatile in header).
    u32 evtGate = lbl_eu_80661D40;
    (void)evtGate;
    CfMoveEvt98* evt =
        (CfMoveEvt98*)((CfActorObj*)self->field_0x18)->sub.field_98;
    u32 res;
    if (evt == nullptr) {
        res = 0;
    } else if ((evt->field_0x7A4 & 0x40000000) == 0) {
        res = 0;
    } else {
        if (self->field_0x20 == 0xFFFFFFFF) {
            // Refresh the cached effect handle through the event object.
            self->field_0x20 = evt->vf44();
        }
        if (self->field_0x20 == 0xFFFFFFFF) {
            res = 0;
        } else {
            // Retail reloads the registration slot before the query.
            res = (u32)((CfMoveEvt98*)((CfActorObj*)self->field_0x18)
                            ->sub.field_98)->vf40();
        }
    }
    // No live effect: fall back to the event's stored result object.
    if (res == 0) {
        CfMoveEvt98* fb = (CfMoveEvt98*)self->mSubObj->field_0x98;
        if ((fb->field_0x7A4 & 0x40000000) != 0) {
            res = fb->field_0x14AC;
        }
    }
    if (res == 0) {
        return;
    }
    CfMovePosObj* obj = (CfMovePosObj*)res;
    // Buffered component temps: retail allocates x/y/z in source order
    // (f1/f0/f2) but schedules the loads z/y/x between the id constants.
    f32 vz = obj->field_0x2C;
    u32 flag = 0;      // set when the shake counter is positive
    f32 vy = obj->field_0x1C;
    u32 kind = 0x2e;   // default effect id
    f32 vx = obj->field_0xC;
    u32 sndId = 0xd9;  // default impact sound
    CfMoveVec3f vec;
    vec.x = vx;
    vec.y = vy;
    vec.z = vz;
    vec.y = lbl_eu_80666C74 +
            ((CfMoveEvt60*)((CfActorObj*)self->field_0x18)->sub.field_C4)
                ->field_4FC;

    CfObjectImplMoveSubObj* sub = self->mSubObj;
    CfMoveEvt60* e60 =
        (CfMoveEvt60*)((CfEmbeddedSubObj_3E9C*)sub)->field_C4;
    if (e60->field_3C4 > lbl_eu_80666CA0 ||
        e60->field_4F8 < lbl_eu_80666C88) {
        // Move window expired: fixed tier plus a shake trigger while the
        // global presentation counter runs.
        kind = 0x2d;
        sndId = 0xd8;
        if ((int)lbl_eu_80663EF0 > 0) {
            flag = 1;
        }
    } else if ((sub->field_0x64 & 0x8000) != 0) {
        lbl_eu_80663EF0 = 0x3c;
    }
    CfMoveMgrEfView* mgr =
        (CfMoveMgrEfView*)func_8008187C__Q22cf13CfGameManagerFv(kind);
    mgr->vfn9C(&vec);
    CfMoveEvt98* evp = (CfMoveEvt98*)self->mSubObj->field_0x98;
    f32 epos[4] = {evp->field_760, evp->field_764, evp->field_768,
        lbl_eu_80666C64};
    func_800ACC64(mgr, epos);
    if (flag == 0) {
        f32 vol = ((f32*)func_8049603C((CScn*)lbl_eu_80663E14))[3];
        func_801BFDE8(0, sndId, (u32)(uintptr_t)&vec, lbl_eu_80666C64 - vol,
            lbl_eu_80666C98);
    }
    self->field_0x24 = lbl_eu_80666CA4;
}

// Contact/move-start driver: transforms the contact point into world space,
// runs the screen-edge checks, then either fires the fixed 0xce presentation
// chain or selects tiered move/sound ids from the move state (+0x4ac) and
// drives the effect-manager orientation update (quaternion decomposition of
// the surface-relative direction into the slot-0xbc angle triple).
void func_800CF064(CfObjectImplMoveObj* self, CfMoveContact* param) {
    if (self->mSubObj->field_0x98 == 0) {
        return;
    }
    if (self->mSubObj->mSomeId == 0) {
        return;
    }
    if ((((CfMoveEvt60*)self->mSubObj->mSomeId)->field_0xC & 2) == 0) {
        return;
    }

    // World-space transform of the contact point (matrix from the driver or
    // the shared presentation matrix + 0x48).
    void* mtx;
    if ((s8)param->field_1C == 0) {
        mtx = (u8*)func_8048315C() + 0x48;
    } else {
        mtx = ((CfDriverSlot120*)self->mSubObj)->vfn120(&param->field_1C);
    }
    if (mtx == nullptr) {
        return;
    }
    Vec world;
    PSMTXMultVec((const f32(*)[4])mtx, (Vec*)&param->field_10[0], &world);

    // Lift by the constant Y offset, run the screen probe on a copy of the
    // lifted point; the normal buffer is filled by func_804BE4E0.
    nw4r::math::VEC3 lift;
    lift.x = lbl_eu_80666C60;
    lift.y = lbl_eu_80666CA8;
    lift.z = lbl_eu_80666C60;
    nw4r::math::VEC3 lifted;
    nw4r::math::VEC3Add(&lifted, (const nw4r::math::VEC3*)&world, &lift);
    nw4r::math::VEC3 chk = lifted;
    // Variant ABI (r7 ptr + f1): cast keeps the retail arg marshaling while
    // the header decl stays canonical (CfCam BE398Fn convention).
    if (((BE398PtrF32)func_804BE398)(&chk, 0, 0, 0, &lifted, lbl_eu_80666CAC) == 0) {
        return;
    }
    func_804BE4B4(&world, 0);
    CfMoveVec3f nrm;
    func_804BE4E0(&nrm, 0);

    // Only assigned on the early path; the orientation block below reads the
    // stack slot stale on the tier-selection path (matches retail).
    f32 timer98;
    if (((CfMoveEvt60*)self->mSubObj->mSomeId)->field_4F8 > lbl_eu_80666C60) {
        if (((CfMoveEvt60*)self->mSubObj->mSomeId)->field_4AC == 8) {
            return;
        }
        if (param->field_0xA == 0x13 &&
            ((CfMoveEvt60*)self->mSubObj->mSomeId)->field_4F8 >= lbl_eu_80666C9C) {
            func_800CEE80(self);
            return;
        }
        if (!(((CfMoveEvt60*)self->mSubObj->mSomeId)->field_4F8 <= lbl_eu_80666C6C)) {
            return;
        }
        // Early presentation path: attach effect 0x15 at the contact point
        // and fire the fixed 0xce sound chain.
        timer98 = lbl_eu_80666C74 +
                  ((CfMoveEvt60*)self->mSubObj->mSomeId)->field_4FC;
        CfMoveMgrEfView* mgr =
            (CfMoveMgrEfView*)func_8008187C__Q22cf13CfGameManagerFv(0x15);
        mgr->vfn9C((CfMoveVec3f*)&world);
        CfMoveEvt98* evt = (CfMoveEvt98*)self->mSubObj->field_0x98;
        f32 epos[4] = {evt->field_760, evt->field_764, evt->field_768,
            lbl_eu_80666C64};
        func_800ACC64(mgr, epos);
        if (lbl_eu_80663EF0 != 0) {
            return;
        }
        f32 vol = ((f32*)func_8049603C((CScn*)lbl_eu_80663E14))[3];
        u32 handle = self->mSubObj->field_0x74;
        func_801BFE20(0, 0xce, (u8*)(uintptr_t)handle,
            lbl_eu_80666CA8 * (lbl_eu_80666C64 - vol), lbl_eu_80666C98);
        return;
    }

    // Tier selection: map (request mode byte, move state) to an effect id
    // (effId, -1 = skip) and a sound id (sndId, -1 = none).
    int effId = -1;
    u32 sndId = (u32)-1;
    u32 state = ((CfMoveEvt60*)self->mSubObj->mSomeId)->field_4AC;
    u8 sel = param->field_0xA;
    if (sel == 0) {
        if (state == 4) {
            sndId = state + 0xc9;
            effId = 0x17;
        } else if (state == 5) {
            sndId = 0xcf;
            effId = 0x18;
            if ((self->mSubObj->field_0x64 & 2) != 0 &&
            self->mSubObj->field_0x8C == 6) {
                effId = 0x1a;
            }
        } else if (state == 6) {
            effId = 0x19;
            sndId = 0xd1;
        } else if (state == 7) {
            sndId = 0xd2;
        }
    } else if (sel == 0x13) {
        sndId = state + 0xd3;
        if (state <= 3) {
            effId = 0x2b;
        } else if (state == 4) {
            effId = 0x29;
        } else if (state == 5) {
            effId = 0x32;
            sndId = 0xda;
        }
        if (((CfMoveEvt60*)self->mSubObj->mSomeId)->field_39C < lbl_eu_80666CB0) {
            effId += 1;
        }
        if (state == 6) {
            effId = 0x19;
            sndId = 0xdb;
        } else if (state == 7) {
            sndId = 0xd3;
        }
    }

    if (effId >= 0 && state != 1 && state != 2 &&
        (((CfMoveEvt60*)self->mSubObj->mSomeId)->field_4EC & 0x01000000) == 0) {
        // Orientation update: spawn the tiered effect, rebuild its rotation
        // from the move direction (quaternion toward the surface normal),
        // then decompose into the angle triple.
        CfMoveMgrEfView* mgr =
            (CfMoveMgrEfView*)func_8008187C__Q22cf13CfGameManagerFv(effId);
        timer98 += lbl_eu_80666CB4;
        if (state == 5) {
            timer98 += lbl_eu_80666CB8;
        }
        mgr->vfn9C((CfMoveVec3f*)&world);
        CfMoveEvt60* mv = (CfMoveEvt60*)self->mSubObj->mSomeId;
        f32 ang = lbl_eu_80666CBC * mv->field_444;
        f32 k = lbl_eu_80666CC0 * ang;
        (void)SinFIdx__Q24nw4r4mathFf(ang);
        Quaternion dirq;
        dirq.x = ml::CVec3::unitY.x * k;
        dirq.y = ml::CVec3::unitY.y * k;
        dirq.z = ml::CVec3::unitY.z * k;
        f32 k2 = lbl_eu_80666CC0 * ang;
        dirq.w = k2;
        (void)CosFIdx__Q24nw4r4mathFf(k2);
        f32 d = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&ml::CVec3::unitY,
                                    (const nw4r::math::VEC3*)&nrm);

        // Rotation quaternion from unitY toward the surface normal.
        Quaternion q;
        if (d < lbl_eu_80666CC4) {
            q.x = lbl_eu_80666C60;
            q.y = lbl_eu_80666C64;
            q.z = lbl_eu_80666C60;
            q.w = lbl_eu_80666C60;
        } else {
            Vec cross;
            PSVECCrossProduct((const Vec*)&ml::CVec3::unitY,
                              (const Vec*)&nrm, &cross);
            f32 m = lbl_eu_80666C70 * (lbl_eu_80666C64 + d);
            if (m < lbl_eu_80666C60) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 norm = lbl_eu_80666C60;
            if (m >= lbl_eu_80666C60) {
                norm = m * FrSqrt__Q24nw4r4mathFf(m);
            }
            f32 inv = lbl_eu_80666C64 / norm;
            q.x = cross.x * inv;
            q.y = cross.y * inv;
            q.z = cross.z * inv;
            q.w = lbl_eu_80666CBC * norm;
        }
        PSQUATMultiply(&q, &dirq, &q);

        // Quaternion -> angle triple (pitch clamped to +/-pi/2; the doubled
        // clamp sequence mirrors retail).
        f32 dy2 = q.y + q.y;
        f32 dz2 = q.z + q.z;
        f32 dx2 = q.x + q.x;
        f32 pitch = q.w * dy2 - q.x * dz2;
        if (pitch >= lbl_eu_80666C64) {
            pitch = lbl_eu_80666C64;
        } else if (pitch <= lbl_eu_80666CAC) {
            pitch = lbl_eu_80666CAC;
        }
        if (pitch < lbl_eu_80666CAC) {
            pitch = lbl_eu_80666CAC;
        }
        if (pitch > lbl_eu_80666C64) {
            pitch = lbl_eu_80666C64;
        }
        if (!(pitch >= lbl_eu_80666CAC && pitch <= lbl_eu_80666C64)) {
            nw4r::db::Warning(lbl_eu_8052ADB0, 0xe4, lbl_eu_8052AD88);
        }
        pitch = asin(pitch);
        f32 axd = q.x * dx2;
        f32 axy = q.x * dy2;
        f32 azd = q.z * dz2;
        f32 awd = q.w * dz2;
        f32 angs[3];
        f32 lim = lbl_eu_8066A200;
        if (pitch < lim) {
            if (pitch > -lim) {
                f32 ayd = q.y * dy2;
                f32 awx = q.w * dx2;
                angs[0] = lbl_eu_80666CC8 *
                          Atan2FIdx__Q24nw4r4mathFff(
                              q.y * dz2 + awx,
                              lbl_eu_80666C64 - (axd + ayd));
                angs[2] = lbl_eu_80666CC8 *
                          Atan2FIdx__Q24nw4r4mathFff(
                              axy + awd,
                              lbl_eu_80666C64 - (ayd + azd));
            } else {
                angs[2] = lbl_eu_80666C60;
                angs[0] = -(lbl_eu_80666CC8 *
                            Atan2FIdx__Q24nw4r4mathFff(
                                axy - awd,
                                lbl_eu_80666C64 - (axd + azd)));
            }
        } else {
            angs[2] = lbl_eu_80666C60;
            angs[0] = lbl_eu_80666CC8 *
                      Atan2FIdx__Q24nw4r4mathFff(
                          axy - awd,
                          lbl_eu_80666C64 - (axd + azd));
        }
        mgr->vfBC(angs);
        CfMoveEvt98* evt2 = (CfMoveEvt98*)self->mSubObj->field_0x98;
        f32 epos2[4] = {evt2->field_760, evt2->field_764, evt2->field_768,
            lbl_eu_80666C64};
        func_800ACC64(mgr, epos2);
    }

    // Tail: tiered impact sound.
    if ((int)sndId >= 0 && lbl_eu_80663EF0 == 0) {
        f32 vol = ((f32*)func_8049603C((CScn*)lbl_eu_80663E14))[3];
        u32 handle = self->mSubObj->field_0x74;
        func_801BFE20(0, sndId, (u8*)(uintptr_t)handle,
            lbl_eu_80666CA8 * (lbl_eu_80666C64 - vol), lbl_eu_80666C98);
    }
}

// Impact-effect spawner: transforms the incoming contact point into world
// space, runs the screen/ground probes, then either fires the fixed 0x2d dust
// effect (short-timer path) or rebuilds the effect orientation from the move
// direction (quaternion decomposition into the slot-0xbc angle triple) before
// the tiered impact sound.
void func_800CF810(CfObjectImplMoveObj* self, CfMoveContact* param) {
    CfObjectImplMoveSubObj* drv = self->mSubObj;
    if (drv->field_0x98 == 0)
        return;
    if (drv->mSomeId == 0)
        return;
    if ((((CfMoveEvt60*)drv->mSomeId)->field_0xC & 2) == 0)
        return;

    // Contact point -> world space: driver matrix (slot 0x120) or the shared
    // presentation matrix + 0x48.
    void* mtx;
    if ((s8)param->field_1C != 0) {
        mtx = ((CfDriverSlot120*)drv)->vfn120(&param->field_1C);
    } else {
        mtx = (u8*)func_8048315C() + 0x48;
    }
    if (mtx == nullptr)
        return;

    CfMoveVec3f world;
    PSMTXMultVec((const f32(*)[4])mtx, (Vec*)&param->field_10[0],
                 (Vec*)&world);

    // Lift to presentation height and run the screen/ground probes; the
    // probe buffer at +0xa0 receives the surface normal used below.
    CfMoveVec3f lift;
    lift.x = lbl_eu_80666C60;
    lift.y = lbl_eu_80666CA8;
    lift.z = lbl_eu_80666C60;
    CfMoveVec3f wpos;
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&wpos, (nw4r::math::VEC3*)&world,
                        (nw4r::math::VEC3*)&lift);
    CfMoveVec3f probe;
    probe.x = wpos.x;
    probe.y = wpos.y;
    probe.z = wpos.z;
    if (((BE398PtrF32)func_804BE398)(&probe, 0, 0, 0, &wpos, lbl_eu_80666CCC) == 0)
        return;
    func_804BE4B4(&world, 0);
    CfMoveVec3f nrm;
    func_804BE4E0(&nrm, 0);

    f32 scale =
        ((CfMoveEvt60*)self->mSubObj->field_0x98)->field_2E8;   // f26
    f32 timer = ((CfMoveEvt60*)self->mSubObj->mSomeId)->field_4F8;
    // NOTE: retail only initializes t on the short-timer path; the long path
    // reads the stack slot stale and adds its offsets to garbage.
    f32 t;
    u32 sndId;
    if (timer > lbl_eu_80666C60) {
        if (timer > lbl_eu_80666C6C)
            return;
        t = lbl_eu_80666C74 +
            ((CfMoveEvt60*)self->mSubObj->mSomeId)->field_4FC;
        CfMoveMgrEfView* mgr =
            (CfMoveMgrEfView*)func_8008187C__Q22cf13CfGameManagerFv(0x2d);
        if (mgr == nullptr)
            return;
        mgr->vfn9C(&world);
        f32 epos[4];
        epos[0] = ((CfMoveEvt98*)self->mSubObj->field_0x98)->field_760;
        epos[1] = ((CfMoveEvt98*)self->mSubObj->field_0x98)->field_764;
        epos[2] = ((CfMoveEvt98*)self->mSubObj->field_0x98)->field_768;
        epos[3] = lbl_eu_80666C64;
        func_800ACC64(mgr, epos);
        if ((self->mSubObj->field_0x64 & 0x10) != 0) {
            CfMoveVec3f sv;
            sv.x = lbl_eu_80666C6C * scale;
            sv.y = sv.x;
            sv.z = sv.x;
            func_800ACEF8(mgr, &sv);
        }
        sndId = 0x19a;
        if (scale > lbl_eu_80666CD0)
            sndId = 0x19d;
        else if (scale > lbl_eu_80666CD4)
            sndId = 0x19c;
        else if (scale > lbl_eu_80666C70)
            sndId = 0x19b;
    } else {
        u32 state = ((CfMoveEvt60*)self->mSubObj->mSomeId)->field_4AC;
        int effId = 0x2b;
        if (state == 4)
            effId = 0x29;
        else if (state == 5)
            effId = 0x32;
        else if (state == 6)
            effId = 0x18;
        effId++;
        if (effId >= 0 && state != 1 &&
            (((CfMoveEvt60*)self->mSubObj->mSomeId)->field_4EC &
             0x01000000) == 0) {
            // Orientation update: rebuild the effect rotation from the move
            // direction. dirQuat is unitY scaled by k with w = k.
            t += lbl_eu_80666CB4;
            if (state == 5)
                t += lbl_eu_80666CB8;
            CfMoveMgrEfView* mgr = (CfMoveMgrEfView*)
                func_8008187C__Q22cf13CfGameManagerFv(effId);
            mgr->vfn9C(&world);
            f32 ang = lbl_eu_80666CBC *
                      ((CfMoveEvt60*)self->mSubObj->mSomeId)->field_444;
            f32 k = lbl_eu_80666CC0 * ang;
            f32 sy = SinFIdx__Q24nw4r4mathFf(k);
            Quaternion dirq;
            dirq.x = ml::CVec3::unitY.x * k;
            dirq.y = ml::CVec3::unitY.y * k;
            dirq.z = ml::CVec3::unitY.z * k;
            f32 cy = CosFIdx__Q24nw4r4mathFf(k);
            dirq.w = k;
            (void)sy;
            (void)cy;
            f32 d = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&dirq,
                                        (const nw4r::math::VEC3*)&nrm);

            // Rotation quaternion from unitY toward the surface normal,
            // then compose with the direction quaternion.
            Quaternion q;
            if (d < lbl_eu_80666CC4) {
                q.x = lbl_eu_80666C60;
                q.y = lbl_eu_80666C64;
                q.z = lbl_eu_80666C60;
                q.w = lbl_eu_80666C60;
            } else {
                Vec cross;
                PSVECCrossProduct((const Vec*)&ml::CVec3::unitY,
                                  (const Vec*)&nrm, &cross);
                f32 m = lbl_eu_80666C70 * (lbl_eu_80666C64 + d);
                if (m < lbl_eu_80666C60)
                    nw4r::db::Warning(lbl_eu_80526324, 0x273,
                                      lbl_eu_80526300);
                f32 norm = lbl_eu_80666C60;
                if (m > lbl_eu_80666C60) {
                    f32 mag = lbl_eu_80666C64 + d;
                    norm = lbl_eu_80666C70 * mag *
                           FrSqrt__Q24nw4r4mathFf(mag);
                }
                f32 inv = lbl_eu_80666C64 / norm;
                q.x = cross.x * inv;
                q.y = cross.y * inv;
                q.z = cross.z * inv;
                q.w = lbl_eu_80666CBC * norm;
            }
            PSQUATMultiply(&q, &dirq, &q);

            // Quaternion -> angle triple for the effect's slot-0xbc setter.
            f32 dx = q.x + q.x;
            f32 dy = q.y + q.y;
            f32 dz = q.z + q.z;
            f32 sp = -(q.x * dz - q.w * dy);
            if (sp > lbl_eu_80666C64)
                sp = lbl_eu_80666C64;
            else if (sp <= lbl_eu_80666CAC)
                sp = lbl_eu_80666CAC;
            if (sp < lbl_eu_80666CAC)
                sp = lbl_eu_80666CAC;
            if (sp > lbl_eu_80666C64)
                sp = lbl_eu_80666C64;
            f32 pitch = asin(sp);
            f32 lim = lbl_eu_8066A200;
            f32 axd = q.x * dx;
            f32 axy = q.x * dy;
            f32 azdz = q.z * dz;
            f32 awdz = q.w * dz;
            f32 angs[3];
            angs[1] = pitch;
            if (pitch < lim) {
                if (pitch > -lim) {
                    f32 aydy = q.y * dy;
                    f32 aydz = q.y * dz;
                    f32 awdx = q.w * dx;
                    angs[0] = lbl_eu_80666CC8 *
                              Atan2FIdx__Q24nw4r4mathFff(
                                  aydz + awdx,
                                  lbl_eu_80666C64 - (axd + aydy));
                    angs[2] = lbl_eu_80666CC8 *
                              Atan2FIdx__Q24nw4r4mathFff(
                                  axy + awdz,
                                  lbl_eu_80666C64 - (aydy + azdz));
                } else {
                    angs[2] = lbl_eu_80666C60;
                    angs[0] = -(lbl_eu_80666CC8 *
                                Atan2FIdx__Q24nw4r4mathFff(
                                    axy - awdz,
                                    lbl_eu_80666C64 - (axd + azdz)));
                }
            } else {
                angs[2] = lbl_eu_80666C60;
                angs[0] = lbl_eu_80666CC8 *
                          Atan2FIdx__Q24nw4r4mathFff(
                              axy - awdz,
                              lbl_eu_80666C64 - (axd + azdz));
            }
            mgr->vfBC(angs);
            f32 epos[4];
            epos[0] = ((CfMoveEvt98*)self->mSubObj->field_0x98)->field_760;
            epos[1] = ((CfMoveEvt98*)self->mSubObj->field_0x98)->field_764;
            epos[2] = ((CfMoveEvt98*)self->mSubObj->field_0x98)->field_768;
            epos[3] = lbl_eu_80666C64;
            func_800ACC64(mgr, epos);
            if ((self->mSubObj->field_0x64 & 0x10) != 0) {
                CfMoveVec3f sv;
                sv.x = lbl_eu_80666C6C * scale;
                sv.y = sv.x;
                sv.z = sv.x;
                func_800ACEF8(mgr, &sv);
            }
        }
        sndId = 0x196;
        if (scale > lbl_eu_80666CD0)
            sndId = 0x199;
        else if (scale > lbl_eu_80666CD4)
            sndId = 0x198;
        else if (scale > lbl_eu_80666C70)
            sndId = 0x197;
    }
    // Shared tail: tiered impact sound at the scene volume.
    if (lbl_eu_80663EF0 != 0)
        return;
    u32 handle = self->mSubObj->field_0x74;
    f32 vol = ((f32*)func_8049603C((CScn*)lbl_eu_80663E14))[3];
    func_801BFE20(0, sndId, (u8*)(uintptr_t)handle,
                  lbl_eu_80666CA8 * (lbl_eu_80666C64 - vol),
                  lbl_eu_80666C98);
}

void cf::CfObjectImplMove::func_800CFFA0(unsigned int* param) {
    unsigned int* self = reinterpret_cast<unsigned int*>(this);
    if (param == (unsigned int*)self[7]) {
        param[44] = 0;
        self[7] = 0;
    }
}

void cf::CfObjectImplMove::func_800CFFBC() {
    // Adjusted-this destructor thunk: CfObjectImplMove sits at +0xc inside
    // its containing object; retail adjusts this by -0xc and tail-calls the
    // destructor (1-arg form, no delete flag).
    __dt__Q22cf16CfObjectImplMoveFv(reinterpret_cast<u8*>(this) - 0xc);
}

void cf::CfObjectImplMove::func_800CFFC4() {
    // Adjusted-this destructor thunk: CfObjectImplMove sits at +0x10 inside
    // its containing object; retail adjusts this by -0x10 and tail-calls the
    // destructor (1-arg form, no delete flag).
    __dt__Q22cf16CfObjectImplMoveFv(reinterpret_cast<u8*>(this) - 0x10);
}
