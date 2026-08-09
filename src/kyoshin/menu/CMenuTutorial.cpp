// Decomp of kyoshin/menu/CMenuTutorial.
// Tutorial menu screen process (CProcess + IScnRender MI).

#include "kyoshin/menu/CMenuTutorial.hpp"

#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CCollepedia.hpp"
#include "monolib/core/CPadManager.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_drawInfo.h>

extern unsigned long lbl_eu_80664A28;
extern u32 lbl_eu_80663E28;
extern u32 lbl_eu_80663E24;
extern char lbl_eu_80510260[];

/*
 * Retail-unmangled C-linkage callees in other TUs.
 *
 * The tutorial widget (CTutorial) and title/help bar (CTitleAHelp) functions
 * are emitted by retail under their bare names (no Itanium mangling), as are
 * the CF/ui helpers below. They are declared extern "C" so calls bind to the
 * literal retail symbol (MWCC would otherwise mangle cmpwi`func_8029ABB8` to
 * `func_8029ABB8__FP10CTutorialPQ34nw4r3lyt8DrawInfo`). CTitleAHelp.hpp also
 * declares many of these as members, but retail uses the unmangled form, so
 * free-function calls with an explicit subobject pointer reproduce retail.
 */
extern "C" {
void func_801C3FF0(CTitleAHelp*);
void func_801C40A0(CTitleAHelp*);
u8 func_801C4114(CTitleAHelp*);
void func_801C412C(CTitleAHelp*);
void func_801C41C0(CTitleAHelp*, char*);
void func_801C41E8(CTitleAHelp*, u8);
void func_801C4760(CTitleAHelp*);
void func_801C4080(CTitleAHelp*, nw4r::lyt::DrawInfo*);

void func_8029ABB8(CTutorial*, nw4r::lyt::DrawInfo*);
void func_8029AB28(CTutorial*);
void func_8029ABD8(CTutorial*);
void func_8029ACC4(CTutorial*);
u8 func_8029AE5C(CTutorial*);
int func_8029ACAC(CTutorial*);
int func_8029ACBC(CTutorial*);
void func_8029AD88(CTutorial*);
void func_8029ACEC(CTutorial*);
void func_8029ADF8(CTutorial*);
void func_801C414C(CTitleAHelp*);

u8 func_8013B980();
void func_80135550();
u8 code80135FDC_getByte_64080();
void code80135FDC_postIncByte_64080();
void func_8008294C__Q22cf13CfGameManagerFv(u8);
bool func_80086F9C__Q22cf13CfGameManagerFv(s16);
}

// Phase handlers referenced by Move() before their definitions below.
void func_8029A668(CMenuTutorial* self);
void func_8029A7B4(CMenuTutorial* self);
void func_8029A764(CMenuTutorial* self);
void func_8029A8D4(CMenuTutorial* self);

/* Retail constructor symbol (unmangled global). Kept out-of-line so the
 * factory (func_8029A5DC) emits a real bl to it, and returns `this` in r3
 * like a real constructor (retail relies on it). */
__declspec(noinline) CMenuTutorial* __ct__CMenuTutorial(CMenuTutorial* self, CProcess* parent, u32 arg2) {
    return self;
}

CMenuTutorial::~CMenuTutorial() {}

