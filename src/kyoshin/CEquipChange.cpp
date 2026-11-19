#define IWORK_EVENT_INLINE_DTOR // ~CEquipChange elides the empty base dtor (retail)

// harness_catalog dropped: this TU uses none of its VM/script-helper
// headers, and it drags in the stale void*/void* CItem_initItemImplInstances
// declaration (via CTaskGameEff -> CfGameManager) that conflicts with the
// real one in kyoshin/cf/CItem.hpp (see below).
// Real item-impl owner first (before CItemBoxInfo.hpp's shim macro exists,
// which would otherwise rewrite this header's own declaration). Its decls are
// canonical for the item tables, CItem_kindPtr and CItem_slotCnt.
#include "kyoshin/cf/CItem.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CEquipChange.hpp"
#include "kyoshin/cf/CtrlObjectParam.hpp"
// Out-of-line CCur copy (weak CBaseCur-family operator=, deduped into CMenuShopSell).
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
// Real item-impl accessor (CItem.hpp) for this TU only: undo the
// itemimplshim macro from CItemBoxInfo.hpp (other TUs keep using it).
#undef CItem_initItemImplInstances
#include "monolib/device/CDeviceFont.hpp"  // IDeviceFontInfo::getFont (+0x24)
#include "kyoshin/cf/object/CActorParam.hpp"  // CActorParam_getBattleParams (+0x224)
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/util/MemManager.hpp"
#include <stdio.h>
#include "kyoshin/CfEnumListHolder.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// const routes the symbol into the readonly sdata2 pool; MWCC then hoists the
// lfs ABOVE the frame stores (retail shape) - see MWCC_CASES §CExchangeWin.
extern const float lbl_eu_806682A8;
static const u32 sEquipSceneNameTags[3] = { 0x10, 0x100000, 0x200000 };


CEquipChange::~CEquipChange() {}

CEquipChange::CEquipChange()
    : field_24(0), field_28(0), field_2C(0), field_30(0), field_34(0), field_38(0), field_3C(0),
      field_40(0), field_44(0), field_48(0), field_4C(0), field_4D(1), field_98(0), field_99(0),
      mItemBoxInfo(0, 0) {
    mItemBoxInfo = CItemBoxInfo(4, 0);
    mEquipItemBox = CEquipItemBox();
}

// Target us-80203d88: load both bind files (two string records at
// lbl_eu_80508168) into file handles 0x24/0x28, then init the item-box info
// layout (0xA4) and the equip item box (0x2B0).
// Retail frame is the -O4,s stmw/lmw pair.
void CEquipChange::LoadFiles() {
    const char* path = lbl_eu_80508168;
    field_24 = (u32)CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                                path, this, 0, 0);
    CDeviceFile::setHandleFlag1((CFileHandle*)field_24);
    field_28 = (u32)CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                                path + 0x18, this, 0, 0);
    CDeviceFile::setHandleFlag1((CFileHandle*)field_28);
    mItemBoxInfo.LoadFiles();
    mEquipItemBox.loadEIBFiles();
}

// Forward declarations for functions whose definitions live at the end of this
// TU. Declaring (not defining) them here prevents MWCC from inlining their
// bodies into callers like EquipChange_GetLockedState / EquipChange_OnApplyToStA, which must emit the
// retail `bl` (external) calls.

u32 CEquipChange::IsActiveFlag() { return field_4D; }

int CEquipChange::CheckBoxOpen() { return mEquipItemBox.getEIBOpenFlag(); }

int CEquipChange::IsMenuBusy() { return mEquipItemBox.eibMenuBusy(); }

// Target us-802040c8: is either the sort menu or the sub-page current.
int CEquipChange::IsSortOrSubPage() {
    return (mEquipItemBox.eibMenuBusy() != 0) || (mEquipItemBox.getEIBNamePane() != 0);
}

// Target us-8020405c: read selection at 0xA4 unless the item box confirm op is busy.
int CEquipChange::GetSelWhenReady() {
    if (mItemBoxInfo.GetState() == 0)
        return 0;
    if (mEquipItemBox.eibWindowsReady() != 0)
        return field_4C;
    return 0;
}

// Target us-8020411c: item box ready and current selection is index 3.
int CEquipChange::IsWeaponRow() {
    if (mEquipItemBox.getEIBOpenFlag() != 0 && (u8)MapCursorToCat() == 3)
        return 1;
    return 0;
}

// Target us-8020417c: subcur visible, else rely on the item box.
int CEquipChange::IsSubcurBusy() {
    if (mSubCur.BothSubPanesOn() != 0)
        return 1;
    return mEquipItemBox.eibSysWinBusy();
}

// Target us-802041c4: initialise the equip-change screen. When idle (0x48),
// set state 1, init the item-info window, repack the selection word and play
// the 0x6D sound. Retail recomputes self+0xA4 per call site (no CSE) and
// packs the nibbles low-to-high starting from the EquipChange_MapCursorToSlot result.
void CEquipChange::InitScreen() {
    if (field_48 == 0) {
        field_48 = 1;
        field_4D = 0;
        mItemBoxInfo.Open();
        mItemBoxInfo.HidePanes((u8)MapCursorToCat());
        u8 f99 = field_99;
        u32 cur38 = (u32)MapCursorToCat();
        u32 cur3a = (u32)MapCursorToSlot();
        mItemBoxInfo.SetItem((u16)((cur3a & 0xF) | ((cur38 & 0xF) << 4) | ((u32)f99 << 8)), ResolveEquipItem(), 1);
        playUISound(0x6d);
    }
}

// Target us-80204270: close the equip-change screen. From state 3, enter
// state 4, close the three cursors, advance the item-info window, reset the
// three layout slot animations and play the 0x6 sound.
void CEquipChange::CloseScreen() {
    if ((s32)field_48 == 3) {
        field_48 = 4;
        field_4D = 0;
        mCur14.SetVisible(0);
        mCur15.SetVisible(0);
        mSubCur.SetVisible(0);
        mItemBoxInfo.Close();
        ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_3C, 0);
        ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_40, 0);
        ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_38, 1);
        playUISound(6);
    }
}

// Target us-8020433c: advance the equip selection. When the box gates are
// closed, roll the cursor-run flag 0x98 backwards past non-equipped slots,
// then refresh the equip info window; otherwise (both gates open) hand the
// input to the item box. Retail frame saves 4 regs via stmw.
void CEquipChange::CursorPrev() {
    if (mEquipItemBox.getEIBOpenFlag() != 0) {
        if (mEquipItemBox.getEIBActiveMark() != 0)
            mEquipItemBox.eibNavLeft();
        return;
    }
    // Mirrors the fully-matched forward sibling EquipChange_CursorNext: temp-based
    // pre-wrap, one probe outside the rotated loop guarding entry, and a
    // plain decrement at the loop tail.
    u8 v = (u8)(field_98 - 1);
    field_98 = (s8)v;
    if ((s8)v < 0)
        field_98 = 13;
    u8 idx = (u8)MapCursorToSlot();
    if (idx != 0 && mSlotMarks[-1 + idx] == 0) {
        field_98 = (s8)(field_98 - 1);
        while (true) {
            if (field_98 < 0)
                field_98 = 13;
            idx = (u8)MapCursorToSlot();
            if (idx == 0)
                break;
            if (mSlotMarks[-1 + idx] != 0)
                break;
            field_98 = (s8)(field_98 - 1);
        }
    }
    RefreshCursorPos();
    u8 f99 = field_99;
    u32 cur38 = (u32)MapCursorToCat();
    u32 cur3a = (u32)MapCursorToSlot();
    void* a8 = ResolveEquipItem();
    // Two-step pack (same shape as matched EquipChange_CursorNext): the shifted term
    // first makes MWCC fold the unshifted cur3a into the clrlwi base.
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    mItemBoxInfo.SetItem(packed, a8, 1);
    int v38 = MapCursorToCat();
    mItemBoxInfo.HidePanes((u8)v38);
    playUISound(1);
}

