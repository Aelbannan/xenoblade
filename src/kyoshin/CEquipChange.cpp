// Auto-scaffolded catalog TU for kyoshin/CEquipChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CEquipChange.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"

// const routes the symbol into the readonly sdata2 pool; MWCC then hoists the
// lfs ABOVE the frame stores (retail shape) - see MWCC_CASES §CExchangeWin.
extern const float lbl_eu_806682A8;

extern "C" void func_80138078__FUl(u32 op);
extern "C" u8 func_801392B4(u32);
extern "C" void* func_802052A8(CEquipChange* self);
extern "C" int func_802031A0(CEquipChange* self);

// Target us-80203cec: destructor. Retail frame uses the stmw/lmw save shape
// with the null-check cmpwi hoisted above the frame stores; the extern "C"
// free-function form avoids MWCC's implicit member destruction (which would
// emit a duplicate __dt__13CEquipItemBoxFv after the body - MWCC_CASES
// kyoshin-dtors) and the optimize_for_size pragma forces the stmw merge.
// Sub-object dtors run in retail order (reverse layout order), then
// conditional operator delete when the delete flag is set.
#pragma optimize_for_size on
extern "C" void* __dt__12CEquipChangeFv(CEquipChange* self, int flags) {
    if (self != 0) {
        __dt__13CEquipItemBoxFv(&self->mEquipItemBox, -1);
        __dt__12CItemBoxInfoFv((CItemBoxInfo*)self->_padA4, -1);
        __dt__7CSubCurFv((CBaseCur*)self->field_80, -1);
        __dt__6CCur15Fv((CBaseCur*)self->_pad68, -1);
        __dt__6CCur14Fv((CBaseCur*)self->_pad50, -1);
        __dt__17UnkClass_8045F564Fv(self->_pad14, -1);
        __dt__17UnkClass_8045F564Fv(self->_pad04, -1);
        if (flags > 0)
            __dl__FPv(self);
    }
    return self;
}
#pragma optimize_for_size off

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
extern "C" int func_802031A0(CEquipChange* self);

u8 CEquipChange::func_802023C0() { return field_4D; }

void CEquipChange::func_802023C8() { func_802865A0(&mEquipItemBox); }

void CEquipChange::func_8020247C() { func_80286650(&mEquipItemBox); }

// Target us-802040c8: is either the sort menu or the sub-page current.
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
int func_802023D0(CEquipChange* self) {
    return (func_80286650(&self->mEquipItemBox) != 0) || (func_80286698(&self->mEquipItemBox) != 0);
}
#pragma optimize_for_size off

// Target us-8020405c: read selection at 0xA4 unless the item box confirm op is busy.
int func_80202364(CEquipChange* self) {
    if (getItemBoxState__FP12CItemBoxInfo((u8*)self + 0xA4) == 0)
        return 0;
    if (func_8028652C(&self->mEquipItemBox) != 0)
        return self->field_4C;
    return 0;
}

// Target us-8020411c: item box ready and current selection is index 3.
int func_80202424(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0 && (u8)func_80203138(self) == 3)
        return 1;
    return 0;
}

// Target us-8020417c: subcur visible, else rely on the item box.
int func_80202484(CEquipChange* self) {
    if (func_801D2ED8((CBaseCur*)self->field_80) != 0)
        return 1;
    return func_802866A0(&self->mEquipItemBox);
}

// Target us-802041c4: initialise the equip-change screen. When idle (0x48),
// set state 1, init the item-info window, repack the selection word and play
// the 0x6D sound. Retail frame saves 4 regs via stmw (opt-space prologue).
#pragma optimize_for_size on
void func_802024CC(CEquipChange* self) {
    if (self->field_48 == 0) {
        self->field_48 = 1;
        self->field_4D = 0;
        func_801D421C((CItemBoxInfo*)((u8*)self + 0xA4));
        func_801D4260((CItemBoxInfo*)((u8*)self + 0xA4), (u8)func_80203138(self));
        u8 f99 = self->field_99;
        u32 r38 = (u32)func_80203138(self);
        u32 r3a = (u32)func_802031A0(self);
        func_801D47D4((CItemBoxInfo*)self->_padA4,
                      (u16)(((u32)f99 << 8) | ((r38 & 0xF) << 4) | (r3a & 0xF)),
                      func_802052A8(self), 1);
        func_80138078__FUl(0x6d);
    }
}
#pragma optimize_for_size off

// Target us-80204270: close the equip-change screen. From state 3, enter
// state 4, close the three cursors, advance the item-info window, reset the
// three layout slot animations and play the 0x6 sound.
void func_80202578(CEquipChange* self) {
    if ((s32)self->field_48 == 3) {
        self->field_48 = 4;
        self->field_4D = 0;
        func_801D216C((void*)((u8*)self + 0x50), 0);
        func_801D216C((void*)((u8*)self + 0x68), 0);
        func_801D216C((void*)self->field_80, 0);
        advanceItemBoxState((CItemBoxInfo*)((u8*)self + 0xA4));
        ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_3C, 0);
        ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_40, 0);
        ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_38, 1);
        func_80138078__FUl(6);
    }
}

