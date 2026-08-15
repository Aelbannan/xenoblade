// kyoshin/menu/CMenuPause
// Pause menu screen controller (decompiled).

#include "kyoshin/menu/CMenuPause.hpp"

extern "C" void __dt__10CMenuPauseFv(void*, int);
extern "C" void cbRenderBefore__10CMenuPauseFv(void*);

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_drawInfo.h>

extern u32 lbl_eu_80663E28;

// Forward declaration for the pause-exit input handler (defined below). Retail
// emits this under C linkage (`func_80252564`), so it must stay unmangled.
extern "C" void func_80252564(CMenuPause* p);

void CMenuPause::Init() {
    // u16 -> f32 conversion unions; the 0x43300000 high words are written up
    // front so MWCC hoists the constant stores into the prologue. The
    // subtraction is done in single precision so MWCC emits fsubs (the
    // retail shape) instead of a double-precision fsub.
    CMenuPauseF64Conv convH, convW;
    convW.w[0] = 0x43300000;
    convH.w[0] = 0x43300000;

    func_8008294C__Q22cf13CfGameManagerFv(true);
    func_80188890(1);

    // Scoped MEM2 region guard, then build the layout + animations.
    mMemRegion.createRegion((int)mtl::MemManager::getHandleMEM2(), 0x2000,
                            lbl_eu_8050C5C8, 0);
    u8 regionBuf[4];
    __ct__14Class_8045F858FP17UnkClass_8045F564(regionBuf, &mMemRegion);
    mtl::MemManager::func_80434A4C(false);

    func_80136E84(&mLayout, func_801355F4(), &lbl_eu_8050C5C8[0xb]);
    func_80136F08(mLayout,
                  reinterpret_cast<nw4r::lyt::AnimTransform**>(&mField80),
                  func_801355F4(), &lbl_eu_8050C5C8[0x24]);

    // Bind the font and hand the loaded font object over to the root pane.
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    void* fontObj = CDeviceFont::func_80452C10(1, mLayout);
    u32 fontResult = ((CMenuPauseFontView*)fontObj)->sf9();
    func_8013676C(rootPane, fontResult);

    mLayout->SetAnimationEnable(
        reinterpret_cast<nw4r::lyt::AnimTransform*>(mField80), true);
    mLayout->Animate(0);

    // Fill the two message-table text fields.
    func_80136B4C(mLayout, &lbl_eu_8050C5C8[0x50],
                  func_80136190(&lbl_eu_8050C5C8[0x40], &lbl_eu_8050C5C8[0x4b],
                                8),
                  0);
    func_80136B4C(mLayout, &lbl_eu_8050C5C8[0x70],
                  func_80136190(&lbl_eu_8050C5C8[0x5d], &lbl_eu_8050C5C8[0x6b],
                                0x6b),
                  0);

    // ---- Cursor texture palette variants (classic vs remote pad). ----
    // Each: pick a message name by pad type, load the "timg" texture
    // resource, bind it (func_80137E7C) and set the pane size from the
    // texture's 2D dimension header (u16 w/h converted via the 2^52 trick).
    {
        const char* msgName = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                                  ? &lbl_eu_8050C5C8[0x7d]
                                  : &lbl_eu_8050C5C8[0x86];
        u16 msg = func_8013606C(&lbl_eu_8050C5C8[0x5d], msgName, 0x6b);
        char* handle = func_80138F78(msg);
        nw4r::lyt::ArcResourceAccessor* acc = func_801355F4();
        void* tex = acc->GetResource(0x74696D67, handle, NULL);
        if (tex != NULL) {
            func_80137E7C(mLayout, (void*)&lbl_eu_8050C5C8[0x8f], tex);
            CMenuPauseTexDims* dims =
                ((CMenuPauseTexObj*)tex)->mChain->mDims;
            u16 v2 = dims->field_0x2;
            u16 v0 = dims->field_0x0;
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()
                                         ->FindPaneByName(&lbl_eu_8050C5C8[0x8f],
                                                          true);
            if (pane != NULL) {
                convW.w[1] = v2;
                convH.w[1] = v0;
                nw4r::lyt::Size size;
                size.width = (f32)(convW.d - lbl_eu_806687D0);
                size.height = (f32)(convH.d - lbl_eu_806687D0);
                pane->SetSize(size);
            }
        }
    }
    {
        const char* msgName = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                                  ? &lbl_eu_8050C5C8[0x7d]
                                  : &lbl_eu_8050C5C8[0x86];
        u16 msg = func_8013606C(&lbl_eu_8050C5C8[0x5d], msgName, 0x6c);
        char* handle = func_80138F78(msg);
        nw4r::lyt::ArcResourceAccessor* acc = func_801355F4();
        void* tex = acc->GetResource(0x74696D67, handle, NULL);
        if (tex != NULL) {
            func_80137E7C(mLayout, (void*)&lbl_eu_8050C5C8[0xa6], tex);
            CMenuPauseTexDims* dims =
                ((CMenuPauseTexObj*)tex)->mChain->mDims;
            u16 v2 = dims->field_0x2;
            u16 v0 = dims->field_0x0;
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()
                                         ->FindPaneByName(&lbl_eu_8050C5C8[0xa6],
                                                          true);
            if (pane != NULL) {
                convW.w[1] = v2;
                convH.w[1] = v0;
                nw4r::lyt::Size size;
                size.width = (f32)(convW.d - lbl_eu_806687D0);
                size.height = (f32)(convH.d - lbl_eu_806687D0);
                pane->SetSize(size);
            }
        }
    }

    mState = 1;
    func_80138078__FUl(0xd);

    mField74[0] = mField74[1] = mField74[2] = mField74[3] = 0; // field 0x74

    // Register as a scene render callback; IScnRender subobject lives at +0x5c.
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&mRenderVtable);
    }
    mScene->addRenderCB(cb, 0x12, 0);

    mMemRegion.func_8045F810();
    // regionGuard destructor runs here.
    __dt__14Class_8045F858Fv(regionBuf, -1);
}


