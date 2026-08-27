// Catalog TU for kyoshin/menu/CMenuPTChangeNotice
// Party-member change notice screen task reconstruction.

#include "kyoshin/harness_catalog.hpp"

#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/scn/IScnRender.hpp"

#include <cstdio>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>

#include "kyoshin/menu/CMenuPTChangeNotice.hpp"

// forward declaration for scaffold thunk reference
void cbRenderBefore__19CMenuPTChangeNoticeFv(void*);

// ---------------------------------------------------------------------------
// Constructor (unmangled retail symbol). Not yet matched (us-8029573c) -
// placeholder body keeps the ctor out-of-line so the factory below emits the
// real call instead of inlining an empty stub.
// ---------------------------------------------------------------------------
extern "C" CMenuPTChangeNotice* __ct__CMenuPTChangeNotice(CMenuPTChangeNotice* self,
                                                          void* arg) {
    self->mVtab = (u32)lbl_eu_80538888;
    self->mWorkEventVt = (u32)lbl_eu_80538888 + 0x24;
    self->mScnRenderVt = (u32)lbl_eu_80538888 + 0xac;
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);
    return self;
}

// ---------------------------------------------------------------------------
// Destructor. Stores the class + dispatch vtables by hand, disables pad input,
// destroys the embedded MEM2 region and the IUIWindow base, then the
// deleting-flag path calls operator delete.
// ---------------------------------------------------------------------------
extern "C" CMenuPTChangeNotice* __dt__19CMenuPTChangeNoticeFv(CMenuPTChangeNotice* self,
                                                              int flags) {
    if (self != 0) {
        self->mVtab = (u32)lbl_eu_80538888;
        self->mWorkEventVt = (u32)lbl_eu_80538888 + 0x24;
        self->mScnRenderVt = (u32)lbl_eu_80538888 + 0xac;
        setInputDisableTime__Q22cf9CfPadTaskFf(lbl_eu_80668B80);
        __dt__17UnkClass_8045F564Fv(&self->mMemRegion, -1);
        __dt__9IUIWindowFv(self, 0);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void CMenuPTChangeNotice::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(true);

    // Pass the IWorkEvent subobject to the async file loader; retail keeps the
    // null-checked compare/branch pair around the +0x6c adjustment.
    IWorkEvent* evt = reinterpret_cast<IWorkEvent*>(this);
    if (this != 0) {
        evt = reinterpret_cast<IWorkEvent*>(&mWorkEventVt);
    }
    mFileHandle = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        mtl::MemManager::getHandleMEM2(), lbl_eu_80662AF8, evt, 0, 0);
}

void CMenuPTChangeNotice::Term() {}

void CMenuPTChangeNotice::Move() {}

