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
#include "monolib/core/CPadManager.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_drawInfo.h>

// Retail destructor symbol (mangled member name); C-linkage literal so the
// IScnRender thunk (func_8029A92C) binds to the real dtor symbol.
extern "C" void __dt__13CMenuTutorialFv(CMenuTutorial* self);

/*
 * Retail constructor symbol (unmangled global in US). Written as a free
 * function so the factory (func_8029A5DC) emits a real bl to the bare retail
 * symbol, and returns `this` in r3 like a real constructor (retail relies on
 * it). Mirrors the CMenuCollepedia ctor pattern: base ctor, temp vtable
 * store, null PMF data copy, then the composite vtable + the IScnRender
 * sub-vtable at +0x58, then each embedded widget's ctor and the final state
 * bytes + input-flag snapshot.
 */
extern "C" __declspec(noinline) CMenuTutorial* __ct__CMenuTutorial(
    CMenuTutorial* self, CProcess* parent, u32 arg2) {
    __ct__8CProcessFv((CProcess*)self);

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable
    // and the IScnRender sub-vtable at +0x58.
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;
    const u32* nullPtmf = __ptmf_null;
    // Copy the null PMF words in retail store order: 0x40, 0x3C, 0x44, then
    // 0x4C, 0x48, 0x50 (with a reload for the second group).
    u32 w0 = nullPtmf[0];
    u32 w1 = nullPtmf[1];
    self->ptmf0[1] = w1;
    self->ptmf0[0] = w0;
    self->ptmf0[2] = nullPtmf[2];
    w0 = nullPtmf[0];
    w1 = nullPtmf[1];
    self->ptmf1[1] = w1;
    self->ptmf1[0] = w0;
    self->ptmf1[2] = nullPtmf[2];
    self->field_54 = 0;
    self->field_55 = 0;

    // Composite vtable + IScnRender sub-vtable computed from one address so
    // MWCC reuses the register (retail: lis/addi r6 once, then addi r0,r6,0x24).
    u32 composite = (u32)lbl_eu_805391C0;
    *(u32*)((u8*)self + 0x10) = composite;
    *(u32*)((u8*)self + 0x58) = composite + 0x24;
    self->mParentRef = parent;

    __ct__CTutorial(&self->mTutorial, 0, 0);
    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);

    self->mIsInitialised = 0;
    self->mType = arg2;
    self->mSomething = 0;
    self->mSavedInputFlags = 0;
    // Bit 1 of the Init flag word decides whether the open event is fired;
    // the byte is stored twice because retail does (0 then the extracted bit).
    self->mSomething = (lbl_eu_80663E24 >> 1) & 1;
    if (self->mSomething == 0) {
        func_8008294C__Q22cf13CfGameManagerFv(1);
    }

    code80135FDC_postIncByte_64080();
    self->mSavedInputFlags = cf::CfGameManager::getEnabledInputFlags();

    // Re-enable the input-flag set the player had before the tutorial:
    // classic controller (0x60000000) or core pad (0x30).
    u32 padFlags =
        func_80086F9C__Q22cf13CfGameManagerFv(-1) ? 0x60000000 : 0x30;
    cf::CfGameManager::enablePadFlags((u32)-1, false);
    cf::CfGameManager::enablePadFlags(padFlags, true);
    return self;
}

CMenuTutorial::~CMenuTutorial() {}

