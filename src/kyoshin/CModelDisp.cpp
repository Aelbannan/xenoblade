// Auto-scaffolded catalog TU for kyoshin/CModelDisp
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CModelDisp.hpp"
#include "kyoshin/menu/parts/CModelDispEquip.hpp"

// Cross-TU calls (retail unmangled symbols; declared in CModelDisp.hpp)
extern "C" void func_801FC3B0(CModelDisp* self);
extern "C" __declspec(noinline) void func_801FCAC8(CModelDisp* self);
extern "C" void func_801FC218(CModelDisp* self);
extern "C" void func_801FC15C(CModelDisp* self);

// Sub-object ctor/dtor stubs (retail func_801FBEB8 / __dt__801FBF0C). Their
// bodies are separate match targets; they exist here only so the constructor's
// __construct_array call carries the exact retail reloc names.
// (CActParamAnimGame ctor/dtor declared in CModelDispEquip.hpp.)
extern "C" void* func_801FBEB8(CModelDispSub* sub) {
    __ct__Q22cf17CActParamAnimGameFv((cf::CActParamAnimGame*)((u8*)sub + 0xC));
    __construct_array((CModelDispSub*)((u8*)sub + 0x550), (void*)__ct__Q22cf17CActParamAnimGameFv,
                      (void*)__dt__Q22cf17CActParamAnimGameFv, 0x53C, 2);
    return sub;
}

extern "C" void __destroy_arr(void*, void*, int, int);

// Sub-object destructor: destroys the two-member CActParamAnimGame array at
// +0x550 (0x53C stride) and the single one at +0xC, then frees the object
// when the delete flag is set; returns the object (retail mr r3, r30). The
// scoped optimize_for_size merges the r30/r31 saves into stmw/lmw like the
// retail body (plain -O4,p emits two stw's; MWCC_CASES stmw pattern).
#pragma push
#pragma optimize_for_size on
u8* __dt__801FBF0C(CModelDispSub* obj, int flag) {
    if (obj != 0) {
        __destroy_arr((char*)obj + 0x550, (void*)__dt__Q22cf17CActParamAnimGameFv,
                      0x53C, 2);
        __dt__Q22cf17CActParamAnimGameFv((cf::CActParamAnimGame*)((char*)obj + 0xC), -1);
        if (flag > 0) {
            ::operator delete(obj);
        }
    }
    return (u8*)obj;
}
#pragma pop

// Static helper inlined by -ipa file: its inner loops carry PARAMETER bounds,
// so MWCC keeps them as mtctr/bdnz countdown loops even after the call-site
// constants 2/8 are folded (direct constant-bound loops would fully unroll).
static void ClearSubWords(CModelDispSub* sub, int c1, int c2) {
    u8 j;
    for (j = 0; j < c1; j++) {
        (&sub->mFlagFD0)[j] = 0;
    }
    for (j = 0; j < c2; j++) {
        // Retail zeroes 8 words at 0xFD8, spilling 8 bytes past the 0xFF0
        // stride (the tail member is only 0x18 bytes) - reproduced verbatim.
        ((u32*)sub->_FD8)[j] = 0;
    }
}

#pragma push
#pragma optimize_for_size on
CModelDisp::CModelDisp(u8* initParam) {
    mVtbl = lbl_eu_80535390;
    mInitParam = initParam;
    __construct_array(&mSubs[0], (void*)func_801FBEB8, (void*)__dt__801FBF0C, 0xff0, 3);

    f32 v = lbl_eu_806681E8;
    field_2FDC = v;
    field_2FE0 = lbl_eu_806681EC;
    field_2FD8 = 0;
    field_2FE4 = 1;

    for (u8 i = 0; i < 3; i++) {
        CModelDispSub* sub = (CModelDispSub*)((u8*)this + i * 0xFF0);
        sub->mpController = NULL;
        sub->mObj0C = NULL;
        sub->field_10 = NULL;
        sub->field_554 = NULL;
        sub->field_550 = 0;

        // Slot pointers are cleared word-wise off the sub-object base
        // (indexed recompute, matching retail's rlwinm+add addressing).
        for (u8 j = 0; j < 2; j++) {
            ((CModelDispSub*)((u32*)sub + j))->mFlagFD0 = 0;
        }
        for (u8 j = 0; j < 8; j++) {
            // Retail zeroes 8 words at +0xFD8, spilling 8 bytes past the 0xFF0
            // stride (the tail member is only 0x18 bytes) - reproduced verbatim.
            *(u32*)(&((CModelDispSub*)((u32*)sub + j))->_FD8) = 0;
        }
    }
}
#pragma pop

