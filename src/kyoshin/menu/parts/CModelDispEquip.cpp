// CModelDispEquip - Equipment display model
// High-level C/C++ reconstruction

#include "kyoshin/menu/parts/CModelDispEquip.hpp"

#include "kyoshin/code_80135FDC.hpp"

// Forward decls: state-dispatch callees defined later in this TU.
extern "C" void func_801FFDB0(CModelDispEquip*);
extern "C" void func_801FF9AC(CModelDispEquip*);
extern "C" void func_801FFAC8(CModelDispEquip*);
extern "C" void func_801FFADC(CModelDispEquip*);
extern "C" void func_801FF96C(CModelDispEquip*);
extern "C" __declspec(noinline) void func_801FF6DC(u8* ptr);
extern "C" int func_80200C20(CModelDispEquip* self, u32 param);
extern "C" void func_80200388(void* self, u32 val);
extern "C" void func_801FFBC4(CModelDispEquip*, CActParamHolder*);
extern "C" __declspec(noinline) void func_801FFAB4(float* dest, float a, float b, float c, float d);

// ============================================================
// Target: us-80201254 | __ct__CModelDispEquip (constructor)
// ============================================================
extern "C" void __ct__CModelDispEquip(CModelDispEquip* self, u32 somePtr, s32 equipSlot) {
    CModelDispEquip* obj = self;
    obj->somePtr = somePtr;
    u32 vtbl = (u32)lbl_eu_805354C8;
    CActParamHolder* holder = &obj->actParamHolder;
    obj->_vtable = vtbl;
    obj->_vtable2 = vtbl + 0x88;
    obj->_vtable3 = vtbl + 0xB4;
    __ct__Q22cf17CActParamAnimGameFv(reinterpret_cast<cf::CActParamAnimGame*>(&holder->actParam));
    __construct_array(&holder->actParams[0], (void*)__ct__Q22cf17CActParamAnimGameFv,
                      (void*)__dt__Q22cf17CActParamAnimGameFv, 0x53C, 2);
    obj->weaponId = 0;
    obj->equipSlot = equipSlot;
    obj->state = 0;
    obj->alpha = lbl_eu_80668270;
    obj->alphaTimer = lbl_eu_80668274;
    obj->state20 = 1;
    obj->state21 = 1;
    FileSlot* slot = &obj->fileSlots[0];
    do {
        func_801FF6DC(reinterpret_cast<u8*>(slot));
        slot++;
    } while (slot < &obj->fileSlots[9]);
    obj->modelFileHandle = 0;
    obj->modelData = 0;
    holder->field_0x00 = 0;
    holder->field_0x04 = 0;
    holder->field_0x08 = 0;
    holder->unk_55C = 0;
    holder->timer = 0;
    obj->currentModelPtr = 0;
    // Explicit countdowns keep MWCC from unrolling the small init loops;
    // retail's mtctr/bdnz shapes (MWCC_CASES §16).
    u8 i = 0;
    u8 n = 2;
    do {
        holder->animModelPtrs[i] = 0;
        ((CActParamHolderTail*)holder)->animPtrs[i] = 0;
        i++;
        n--;
    } while (n != 0);
    i = 0;
    n = 8;
    do {
        ((CActParamHolderTail*)holder)->equipPtrs[i] = 0;
        i++;
        n--;
    } while (n != 0);
    FileSlot tmp;
    for (u8 j = 0; j < 9; j++) {
        func_801FF6DC(reinterpret_cast<u8*>(&tmp));
        obj->fileSlots[j] = tmp;
    }
}

// ============================================================
// Target: us-802013cc | func_801FF6DC
// ============================================================
extern "C" __declspec(noinline) void func_801FF6DC(u8* ptr) {
    *(u32*)ptr = 0;
    *(u32*)(ptr + 4) = 0;
    *(u8*)(ptr + 8) = 0;
}

// ============================================================
// Target: us-802013e0 | __dt__801FF6F0
// ============================================================
// Destroys the holder's act-param array via __destroy_arr, then the act-param
// object, then frees the holder if flag > 0 (retail order: array, obj, delete).
extern "C" __declspec(noinline) void* __dt__801FF6F0(CActParamHolder* self, int flag) {
    if (!self)
        goto done;
    __destroy_arr(self->actParams, (void*)__dt__Q22cf17CActParamAnimGameFv, 0x53C, 2);
    __dt__Q22cf17CActParamAnimGameFv(reinterpret_cast<cf::CActParamAnimGame*>(&self->actParam), -1);
    if (flag > 0)
        ::operator delete(self);
done:
    return self;
}

// ============================================================
// Target: us-8020144c | __dt__15CModelDispEquipFv
// ============================================================
// NOTE: retail prologue is `stmw r30` (shape a) - MWCC_CASES says only
// #pragma optimize_for_size merges the saves (forbidden this session);
// keeping the recipe source shape so the pragma can be added later.
extern "C" void* __dt__15CModelDispEquipFv(CModelDispEquip* self, int param) {
    if (self != 0) {
        __dt__801FF6F0(&self->actParamHolder, -1);
        if (param > 0)
            ::operator delete(self);
    }
    return self;
}

