// Auto-scaffolded catalog TU for kyoshin/CEquipChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CEquipChange.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"

extern float lbl_eu_806682A8;
extern char lbl_eu_80508168[];

void func_801D47D4(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4);
void func_801D4260(CItemBoxInfo* info, u8 arg2);
void func_80138078__FUl(u32 op);
void func_80286B94(CEquipItemBox* box);
void func_80286D7C(CEquipItemBox* box);
int func_80288948(CEquipItemBox* box);
int func_80288530(CEquipItemBox* box);
int func_80287D58(CEquipItemBox* box);
int func_802882A4(CEquipItemBox* box);
void func_802873D8(CEquipItemBox* box);
int func_80286698(CEquipItemBox* box);
void func_80287F04(CEquipItemBox* box, u16 arg2, void* arg3, u16 arg4);
void func_801D216C(void* obj, int arg);
u32 func_801392B4(u32);
void* func_802052A8(CEquipChange* self);
int func_802031A0(CEquipChange* self);
void func_80202EB4(CEquipChange* self, u8 cat);

// Target us-80203cec: destructor. Destroys sub-objects at fixed member
// offsets (descending layout order), then frees the top-level object when
// the delete flag is set (MWCC-generated D2-in-D1 wrapper).
CEquipChange::~CEquipChange() {
    __dt__13CEquipItemBoxFv(&mEquipItemBox, -1);
    __dt__12CItemBoxInfoFv((CItemBoxInfo*)((u8*)this + 0xA4), -1);
    __dt__7CSubCurFv((CBaseCur*)((u8*)this + 0x80), -1);
    __dt__6CCur15Fv((CBaseCur*)((u8*)this + 0x68), -1);
    __dt__6CCur14Fv((CBaseCur*)((u8*)this + 0x50), -1);
    __dt__17UnkClass_8045F564Fv((UnkClass_8045F564*)((u8*)this + 0x14), -1);
    __dt__17UnkClass_8045F564Fv((UnkClass_8045F564*)((u8*)this + 0x4), -1);
}

// Target us-80203d88: load both bind files (two string records at
// lbl_eu_80508168) into file handles 0x24/0x28, then init the item-box info
// layout (0xA4) and the equip item box (0x2B0).
void func_80202090(CEquipChange* self) {
    const char* path = lbl_eu_80508168;
    self->field_24 = (u32)CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                                path, reinterpret_cast<IWorkEvent*>(self), 0, 0);
    CDeviceFile::setHandleFlag1((CFileHandle*)self->field_24);
    self->field_28 = (u32)CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                                path + 0x18, reinterpret_cast<IWorkEvent*>(self), 0, 0);
    CDeviceFile::setHandleFlag1((CFileHandle*)self->field_28);
    func_801D4054((CItemBoxInfo*)((u8*)self + 0xA4));
    func_802861A8(&self->mEquipItemBox);
}

// Forward declarations for functions whose definitions live at the end of this
// TU. Declaring (not defining) them here prevents MWCC from inlining their
// bodies into callers like func_80203994 / func_80203FCC, which must emit the
// retail `bl` (external) calls.
int func_802031A0(CEquipChange* self);

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

// Target us-802045dc: when both item-box gates are closed, remap the
// cursor-run flag 0x98 (see func_80203138's 0/2/4/6/8/10/12->... mapping) and
// refresh the equip info window with the compressed selection word.
void func_802028E4(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) == 0) {
        code80135FDC_getByte_64077();
        s8 sw = self->field_98;
        switch (sw) {
        case 0: self->field_98 = 12; break;
        case 1:
        case 2:
        case 3:
        case 4: self->field_98 = 0; break;
        case 5:
        case 6: self->field_98 = 4; break;
        case 7:
        case 8: self->field_98 = 6; break;
        case 9:
        case 10: self->field_98 = 8; break;
        case 11:
        case 12: self->field_98 = 10; break;
        case 13: self->field_98 = 12; break;
        }
        func_802040FC(self);
        u8 f99 = self->field_99;
        u32 r38 = (u32)self->func_80203138();
        u32 r3a = (u32)func_802031A0(self);
        func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4),
                      (u16)(((u32)f99 << 8) | ((r38 & 0xF) << 4) | (r3a & 0xF)),
                      func_802052A8(self), 1);
        func_801D4260((CItemBoxInfo*)((u8*)self + 0xA4), (u8)self->func_80203138());
        func_80138078__FUl(1);
    } else if (func_802865A8(&self->mEquipItemBox) != 0) {
        func_80286B94(&self->mEquipItemBox);
    }
}

