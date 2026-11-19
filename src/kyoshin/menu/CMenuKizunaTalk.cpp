// Decomp of kyoshin/menu/CMenuKizunaTalk.
// Kizuna (friendship) talk screen: singleton process with IUIWindow +
// IWorkEvent + IScnRender bases, an embedded CCur18 cursor and CSysWin panel.

#include "kyoshin/menu/CMenuKizunaTalk.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

#include "functions.hpp"

#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/work/CEventFile.hpp"
#include <nw4r/lyt.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// KEEP (extern-C cleanup 2026-10-23): CLibLayout.hpp cannot be included here
// (it brings the canonical IWorkEvent, which clashes with this TU's KEEP copy).
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();

#include "kyoshin/cf/CfGameManager.hpp"  // CtrlRemote_SetSharedBit
extern "C" void setGXColorS10(void*, s16, s16, s16, s16);
extern "C" void SplitU32ToS16s(short*, unsigned long);

extern "C" void __dt__15CMenuKizunaTalkFv(void*, int);
extern "C" void cbRenderBefore__15CMenuKizunaTalkFv(void*);

// Defined later in this TU (retail symbol is unmangled).
extern "C" void checkKizunaGate(CMenuKizunaTalk* self);

CMenuKizunaTalk* CMenuKizunaTalk::sInstance;

// ---------------------------------------------------------------------------
// Constructor (retail symbol is the unmangled __ct__CMenuKizunaTalk, so the
// body is written as a C-linkage free function; the factory createKizunaTalkSingleton
// calls it). Runs the CProcess base ctor, stores the composite vtable and the
// IWorkEvent / IScnRender subobject vtables, then constructs the embedded
// memory region and cursor; the remaining scalar state gets retail defaults.
// ---------------------------------------------------------------------------
extern "C" CMenuKizunaTalk* __ct__CMenuKizunaTalk(CMenuKizunaTalk* obj,
                                                  CScn* scene, u32 charId) {
    __ct__8CProcessFv(obj);

    obj->mField10 = (u32)lbl_eu_8052D238;
    obj->mLayout = 0;
    obj->mField58 = 0;
    obj->mField5C = 0;
    obj->mField60 = -1;
    obj->mField64 = 0;
    obj->mField65 = 0;
    obj->mField66 = 0;
    obj->mField67 = 1;
    obj->mField68 = 0;

    obj->mField10 = (u32)lbl_eu_80533A98;
    // Subobject vtable slots (composite vtable + 0x24 / + 0xAC).
    u32 workVt = (u32)lbl_eu_80533A98 + 0x24;
    u32 renderVt = (u32)lbl_eu_80533A98 + 0xAC;
    static_cast<KizunaWorkEvent*>(obj)->mVtable = workVt;
    static_cast<KizunaScnRender*>(obj)->mVtable = renderVt;
    obj->mScene = scene;

    new (&obj->mMemRegion) UnkClass_8045F564;
    obj->mFile1 = 0;
    obj->mFile2 = 0;
    obj->mArcAccessor = 0;
    obj->mAnim = 0;
    obj->mCharId = charId;
    obj->mField9C = 1;
    obj->mFieldA0 = 0;
    obj->mFieldA4 = 0;

    __ct__6CCur18FPQ34nw4r3lyt19ArcResourceAccessor(&obj->mCur, 0);
    return obj;
}

// ---------------------------------------------------------------------------
// Destructor: the CSysWin / CCur18 / UnkClass_8045F564 regions are raw
// storage whose retail dtors are C-linkage free functions, so they are
// destroyed by hand in reverse construction order; the IUIWindow base dtor
// (__dt__9IUIWindowFv) is emitted automatically after the body.
// ---------------------------------------------------------------------------
CMenuKizunaTalk::~CMenuKizunaTalk() {
    __dt__7CSysWinFv(&mSysWin[0], -1);
}