// ============================================================
// Target: us-802014a0 | func_801FF7B0
// ============================================================
// Snap scale1/scale2 to the sinit-built global vectors (bit-copied as words,
// retail lwzu/lwz -> stw), then sync pose+scales. Pointer-walk locals trigger
// MWCC's lwzu base fold (MWCC_CASES btm_sco_init note).
extern "C" void func_801FF7B0(CModelDispEquip* self) {
    struct V3 {
        u32 w[3];
    };
    *(V3*)self->scale1 = *(const V3*)lbl_eu_80576550;
    *(V3*)self->scale2 = *(const V3*)lbl_eu_8057655C;
    func_8049EFF8(func_80496264(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// Target: us-8020151c | func_801FF82C
// ============================================================
extern "C" void func_801FF82C(CModelDispEquip* self) {
    switch (self->state) {
    case 0:
        if (self->state21 != 0)
            return;
        ((void (*)(CModelDispEquip*))func_801FFDB0)(self);
        break;
    case 1:
        func_801FF9AC(self);
        break;
    case 2:
        ((void (*)(CModelDispEquip*))func_801FFAC8)(self);
        break;
    case 3:
        func_801FFADC(self);
        break;
    }
}

// ============================================================
// Target: us-80201564 | func_801FF874
// ============================================================
// Reset the equip display: drain the 9 slot + model file jobs, free loaded
// slot buffers and the model arc buffer, then mark the state-21 flag.
extern "C" void func_801FF874(CModelDispEquip* self) {
    waitForDrawDone__9CDeviceVIFv();
    for (u8 i = 0; i < 9; i++) {
        func_801390E0__FPP11CFileHandle(&self->fileSlots[i].handle);
    }
    func_801FFBC4(self, &self->actParamHolder);
    for (u8 i = 0; i < 9; i++) {
        FileSlot* slot = &self->fileSlots[i];
        if (slot->flag != 0) {
            slot->data = 0;
            slot->flag = 0;
        } else if (slot->data != 0) {
            mtl::MemManager::deallocate(slot->data);
            slot->data = 0;
        }
    }
    func_801390E0__FPP11CFileHandle(&self->modelFileHandle);
    if (self->modelData != NULL) {
        func_804CC1D8(lbl_eu_8065FC18);
        if (self->modelData != NULL) {
            mtl::MemManager::deallocate(self->modelData);
            self->modelData = NULL;
        }
    }
    self->state21 = 1;
}

// ============================================================
// Target: us-8020164c | func_801FF95C (getState20)
// ============================================================
extern "C" u8 func_801FF95C(CModelDispEquip* self) {
    return self->state20;
}

// ============================================================
// Target: us-80201654 | func_801FF964 (getState21)
// ============================================================
extern "C" u8 func_801FF964(CModelDispEquip* self) {
    return self->state21;
}

// ============================================================
// Target: us-8020165c | func_801FF96C (setState14)
// ============================================================
extern "C" void func_801FF96C(CModelDispEquip* self) {
    if (self->state != 0) return;
    self->state = 1;
    self->state20 = 0;
}

// ============================================================
// Target: us-8020167c | func_801FF98C (setState14_3)
// ============================================================
extern "C" void func_801FF98C(CModelDispEquip* self) {
    self->state = 3;
    self->state20 = 0;
}

// ============================================================
// Target: us-80201690 | func_801FF9A0
// ============================================================
extern "C" u32 func_801FF9A0(CModelDispEquip* self) {
    return (u32)(u8)self->equipSlot;
}

// ============================================================
// Target: us-8020169c | func_801FF9AC
// ============================================================
// Fade the display out (state 1 step): advance the alpha timer and, once it
// passes the threshold, step alpha down toward 1.0 (clamped, with the state
// flipped to 2), push the new alpha into the act-param object, and write the
// quad color (1, 1, 1, 1-alpha) into both animation-model slots.
void func_801FF9AC(CModelDispEquip* self) {
    self->alphaTimer += lbl_eu_80668270;
    if (self->alphaTimer >= lbl_eu_80668278) {
        self->alpha -= lbl_eu_8066827C;
        if (self->alpha < lbl_eu_80668274) {
            self->alpha = lbl_eu_80668274;
            self->state = 2;
            self->state20 = 1;
        }
        CActParamHolder* holder = &self->actParamHolder;
        void* obj = holder->field_0x00;
        if (obj != 0) {
            reinterpret_cast<CModelDispVt48*>(obj)->m18(self->alpha);
        }
        struct V4 {
            u32 w[4];
        };
        u32 tmp[4];
        func_801FFAB4(reinterpret_cast<float*>(tmp), lbl_eu_80668270, lbl_eu_80668270,
                      lbl_eu_80668270, lbl_eu_80668270 - self->alpha);
        // Explicit countdown keeps MWCC from unrolling the 2-iteration loop;
        // retail's mtctr/bdnz needs unit -O4,s (MWCC_CASES §16).
        u8 i = 0;
        u8 n = 2;
        do {
            CModelDispAnimColor* p =
                (CModelDispAnimColor*)((CActParamHolderTail*)holder)->animPtrs[i];
            if (p != 0)
                *(V4*)&p->field_0x40 = *(const V4*)tmp;
            i++;
            n--;
        } while (n != 0);
    }
}

// ============================================================
// Target: us-802017a4 | func_801FFAB4 (storeFloats)
// ============================================================
extern "C" __declspec(noinline) void func_801FFAB4(float* dest, float a, float b, float c, float d) {
    dest[0] = a;
    dest[1] = b;
    dest[2] = c;
    dest[3] = d;
}

// ============================================================
// Target: us-802017cc | func_801FFADC
// ============================================================
// Fade the equip display in (state 3 step): bump alpha toward the clamp,
// poke the act-param object with the new alpha, then write the quad color
// (clamp, clamp, clamp, clamp - alpha) into both animation-model slots.
extern "C" __declspec(noinline) void func_801FFADC(CModelDispEquip* self) {
    self->alpha += lbl_eu_8066827C;
    if (self->alpha > lbl_eu_80668270) {
        self->alpha = lbl_eu_80668270;
        self->state = 0;
        self->state20 = 1;
    }
    CActParamHolder* holder = &self->actParamHolder;
    void* obj = holder->field_0x00;
    if (obj != 0) {
        ((void (*)(void*, f32))(*(void***)obj)[18])(obj, self->alpha);
    }
    u32 tmp[4];
    func_801FFAB4((float*)tmp, lbl_eu_80668270, lbl_eu_80668270, lbl_eu_80668270,
                  lbl_eu_80668270 - self->alpha);
    for (u8 i = 0; i < 2; i++) {
        CModelDispAnimColor* p =
            (CModelDispAnimColor*)((CActParamHolderTail*)holder)->animPtrs[i];
        if (p != 0) {
            p->field_0x40 = tmp[0];
            p->field_0x44 = tmp[1];
            p->field_0x48 = tmp[2];
            p->field_0x4C = tmp[3];
        }
    }
}

// ============================================================
// Target: us-802018b4 | func_801FFBC4
// ============================================================
// Tear down the equip display: stop the effect slots and anim-model slots,
// release the loaded models/records, and drain the file slots.
extern "C" void func_801FFBC4(CModelDispEquip* self, CActParamHolder* holder) {
    waitForDrawDone__9CDeviceVIFv();
    if (holder->field_0x00 == 0) return;
    for (u8 i = 0; i < 2; i++) {
        CModelDispEffectView* p = reinterpret_cast<CModelDispEffectView*>(
            ((CActParamHolderTail*)holder)->animPtrs[i]);
        if (p != 0) {
            func_804E3D48(p, self != 0 ? reinterpret_cast<CModelDispEffectView*>(
                reinterpret_cast<u8*>(self) + 8) : 0);
            // Re-read the slot: the retail reloads 0xff4(rX) after the call.
            func_804E3CCC(reinterpret_cast<CModelDispEffectView*>(
                ((CActParamHolderTail*)holder)->animPtrs[i]));
            ((CActParamHolderTail*)holder)->animPtrs[i] = 0;
        }
        CModelDispModelVt* m = reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[i]);
        if (m != 0) {
            if (self->equipSlot >= 3) {
                reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->mC8(m);
                func_8004B6BC(&holder->actParams[i], holder->unk_55C);
            }
            reinterpret_cast<CModelDispVtE0*>(&holder->actParams[i])->m38();
        }
    }
    func_80495E60(holder->unk_55C);
    holder->unk_55C = 0;
    for (u8 i = 0; i < 2; i++) {
        CModelDispModelVt* m = reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[i]);
        if (m != 0) {
            func_80495E60(reinterpret_cast<CModelDispObj*>(m));
            holder->animModelPtrs[i] = 0;
        }
    }
    func_8004B6BC(&holder->actParam, holder->field_0x04);
    func_8004B6BC(&holder->actParam, holder->field_0x08);
    reinterpret_cast<CModelDispVtE0*>(&holder->actParam)->m38();
    func_80495E60(holder->field_0x04);
    func_80495E60(holder->field_0x08);
    func_80495E60(reinterpret_cast<CModelDispObj*>(holder->field_0x00));
    holder->field_0x00 = 0;
    for (u8 i = 0; i < 9; i++) {
        if (self->fileSlots[i].flag != 0) {
            self->fileSlots[i].data = 0;
            self->fileSlots[i].flag = 0;
        } else if (self->fileSlots[i].data != 0) {
            mtl::MemManager::deallocate(self->fileSlots[i].data);
            self->fileSlots[i].data = 0;
        }
    }
    func_801390E0__FPP11CFileHandle(&self->modelFileHandle);
    if (self->modelData != 0) {
        func_804CC1D8(lbl_eu_8065FC18);
        if (self->modelData != 0) {
            mtl::MemManager::deallocate(self->modelData);
            self->modelData = 0;
        }
    }
}

// ============================================================
// Target: us-80201ab4 | func_801FFDC4
// ============================================================
// Build the equipment display: select the enum list for the current equip
// slot, verify the first actor (model id, move state, file load), then create
// the display model, wire the param slots + anim models, and start the idle
// animation.
extern "C" void func_801FFDC4(CModelDispEquip* self) {
    CActParamHolder* holder = &self->actParamHolder;
    // Pointer-walk loads trigger the retail lwzu base fold (MWCC_CASES
    // btm_sco_init note): loads grouped before the stores.
    u32* srcp = lbl_eu_80507FD0;
    u32 v0 = *srcp++;
    u32 v1 = *srcp++;
    u32 v2 = *srcp++;
    u32 src[3];
    src[0] = v0;
    src[1] = v1;
    src[2] = v2;
    CModelDispListHolder lh;
    func_80043D90(&lh);
    func_800F4A98(func_80043F18(&lh), src[self->equipSlot], 0);
    CModelDispEnumList* list = func_80043F18(&lh);
    if (list->field_620 >= 1) {
        CModelDispSlot* slot = func_800F6EC0(func_80043F18(&lh), 0);
        cf::CfObjectMove* cfMove = slot->field_04;
        if (cfMove != 0) {
            CModelDispActor* actor = func_800BFC68(cfMove);
            int ready = actor != 0 ? 1 : 0;
            u32 m = 0;
            if (actor != 0) {
                m = actor->field_3F2C;
                if (m == 0) ready = 0;
                if (reinterpret_cast<CModelDispMoveVt*>(&actor->move)->m74() == 0) ready = 0;
                if (func_80200C20(self, actor->field_3F28) == 0) ready = 0;
            }
            if (holder->field_0x00 == 0 && ready != 0) {
                // ---- build the display model ----
                ((CActParamHolderTail*)holder)->currentModelPtr = (u32)actor;
                holder->field_0x00 = func_80495E8C(self->somePtr, m, -1, 1);
                reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->m64(0);
                ((CActParamHolderTail*)holder)->equipPtrs[1] =
                    ((u32)reinterpret_cast<CModelDispMoveVt*>(&actor->move)->m148(1) >> 12) & 0x3FF;
                s16 be = func_800BE954(reinterpret_cast<CModelDispMoveVt*>(&actor->move));
                CModelDispParamSlot* param = func_80062C28(be, 0);
                for (int idx = 2; idx <= 5; idx++) {
                    if (reinterpret_cast<CModelDispMoveVt*>(&actor->move)->m148(idx) != 0) {
                        CModelDispVt8* obj = param[idx].field_2C;
                        func_804831C4(reinterpret_cast<CModelDispModelVt*>(holder->field_0x00),
                                      obj->m00(&param[idx], actor->field_3F28));
                        ((CActParamHolderTail*)holder)->equipPtrs[idx] =
                            ((u32)reinterpret_cast<CModelDispMoveVt*>(&actor->move)->m148(idx) >> 12) & 0x3FF;
                    }
                }
                holder->field_0x04 = func_800584B8(self->somePtr, actor->field_3F30, &lbl_eu_80507FF8[0]);
                holder->field_0x08 = func_800584B8(self->somePtr,
                    reinterpret_cast<CModelDispMoveVt*>(&actor->move)->m184(), &lbl_eu_80507FF8[4]);
                reinterpret_cast<CModelDispVtE0*>(&holder->actParam)->m38();
                func_8004B624(&holder->actParam, reinterpret_cast<CModelDispModelVt*>(holder->field_0x00),
                              holder->field_0x08, reinterpret_cast<CModelDispMoveVt*>(&actor->move)->m184());
                func_8004B6A4(&holder->actParam, holder->field_0x04, actor->field_3F30);
                holder->actParam.field_0x0C &= ~0x800000;
                f32 tmpB[4];
                func_8004B354(&holder->actParam,
                    (const f32*)func_8004B60C(tmpB, lbl_eu_8066827C, lbl_eu_80668274, lbl_eu_80668274));
                holder->actParam.field_0x0C |= 0x160;
                func_80200388(&holder->actParam,
                    self != 0 ? reinterpret_cast<u32>(reinterpret_cast<u8*>(self) + 4) : 0);
                f32 tmpA[4];
                func_8004B60C(tmpA, lbl_eu_8066827C, lbl_eu_80668274, lbl_eu_80668274);
                struct V3 {
                    u32 w[3];
                };
                *(V3*)&self->colorR = *(const V3*)tmpA;
                self->colorA = lbl_eu_80668274;
                CModelDispParamSlot* mainSlot = func_80062DA4(be);
                if ((actor->field_3F08 & 0x1000) != 0) {
                    holder->animModelPtrs[0] = func_80495E94(self->somePtr,
                        mainSlot->field_2C->m00(mainSlot, 0));
                    if (holder->animModelPtrs[0] != 0) {
                        reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->mC4(
                            reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[0]),
                            func_800BED80(reinterpret_cast<CModelDispMoveVt*>(&actor->move), 0), 0);
                    }
                }
                if ((actor->field_3F08 & 0x2000) != 0) {
                    holder->animModelPtrs[1] = func_80495E94(self->somePtr,
                        mainSlot->field_2C->m00(mainSlot, 0));
                    if (holder->animModelPtrs[1] != 0) {
                        reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->mC4(
                            reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[1]),
                            func_800BED80(reinterpret_cast<CModelDispMoveVt*>(&actor->move), 1), 0);
                    }
                }
                if ((actor->field_3F08 & 0x20000) != 0) {
                    if (reinterpret_cast<CModelDispMoveVt*>(&actor->move)->m188() != 0) {
                        CModelDispMca mca;
                        __ct__CMcaFile(&mca, reinterpret_cast<CModelDispMoveVt*>(&actor->move)->m188());
                        holder->unk_55C = func_80495EAC(self->somePtr, mca.mDataAdj, &lbl_eu_80507FF8[8]);
                    }
                }
                for (u8 i = 0; i < 2; i++) {
                    CModelDispModelVt* am =
                        reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[i]);
                    if (am != 0 && holder->unk_55C != 0) {
                        holder->actParams[i].field_0x378 = i;
                        func_8005A594(&holder->actParams[i]);
                        func_8004B624(&holder->actParams[i], am, holder->unk_55C,
                                      reinterpret_cast<CModelDispMoveVt*>(&actor->move)->m188());
                        func_8004B9D4(&holder->actParams[i],
                                      func_8004C5EC(&holder->actParam), 0, -1, 0);
                    }
                }
                if (actor->field_3F28 == 8) {
                    if (func_800822F4__Q22cf13CfGameManagerFv() >= 0x167) {
                        reinterpret_cast<CModelDispVt28*>(actor->field_3F34)->m28(&lbl_eu_80507FF8[0xC], 0);
                    }
                }
                holder->timer = 0x96;
                reinterpret_cast<CModelDispVt48*>(holder->field_0x00)->m18(self->alpha);
                reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->m9C(3, 0);
                self->state21 = 1;
                func_801FF96C(self);
            } else if (holder->field_0x00 != 0 && ready == 0) {
                func_801FFBC4(self, holder);
            }
        }
    } else {
        func_801FFBC4(self, holder);
    }
    __dt__80043E88(&lh, -1);
}

