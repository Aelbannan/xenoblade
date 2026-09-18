// CModelDispEquip - Equipment display model
// High-level C/C++ reconstruction

#include "kyoshin/menu/parts/CModelDispEquip.hpp"

#include "kyoshin/code_80135FDC.hpp"

// Forward decls: state-dispatch callees defined later in this TU.
extern "C" void ModelDispEquip_BuildDisplay(CModelDispEquip*);
extern "C" void ModelDispEquip_StepFadeOut(CModelDispEquip*);
extern "C" void ModelDispEquip_StepIdleBuildOrTick(CModelDispEquip*);
extern "C" void ModelDispEquip_StepFadeIn(CModelDispEquip*);
extern "C" void ModelDispEquip_StartFadeOut(CModelDispEquip*);
extern "C" __declspec(noinline) void ModelDispEquip_ClearFileSlot(u8* ptr);
extern "C" int ModelDispEquip_LoadModelArc(CModelDispEquip* self, u32 param);
extern "C" void ModelDispEquip_SetActParamParent(void* self, u32 val);
extern "C" void ModelDispEquip_TeardownHolder(CModelDispEquip*, CActParamHolder*);
extern "C" __declspec(noinline) void ModelDispEquip_StoreFloat4(float* dest, float a, float b, float c, float d);

// ============================================================
// us-80201254 | __ct__CModelDispEquip (constructor)
// ============================================================
CModelDispEquip::CModelDispEquip(u32 somePtr, s32 equipSlot) {
    // No self alias: retail keeps only self(r31), holder and the equipSlot
    // param in the saved pool (r31/r29/r28) - an extra live value shifts
    // _savegpr_27 vs _savegpr_28.
    this->somePtr = somePtr;
    _vtable = (u32)lbl_eu_805354C8;
    // Early holder alias: retail materializes self+0x10 once here (r29); it
    // dies after __construct_array so the file-slot walk can reuse the reg.
    CActParamHolder* holder = &this->actParamHolder;
    _vtable2 = (u32)lbl_eu_805354C8 + 0x88;
    _vtable3 = (u32)lbl_eu_805354C8 + 0xB4;
    __ct__Q22cf17CActParamAnimGameFv(reinterpret_cast<cf::CActParamAnimGame*>(&holder->actParam));
    __construct_array(&holder->actParams[0], (void*)__ct__Q22cf17CActParamAnimGameFv,
                      (void*)__dt__Q22cf17CActParamAnimGameFv, 0x53C, 2);
    weaponId = 0;
    this->equipSlot = equipSlot;
    state = 0;
    alpha = lbl_eu_80668270;
    alphaTimer = lbl_eu_80668274;
    state20 = 1;
    state21 = 1;
    FileSlot* slot = &this->fileSlots[0];
    do {
        ModelDispEquip_ClearFileSlot(reinterpret_cast<u8*>(slot));
        slot++;
    } while (slot < &this->fileSlots[9]);
    modelFileHandle = 0;
    modelData = 0;
    // Scalar clears stay this-> based (retail stores via self+offset); the
    // array clears below re-derive the holder base for their indexed walks.
    this->actParamHolder.field_0x00 = 0;
    this->actParamHolder.field_0x04 = 0;
    this->actParamHolder.field_0x08 = 0;
    this->actParamHolder.unk_55C = 0;
    this->actParamHolder.timer = 0;
    this->actParamHolder.currentModelPtr = 0;
    CActParamHolder* tail = &this->actParamHolder;
    // Single up-counting index doubles as the address scale (retail
    // clrlslwi r0,r6,24,2) with a rolled mtctr/bdnz loop body.
    for (u8 i = 0; i < 2; i++) {
        tail->animModelPtrs[i] = 0;
        ((CActParamHolderTail*)tail)->animPtrs[i] = 0;
    }
    for (u8 i = 0; i < 8; i++) {
        ((CActParamHolderTail*)tail)->equipPtrs[i] = 0;
    }
    // Retail re-inits a stack temp per slot and copies it field-by-field
    // (lwz/stw x2 + lbz/stb), not as one struct assignment.
    FileSlot tmp;
    for (u8 j = 0; j < 9; j++) {
        ModelDispEquip_ClearFileSlot(reinterpret_cast<u8*>(&tmp));
        this->fileSlots[j].handle = tmp.handle;
        this->fileSlots[j].data = tmp.data;
        this->fileSlots[j].flag = tmp.flag;
    }
}

// ============================================================
// us-802013cc | ModelDispEquip_ClearFileSlot
// ============================================================
extern "C" __declspec(noinline) void ModelDispEquip_ClearFileSlot(u8* ptr) {
    *(u32*)ptr = 0;
    *(u32*)(ptr + 4) = 0;
    *(u8*)(ptr + 8) = 0;
}

