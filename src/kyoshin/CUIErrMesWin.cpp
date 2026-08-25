// Auto-scaffolded catalog TU for kyoshin/CUIErrMesWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"

#include "kyoshin/CUIErrMesWin.hpp"
#include "kyoshin/cf/CfGimmick.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <revolution/gx/GXPixel.h>

// Forward declarations (retail functions defined later in this TU).
// func_802B5148 is declared extern "C" + noinline in CUIErrMesWin.hpp so its
// call sites stay out-of-line. func_802B5254 is a noinline stub (matched in a
// separate session) so func_802B515C's call stays a direct `bl func_802B5254`.
// All three keep C linkage so call-site relocs emit the retail unmangled names.
extern "C" void func_802B515C(CErrMesSub* self, int flag);
extern "C" void func_802B58A4(CErrMesSub* self);
extern "C" __declspec(noinline) void func_802B5254(CErrMesSub* self,
                                                    CErrMesObjView* player,
                                                    void* candidate, void* d80);

// func_802B5AC8 is defined later in this TU; forward-declared so func_802B5970
// (defined before it) can call it. C linkage: the retail symbol is the
// unmangled global name func_802B5AC8.
extern "C" int func_802B5AC8(CErrMesOwner* a, CErrMesOwner* b, CErrMesOwner* c);

// func_802B4B84 (us-802b75f4) - reset an error-message record, then hand the
// embedded sub-object to func_802B5148. Defined BEFORE func_802B5148's body so
// MWCC keeps the call out-of-line (retail emits `bl func_802B5148`; placing it
// after the stub definition would inline the empty stub away).
void func_802B4B84(CErrMesEntry* self) {
    func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
    self->field_0 = 0;
    self->field_4 = 0;
    self->field_8 = 0;
    func_802B5148(&self->sub);
}

void func_802B5130(void* self) { ((void(*)(void*))__dt__12CUIErrMesWinFv)((char*)self - 0x54); }


// Dispatch the window-state ptmf table (lbl_eu_8053AEB0, indexed by the state
// byte), then advance the embedded system window.
void CUIErrMesWin::Move() {
    (this->*lbl_eu_8053AEB0[field_0x9C])();
    func_8022B748(&mSysWin);
}

// CUIErrMesWin::Init (us-802b770c) - attach the render callback, then build
// the embedded system window layout via its vtable slot 34 (+0x88).
void CUIErrMesWin::Init() {
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->addRenderCB(render, 0xd, 1);
    reinterpret_cast<CErrMesSysWinView*>(mSysWin)->v32();
}

// CUIErrMesWin::Term (us-802b7768) - detach the render callback, release the
// system window, and clear the global active flag.
void CUIErrMesWin::Term() {
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->removeRenderCB(render);
    func_8022B7F4(&mSysWin);
    lbl_eu_80664C28 = 0;
}

// CUIErrMesWin::cbRenderBefore (us-802b7804) - disable Z testing and draw
// the embedded system window with a layout DrawInfo (raw 0x60-byte buffer
// like CMenuTitle::cbRenderBefore; the dtor is called directly with -1).
extern "C" void func_8022B7C8(void* syswin, nw4r::lyt::DrawInfo* drawInfo);