// ============================================================
// Target: us-80202078 | func_80200388
// ============================================================
extern "C" void func_80200388(void* self, u32 val) { *(u32*)((u8*)self + 4) = val; *(u32*)((u8*)self + 0x34) = val; }

// ============================================================
// Target: us-80202084 | func_80200394
// ============================================================
// Build the equipment display for the 4th+ equip slot (equipSlot >= 3,
// dispatched from func_801FFDB0): resolve the character record, load the 3
// shared equip files + 5 per-slot model files, then construct the display
// model and wire up the anim models (mirror of func_801FFDC4's build tail).
extern "C" void func_80200394(CModelDispEquip* self) {
    CActParamHolder* holder = &self->actParamHolder;
    u32 outType;
    CModelDispFileCtx ctx;
    f32 tmpA[3];
    f32 tmpB[3];
    CModelDispMca mca;
    char buf1[0x20];
    char buf2[0x20];
    char buf3[0x20];
    func_801F981C(&ctx);
    if (holder->field_0x00 != 0) return;

    func_8009ECB0();
    u8 charId = (u8)func_801392B4((u8)self->equipSlot);
    if (charId == 0) return;

    CModelDispCharRecord* rec = (CModelDispCharRecord*)func_8009EC9C(charId);
    self->weaponId = rec->weaponId;
    ((CActParamHolderTail*)holder)->equipPtrs[1] = func_80141E90(charId, (s16)(u16)rec->field_0x0E, 1, 0);
    ((CActParamHolderTail*)holder)->equipPtrs[2] = func_80141E90(charId, (s16)(u16)rec->field_0x10, 2, 0);
    ((CActParamHolderTail*)holder)->equipPtrs[3] = func_80141E90(charId, (s16)(u16)rec->field_0x12, 3, 0);
    ((CActParamHolderTail*)holder)->equipPtrs[4] = func_80141E90(charId, (s16)(u16)rec->field_0x14, 4, 0);
    ((CActParamHolderTail*)holder)->equipPtrs[5] = func_80141E90(charId, (s16)(u16)rec->field_0x16, 5, 0);

    u32 v6 = func_80141FE0(self->weaponId & 0xFFFF);
    ((CActParamHolderTail*)holder)->equipPtrs[6] = v6;
    s32 allLoaded = 1;
    // Shared equip files 1-3 (slots 6-8): only start a load when the slot has
    // neither data nor an in-flight handle. The "4/0xC/0xD + bits 20-26 == 0x2B"
    // condition picks a variant filename (extra component in the sprintf).
    if (self->fileSlots[6].data == 0) {
        if (self->fileSlots[6].handle == 0) {
            u32 ex = (v6 >> 20) & 0x7F;
            if ((charId == 4 || (u16)(charId + 0x10000 - 0xC) <= 1) && ex == 0x2B) {
                sprintf(buf1, &lbl_eu_80507FF8[0x11], charId, ex, 0xa);
            } else {
                sprintf(buf1, &lbl_eu_80507FF8[0x11], charId, 0, 0xa);
            }
            int size = getFileSize__11CDeviceFileFPCc(buf1, 1);
            u32 alloc = func_801F9894(&ctx, (u32)size);
            CFileHandle* fh = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(alloc, buf1, self, 0, 0);
            self->fileSlots[6].handle = fh;
            setHandleFlag1__11CDeviceFileFP11CFileHandle(fh);
        }
        allLoaded = 0;
    }
    if (self->fileSlots[7].data == 0) {
        if (self->fileSlots[7].handle == 0) {
            u32 ex2 = (((CActParamHolderTail*)holder)->equipPtrs[6] >> 20) & 0x7F;
            if ((charId == 4 || (u16)(charId + 0x10000 - 0xC) <= 1) && ex2 == 0x2B) {
                sprintf(buf2, &lbl_eu_80507FF8[0x11], charId, ex2, 0x14);
            } else {
                sprintf(buf2, &lbl_eu_80507FF8[0x11], charId, 0, 0x14);
            }
            int size = getFileSize__11CDeviceFileFPCc(buf2, 1);
            u32 alloc = func_801F9894(&ctx, (u32)size);
            CFileHandle* fh = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(alloc, buf2, self, 0, 0);
            self->fileSlots[7].handle = fh;
            setHandleFlag1__11CDeviceFileFP11CFileHandle(fh);
        }
        allLoaded = 0;
    }
    if (self->fileSlots[8].data == 0) {
        if (self->fileSlots[8].handle == 0) {
            char* name = func_80136190(&lbl_eu_80507FF8[0x2F], &lbl_eu_80507FF8[0x3B], rec->weaponId);
            sprintf(buf3, &lbl_eu_80507FF8[0x44], name + 2);
            if (strlen(buf3) >= 0x15) {
                if (buf3[0xE] != '9') buf3[0xE] = '1';
                buf3[0xF] = '0';
                buf3[0x10] = '1';
            }
            u32 ex3 = (((CActParamHolderTail*)holder)->equipPtrs[6] >> 20) & 0x7F;
            if ((charId == 4 || (u16)(charId + 0x10000 - 0xC) <= 1) && ex3 == 0x2B) {
                buf3[0xE] = '3';
            }
            int size = getFileSize__11CDeviceFileFPCc(buf3, 1);
            if (size >= 0) {
                u32 alloc = func_801F9894(&ctx, (u32)size);
                CFileHandle* fh = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(alloc, buf3, self, 0, 0);
                self->fileSlots[8].handle = fh;
                setHandleFlag1__11CDeviceFileFP11CFileHandle(fh);
            }
        }
        allLoaded = 0;
    }

    // Per-slot equip model files (slots 0-4): charId <= 8 registers the model
    // directly via func_80062AD8; larger ids go through the async file load.
    for (u32 i = 1; i <= 5; i++) {
        u32 modelId = ((CActParamHolderTail*)holder)->equipPtrs[i];
        if (modelId == 0) continue;
        ml::FixStr<64> buf(true);
        if (func_800AA33C(buf, modelId, 1, 1) == 0) continue;
        FileSlot* fs = &self->fileSlots[i - 1];
        if (fs->data != 0) continue;
        allLoaded = 0;
        if (func_80062A00() == 0) continue;
        if (charId > 8) {
            if (fs->handle != 0) continue;
            int size = getFileSize__11CDeviceFileFPCc(buf.mString, 1);
            u32 alloc = func_801F9894(&ctx, (u32)size);
            CFileHandle* fh = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(alloc, buf.mString, self, 0, 0);
            fs->handle = fh;
            setHandleFlag1__11CDeviceFileFP11CFileHandle(fh);
        } else {
            fs->flag = 1;
            fs->data = (u8*)func_80062AD8(((CActParamHolderTail*)holder)->equipPtrs[i], &outType);
            if (outType == 0xFFFFFFFF) {
                func_8007E038__Q22cf13CfGameManagerFv(charId, 0);
            }
        }
    }

    if (func_80200C20(self, charId) == 0) allLoaded = 0;
    if (allLoaded == 0) return;

    // ---- build the display model ----
    CModelDispModelVt* model = func_80495E8C(self->somePtr, (u32)self->fileSlots[0].data, -1, 1);
    holder->field_0x00 = model;
    model->m64(0);
    func_80485684(holder->field_0x00, 1);
    func_80482DF4(holder->field_0x00, 1);
    if (holder->field_0x00 == 0) return;
    func_804831C4(reinterpret_cast<CModelDispModelVt*>(holder->field_0x00),
                  reinterpret_cast<CModelDispNameParam*>(self->fileSlots[1].data));
    func_804831C4(reinterpret_cast<CModelDispModelVt*>(holder->field_0x00),
                  reinterpret_cast<CModelDispNameParam*>(self->fileSlots[2].data));
    func_804831C4(reinterpret_cast<CModelDispModelVt*>(holder->field_0x00),
                  reinterpret_cast<CModelDispNameParam*>(self->fileSlots[3].data));
    func_804831C4(reinterpret_cast<CModelDispModelVt*>(holder->field_0x00),
                  reinterpret_cast<CModelDispNameParam*>(self->fileSlots[4].data));

    // Pick the anim-model set for the weapon type (2 = off-hand only,
    // 3 = both, 8 = both, anything else = main-hand only).
    u8* f5 = self->fileSlots[5].data;
    u8 wtype = func_8013600C(&lbl_eu_80507FF8[0x2F], &lbl_eu_80507FF8[0x58], rec->weaponId);
    if (wtype == 8) {
        holder->animModelPtrs[0] = func_80495E94(self->somePtr, reinterpret_cast<CModelDispNameParam*>(f5));
        holder->animModelPtrs[1] = func_80495E94(self->somePtr, reinterpret_cast<CModelDispNameParam*>(f5));
    } else {
        wtype = func_8013600C(&lbl_eu_80507FF8[0x2F], &lbl_eu_80507FF8[0x62], rec->weaponId);
        if (wtype == 3) {
            holder->animModelPtrs[0] = func_80495E94(self->somePtr, reinterpret_cast<CModelDispNameParam*>(f5));
            holder->animModelPtrs[1] = func_80495E94(self->somePtr, reinterpret_cast<CModelDispNameParam*>(f5));
        } else if (wtype == 2) {
            holder->animModelPtrs[0] = func_80495E94(self->somePtr, reinterpret_cast<CModelDispNameParam*>(f5));
        } else {
            holder->animModelPtrs[1] = func_80495E94(self->somePtr, reinterpret_cast<CModelDispNameParam*>(f5));
        }
    }

    // Name the anim models and attach them to the display model.
    if (holder->animModelPtrs[0] != 0) {
        u8 sub = (u8)func_8014235C(rec->weaponId, &lbl_eu_80507FF8[0x58], 0);
        if (sub != 0) {
            reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->mC4(
                reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[0]), func_800BBC08(sub), 0);
        }
    }
    if (holder->animModelPtrs[1] != 0) {
        u8 sub = (u8)func_8014235C(rec->weaponId, &lbl_eu_80507FF8[0x58], 1);
        if (sub != 0) {
            reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->mC4(
                reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[1]), func_800BBC08(sub), 0);
        }
    }

    u32 f7 = (u32)self->fileSlots[7].data;
    u32 f6 = (u32)self->fileSlots[6].data;
    holder->field_0x04 = func_800584B8(self->somePtr, f7, &lbl_eu_80507FF8[0]);
    holder->field_0x08 = func_800584B8(self->somePtr, f6, &lbl_eu_80507FF8[4]);
    reinterpret_cast<CModelDispVtE0*>(&holder->actParam)->m38();
    func_8004B624(&holder->actParam, reinterpret_cast<CModelDispModelVt*>(holder->field_0x00),
                  holder->field_0x08, f6);
    func_8004B6A4(&holder->actParam, holder->field_0x04, f7);
    holder->actParam.field_0x0C &= ~0x800000;
    func_8004B354(&holder->actParam,
                  (const f32*)func_8004B60C(tmpB, lbl_eu_8066827C, lbl_eu_80668274, lbl_eu_80668274));
    holder->actParam.field_0x0C |= 0x160;
    func_80200388(&holder->actParam,
                  self != 0 ? reinterpret_cast<u32>(reinterpret_cast<u8*>(self) + 4) : 0);
    func_8004B60C(tmpA, lbl_eu_8066827C, lbl_eu_80668274, lbl_eu_80668274);
    self->colorG = tmpA[1];
    self->colorR = tmpA[0];
    self->colorB = tmpA[2];
    self->colorA = lbl_eu_80668274;

    u32 f8 = (u32)self->fileSlots[8].data;
    __ct__CMcaFile(&mca, f8);
    holder->unk_55C = func_80495EAC(self->somePtr, mca.mDataAdj, &lbl_eu_80507FF8[8]);

    // Wire both anim-model slots into the act-param array and start their anims.
    for (u8 i = 0; i < 2; i++) {
        CModelDispModelVt* am = reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[i]);
        if (am != 0 && holder->unk_55C != 0) {
            holder->actParams[i].field_0x378 = i;
            func_8005A594(&holder->actParams[i]);
            func_8004B624(&holder->actParams[i], am, holder->unk_55C, f8);
            func_8004B9D4(&holder->actParams[i], func_8004C5EC(&holder->actParam), 0, -1, 0);
        }
    }

    // Character 8 gets a special greeting at high progress.
    if (charId == 8) {
        if (func_800822F4__Q22cf13CfGameManagerFv() >= 0x167) {
            reinterpret_cast<CModelDispVt28*>(holder->field_0x00)->m28(&lbl_eu_80507FF8[0xC], 0);
        }
    }

    holder->timer = 0x96;
    reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->m48(self->alpha);
    reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->m9C(3, 0);
    self->state21 = 1;
    func_801FF96C(self);
}