// Target us-80204488: when both item-box gates are closed, roll the cursor
// flag 0x98 forward past non-equipped slots, then refresh the equip info
// window; otherwise (both gates open) hand the input to the item box. Retail
// frame saves 4 regs via stmw (opt-space prologue).
void CEquipChange::CursorNext() {
    if (mEquipItemBox.getEIBOpenFlag() != 0) {
        if (mEquipItemBox.getEIBActiveMark() != 0)
            mEquipItemBox.eibNavDown();
        return;
    }
    u8 v = (u8)(field_98 + 1);
    field_98 = (s8)v;
    if ((s8)v > 0xd)
        field_98 = 0;
    u8 idx = (u8)MapCursorToSlot();
    if (idx != 0 && mSlotMarks[-1 + idx] == 0) {
        field_98 = (s8)(field_98 + 1);
        while (true) {
            if (field_98 > 0xd)
                field_98 = 0;
            idx = (u8)MapCursorToSlot();
            if (idx == 0)
                break;
            if (mSlotMarks[-1 + idx] != 0)
                break;
            field_98 = (s8)(field_98 + 1);
        }
    }
    RefreshCursorPos();
    u8 f99 = field_99;
    u32 cur38 = (u32)MapCursorToCat();
    u32 cur3a = (u32)MapCursorToSlot();
    void* a8 = ResolveEquipItem();
    // Two-step pack (same shape as matched EquipChange_OnAnim40ToSt7): the shifted term
    // first makes MWCC fold the unshifted cur3a into the clrlwi base.
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    mItemBoxInfo.SetItem(packed, a8, 1);
    int v38 = MapCursorToCat();
    mItemBoxInfo.HidePanes((u8)v38);
    playUISound(1);
}

// Target us-802045dc: when both item-box gates are closed, remap the
// cursor-run flag 0x98 (see EquipChange_MapCursorToCat's 0/2/4/6/8/10/12->... mapping) and
// refresh the equip info window with the compressed selection word. Retail
// emits one li/stb block per case (14 separate blocks in the jump table).
void CEquipChange::CursorUp() {
    if (mEquipItemBox.getEIBOpenFlag() != 0) {
        if (mEquipItemBox.getEIBActiveMark() != 0)
            mEquipItemBox.eibNavUp();
        return;
    }
    code80135FDC_getByte_64077();
    switch (field_98) {
    case 0: field_98 = 12; break;
    case 1: field_98 = 0; break;
    case 2: field_98 = 0; break;
    case 3: field_98 = 0; break;
    case 4: field_98 = 0; break;
    case 5: field_98 = 4; break;
    case 6: field_98 = 4; break;
    case 7: field_98 = 6; break;
    case 8: field_98 = 6; break;
    case 9: field_98 = 8; break;
    case 10: field_98 = 8; break;
    case 11: field_98 = 10; break;
    case 12: field_98 = 10; break;
    case 13: field_98 = 12; break;
    }
    RefreshCursorPos();
    u8 f99 = field_99;
    u32 cur38 = (u32)MapCursorToCat();
    u32 cur3a = (u32)MapCursorToSlot();
    void* a8 = ResolveEquipItem();
    // Two-step pack (same shape as matched EquipChange_OnAnim40ToSt7): the shifted term
    // first makes MWCC fold the unshifted cur3a into the clrlwi base.
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    mItemBoxInfo.SetItem(packed, a8, 1);
    int v38 = MapCursorToCat();
    mItemBoxInfo.HidePanes((u8)v38);
    playUISound(1);
}

// Target us-80204768: sibling of EquipChange_CursorUp - same guard/refresh shape
// but a different remap table for field_98 (also one li/stb block per case).
void CEquipChange::CursorRight() {
    if (mEquipItemBox.getEIBOpenFlag() != 0) {
        if (mEquipItemBox.getEIBActiveMark() != 0)
            mEquipItemBox.eibNavRight();
        return;
    }
    code80135FDC_getByte_64077();
    switch (field_98) {
    case 0: field_98 = 4; break;
    case 1: field_98 = 4; break;
    case 2: field_98 = 4; break;
    case 3: field_98 = 4; break;
    case 4: field_98 = 6; break;
    case 5: field_98 = 6; break;
    case 6: field_98 = 8; break;
    case 7: field_98 = 8; break;
    case 8: field_98 = 10; break;
    case 9: field_98 = 10; break;
    case 10: field_98 = 12; break;
    case 11: field_98 = 12; break;
    case 12: field_98 = 0; break;
    case 13: field_98 = 0; break;
    }
    RefreshCursorPos();
    u8 f99 = field_99;
    u32 cur38 = (u32)MapCursorToCat();
    u32 cur3a = (u32)MapCursorToSlot();
    void* a8 = ResolveEquipItem();
    // Two-step pack (same shape as matched EquipChange_OnAnim40ToSt7).
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    mItemBoxInfo.SetItem(packed, a8, 1);
    int v38 = MapCursorToCat();
    mItemBoxInfo.HidePanes((u8)v38);
    playUISound(1);
}

void CEquipChange::ConfirmSort() {
    if (mEquipItemBox.getEIBOpenFlag() != 0 && mEquipItemBox.getEIBActiveMark() != 0)
        mEquipItemBox.eibConfirmSort();
}

// Target us-80204944: when both item-box gates are open, either reset the box
// cursor (eibHandleSubPage) or open the sub-page (finishEIBEntry) depending on
// eibInputBlocked, entering state 9.
void CEquipChange::OpenSubPage() {
    if (mEquipItemBox.getEIBOpenFlag() == 0)
        return;
    if (mEquipItemBox.getEIBActiveMark() == 0)
        return;
    if (mEquipItemBox.eibInputBlocked() != 0) {
        mEquipItemBox.eibHandleSubPage(0);
    } else {
        mEquipItemBox.finishEIBEntry();
        field_48 = 9;
        field_4D = 0;
    }
}

// Target us-802049c4: item-box confirm / apply flow. When the box gates are
// open and not sub-paging, finalize the selection (eibApplySelect pack) and
// hand off to EquipChange_RefreshCategory. The gate-closed branch refuses when the
// character is busy or the selection points at an empty / locked slot. Retail
// frame saves 4 regs via stmw (opt-space prologue).
void CEquipChange::ConfirmSelect() {
    if (mEquipItemBox.getEIBOpenFlag() != 0) {
        if (mEquipItemBox.getEIBActiveMark() == 0)
            return;
        if (mEquipItemBox.getEIBNamePane() != 0) {
            mEquipItemBox.stepEIBSysWin2();
            return;
        }
        // .L_80204A14
        if (mEquipItemBox.isEIBSelectBlocked() == 0) {
            playUISound(5);
            return;
        }
        // .L_80204A30
        mEquipItemBox.handleEIBConfirm();
        RebuildCursorList();
        u8 cat = field_99;
        int cur38 = MapCursorToCat();
        int cur3a = MapCursorToSlot();
        void* a8 = ResolveEquipItem();
        // Two-step pack (same shape as matched EquipChange_OnAnim40ToSt7).
        u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
        packed = (u16)(packed | ((u32)cat << 8));
        mEquipItemBox.eibApplySelect(packed, a8);
        RefreshCategory(field_99);
        return;
    }
    // gate closed branch (.L_80204A8C)
    CBdatCharData* data = (CBdatCharData*)CtrlObjectParam_GetCharEntry((u8)GetCollectedFlagByte(field_99));
    if (data->field_176C == 1) {
        if ((u8)MapCursorToCat() == 3)
            return;
        playUISound(5);
        return;
    }
    if ((u8)MapCursorToCat() == 3) {
        if (mSlotMarks[-1 + (u8)MapCursorToSlot()] == 0)
            return;
        if ((u8)mSlotMarks[-1 + (u8)MapCursorToSlot()] == 2)
            return;
    }
    // .L_80204B14: reset the three layout slot animations and close the cursors.
    ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_3C, 0);
    ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_38, 0);
    ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_40, 1);
    field_48 = 6;
    field_4D = 0;
    mCur14.SetVisible(0);
    mCur15.SetVisible(0);
    playUISound(3);
}

// (EquipChange_MapCursorToCat and EquipChange_MapCursorToSlot are defined at the END of this TU. Retail
// calls them externally at every site; the global address-takes at the end of
// the TU stop MWCC's -inline auto / -ipa file inliner from folding their
// bodies into callers (the bodies stay emitted as standalone symbols).)

