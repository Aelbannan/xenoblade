#define IWORK_EVENT_INLINE_DTOR
// TU for kyoshin/CItemBoxGridSubMenu
// Submenu overlay widget for the item grid screen.

#include "kyoshin/CItemBoxGridSubMenu.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" char lbl_eu_805084BC[];
extern "C" const float lbl_eu_80668300; // 80.0f
extern "C" const float lbl_eu_80668304; // 0.0f
extern "C" const float lbl_eu_80668308; // 3.0f
extern "C" const float lbl_eu_8066830C; // 100.0f
extern "C" const float lbl_eu_80668310; // -17.0f
extern "C" const float lbl_eu_80668314; // 120.0f
extern "C" const float lbl_eu_80668318; // -37.0f
extern "C" const double lbl_eu_806682F8;
extern "C" u32 getPackedFont__Fv();
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int);
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();
extern "C" void drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void*, void*, int, int);
void playUISound(u32);

// C-linkage pseudo-imports for this TU (playUISound__FUl, lbl_eu_805084BC,
// the lbl_eu_806683xx sdata2 constants, getPackedFont__Fv,
// isClassicController__Q22cf13CfGameManagerFv, CUICfManager_getArcResourceAccessor,
// drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii) now live in the
// "C-linkage imports" section above.

// Forward declarations for functions not in included headers
void TagCopyVec2f(char*, const char*);
void writePanePos(char*, char*);
void Cur_SetPaneTranslate(nw4r::lyt::Pane* pane, const nw4r::math::VEC3* trans);

CItemBoxGridSubMenu::CItemBoxGridSubMenu() {
    mAccessor = NULL;
    mLayout = NULL;
    mAnimDefault = NULL;
    mRootPane = NULL;
    mTxtBoxA = NULL;
    mTxtBoxB = NULL;
    mTxtBoxC = NULL;
    mIsVisible = 0;
    mIsOpened = 1;
    mAnimState = 0;
    mSubState = 0;
    mSelectedIdx = 0;
}

CItemBoxGridSubMenu::~CItemBoxGridSubMenu() {
}

// Initialization function: loads layout, resources, finds panes
void CItemBoxGridSubMenu::GridSubMenu_InitLayout(nw4r::lyt::ArcResourceAccessor* accessor) {
    mAccessor = accessor;

    buildLayout(&mLayout, accessor, &lbl_eu_805084BC[0x00]);
    bindLayoutAnimTransform(mLayout, &mAnimDefault, accessor, &lbl_eu_805084BC[0x19]);

    u8* fontObj = (u8*)CDeviceFont::getFontInfo(1, mLayout);
    nw4r::lyt::Pane* root = mLayout->GetRootPane();
    u32 fontVal = (u32)((IDeviceFontInfo*)fontObj)->getFont();
    ApplyFontToTxtPanes(root, (const nw4r::ut::Font*)fontVal);

    u32 color = getPackedFont__Fv();
    setLayoutTextBoxFont(mLayout, &lbl_eu_805084BC[0x35], color);

    mLayout->BindAnimation(mAnimDefault);
    mLayout->Animate(0);

    mRootPane = mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0x3f], true);
    mTxtBoxA = (nw4r::lyt::TextBox*)mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0x4e], true);
    mTxtBoxB = (nw4r::lyt::TextBox*)mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0x59], true);
    mTxtBoxC = (nw4r::lyt::TextBox*)mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0x64], true);

    nw4r::lyt::Pane* collPane = mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0x6f], true);
    collPane->SetVisible(false);

    char* str = (char*)BdatTouchStringCell(&lbl_eu_805084BC[0x78], &lbl_eu_805084BC[0x86], 0x2b);
    LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_805084BC[0x8b], str, 0);

    int gmVal = isClassicController__Q22cf13CfGameManagerFv(-1);
    const char* fileID = (gmVal == 0) ? &lbl_eu_805084BC[0xa0] : &lbl_eu_805084BC[0x97];

    u16 msgId = BdatGetU16ByTableKey(&lbl_eu_805084BC[0x78], (char*)fileID, 0x2b);
    u8* tex = (u8*)MakeTplNameSysFile(msgId);

    u8* sys = (u8*)CUICfManager_getArcResourceAccessor();
    u8* mat = (u8*)((nw4r::lyt::ArcResourceAccessor*)sys)->GetResource(0x74696d67, (const char*)tex, 0);
    if (mat != NULL) {
        PaneSetTexPaletteByName(mLayout, &lbl_eu_805084BC[0xa9], (TPLPalette*)mat);

        nw4r::lyt::Pane* picPane = mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805084BC[0xa9], true);
        u16 w = *(u16*)(*(u8**)mat + 8 + 2);
        u16 h = *(u16*)(*(u8**)mat + 8 + 0);

        nw4r::lyt::Size sz;
        sz.width = (f32)w;
        sz.height = (f32)h;
        picPane->SetSize(sz);
    }
}