// Target us-80204768: sibling of func_802028E4 - same guard/refresh shape
// but a different remap table for field_98.
void func_80202A70(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) == 0) {
        code80135FDC_getByte_64077();
        s8 sw = self->field_98;
        switch (sw) {
        case 0:
        case 1:
        case 2:
        case 3: self->field_98 = 4; break;
        case 4:
        case 5: self->field_98 = 6; break;
        case 6:
        case 7: self->field_98 = 8; break;
        case 8:
        case 9: self->field_98 = 10; break;
        case 10:
        case 11: self->field_98 = 12; break;
        case 12:
        case 13: self->field_98 = 0; break;
        }
        func_802040FC(self);
        u8 f99 = self->field_99;
        u32 r38 = (u32)self->func_80203138();
        u32 r3a = (u32)func_802031A0(self);
        func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4),
                      (u16)(((u32)f99 << 8) | ((r38 & 0xF) << 4) | (r3a & 0xF)),
                      func_802052A8(self), 1);
        func_801D4260((CItemBoxInfo*)((u8*)self + 0xA4), (u8)self->func_80203138());
        func_80138078__FUl(1);
    } else if (func_802865A8(&self->mEquipItemBox) != 0) {
        func_80286D7C(&self->mEquipItemBox);
    }
}

void func_80202BFC(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0 && func_802865A8(&self->mEquipItemBox) != 0)
        func_802870DC(&self->mEquipItemBox);
}

void func_80202C4C(){}

// vtable[0x2C] call on a layout object: (obj, arg, mode). Inlined 3x in
// func_80202CCC to reproduce retail's per-site bctrl sequences.
static void callSetSlot(void* obj, void* arg, int mode) {
    void* vtbl = *(void**)obj;
    void (*fn)(void*, void*, int) = (void(*)(void*, void*, int))*(void**)((u8*)vtbl + 0x2C);
    fn(obj, arg, mode);
}

// Target us-802049c4: item-box confirm / apply flow. When the box gates are
// open and not sub-paging, finalize the selection (func_80287F04 pack) and
// hand off to func_80202EB4. The gate-closed branch has its own checks.
void func_80202CCC(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) == 0) {
        // gate closed branch (.L_80204A8C)
        int v = *(int*)((u8*)func_8009EC9C((u8)func_801392B4(self->field_99)) + 0x176C);
        if (v == 1) {
            if ((u8)self->func_80203138() == 3)
                return;
            func_80138078__FUl(5);
            return;
        }
        if ((u8)self->func_80203138() == 3) {
            u8 idx = (u8)func_802031A0(self);
            s8 slot = *((s8*)self + 0x99 + idx);
            if (slot == 0)
                return;
            if ((u8)slot == 2)
                return;
        }
        // .L_80204B14
        callSetSlot((void*)self->field_34, (void*)self->field_3C, 0);
        callSetSlot((void*)self->field_34, (void*)self->field_38, 0);
        callSetSlot((void*)self->field_34, (void*)self->field_40, 1);
        func_801D216C((void*)((u8*)self + 0x50), 0);
        self->field_48 = 6;
        self->field_4D = 0;
        func_801D216C((void*)((u8*)self + 0x68), 0);
        func_80138078__FUl(3);
        return;
    }
    if (func_802865A8(&self->mEquipItemBox) == 0)
        return;
    if (func_80286698(&self->mEquipItemBox) == 0) {
        // .L_80204A14
        if (func_802882A4(&self->mEquipItemBox) == 0) {
            func_80138078__FUl(5);
            return;
        }
        func_802873D8(&self->mEquipItemBox);
        // .L_80204A30
        func_802042C0(self);
        u8 cat = self->field_99;
        u32 r38 = (u32)self->func_80203138();
        u32 r3a = (u32)func_802031A0(self);
        void* r5 = func_802052A8(self);
        u16 packed = (u16)(((u32)cat << 8) | ((r38 & 0xF) << 4) | (r3a & 0xF));
        func_80287F04(&self->mEquipItemBox, packed, r5, 1);
        func_80202EB4(self, self->field_99);
    } else {
        func_80287D58(&self->mEquipItemBox);
    }
}