// ============================================================
// Target: us-80202914 | func_80200C20
// ============================================================
// Load the equip model arc: build the packed path token from `param` and
// the manager handle, size the file, pick an alloc handle for the buffer
// (func_801F9894), and kick the async read into modelFileHandle.
extern "C" int func_80200C20(CModelDispEquip* self, u32 param) {
    CModelDispFileCtx ctx;
    func_801F981C(&ctx);
    if (self->modelData != 0) return 1;
    if (self->modelFileHandle != 0) return 0;
    void* h = func_8007DE94__Q22cf13CfGameManagerFv(param, 5);
    ml::FixStr<64> buf(true);
    u32 packed = (param << 20) | 0x78000000 | (((u32)h >> 10) & 0x1FC00);
    func_800AA33C(buf, packed, 1, 1);
    int size = getFileSize__11CDeviceFileFPCc(buf.mString, 1);
    if (size < 0) return 0;
    u32 alloc = func_801F9894(&ctx, (u32)size);
    CFileHandle* fh = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(alloc, buf.mString, self, 0, 0);
    self->modelFileHandle = fh;
    setHandleFlag1__11CDeviceFileFP11CFileHandle(fh);
    return 0;
}

// ============================================================
// Target: us-802029e0 | func_80200CE8
// ============================================================
// Per-frame tick for the equip display: while the holder is live, a countdown
// timer periodically rolls a random sign that picks one of three anim ids; for
// the two ids with matching anim state the animation is replayed. Then the
// act-param object and both occupied animation-model slots are ticked.
void func_80200CE8(CModelDispEquip* self) {
    int i;
    CActParamHolder* holder = &self->actParamHolder;
    if (self->actParamHolder.field_0x00 == 0)
        return;
    if (holder->timer <= 0) {
        getInstance__Q22ml6MTRandFv();
        u32 r = rand31__Q22ml6MTRandFv();
        s32 sign = (s32)(((r & 1) ^ (r >> 31)) - (r >> 31));
        u32 anim = lbl_eu_80662738[sign];
        if ((anim == 0x12 && holder->actParam.field_0x374 == 0) ||
            (anim == 0x7 && holder->actParam.field_0x374 == 0xb)) {
            func_8004B9D4(&holder->actParam, reinterpret_cast<void*>(anim), 0, -1, 0);
        }
        holder->timer = 0x96;
    }
    holder->timer--;
    func_8004CF00(&holder->actParam);
    for (i = 0; i < 2; i++) {
        if (holder->animModelPtrs[i] != 0)
            func_8004CF00(&holder->actParams[i]);
    }
}

