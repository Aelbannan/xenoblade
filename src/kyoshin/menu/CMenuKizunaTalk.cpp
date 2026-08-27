// Decomp of kyoshin/menu/CMenuKizunaTalk.
// Kizuna (friendship) talk screen: singleton process with IUIWindow +
// IWorkEvent + IScnRender bases, an embedded CCur18 cursor and CSysWin panel.

#include "kyoshin/menu/CMenuKizunaTalk.hpp"

#include "functions.hpp"

#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/work/CEventFile.hpp"
#include <nw4r/lyt.h>

extern "C" void __dt__15CMenuKizunaTalkFv(void*, int);
extern "C" void cbRenderBefore__15CMenuKizunaTalkFv(void*);

// Defined later in this TU (retail symbol is unmangled); noinline keeps
// MWCC from inlining the body into func_801BCF48.
extern "C" void func_801BD630(CMenuKizunaTalk* self);

// ---------------------------------------------------------------------------
// Constructor (retail symbol is the unmangled __ct__CMenuKizunaTalk, so the
// body is written as a C-linkage free function; the factory func_801BCEBC
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
    static_cast<IWorkEvent*>(obj)->mVtable = workVt;
    static_cast<IScnRender*>(obj)->mVtable = renderVt;
    obj->mScene = scene;

    __ct__17UnkClass_8045F564Fv((UnkClass_8045F564*)&obj->mMemRegion[0]);
    obj->mFile1 = 0;
    obj->mFile2 = 0;
    obj->mArcAccessor = 0;
    obj->mAnim = 0;
    obj->mCharId = charId;
    obj->mField9C = 1;
    obj->mFieldA0 = 0;
    obj->mFieldA4 = 0;

    __ct__CCur18(&obj->mCur[0], 0);
    return obj;
}

// ---------------------------------------------------------------------------
// Destructor: the CSysWin / CCur18 / UnkClass_8045F564 regions are raw
// storage whose retail dtors are C-linkage free functions, so they are
// destroyed by hand in reverse construction order; the IUIWindow base dtor
// (__dt__9IUIWindowFv) is emitted automatically after the body.
// ---------------------------------------------------------------------------
#pragma optimize_for_size on
CMenuKizunaTalk::~CMenuKizunaTalk() {
    __dt__7CSysWinFv(&mSysWin[0], -1);
    __dt__6CCur18Fv(&mCur[0], -1);
    __dt__17UnkClass_8045F564Fv(&mMemRegion[0], -1);
}
#pragma optimize_for_size off

// Initialise the talk screen: request the shared archive (the loader gets the
// IWorkEvent subobject at +0x6C via the base conversion), then set the
// character's affinity-talk-seen BDAT flag.
#pragma optimize_for_size on
void CMenuKizunaTalk::Init() {
    mFile2 = (CFileHandle*)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        func_800A9D90(), lbl_eu_80505118, static_cast<IWorkEvent*>(this), 0, 0);
    func_8009D018(mCharId + 0x3440, 1);
}
#pragma optimize_for_size off

// Tear the talk screen down: release the file handles and the layout archive,
// remove the render callback, run the embedded widget teardowns and clear the
// singleton / BDAT globals.
#pragma optimize_for_size on
void CMenuKizunaTalk::Term() {
    waitForDrawDone__9CDeviceVIFv();
    getEntry__5CBdatFUl(2);
    closeFileHandle__FPP11CFileHandle(reinterpret_cast<void**>(&mFile1));
    closeFileHandle__FPP11CFileHandle(reinterpret_cast<void**>(&mFile2));

    mScene->removeRenderCB(this);

    if (mLayout != 0) {
        // Layout virtual slot 2 (vtable + 0x8): release/unbind with flag 1.
        // Retail emits the null guard twice (outer block guard + inner call
        // guard), so the source nests two identical checks.
        if (mLayout != 0) {
            reinterpret_cast<CLayoutView*>(mLayout)->vf02(1);
        }
        mLayout = 0;
    }

    releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(mArcAccessor);
    mArcAccessor = 0;
    func_8022B7F4(&mSysWin[0]);

    // CCur18 cursor virtual slot 3 (vtable + 0xC): per-frame update.
    reinterpret_cast<CCur18View*>(&mCur[0])->vf03();

    lbl_eu_80664424 = 0;
    deleteRegion__17UnkClass_8045F564Fv(&mMemRegion[0]);
    lbl_eu_80664420 = 0;
}
#pragma optimize_for_size off

