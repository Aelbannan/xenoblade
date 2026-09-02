// Translation unit for kyoshin/cf/CfResTboxImpl
// Resource textbox implementation.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfResTboxImpl.hpp"
#include "kyoshin/cf/object/CfObjectObj.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"

using namespace cf;

// us-801faaf0 - constructor: base CfResObjImpl ctor (parent passed through in
// r4), then store the retail vtable label at +0x10 (manual-vtable scheme).
void* __ct__cf_CfResTboxImpl(CfResTboxImpl* self, void* parent) {
    __ct__cf_CfResObjImpl(self, parent);
    self->vtbl() = &lbl_eu_80535204;
    return self;
}

int func_801F8E70() { return 256; }

// us-801fab34 - open test: dispatch the +0x14 vtable slot and negate the
// bool result (retail ends at the raw neg; an int return keeps the setnz
// off).
int func_801F8E78(CfResTboxImpl* self) {
    return -(int)!self->func_8016C860();
}

// us-801fab6c - open/activate the textbox resource. Runs the parent's
// 0x17C/0x178 init slots, gates on CfGameManager state / global flags, then
// builds the textbox resources through the parent's +0x90/+0x94/+0x9C slots
// (selected by parent flag bits 0x20/0x10), clears flag bits 0x4/0x20000000,
// dispatches the +0x168 anim setter and finally runs the +0x38/+0x98
// sub-objects.
void func_801F8EB0(CfResTboxImpl* self) {
    u32 outC;
    u32 out8;
    u8 buf64[0x44];

    ((cf::CfObjectModel*)self->field_00)->CfObjectModel_UnkVirtualFunc2();
    ((cf::CfObjectModel*)self->field_00)->CfObjectModel_UnkVirtualFunc1();
    self->field_00->field_90 = 0;
    CfResTboxParent* parent = self->field_00;
    parent->field_94 = 0;
    if (((cf::CfGameManager*)parent)->getEffectFlagState() == 0) return;
    if (lbl_eu_80663E24 & 0x100000) return;
    if (isSceneActive__Q22cf13CfGameManagerFv() != 0) return;
    ResInfoEntry* entry = (ResInfoEntry*)func_80063080();
    int ok = 1;
    if (entry->field_0x2C->isInUse(entry) == 0) ok = 0;
    if (!ok) return;
    self->field_08 = self->field_08 + 1;
    mtl::MemManager::setMemInitFlag(false);
    if ((self->field_00->field_6C & 0x20) && self->field_00->field_98 == 0) {
        u32 handle1 = self->func_8016CCBC(1);
        self->field_00->field_90 = func_80066E7C(entry, handle1);
        self->field_00->field_90 =
            (u8*)func_80062114((char*)self->field_00->field_90, 0, (void**)&outC);
        float anim = ((cf::CfObject*)self->field_00)->CfObject_UnkVirtualFunc36();
        u8* obj = func_80489A60((u8*)lbl_eu_80663E14, self->field_00->field_90, 6, 1, 0, 0x70);
        func_800BBADC(self->field_00, obj);
        ((cf::CfObject*)self->field_00)->CfObject_UnkVirtualFunc35(anim);
    }
    if (self->field_00->field_6C & 0x10) {
        ((ml::FixStr<64>*)buf64)->mString[0] = 0;
        ((ml::FixStr<64>*)buf64)->mLength = 0;
        if (self->field_00->field_9C == 0) {
            u32 handle0 = self->func_8016CCBC(0);
            func_800AA33C(*(ml::FixStr<64>*)buf64, handle0, 0, 0);
            u32 handle1 = self->func_8016CCBC(1);
            self->field_00->field_94 = func_80066E7C(entry, handle1);
            self->field_00->field_94 =
                (u8*)func_80062114((char*)self->field_00->field_94, 1, (void**)&out8);
            self->field_00->field_9C = (u8*)func_800584B8(
                (u32)CfRes_getD80Flag(), (u32)self->field_00->field_94, (const char*)buf64);
        }
    }
    mtl::MemManager::setMemInitFlag(true);
    if (self->field_00->field_6C & 0x20000000) {
        f32 anim = lbl_eu_806681D0;
        self->field_00->field_6C =
            *(volatile u32*)&self->field_00->field_6C & ~0x20000000;
        ((cf::CfObject*)self->field_00)->CfObject_UnkVirtualFunc70(anim);
        func_800BC3B0((cf::CfObjectMove*)self->field_00, lbl_eu_806681D4);
    }
    func_800BCFA0((cf::CfObjectMove*)self->field_00);
    if (self->field_00->field_38 != 0) {
        ((cf::CfObject*)self->field_00->field_38)->CfObject_UnkVirtualFunc24();
    }
    if (self->field_00->field_98 != 0) {
        self->field_00->field_98->vfunc88(1);
    }
}

void func_801F91B0(void) {}

void func_801F91B4(CfResTboxImpl* self) {
    if (lbl_eu_80664660 == 0) {
        u32* src = __ptmf_null;
        u32* dst = (u32*)lbl_eu_805351E0;
        *dst++ = *src++;
        dst[0] = src[0];
        dst[1] = src[1];
        lbl_eu_80664660 = 1;
    }
    u16 idx = self->field_08;
    if (idx < 3) {
        (self->*lbl_eu_805351E0[idx])();
    }
}

cf::CfResTboxImpl::~CfResTboxImpl() {}

bool func_801F9268(unsigned char* p, int i, int j) {
    return p[i * 0x49 + j * 2] != 0;
}

void func_801F9288(unsigned char* base, int idx1, int idx2, int idx3) {
    unsigned char* p = base + idx1 * 73 + idx2 * 16 + idx3 * 2;
    p[1] |= 0x80;
}

void func_801F92B0(unsigned char* base, int idx1, int idx2, int idx3) {
    unsigned char* p = base + idx1 * 73 + idx2 * 16 + idx3 * 2;
    p[1] |= 0x40;
}
