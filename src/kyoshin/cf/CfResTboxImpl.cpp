// Translation unit for kyoshin/cf/CfResTboxImpl
// Resource textbox implementation.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfResTboxImpl.hpp"
#include "kyoshin/cf/object/CfObjectObj.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

using namespace cf;

// us-801faaf0 - constructor: base CfResObjImpl ctor (parent passed through in
// r4), then store the retail vtable label at +0x10 (manual-vtable scheme).
void* __ct__cf_CfResTboxImpl(CfResTboxImpl* self, void* parent) {
    __ct__cf_CfResObjImpl(self, parent);
    self->field_10 = lbl_eu_80535204;
    return self;
}

int func_801F8E70() { return 256; }

// us-801fab34 - open test: dispatch the +0x14 vtable slot and negate the
// bool result (retail ends at the raw neg; an int return keeps the setnz
// off).
int func_801F8E78(CfResTboxImpl* self) {
    return -(int)!((CfResTboxVtIf*)self)->_v014();
}

// us-801fab6c - open/activate the textbox resource. Runs the parent's
// 0x17C/0x178 init slots, gates on CfGameManager state / global flags, then
// builds the textbox resources through the parent's +0x90/+0x94/+0x9C slots
// (selected by parent flag bits 0x20/0x10), clears flag bits 0x4/0x20000000,
// dispatches the +0x168 anim setter and finally runs the +0x38/+0x98
// sub-objects.
void func_801F8EB0(CfResTboxImpl* self) {
    u32 out8;          // sp+0x8 (func_80062114 out arg, block 2)
    u32 outC;          // sp+0xC (func_80062114 out arg, block 1)
    u8 buf64[0x44];    // sp+0x10 (FixStr<64> name buffer, block 2)

    ((CfResTboxParentVtIf*)self->field_00)->_v17C();
    ((CfResTboxParentVtIf*)self->field_00)->_v178();
    self->field_00->field_90 = 0;
    self->field_00->field_94 = 0;
    if (((cf::CfGameManager*)self->field_00)->func_80082900() == 0) return;
    if (lbl_eu_80663E24 & 0x100000) return;
    if (func_8008585C__Q22cf13CfGameManagerFv() != 0) return;
    ResInfoEntry* entry = (ResInfoEntry*)func_80063080();
    int ok = 1;
    if (entry->field_0x2C->vfunc0C(entry) == 0) ok = 0;
    if (!ok) return;
    self->field_08 = self->field_08 + 1;
    mtl::MemManager::func_80434A4C(false);
    if ((self->field_00->field_6C & 0x20) && self->field_00->field_98 == 0) {
        u32 handle1 = ((CfResTboxVtIf*)self)->_v034(1);
        self->field_00->field_90 = func_80066E7C(entry, handle1);
        self->field_00->field_90 =
            (u8*)func_80062114((char*)self->field_00->field_90, 0, (void**)&outC);
        float anim = ((CfResTboxParentVtIf*)self->field_00)->_v0E0();
        u8* obj = func_80489A60((u8*)lbl_eu_80663E14, self->field_00->field_90, 6, 1, 0, 0x70);
        func_800BBADC(self->field_00, obj);
        ((CfResTboxParentVtIf*)self->field_00)->_v0DC(anim);
    }
    if (self->field_00->field_6C & 0x10) {
        ml::FixStr<64>& name = *(ml::FixStr<64>*)buf64;
        name.mString[0] = 0;   // retail stb/stw init (the FixStr ctor would emit a bl)
        name.mLength = 0;
        if (self->field_00->field_9C == 0) {
            u32 handle0 = ((CfResTboxVtIf*)self)->_v034(0);
            func_800AA33C(name, handle0, 0, 0);
            u32 handle1 = ((CfResTboxVtIf*)self)->_v034(1);
            self->field_00->field_94 = func_80066E7C(entry, handle1);
            self->field_00->field_94 =
                (u8*)func_80062114((char*)self->field_00->field_94, 1, (void**)&out8);
            self->field_00->field_9C = (u8*)func_800584B8(
                (u32)CfRes_getD80Flag(), (u32)self->field_00->field_94, (const char*)&name);
        }
    }
    mtl::MemManager::func_80434A4C(true);
    if (self->field_00->field_6C & 0x20000000) {
        self->field_00->field_6C &= ~0x20000004;
        ((CfResTboxParentVtIf*)self->field_00)->_v168(lbl_eu_806681D0);
        func_800BC3B0((cf::CfObjectMove*)self->field_00, lbl_eu_806681D4);
    }
    func_800BCFA0((cf::CfObjectMove*)self->field_00);
    if (self->field_00->field_38 != 0) {
        ((CfResTbox38ObjIf*)self->field_00->field_38)->_v0B0();
    }
    if (self->field_00->field_98 != 0) {
        ((CfResTbox98ObjIf*)self->field_00->field_98)->_v088(1);
    }
}

void func_801F91B0(void) {}

// us-801fae70 - one-time install of the null PMTF into lbl_eu_805351E0, then
// dispatch through the counter-selected entry (when < 3).
void func_801F91B4(CfResTboxImpl* self) {
    if (lbl_eu_80664660 == 0) {
        *(CfPmf3*)lbl_eu_805351E0 = *(CfPmf3*)__ptmf_null;
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

// us-801faf44 - set the enable bit (0x80) on the slot's flag byte.
void func_801F9288(unsigned char* base, int idx1, int idx2, int idx3) {
    unsigned char* p = base + idx1 * 73 + idx2 * 16 + idx3 * 2;
    p[1] |= 0x80;
}

extern "C" void func_801F92B0(unsigned char* base, int idx1, int idx2, int idx3) {
    int offset = idx1 * 73 + idx2 * 16 + idx3 * 2;
    base[offset + 1] |= 0x40;
}
