// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPlayAward
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/CTutorial.hpp"     // func_80138078 (retail func_80138078__FUl)
#include "kyoshin/code_80135FDC.hpp" // func_80137510 / func_80137444 (anim frame advance)
#include "kyoshin/menu/CMenuPlayAward.hpp"
#include "monolib/device/CDeviceFile.hpp"  // CDeviceFile::readFile / readCommonArchiveFile
#include "monolib/device/CDeviceVI.hpp"       // CDeviceVI::waitForDrawDone
#include "monolib/util/MemManager.hpp"        // mtl::MemManager::allocate
#include "monolib/work/CProcess.hpp"          // CProcess::Regist
#include "monolib/work/CWorkThreadSystem.hpp" // CWorkThreadSystem::getWorkMem
// CScn (with removeRenderCB) comes from the minimal declaration in
// CTaskGameEff.hpp via harness_catalog.hpp -- CScn.hpp redefines it.

// Retail constructor symbol (unmangled global in US). Stub body: the factory
// (func_80270308) needs a real `bl` to the bare retail symbol, so the ctor is
// kept out-of-line with __declspec(noinline).
__declspec(noinline) CMenuPlayAward* __ct__CMenuPlayAward(CMenuPlayAward* self, CScn* scene) {
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

void CMenuPlayAward::Init() {}

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

void CMenuPlayAward::Move() {}

void CMenuPlayAward::cbRenderBefore() {}

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

void func_80270454(){}

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

void func_80270770(){}

// Copy an 8-byte award entry (load both words before storing, like entries[i] = src).
void func_80270AD8(CPlayAwardEntry* dst, const CPlayAwardEntry* src) { *dst = *src; }

// Entry lookup: array base + (pageIndex << 11) + ((param & 0xFF) << 3);
// returns 0 when param >= 0x100.
u8* func_80270AEC(CPlayAwardEntryArray* self, int param) {
    if (param >= 256) return 0;
    return (u8*)self + (self->mPageIndex << 11) + ((param & 0xFF) << 3);
}

void __ct__CPlayAwardList(){}

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

void func_80270E64(CPlayAwardList* self) {}

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

void func_80270F74(CPlayAwardList* self) {}

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

void func_802710D4(){}

void func_80271190(){}

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

void func_80271300(){}

void func_802713BC(){}

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

void func_80271680(CPlayAwardList* self) {}

void func_80271730(CPlayAwardList* self) {}

void func_802717F8(){}

void func_802719F8(){}

void CPlayAwardList::OnFileEvent() {}

extern "C" unsigned long func_8027037C(void) { return lbl_eu_806648A0 != 0; }