void CMenuTutorial::Init() {
    char* name = func_80136190(lbl_eu_80510260, lbl_eu_80510260 + 9, 0x23);

    // Re-initialise the embedded CTitleAHelp via a temporary: copy the unk4
    // sub-object with its copy-ctor, then copy the remaining scalar fields.
    u8 tempTitle[0x38];
    __ct__CTitleAHelp((CTitleAHelp*)tempTitle, name, 0x6f);

    __ct__UnkClass_8011C974(&mTitleAHelp.unk4, tempTitle + 0x4);
    mTitleAHelp.mFileHandle = *(CFileHandle**)(tempTitle + 0x14);
    mTitleAHelp.mArcResourceAccessor =
        *(nw4r::lyt::ArcResourceAccessor**)(tempTitle + 0x18);
    mTitleAHelp.mLayout = *(nw4r::lyt::Layout**)(tempTitle + 0x1c);
    mTitleAHelp.mAnimTrans20 = *(nw4r::lyt::AnimTransform**)(tempTitle + 0x20);
    mTitleAHelp.mAnimTrans24 = *(nw4r::lyt::AnimTransform**)(tempTitle + 0x24);
    mTitleAHelp.unk28 = *(u8*)(tempTitle + 0x28);
    mTitleAHelp.unk2c = *(s32*)(tempTitle + 0x2c);
    mTitleAHelp.mName = *(char**)(tempTitle + 0x30);
    mTitleAHelp.unk34 = *(u8*)(tempTitle + 0x34);
    mTitleAHelp.unk35 = *(u8*)(tempTitle + 0x35);
    mTitleAHelp.unk36 = *(u8*)(tempTitle + 0x36);
    mTitleAHelp.unk37 = *(u8*)(tempTitle + 0x37);
    __dt__11CTitleAHelpFv((CTitleAHelp*)tempTitle, -1);

    CTitleAHelp_load(&mTitleAHelp);

    // Re-initialise the embedded CTutorial via a second temporary.
    u8 tempTutorial[0x54];
    __ct__CTutorial((CTutorial*)tempTutorial, this->mType, 1);

    __ct__UnkClass_8011C974(&mTutorial.mRegion0, tempTutorial + 0x4);
    __ct__UnkClass_8011C974(&mTutorial.mRegion1, tempTutorial + 0x14);

    mTutorial.mFileHandle0 = *(CFileHandle**)(tempTutorial + 0x24);
    mTutorial.mFileHandle1 = *(CFileHandle**)(tempTutorial + 0x28);
    mTutorial.mFileHandle2 = *(CFileHandle**)(tempTutorial + 0x2c);
    mTutorial.mAccessor0 =
        *(nw4r::lyt::ArcResourceAccessor**)(tempTutorial + 0x30);
    mTutorial.mAccessor1 =
        *(nw4r::lyt::ArcResourceAccessor**)(tempTutorial + 0x34);
    mTutorial.mpLayout = *(nw4r::lyt::Layout**)(tempTutorial + 0x38);
    mTutorial.mpAnimTrans0 =
        *(nw4r::lyt::AnimTransform**)(tempTutorial + 0x3c);
    mTutorial.mpAnimTrans1 =
        *(nw4r::lyt::AnimTransform**)(tempTutorial + 0x40);
    mTutorial.field_44 = *(u8*)(tempTutorial + 0x44);
    mTutorial.field_45 = *(u8*)(tempTutorial + 0x45);
    mTutorial.field_46 = *(u8*)(tempTutorial + 0x46);
    mTutorial.field_47 = *(u8*)(tempTutorial + 0x47);
    mTutorial.field_48 = *(u8*)(tempTutorial + 0x48);
    mTutorial.field_4C = *(u8**)(tempTutorial + 0x4c);
    mTutorial.field_50 = *(s8*)(tempTutorial + 0x50);
    mTutorial.field_51 = *(s8*)(tempTutorial + 0x51);
    mTutorial.field_52 = *(u8*)(tempTutorial + 0x52);
    mTutorial.field_53 = *(u8*)(tempTutorial + 0x53);
    __dt__9CTutorialFv((CTutorial*)tempTutorial, -1);

    func_8029AA34(&mTutorial);

    // Register this screen's IScnRender subobject as a render callback on the
    // owning scene (retail null-checks `this`).
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
    // Gate: skip the whole move when the task is busy or the global mode bit
    // (0x200000) is set. The &&-chain + goto body + return shape reproduces
    // retail's branch-over-branch `beq body; b exit` (MWCC_REFERENCE §8960).
    if (CTaskGame::func_800426F0() == 0 &&
        (lbl_eu_80663E28 & (1u << 21)) == 0) {
        goto body;
    }
    return;
body:
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
    return;
}

void CMenuTutorial::Draw() {}

void CMenuTutorial::cbRenderBefore() {
    CTaskGame::getInstance();
    // Gate: skip the render when the task is busy or the global mode bit
    // (0x200000) is set. Same branch-over-branch shape as Move().
    if (CTaskGame::func_800426F0() == 0 &&
        (lbl_eu_80663E28 & (1u << 21)) == 0) {
        goto body;
    }
    return;
body:
    if (func_8013BE50() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI ct/dt calls to
    // match the retail direct calls (a C++ local would virtual-
    // dispatch its scope-exit destructor and bloat the body).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)drawInfo);
    func_80137250((nw4r::lyt::DrawInfo*)drawInfo);
    func_8029ABB8(&mTutorial, (nw4r::lyt::DrawInfo*)drawInfo);
    func_801C4080(&mTitleAHelp, (nw4r::lyt::DrawInfo*)drawInfo);
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)drawInfo, -1);
    return;
}

/*
 * When the title/help bar is idle and the tutorial widget has finished its
 * current animation, mark the tutorial as having reached phase 2.
 */
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
            func_80136190(lbl_eu_80510260 + 0xe, lbl_eu_80510260 + 0x17,
                self->mType));

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
        func_80138078(0x6d);
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
void func_8029A92C(IScnRender* sub) {
    __dt__13CMenuTutorialFv((CMenuTutorial*)((char*)sub - 0x58));
}

extern "C" unsigned long func_8029A658(void) { return lbl_eu_80664A28 != 0; }