void CUIErrMesWin::cbRenderBefore() {
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    u8 drawInfo[0x60];
    __ct__Q34nw4r3lyt8DrawInfoFv(drawInfo);
    func_80137250(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
    func_8022B7C8(mSysWin, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
    __dt__Q34nw4r3lyt8DrawInfoFv(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), -1);
}

// CUIErrMesWin factory ctor (us-802b7864, retail stripped name
// __ct__802B4DF4) - singleton creator. Returns 0 when the window already
// exists; otherwise allocates the 0xA4-byte window on the work heap,
// constructs CProcess out-of-line and fills the CTTask/CUIErrMesWin region by
// hand (interim vtable, null ptmf slots, composite vtable + sub-vtables,
// CSysWin storage, state bytes), registers it under `parent` and returns the
// singleton global.
CUIErrMesWin* __ct__802B4DF4(CProcess* parent, u32 a, u16 b, u8 c) {
    if (lbl_eu_80664C28 != 0) {
        return 0;
    }
    CUIErrMesWin* obj = (CUIErrMesWin*)mtl::MemManager::allocate(
        0xa4, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        CErrMesWinCtorView* v = reinterpret_cast<CErrMesWinCtorView*>(obj);
        __ct__8CProcessFv((CProcess*)v);

        // Interim CTTask<CUIErrMesWin> vtable (overwritten below).
        v->mProcessVt = (u32)lbl_eu_8053AFFC;

        // Null ptmf -> the Move/Draw callback slots (0x3C / 0x48). Retail
        // loads ptmf[1]/ptmf[0] before the first store, then reloads each
        // word for the second triple (the stores can alias the non-const
        // global), so mirror that load order explicitly.
        u32* src = __ptmf_null;
        u32 w1 = src[1];
        u32 w0 = src[0];
        v->ptmfMove[0] = w0;
        v->ptmfMove[1] = w1;
        u32 w2 = src[2];
        v->ptmfMove[2] = w2;
        u32 q1 = src[1];
        u32 q0 = src[0];
        v->ptmfDraw[0] = q0;
        v->ptmfDraw[1] = q1;
        u32 q2 = src[2];
        v->ptmfDraw[2] = q2;

        // Final composite vtable + sub-vtable pointers + scalar tail.
        v->mProcessVt = (u32)lbl_eu_8053AF08;
        v->field_0x54 = (u32)lbl_eu_8053AF08 + 0x24;
        v->field_0x58 = (u32)lbl_eu_8053AF08 + 0xac;
        v->field_0x5C = a;
        __ct__CSysWin(v->mSysWin, 0);
        v->field_0x9C = 0;
        v->field_0x9E = b;
        v->field_0xA0 = c;
    }
    lbl_eu_80664C28 = (u32)obj;
    obj->Regist(parent, false);
    return (CUIErrMesWin*)lbl_eu_80664C28;
}

extern "C" void func_802B4EF4(void* self) { *(u8*)((u8*)self + 0x9C) = 1; }

// func_802B4F00 (us-802b7970) - once the embedded CSysWin is ready, mark the
// window state byte as "ready" (2).
void func_802B4F00(CUIErrMesWin* self) {
    if (CSysWin_isReady(&self->mSysWin)) {
        self->field_0x9C = 2;
    }
}

// func_802B4F40 (us-802b79b0) - load the error message text for field_0x9E
// from the string pool, hand it to the embedded CSysWin with the window kind
// from field_0xA0, open it and advance the state byte to 3.
void func_802B4F40(CUIErrMesWin* self) {
    char* msg = func_80136190(lbl_eu_805135E0, lbl_eu_805135E0 + 0xb, self->field_0x9E);
    func_8022B9B4(&self->mSysWin[0], msg, 0);
    func_8022BFC8(reinterpret_cast<CSysWin*>(&self->mSysWin[0]), self->field_0xA0);
    func_8022B8B8(&self->mSysWin[0]);
    self->field_0x9C = 3;
}

// func_802B4FA8 (us-802b7a18) - once the embedded CSysWin is active, mark the
// window state byte as "active" (4).
void func_802B4FA8(CUIErrMesWin* self) {
    if (CSysWin_isActive(&self->mSysWin)) {
        self->field_0x9C = 4;
    }
}

// func_802B4FE8 (us-802b7a58) - on a confirm press (A on Wii pads / X on the
// classic controller mask when func_80086F9C reports classic), close the
// embedded CSysWin and advance the state byte to 5.
void func_802B4FE8(CUIErrMesWin* self) {
    CErrMesPad* pad = reinterpret_cast<CErrMesPad*>(getCurrentPad__Q22cf13CfGameManagerFv());
    // Button mask: 0x10 by default; the classic-controller path uses 0x20000000.
    u32 mask = 0x10;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        mask = 0x20000000;
    }
    if (pad->field_0x4 & mask) {
        func_8022B8E4(&self->mSysWin[0]);
        self->field_0x9C = 5;
    }
}

