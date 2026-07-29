// TU for kyoshin/CItemBoxGridSubMenu
// Submenu overlay widget for the item grid screen.

#include "kyoshin/CItemBoxGridSubMenu.hpp"
#include "kyoshin/code_80135FDC.hpp"

extern "C" void func_80138078__FUl(u32);
extern void* lbl_eu_80535750[];
extern void func_80137924(void*, void*, void*, void*);

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

void func_8020844C(){}

void func_802084D4(){}

void func_80208760(CItemBoxGridSubMenu* self, CItemBoxGridSubMenu* other) {
    nw4r::lyt::TextBox* boxes[3];
    boxes[0] = other->mTxtBoxA;
    boxes[1] = other->mTxtBoxB;
    boxes[2] = other->mTxtBoxC;
    func_80137924(self, boxes[other->mSelectedIdx], other->mRootPane,
                  other->mLayout->GetRootPane());
}

void func_802087B8(){}

void func_80208838(void){}

void CItemBoxGridSubMenu::func_80208844() {
    if (func_80137444(mAnimDefault, 1.0f) != 0) {
        mAnimState = 2;
        mIsOpened = 1;
    }
}

void CItemBoxGridSubMenu::func_80208890() {
    if (func_80137510(mAnimDefault, 1.0f) != 0) {
        mAnimState = 0;
        mIsOpened = 1;
        mIsVisible = 0;
    }
}

extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void*, void*, int, int);
void func_802082D0(void* self){
    if (*(u8*)((u8*)self + 0x20) == 0) return;
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(*(void**)((u8*)self + 8), 0, 0, 1);
}