// Target us-8020433c: advance the equip selection. When the box gates are
// closed, roll the cursor-run flag 0x98 backwards past non-equipped slots,
// then refresh the equip info window; otherwise (both gates open) hand the
// input to the item box. Retail frame saves 4 regs via stmw.
#pragma optimize_for_size on
void func_80202644(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0) {
        if (func_802865A8(&self->mEquipItemBox) != 0)
            func_802867E0(&self->mEquipItemBox);
    } else {
        self->field_98 = (s8)(self->field_98 - 1);
        if (self->field_98 < 0)
            self->field_98 = 0xd;
        while (true) {
            if (self->field_98 < 0)
                self->field_98 = 0xd;
            u8 idx = (u8)func_802031A0(self);
            if (idx == 0)
                break;
            if (((u8*)self)[0x99 + idx] != 0)
                break;
            self->field_98 = (s8)(self->field_98 - 1);
        }
        func_802040FC(self);
        u8 f99 = self->field_99;
        int cur38 = func_80203138(self);
        int cur3a = func_802031A0(self);
        func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4),
                      (u16)(((u32)f99 << 8) | ((cur38 & 0xF) << 4) | (cur3a & 0xF)),
                      func_802052A8(self), 1);
        func_801D4260((CItemBoxInfo*)((u8*)self + 0xA4), (u8)func_80203138(self));
        func_80138078__FUl(1);
    }
}
#pragma optimize_for_size off

// Target us-80204488: when both item-box gates are closed, roll the cursor
// flag 0x98 forward past non-equipped slots, then refresh the equip info
// window; otherwise (both gates open) hand the input to the item box. Retail
// frame saves 4 regs via stmw (opt-space prologue).
#pragma optimize_for_size on
void func_80202790(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0) {
        if (func_802865A8(&self->mEquipItemBox) != 0)
            func_802869B4(&self->mEquipItemBox);
        return;
    }
    u8 v = (u8)(self->field_98 + 1);
    self->field_98 = (s8)v;
    if ((s8)v > 0xd)
        self->field_98 = 0;
    u8 idx = (u8)func_802031A0(self);
    if (idx != 0 && ((u8*)self)[0x99 + idx] == 0) {
        self->field_98 = (s8)(self->field_98 + 1);
        while (true) {
            if (self->field_98 > 0xd)
                self->field_98 = 0;
            idx = (u8)func_802031A0(self);
            if (idx == 0)
                break;
            if (((u8*)self)[0x99 + idx] != 0)
                break;
            self->field_98 = (s8)(self->field_98 + 1);
        }
    }
    func_802040FC(self);
    u8 f99 = self->field_99;
    u32 cur38 = (u32)func_80203138(self);
    u32 cur3a = (u32)func_802031A0(self);
    void* a8 = func_802052A8(self);
    // Two-step pack (same shape as matched func_80203EE4): the shifted term
    // first makes MWCC fold the unshifted cur3a into the clrlwi base.
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4), packed, a8, 1);
    int v38 = func_80203138(self);
    func_801D4260((CItemBoxInfo*)self->_padA4, (u8)v38);
    func_80138078__FUl(1);
}
#pragma optimize_for_size off

// Target us-802045dc: when both item-box gates are closed, remap the
// cursor-run flag 0x98 (see func_80203138's 0/2/4/6/8/10/12->... mapping) and
// refresh the equip info window with the compressed selection word. Retail
// emits one li/stb block per case (14 separate blocks in the jump table).
#pragma optimize_for_size on
void func_802028E4(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0) {
        if (func_802865A8(&self->mEquipItemBox) != 0)
            func_80286B94(&self->mEquipItemBox);
        return;
    }
    code80135FDC_getByte_64077();
    switch (self->field_98) {
    case 0: self->field_98 = 12; break;
    case 1: self->field_98 = 0; break;
    case 2: self->field_98 = 0; break;
    case 3: self->field_98 = 0; break;
    case 4: self->field_98 = 0; break;
    case 5: self->field_98 = 4; break;
    case 6: self->field_98 = 4; break;
    case 7: self->field_98 = 6; break;
    case 8: self->field_98 = 6; break;
    case 9: self->field_98 = 8; break;
    case 10: self->field_98 = 8; break;
    case 11: self->field_98 = 10; break;
    case 12: self->field_98 = 10; break;
    case 13: self->field_98 = 12; break;
    }
    func_802040FC(self);
    u8 f99 = self->field_99;
    u32 cur38 = (u32)func_80203138(self);
    u32 cur3a = (u32)func_802031A0(self);
    void* a8 = func_802052A8(self);
    // Two-step pack (same shape as matched func_80203EE4): the shifted term
    // first makes MWCC fold the unshifted cur3a into the clrlwi base.
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4), packed, a8, 1);
    int v38 = func_80203138(self);
    func_801D4260((CItemBoxInfo*)self->_padA4, (u8)v38);
    func_80138078__FUl(1);
}
#pragma optimize_for_size off

