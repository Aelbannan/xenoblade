// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPlayAward
// Replace stubs with high-level C/C++ during decomp.

#include <stdio.h>   // sprintf (award pane names)
#include <revolution/gx/GXPixel.h>  // GXSetZMode

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/CTutorial.hpp"     // func_80138078 (retail func_80138078__FUl)
#include "kyoshin/code_80135FDC.hpp" // func_80137510 / func_80137444 (anim frame advance)
#include "kyoshin/cf/CfGameManager.hpp" // cf::CfGameManager::getCurrentPad / getCfPadData
#include "monolib/core/CPadManager.hpp" // CPad::mPressedButtonFlags
#include "kyoshin/menu/CMenuPlayAward.hpp"
#include "monolib/work/CEventFile.hpp" // CEventFile (event->mFileHandle in OnFileEvent)
#include "monolib/device/CDeviceFile.hpp"  // CDeviceFile::readFile / readCommonArchiveFile
#include "monolib/device/CDeviceVI.hpp"       // CDeviceVI::waitForDrawDone
#include "monolib/util/MemManager.hpp"        // mtl::MemManager::allocate
#include "monolib/work/CProcess.hpp"          // CProcess::Regist
#include "monolib/work/CWorkThreadSystem.hpp" // CWorkThreadSystem::getWorkMem
// CScn (with removeRenderCB) comes from the minimal declaration in
// CTaskGameEff.hpp via harness_catalog.hpp -- CScn.hpp redefines it.

// Retail constructor symbol (unmangled global in US). Kept out-of-line so the
// factory (func_80270308) emits a real bl to it, and returns `this` in r3
// like a real constructor.
__declspec(noinline) CMenuPlayAward* __ct__CMenuPlayAward(CMenuPlayAward* self, CScn* scene) {
    __ct__8CProcessFv((CProcess*)self);

    // vtable fixups: temp (CProcess) vtable first, then the null PMF
    // callback slots, then the composite vtable + the IScnRender sub-vtable
    // at +0x58 (same scheme as CMenuTutorialList's ctor).
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;
    self->ptmf0[0] = __ptmf_null[0];
    self->ptmf0[1] = __ptmf_null[1];
    self->ptmf0[2] = __ptmf_null[2];
    self->ptmf1[0] = __ptmf_null[0];
    self->ptmf1[1] = __ptmf_null[1];
    self->ptmf1[2] = __ptmf_null[2];
    self->mField54 = 0;
    self->mField55 = 0;
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_80537BB0;
    *(u32*)((u8*)self + 0x58) = (u32)lbl_eu_80537BB0 + 0x24;
    self->mScene = scene;

    // Embedded widgets in construction order.
    __ct__CBgTex(&self->mBgTex, 0);
    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
    __ct__CPlayAwardList(&self->mPlayAwardList, 0);

    self->mField115C = lbl_eu_80668998;
    self->mField1160 = 0;
    return self;
}

/* Destructor (retail D2 form, symbol keeps its C++ mangling). Written as a
 * free function with an explicit flags parameter (same scheme as
 * CMenuTutorialList/CMenuSave): a real member destructor would make MWCC
 * auto-emit a vtable store that retail lacks. Subobjects are destroyed in
 * reverse construction order with the -1 subobject flag, then the game-side
 * CProcess dtor wrapper __dt__800FED0C with flag 0, then conditional
 * operator delete when flags > 0. */