// Target us-80204f08: main equip-change flow. Returns 0 when the character is
// busy and the cursor sits on a non-equippable slot, when the item-box gates
// reject, when the selected slot is empty/locked, or when a category-restricted
// item cannot be equipped; 1 after the generic refresh; 2/3 from the weapon-row
// paths. Retail frame saves 4 regs via stmw (opt-space prologue).
int CEquipChange::RunMainFlow() {
    CBdatCharData* obj = (CBdatCharData*)CtrlObjectParam_GetCharEntry((u8)GetCollectedFlagByte(field_99));
    if (obj->field_176C == 1) {
        s8 cur = field_98;
        if (cur == 0 || cur == 4 || cur == 6 || cur == 8 || cur == 10 || cur == 12)
            return 0;
    }
    if (mEquipItemBox.getEIBOpenFlag() == 0 && field_98 == 0)
        return 0;
    int cat = MapCursorToCat();
    if (mEquipItemBox.getEIBOpenFlag() != 0) {
        if (mEquipItemBox.eibRowMatches() == 0)
            return 0;
        cat = mEquipItemBox.getEIBSelCat();
    }
    if ((u8)cat != 3) {
    switch ((u8)cat) {
    case 2: {
        s16 id = obj->field_26;
        if (id == -1)
            return 0;
        void* item = CItem_kindPtr(2, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
            if (isOne && CtrlRemote_TouchBitByArg(0x3508) == 0)
                return 0;
        }
        CtrlObjectParam_SetEquipSlot5(obj, -1);
        break;
    }
    case 4: {
        s16 id = obj->field_1C;
        if (id == -1)
            return 0;
        void* item = CItem_kindPtr(4, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
            if (isOne && CtrlRemote_TouchBitByArg(0x3508) == 0)
                return 0;
        }
        CtrlObjectParam_SetEquipSlot0(obj, -1);
        break;
    }
    case 5: {
        s16 id = obj->field_1E;
        if (id == -1)
            return 0;
        void* item = CItem_kindPtr(5, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
            if (isOne && CtrlRemote_TouchBitByArg(0x3508) == 0)
                return 0;
        }
        CtrlObjectParam_SetEquipSlot1(obj, -1);
        break;
    }
    case 6: {
        s16 id = obj->field_20;
        if (id == -1)
            return 0;
        void* item = CItem_kindPtr(6, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
            if (isOne && CtrlRemote_TouchBitByArg(0x3508) == 0)
                return 0;
        }
        reinterpret_cast<cf::CtrlObjectParamData*>(obj)->setArgType2((void*)-1);
        break;
    }
    case 7: {
        s16 id = obj->field_22;
        if (id == -1)
            return 0;
        void* item = CItem_kindPtr(7, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
            if (isOne && CtrlRemote_TouchBitByArg(0x3508) == 0)
                return 0;
        }
        reinterpret_cast<cf::CtrlObjectParamData*>(obj)->setArgType3((void*)-1);
        break;
    }
    case 8: {
        s16 id = obj->field_24;
        if (id == -1)
            return 0;
        void* item = CItem_kindPtr(8, id);
        if (item != NULL && *(u32*)item != 0) {
            u32 word = *(u32*)item;
            u8 v = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_80508168[0x34], word >> 20);
            u32 isOne = v - 1;
            isOne = (isOne == 0) ? 1u : 0u;
            if (isOne && CtrlRemote_TouchBitByArg(0x3508) == 0)
                return 0;
        }
        CtrlObjectParam_SetEquipSlot4(obj, -1);
        break;
    }
    default:
        break;
    }
    // shared refresh: wake the bdat row, filter the scene's enemy list and
    // play the 0x76 sound.
    CtrlObjectParam_SyncParamFromActor(obj);
    if (field_99 < 3) {
        u32 names[3];
        // Post-increment loads fold the base materialization into the first
        // access (retail: lis + lwzu + +4/+8 displacements).
        const u32* src = sEquipSceneNameTags;
        names[0] = *src++;
        names[1] = *src++;
        names[2] = *src++;
        CfEnumListHolder holder;
        startEnumObjects(holder.get(), names[field_99], 0);
        if (((CEquipEnumList*)holder.get())->field_0x620 >= 1) {
            CEquipEnumListSlot* slot = (CEquipEnumListSlot*)getEntryAt(holder.get(), 0);
            if (slot->field_0x4 != 0) {
                CfObjectPc_setupFrame(getCfObjectPc__FPQ22cf12CfObjectMove(slot->field_0x4), 0);
            }
        }
    }
    playUISound(0x76);
        return 1;
    }
    // (u8)cat == 3 - weapon-row path: resolve the sub-cursor, reject
    // empty/locked slots, then equip the row item through the item-impl
    // vtable[0x44] hook.
    int v = MapCursorToSlot();
    if (mEquipItemBox.getEIBOpenFlag() != 0)
        v = mEquipItemBox.findEIBEquipSlot();
    u8 cursor = (u8)v;
    u8 slot = mSlotMarks[cursor - 1];
    if (slot == 0)
        return 0;
    if (slot == 1)
        return 0;
    if (slot == 2) {
        playUISound(5);
        return 2;
    }
    int kind;
    switch (cursor) {
    case 1: case 2: case 3: kind = 2; break;
    case 4: kind = 4; break;
    case 5: kind = 5; break;
    case 6: kind = 6; break;
    case 7: kind = 7; break;
    case 8: kind = 8; break;
    default: break;
    }
    if ((u8)v == 2)
        v = 1;
    else
        v = ((int)(u8)v - 3 > 0) ? 2 : 0;
    s16 id = -1;
    switch (kind) {
    case 2: id = obj->field_26; break;
    case 4: id = obj->field_1C; break;
    case 5: id = obj->field_1E; break;
    case 6: id = obj->field_20; break;
    case 7: id = obj->field_22; break;
    case 8: id = obj->field_24; break;
    default: break;
    }
    kind = (int)CItem_kindPtr((u8)kind, id);
    if (kind != 0 && *(u32*)kind != 0) {
        CItemImpl* inst = CItem_initItemImplInstances((CItemData*)kind);
        inst->vf44((CItemData*)kind, (u8)v, -1);
    }
    RebuildCursorList();
    mItemBoxInfo.HidePanes((u8)MapCursorToCat());
    // Lifetimes mirror retail: f99 (r28) and cur38 (r29) feed rlwimi inserts
    // into the packed word based on the fresh EquipChange_MapCursorToSlot low nibble.
    u8 f99 = field_99;
    int cur38 = MapCursorToCat();
    u32 packed = (u32)MapCursorToSlot() & 0xF;
    packed |= ((u32)(cur38 & 0xF) << 4);
    packed |= ((u32)f99 << 8);
    mItemBoxInfo.SetItem((u16)packed, ResolveEquipItem(), 1);
    if (mEquipItemBox.getEIBOpenFlag() != 0)
        mEquipItemBox.eibOpenPages((u16)packed, 0, 1);
    playUISound(0x77);
    return 3;
}

// Target us-80205624: raw box count, cleared when the current selection is 3.
int CEquipChange::GetBoxCount() {
    int result = mEquipItemBox.takeEIBAction();
    if ((u8)MapCursorToCat() == 3)
        result = 0;
    return result;
}

void CEquipChange::ShowSubPanes(bool visible) { mSubCur.ShowTwoSubPanes(visible); }

void CEquipChange::HandleEIBName() { mEquipItemBox.handleEIBNamePane(); }

void CEquipChange::TryCloseEIBRow() { mEquipItemBox.eibTryCloseRow(); }

// Target us-8020568c: cursor 3 is selected AND the per-slot byte at
// self[idx+0x99] (index from EquipChange_MapCursorToSlot) equals 2.
int CEquipChange::GetLockedState() {
    if ((u8)MapCursorToCat() == 3) {
        u8 idx = (u8)MapCursorToSlot();
        if (mSlotMarks[-1 + idx] == 2)
            return 2;
    }
    return 0;
}