void func_802B5054(CUIErrMesWin* self) {
    if (CSysWin_isActive(&self->mSysWin)) {
        self->field_0x9C = 6;
    }
}

extern "C" void func_802B5094(void* self) { *(u8*)((u8*)self + 57) = 1; }


void func_802B5138(void* self) { ((void(*)(void*))cbRenderBefore__12CUIErrMesWinFv)((char*)self - 0x58); }

void func_802B5140(void* self) { ((void(*)(void*))__dt__12CUIErrMesWinFv)((char*)self - 0x58); }

// func_802B5148 (us-802b7bb8) - if the sub-object's active flag is set, run its
// update (func_802B58A4); otherwise do nothing.
void func_802B5148(CErrMesSub* self) {
    if (self->field_0 != 0) {
        func_802B58A4(self);
    }
}

// func_802B515C (us-802b7bcc) - per-frame error-message sub-object update.
// With a non-zero `flag`, resolve the player's current action source and, when
// a valid candidate is found, play its voice (func_802B5254); otherwise reset
// the active records (func_802B58A4). With flag == 0, reset when the
// sub-object is active.
extern "C" void func_802B515C(CErrMesSub* self, int flag) {
    if (flag != 0) {
        void* d80 = func_80496264((void*)CfRes_getD80Flag(), -1);
        CErrMesObjView* p1 =
            reinterpret_cast<CErrMesObjView*>(
                func_8016FE34(getPlayer__Q22cf13CfGameManagerFi(0)));
        CErrMesObjView* p2 =
            reinterpret_cast<CErrMesObjView*>(
                func_8016FE34(getPlayer__Q22cf13CfGameManagerFi(0)));
        void* candidate;
        if (p2 == 0) {
            candidate = 0;
        } else {
            BOOL id = p2->objectParam.CObjectParam_UnkVirtualFunc5();
            if (id == 0) {
                candidate = 0;
            } else {
                void* src = func_8016FE34(func_800B708C(id));
                if (src != 0) {
                    candidate = src;
                } else {
                    candidate = 0;
                }
            }
        }
        if (p1 != 0 && candidate != 0) {
            func_802B5254(self, p1, candidate, d80);
        } else {
            func_802B58A4(self);
        }
    } else {
        if (self->field_0 != 0) {
            func_802B58A4(self);
        }
    }
}

// func_802B5254 (us-802b7cc4) - play the error-message voice. Stub (matched in
// a separate session); noinline keeps func_802B515C's call a direct bl.
extern "C" __declspec(noinline) void func_802B5254(CErrMesSub* self,
                                                    CErrMesObjView* player,
                                                    void* candidate, void* d80) {}

// func_802B58A4 (us-802b8314) - clear the active flag on every live record in
// the voice/actor list, the gimmick list and the enemy list, then clear this
// sub-object's own active flag. The actor-list items point at the embedded
// +0x3E9C sub-object, so the record base is recovered by subtracting 0x3E9C.
extern "C" void func_802B58A4(CErrMesSub* self) {
    u8* base;
    CErrMesList* list = func_800B6BA4();
    for (CErrMesListNode* node = list->sentinel->next; node != list->sentinel; node = node->next) {
        base = node->object;
        if (base != 0) base -= 0x3E9C;
        reinterpret_cast<CErrMesRecord*>(base)->field_0x4590 = 0;
    }
    CfGimmickList* glist = func_800B6BC8();
    for (CfGimmickListNode* gnode = glist->head->next; gnode != glist->head; gnode = gnode->next) {
        base = reinterpret_cast<u8*>(gnode->object);
        if (base != 0) base -= 0x3E9C;
        reinterpret_cast<CErrMesRecord*>(base)->field_0x4590 = 0;
    }

    CErrMesList* elist = func_800B6C7C();
    for (CErrMesListNode* enode = elist->sentinel->next; enode != elist->sentinel; enode = enode->next) {
        reinterpret_cast<CErrMesEnemyObj*>(enode->object)->field_0x6F4 = 0;
    }

    self->field_0 = 0;
}