// Initialise the talk screen: request the shared archive (the loader gets the
// IWorkEvent subobject at +0x6C via the base conversion), then set the
// character's affinity-talk-seen BDAT flag.
void CMenuKizunaTalk::Init() {
    mFile2 = (CFileHandle*)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        KyoshinHeap_GetField44(), lbl_eu_80505118, static_cast<KizunaWorkEvent*>(this), 0, 0);
    CtrlRemote_SetSharedBit(mCharId + 0x3440, 1);
}

// Tear the talk screen down: release the file handles and the layout archive,
// remove the render callback, run the embedded widget teardowns and clear the
// singleton / BDAT globals.
void CMenuKizunaTalk::Term() {
    waitForDrawDone__9CDeviceVIFv();
    CBdat::getEntry(2);
    closeFileHandle((CFileHandle**)&mFile1);
    closeFileHandle((CFileHandle**)&mFile2);

    mScene->removeRenderCB(reinterpret_cast<IScnRender*>(static_cast<KizunaScnRender*>(this)));

    if (mLayout != 0) {
        // Layout virtual slot 2 (vtable + 0x8): release/unbind with flag 1.
        // Retail emits the null guard twice (outer block guard + inner call
        // guard), so the source nests two identical checks.
        if (mLayout != 0) {
            reinterpret_cast<CLayoutView*>(mLayout)->vf02(1);
        }
        mLayout = 0;
    }

    releaseArcResourceAccessor(mArcAccessor);
    mArcAccessor = 0;
    reinterpret_cast<CSysWin*>(&mSysWin[0])->termLayout();

    // CCur18 cursor virtual slot 3 (vtable + 0xC): per-frame update.
    mCur.cleanup();

    sKizunaBdatTable = 0;
    mMemRegion.deleteRegion();
    CMenuKizunaTalk::sInstance = 0;
}

void CMenuKizunaTalk::Move() {}

void CMenuKizunaTalk::cbRenderBefore() {}

// ---------------------------------------------------------------------------
// Factory: lazily allocate + construct the single kizuna talk instance from
// work memory and register it under `parent`. Returns 0 when the singleton
// already exists.
// ---------------------------------------------------------------------------
CMenuKizunaTalk* createKizunaTalkSingleton(CProcess* parent, CScn* scene, u32 charId) {
    if (CMenuKizunaTalk::sInstance != 0) {
        return 0;
    }
    mtl::ALLOC_HANDLE mem = CWorkThreadSystem::getWorkMem();
    CMenuKizunaTalk* obj =
        (CMenuKizunaTalk*)mtl::MemManager::allocate(0xfc, mem);
    if (obj != 0) {
        obj = __ct__CMenuKizunaTalk(obj, scene, charId);
    }
    CMenuKizunaTalk::sInstance = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, false);
    return CMenuKizunaTalk::sInstance;
}

// (CMenuKizunaTalk::sInstance != 0) - the size-optimized subic/subfe form.
extern "C" bool isKizunaTalkActive() { return CMenuKizunaTalk::sInstance != 0; }

// ---------------------------------------------------------------------------
// Window-state kickoff (retail kickoffKizunaWindow): refresh the availability gate
// via checkKizunaGate, then per window state: 0/2 rebuild the CSysWin panel from
// a stack temp (directions 0 / 3) and dispatch the layout-build virtual at
// vtable +0x88; state 2 also seeds the cursor from a CCur18 temp. State 1
// requests the talk-list file through the IWorkEvent subobject (+0x6C,
// null-this-safe).
// ---------------------------------------------------------------------------
void kickoffKizunaWindow(CMenuKizunaTalk* self) {
    if (sKizunaBdatTable == 0) {
        return;
    }
    self->mFieldA0 = 1;
    checkKizunaGate(self);
    // Shared CSysWin scratch plus a full CCur18-sized cursor scratch.
    KizunaScratch scratch;
    switch (self->mFieldA4) {
    case 0:
        __ct__7CSysWinFUc(&scratch.mWin0[0], 0);
        *reinterpret_cast<CSysWin*>(&self->mSysWin[0]) = *reinterpret_cast<const CSysWin*>(&scratch.mWin0[0]);
        __dt__7CSysWinFv(&scratch.mWin0[0], -1);
        reinterpret_cast<KizunaSysWinView*>(&self->mSysWin[0])->v20();
        break;
    case 1: {
        // Null-safe IWorkEvent subobject address (retail beq/addi shape).
        KizunaWorkEvent* evt = reinterpret_cast<KizunaWorkEvent*>(self);
        if (self != 0) {
            evt = &static_cast<KizunaWorkEvent&>(*self);
        }
        self->mFile1 =
            (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
                (u32)mtl::MemManager::getHandleMEM2(), &lbl_eu_80505118[0x1c],
                evt, 0, 0);
        break;
    }
    case 2: {
        __ct__7CSysWinFUc(&scratch.mWin2[0], 3);
        *reinterpret_cast<CSysWin*>(&self->mSysWin[0]) = *reinterpret_cast<const CSysWin*>(&scratch.mWin2[0]);
        __dt__7CSysWinFv(&scratch.mWin2[0], -1);
        reinterpret_cast<KizunaSysWinView*>(&self->mSysWin[0])->v20();

        self->mCur = CCur18(CUICfManager_getArcResourceAccessor());
        self->mCur.initLayout();
        break;
    }
    }
}