// ============================================================
// Target: us-80202ad8 | OnFileEvent__15CModelDispEquipFP10CEventFile
// ============================================================
// CDeviceFile async-load callback: find the slot whose handle matches the
// event's file handle, hand the loaded buffer to the slot, and clear the
// handle. The 9-slot scan falls back to the dedicated model slot (0x1090),
// whose buffer becomes modelData (only for event type 1).
int CModelDispEquip::OnFileEvent(CEventFile* event) {
    u8* d;
    for (u8 i = 0; i < 9; i++) {
        FileSlot* slot = &fileSlots[i];
        if (slot->handle != event->field_04)
            continue;
        d = slot->handle->mData;
        slot->handle->mData = 0;
        slot->data = d;
        slot->handle = 0;
        return 1;
    }
    if (modelFileHandle == event->field_04) {
        if (event->field_00 == 1) {
            d = modelFileHandle->mData;
            modelFileHandle->mData = 0;
            modelData = d;
            func_804CC1BC(lbl_eu_8065FC18);
        }
        modelFileHandle = 0;
    }
    return 0;
}

// ============================================================
// Target: us-80202b8c | func_80200E94
// ============================================================
// Per-slot model hooks: if the animation model slot is occupied, stop the old
// model via vtable+0xC8 then re-arm it via vtable+0xC4 with (model, arg, 0).
extern "C" void func_80200E94(CModelDispEquip* self, void* arg, int index) {
    CActParamHolder* holder = &self->actParamHolder;
    if (holder->animModelPtrs[index] != 0) {
        reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->mC8(
            reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[index]));
        reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->mC4(
            reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[index]),
            reinterpret_cast<CModelDispNameParam*>(arg), 0);
    }
}