// Target us-80204768: sibling of func_802028E4 - same guard/refresh shape
// but a different remap table for field_98 (also one li/stb block per case).
#pragma optimize_for_size on
void func_80202A70(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0) {
        if (func_802865A8(&self->mEquipItemBox) != 0)
            func_80286D7C(&self->mEquipItemBox);
        return;
    }
    code80135FDC_getByte_64077();
    switch (self->field_98) {
    case 0: self->field_98 = 4; break;
    case 1: self->field_98 = 4; break;
    case 2: self->field_98 = 4; break;
    case 3: self->field_98 = 4; break;
    case 4: self->field_98 = 6; break;
    case 5: self->field_98 = 6; break;
    case 6: self->field_98 = 8; break;
    case 7: self->field_98 = 8; break;
    case 8: self->field_98 = 10; break;
    case 9: self->field_98 = 10; break;
    case 10: self->field_98 = 12; break;
    case 11: self->field_98 = 12; break;
    case 12: self->field_98 = 0; break;
    case 13: self->field_98 = 0; break;
    }
    func_802040FC(self);
    u8 f99 = self->field_99;
    u32 cur38 = (u32)func_80203138(self);
    u32 cur3a = (u32)func_802031A0(self);
    void* a8 = func_802052A8(self);
    // Two-step pack (same shape as matched func_80203EE4).
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4), packed, a8, 1);
    int v38 = func_80203138(self);
    func_801D4260((CItemBoxInfo*)self->_padA4, (u8)v38);
    func_80138078__FUl(1);
}
#pragma optimize_for_size off

void func_80202BFC(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0 && func_802865A8(&self->mEquipItemBox) != 0)
        func_802870DC(&self->mEquipItemBox);
}

// Target us-80204944: when both item-box gates are open, either reset the box
// cursor (func_80287250) or open the sub-page (func_80286740) depending on
// func_802865B0, entering state 9.
void func_80202C4C(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) == 0)
        return;
    if (func_802865A8(&self->mEquipItemBox) == 0)
        return;
    if (func_802865B0(&self->mEquipItemBox) != 0) {
        func_80287250(&self->mEquipItemBox, 0);
    } else {
        func_80286740(&self->mEquipItemBox);
        self->field_48 = 9;
        self->field_4D = 0;
    }
}

// Target us-802049c4: item-box confirm / apply flow. When the box gates are
// open and not sub-paging, finalize the selection (func_80287F04 pack) and
// hand off to func_80202EB4. The gate-closed branch refuses when the
// character is busy or the selection points at an empty / locked slot. Retail
// frame saves 4 regs via stmw (opt-space prologue).
#pragma optimize_for_size on
void func_80202CCC(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0) {
        if (func_802865A8(&self->mEquipItemBox) == 0)
            return;
        if (func_80286698(&self->mEquipItemBox) != 0) {
            func_80287D58(&self->mEquipItemBox);
            return;
        }
        // .L_80204A14
        if (func_802882A4(&self->mEquipItemBox) == 0) {
            func_80138078__FUl(5);
            return;
        }
        // .L_80204A30
        func_802873D8(&self->mEquipItemBox);
        func_802042C0(self);
        u8 cat = self->field_99;
        int cur38 = func_80203138(self);
        int cur3a = func_802031A0(self);
        void* a8 = func_802052A8(self);
        // Two-step pack (same shape as matched func_80203EE4).
        u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
        packed = (u16)(packed | ((u32)cat << 8));
        func_80287F04(&self->mEquipItemBox, packed, a8);
        func_80202EB4(self, self->field_99);
        return;
    }
    // gate closed branch (.L_80204A8C)
    CBdatCharData* data = (CBdatCharData*)func_8009EC9C((u8)func_801392B4(self->field_99));
    if (data->field_176C == 1) {
        if ((u8)func_80203138(self) == 3)
            return;
        func_80138078__FUl(5);
        return;
    }
    if ((u8)func_80203138(self) == 3) {
        if (((u8*)self)[0x99 + (u8)func_802031A0(self)] == 0)
            return;
        if ((u8)((u8*)self)[0x99 + (u8)func_802031A0(self)] == 2)
            return;
    }
    // .L_80204B14: reset the three layout slot animations and close the cursors.
    ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_3C, 0);
    ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_38, 0);
    ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_40, 1);
    self->field_48 = 6;
    self->field_4D = 0;
    func_801D216C(self->_pad50, 0);
    func_801D216C(self->_pad68, 0);
    func_80138078__FUl(3);
}
#pragma optimize_for_size off

// (func_80203138 and func_802031A0 are defined at the END of this TU. Retail
// calls them externally at every site; the global address-takes at the end of
// the TU stop MWCC's -inline auto / -ipa file inliner from folding their
// bodies into callers (the bodies stay emitted as standalone symbols).)