// ---------------------------------------------------------------------------
// Kizuna talk window advance (retail updateKizunaTalkWindow): state machine on
// mFieldA4. State 0 builds the "talk" window, state 1 waits for the layout
// then plays the open sound, state 2 builds the "leave" window with the
// character name; every successful state sets mFieldA0 and (re)registers the
// render callback.
// ---------------------------------------------------------------------------
extern "C" void updateKizunaTalkWindow(CMenuKizunaTalk* self) {
    switch (self->mFieldA4) {
    case 0:
        if (reinterpret_cast<CSysWin*>(&self->mSysWin[0])->isReady() == 0) {
            return;
        }
        reinterpret_cast<CSysWin*>(&self->mSysWin[0])->setKindContent(reinterpret_cast<char*>((u32)BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x24)), 0);
        reinterpret_cast<CSysWin*>(&self->mSysWin[0])->applyKind(1);
        reinterpret_cast<CSysWin*>(&self->mSysWin[0])->openPhase1();
        break;
    case 1:
        if (self->mLayout == 0) {
            return;
        }
        playUISound(0xd);
        break;
    case 2:
        if (reinterpret_cast<CSysWin*>(&self->mSysWin[0])->isReady() == 0) {
            return;
        }
        char* s1 = BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x2a);
        char* s2 = BdatGetPtrDirect(sKizunaBdatTable, &lbl_eu_80505118[0x43], self->mCharId);
        char* s3 = BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x2c);
        char* s4 = BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x2d);
        reinterpret_cast<CSysWin*>(&self->mSysWin[0])->setKindContent(reinterpret_cast<char*>((u32)s1), reinterpret_cast<char*>((int)s2));
        reinterpret_cast<CSysWin*>(&self->mSysWin[0])->setTwoTextValues(reinterpret_cast<char*>((u32)s3), reinterpret_cast<char*>((u32)s4));
        reinterpret_cast<CSysWin*>(&self->mSysWin[0])->applyKind(0);
        reinterpret_cast<CSysWin*>(&self->mSysWin[0])->openPhase1();
        break;
    }
    self->mFieldA0 = 2;
    self->mScene->addRenderCB(reinterpret_cast<IScnRender*>(static_cast<KizunaScnRender*>(self)), 0xd, 0);
}

