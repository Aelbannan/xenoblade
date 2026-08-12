// CModelDispEquip - Equipment display model
// High-level C/C++ reconstruction

#include "kyoshin/menu/parts/CModelDispEquip.hpp"

#include "kyoshin/code_80135FDC.hpp"

// Forward decls: state-dispatch callees defined later in this TU.
extern "C" void func_801FFDB0(CModelDispEquip*);
extern "C" void func_801FF9AC(CModelDispEquip*);
extern "C" void func_801FFAC8(CModelDispEquip*);
extern "C" void func_801FFADC(CModelDispEquip*);

// ============================================================
// Target: us-80201254 | __ct__CModelDispEquip (constructor)
// ============================================================
extern "C" void __ct__CModelDispEquip(CModelDispEquip* self, u32 somePtr, s32 equipSlot) {
    CModelDispEquip* obj = self;
    obj->somePtr = somePtr;
    obj->equipSlot = equipSlot;
    obj->state = 0;
    obj->alpha = 0.0f;
    obj->alphaTimer = 1.0f;
    obj->state20 = 1;
    obj->state21 = 1;
    obj->weaponId = 0;
    obj->actParamHolder.timer = 0;
    obj->actParamHolder.unk_55C = 0;
    obj->currentModelPtr = 0;
    obj->modelFileHandle = 0;
    obj->modelData = 0;
    for (int i = 0; i < 2; i++) {
        obj->actParamHolder.animModelPtrs[i] = 0;
        obj->animPtrs[i] = 0;
    }
    for (int i = 0; i < 8; i++) {
        obj->equipPtrs[i] = 0;
    }
    for (int i = 0; i < 9; i++) {
        obj->fileSlots[i].handle = 0;
        obj->fileSlots[i].data = 0;
        obj->fileSlots[i].flag = 0;
    }
}

// ============================================================
// Target: us-802013cc | func_801FF6DC
// ============================================================
extern "C" void func_801FF6DC(u8* ptr) {
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
// NOTE: retail prologue is `stmw r30` (shape a) - MWCC_REFERENCE says only
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
// MWCC's lwzu base fold (MWCC_REFERENCE btm_sco_init note).
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
extern "C" void func_801FF874() {}

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
extern "C" __declspec(noinline) void func_801FF9AC(CModelDispEquip* self) {}

// ============================================================
// Target: us-802017a4 | func_801FFAB4 (storeFloats)
// ============================================================
extern "C" void func_801FFAB4(float* dest, float a, float b, float c, float d) {
    dest[0] = a;
    dest[1] = b;
    dest[2] = c;
    dest[3] = d;
}

// ============================================================
// Target: us-802017cc | func_801FFADC
// ============================================================
extern "C" __declspec(noinline) void func_801FFADC(CModelDispEquip* self) {}

// ============================================================
// Target: us-802018b4 | func_801FFBC4
// ============================================================
extern "C" void func_801FFBC4() {}

// ============================================================
// Target: us-80201ab4 | func_801FFDC4
// ============================================================
extern "C" void func_801FFDC4() {}

// ============================================================
// Target: us-80202078 | func_80200388
// ============================================================
extern "C" void func_80200388(void* self, u32 val) { *(u32*)((u8*)self + 4) = val; *(u32*)((u8*)self + 0x34) = val; }

// ============================================================
// Target: us-80202084 | func_80200394
// ============================================================
extern "C" void func_80200394() {}

// ============================================================
// Target: us-80202914 | func_80200C20
// ============================================================
extern "C" void func_80200C20() {}

// ============================================================
// Target: us-802029e0 | func_80200CE8
// ============================================================
extern "C" void func_80200CE8() {}

// ============================================================
// Target: us-80202ad8 | OnFileEvent__15CModelDispEquipFP10CEventFile
// ============================================================
extern "C" void OnFileEvent__15CModelDispEquipFP10CEventFile() {}

// ============================================================
// Target: us-80202b8c | func_80200E94
// ============================================================
// Per-slot model hooks: if the animation model slot is occupied, stop the old
// model via vtable+0xC8 then re-arm it via vtable+0xC4 with (model, arg, 0).
extern "C" void func_80200E94(CModelDispEquip* self, void* arg, int index) {
    CActParamHolder* holder = &self->actParamHolder;
    if (holder->animModelPtrs[index] != 0) {
        void** vtbl = *(void***)holder->field_0x00;
        // vtable+0xC8: (obj, modelPtr, index)
        ((void (*)(void*, void*, int))vtbl[50])(holder->field_0x00, holder->animModelPtrs[index], index);
        // vtable+0xC4: (obj, modelPtr, arg, 0) - reload obj/vtbl (call may clobber)
        vtbl = *(void***)holder->field_0x00;
        ((void (*)(void*, void*, void*, int))vtbl[49])(holder->field_0x00, holder->animModelPtrs[index], arg, 0);
    }
}

// ============================================================
// Target: us-80202c00 | func_80200F08
// ============================================================
extern "C" void func_80200F08() {}

// ============================================================
// Target: us-80202ca8 | func_80200FB0
// ============================================================
extern "C" void func_80200FB0() {}

// ============================================================
// Target: us-80202e40 | func_80201148
// ============================================================
extern "C" void func_80201148() {}

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
extern "C" void func_8020131C() {}

// ============================================================
// Target: us-80203138 | func_80201440 (empty)
// ============================================================
extern "C" void func_80201440() {}

// ============================================================
// Target: us-8020313c | func_80201444
// ============================================================
extern "C" void func_80201444(CModelDispEquip* self, u32 val) {
    // Explicit countdown keeps MWCC from unrolling the 2-iteration loop;
    // retail's mtctr/bdnz needs unit -O4,s (MWCC_REFERENCE §16).
    u8 i = 0;
    u8 n = 2;
    do {
        if (self->animPtrs[i] == val)
            self->animPtrs[i] = 0;
        i++;
        n--;
    } while (n != 0);
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
extern "C" void func_80201808() {}

// ============================================================
// Target: us-802035f8 | func_80201900
// ============================================================
extern "C" void func_80201900() {}

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

void CModelDispEquip::OnFileEvent() {}
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