// Target us-802056ec: sub-cursor busy returns 0x2E; otherwise key the state
// byte (>=2 => sub-page open flag) and the item-box gate: gate open routes
// to eibHudPrompt, gate closed picks a menu id from the cursor state.
// Retail saves 2 regs via stmw/lmw (opt-space prologue).
int CEquipChange::GetMenuId() {
    if (mSubCur.BothSubPanesOn() != 0)
        return 0x2E;
    u8 b = code80135FDC_getByte_64077();
    int flag = (int)((u32)(1 - b) >> 31);
    if (mEquipItemBox.getEIBOpenFlag() != 0)
        return mEquipItemBox.eibHudPrompt();
    if ((u8)MapCursorToCat() == 2) {
        if (flag != 0)
            return 0x15;
        return 0x17;
    }
    if (flag != 0)
        return 0x14;
    return 0x16;
}

// Target us-80205790: resolve the currently-equipped item's resource id from
// the active category (box gate open: getEIBSelCat category; gate closed:
// field_98) and the per-category item id read from the bdat row (obj+off).
// Opt-space merges the callee-saved saves into stmw r30 and copies r3->r30
// before r4->r31 (retail prologue shape; MWCC_CASES kyoshin leaf patterns).
int CEquipChange::GetEquippedResId(u32 param) {
    if (mEquipItemBox.getEIBOpenFlag() != 0) {
        if (param == 0 && mEquipItemBox.isEIBCatMatch() == 0)
            return 0;
        CBdatCharData* obj = (CBdatCharData*)CtrlObjectParam_GetCharEntry((u8)GetCollectedFlagByte(field_99));
        // signed compare (retail cmpi chain) - category value 2/4/5/6/7/8
        int r0 = (u8)mEquipItemBox.getEIBSelCat();
        CItemExt* item;
        switch (r0) {
        case 2: item = CItem_kindPtr(2, obj->field_26); break;
        case 4: item = CItem_kindPtr(4, obj->field_1C); break;
        case 5: item = CItem_kindPtr(5, obj->field_1E); break;
        case 6: item = CItem_kindPtr(6, obj->field_20); break;
        case 7: item = CItem_kindPtr(7, obj->field_22); break;
        case 8: item = CItem_kindPtr(8, obj->field_24); break;
        default: return 0;
        }
        // tail duplicated per branch in retail (.L_80205894 / .L_80205960)
        if (item != NULL && item->field_00 != 0)
            return item->field_00 >> 20;
    } else {
        // box gate closed - key off the cursor-run flag field_98
        CBdatCharData* obj = (CBdatCharData*)CtrlObjectParam_GetCharEntry((u8)GetCollectedFlagByte(field_99));
        s8 r0 = field_98;
        CItemExt* item;
        // retail chain tests 0/4/6/8/10/11 (extsb. + cmpwi, all tests first)
        switch (r0) {
        case 0: item = CItem_kindPtr(2, obj->field_26); break;
        case 4: item = CItem_kindPtr(4, obj->field_1C); break;
        case 6: item = CItem_kindPtr(5, obj->field_1E); break;
        case 8: item = CItem_kindPtr(6, obj->field_20); break;
        case 10: item = CItem_kindPtr(7, obj->field_22); break;
        case 11: item = CItem_kindPtr(8, obj->field_24); break;
        default: return 0;
        }
        if (item != NULL && item->field_00 != 0)
            return item->field_00 >> 20;
    }
    return 0;
}

// Target us-80205994: lift the last box byte if the box gate is open.
u32 CEquipChange::GetBoxTailByte() {
    if (mEquipItemBox.getEIBOpenFlag() != 0)
        return mEquipItemBox.unk_37c;
    return 0;
}

// Target us-802059d8: when the 0x38 anim finishes, reset the three layout
// slot animations via the 0x34 object's vtable[0x2C] hook and enter state 2.
void CEquipChange::OnAnim38ToSt2() {
    if (advanceAnimTransform(field_38, lbl_eu_806682A8) == 0)
        return;
    ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_40, 0);
    ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_38, 0);
    ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_3C, 1);
    field_48 = 2;
}

// Target us-80205a70: when the 0x3C anim finishes, refresh the item info
// window (calcItemBoxPaneVec pack), drive the sub-cursor vtable[0x10] hook with the
// packed buffer, close the sub-cursor, and reset state via EquipChange_RefreshCursorPos.
void CEquipChange::OnAnim3CToSt3() {
    if (advanceAnimTransform(field_3C, lbl_eu_806682A8) == 0)
        return;
    field_48 = 3;
    field_4D = 1;
    nw4r::math::VEC3 buf = mItemBoxInfo.GetSlotPos(field_99);
    mSubCur.setRootPaneTranslate(&buf);
    mSubCur.SetVisible(1);
    RefreshCursorPos();
}

// Target us-80205af8: when the 0x3C anim finishes, reset the three layout
// slot animations via the 0x34 object's vtable[0x2C] hook and enter state 5.
// Sibling of EquipChange_OnAnim38ToSt2 (guard on field_38, state 2).
void CEquipChange::OnAnim3CToSt5() {
    if (AnimRewindFrame(field_3C, lbl_eu_806682A8) == 0)
        return;
    ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_3C, 0);
    ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_40, 0);
    ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_38, 1);
    field_48 = 5;
}

// Target us-80205b90: when the 0x38 anim finishes, clear state 0x48 and flag 0x4D.
void CEquipChange::OnAnim38ToIdle() {
    if (AnimRewindFrame(field_38, lbl_eu_806682A8) != 0) {
        field_4D = 1;
        field_48 = 0;
    }
}

// Target us-80205bdc: when the 0x40 anim finishes, repack the equip info
// (setEIBBoxInfo/80287DB4 with the packed (field_99<<8 | cur38<<4 | cur3a)
// word) and re-init the box, entering state 7. Retail saves 4 regs via
// _savegpr_28 (opt-space prologue).
void CEquipChange::OnAnim40ToSt7() {
    if (advanceAnimTransform(field_40, lbl_eu_806682A8) == 0)
        return;
    u8 f99 = field_99;
    int cur38 = MapCursorToCat();
    int cur3a = MapCursorToSlot();
    mEquipItemBox.setEIBBoxInfo((u32)&mItemBoxInfo);
    void* a8 = ResolveEquipItem();
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    mEquipItemBox.eibOpenPages(packed, a8, 0);
    mEquipItemBox.openEIBBox();
    field_48 = 7;
}

// Target us-80205c7c: when the box sub-page closes, enter state 8.
void CEquipChange::OnSubPageClose() {
    if (mEquipItemBox.getEIBActiveMark() != 0) {
        field_48 = 8;
        field_4D = 1;
    }
}

// Target us-80205cc4: when the box sub-page gate is open, refresh the equip
// info window with the packed selection word and enter state 0xA.
// -O4,s keeps the retail stmw r28 frame; the packed word is built low-to-high
// ((sub&0xF) | (main&0xF)<<4 | f99<<8) so MWCC merges the ORs into rlwimi.
void CEquipChange::OnApplyToStA() {
    if (mEquipItemBox.getEIBActiveMark() == 0)
        return;
    field_48 = 0xA;
    u8 f99 = field_99;
    int lo = MapCursorToCat();
    int hi = MapCursorToSlot();
    int packed = ((lo & 0xF) << 4) | (hi & 0xF);
    int m = (int)ResolveEquipItem();
    mItemBoxInfo.SetItem((u16)(packed | ((u32)f99 << 8)), (void*)m, 1);}

// Target us-80205d44: when the 0x40 anim finishes, enter state 3 and refresh.
void CEquipChange::OnAnim40ToSt3() {
    if (AnimRewindFrame(field_40, lbl_eu_806682A8) != 0) {
        field_48 = 3;
        field_4D = 1;
        RefreshCursorPos();
    }
}

