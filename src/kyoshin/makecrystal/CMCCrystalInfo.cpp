// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCCrystalInfo
// Replace stubs with high-level C/C++ during decomp.

// Inline-empty ~IWorkEvent: the retail destructor elides the base call.
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/cf/CfGameManager.hpp"  // canonical CItem_initItemImplInstances decl
#include "kyoshin/makecrystal/CMCCrystalInfo.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/work/CEventFile.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

namespace nw4r { namespace lyt { class DrawInfo; } }

void SplitU32ToS16s(void*, u32);
void setGXColorS10(void*, u32, u32, u32, u32);
extern "C" void MCCrystal_StepIntro(CMCCrystalInfo*);
extern "C" void MCCrystal_StepMain(CMCCrystalInfo*);
extern "C" void MCCrystal_RewindMain(CMCCrystalInfo*);
extern "C" void MCCrystal_RewindIntro(CMCCrystalInfo*);
extern "C" void MCCrystal_StepAnim4(CMCCrystalInfo*);
extern "C" void MCCrystal_StepAnim3(CMCCrystalInfo*);
extern "C" void MCCrystal_RewindAnim3(CMCCrystalInfo*);

// Small-data symbol (plain C++ extern; lives in another TU).
extern u32 lbl_eu_806640D8;


s16 sPane1Color0[4] = {0};
s16 sPane1Color1[4] = {0};
s16 sPane1Alpha0[4] = {0};
s16 sPane1Alpha1[4] = {0};
s16 sPane2Color0[4] = {0};
s16 sPane2Color1[4] = {0};
s16 sPane2Alpha0[4] = {0};
s16 sPane2Alpha1[4] = {0};

void MCCrystal_BuildItemBuf(CrystalItemBuf* out, CMCCrystalInfo*, u32, CMCCItemHandle*);

CMCCrystalInfo::CMCCrystalInfo()
    : mFileHandle1(0), mFileHandle2(0), mArcResAccessor(0), mField30(0), mLayout(0),
      mAnimTransform1(0), mAnimTransform2(0), mAnimTransform3(0), mAnimTransform4(0),
      mEnabled(0), mState(0), mIsActive(0), mField51(1) {}
CMCCrystalInfo::~CMCCrystalInfo() {}
void MCCrystal_LoadLayouts(CMCCrystalInfo* self)
{
    // Load the two crystal-info layout files (names at lbl_eu_80508DF8/+0x1a).
    u32 handle = (u32)mtl::MemManager::getHandleMEM2();
    self->mFileHandle1 = (u32)CDeviceFile::readFile(handle, "menu/jp/McCrystalInfo.arc", self, 0, 0);
    handle = (u32)mtl::MemManager::getHandleMEM2();
    self->mFileHandle2 = (u32)CDeviceFile::readFile(handle, "menu/jp/tpl/CrystalIcon.arc", self, 0, 0);
}

void MCCrystal_StepState(CMCCrystalInfo* self)
{
    if (self->mEnabled) {
        switch (self->mState) {
        case 1:
            MCCrystal_StepIntro(self);
            break;
        case 2:
            MCCrystal_StepMain(self);
            break;
        case 4:
            MCCrystal_RewindMain(self);
            break;
        case 5:
            MCCrystal_RewindIntro(self);
            break;
        case 6:
            MCCrystal_StepAnim4(self);
            break;
        case 7:
            MCCrystal_StepAnim3(self);
            break;
        case 8:
            MCCrystal_RewindAnim3(self);
            break;
        }
        ((nw4r::lyt::Layout*)self->mLayout)->Animate(0);
    }
}

void MCCrystal_DrawLayout(CMCCrystalInfo* self, nw4r::lyt::DrawInfo* drawInfo)
{
    if (self->mEnabled) {
        drawLayout(
            (nw4r::lyt::Layout*)self->mLayout, drawInfo, 0, 1);
    }
}

void MCCrystal_Teardown(CMCCrystalInfo* self)
{
    closeFileHandle((CFileHandle**)&self->mFileHandle1);
    closeFileHandle((CFileHandle**)&self->mFileHandle2);
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)self->mLayout;
    self->mEnabled = 0;
    if (layout != 0) {
        // Deleting dtor: MWCC emits a virtual call to ~Layout (vtable +8)
        // with the delete flag in r4, guarded by its own null check.
        delete layout;
        self->mLayout = 0;
    }
    releaseArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)self->mArcResAccessor);
    releaseArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)self->mField30);
    self->mMemRegion1.deleteRegion();
    self->mMemRegion2.deleteRegion();
}

