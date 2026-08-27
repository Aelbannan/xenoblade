// Auto-scaffolded catalog TU for kyoshin/CTutorial
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/CTutorial.hpp"

// func_80136910 and friends are declared in CTutorial.hpp; the shared
// code_80135FDC.hpp declaration of func_80136910 (u8 third parameter) is
// deliberately NOT included here - retail's caller saw an int-width
// parameter, which changes MWCC's argument-extension codegen.

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include <nw4r/lyt.h>

#include <functions.hpp>

// Fake SI interface for the nw4r::lyt::Layout deleting-destructor dispatch at
// vtable slot 2 (+0x8); real virtual dispatch reproduces the retail
// `lwz r12,0(r3); lwz r12,8(r12); mtctr; bctrl` sequence.
struct CTutorialLayoutDtorVt {
    virtual void destroy(u32 flags);
};

// Constructor: install the IWorkEvent vtable, construct the two memory
// regions (in declaration order), null every pointer, then set the state
// bytes - 0x47 defaults to visible; the two ctor args land at 0x48 (locale)
// and 0x53 (region/flags).
CTutorial::CTutorial(u8 param_1, u8 param_2) : CTutorialVtblBase() {
    mFileHandle0 = nullptr;
    mFileHandle1 = nullptr;
    mFileHandle2 = nullptr;
    mAccessor0 = nullptr;
    mAccessor1 = nullptr;
    mpLayout = nullptr;
    mpAnimTrans0 = nullptr;
    mpAnimTrans1 = nullptr;
    field_44 = 0;
    field_45 = 0;
    field_46 = 0;
    field_47 = 1;
    field_48 = param_1;
    field_4C = nullptr;
    field_50 = 0;
    field_51 = 0;
    field_52 = 0;
    field_53 = param_2;
}

u8 CTutorial::func_8029ACAC() { return this->field_46; }
u8 CTutorial::func_8029ACB4() { return this->field_47; }
u8 CTutorial::func_8029ACBC() { return this->field_52; }

/* Draw the tutorial layout when the active gate (0x44) is set. r4 (the draw
 * info) is passed straight through to the layout draw helper. */
void CTutorial::func_8029ABB8(nw4r::lyt::DrawInfo* drawInfo) {
    if (field_44 != 0) {
        func_80137038(mpLayout, drawInfo, 0, 1);
    }
}

/* Idempotent start: if not already running, mark state 1 and fire the 0x8
 * UI sound/effect event. */
void CTutorial::func_8029ACC4() {
    if (field_45 == 0) {
        field_45 = 1;
        field_47 = 0;
        func_80138078(8);
    }
}

u8 CTutorial::func_8029AE5C() {
    s8 last = field_51;
    s8 cur = field_50;
    if (cur == last - 1) {
        if (last == 1) {
            return 2;
        }
        return 3;
    }
    return cur != 0;
}

// Advance-anim start: disable anim0 / enable anim1, mark state 2, and re-bind
// both transforms onto the layout before kicking an animation tick.
void CTutorial::func_8029AE9C() {
    if (func_80137444(mpAnimTrans0, lbl_eu_80668C08) != 0) {
        field_45 = 2;
        mpLayout->SetAnimationEnable(mpAnimTrans0, false);
        mpLayout->SetAnimationEnable(mpAnimTrans1, true);
        mpLayout->Animate(0);
    }
}

// Compiler-generated complete-object dtor: destroy the two memory regions in
// reverse declaration order, then free the object when the delete flag is set.
CTutorial::~CTutorial() {}

/* Advance-animation (0x40) reached the end frame: state 3, visible. */
__declspec(noinline) void CTutorial::func_8029AF30() {
    if (func_80137444(mpAnimTrans1, lbl_eu_80668C08) != 0) {
        field_45 = 3;
        field_47 = 1;
    }
}