void CMenuTutorial::Init() {
    char* name = func_80136190(lbl_eu_80510260, lbl_eu_80510260 + 9, 0x23);

    // Re-initialise the embedded CTitleAHelp via a temporary: copy the unk4
    // sub-object with its copy-ctor, then copy the remaining scalar fields.
    u8 tempTitle[0x38];
    extern void __ct__CTitleAHelp(void*, char*, int);
    __ct__CTitleAHelp(tempTitle, name, 0x6f);

    extern void __ct__UnkClass_8011C974(void*, void*);
    __ct__UnkClass_8011C974((u8*)this + 0xb8, tempTitle + 0x4);

    *(u32*)((u8*)this + 0xc8) = *(u32*)(tempTitle + 0x1c);
    *(u32*)((u8*)this + 0xcc) = *(u32*)(tempTitle + 0x20);
    *(u32*)((u8*)this + 0xd0) = *(u32*)(tempTitle + 0x24);
    *(u32*)((u8*)this + 0xd4) = *(u32*)(tempTitle + 0x28);
    *(u32*)((u8*)this + 0xd8) = *(u32*)(tempTitle + 0x2c);
    *(u8*)((u8*)this + 0xdc) = *(u8*)(tempTitle + 0x30);
    *(u32*)((u8*)this + 0xe0) = *(u32*)(tempTitle + 0x34);
    *(u32*)((u8*)this + 0xe4) = *(u32*)(tempTitle + 0x38);
    *(u8*)((u8*)this + 0xe8) = *(u8*)(tempTitle + 0x3c);
    *(u8*)((u8*)this + 0xe9) = *(u8*)(tempTitle + 0x3d);
    *(u8*)((u8*)this + 0xea) = *(u8*)(tempTitle + 0x3e);
    *(u8*)((u8*)this + 0xeb) = *(u8*)(tempTitle + 0x3f);

    extern void __dt__11CTitleAHelpFv(void*, int);
    __dt__11CTitleAHelpFv(tempTitle, -1);

    extern void CTitleAHelp_load(void*);
    CTitleAHelp_load((u8*)this + 0xb4);

    // Re-initialise the embedded CTutorial via a second temporary.
    u8 tempTutorial[0x54];
    extern void __ct__CTutorial(void*, int, int);
    __ct__CTutorial(tempTutorial, this->mType, 1);

    __ct__UnkClass_8011C974((u8*)this + 0x64, tempTutorial + 0x4);
    __ct__UnkClass_8011C974((u8*)this + 0x74, tempTutorial + 0x14);

    *(u32*)((u8*)this + 0x84) = *(u32*)(tempTutorial + 0x24);
    *(u32*)((u8*)this + 0x88) = *(u32*)(tempTutorial + 0x28);
    *(u32*)((u8*)this + 0x8c) = *(u32*)(tempTutorial + 0x2c);
    *(u32*)((u8*)this + 0x90) = *(u32*)(tempTutorial + 0x30);
    *(u32*)((u8*)this + 0x94) = *(u32*)(tempTutorial + 0x34);
    *(u32*)((u8*)this + 0x98) = *(u32*)(tempTutorial + 0x38);
    *(u32*)((u8*)this + 0x9c) = *(u32*)(tempTutorial + 0x3c);
    *(u32*)((u8*)this + 0xa0) = *(u32*)(tempTutorial + 0x40);
    *(u8*)((u8*)this + 0xa4) = *(u8*)(tempTutorial + 0x44);
    *(u8*)((u8*)this + 0xa5) = *(u8*)(tempTutorial + 0x45);
    *(u8*)((u8*)this + 0xa6) = *(u8*)(tempTutorial + 0x46);
    *(u8*)((u8*)this + 0xa7) = *(u8*)(tempTutorial + 0x47);
    *(u8*)((u8*)this + 0xa8) = *(u8*)(tempTutorial + 0x48);
    *(u32*)((u8*)this + 0xac) = *(u32*)(tempTutorial + 0x4c);
    *(u8*)((u8*)this + 0xb0) = *(u8*)(tempTutorial + 0x50);
    *(u8*)((u8*)this + 0xb1) = *(u8*)(tempTutorial + 0x51);
    *(u8*)((u8*)this + 0xb2) = *(u8*)(tempTutorial + 0x52);
    *(u8*)((u8*)this + 0xb3) = *(u8*)(tempTutorial + 0x53);

    extern void __dt__9CTutorialFv(void*, int);
    __dt__9CTutorialFv(tempTutorial, -1);

    extern void func_8029AA34(void*);
    func_8029AA34((u8*)this + 0x60);

    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&mIScnRender);
    }
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(cb, 0x11, 0);
}

void CMenuTutorial::Term() {
    CDeviceVI::waitForDrawDone();

    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = reinterpret_cast<IScnRender*>(&mIScnRender);
    }
    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(renderCB);

    func_801C40A0(&mTitleAHelp);
    func_8029ABD8(&mTutorial);

    lbl_eu_80664A28 = 0;
    cf::CfGameManager::enablePadFlags((u32)-1, false);
    cf::CfGameManager::enablePadFlags(mSavedInputFlags, true);

    func_8013B980();
    if (mSomething == 0) {
        if (code80135FDC_getByte_64080() == 0) {
            func_8008294C__Q22cf13CfGameManagerFv(0);
        }
        func_80135550();
    }
}

void CMenuTutorial::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        return;
    }
    if (lbl_eu_80663E28 & (1u << 21)) {
        return;
    }
    switch (mIsInitialised) {
    case 0:
        func_8029A668(this);
        break;
    case 1:
        func_8029A764(this);
        break;
    case 2:
        func_8029A7B4(this);
        break;
    case 3:
        func_8029A8D4(this);
        break;
    }
    func_801C3FF0(&mTitleAHelp);
    func_8029AB28(&mTutorial);
}

void CMenuTutorial::Draw() {}

void CMenuTutorial::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        return;
    }
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) {
        if (func_8013BE50() != 0) {
            GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
            nw4r::lyt::DrawInfo drawInfo;
            func_80137250(&drawInfo);
            func_8029ABB8(&mTutorial, &drawInfo);
            func_801C4080(&mTitleAHelp, &drawInfo);
        }
    }
}

/*
 * When the title/help bar is idle and the tutorial widget has finished its
 * current animation, mark the tutorial as having reached phase 2.
 */
