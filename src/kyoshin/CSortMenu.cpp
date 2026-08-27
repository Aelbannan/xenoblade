// CSortMenu - sort menu UI widget
// High-level C/C++ reconstruction from retail PPC

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSortMenu.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CEventFile.hpp"

// External symbols
extern char lbl_eu_8050624C[];  // String table for resource names (non-C-linkage, kept here)
extern const float lbl_eu_80668000;  // 1.0f literal pool (SDA21): animation completion threshold

// Intra-TU forward declarations (definitions in this file; pre-existing missing
// declarations - required for the TU to compile under -lang=c++)
// noinline: retail calls the state handlers via bl from func_801D3160;
// without it MWCC -inline auto inlines them there and bloats the TU.
extern "C" __declspec(noinline) void func_801D3878(CSortMenu*);
extern "C" __declspec(noinline) void func_801D390C(CSortMenu*);
extern "C" __declspec(noinline) void func_801D3958(CSortMenu*);
extern "C" __declspec(noinline) void func_801D39EC(CSortMenu*);
// noinline: retail calls func_801D3A3C via bl from every caller; without it MWCC
// -inline auto inlines the loop body into each caller and bloats the TU.
extern "C" __declspec(noinline) void func_801D3A3C(CSortMenu*);

// Retail-named SDA2 constants.
extern const float lbl_eu_80667FF0;  // row height (SDA21)

// ============================================================================
// CSortMenu constructor
// ============================================================================
extern "C" CSortMenu* __ct__CSortMenu(CSortMenu* _this) {
    // Vtable + embedded sub-object first (retail: stw vtable, then the
    // UnkClass_8045F564 ctor at +4), then the scalar fields, the scrollbar,
    // and the tail counters.
    *(void**)_this = (void*)lbl_eu_805349D0;
    __ct__17UnkClass_8045F564Fv(&_this->_04[0]);
    _this->mFileHandle = NULL;
    _this->mArcResAcc = NULL;
    _this->mpLayout = NULL;
    _this->mpAnimTrans0 = NULL;
    _this->mpAnimTrans1 = NULL;
    _this->field_0x28 = 0;
    _this->field_0x29 = 0;
    _this->field_0x2A = 0;
    _this->field_0x2B = 1;
    __ct__CScrollBar((u8*)_this + 0x2C, 0);
    _this->mCount = 0;
    _this->mPage = 0;
    _this->mSubPage = 0;
    return _this;
}

// ============================================================================
// CSortMenu destructor
// ============================================================================
extern "C" CSortMenu* __dt__9CSortMenuFv(CSortMenu* _this, int flags) {
    if (_this != NULL) {
        __dt__10CScrollBarFv((u8*)_this + 0x2C, -1);
        __dt__17UnkClass_8045F564Fv((u8*)_this + 0x04, -1);
        if (flags > 0) {
            __dl__FPv(_this);
        }
    }
    return _this;
}

// ============================================================================
// func_801D3064: Init sort menu - read file, create scrollbar data
// ============================================================================
extern "C" void func_801D3064(CSortMenu* _this) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    _this->mFileHandle = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)handle, lbl_eu_8050624C + 0x00, _this, 0, 0);
    setHandleFlag1__11CDeviceFileFP11CFileHandle(_this->mFileHandle);

    // Build a temporary CScrollBar that carries the initial data for the member.
    CScrollBarData temp;
    __ct__CScrollBar(&temp, 2);

    // Copy the constructed body into the member (retail omits the vtable at +0x0).
    _this->mScrollBar.mMemRegion[0] = temp.mMemRegion[0];
    _this->mScrollBar.mMemRegion[1] = temp.mMemRegion[1];
    _this->mScrollBar.mMemRegion[2] = temp.mMemRegion[2];
    _this->mScrollBar.mMemRegion[3] = temp.mMemRegion[3];
    _this->mScrollBar.mFileHandle = temp.mFileHandle;
    _this->mScrollBar.mAccessor = temp.mAccessor;
    _this->mScrollBar.mLayout = temp.mLayout;
    _this->mScrollBar.mAnimTrans = temp.mAnimTrans;
    _this->mScrollBar.mReady = temp.mReady;
    _this->mScrollBar.mVisible = temp.mVisible;
    _this->mScrollBar.mState = temp.mState;
    _this->mScrollBar.mActive = temp.mActive;
    _this->mScrollBar.mAnimOffset = temp.mAnimOffset;
    _this->mScrollBar.mScrollPosY = temp.mScrollPosY;
    _this->mScrollBar.mScrollRatio = temp.mScrollRatio;
    _this->mScrollBar.mThumbHeight = temp.mThumbHeight;
    _this->mScrollBar.mContentHeight = temp.mContentHeight;
    _this->mScrollBar.mDirection = temp.mDirection;

    __dt__10CScrollBarFv(&temp, -1);
    func_801F34F4(&_this->mScrollBar);
}