void CMenuKizunaTalk::Move() {}

void CMenuKizunaTalk::cbRenderBefore() {}

// ---------------------------------------------------------------------------
// Factory: lazily allocate + construct the single kizuna talk instance from
// work memory and register it under `parent`. Returns 0 when the singleton
// already exists.
// ---------------------------------------------------------------------------
#pragma optimize_for_size on
CMenuKizunaTalk* func_801BCEBC(CProcess* parent, CScn* scene, u32 charId) {
    if (lbl_eu_80664420 != 0) {
        return 0;
    }
    mtl::ALLOC_HANDLE mem = CWorkThreadSystem::getWorkMem();
    CMenuKizunaTalk* obj =
        (CMenuKizunaTalk*)mtl::MemManager::allocate(0xfc, mem);
    if (obj != 0) {
        obj = __ct__CMenuKizunaTalk(obj, scene, charId);
    }
    lbl_eu_80664420 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, false);
    return lbl_eu_80664420;
}
#pragma optimize_for_size off

// (lbl_eu_80664420 != 0) - needs the size-optimized subic/subfe form; the
// preceding `#pragma optimize_for_size off` would revert to -O4,p codegen
#pragma optimize_for_size on
extern "C" bool func_801BCF38() { return lbl_eu_80664420 != 0; }
#pragma optimize_for_size off

// ---------------------------------------------------------------------------
// Window-state kickoff (retail func_801BCF48): refresh the availability gate
// via func_801BD630, then per window state: 0/2 rebuild the CSysWin panel from
// a stack temp (directions 0 / 3) and dispatch the layout-build virtual at
// vtable +0x88; state 2 also seeds the cursor from a CCur18 temp. State 1
// requests the talk-list file through the IWorkEvent subobject (+0x6C,
// null-this-safe).
// ---------------------------------------------------------------------------
#pragma optimize_for_size on
void func_801BCF48(CMenuKizunaTalk* self) {
    if (lbl_eu_80664424 == 0) {
        return;
    }
    self->mFieldA0 = 1;
    func_801BD630(self);
    // Shared CSysWin scratch plus a full CCur18-sized cursor scratch.
    KizunaScratch scratch;
    switch (self->mFieldA4) {
    case 0:
        __ct__CSysWin(&scratch.mWin0[0], 0);
        func_8016742C(&self->mSysWin[0], &scratch.mWin0[0]);
        __dt__7CSysWinFv(&scratch.mWin0[0], -1);
        reinterpret_cast<KizunaSysWinView*>(&self->mSysWin[0])->v20();
        break;
    case 1: {
        // Null-safe IWorkEvent subobject address (retail beq/addi shape).
        IWorkEvent* evt = reinterpret_cast<IWorkEvent*>(self);
        if (self != 0) {
            evt = &static_cast<IWorkEvent&>(*self);
        }
        self->mFile1 =
            (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
                (u32)getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_80505118[0x1c],
                evt, 0, 0);
        break;
    }
    case 2: {
        __ct__CSysWin(&scratch.mWin2[0], 3);
        func_8016742C(&self->mSysWin[0], &scratch.mWin2[0]);
        __dt__7CSysWinFv(&scratch.mWin2[0], -1);
        reinterpret_cast<KizunaSysWinView*>(&self->mSysWin[0])->v20();

        __ct__CCur18(&scratch.mCur[0], func_801355F4());
        KizunaCurFields* dst = reinterpret_cast<KizunaCurFields*>(&self->mCur[0]);
        KizunaCurFields* src = reinterpret_cast<KizunaCurFields*>(&scratch.mCur[0]);
        dst->f_04 = src->f_04;
        dst->f_08 = src->f_08;
        dst->f_0c = src->f_0c;
        dst->f_10 = src->f_10;
        // Byte-sized page fields copied individually (retail lbz/stb pairs).
        dst->f_14 = src->f_14;
        dst->f_15 = src->f_15;
        __dt__6CCur18Fv(&scratch.mCur[0], -1);
        reinterpret_cast<CCur18View*>(&self->mCur[0])->vf02();
        break;
    }
    }
}
#pragma optimize_for_size off