// Bind the three 0xFF0-strided slot sub-objects (+8 payload) via
// func_801FC2B4 (retail stmw frame + indexed loop).
// extern "C" keeps the call-site relocs at the retail unmangled name;
// noinline keeps -ipa from folding the body into its callers.
extern "C" __declspec(noinline) void func_801FC2B4(void* self, CActParamHolder* h);
#pragma push
#pragma optimize_for_size on
// Tears down one 0xFF0-stride holder payload: releases both animation-model
// slots and their controllers, unbinds the two act params, flushes the main
// act param, frees all held objects and clears the owner pointer.
extern "C" void func_801FC2B4(void* self, CActParamHolder* h) {
    if (h->field_0x00 == NULL) {
        return;
    }
    for (u8 i = 0; i < 2; i++) {
        if (h->animModelPtrs[i] != NULL) {
            // Controller pointer lives in the first word of each act param.
            ((CDispFlushVt*)&h->actParams[i])->m38();
        }
    }
    func_80495E60(h->unk_55C);
    h->unk_55C = NULL;
    for (u8 i = 0; i < 2; i++) {
        if (h->animModelPtrs[i] != NULL) {
            func_80495E60((CModelDispObj*)h->animModelPtrs[i]);
            h->animModelPtrs[i] = NULL;
        }
    }
    func_8004B6BC(&h->actParam, h->field_0x04);
    func_8004B6BC(&h->actParam, h->field_0x08);
    ((CDispFlushVt*)&h->actParam)->m38();
    func_80495E60(h->field_0x04);
    func_80495E60(h->field_0x08);
    func_80495E60((CModelDispObj*)h->field_0x00);
    h->field_0x00 = NULL;
}
#pragma pop
#pragma optimize_for_size on  // -O4,s keeps the stmw frame and the rolled loop
extern "C" void func_801FC0C4(CModelDisp* self) {
    for (u8 i = 0; i < 3; i++)
        func_801FC2B4(self, (CActParamHolder*)((u8*)self + i * 0xFF0 + 8));
}
#pragma optimize_for_size off

u8 func_801FC114(void* self) { return ((CModelDisp*)self)->field_2FE4; }

// retail: if (field_2FD8 == 2) { field_2FD8 = 3; field_2FE4 = 0; }
extern "C" void func_801FC13C(CModelDisp* self) {
    if (self->field_2FD8 == 2) {
        self->field_2FD8 = 3;
        self->field_2FE4 = 0;
    }
}

// Advances field_2FE0 by lbl_eu_806681E8 each call. When it reaches
// lbl_eu_806681F4, decrements field_2FDC by lbl_eu_806681F8 (clamped to
// lbl_eu_806681EC) and calls each sub-object's vmethod (+0x48).
extern "C" void func_801FC15C(CModelDisp* self) {
    f32 t = self->field_2FE0 + lbl_eu_806681E8;
    self->field_2FE0 = t;
    if (t >= lbl_eu_806681F4) {
        self->field_2FDC -= lbl_eu_806681F8;
        if (self->field_2FDC < lbl_eu_806681EC) {
            self->field_2FDC = lbl_eu_806681EC;
            self->field_2FD8 = 2;
            self->field_2FE4 = 1;
        }

        // Iterate over 3 sub-objects and call vmethod on controller pointer
        for (u8 i = 0; i < 3; i++) {
            CModelDispSub* sub = (CModelDispSub*)((u8*)self + i * 0xFF0);
            if (sub->mpController != NULL) {
                // vcall: vtable[0x48/4 = 18] - takes field_2FDC as float arg
                typedef void (*VMethod48)(void*, f32);
                VMethod48* vtbl = *(VMethod48**)sub->mpController;
                vtbl[18](sub->mpController, self->field_2FDC);
            }
        }
    }
}

// Retail: destroys the three 0xFF0-stride sub-objects via __destroy_arr,
// then frees the object when the compiler-generated delete flag is set.
// -O4,s gives the retail stmw/lmw two-register save frame.
#pragma push
#pragma optimize_for_size on
CModelDisp::~CModelDisp() {
    __destroy_arr(&mSubs[0], (void*)__dt__801FBF0C, 0xFF0, 3);
}
#pragma pop

