// Auto-scaffolded catalog TU for kyoshin/CEquipChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CEquipChange.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/util/MemManager.hpp"
#include <stdio.h>

// const routes the symbol into the readonly sdata2 pool; MWCC then hoists the
// lfs ABOVE the frame stores (retail shape) - see MWCC_CASES §CExchangeWin.
extern const float lbl_eu_806682A8;

extern "C" void playUISound__FUl(u32 op);
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

// Target us-8020377c: constructor. Seeds the manual vtable, constructs the
// two UnkClass_8045F564 mem regions, zeroes the scalar head, builds the three
// cursors, then copy-constructs the CItemBoxInfo / CEquipItemBox members from
// fresh stack temporaries (CItemBoxInfo(4,0) and default CEquipItemBox) whose
// config regions are copied field-by-field through the same helpers as
// func_801F9CB4 (UnkClass_8011C974 mem regions, scalars load-store
// interleaved, counted 8-byte pair loops).
#pragma optimize_for_size on
/* Retail's symbol is the unmangled free function __ct__CEquipChange (not a
   mangled member ctor), so it is written with C linkage - no implicit
   member construction is injected and __ct__CEquipItemBox is called
   manually mid-body, exactly as retail does. */
extern "C" CEquipChange* __ct__CEquipChange(CEquipChange* self) {
    self->mVtbl = reinterpret_cast<void*>(lbl_eu_80535688);
    __ct__17UnkClass_8045F564Fv(&self->_pad04[0]);
    __ct__17UnkClass_8045F564Fv(&self->_pad14[0]);
    self->field_24 = 0;
    self->field_28 = 0;
    self->field_2C = 0;
    self->field_30 = 0;
    self->field_34 = 0;
    self->field_38 = 0;
    self->field_3C = 0;
    self->field_40 = 0;
    self->field_44 = 0;
    self->field_48 = 0;
    self->field_4C = 0;
    self->field_4D = 1;
    __ct__CCur14((u8*)self + 0x50, (nw4r::lyt::ArcResourceAccessor*)0);
    __ct__CCur15((u8*)self + 0x68, (nw4r::lyt::ArcResourceAccessor*)0);
    __ct__CSubCur((u8*)self + 0x80, (nw4r::lyt::ArcResourceAccessor*)0);
    self->field_98 = 0;
    self->field_99 = 0;
    __ct__CItemBoxInfo(reinterpret_cast<CItemBoxInfo*>(&self->_padA4[0]), 0, 0);
    __ct__CEquipItemBox(&self->mEquipItemBox);

    CEqChStateView* v = reinterpret_cast<CEqChStateView*>(self);

    CEqChBoxTemp boxTmp;

    // --- copy config out of a fresh CItemBoxInfo(4,0) temp ---
    __ct__CItemBoxInfo(reinterpret_cast<CItemBoxInfo*>(&boxTmp), 4, 0);
    __ct__UnkClass_8011C974(&v->fa8[0], &boxTmp.f04[0]);
    __ct__UnkClass_8011C974(&v->fb8[0], &boxTmp.f14[0]);
    v->fc8[0] = boxTmp.f24[0];
    v->fc8[1] = boxTmp.f24[1];
    v->fc8[2] = boxTmp.f24[2];
    v->fc8[3] = boxTmp.f24[3];
    v->fc8[4] = boxTmp.f24[4];
    v->fc8[5] = boxTmp.f24[5];
    v->fc8[6] = boxTmp.f24[6];
    v->fc8[7] = boxTmp.f24[7];
    v->fc8[8] = boxTmp.f24[8];
    v->fc8[9] = boxTmp.f24[9];
    v->fc8[10] = boxTmp.f24[10];
    v->fc8[11] = boxTmp.f24[11];
    v->fc8[12] = boxTmp.f24[12];
    v->fc8[13] = boxTmp.f24[13];
    v->fc8[14] = boxTmp.f24[14];
    v->fc8[15] = boxTmp.f24[15];
    v->fc8[16] = boxTmp.f24[16];
    v->fc8[17] = boxTmp.f24[17];
    v->fc8[18] = boxTmp.f24[18];
    v->fc8[19] = boxTmp.f24[19];
    v->fc8[20] = boxTmp.f24[20];
    v->fc8[21] = boxTmp.f24[21];
    v->fc8[22] = boxTmp.f24[22];
    v->fc8[23] = boxTmp.f24[23];
    v->fc8[24] = boxTmp.f24[24];
    v->fc8[25] = boxTmp.f24[25];
    v->fc8[26] = boxTmp.f24[26];
    v->f134 = boxTmp.f134;
    v->f138 = boxTmp.f138;
    v->f13c = boxTmp.f13c;
    v->f13d = boxTmp.f13d;
    v->f13e = boxTmp.f13e;
    u32 vHi = boxTmp.f144;
    u32 vLo = boxTmp.f140;
    v->f144 = vHi;
    v->f140 = vLo;
    vHi = boxTmp.f14c;
    vLo = boxTmp.f148;
    v->f14c = vHi;
    v->f148 = vLo;
    v->f150 = boxTmp.f150;
    v->f152 = boxTmp.f152;
    func_8018BE74(&v->f154[0], &boxTmp.f154[0]);
    __dt__12CItemBoxInfoFv(reinterpret_cast<CItemBoxInfo*>(&boxTmp), -1);

    // --- copy config out of a fresh default CEquipItemBox temp ---
    CEqChEquipTemp eqTmp;
    __ct__CEquipItemBox(&eqTmp);
    __ct__UnkClass_8011C974(&v->f2b4[0], &eqTmp.f04[0]);
    __ct__UnkClass_8011C974(&v->f2c4[0], &eqTmp.f14[0]);
    v->f2d4[0] = eqTmp.f24[0];
    v->f2d4[1] = eqTmp.f24[1];
    v->f2d4[2] = eqTmp.f24[2];
    v->f2d4[3] = eqTmp.f24[3];
    v->f2d4[4] = eqTmp.f24[4];
    v->f2d4[5] = eqTmp.f24[5];
    v->f2d4[6] = eqTmp.f24[6];
    v->f2f0[0] = eqTmp.f2f0[0];
    v->f2f0[1] = eqTmp.f2f0[1];
    v->f2f0[2] = eqTmp.f2f0[2];
    v->f2f0[3] = eqTmp.f2f0[3];
    func_801FA220(&v->f2f4[0], &eqTmp.f2f4[0]);
    func_801FA220(&v->f30c[0], &eqTmp.f30c[0]);
    func_8018B0FC(&v->f324[0], &eqTmp.f324[0]);
    __ct__UnkClass_8011C974(&v->f340[0], &eqTmp.f340[0]);
    v->f350[0] = eqTmp.f350[0];
    v->f350[1] = eqTmp.f350[1];
    v->f350[2] = eqTmp.f350[2];
    v->f350[3] = eqTmp.f350[3];
    v->f350[4] = eqTmp.f350[4];
    v->f364[0] = eqTmp.f364[0];
    v->f364[1] = eqTmp.f364[1];
    v->f364[2] = eqTmp.f364[2];
    v->f364[3] = eqTmp.f364[3];
    __ct__UnkClass_8011C974(&v->f36c[0], &eqTmp.f36c[0]);
    v->f37c[0] = eqTmp.f37c[0];
    v->f37c[1] = eqTmp.f37c[1];
    v->f37c[2] = eqTmp.f37c[2];
    v->f37c[3] = eqTmp.f37c[3];
    v->f38c[0] = eqTmp.f38c[0];
    v->f38c[1] = eqTmp.f38c[1];
    v->f38c[2] = eqTmp.f38c[2];
    v->f38c[3] = eqTmp.f38c[3];
    v->f390[0] = eqTmp.f390[0];
    v->f390[1] = eqTmp.f390[1];
    v->f390[2] = eqTmp.f390[2];
    v->f390[3] = eqTmp.f390[3];
    v->f390[4] = eqTmp.f390[4];
    v->f3a4 = eqTmp.f3a4;
    const u32* ws = reinterpret_cast<const u32*>(&eqTmp.f3a4);
    u32* wd = reinterpret_cast<u32*>(&v->f3a4);
    for (int i = 0; i < 16; i++, ws += 2, wd += 2) {
        wd[1] = ws[1];
        wd[2] = ws[2];
    }
    v->f428[0] = eqTmp.f428[0];
    v->f428[1] = eqTmp.f428[1];
    v->f428[2] = eqTmp.f428[2];
    func_8016742C(&v->f42c[0], &eqTmp.f42c[0]);
    func_8016742C(&v->f468[0], &eqTmp.f468[0]);
    v->f4a4[0] = eqTmp.f4a4[0];
    v->f4a4[1] = eqTmp.f4a4[1];
    v->f4a4[2] = eqTmp.f4a4[2];
    v->f4a8 = eqTmp.f4a8;
    v->f4aa = eqTmp.f4aa;
    v->f4ac = eqTmp.f4ac;
    v->f4ae = eqTmp.f4ae;
    u32 vHi2 = eqTmp.f4b4;
    u32 vLo2 = eqTmp.f4b0;
    v->f4b4 = vHi2;
    v->f4b0 = vLo2;
    v->f4b8 = eqTmp.f4b8;
    v->f4bc = eqTmp.f4bc;
    func_8018BE74(&v->f4c0[0], &eqTmp.f4c0[0]);
    v->f61c = eqTmp.f61c;
    v->f620 = eqTmp.f620;
    v->f622[0] = eqTmp.f622[0];
    v->f622[1] = eqTmp.f622[1];
    v->f622[2] = eqTmp.f622[2];
    v->f622[3] = eqTmp.f622[3];
    v->f622[4] = eqTmp.f622[4];
    v->f622[5] = eqTmp.f622[5];
    v->f622[6] = eqTmp.f622[6];
    v->f622[7] = eqTmp.f622[7];
    v->f622[8] = eqTmp.f622[8];
    v->f622[9] = eqTmp.f622[9];
    v->f622[10] = eqTmp.f622[10];
    ws = reinterpret_cast<const u32*>(&eqTmp.f622[8]);
    wd = reinterpret_cast<u32*>(&v->f622[8]);   // base 0x62a
    for (int i = 0; i < 0x400; i++, ws += 2, wd += 2) {
        wd[1] = ws[1];
        wd[2] = ws[2];
    }
    v->f262e = eqTmp.f262e;
    v->f2630[0] = eqTmp.f2630[0];
    v->f2630[1] = eqTmp.f2630[1];
    v->f2630[2] = eqTmp.f2630[2];
    v->f2630[3] = eqTmp.f2630[3];
    ws = reinterpret_cast<const u32*>(&eqTmp.f2630[0]);
    wd = reinterpret_cast<u32*>(&v->f2630[0]);  // base 0x2630
    for (int i = 0; i < 4; i++, ws += 2, wd += 2) {
        wd[1] = ws[1];
        wd[2] = ws[2];
    }
    ws = reinterpret_cast<const u32*>(&eqTmp.f2650[0]);
    wd = reinterpret_cast<u32*>(&v->f2650[0]);  // base 0x2650
    for (int i = 0; i < 0x80; i++, ws += 2, wd += 2) {
        wd[1] = ws[1];
        wd[2] = ws[2];
    }
    v->f2a54 = eqTmp.f27a4;
    __dt__13CEquipItemBoxFv(reinterpret_cast<CEquipItemBox*>(&eqTmp), -1);
    return self;
}
#pragma optimize_for_size off