// ============================================================
// us-802013e0 | __dt__801FF6F0
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
// us-8020144c | __dt__15CModelDispEquipFv
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
// us-802014a0 | ModelDispEquip_SyncScalePose
// ============================================================
// Snap scale1/scale2 to the sinit-built global vectors (bit-copied as words,
// retail lwzu/lwz -> stw), then sync pose+scales. Pointer-walk locals trigger
// MWCC's lwzu base fold (MWCC_CASES btm_sco_init note).
extern "C" void ModelDispEquip_SyncScalePose(CModelDispEquip* self) {
    struct V3 {
        u32 w[3];
    };
    *(V3*)self->scale1 = *(const V3*)lbl_eu_80576550;
    *(V3*)self->scale2 = *(const V3*)lbl_eu_8057655C;
    func_8049EFF8(Scn_FindCamItem(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// us-8020151c | ModelDispEquip_StepStateDispatch
// ============================================================
extern "C" void ModelDispEquip_StepStateDispatch(CModelDispEquip* self) {
    switch (self->state) {
    case 0:
        if (self->state21 != 0)
            return;
        ((void (*)(CModelDispEquip*))ModelDispEquip_BuildDisplay)(self);
        break;
    case 1:
        ModelDispEquip_StepFadeOut(self);
        break;
    case 2:
        ((void (*)(CModelDispEquip*))ModelDispEquip_StepIdleBuildOrTick)(self);
        break;
    case 3:
        ModelDispEquip_StepFadeIn(self);
        break;
    }
}

// ============================================================
// us-80201564 | ModelDispEquip_ResetDisplay
// ============================================================
// Reset the equip display: drain the 9 slot + model file jobs, free loaded
// slot buffers and the model arc buffer, then mark the state-21 flag.
extern "C" void ModelDispEquip_ResetDisplay(CModelDispEquip* self) {
    waitForDrawDone__9CDeviceVIFv();
    for (u8 i = 0; i < 9; i++) {
        closeFileHandle__FPP11CFileHandle(&self->fileSlots[i].handle);
    }
    ModelDispEquip_TeardownHolder(self, &self->actParamHolder);
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
    closeFileHandle__FPP11CFileHandle(&self->modelFileHandle);
    u8* md = self->modelData;
    if (md != NULL) {
        EffSched_LookupB(lbl_eu_8065FC18, md);
        u8* md2 = self->modelData;
        if (md2 != NULL) {
            mtl::MemManager::deallocate(md2);
            self->modelData = NULL;
        }
    }
    self->state21 = 1;
}

// ============================================================
// us-8020164c | ModelDispEquip_GetState20 (getState20)
// ============================================================
extern "C" u8 ModelDispEquip_GetState20(CModelDispEquip* self) {
    return self->state20;
}

// ============================================================
// us-80201654 | ModelDispEquip_GetState21 (getState21)
// ============================================================
extern "C" u8 ModelDispEquip_GetState21(CModelDispEquip* self) {
    return self->state21;
}

// ============================================================
// us-8020165c | ModelDispEquip_StartFadeOut (setState14)
// ============================================================
extern "C" void ModelDispEquip_StartFadeOut(CModelDispEquip* self) {
    if (self->state != 0) return;
    self->state = 1;
    self->state20 = 0;
}

// ============================================================
// us-8020167c | ModelDispEquip_StartFadeIn (setState14_3)
// ============================================================
extern "C" void ModelDispEquip_StartFadeIn(CModelDispEquip* self) {
    self->state = 3;
    self->state20 = 0;
}

// ============================================================
// us-80201690 | ModelDispEquip_GetEquipSlot
// ============================================================
extern "C" u32 ModelDispEquip_GetEquipSlot(CModelDispEquip* self) {
    return (u32)(u8)self->equipSlot;
}

// ============================================================
// us-8020169c | ModelDispEquip_StepFadeOut
// ============================================================
// Fade the display out (state 1 step): advance the alpha timer and, once it
// passes the threshold, step alpha down toward 1.0 (clamped, with the state
// flipped to 2), push the new alpha into the act-param object, and write the
// quad color (1, 1, 1, 1-alpha) into both animation-model slots.
void ModelDispEquip_StepFadeOut(CModelDispEquip* self) {
    self->alphaTimer += lbl_eu_80668270;
    if (self->alphaTimer >= lbl_eu_80668278) {
        self->alpha -= lbl_eu_8066827C;
        if (self->alpha < lbl_eu_80668274) {
            self->alpha = lbl_eu_80668274;
            self->state = 2;
            self->state20 = 1;
        }
        CActParamHolder* holder = &self->actParamHolder;
        CScnItemModel* obj = reinterpret_cast<CScnItemModel*>(holder->field_0x00);
        if (obj != 0) {
            obj->vfunc48(self->alpha);
        }
        struct V4 {
            u32 w[4];
        };
        u32 tmp[4];
        ModelDispEquip_StoreFloat4(reinterpret_cast<float*>(tmp), lbl_eu_80668270, lbl_eu_80668270,
                      lbl_eu_80668270, lbl_eu_80668270 - self->alpha);
        // Word-copy gives retail's paired load/store schedule.
        // p declared before the loop index so MWCC's low-to-high scratch
        // coloring lands it in r4 with the index in r5 (retail allocation).
        CModelDispAnimColor* p;
        for (u8 i = 0; i < 2; i++) {
            p = (CModelDispAnimColor*)((CActParamHolderTail*)holder)->animPtrs[i];
            if (p != 0)
                *(V4*)&p->field_0x40 = *(const V4*)tmp;
        }
    }
}

// ============================================================
// us-802017a4 | ModelDispEquip_StoreFloat4 (storeFloats)
// ============================================================
extern "C" __declspec(noinline) void ModelDispEquip_StoreFloat4(float* dest, float a, float b, float c, float d) {
    dest[0] = a;
    dest[1] = b;
    dest[2] = c;
    dest[3] = d;
}

// ============================================================
// us-802017cc | ModelDispEquip_StepFadeIn
// ============================================================
// Fade the equip display in (state 3 step): bump alpha toward the clamp,
// poke the act-param object with the new alpha, then write the quad color
// (clamp, clamp, clamp, clamp - alpha) into both animation-model slots.
extern "C" __declspec(noinline) void ModelDispEquip_StepFadeIn(CModelDispEquip* self) {
    self->alpha += lbl_eu_8066827C;
    if (self->alpha > lbl_eu_80668270) {
        self->alpha = lbl_eu_80668270;
        self->state = 0;
        self->state20 = 1;
    }
    CActParamHolder* holder = &self->actParamHolder;
    CScnItemModel* obj = reinterpret_cast<CScnItemModel*>(holder->field_0x00);
    if (obj != 0) {
        obj->vfunc48(self->alpha);
    }
    u32 tmp[4];
    ModelDispEquip_StoreFloat4((float*)tmp, lbl_eu_80668270, lbl_eu_80668270, lbl_eu_80668270,
                  lbl_eu_80668270 - self->alpha);
    // Word-copy gives retail's paired load/store schedule.
    // p declared before the loop index for retail scratch coloring (r4/r5).
    struct V4 { u32 w[4]; };
    CModelDispAnimColor* p;
    for (u8 i = 0; i < 2; i++) {
        p = (CModelDispAnimColor*)((CActParamHolderTail*)holder)->animPtrs[i];
        if (p != 0)
            *(V4*)&p->field_0x40 = *(const V4*)tmp;
    }
}

// ============================================================
// us-802018b4 | ModelDispEquip_TeardownHolder
// ============================================================
// Tear down the equip display: stop the effect slots and anim-model slots,
// release the loaded models/records, and drain the file slots.
// Register-shape notes (vs retail): the loop indices are u8 (clrlslwi/clrlwi
// address math), the effect-parent arg is `self ? self+8 : self` (cmplwi/mr/
// beq/addi shape), and the anim-slot pointers are re-read from memory after
// the stop call instead of being cached.
extern "C" void ModelDispEquip_TeardownHolder(CModelDispEquip* self, CActParamHolder* holder) {
    waitForDrawDone__9CDeviceVIFv();
    if (holder->field_0x00 == 0)
        return;
    for (u8 i = 0; i < 2; i++) {
        CModelDispEffectView* e = reinterpret_cast<CModelDispEffectView*>(holder->animPtrs[i]);
        if (e != 0) {
            // Retail shape: cmplwi/mr-default/beq/addi - an if-update, not a
            // ternary (ternary reorders the mr past the branch).
            CModelDispEffectView* parent = reinterpret_cast<CModelDispEffectView*>(self);
            if (self != 0)
                parent = reinterpret_cast<CModelDispEffectView*>(reinterpret_cast<u8*>(self) + 8);
            schedDetachChildSlot(e, parent);
            schedClearFlag15Update(reinterpret_cast<CModelDispEffectView*>(holder->animPtrs[i]));
            holder->animPtrs[i] = 0;
        }
        CScnItemModel* m = reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[i]);
        if (m != 0) {
            if (self->equipSlot >= 3) {
                reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfuncC8(m);
                releaseAnimObj(&holder->actParams[i], holder->unk_55C);
            }
            reinterpret_cast<CActParamAnim*>(&holder->actParams[i])->func_8004B114();
        }
    }
    Scn_IsAnimActiveOrNull(holder->unk_55C);
    holder->unk_55C = 0;
    for (u8 i = 0; i < 2; i++) {
        CModelDispObj* m = reinterpret_cast<CModelDispObj*>(holder->animModelPtrs[i]);
        if (m != 0) {
            Scn_IsAnimActiveOrNull(m);
            holder->animModelPtrs[i] = 0;
        }
    }
    releaseAnimObj(&holder->actParam, holder->field_0x04);
    releaseAnimObj(&holder->actParam, holder->field_0x08);
    reinterpret_cast<CActParamAnim*>(&holder->actParam)->func_8004B114();
    Scn_IsAnimActiveOrNull(holder->field_0x04);
    Scn_IsAnimActiveOrNull(holder->field_0x08);
    Scn_IsAnimActiveOrNull(reinterpret_cast<CModelDispObj*>(holder->field_0x00));
    holder->field_0x00 = 0;
    // Direct member indexing: MWCC CSEs the &fileSlots[i] temp into the same
    // scratch color the earlier anim-slot walks used (retail r28).
    for (u8 i = 0; i < 9; i++) {
        if (self->fileSlots[i].flag != 0) {
            self->fileSlots[i].data = 0;
            self->fileSlots[i].flag = 0;
        } else if (self->fileSlots[i].data != 0) {
            mtl::MemManager::deallocate(self->fileSlots[i].data);
            self->fileSlots[i].data = 0;
        }
    }
    closeFileHandle__FPP11CFileHandle(&self->modelFileHandle);
    // Nested re-tests: each condition reloads modelData from memory.
    if (self->modelData != 0) {
        EffSched_LookupB(lbl_eu_8065FC18, self->modelData);
        u8* md = self->modelData;
        if (md != 0) {
            mtl::MemManager::deallocate(md);
            self->modelData = 0;
        }
    }
}

// ============================================================
// us-80201ab4 | ModelDispEquip_BuildPartyModel
// ============================================================
// Build the equipment display: select the enum list for the current equip
// slot, verify the first actor (model id, move state, file load), then create
// the display model, wire the param slots + anim models, and start the idle
// animation.
// Retail allocation: 0x60 frame saving r26-r31 - exactly six values live
// across calls (self/holder/actor/flag + two locals); keep helper temps
// scoped tightly and never hold more than six past a call boundary.
extern "C" void ModelDispEquip_BuildPartyModel(CModelDispEquip* self) {
    // Pointer-walk loads trigger the retail lwzu base fold (MWCC_CASES
    // btm_sco_init note); the scheduler hoists all three loads ahead of the
    // stores like retail.
    u32 src[3];
    CModelDispMca mca;
    f32 tmpB[3];
    f32 tmpA[3];
    CModelDispListHolder lh;
    u32* srcp = lbl_eu_80507FD0;
    src[0] = *srcp++;
    src[1] = *srcp++;
    src[2] = *srcp++;
    CActParamHolder* holder = &self->actParamHolder;
    CTaskGame_enumListCtor(&lh);
    startEnumObjects(CTaskGame_enumListGet(&lh), src[self->equipSlot], 0);
    if (CTaskGame_enumListGet(&lh)->field_620 >= 1) {
        CModelDispSlot* slot = getEntryAt(CTaskGame_enumListGet(&lh), 0);
        cf::CfObjectMove* cfMove = slot->field_04;
        if (cfMove != 0) {
            CModelDispActor* actor = getCfObjectPc(cfMove);
            int ready = actor != 0;
            u32 m = 0;
            if (actor != 0) {
                m = actor->field_3F2C;
                if (m == 0) ready = 0;
                if (reinterpret_cast<cf::CfObject*>(&actor->move)->CfObject_isMoveActiveNow() == 0) ready = 0;
                if (ModelDispEquip_LoadModelArc(self, actor->field_3F28) == 0) ready = 0;
            }
            if (holder->field_0x00 == 0 && ready != 0) {
                // ---- build the display model (retail statement order) ----
                ((CActParamHolderTail*)holder)->currentModelPtr = (u32)actor;
                holder->field_0x00 = Scn_SetupAnim(self->somePtr, m, -1, 1);
                reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfunc64(0);
                ((CActParamHolderTail*)holder)->equipPtrs[1] =
((u32)reinterpret_cast<cf::CfObject*>(&actor->move)->CfObject_getSlotBits(1) >> 10) & 0x3FF;
                s16 be = CfObjectMove_getSubB0FieldA(reinterpret_cast<cf::CfObjectMove*>(&actor->move));
                CModelDispParamSlot* param = CfRes_getPcGridEntry(be, 0);
                // u8 index: retail emits clrlwi/mulli address math + cmplwi/ble
                for (u8 idx = 2; idx <= 5; idx++) {
if (reinterpret_cast<cf::CfObject*>(&actor->move)->CfObject_getSlotBits(idx) != 0) {
                        CResLookup* obj = param[idx].field_2C;
                        func_804831C4(reinterpret_cast<CScnItemModel*>(holder->field_0x00),
                                      reinterpret_cast<CModelDispNameParam*>(obj->getResourceBase(&param[idx], actor->field_3F28)));
                        holder->equipPtrs[idx] =
((u32)reinterpret_cast<cf::CfObject*>(&actor->move)->CfObject_getSlotBits(idx) >> 10) & 0x3FF;
                    }
                }
                holder->field_0x04 = initMcaFile(self->somePtr, actor->field_3F30, &lbl_eu_80507FF8[0]);
                holder->field_0x08 = initMcaFile(self->somePtr,
                    reinterpret_cast<u32>(reinterpret_cast<cf::CfObjectModel*>(&actor->move)->CfObjectModel_getAnimState()), &lbl_eu_80507FF8[4]);
                reinterpret_cast<CActParamAnim*>(&holder->actParam)->func_8004B114();
                attachAnimObj(&holder->actParam, reinterpret_cast<CScnItemModel*>(holder->field_0x00),
                              holder->field_0x08, reinterpret_cast<u32>(reinterpret_cast<cf::CfObjectModel*>(&actor->move)->CfObjectModel_getAnimState()));
                pushAnimNode(&holder->actParam, holder->field_0x04, actor->field_3F30);
                // Pinned via the §17.6 rotate intrinsic: plain '&= ~0x800000'
                // lets the optimizer pick a different mask encoding here.
                holder->actParam.field_0x0C =
                    __rlwinm(holder->actParam.field_0x0C, 0, 24, 22);
                setAnimSubPos(&holder->actParam,
                    (const f32*)writeVec3f(tmpB, lbl_eu_8066827C, lbl_eu_80668274, lbl_eu_80668274));
                holder->actParam.field_0x0C |= 0x160;
                // Copy-then-adjust shape (retail cmpwi/mr/beq/addi): default
                // is self itself, not null.
                u32 parentArg = reinterpret_cast<u32>(self);
                if (self != 0)
                    parentArg = reinterpret_cast<u32>(reinterpret_cast<u8*>(self) + 4);
                ModelDispEquip_SetActParamParent(&holder->actParam, parentArg);
                writeVec3f(tmpA, lbl_eu_8066827C, lbl_eu_80668274, lbl_eu_80668274);
                // Word-copy gives retail's lwz/stw schedule (a float copy would
                // emit lfs/stfs).
                struct V3 {
                    u32 w[3];
                };
                *(V3*)&self->colorR = *(const V3*)tmpA;
                self->colorA = lbl_eu_80668274;
                CModelDispParamSlot* mainSlot = CfRes_getArrayElem18Idx(be);
                if ((actor->field_3F08 & 0x1000) != 0) {
                    holder->animModelPtrs[0] = Scn_SetupAnimDefault(self->somePtr,
                        reinterpret_cast<CModelDispNameParam*>(mainSlot->field_2C->getResourceBase(mainSlot, 0)));
                    if (holder->animModelPtrs[0] != 0) {
                        reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfuncC4(
                            reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[0]),
                            reinterpret_cast<u32>(CfObjectMove_getBdatNameCol7(reinterpret_cast<cf::CfObjectMove*>(&actor->move), 0)), 0);
                    }
                }
                if ((actor->field_3F08 & 0x2000) != 0) {
                    holder->animModelPtrs[1] = Scn_SetupAnimDefault(self->somePtr,
                        reinterpret_cast<CModelDispNameParam*>(mainSlot->field_2C->getResourceBase(mainSlot, 0)));
                    if (holder->animModelPtrs[1] != 0) {
                        reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfuncC4(
                            reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[1]),
                            reinterpret_cast<u32>(CfObjectMove_getBdatNameCol7(reinterpret_cast<cf::CfObjectMove*>(&actor->move), 1)), 0);
                    }
                }
                // Retail nests the actParam walk inside both 0x20000 and
                // getAnimFlags() so the two beqs skip the loop as well.
                if ((actor->field_3F08 & 0x20000) != 0) {
                    if (reinterpret_cast<cf::CfObjectModel*>(&actor->move)->CfObjectModel_getAnimFlags() != 0) {
                        __ct__CMcaFile(&mca, reinterpret_cast<cf::CfObjectModel*>(&actor->move)->CfObjectModel_getAnimFlags());
                        holder->unk_55C = Scn_InitGlobalA(self->somePtr, mca.mDataAdj, &lbl_eu_80507FF8[8]);
                        for (u8 i = 0; i < 2; i++) {
                            CScnItemModel* am =
                                reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[i]);
                            if (am == 0 || holder->unk_55C == 0)
                                continue;
                            holder->actParams[i].field_0x378 = i;
                            AnimGame_SetStateFlags(&holder->actParams[i]);
                            attachAnimObj(&holder->actParams[i], am, holder->unk_55C,
                                          reinterpret_cast<cf::CfObjectModel*>(&actor->move)->CfObjectModel_getAnimFlags());
                            func_8004B9D4(&holder->actParams[i],
                                          getAnimModelId(&holder->actParam), 0, -1, 0);
                        }
                    }
                }
                if (actor->field_3F28 == 8) {
                    if (getQueuedFileEventCount__Q22cf13CfGameManagerFv() >= 0x167) {
                        reinterpret_cast<CScnItemModel*>(actor->field_3F34)->vfunc28(reinterpret_cast<u32>(&lbl_eu_80507FF8[0xC]), 0);
                    }
                }
                holder->timer = 0x96;
                reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfunc48(self->alpha);
                reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfunc9C(3, 0);
                self->state21 = 1;
                ModelDispEquip_StartFadeOut(self);
            } else if (holder->field_0x00 != 0 && ready == 0) {
                ModelDispEquip_TeardownHolder(self, holder);
            }
        }
    } else {
        ModelDispEquip_TeardownHolder(self, holder);
    }
    __dt__80043E88(&lh, -1);
}

