// kyoshin/menu/CMenuPause
// Pause menu screen controller (decompiled).

#include "kyoshin/menu/CMenuPause.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_drawInfo.h>

extern u32 lbl_eu_80663E28;

// Forward declaration for the pause-exit input handler (defined below).
void func_80252564(CMenuPause* p);

void CMenuPause::Init() {}

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
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    if (lbl_eu_80663E28 & (1u << 21)) {
        goto done;
    }
    if (!func_8013BE50()) {
        goto done;
    }
    if (mLayout == NULL) {
        goto done;
    }

    switch (mState) {
    case 1:
        // Opening animation: tick it forward; when it finishes arms state 2.
        if (func_80137444(reinterpret_cast<nw4r::lyt::AnimTransform*>(mField80),
                          1.0f) != 0) {
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
                          1.0f) != 0) {
            mState = 0;
            mBProcess[0x50] = 1; // field 0x54 flag
        }
        break;
    }

    // Pump the layout animation once.
    void* vtab = *(void**)mLayout;
    ((void (*)(void*, bool))((void**)vtab)[0x38 / 4])(mLayout, 0);
done:
    ;
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

        u32 ptmf1 = ptmf[1];
        u32 ptmf0 = ptmf[0];
        char* iscnVtbl1 = vtFinal + 0x24;
        shim->callbacks[0] = ptmf0;
        shim->callbacks[1] = ptmf1;

        u32 ptmf2 = ptmf[2];
        char* iscnVtbl2 = vtFinal + 0xac;
        shim->callbacks[2] = ptmf2;

        ptmf1 = ptmf[1];
        ptmf0 = ptmf[0];
        shim->callbacks[3] = ptmf0;
        shim->callbacks[4] = ptmf1;
        ptmf2 = ptmf[2];
        shim->callbacks[5] = ptmf2;

        shim->field54 = 0;
        shim->field55 = 0;

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

void func_80252564(CMenuPause* p) {
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
void func_80252628(void* self) { ((CMenuPause*)((char*)self - 0x58))->~CMenuPause(); }
void func_80252630(void* self) { ((CMenuPause*)((char*)self - 0x5c))->cbRenderBefore(); }
void func_80252638(void* self) { ((CMenuPause*)((char*)self - 0x5c))->~CMenuPause(); }