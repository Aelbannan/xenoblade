// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCCrystalList
// Replace stubs with high-level C/C++ during decomp.

#define IWORK_EVENT_INLINE_DTOR // ~CMCCrystalList elides the empty base dtor (retail)
#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/makecrystal/CMCCrystalList.hpp"

#include <cstdio>
#include <nw4r/lyt.h>
#include "monolib/util/MemManager.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CEventFile.hpp"

#include <revolution/os/OSError.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// Local imports. Linkage/signatures mirror the retail symbols exactly.
// Retail symbols for these layout builders are unmangled - C linkage keeps
// the call relocs bound to the retail names.
// Retail symbol for this helper is the unmangled name - keep C linkage.
extern "C" {
}

extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);

// Retail 0x80224580: constructor. Flat C-linkage free function (retail symbol
// is the unmangled __ct__CMCCrystalList, called as such from
// CModelDispMakeCrystal). Stores the retail vtable label, default-constructs
// both mem-region members, zeroes every pointer/flag field, then clears the
// 8 slot states with a rolled u8-counter loop (-O4,s shape).
CMCCrystalList::CMCCrystalList()
{
    mFileHandle1 = nullptr;
    mFileHandle2 = nullptr;
    mArcResAccessor = nullptr;
    mArcResAccessor2 = nullptr;
    mLayout = nullptr;
    mAnimTrans1 = nullptr;
    mAnimTrans2 = nullptr;
    mAnimTrans3 = nullptr;
    mAnimTrans4 = nullptr;
    mAnimRes5 = nullptr;
    mState = 0;
    mStateIdx = 0;
    mDataFlag = 0;
    mIsActive = 1;
    for (u8 i = 0; i < 8; i++) {
        mSlotStates[i] = 0;
    }
}

// UnkClass_8045F564 member dtor and operator delete (retail names).
extern "C" void __dt__17UnkClass_8045F564Fv(void* self, int flags);

// Real destructor: the two embedded region members are typed members, so MWCC
// destroys them (+0x14 then +0x04) and runs the retail delete-flag epilogue.
CMCCrystalList::~CMCCrystalList() {}

// Retail 0x80222848: start async loads of the crystal-list layout archive
// (root name) and its secondary archive (+0x1a), then clear mDataFlag.
void crystalListLoadFiles(CMCCrystalList* self)
{
    self->mFileHandle1 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), "menu/jp/MCCrystalList.arc", self, 0, 0);
    self->mFileHandle2 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), "menu/jp/tpl/CrystalIcon.arc", self, 0, 0);
    self->mDataFlag = 0;
}

// Per-state animation handlers for the crystalListDispatch dispatcher (defined below).
// extern "C" so call relocs resolve to the unmangled retail names.
extern "C" void crystalListWaitAnim1To2(CMCCrystalList* self);
extern "C" void crystalListWaitAnim2To3(CMCCrystalList* self);
extern "C" void crystalListPollSlots(CMCCrystalList* self);
extern "C" void crystalListRewindAnim2To5(CMCCrystalList* self);
extern "C" void crystalListRewindAnim1To0(CMCCrystalList* self);
extern "C" void crystalListWaitAnim3To3(CMCCrystalList* self);
extern "C" void crystalListRewindAnim3To3(CMCCrystalList* self);
extern "C" void crystalListWaitAnim4To3(CMCCrystalList* self);

// Retail 0x802246F8: state-machine dispatcher. When mState != 0, dispatch on
// mStateIdx (0..7) to the per-state animation handlers, then re-animate the
// layout (virtual Animate at vtable+0x38, r4 = 0 option). Values 8+ (and the
// switch default) fall through to the same Animate tail.
// NOTE: case 8 keeps its own label (not merged with default), otherwise MWCC
// shrinks the jump-table bound immediate from 8 to 7.
void crystalListDispatch(CMCCrystalList* self)
{
    if (self->mState != 0) {
        switch (self->mStateIdx) {
        case 0: crystalListWaitAnim1To2(self); break;
        case 1: crystalListWaitAnim2To3(self); break;
        case 2: crystalListPollSlots(self); break;
        case 3: crystalListRewindAnim2To5(self); break;
        case 4: crystalListRewindAnim1To0(self); break;
        case 5: crystalListWaitAnim3To3(self); break;
        case 6: crystalListRewindAnim3To3(self); break;
        case 7: crystalListWaitAnim4To3(self); break;
        case 8: break;
        }
        self->mLayout->Animate(0);
    }
}