// ============================================================
// us-80202078 | ModelDispEquip_SetActParamParent
// ============================================================
// Retail always calls this (never inlines it) - keep the noinline.
extern "C" __declspec(noinline) void ModelDispEquip_SetActParamParent(void* self, u32 val) { *(u32*)((u8*)self + 4) = val; *(u32*)((u8*)self + 0x34) = val; }

// ============================================================
// us-80202084 | ModelDispEquip_BuildEquipModel
// ============================================================
// Build the equipment display for the 4th+ equip slot (equipSlot >= 3,
// dispatched from ModelDispEquip_BuildDisplay): resolve the character record, load the 3
// shared equip files + 5 per-slot model files, then construct the display
// model and wire up the anim models (mirror of ModelDispEquip_BuildPartyModel's build tail).
extern "C" void ModelDispEquip_BuildEquipModel(CModelDispEquip* self) {
    CActParamHolder* holder = &self->actParamHolder;
    u32 outType;
    CModelDispFileCtx ctx;
    f32 tmpA[3];
    f32 tmpB[3];
    CModelDispMca mca;
    char buf1[0x20];
    char buf2[0x20];
    char buf3[0x20];
    PartyStateWin_InitMemCounters(&ctx);
    if (holder->field_0x00 != 0) return;

    CtrlObjectParam_GetSlotTableBase();
    u8 charId = (u8)GetCollectedFlagByte((u8)self->equipSlot);
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
            u32 alloc = PartyStateWin_PickAllocHandle(&ctx, (u32)size);
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
            u32 alloc = PartyStateWin_PickAllocHandle(&ctx, (u32)size);
            CFileHandle* fh = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(alloc, buf2, self, 0, 0);
            self->fileSlots[7].handle = fh;
            setHandleFlag1__11CDeviceFileFP11CFileHandle(fh);
        }
        allLoaded = 0;
    }
    if (self->fileSlots[8].data == 0) {
        if (self->fileSlots[8].handle == 0) {
            char* name = BdatTouchStringCell(&lbl_eu_80507FF8[0x2F], &lbl_eu_80507FF8[0x3B], rec->weaponId);
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
                u32 alloc = PartyStateWin_PickAllocHandle(&ctx, (u32)size);
                CFileHandle* fh = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(alloc, buf3, self, 0, 0);
                self->fileSlots[8].handle = fh;
                setHandleFlag1__11CDeviceFileFP11CFileHandle(fh);
            }
        }
        allLoaded = 0;
    }

    // Per-slot equip model files (slots 0-4): charId <= 8 registers the model
    // directly via CfRes_tryResolveToken; larger ids go through the async file load.
    // Buffer declared outside the loop with explicit per-iteration resets:
    // the invariant zero stores hoist into a saved reg (retail r20) instead
    // of being rematerialized each iteration.
    ml::FixStr<64> buf(false);
    // false-init + explicit resets keep the ctor inline (retail emits two
    // zero stores); note MWCC folds any spelling of these constants into
    // identical code.
    for (u32 i = 1; i <= 5; i++) {
        u32 modelId = ((CActParamHolderTail*)holder)->equipPtrs[i];
        if (modelId == 0) continue;
        buf.mString[0] = '\0';
        buf.mLength = 0;
        if (func_800AA33C(buf, modelId, 1, 1) == 0) continue;
        FileSlot* fs = &self->fileSlots[i - 1];
        if (fs->data != 0) continue;
        allLoaded = 0;
        if (CfRes_isGridLoadIdle() == 0) continue;
        if (charId > 8) {
            if (fs->handle != 0) continue;
            int size = getFileSize__11CDeviceFileFPCc(buf.mString, 1);
            u32 alloc = PartyStateWin_PickAllocHandle(&ctx, (u32)size);
            CFileHandle* fh = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(alloc, buf.mString, self, 0, 0);
            fs->handle = fh;
            setHandleFlag1__11CDeviceFileFP11CFileHandle(fh);
        } else {
            fs->flag = 1;
            fs->data = (u8*)CfRes_tryResolveToken(((CActParamHolderTail*)holder)->equipPtrs[i], &outType);
            if (outType == 0xFFFFFFFF) {
                syncFieldData__Q22cf13CfGameManagerFv(charId, 0);
            }
        }
    }

    if (ModelDispEquip_LoadModelArc(self, charId) == 0) allLoaded = 0;
    if (allLoaded == 0) return;

    // ---- build the display model ----
    // No cached model local: retail reloads holder->field_0x00 (r30-based
    // lwz) before every use; an extra live value shifts the saved-reg pool.
    holder->field_0x00 = Scn_SetupAnim(self->somePtr, (u32)self->fileSlots[0].data, -1, 1);
    reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfunc64(0);
    simSetLeafFlag4000(reinterpret_cast<CModelDispObj*>(holder->field_0x00), 1);
    func_80482DF4(reinterpret_cast<CModelDispObj*>(holder->field_0x00), 1);
    if (holder->field_0x00 == 0) return;
    func_804831C4(reinterpret_cast<CScnItemModel*>(holder->field_0x00),
                  reinterpret_cast<CModelDispNameParam*>(self->fileSlots[1].data));
    func_804831C4(reinterpret_cast<CScnItemModel*>(holder->field_0x00),
                  reinterpret_cast<CModelDispNameParam*>(self->fileSlots[2].data));
    func_804831C4(reinterpret_cast<CScnItemModel*>(holder->field_0x00),
                  reinterpret_cast<CModelDispNameParam*>(self->fileSlots[3].data));
    func_804831C4(reinterpret_cast<CScnItemModel*>(holder->field_0x00),
                  reinterpret_cast<CModelDispNameParam*>(self->fileSlots[4].data));

    // Pick the anim-model set for the weapon type (2 = off-hand only,
    // 3 = both, 8 = both, anything else = main-hand only).
    // f5/wtype stay untyped-register-friendly: retail holds f5 in r20 across
    // the whole dispatch and the wtype byte in r0.
    CModelDispNameParam* f5 = reinterpret_cast<CModelDispNameParam*>(self->fileSlots[5].data);
    u8 wtype = (u8)BdatGetU8ByTableKey(&lbl_eu_80507FF8[0x2F], &lbl_eu_80507FF8[0x58], rec->weaponId);
    if (wtype == 8) {
        holder->animModelPtrs[0] = Scn_SetupAnimDefault(self->somePtr, f5);
        holder->animModelPtrs[1] = Scn_SetupAnimDefault(self->somePtr, f5);
    } else {
        wtype = (u8)BdatGetU8ByTableKey(&lbl_eu_80507FF8[0x2F], &lbl_eu_80507FF8[0x62], rec->weaponId);
        if (wtype == 3) {
            holder->animModelPtrs[0] = Scn_SetupAnimDefault(self->somePtr, reinterpret_cast<CModelDispNameParam*>(f5));
            holder->animModelPtrs[1] = Scn_SetupAnimDefault(self->somePtr, reinterpret_cast<CModelDispNameParam*>(f5));
        } else if (wtype == 2) {
            holder->animModelPtrs[0] = Scn_SetupAnimDefault(self->somePtr, f5);
        } else {
            holder->animModelPtrs[1] = Scn_SetupAnimDefault(self->somePtr, f5);
        }
    }

    // Name the anim models and attach them to the display model.
    if (holder->animModelPtrs[0] != 0) {
        u8 sub = (u8)func_8014235C(rec->weaponId, &lbl_eu_80507FF8[0x58], 0);
        if (sub != 0) {
            reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfuncC4(
                reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[0]),
                reinterpret_cast<u32>(CfModel_GetBdatString(sub)), 0);
        }
    }
    if (holder->animModelPtrs[1] != 0) {
        u8 sub = (u8)func_8014235C(rec->weaponId, &lbl_eu_80507FF8[0x58], 1);
        if (sub != 0) {
            reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfuncC4(
                reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[1]),
                reinterpret_cast<u32>(CfModel_GetBdatString(sub)), 0);
        }
    }

    u32 f7 = (u32)self->fileSlots[7].data;
    u32 f6 = (u32)self->fileSlots[6].data;
    holder->field_0x04 = initMcaFile(self->somePtr, f7, &lbl_eu_80507FF8[0]);
    holder->field_0x08 = initMcaFile(self->somePtr, f6, &lbl_eu_80507FF8[4]);
    reinterpret_cast<CActParamAnim*>(&holder->actParam)->func_8004B114();
    attachAnimObj(&holder->actParam, reinterpret_cast<CScnItemModel*>(holder->field_0x00),
                  holder->field_0x08, f6);
    pushAnimNode(&holder->actParam, holder->field_0x04, f7);
    holder->actParam.field_0x0C &= ~0x800000;
    setAnimSubPos(&holder->actParam,
                  (const f32*)writeVec3f(tmpB, lbl_eu_8066827C, lbl_eu_80668274, lbl_eu_80668274));
    holder->actParam.field_0x0C |= 0x160;
    // Copy-then-adjust shape (retail cmpwi/mr/beq/addi): default is self
    // itself, not null.
    u32 parentArg = reinterpret_cast<u32>(self);
    if (self != 0)
        parentArg = reinterpret_cast<u32>(reinterpret_cast<u8*>(self) + 4);
    ModelDispEquip_SetActParamParent(&holder->actParam, parentArg);
    writeVec3f(tmpA, lbl_eu_8066827C, lbl_eu_80668274, lbl_eu_80668274);
    // Word-copy gives retail's lwz/stw color schedule (float assigns would
    // emit lfs/stfs). f8 is interleaved mid-copy like retail.
    struct V3 {
        u32 w[3];
    };
    *(V3*)&self->colorR = *(const V3*)tmpA;
    self->colorA = lbl_eu_80668274;

    u32 f8 = (u32)self->fileSlots[8].data;
    __ct__CMcaFile(&mca, f8);
    holder->unk_55C = Scn_InitGlobalA(self->somePtr, mca.mDataAdj, &lbl_eu_80507FF8[8]);

    // Wire both anim-model slots into the act-param array and start their anims.
    for (u8 i = 0; i < 2; i++) {
        CScnItemModel* am = reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[i]);
        if (am != 0 && holder->unk_55C != 0) {
            holder->actParams[i].field_0x378 = i;
            AnimGame_SetStateFlags(&holder->actParams[i]);
            attachAnimObj(&holder->actParams[i], am, holder->unk_55C, f8);
            func_8004B9D4(&holder->actParams[i], getAnimModelId(&holder->actParam), 0, -1, 0);
        }
    }

    // Character 8 gets a special greeting at high progress.
    if (charId == 8) {
        if (getQueuedFileEventCount__Q22cf13CfGameManagerFv() >= 0x167) {
            reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfunc28(reinterpret_cast<u32>(&lbl_eu_80507FF8[0xC]), 0);
        }
    }

    holder->timer = 0x96;
    reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfunc48(self->alpha);
    reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfunc9C(3, 0);
    self->state21 = 1;
    ModelDispEquip_StartFadeOut(self);
}