extern "C" CMenuPlayAward* __dt__14CMenuPlayAwardFv(CMenuPlayAward* _this, int flags) {
    if (_this != 0) {
        __dt__14CPlayAwardListFv(&_this->mPlayAwardList, -1);
        __dt__11CTitleAHelpFv(&_this->mTitleAHelp, -1);
        __dt__6CBgTexFv(&_this->mBgTex, -1);
        __dt__800FED0C(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// Re-initialise the award-list menu screen: rebuild each embedded widget
// (CBgTex / CTitleAHelp / CPlayAwardList) by constructing a stack temporary
// and copying its body into the embedded storage, then register this screen
// as a render callback on the owning scene (same pattern as CMenuCollepedia).
void CMenuPlayAward::Init() {
    func_8008294C__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the background CBgTex via a temporary ---
    u8 tempBgTex[0x20];
    __ct__CBgTex((CBgTex*)tempBgTex, 0);

    __ct__UnkClass_8011C974(&mBgTex.mMemRegion, tempBgTex + 0x4);
    mBgTex.mFileHandle = *(CFileHandle**)(tempBgTex + 0x14);
    mBgTex.mLayout = *(nw4r::lyt::Layout**)(tempBgTex + 0x18);
    mBgTex.mLayoutReady = *(bool*)(tempBgTex + 0x1c);
    mBgTex.mLoaded = *(u8*)(tempBgTex + 0x1d);
    mBgTex.mPtmMode = *(u8*)(tempBgTex + 0x1e);
    __dt__6CBgTexFv((CBgTex*)tempBgTex, -1);

    func_801C3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp ---
    char* name = func_80136190(lbl_eu_8050E7A0, lbl_eu_8050E7A0 + 0xb, 0x9);

    u8 tempTitle[0x38];
    __ct__CTitleAHelp((CTitleAHelp*)tempTitle, name, 0x66);

    __ct__UnkClass_8011C974(&mTitleAHelp.unk4, tempTitle + 0x4);
    mTitleAHelp.mFileHandle = *(CFileHandle**)(tempTitle + 0x14);
    mTitleAHelp.mArcResourceAccessor =
        *(nw4r::lyt::ArcResourceAccessor**)(tempTitle + 0x18);
    mTitleAHelp.mLayout = *(nw4r::lyt::Layout**)(tempTitle + 0x1c);
    mTitleAHelp.mAnimTrans20 = *(nw4r::lyt::AnimTransform**)(tempTitle + 0x20);
    mTitleAHelp.mAnimTrans24 = *(nw4r::lyt::AnimTransform**)(tempTitle + 0x24);
    mTitleAHelp.unk28 = *(u8*)(tempTitle + 0x28);
    mTitleAHelp.unk2c = *(s32*)(tempTitle + 0x2c);
    mTitleAHelp.mName = *(char**)(tempTitle + 0x30);
    mTitleAHelp.unk34 = *(u8*)(tempTitle + 0x34);
    mTitleAHelp.unk35 = *(u8*)(tempTitle + 0x35);
    mTitleAHelp.unk36 = *(u8*)(tempTitle + 0x36);
    mTitleAHelp.unk37 = *(u8*)(tempTitle + 0x37);
    __dt__11CTitleAHelpFv((CTitleAHelp*)tempTitle, -1);

    CTitleAHelp_load(&mTitleAHelp);

    // --- Re-initialise the embedded CPlayAwardList via a temporary ---
    u8 tempList[0x10A4];
    __ct__CPlayAwardList((CPlayAwardList*)tempList, 0);

    __ct__UnkClass_8011C974(&mPlayAwardList.mMemRegion, tempList + 0x4);
    mPlayAwardList.mFileHandle = *(CFileHandle**)(tempList + 0x14);
    mPlayAwardList.mFileHandle2 = *(CFileHandle**)(tempList + 0x18);
    mPlayAwardList.mArcAccessor1C =
        *(nw4r::lyt::ArcResourceAccessor**)(tempList + 0x1c);
    mPlayAwardList.mLayout20 = *(nw4r::lyt::Layout**)(tempList + 0x20);
    mPlayAwardList.mAnimTrans24 = *(nw4r::lyt::AnimTransform**)(tempList + 0x24);
    mPlayAwardList.mAnimTrans28 = *(nw4r::lyt::AnimTransform**)(tempList + 0x28);
    mPlayAwardList.field_0x2C = *(u32*)(tempList + 0x2c);
    // Cursor body (+0x30..+0x45, skipping the +0x00 vtable pointer).
    CCur18Data* curDst = reinterpret_cast<CCur18Data*>(&mPlayAwardList.mCursor[0]);
    CCur18Data* curSrc = reinterpret_cast<CCur18Data*>(tempList + 0x30);
    curDst->field_4 = curSrc->field_4;
    curDst->field_8 = curSrc->field_8;
    curDst->field_C = curSrc->field_C;
    curDst->field_10 = curSrc->field_10;
    curDst->field_14 = curSrc->field_14;
    curDst->field_15 = curSrc->field_15;
    // Scroll bar: 16-byte mem region via the helper, then the field body.
    __ct__UnkClass_8011C974(&mPlayAwardList.mScrollBar[4], tempList + 0x4c);
    CScrollBarData* barDst = reinterpret_cast<CScrollBarData*>(&mPlayAwardList.mScrollBar[0]);
    CScrollBarData* barSrc = reinterpret_cast<CScrollBarData*>(tempList + 0x48);
    barDst->mFileHandle = barSrc->mFileHandle;
    // Entry-array copy loop pointers (retail sets these up between the
    // scrollbar field copies and the tail-field copies).
    CPlayAwardEntry* dst = &mPlayAwardList.mEntryArray.mEntries[0];
    CPlayAwardEntry* src =
        reinterpret_cast<CPlayAwardEntry*>(tempList + 0x94);
    barDst->mAccessor = barSrc->mAccessor;
    barDst->mLayout = barSrc->mLayout;
    barDst->mAnimTrans = barSrc->mAnimTrans;
    barDst->mReady = barSrc->mReady;
    barDst->mVisible = barSrc->mVisible;
    barDst->mState = barSrc->mState;
    barDst->mActive = barSrc->mActive;
    barDst->mAnimOffset = barSrc->mAnimOffset;
    barDst->mScrollPosY = barSrc->mScrollPosY;
    barDst->mScrollRatio = barSrc->mScrollRatio;
    barDst->mThumbHeight = barSrc->mThumbHeight;
    barDst->mContentHeight = barSrc->mContentHeight;
    barDst->mDirection = barSrc->mDirection;
    // List tail fields (state bytes + scroll halfwords + page byte).
    mPlayAwardList.field_0x88 = *(u8*)(tempList + 0x88);
    mPlayAwardList.field_0x89 = *(u8*)(tempList + 0x89);
    mPlayAwardList.field_0x8A = *(u8*)(tempList + 0x8a);
    mPlayAwardList.field_0x8B = *(u8*)(tempList + 0x8b);
    mPlayAwardList.field_0x8C = *(s16*)(tempList + 0x8c);
    mPlayAwardList.field_0x8E = *(s16*)(tempList + 0x8e);
    mPlayAwardList.field_0x90 = *(u8*)(tempList + 0x90);
    // The 512-entry array, copied element by element (retail 8-byte loop).
    do {
        *dst = *src;
        dst++;
        src++;
    } while (dst < &mPlayAwardList.mEntryArray.mEntries[0x200]);
    // Trailing state fields (+0x1000..+0x100F) of the entry array.
    CPlayAwardEntryArray* arrDst = &mPlayAwardList.mEntryArray;
    CPlayAwardEntryArray* arrSrc =
        reinterpret_cast<CPlayAwardEntryArray*>(tempList + 0x94);
    arrDst->field_1000 = arrSrc->field_1000;
    arrDst->field_1001 = arrSrc->field_1001;
    arrDst->field_1004 = arrSrc->field_1004;
    *(u16*)&arrDst->field_1008[0] = *(u16*)&arrSrc->field_1008[0];
    arrDst->mPageIndex = arrSrc->mPageIndex;
    arrDst->field_100C = arrSrc->field_100C;
    arrDst->field_100E = arrSrc->field_100E;
    __dt__14CPlayAwardListFv((CPlayAwardList*)tempList, -1);

    func_80270CEC(&mPlayAwardList);

    // Register this screen as a render callback on the owning scene (the
    // `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = &mIScnRender;
    }
    mScene->addRenderCB(cb, 0xd, 0);
}

void CMenuPlayAward::Term() {
    CDeviceVI::waitForDrawDone();

    // The IScnRender render callback lives at +0x58; the `if (this)` is the
    // MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58 (same as the
    // sibling CMenuTutorialList/CMenuCollepedia Term bodies).
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = reinterpret_cast<IScnRender*>(&mIScnRender);
    }
    mScene->removeRenderCB(renderCB);

    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_80270E64(&mPlayAwardList);

    lbl_eu_806648A0 = 0;
    func_8008294C__Q22cf13CfGameManagerFv(0);
}

void CMenuPlayAward::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto exit;
    }
    // Branch-over-branch guard (same shape as cbRenderBefore / CMenuTutorialList):
    // exit label before body label emits retail's `beq body; b exit`.
    if (!(lbl_eu_80663E28 & (1u << 21))) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    // Close request: the menu's close button (classic: bit 23, wii: bit 10)
    // plays the close sound and flags the closing state (0x54) while the
    // help-display byte (0x1160) leaves the phase dispatch.
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 close;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        close = (pad->mPressedButtonFlags >> 23) & 1;
    } else {
        close = (pad->mPressedButtonFlags >> 10) & 1;
    }
    if (close != 0) {
        if (func_800FEDF8() != 0) {
            func_800FF914();
            func_80138078(6);
        }
        mField1160 = 4;
        mField54 = 1;
    }

    // Phase state machine (dispatch on the help-display byte 0x1160).
    switch (mField1160) {
    case 0:
        func_8027038C(this);
        break;
    case 1:
        func_80270404(this);
        break;
    case 2:
        func_80270454(this);
        break;
    case 3:
        func_802705F4(this);
        break;
    }

    func_801C3D54(&mBgTex);
    func_801C3FF0(&mTitleAHelp);
    func_80270D64(&mPlayAwardList);
}