namespace nw4r { namespace lyt { class DrawInfo; } }

// Retail 0x80222984: full teardown of the crystal-list UI.
extern "C" void __dt__80222984(CMCCrystalList* self) {
    closeFileHandle(&self->mFileHandle1);
    closeFileHandle(&self->mFileHandle2);
    self->mState = 0;
    if (self->mLayout != nullptr) {
        self->mLayout->UnbindAllAnimation();
    }
    if (self->mAnimRes5 != nullptr) {
        delete self->mAnimRes5;
        self->mAnimRes5 = nullptr;
    }
    if (self->mLayout != nullptr) {
        delete self->mLayout;
        self->mLayout = nullptr;
    }
    releaseArcResourceAccessor(self->mArcResAccessor);
    releaseArcResourceAccessor(self->mArcResAccessor2);
    self->mArcResAccessor = nullptr;
    self->mArcResAccessor2 = nullptr;
    self->mMemRegion1.deleteRegion();
    self->mMemRegion2.deleteRegion();
}

void CMCCrystalList::crystalListDrawIfActive(nw4r::lyt::DrawInfo* drawInfo)
{
    if (mState != 0) {
        drawLayout(*reinterpret_cast<nw4r::lyt::Layout**>(reinterpret_cast<unsigned char*>(this) + 0x34), drawInfo, 0, 1);
    }
}

u32 CMCCrystalList::crystalListGetDataFlag() { return mDataFlag; }

u32 CMCCrystalList::crystalListIsActive() { return mIsActive; }

// Forward decls for functions defined later in this TU. They are C-linkage in
// retail (unmangled symbols), so extern "C" keeps the call relocs unmangled.
extern "C" void crystalListEnableAnim1(CMCCrystalList* self);
extern "C" void crystalListDisableAnims(CMCCrystalList* self);
extern "C" void crystalListStepSlot(CMCCrystalList* self, u32 idx);
extern "C" void crystalListEnableAnim2(CMCCrystalList* self);
extern "C" void crystalListEnableAnim3(CMCCrystalList* self);
extern "C" void crystalListEnableAnim4(CMCCrystalList* self);

void crystalListState0To1(CMCCrystalList* self)
{
    if (self->mStateIdx == 0) {
        self->mStateIdx = 1;
        self->mIsActive = 0;
        crystalListEnableAnim1(self);
    }
}

void crystalListState3To6(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 6;
        self->mIsActive = 0;
        crystalListEnableAnim3(self);
    }
}

void crystalListState3To7(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 7;
        self->mIsActive = 0;
        crystalListEnableAnim3(self);
    }
}

void crystalListState3To8(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 8;
        self->mIsActive = 0;
        crystalListEnableAnim4(self);
    }
}

void crystalListState3To4(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 4;
        self->mIsActive = 0;
        crystalListEnableAnim2(self);
    }
}

// Retail 0x80222B14: stamp one crystal slot's name/count text panes, then
// look up its material/texture description in the BDAT sheet and bind it.
// kind selects which of seven texture-name strings gets resolved.
void func_80222B14(CMCCrystalList* self, u32 idx, u32 id, u8 countArg) {
    char buf[0x20];
    // BDAT name lookup happens BEFORE the slot number is computed.
    char* name = BdatGetPtrDirect((const void*)::cf::CfBdat::spBtlSkillListFileData, "name", id);
    u32 slot = idx + 1;
    sprintf(buf, "txt_listname%02d", slot);
    LayoutSetTextBoxFmtValue(self->mLayout, buf, name, 0);
    sprintf(buf, "txt_listvalue%02d", slot);
    setLayoutTextBoxNumber(self->mLayout, buf, countArg);
    sprintf(buf, "txt_listpara%02d", slot);
    char* desc = BdatTouchStringCell("MNU_item", "name", 0x21);
    LayoutSetTextBoxFmtValue(self->mLayout, buf, desc, 0);

    void* tex = NULL;
    switch (BdatGetU8Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, "atr_type", id)) {
    case 0:
        tex = self->mArcResAccessor2->GetResource(
            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
            "mf00_reg30_crys00.tpl", NULL);
        break;
    case 4:
        tex = self->mArcResAccessor2->GetResource(
            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
            "mf00_reg30_crys01.tpl", NULL);
        break;
    case 5:
        tex = self->mArcResAccessor2->GetResource(
            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
            "mf00_reg30_crys02.tpl", NULL);
        break;
    case 6:
        tex = self->mArcResAccessor2->GetResource(
            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
            "mf00_reg30_crys03.tpl", NULL);
        break;
    case 7:
        tex = self->mArcResAccessor2->GetResource(
            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
            "mf00_reg30_crys04.tpl", NULL);
        break;
    case 8:
        tex = self->mArcResAccessor2->GetResource(
            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
            "mf00_reg30_crys05.tpl", NULL);
        break;
    case 9:
        tex = self->mArcResAccessor2->GetResource(
            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
            "mf00_reg30_crys06.tpl", NULL);
        break;
    }
    if (tex != NULL) {
        sprintf(buf, "pic_ethcol%02d", slot);
        PaneSetTexPaletteByName(self->mLayout, (const char*)buf, (TPLPalette*)tex);
    }
}