// Target us-80204f08: main equip-change flow. Returns 0 when the character is
// busy and the cursor sits on a non-equippable slot, when the item-box gates
// reject, when the selected slot is empty/locked, or when a category-restricted
// item cannot be equipped; 1 after the generic refresh; 2/3 from the weapon-row
// paths. Retail frame saves 4 regs via stmw (opt-space prologue).
#pragma optimize_for_size on
int func_80203210(CEquipChange* self) {
    CBdatCharData* obj = (CBdatCharData*)func_8009EC9C((u8)func_801392B4(self->field_99));
    if (obj->field_176C == 1) {
        s8 cur = self->field_98;
        if (cur == 0 || cur == 4 || cur == 6 || cur == 8 || cur == 10 || cur == 12)
            return 0;
    }
    if (func_802865A0(&self->mEquipItemBox) == 0 && self->field_98 == 0)
        return 0;
    int cat = func_80203138(self);
    if (func_802865A0(&self->mEquipItemBox) != 0) {
        if (func_8028847C(&self->mEquipItemBox) == 0)
            return 0;
        cat = func_80288530(&self->mEquipItemBox);
    }
    if ((u8)cat != 3) {
    switch ((u8)cat) {
    case 2: {
        s16 id = obj->field_26;
        if (id == -1)
            return 0;
        void* item = func_80157C4C(2, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)func_801361E8(lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = (v - 1) == 0;
            if (isOne && func_8009CF8C(0x3508) == 0)
                return 0;
        }
        func_8009E0A8(obj, -1);
        break;
    }
    case 4: {
        s16 id = obj->field_1C;
        if (id == -1)
            return 0;
        void* item = func_80157C4C(4, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)func_801361E8(lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = (v - 1) == 0;
            if (isOne && func_8009CF8C(0x3508) == 0)
                return 0;
        }
        func_8009E024(obj, -1);
        break;
    }
    case 5: {
        s16 id = obj->field_1E;
        if (id == -1)
            return 0;
        void* item = func_80157C4C(5, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)func_801361E8(lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = (v - 1) == 0;
            if (isOne && func_8009CF8C(0x3508) == 0)
                return 0;
        }
        func_8009E030(obj, -1);
        break;
    }
    case 6: {
        s16 id = obj->field_20;
        if (id == -1)
            return 0;
        void* item = func_80157C4C(6, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)func_801361E8(lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = (v - 1) == 0;
            if (isOne && func_8009CF8C(0x3508) == 0)
                return 0;
        }
        func_8009E03C(obj, -1);
        break;
    }
    case 7: {
        s16 id = obj->field_22;
        if (id == -1)
            return 0;
        void* item = func_80157C4C(7, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)func_801361E8(lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = (v - 1) == 0;
            if (isOne && func_8009CF8C(0x3508) == 0)
                return 0;
        }
        func_8009E048(obj, -1);
        break;
    }
    case 8: {
        s16 id = obj->field_24;
        if (id == -1)
            return 0;
        void* item = func_80157C4C(8, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)func_801361E8(lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = (v - 1) == 0;
            if (isOne && func_8009CF8C(0x3508) == 0)
                return 0;
        }
        func_8009E054(obj, -1);
        break;
    }
    default:
        break;
    }
    // shared refresh: wake the bdat row, filter the scene's enemy list and
    // play the 0x76 sound.
    func_800A1370(obj);
    if (self->field_99 < 3) {
        CEquipEnumHolder holder;
        u32 names[3];
        // Post-increment loads fold the base materialization into the first
        // access (retail: lis + lwzu + +4/+8 displacements).
        const u32* src = lbl_eu_80508068;
        names[0] = *src++;
        names[1] = *src++;
        names[2] = *src++;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), names[self->field_99], 0);
        if (((CEquipEnumList*)func_80043F18(&holder))->field_0x620 >= 1) {
            CEquipEnumListSlot* slot = (CEquipEnumListSlot*)func_800F6EC0(func_80043F18(&holder), 0);
            if (slot->field_0x4 != 0) {
                func_800BFDE0(func_800BFC68__FPQ22cf12CfObjectMove(slot->field_0x4), 0);
            }
        }
        __dt__80043E88(&holder, -1);
    }
    func_80138078__FUl(0x76);
        return 1;
    }
    // (u8)cat == 3 - weapon-row path: resolve the sub-cursor, reject
    // empty/locked slots, then equip the row item through the item-impl
    // vtable[0x44] hook.
    int v = func_802031A0(self);
    if (func_802865A0(&self->mEquipItemBox) != 0)
        v = func_80288544(&self->mEquipItemBox);
    u8 r4 = (u8)v;
    u8 slot = self->_pad9A[r4 - 1];
    if (slot == 0)
        return 0;
    if (slot == 1)
        return 0;
    if (slot == 2) {
        func_80138078__FUl(5);
        return 2;
    }
    int r29;
    switch (r4) {
    case 1: case 2: case 3: r29 = 2; break;
    case 4: r29 = 4; break;
    case 5: r29 = 5; break;
    case 6: r29 = 6; break;
    case 7: r29 = 7; break;
    case 8: r29 = 8; break;
    default: break;
    }
    if ((u8)v == 2)
        v = 1;
    else
        v = (u32)((u8)v - 3) < 1;
    s16 id = -1;
    switch (r29) {
    case 2: id = obj->field_26; break;
    case 4: id = obj->field_1C; break;
    case 5: id = obj->field_1E; break;
    case 6: id = obj->field_20; break;
    case 7: id = obj->field_22; break;
    case 8: id = obj->field_24; break;
    default: break;
    }
    r29 = (int)func_80157C4C((u8)r29, id);
    if (r29 != 0 && *(u32*)r29 != 0) {
        void* inst = CItem_initItemImplInstances((void*)r29);
        ((CItemImplVtblView*)inst)->v15((void*)r29, (u8)v, -1);
    }
    func_802042C0(self);
    func_801D4260((CItemBoxInfo*)((u8*)self + 0xA4), (u8)func_80203138(self));
    u8 f99 = self->field_99;
    int cur38 = func_80203138(self);
    int cur3a = func_802031A0(self);
    u32 packed = (u32)(cur3a & 0xF);
    packed = (packed & 0xF) | ((u32)(cur38 & 0xF) << 4);
    packed = (packed & 0xFF) | ((u32)f99 << 8);
    func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4), (u16)packed, func_802052A8(self), 1);
    if (func_802865A0(&self->mEquipItemBox) != 0)
        func_80287DB4(&self->mEquipItemBox, (u16)packed, 0, 1);
    func_80138078__FUl(0x77);
    return 3;
}
#pragma optimize_for_size off

