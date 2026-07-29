// TU for kyoshin/CItemBoxGridSubMenu
// Submenu overlay widget for the item grid screen.

#include "kyoshin/CItemBoxGridSubMenu.hpp"

extern "C" void func_80138078__FUl(u32);
extern void* lbl_eu_80535750[];

// Forward declarations for local helpers
void func_80208844();
void func_80208890();

void* __ct__CItemBoxGridSubMenu(void* self) {
    CItemBoxGridSubMenu* s = (CItemBoxGridSubMenu*)self;
    s->mVtbl = lbl_eu_80535750;
    s->mAccessor = NULL;
    s->mLayout = NULL;
    s->mAnimDefault = NULL;
    s->mRootPane = NULL;
    s->mTxtBoxA = NULL;
    s->mTxtBoxB = NULL;
    s->mTxtBoxC = NULL;
    s->mIsVisible = 0;
    s->mIsOpened = 1;
    s->mAnimState = 0;
    s->mSubState = 0;
    s->mSelectedIdx = 0;
    return self;
}

CItemBoxGridSubMenu::~CItemBoxGridSubMenu() {
}

u8 CItemBoxGridSubMenu::func_80208358() {
    return mIsVisible;
}

u8 CItemBoxGridSubMenu::func_80208360() {
    return mIsOpened;
}

void CItemBoxGridSubMenu::func_802083CC() {
    if (mAnimState != 2)
        return;

    switch (mSubState) {
    case 1:
        mSelectedIdx = 0;
        break;
    case 2:
        mSelectedIdx = (unsigned char)(mSelectedIdx - 1);
        if ((signed char)mSelectedIdx < 0)
            mSelectedIdx = 1;
        break;
    case 3:
        mSelectedIdx = (unsigned char)(mSelectedIdx - 1);
        if ((signed char)mSelectedIdx < 0)
            mSelectedIdx = 2;
        break;
    }
}

void CItemBoxGridSubMenu::func_80208368() {
    if (mAnimState != 0)
        return;
    if (mSubState == 0)
        return;

    mAnimState = 1;
    mIsOpened = 0;
    mSelectedIdx = 0;
    mIsVisible = 1;
    func_80138078__FUl(13);
}

// Target: us-8020a0d0 — func_802082F0
// Cleanup: hides the submenu, destroys the layout, and nulls pointers.
void CItemBoxGridSubMenu::func_802082F0() {
    mIsVisible = 0;
    if (mLayout) {
        delete mLayout;
        mLayout = NULL;
    }
    mAccessor = NULL;
}

// Target: us-8020a040 — func_80208260
// Update: animates the layout based on anim state (opening/closing).
void CItemBoxGridSubMenu::func_80208260() {
    if (!mIsVisible) {
        return;
    }
    if ((int)mAnimState == 1) {
        func_80208844();
    } else if ((int)mAnimState == 3) {
        func_80208890();
    }
    mLayout->Animate(0);
}

// Target: us-8020a598 — func_802087B8
// Returns action based on sub-state and selected index.
int CItemBoxGridSubMenu::func_802087B8() {
    switch (mSubState) {
    case 1:
        return 1;
    case 2:
        if (mSelectedIdx == 0) {
            return 2;
        }
        if (mSelectedIdx == 1) {
            return 1;
        }
        return 0;
    case 3:
        if (mSelectedIdx == 0) {
            return 2;
        }
        if (mSelectedIdx == 2) {
            return 1;
        }
        return 0;
    }
    return 0;
}

void func_8020844C(){}

void func_802084D4(){}

void func_80208760(){}

extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void*, void*, int, int);
void func_802082D0(void* self){
    if (*(u8*)((u8*)self + 0x20) == 0) return;
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(*(void**)((u8*)self + 8), 0, 0, 1);
}

DECOMP_DONT_INLINE void func_80208838(void) {
    volatile int v = 0;
    func_80138078__FUl(v);
}

DECOMP_DONT_INLINE void func_80208844() {
    volatile int v = 1;
    func_80138078__FUl(v);
}

DECOMP_DONT_INLINE void func_80208890() {
    volatile int v = 2;
    func_80138078__FUl(v);
}