// ============================================================================
// func_801D3160: Move - state machine dispatch
// ============================================================================
extern "C" void func_801D3160(CSortMenu* _this) {
    if (_this->field_0x28 == 0) return;

    switch (_this->field_0x2A) {
    case 1:
        func_801D3878(_this);
        break;
    case 2:
        func_801D390C(_this);
        break;
    case 4:
        func_801D3958(_this);
        break;
    case 5:
        func_801D39EC(_this);
        break;
    }

    _this->mpLayout->Animate(0);
    func_801F3540((u8*)_this + 0x2C);
}

// ============================================================================
// func_801D31F8: Draw
// ============================================================================
extern "C" void func_801D31F8(CSortMenu* _this, nw4r::lyt::DrawInfo* drawInfo) {
    if (_this->field_0x28 == 0) return;
    func_80137038(_this->mpLayout, drawInfo, 0, 1);
    if (_this->mCount > 5) {
        func_801F35B0((u8*)_this + 0x2C, drawInfo);
    }
}

// ============================================================================
// func_801D3258: Term - cleanup
// ============================================================================
extern "C" void func_801D3258(CSortMenu* _this) {
    func_801390E0(&_this->mFileHandle);
    _this->field_0x28 = 0;
    if (_this->mpLayout != NULL) {
        // Retail calls the virtual deleting destructor (vtable slot +0x8, r4=1).
        delete _this->mpLayout;
        _this->mpLayout = NULL;
    }
    func_80139124(_this->mArcResAcc);
    _this->mArcResAcc = NULL;
    func_8045F778__17UnkClass_8045F564Fv((u8*)_this + 0x04);
    func_801F35DC((u8*)_this + 0x2C);
}

// ============================================================================
// func_801D32DC: Check if scrollbar is visible
// ============================================================================
extern "C" u8 func_801D32DC(CSortMenu* _this) {
    if (CScrollBar_isVisible((u8*)_this + 0x2C) != 0) {
        return _this->field_0x29;
    }
    return 0;
}

// ============================================================================
// func_801D3320: Get field_0x28
// ============================================================================
extern "C" u8 func_801D3320(CSortMenu* _this) {
    return _this->field_0x28;
}

// ============================================================================
// func_801D3328: Get field_0x2B
// ============================================================================
extern "C" u8 func_801D3328(CSortMenu* _this) {
    return _this->field_0x2B;
}

// ============================================================================
// func_801D3330: Open / initialize sort menu
// Each GetRootPane() is spelled out fresh - retail reloads mpLayout from the
// object before every use instead of caching the root pane.
// ============================================================================
extern "C" void func_801D3330(CSortMenu* _this) {
    if (_this->field_0x2A != 0) return;

    _this->field_0x2A = 1;
    _this->field_0x2B = 0;
    _this->field_0x28 = 1;

    char* s = lbl_eu_8050624C;
    nw4r::lyt::Pane* paneTxt1 = _this->mpLayout->GetRootPane()->FindPaneByName(s + 0x15, true);
    nw4r::lyt::Pane* paneTxt2 = _this->mpLayout->GetRootPane()->FindPaneByName(s + 0x1d, true);
    f32 textBuf[3];
    func_80137924(textBuf, paneTxt1, paneTxt2, _this->mpLayout->GetRootPane());

    func_801F3670(&_this->mScrollBar, &textBuf);
    func_801F36BC(&_this->mScrollBar, 5, _this->mCount);
    func_801F367C(&_this->mScrollBar);

    func_801D3A3C(_this);
}

// ============================================================================
// func_801D3408: Set state to 4 (scroll down page)
// ============================================================================
extern "C" void func_801D3408(CSortMenu* _this) {
    if (_this->field_0x2A != 3) return;
    _this->field_0x2A = 4;
    _this->field_0x2B = 0;
    func_801F369C((u8*)_this + 0x2C);
}

