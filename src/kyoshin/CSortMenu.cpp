// CSortMenu — sort menu UI widget
// High-level C/C++ reconstruction from retail PPC

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSortMenu.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CFileHandle.hpp"

// External symbols
extern "C" void* lbl_eu_805349D0[];  // CSortMenu vtable
extern "C" void* lbl_eu_805349B8[];  // String table for sort menu pane names
extern char lbl_eu_8050624C[];       // String table for resource names

extern "C" void __dl__FPv(void*);
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" void func_8045F810__17UnkClass_8045F564Fv(void*);
extern "C" void __ct__14Class_8045F858Fv(void*, void*);
extern "C" void __dt__14Class_8045F858Fv(void*, int);
extern "C" void createRegion__17UnkClass_8045F564FiiPCci(void*, int, int, const char*, int);

extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void func_80434A4C__Q23mtl10MemManagerFb(bool);
extern "C" void* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" void setHandleFlag1__11CDeviceFileFP11CFileHandle(void*);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(unsigned long, nw4r::lyt::Layout*);
extern "C" void func_8015780C(int);

// CScrollBar functions
extern "C" void __ct__10CScrollBarFv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void func_801F34F4(void*);
extern "C" void func_801F3540(void*);
extern "C" void func_801F35B0(void*, void*);
extern "C" void func_801F35DC(void*);
extern "C" u8 CScrollBar_isVisible(void*);
extern "C" void func_801F3670(void*, void*);
extern "C" void func_801F36BC(void*, int, int);
extern "C" void func_801F367C(void*);
extern "C" void func_801F369C(void*);
extern "C" void func_801F3850(void*, int);
extern "C" void func_801F34F4(void*);
extern "C" void func_80137924(void*, void*, void*, void*);

// ============================================================================
// CSortMenu constructor
// ============================================================================
extern "C" CSortMenu* __ct__CSortMenu(CSortMenu* _this) {
    // Initialize vtable
    _this->mFileHandle = NULL;
    _this->mArcResAcc = NULL;
    _this->mpLayout = NULL;
    _this->mpAnimTrans0 = NULL;
    _this->mpAnimTrans1 = NULL;
    _this->field_0x28 = 0;
    _this->field_0x29 = 0;
    _this->field_0x2A = 0;
    _this->field_0x2B = 1;
    __ct__10CScrollBarFv((u8*)_this + 0x2C, 0);
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
// func_801D3064: Init sort menu — read file, create scrollbar data
// ============================================================================
extern "C" void func_801D3064(CSortMenu* _this) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    _this->mFileHandle = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)handle, lbl_eu_8050624C + 0x00, _this, 0, 0);
    setHandleFlag1__11CDeviceFileFP11CFileHandle(_this->mFileHandle);

    // Create a temporary CScrollBar to initialize the scrollbar data fields
    u8 stackScrollBar[0x48];
    __ct__10CScrollBarFv(stackScrollBar, 2);

    // Copy CScrollBar data fields from stack to member
    // CScrollBar data starts at offset 0x0C within the struct
    u32* src = (u32*)(stackScrollBar + 0x0C);
    u32* dst = (u32*)((u8*)_this + 0x30);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    dst[4] = src[4];
    dst[5] = src[5];
    dst[6] = src[6];
    dst[7] = src[7];
    // u8 fields at 0x2C-0x2F -> 0x50-0x53
    u8* src8 = (u8*)(stackScrollBar + 0x2C);
    u8* dst8 = (u8*)_this + 0x50;
    dst8[0] = src8[0];
    dst8[1] = src8[1];
    dst8[2] = src8[2];
    dst8[3] = src8[3];
    // f32 fields at 0x30-0x40 -> 0x54-0x64
    f32* srcf = (f32*)(stackScrollBar + 0x30);
    f32* dstf = (f32*)((u8*)_this + 0x54);
    dstf[0] = srcf[0];
    dstf[1] = srcf[1];
    dstf[2] = srcf[2];
    dstf[3] = srcf[3];
    dstf[4] = srcf[4];
    // u8 at 0x44 -> 0x68
    ((u8*)_this)[0x68] = ((u8*)(stackScrollBar + 0x44))[0];

    __dt__10CScrollBarFv(stackScrollBar, -1);
    func_801F34F4((u8*)_this + 0x2C);
}

// ============================================================================
// func_801D3160: Move — state machine dispatch
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
// func_801D3258: Term — cleanup
// ============================================================================
extern "C" void func_801D3258(CSortMenu* _this) {
    func_801390E0(&_this->mFileHandle);
    if (_this->mpLayout != NULL) {
        _this->mpLayout->Destroy(1);
        _this->mpLayout = NULL;
    }
    func_80139124(_this->mArcResAcc);
    _this->mArcResAcc = NULL;
    _this->field_0x28 = 0;
    func_8045F778__17UnkClass_8045F564Fv((u8*)_this + 0x04);
    func_801F35DC((u8*)_this + 0x2C);
}