// Fade-in driver: advances field_2FDC toward lbl_eu_806681E8 (the target
// value); once past it, clamps and calls each active slot's vmethod (+0x48)
// with the current alpha.
extern "C" void func_801FC218(CModelDisp* self) {
    self->field_2FDC += lbl_eu_806681F8;
    if (lbl_eu_806681E8 < self->field_2FDC) {
        self->field_2FDC = lbl_eu_806681E8;
        self->field_2FD8 = 0;
        self->field_2FE4 = 1;
    }

    for (u8 i = 0; i < 3; i++) {
        CModelDispSub* sub = (CModelDispSub*)((u8*)self + i * 0xFF0);
        if (sub->mpController != NULL) {
            // vmethod at vtable +0x48 takes the current alpha
            typedef void (*VMethod48)(void*, f32);
            VMethod48* vtbl = *(VMethod48**)sub->mpController;
            vtbl[18](sub->mpController, self->field_2FDC);
        }
    }
}


#pragma push
#pragma optimize_for_size on
// Main per-frame update for the three 0xFF0-strided display slots: resolves
// each slot's actor from the enum list, validates it through the actor's
// controller vtable chain, (re)builds the display model + two animation
// objects, applies per-slot colors, and rearms the slot timer (150 frames).
extern "C" void func_801FC3B0(CModelDisp* self) {
    u32 names[3];
    // Post-increment loads fold the base materialization into the first
    // access (retail: lis + lwzu + +4/+8 displacements).
    const u32* src = lbl_eu_80507CD0;
    names[0] = *src++;
    names[1] = *src++;
    names[2] = *src++;
    char* nameBase = lbl_eu_80507CF4; // NUL-separated name strings at +0/+4/+8/+C
    for (u8 i = 0; i < 3; i++) {
        CActParamHolder* h = (CActParamHolder*)((u8*)self + i * 0xFF0 + 8);
        CModelDispListHolder holder;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), names[i], 0);
        if (((CModelDispEnumList*)func_80043F18(&holder))->field_620 == 0) {
            func_801FC2B4(self, h);
            __dt__80043E88(&holder, -1);
            continue;
        }
        CModelDispSlot* slot = func_800F6EC0(func_80043F18(&holder), 0);
        if (slot->field_04 == NULL) {
            __dt__80043E88(&holder, -1);
            continue;
        }
        CModelDispActor* actor = func_800BFC68(slot->field_04);
        // Retail recomputes actor+0x3E9C per vcall instead of caching the
        // embedded-move pointer, freeing one callee-saved register.
        int ok = actor != NULL;
        u32 charId = 0;
        if (ok) {
            charId = actor->field_3F2C;
            if (charId == 0)
                ok = false;
            s16 idx = func_800BE954((CModelDispMoveVt*)&actor->move[0]);
            CModelDispParamSlot* table = func_80062C28(idx, 0);
            for (u8 j = 2; j <= 5; j++) {
                if (((CModelDispMoveVt*)&actor->move[0])->m148(j) == 0)
                    continue;
                CModelDispParamSlot* ps = &table[j];
                if (ps->field_2C->m00(ps, actor->field_3F28) == NULL) {
                    ok = false;
                    break;
                }
            }
            if (actor->field_3F30 == 0 ||
                ((CModelDispMoveVt*)&actor->move[0])->m184() == 0)
                ok = false;
            CModelDispParamSlot* e = func_80062DA4(idx);
            if (e->field_2C->m00(e, 0) == NULL)
                ok = false;
            if (((CDispVt40*)e->field_2C)->m40(e) == 0)
                ok = false;
            if (((CModelDispMoveVt*)&actor->move[0])->m188() == 0)
                ok = false;
        }
        if (h->field_0x00 != NULL && ok != false) {
            // Build: create the display model and rebind both anim slots.
            h->field_0x00 = func_80495E8C((u32)self->mInitParam, charId, -1, 1);
            ((CModelDispModelVt*)h->field_0x00)->m64(0);
            u32 stateBits = ((CModelDispMoveVt*)&actor->move[0])->m148(1);
            ((CModelDispSub*)h)->mFlagFD4 = (stateBits >> 12) & 0x3FF;
            s16 idx2 = func_800BE954((CModelDispMoveVt*)&actor->move[0]);
            CModelDispParamSlot* table2 = func_80062C28(idx2, 0);
            for (u8 j = 2; j <= 5; j++) {
                if (((CModelDispMoveVt*)&actor->move[0])->m148(j) != 0) {
                    CModelDispParamSlot* ps = &table2[j];
                    CModelDispNameParam* p = ps->field_2C->m00(ps, actor->field_3F28);
                    func_804831C4((CModelDispModelVt*)h->field_0x00, p);
                    u32 bits = ((CModelDispMoveVt*)&actor->move[0])->m148(j);
                    // Slot words are addressed word-wise off the holder base.
                    ((CModelDispSub*)((u32*)h + j))->mFlagFD0 = (bits >> 12) & 0x3FF;
                }
            }
            h->field_0x04 = func_800584B8((u32)self->mInitParam, actor->field_3F30,
                                          (char*)lbl_eu_80507CF4);
            h->field_0x08 = func_800584B8(
                (u32)self->mInitParam,
                ((CModelDispMoveVt*)&actor->move[0])->m184(), nameBase + 4);
            ((CDispFlushVt*)&h->actParam)->m38();
            func_8004B624(&h->actParam, (CModelDispModelVt*)h->field_0x00,
                          h->field_0x08,
                          ((CModelDispMoveVt*)&actor->move[0])->m184());
            func_8004B6A4(&h->actParam, h->field_0x04, actor->field_3F30);
            // Clear act-param busy bit 23, then restart its chain buffer.
            ((CDispHolderWordView*)h)->flags18 &= ~0x800000;
            func_8004B9D4(&h->actParam, (void*)1, 0, -1, 0);
            // Per-slot color triple (f1 differs when the weapon id is 6).
            f32 f0 = lbl_eu_806681EC;
            f32 f1 = lbl_eu_806681EC;
            f32 f2 = lbl_eu_806681EC;
            switch (i) {
            case 0:
                f0 = lbl_eu_806681FC;
                break;
            case 1:
                f0 = lbl_eu_80668200;
                f2 = lbl_eu_80668204;
                break;
            case 2:
                f0 = lbl_eu_80668208;
                f2 = lbl_eu_80668204;
                break;
            }
            if (actor->field_3F28 == 6) {
                switch (i) {
                case 0:
                    f1 = lbl_eu_8066820C;
                    break;
                case 1:
                    f1 = lbl_eu_80668210;
                    break;
                case 2:
                    f1 = lbl_eu_80668210;
                    break;
                }
            }
            f32 vec[3];
            vec[0] = f0;
            vec[1] = f1;
            vec[2] = f2;
            func_8004B354(&h->actParam, vec);
            ((CDispHolderWordView*)h)->flags18 |= 0x160;
            ((CDispHolderWordView*)h)->owner10 = self;
            ((CDispHolderWordView*)h)->owner40 = self;
            CModelDispParamSlot* e3 = func_80062DA4(idx2);
            if (actor->field_3F08 & 0x1000) {
                CModelDispNameParam* np = e3->field_2C->m00(e3, 0);
                CModelDispModelVt* am = func_80495E94((u32)self->mInitParam, np);
                h->animModelPtrs[0] = am;
                if (am != NULL) {
                    CModelDispNameParam* bp =
                        func_800BED80((CModelDispMoveVt*)&actor->move[0], 0);
                    ((CModelDispSubVt*)h->field_0x00)->mC4(am, bp, NULL);
                }
            }
            if (actor->field_3F08 & 0x2000) {
                CModelDispNameParam* np = e3->field_2C->m00(e3, 0);
                CModelDispModelVt* am = func_80495E94((u32)self->mInitParam, np);
                h->animModelPtrs[1] = am;
                if (am != NULL) {
                    CModelDispNameParam* bp =
                        func_800BED80((CModelDispMoveVt*)&actor->move[0], 1);
                    ((CModelDispSubVt*)h->field_0x00)->mC4(am, bp, NULL);
                }
            }
            if (actor->field_3F08 & 0x20000 &&
                ((CModelDispMoveVt*)&actor->move[0])->m188() != 0) {
                CModelDispMca mca;
                __ct__CMcaFile(
                    &mca, ((CModelDispMoveVt*)&actor->move[0])->m188());
                h->unk_55C = func_80495EAC((u32)self->mInitParam, mca.mDataAdj,
                                           nameBase + 8);
                for (u8 j = 0; j < 2; j++) {
                    CModelDispModelVt* mp = (CModelDispModelVt*)h->animModelPtrs[j];
                    if (mp != NULL && h->unk_55C != NULL) {
                        CActParamAnimView* ap = &h->actParams[j];
                        ap->field_0x378 = j;
                        func_8005A594(ap);
                        func_8004B624(
                            ap, mp, h->unk_55C,
                            ((CModelDispMoveVt*)&actor->move[0])->m188());
                        func_8004B9D4(ap, (void*)1, 0, -1, 0);
                    }
                }
            }
            // Weapon-id-8 gate: announce via the actor's battle sub-object.
            if (actor->field_3F28 == 8 &&
                func_800822F4__Q22cf13CfGameManagerFv() >= 0x168) {
                ((CModelDispVt28*)actor->field_3F34)->m28(nameBase + 0xC, 0);
            }
            h->timer = 150;
            ((CModelDispModelVt*)h->field_0x00)->m48(self->field_2FDC);
            ((CModelDispModelVt*)h->field_0x00)->m9C(3, 0);
            func_801FCAC8(self);
        }
        // Teardown path: re-tested after the build block (retail shares the
        // already-loaded field_00/ok registers across both tests).
        if (h->field_0x00 == NULL || ok == false) {
            func_801FC2B4(self, h);
        }
        __dt__80043E88(&holder, -1);
    }
}
#pragma pop