// ---------------------------------------------------------------------------
// Kizuna talk window advance (retail func_801BD0B4): state machine on
// mFieldA4. State 0 builds the "talk" window, state 1 waits for the layout
// then plays the open sound, state 2 builds the "leave" window with the
// character name; every successful state sets mFieldA0 and (re)registers the
// render callback.
// ---------------------------------------------------------------------------
void func_801BD0B4(CMenuKizunaTalk* self) {
    switch (self->mFieldA4) {
    case 0:
        if (CSysWin_isReady(&self->mSysWin[0]) == 0) {
            return;
        }
        func_8022B9B4(&self->mSysWin[0],
                      (u32)func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x24), 0);
        func_8022BFC8(&self->mSysWin[0], 1);
        func_8022B8B8(&self->mSysWin[0]);
        break;
    case 1:
        if (self->mLayout == 0) {
            return;
        }
        playUISound__FUl(0xd);
        break;
    case 2:
        if (CSysWin_isReady(&self->mSysWin[0]) == 0) {
            return;
        }
        char* s1 = func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x2a);
        char* s2 = func_8013639C(lbl_eu_80664424, &lbl_eu_80505118[0x43], self->mCharId);
        char* s3 = func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x2c);
        char* s4 = func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x2d);
        func_8022B9B4(&self->mSysWin[0], (u32)s1, (int)s2);
        func_8022BF6C(&self->mSysWin[0], (u32)s3, (u32)s4);
        func_8022BFC8(&self->mSysWin[0], 0);
        func_8022B8B8(&self->mSysWin[0]);
        break;
    }
    self->mFieldA0 = 2;
    self->mScene->addRenderCB(self, 0xd, 0);
}

// ---------------------------------------------------------------------------
// Kizuna talk window advance (retail func_801BD228): state machine on
// mFieldA4. State 0/2 wait for the CSysWin panel to become active; state 1
// waits for the layout animation to reach its target frame. State 2 also
// feeds the panel's current page into the cursor's selection update.
// ---------------------------------------------------------------------------
void func_801BD228(CMenuKizunaTalk* self) {
    switch (self->mFieldA4) {
    case 0:
        if (CSysWin_isActive(&self->mSysWin[0]) != 0) {
            self->mFieldA0 = 3;
        }
        break;
    case 1:
        if (advanceAnimTransform(self->mAnim, lbl_eu_80667E68) != 0) {
            self->mFieldA0 = 3;
        }
        break;
    case 2:
        if (CSysWin_isActive(&self->mSysWin[0]) != 0) {
            self->mFieldA0 = 3;
            func_801D216C(&self->mCur[0], 1);
            u8 tmp[0x10];
            func_8022C1B4(tmp, &self->mSysWin[0], (u8)self->mField9C);
            reinterpret_cast<CCur18View*>(&self->mCur[0])->vf04(tmp);
        }
        break;
    }
}