// ---------------------------------------------------------------------------
// Kizuna talk window advance (retail advanceKizunaWindow): state machine on
// mFieldA4. State 0/2 wait for the CSysWin panel to become active; state 1
// waits for the layout animation to reach its target frame. State 2 also
// feeds the panel's current page into the cursor's selection update.
// ---------------------------------------------------------------------------
void advanceKizunaWindow(CMenuKizunaTalk* self) {
    switch (self->mFieldA4) {
    case 0:
        if (reinterpret_cast<CSysWin*>(&self->mSysWin[0])->isActive() != 0) {
            self->mFieldA0 = 3;
        }
        break;
    case 1:
        if (advanceAnimTransform(self->mAnim, lbl_eu_80667E68) != 0) {
            self->mFieldA0 = 3;
        }
        break;
    case 2:
        if (reinterpret_cast<CSysWin*>(&self->mSysWin[0])->isActive() != 0) {
            self->mFieldA0 = 3;
            self->mCur.SetVisible(1);
            u8 tmp[0x10];
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(tmp), reinterpret_cast<CSysWin*>(&self->mSysWin[0]), (u8)self->mField9C);
            self->mCur.setRootPaneTranslate(reinterpret_cast<nw4r::math::VEC3*>(tmp));
        }
        break;
    }
}

// ---------------------------------------------------------------------------
// Kizuna talk window-state advance (retail driveKizunaTalkPad): reads the cf pad
// and drives the mFieldA4 window state machine. States 0/1 wait for the left/
// right D-pad bits, state 2 handles cancel/confirm page navigation and the
// confirm/advance actions. The D-pad bit position differs between player 0
// (co-op) and the others.
// ---------------------------------------------------------------------------
void driveKizunaTalkPad(CMenuKizunaTalk* self) {
    KizunaTalkPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    u32 cancel, confirm, left, right;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Player 0 (co-op): D-pad bits sit at different positions.
        u32 pressed = pad->mPressedFlags;
        u32 turbo = pad->mTurboFlags;
        cancel = (turbo & 0x8004) != 0;
        // Two-bit gather (bits 0x10000|0x8) -> MWCC rlwinm/rlwimi form.
        confirm = (turbo & 0x10008) != 0;
        left = (pressed >> 21) & 1;
        right = (pressed >> 22) & 1;
    } else {
        u32 pressed = pad->mPressedFlags;
        u32 turbo = pad->mTurboFlags;
        cancel = (turbo & 0x8004) != 0;
        confirm = (turbo & 0x10008) != 0;
        left = (pressed >> 4) & 1;
        right = (pressed >> 5) & 1;
    }

    switch (self->mFieldA4) {
    case 0:
        if (left != 0) {
            self->mField60 = 2;
            playUISound(3);
            self->mFieldA0 = 4;
            reinterpret_cast<CSysWin*>(&self->mSysWin[0])->advancePhase3();
        } else if (right != 0) {
            self->mField60 = 2;
            self->mFieldA0 = 4;
            playUISound(6);
            reinterpret_cast<CSysWin*>(&self->mSysWin[0])->advancePhase3();
        }
        break;
    case 1:
        if (left != 0) {
            self->mField60 = 2;
            playUISound(3);
            self->mFieldA0 = 4;
        } else if (right != 0) {
            self->mField60 = 2;
            self->mFieldA0 = 4;
            playUISound(6);
        }
        break;
    case 2:
        if (cancel != 0) {
            // Cancel: move the cursor page up (wrap below 0 to 1).
            self->mField9C = (s8)(self->mField9C - 1);
            if (self->mField9C < 0) self->mField9C = 1;
            playUISound(1);
            nw4r::math::VEC3 tmp;
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&tmp), reinterpret_cast<CSysWin*>(&self->mSysWin[0]), (u8)self->mField9C);
            self->mCur.setRootPaneTranslate(&tmp);
        } else if (confirm != 0) {
            // Confirm: move the cursor page down (wrap over 1 to 0).
            self->mField9C = (s8)(self->mField9C + 1);
            if (self->mField9C > 1) self->mField9C = 0;
            playUISound(1);
            nw4r::math::VEC3 tmp;
            CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&tmp), reinterpret_cast<CSysWin*>(&self->mSysWin[0]), (u8)self->mField9C);
            self->mCur.setRootPaneTranslate(&tmp);
        } else if (left != 0) {
            // Left: confirm the current page and advance the window.
            s8 page = self->mField9C;
            self->mField60 = page + 1;
            if (page == 0) {
                playUISound(0x37);
            } else {
                playUISound(3);
            }
            self->mFieldA0 = 4;
            reinterpret_cast<CSysWin*>(&self->mSysWin[0])->advancePhase3();
            self->mCur.SetVisible(0);
        } else if (right != 0) {
            self->mField60 = 2;
            self->mFieldA0 = 4;
            playUISound(6);
            reinterpret_cast<CSysWin*>(&self->mSysWin[0])->advancePhase3();
            self->mCur.SetVisible(0);
        }
        break;
    }
}