void CMenuPlayAward::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto exit;
    }
    // Branch-over-branch guard (same shape as CMenuTutorialList::cbRenderBefore):
    // exit label before body label emits retail's `beq body; b exit`.
    if (!(lbl_eu_80663E28 & (1u << 21))) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    if (func_8013BE50() == 0) {
        goto exit;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw DrawInfo storage: a class-typed local would make MWCC virtual-dispatch
    // the dtor; retail calls __ct__/__dt__Q34nw4r3lyt8DrawInfoFv directly.
    u8 drawInfo[0x60];
    __ct__Q34nw4r3lyt8DrawInfoFv(drawInfo);
    func_80137250(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
    func_801C3D7C(&mBgTex, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
    func_80270E04(&mPlayAwardList, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
    func_801C4080(&mTitleAHelp, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
    __dt__Q34nw4r3lyt8DrawInfoFv(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), -1);
}

/* Factory: lazily allocate + construct the single award-list menu instance
 * and register it as a child of `parent`. Returns the stored instance (or 0
 * if it already exists). */
CMenuPlayAward* func_80270308(CProcess* parent, CScn* scene) {
    if (lbl_eu_806648A0 != 0) {
        return 0;
    }
    CMenuPlayAward* obj = (CMenuPlayAward*)mtl::MemManager::allocate(
        0x1164, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuPlayAward(obj, scene);
    }
    lbl_eu_806648A0 = (unsigned long)obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0x0);
    return (CMenuPlayAward*)lbl_eu_806648A0;
}

void stub_us_80272800() {}

// Help-bar show: when the background, title/help bar and award list are all
// ready, display the help bar, play its sound, and flag the menu (0x1160 = 1).
void func_8027038C(CMenuPlayAward* self) {
    if (func_801C3E34(&self->mBgTex) != 0
        && func_801C4114(&self->mTitleAHelp) != 0
        && func_80270F28(&self->mPlayAwardList) != 0) {
        func_801C412C(&self->mTitleAHelp);
        func_80270F74(&self->mPlayAwardList);
        self->mField1160 = 1;
        func_80138078(0x6d);
    }
}

// Help-bar show: when the title/help bar is idle and the award list is in its
// "ready" state, display the help bar (byte at 0x1160 = 2).
void func_80270404(CMenuPlayAward* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 && func_80270F6C(&self->mPlayAwardList) != 0) {
        self->mField1160 = 2;
    }
}

void func_80270454(CMenuPlayAward* self) {
    self->mField115C += lbl_eu_8066899C;
    if (self->mField115C > lbl_eu_806689A0) {
        self->mField115C = lbl_eu_806689A0;
    }

    CPlayAwardPadData* pad =
        reinterpret_cast<CPlayAwardPadData*>(cf::CfGameManager::getCfPadData());

    // Directional/confirm trigger bits differ between controller types; the
    // whole extraction block is re-run per branch (retail reloads both flag
    // words inside each branch). Same shape as CMenuOption's input handler.
    bool up, down, pageUp, pageDown;
    bool left, right, pageLeft;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        u32 turbo = pad->mTurboPressButtonFlags;   // +0x104
        u32 pressed = pad->mPadPressedFlags;       // +0x04
        up = (turbo & 0x8004) != 0;
        down = (turbo & 0x10008) != 0;
        pageUp = (turbo & 0x2001) != 0;
        pageDown = (turbo & 0x4002) != 0;
        left = (pressed >> 21) & 1;
        right = (pressed >> 22) & 1;
        pageLeft = (pressed >> 10) & 1;
    } else {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        up = (turbo & 0x8004) != 0;
        down = (turbo & 0x10008) != 0;
        pageUp = (turbo & 0x2001) != 0;
        pageDown = (turbo & 0x4002) != 0;
        left = (pressed >> 4) & 1;
        right = (pressed >> 5) & 1;
        pageLeft = (pressed >> 6) & 1;
    }

    // Input dispatch: a held close button (right) closes the award list;
    // otherwise the turbo inputs drive cursor/scroll/page moves (pageLeft =
    // page flip, lowest priority). left blocks the other inputs.
    if (left) {
        // nothing -- blocks the remaining input handlers
    } else if (right) {
        func_801C414C(&self->mTitleAHelp);
        func_80271070(&self->mPlayAwardList);
        self->mField1160 = 3;
    } else if (up) {
        func_802710D4(&self->mPlayAwardList);
    } else if (down) {
        func_80271190(&self->mPlayAwardList);
    } else if (pageUp) {
        func_80271260(&self->mPlayAwardList);
    } else if (pageDown) {
        func_80271300(&self->mPlayAwardList);
    } else if (pageLeft) {
        func_802713BC(&self->mPlayAwardList);
    }
    s32 page = func_80271468(&self->mPlayAwardList);
    func_801C41E8(&self->mTitleAHelp, (u8)page);
}

// Help-bar hide: when the title/help bar is idle and the award list is in its
// "ready" state, mark the menu as closing (byte at 0x54 = 1).
void func_802705F4(CMenuPlayAward* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 && func_80270F6C(&self->mPlayAwardList) != 0) {
        self->mField54 = 1;
    }
}

void func_80270644(void* self) { ((void(*)(void*))cbRenderBefore__14CMenuPlayAwardFv)((char*)self - 0x58); }

void func_8027064C(void* self) { ((void(*)(void*))__dt__14CMenuPlayAwardFv)((char*)self - 0x58); }