// Retail callee symbols are unmangled globals; declared as int so the result
// is compared with cmpwi (no byte mask) like retail.
extern int func_8029ACB4(CTutorial* c);
extern int isIdle__11CTitleAHelpFv(CTitleAHelp* h);
void func_8029A764(CMenuTutorial* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8029ACB4(&self->mTutorial) != 0) {
        self->mIsInitialised = 2;
    }
}

/*
 * Factory: lazily allocate + construct the single tutorial menu instance and
 * register it as a child of `self`. Returns the stored instance (or 0 if it
 * already exists).
 */
CMenuTutorial* func_8029A5DC(CProcess* self, CProcess* parent, u32 arg2) {
    if (lbl_eu_80664A28 != 0) {
        return 0;
    }
    CMenuTutorial* obj = (CMenuTutorial*)mtl::MemManager::allocate(
        0xf4, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuTutorial(obj, parent, arg2);
    }
    lbl_eu_80664A28 = (unsigned long)obj;
    obj->Regist(self, 0x0);
    return (CMenuTutorial*)lbl_eu_80664A28;
}

/*
 * Phase 0 handler: run the opening tutorial intro sequence. Advances the
 * tutorial widget phase and schedules the tutorial menu's next stage.
 */
void func_8029A668(CMenuTutorial* self) {
    if (func_801C4114(&self->mTitleAHelp) != 0 &&
        func_8029ACAC(&self->mTutorial) != 0) {
        func_801C4760(&self->mTitleAHelp);
        func_801C41C0(&self->mTitleAHelp,
            func_80136190(lbl_eu_80510260 + 0xe, lbl_eu_80510260 + 0x17, self->mType));

        u8 phase = func_8029AE5C(&self->mTutorial);
        switch (phase) {
        case 0:
            func_801C41E8(&self->mTitleAHelp, 0x6f);
            break;
        case 1:
            func_801C41E8(&self->mTitleAHelp, 0x70);
            break;
        case 2:
            func_801C41E8(&self->mTitleAHelp, 0x71);
            break;
        case 3:
            func_801C41E8(&self->mTitleAHelp, 0x72);
            break;
        }
        func_801C412C(&self->mTitleAHelp);
        func_8029ACC4(&self->mTutorial);
        self->mIsInitialised = 1;
        func_80138078__FUl(0x6d);
    }
}

/*
 * Phase 2 (case 2) handler: react to confirm/cancel input on the tutorial
 * widget, then repaint the title/help bar labels according to the widget's
 * current phase.
 */
void func_8029A7B4(CMenuTutorial* self) {
    if (func_8029ACAC(&self->mTutorial) != 0) {
        CPad* pad = cf::CfGameManager::getCurrentPad();
        u32 first;
        u32 second;
        if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
            first = (pad->mPressedButtonFlags >> 21) & 1;
            second = (pad->mPressedButtonFlags >> 22) & 1;
        } else {
            first = (pad->mPressedButtonFlags >> 4) & 1;
            second = (pad->mPressedButtonFlags >> 5) & 1;
        }
        if (first) {
            func_8029AD88(&self->mTutorial);
            if (func_8029ACBC(&self->mTutorial) != 0) {
                func_801C414C(&self->mTitleAHelp);
                func_8029ACEC(&self->mTutorial);
                self->mIsInitialised = 3;
            }
        } else if (second) {
            func_8029ADF8(&self->mTutorial);
        }

        u8 phase = func_8029AE5C(&self->mTutorial);
        switch (phase) {
        default:
            break;
        case 0:
            func_801C41E8(&self->mTitleAHelp, 0x6f);
            break;
        case 1:
            func_801C41E8(&self->mTitleAHelp, 0x70);
            break;
        case 2:
            func_801C41E8(&self->mTitleAHelp, 0x71);
            break;
        case 3:
            func_801C41E8(&self->mTitleAHelp, 0x72);
            break;
        }
    }
}

/*
 * Same idle+advance check as func_8029A764, but advances the tutorial to
 * phase 1 (writes the state byte at offset 0x54).
 */
void func_8029A8D4(CMenuTutorial* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8029ACB4(&self->mTutorial) != 0) {
        self->field_54 = 1;
    }
}

/**
 * IScnRender vtable this-adjusting thunk: subi r3, r3, 0x58; b cbRenderBefore.
 */
void func_8029A924(IScnRender* sub) {
    ((CMenuTutorial*)((char*)sub - 0x58))->cbRenderBefore();
}

/**
 * IScnRender vtable this-adjusting thunk: subi r3, r3, 0x58; b __dt__.
 * Tail-calls the destructor, leaving r4 (delete flag) as caller leftover.
 */
extern void __dt__13CMenuTutorialFv(CMenuTutorial*);
void func_8029A92C(IScnRender* sub) {
    __dt__13CMenuTutorialFv((CMenuTutorial*)((char*)sub - 0x58));
}

extern "C" unsigned long func_8029A658(void) { return lbl_eu_80664A28 != 0; }