// ---------------------------------------------------------------------------
// func_8029348C - async file-load completion: build the notice layout from the
// freshly loaded arc. Returns 1 on success.
// ---------------------------------------------------------------------------
extern "C" int func_8029348C(CMenuPTChangeNotice* self, CEventFile* evt) {
    // Build the notice layout from the freshly loaded arc when the event's
    // handle matches the load we issued.
    if (self->mFileHandle == (u32)evt->mFileHandle) {
        // Scoped MEM2 region guard, then build the layout + animations.
    self->mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x10000,
                                  lbl_eu_8050FC20, 0);
    char buf[0x20];
    float srcA[2];
    float srcB[2];
    Class_8045F858 regionGuard(&self->mMemRegion);

    // Take the archive buffer out of the file handle and hand it to the
    // resource accessor.
    CFileHandle* fh = (CFileHandle*)self->mFileHandle;
    u8* data = fh->mData;
    fh->mData = NULL;
    mtl::MemManager::setMemInitFlag(false);
    self->mAccessor = CLibLayout::createArcResourceAccessor();
    self->mAccessor->Attach(data, &lbl_eu_8050FC20[0x14]);

    func_80136E84(&self->mLayout90, self->mAccessor, &lbl_eu_8050FC20[0x18]);
    func_80136F08(self->mLayout90, &self->mAnim94, self->mAccessor,
                  &lbl_eu_8050FC20[0x31]);

    // Bind the font and hand the loaded font object over to the root pane.
    nw4r::lyt::Pane* rootPane = self->mLayout90->GetRootPane();
    void** fontObj = (void**)getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
        1, self->mLayout90);
    u32 fontResult = ((u32 (*)(void*))fontObj[0x24 / 4])(fontObj);
    func_8013676C(rootPane, fontResult);

    self->mLayout90->SetAnimationEnable(self->mAnim94, true);
    self->mAnim94->SetFrame(lbl_eu_80668B84);
    self->mLayout90->Animate(0);

    // Default-0 text fields.
    func_80136B4C(self->mLayout90, &lbl_eu_8050FC20[0x5c],
                  func_80136190(&lbl_eu_8050FC20[0x4d], &lbl_eu_8050FC20[0x57],
                                0xb),
                  0);
    func_80136B4C(self->mLayout90, &lbl_eu_8050FC20[0x69],
                  func_80136190(&lbl_eu_8050FC20[0x4d], &lbl_eu_8050FC20[0x57],
                                0xa),
                  0);
    func_80136B4C(self->mLayout90, &lbl_eu_8050FC20[0x89],
                  func_80136190(&lbl_eu_8050FC20[0x76], &lbl_eu_8050FC20[0x84],
                                0x2f),
                  0);

    // Party-member icon texture: pick the message name by pad type, load the
    // "timg" texture, bind it and set its pixel size on the pane.
    {
        const char* msgName = isClassicController__Q22cf13CfGameManagerFv(-1)
                                  ? &lbl_eu_8050FC20[0x95]
                                  : &lbl_eu_8050FC20[0x9e];
        u16 msg = func_8013606C(&lbl_eu_8050FC20[0x76], msgName, 0x2f);
        char* handle = func_80138F78(msg);
        nw4r::lyt::ArcResourceAccessor* mgr = func_801355F4();
        void* tex = mgr->GetResource(0x74696D67 /* 'timg' */, handle, NULL);
        if (tex != NULL) {
            func_80137E7C(self->mLayout90, &lbl_eu_8050FC20[0xa7], tex);
            // Texture sub-resource carries a 2D dimension header (u16 w,h).
            CMenuPTChangeNotice::CTextureDims* dims = ((CMenuPTChangeNotice::CTextureObj*)tex)->mChain->mDims;
            u16 h = dims->mH;
            u16 w = dims->mW;
            nw4r::lyt::Pane* pane = self->mLayout90->GetRootPane();
            void* res = ((void* (*)(void*, const char*, u32, void*))(
                (*(void***)pane))[0x3c / 4])(pane, &lbl_eu_8050FC20[0xa7], 1,
                                             dims);
            if (res != NULL) {
                srcA[0] = (float)(u32)h;
                srcA[1] = (float)(u32)w;
                func_80124288((nw4r::lyt::Pane*)res, srcA);
            }
        }
    }

    if (!func_8009CF8C(0x334b)) {
        // Party icons unavailable: hide the two member panes.
        nw4r::lyt::Pane* pane = self->mLayout90->GetRootPane()
                                     ->FindPaneByName(&lbl_eu_8050FC20[0xb1],
                                                      true);
        if (pane != NULL) {
            func_80124270(pane, 0);
        }
        pane = self->mLayout90->GetRootPane()
                   ->FindPaneByName(&lbl_eu_8050FC20[0xb8], true);
        if (pane != NULL) {
            func_80124270(pane, 0);
        }
    } else {
        func_80136B4C(self->mLayout90, &lbl_eu_8050FC20[0xb1],
                      func_80136190(&lbl_eu_8050FC20[0x76],
                                    &lbl_eu_8050FC20[0x84], 0x2e),
                      0);
        const char* msgName = isClassicController__Q22cf13CfGameManagerFv(-1)
                                  ? &lbl_eu_8050FC20[0x95]
                                  : &lbl_eu_8050FC20[0x9e];
        u16 msg = func_8013606C(&lbl_eu_8050FC20[0x76], msgName, 0x2e);
        char* handle = func_80138F78(msg);
        nw4r::lyt::ArcResourceAccessor* mgr = func_801355F4();
        void* tex = mgr->GetResource(0x74696D67 /* 'timg' */, handle, NULL);
        if (tex != NULL) {
            func_80137E7C(self->mLayout90, &lbl_eu_8050FC20[0xb8], tex);
            CMenuPTChangeNotice::CTextureDims* dims = ((CMenuPTChangeNotice::CTextureObj*)tex)->mChain->mDims;
            u16 h = dims->mH;
            u16 w = dims->mW;
            nw4r::lyt::Pane* pane = self->mLayout90->GetRootPane();
            void* res = ((void* (*)(void*, const char*, u32, void*))(
                (*(void***)pane))[0x3c / 4])(pane, &lbl_eu_8050FC20[0xb8], 1,
                                             dims);
            if (res != NULL) {
                srcB[0] = (float)(u32)h;
                srcB[1] = (float)(u32)w;
                func_80124288((nw4r::lyt::Pane*)res, srcB);
            }
        }
    }

    // Slot panes: active slots (< count) get the member icon texture bound,
    // inactive slots are hidden (pane cleared).
    {
        func_80139198(0);
        u8 count = code80135FDC_getByte_64077();
        for (u32 i = 0; i < 7; i++) {
            u8 idx = (u8)i;
            if (idx < count) {
                u8 val = func_801392B4(idx);
                if (idx < 3) {
                    sprintf(buf, &lbl_eu_8050FC20[0xde], idx + 1);
                } else {
                    sprintf(buf, &lbl_eu_8050FC20[0xec], idx - 2);
                }
                u16 msg = func_80136254(lbl_eu_80664090,
                                        &lbl_eu_8050FC20[0xfa], val);
                char* str = func_80138F78(msg);
                void* tex = self->mAccessor->GetResource(0x74696D67, str, NULL);
                if (tex != NULL) {
                    func_80137E7C(self->mLayout90, buf, tex);
                }
            } else {
                if (idx < 3) {
                    sprintf(buf, &lbl_eu_8050FC20[0xc2], idx + 1);
                } else {
                    sprintf(buf, &lbl_eu_8050FC20[0xd0], idx - 2);
                }
                nw4r::lyt::Pane* pane =
                    self->mLayout90->GetRootPane()->FindPaneByName(buf, true);
                if (pane != NULL) {
                    func_80124270(pane, 0);
                }
            }
        }
    }

    // Affinity panes: same pattern over the three affinity slots.
    {
        u8 count = code80135FDC_getByte_6407E();
        for (u32 i = 0; i < 3; i++) {
            u8 idx = (u8)i;
            if (idx < count) {
                u8 val = func_801392C8(idx);
                sprintf(buf, &lbl_eu_8050FC20[0x113], idx + 1);
                u16 msg = func_80136254(lbl_eu_80664098,
                                        &lbl_eu_8050FC20[0x121], val);
                char* str = func_80138F78(msg);
                nw4r::lyt::ArcResourceAccessor* mgr = func_801355F4();
                void* tex = mgr->GetResource(0x74696D67, str, NULL);
                if (tex != NULL) {
                    func_80137E7C(self->mLayout90, buf, tex);
                }
            } else {
                sprintf(buf, &lbl_eu_8050FC20[0x105], idx + 1);
                nw4r::lyt::Pane* pane =
                    self->mLayout90->GetRootPane()->FindPaneByName(buf, true);
                if (pane != NULL) {
                    func_80124270(pane, 0);
                }
            }
        }
    }

    // Register as a scene render callback; IScnRender subobject lives at +0x70.
    IScnRender* cb = reinterpret_cast<IScnRender*>(self);
    if (self != 0) {
        cb = reinterpret_cast<IScnRender*>(&self->mScnRenderVt);
    }
    self->mScene->addRenderCB(cb, 0xa, 0);

        self->mFileHandle = 0;
        self->mMemRegion.func_8045F810();
        return 1;
    }
    return 0;
}