// ============================================================
// us-80202914 | ModelDispEquip_LoadModelArc
// ============================================================
// Load the equip model arc: build the packed path token from `param` and
// the manager handle, size the file, pick an alloc handle for the buffer
// (PartyStateWin_PickAllocHandle), and kick the async read into modelFileHandle.
// Path token: param<<20 | 0x78000000 | (rotl(handle,22) into bits 15..21) -
// retail emits slwi + oris + rlwimi for exactly this expression form.
extern "C" int ModelDispEquip_LoadModelArc(CModelDispEquip* self, u32 param) {
    CModelDispFileCtx ctx;
    PartyStateWin_InitMemCounters(&ctx);
    if (self->modelData != 0)
        return 1;
    // Positive-condition nesting: retail branches OVER the body to the shared
    // li r3,0 tail (bne/blt), instead of early-returning from inside.
    if (self->modelFileHandle == 0) {
        void* h = getBdatEntryColumn__Q22cf13CfGameManagerFv(param, 5);
        // false-init + explicit field stores keep MWCC from emitting the
        // out-of-line FixStr(bool) call - retail inlines the two zero stores.
        ml::FixStr<64> buf(false);
        buf.mString[0] = '\0';
        buf.mLength = 0;
        // retail: slwi(param,20) + oris 0x7800, then rlwimi(h rot22 into bits
        // 15..21). Keeping the insert inside the arg expression makes MWCC
        // copy the manager handle out of r3 (mr r6) before taking &buf.
        func_800AA33C(buf,
                      __rlwimi(((u32)param << 20) | 0x78000000, (u32)h, 22, 15, 21),
                      1, 1);
        int size = getFileSize__11CDeviceFileFPCc(buf.mString, 1);
        if (size >= 0) {
            u32 alloc = PartyStateWin_PickAllocHandle(&ctx, (u32)size);
            CFileHandle* fh =
                (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(alloc, buf.mString, self, 0, 0);
            self->modelFileHandle = fh;
            setHandleFlag1__11CDeviceFileFP11CFileHandle(fh);
        }
    }
    return 0;
}

// ============================================================
// us-802029e0 | ModelDispEquip_TickAnim
// ============================================================
// Per-frame tick for the equip display: while the holder is live, a countdown
// timer periodically rolls a random sign that picks one of three anim ids; for
// the two ids with matching anim state the animation is replayed. Then the
// act-param object and both occupied animation-model slots are ticked.
void ModelDispEquip_TickAnim(CModelDispEquip* self) {
    int i;
    CActParamHolder* holder = &self->actParamHolder;
    if (self->actParamHolder.field_0x00 == 0)
        return;
    if (holder->timer <= 0) {
        getInstance__Q22ml6MTRandFv();
        u32 r = rand31__Q22ml6MTRandFv();
        s32 sign = (s32)(((r & 1) ^ (r >> 31)) - (r >> 31));
        s32 anim = lbl_eu_80662738[sign];
        if ((anim == 0x12 && holder->actParam.field_0x374 == 0) ||
            (anim == 0x7 && holder->actParam.field_0x374 == 0xb)) {
            func_8004B9D4(&holder->actParam, reinterpret_cast<void*>(anim), 0, -1, 0);
        }
        holder->timer = 0x96;
    }
    holder->timer--;
    tickAnimFrame(&holder->actParam);
    for (i = 0; i < 2; i++) {
        if (holder->animModelPtrs[i] != 0)
            tickAnimFrame(&holder->actParams[i]);
    }
}

// ============================================================
// us-80202ad8 | OnFileEvent__15CModelDispEquipFP10CEventFile
// ============================================================
// CDeviceFile async-load callback: find the slot whose handle matches the
// event's file handle, hand the loaded buffer to the slot, and clear the
// handle. The 9-slot scan falls back to the dedicated model slot (0x1090),
// whose buffer becomes modelData (only for event type 1).
int CModelDispEquip::OnFileEvent(CEventFile* event) {
    // h declared before d: MWCC colors locals in declaration order here, and
    // retail wants the buffer temp (d) in r4 with the lbl pointer in r5.
    CFileHandle* h;
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
    h = modelFileHandle;
    if (h == event->field_04) {
        if (event->field_00 == 1) {
            d = h->mData;
            h->mData = 0;
            modelData = d;
            // Two-arg register call: retail passes the old buffer (d, r4)
            // alongside the manager.
            EffSched_LookupA(lbl_eu_8065FC18, d);
        }
        modelFileHandle = 0;
    }
    return 0;
}

// ============================================================
// us-80202b8c | ModelDispEquip_RearmAnimSlot
// ============================================================
// Per-slot model hooks: if the animation model slot is occupied, stop the old
// model via vtable+0xC8 then re-arm it via vtable+0xC4 with (model, arg, 0).
extern "C" void ModelDispEquip_RearmAnimSlot(CModelDispEquip* self, void* arg, int index) {
    CActParamHolder* holder = &self->actParamHolder;
    if (holder->animModelPtrs[index] != 0) {
        reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfuncC8(
            reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[index]));
        reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfuncC4(
            reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[index]),
            reinterpret_cast<u32>(arg), 0);
    }
}