extern "C" void func_80200F08(CModelDispEquip* self, void* move, void* arg, int index) {
    if (move == 0) return;
    if (func_800BBC04(arg) <= 0) return;
    CActParamHolder* holder = &self->actParamHolder;
    if (holder->animModelPtrs[index] == 0) return;
    // Stop + re-arm the animation-model slot (same shape as func_80200E94),
    // guarded on the move pointer and the name lookup.
    reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->mC8(
        reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[index]));
    reinterpret_cast<CModelDispModelVt*>(holder->field_0x00)->mC4(
        reinterpret_cast<CModelDispModelVt*>(holder->animModelPtrs[index]),
        reinterpret_cast<CModelDispNameParam*>(arg), 0);
}

// ============================================================
// Target: us-80202ca8 | func_80200FB0
// ============================================================
// Stop the previous equipment model and hand the picked name record to
// func_80200F08: `kind` 0 uses the anim slot (func_800BEDC4), `kind` 1 the
// equipment slot (func_800BED80); `subKind` picks the slot index.
extern "C" void func_80200FB0(CModelDispEquip* self, CModelDispParent* parent,
                              int kind, int subKind) {
    CActParamHolder* holder = &self->actParamHolder;
    if (holder->field_0x00 != parent->field_0x3A0) return;
    // Pointer-walk loads trigger the retail lwzu base fold (MWCC_CASES
    // btm_sco_init note): loads grouped before the stores.
    u32* srcp = lbl_eu_80507FDC;
    u32 v0 = *srcp++;
    u32 v1 = *srcp++;
    u32 v2 = *srcp++;
    u32 src[3];
    src[0] = v0;
    src[1] = v1;
    src[2] = v2;
    CModelDispListHolder lh;
    func_80043D90(&lh);
    CModelDispEnumList* list = func_80043F18(&lh);
    func_800F4A98(list, src[self->equipSlot], 0);
    list = func_80043F18(&lh);
    if (list->field_620 >= 1) {
        CModelDispSlot* slot = func_800F6EC0(list, 0);
        cf::CfObjectMove* cfMove = slot->field_04;
        if (cfMove != 0) {
            CModelDispActor* actor = func_800BFC68(cfMove);
            int idx = 0;
            CModelDispNameParam* res = 0;
            if (kind == 0) {
                if (subKind == 0) {
                    idx = 0;
                    res = func_800BEDC4(reinterpret_cast<CModelDispMoveVt*>(&actor->move), 0);
                } else if (subKind == 1) {
                    idx = 1;
                    res = func_800BEDC4(reinterpret_cast<CModelDispMoveVt*>(&actor->move), 1);
                }
                if (res != 0) func_80200F08(self, actor, res, idx);
            } else if (kind == 1) {
                if (subKind == 0) {
                    idx = 0;
                    res = func_800BED80(reinterpret_cast<CModelDispMoveVt*>(&actor->move), 0);
                } else if (subKind == 1) {
                    idx = 1;
                    res = func_800BED80(reinterpret_cast<CModelDispMoveVt*>(&actor->move), 1);
                }
                if (res != 0) func_80200F08(self, actor, res, idx);
            }
        }
    }
    __dt__80043E88(&lh, -1);
}

// ============================================================
// Target: us-80202e40 | func_80201148
// ============================================================
// Swap the equipment display model for `kind` (0/1) and `subKind` (0/1):
// look up the weapon's name record via the Bdat column (string table at
// lbl_eu_80507FF8 + 0x58/0x62) and hand it to func_80200E94.
extern "C" void func_80201148(CModelDispEquip* self, CModelDispNameParam* unused,
                              int kind, int subKind) {
    getPlayer__Q22cf13CfGameManagerFi(0);
    int index = 0;
    CModelDispNameParam* arg = 0;
    switch (kind) {
    case 0:
        if (subKind == 0) {
            index = 0;
            arg = func_800BBC08(func_8014235C(self->weaponId, &lbl_eu_80507FF8[0x62], 0));
        } else if (subKind == 1) {
            index = 1;
            arg = func_800BBC08(func_8014235C(self->weaponId, &lbl_eu_80507FF8[0x62], 1));
        }
        if (arg != 0) func_80200E94(self, arg, index);
        break;
    case 1:
        if (subKind == 0) {
            index = 0;
            arg = func_800BBC08(func_8014235C(self->weaponId, &lbl_eu_80507FF8[0x58], 0));
        } else if (subKind == 1) {
            index = 1;
            arg = func_800BBC08(func_8014235C(self->weaponId, &lbl_eu_80507FF8[0x58], 1));
        }
        if (arg != 0) func_80200E94(self, arg, index);
        break;
    }
}