// ---------------------------------------------------------------------------
// Kizuna talk window-state advance (retail func_801BD2F8): reads the cf pad
// and drives the mFieldA4 window state machine. States 0/1 wait for the left/
// right D-pad bits, state 2 handles cancel/confirm page navigation and the
// confirm/advance actions. The D-pad bit position differs between player 0
// (co-op) and the others.
// ---------------------------------------------------------------------------
#pragma optimize_for_size on
void func_801BD2F8(CMenuKizunaTalk* self) {
    KizunaTalkPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    u32 cancel, confirm, left, right;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
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
            playUISound__FUl(3);
            self->mFieldA0 = 4;
            func_8022B8E4(&self->mSysWin[0]);
        } else if (right != 0) {
            self->mField60 = 2;
            self->mFieldA0 = 4;
            playUISound__FUl(6);
            func_8022B8E4(&self->mSysWin[0]);
        }
        break;
    case 1:
        if (left != 0) {
            self->mField60 = 2;
            playUISound__FUl(3);
            self->mFieldA0 = 4;
        } else if (right != 0) {
            self->mField60 = 2;
            self->mFieldA0 = 4;
            playUISound__FUl(6);
        }
        break;
    case 2:
        if (cancel != 0) {
            // Cancel: move the cursor page up (wrap below 0 to 1).
            self->mField9C = (s8)(self->mField9C - 1);
            if (self->mField9C < 0) self->mField9C = 1;
            playUISound__FUl(1);
            u8 tmp[0xC];
            func_8022C1B4(tmp, &self->mSysWin[0], (u8)self->mField9C);
            reinterpret_cast<CCur18View*>(&self->mCur[0])->vf04(tmp);
        } else if (confirm != 0) {
            // Confirm: move the cursor page down (wrap over 1 to 0).
            self->mField9C = (s8)(self->mField9C + 1);
            if (self->mField9C > 1) self->mField9C = 0;
            playUISound__FUl(1);
            u8 tmp[0xC];
            func_8022C1B4(tmp, &self->mSysWin[0], (u8)self->mField9C);
            reinterpret_cast<CCur18View*>(&self->mCur[0])->vf04(tmp);
        } else if (left != 0) {
            // Left: confirm the current page and advance the window.
            s8 page = self->mField9C;
            self->mField60 = page + 1;
            if (page == 0) {
                playUISound__FUl(0x37);
            } else {
                playUISound__FUl(3);
            }
            self->mFieldA0 = 4;
            func_8022B8E4(&self->mSysWin[0]);
            func_801D216C(&self->mCur[0], 0);
        } else if (right != 0) {
            self->mField60 = 2;
            self->mFieldA0 = 4;
            playUISound__FUl(6);
            func_8022B8E4(&self->mSysWin[0]);
            func_801D216C(&self->mCur[0], 0);
        }
        break;
    }
}
#pragma optimize_for_size off

// Advance the kizuna talk window state machine (retail func_801BD594):
// window states 0 and 2 wait for the CSysWin panel to become active, state 1
// waits for the embedded animation transform to finish; each sets the
// IUIWindow "advance" flag when its wait completes.
#pragma optimize_for_size on
void func_801BD594(CMenuKizunaTalk* self) {
    switch (self->mFieldA4) {
    case 0:
        if (CSysWin_isActive(&self->mSysWin[0]) != 0) {
            self->mField64 = 1;
        }
        break;
    case 1:
        if (func_80137510(self->mAnim, lbl_eu_80667E68) != 0) {
            self->mField64 = 1;
        }
        break;
    case 2:
        if (CSysWin_isActive(&self->mSysWin[0]) != 0) {
            self->mField64 = 1;
        }
        break;
    }
}
#pragma optimize_for_size off

