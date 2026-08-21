// CfResObjImpl - resource implementation base of the kyoshin cf resource
// family (siblings: CfResTboxImpl / CfResPcImpl / CfResReloadImpl).

#include "kyoshin/harness_catalog.hpp"
#include <string.h>

#include "kyoshin/cf/CfResObjImpl.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "monolib/util/MemManager.hpp"

namespace cf {}
using namespace cf;

// Sentinel used for the invalid resource state halfwords (+0x0A/+0x0E).
static const s16 RES_STATE_INVALID = -1;

// Retail ctor: stores the parent ref first, seeds the constant fields
// (0.0f float, state halfwords), installs the manual vtable at +0x10 and
// zeroes the 2-word work area at +0x14 via memset.
cf::CfResObjImpl::CfResObjImpl(cf::CfResObjParent* parent) {
    field_00 = parent;
    field_04 = lbl_eu_80667690;
    // Dead zero-store: MWCC eliminates the store but its (merged) constant
    // node is born before the -1, which flips the r7/r8 constant coloring.
    field_08 = 0;
    field_0A = -1;
    field_0C = 0;
    field_0E = -1;
    field_10 = lbl_eu_80530F44;
    field_08 = 1;
    memset(&field_14, 0, sizeof(field_14));
}

// Arithmetic booleanization of the u16 "state >= 2" test on the halfword
// at +0x08 (MWCC's value-first `>=` idiom: subi(v-2)/orc(v|~2)/srwi(1)/
// subf/srwi(31)). Also reachable as vtable slot +0x14 of the manual vtable
// (func_8016C950 dispatches it).
int func_8016C860(CfResObjImpl* self) {
    return (u32)self->field_08 >= 2;
}

int func_8016C880(void* self) { return 128; }

// us-8016dbe4 - resource sound-notify: when the +0x14 in-use test passes,
// fetch the sound id from the +0x60 slot and, when the resolved sound slot
// holds a live sound object and arg4 is nonzero, set its player priority to
// arg4. f1/f2 pass through to func_801BFE20; arg3 (r5) is an unused
// register-slot parameter (retail never reads it).
void func_8016C888(cf::CfResObjImpl* self, int arg2, int arg3, int arg4, f32 f1, f32 f2) {
    if (((cf::CfResObjImplVtIf*)self)->_v014() != 0) {
        int id = ((cf::CfResObjImplVtIf*)self)->_v060();
        cf::SoundSlotEntry* slot =
            func_801BFAE4((u16)func_801BFE20(id, arg2, self->field_00->field_74, f1, f2));
        if (slot != 0 && arg4 != 0 && slot->field_00 != 0) {
            slot->field_00->SetPlayerPriority(arg4);
        }
    }
}

// Dispatch the in-use test (vtable slot +0x14 of the manual vtable at +0x10);
// map the boolean result to -1/1 (retail preloads -1 and skips the 1 on a
// zero result).
int func_8016C950(CfResObjImpl* self) {
    if (((CfResObjImplVtIf*)self)->_v014() != 0) {
        return 1;
    }
    return -1;
}