// Converted to inline member function in header
// Out-of-line C-linkage copy so the retail func_8021A8EC symbol pairs
// (lbz r3,80(r3): mIsActive at +0x50).
extern "C" u8 func_8021A8EC(const CMCCrystalInfo* self) {
    return self->mIsActive;
}

void MCCrystal_OpenFromClosed(CMCCrystalInfo* self)
{
    if (self->mState == 0) {
        self->mState = 1;
        self->mField51 = 0;
        return MCCrystal_BindIntro(self);
    }
}

void MCCrystal_PlayOutro(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 4;
        self->mField51 = 0;
        return MCCrystal_BindMain(self);
    }
}

void MCCrystal_PlayAnim4(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 6;
        self->mField51 = 0;
        return MCCrystal_BindAnim4(self);
    }
}

void MCCrystal_PlayAnim3A(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 7;
        self->mField51 = 0;
        return MCCrystal_BindAnim3(self);
    }
}

void MCCrystal_PlayAnim3B(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 8;
        self->mField51 = 0;
        return MCCrystal_BindAnim3(self);
    }
}
void MCCrystal_ShowItem(CMCCrystalInfo* self, u32 arg4, CMCCItemData* item)
{
    CMCCItemData* p;
    if (item != 0) {
        p = item;
    } else {
        p = 0;
    }
    u32 val;
    if (p != 0) {
        val = p->word0 >> 20;
    } else {
        val = arg4;
    }
    int code = BdatGetItemType(val & 0xFFFF);
    if (p != 0 && p->word0 != 0) {
        // Type nibble lives at bits 16-19; crystal type 9 with sub-flag 2/3
        // forces the crystal display path.
        u32 type = (p->word0 >> 16) & 0xF;
        if ((type == 9 && (u32)(p->field07 & 3) == 2) ||
            (type == 9 && (u32)(p->field07 & 3) == 3))
            code = 9;
    }
    // Type 9 (crystal) routes to the crystal slot fill; otherwise clear them.
    if ((code & 0xFFFF) == 9) {
        MCCrystal_ShowSlots(self, (u16)val, item);
    } else {
        MCCrystal_ClearTexts(self);
    }
}

void MCCrystal_FillSlot(CMCCrystalInfo* self, u32 idxBase, u32 arg5, u8 arg6, u32 arg7)
{
    char buf[0x20];
    u32 msgId = arg5;   // materialized first: retail spills it (r25) before idxBase
    u32 idx;
    char* msgName;

    // Crystal name for slot idx, then colour it (colour pair depends on arg7).
    msgName = BdatGetPtrDirect((char*)lbl_eu_806640D8, "name", msgId);
    idx = idxBase + 1;
    sprintf(buf, "txt_listname%02d", idx);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, buf, msgName, 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)self->mLayout, buf,
        (GXColorS10*)(arg7 ? &sPane1Alpha0 : &sPane1Color0),
        (GXColorS10*)(arg7 ? &sPane1Alpha1 : &sPane1Color1));

    // Slot number text + element image index, coloured with the second pair.
    sprintf(buf, "txt_listvalue%02d", idx);
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)self->mLayout, buf, arg6);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)self->mLayout, buf,
        (GXColorS10*)(arg7 ? &sPane2Alpha0 : &sPane2Color0),
        (GXColorS10*)(arg7 ? &sPane2Alpha1 : &sPane2Color1));

    // Description string + colour.
    sprintf(buf, "txt_listpara%02d", idx);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, buf,
        BdatTouchStringCell("MNU_item", "name", 0x21), 0);
    PaneMatSetTevColorsByName((nw4r::lyt::Layout*)self->mLayout, buf,
        (GXColorS10*)(arg7 ? &sPane2Alpha0 : &sPane2Color0),
        (GXColorS10*)(arg7 ? &sPane2Alpha1 : &sPane2Color1));

    // Pick the crystal picture to show for this slot based on the item code.
    void* res = 0;
    u8 code = BdatGetU8Direct((const void*)lbl_eu_806640D8, "atr_type", msgId);
    switch (code) {
    case 0:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          "mf00_reg30_crys00.tpl", 0);
        break;
    case 4:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          "mf00_reg30_crys01.tpl", 0);
        break;
    case 5:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          "mf00_reg30_crys02.tpl", 0);
        break;
    case 6:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          "mf00_reg30_crys03.tpl", 0);
        break;
    case 7:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          "mf00_reg30_crys04.tpl", 0);
        break;
    case 8:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          "mf00_reg30_crys05.tpl", 0);
        break;
    case 9:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          "mf00_reg30_crys06.tpl", 0);
        break;
    }

    if (res != 0) {
        sprintf(buf, "pic_ethcol%02d", idx);
        PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->mLayout, (const char*)buf, (TPLPalette*)res);
    }
}