// ============================================================================
// func_801D3430: Set layout position
// ============================================================================
extern "C" void func_801D3430(CSortMenu* _this, const nw4r::math::VEC3* pos) {
    nw4r::lyt::Pane* rootPane = _this->mpLayout->GetRootPane();
    rootPane->SetTranslate(*pos);
}

// ============================================================================
// func_801D3454: Update pane text with formatted string
// Declare-then-assign keeps the lis/addi pairs in retail order (strTable ->
// r4, string base -> r31); the first rootPane stays live as the 4th arg while
// the second FindPaneByName reloads it fresh.
// ============================================================================
extern "C" void func_801D3454(u8* textObj, CSortMenu* _this) {
    char buf[0x1c];
    void** strTable;
    char* s;
    strTable = lbl_eu_805349B8;
    s = lbl_eu_8050624C;

    sprintf(buf, s + 0x2c, strTable[(s8)_this->mPage]);

    nw4r::lyt::Pane* rootPane = _this->mpLayout->GetRootPane();
    nw4r::lyt::Pane* pane1 = rootPane->FindPaneByName(s + 0x1d, true);
    nw4r::lyt::Pane* pane2 = _this->mpLayout->GetRootPane()->FindPaneByName(buf, true);

    func_80137924(textObj, pane2, pane1, rootPane);
}

// ============================================================================
// func_801D350C: Reset count to 0
// ============================================================================
extern "C" void func_801D350C(CSortMenu* _this) {
    _this->mCount = 0;
}

// ============================================================================
// func_801D3518: Add a value to the array
// ============================================================================
extern "C" void func_801D3518(CSortMenu* _this, int value) {
    u8 count = _this->mCount;
    if (count >= 32) return;
    _this->mArray[count] = value;
    _this->mCount = count + 1;
}

// ============================================================================
// func_801D353C: Set page
// ============================================================================
extern "C" void func_801D353C(CSortMenu* _this, s8 page) {
    nw4r::lyt::Pane* pane =
        _this->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_8050624C + 0x2f, true);

    // Local copy of the pane size region; member-wise so MWCC emits an
    // lfs/stfs pair (a whole-struct copy collapses to lwz).
    PaneSizeRegion size;
    CSortMenuPaneSizeView* paneView = (CSortMenuPaneSizeView*)pane;

    size.width = paneView->width;
    size.height = paneView->height;

    // Visible rows: 5 unless fewer items remain (signed: retail converts
    // via the xoris sign-flip path).
    s32 rows = (_this->mCount < 5) ? (s32)_this->mCount : 5;

    // (f32)rows: MWCC seeds 0x4330xxxx on the stack and subtracts the
    // int->double conversion magic via lfd (retail loads it from the shared
    // DOL pool slot lbl_eu_80667FF8; UNIT_RULES retargets the TU-local
    // literal there).
    size.height = lbl_eu_80667FF0 * (f32)rows;
    paneView->width = size.width;
    paneView->height = size.height;

    _this->mPage = page;
    _this->mSubPage = 0;

    if (page >= 5) {
        _this->mSubPage = page - 4;
        _this->mPage = 4;
    }

    func_801D3A3C(_this);
}

// ============================================================================
// func_801D3620: Scroll up
// ============================================================================
extern "C" void func_801D3620(CSortMenu* _this) {
    _this->mPage = (u8)(_this->mPage - 1);
    if ((s8)_this->mPage < 0) {
        _this->mPage = 0;
        _this->mSubPage = (u8)(_this->mSubPage - 1);

        if ((s8)_this->mSubPage < 0) {
            u8 count = _this->mCount;
            if (count >= 5) {
                _this->mPage = 4;
                _this->mSubPage = count - 5;
            } else {
                _this->mPage = (u8)(count - 1);
                _this->mSubPage = 0;
                if ((s8)_this->mPage < 0) {
                    _this->mPage = 0;
                }
            }
        }
    }
    func_801D3A3C(_this);
}

// ============================================================================
// func_801D3698: Scroll down
// ============================================================================
extern "C" void func_801D3698(CSortMenu* _this) {
    u8 count = _this->mCount;
    if (count >= 5) {
        u8 pageU = (u8)(_this->mPage + 1);
        _this->mPage = pageU;
        s8 page = (s8)pageU;
        if (page >= 5) {
            _this->mPage = 4;
            s32 sp = (s32)_this->mSubPage + 1;
            _this->mSubPage = (u8)sp;
            u8 spu = (u8)sp;
            s8 sps = (s8)spu;
            if (sps > (s32)(count - 5)) {
                _this->mPage = 0;
                _this->mSubPage = 0;
            }
        }
    } else {
        u8 pageU = (u8)(_this->mPage + 1);
        _this->mPage = pageU;
        s8 page = (s8)pageU;
        if (page >= count) {
            _this->mPage = 0;
            _this->mSubPage = 0;
        }
    }
    func_801D3A3C(_this);
}