// Advance the kizuna talk window state machine (retail pollKizunaTalkReady):
// window states 0 and 2 wait for the CSysWin panel to become active, state 1
// waits for the embedded animation transform to finish; each sets the
// IUIWindow "advance" flag when its wait completes.
extern "C" void pollKizunaTalkReady(CMenuKizunaTalk* self) {
    switch (self->mFieldA4) {
    case 0:
        if (reinterpret_cast<CSysWin*>(&self->mSysWin[0])->isActive() != 0) {
            self->mField64 = 1;
        }
        break;
    case 1:
        if (AnimRewindFrame(self->mAnim, lbl_eu_80667E68) != 0) {
            self->mField64 = 1;
        }
        break;
    case 2:
        if (reinterpret_cast<CSysWin*>(&self->mSysWin[0])->isActive() != 0) {
            self->mField64 = 1;
        }
        break;
    }
}

// ---------------------------------------------------------------------------
// Kizuna talk gating check (retail checkKizunaGate): reads the character's
// affinity-talk columns and decides whether the talk screen is available;
// writes the resulting mFieldA4 gate state.
// ---------------------------------------------------------------------------
// Retail symbol is the unmangled checkKizunaGate.
extern "C" void checkKizunaGate(CMenuKizunaTalk* self) {
    u16 total = BdatGetU16Direct(sKizunaBdatTable, &lbl_eu_80505118[0x49], self->mCharId);
    if ((u32)(u16)total <= CtrlRemote_TouchBitByArg((u32)0x20)) {
        u32 b1 = 0, b2 = 0, b3 = 0, b4 = 0;
        u8 talk = BdatGetU8Direct((const void*)sKizunaBdatTable, &lbl_eu_80505118[0x4e], self->mCharId);
        u8 talk2 = BdatGetU8Direct((const void*)sKizunaBdatTable, &lbl_eu_80505118[0x57], self->mCharId);
        u16 value = BdatGetU16Direct(sKizunaBdatTable, &lbl_eu_80505118[0x60], self->mCharId);
        u8 rank = BdatGetU8Direct((const void*)sKizunaBdatTable, &lbl_eu_80505118[0x69], self->mCharId);
        if (CtrlRemote_TouchBitByArg((u32)0x20) >= 0x113) {
            if ((u8)talk == 3) talk = 8;
            if ((u8)talk2 == 3) talk2 = 8;
        }
        int* list = CtrlObjectParam_GetSlotTableBase() + 1;
        for (u32 i = 0; i < 3; i++) {
            u32 v = (u32)list[i];
            if (talk == v) {
                b1 = 1;
            } else if (talk2 == v) {
                b2 = 1;
            }
        }
        if ((u16)value <= (u16)LookupRelateValue((u8)talk, (u8)talk2)) b3 = 1;
        u8 cur = (u8)CfT_DispatchFrameCounts();
        if ((u8)rank == 0 || (u8)rank == cur) b4 = 1;
        if (b1 != 0 && b2 != 0 && b3 != 0 && b4 != 0) {
            self->mFieldA4 = 2;
        } else {
            self->mFieldA4 = 1;
        }
    } else {
        self->mFieldA4 = 0;
    }
}

