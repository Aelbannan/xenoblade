// TU for kyoshin/CItemBoxGridSubMenu
// Submenu overlay widget for the item grid screen.

#include "kyoshin/CItemBoxGridSubMenu.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFont.hpp"

// C-linkage pseudo-imports for this TU (func_80138078__FUl, lbl_eu_805084BC,
// the lbl_eu_806683xx sdata2 constants, func_801355A0__Fv,
// func_80086F9C__Q22cf13CfGameManagerFv, func_801355F4,
// func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii) now live in the
// "C-linkage imports" section of kyoshin/CItemBoxGridSubMenu.hpp.

extern void* lbl_eu_80535750[];

// Forward declarations for functions not in included headers
void func_80136D74(char*, const char*, int);
void func_80127BC4(char*, const char*);
void func_80124288(char*, char*);
void func_801D2150(nw4r::lyt::Pane* pane, const nw4r::math::VEC3* trans);
void code80135FDC_setVec3(float*, float, float, float);

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

void CItemBoxGridSubMenu::func_802084D4(int arg) {
    mSubState = 0;
    if (mAnimState != 0) {
        return;
    }

    mSubState = 1;

    func_80136D74((char*)mTxtBoxA, &lbl_eu_805084BC[0xb3], 0);
    func_80136D74((char*)mTxtBoxB, &lbl_eu_805084BC[0xb3], 0);
    func_80136D74((char*)mTxtBoxC, &lbl_eu_805084BC[0xb3], 0);

    if (arg == 10) {
        func_80136D74((char*)mTxtBoxA, (char*)func_80136190(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'A'), 0);
        func_80136D74((char*)mTxtBoxB, (char*)func_80136190(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'C'), 0);
        mSubState = 2;
    } else if (arg == 13) {
        func_80136D74((char*)mTxtBoxA, (char*)func_80136190(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], '@'), 0);
        func_80136D74((char*)mTxtBoxB, (char*)func_80136190(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'C'), 0);
        mSubState = 2;
    } else {
        func_80136D74((char*)mTxtBoxA, (char*)func_80136190(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'C'), 0);
    }

    nw4r::lyt::Pane* pane1 = mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0xc2], true);
    nw4r::lyt::Pane* pane2 = mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0xcb], true);

    switch (mSubState) {
    case 1: {
        nw4r::lyt::Size sz = pane1->GetSize();
        sz.height = lbl_eu_80668300;
        pane1->SetSize(sz);

        nw4r::math::VEC3 v;
        v.x = lbl_eu_80668304;
        v.y = lbl_eu_80668308;
        v.z = lbl_eu_80668304;
        func_801D2150(pane2, &v);
        break;
    }
    case 2: {
        nw4r::lyt::Size sz = pane1->GetSize();
        sz.height = lbl_eu_8066830C;
        pane1->SetSize(sz);

        nw4r::math::VEC3 v;
        v.x = lbl_eu_80668304;
        v.y = lbl_eu_80668310;
        v.z = lbl_eu_80668304;
        func_801D2150(pane2, &v);
        break;
    }
    case 3: {
        nw4r::lyt::Size sz = pane1->GetSize();
        sz.height = lbl_eu_80668314;
        pane1->SetSize(sz);

        nw4r::math::VEC3 v;
        v.x = lbl_eu_80668304;
        v.y = lbl_eu_80668318;
        v.z = lbl_eu_80668304;
        func_801D2150(pane2, &v);
        break;
    }
    }
}

// Initialization function: loads layout, resources, finds panes
void func_80207FC8(CItemBoxGridSubMenu* self, nw4r::lyt::ArcResourceAccessor* accessor) {
    self->mAccessor = accessor;

    func_80136E84(&self->mLayout, accessor, &lbl_eu_805084BC[0x00]);
    func_80136F08(self->mLayout, &self->mAnimDefault, accessor, &lbl_eu_805084BC[0x19]);

    u8* fontObj = (u8*)CDeviceFont::func_80452C10(1, self->mLayout);
    nw4r::lyt::Pane* root = self->mLayout->GetRootPane();
    u32 fontVal = (*(u32(*)(u8*))(*(u32**)fontObj + 9))(fontObj);
    func_8013676C(root, fontVal);

    u32 color = func_801355A0__Fv();
    func_801368C0(self->mLayout, &lbl_eu_805084BC[0x35], color);

    self->mLayout->BindAnimation(self->mAnimDefault);
    self->mLayout->Animate(0);

    self->mRootPane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0x3f], true);
    self->mTxtBoxA = (nw4r::lyt::TextBox*)self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0x4e], true);
    self->mTxtBoxB = (nw4r::lyt::TextBox*)self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0x59], true);
    self->mTxtBoxC = (nw4r::lyt::TextBox*)self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0x64], true);

    nw4r::lyt::Pane* collPane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0x6f], true);
    collPane->SetVisible(false);

    char* str = (char*)func_80136190(&lbl_eu_805084BC[0x78], &lbl_eu_805084BC[0x86], 0x2b);
    func_80136B4C(self->mLayout, &lbl_eu_805084BC[0x8b], str, 0);

    int gmVal = func_80086F9C__Q22cf13CfGameManagerFv(-1);
    const char* fileID = (gmVal == 0) ? &lbl_eu_805084BC[0xa0] : &lbl_eu_805084BC[0x97];

    u16 msgId = func_8013606C(&lbl_eu_805084BC[0x78], (char*)fileID, 0x2b);
    u8* tex = (u8*)func_80138F78(msgId);

    u8* sys = (u8*)func_801355F4();
    u8* mat = (*(u8*(*)(u8*, u32, u8*, u32))(*(u32**)sys + 3))(sys, 0x74696d67, tex, 0);
    if (mat != NULL) {
        func_80137E7C(self->mLayout, &lbl_eu_805084BC[0xa9], mat);

        nw4r::lyt::Pane* picPane = self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0xa9], true);
        u16 w = *(u16*)(*(u8**)mat + 8 + 2);
        u16 h = *(u16*)(*(u8**)mat + 8 + 0);

        nw4r::lyt::Size sz;
        sz.width = (f32)(s32)w;
        sz.height = (f32)(s32)h;
        picPane->SetSize(sz);
    }
}

void func_80208760(){}

void func_802087B8(){}

void func_80208838(CItemBoxGridSubMenu* self) {
    self->mSelectedIdx = -1;
}

void func_80208844(){}

void func_80208890(){}

// retail: if (field_20) tail func_80137038(*(self+8), drawInfo passthrough, 0, 1)
extern "C" void func_802082D0(u8* self, void* drawInfo){
    if (self[0x20] == 0) return;
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(*(void**)(self + 8), drawInfo, 0, 1);
}