// Target us-80205624: raw box count, cleared when the current selection is 3.
#pragma optimize_for_size on  // -O4,s keeps the retail stmw/lmw frame
int func_8020392C(CEquipChange* self) {
    int result = func_80287EE8(&self->mEquipItemBox);
    if ((u8)func_80203138(self) == 3)
        result = 0;
    return result;
}
#pragma optimize_for_size off

void func_801D2E4C(void* self);
void CEquipChange::func_8020397C() { func_801D2E4C(field_80); }

void func_80287FE0(void* self);
void CEquipChange::func_80203984() { func_80287FE0(&mEquipItemBox); }

void func_802886D8(void* self);
void CEquipChange::func_8020398C() { func_802886D8(&mEquipItemBox); }

// Target us-8020568c: cursor 3 is selected AND the per-slot byte at
// self[idx+0x99] (index from func_802031A0) equals 2.
int func_80203994(CEquipChange* self) {
    if ((u8)func_80203138(self) == 3) {
        u8 idx = (u8)func_802031A0(self);
        if (((u8*)self)[0x99 + idx] == 2)
            return 2;
    }
    return 0;
}

// Target us-802056ec: sub-cursor busy returns 0x2E; otherwise key the state
// byte (>=2 => sub-page open flag) and the item-box gate: gate open routes
// to func_8028876C, gate closed picks a menu id from the cursor state.
// Retail saves 2 regs via stmw/lmw (opt-space prologue).
#pragma optimize_for_size on
extern "C" int func_802039F4(CEquipChange* self) {
    if (func_801D2ED8((CBaseCur*)self->field_80) != 0)
        return 0x2E;
    u8 b = code80135FDC_getByte_64077();
    int flag = (int)((u32)(1 - b) >> 31);
    if (func_802865A0(&self->mEquipItemBox) != 0)
        return func_8028876C(&self->mEquipItemBox);
    if ((u8)func_80203138(self) == 2) {
        if (flag != 0)
            return 0x15;
        return 0x17;
    }
    if (flag != 0)
        return 0x14;
    return 0x16;
}
#pragma optimize_for_size off

// Target us-80205790: resolve the currently-equipped item's resource id from
// the active category (box gate open: func_80288530 category; gate closed:
// field_98) and the per-category item id read from the bdat row (obj+off).
// Opt-space merges the callee-saved saves into stmw r30 and copies r3->r30
// before r4->r31 (retail prologue shape; MWCC_CASES kyoshin leaf patterns).
#pragma optimize_for_size on
int func_80203A98(CEquipChange* self, u32 param) {
    if (func_802865A0(&self->mEquipItemBox) != 0) {
        if (param == 0 && func_80288948(&self->mEquipItemBox) == 0)
            return 0;
        u8* obj = (u8*)func_8009EC9C((u8)func_801392B4(self->field_99));
        // signed compare (retail cmpi chain) - category value 2/4/5/6/7/8
        int r0 = (u8)func_80288530(&self->mEquipItemBox);
        void* item;
        switch (r0) {
        case 2: item = func_80157C4C(2, *(s16*)(obj + 0x26)); break;
        case 4: item = func_80157C4C(4, *(s16*)(obj + 0x1c)); break;
        case 5: item = func_80157C4C(5, *(s16*)(obj + 0x1e)); break;
        case 6: item = func_80157C4C(6, *(s16*)(obj + 0x20)); break;
        case 7: item = func_80157C4C(7, *(s16*)(obj + 0x22)); break;
        case 8: item = func_80157C4C(8, *(s16*)(obj + 0x24)); break;
        default: return 0;
        }
        // tail duplicated per branch in retail (.L_80205894 / .L_80205960)
        if (item != NULL && *(u32*)item != 0)
            return *(u32*)item >> 20;
    } else {
        // box gate closed - key off the cursor-run flag field_98
        u8* obj = (u8*)func_8009EC9C((u8)func_801392B4(self->field_99));
        s8 r0 = self->field_98;
        void* item;
        // retail chain tests 0/4/6/8/10/11 (extsb. + cmpwi, all tests first)
        switch (r0) {
        case 0: item = func_80157C4C(2, *(s16*)(obj + 0x26)); break;
        case 4: item = func_80157C4C(4, *(s16*)(obj + 0x1c)); break;
        case 6: item = func_80157C4C(5, *(s16*)(obj + 0x1e)); break;
        case 8: item = func_80157C4C(6, *(s16*)(obj + 0x20)); break;
        case 10: item = func_80157C4C(7, *(s16*)(obj + 0x22)); break;
        case 11: item = func_80157C4C(8, *(s16*)(obj + 0x24)); break;
        default: return 0;
        }
        if (item != NULL && *(u32*)item != 0)
            return *(u32*)item >> 20;
    }
    return 0;
}
#pragma optimize_for_size off

// Target us-80205994: lift the last box byte if the box gate is open.
u8 func_80203C9C(CEquipChange* self) {
    if (func_802865A0(&self->mEquipItemBox) != 0)
        return self->mEquipItemBox.unk_37c;
    return 0;
}