void CMenuPause::Term() {
    CDeviceVI::waitForDrawDone();

    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = reinterpret_cast<IScnRender*>(&mRenderVtable);
    }
    mScene->removeRenderCB(renderCB);

    if (mLayout != NULL) {
        delete mLayout;
        mLayout = NULL;
    }

    mMemRegion.func_8045F778();

    lbl_eu_806647C8 = NULL;
    func_80188890(0);
    func_8008294C__Q22cf13CfGameManagerFv(false);
}

void CMenuPause::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) goto exit;
    if ((lbl_eu_80663E28 & (1u << 21)) != 0) goto exit;
    // Branch-over-branch guard: `goto body` with the `exit` label + return
    // placed BEFORE `body` keeps MWCC from folding the bit test to a single
    // `bne` -- it emits retail's `beq body; b exit` (docs/MWCC_REFERENCE.md
    // 5824 / 4707).
    goto body;
exit:
    return;
body:
    if (!func_8013BE50()) {
        goto exit;
    }
    if (mLayout == NULL) {
        goto exit;
    }

    switch (mState) {
    case 1:
        // Opening animation: tick it forward; when it finishes arms state 2.
        if (func_80137444(reinterpret_cast<nw4r::lyt::AnimTransform*>(mField80),
                          lbl_eu_806687D8) != 0) {
            mState = 2;
        }
        break;
    case 2:
        // Wait for a cancel/confirm input to leave the pause menu.
        func_80252564(this);
        break;
    case 3:
        // Closing animation: when it finishes, reset to state 0 / clear flag.
        if (func_80137510(reinterpret_cast<nw4r::lyt::AnimTransform*>(mField80),
                          lbl_eu_806687D8) != 0) {
            mState = 0;
            mBProcess[0x50] = 1; // field 0x54 flag
        }
        break;
    }

    // Pump the layout animation once.
    mLayout->Animate(0);
}

void CMenuPause::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    if (lbl_eu_80663E28 & (1u << 21)) {
        goto done;
    }
    if (!func_8013BE50()) {
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        if (mLayout != NULL) {
            func_80137038(mLayout, &drawInfo, 0, 1);
        }
    }
done:
    ;
}

// Raw byte shim over the embedded CProcess region + CMenuPause fields so the
// factory constructor can fill the fixed-offset slots (vtable +0x10, PTMF
// callbacks +0x3C..+0x53, flags +0x54/+0x55, subobject vtables) without pointer
// arithmetic. Mirrors CProcess's layout (vtable 0x10 .. flags 0x55).
struct CMenuPauseCtorShim {
    u8 _00[0x10];
    void* vtable;          // 0x10 -- CProcess vtable, overwritten by final vtable
    u8 _14[0x28];          // 0x14-0x3B
    u32 callbacks[6];      // 0x3C-0x53 -- PTMF callback slots (__ptmf_null copies)
    u8 field54;            // 0x54
    u8 field55;            // 0x55
    void* iscnVtable1;     // 0x58 -- IScnRender subobject vtable
    void* iscnVtable2;     // 0x5c -- IScnRender subobject vtable
    CScn* mScene;          // 0x60
    UnkClass_8045F564 mMemRegion; // 0x64 (0x10 bytes)
    u8 _74[0x7c - 0x74];   // 0x74-0x7b
    void* mLayout;         // 0x7c
    u32 mField80;          // 0x80
    u8 mState;             // 0x84
};