// ============================================================================
// func_801D3724: Page up
// ============================================================================
extern "C" void func_801D3724(CSortMenu* _this) {
    u8 count = _this->mCount;
    if (count >= 5) {
        u8 subU8 = (u8)(_this->mSubPage - 5);
        _this->mSubPage = subU8;
        if ((s8)subU8 < 0) {
            _this->mPage = (u8)(subU8 + 4);
            _this->mSubPage = 0;
            if ((s8)(u8)(subU8 + 4) < 0) {
                _this->mPage = 0;
            }
        }
    } else {
        _this->mPage = 0;
        _this->mSubPage = 0;
    }
    func_801D3A3C(_this);
}

// ============================================================================
// func_801D377C: Page down
// ============================================================================
extern "C" void func_801D377C(CSortMenu* _this) {
    u8 count = _this->mCount;
    if (count >= 5) {
        s32 maxSub = (s32)(count - 5);
        s32 sp5 = (s32)_this->mSubPage + 5;
        _this->mSubPage = (u8)sp5;
        u8 sp5u = (u8)sp5;
        if ((s8)sp5u > maxSub) {
            // New page comes from the truncated byte, not the sign-extended sum.
            s32 newPage = (s32)sp5u - maxSub;
            _this->mPage = (u8)newPage;
            _this->mSubPage = (u8)maxSub;
            if ((s8)_this->mPage >= 5) {
                _this->mPage = 4;
            }
        }
    } else {
        _this->mPage = (u8)(count - 1);
        _this->mSubPage = 0;
        if ((s8)_this->mPage < 0) {
            _this->mPage = 0;
        }
    }
    func_801D3A3C(_this);
}

// ============================================================================
// func_801D37F4: Get page + subPage
// ============================================================================
extern "C" u8 func_801D37F4(CSortMenu* _this) {
    return _this->mPage + _this->mSubPage;
}

// ============================================================================
// func_801D3808: Get page
// ============================================================================
extern "C" u8 func_801D3808(CSortMenu* _this) {
    return _this->mPage;
}

// ============================================================================
// func_801D3810: Get subPage
// ============================================================================
extern "C" u8 func_801D3810(CSortMenu* _this) {
    return _this->mSubPage;
}

// ============================================================================
// func_801D3818: Convert sort value to page+subpage
// ============================================================================
#pragma optimize_for_size on
extern "C" void func_801D3818(CSortMenu* _this, int value, u8* outPage, u8* outSubPage) {
    *outPage = func_8015780C(value);
    *outSubPage = 0;
    if ((s8)*outPage >= 5) {
        *outSubPage = *outPage - 4;
        *outPage = 4;
    }
}
#pragma optimize_for_size off

// ============================================================================
// func_801D3878: State 1 handler - opening animation
// ============================================================================
extern "C" __declspec(noinline) void func_801D3878(CSortMenu* _this) {
    if (func_80137444(_this->mpAnimTrans0, lbl_eu_80668000) != 0) {
        _this->field_0x2A = 2;
        _this->mpLayout->SetAnimationEnable(_this->mpAnimTrans0, false);
        _this->mpLayout->SetAnimationEnable(_this->mpAnimTrans1, true);
        _this->mpLayout->Animate(0);
    }
}

// ============================================================================
// func_801D390C: State 2 handler - wait for anim1
// ============================================================================
extern "C" __declspec(noinline) void func_801D390C(CSortMenu* _this) {
    if (func_80137444(_this->mpAnimTrans1, 1.0f) != 0) {
        _this->field_0x2A = 3;
        _this->field_0x2B = 1;
    }
}

// ============================================================================
// func_801D3958: State 4 handler - scroll animation
// ============================================================================
extern "C" __declspec(noinline) void func_801D3958(CSortMenu* _this) {
    if (func_80137510(_this->mpAnimTrans1, lbl_eu_80668000) != 0) {
        _this->field_0x2A = 5;
        _this->mpLayout->SetAnimationEnable(_this->mpAnimTrans1, false);
        _this->mpLayout->SetAnimationEnable(_this->mpAnimTrans0, true);
        _this->mpLayout->Animate(0);
    }
}