// retail: if (field_20) tail drawLayout(*(self+8), drawInfo passthrough, 0, 1)
void CItemBoxGridSubMenu::GridSubMenu_DrawLayoutGated(void* drawInfo) {
    if (mIsVisible == 0) return;
    drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(mLayout, drawInfo, 0, 1);
}

int CItemBoxGridSubMenu::GridSubMenu_IsVisibleFlag() {
    return mIsVisible;
}

int CItemBoxGridSubMenu::GridSubMenu_IsOpenedFlag() {
    return mIsOpened;
}

void CItemBoxGridSubMenu::GridSubMenu_BeginOpenAnim() {
    if (mAnimState != 0)
        return;
    if (mSubState == 0)
        return;

    mAnimState = 1;
    mIsOpened = 0;
    mSelectedIdx = 0;
    mIsVisible = 1;
    playUISound(13);
}

// retail: if (field_22 == 2) { field_22 = 3; field_21 = 0; tail playUISound(0xE) }
void CItemBoxGridSubMenu::GridSubMenu_AdvanceState2To3() {
    if (mAnimState == 2) {
        mAnimState = 3;
        mIsOpened = 0;
        playUISound(0xE);
    }
}

void CItemBoxGridSubMenu::GridSubMenu_MoveSelectionPrev() {
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

void CItemBoxGridSubMenu::GridSubMenu_MoveSelectionNext() {
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

void CItemBoxGridSubMenu::GridSubMenu_SetupOptions(int arg) {
    mSubState = 0;
    if (mAnimState != 0) {
        return;
    }

    mSubState = 1;

    SetLayoutSelfText((nw4r::lyt::Layout*)((char*)mTxtBoxA), &lbl_eu_805084BC[0xb3], 0);
    SetLayoutSelfText((nw4r::lyt::Layout*)((char*)mTxtBoxB), &lbl_eu_805084BC[0xb3], 0);
    SetLayoutSelfText((nw4r::lyt::Layout*)((char*)mTxtBoxC), &lbl_eu_805084BC[0xb3], 0);

    if (arg == 10) {
        SetLayoutSelfText((nw4r::lyt::Layout*)((char*)mTxtBoxA), (char*)BdatTouchStringCell(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'A'), 0);
        SetLayoutSelfText((nw4r::lyt::Layout*)((char*)mTxtBoxB), (char*)BdatTouchStringCell(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'C'), 0);
        mSubState = 2;
    } else if (arg == 13) {
        SetLayoutSelfText((nw4r::lyt::Layout*)((char*)mTxtBoxA), (char*)BdatTouchStringCell(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], '@'), 0);
        SetLayoutSelfText((nw4r::lyt::Layout*)((char*)mTxtBoxB), (char*)BdatTouchStringCell(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'C'), 0);
        mSubState = 2;
    } else {
        SetLayoutSelfText((nw4r::lyt::Layout*)((char*)mTxtBoxA), (char*)BdatTouchStringCell(&lbl_eu_805084BC[0xb4], &lbl_eu_805084BC[0xbd], 'C'), 0);
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
        Cur_SetPaneTranslate(pane2, &v);
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
        Cur_SetPaneTranslate(pane2, &v);
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
        Cur_SetPaneTranslate(pane2, &v);
        break;
    }
    }
}

// Select one of the three +0x14/+0x18/+0x1C pointers by the sign-extended
// +0x24 index and forward the pair through ScalePaneWorldPos. The VEC3
// destination is r3 and the submenu r4, so this stays a free function.
extern "C" void GridSubMenu_GetCursorPos(void* self, void* src) {
    void* arr[3];
    arr[0] = *(void**)((u8*)src + 0x14);
    arr[1] = *(void**)((u8*)src + 0x18);
    arr[2] = *(void**)((u8*)src + 0x1C);
    void* sel = arr[(s8)*(u8*)((u8*)src + 0x24)];
    ScalePaneWorldPos((nw4r::math::VEC3*)self, (nw4r::lyt::Pane*)sel, (nw4r::lyt::Pane*)(*(void**)((u8*)src + 0x10)),
                  (nw4r::lyt::Pane*)(*(void**)((u8*)*(void**)((u8*)src + 8) + 0x10)));
}

void GridSubMenu_GetOptionCount(){}

void CItemBoxGridSubMenu::GridSubMenu_ClearSelection() {
    mSelectedIdx = -1;
}

void CItemBoxGridSubMenu::GridSubMenu_FinishOpenAnim() {
    if (advanceAnimTransform(mAnimDefault, 1.0f) != 0) {
        mAnimState = 2;
        mIsOpened = 1;
    }
}

// When the +0xC sub-object is visible (AnimRewindFrame with the constant),
// reset the selection bytes (descending store order matches the retail).
void CItemBoxGridSubMenu::GridSubMenu_RewindSelection() {
    extern const f32 lbl_eu_8066831C;
    if (AnimRewindFrame(mAnimDefault, lbl_eu_8066831C)) {
        mAnimState = 0;
        mIsOpened = 1;
        mIsVisible = 0;
    }
}