// us-8016dce8 - resource open/activate. Runs the parent's 0x17C/0x178 init
// slots, gates on CfGameManager state / the +0x2C resolution probe, then
// builds the model + text resources through the parent's +0x90/+0x94/+0x9C
// slots (selected by parent flag bits 0x20/0x10), dispatches the +0xDC anim
// setter and finally runs the +0x38/+0x98 sub-objects.
void func_8016C98C(cf::CfResObjImpl* self) {
    u8 buf64[0x44];    // sp+0x8 (FixStr<64> name buffer)

    ((cf::CfResObjParentVtIf*)self->field_00)->_v17C();
    ((cf::CfResObjParentVtIf*)self->field_00)->_v178();
    self->field_00->field_90 = 0;
    cf::CfResObjParent* parent = self->field_00;
    parent->field_94 = 0;
    if (((cf::CfGameManager*)parent)->func_80082900() == 0) return;
    ResInfoEntry* entry = (ResInfoEntry*)func_80063038();
    int ok = 1;
    if (entry->field_0x2C->vfunc0C(entry) == 0) ok = 0;
    if (!ok) return;
    self->field_08 = self->field_08 + 1;
    mtl::MemManager::func_80434A4C(false);
    if ((self->field_00->field_6C & 0x20) && self->field_00->field_98 == 0) {
        u32 handle1 = ((cf::CfResObjImplVtIf*)self)->_v034(1);
        self->field_00->field_90 = func_80066E7C(entry, handle1);
        f32 anim = ((cf::CfResObjParentVtIf*)self->field_00)->_v0E0();
        u8* obj = func_80489A60((u8*)lbl_eu_80663E14, self->field_00->field_90, 6, 1, 0, 0x70);
        func_800BBADC(self->field_00, obj);
        if (self->field_00->field_98 != 0 &&
            (((cf::CfResObjModel98Data*)self->field_00->field_98)->field_7A4 & 0x800000) != 0 &&
            cf::CfGameManager::func_80083298() != 0 &&
            ((cf::CfResObjGm98View*)cf::CfGameManager::func_80083298())->field_98 != 0) {
            self->field_00->field_98->_v078(((cf::CfResObjGm98View*)cf::CfGameManager::func_80083298())->field_98);
        }
        ((cf::CfResObjParentVtIf*)self->field_00)->_v0DC(anim);
    }
    if (self->field_00->field_6C & 0x10) {
        // Direct buffer derefs so the address is recomputed at each use
        // (retail emits stb/stw against sp and `addi r3, sp, 8` before the
        // formatter call, rather than keeping &buf64 in a saved register).
        ((ml::FixStr<64>*)buf64)->mString[0] = 0;   // retail stb/stw init (no bl ctor)
        ((ml::FixStr<64>*)buf64)->mLength = 0;
        if (self->field_00->field_9C == 0) {
            u32 handle0 = ((cf::CfResObjImplVtIf*)self)->_v034(0);
            func_800AA33C(*(ml::FixStr<64>*)buf64, handle0, 0, 0);
            u32 handle1 = ((cf::CfResObjImplVtIf*)self)->_v034(0);
            self->field_00->field_94 = func_80066E7C(entry, handle1);
            self->field_00->field_9C = (u8*)func_800584B8(
                (u32)CfRes_getD80Flag(), (u32)self->field_00->field_94, (const char*)buf64);
        }
    }
    mtl::MemManager::func_80434A4C(true);
    func_800BCFA0((cf::CfObjectMove*)self->field_00);
    u32 handle = ((cf::CfResObjImplVtIf*)self)->_v034(1);
    u8* obj = func_80066E7C(entry, (handle & 0x07FFFFFF) | 0xF0000000);
    if (obj != 0) {
        func_804B0A6C(self->field_00->field_60C, obj);
        func_800BE824(self->field_00, 1);
        self->field_00->field_6B8 = 2;
        self->field_00->field_6B4 |= 0x800;
    }
    if (self->field_00->field_38 != 0) {
        self->field_00->field_38->_v0B0();
    }
    if (self->field_00->field_98 != 0) {
        self->field_00->field_98->_v088((self->field_00->field_6C >> 28) & 1);
    }
}

// Indexed read of the 2-word work area at +0x14 (index must be < 2).
u32 func_8016CCBC(CfResObjImpl* self, int index) {
    if (index < 2) {
        return self->field_14[index];
    }
    return 0;
}

void func_8016CCDC(void) {}

// One-time install of the null PMTF into lbl_eu_80530F20, then dispatch
// through the state-selected entry (when the state at +0x08 is < 3).
void func_8016CCE0(CfResObjImpl* self) {
    if (lbl_eu_80664270 == 0) {
        u32* src = __ptmf_null;
        u32* dst = (u32*)lbl_eu_80530F20;
        dst[0] = *src++;
        dst[1] = *src++;
        dst[2] = *src++;
        lbl_eu_80664270 = 1;
    }
    u16 idx = self->field_08;
    if (idx < 3) {
        (self->*lbl_eu_80530F20[idx])();
    }
}

extern "C" int func_8016CD54(u8* self) { return 0; }

extern "C" int func_8016CD5C(u8* self) { return 0; }

extern "C" void func_8016CD64(void) {}

extern "C" void func_8016CD68(u8* self, int index, int value) {
    if (index < 2) {
        int* arr = (int*)((char*)self + 0x14);
        arr[index] = value;
    }
}

cf::CfResObjImpl::~CfResObjImpl() {}