extern "C" void ModelDispEquip_RearmAnimSlotChecked(CModelDispEquip* self, void* move, void* arg, int index) {
    if (move == 0) return;
    if (CfModel_UpdateBdat(arg) <= 0) return;
    CActParamHolder* holder = &self->actParamHolder;
    if (holder->animModelPtrs[index] == 0) return;
    // Stop + re-arm the animation-model slot (same shape as ModelDispEquip_RearmAnimSlot),
    // guarded on the move pointer and the name lookup.
    reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfuncC8(
        reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[index]));
    reinterpret_cast<CScnItemModel*>(holder->field_0x00)->vfuncC4(
        reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[index]),
        reinterpret_cast<u32>(arg), 0);
}

// ============================================================
// us-80202ca8 | ModelDispEquip_SwapPartyEquip
// ============================================================
// Stop the previous equipment model and hand the picked name record to
// ModelDispEquip_RearmAnimSlotChecked: `kind` 0 uses the anim slot (CfObjectMove_getBdatNameCol11), `kind` 1 the
// equipment slot (CfObjectMove_getBdatNameCol7); `subKind` picks the slot index.
extern "C" void ModelDispEquip_SwapPartyEquip(CModelDispEquip* self, CModelDispParent* parent,
                              int kind, int subKind) {
    CActParamHolder* holder = &self->actParamHolder;
    if (holder->field_0x00 != parent->field_0x3A0) return;
    // 12-byte filter triple: retail copies it as one struct value (lis/lwzu/
    // lwz/lwz into r6/r5/r4/r0) before storing to the stack slot.
    CModelDispFilterTbl tbl = *(const CModelDispFilterTbl*)lbl_eu_80507FDC;
    CModelDispListHolder lh;
    CTaskGame_enumListCtor(&lh);
    startEnumObjects(CTaskGame_enumListGet(&lh), tbl.slot[self->equipSlot], 0);
    // Fresh enum-list walks: the count check and the slot lookup each re-call
    // CTaskGame_enumListGet (retail never reuses the previous result).
    if (CTaskGame_enumListGet(&lh)->field_620 >= 1) {
        CModelDispSlot* slot = getEntryAt(CTaskGame_enumListGet(&lh), 0);
        cf::CfObjectMove* cfMove = slot->field_04;
        if (cfMove != 0) {
            CModelDispActor* actor = getCfObjectPc(cfMove);
            // Retail initializes res (r5) before idx (r30).
            CModelDispNameParam* res = 0;
            int idx = 0;
            // Retail builds the dispatch as cmpwi/beq chains with out-of-line
            // case bodies (switch codegen), sharing one res check per kind.
            switch (kind) {
            case 0:
                switch (subKind) {
                case 0:
                    idx = 0;
                    res = CfObjectMove_getBdatNameCol11(reinterpret_cast<cf::CfObjectMove*>(&actor->move), idx);
                    break;
                case 1:
                    idx = 1;
                    res = CfObjectMove_getBdatNameCol11(reinterpret_cast<cf::CfObjectMove*>(&actor->move), idx);
                    break;
                }
                if (res != 0) ModelDispEquip_RearmAnimSlotChecked(self, actor, res, idx);
                break;
            case 1:
                switch (subKind) {
                case 0:
                    idx = 0;
                    res = CfObjectMove_getBdatNameCol7(reinterpret_cast<cf::CfObjectMove*>(&actor->move), idx);
                    break;
                case 1:
                    idx = 1;
                    res = CfObjectMove_getBdatNameCol7(reinterpret_cast<cf::CfObjectMove*>(&actor->move), idx);
                    break;
                }
                if (res != 0) ModelDispEquip_RearmAnimSlotChecked(self, actor, res, idx);
                break;
            }
        }
    }
    __dt__80043E88(&lh, -1);
}

