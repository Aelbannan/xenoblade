#include "kyoshin/menu/CMenuVision.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/lib/CLibStaticData.hpp"

// Globals shared across menu units
extern "C" {
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
extern const f32 lbl_eu_80667DC0; // 0.0f
extern const f32 lbl_eu_80667DC4; // 1.0f
extern const f32 lbl_eu_80667DC8; // scale factor
extern char lbl_eu_80504268[];    // string table base
extern u32 lbl_eu_805041C0[];     // 6 pane-name pointers
}

extern u32 func_801355A0();

extern void func_801AFAD0(CMenuVision*, CMenuVisionEntry*);
extern f32 func_800F4424(void*);
extern u32 lbl_eu_80664388;

// Pad accessor for pane float fields at known retail offsets
struct PaneTransAccess {
    u8 _pad[0x4C];
    f32 transX;
    f32 transY;
};

// --- Stubs for non-target functions ---
void __ct__CMenuVision(){}

CMenuVision::~CMenuVision() {}

extern "C" unsigned long func_801AC088() {
    extern unsigned long lbl_eu_80664388;
    return lbl_eu_80664388 != 0;
}

void func_801AC09C(){}

void func_801AC124(){}

void func_801AC1F8(){}

void CMenuVision::Term() {}

void CMenuVision::cbRenderBefore() {}

void func_801ACCE0(){}

void func_801ACD5C(){}

void func_801AD504(){}

void func_801AF934(){}

void func_801AFAD0(CMenuVision*, CMenuVisionEntry*){}

// Mangled linker names used by adjustor thunks below

void func_801AFE04(void* self) { ((void(*)(void*))__dt__11CMenuVisionFv)((char*)self - 0x58); }

void func_801AFE0C(void* self) { ((void(*)(void*))cbRenderBefore__11CMenuVisionFv)((char*)self - 0x5c); }

void func_801AFE14(void* self) { ((void(*)(void*))__dt__11CMenuVisionFv)((char*)self - 0x5c); }

void sinit_801AFCE8(){}

// --- CMenuVision::Move ---
void CMenuVision::Move() {
    // Gate: return early if game paused, event busy, or UI suppressed
    if (CTaskGame::getInstance()->func_800426F0()) {
        return;
    }
    if (lbl_eu_80663E28 & 0x400) { // bit 10 (IBM bit 21)
        return;
    }
    if (!func_8013BE50()) {
        return;
    }
    if (lbl_eu_80663E24 & 0x40) { // bit 6 (IBM bit 25)
        return;
    }

    // Check if any entry has non-zero state - if all are zero, skip update
    bool anyActive = false;
    for (int i = 0; i < 6; i++) {
        if (mEntries[i].mState != 0) {
            anyActive = true;
            break;
        }
    }
    if (!anyActive) {
        return;
    }

    f32 scale = lbl_eu_80667DC8;
    f32 zero = lbl_eu_80667DC0;
    const char* strBase = lbl_eu_80504268;

    for (int i = 0; i < 6; i++) {
        CMenuVisionEntry& e = mEntries[i];
        nw4r::lyt::Layout* layout = e.mLayout;

        switch (e.mState) {
        case 1:
            // Play mAnim1 to 1.0f; when done, advance to state 3
            if (func_80137444(e.mAnim1, lbl_eu_80667DC4)) {
                e.mState = 3;
            }
            break;

        case 2: {
            void* bmObj = cf::CBattleManager::getInstance()->func_800EA444();
            if (bmObj != NULL) {
                // Check if the first indicator pane is visible
                nw4r::lyt::Pane* pane1 = layout->GetRootPane()->FindPaneByName(strBase + 0x195, true);
                if (reinterpret_cast<PaneVisAccess*>(pane1)->visByte & 1) {
                    // Read transX/transY from position pane, scale transX
                    nw4r::lyt::Pane* posPane = layout->GetRootPane()->FindPaneByName(strBase + 0x215, true);
                    f32 savedX = reinterpret_cast<PaneTransAccess*>(posPane)->transX;
                    f32 savedY = reinterpret_cast<PaneTransAccess*>(posPane)->transY;
                    savedX = scale * func_800F4424(bmObj);
                    // Re-find and write back
                    posPane = layout->GetRootPane()->FindPaneByName(strBase + 0x215, true);
                    reinterpret_cast<PaneTransAccess*>(posPane)->transX = savedX;
                    reinterpret_cast<PaneTransAccess*>(posPane)->transY = savedY;
                }
            } else {
                // Disable all anims except mAnim2, set mAnim2 to frame 0, advance to state 3
                layout->SetAnimationEnable(e.mAnim3, false);
                layout->SetAnimationEnable(e.mAnim4, false);
                layout->SetAnimationEnable(e.mAnim5, false);
                layout->SetAnimationEnable(e.mAnim6, false);
                layout->SetAnimationEnable(e.mAnim7, false);
                layout->SetAnimationEnable(e.mAnim8, false);
                layout->SetAnimationEnable(e.mAnim1, false);
                layout->SetAnimationEnable(e.mAnim2, true);
                e.mAnim2->SetFrame(zero);
                e.mState = 3;
            }
            break;
        }

        case 3:
            // Play mAnim2 to 1.0f; when done, return to state 0
            if (func_80137444(e.mAnim2, lbl_eu_80667DC4)) {
                e.mState = 0;
            }
            break;

        case 4:
            // Play mAnim4 to 1.0f; when done, return to state 0
            if (func_80137444(e.mAnim4, lbl_eu_80667DC4)) {
                e.mState = 0;
            }
            break;

        case 5:
            func_801AFAD0(this, &e);
            break;
        }

        layout->Animate(0);
    }
}