// Rewind-anim start: mirror of func_8029AE9C with the transforms swapped.
void CTutorial::func_8029AF7C() {
    if (func_80137510(mpAnimTrans1, lbl_eu_80668C08) != 0) {
        field_45 = 5;
        mpLayout->SetAnimationEnable(mpAnimTrans1, false);
        mpLayout->SetAnimationEnable(mpAnimTrans0, true);
        mpLayout->Animate(0);
    }
}

/* Rewind-animation (0x3C) reached the start frame: state 0, visible. */
__declspec(noinline) void CTutorial::func_8029B010() {
    if (func_80137510(mpAnimTrans0, lbl_eu_80668C08) != 0) {
        field_45 = 0;
        field_47 = 1;
    }
}

/* Refresh the tutorial page: set both text panes from the current/bound page
 * counters, sprintf the locale-specific texture name, fetch it from the
 * loaded locale archive (mFileHandle1 actually holds an accessor-like object)
 * and bind it onto the layout. */
void CTutorial::func_8029B124() {
    char buf[0x20];
    // Sign-extend before increment: retail computes extsb(field_50)+1.
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(mpLayout, &lbl_eu_80510290[0x79], field_50 + 1);
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(mpLayout, &lbl_eu_80510290[0x82], field_51);
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        sprintf(buf, &lbl_eu_80510290[0x8b], field_48, field_50 + 1);
    } else {
        sprintf(buf, &lbl_eu_80510290[0xa0], field_48, field_50 + 1);
    }
    void* tex = reinterpret_cast<nw4r::lyt::ResourceAccessor*>(mFileHandle1)
                    ->GetResource(0x74696d67, buf, 0);
    if (tex != NULL) {
        func_80137E7C(mpLayout, &lbl_eu_80510290[0xb5], tex);
    }
}

// Tutorial data reload: validate the loaded resources, resolve the BDAT
// text-table pointer when the region gate (0x53) is clear, then reset the
// page counters and refresh.
extern "C" void func_8029B05C(CTutorial* self) {
    if (self->field_53 != 0) {
        if (self->mAccessor0 == nullptr || self->field_4C == nullptr ||
            self->mAccessor1 == nullptr)
            return;
    } else {
        if (self->mAccessor0 == nullptr || self->mAccessor1 == nullptr)
            return;
        // Resolve the tutorial BDAT table handle by its pooled tag string.
        func_8003AA34();
        lbl_eu_80664A30 = (u32)getFP(&lbl_eu_80510290[0x6b]);
    }
    self->field_46 = 1;
    self->field_44 = 1;
    self->field_50 = 0;
    self->field_51 =
        (s8)func_801361E8(lbl_eu_80664A30, &lbl_eu_80510290[0x74], self->field_48);
    self->func_8029B124();}

// Reset a block of UI flags (0x3340..0x33BE) owned by this tutorial, then set
// the owner id 0x270 entry to a heap/direct-address classification mask.
void CTutorial::func_8029B498() {
    s16 i = 0x3340;
    while (i < 0x33bf) {
        func_8009D018(i, (u32)this);
        i++;
    }
    // (a | -a) >> 31 (arithmetic) is -1 for any nonzero address, 0 for null;
    // masked to the 0x7F000000 window.
    s32 addr = (s32)this;
    func_8009D018(0x270, ((addr | -addr) >> 31) & 0x7f000000);
}

// Page-counter tick: play the confirm sound while the counter is nonzero,
// decrement it (u8-domain), clamp negatives to zero, then refresh.
extern "C" void func_8029ADF8(CTutorial* self) {
    if (self->field_50 != 0)
        func_80138078(8);
    u8 next = (u8)self->field_50 - 1;
    self->field_50 = (s8)next;
    if ((s8)next < 0)
        self->field_50 = 0;
    self->func_8029B124();
}