// Retail 0x801FBFD8: fetches the pose block for this display's init param,
// builds two vec3 temps with func_8004B60C, applies them to the pose via
// func_8049EFF8, then runs the fade-in state (func_801FC3B0).
extern "C" void func_801FBFD8(CModelDisp* self) {
    void* pose = func_80496264(self->mInitParam, -1);
    f32 vecA[3]; // sp+0x08 in retail
    f32 vecB[3]; // sp+0x14 in retail
    func_8004B60C(&vecA[0], lbl_eu_806681EC, lbl_eu_806681E8, lbl_eu_806681EC);
    func_8004B60C(&vecB[0], lbl_eu_806681EC, lbl_eu_806681F0, lbl_eu_806681F4);
    func_8049EFF8(pose, 0.0f, (void*)&vecA[0], (void*)&vecB[0]);
    func_801FC3B0(self);
}

int func_801FCAC0(void* self) { return 0; }

// Retail 0x801FCAC8: for each active slot, restart its chain buffer once
// (guarded by the signed countdown at +0x550), then flush it via
// func_8004CF00. Resetting the countdown to 150 throttles the restart.
extern "C" __declspec(noinline) void func_801FCAC8(CModelDisp* self) {
    for (u8 i = 0; i < 3; i++) {
        CModelDispSub* sub = (CModelDispSub*)((u8*)self + i * 0xFF0);
        if (sub->mpController == NULL) {
            continue;
        }
        if (sub->field_550 <= 0) {
            func_8004B9D4((u8*)sub + 0x14, (void*)1, 0, -1, 0);
            sub->field_550 = 150;
        }
        func_8004CF00((CActParamAnimView*)((u8*)sub + 0x14));
    }
}