// ============================================================
// us-80202e40 | ModelDispEquip_SwapWeaponEquip
// ============================================================
// Swap the equipment display model for `kind` (0/1) and `subKind` (0/1):
// look up the weapon's name record via the Bdat column (string table at
// lbl_eu_80507FF8 + 0x58/0x62) and hand it to ModelDispEquip_RearmAnimSlot.
extern "C" void ModelDispEquip_SwapWeaponEquip(CModelDispEquip* self, u32 unused,
                              int kind, int subKind) {
    getPlayer__Q22cf13CfGameManagerFi(0);
    // arg declared before index so MWCC colors arg=r4, index=r31 like retail.
    CModelDispNameParam* arg = 0;
    int index = 0;
    // Retail builds both dispatches as cmpwi/beq chains with out-of-line
    // case bodies (switch codegen), re-checking arg per kind block.
    switch (kind) {
    case 0:
        switch (subKind) {
        case 0:
            index = 0;
            arg = CfModel_GetBdatString((u8)func_8014235C(self->weaponId, &lbl_eu_80507FF8[0x62], 0));
            break;
        case 1:
            index = 1;
            arg = CfModel_GetBdatString((u8)func_8014235C(self->weaponId, &lbl_eu_80507FF8[0x62], 1));
            break;
        }
        if (arg != 0)
            ModelDispEquip_RearmAnimSlot(self, arg, index);
        break;
    case 1:
        switch (subKind) {
        case 0:
            index = 0;
            arg = CfModel_GetBdatString((u8)func_8014235C(self->weaponId, &lbl_eu_80507FF8[0x58], 0));
            break;
        case 1:
            index = 1;
            arg = CfModel_GetBdatString((u8)func_8014235C(self->weaponId, &lbl_eu_80507FF8[0x58], 1));
            break;
        }
        if (arg != 0)
            ModelDispEquip_RearmAnimSlot(self, arg, index);
        break;
    }
}