// Font object returned by CDeviceFont::getFontInfo: vtable slot 9 (+0x24)
// yields the u32 font handle bound into the layout. Never instantiated, so no
// vtable is emitted; a genuine virtual call reproduces the retail dispatch.
struct CTutorialFontView {
    virtual void sf2() = 0;
    virtual void sf3() = 0;
    virtual void sf4() = 0;
    virtual void sf5() = 0;
    virtual void sf6() = 0;
    virtual void sf7() = 0;
    virtual void sf8() = 0;
    virtual u32 sf9() = 0; // vtable offset 0x24
};

/* File-completion dispatcher: whichever of the three requested files arrived
 * determines the branch. Handle 0 (layout arc) builds the whole layout stack;
 * handle 1 (locale data) registers the BDAT table; handle 2 (game data)
 * attaches its archive then refreshes. Returns whether the event was ours. */
bool CTutorial::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle0 == pEventFile->mFileHandle) {
        // Scratch heap region (RAII Class_8045F858 guard).
        mRegion0.createRegion(mtl::MemManager::getHandleMEM2(), 0x1800,
                              &lbl_eu_80510290[0xbd], 0);
        Class_8045F858 regionHost(&mRegion0);

        // Take ownership of the loaded arc buffer.
        void* arcData = mFileHandle0->getData();
        mtl::MemManager::setMemInitFlag(false);

        mAccessor0 = CLibLayout::createArcResourceAccessor();
        mAccessor0->Attach(arcData, &lbl_eu_80510290[0xc7]);

        func_80136E84(&mpLayout, mAccessor0, &lbl_eu_80510290[0xcb]);
        func_80136F08(mpLayout, &mpAnimTrans0, mAccessor0,
                      &lbl_eu_80510290[0xe2]);
        func_80136F08(mpLayout, &mpAnimTrans1, mAccessor0,
                      &lbl_eu_80510290[0xfc]);

        // Bind the shared font onto the root pane (retail loads the root pane
        // before fetching the font handle from vtable slot 9).
        nw4r::lyt::Pane* rootPane = mpLayout->GetRootPane();
        u32 fontResult = static_cast<CTutorialFontView*>(
                             CDeviceFont::getFontInfo(1, mpLayout))->sf9();
        func_8013676C(rootPane, fontResult);

        u32 lang = func_801355A0();
        if (lang != 0) {
            func_801368C0(mpLayout, &lbl_eu_80510290[0x79], lang);
            func_801368C0(mpLayout, &lbl_eu_80510290[0x82], lang);
        }

        // Start paused on the rewind transform (anim0 enabled, anim1 not).
        mpLayout->SetAnimationEnable(mpAnimTrans1, false);
        mpLayout->SetAnimationEnable(mpAnimTrans0, true);
        mpLayout->Animate(0);

        mFileHandle0 = nullptr;
        mRegion0.func_8045F810();
        return true;
    }
    if (mFileHandle1 == pEventFile->mFileHandle) {
        // Locale data: hand the buffer to the BDAT system, resolve the shared
        // tutorial table, then refresh the page state.
        void* data = mFileHandle1->mData;
        mFileHandle1->mData = nullptr;
        field_4C = (u8*)data;
        CBdat::func_8003AA78(4, data);
        func_8003AA34();
        lbl_eu_80664A30 = (u32)getFP(&lbl_eu_80510290[0x6b]);
        func_8029B05C(this);
        mFileHandle1 = nullptr;
        return true;
    }
    if (mFileHandle2 == pEventFile->mFileHandle) {
        // Game data archive: attach to a fresh accessor, then refresh.
        mRegion1.createRegion(mtl::MemManager::getHandleMEM2(), 0x1000,
                              &lbl_eu_80510290[0x11b], 0);
        Class_8045F858 regionHost(&mRegion1);

        void* arcData = mFileHandle2->getData();
        mtl::MemManager::setMemInitFlag(false);

        mAccessor1 = CLibLayout::createArcResourceAccessor();
        mAccessor1->Attach(arcData, &lbl_eu_80510290[0xc7]);

        func_8029B05C(this);
        mFileHandle2 = nullptr;
        mRegion1.func_8045F810();
        return true;
    }
    return false;
}