// Target us-80205d98: when both file handles (0x34, 0x30) are set, load the
// retail bind data and clear the cursor-run flag 0x98.
void CEquipChange::MaybeRebuild() {
    if (field_34 == 0)
        goto end;
    if (field_30 != 0)
        goto body;
    goto end;
body:
    RebuildCursorList();
    field_4C = 1;
    field_44 = 1;
    field_98 = 0;
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
void CEquipChange::RefreshCursorPos() {
    nw4r::math::VEC3 pos;
    // Word view of the 7-pair block: the cursor byte selects one of the 14
    // words. Same 0x38-byte stack slot, so the frame layout is unchanged.
    union {
        CEquipColorPair pair[7];
        u32 word[14];
    } palette;
    // 0x38-byte block copy (7 pairs of 8 bytes) - the block-copy path emits
    // the retail lwzu/stwu counted loop.
    *(CEquipPaletteBlock*)&palette = *(const CEquipPaletteBlock*)lbl_eu_80508120;

    // The retail reloads the sub-object pointer (field_34 -> +0x10) at every
    // call site, so keep the access inline (a temp would live in a callee-saved
    // register across the calls).
    CEquipColorPair scale;
    nw4r::lyt::Pane* pane =
        ((nw4r::lyt::Layout*)field_34)->GetRootPane()->FindPaneByName((const char*)palette.word[field_98], true);
    AccumPaneWorldPos(&pos, pane);

    pane = ((nw4r::lyt::Layout*)field_34)->GetRootPane()->FindPaneByName(lbl_eu_80508168 + 0x3E, true);
    scale = ((const CPaneScaleView*)pane)->mScale;
    pos.x *= *(const float*)&scale.a;
    pos.y *= *(const float*)&scale.b;

    pane = ((nw4r::lyt::Layout*)field_34)->GetRootPane()->FindPaneByName(lbl_eu_80508168 + 0x46, true);
    *(float*)&scale.a = pane->GetScale().x;
    *(float*)&scale.b = pane->GetScale().y;
    pos.x *= *(const float*)&scale.a;

    if (field_98 == 0 || field_98 == 4 || field_98 == 6 ||
        field_98 == 8 || field_98 == 10 || field_98 == 12) {
        mCur14.setRootPaneTranslate(&pos);
        mCur14.SetVisible(1);
        mCur15.SetVisible(0);
    } else {
        mCur15.setRootPaneTranslate(&pos);
        mCur15.SetVisible(1);
        mCur14.SetVisible(0);
    }
}


// Target us-80205fb8: full refresh of the equip-change screen. Reads the
// current character's equipped rows out of the bdat character data, resolves
// each category row to an item through CItem_kindPtr, then walks the layout
// panes: the three weapon-slot panes (loop 1) show either the directly
// equipped item (impl v14 slot lookup), a bdat-rebuilt entry (impl v9 packed
// record), or an empty label; the five armour-category panes (loop 2) mirror
// the same three-way split using the category items cached up front. Every
// processed entry is appended to the cursor record array handed to
// EquipChange_CopyCursorRec so the cursors land on real entries only.
void EquipChange_CopyCursorRec(CEqChCursorRec* dst, CEqChCursorRec* src);
void CEquipChange::RebuildCursorList() {
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
    u8* marks = mSlotMarks;
    int count = 0;
    int nRecs = 0;

    CBdatCharData* cd = (CBdatCharData*)CtrlObjectParam_GetCharEntry(GetCollectedFlagByte(field_99));
    CtrlObjectParam_SyncParamFromActorEx(cd, 1);
    // Stats sub-object at +0x17C is an embedded cf::CActorParam; refresh it
    // through its real +0x224 slot (CActorParam_getBattleParams), not a
    // manual vtable-word call (CfNandManager subSlot precedent).
    ((cf::CActorParam*)((u8*)cd + 0x17C))->CActorParam_getBattleParams();
    CtrlObjectParam_GetArtsStatsRow(&cd->field_1C, 5);

    s16 w4 = cd->field_1C;
    s16 w5 = cd->field_1E;
    s16 w6 = cd->field_20;
    s16 w7 = cd->field_22;
    s16 w8 = cd->field_24;

    void* weaponItem = CItem_kindPtr(2, cd->field_26);
    void* catItems[5];
    catItems[0] = CItem_kindPtr(4, w4);
    catItems[1] = CItem_kindPtr(5, w5);
    catItems[2] = CItem_kindPtr(6, w6);
    catItems[3] = CItem_kindPtr(7, w7);
    catItems[4] = CItem_kindPtr(8, w8);

    char* base = lbl_eu_80508168;
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)field_34;
    LayoutSetTextBoxFmtValue(layout, &base[0x55], &base[0x5f], 0);
    LayoutSetTextBoxFmtValue(layout, &base[0x60], &base[0x5f], 0);
    LayoutSetTextBoxFmtValue(layout, &base[0x6a], &base[0x5f], 0);
    LayoutSetTextBoxFmtValue(layout, &base[0x74], &base[0x5f], 0);
    LayoutSetTextBoxFmtValue(layout, &base[0x7e], &base[0x5f], 0);
    LayoutSetTextBoxFmtValue(layout, &base[0x88], &base[0x5f], 0);

    if (weaponItem != NULL) {
        LayoutSetTextBoxFmtValue(layout, &base[0x92],
                      CItem_initItemImplInstances((CItemData*)weaponItem)->vf20((CItemData*)weaponItem), 0);
        LayoutSetTextBoxFmtValue(layout, &base[0x9f],
                      CItem_initItemImplInstances((CItemData*)weaponItem)->vf20((CItemData*)weaponItem), 0);
        u8 equipped = CItem_initItemImplInstances((CItemData*)weaponItem)->vf30((CItemData*)weaponItem);
        if (equipped != 0) {
            LayoutSetTextBoxFmtValue(layout, &base[0x55], getErrMesText16(), 0);
            PaneMatSetTevColorsByName(layout, &base[0x55], (const GXColorS10*)&sEquipCursorColorA0, (const GXColorS10*)&sEquipCursorColorB0);
        }

        for (int i = 0; i < 3; i++) {
            void* texRes;
            sprintf(iconName1, &base[0xae], i);
            sprintf(labelName1, &base[0xbf], i);
            sprintf(imageName1, &base[0xd2], i);
            sprintf(paneName1, &base[0xdf], i);
            nw4r::lyt::Pane* pane =
                ((nw4r::lyt::Layout*)field_34)->GetRootPane()->FindPaneByName(paneName1, true);
            setPaneVisible(pane, 1);
            texRes = field_2C->GetResource(0x74696D67, &base[0xec], NULL);
            if ((u8)i < equipped) {
                int slot = (s16)CItem_initItemImplInstances((CItemData*)weaponItem)->vf40((CItemData*)weaponItem, (u8)i);
                if (slot == -1) {
                    // bdat-rebuild path: packed equip record from impl v9
                    CEquipV9Result* res =
                        (CEquipV9Result*)CItem_initItemImplInstances((CItemData*)weaponItem)->vf2C((CItemData*)weaponItem, 0);
                    if (res != NULL && (res->field_04 & 1)) {
                        u32 packed = res->field_00;
                        marks[count++] = 2;
                        tmp.field_00 = (u16)((res->field_04 >> 16) & 0xFFF);
                        tmp.field_04 = (u32)(s16)((packed >> 11) & 0x7FF);
                        void** arrPtr = &lbl_eu_806640D8_arr[count - 1];
                        void* tbl = *arrPtr;
                        EquipChange_CopyCursorRec(&recs[nRecs++], &tmp);
                        char* lbl = BdatTouchStringCell(
                            &base[0xff], &base[0x108], 0x1E - (((packed >> 22) & 7) - 1));
                        char* nm = BdatGetPtrDirect(tbl, &base[0x108], tmp.field_00);
                        sprintf(textBuf1, &base[0x10d], nm, lbl);
                        u8 code = BdatGetU8Direct((const void*)tbl, &base[0x112], tmp.field_00);
                        switch (code) {
                        case 0:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x11b], NULL);
                            break;
                        case 4:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x131], NULL);
                            break;
                        case 5:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x147], NULL);
                            break;
                        case 6:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x15d], NULL);
                            break;
                        case 7:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x173], NULL);
                            break;
                        case 8:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x189], NULL);
                            break;
                        case 9:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x19f], NULL);
                            break;
                        default:
                            break;
                        }
                        LayoutSetTextBoxFmtValue(layout, &base[0x55], getErrMesText15(), 0);
                        PaneMatSetTevColorsByName(layout, &base[0x55], (const GXColorS10*)&sEquipCursorColorA1, (const GXColorS10*)&sEquipCursorColorB1);
                    } else {
                        marks[count++] = 1;
                        char* lbl = BdatTouchStringCell(&base[0xff], &base[0x108], 0x2a);
                        sprintf(textBuf1, &base[0x1b5], lbl);
                    }
                } else {
                    void* item = CItem_kindPtr(3, (s16)slot);
                    if (item != NULL) {
                        marks[count++] = 3;
                        tmp.field_04 = (u32)CItem_initItemImplInstances((CItemData*)item)->vf90((CItemData*)item);
                        tmp.field_00 = BdatGetItemId(*(u32*)item >> 20);
                        EquipChange_CopyCursorRec(&recs[nRecs++], &tmp);
                        u8 cat = CItem_initItemImplInstances((CItemData*)item)->vf08((CItemData*)item);
                        char* lbl = BdatTouchStringCell(&base[0xff], &base[0x108], 0x1E - (cat - 1));
                        sprintf(textBuf1, &base[0x10d],
                                CItem_initItemImplInstances((CItemData*)item)->vf20((CItemData*)item), lbl);
                        u32 iconRow =
                            CItem_initItemImplInstances((CItemData*)item)->vf54((CItemData*)item);
                        void* tbl = lbl_eu_806640D8_arr[cat - 1];
                        u8 code =
                            BdatGetU8Direct((const void*)tbl, &base[0x112], BdatGetItemId(iconRow & 0xFFFF));
                        switch (code) {
                        case 0:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x11b], NULL);
                            break;
                        case 4:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x131], NULL);
                            break;
                        case 5:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x147], NULL);
                            break;
                        case 6:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x15d], NULL);
                            break;
                        case 7:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x173], NULL);
                            break;
                        case 8:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x189], NULL);
                            break;
                        case 9:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x19f], NULL);
                            break;
                        default:
                            break;
                        }
                    }
                }
            } else {
                marks[count++] = 0;
                char* lbl = BdatTouchStringCell(&base[0xff], &base[0x108], 0x6d);
                sprintf(textBuf1, &base[0x1b5], lbl);
                setPaneVisible(pane, 0);
            }
            LayoutSetTextBoxText(layout, iconName1, textBuf1, 0);
            LayoutSetTextBoxText(layout, labelName1, textBuf1, 0);
            if (texRes != NULL)
                PaneSetTexPaletteByName(layout, (const char*)imageName1, (TPLPalette*)((u32)texRes));
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
            ((nw4r::lyt::Layout*)field_34)->GetRootPane()->FindPaneByName(paneName3, true);
        setPaneVisible(pane, 1);
        texRes = field_2C->GetResource(0x74696D67, &base[0xec], NULL);
        void* item = catItems[i];
        if (item == NULL) {
            char* lbl = BdatTouchStringCell(&base[0xff], &base[0x108], 0x6d);
            LayoutSetTextBoxFmtValue(layout, iconName2, lbl, 0);
            LayoutSetTextBoxFmtValue(layout, labelName2, lbl, 0);
            marks[count++] = 0;
            sprintf(textBuf2, &base[0x1b5], lbl);
            setPaneVisible(pane, 0);
        } else {
            LayoutSetTextBoxFmtValue(layout, iconName2,
                          CItem_initItemImplInstances((CItemData*)item)->vf20((CItemData*)item), 0);
            LayoutSetTextBoxFmtValue(layout, labelName2,
                          CItem_initItemImplInstances((CItemData*)item)->vf20((CItemData*)item), 0);
            if (CItem_initItemImplInstances((CItemData*)item)->vf30((CItemData*)item) != 0) {
                sprintf(rebuildName, &base[0x1d8], i + 1);
                LayoutSetTextBoxFmtValue(layout, rebuildName, getErrMesText16(), 0);
                PaneMatSetTevColorsByName(layout, rebuildName, (const GXColorS10*)&sEquipCursorColorA0, (const GXColorS10*)&sEquipCursorColorB0);
                int slot =
                    (s16)CItem_initItemImplInstances((CItemData*)item)->vf40((CItemData*)item, 0);
                if (slot == -1) {
                    CEquipV9Result* res =
                        (CEquipV9Result*)CItem_initItemImplInstances((CItemData*)item)->vf2C((CItemData*)item, 0);
                    if (res != NULL && (res->field_04 & 1)) {
                        u32 packed = res->field_00;
                        marks[count++] = 2;
                        tmp.field_00 = (u16)((res->field_04 >> 16) & 0xFFF);
                        tmp.field_04 = (u32)(s16)((packed >> 11) & 0x7FF);
                        void* tbl = lbl_eu_806640D8_arr[count - 1];
                        EquipChange_CopyCursorRec(&recs[nRecs++], &tmp);
                        char* lbl = BdatTouchStringCell(
                            &base[0xff], &base[0x108], 0x1E - (((packed >> 22) & 7) - 1));
                        char* nm = BdatGetPtrDirect(tbl, &base[0x108], tmp.field_00);
                        sprintf(textBuf2, &base[0x10d], nm, lbl);
                        u8 code = BdatGetU8Direct((const void*)tbl, &base[0x112], tmp.field_00);
                        switch (code) {
                        case 0:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x11b], NULL);
                            break;
                        case 4:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x131], NULL);
                            break;
                        case 5:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x147], NULL);
                            break;
                        case 6:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x15d], NULL);
                            break;
                        case 7:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x173], NULL);
                            break;
                        case 8:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x189], NULL);
                            break;
                        case 9:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x19f], NULL);
                            break;
                        default:
                            break;
                        }
                        LayoutSetTextBoxFmtValue(layout, rebuildName, getErrMesText15(), 0);
                        PaneMatSetTevColorsByName(layout, rebuildName, (const GXColorS10*)&sEquipCursorColorA1, (const GXColorS10*)&sEquipCursorColorB1);
                    } else {
                        marks[count++] = 1;
                        char* lbl = BdatTouchStringCell(&base[0xff], &base[0x108], 0x2a);
                        sprintf(textBuf2, &base[0x1b5], lbl);
                    }
                } else {
                    void* item2 = CItem_kindPtr(3, (s16)slot);
                    if (item2 != NULL) {
                        marks[count++] = 3;
                        tmp.field_04 =
                            (u32)CItem_initItemImplInstances((CItemData*)item2)->vf90((CItemData*)item2);
                        tmp.field_00 = BdatGetItemId(*(u32*)item2 >> 20);
                        EquipChange_CopyCursorRec(&recs[nRecs++], &tmp);
                        u8 cat = CItem_initItemImplInstances((CItemData*)item2)->vf08((CItemData*)item2);
                        char* lbl = BdatTouchStringCell(&base[0xff], &base[0x108], 0x1E - (cat - 1));
                        sprintf(textBuf2, &base[0x10d],
                                CItem_initItemImplInstances((CItemData*)item2)->vf20((CItemData*)item2),
                                lbl);
                        u32 iconRow =
                            CItem_initItemImplInstances((CItemData*)item2)->vf54((CItemData*)item2);
                        void* tbl = lbl_eu_806640D8_arr[cat - 1];
                        u8 code =
                            BdatGetU8Direct((const void*)tbl, &base[0x112], BdatGetItemId(iconRow & 0xFFFF));
                        switch (code) {
                        case 0:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x11b], NULL);
                            break;
                        case 4:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x131], NULL);
                            break;
                        case 5:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x147], NULL);
                            break;
                        case 6:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x15d], NULL);
                            break;
                        case 7:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x173], NULL);
                            break;
                        case 8:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x189], NULL);
                            break;
                        case 9:
                            texRes = ((nw4r::lyt::ArcResourceAccessor*)field_30)->GetResource(0x74696D67, &base[0x19f], NULL);
                            break;
                        default:
                            break;
                        }
                        LayoutSetTextBoxFmtValue(layout, rebuildName, getErrMesText15(), 0);
                        PaneMatSetTevColorsByName(layout, rebuildName, (const GXColorS10*)&sEquipCursorColorA1, (const GXColorS10*)&sEquipCursorColorB1);
                    }
                }
            } else {
                marks[count++] = 0;
                char* lbl = BdatTouchStringCell(&base[0xff], &base[0x108], 0x6d);
                sprintf(textBuf2, &base[0x1b5], lbl);
                setPaneVisible(pane, 0);
            }
        }
        LayoutSetTextBoxText(layout, paneName2, textBuf2, 0);
        LayoutSetTextBoxText(layout, labelName3, textBuf2, 0);
        if (texRes != NULL)
            PaneSetTexPaletteByName(layout, (const char*)imageName2, (TPLPalette*)((u32)texRes));
    }
}