// Retail 0x80222D9C: restamp every crystal slot's text panes from the BDAT
// sheet and re-resolve each slot's icon texture. When arg == 0 and the list is
// in an active state, also refresh visibility and clear the slot's active
// flag (note: index runs 1..8, so slot 0 is never cleared - retail quirk).
extern "C" void crystalListClearSlots(CMCCrystalList* self, u32 arg) {
    if (arg == 0 && self->mStateIdx >= 3) {
        crystalListDisableAnims(self);
    }
    for (u8 i = 1; i <= 8; i++) {
        char buf[0x28];
        sprintf(buf, "txt_listname%02d", i);
        LayoutSetTextBoxFmtValue(self->mLayout, buf, "", 0);
        sprintf(buf, "txt_listvalue%02d", i);
        LayoutSetTextBoxFmtValue(self->mLayout, buf, "", 0);
        sprintf(buf, "txt_listpara%02d", i);
        LayoutSetTextBoxFmtValue(self->mLayout, buf, "", 0);

        void* tex = self->mArcResAccessor->GetResource(
            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
            "mf00_com00_dmy.tpl", NULL);
        if (tex != NULL) {
            sprintf(buf, "pic_ethcol%02d", i);
            PaneSetTexPaletteByName(self->mLayout, (const char*)buf, (TPLPalette*)tex);
        }

        if (arg == 0 && self->mStateIdx >= 3) {
            sprintf(buf, "pic_heat%02d", i);
            setPaneVisible(
                self->mLayout->GetRootPane()->FindPaneByName(buf, true), 0);
            AnimJumpToLast(
                self->mLayout->GetRootPane()->FindPaneByName(buf, true),
                self->mAnimRes5);
            self->mSlotStates[i] = 0;
        }
    }
    self->mLayout->Animate(0);
}

// Retail 0x80222F64: format the pane name for slot idx+1, look up two panes
// through the layout root (a fixed label pane and the formatted slot pane),
// then bind them via ScalePaneWorldPos.
void crystalListAlignSlot(CMCCrystalList* self, int idx)
{
    char buf[0x28];
    sprintf(buf, "nul_list%02d", idx + 1);
    nw4r::lyt::Pane* rootPane = self->mLayout->GetRootPane();
    void* paneA = rootPane->FindPaneByName("nul_proportion", true);
    void* paneB = rootPane->FindPaneByName(buf, true);
    ScalePaneWorldPos((nw4r::math::VEC3*)self, (nw4r::lyt::Pane*)paneB, (nw4r::lyt::Pane*)paneA, rootPane);
}