// Unload: wait for draw done, release BDAT index 4 when the region gate is
// clear, close the three file handles, destroy + clear the layout, release
// both accessors, free the data buffer, clear the shared BDAT pointer, then
// tear down both memory regions in declaration order.
void CTutorial::func_8029ABD8() {
    CDeviceVI::waitForDrawDone();
    if (field_53 != 0) {
        CBdat::getEntry(4);
    }
    func_801390E0(&mFileHandle0);
    func_801390E0(&mFileHandle1);
    func_801390E0(&mFileHandle2);
    field_44 = 0;
    nw4r::lyt::Layout* layout = mpLayout;
    if (layout != nullptr) {
        if (layout != nullptr) {
            reinterpret_cast<CTutorialLayoutDtorVt*>(layout)->destroy(1);
        }
        mpLayout = nullptr;
    }
    func_80139124(mAccessor0);
    mAccessor0 = nullptr;
    func_80139124(mAccessor1);
    mAccessor1 = nullptr;
    if (field_4C != nullptr) {
        mtl::MemManager::deallocate(field_4C);
        field_4C = nullptr;
    }
    lbl_eu_80664A30 = 0;
    mRegion0.func_8045F778();
    mRegion1.func_8045F778();
}

// Load the three tutorial data files: the layout arc, then (when the region
// gate 0x53 is set) a locale-specific data file whose path is sprintf-ed, and
// finally a game data file whose path depends on the CfGameManager flag.
void CTutorial::func_8029AA34() {
    char buf[0x40];
    mFileHandle0 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), lbl_eu_80510290,
        reinterpret_cast<IWorkEvent*>(this), 0, 0);
    if (field_53 != 0) {
        // Format string has no conversions - retail passes no varargs here
        // (the stale r5 in the retail asm is leftover from the previous call).
        sprintf(buf, &lbl_eu_80510290[0x15]);
        mFileHandle1 = CDeviceFile::readFile(
            mtl::MemManager::getHandleMEM2(), buf,
            reinterpret_cast<IWorkEvent*>(this), 0, 0);
    }
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        sprintf(buf, &lbl_eu_80510290[0x33], field_48);
    } else {
        sprintf(buf, &lbl_eu_80510290[0x4f], field_48);
    }
    mFileHandle2 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), buf,
        reinterpret_cast<IWorkEvent*>(this), 0, 0);
    field_46 = 0;
}
extern "C" void func_8029AB28(CTutorial* self) {
    if (self->field_44 == 0) {
        return;
    }
    switch (self->field_45) {
    case 1:
        self->func_8029AE9C();
        break;
    case 2:
        self->func_8029AF30();
        break;
    case 4:
        self->func_8029AF7C();
        break;
    case 5:
        self->func_8029B010();
        break;
    default:
        break;
    }
    self->mpLayout->Animate(0);
}

/* Advance-anim fully finished: state 4, hide, swap back to anim0 (rewind
 * transform) and re-tick the layout, then play the completion sound. */
void CTutorial::func_8029ACEC() {
    if (field_45 == 3) {
        field_45 = 4;
        field_47 = 0;
        mpLayout->SetAnimationEnable(mpAnimTrans1, false);
        mpLayout->SetAnimationEnable(mpAnimTrans0, true);
        mpLayout->Animate(0);
        func_80138078(9);
    }
}
// Page-navigation helper: mark complete when the counter already sits on the
// last page, else advance it clamped to bound-1, then refresh and play the
// confirm sound.
void CTutorial::func_8029AD88() {
    if ((s8)field_50 == (s8)field_51 - 1) {
        field_52 = 1;
        return;
    }
    field_50 = field_50 + 1;
    if ((s8)(u8)(field_50) >= (s8)field_51) {
        field_50 = field_51 - 1;
    }
    func_8029B124();
    func_80138078(8);
}