void EquipChange_CopyCursorRec(CEqChCursorRec* dst, CEqChCursorRec* src) {
    u16 a = src->field_00;
    u32 b = src->field_04;
    dst->field_04 = b;
    dst->field_00 = a;
}

// Target us-802073c8: file-load event handler for the two bind files loaded
// by EquipChange_LoadBindFiles. Handle-1 branch builds the whole equip-change screen:
// 0xE000 scratch region, arc accessor, main layout + three anim transforms,
// shared text object seeding six labels, animation enables, cursor palette
// colours copied into the sdata2 tables, and the three cursors built from
// stack temporaries. Handle-2 branch only attaches the second accessor and
// caches it in the global page flag.
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
        mRegion04.createRegion(memHandle,
                                                 0xE000, &base[0x1ED], 1);
        Class_8045F858 host(&mRegion04);
        CFileHandle* handle = (CFileHandle*)field_24;
        data = handle->getData();
        mtl::MemManager::setMemInitFlag(false);
        field_2C = CLibLayout::createArcResourceAccessor();
        field_2C->Attach(data, &base[0x1FA]);
        buildLayout((nw4r::lyt::Layout**)&field_34, field_2C, &base[0x1FE]);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)field_34, &field_38, field_2C, &base[0x20E]);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)field_34, &field_3C, field_2C, &base[0x221]);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)field_34, &field_40, field_2C, &base[0x239]);
        nw4r::lyt::Pane* root = ((nw4r::lyt::Layout*)field_34)->GetRootPane();
        void* fontObj = CDeviceFont::getFontInfo(
            1, (nw4r::lyt::Layout*)field_34);
        ApplyFontToTxtPanes(root, (const nw4r::ut::Font*)(static_cast<IDeviceFontInfo*>(fontObj)->getFont()));

        // Seed the label textboxes with the shared text object.
        char* text = (char*)CUICfManager_getPackedFont9C();
        if (text != NULL) {
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x55], (u32)text);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x60], (u32)text);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x6A], (u32)text);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x74], (u32)text);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x7E], (u32)text);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)field_34, &base[0x88], (u32)text);
        }

        ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_3C, 0);
        ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_40, 0);
        ((nw4r::lyt::Layout*)field_34)->SetAnimationEnable(field_38, 1);
        ((nw4r::lyt::Layout*)field_34)->Animate(0);

        // Cursor colour palette: read both highlight pairs off the pane and
        // copy them into the sdata2 colour tables, propagating the alpha
        // halves to the paired entries.
        // Retail materializes the full symbol address here (fresh lis/addi),
        // not the cached base pointer.
        scratch = ((nw4r::lyt::Layout*)field_34)->GetRootPane()->FindPaneByName(lbl_eu_80508168 + 0x60, true);
        GXColorS10 color = PaneMatGetS16Quad((nw4r::lyt::Pane*)scratch, (u32)0);
        CopyVec4s((GXColorS10*)&sEquipCursorSrcColorA, &color);
        color = PaneMatGetS16Quad((nw4r::lyt::Pane*)scratch, (u32)1);
        CopyVec4s((GXColorS10*)&sEquipCursorSrcColorB, &color);
        // Alpha halves propagate to the paired colour entries; retail keeps
        // each table address in its own register (pointer locals).
        char* colA = (char*)&sEquipCursorSrcColorA;
        char* colB = (char*)&sEquipCursorSrcColorB;
        s16 alpha0 = *(s16*)(colA + 6);
        char* dstA = (char*)&sEquipCursorColorA0;
        char* dstC = (char*)&sEquipCursorColorA1;
        s16 alpha1 = *(s16*)(colB + 6);
        char* dstB = (char*)&sEquipCursorColorB0;
        char* dstD = (char*)&sEquipCursorColorB1;
        *(s16*)(dstA + 6) = alpha0;
        *(s16*)(dstB + 6) = alpha1;
        *(s16*)(dstC + 6) = alpha0;
        *(s16*)(dstD + 6) = alpha1;

        // Build each cursor from a stack temporary, copy it into place, run
        // its reset hook, then advance to the next one. Retail reloads the
        // accessor from field_2C at every use (never cached in a register).
        mCur14 = CCur14(field_2C);
        mCur14.initLayout();

        mCur15 = CCur15(field_2C);
        mCur15.initLayout();

        mSubCur = CSubCur(field_2C);
        mSubCur.initLayout();

        mSubCur.ShowTwoSubPanes(0);
        MaybeRebuild();
        field_24 = 0;
        mRegion04.validateHeap();
        return true;
    }
    if (field_28 == (u32)file->mFileHandle) {
        int memHandle = (int)mtl::MemManager::getHandleMEM2();
        base = lbl_eu_80508168;
        mRegion14.createRegion(memHandle,
                                                 0x100, &base[0x250], 1);
        Class_8045F858 host(&mRegion14);
        CFileHandle* handle = (CFileHandle*)field_28;
        scratch = handle->getData();
        mtl::MemManager::setMemInitFlag(false);
        nw4r::lyt::ArcResourceAccessor* acc = CLibLayout::createArcResourceAccessor();
        field_30 = (u32)acc;
        acc->Attach(scratch, &base[0x1FA]);
        // Cache the second accessor in the global equip-page flag slot.
        sEquipChangeAccessor = (u32)field_30;
        MaybeRebuild();
        field_28 = 0;
        mRegion14.validateHeap();
        return true;
    }
    return false;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Target us-802077c8: static init - build the six equip-change colour/state