// Target us-80203d88: load both bind files (two string records at
// lbl_eu_80508168) into file handles 0x24/0x28, then init the item-box info
// layout (0xA4) and the equip item box (0x2B0).
// Retail frame is the optimize_for_size stmw/lmw pair.
#pragma optimize_for_size on
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
#pragma optimize_for_size off

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
// the 0x6D sound. Retail recomputes self+0xA4 per call site (no CSE) and
// packs the nibbles low-to-high starting from the func_802031A0 result.
#pragma optimize_for_size on
void func_802024CC(CEquipChange* self) {
    if (self->field_48 == 0) {
        self->field_48 = 1;
        self->field_4D = 0;
        func_801D421C((CItemBoxInfo*)((u8*)self + 0xA4));
        func_801D4260((CItemBoxInfo*)((u8*)self + 0xA4), (u8)func_80203138(self));
        u8 f99 = self->field_99;
        u32 cur38 = (u32)func_80203138(self);
        u32 cur3a = (u32)func_802031A0(self);
        func_801D47D4((CItemBoxInfo*)self->_padA4,
                      (u16)(((u32)f99 << 8) | ((cur38 & 0xF) << 4) | (cur3a & 0xF)),
                      func_802052A8(self), 1);
        playUISound__FUl(0x6d);
    }
}

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
        playUISound__FUl(6);
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
        return;
    }
    // Mirrors the fully-matched forward sibling func_80202790: temp-based
    // pre-wrap, one probe outside the rotated loop guarding entry, and a
    // plain decrement at the loop tail.
    u8 v = (u8)(self->field_98 - 1);
    self->field_98 = (s8)v;
    if ((s8)v < 0)
        self->field_98 = 13;
    u8 idx = (u8)func_802031A0(self);
    if (idx != 0 && ((u8*)self)[0x99 + idx] == 0) {
        self->field_98 = (s8)(self->field_98 - 1);
        while (true) {
            if (self->field_98 < 0)
                self->field_98 = 13;
            idx = (u8)func_802031A0(self);
            if (idx == 0)
                break;
            if (((u8*)self)[0x99 + idx] != 0)
                break;
            self->field_98 = (s8)(self->field_98 - 1);
        }
    }
    func_802040FC(self);
    u8 f99 = self->field_99;
    u32 cur38 = (u32)func_80203138(self);
    u32 cur3a = (u32)func_802031A0(self);
    void* a8 = func_802052A8(self);
    // Two-step pack (same shape as matched func_80202790): the shifted term
    // first makes MWCC fold the unshifted cur3a into the clrlwi base.
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4), packed, a8, 1);
    int v38 = func_80203138(self);
    func_801D4260((CItemBoxInfo*)self->_padA4, (u8)v38);
    playUISound__FUl(1);
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
    playUISound__FUl(1);
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
    playUISound__FUl(1);
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
    playUISound__FUl(1);
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
            playUISound__FUl(5);
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
        playUISound__FUl(5);
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
    playUISound__FUl(3);
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
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
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
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
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
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
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
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
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
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
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
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
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
                func_800BFDE0(getCfObjectPc__FPQ22cf12CfObjectMove(slot->field_0x4), 0);
            }
        }
        __dt__80043E88(&holder, -1);
    }
    playUISound__FUl(0x76);
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
        playUISound__FUl(5);
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
        v = ((int)(u8)v - 3 > 0) ? 2 : 0;
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
    // Lifetimes mirror retail: f99 (r28) and cur38 (r29) feed rlwimi inserts
    // into the packed word based on the fresh func_802031A0 low nibble.
    u8 f99 = self->field_99;
    int cur38 = func_80203138(self);
    u32 packed = (u32)func_802031A0(self) & 0xF;
    packed |= ((u32)(cur38 & 0xF) << 4);
    packed |= ((u32)f99 << 8);
    func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4), (u16)packed, func_802052A8(self), 1);
    if (func_802865A0(&self->mEquipItemBox) != 0)
        func_80287DB4(&self->mEquipItemBox, (u16)packed, 0, 1);
    playUISound__FUl(0x77);
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
    if (advanceAnimTransform(self->field_38, lbl_eu_806682A8) == 0)
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
    if (advanceAnimTransform(self->field_3C, lbl_eu_806682A8) == 0)
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
    if (advanceAnimTransform(self->field_40, lbl_eu_806682A8) == 0)
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
// -O4,s keeps the retail stmw r28 frame; the packed word is built low-to-high
// ((sub&0xF) | (main&0xF)<<4 | f99<<8) so MWCC merges the ORs into rlwimi.
#pragma optimize_for_size on
extern "C" void __declspec(noinline) func_80203FCC(CEquipChange* self) {
    if (func_802865A8(&self->mEquipItemBox) == 0)
        return;
    self->field_48 = 0xA;
    u8 f99 = self->field_99;
    int lo = func_80203138(self);
    int hi = func_802031A0(self);
    int packed = (hi & 0xF) | ((lo & 0xF) << 4);
    int m = (int)func_802052A8(self);
    func_801D47D4((CItemBoxInfo*)((u8*)self + 0xA4),
                  (u16)(packed | ((u32)f99 << 8)),
                  (void*)m, 1);}