// ============================================================================
// func_801D32DC: Check if scrollbar is visible
// ============================================================================
extern "C" u8 func_801D32DC(CSortMenu* _this) {
    if (CScrollBar_isVisible((u8*)_this + 0x2C)) {
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
// ============================================================================
extern "C" void func_801D3330(CSortMenu* _this) {
    if (_this->field_0x2A != 0) return;

    _this->field_0x2A = 1;
    _this->field_0x2B = 0;
    _this->field_0x28 = 1;

    nw4r::lyt::Pane* rootPane = _this->mpLayout->GetRootPane();
    nw4r::lyt::Pane* paneTxt1 = rootPane->FindPaneByName(lbl_eu_8050624C + 0x15, true);
    nw4r::lyt::Pane* paneTxt2 = rootPane->FindPaneByName(lbl_eu_8050624C + 0x1d, true);

    u8 textBuf[0x28];
    func_80137924(textBuf, paneTxt1, paneTxt2, _this->mpLayout->GetRootPane());

    func_801F3670((u8*)_this + 0x2C, textBuf);
    func_801F36BC((u8*)_this + 0x2C, 5, _this->mCount);
    func_801F367C((u8*)_this + 0x2C);

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
// ============================================================================
extern "C" void func_801D3454(void* textObj, CSortMenu* _this) {
    char buf[0x40];
    int pageIdx = (s8)_this->mPage;
    const char* fmtStr = (const char*)lbl_eu_805349B8[pageIdx];
    sprintf(buf, lbl_eu_8050624C + 0x2c, fmtStr);

    nw4r::lyt::Pane* rootPane = _this->mpLayout->GetRootPane();
    nw4r::lyt::Pane* pane1 = rootPane->FindPaneByName(lbl_eu_8050624C + 0x1d, true);
    nw4r::lyt::Pane* pane2 = rootPane->FindPaneByName(buf, true);

    u8 newTextBuf[0x40];
    func_80137924(newTextBuf, pane2, pane1, rootPane);
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
    nw4r::lyt::Pane* rootPane = _this->mpLayout->GetRootPane();
    nw4r::lyt::Pane* pane = rootPane->FindPaneByName(lbl_eu_8050624C + 0x2f, true);
    f32 height = *(f32*)((u8*)pane + 0x4C);
    f32 width = *(f32*)((u8*)pane + 0x50);

    u8 count = _this->mCount;
    u8 itemsPerPage = (count < 5) ? count : 5;

    f32 newWidth = height - (f32)(int)itemsPerPage * 16.0f;

    *(f32*)((u8*)pane + 0x4C) = height;
    *(f32*)((u8*)pane + 0x50) = newWidth;

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
    s8 page = (s8)_this->mPage - 1;
    _this->mPage = (u8)page;

    if (page < 0) {
        s8 subPage = (s8)_this->mSubPage - 1;
        _this->mPage = 0;
        _this->mSubPage = (u8)subPage;

        if (subPage < 0) {
            u8 count = _this->mCount;
            if (count >= 5) {
                _this->mPage = 4;
                _this->mSubPage = count - 5;
            } else {
                s8 newPage = (s8)(count - 1);
                _this->mPage = (u8)newPage;
                _this->mSubPage = 0;
                if (newPage < 0) {
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
        s8 page = (s8)_this->mPage + 1;
        _this->mPage = (u8)page;
        if (page >= 5) {
            s8 subPage = (s8)_this->mSubPage + 1;
            _this->mPage = 4;
            _this->mSubPage = (u8)subPage;
            if (subPage > (s8)(count - 5)) {
                _this->mPage = 0;
                _this->mSubPage = 0;
            }
        }
    } else {
        s8 page = (s8)_this->mPage + 1;
        _this->mPage = (u8)page;
        if (page >= (s8)count) {
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
        s8 subPage = (s8)_this->mSubPage - 5;
        _this->mSubPage = (u8)subPage;
        if (subPage < 0) {
            s8 newPage = (subPage & 0xFF) + 4;
            _this->mPage = (u8)newPage;
            _this->mSubPage = 0;
            if (newPage < 0) {
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
        s8 subPage = (s8)_this->mSubPage + 5;
        _this->mSubPage = (u8)subPage;
        s8 maxSubPage = (s8)(count - 5);
        if (subPage > maxSubPage) {
            s8 newPage = subPage - maxSubPage;
            _this->mPage = (u8)newPage;
            _this->mSubPage = (u8)maxSubPage;
            if (newPage >= 5) {
                _this->mPage = 4;
            }
        }
    } else {
        s8 page = (s8)(count - 1);
        _this->mPage = (u8)page;
        _this->mSubPage = 0;
        if (page < 0) {
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
extern "C" void func_801D3818(CSortMenu* _this, int value, u8* outPage, u8* outSubPage) {
    u8 page = func_8015780C(value);
    *outPage = page;
    *outSubPage = 0;
    if ((s8)page >= 5) {
        *outSubPage = page - 4;
        *outPage = 4;
    }
}

// ============================================================================
// func_801D3878: State 1 handler — opening animation
// ============================================================================
extern "C" void func_801D3878(CSortMenu* _this) {
    if (func_80137444(_this->mpAnimTrans0, 1.0f) != 0) {
        _this->field_0x2A = 2;
        _this->mpLayout->BindAnimation(_this->mpAnimTrans0, false);
        _this->mpLayout->BindAnimation(_this->mpAnimTrans1, true);
        _this->mpLayout->Animate(0);
    }
}

// ============================================================================
// func_801D390C: State 2 handler — wait for anim1
// ============================================================================
extern "C" void func_801D390C(CSortMenu* _this) {
    if (func_80137444(_this->mpAnimTrans1, 1.0f) != 0) {
        _this->field_0x2A = 3;
        _this->field_0x2B = 1;
    }
}

// ============================================================================
// func_801D3958: State 4 handler — scroll animation
// ============================================================================
extern "C" void func_801D3958(CSortMenu* _this) {
    if (func_80137510(_this->mpAnimTrans1, 1.0f) != 0) {
        _this->field_0x2A = 5;
        _this->mpLayout->BindAnimation(_this->mpAnimTrans1, false);
        _this->mpLayout->BindAnimation(_this->mpAnimTrans0, true);
        _this->mpLayout->Animate(0);
    }
}

// ============================================================================
// func_801D39EC: State 5 handler — closing animation
// ============================================================================
extern "C" void func_801D39EC(CSortMenu* _this) {
    if (func_80137510(_this->mpAnimTrans0, 1.0f) != 0) {
        _this->field_0x2A = 0;
        _this->field_0x2B = 1;
        _this->field_0x28 = 0;
    }
}

// ============================================================================
// func_801D3A3C: Update pane text for all 5 slots
// ============================================================================
extern "C" void func_801D3A3C(CSortMenu* _this) {
    for (int i = 0; i < 5; i++) {
        int idx = i + (s8)_this->mSubPage;
        if (idx >= (int)_this->mCount) {
            func_80136B4C(_this->mpLayout, (const char*)lbl_eu_805349B8[i], lbl_eu_8050624C + 0x3e, 0);
        } else {
            func_80136B4C(_this->mpLayout, (const char*)lbl_eu_805349B8[i], (const char*)(uintptr_t)_this->mArray[idx], 0);
        }
    }
    func_801F3850((u8*)_this + 0x2C, (s8)_this->mSubPage);
}

// ============================================================================
// CSortMenu::OnFileEvent — file load completion callback
// ============================================================================
extern "C" int OnFileEvent__9CSortMenuFP10CEventFile(CSortMenu* _this, CEventFile* event) {
    if (_this->mFileHandle != (CFileHandle*)((u8*)event + 4)) {
        return 0;
    }

    void* memHandle = getHandleMEM2__Q23mtl10MemManagerFv();
    createRegion__17UnkClass_8045F564FiiPCci((u8*)_this + 0x04, (int)memHandle, 0x2000, lbl_eu_8050624C + 0x3f, 1);

    u8 fileBuf[0x10];
    __ct__14Class_8045F858Fv(fileBuf, (u8*)_this + 0x04);

    void* fileData = _this->mFileHandle->mData;
    _this->mFileHandle->mData = NULL;

    func_80434A4C__Q23mtl10MemManagerFb(false);
    _this->mArcResAcc = (nw4r::lyt::ArcResourceAccessor*)createArcResourceAccessor__10CLibLayoutFv();
    _this->mArcResAcc->Attach(fileData, lbl_eu_8050624C + 0x49);

    func_80136E84(&_this->mpLayout, _this->mArcResAcc, lbl_eu_8050624C + 0x4d);
    func_80136F08(_this->mpLayout, &_this->mpAnimTrans0, _this->mArcResAcc, lbl_eu_8050624C + 0x63);
    func_80136F08(_this->mpLayout, &_this->mpAnimTrans1, _this->mArcResAcc, lbl_eu_8050624C + 0x7c);

    nw4r::lyt::Pane* rootPane = _this->mpLayout->GetRootPane();
    void* fontHandle = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, _this->mpLayout);
    void* fontVtable = *(void**)fontHandle;
    void (*setFontFunc)(void*, void*) = (void (*)(void*, void*))((void**)fontVtable)[9];
    setFontFunc(fontHandle, rootPane);

    _this->mpLayout->BindAnimation(_this->mpAnimTrans1, false);
    _this->mpLayout->BindAnimation(_this->mpAnimTrans0, true);
    _this->mpLayout->Animate(0);

    if (_this->mpLayout != NULL) {
        _this->field_0x29 = 1;
    }

    _this->mFileHandle = NULL;
    func_8045F810__17UnkClass_8045F564Fv((u8*)_this + 0x04);
    __dt__14Class_8045F858Fv(fileBuf, -1);

    return 1;
}