// palette entries (sdata2) used by the item box UI.
void sinit_802059E8() {
    SplitU32ToS16s(&sEquipCursorSrcColorA, 0);
    SplitU32ToS16s(&sEquipCursorSrcColorB, 0);
    setGXColorS10(&sEquipCursorColorA0, 0x12, 0xa3, 0xe7, 0);
    setGXColorS10(&sEquipCursorColorB0, 0xff, 0xff, 0xff, 0);
    setGXColorS10(&sEquipCursorColorA1, 0xb3, 0x9, 0xc0, 0);
    setGXColorS10(&sEquipCursorColorB1, 0xff, 0xff, 0xff, 0);
}

// --- function definitions kept at end of TU so earlier callers emit `bl` ----

// Target us-80204bac: apply the equip change for the given category. Packs
// the item-box info (calcItemBoxPaneVec), drives the sub-cursor vtable[0x10] hook
// with the packed buffer and activates the sub-cursor, refreshes the item
// info window, then resolves the item under the cursor: category 3 re-equips
// the weapon row (obj+0x26) unless the equipped count is below the selection,
// otherwise it re-equips the sub-cursor row via the 0x806682A0 category table
// and the obj u16 id fields. When the cursor cannot be placed (flag 0) the
// refresh block re-runs. Frame saves 7 regs via stmw r25 (opt-space).
void CEquipChange::RefreshCategory(u8 cat) {
    field_99 = cat;
    // Local block mirrors the retail frame: {word,byte} pair at 0x8/0xC,
    // item-box info pack at 0x10, per-category id row at 0x1C.
    u32 catWord;
    u8 catByte;
    s16 ids[5];
    nw4r::math::VEC3 buf = mItemBoxInfo.GetSlotPos(cat);
    mSubCur.setRootPaneTranslate(&buf);
    mSubCur.SetActive();
    RebuildCursorList();
    mItemBoxInfo.HidePanes((u8)MapCursorToCat());
    u8 f99 = field_99;
    int cur38 = MapCursorToCat();
    int cur3a = MapCursorToSlot();
    if ((u8)cur38 == 3) {
        int flag = 0;
        u8* obj = (u8*)CtrlObjectParam_GetCharEntry((u8)GetCollectedFlagByte(field_99));
        if ((u8)cur3a <= 3) {
            s16 id = *(s16*)(obj + 0x26);
            if (id == -1) {
                field_98 = 0;
            } else {
                void* item = CItem_kindPtr(2, id);
                if (item == NULL || *(u32*)item == 0) {
                    field_98 = 0;
                } else {
                    CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
                    u8 equipped = inst->vf30((CItemData*)item);
                    if (equipped < (u8)cur3a)
                        field_98 = (s8)equipped;
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
                void* item = CItem_kindPtr(((u8*)&catWord)[idx], id);
                if (item != NULL && *(u32*)item != 0) {
                    CItemImpl* inst = CItem_initItemImplInstances((CItemData*)item);
                    if (inst->vf30((CItemData*)item) != 0)
                        flag = 1;
                }
            }
            if (flag == 0)
                field_98 = (s8)(idx * 2 + 4);
        }
        if (flag == 0) {
            RebuildCursorList();
            mItemBoxInfo.HidePanes((u8)MapCursorToCat());
            RefreshCursorPos();
            f99 = field_99;
            cur38 = MapCursorToCat();
            cur3a = MapCursorToSlot();
        }
    }
    u16 packed = (u16)(((cur38 & 0xF) << 4) | (cur3a & 0xF));
    packed = (u16)(packed | ((u32)f99 << 8));
    mItemBoxInfo.SetItem(packed, ResolveEquipItem(), 1);
    if (mEquipItemBox.getEIBOpenFlag() != 0)
        mEquipItemBox.eibOpenPages(packed, 0, 1);
}

// Target us-80207088: resolve the item record under the current equip-change
// selection. Non-weapon categories look the per-category equipped id up in
// the bdat row and return CItem_kindPtr(cat, id). The weapon-row path (cat 3)
// maps the sub-cursor index to a category, fetches the item, then either
// equips it through the item-impl vtable hooks or rebuilds the scratch item
// sEquipChangeWorkItem from bdat (guarded by sEquipWorkItemInitialized) and returns it.
// bdat-rebuild fallback shape needs the opt-space stmw r28 frame.
void* CEquipChange::ResolveEquipItem() {
    // cat declared first - its home web (shared with mapped) gets priority.
    int cat;
    CBdatCharData* obj = (CBdatCharData*)CtrlObjectParam_GetCharEntry((u8)GetCollectedFlagByte(field_99));
    cat = MapCursorToCat();
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
        return CItem_kindPtr((u8)cat, id);
    } else {
    // Weapon-row path: map the sub-cursor index to a category. Indices 1..3
    // collapse to category 2; 4..8 pass through; anything else stays 3.
    int idx = MapCursorToSlot();
    u8 slot = mSlotMarks[(u8)idx - 1];
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
    void* item = CItem_kindPtr((u8)mapped, wid);
    if (item == NULL)
        return 0;
    if (*(u32*)item == 0)
        return 0;
    CItemImpl* impl = CItem_initItemImplInstances((CItemData*)item);
    CEquipV9Result* res =
        (CEquipV9Result*)impl->vf2C((CItemData*)item, (u32)flag);
    if (res != NULL && (res->field_04 & 1) == 0) {
        // Direct-equip path: second hook returns the crystal id to equip.
        u32 r = impl->vf40((CItemData*)item, (u8)flag);
        return CItem_kindPtr(3, (s16)r);
    }
    if (res == NULL)
        return 0;
    // bdat-rebuild fallback: clear the scratch item once, find the last bdat
    // row whose string column reads 3, rebuild sEquipChangeWorkItem from it.
    if (sEquipWorkItemInitialized == 0) {
        sEquipChangeWorkItem.field_04 = 0;
        sEquipChangeWorkItem.field_00 = 0;
        sEquipWorkItemInitialized = 1;
    }
    void* table = (void*)lbl_eu_806640EC;
    u32 cnt = Bdat_GetMaxRow_B1EC(table);
    u16 row = (u16)(cnt - 1);
    while (row != 0) {
        u32 val = getBdatStringColumnValue(table, &lbl_eu_80508168[0x1E4], (s32)row);
        if ((u16)val == 3)
            break;
        row--;
    }
    u32 w = res->field_00;
    u16 h = res->field_04;
    CItem_initSlot((CItemData*)&sEquipChangeWorkItem, row, ((u32)h >> 16) & 0xFFF, (w >> 22) & 7);
    CItemImpl* impl2 = CItem_initItemImplInstances((CItemData*)&sEquipChangeWorkItem);
    impl2->vf94((CItemData*)&sEquipChangeWorkItem, (s16)((w >> 11) & 0x7FF));
    return &sEquipChangeWorkItem;
    }
}