// (func_80203138 and func_802031A0 are defined at the END of this TU to avoid
// inlining them into callers such as func_80203994 / func_80203FCC.)

void func_80203210(){
    // Address-take forces out-of-line bodies + external `bl` calls in all
    // callers (retail calls these helpers externally, never inlines).
    int (*volatile pa)(CEquipChange*) = func_802031A0;
    int (CEquipChange::*volatile p38)() = &CEquipChange::func_80203138;
    (void)pa;
    (void)p38;
}

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

// Target us-8020568c: cursor 3 is selected AND the per-slot byte at
// self[idx+0x99] (index from func_802031A0) equals 2.
int func_80203994(CEquipChange* self) {
    if ((u8)self->func_80203138() == 3) {
        u8 idx = (u8)func_802031A0(self);
        if (((u8*)self)[0x99 + idx] == 2)
            return 2;
    }
    return 0;
}

void func_802039F4(){}

// Target us-80205790: resolve the currently-equipped item's resource id from
// the active category (box gate open: func_80288530 category; gate closed:
// field_98) and the per-category item id read from the bdat row (obj+off).
int func_80203A98(CEquipChange* self, u32 param) {
    void* item;
    if (func_802865A0(&self->mEquipItemBox) != 0) {
        if (param == 0 && func_80288948(&self->mEquipItemBox) == 0)
            return 0;
        u8* obj = (u8*)func_8009EC9C((u8)func_801392B4(self->field_99));
        u32 r0 = (u8)func_80288530(&self->mEquipItemBox);
        switch (r0) {
        case 2: item = func_80157C4C(2, *(s16*)(obj + 0x26)); break;
        case 4: item = func_80157C4C(4, *(s16*)(obj + 0x1c)); break;
        case 5: item = func_80157C4C(5, *(s16*)(obj + 0x1e)); break;
        case 6: item = func_80157C4C(6, *(s16*)(obj + 0x20)); break;
        case 7: item = func_80157C4C(7, *(s16*)(obj + 0x22)); break;
        case 8: item = func_80157C4C(8, *(s16*)(obj + 0x24)); break;
        default: return 0;
        }
    } else {
        // box gate closed - key off the cursor-run flag field_98
        u8* obj = (u8*)func_8009EC9C((u8)func_801392B4(self->field_99));
        s8 r0 = self->field_98;
        if (r0 == 0)
            item = func_80157C4C(2, *(s16*)(obj + 0x26));
        else if (r0 == 4)
            item = func_80157C4C(4, *(s16*)(obj + 0x1c));
        else if (r0 == 6)
            item = func_80157C4C(5, *(s16*)(obj + 0x1e));
        else if (r0 == 8)
            item = func_80157C4C(6, *(s16*)(obj + 0x20));
        else if (r0 == 10)
            item = func_80157C4C(7, *(s16*)(obj + 0x22));
        else if (r0 == 12)
            item = func_80157C4C(8, *(s16*)(obj + 0x24));
        else
            return 0;
    }
    if (item != NULL) {
        u32 v = *(u32*)item;
        if (v != 0)
            return v >> 20;
    }
    return 0;
}

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

// Target us-80205d98: when both file handles (0x34, 0x30) are set, load the
// retail bind data and clear the cursor-run flag 0x98.
void func_802040A0(CEquipChange* self) {
    if (self->field_34 == 0)
        goto end;
    if (self->field_30 != 0)
        goto body;
    goto end;
body:
    func_802042C0(self);
    self->field_4C = 1;
    self->field_44 = 1;
    self->field_98 = 0;
end:
    ;
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

bool CEquipChange::OnFileEvent(CEventFile* file) { return true; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_802059E8(){}

// --- function definitions kept at end of TU so earlier callers emit `bl` ----

// placeholder body for func_802052A8 (target us-80207088); real body below
void* func_802052A8(CEquipChange* self) { return (void*)self->field_30; }

// placeholder body for func_80202EB4 (target us-80204bac)
void func_80202EB4(CEquipChange* self, u8 cat) { (void)self; (void)cat; }

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
// values 0..6 and 13 (real-body case at 13 keeps the table range at 13 so
// entries 7..12 route to default 0).
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

extern "C" void func_80202110() {}
extern "C" void func_802021E4() {}
extern "C" void func_8020228C() {}