// Target us-802059d8: when the 0x38 anim finishes, reset the three layout
// slot animations via the 0x34 object's vtable[0x2C] hook and enter state 2.
extern "C" void __declspec(noinline) func_80203CE0(CEquipChange* self) {
    if (func_80137444(self->field_38, lbl_eu_806682A8) == 0)
        return;
    ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_40, 0);
    ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_38, 0);
    ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_3C, 1);
    self->field_48 = 2;
}

// Target us-80205a70: when the 0x3C anim finishes, refresh the item info
// window (func_801D4B3C pack), drive the sub-cursor vtable[0x10] hook with the
// packed buffer, close the sub-cursor, and reset state via func_802040FC.
extern "C" void __declspec(noinline) func_80203D78(CEquipChange* self) {
    if (func_80137444(self->field_3C, lbl_eu_806682A8) == 0)
        return;
    self->field_48 = 3;
    self->field_4D = 1;
    char buf[0x10];
    func_801D4B3C(buf, (CItemBoxInfo*)((u8*)self + 0xA4), self->field_99);
    ((CSubCurVtblView*)self->field_80)->v2(buf);
    func_801D216C((void*)((u8*)self + 0x80), 1);
    func_802040FC(self);
}

// Target us-80205af8: when the 0x3C anim finishes, reset the three layout
// slot animations via the 0x34 object's vtable[0x2C] hook and enter state 5.
// Sibling of func_80203CE0 (guard on field_38, state 2).
extern "C" void __declspec(noinline) func_80203E00(CEquipChange* self) {
    if (func_80137510(self->field_3C, lbl_eu_806682A8) == 0)
        return;
    ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_3C, 0);
    ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_40, 0);
    ((CLayoutVtbl11*)(u32)self->field_34)->v9(self->field_38, 1);
    self->field_48 = 5;
}

// Target us-80205b90: when the 0x38 anim finishes, clear state 0x48 and flag 0x4D.
extern "C" void __declspec(noinline) func_80203E98(CEquipChange* self) {
    if (func_80137510(self->field_38, lbl_eu_806682A8) != 0) {
        self->field_4D = 1;
        self->field_48 = 0;
    }
}

// Target us-80205bdc: when the 0x40 anim finishes, repack the equip info
// (func_80287EFC/80287DB4 with the packed (field_99<<8 | cur38<<4 | cur3a)
// word) and re-init the box, entering state 7. Retail saves 4 regs via
// _savegpr_28 (opt-space prologue).
#pragma optimize_for_size on
extern "C" void __declspec(noinline) func_80203EE4(CEquipChange* self) {
    if (func_80137444(self->field_40, lbl_eu_806682A8) == 0)
        return;
    u8 f99 = self->field_99;
    int cur38 = func_80203138(self);
    int cur3a = func_802031A0(self);
    func_80287EFC(&self->mEquipItemBox, (u32)self->_padA4);
    void* a8 = func_802052A8(self);
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    func_80287DB4(&self->mEquipItemBox, packed, a8, 0);
    func_802866E8(&self->mEquipItemBox);
    self->field_48 = 7;
}
#pragma optimize_for_size off

// Target us-80205c7c: when the box sub-page closes, enter state 8.
extern "C" void __declspec(noinline) func_80203F84(CEquipChange* self) {
    if (func_802865A8(&self->mEquipItemBox) != 0) {
        self->field_48 = 8;
        self->field_4D = 1;
    }
}

// Target us-80205cc4: when the box sub-page gate is open, refresh the equip
// info window with the packed selection word and enter state 0xA.
extern "C" void __declspec(noinline) func_80203FCC(CEquipChange* self) {
    if (func_802865A8(&self->mEquipItemBox) == 0)
        return;
    self->field_48 = 0xA;
    u8 f99 = self->field_99;
    u32 r38 = (u32)func_80203138(self);
    u32 r3a = (u32)func_802031A0(self);
    func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4),
                  (u16)(((u32)f99 << 8) | ((r38 & 0xF) << 4) | (r3a & 0xF)),
                  func_802052A8(self), 1);
}