// Entry-array initialiser (retail func_80270654): construct the 512 award
// entries via the MWCC array helper, then initialise the trailing state
// fields. Returns self like a constructor.
CPlayAwardEntryArray* func_80270654(CPlayAwardEntryArray* self) {
    __construct_array(self->mEntries, (void*)func_802706C4, (void*)__dt__802706D4, 8, 0x200);
    self->field_1000 = 0;
    self->field_1001 = 0;
    self->field_1004 = lbl_eu_806689A8;
    self->mPageIndex = 0;
    self->field_100C = 0;
    self->field_100E = 0;
    self->field_1008[0] = 0;
    self->field_1008[1] = 0;
    return self;
}

void func_802706C4(CPlayAwardEntry* self) {
    self->word0 = 0;
    self->word1 = 0;
}

// Deleting destructor for CPlayAwardEntry array elements: free the object when
// non-null and the deleting flag is set.
void* __dt__802706D4(CPlayAwardEntry* self, int mode) {
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

// Destructor (retail __dt__80270714): destroys the 512-entry award array via
// __destroy_arr, then frees the object if deleting.
void* __dt__80270714(CPlayAwardEntryArray* self, int mode) {
    if (self != 0) {
        __destroy_arr(self->mEntries, (void*)&__dt__802706D4, 8, 0x200);
        if (mode > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Rebuild the award-list entry array from the BDAT award table: count the
// present award types, sort their indices by sort key, then append each
// award's text binding to the page indexed by its type key. Skips entirely
// when the array was already built (field_1000 nonzero).
void func_80270770(CPlayAwardEntryArray* self) {
    if (self->field_1000 != 0) {
        return;
    }

    char* const s = lbl_eu_8050E7C0;
    void* fp = getFP__FPCc(s);
    void* bdat = lbl_eu_806648A8;
    self->field_1001 = (u8)func_8003B1EC(fp);

    // Count the award types present in the BDAT award table (row 0x312c +
    // (i % 200)); the total goes into field_1000.
    for (u8 i = 1; i <= self->field_1001; i++) {
        if (func_8009CF8C(0x312c + (u8)(i % 200)) != 0) {
            self->field_1000++;
        }
    }

    // Page-progress ratio: present / total, scaled.
    self->field_1004 =
        (float)self->field_1000 / (float)self->field_1001 * lbl_eu_806689AC;

    CPlayAwardEntry entry;
    u8 buf[0x100];
    u8 count = self->field_1001;
    // Fill the sort buffer with the award indices 1..count.
    for (u8 j = 0; j < count; j++) {
        buf[j] = j + 1;
    }

    // Bubble-sort the indices by their BDAT sort key (column +0xe).
    for (u8 outer = 0; outer < self->field_1001 - 1; outer++) {
        int swapped = 0;
        for (u8 inner = 0; inner < (self->field_1001 - 1) - outer; inner++) {
            u8 a = buf[inner];
            u8 b = buf[inner + 1];
            if (func_801361E8((u32)bdat, &s[0xe], a) >
                func_801361E8((u32)bdat, &s[0xe], b)) {
                buf[inner] = b;
                buf[inner + 1] = a;
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }

    // Build the per-page award entries: bind the text for each award and
    // append it to the page indexed by its type key.
    char* def = func_80136190(&s[0x14], &s[0x1f], 0x10);
    for (u8 k = 0; k < count; k++) {
        u8 val = buf[k];
        u8 page = (u8)(func_801361E8((u32)bdat, &s[0x24], val) - 1);
        int has = func_8009CF8C(0x312c + val % 200) != 0;

        u8 key2 = func_801361E8((u32)bdat, &s[0x2a], val);
        u16 key3 = func_80136254(bdat, &s[0x36], val);
        int flag = 0;
        if (key2 <= self->field_1000 && key3 <= func_8009CF8C(0x20)) {
            flag = 1;
        }

        char* s1;
        char* s2;
        if (has != 0) {
            s1 = func_8013639C(fp, &s[0x42], val);
            s2 = func_8013639C(bdat, &s[0x48], val);
        } else if (flag != 0) {
            s1 = def;
            s2 = func_8013639C(bdat, &s[0x51], val);
        } else {
            s1 = def;
            s2 = def;
        }

        entry.word0 = (u32)s1;
        entry.word1 = (u32)s2;
        u8 cnt = self->field_1008[page];
        self->field_1008[page] = cnt + 1;
        func_80270AD8(&self->mEntries[page * 0x200 + cnt], &entry);

        // Remember the scroll position for the entry the player is on.
        u8 cur = code80135FDC_getByte_6407F();
        if (cur != 0 && cur == val) {
            self->mPageIndex = page;
            s16 c = (s16)(self->field_1008[page] - 1);
            self->field_100C = c;
            self->field_100E = 0;
            if (c >= 9) {
                self->field_100E = (s16)(c - 8);
                self->field_100C = 8;
            }
        }
    }
}

// Copy an 8-byte award entry (load both words before storing, like entries[i] = src).
void func_80270AD8(CPlayAwardEntry* dst, const CPlayAwardEntry* src) { *dst = *src; }

// Entry lookup: array base + (pageIndex << 11) + ((param & 0xFF) << 3);
// returns 0 when param >= 0x100.
u8* func_80270AEC(CPlayAwardEntryArray* self, int param) {
    if (param >= 256) return 0;
    return (u8*)self + (self->mPageIndex << 11) + ((param & 0xFF) << 3);
}

// Retail ctor (unmangled US symbol). Builds the embedded widgets, constructs
// the 512-entry array and copies a flag-5 temporary scrollbar body into the
// member (the +0x00 vtable was already set by the first ctor call).
CPlayAwardList* __ct__CPlayAwardList(CPlayAwardList* self, int arg) {
    *(u32*)self = (u32)lbl_eu_80537C20;   // vtable (free-function ctor, manual store)
    new (&self->mMemRegion) UnkClass_8045F564();
    self->mFileHandle = 0;
    self->mFileHandle2 = 0;
    self->mArcAccessor1C = 0;
    self->mLayout20 = 0;
    self->mAnimTrans24 = 0;
    self->mAnimTrans28 = 0;
    self->field_0x2C = 0;
    __ct__CCur18(self->mCursor, 0);
    __ct__CScrollBar(self->mScrollBar, 0);
    self->field_0x88 = 0;
    self->field_0x89 = 0;
    self->field_0x8A = 0;
    self->field_0x8B = 1;
    self->field_0x8C = 0;
    self->field_0x8E = 0;
    self->field_0x90 = 0;
    func_80270654(&self->mEntryArray);

    // Temp scrollbar body copy (retail copies each field: lwz/stw, lbz/stb,
    // lfs/stfs).
    CScrollBarData temp;
    __ct__CScrollBar((u8*)&temp, 5);
    CScrollBarData* dst = reinterpret_cast<CScrollBarData*>(&self->mScrollBar[0]);
    dst->mMemRegion[0] = temp.mMemRegion[0];
    dst->mMemRegion[1] = temp.mMemRegion[1];
    dst->mMemRegion[2] = temp.mMemRegion[2];
    dst->mMemRegion[3] = temp.mMemRegion[3];
    dst->mFileHandle = temp.mFileHandle;
    dst->mAccessor = temp.mAccessor;
    dst->mLayout = temp.mLayout;
    dst->mAnimTrans = temp.mAnimTrans;
    dst->mReady = temp.mReady;
    dst->mVisible = temp.mVisible;
    dst->mState = temp.mState;
    dst->mActive = temp.mActive;
    dst->mAnimOffset = temp.mAnimOffset;
    dst->mScrollPosY = temp.mScrollPosY;
    dst->mScrollRatio = temp.mScrollRatio;
    dst->mThumbHeight = temp.mThumbHeight;
    dst->mContentHeight = temp.mContentHeight;
    dst->mDirection = temp.mDirection;
    __dt__10CScrollBarFv((u8*)&temp, -1);
    return self;
}

/* Destructor (retail D2 form; symbol keeps its C++ mangling). Written as a
 * free function with an explicit dealloc flag: the retail body has no vtable
 * store, and the CScrollBar/CCur18 regions are opaque byte storages whose
 * retail dtors must be invoked explicitly (reverse construction order: entry
 * array, scroll bar, cursor, then the UnkClass_8045F564 region). */
CPlayAwardList* __dt__14CPlayAwardListFv(CPlayAwardList* self, int dealloc) {
    if (self != 0) {
        CPlayAwardEntryArray* entryArr = &self->mEntryArray;
        if (entryArr != 0) {
            __destroy_arr(entryArr->mEntries, (void*)__dt__802706D4, 8, 0x200);
        }
        __dt__10CScrollBarFv(self->mScrollBar, -1);
        __dt__6CCur18Fv(self->mCursor, -1);
        self->mMemRegion.~UnkClass_8045F564();
        if (dealloc > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Load the award-list layout arc files: the list arc via readFile (handle from
// MEM2) and the common archive via readCommonArchiveFile (handle re-queried by
// func_800A9D90). Then build the scroll bar layout and clear the flag byte.
void func_80270CEC(CPlayAwardList* self) {
    self->mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), &lbl_eu_8050E7C0[0x5a], self, 0, 0);
    self->mFileHandle2 = CDeviceFile::readCommonArchiveFile(
        func_800A9D90(), &lbl_eu_8050E7C0[0x74], self, 0, 0);
    func_801F34F4(self->mScrollBar);
    self->field_0x8A = 0;
}

// Per-frame award-list move: when the list is loaded (field_0x88), run the
// state-transition handler for the current state byte, advance the layout
// animation, and update the cursor + scroll bar.
void func_80270D64(CPlayAwardList* self) {
    if (self->field_0x88 != 0) {
        switch (self->field_0x89) {
        case 1:
            func_80271480(self);
            break;
        case 2:
            func_802714D4(self);
            break;
        case 4:
            func_80271528(self);
            break;
        case 5:
            func_80271574(self);
            break;
        }
        self->mLayout20->Animate(0);
        func_801D202C(self->mCursor);
        func_801F3540(self->mScrollBar);
    }
}

// Draw the award-list layout, scroll bar and cursor when the list is loaded
// (field_0x88 nonzero).
void func_80270E04(CPlayAwardList* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_0x88 != 0) {
        func_80137038(self->mLayout20, drawInfo, 0, 1);
        func_801F35B0(self->mScrollBar, drawInfo);
        func_801D20B0(self->mCursor, drawInfo);
    }
}

// Release the award-list widgets: close the two file handles, destroy the
// layout + 0x2C object, release the arc resource accessor, then run the
// cursor/scrollbar/region teardown helpers.
void func_80270E64(CPlayAwardList* self) {
    func_801390E0(&self->mFileHandle);
    func_801390E0(&self->mFileHandle2);
    self->field_0x88 = 0;
    if (self->mLayout20 != 0) {
        delete self->mLayout20;
        self->mLayout20 = 0;
    }
    if (self->field_0x2C != 0) {
        delete reinterpret_cast<CPlayAwardAttrObj*>(self->field_0x2C);
        self->field_0x2C = 0;
    }
    func_80139124(self->mArcAccessor1C);
    reinterpret_cast<CCursor18*>(&self->mCursor)->vf3();
    func_801F35DC(self->mScrollBar);
    self->mMemRegion.func_8045F778();
}

bool CScrollBar_isVisible(void*);

// Award list "ready to show" query: visible when the embedded scroll bar is
// visible AND the flag byte at 0x8A is set. Int return so callers compare
// with cmpwi directly (cf. CMenuTutorialList.hpp).
int func_80270F28(CPlayAwardList* self) {
    if (CScrollBar_isVisible((u8*)self + 0x48)) {
        return self->field_0x8A;
    }
    return 0;
}

int func_80270F6C(CPlayAwardList* self) { return self->field_0x8B; }

void func_80270F74(CPlayAwardList* self) {
    if (self->field_0x89 != 0) return;
    self->field_0x89 = 1;
    self->field_0x8B = 0;
    func_802715C0(self);

    // Scroll the bar in with the 3-float init vector.
    float vec[3];
    vec[0] = lbl_eu_806689B8;
    vec[1] = lbl_eu_806689BC;
    vec[2] = lbl_eu_806689A8;
    func_801F3670(self->mScrollBar, vec);

    func_801F36BC(self->mScrollBar, 9,
                  self->mEntryArray.field_1008[self->mEntryArray.mPageIndex]);

    // Restore the scroll position from the entry-array state (reads as u16,
    // stored into the s16 fields; the >= 9 clamp compares the sign-extended
    // original).
    u16 c = (u16)self->mEntryArray.field_100C;
    u16 e = (u16)self->mEntryArray.field_100E;
    self->field_0x8C = (s16)c;
    self->field_0x8E = (s16)e;
    if ((s16)c >= 9) self->field_0x8C = 8;
    if (self->field_0x8C < 0) self->field_0x8C = 0;
    if (self->field_0x8E < 0) self->field_0x8E = 0;

    self->field_0x90 = self->mEntryArray.mPageIndex;
    func_801F3850(self->mScrollBar, self->field_0x8E);
    func_802717F8(self);
    func_80271680(self);
    func_80271730(self);
}

// Award-list close request (retail func_80271070): when the list is in its
// "open" state (3), mark it closing (4), reset the ready flag, stop the open
// animations, hide the cursor, scroll the bar out and play the close sound.
void func_80271070(CPlayAwardList* self) {
    if (self->field_0x89 == 3) {
        self->field_0x89 = 4;
        self->field_0x8B = 0;
        func_80271620(self);
        func_801D216C(self->mCursor, 0);
        func_801F369C(self->mScrollBar);
        func_80138078(6);
    }
}

// Award-list cursor down: decrement the scroll position, and when it wraps
// past the bottom of the page, move to the previous page (9-entry rows,
// 8-entry remainder page). Refresh the list + scroll bar afterwards.
void func_802710D4(CPlayAwardList* self) {
    u8 page = self->mEntryArray.mPageIndex;
    u8 count = self->mEntryArray.field_1008[page];
    self->field_0x8C = self->field_0x8C - 1;
    if (self->field_0x8C < 0) {
        self->field_0x8C = 0;
        self->field_0x8E = self->field_0x8E - 1;
        if (self->field_0x8E < 0) {
            if (count >= 9) {
                self->field_0x8C = 8;
                self->field_0x8E = count - 9;
            } else {
                s16 c = count - 1;
                self->field_0x8C = c;
                self->field_0x8E = 0;
                if (c < 0) {
                    self->field_0x8C = 0;
                }
            }
        }
    }
    func_80271680(self);
    func_80271730(self);
    func_801F3850(self->mScrollBar, self->field_0x8E);
    func_80138078(1);
}

// Award-list cursor down: advance the scroll position, wrapping through the
// 9-entry page / 8-entry remainder pages. Refresh the list + scroll bar.
void func_80271190(CPlayAwardList* self) {
    u8 count = self->mEntryArray.field_1008[self->mEntryArray.mPageIndex];
    s16 off;
    int delta;
    if (count >= 9) {
        s16 cur = self->field_0x8C + 1;
        self->field_0x8C = cur;
        if (cur >= 9) {
            self->field_0x8C = 8;
            delta = count - 9;
            off = (s16)(self->field_0x8E + 1);
            self->field_0x8E = off;
            if (off > delta) {
                self->field_0x8C = 0;
                self->field_0x8E = 0;
            }
        }
    } else {
        s16 cur = self->field_0x8C + 1;
        self->field_0x8C = cur;
        if (cur >= count) {
            self->field_0x8C = 0;
            self->field_0x8E = 0;
        }
    }
    func_80271680(self);
    func_80271730(self);
    func_801F3850(self->mScrollBar, self->field_0x8E);
    func_80138078(1);
}

// Award-list scroll step: when the current page's entry count reaches 9,
// decrement the scroll offset by 9 (wrapping through the 8-entry remainder),
// otherwise reset both scroll fields. Then refresh the list and scroll bar.
void func_80271260(CPlayAwardList* self) {
    if (self->mEntryArray.field_1008[self->mEntryArray.mPageIndex] >= 9) {
        s16 scroll = self->field_0x8E - 9;
        self->field_0x8E = scroll;
        if (scroll < 0) {
            self->field_0x8C = scroll + 8;
            self->field_0x8E = 0;
            if ((s16)(scroll + 8) < 0) {
                self->field_0x8C = 0;
            }
        }
    } else {
        self->field_0x8C = 0;
        self->field_0x8E = 0;
    }
    func_80271680(self);
    func_80271730(self);
    func_801F3850(self->mScrollBar, self->field_0x8E);
    func_80138078(1);
}

// Award-list cursor up: when the current page's entry count reaches 9,
// advance the scroll offset by 9 (wrapping through the 8-entry remainder),
// otherwise reset both scroll fields. Refresh the list + scroll bar.
void func_80271300(CPlayAwardList* self) {
    u8 count = self->mEntryArray.field_1008[self->mEntryArray.mPageIndex];
    if (count >= 9) {
        int delta = count - 9;
        s16 off = self->field_0x8E + 9;
        self->field_0x8E = off;
        if (off > delta) {
            s16 diff = off - delta;
            self->field_0x8C = diff;
            self->field_0x8E = delta;
            if (diff >= 9) {
                self->field_0x8C = 8;
            }
        }
    } else {
        self->field_0x8C = count - 1;
        self->field_0x8E = 0;
        if (self->field_0x8C < 0) {
            self->field_0x8C = 0;
        }
    }
    func_80271680(self);
    func_80271730(self);
    func_801F3850(self->mScrollBar, self->field_0x8E);
    func_80138078(1);
}

// Page-flip: increment the 0x90 page byte (wrapping 2 -> 0), stash the page
// index for entry lookup, then re-range the scroll bar and refresh the list.
void func_802713BC(CPlayAwardList* self) {
    self->field_0x90++;
    if (self->field_0x90 >= 2) {
        self->field_0x90 = 0;
    }
    self->field_0x8C = 0;
    self->field_0x8E = 0;
    u8 page = (u8)self->field_0x90;
    if (page < 2) {
        self->mEntryArray.mPageIndex = page;
    }
    u8 count = self->mEntryArray.field_1008[self->mEntryArray.mPageIndex];
    func_801F36BC(self->mScrollBar, 9, count);
    func_801F3850(self->mScrollBar, self->field_0x8E);
    func_802717F8(self);
    func_80271680(self);
    func_80271730(self);
    func_80138078(0xa);
}

// Page-flip helper: 0x67 when the 0x90 page byte is nonzero, else 0x66.
s32 func_80271468(CPlayAwardList* self) { return (self->field_0x90 != 0) + 0x66; }

// Award list open transition: once the list's forward animation reaches its
// end, mark the list state as showing (2), run the page/scroll setup and
// request the scroll bar scroll-in.
void func_80271480(CPlayAwardList* self) {
    if (func_80137444(self->mAnimTrans24, lbl_eu_806689C0) != 0) {
        self->field_0x89 = 2;
        func_80271620(self);
        func_801F367C(self->mScrollBar);
    }
}

// Award list open transition: once the list's forward animation reaches its
// end, mark the list state as opening (3), run the page/scroll setup and set
// the ready flag.
void func_802714D4(CPlayAwardList* self) {
    if (func_80137444(self->mAnimTrans28, lbl_eu_806689C0) != 0) {
        self->field_0x89 = 3;
        func_80271730(self);
        self->field_0x8B = 1;
    }
}

// Award list close transition: once the list's reverse animation reaches its
// start, mark the list state as closed (5) and run the close sequence.
void func_80271528(CPlayAwardList* self) {
    if (func_80137510(self->mAnimTrans28, lbl_eu_806689C0) != 0) {
        self->field_0x89 = 5;
        func_802715C0(self);
    }
}

// Award list close transition (reverse): when the list's reverse animation
// reaches its start, reset the state (0) and set the ready flag (1).
void func_80271574(CPlayAwardList* self) {
    if (func_80137510(self->mAnimTrans24, lbl_eu_806689C0) != 0) {
        self->field_0x89 = 0;
        self->field_0x8B = 1;
    }
}

// Award list close transition: bind the reverse/forward animation transforms
// to the layout (anim28 disabled, anim24 enabled).
void func_802715C0(CPlayAwardList* self) {
    self->mLayout20->SetAnimationEnable(self->mAnimTrans28, false);
    self->mLayout20->SetAnimationEnable(self->mAnimTrans24, true);
}

// Award list open transition: bind the forward/reverse animation transforms
// to the layout (anim24 disabled, anim28 enabled).
void func_80271620(CPlayAwardList* self) {
    self->mLayout20->SetAnimationEnable(self->mAnimTrans24, false);
    self->mLayout20->SetAnimationEnable(self->mAnimTrans28, true);
}

// Refresh the 9 award-entry panes of the current page: format each pane name
// and bind the entry words into the layout (first text white, second text in
// the field_0x2C attribute color).
void func_80271680(CPlayAwardList* self) {
    char buf[0x20];
    for (u8 i = 0; i < 9; i++) {
        CPlayAwardEntry* entry = reinterpret_cast<CPlayAwardEntry*>(
            func_80270AEC(&self->mEntryArray, (u8)(i + self->field_0x8E)));
        sprintf(buf, &lbl_eu_8050E7C0[0x93], i + 1);
        func_80136B4C(self->mLayout20, buf, reinterpret_cast<char*>(entry->word0), 0);
        sprintf(buf, &lbl_eu_8050E7C0[0x9f], i + 1);
        func_80136B4C(self->mLayout20, buf, reinterpret_cast<char*>(entry->word1),
                      self->field_0x2C);
    }
}

// Refresh the cursor position: format the current pane name, show the cursor,
// find the two anchor panes, translate the cursor's target position and move
// the cursor to it (retail reloads the root pane each time).
void func_80271730(CPlayAwardList* self) {
    nw4r::math::VEC3 pos;
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050E7C0[0x93], self->field_0x8C + 1);
    func_801D216C(self->mCursor, 1);
    nw4r::lyt::Pane* pane1 = self->mLayout20->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = self->mLayout20->GetRootPane()->FindPaneByName(&lbl_eu_8050E7C0[0xab], true);
    func_80137924(&pos, pane1, pane2, self->mLayout20->GetRootPane());
    reinterpret_cast<CCursor18*>(&self->mCursor)->vf4(&pos);
}

void func_802717F8(CPlayAwardList* self) {
    // Load the two page-title strings ("%s"-style templates from the pool).
    char* s1;
    char* s2;
    void* fp = getFP__FPCc(&lbl_eu_8050E7C0[0x14]);
    s1 = func_8013639C(fp, &lbl_eu_8050E7C0[0x1f], 0xa);
    s2 = func_8013639C(fp, &lbl_eu_8050E7C0[0x1f], 0xb);
    func_80136B4C(self->mLayout20, &lbl_eu_8050E7C0[0xba],
                  self->field_0x90 != 0 ? s2 : s1, 0);

    // Resolve the award title and the two per-page help strings, then look up
    // the 'timg' texture for the page and size its pane from the row/col.
    char* s5;
    char* s4 = func_80136190(&lbl_eu_8050E7C0[0xc4], &lbl_eu_8050E7C0[0xd2], 0x8a);
    s5 = func_80136190(&lbl_eu_8050E7C0[0xc4], &lbl_eu_8050E7C0[0xd2], 0x89);
    const char* sel = func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0
                          ? &lbl_eu_8050E7C0[0xd7]
                          : &lbl_eu_8050E7C0[0xe0];
    u16 msgId = func_8013606C(&lbl_eu_8050E7C0[0xc4], sel, 0x3a);
    char* texName = func_80138F78((u32)msgId);
    nw4r::lyt::ArcResourceAccessor* mgr = func_801355F4();
    CPlayAwardMsgObj* obj = reinterpret_cast<CPlayAwardMsgObj*>(
        mgr->GetResource(0x74696D67, texName, 0));
    if (obj != 0) {
        func_80137E7C(self->mLayout20, &lbl_eu_8050E7C0[0xe9], obj);
        CPlayAwardCoords* coords = obj->chain->pCoords;
        u16 row = coords->row;
        u16 col = coords->col;
        nw4r::lyt::Pane* pane = self->mLayout20->GetRootPane()->FindPaneByName(
            &lbl_eu_8050E7C0[0xe9], true);
        if (pane != 0) {
            float src[2];
            src[0] = (float)row;
            src[1] = (float)col;
            reinterpret_cast<CPlayAwardPaneSize*>(pane)->width = src[0];
            reinterpret_cast<CPlayAwardPaneSize*>(pane)->height = src[1];
        }
    }

    // Bind the page title/help strings (help string differs by page).
    func_80136B4C(self->mLayout20, &lbl_eu_8050E7C0[0xf5],
                  self->field_0x90 == 0 ? s5 : s4, 0);
    char* s6 = func_80136190(&lbl_eu_8050E7C0[0xfd], &lbl_eu_8050E7C0[0x108], 0x19);
    func_80136B4C(self->mLayout20, &lbl_eu_8050E7C0[0x10d], s6, 0);
}

// Award-list load handler: when the layout arc accessor is ready and the
// common archive has finished loading, build the entry array on the stack and
// copy it into the embedded region, then refresh the list.
void func_802719F8(CPlayAwardList* self) {
    if (self->mArcAccessor1C != 0 && self->mFileHandle2 == 0) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    self->field_0x8A = 1;
    self->field_0x88 = 1;
    CPlayAwardEntryArray tmp;
    func_80270654(&tmp);
    for (u32 i = 0; i < 0x200; i++) {
        self->mEntryArray.mEntries[i] = tmp.mEntries[i];
    }
    self->mEntryArray.field_1000 = tmp.field_1000;
    self->mEntryArray.field_1001 = tmp.field_1001;
    self->mEntryArray.field_1004 = tmp.field_1004;
    // Copy both per-page counts as one halfword (retail lhz/sth pair).
    *(u16*)&self->mEntryArray.field_1008[0] = *(u16*)&tmp.field_1008[0];
    self->mEntryArray.mPageIndex = tmp.mPageIndex;
    self->mEntryArray.field_100C = tmp.field_100C;
    self->mEntryArray.field_100E = tmp.field_100E;
    __destroy_arr(tmp.mEntries, (void*)&__dt__802706D4, 8, 0x200);
    func_80270770(&self->mEntryArray);
    func_802717F8(self);
    func_80271680(self);
}

// File-load completion callback: when the award-list arc (mFileHandle) is
// ready, build the whole layout (region scratch guard, tag processor, arc
// accessor, layout + anim transforms, font binding, text bindings, award
// panes, cursor); when the common archive (mFileHandle2) is ready, detach its
// buffer into the BDAT tables and refresh. Returns 1 when handled.
bool CPlayAwardList::OnFileEvent(CEventFile* event) {
    if (mFileHandle == event->mFileHandle) {
        // Scratch heap region guard around the layout build.
        u8 regionBuf[8];
        char buf[0x20];
        u8 tmpCur[0x18];
        mMemRegion.createRegion((int)mtl::MemManager::getHandleMEM2(), 0x10000,
                                &lbl_eu_8050E7C0[0x118], 0);
        __ct__14Class_8045F858FP17UnkClass_8045F564(regionBuf, &mMemRegion);

        // Detach the loaded file buffer and hand it to the layout accessor.
        void* fileData = mFileHandle->getData();
        func_80434A4C__Q23mtl10MemManagerFb(false);

        // Allocate the tag processor (0x858 bytes, layout memory pool).
        void* tagProc = allocate__Q23mtl10MemManagerFUlUl(
            0x858, getAllocHandle__10CLibLayoutFv());
        if (tagProc != 0) {
            tagProc = __ct__CTagProcessor(tagProc);
        }
        field_0x2C = (u32)tagProc;

        // Build the layout + forward/reverse anim transforms from the arc.
        mArcAccessor1C = (nw4r::lyt::ArcResourceAccessor*)
            createArcResourceAccessor__10CLibLayoutFv();
        mArcAccessor1C->Attach(fileData, &lbl_eu_8050E7C0[0x127]);
        func_80136E84(&mLayout20, mArcAccessor1C, &lbl_eu_8050E7C0[0x12b]);
        func_80136F08(mLayout20, &mAnimTrans24, mArcAccessor1C,
                      &lbl_eu_8050E7C0[0x13e]);
        func_80136F08(mLayout20, &mAnimTrans28, mArcAccessor1C,
                      &lbl_eu_8050E7C0[0x154]);

        // Bind the shared font into the layout's root pane.
        nw4r::lyt::Pane* rootPane = mLayout20->GetRootPane();
        CPlayAwardFontObj* fontObj = (CPlayAwardFontObj*)
            func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mLayout20);
        u32 fontResult = fontObj->getFontHandle();
        func_8013676C(rootPane, fontResult);

        func_801368C0(mLayout20, &lbl_eu_8050E7C0[0x16f], func_801355BC());
        func_802715C0(this);
        mLayout20->Animate(0);

        // Bind the three page-title/help strings.
        void* fp = getFP__FPCc(&lbl_eu_8050E7C0[0x14]);
        func_80136B4C(mLayout20, &lbl_eu_8050E7C0[0xba],
                      func_8013639C(fp, &lbl_eu_8050E7C0[0x1f], 0xa), 0);
        func_80136B4C(mLayout20, &lbl_eu_8050E7C0[0x179],
                      func_8013639C(fp, &lbl_eu_8050E7C0[0x1f], 0xb), 0);
        func_80136B4C(mLayout20, &lbl_eu_8050E7C0[0x16f],
                      func_8013639C(fp, &lbl_eu_8050E7C0[0x1f], 0xd), 0);

        // Bind the eight award panes and stash the tag processor on each.
        for (u8 i = 1; i <= 9; i++) {
            sprintf(buf, &lbl_eu_8050E7C0[0x9f], i);
            nw4r::lyt::Pane* pane =
                mLayout20->GetRootPane()->FindPaneByName(buf, true);
            reinterpret_cast<CPlayAwardPaneUser*>(pane)->field_F8 = field_0x2C;
        }

        // Build the cursor on the stack, copy its body into the member region
        // (skipping the +0x00 vtable pointer) and destroy the temp.
        __ct__CCur18(tmpCur, func_801355F4());
        CCur18Data* curDst = reinterpret_cast<CCur18Data*>(&mCursor[0]);
        CCur18Data* curSrc = reinterpret_cast<CCur18Data*>(tmpCur);
        curDst->field_4 = curSrc->field_4;
        curDst->field_8 = curSrc->field_8;
        curDst->field_C = curSrc->field_C;
        curDst->field_10 = curSrc->field_10;
        curDst->field_14 = curSrc->field_14;
        curDst->field_15 = curSrc->field_15;
        __dt__6CCur18Fv(tmpCur, -1);
        reinterpret_cast<CCursor18*>(&mCursor)->vf2();

        // Refresh the list, release the file handle and the scratch region.
        func_802719F8(this);
        mFileHandle = 0;
        mMemRegion.func_8045F810();
        __dt__14Class_8045F858Fv(regionBuf, -1);
        return 1;
    }
    if (mFileHandle2 == event->mFileHandle) {
        // Common archive finished: detach its buffer into the BDAT tables.
        void* fileData2 = mFileHandle2->getData();
        func_8003AA78__5CBdatFUlPv(2, fileData2);
        func_8003AA34();
        lbl_eu_806648A8 = getFP__FPCc(&lbl_eu_8050E7C0[0x183]);
        mFileHandle2 = 0;
        func_802719F8(this);
        return 1;
    }
    return 0;
}

extern "C" unsigned long func_8027037C(void) { return lbl_eu_806648A0 != 0; }