// Clears every crystal-slot display buffer.
extern "C" void MCCrystal_ClearSlots(CMCCrystalInfo* self)
{
    char buf[0x20];
    for (u8 i = 1; i <= 8; i++) {
        sprintf(buf, "txt_listname%02d", i);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, buf, "", 0);
        sprintf(buf, "txt_listvalue%02d", i);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, buf, "", 0);
        sprintf(buf, "txt_listpara%02d", i);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, buf, "", 0);
        void* res = ((nw4r::lyt::ArcResourceAccessor*)self->mArcResAccessor)
                        ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                                      "mf00_com00_dmy.tpl", NULL);
        if (res != 0) {
            sprintf(buf, "pic_ethcol%02d", i);
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)self->mLayout, (const char*)buf, (TPLPalette*)res);
        }
    }
}
void MCCrystal_AlignPane(CMCCrystalInfo* self, CMCCrystalInfo* other, u32 paneIdx)
{
    char buf[0x20];
    sprintf(buf, "nul_list%02d", paneIdx + 1);
    nw4r::lyt::Pane* r1 =
        (*(nw4r::lyt::Pane**)((u8*)other->mLayout + 0x10))->FindPaneByName(buf, true);
    // Retail reloads other->mLayout and its root pane for every lookup.
    nw4r::lyt::Pane* r2 =
        (*(nw4r::lyt::Pane**)((u8*)other->mLayout + 0x10))
            ->FindPaneByName("nul_proportion", true);
    ScalePaneWorldPos((nw4r::math::VEC3*)self, r1, r2, *(nw4r::lyt::Pane**)((u8*)other->mLayout + 0x10));
}

void MCCrystal_StepIntro(CMCCrystalInfo* self)
{
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->mAnimTransform1, 1.0f)) {
        self->mState = 2;
        return MCCrystal_BindMain(self);
    }
}

void MCCrystal_StepMain(CMCCrystalInfo* self)
{
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->mAnimTransform2, 1.0f)) {
        self->mState = 3;
        self->mField51 = 1;
    }
}

void MCCrystal_RewindMain(CMCCrystalInfo* self)
{
    if (AnimRewindFrame((nw4r::lyt::AnimTransform*)self->mAnimTransform2, 1.0f)) {
        self->mState = 5;
        return MCCrystal_BindIntro(self);
    }
}

void MCCrystal_RewindIntro(CMCCrystalInfo* self)
{
    if (AnimRewindFrame((nw4r::lyt::AnimTransform*)self->mAnimTransform1, 1.0f)) {
        self->mState = 0;
        self->mField51 = 1;
    }
}

void MCCrystal_StepAnim4(CMCCrystalInfo* self)
{
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->mAnimTransform4, 1.0f)) {
        self->mField51 = 1;
        self->mState = 0;
    }
}

void MCCrystal_StepAnim3(CMCCrystalInfo* self)
{
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->mAnimTransform3, 1.0f)) {
        self->mField51 = 1;
        self->mState = 3;
    }
}

void MCCrystal_RewindAnim3(CMCCrystalInfo* self)
{
    if (AnimRewindFrame((nw4r::lyt::AnimTransform*)self->mAnimTransform3, lbl_eu_80668498)) {
        self->mField51 = 1;
        self->mState = 3;
    }
}