// ============================================================================
// func_801D39EC: State 5 handler - closing animation
// ============================================================================
extern "C" __declspec(noinline) void func_801D39EC(CSortMenu* _this) {
    if (func_80137510(_this->mpAnimTrans0, 1.0f) != 0) {
        _this->field_0x2A = 0;
        _this->field_0x2B = 1;
        _this->field_0x28 = 0;
    }
}

// ============================================================================
// func_801D3A3C: Update pane text for all 5 slots
// optimize_for_size: retail uses an stmw r28/lmw r28 block-save frame.
// ============================================================================
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_801D3A3C(CSortMenu* _this) {
    // Declare-then-assign: resBase is born first (takes r31), but strTable's
    // lis/addi must be EMITTED first (retail schedule) into r30.
    char* resBase;
    void** strTable = lbl_eu_805349B8;
    resBase = lbl_eu_8050624C;
    // Slots past the entry count fall back to the shared "no item" string.
    // u8 counter: retail masks i to 8 bits at every use (clrlwi/clrlslwi).
    for (u8 i = 0; i < 5; i++) {
        int idx = i + (s8)_this->mSubPage;
        if (idx >= (int)_this->mCount) {
            func_80136B4C(_this->mpLayout, (char*)strTable[i], resBase + 0x3e, 0);
        } else {
            func_80136B4C(_this->mpLayout, (char*)strTable[i], (char*)_this->mArray[idx], 0);
        }
    }
    // Single vreg for the converted value keeps the lbz/extsb/clrlwi chain
    // in one register (retail does not split it across r0/r4).
    u32 subPage = (u16)(s8)_this->mSubPage;
    CScrollBarData* scrollBar = &_this->mScrollBar;
    func_801F3850(scrollBar, subPage);
}
#pragma optimize_for_size off

// ============================================================================
// CSortMenu::OnFileEvent - file load completion callback
// optimize_for_size: retail uses an stmw r29 block-save frame.
// ============================================================================
#pragma optimize_for_size on
int CSortMenu::OnFileEvent(CEventFile* event) {
    if (mFileHandle == event->mFileHandle) {
        char* s = lbl_eu_8050624C;  // resource-name string table base (r31)

        createRegion__17UnkClass_8045F564FiiPCci(&_04[0],
                                  (int)getHandleMEM2__Q23mtl10MemManagerFv(),
                                  0x2000, s + 0x3f, 1);

        u8 sp8[8];  // Class_8045F858 is 8 bytes (UnkClass* + u32)
        __ct__14Class_8045F858FP17UnkClass_8045F564(sp8, &_04[0]);

        // Save the file data buffer and clear the handle's reference to it.
        void* fileData = mFileHandle->getData();

        func_80434A4C__Q23mtl10MemManagerFb(false);
        mArcResAcc = (nw4r::lyt::ArcResourceAccessor*)createArcResourceAccessor__10CLibLayoutFv();
        mArcResAcc->Attach(fileData, s + 0x49);

        func_80136E84(&mpLayout, mArcResAcc, s + 0x4d);
        func_80136F08(mpLayout, &mpAnimTrans0, mArcResAcc, s + 0x63);
        func_80136F08(mpLayout, &mpAnimTrans1, mArcResAcc, s + 0x7c);

        // Root pane sits at Layout + 0x10 (direct field access, not a virtual
        // call). Retail order: load mpLayout, materialize the font-id constant,
        // then read the root pane before calling the font builder.
        nw4r::lyt::Layout* lyt = mpLayout;
        int fontId = 1;
        nw4r::lyt::Pane* rootPane = *(nw4r::lyt::Pane**)((u8*)lyt + 0x10);
        void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(fontId, lyt);
        // Virtual call through the font object's vtable slot +0x24.
        u32 result = ((CDeviceFontItf*)fontObj)->getResource();
        func_8013676C(rootPane, result);

        mpLayout->SetAnimationEnable(mpAnimTrans1, false);
        mpLayout->SetAnimationEnable(mpAnimTrans0, true);
        mpLayout->Animate(0);

        if (mpLayout != NULL) {
            field_0x29 = 1;
        }

        mFileHandle = NULL;
        func_8045F810__17UnkClass_8045F564Fv(&_04[0]);
        __dt__14Class_8045F858Fv(sp8, -1);

        return 1;
    }
    return 0;
}
#pragma optimize_for_size off