// func_802B5970 (us-802b83e0) - play an error-message voice for a battle
// triple (owner, attacker, target). Resolves both actors' action sources,
// requires both to exist and be voice-idle, picks a voice id via
// func_802B5AC8, verifies the arts-state gate (func_80174C98, mode 0x803)
// and allocates a 0xA0 voice handle before playing through the attacker's
// embedded +0x3E9C sub-object. Always returns 0.
int func_802B5970(CErrMesOwner* owner, int actorA, int actorB) {
    // Declared hB first so MWCC's allocator hands hA the higher register
    // (retail keeps hA in r31, hB in r30); computed hA first (retail order).
    CErrMesVoiceHandle* hB;
    CErrMesVoiceHandle* hA = reinterpret_cast<CErrMesVoiceHandle*>(
        func_8016FE34(func_800B708C(actorA)));
    hB = reinterpret_cast<CErrMesVoiceHandle*>(
        func_8016FE34(func_800B708C(actorB)));
    if (hA == 0 || hB == 0) return 0;
    if (hA->isActive() != 0) return 0;
    if (hB->isActive() != 0) return 0;
    int voiceId = func_802B5AC8(owner, reinterpret_cast<CErrMesOwner*>(hA),
                                reinterpret_cast<CErrMesOwner*>(hB));
    if (voiceId <= 0) return 0;

    // Query the attacker's +4 arts-state sub-object and gate on its word.
    u32 val = *(u32*)hA->unk4->v10();
    if (func_80174C98(hA, &val, 0x803) == 0) return 0;
    if (func_802A330C(0xa0, 1) == 0) return 0;

    CCharVoice* voice = reinterpret_cast<CCharVoice*>(hA);
    if (hA != 0) voice = reinterpret_cast<CCharVoice*>(&hA->sub);
    func_802A3D54(voice, voiceId, 0xa0);
    return 0;
}

// func_802B5AC8 (us-802b8538) - pick the error-message voice id for a
// battle-relevant triple (owner/attacker/target flag words). Requires the
// attacker flag bit 2 and both other flag bit 1, plus a 25% random gate;
// then maps the two actors' battle states (func_802A77E8) to a voice id
// (0x900..0x902) or -1. C linkage so the definition emits the retail
// unmangled name func_802B5AC8.
extern "C" int func_802B5AC8(CErrMesOwner* a, CErrMesOwner* b, CErrMesOwner* c) {
    if (!(a->field_0x3F00 & 0x4)) return -1;
    if (!(b->field_0x3F00 & 0x2)) return -1;
    if (!(c->field_0x3F00 & 0x2)) return -1;
    if (ml::math::mtRand(100) >= 25) return -1;
    int x = func_802A77E8(b);
    int y = func_802A77E8(c);
    if (x == 1 && y == 3) return 0x900;
    if (x == 1 && y == 2) return 0x901;
    if (x == 1 && y == 4) return 0x902;
    if (x == 3 && y == 2) return 0x901;
    return -1;
}

// CUIErrMesWin::~CUIErrMesWin (us-802b769c) - destroy the embedded CSysWin,
// then the CProcess base (CTTask's empty dtor inlines down to CProcess), then
// conditional operator delete when the deleting flag is set.
CUIErrMesWin::~CUIErrMesWin() {
    __dt__7CSysWinFv(mSysWin, -1);
}

// CMenuTitle::~CMenuTitle (us-802b8690) - destroy the +0x60 CTitle sub-object,
// then the CProcess base, then conditional operator delete.
CMenuTitle::~CMenuTitle() {
    __dt__6CTitleFv(&field_0x60[0], -1);
}

// CMenuTitle::Init (us-802b8700) - attach the render callback, then initialize
// the +0x60 sub-object.
void CMenuTitle::Init() {
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->addRenderCB(render, 0xd, 1);
    func_802B73D4(&field_0x60[0]);
}

// CMenuTitle::Term (us-802b8750) - wait for draw completion, detach the render
// callback, release the +0x60 sub-object, and clear the global active flag.
void CMenuTitle::Term() {
    CDeviceVI::waitForDrawDone();
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->removeRenderCB(render);
    func_802B74F4(&field_0x60[0]);
    lbl_eu_80664C30 = 0;
}