// Build the crystal-slot display buffers.
void MCCrystal_BuildItemBuf(CrystalItemBuf* out, CMCCrystalInfo* self, u32 data, CMCCItemHandle* item)
{
    CrystalItemBuf buf;
    CMCCItemHandle* item2;
    if (item != 0) {
        item2 = item;
    } else {
        item2 = 0;
    }
    BdatGetItemType(data);
    BdatGetItemId(data);
    CItemImplInstancesFacade* inst = (CItemImplInstancesFacade*)CItem_initItemImplInstances((CItemData*)item2);
    u8 count = inst->GetCount(item2);
    buf.count = count;
    buf.str = (char*)BdatTouchStringCell("MNU_item", "name",
                                   0x1e - (count - 1));
    buf.field21 = 0;
    for (u32 i = 0; i < 4; i++) {
        CItemImplInstancesFacade* inst2 = (CItemImplInstancesFacade*)CItem_initItemImplInstances((CItemData*)item2);
        u16 n = inst2->GetName(item2, (u8)i);
        if (n > 0) {
            buf.names[buf.field21] = BdatGetPtrDirect((char*)lbl_eu_806640D8,
                                                   "name", n);
            CItemImplInstancesFacade* inst3 = (CItemImplInstancesFacade*)CItem_initItemImplInstances((CItemData*)item2);
            buf.flags[buf.field21] = inst3->GetFlag(item2, (u8)i);
            buf.field21++;
        }
    }
    // Word-pair block copy of the buffer (counted lwzu/stwu loop under -O4,s).
    u32* dst = (u32*)((char*)out - 4);
    u32* src = (u32*)((char*)&buf - 4);
    for (int j = 4; j != 0; j--) {
        *++dst = *++src;
    }
    *++dst = *++src;
}

// Fill the crystal slot texts from the item's crystal entries.
void MCCrystal_ShowSlots(CMCCrystalInfo* self, u16 arg2, CMCCItemHandle* item)
{
    char buf[0x20];
    MCCrystal_ClearTexts(self);
    CrystalItemBuf bufB;
    MCCrystal_BuildItemBuf(&bufB, self, arg2, item);
    // Word-pair block copy of the filled buffer into a fixed frame slot;
    // -O4,s keeps this as the retail counted lwzu/stwu loop.
    CrystalItemBuf slots;
    u32* dst = (u32*)((char*)&slots - 4);
    u32* src = (u32*)((char*)&bufB - 4);
    for (int j = 4; j != 0; j--) {
        *++dst = *++src;
    }
    *++dst = *++src;
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, "txt_value30",
                  slots.str, 0);
    u8 count = slots.field21;
    for (u8 i = 0; i < count; i++) {
        sprintf(buf, "txt_para%02d", (i * 2) + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, buf, slots.names[i], 0);
        sprintf(buf, "txt_value%02d", i + 0x1f);
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)self->mLayout, buf, slots.flags[i]);
        sprintf(buf, "txt_para%02d", (i * 2) + 0x20);
        char* s = BdatTouchStringCell("MNU_item", "name", 0x21);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, buf, s, 0);
    }
}

// Clears every crystal-slot text entry (used before refilling the display).
void MCCrystal_ClearTexts(CMCCrystalInfo* self)
{
    char buf[0x20];
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, "txt_value30",
                  "", 0);
    for (u8 i = 0; i < 4; i++) {
        sprintf(buf, "txt_para%02d", (i * 2) + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, buf, "", 0);
        sprintf(buf, "txt_value%02d", i + 0x1f);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, buf, "", 0);
        sprintf(buf, "txt_para%02d", (i * 2) + 0x20);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)self->mLayout, buf, "", 0);
    }
}

// Sets the ready/active flags once both the layout and its resource accessor
// are present.
extern "C" void func_8021B500(void* this_) {
    unsigned int* p34 = reinterpret_cast<unsigned int*>(static_cast<char*>(this_) + 0x34);
    unsigned int* p30 = reinterpret_cast<unsigned int*>(static_cast<char*>(this_) + 0x30);
    if (*p34 != 0) {
        if (*p30 != 0) {
            *reinterpret_cast<unsigned char*>(static_cast<char*>(this_) + 0x48) = 1;
            *reinterpret_cast<unsigned char*>(static_cast<char*>(this_) + 0x50) = 1;
        }
    }
}

