// Auto-scaffolded catalog TU for kyoshin/CEquipChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CEquipChange.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CItemBoxInfo.hpp"

extern float lbl_eu_806682A8;

// Retail emits these as C-style (unmangled) symbols, so reference them with C
// linkage. func_801D2ED8/CEquipItemBox gates take the object and return status.
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" u8 func_801D2ED8(CBaseCur*);
extern "C" int func_802865A0(CEquipItemBox* box);
extern "C" int func_802865A8(CEquipItemBox* box);
extern "C" int func_80286650(CEquipItemBox* box);
extern "C" int func_80286698(CEquipItemBox* box);
extern "C" int func_802866A0(CEquipItemBox* box);
extern "C" int func_8028652C(CEquipItemBox* box);
extern "C" int func_80287EE8(CEquipItemBox* box);
extern "C" void func_802870DC(CEquipItemBox* box);
extern "C" void func_802040FC(CEquipChange* self);

u8 CEquipChange::func_802023C0() { return field_4D; }

void CEquipChange::func_802023C8() { func_802865A0(&mEquipItemBox); }

void CEquipChange::func_8020247C() { func_80286650(&mEquipItemBox); }

// Target us-802040c8: is either the sort menu or the sub-page current.
int func_802023D0(CEquipChange* self) {
    int result = 0;
    if (func_80286650(&self->mEquipItemBox) != 0)
        result = 1;
    else if (func_80286698(&self->mEquipItemBox) != 0)
        result = 1;
    return result;
}

// Target us-8020405c: read selection at 0xA4 unless the item box confirm op is busy.
int func_80202364(CEquipChange* self) {
    if (getItemBoxState((CItemBoxInfo*)((u8*)self + 0xA4)) == 0)
        return 0;
    if (func_8028652C(&self->mEquipItemBox) != 0)
        return self->field_4C;
    return 0;
}

// Target us-8020411c: item box ready and current selection is index 3.
int func_80202424(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0 && (u8)self->func_80203138() == 3)
        return 1;
    return 0;
}

// Target us-8020417c: subcur visible, else rely on the item box.
int func_80202484(CEquipChange* self) {
    if (func_801D2ED8((CBaseCur*)self->field_80) != 0)
        return 1;
    return func_802866A0(&self->mEquipItemBox);
}

void func_802024CC(){}

void func_80202578(){}

void func_80202644(){}

void func_80202790(){}

void func_802028E4(){}

void func_80202A70(){}

// Target us-802048f4: if both box gates are open, refresh the item box.
void func_80202BFC(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0 && func_802865A8(&self->mEquipItemBox) != 0)
        func_802870DC(&self->mEquipItemBox);
}

void func_80202C4C(){}

void func_80202CCC(){}

void func_80202EB4(){}

int CEquipChange::func_80203138() {
    signed char value = field_98;
    if (value == 0)
        return 2;
    if (value == 4)
        return 4;
    if (value == 6)
        return 5;
    if (value == 8)
        return 6;
    if (value == 10)
        return 7;
    if (value == 12)
        return 8;
    return 3;
}

// Target us-80204e98: field_98 (s8) maps via a dense jump table to 1..8 for
// values 0..7; any other value (including 8..13 that fall inside the table
// bound) returns 0.
int func_802031A0(CEquipChange* self) {
    switch (self->field_98) {
    case 0: return 1;
    case 1: return 2;
    case 2: return 3;
    case 3: return 4;
    case 4: return 5;
    case 5: return 6;
    case 6: return 7;
    case 13: return 8;
    default: return 0;
    }
}

void func_80203210(){}

// Target us-80205624: raw box count, cleared when the current selection is 3.
int func_8020392C(CEquipChange* self) {
    int result = func_80287EE8(&self->mEquipItemBox);
    if ((u8)self->func_80203138() == 3)
        result = 0;
    return result;
}

void func_801D2E4C(void* self);
void CEquipChange::func_8020397C() { func_801D2E4C(field_80); }

void func_80287FE0(void* self);
void CEquipChange::func_80203984() { func_80287FE0(&mEquipItemBox); }

void func_802886D8(void* self);
void CEquipChange::func_8020398C() { func_802886D8(&mEquipItemBox); }

void func_80203994(){}

void func_802039F4(){}

void func_80203A98(){}

// Target us-80205994: lift the last box byte if the box gate is open.
u8 func_80203C9C(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0)
        return self->mEquipItemBox.unk_37c;
    return 0;
}

void func_80203CE0(){}

void func_80203D78(){}

void func_80203E00(){}

// Target us-80205b90: when the 0x38 anim finishes, clear state 0x48 and flag 0x4D.
void func_80203E98(CEquipChange* self) {
    if (func_80137510(self->field_38, lbl_eu_806682A8) != 0) {
        self->field_4D = 1;
        self->field_48 = 0;
    }
}

void func_80203EE4(){}

// Target us-80205c7c: when the box sub-page closes, enter state 8.
void func_80203F84(CEquipChange* self) {
    if (func_802865A8(&self->mEquipItemBox) != 0) {
        self->field_48 = 8;
        self->field_4D = 1;
    }
}

void func_80203FCC(){}

// Target us-80205d44: when the 0x40 anim finishes, enter state 3 and refresh.
void func_8020404C(CEquipChange* self) {
    if (func_80137510(self->field_40, lbl_eu_806682A8) != 0) {
        self->field_48 = 3;
        self->field_4D = 1;
        func_802040FC(self);
    }
}

extern "C" void func_802042C0(CEquipChange* self);

// Target us-80205d98: when both file handles (0x34, 0x30) are set, load the
// retail bind data and clear the cursor-run flag 0x98.
void func_802040A0(CEquipChange* self) {
    if (self->field_34 != 0 && self->field_30 != 0) {
        func_802042C0(self);
        self->field_4C = 1;
        self->field_44 = 1;
        self->field_98 = 0;
    }
}

// Not a target; body kept non-empty
extern "C" void func_802040FC(CEquipChange* self) {
    self->field_48 = 0;
}


// func_802042C0 is a large retail function (0x80205FB8, 0x10BC bytes) not
// recreated in this TU; reference the external retail symbol rather than
// redefining it (a stub definition would inline away the call in
// func_802040A0/func_8020404C).

void func_80205294(void* dst, void* src) {
    u16 a = *(u16*)src;
    u32 b = *(u32*)((u8*)src + 4);
    *(u16*)dst = a;
    *(u32*)((u8*)dst + 4) = b;
}

void func_802052A8(){}

bool CEquipChange::OnFileEvent(CEventFile* file) { return true; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_802059E8(){}