#pragma push
#pragma optimize_for_size on
// Slot rebind scan (func_801FCB4C caller): finds the sub-object whose bound
// model pointer matches param->field_0x3A0, enumerates the object list
// filtered by the per-slot type table, and rebinds the matched actor's
// animation slot via func_801FCB4C. Returns after the first match.
// Defined BEFORE func_801FCB4C and declared noinline so -ipa does not
// fold the callee's body into this dispatch (retail keeps a bl).
// Defined BEFORE func_801FCB4C and declared noinline so -ipa does not
// fold the callee's body into this dispatch (retail keeps a bl).
extern "C" __declspec(noinline) void func_801FCB4C(CModelDisp* self, s32 enable,
                                                   s32 idx, void* arg,
                                                   s32 slotIdx);
void func_801FCBF4(CModelDisp* self, CModelDispParent* param, s32 enable,
                   s32 slotIdx) {
    for (u8 i = 0; i < 3; i++) {
        CModelDispSub* sub = (CModelDispSub*)((u8*)self + i * 0xFF0);
        if (sub->mpController == param->field_0x3A0) {
            u32 names[3];
            // Post-increment loads fold the base materialization into the
            // first access (retail: lis + lwzu + +4/+8 displacements).
            const u32* src = lbl_eu_80507CDC;
            names[0] = *src++;
            names[1] = *src++;
            names[2] = *src++;
            CModelDispListHolder holder;
            func_80043D90(&holder);
            func_800F4A98(func_80043F18(&holder), names[i], 0);
            if (((CModelDispEnumList*)func_80043F18(&holder))->field_620 >= 1) {
                CModelDispSlot* slot = func_800F6EC0(func_80043F18(&holder), 0);
                if (slot->field_04 != NULL) {
                    CModelDispActor* actor = func_800BFC68(slot->field_04);
                    CModelDispNameParam* res = NULL; // name param from the lookup
                    s32 flag = 0;                    // slot kind for the rebinder
                    // Retail compiles both dispatches as switch statements
                    // (chained cmpi + beq arms).
                    switch (enable) {
                    case 0:
                        switch (slotIdx) {
                        case 0:
                            flag = 0;
                            res = func_800BEDC4((CModelDispMoveVt*)&actor->move[0], 0);
                            break;
                        case 1:
                            flag = 1;
                            res = func_800BEDC4((CModelDispMoveVt*)&actor->move[0], 1);
                            break;
                        }
                        if (res != NULL) {
                            func_801FCB4C(self, (s32)actor, i, res, flag);
                        }
                        break;
                    case 1:
                        switch (slotIdx) {
                        case 0:
                            flag = 0;
                            res = func_800BED80((CModelDispMoveVt*)&actor->move[0], 0);
                            break;
                        case 1:
                            flag = 1;
                            res = func_800BED80((CModelDispMoveVt*)&actor->move[0], 1);
                            break;
                        }
                        if (res != NULL) {
                            func_801FCB4C(self, (s32)actor, i, res, flag);
                        }
                        break;
                    }
                }
            }
            __dt__80043E88(&holder, -1);
            return;
        }
    }
}
#pragma pop