// ============================================================
// us-80201aa0 | ModelDispEquip_BuildDisplay
// ============================================================
extern "C" void ModelDispEquip_BuildDisplay(CModelDispEquip* self) {
    if (self->equipSlot < 3)
        ((void(*)(CModelDispEquip*))ModelDispEquip_BuildPartyModel)(self);
    else
        ((void(*)(CModelDispEquip*))ModelDispEquip_BuildEquipModel)(self);
}

// ============================================================
// us-80202c94 | ModelDispEquip_RefreshEquipRef
// ============================================================
extern "C" void ModelDispEquip_RefreshEquipRef(CModelDispEquip* self) {
    if (self->equipSlot < 3)
        ((void(*)(CModelDispEquip*))ModelDispEquip_SwapPartyEquip)(self);
    else
        ((void(*)(CModelDispEquip*))ModelDispEquip_SwapWeaponEquip)(self);
}

// ============================================================
// us-802017b8 | ModelDispEquip_StepIdleBuildOrTick
// ============================================================
extern "C" void ModelDispEquip_StepIdleBuildOrTick(CModelDispEquip* self) {
    if (self->state21 == 0)
        ((u32(*)(CModelDispEquip*))ModelDispEquip_BuildDisplay)(self);
    else
        ((void(*)(CModelDispEquip*))ModelDispEquip_TickAnim)(self);
}

// ============================================================
// us-80202f90 | ModelDispEquip_ReplaySlotAnims
// ============================================================
// For each occupied animation-model slot: pull the next chain object off the
// act-param, cache it at actParams[i]+0x4B4, and replay animation `arg`.
extern "C" void ModelDispEquip_ReplaySlotAnims(CModelDispEquip* self, u32 unused, void* arg) {
    CActParamHolder* holder = &self->actParamHolder;
    for (u8 i = 0; i < 2; i++) {
        if (holder->animModelPtrs[i] != 0) {
            void* p = getAnimChain(&holder->actParam);
            holder->actParams[i].field_0x4B4 = p;
            func_8004B9D4(&holder->actParams[i], arg, 0, -1, 0);
        }
    }
}

// ============================================================
// us-80203010 | ModelDispEquip_Noop1318 (empty)
// ============================================================
extern "C" void ModelDispEquip_Noop1318() {}

// ============================================================
// us-80203014 | ModelDispEquip_HandleSlotCmd
// ============================================================
// Handle a model-slot command: cmd 2 loads the anim model into the first free
// slot (creating an effect from the model arc, wiring it via the second-base
// adjusted-this call, and caching the vtable+0xA8 chain object); cmd 3 marks
// every occupied slot's effect active (byte +0x59).
// Retail passes the command buffer in r5: the vtable entry forwards
// (this, unused, cmd), so the middle parameter is part of the signature.
// Retail symbol is unmangled (global), so the free function has C linkage.
extern "C" void ModelDispEquip_HandleSlotCmd(CModelDispEquip* self, u32 unused,
                              const CModelDispEquipCmd* cmd) {
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
                    // Copy-then-adjust shape: retail passes self itself when
                    // null (mr r4,r26; beq skips the +8).
                    CModelDispEffectView* parent =
                        reinterpret_cast<CModelDispEffectView*>(self);
                    if (parent != 0)
                        parent = reinterpret_cast<CModelDispEffectView*>(
                            reinterpret_cast<u8*>(parent) + 8);
                    schedAttachChildSlot(effect, parent);
                    void* chain =
                        reinterpret_cast<void*>(reinterpret_cast<CScnItemModel*>(holder->animModelPtrs[i])->vfuncA8());
                    reinterpret_cast<CModelDispEffectView*>(
                        ((CActParamHolderTail*)holder)->animPtrs[i])->field_0x14 =
                        reinterpret_cast<u32>(chain);
                }
            }
        }
    } else if (cmd->field_0x0A == 3) {
        // effect declared before the index for retail scratch coloring.
        CModelDispEffectView* effect;
        for (u8 i = 0; i < 2; i++) {
            effect =
                (CModelDispEffectView*)((CActParamHolderTail*)holder)->animPtrs[i];
            if (effect != 0)
                effect->field_0x59 = 1;
        }
    }
}

// ============================================================
// us-80203138 | ModelDispEquip_Noop1440 (empty)
// ============================================================
extern "C" void ModelDispEquip_Noop1440() {}