// Dispatch the state ptmf table (lbl_eu_8053B040, indexed by the +0xE8 state
// byte), then advance the +0x60 sub-object (func_802B744C).
void CMenuTitle::Move() {
    (this->*lbl_eu_8053B040[field_0xE8])();
    func_802B744C(&field_0x60[0]);
}

// CMenuTitle::cbRenderBefore (us-802b87f0) - when the +0xE9 gate byte is set,
// disable Z testing and draw the +0x60 title sub-object with a layout DrawInfo
// (raw 0x64-byte buffer: MWCC would virtual-dispatch a class-typed local's
// dtor, retail calls __dt__Q34nw4r3lyt8DrawInfoFv directly with -1).
void CMenuTitle::cbRenderBefore() {
    if (field_0xE9 != 0) {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        u8 drawInfo[0x60];
        __ct__Q34nw4r3lyt8DrawInfoFv(drawInfo);
        func_80137250(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
        func_802B74A8(&field_0x60[0], reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
        __dt__Q34nw4r3lyt8DrawInfoFv(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), -1);
    }
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CTTask<T> is declared in kyoshin/CTaskGameEff.hpp (via harness_catalog.hpp);
// specializations below emit the retail Move/Draw/dtor symbols.
// Test the ptmf hook at +0x3C / +0x48 and dispatch through __ptmf_scall if
// non-null (static_cast mirrors CTTask.hpp's inline body).
// CTTask<CUIErrMesWin>::~CTTask (us-802b7644) - empty body; MWCC emits the
// null check, CProcess base dtor call, and conditional operator delete.
template<>
CTTask<CUIErrMesWin>::~CTTask() {}

template<> void CTTask<CUIErrMesWin>::Move() {
    if (mMoveFunc) {
        (static_cast<CUIErrMesWin*>(this)->*mMoveFunc)();
    }
}

template<> void CTTask<CUIErrMesWin>::Draw() {
    if (mDrawFunc) {
        (static_cast<CUIErrMesWin*>(this)->*mDrawFunc)();
    }
}
class CMenuTitle;
template<> CTTask<CMenuTitle>::~CTTask() {}

// func_802B48A0 (us-802b7310) - reset an error-message record, then hand the
// embedded sub-object to func_802B5148.
void func_802B48A0(CErrMesEntry* self) {
    self->field_0 = 0;
    self->field_4 = 0;
    self->field_8 = 0;
    func_802B5148(&self->sub);
}

// func_802B48B8 (us-802b7328) - forward the record's sub-object to
// func_802B515C, passing 1 if the record is marked active (byte 8) in state 2
// (word 4), else 0.
void func_802B48B8(CErrMesEntry* self) {
    int flag = 0;
    if (self->field_8 != 0 && self->field_4 == 2) {
        flag = 1;
    }
    func_802B515C(&self->sub, flag);
}
// func_802B48E4 (us-802b7354) - (re)register an error-message entry against
// `owner`: keep the record untouched when it is already registered for the
// same owner word; otherwise store the owner word, mark the record active and
// snapshot the battle-active bit. On a fresh registration dispatch 0x29 (or
// 0x24 when battle is not active) with the owner's +0x3E9C sub-object.
void func_802B48E4(CErrMesEntry* self, CErrMesOwner* owner) {
    int flag;
    if (self->field_4 == 1 && self->field_0 == owner->field_0x3F10) {
        flag = 0;
    } else {
        self->field_0 = owner->field_0x3F10;
        self->field_4 = 1;
        self->field_8 = (u8)((owner->field_0x3F00 >> 1) & 1);
        flag = 1;
    }
    if (flag != 0) {
        if (owner->field_0x3F00 & 0x2) {
            // Pass the embedded +0x3E9C sub-object address, or 0 when owner is
            // null. Expressed as default+override so MWCC emits the retail
            // conditional `addi` (no separate null materialization).
            u8* sub = reinterpret_cast<u8*>(owner);
            if (owner != 0) sub = &owner->field_0x3E9C;
            func_80081F28__Q22cf13CfGameManagerFv(0x29, sub);
        } else {
            u8* sub = reinterpret_cast<u8*>(owner);
            if (owner != 0) sub = &owner->field_0x3E9C;
            func_80081F28__Q22cf13CfGameManagerFv(0x24, sub);
        }
    }
}
// func_802B4968 (us-802b73d8) - register an error-message entry against
// `owner` in state 2 (mirror of func_802B48E4's state-1 logic). On a fresh
// registration, snapshot the battle-active bit and dispatch 0x27 (or 0x11
// when the actor's battle counter is below 4) with the owner's +0x3E9C
// sub-object.
void func_802B4968(CErrMesEntry* self, CErrMesOwner* owner) {
    int flag;
    if (self->field_4 == 2 && self->field_0 == owner->field_0x3F10) {
        flag = 0;
    } else {
        self->field_0 = owner->field_0x3F10;
        self->field_4 = 2;
        self->field_8 = (u8)((owner->field_0x3F00 >> 1) & 1);
        flag = 1;
    }
    if (flag != 0) {
        if (owner->field_0x3F00 & 0x2) {
            CErrMesActor15E4* src = reinterpret_cast<CErrMesActor15E4*>(
                func_8016FE34(func_800B708C(
                    reinterpret_cast<CErrMesObjView*>(owner)
                        ->objectParam.CObjectParam_UnkVirtualFunc5())));
            // Materialise the battle-counter check as a 0/1 word (retail keeps
            // it in r0 and re-tests it) rather than fusing it into a branch.
            int cond;
            if (src != 0) {
                cond = src->field_0x15E4 >= 4;
            } else {
                cond = 0;
            }
            if (cond == 0) {
                u8* sub = reinterpret_cast<u8*>(owner);
                if (owner != 0) sub = &owner->field_0x3E9C;
                func_80081F28__Q22cf13CfGameManagerFv(0x11, sub);
            } else {
                u8* sub = reinterpret_cast<u8*>(owner);
                if (owner != 0) sub = &owner->field_0x3E9C;
                func_80081F28__Q22cf13CfGameManagerFv(0x27, sub);
            }
        }
    }
}
// func_802B4A68 (us-802b74d8) - register an error-message entry against
// `owner` in state 3 (mirror of func_802B48E4's state-1 logic). On a fresh
// registration, snapshot the battle-active bit; when battle is active dispatch
// 0x28 (or 0x12 when the actor's battle counter is below 4) with the owner's
// +0x3E9C sub-object, otherwise dispatch 0x25.
void func_802B4A68(CErrMesEntry* self, CErrMesOwner* owner) {
    int flag;
    if (self->field_4 == 3 && self->field_0 == owner->field_0x3F10) {
        flag = 0;
    } else {
        self->field_0 = owner->field_0x3F10;
        self->field_4 = 3;
        self->field_8 = (u8)((owner->field_0x3F00 >> 1) & 1);
        flag = 1;
    }
    if (flag != 0) {
        if (owner->field_0x3F00 & 0x2) {
            CErrMesActor15E4* src = reinterpret_cast<CErrMesActor15E4*>(
                func_8016FE34(func_800B708C(
                    reinterpret_cast<CErrMesObjView*>(owner)
                        ->objectParam.CObjectParam_UnkVirtualFunc5())));
            // Materialise the battle-counter check as a 0/1 word (retail keeps
            // it in r0 and re-tests it) rather than fusing it into a branch.
            int cond;
            if (src != 0) {
                cond = src->field_0x15E4 >= 4;
            } else {
                cond = 0;
            }
            if (cond == 0) {
                u8* sub = reinterpret_cast<u8*>(owner);
                if (owner != 0) sub = &owner->field_0x3E9C;
                func_80081F28__Q22cf13CfGameManagerFv(0x12, sub);
            } else {
                u8* sub = reinterpret_cast<u8*>(owner);
                if (owner != 0) sub = &owner->field_0x3E9C;
                func_80081F28__Q22cf13CfGameManagerFv(0x28, sub);
            }
        } else {
            u8* sub = reinterpret_cast<u8*>(owner);
            if (owner != 0) sub = &owner->field_0x3E9C;
            func_80081F28__Q22cf13CfGameManagerFv(0x25, sub);
        }
    }
}