#pragma optimize_for_size off

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
// noinline keeps -ipa from folding this into OnFileEvent's two branches
// (retail emits external bl calls).
extern "C" __declspec(noinline) void func_802040A0(CEquipChange* self) {
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


// Target us-80205fb8: full refresh of the equip-change screen. Reads the
// current character's equipped rows out of the bdat character data, resolves
// each category row to an item through func_80157C4C, then walks the layout
// panes: the three weapon-slot panes (loop 1) show either the directly
// equipped item (impl v14 slot lookup), a bdat-rebuilt entry (impl v9 packed
// record), or an empty label; the five armour-category panes (loop 2) mirror
// the same three-way split using the category items cached up front. Every
// processed entry is appended to the cursor record array handed to
// func_80205294 so the cursors land on real entries only.
#pragma optimize_for_size on
void func_80205294(void* dst, void* src);
extern "C" void func_802042C0(CEquipChange* self) {
    CEqChCursorRec recs[8];
    // retail zeroes these with straight-line stores, not a loop
    recs[0].field_00 = 0;
    recs[1].field_00 = 0;
    recs[2].field_00 = 0;
    recs[3].field_00 = 0;
    recs[4].field_00 = 0;
    recs[5].field_00 = 0;
    recs[6].field_00 = 0;
    recs[7].field_00 = 0;
    recs[0].field_04 = 0;
    recs[1].field_04 = 0;
    recs[2].field_04 = 0;
    recs[3].field_04 = 0;
    recs[4].field_04 = 0;
    recs[5].field_04 = 0;
    recs[6].field_04 = 0;
    recs[7].field_04 = 0;
    CEqChCursorRec tmp;
    char textBuf1[0x20];    // sp+0x128
    char iconName1[0x20];   // sp+0x1a8
    char labelName1[0x20];  // sp+0x188
    char imageName1[0x20];  // sp+0x168
    char paneName1[0x20];   // sp+0x148
    char textBuf2[0x20];    // sp+0x48
    char iconName2[0x20];   // sp+0x108
    char labelName2[0x20];  // sp+0xe8
    char paneName2[0x20];   // sp+0xc8
    char labelName3[0x20];  // sp+0xa8
    char imageName2[0x20];  // sp+0x88
    char paneName3[0x20];   // sp+0x68
    char rebuildName[0x20]; // sp+0x28
    u8* marks = self->_pad9A;
    int count = 0;
    int nRecs = 0;

    CBdatCharData* cd = (CBdatCharData*)func_8009EC9C(func_801392B4(self->field_99));
    func_800A13C4(cd, 1);
    // stats sub-object at +0x17C: refresh call through its vtable word 0x89.
    void** statsVt = *(void***)((u8*)cd + 0x17C);
    ((void (*)(void*))statsVt[0x89])((u8*)cd + 0x17C);
    func_8009D7E4(&cd->field_1C, 5);

    s16 w4 = cd->field_1C;
    s16 w5 = cd->field_1E;
    s16 w6 = cd->field_20;
    s16 w7 = cd->field_22;
    s16 w8 = cd->field_24;

    void* weaponItem = func_80157C4C(2, cd->field_26);
    void* catItems[5];
    catItems[0] = func_80157C4C(4, w4);
    catItems[1] = func_80157C4C(5, w5);
    catItems[2] = func_80157C4C(6, w6);
    catItems[3] = func_80157C4C(7, w7);
    catItems[4] = func_80157C4C(8, w8);

    char* base = lbl_eu_80508168;
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)self->field_34;
    func_80136B4C(layout, &base[0x55], &base[0x5f], 0);
    func_80136B4C(layout, &base[0x60], &base[0x5f], 0);
    func_80136B4C(layout, &base[0x6a], &base[0x5f], 0);
    func_80136B4C(layout, &base[0x74], &base[0x5f], 0);
    func_80136B4C(layout, &base[0x7e], &base[0x5f], 0);
    func_80136B4C(layout, &base[0x88], &base[0x5f], 0);

    if (weaponItem != NULL) {
        func_80136B4C(layout, &base[0x92],
                      ((CItemImplVtblView*)CItem_initItemImplInstances(weaponItem))->v6(weaponItem), 0);
        func_80136B4C(layout, &base[0x9f],
                      ((CItemImplVtblView*)CItem_initItemImplInstances(weaponItem))->v6(weaponItem), 0);
        u8 equipped = ((CItemImplVtblView*)CItem_initItemImplInstances(weaponItem))->v10(weaponItem);
        if (equipped != 0) {
            func_80136B4C(layout, &base[0x55], func_eu_802B148C(), 0);
            func_80139A18(layout, &base[0x55], &lbl_eu_80664678, &lbl_eu_80664680);
        }

        for (int i = 0; i < 3; i++) {
            void* texRes;
            sprintf(iconName1, &base[0xae], i);
            sprintf(labelName1, &base[0xbf], i);
            sprintf(imageName1, &base[0xd2], i);
            sprintf(paneName1, &base[0xdf], i);
            nw4r::lyt::Pane* pane =
                ((CLayoutSubVtbl13*)((CLayoutView*)self->field_34)->field_10)->v13((u32)paneName1, 1);
            func_80124270(pane, 1);
            texRes = ((CEqChTexVtbl*)*(void**)self->field_2C)->fn[3](
                (u32)self->field_2C, 0x74696D67, &base[0xec], 0);
            if ((u8)i < equipped) {
                int slot = (s16)((CItemImplVtblView*)CItem_initItemImplInstances(weaponItem))
                               ->v14(weaponItem, (u8)i);
                if (slot == -1) {
                    // bdat-rebuild path: packed equip record from impl v9
                    CEquipV9Result* res =
                        (CEquipV9Result*)((CItemImplVtblView*)CItem_initItemImplInstances(weaponItem))
                            ->v9(weaponItem, 0);
                    if (res != NULL && (res->field_04 & 1)) {
                        u32 packed = res->field_00;
                        marks[count++] = 2;
                        tmp.field_00 = (u16)((res->field_04 >> 16) & 0xFFF);
                        tmp.field_04 = (u32)(s16)((packed >> 11) & 0x7FF);
                        void** arrPtr = &lbl_eu_806640D8_arr[count - 1];
                        void* tbl = *arrPtr;
                        func_80205294(&recs[nRecs++], &tmp);
                        char* lbl = func_80136190(
                            &base[0xff], &base[0x108], 0x1E - (((packed >> 22) & 7) - 1));
                        char* nm = func_8013639C(tbl, &base[0x108], tmp.field_00);
                        sprintf(textBuf1, &base[0x10d], nm, lbl);
                        u32 code = func_801361E8((u32)tbl, &base[0x112], tmp.field_00);
                        switch (code) {
                        case 0:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x11b], 0);
                            break;
                        case 4:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x131], 0);
                            break;
                        case 5:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x147], 0);
                            break;
                        case 6:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x15d], 0);
                            break;
                        case 7:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x173], 0);
                            break;
                        case 8:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x189], 0);
                            break;
                        case 9:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x19f], 0);
                            break;
                        default:
                            break;
                        }
                        func_80136B4C(layout, &base[0x55], func_eu_802B1474(), 0);
                        func_80139A18(layout, &base[0x55], &lbl_eu_80664688, &lbl_eu_80664690);
                    } else {
                        marks[count++] = 1;
                        char* lbl = func_80136190(&base[0xff], &base[0x108], 0x2a);
                        sprintf(textBuf1, &base[0x1b5], lbl);
                    }
                } else {
                    void* item = func_80157C4C(3, (s16)slot);
                    if (item != NULL) {
                        marks[count++] = 3;
                        tmp.field_04 = (u32)((CItemImplVtblView*)CItem_initItemImplInstances(item))->v34(item);
                        tmp.field_00 = func_80139358(*(u32*)item >> 20);
                        func_80205294(&recs[nRecs++], &tmp);
                        u8 cat = ((CItemImplVtblView*)CItem_initItemImplInstances(item))->v0(item);
                        char* lbl = func_80136190(&base[0xff], &base[0x108], 0x1E - (cat - 1));
                        sprintf(textBuf1, &base[0x10d],
                                ((CItemImplVtblView*)CItem_initItemImplInstances(item))->v6(item), lbl);
                        u32 iconRow =
                            ((CItemImplVtblView*)CItem_initItemImplInstances(item))->v19(item);
                        void* tbl = lbl_eu_806640D8_arr[cat - 1];
                        u32 code =
                            func_801361E8((u32)tbl, &base[0x112], func_80139358(iconRow & 0xFFFF));
                        switch (code) {
                        case 0:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x11b], 0);
                            break;
                        case 4:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x131], 0);
                            break;
                        case 5:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x147], 0);
                            break;
                        case 6:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x15d], 0);
                            break;
                        case 7:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x173], 0);
                            break;
                        case 8:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x189], 0);
                            break;
                        case 9:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x19f], 0);
                            break;
                        default:
                            break;
                        }
                    }
                }
            } else {
                marks[count++] = 0;
                char* lbl = func_80136190(&base[0xff], &base[0x108], 0x6d);
                sprintf(textBuf1, &base[0x1b5], lbl);
                func_80124270(pane, 0);
            }
            func_80136A1C(layout, iconName1, textBuf1, 0);
            func_80136A1C(layout, labelName1, textBuf1, 0);
            if (texRes != NULL)
                func_80137E7C(layout, imageName1, (u32)texRes);
        }
    }

    // armour categories: five panes, same three-way entry split
    for (int i = 0; i < 5; i++) {
        void* texRes;
        sprintf(iconName2, &base[0x1b8], i + 1);
        sprintf(labelName2, &base[0x1c7], i + 1);
        sprintf(paneName2, &base[0xae], i + 3);
        sprintf(labelName3, &base[0xbf], i + 3);
        sprintf(imageName2, &base[0xd2], i + 3);
        sprintf(paneName3, &base[0xdf], i + 3);
        nw4r::lyt::Pane* pane =
            ((CLayoutSubVtbl13*)((CLayoutView*)self->field_34)->field_10)->v13((u32)paneName3, 1);
        func_80124270(pane, 1);
        texRes = ((CEqChTexVtbl*)*(void**)self->field_2C)->fn[3](
            (u32)self->field_2C, 0x74696D67, &base[0xec], 0);
        void* item = catItems[i];
        if (item == NULL) {
            char* lbl = func_80136190(&base[0xff], &base[0x108], 0x6d);
            func_80136B4C(layout, iconName2, lbl, 0);
            func_80136B4C(layout, labelName2, lbl, 0);
            marks[count++] = 0;
            sprintf(textBuf2, &base[0x1b5], lbl);
            func_80124270(pane, 0);
        } else {
            func_80136B4C(layout, iconName2,
                          ((CItemImplVtblView*)CItem_initItemImplInstances(item))->v6(item), 0);
            func_80136B4C(layout, labelName2,
                          ((CItemImplVtblView*)CItem_initItemImplInstances(item))->v6(item), 0);
            if (((CItemImplVtblView*)CItem_initItemImplInstances(item))->v10(item) != 0) {
                sprintf(rebuildName, &base[0x1d8], i + 1);
                func_80136B4C(layout, rebuildName, func_eu_802B148C(), 0);
                func_80139A18(layout, rebuildName, &lbl_eu_80664678, &lbl_eu_80664680);
                int slot =
                    (s16)((CItemImplVtblView*)CItem_initItemImplInstances(item))->v14(item, 0);
                if (slot == -1) {
                    CEquipV9Result* res =
                        (CEquipV9Result*)((CItemImplVtblView*)CItem_initItemImplInstances(item))
                            ->v9(item, 0);
                    if (res != NULL && (res->field_04 & 1)) {
                        u32 packed = res->field_00;
                        marks[count++] = 2;
                        tmp.field_00 = (u16)((res->field_04 >> 16) & 0xFFF);
                        tmp.field_04 = (u32)(s16)((packed >> 11) & 0x7FF);
                        void* tbl = lbl_eu_806640D8_arr[count - 1];
                        func_80205294(&recs[nRecs++], &tmp);
                        char* lbl = func_80136190(
                            &base[0xff], &base[0x108], 0x1E - (((packed >> 22) & 7) - 1));
                        char* nm = func_8013639C(tbl, &base[0x108], tmp.field_00);
                        sprintf(textBuf2, &base[0x10d], nm, lbl);
                        u32 code = func_801361E8((u32)tbl, &base[0x112], tmp.field_00);
                        switch (code) {
                        case 0:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x11b], 0);
                            break;
                        case 4:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x131], 0);
                            break;
                        case 5:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x147], 0);
                            break;
                        case 6:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x15d], 0);
                            break;
                        case 7:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x173], 0);
                            break;
                        case 8:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x189], 0);
                            break;
                        case 9:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x19f], 0);
                            break;
                        default:
                            break;
                        }
                        func_80136B4C(layout, rebuildName, func_eu_802B1474(), 0);
                        func_80139A18(layout, rebuildName, &lbl_eu_80664688, &lbl_eu_80664690);
                    } else {
                        marks[count++] = 1;
                        char* lbl = func_80136190(&base[0xff], &base[0x108], 0x2a);
                        sprintf(textBuf2, &base[0x1b5], lbl);
                    }
                } else {
                    void* item2 = func_80157C4C(3, (s16)slot);
                    if (item2 != NULL) {
                        marks[count++] = 3;
                        tmp.field_04 =
                            (u32)((CItemImplVtblView*)CItem_initItemImplInstances(item2))->v34(item2);
                        tmp.field_00 = func_80139358(*(u32*)item2 >> 20);
                        func_80205294(&recs[nRecs++], &tmp);
                        u8 cat = ((CItemImplVtblView*)CItem_initItemImplInstances(item2))->v0(item2);
                        char* lbl = func_80136190(&base[0xff], &base[0x108], 0x1E - (cat - 1));
                        sprintf(textBuf2, &base[0x10d],
                                ((CItemImplVtblView*)CItem_initItemImplInstances(item2))->v6(item2),
                                lbl);
                        u32 iconRow =
                            ((CItemImplVtblView*)CItem_initItemImplInstances(item2))->v19(item2);
                        void* tbl = lbl_eu_806640D8_arr[cat - 1];
                        u32 code =
                            func_801361E8((u32)tbl, &base[0x112], func_80139358(iconRow & 0xFFFF));
                        switch (code) {
                        case 0:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x11b], 0);
                            break;
                        case 4:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x131], 0);
                            break;
                        case 5:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x147], 0);
                            break;
                        case 6:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x15d], 0);
                            break;
                        case 7:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x173], 0);
                            break;
                        case 8:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x189], 0);
                            break;
                        case 9:
                            texRes = ((CEqChTexVtbl*)*(void**)self->field_30)->fn[3](
                                self->field_30, 0x74696D67, &base[0x19f], 0);
                            break;
                        default:
                            break;
                        }
                        func_80136B4C(layout, rebuildName, func_eu_802B1474(), 0);
                        func_80139A18(layout, rebuildName, &lbl_eu_80664688, &lbl_eu_80664690);
                    }
                }
            } else {
                marks[count++] = 0;
                char* lbl = func_80136190(&base[0xff], &base[0x108], 0x6d);
                sprintf(textBuf2, &base[0x1b5], lbl);
                func_80124270(pane, 0);
            }
        }
        func_80136A1C(layout, paneName2, textBuf2, 0);
        func_80136A1C(layout, labelName3, textBuf2, 0);
        if (texRes != NULL)
            func_80137E7C(layout, imageName2, (u32)texRes);
    }
}
#pragma optimize_for_size off