// ============================================================
// us-8020313c | ModelDispEquip_ClearAnimPtr
// ============================================================
extern "C" void ModelDispEquip_ClearAnimPtr(CModelDispEquip* self, u32 val) {
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
// us-80203174 | ModelDispEquip_SetEquipSlot
// ============================================================
extern "C" void ModelDispEquip_SetEquipSlot(CModelDispEquip* self, s32 val) {
    // Calls through a cast pointer to keep MWCC from inlining the empty stub
    ((void(*)(CModelDispEquip*, s32))ModelDispEquip_ResetDisplay)(self, val);
    if (val >= 0)
        self->equipSlot = val;
    self->state21 = 0;
}

// ============================================================
// us-802031b8 | ModelDispEquip_NextEquipSlot
// ============================================================
extern "C" void ModelDispEquip_NextEquipSlot(CModelDispEquip* self) {
    ((void (*)(CModelDispEquip*))ModelDispEquip_ResetDisplay)(self);
    self->equipSlot += 1;
    if (self->equipSlot >= code80135FDC_getByte_64077())
        self->equipSlot = 0;
    self->state21 = 0;
}

// ============================================================
// us-80203214 | ModelDispEquip_PrevEquipSlot
// ============================================================
extern "C" void ModelDispEquip_PrevEquipSlot(CModelDispEquip* self) {
    ((void (*)(CModelDispEquip*))ModelDispEquip_ResetDisplay)(self);
    if (--self->equipSlot < 0)
        self->equipSlot = code80135FDC_getByte_64077() - 1;
    self->state21 = 0;
}

// ============================================================
// us-80203268 | ModelDispEquip_FadeAlphaUp
// ============================================================
// Fade alpha up while the act-param sub-object is live; bit 0x20 of the
// act-param flags guards the setTurnScale call.
extern "C" void ModelDispEquip_FadeAlphaUp(CModelDispEquip* self) {
    if (self->actParamHolder.field_0x00 != 0) {
        self->colorA += lbl_eu_80668280;
        self->actParamHolder.actParam.field_0x0C &= ~0x20;
        setTurnScale(&self->actParamHolder.actParam, self->colorA);
        self->actParamHolder.actParam.field_0x0C |= 0x20;
    }
}

// ============================================================
// us-802032cc | ModelDispEquip_FadeAlphaDown
// ============================================================
// Fade alpha down (mirror of ModelDispEquip_FadeAlphaUp).
extern "C" void ModelDispEquip_FadeAlphaDown(CModelDispEquip* self) {
    if (self->actParamHolder.field_0x00 != 0) {
        self->colorA -= lbl_eu_80668280;
        self->actParamHolder.actParam.field_0x0C &= ~0x20;
        setTurnScale(&self->actParamHolder.actParam, self->colorA);
        self->actParamHolder.actParam.field_0x0C |= 0x20;
    }
}

// ============================================================
// us-80203330 | ModelDispEquip_ScaleHeightUp
// ============================================================
// Ramp scale1[1]/scale2[1] up by the step, clamped at the upper bounds.
extern "C" void ModelDispEquip_ScaleHeightUp(CModelDispEquip* self) {
    self->scale1[1] += lbl_eu_80668280;
    if (self->scale1[1] > lbl_eu_80668284)
        self->scale1[1] = lbl_eu_80668284;
    self->scale2[1] += lbl_eu_80668280;
    if (self->scale2[1] > lbl_eu_80668288)
        self->scale2[1] = lbl_eu_80668288;
    func_8049EFF8(Scn_FindCamItem(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// us-802033b4 | ModelDispEquip_ScaleHeightDown
// ============================================================
// Ramp scale1[1]/scale2[1] down by the step, clamped at the lower bounds.
extern "C" void ModelDispEquip_ScaleHeightDown(CModelDispEquip* self) {
    self->scale1[1] -= lbl_eu_80668280;
    if (self->scale1[1] < lbl_eu_8066828C)
        self->scale1[1] = lbl_eu_8066828C;
    self->scale2[1] -= lbl_eu_80668280;
    if (self->scale2[1] < lbl_eu_80668290)
        self->scale2[1] = lbl_eu_80668290;
    func_8049EFF8(Scn_FindCamItem(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// us-80203438 | ModelDispEquip_ScaleDepthDown
// ============================================================
// Decrement scale1[2] toward lbl_eu_80668294, then sync pose + scales.
extern "C" void ModelDispEquip_ScaleDepthDown(CModelDispEquip* self) {
    self->scale1[2] -= lbl_eu_80668280;
    if (self->scale1[2] < lbl_eu_80668294)
        self->scale1[2] = lbl_eu_80668294;
    func_8049EFF8(Scn_FindCamItem(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// us-8020349c | ModelDispEquip_ScaleDepthUp
// ============================================================
// Increment scale1[2] toward lbl_eu_80668298 (mirror of ModelDispEquip_ScaleDepthDown).
extern "C" void ModelDispEquip_ScaleDepthUp(CModelDispEquip* self) {
    self->scale1[2] += lbl_eu_80668280;
    if (self->scale1[2] > lbl_eu_80668298)
        self->scale1[2] = lbl_eu_80668298;
    func_8049EFF8(Scn_FindCamItem(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// us-80203500 | ModelDispEquip_ResetColorStep
// ============================================================
// Re-arm the display: while the holder is live, rebuild the color quad from
// the (step, 1, 1) vector, poke the act-param object with colorA, and push
// the new colors through setAnimSubPos. Then snap scale1/scale2 to the
// sinit-built globals and sync pose + scales (same tail as ModelDispEquip_SyncScalePose).
void ModelDispEquip_ResetColorStep(CModelDispEquip* self) {
    if (self->actParamHolder.field_0x00 != 0) {
        struct V3 {
            u32 w[3];
        };
        u32 tmp[4];
        writeVec3f(tmp, lbl_eu_8066827C, lbl_eu_80668274, lbl_eu_80668274);
        *(V3*)&self->colorR = *(const V3*)tmp;
        self->colorA = lbl_eu_80668274;
        self->actParamHolder.actParam.field_0x0C &= ~0x20;
        setTurnScale(&self->actParamHolder.actParam, self->colorA);
        self->actParamHolder.actParam.field_0x0C = (self->actParamHolder.actParam.field_0x0C | 0x20) & ~0x100;
        setAnimSubPos(&self->actParamHolder.actParam, &self->colorR);
        self->actParamHolder.actParam.field_0x0C |= 0x100;
    }
    struct V3 {
        u32 w[3];
    };
    *(V3*)self->scale1 = *(const V3*)lbl_eu_80576550;
    *(V3*)self->scale2 = *(const V3*)lbl_eu_8057655C;
    func_8049EFF8(Scn_FindCamItem(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// us-802035f8 | ModelDispEquip_ResetColorWhite
// ============================================================
// Same as ModelDispEquip_ResetColorStep but the color quad is built from (1, 1, 1) - used to
// reset the display colors to neutral white.
void ModelDispEquip_ResetColorWhite(CModelDispEquip* self) {
    if (self->actParamHolder.field_0x00 != 0) {
        struct V3 {
            u32 w[3];
        };
        u32 tmp[4];
        writeVec3f(tmp, lbl_eu_80668274, lbl_eu_80668274, lbl_eu_80668274);
        *(V3*)&self->colorR = *(const V3*)tmp;
        self->colorA = lbl_eu_80668274;
        self->actParamHolder.actParam.field_0x0C &= ~0x20;
        setTurnScale(&self->actParamHolder.actParam, self->colorA);
        self->actParamHolder.actParam.field_0x0C = (self->actParamHolder.actParam.field_0x0C | 0x20) & ~0x100;
        setAnimSubPos(&self->actParamHolder.actParam, &self->colorR);
        self->actParamHolder.actParam.field_0x0C |= 0x100;
    }
    struct V3 {
        u32 w[3];
    };
    *(V3*)self->scale1 = *(const V3*)lbl_eu_80576550;
    *(V3*)self->scale2 = *(const V3*)lbl_eu_8057655C;
    func_8049EFF8(Scn_FindCamItem(reinterpret_cast<void*>(self->somePtr), -1), lbl_eu_80668274, self->scale1, self->scale2);
}

// ============================================================
// us-802036f0 | sinit_802019F8
// ============================================================
extern "C" void sinit_802019F8() {
    writeVec3f(lbl_eu_80576550, lbl_eu_80668274, lbl_eu_8066829C, lbl_eu_80668278);
    writeVec3f(lbl_eu_8057655C, lbl_eu_80668274, lbl_eu_80668270, lbl_eu_80668274);
}

// ============================================================
// Member function implementations (for thunks / virtual dispatch)
// ============================================================
void CModelDispEquip::resetBase() {
    *(unsigned long*)this = 0;
    *(unsigned long*)((char*)this + 4) = 0;
    ((unsigned char*)this)[8] = 0;
}

void CModelDispEquip::ModelDispEquip_ReplaySlotAnims() {}
void CModelDispEquip::vfunc18() {}
void CModelDispEquip::ModelDispEquip_HandleSlotCmd() {}
void CModelDispEquip::vfunc40() {}

void CModelDispEquip::thunk4_1298() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->ModelDispEquip_ReplaySlotAnims();
}

void CModelDispEquip::thunk4_1440() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->vfunc40();
}

void CModelDispEquip::thunk4_131C() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->ModelDispEquip_HandleSlotCmd();
}

void CModelDispEquip::thunk4_1318() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->vfunc18();
}

void CModelDispEquip::thunk4_F9C() {
    ModelDispEquip_RefreshEquipRef(reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4));
}

void CModelDispEquip::thunk4_dtor() {
    ((void(*)(void*))__dt__15CModelDispEquipFv)((char*)this - 4);
}

void CModelDispEquip::thunk8_1444() {
    ModelDispEquip_ClearAnimPtr(reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 8), 0);
}

void CModelDispEquip::thunk8_dtor() {
    ((void(*)(void*))__dt__15CModelDispEquipFv)((char*)this - 8);
}
// Retail: subi r3,r3,-4; b <target> (extern "C" free thunks, unmangled).
// The tail-call only adjusts r3; the callee's other params are left as-is,
// so the callees are cast to single-argument function pointers.
extern "C" void ModelDispEquip_ThunkReplayAnims(void* self) { ((void(*)(void*))ModelDispEquip_ReplaySlotAnims)((char*)self - 4); }
extern "C" void ModelDispEquip_ThunkSlotCmd(void* self) { ((void(*)(void*))ModelDispEquip_HandleSlotCmd)((char*)self - 4); }
extern "C" void ModelDispEquip_ThunkRefreshRef(void* self) { ((void(*)(void*))ModelDispEquip_RefreshEquipRef)((char*)self - 4); }
// Retail: subi r3,r3,-8; b ModelDispEquip_ClearAnimPtr
extern "C" void ModelDispEquip_ThunkClearAnimPtr(void* self) { ((void(*)(void*))ModelDispEquip_ClearAnimPtr)((char*)self - 8); }