// Target us-80205d44: when the 0x40 anim finishes, enter state 3 and refresh.
extern "C" void __declspec(noinline) func_8020404C(CEquipChange* self) {
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

// Target us-80205df4: refresh the equip-cursor positions. Copies the 14-word
// colour palette from lbl_eu_80508120 (0x38 bytes, 7 pairs), then uses the
// layout sub-object's vtable[0x3C] to fetch three panes: one keyed by the
// palette colour for field_98, two by name. The first pane's accumulated
// translate (pos) is scaled by the other two panes' scale, and the result
// positions either the 0x50 or the 0x68 cursor depending on field_98.
// Opt-space keeps the palette copy as a retail mtctr/bdnz loop (unrolled
// under -O4,p); the block-copy form gives the lwzu/stwu update addressing.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_802040FC(CEquipChange* self) {
    nw4r::math::VEC3 pos;
    CEquipColorPair palette[7];
    // 0x38-byte block copy (7 pairs of 8 bytes) - the block-copy path emits
    // the retail lwzu/stwu counted loop.
    *(CEquipPaletteBlock*)palette = *(const CEquipPaletteBlock*)lbl_eu_80508120;

    // The retail reloads the sub-object pointer (field_34 -> +0x10) at every
    // call site, so keep the access inline (a temp would live in a callee-saved
    // register across the calls).
    CEquipColorPair scale;
    nw4r::lyt::Pane* pane =
        ((CLayoutSubVtbl13*)((CLayoutView*)(u32)self->field_34)->field_10)
            ->v13(((u32*)palette)[self->field_98], 1);
    func_801375A0(&pos, pane);

    pane = ((CLayoutSubVtbl13*)((CLayoutView*)(u32)self->field_34)->field_10)
               ->v13((u32)(lbl_eu_80508168 + 0x3E), 1);
    scale = ((const CPaneScaleView*)pane)->mScale;
    pos.x *= *(const float*)&scale.a;
    pos.y *= *(const float*)&scale.b;

    pane = ((CLayoutSubVtbl13*)((CLayoutView*)(u32)self->field_34)->field_10)
               ->v13((u32)(lbl_eu_80508168 + 0x46), 1);
    *(float*)&scale.a = pane->GetScale().x;
    *(float*)&scale.b = pane->GetScale().y;
    pos.x *= *(const float*)&scale.a;

    if (self->field_98 == 0 || self->field_98 == 4 || self->field_98 == 6 ||
        self->field_98 == 8 || self->field_98 == 10 || self->field_98 == 12) {
        ((CCurVtblView*)self->_pad50)->v2(&pos);
        func_801D216C(self->_pad50, 1);
        func_801D216C(self->_pad68, 0);
    } else {
        ((CCurVtblView*)self->_pad68)->v2(&pos);
        func_801D216C(self->_pad68, 1);
        func_801D216C(self->_pad50, 0);
    }
}
#pragma optimize_for_size off


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
// Target us-802077c8: static init - build the six equip-change colour/state
// palette entries (sdata2) used by the item box UI.
void sinit_802059E8() {
    func_801D1F9C(&lbl_eu_80664668, 0);
    func_801D1F9C(&lbl_eu_80664670, 0);
    func_801C4B60(&lbl_eu_80664678, 0x12, 0xa3, 0xe7, 0);
    func_801C4B60(&lbl_eu_80664680, 0xff, 0xff, 0xff, 0);
    func_801C4B60(&lbl_eu_80664688, 0xb3, 0x9, 0xc0, 0);
    func_801C4B60(&lbl_eu_80664690, 0xff, 0xff, 0xff, 0);
}

// --- function definitions kept at end of TU so earlier callers emit `bl` ----

// Target us-80204bac: apply the equip change for the given category. Packs
// the item-box info (func_801D4B3C), drives the sub-cursor vtable[0x10] hook
// with the packed buffer and activates the sub-cursor, refreshes the item
// info window, then resolves the item under the cursor: category 3 re-equips
// the weapon row (obj+0x26) unless the equipped count is below the selection,
// otherwise it re-equips the sub-cursor row via the 0x806682A0 category table
// and the obj u16 id fields. When the cursor cannot be placed (flag 0) the
// refresh block re-runs. Frame saves 7 regs via stmw r25 (opt-space).
#pragma optimize_for_size on
void __declspec(noinline) func_80202EB4(CEquipChange* self, u8 cat) {
    self->field_99 = cat;
    u8 cats[5];          // 0x08 - category byte block {word, byte}
    s16 buf[0x10];       // 0x10 - item-box info pack
    func_801D4B3C(buf, (CItemBoxInfo*)((u8*)self + 0xA4), cat);
    ((CSubCurVtblView*)self->field_80)->v2(buf);
    func_801D2174((CBaseCur*)self->field_80);
    func_802042C0(self);
    func_801D4260((CItemBoxInfo*)((u8*)self + 0xA4), (u8)func_80203138(self));
    u8 f99 = self->field_99;
    int cur38 = func_80203138(self);
    int cur3a = func_802031A0(self);
    if ((u8)cur38 == 3) {
        int flag = 0;
        u8* obj = (u8*)func_8009EC9C((u8)func_801392B4(self->field_99));
        if ((u8)cur3a <= 3) {
            s16 id = *(s16*)(obj + 0x26);
            if (id == -1) {
                self->field_98 = 0;
            } else {
                void* item = func_80157C4C(2, id);
                if (item != NULL && *(u32*)item != 0) {
                    void* inst = CItem_initItemImplInstances(item);
                    u8 equipped = ((CItemImplVtblView*)inst)->v10(item);
                    if (equipped < (u8)cur3a)
                        self->field_98 = (s8)equipped;
                    else
                        flag = 1;
                } else {
                    self->field_98 = 0;
                }
            }
        } else {
            int idx = (u8)cur3a - 4;
            s16 v1 = *(s16*)(obj + 0x1c);
            s16 v2 = *(s16*)(obj + 0x1e);
            s16 v3 = *(s16*)(obj + 0x20);
            s16 v4 = *(s16*)(obj + 0x22);
            s16 v5 = *(s16*)(obj + 0x24);
            u32 word = *(u32*)&lbl_eu_806682A0[2 * idx];
            u8 byte = lbl_eu_806682A4[2 * idx];
            buf[6] = v1;
            buf[7] = v2;
            buf[8] = v3;
            buf[9] = v4;
            buf[10] = v5;
            s16 id = buf[6 + idx];
            *(u32*)&cats[0] = word;
            cats[4] = byte;
            if (id != -1) {
                void* item = func_80157C4C(cats[idx], id);
                if (item != NULL && *(u32*)item != 0) {
                    void* inst = CItem_initItemImplInstances(item);
                    if (((CItemImplVtblView*)inst)->v10(item) != 0)
                        flag = 1;
                }
            }
            if (flag == 0)
                self->field_98 = (s8)(2 * idx + 4);
        }
        if (flag == 0) {
            func_802042C0(self);
            func_801D4260((CItemBoxInfo*)((u8*)self + 0xA4), (u8)func_80203138(self));
            func_802040FC(self);
            f99 = self->field_99;
            cur38 = func_80203138(self);
            cur3a = func_802031A0(self);
        }
    }
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4), packed, func_802052A8(self), 1);
    if (func_802865A0(&self->mEquipItemBox) != 0)
        func_80287DB4(&self->mEquipItemBox, packed, 0, 1);
}
#pragma optimize_for_size off