// --- CMenuVision::Init ---
void CMenuVision::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mLayoutMem.createRegion(handle, 0x2C000, lbl_eu_80504268, 0);

    Class_8045F858 regionGuard(&mLayoutMem);

    // Copy 6 pane-name pointers from lbl_eu_805041C0 to local array
    const char* paneNames[6];
    paneNames[0] = (const char*)lbl_eu_805041C0[0];
    paneNames[1] = (const char*)lbl_eu_805041C0[1];
    paneNames[2] = (const char*)lbl_eu_805041C0[2];
    paneNames[3] = (const char*)lbl_eu_805041C0[3];
    paneNames[4] = (const char*)lbl_eu_805041C0[4];
    paneNames[5] = (const char*)lbl_eu_805041C0[5];

    nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
    const char* strBase = lbl_eu_80504268;
    f32 zero = lbl_eu_80667DC0;

    for (int i = 0; i < 6; i++) {
        CMenuVisionEntry& e = mEntries[i];

        func_80136E84(&e.mLayout, accessor, (char*)strBase + 0xb);
        nw4r::lyt::Layout* layout = e.mLayout;

        func_80136F08(layout, &e.mAnim1, accessor, (char*)strBase + 0x2b);
        func_80136F08(layout, &e.mAnim2, accessor, (char*)strBase + 0x4e);
        func_80136F08(layout, &e.mAnim3, accessor, (char*)strBase + 0x72);
        func_80136F08(layout, &e.mAnim4, accessor, (char*)strBase + 0x9a);
        func_80136F08(layout, &e.mAnim5, accessor, (char*)strBase + 0xc5);
        func_80136F08(layout, &e.mAnim6, accessor, (char*)strBase + 0xef);
        func_80136F08(layout, &e.mAnim7, accessor, (char*)strBase + 0x119);
        func_80136F08(layout, &e.mAnim8, accessor, (char*)strBase + 0x13f);

        // Disable anims 2-8, enable anim1, reset to frame 0
        layout->SetAnimationEnable(e.mAnim2, false);
        layout->SetAnimationEnable(e.mAnim3, false);
        layout->SetAnimationEnable(e.mAnim4, false);
        layout->SetAnimationEnable(e.mAnim5, false);
        layout->SetAnimationEnable(e.mAnim6, false);
        layout->SetAnimationEnable(e.mAnim7, false);
        layout->SetAnimationEnable(e.mAnim8, false);
        layout->SetAnimationEnable(e.mAnim1, true);
        e.mAnim1->SetFrame(zero);
        layout->Animate(0);

        // Hide 6 standard panes by clearing bit 0 of visByte
        nw4r::lyt::Pane* rootPane = layout->GetRootPane();
        nw4r::lyt::Pane* p;

        p = rootPane->FindPaneByName(strBase + 0x169, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = rootPane->FindPaneByName(strBase + 0x174, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = rootPane->FindPaneByName(strBase + 0x17f, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = rootPane->FindPaneByName(strBase + 0x18a, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = rootPane->FindPaneByName(strBase + 0x195, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = rootPane->FindPaneByName(strBase + 0x1a2, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;

        // Show the entry-specific pane (clear all bits, then set bit 0)
        p = rootPane->FindPaneByName(paneNames[i], true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        reinterpret_cast<PaneVisAccess*>(p)->visByte |= 1;

        // Per-index special setup
        if (i == 1) {
            u8* fontObj = (u8*)CDeviceFont::func_80452C10(1, layout);
            typedef u32 (*FontVFn)(void*);
            u32 fontVal = (*reinterpret_cast<FontVFn**>(fontObj))[0x24 / 4](fontObj);

            func_801368C0(layout, (char*)strBase + 0x1ab, fontVal);
            func_801368C0(layout, (char*)strBase + 0x1b8, fontVal);
            func_801368C0(layout, (char*)strBase + 0x1c5, fontVal);
            func_801368C0(layout, (char*)strBase + 0x1d2, fontVal);
            func_801368C0(layout, (char*)strBase + 0x1df, fontVal);
            func_801368C0(layout, (char*)strBase + 0x1ed, fontVal);
        } else if (i == 3) {
            u32 val = func_801355A0();
            func_801368C0(layout, (char*)strBase + 0x1fb, val);
        } else if (i == 5) {
            u32 val = func_801355A0();
            func_801368C0(layout, (char*)strBase + 0x208, val);
        }
    }

    // Register render-before callback (null-safe IScnRender subobject at +0x5C)
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&unk00[0x5C]);
    }
    mScn->addRenderCB(cb, 0xF, 0);
}