// ---------------------------------------------------------------------------
// Kizuna talk gating check (retail func_801BD630): reads the character's
// affinity-talk columns and decides whether the talk screen is available;
// writes the resulting mFieldA4 gate state.
// ---------------------------------------------------------------------------
// Retail symbol is the unmangled func_801BD630.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_801BD630(CMenuKizunaTalk* self) {
    u16 total = func_80136254(lbl_eu_80664424, &lbl_eu_80505118[0x49], self->mCharId);
    if ((u32)(u16)total <= func_8009CF8C((u32)0x20)) {
        u32 b1 = 0, b2 = 0, b3 = 0, b4 = 0;
        u32 talk = func_801361E8((u32)lbl_eu_80664424, &lbl_eu_80505118[0x4e], self->mCharId);
        u32 talk2 = func_801361E8((u32)lbl_eu_80664424, &lbl_eu_80505118[0x57], self->mCharId);
        u16 value = func_80136254(lbl_eu_80664424, &lbl_eu_80505118[0x60], self->mCharId);
        u32 rank = func_801361E8((u32)lbl_eu_80664424, &lbl_eu_80505118[0x69], self->mCharId);
        if (func_8009CF8C((u32)0x20) >= 0x113) {
            if ((u8)talk == 3) talk = 8;
            if ((u8)talk2 == 3) talk2 = 8;
        }
        int* list = func_8009ECB0();
        for (u32 i = 0; i < 3; i++) {
            u32 v = (u32)list[i + 1];
            if ((u8)talk == v) {
                b1 = 1;
            } else if ((u8)talk2 == v) {
                b2 = 1;
            }
        }
        if ((u16)value <= (u16)func_8013A7D0((u8)talk, (u8)talk2)) b3 = 1;
        u8 cur = (u8)func_8006A6D0();
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
#pragma optimize_for_size off

// ---------------------------------------------------------------------------
// Talk list file-event handler (retail func_801BD7D8): when the requested
// talk-list archive finishes loading it builds the layout, fills in the
// character/rank text and loads the character portrait texture; when the
// shared BDAT archive finishes it reloads the character table pointer.
// ---------------------------------------------------------------------------
int func_801BD7D8(CMenuKizunaTalk* self, CEventFile* evt) {
    CFileHandle* handle = evt->mFileHandle;
    if (self->mFile1 == handle) {
        void* mem2 = getHandleMEM2__Q23mtl10MemManagerFv();
        ((UnkClass_8045F564*)&self->mMemRegion[0])->createRegion(
            (int)(u32)mem2, 0x2000, &lbl_eu_80505118[0x6e], 0);
        Class_8045F858 regionHandle((UnkClass_8045F564*)&self->mMemRegion[0]);
        void* data = self->mFile1->getData();
        setMemInitFlag__Q23mtl10MemManagerFb(false);
        nw4r::lyt::ArcResourceAccessor* acc = createArcResourceAccessor__10CLibLayoutFv();
        self->mArcAccessor = acc;
        Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(acc, data, &lbl_eu_80505118[0x7e]);
        buildLayout__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
            &self->mLayout, acc, &lbl_eu_80505118[0x82]);
        bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            self->mLayout, &self->mAnim, acc, &lbl_eu_80505118[0x9b]);

        nw4r::lyt::Layout* layout = self->mLayout;
        nw4r::lyt::Pane* rootPane = layout->GetRootPane();
        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, layout);
        u32 fontVal = ((u32 (*)(void*))((void**)fontObj)[0x24 / 4])(fontObj);
        func_8013676C(rootPane, fontVal);

        u32 charName = func_801361E8((u32)lbl_eu_80664424, &lbl_eu_80505118[0x4e], self->mCharId);
        u32 charName2 = func_801361E8((u32)lbl_eu_80664424, &lbl_eu_80505118[0x57], self->mCharId);
        u16 rank = func_80136254(lbl_eu_80664424, &lbl_eu_80505118[0x60], self->mCharId);
        u32 affinity = func_801361E8((u32)lbl_eu_80664424, &lbl_eu_80505118[0x69], self->mCharId);

        func_80136B4C(layout, &lbl_eu_80505118[0xb7],
                      func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x25), 0);
        func_80136B4C(layout, &lbl_eu_80505118[0xc4],
                      func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x26), 0);
        func_80136B4C(layout, &lbl_eu_80505118[0xd1],
                      func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x28), 0);
        func_80136B4C(layout, &lbl_eu_80505118[0xde],
                      func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x27), 0);
        func_80136B4C(layout, &lbl_eu_80505118[0xf6],
                      func_80136190(&lbl_eu_80505118[0xeb], &lbl_eu_80505118[0x3e], (u8)charName), 0);
        func_80136B4C(layout, &lbl_eu_80505118[0x106],
                      func_80136190(&lbl_eu_80505118[0xeb], &lbl_eu_80505118[0x3e], (u8)charName2), 0);

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
            func_80137E7C(layout, &lbl_eu_80505118[0x18e], rankTex);
        }

        // Affinity rank name text.
        char* rankName;
        switch ((u8)affinity) {
        case 0: rankName = func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x23); break;
        case 1: rankName = func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x1f); break;
        case 2: rankName = func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x20); break;
        case 3: rankName = func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x21); break;
        case 4: rankName = func_80136190(&lbl_eu_80505118[0x33], &lbl_eu_80505118[0x3e], 0x22); break;
        }
        func_80136B4C(layout, &lbl_eu_80505118[0x19a], rankName, 0);

        char* talkText = func_80136190(&lbl_eu_80505118[0x1aa], &lbl_eu_80505118[0x1b8], 0x2c);
        func_80136B4C(layout, &lbl_eu_80505118[0x1bd], talkText, 0);

        // Character portrait texture: name differs per player in co-op.
        s32 player = isClassicController__Q22cf13CfGameManagerFv(-1);
        const char* name = &lbl_eu_80505118[0x1d2];
        if (player != 0) name = &lbl_eu_80505118[0x1c9];
        u16 msgId = func_8013606C(&lbl_eu_80505118[0x1aa], name, 0x2c);
        char* texName = func_80138F78((u32)msgId);
        void* sys = func_801355F4();
        KizunaTextureView* tex = (KizunaTextureView*)((void* (*)(void*, u32, void*, u32))(
            (void**)sys)[0xc / 4])(sys, 0x74696d67, texName, 0);
        if (tex != 0) {
            func_80137E7C(layout, &lbl_eu_80505118[0x1db], tex);
            nw4r::lyt::Pane* picPane = layout->GetRootPane();
            u16 w = tex->mHeader->mWidth;
            u16 h = tex->mHeader->mHeight;
            u32 res = ((u32 (*)(nw4r::lyt::Pane*, const char*, u32, void*))(
                (void**)picPane)[0x3c / 4])(picPane, &lbl_eu_80505118[0x1db], 1, tex->mHeader);
            if (res != 0) {
                // Convert the u16 texture dimensions to float via the shared
                // 0x43300000 magic double and size the picture pane.
                KizunaF64Conv cw, ch;
                cw.w[1] = (u32)w;
                cw.w[0] = 0x43300000;
                ch.w[1] = (u32)h;
                ch.w[0] = 0x43300000;
                nw4r::lyt::Size size;
                size.width = (f32)(cw.d - lbl_eu_80667E70);
                size.height = (f32)(ch.d - lbl_eu_80667E70);
                picPane->SetSize(size);
            }
        }

        self->mAnim->SetFrame(lbl_eu_80667E6C);
        self->mLayout->Animate(0);
        self->mFile1 = 0;
        ((UnkClass_8045F564*)&self->mMemRegion[0])->func_8045F810();
        return 1;
    }
    if (self->mFile2 == handle) {
        void* data2 = self->mFile2->getData();
        setBdatEntry__5CBdatFUlPv(2, data2);
        func_8003AA34();
        lbl_eu_80664424 = (u8*)getFP__FPCc(&lbl_eu_80505118[0x1e5]);
        self->mFile2 = 0;
        return 1;
    }
    return 0;
}

void OnFileEvent__15CMenuKizunaTalkFP10CEventFile(void* self) { ((void(*)(void*))func_801BD7D8)((char*)self - 0x6c); }

void func_801BDD8C(void* self) { ((void(*)(void*))__dt__15CMenuKizunaTalkFv)((char*)self - 0x6c); }

void func_801BDD94(void* self) { ((void(*)(void*))cbRenderBefore__15CMenuKizunaTalkFv)((char*)self - 0x70); }

extern "C" void func_801BDD9C(void* self) { ((void(*)(void*))__dt__15CMenuKizunaTalkFv)((char*)self - 0x70); }