// Bind the crystal-info intro animation to the layout and start it playing.
// (Called on state 0->1 and 4->5 transitions.)
void MCCrystal_BindIntro(CMCCrystalInfo* self)
{
    ((nw4r::lyt::Layout*)self->mLayout)->UnbindAllAnimation();
    ((nw4r::lyt::Layout*)self->mLayout)->BindAnimation((nw4r::lyt::AnimTransform*)self->mAnimTransform1);
    ((nw4r::lyt::Layout*)self->mLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->mAnimTransform1, true);
    ((nw4r::lyt::Layout*)self->mLayout)->Animate(0);
}

// Bind the crystal-info outro animation to the layout and start it playing.
// (Called on state 3->4 transitions.)
void MCCrystal_BindMain(CMCCrystalInfo* self)
{
    ((nw4r::lyt::Layout*)self->mLayout)->UnbindAllAnimation();
    ((nw4r::lyt::Layout*)self->mLayout)->BindAnimation((nw4r::lyt::AnimTransform*)self->mAnimTransform2);
    ((nw4r::lyt::Layout*)self->mLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->mAnimTransform2, true);
    ((nw4r::lyt::Layout*)self->mLayout)->Animate(0);
}

// Bind the crystal-info intro animation variant for the 3rd anim transform
// (mAnimTransform3) and start it playing. Called on state 3->7/3->8.
void MCCrystal_BindAnim3(CMCCrystalInfo* self)
{
    ((nw4r::lyt::Layout*)self->mLayout)->UnbindAllAnimation();
    ((nw4r::lyt::Layout*)self->mLayout)->BindAnimation((nw4r::lyt::AnimTransform*)self->mAnimTransform3);
    ((nw4r::lyt::Layout*)self->mLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->mAnimTransform3, true);
    ((nw4r::lyt::Layout*)self->mLayout)->Animate(0);
}

// Bind the crystal-info intro animation variant for the 4th anim transform
// (mAnimTransform4) and start it playing. Called on state 3->6.
void MCCrystal_BindAnim4(CMCCrystalInfo* self)
{
    ((nw4r::lyt::Layout*)self->mLayout)->UnbindAllAnimation();
    ((nw4r::lyt::Layout*)self->mLayout)->BindAnimation((nw4r::lyt::AnimTransform*)self->mAnimTransform4);
    ((nw4r::lyt::Layout*)self->mLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->mAnimTransform4, true);
    ((nw4r::lyt::Layout*)self->mLayout)->Animate(0);
}