// placeholder body for func_802052A8 (target us-80207088); real body below
extern "C" __declspec(noinline) void* func_802052A8(CEquipChange* self) { return (void*)self->field_30; }

// Target us-80204e30: field_98 (s8) maps 0/4/6/8/10/12 -> 2/4/5/6/7/8,
// anything else -> 3. Free function (retail symbol is unmangled, hence C
// linkage).
// Retail emits external `bl` calls to this helper at every call site, so
// __declspec(noinline) keeps the body standalone under -inline auto / -ipa.
extern "C" __declspec(noinline) int func_80203138(CEquipChange* self) {
    signed char value = self->field_98;
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
// entries 7..12 route to default 0). Same noinline rationale as func_80203138.
extern "C" __declspec(noinline) int func_802031A0(CEquipChange* self) {
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

// Target us-80203e08: per-frame equip-change update. While visible (0x44),
// dispatch on state 0x48 (jump table 0..10) to the layout-anim waiters, then
// drive the layout vtable[0x38] hook and update the cursors / item info
// window / equip item box.
// The state handlers are matched targets defined above; their extern "C" +
// noinline definitions keep -ipa file from folding their bodies into the
// switch and make the call relocs use the unmangled retail names.
void func_80202110(CEquipChange* self) {
    if (self->field_44 == 0)
        return;
    switch (self->field_48) {
    case 0: func_80203CE0(self); break;
    case 1: func_80203D78(self); break;
    case 2: func_80203E00(self); break;
    case 3: func_80203E98(self); break;
    case 4: func_80203EE4(self); break;
    case 5: func_80203F84(self); break;
    case 6: func_80203FCC(self); break;
    case 7: func_8020404C(self); break;
    default:
    case 8:
    case 9:
    case 10:
        break;
    }
    ((CLayoutVtbl11*)(u32)self->field_34)->v12(0);
    func_801D202C((void*)((u8*)self + 0x50));
    func_801D202C((void*)((u8*)self + 0x68));
    func_801D202C((void*)self->field_80);
    func_801D40C4((CItemBoxInfo*)((u8*)self + 0xA4));
    func_80286264(&self->mEquipItemBox);
}
// Target us-80203edc: draw the equip-change screen. Draws the main layout
// (field_34) when visible (0x44), then the two cursors (0x50/0x68) only when
// the sub-cursor (0x80) is idle, flags the item box (0x62B), and finally the
// item info window + sub-cursor + item box. Retail frame uses stmw/lmw.
#pragma optimize_for_size on
extern "C" void func_802021E4(CEquipChange* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_44 == 0)
        return;
    func_80137038((nw4r::lyt::Layout*)self->field_34, drawInfo, 0, 1);
    if (func_801D2ED8((CBaseCur*)self->field_80) == 0) {
        func_801D20B0((CBaseCur*)self->_pad50, drawInfo);
        func_801D20B0((CBaseCur*)self->_pad68, drawInfo);
        self->mEquipItemBox.unk_37b = 1;
    } else {
        self->mEquipItemBox.unk_37b = 0;
    }
    func_801D4154((CItemBoxInfo*)self->_padA4, drawInfo);
    func_801D20B0((CBaseCur*)self->field_80, drawInfo);
    func_80286340(&self->mEquipItemBox, drawInfo);
}
#pragma optimize_for_size off
// Target us-80203f84: teardown of the equip-change screen. Release the two
// file handles, clear the visible flag, delete the layout object at 0x34
// (guarded double-check), clear the global page flag, release both arc
// accessors and the 0x04 memory region, reset the three cursors, and close
// the item info window + box.
void func_8020228C(CEquipChange* self) {
    func_801390E0__FPP11CFileHandle(&self->field_24);
    func_801390E0__FPP11CFileHandle(&self->field_28);
    self->field_44 = 0;
    if (self->field_34 != 0) {
        if (self->field_34 != 0)
            ((CLayoutVtbl11*)(u32)self->field_34)->v0(1);
        self->field_34 = 0;
    }
    lbl_eu_80664698 = 0;
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(self->field_2C);
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor((void*)self->field_30);
    func_8045F778__17UnkClass_8045F564Fv(&self->_pad04[0]);
    ((CCurVtblView*)((u8*)self + 0x50))->v1();
    ((CCurVtblView*)((u8*)self + 0x68))->v1();
    ((CCurVtblView*)self->field_80)->v1();
    func_801D4174((CItemBoxInfo*)((u8*)self + 0xA4));
    func_80286454(&self->mEquipItemBox);
}