// Retail 0x80223004: resolve the three text/icon panes for slot idx+1, then
// apply a state update selected by `kind` (0/1/2) with threshold `val`:
// kind 0: >=200 -> mark slot active, bind anim, optionally bind 'itmg' texture
//         < 200 -> clear slot, bind anim only
// kind 1: >=300 -> like kind 0 but with the second texture name;
//         < 300 -> clear slot and (if texture found) rebind via AnimJumpToLast
// kind 2: always clear slot; optional texture + AnimJumpToLast rebind.
// Finally toggles pane visibility: first two panes follow `result`, the third
// gets its inverse.
void crystalListUpdateSlot(CMCCrystalList* self, u32 idx, u32 val, int kind) {
    u32 slot = idx + 1;
    nw4r::lyt::Pane* paneA;
    nw4r::lyt::Pane* paneB;
    nw4r::lyt::Pane* paneC;
    u32 result;
    char buf[0x20];

    sprintf(buf, "txt_listpara%02d", slot);
    paneA = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
    sprintf(buf, "txt_listvalue%02d", slot);
    paneB = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
    sprintf(buf, "pic_heat%02d", slot);
    paneC = self->mLayout->GetRootPane()->FindPaneByName(buf, true);

    result = 1;
    switch (kind) {
    case 0:
        if (val >= 200) {
            self->mSlotStates[idx] = 1;
            crystalListDisableAnims(self);
            AnimResetToFirst(paneC, self->mAnimRes5);
            void* tex = self->mArcResAccessor->GetResource(
                0x74696D67, "mf10_cry00_heat.tpl", NULL); // 'itmg'
            if (tex != NULL) {
                PaneSetTexPalette(paneC, (TPLPalette*)tex);
            }
            result = 0;
        } else {
            self->mSlotStates[idx] = 0;
            crystalListDisableAnims(self);
            AnimResetToFirst(paneC, self->mAnimRes5);
        }
        break;
    case 1:
        if (val >= 300) {
            self->mSlotStates[idx] = 1;
            crystalListDisableAnims(self);
            AnimResetToFirst(paneC, self->mAnimRes5);
            void* tex = self->mArcResAccessor->GetResource(
                0x74696D67, "mf10_cry00_mega.tpl", NULL);
            if (tex != NULL) {
                PaneSetTexPalette(paneC, (TPLPalette*)tex);
            }
            result = 0;
        } else {
            self->mSlotStates[idx] = 0;
            void* tex = self->mArcResAccessor->GetResource(
                0x74696D67, "mf10_cry00_heat.tpl", NULL);
            if (tex != NULL) {
                PaneSetTexPalette(paneC, (TPLPalette*)tex);
                crystalListDisableAnims(self);
                AnimJumpToLast(paneC, self->mAnimRes5);
            }
            result = 0;
        }
        break;
    case 2:
        self->mSlotStates[idx] = 0;
        {
            void* tex = self->mArcResAccessor->GetResource(
                0x74696D67, "mf10_cry00_mega.tpl", NULL);
            if (tex != NULL) {
                PaneSetTexPalette(paneC, (TPLPalette*)tex);
                crystalListDisableAnims(self);
                AnimJumpToLast(paneC, self->mAnimRes5);
            }
        }
        result = 0;
        break;
    }

    setPaneVisible(paneA, result);
    setPaneVisible(paneB, result);
    setPaneVisible(paneC, !result);
}

// Retail 0x802232E4: reset anim trans 4's frame to 0, then re-animate the
// layout (virtual Animate at vtable+0x38, r4 = 0 option).
void crystalListResetAnim4Play(CMCCrystalList* self)
{
    crystalListEnableAnim4(self);
    self->mAnimTrans4->SetFrame(0.0f);
    self->mLayout->Animate(0);
}

// Retail 0x80223334: switch to anim set 3 (crystalListEnableAnim3), rewind anim trans
// 3 to (frame-size - sentinel) frames, then re-animate the layout.
void crystalListResetAnim3(CMCCrystalList* self)
{
    crystalListEnableAnim3(self);
    float f = self->mAnimTrans3->GetFrameSize();
    f = f - 1.0f;
    self->mAnimTrans3->SetFrame(f);
    self->mLayout->Animate(0);
}

// Retail 0x802233AC: wait for anim trans 1 (via advanceAnimTransform) to finish,
// then enter state 2 and run crystalListEnableAnim2.
extern "C" void crystalListWaitAnim1To2(CMCCrystalList* self)
{
    if (advanceAnimTransform(self->mAnimTrans1, 1.0f)) {
        self->mStateIdx = 2;
        crystalListEnableAnim2(self);
    }
}

