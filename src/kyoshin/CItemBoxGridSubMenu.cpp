// TU for kyoshin/CItemBoxGridSubMenu
// Submenu overlay widget for the item grid screen.

#include "kyoshin/CItemBoxGridSubMenu.hpp"
#include "kyoshin/code_80135FDC.hpp"

extern "C" void func_80138078__FUl(u32);
extern void* lbl_eu_80535750[];

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

void CItemBoxGridSubMenu::func_8020844C() {
    if (mAnimState != 2)
        return;

    switch (mSubState) {
    case 1:
        mSelectedIdx = 0;
        break;
    case 2:
        mSelectedIdx++;
        if (mSelectedIdx > 1)
            mSelectedIdx = 0;
        break;
    case 3:
        mSelectedIdx++;
        if (mSelectedIdx > 2)
            mSelectedIdx = 0;
        break;
    }
}

/* Helper externs for layout-text and pane manipulation used by func_802084D4. */
extern "C" char lbl_eu_805084BC[];
extern void func_80136D74(void*, const char*, int);
extern void func_80127BC4(void*, const void*);
extern void func_80124288(void*, void*);
extern void code80135FDC_setVec3(float*, float, float, float);
extern void func_801D2150(nw4r::lyt::Pane*, const nw4r::math::VEC3*);

void CItemBoxGridSubMenu::func_802084D4(int arg) {
    mSubState = 0;
    if (mAnimState != 0) {
        return;
    }

    mSubState = 1;

    // Set default text on all three text boxes
    func_80136D74(mTxtBoxA, &lbl_eu_805084BC[0xb3], 0); // "MNU_item"
    func_80136D74(mTxtBoxB, &lbl_eu_805084BC[0xb3], 0);
    func_80136D74(mTxtBoxC, &lbl_eu_805084BC[0xb3], 0);

    if (arg == 10) {
        func_80136D74(mTxtBoxA, func_80136190(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'A'), 0);
        func_80136D74(mTxtBoxB, func_80136190(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'C'), 0);
        mSubState = 2;
    } else if (arg == 13) {
        func_80136D74(mTxtBoxA, func_80136190(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], '@'), 0);
        func_80136D74(mTxtBoxB, func_80136190(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'C'), 0);
        mSubState = 2;
    } else {
        func_80136D74(mTxtBoxA, func_80136190(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'C'), 0);
    }

    // Find panes for positioning
    nw4r::lyt::Pane* pane1 = mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0xc2], true);  // "win_slct"
    nw4r::lyt::Pane* pane2 = mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0xcb], true);  // "nul_close01"

    switch (mSubState) {
    case 1: {
        float sz[2];
        func_80127BC4(sz, (u8*)pane1 + 0x4c);
        sz[1] = 80.0f;
        func_80124288(pane1, sz);

        float trans[3];
        code80135FDC_setVec3(trans, 0.0f, 3.0f, 0.0f);
        func_801D2150(pane2, (nw4r::math::VEC3*)trans);
        break;
    }
    case 2: {
        float sz[2];
        func_80127BC4(sz, (u8*)pane1 + 0x4c);
        sz[1] = 100.0f;
        func_80124288(pane1, sz);

        float trans[3];
        code80135FDC_setVec3(trans, 0.0f, -17.0f, 0.0f);
        func_801D2150(pane2, (nw4r::math::VEC3*)trans);
        break;
    }
    case 3: {
        float sz[2];
        func_80127BC4(sz, (u8*)pane1 + 0x4c);
        sz[1] = 120.0f;
        func_80124288(pane1, sz);

        float trans[3];
        code80135FDC_setVec3(trans, 0.0f, -37.0f, 0.0f);
        func_801D2150(pane2, (nw4r::math::VEC3*)trans);
        break;
    }
    }
}

void func_80208760(){}

void func_802087B8(){}

void func_80208838(void){}

void func_80208844(){}

void func_80208890(){}

extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void*, void*, int, int);
void func_802082D0(void* self){
    if (*(u8*)((u8*)self + 0x20) == 0) return;
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(*(void**)((u8*)self + 8), 0, 0, 1);
}