// Target us-80204e30: field_98 (s8) maps 0/4/6/8/10/12 -> 2/4/5/6/7/8,
// anything else -> 3. Free function (retail symbol is unmangled, hence C
// linkage).
// Retail emits external `bl` calls to this helper at every call site, so
// keeps the body standalone under -inline auto / -ipa.
int CEquipChange::MapCursorToCat() {
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
int CEquipChange::MapCursorToSlot() {
    switch (field_98) {
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
// The state handlers are matched targets defined above; their extern "C"
// definitions make the call relocs use the unmangled retail names.
void CEquipChange::AdvanceScreen() {
    if (field_44 == 0)
        return;
    // Retail jumptable_eu_805355B0 routing: states 0/3/8 (and >10) are
    // idle; the eight anim-waiter handlers sit on 1,2,4,5,6,7,9,10. The
    // interleaved no-op rows keep the dense table bound at 10.
    switch (field_48) {
    case 1: OnAnim38ToSt2(); break;
    case 2: OnAnim3CToSt3(); break;
    case 4: OnAnim3CToSt5(); break;
    case 5: OnAnim38ToIdle(); break;
    case 6: OnAnim40ToSt7(); break;
    case 7: OnSubPageClose(); break;
    case 9: OnApplyToStA(); break;
    case 10: OnAnim40ToSt3(); break;
    default:
        goto tail;
    }
tail:
    ((nw4r::lyt::Layout*)field_34)->Animate(0);
    mCur14.Update();
    mCur15.Update();
    mSubCur.Update();
    mItemBoxInfo.UpdateAnims();
    mEquipItemBox.updateEIBBox();
}
// Target us-80203edc: draw the equip-change screen. Draws the main layout
// (field_34) when visible (0x44), then the two cursors (0x50/0x68) only when
// the sub-cursor (0x80) is idle, flags the item box (0x62B), and finally the
// item info window + sub-cursor + item box. Retail frame uses stmw/lmw.
void CEquipChange::DrawLayouts(nw4r::lyt::DrawInfo* drawInfo) {
    if (field_44 == 0)
        return;
    drawLayout((nw4r::lyt::Layout*)field_34, drawInfo, 0, 1);
    if (mSubCur.BothSubPanesOn() == 0) {
        mCur14.DrawLayout(drawInfo);
        mCur15.DrawLayout(drawInfo);
        mEquipItemBox.unk_37b = 1;
    } else {
        mEquipItemBox.unk_37b = 0;
    }
    mItemBoxInfo.DrawLayout(drawInfo);
    mSubCur.DrawLayout(drawInfo);
    mEquipItemBox.drawEIBBox(drawInfo);
}
// Target us-80203f84: teardown of the equip-change screen. Release the two
// file handles, clear the visible flag, delete the layout object at 0x34
// (guarded double-check), clear the global page flag, release both arc
// accessors and the 0x04 memory region, reset the three cursors, and close
// the item info window + box.
void CEquipChange::CleanupFiles() {
    closeFileHandle((CFileHandle**)&field_24);
    closeFileHandle((CFileHandle**)&field_28);
    // Retail hoists the field_34 load above the field_44 store and reuses the
    // loaded pointer across the guarded destroy call - keeping it in a local
    // pins it to a volatile reg instead of forcing a second callee-saved.
    void* layout = (void*)field_34;
    field_44 = 0;
    if (layout != 0) {
        // Layout deleting destructor (vtable +0x08, delete flag in r4)
        // via plain delete (same shape as CItemBoxInfo::closeItemBox1).
        delete (nw4r::lyt::Layout*)layout;
        field_34 = 0;
    }
    sEquipChangeAccessor = 0;
    releaseArcResourceAccessor(field_2C);
    releaseArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)field_30);
    mRegion04.deleteRegion();
    mCur14.cleanup();
    mCur15.cleanup();
    mSubCur.cleanup();
    mItemBoxInfo.UnloadFiles();
    mEquipItemBox.closeEIBBox();
}