// Retail 0x802233F8: wait for anim trans 2 to finish, then activate state 3.
extern "C" void crystalListWaitAnim2To3(CMCCrystalList* self)
{
    if (advanceAnimTransform(self->mAnimTrans2, 1.0f) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

// Retail 0x80223444: for each crystal slot in state 1, run crystalListStepSlot.
// u8 loop counter: MWCC masks it once into r4 (clrlwi), which serves both as
// the array-index add operand and the second call argument.
extern "C" void crystalListPollSlots(CMCCrystalList* self)
{
    for (u8 i = 0; i < 8; i++) {
        if ((s8)self->mSlotStates[i] == 1) {
            crystalListStepSlot(self, i);
        }
    }
}

// crystalListStepSlot (recovered): format the slot icon pane name (fmt @ 0x13E),
// resolve the pane through the layout root FindPaneByName virtual, and when
// the StepAnimLinkFrame check passes, clear the slot's state byte.
// The -O4,s unit merges the r30/r31 saves into the retail stmw r30 prologue.
extern "C" void crystalListStepSlot(CMCCrystalList* self, u32 idx) {
    char buf[0x20];
    sprintf(buf, "pic_heat%02d", idx + 1);
    nw4r::lyt::Pane* pane =
        self->mLayout->GetRootPane()->FindPaneByName(buf, true);
    if (StepAnimLinkFrame(pane, self->mAnimRes5) != 0) {
        self->mSlotStates[idx] = 0;
    }
}

// Retail 0x80223498: wait for anim trans 2 (via AnimRewindFrame) to finish,
// then enter state 5 and run crystalListEnableAnim1.
extern "C" void crystalListRewindAnim2To5(CMCCrystalList* self)
{
    if (AnimRewindFrame(self->mAnimTrans2, 1.0f) != 0) {
        self->mStateIdx = 5;
        crystalListEnableAnim1(self);
    }
}

// Retail 0x802234E4: wait for anim trans 1 (via AnimRewindFrame) to finish,
// then reset to state 0 and activate.
extern "C" void crystalListRewindAnim1To0(CMCCrystalList* self)
{
    if (AnimRewindFrame(self->mAnimTrans1, 1.0f) != 0) {
        self->mStateIdx = 0;
        self->mIsActive = 1;
    }
}

// Retail 0x80223530: wait for anim trans 3 to finish, then activate state 3.
extern "C" void crystalListWaitAnim3To3(CMCCrystalList* self)
{
    if (advanceAnimTransform(self->mAnimTrans3, 1.0f) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

// Retail 0x8022357C: wait for anim trans 3 (via AnimRewindFrame) to finish,
// then activate state 3.
extern "C" void crystalListRewindAnim3To3(CMCCrystalList* self)
{
    if (AnimRewindFrame(self->mAnimTrans3, 1.0f) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

// Retail 0x802235C8: wait for anim trans 4 (via advanceAnimTransform) to finish,
// then activate state 3.
extern "C" void crystalListWaitAnim4To3(CMCCrystalList* self)
{
    if (advanceAnimTransform(self->mAnimTrans4, 1.0f) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

// Retail 0x802254D8: layout animation setup. Disable anim resource 5 across
// the whole pane tree (recursive via the root pane), then toggle the
// per-layout anim transforms: disable 4/3/2, enable 1.
extern "C" void crystalListEnableAnim1(CMCCrystalList* self)
{
    self->mLayout->GetRootPane()->SetAnimationEnable(*self->mAnimRes5, false, true);
    self->mLayout->SetAnimationEnable(self->mAnimTrans4, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans3, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans2, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans1, true);
}

// Retail 0x80225554: same setup, disable 4/3/1, enable 2.
extern "C" void crystalListEnableAnim2(CMCCrystalList* self)
{
    self->mLayout->GetRootPane()->SetAnimationEnable(*self->mAnimRes5, false, true);
    self->mLayout->SetAnimationEnable(self->mAnimTrans4, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans3, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans1, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans2, true);
}

// Retail 0x80225650: same setup, disable 4/1/2, enable 3.
extern "C" void crystalListEnableAnim3(CMCCrystalList* self)
{
    self->mLayout->GetRootPane()->SetAnimationEnable(*self->mAnimRes5, false, true);
    self->mLayout->SetAnimationEnable(self->mAnimTrans4, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans1, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans2, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans3, true);
}

// Retail 0x8022570C: same setup, disable 1/2/3, enable 4.
extern "C" void crystalListEnableAnim4(CMCCrystalList* self)
{
    self->mLayout->GetRootPane()->SetAnimationEnable(*self->mAnimRes5, false, true);
    self->mLayout->SetAnimationEnable(self->mAnimTrans1, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans2, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans3, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans4, true);
}

// Retail 0x802257C8: disable all four layout anim transforms, then enable the
// root-pane animation resource (only once mStateIdx has reached state 3).
extern "C" void crystalListDisableAnims(CMCCrystalList* self) {
    if (self->mStateIdx >= 3) {
        self->mLayout->SetAnimationEnable(self->mAnimTrans1, false);
        self->mLayout->SetAnimationEnable(self->mAnimTrans2, false);
        self->mLayout->SetAnimationEnable(self->mAnimTrans3, false);
        self->mLayout->SetAnimationEnable(self->mAnimTrans4, false);
        self->mLayout->GetRootPane()->SetAnimationEnable(*self->mAnimRes5, true, true);
    }
}

// Retail 0x80225890: file-load callback for the crystal list. Two files can
// load asynchronously: the main crystal-list layout (mFileHandle1) and a
// second resource archive (mFileHandle2). Each branch mirrors the
// CMCCrystalInfo::OnFileEvent shape: region-guarded file buffer, layout
// build (layout + 4 anim transforms + anim resource 5), font pane bind,
// per-slot text stamping, and final activation flags.
// -O4,s: retail uses the stmw r28 multi-save prologue.
bool CMCCrystalList::OnFileEvent(CEventFile* pEventFile)
{
    // One function-scope fileData so both archive branches share the r29 color
    // (per-branch locals were r29 in file 1 and r28 in file 2).
    void* fileData;
    if (mFileHandle1 == pEventFile->mFileHandle) {
        // === crystal list file 1 loaded ===
        mMemRegion1.createRegion(
            mtl::MemManager::getHandleMEM2(), 0x10000,
            "CMCCrystalList", 0);
        Class_8045F858 regionGuard1(&mMemRegion1);

        fileData = mFileHandle1->mData;
        mFileHandle1->mData = nullptr;
        mtl::MemManager::setMemInitFlag(false);

        mArcResAccessor = CLibLayout::createArcResourceAccessor();
        mArcResAccessor->Attach(fileData, "arc");

        buildLayout(&mLayout, mArcResAccessor, "mf10_cry00_inf01.brlyt");

        bindLayoutAnimTransform(mLayout, &mAnimTrans1, mArcResAccessor, "mf10_cry00_inf01_in.brlan");
        bindLayoutAnimTransform(mLayout, &mAnimTrans2, mArcResAccessor, "mf10_cry00_inf01_info_in.brlan");
        bindLayoutAnimTransform(mLayout, &mAnimTrans3, mArcResAccessor, "mf10_cry00_inf01_bl_in.brlan");
        bindLayoutAnimTransform(mLayout, &mAnimTrans4, mArcResAccessor, "mf10_cry00_inf01_drop.brlan");
        LayoutBindAnimResource(mLayout, (void**)&mAnimRes5, mArcResAccessor, "mf10_cry00_inf01_heat.brlan");

        // Bind the loaded font's pane into the layout root.
        nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
        ApplyFontToTxtPanes(rootPane,
            (const nw4r::ut::Font*)(reinterpret_cast<CMCCrystalListFontView*>(
                CDeviceFont::getFontInfo(1, mLayout))
                ->vf7()));

        // If a character set is loaded, stamp every crystal-slot text pane
        // (8 slots, two text panes each) with it.
        u32 sh = (u32)CUICfManager_getPackedFont9C();
        if (sh != 0) {
            char buf[0x20];
            for (u8 i = 1; i <= 8; i++) {
                sprintf(buf, "txt_listvalue%02d", i);
                setLayoutTextBoxFont(mLayout, buf, sh);
                sprintf(buf, "txt_listpara%02d", i);
                setLayoutTextBoxFont(mLayout, buf, sh);
            }
        }

        char* crystalName = BdatTouchStringCell("MNU_crystal", "info", 0x2c);
        LayoutSetTextBoxFmtValue(mLayout, "txt_listname00", crystalName, 0);
        char* crystalValue = BdatTouchStringCell("MNU_crystal", "info", 0x2d);
        LayoutSetTextBoxFmtValue(mLayout, "txt_listvalue00", crystalValue, 0);

        crystalListEnableAnim1(this);
        mLayout->Animate(0);

        // Second archive present -> mark the list ready.
        if (mArcResAccessor2 != 0) {
            mDataFlag = 1;
            mState = 1;
        }
        mFileHandle1 = nullptr;
        mMemRegion1.validateHeap();
        return true;
    } else if (mFileHandle2 == pEventFile->mFileHandle) {
        // === crystal list file 2 loaded ===
        mMemRegion2.createRegion(
            mtl::MemManager::getHandleMEM2(), 0x100,
            "CItemBoxInfoTex", 0);
        Class_8045F858 regionGuard2(&mMemRegion2);

        fileData = mFileHandle2->mData;
        mFileHandle2->mData = nullptr;
        mtl::MemManager::setMemInitFlag(false);

        mArcResAccessor2 = CLibLayout::createArcResourceAccessor();
        mArcResAccessor2->Attach(fileData, "arc");

        // Main layout already built -> mark the list ready.
        if (mArcResAccessor != 0) {
            mDataFlag = 1;
            mState = 1;
        }
        mFileHandle2 = nullptr;
        mMemRegion2.validateHeap();
        return true;
    }

    return false;
}