// Singleton-factory constructor: only the first allocation sticks (subsequent
// calls return NULL). Reuses the proven CMenuMapSelect factory register
// scheduling: CProcess ctor, PTMF callback block, flags, final vtable, the two
// IScnRender subobject vtables, mScene, then the memory-region ctor and zeros.
extern "C" CMenuPause* __ct__CMenuPause(CProcess* parent, CProcess* parent2) {
    if (lbl_eu_806647C8 != 0) {
        return 0;
    }

    u32 handle = getWorkMem__17CWorkThreadSystemFv();
    CMenuPauseCtorShim* shim =
        (CMenuPauseCtorShim*)allocate__Q23mtl10MemManagerFUlUl(0x88, handle);

    if (shim != 0) {
        __ct__8CProcessFv((void*)shim);
        shim->vtable = lbl_eu_8052BF70;

        u32* ptmf = __ptmf_null;
        char* vtFinal = lbl_eu_805371A0;
        // Persistent zero used for all the flag/member zero-writes below; keeping
        // it live across the sub-object ctor makes MWCC allocate a callee-saved
        // register (retail r31) and matches the 4-register prologue.
        u8 zero = 0;

        u32 ptmf1 = ptmf[1];
        u32 ptmf0 = ptmf[0];
        char* iscnVtbl1 = vtFinal + 0x24;
        char* iscnVtbl2 = vtFinal + 0xac;
        shim->callbacks[0] = ptmf0;
        shim->callbacks[1] = ptmf1;

        u32 ptmf2 = ptmf[2];
        shim->callbacks[2] = ptmf2;

        ptmf1 = ptmf[1];
        ptmf0 = ptmf[0];
        shim->callbacks[3] = ptmf0;
        shim->callbacks[4] = ptmf1;
        ptmf2 = ptmf[2];
        shim->callbacks[5] = ptmf2;

        shim->field54 = zero;
        shim->field55 = zero;

        shim->vtable = vtFinal;
        shim->iscnVtable1 = iscnVtbl1;
        shim->iscnVtable2 = iscnVtbl2;
        shim->mScene = (CScn*)parent2;

        __ct__17UnkClass_8045F564Fv(&shim->mMemRegion);

        shim->mLayout = 0;
        shim->mField80 = 0;
        shim->mState = 0;
    }

    lbl_eu_806647C8 = (CMenuPause*)shim;
    Regist__8CProcessFP8CProcessb((void*)shim, parent, 0);
    return lbl_eu_806647C8;
}

unsigned long CMenuPause::isInitialized() {
    return lbl_eu_806647C8 != NULL;
}

CMenuPause::~CMenuPause() {
    if (this != NULL) {
        mMemRegion.~UnkClass_8045F564();
        if (this != NULL) {
            __dt__8CProcessFv((void*)this, 0);
        }
    }
}

unsigned long func_80252538() {
    CMenuPause* p = lbl_eu_806647C8;
    if (p == NULL) {
        return 0;
    }
    return p->mState != 3;
}

extern "C" void func_80252564(CMenuPause* p) {
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 first;
    u32 second;
    if (cf::CfGameManager::func_80086F9C(-1) != 0) {
        first = (pad->mPressedButtonFlags >> 21) & 1;
        second = (pad->mPressedButtonFlags >> 22) & 1;
    } else {
        first = (pad->mPressedButtonFlags >> 4) & 1;
        second = (pad->mPressedButtonFlags >> 5) & 1;
    }
    if (first) {
        func_eu_8013C8E8();
        func_800853C8__Q22cf13CfGameManagerFv();
        func_801BFB34(lbl_eu_806687DC, 0, 3, 0);
        func_801BFB34(lbl_eu_806687DC, 0, 0xe, 0);
        p->mState = 3;
    } else if (second) {
        func_80138078__FUl(6);
        func_80138078__FUl(0xe);
        p->mState = 3;
    }
}

// IScnRender / IWorkEvent adjuster thunks (subobjects embedded in CMenuPause).
void func_80252628(void* self) { ((void(*)(void*))__dt__10CMenuPauseFv)((char*)self - 0x58); }
void func_80252630(void* self) { ((void(*)(void*))cbRenderBefore__10CMenuPauseFv)((char*)self - 0x5c); }
void func_80252638(void* self) { ((void(*)(void*))__dt__10CMenuPauseFv)((char*)self - 0x5c); }