// Slot rebind: when enabled and the arg object reports active (count > 0),
// fetches slot idx's animation-model pointer and rebinds it on field_00's
// model interface (+0xC8 reset, then +0xC4 with the new pointer).
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_801FCB4C(CModelDisp* self, s32 enable, s32 idx, void* arg, s32 slotIdx) {
    if (enable == 0)
        return;
    if (func_800BBC04(arg) <= 0)
        return;
    {
        // Slot table is addressed word-wise off the sub-object base.
        CModelDispSub* entry;
        u32 slotOff = slotIdx * 4;
        u32 subOff = idx * 0xFF0;
        CModelDispSub* sub = (CModelDispSub*)((u8*)self + subOff + 8);
        entry = (CModelDispSub*)((u8*)sub + slotOff);
        if (entry->mSlotPtrA != NULL) {
            ((CModelDispSubVt*)sub->field_00)->mC8();
            ((CModelDispSubVt*)sub->field_00)->mC4(entry->mSlotPtrA, arg, NULL);
        }
    }
}
#pragma pop

int func_801FCBEC(void* self) { return 0; }

#pragma push
#pragma optimize_for_size on
// Scans sub-objects for one whose mpController matches param's field_0x3A0,
// then dispatches getNextChainObj / setParam calls for active slots.
void func_801FCDB4(CModelDisp* self, void* param, void* chainArg) {
    for (u8 i = 0; i < 3; i++) {
        CModelDispSub* sub = (CModelDispSub*)((u8*)self + i * 0xFF0);
        if ((u32)sub->mpController == *(u32*)((u8*)param + 0x3A0)) {
            if (sub->mFlagFD0 != 0) {
                sub->mResultA = func_8004B9B8((u8*)sub + 0x14);
                func_8004B9D4(sub->mBuffer, chainArg, 0, -1, 0);
            }

            if (sub->mFlagFD4 != 0) {
                sub->mResultB = func_8004B9B8((u8*)sub + 0x14);
                func_8004B9D4(sub->_A94, chainArg, 0, -1, 0);
            }

            break;
        }
    }
}
#pragma pop

extern "C" void func_801FC11C(void* self) {
    if (*(u8*)((u8*)self + 0x2FD8) != 0) return;
    *(u8*)((u8*)self + 0x2FD8) = 1;
    *(u8*)((u8*)self + 0x2FE4) = 0;
}

// Defined after its callees so -ipa does not fold their bodies into this
// dispatch (retail keeps them as separate bl calls).
// Per-frame state dispatch on field_2FD8.
extern "C" void func_801FC060(CModelDisp* self) {
    switch (self->field_2FD8) {
    case 1:
        func_801FC15C(self);
        break;
    case 2:
        func_801FC3B0(self);
        func_801FCAC8(self);
        break;
    case 3:
        func_801FC218(self);
        break;
    }
}