bool CMCCrystalInfo::OnFileEvent(CEventFile* pEventFile)
{
    if (mFileHandle1 == (u32)pEventFile->mFileHandle) {
        // === crystal info file 1 loaded ===
        mMemRegion1.createRegion(
            mtl::MemManager::getHandleMEM2(), 0x18000,
            "CMCCrystalInfo", 0);
        Class_8045F858 regionGuard1(&mMemRegion1);

        CFileHandle* h1 = (CFileHandle*)mFileHandle1;
        void* fileData = h1->mData;
        h1->mData = nullptr;
        mtl::MemManager::setMemInitFlag(false);

        mArcResAccessor = CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)mArcResAccessor)
            ->Attach(fileData, "arc");

        buildLayout((nw4r::lyt::Layout**)&mLayout, (nw4r::lyt::ArcResourceAccessor*)mArcResAccessor, "mf10_cry00_inf00.brlyt");

        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, (nw4r::lyt::AnimTransform**)&mAnimTransform1, (nw4r::lyt::ArcResourceAccessor*)mArcResAccessor, "mf10_cry00_inf00_in.brlan");
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, (nw4r::lyt::AnimTransform**)&mAnimTransform2, (nw4r::lyt::ArcResourceAccessor*)mArcResAccessor, "mf10_cry00_inf00_info_in.brlan");
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, (nw4r::lyt::AnimTransform**)&mAnimTransform3, (nw4r::lyt::ArcResourceAccessor*)mArcResAccessor, "mf10_cry00_inf00_box.brlan");
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, (nw4r::lyt::AnimTransform**)&mAnimTransform4, (nw4r::lyt::ArcResourceAccessor*)mArcResAccessor, "mf10_cry00_inf00_out.brlan");

        // Bind the loaded font's pane into the layout root.
        nw4r::lyt::Pane* rootPane =
            ((nw4r::lyt::Layout*)mLayout)->GetRootPane();
        ApplyFontToTxtPanes(rootPane,
            (const nw4r::ut::Font*)(reinterpret_cast<CMCCrystalInfoFontView*>(
                CDeviceFont::getFontInfo(1, (nw4r::lyt::Layout*)mLayout))
                ->vf7()));

        // If a character set is loaded, stamp every text pane with it.
        u32 sh = (u32)CUICfManager_getPackedFont9C();
        if (sh != 0) {
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, "txt_value30", sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, "txt_value31", sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, "txt_value32", sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, "txt_value33", sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, "txt_value34", sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, "txt_para32", sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, "txt_para34", sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, "txt_para36", sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, "txt_para38", sh);
            char buf[0x20];
            for (u8 i = 1; i <= 8; i++) {
                sprintf(buf, "txt_listvalue%02d", i);
                setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, buf, sh);
                sprintf(buf, "txt_listpara%02d", i);
                setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, buf, sh);
            }
        }

        MCCrystal_BindIntro(this);
        ((nw4r::lyt::Layout*)mLayout)->Animate(0);

        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, "txt_para30",
            (char*)BdatTouchStringCell("MNU_item", "name", 0x18), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, "txt_listname00",
            (char*)BdatTouchStringCell("MNU_crystal", "info", 0x2c), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, "txt_listvalue00",
            (char*)BdatTouchStringCell("MNU_crystal", "info", 0x2d), 0);

        MCCrystal_ClearSlots(this);

        // Pull the two colour pairs off their panes and refresh the alpha
        // channel of the shared slot colours.
        nw4r::lyt::Pane* pane1 = ((nw4r::lyt::Layout*)mLayout)->GetRootPane()
            ->FindPaneByName("txt_listname01", true);
        GXColorS10 fs1a = PaneMatGetS16Quad(pane1, 0);
        CopyVec4s((GXColorS10*)sPane1Color0, (const GXColorS10*)((short*)&fs1a));
        GXColorS10 fs1b = PaneMatGetS16Quad(pane1, 1);
        CopyVec4s((GXColorS10*)sPane1Color1, (const GXColorS10*)((short*)&fs1b));
        sPane1Alpha0[3] = sPane1Color0[3];
        sPane1Alpha1[3] = sPane1Color1[3];

        nw4r::lyt::Pane* pane2 = ((nw4r::lyt::Layout*)mLayout)->GetRootPane()
            ->FindPaneByName("txt_listvalue01", true);
        GXColorS10 fs2a = PaneMatGetS16Quad(pane2, 0);
        CopyVec4s((GXColorS10*)sPane2Color0, (const GXColorS10*)((short*)&fs2a));
        GXColorS10 fs2b = PaneMatGetS16Quad(pane2, 1);
        CopyVec4s((GXColorS10*)sPane2Color1, (const GXColorS10*)((short*)&fs2b));
        sPane2Alpha0[3] = sPane2Color0[3];
        sPane2Alpha1[3] = sPane2Color1[3];

        func_8021B500(this);
        mFileHandle1 = 0;
        mMemRegion1.validateHeap();
        return true;
    } else if (mFileHandle2 == (u32)pEventFile->mFileHandle) {
        // === crystal info file 2 loaded ===
        mMemRegion2.createRegion(
            mtl::MemManager::getHandleMEM2(), 0x100, "CItemBoxInfoTex", 0);
        Class_8045F858 regionGuard2(&mMemRegion2);

        CFileHandle* h2 = (CFileHandle*)mFileHandle2;
        void* fileData = h2->mData;
        h2->mData = nullptr;
        mtl::MemManager::setMemInitFlag(false);

        mField30 = (u32)CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)mField30)
            ->Attach(fileData, "arc");

        func_8021B500(this);
        mFileHandle2 = 0;
        mMemRegion2.validateHeap();
        return true;
    }

    return false;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8021BBC4()
{
    SplitU32ToS16s(sPane1Color0, 0);
    SplitU32ToS16s(sPane1Color1, 0);
    setGXColorS10(sPane1Alpha0, 0xd2, 0x28, 0x14, 0);
    setGXColorS10(sPane1Alpha1, 0xd2, 0x28, 0x14, 0);
    SplitU32ToS16s(sPane2Color0, 0);
    SplitU32ToS16s(sPane2Color1, 0);
    setGXColorS10(sPane2Alpha0, 0xff, 0xff, 0xfa, 0);
    setGXColorS10(sPane2Alpha1, 0xd2, 0x28, 0x14, 0);
}