void CMenuPTChangeNotice::cbRenderBefore() {}

// ---------------------------------------------------------------------------
// func_80293B9C - task entry: if the notice singleton already exists return 0,
// otherwise allocate 0x9c bytes from work memory, construct the notice, store
// it in the .sbss global and register it as a CProcess child of `parent`.
// ---------------------------------------------------------------------------
extern "C" CMenuPTChangeNotice* func_80293B9C(CProcess* parent, void* arg) {
    if (lbl_eu_80664A00 != 0) {
        return 0;
    }
    CMenuPTChangeNotice* obj = (CMenuPTChangeNotice*)mtl::MemManager::allocate(
        0x9c, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuPTChangeNotice(obj, arg);
    }
    lbl_eu_80664A00 = (u32)obj;
    Regist__8CProcessFP8CProcessb((CProcess*)obj, parent, 0);
    return (CMenuPTChangeNotice*)lbl_eu_80664A00;
}

void stub_us_8029620c() {}

// ---------------------------------------------------------------------------
// func_80293C20 - advance the notice animation; once it reaches its target
// frame, switch the notice state to 2 (party-change applied).
// ---------------------------------------------------------------------------
extern "C" void func_80293C20(CMenuPTChangeNotice* self) {
    if (func_80137444(self->mAnim94, lbl_eu_80668B90) != 0) {
        self->mField98 = 2;
    }
}

void func_80293C64(){}

// ---------------------------------------------------------------------------
// func_80293D2C - advance the notice animation; once it reaches its target
// frame, raise the party-change trigger flag at +0x64.
// ---------------------------------------------------------------------------
extern "C" void func_80293D2C(CMenuPTChangeNotice* self) {
    if (func_80137510(self->mAnim94, lbl_eu_80668B90) != 0) {
        self->mField64 = 1;
    }
}

void OnFileEvent__19CMenuPTChangeNoticeFP10CEventFile(void* self) { ((void(*)(void*))func_8029348C)((char*)self - 0x6c); }

void func_80293D78(void* self) { ((void(*)(void*))__dt__19CMenuPTChangeNoticeFv)((char*)self - 0x6c); }

void func_80293D80(void* self) { ((void(*)(void*))cbRenderBefore__19CMenuPTChangeNoticeFv)((char*)self - 0x70); }

extern "C" void func_80293D88(void* self) { ((void(*)(void*))__dt__19CMenuPTChangeNoticeFv)((char*)self - 0x70); }

extern "C" unsigned long func_80293C10(void) { return lbl_eu_80664A00 != 0; }