void func_80205294(void* dst, void* src) {
    u16 a = *(u16*)src;
    u32 b = *(u32*)((u8*)src + 4);
    *(u16*)dst = a;
    *(u32*)((u8*)dst + 4) = b;
}

// Target us-802073c8: file-load event handler for the two bind files loaded
// by func_80202090. Handle-1 branch builds the whole equip-change screen:
// 0xE000 scratch region, arc accessor, main layout + three anim transforms,
// shared text object seeding six labels, animation enables, cursor palette
// colours copied into the sdata2 tables, and the three cursors built from
// stack temporaries. Handle-2 branch only attaches the second accessor and
// caches it in the global page flag.
#pragma optimize_for_size on
bool CEquipChange::OnFileEvent(CEventFile* file) {
    // Declared up-front; `scratch` holds the colour-source pane in branch 1
    // and the second file's data pointer in branch 2 (retail shares one
    // callee-saved register between both).
    void* scratch;
    char* base;
    void* data;
    if (field_24 == (u32)file->mFileHandle) {
        int memHandle = (int)mtl::MemManager::getHandleMEM2();
        base = lbl_eu_80508168;
        createRegion__17UnkClass_8045F564FiiPCci(&_pad04[0], memHandle,
                                                 0xE000, &base[0x1ED], 1);
        u32 host[2];
        __ct__14Class_8045F858FP17UnkClass_8045F564(host, &_pad04[0]);
        CFileHandle* handle = (CFileHandle*)field_24;
        data = handle->getData();
        setMemInitFlag__Q23mtl10MemManagerFb(false);
        field_2C = createArcResourceAccessor__10CLibLayoutFv();
        field_2C->Attach(data, &base[0x1FA]);
        buildLayout((nw4r::lyt::Layout**)&field_34, field_2C, &base[0x1FE]);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)field_34, &field_38, field_2C, &base[0x20E]);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)field_34, &field_3C, field_2C, &base[0x221]);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)field_34, &field_40, field_2C, &base[0x239]);
        nw4r::lyt::Pane* root = (nw4r::lyt::Pane*)((CLayoutView*)(u32)field_34)->field_10;
        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
            1, (nw4r::lyt::Layout*)field_34);
        func_8013676C(root, reinterpret_cast<CItemBoxFontInfoVt*>(fontObj)->fontData());

        // Seed the label textboxes with the shared text object.
        char* text = func_801355BC();
        if (text != NULL) {
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x55], (u32)text);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x60], (u32)text);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x6A], (u32)text);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x74], (u32)text);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x7E], (u32)text);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x88], (u32)text);
        }

        ((CLayoutVtbl11*)(u32)field_34)->v9(field_3C, 0);
        ((CLayoutVtbl11*)(u32)field_34)->v9(field_40, 0);
        ((CLayoutVtbl11*)(u32)field_34)->v9(field_38, 1);
        ((CLayoutVtbl11*)(u32)field_34)->v12(0);

        // Cursor colour palette: read both highlight pairs off the pane and
        // copy them into the sdata2 colour tables, propagating the alpha
        // halves to the paired entries.
        // Retail materializes the full symbol address here (fresh lis/addi),
        // not the cached base pointer.
        scratch = ((CLayoutSubVtbl13*)((CLayoutView*)(u32)field_34)->field_10)
                      ->v13((u32)(lbl_eu_80508168 + 0x60), 1);
        CEquipItemBoxFourShorts color = func_801397AC((nw4r::lyt::Pane*)scratch, (u32)0);
        CopyVec4s(&lbl_eu_80664668, &color);
        color = func_801397AC((nw4r::lyt::Pane*)scratch, (u32)1);
        CopyVec4s(&lbl_eu_80664670, &color);
        // Alpha halves propagate to the paired colour entries; retail keeps
        // each table address in its own register (pointer locals).
        char* colA = (char*)&lbl_eu_80664668;
        char* colB = (char*)&lbl_eu_80664670;
        s16 alpha0 = *(s16*)(colA + 6);
        char* dstA = (char*)&lbl_eu_80664678;
        char* dstC = (char*)&lbl_eu_80664688;
        s16 alpha1 = *(s16*)(colB + 6);
        char* dstB = (char*)&lbl_eu_80664680;
        char* dstD = (char*)&lbl_eu_80664690;
        *(s16*)(dstA + 6) = alpha0;
        *(s16*)(dstB + 6) = alpha1;
        *(s16*)(dstC + 6) = alpha0;
        *(s16*)(dstD + 6) = alpha1;

        // Build each cursor from a stack temporary, copy it into place, run
        // its reset hook, then advance to the next one. Retail reloads the
        // accessor from field_2C at every use (never cached in a register).
        u8 tmp14[0x18];
        u8 tmp15[0x18];
        u8 tmpSub[0x24];
        __ct__CCur14(tmp14, field_2C);
        func_8018B0FC(_pad50, tmp14);
        __dt__6CCur14Fv((CBaseCur*)tmp14, -1);
        ((CCurVtblView*)_pad50)->v0();

        __ct__CCur15(tmp15, field_2C);
        func_8018B0FC(_pad68, tmp15);
        __dt__6CCur15Fv((CBaseCur*)tmp15, -1);
        ((CCurVtblView*)_pad68)->v0();

        __ct__CSubCur(tmpSub, field_2C);
        func_8018B0FC(field_80, tmpSub);
        __dt__7CSubCurFv((CBaseCur*)tmpSub, -1);
        ((CCurVtblView*)field_80)->v0();

        func_801D2E4C(field_80, 0);
        func_802040A0(this);
        field_24 = 0;
        validateHeap__17UnkClass_8045F564Fv(&_pad04[0]);
        __dt__14Class_8045F858Fv(host, -1);
        return true;
    }
    if (field_28 == (u32)file->mFileHandle) {
        int memHandle = (int)mtl::MemManager::getHandleMEM2();
        base = lbl_eu_80508168;
        createRegion__17UnkClass_8045F564FiiPCci(&_pad14[0], memHandle,
                                                 0x100, &base[0x250], 1);
        u32 host[2];
        __ct__14Class_8045F858FP17UnkClass_8045F564(host, &_pad14[0]);
        CFileHandle* handle = (CFileHandle*)field_28;
        scratch = handle->getData();
        setMemInitFlag__Q23mtl10MemManagerFb(false);
        nw4r::lyt::ArcResourceAccessor* acc = createArcResourceAccessor__10CLibLayoutFv();
        field_30 = (u32)acc;
        acc->Attach(scratch, &base[0x1FA]);
        // Cache the second accessor in the global equip-page flag slot.
        lbl_eu_80664698 = (u32)field_30;
        func_802040A0(this);
        field_28 = 0;
        validateHeap__17UnkClass_8045F564Fv(&_pad14[0]);
        __dt__14Class_8045F858Fv(host, -1);
        return true;
    }
    return false;
}
#pragma optimize_for_size off

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
    // Local block mirrors the retail frame: {word,byte} pair at 0x8/0xC,
    // item-box info pack at 0x10, per-category id row at 0x1C.
    u32 catWord;
    u8 catByte;
    char buf[0x10];
    s16 ids[5];
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
                if (item == NULL || *(u32*)item == 0) {
                    self->field_98 = 0;
                } else {
                    void* inst = CItem_initItemImplInstances(item);
                    u8 equipped = ((CItemImplVtblView*)inst)->v10(item);
                    if (equipped < (u8)cur3a)
                        self->field_98 = (s8)equipped;
                    else
                        flag = 1;
                }
            }
        } else {
            int idx = (u8)cur3a - 4;
            ids[0] = *(s16*)(obj + 0x1c);
            ids[1] = *(s16*)(obj + 0x1e);
            ids[2] = *(s16*)(obj + 0x20);
            ids[3] = *(s16*)(obj + 0x22);
            ids[4] = *(s16*)(obj + 0x24);
            catWord = *(u32*)&lbl_eu_806682A0[2 * idx];
            catByte = lbl_eu_806682A4[2 * idx];
            s16 id = ids[idx];
            if (id != -1) {
                // category byte is read indexed out of the {word,byte} block
                void* item = func_80157C4C(((u8*)&catWord)[idx], id);
                if (item != NULL && *(u32*)item != 0) {
                    void* inst = CItem_initItemImplInstances(item);
                    if (((CItemImplVtblView*)inst)->v10(item) != 0)
                        flag = 1;
                }
            }
            if (flag == 0)
                self->field_98 = (s8)(idx * 2 + 4);
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

// Target us-80207088: resolve the item record under the current equip-change
// selection. Non-weapon categories look the per-category equipped id up in
// the bdat row and return func_80157C4C(cat, id). The weapon-row path (cat 3)
// maps the sub-cursor index to a category, fetches the item, then either
// equips it through the item-impl vtable hooks or rebuilds the scratch item
// lbl_eu_80576568 from bdat (guarded by lbl_eu_8066469C) and returns it.
// bdat-rebuild fallback shape needs the opt-space stmw r28 frame.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void* func_802052A8(CEquipChange* self) {
    // cat declared first - its home web (shared with mapped) gets priority.
    int cat;
    CBdatCharData* obj = (CBdatCharData*)func_8009EC9C((u8)func_801392B4(self->field_99));
    cat = func_80203138(self);
    if ((u8)cat != 3) {
        s16 id = -1;
        switch ((u8)cat) {
        case 2: id = obj->field_26; break;
        case 4: id = obj->field_1C; break;
        case 5: id = obj->field_1E; break;
        case 6: id = obj->field_20; break;
        case 7: id = obj->field_22; break;
        case 8: id = obj->field_24; break;
        }
        return func_80157C4C((u8)cat, id);
    } else {
    // Weapon-row path: map the sub-cursor index to a category. Indices 1..3
    // collapse to category 2; 4..8 pass through; anything else stays 3.
    int idx = func_802031A0(self);
    u8 slot = self->_pad9A[(u8)idx - 1];
    if (slot == 0)
        return 0;
    if (slot == 1)
        return 0;
    // mapped starts as a plain int copy of cat so both share one register
    // web (retail keeps it unclamped in the callee-saved home).
    int mapped = cat;
    if ((u8)idx <= 3) {
        mapped = 2;
    } else {
        switch ((u8)idx) {
        case 4: mapped = 4; break;
        case 5: mapped = 5; break;
        case 6: mapped = 6; break;
        case 7: mapped = 7; break;
        case 8: mapped = 8; break;
        }
    }
    // Flag keyed on the raw sub-cursor index (2 or 3), not the mapped value.
    int flag;
    if ((u8)idx == 2)
        flag = 1;
    else
        flag = (int)((u32)((u8)idx - 3) < 1);
    s16 wid = -1;
    switch ((u8)mapped) {
    case 2: wid = obj->field_26; break;
    case 4: wid = obj->field_1C; break;
    case 5: wid = obj->field_1E; break;
    case 6: wid = obj->field_20; break;
    case 7: wid = obj->field_22; break;
    case 8: wid = obj->field_24; break;
    }
    void* item = func_80157C4C((u8)mapped, wid);
    if (item == NULL)
        return 0;
    if (*(u32*)item == 0)
        return 0;
    void* impl = CItem_initItemImplInstances(item);
    CEquipV9Result* res =
        (CEquipV9Result*)((CItemImplVtblView*)impl)->v9(item, (u32)flag);
    if (res != NULL && (res->field_04 & 1) == 0) {
        // Direct-equip path: second hook returns the crystal id to equip.
        u32 r = ((CItemImplVtblView*)impl)->v14(item, (u8)flag);
        return func_80157C4C(3, (s16)r);
    }
    if (res == NULL)
        return 0;
    // bdat-rebuild fallback: clear the scratch item once, find the last bdat
    // row whose string column reads 3, rebuild lbl_eu_80576568 from it.
    if (lbl_eu_8066469C == 0) {
        lbl_eu_80576568.field_04 = 0;
        lbl_eu_80576568.field_00 = 0;
        lbl_eu_8066469C = 1;
    }
    void* table = (void*)lbl_eu_806640EC;
    u32 cnt = func_8003B1EC(table);
    u16 row = (u16)(cnt - 1);
    while (row != 0) {
        u32 val = getBdatStringColumnValue(table, &lbl_eu_80508168[0x1E4], (s32)row);
        if ((u16)val == 3)
            break;
        row--;
    }
    u32 w = res->field_00;
    u16 h = res->field_04;
    func_80159F6C(&lbl_eu_80576568, row, ((u32)h >> 16) & 0xFFF, (w >> 22) & 7);
    void* impl2 = CItem_initItemImplInstances(&lbl_eu_80576568);
    ((CItemImplVtblView*)impl2)->v35(&lbl_eu_80576568, (s16)((w >> 11) & 0x7FF));
    return &lbl_eu_80576568;
    }
}
#pragma optimize_for_size off

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
    // Retail jumptable_eu_805355B0 routing: states 0/3/8 (and >10) are
    // idle; the eight anim-waiter handlers sit on 1,2,4,5,6,7,9,10. The
    // interleaved no-op rows keep the dense table bound at 10.
    switch (self->field_48) {
    case 1: func_80203CE0(self); break;
    case 2: func_80203D78(self); break;
    case 4: func_80203E00(self); break;
    case 5: func_80203E98(self); break;
    case 6: func_80203EE4(self); break;
    case 7: func_80203F84(self); break;
    case 9: func_80203FCC(self); break;
    case 10: func_8020404C(self); break;
    default:
        goto tail;
    }
tail:
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
    drawLayout((nw4r::lyt::Layout*)self->field_34, drawInfo, 0, 1);
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
    closeFileHandle__FPP11CFileHandle(&self->field_24);
    closeFileHandle__FPP11CFileHandle(&self->field_28);
    // Retail hoists the field_34 load above the field_44 store and reuses the
    // loaded pointer across the guarded destroy call - keeping it in a local
    // pins it to a volatile reg instead of forcing a second callee-saved.
    void* layout = (void*)self->field_34;
    self->field_44 = 0;
    if (layout != 0) {
        if (layout != 0)
            ((CLayoutVtbl11*)layout)->v0(1);
        self->field_34 = 0;
    }
    lbl_eu_80664698 = 0;
    releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(self->field_2C);
    releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor((void*)self->field_30);
    deleteRegion__17UnkClass_8045F564Fv(&self->_pad04[0]);
    ((CCurVtblView*)((u8*)self + 0x50))->v1();
    ((CCurVtblView*)((u8*)self + 0x68))->v1();
    ((CCurVtblView*)self->field_80)->v1();
    func_801D4174((CItemBoxInfo*)((u8*)self + 0xA4));
    func_80286454(&self->mEquipItemBox);
}
