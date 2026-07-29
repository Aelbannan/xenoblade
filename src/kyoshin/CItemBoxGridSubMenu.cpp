// TU for kyoshin/CItemBoxGridSubMenu
// Submenu overlay widget for the item grid screen.

#include "kyoshin/CItemBoxGridSubMenu.hpp"

#include "kyoshin/code_80135FDC.hpp"

extern void func_80138078(u32);

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

void func_8020844C(){}

void func_802084D4(){}

void func_80208760(){}

void func_802087B8(){}

void CItemBoxGridSubMenu::func_80208838() {
    mSelectedIdx = -1;
}

void func_80208844(){}

void func_80208890(){}

void CItemBoxGridSubMenu::func_802082D0(nw4r::lyt::DrawInfo* drawInfo) {
    if (!mIsVisible)
        return;
    func_80137038(mLayout, drawInfo, 0, 1);
}

void CItemBoxGridSubMenu::func_802083A4() {
    if (mAnimState != 2)
        return;
    mAnimState = 3;
    mIsOpened = 0;
    func_80138078(14);
}