// ============================================================
// Target: us-80201aa0 | func_801FFDB0
// ============================================================
extern "C" void func_801FFDB0(CModelDispEquip* self) {
    if (self->equipSlot < 3)
        ((void(*)(CModelDispEquip*))func_801FFDC4)(self);
    else
        ((void(*)(CModelDispEquip*))func_80200394)(self);
}

// ============================================================
// Target: us-80202c94 | func_80200F9C
// ============================================================
extern "C" void func_80200F9C(CModelDispEquip* self) {
    if (self->equipSlot < 3)
        ((void(*)(CModelDispEquip*))func_80200FB0)(self);
    else
        ((void(*)(CModelDispEquip*))func_80201148)(self);
}

// ============================================================
// Target: us-802017b8 | func_801FFAC8
// ============================================================
extern "C" void func_801FFAC8(CModelDispEquip* self) {
    if (self->state21 == 0)
        ((u32(*)(CModelDispEquip*))func_801FFDB0)(self);
    else
        ((void(*)(CModelDispEquip*))func_80200CE8)(self);
}

// ============================================================
// Target: us-80202f90 | func_80201298
// ============================================================
// For each occupied animation-model slot: pull the next chain object off the
// act-param, cache it at actParams[i]+0x4B4, and replay animation `arg`.
extern "C" void func_80201298(CModelDispEquip* self, u32 unused, void* arg) {
    CActParamHolder* holder = &self->actParamHolder;
    for (u8 i = 0; i < 2; i++) {
        if (holder->animModelPtrs[i] != 0) {
            void* p = func_8004B9B8(&holder->actParam);
            holder->actParams[i].field_0x4B4 = p;
            func_8004B9D4(&holder->actParams[i], arg, 0, -1, 0);
        }
    }
}

// ============================================================
// Target: us-80203010 | func_80201318 (empty)
// ============================================================
extern "C" void func_80201318() {}

// ============================================================
// Target: us-80203014 | func_8020131C
// ============================================================
// Handle a model-slot command: cmd 2 loads the anim model into the first free
// slot (creating an effect from the model arc, wiring it via the second-base
// adjusted-this call, and caching the vtable+0xA8 chain object); cmd 3 marks
// every occupied slot's effect active (byte +0x59).
void func_8020131C(CModelDispEquip* self, CModelDispEquipCmd* cmd) {
    CActParamHolder* holder = &self->actParamHolder;
    if (cmd->field_0x0A == 2) {
        int val = 0xb;
        u8 c = cmd->field_0x1C;
        if ((u8)(c - '0') <= 9)
            val = atoi(&cmd->field_0x1C);
        for (u8 i = 0; i < 2; i++) {
            if (holder->animModelPtrs[i] != 0 &&
                ((CActParamHolderTail*)holder)->animPtrs[i] == 0) {
                CModelDispEffectView* effect =
                    func_804CC1F4(lbl_eu_8065FC18, self->modelData, self->somePtr,
                                  val - 1, 1, 0);
                ((CActParamHolderTail*)holder)->animPtrs[i] = effect;
                if (effect != 0) {
                    func_804E3D0C(self != 0
                                      ? reinterpret_cast<CModelDispEffectView*>(
                                            reinterpret_cast<u8*>(self) + 8)
                                      : 0,
                                  effect);
                    void* chain =
                        reinterpret_cast<CModelDispVtA8*>(holder->animModelPtrs[i])->m2A();
                    effect->field_0x14 = reinterpret_cast<u32>(chain);
                }
            }
        }
    } else if (cmd->field_0x0A == 3) {
        for (u8 i = 0; i < 2; i++) {
            CModelDispEffectView* effect =
                (CModelDispEffectView*)((CActParamHolderTail*)holder)->animPtrs[i];
            if (effect != 0)
                effect->field_0x59 = 1;
        }
    }
}

// ============================================================
// Target: us-80203138 | func_80201440 (empty)
// ============================================================
extern "C" void func_80201440() {}

// ============================================================
// Target: us-8020313c | func_80201444
// ============================================================
extern "C" void func_80201444(CModelDispEquip* self, u32 val) {
    // Holder-base addressing (addi r6,r3,16 + 0xff4(rX) like retail) plus
    // scoped optimize_for_size to keep the 2-iteration loop rolled as the
    // mtlr/bdnz countdown (plain -O4,p unrolls it).
    CActParamHolder* holder = &self->actParamHolder;
    CActParamHolderTail* tail = (CActParamHolderTail*)holder;
#pragma push
#pragma optimize_for_size on
    for (u8 i = 0; i < 2; i++) {
        if (tail->animPtrs[i] == (void*)val) {
            tail->animPtrs[i] = 0;
        }
    }
#pragma pop
}

// ============================================================
// Target: us-80203174 | func_8020147C
// ============================================================
extern "C" void func_8020147C(CModelDispEquip* self, s32 val) {
    // Calls through a cast pointer to keep MWCC from inlining the empty stub
    ((void(*)(CModelDispEquip*, s32))func_801FF874)(self, val);
    if (val >= 0)
        self->equipSlot = val;
    self->state21 = 0;
}

// ============================================================
// Target: us-802031b8 | func_802014C0
// ============================================================
extern "C" void func_802014C0(CModelDispEquip* self) {
    ((void (*)(CModelDispEquip*))func_801FF874)(self);
    self->equipSlot += 1;
    if (self->equipSlot >= code80135FDC_getByte_64077())
        self->equipSlot = 0;
    self->state21 = 0;
}

// ============================================================
// Target: us-80203214 | func_8020151C
// ============================================================
extern "C" void func_8020151C(CModelDispEquip* self) {
    ((void (*)(CModelDispEquip*))func_801FF874)(self);
    if (--self->equipSlot < 0)
        self->equipSlot = code80135FDC_getByte_64077() - 1;
    self->state21 = 0;
}

// ============================================================
// Target: us-80203268 | func_80201570
// ============================================================
// Fade alpha up while the act-param sub-object is live; bit 0x20 of the
// act-param flags guards the func_8004B52C call.
extern "C" void func_80201570(CModelDispEquip* self) {
    if (self->actParamHolder.field_0x00 != 0) {
        self->colorA += lbl_eu_80668280;
        self->actParamHolder.actParam.field_0x0C &= ~0x20;
        func_8004B52C(&self->actParamHolder.actParam, self->colorA);
        self->actParamHolder.actParam.field_0x0C |= 0x20;
    }
}

// ============================================================
// Target: us-802032cc | func_802015D4
// ============================================================
// Fade alpha down (mirror of func_80201570).
extern "C" void func_802015D4(CModelDispEquip* self) {
    if (self->actParamHolder.field_0x00 != 0) {
        self->colorA -= lbl_eu_80668280;
        self->actParamHolder.actParam.field_0x0C &= ~0x20;
        func_8004B52C(&self->actParamHolder.actParam, self->colorA);
        self->actParamHolder.actParam.field_0x0C |= 0x20;
    }
}