// ---------------------------------------------------------------------------
// Talk list file-event handler (retail handleKizunaFileEvent): when the requested
// talk-list archive finishes loading it builds the layout, fills in the
// character/rank text and loads the character portrait texture; when the
// shared BDAT archive finishes it reloads the character table pointer.
// ---------------------------------------------------------------------------
int handleKizunaFileEvent(CMenuKizunaTalk* self, CEventFile* evt) {
    CFileHandle* handle = evt->mFileHandle;
    if (self->mFile1 == handle) {
        mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
        self->mMemRegion.createRegion(
            (int)(u32)mem2, 0x2000, &lbl_eu_80505118[0x6e], 0);
        Class_8045F858 regionHandle(&self->mMemRegion);
        void* data = self->mFile1->getData();
        mtl::MemManager::setMemInitFlag(false);
        nw4r::lyt::ArcResourceAccessor* acc = createArcResourceAccessor__10CLibLayoutFv();
        self->mArcAccessor = acc;
        acc->Attach(data, &lbl_eu_80505118[0x7e]);
        buildLayout(&self->mLayout, acc, &lbl_eu_80505118[0x82]);
        bindLayoutAnimTransform(self->mLayout, &self->mAnim, acc, &lbl_eu_80505118[0x9b]);

        nw4r::lyt::Layout* layout = self->mLayout;
        nw4r::lyt::Pane* rootPane = layout->GetRootPane();
        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, layout);
        u32 fontVal = ((u32 (*)(void*))((void**)fontObj)[0x24 / 4])(fontObj);
        ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontVal);

        u32 charName = BdatGetU8Direct((const void*)sKizunaBdatTable, &lbl_eu_80505118[0x4e], self->mCharId);
        u32 charName2 = BdatGetU8Direct((const void*)sKizunaBdatTable, &lbl_eu_80505118[0x57], self->mCharId);
        u16 rank = BdatGetU16Direct(sKizunaBdatTable, &lbl_eu_80505118[0x60], self->mCharId);
        u32 affinity = BdatGetU8Direct((const void*)sKizunaBdatTable, &lbl_eu_80505118[0x69], self->mCharId);

        LayoutSetTextBoxFmtValue(layout, &lbl_eu_80505118[0xb7],
                      BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x25), 0);
        LayoutSetTextBoxFmtValue(layout, &lbl_eu_80505118[0xc4],
                      BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x26), 0);
        LayoutSetTextBoxFmtValue(layout, &lbl_eu_80505118[0xd1],
                      BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x28), 0);
        LayoutSetTextBoxFmtValue(layout, &lbl_eu_80505118[0xde],
                      BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x27), 0);
        LayoutSetTextBoxFmtValue(layout, &lbl_eu_80505118[0xf6],
                      BdatTouchStringCell(&lbl_eu_80505118[0xeb], &lbl_eu_80505118[0x3e], (u8)charName), 0);
        LayoutSetTextBoxFmtValue(layout, &lbl_eu_80505118[0x106],
                      BdatTouchStringCell(&lbl_eu_80505118[0xeb], &lbl_eu_80505118[0x3e], (u8)charName2), 0);

        // Rank text ladder: pick the portrait texture by the rank value.
        void* rankTex;
        if ((u32)(u16)rank < 0x3e8) {
            rankTex = ((void* (*)(nw4r::lyt::ArcResourceAccessor*, u32, const char*, u32))(
                (void**)self->mArcAccessor)[0xc / 4])(self->mArcAccessor, 0x74696d67,
                                                       &lbl_eu_80505118[0x116], 0);
        } else if ((u32)(u16)rank < 0x7d0) {
            rankTex = ((void* (*)(nw4r::lyt::ArcResourceAccessor*, u32, const char*, u32))(
                (void**)self->mArcAccessor)[0xc / 4])(self->mArcAccessor, 0x74696d67,
                                                       &lbl_eu_80505118[0x12e], 0);
        } else if ((u32)(u16)rank < 0xbb8) {
            rankTex = ((void* (*)(nw4r::lyt::ArcResourceAccessor*, u32, const char*, u32))(
                (void**)self->mArcAccessor)[0xc / 4])(self->mArcAccessor, 0x74696d67,
                                                       &lbl_eu_80505118[0x146], 0);
        } else if ((u32)(u16)rank < 0x1388) {
            rankTex = ((void* (*)(nw4r::lyt::ArcResourceAccessor*, u32, const char*, u32))(
                (void**)self->mArcAccessor)[0xc / 4])(self->mArcAccessor, 0x74696d67,
                                                       &lbl_eu_80505118[0x15e], 0);
        } else {
            rankTex = ((void* (*)(nw4r::lyt::ArcResourceAccessor*, u32, const char*, u32))(
                (void**)self->mArcAccessor)[0xc / 4])(self->mArcAccessor, 0x74696d67,
                                                       &lbl_eu_80505118[0x176], 0);
        }
        if (rankTex != 0) {
            PaneSetTexPaletteByName(layout, &lbl_eu_80505118[0x18e], (TPLPalette*)rankTex);
        }

        // Affinity rank name text.
        char* rankName;
        switch ((u8)affinity) {
        case 0: rankName = BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x23); break;
        case 1: rankName = BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x1f); break;
        case 2: rankName = BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x20); break;
        case 3: rankName = BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x21); break;
        case 4: rankName = BdatTouchStringCell(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x22); break;
        }
        LayoutSetTextBoxFmtValue(layout, &lbl_eu_80505118[0x19a], rankName, 0);

        char* talkText = BdatTouchStringCell(&lbl_eu_80505118[0x1aa], &lbl_eu_80505118[0x1b8], 0x2c);
        LayoutSetTextBoxFmtValue(layout, &lbl_eu_80505118[0x1bd], talkText, 0);

        // Character portrait texture: name differs per player in co-op.
        s32 player = cf::CfGameManager::isClassicController(-1);
        const char* name = &lbl_eu_80505118[0x1d2];
        if (player != 0) name = &lbl_eu_80505118[0x1c9];
        u16 msgId = BdatGetU16ByTableKey(&lbl_eu_80505118[0x1aa], name, 0x2c);
        char* texName = MakeTplNameSysFile((u32)msgId);
        void* sys = CUICfManager_getArcResourceAccessor();
        KizunaTextureView* tex = (KizunaTextureView*)((void* (*)(void*, u32, void*, u32))(
            (void**)sys)[0xc / 4])(sys, 0x74696d67, texName, 0);
        if (tex != 0) {
            PaneSetTexPaletteByName(layout, &lbl_eu_80505118[0x1db], (TPLPalette*)tex);
            nw4r::lyt::Pane* picPane = layout->GetRootPane();
            u16 w = tex->mHeader->mWidth;
            u16 h = tex->mHeader->mHeight;
            u32 res = ((u32 (*)(nw4r::lyt::Pane*, const char*, u32, void*))(
                (void**)picPane)[0x3c / 4])(picPane, &lbl_eu_80505118[0x1db], 1, tex->mHeader);
            if (res != 0) {
                // Size the picture pane from the texture's u16 dimensions.
                nw4r::lyt::Size size;
                size.width = (f32)w;
                size.height = (f32)h;
                picPane->SetSize(size);
            }
        }

        self->mAnim->SetFrame(lbl_eu_80667E6C);
        self->mLayout->Animate(0);
        self->mFile1 = 0;
        self->mMemRegion.validateHeap();
        return 1;
    }
    if (self->mFile2 == handle) {
        void* data2 = self->mFile2->getData();
        CBdat::setBdatEntry(2, data2);
        Bdat_GetTable_AA34();
        sKizunaBdatTable = (u8*)getFP(&lbl_eu_80505118[0x1e5]);
        self->mFile2 = 0;
        return 1;
    }
    return 0;
}

void CMenuKizunaTalk::OnFileEvent(CEventFile* pEvent) { ((void(*)(void*))handleKizunaFileEvent)((char*)this - 0x6c); }

extern "C" void fwdKizunaDtor6C(void* self) { ((void(*)(void*))__dt__15CMenuKizunaTalkFv)((char*)self - 0x6c); }

extern "C" void fwdKizunaCbRender70(void* self) { ((void(*)(void*))cbRenderBefore__15CMenuKizunaTalkFv)((char*)self - 0x70); }

extern "C" void fwdKizunaDtor70(void* self) { ((void(*)(void*))__dt__15CMenuKizunaTalkFv)((char*)self - 0x70); }