// ============================================================
// Target: us-80203330 | func_80201638
// ============================================================
// Ramp scale1[1]/scale2[1] up by the step, clamped at the upper bounds.
extern "C" void func_80201638(CModelDispEquip* self) {
    self->scale1[1] += lbl_eu_80668280;
    if (self->scale1[1] > lbl_eu_80668284)
        self->scale1[1] = lbl_eu_80668284;
    self->scale2[1] += lbl_eu_80668280;
    if (self->scale2[1] > lbl_eu_80668288)
        self->scale2[1] = lbl_eu_80668288;
    func_8049EFF8(func_80496264(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// Target: us-802033b4 | func_802016BC
// ============================================================
// Ramp scale1[1]/scale2[1] down by the step, clamped at the lower bounds.
extern "C" void func_802016BC(CModelDispEquip* self) {
    self->scale1[1] -= lbl_eu_80668280;
    if (self->scale1[1] < lbl_eu_8066828C)
        self->scale1[1] = lbl_eu_8066828C;
    self->scale2[1] -= lbl_eu_80668280;
    if (self->scale2[1] < lbl_eu_80668290)
        self->scale2[1] = lbl_eu_80668290;
    func_8049EFF8(func_80496264(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// Target: us-80203438 | func_80201740
// ============================================================
// Decrement scale1[2] toward lbl_eu_80668294, then sync pose + scales.
extern "C" void func_80201740(CModelDispEquip* self) {
    self->scale1[2] -= lbl_eu_80668280;
    if (self->scale1[2] < lbl_eu_80668294)
        self->scale1[2] = lbl_eu_80668294;
    func_8049EFF8(func_80496264(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// Target: us-8020349c | func_802017A4
// ============================================================
// Increment scale1[2] toward lbl_eu_80668298 (mirror of func_80201740).
extern "C" void func_802017A4(CModelDispEquip* self) {
    self->scale1[2] += lbl_eu_80668280;
    if (self->scale1[2] > lbl_eu_80668298)
        self->scale1[2] = lbl_eu_80668298;
    func_8049EFF8(func_80496264(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// Target: us-80203500 | func_80201808
// ============================================================
// Re-arm the display: while the holder is live, rebuild the color quad from
// the (step, 1, 1) vector, poke the act-param object with colorA, and push
// the new colors through func_8004B354. Then snap scale1/scale2 to the
// sinit-built globals and sync pose + scales (same tail as func_801FF7B0).
void func_80201808(CModelDispEquip* self) {
    if (self->actParamHolder.field_0x00 != 0) {
        struct V3 {
            u32 w[3];
        };
        u32 tmp[4];
        func_8004B60C(tmp, lbl_eu_8066827C, lbl_eu_80668274, lbl_eu_80668274);
        *(V3*)&self->colorR = *(const V3*)tmp;
        self->colorA = lbl_eu_80668274;
        self->actParamHolder.actParam.field_0x0C &= ~0x20;
        func_8004B52C(&self->actParamHolder.actParam, self->colorA);
        self->actParamHolder.actParam.field_0x0C = (self->actParamHolder.actParam.field_0x0C | 0x20) & ~0x100;
        func_8004B354(&self->actParamHolder.actParam, &self->colorR);
        self->actParamHolder.actParam.field_0x0C |= 0x100;
    }
    struct V3 {
        u32 w[3];
    };
    *(V3*)self->scale1 = *(const V3*)lbl_eu_80576550;
    *(V3*)self->scale2 = *(const V3*)lbl_eu_8057655C;
    func_8049EFF8(func_80496264(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// Target: us-802035f8 | func_80201900
// ============================================================
// Same as func_80201808 but the color quad is built from (1, 1, 1) - used to
// reset the display colors to neutral white.
void func_80201900(CModelDispEquip* self) {
    if (self->actParamHolder.field_0x00 != 0) {
        struct V3 {
            u32 w[3];
        };
        u32 tmp[4];
        func_8004B60C(tmp, lbl_eu_80668274, lbl_eu_80668274, lbl_eu_80668274);
        *(V3*)&self->colorR = *(const V3*)tmp;
        self->colorA = lbl_eu_80668274;
        self->actParamHolder.actParam.field_0x0C &= ~0x20;
        func_8004B52C(&self->actParamHolder.actParam, self->colorA);
        self->actParamHolder.actParam.field_0x0C = (self->actParamHolder.actParam.field_0x0C | 0x20) & ~0x100;
        func_8004B354(&self->actParamHolder.actParam, &self->colorR);
        self->actParamHolder.actParam.field_0x0C |= 0x100;
    }
    struct V3 {
        u32 w[3];
    };
    *(V3*)self->scale1 = *(const V3*)lbl_eu_80576550;
    *(V3*)self->scale2 = *(const V3*)lbl_eu_8057655C;
    func_8049EFF8(func_80496264(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// Target: us-802036f0 | sinit_802019F8
// ============================================================
extern "C" void sinit_802019F8() {
    func_8004B60C(lbl_eu_80576550, lbl_eu_80668274, lbl_eu_8066829C, lbl_eu_80668278);
    func_8004B60C(lbl_eu_8057655C, lbl_eu_80668274, lbl_eu_80668270, lbl_eu_80668274);
}

// ============================================================
// Member function implementations (for thunks / virtual dispatch)
// ============================================================
void CModelDispEquip::resetBase() {
    *(unsigned long*)this = 0;
    *(unsigned long*)((char*)this + 4) = 0;
    ((unsigned char*)this)[8] = 0;
}

void CModelDispEquip::func_80201298() {}
void CModelDispEquip::vfunc18() {}
void CModelDispEquip::func_8020131C() {}
void CModelDispEquip::vfunc40() {}

void CModelDispEquip::thunk4_1298() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->func_80201298();
}

void CModelDispEquip::thunk4_1440() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->vfunc40();
}

void CModelDispEquip::thunk4_131C() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->func_8020131C();
}

void CModelDispEquip::thunk4_1318() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->vfunc18();
}

void CModelDispEquip::thunk4_F9C() {
    func_80200F9C(reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4));
}

void CModelDispEquip::thunk4_dtor() {
    ((void(*)(void*))__dt__15CModelDispEquipFv)((char*)this - 4);
}

void CModelDispEquip::thunk8_1444() {
    func_80201444(reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 8), 0);
}

void CModelDispEquip::thunk8_dtor() {
    ((void(*)(void*))__dt__15CModelDispEquipFv)((char*)this - 8);
}
// Retail: subi r3,r3,-4; b func_80201298 (extern "C" free thunks, unmangled)
extern "C" void func_80201A44(void* self) { ((void(*)(void*))func_80201298)((char*)self - 4); }
extern "C" void func_80201A54(void* self) { ((void(*)(void*))func_8020131C)((char*)self - 4); }
extern "C" void func_80201A64(void* self) { ((void(*)(void*))func_80200F9C)((char*)self - 4); }
// Retail: subi r3,r3,-8; b func_80201444
extern "C" void func_80201A74(void* self) { ((void(*)(void*))func_80201444)((char*)self - 8); }
