// CSimpleEveTalkWin - simple event-talk window process (singleton factory
// func_801A20DC). See CSimpleEveTalkWin.hpp for the layout notes.

#include "kyoshin/CSimpleEveTalkWin.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

#include <revolution/gx/GXPixel.h>
#include <string.h>

// Retail declares the constructor as a C-ABI global `__ct__CSimpleEveTalkWin`
// (the split symbol carries no class-length mangling), so it is kept as a
// plain C-linkage global function rather than a member ctor (MWCC would
// mangle a C++ free function to __ct__CSimpleEveTalkWin__FP...).
//
// Runs CProcess's ctor then fills the IUIWindow region by hand (base ctor is
// out-of-line in retail, its effects are emitted inline here): the vtable at
// +0x10 is written twice (temp IUIWindow vtable, then the composite vtable),
// the two __ptmf_null callback slots are copied, then the scalar fields and
// the embedded UnkClass_8045F564 region are constructed and the ctor args are
// stored.
extern "C" CSimpleEveTalkWin* __ct__CSimpleEveTalkWin(CSimpleEveTalkWin* _this,
                                                      CScn* scene, u32 text,
                                                      u8* msgBuf, u8 flag) {
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(_this));

    _this->mVtable = (u32)lbl_eu_8052D238;

    // Copy the null member-function pointer into both callback slots (same
    // shape as the committed ~CSystemWindow/CSysWinSave ctors; retail folds
    // the first load into an lwzu - a known lis/addi-vs-lwzu scheduling
    // residual of this IUIWindow ctor family).
    u32* ptmf = __ptmf_null;
    _this->ptmf0[0] = ptmf[0];
    _this->ptmf0[1] = ptmf[1];
    _this->ptmf0[2] = ptmf[2];
    _this->ptmf1[0] = ptmf[0];
    _this->ptmf1[1] = ptmf[1];
    _this->ptmf1[2] = ptmf[2];

    _this->mpLayout = 0;
    _this->field_58 = 0;
    _this->field_5C = 0;
    _this->field_60 = -1;
    _this->field_64 = 0;
    _this->field_65 = 0;
    _this->field_66 = 0;
    _this->field_67 = 1;
    _this->field_68 = 0;

    _this->mVtable = (u32)lbl_eu_80532EE0;
    _this->mWorkEvent = (u32)(lbl_eu_80532EE0 + 0x24);
    _this->mScnRender = (u32)(lbl_eu_80532EE0 + 0xac);

    __ct__17UnkClass_8045F564Fv(
        reinterpret_cast<UnkClass_8045F564*>(_this->mMemRegion));

    _this->mScene = scene;
    _this->field_88 = 0;
    _this->field_8C = 0;
    _this->field_90 = 0;
    _this->field_94 = 0;
    _this->mMsgBuf = msgBuf;
    _this->field_AC = flag;
    _this->field_AD = 1;
    _this->field_AE = 4;
    _this->field_68 = text;

    return _this;
}

CSimpleEveTalkWin::~CSimpleEveTalkWin() {
    // Destroy the embedded UnkClass_8045F564 storage (raw buffer, explicit
    // C-linkage call with -1), then the CProcess base at +0. The nested double
    // null-check is an MWCC artifact of D2-inlined-into-D1 (same shape as
    // ~CSystemWindow).
    __dt__17UnkClass_8045F564Fv(
        reinterpret_cast<UnkClass_8045F564*>(mMemRegion), -1);
    if (this != 0) {
        if (this != 0) {
            __dt__8CProcessFv(reinterpret_cast<CProcess*>(this), 0);
        }
    }
}

void CSimpleEveTalkWin::Init() {
    mtl::ALLOC_HANDLE mem = mtl::MemManager::getHandleMEM2();
    ((UnkClass_8045F564*)mMemRegion)
        ->createRegion((int)mem, 0x4000, lbl_eu_80503E14, 0);
    // Retail only ever touches the first word of the guard, so a 4-byte slot
    // keeps the SetTranslate VEC3 temp at r1+0xc like retail.
    u8 regionGuard[4];
    __ct__14Class_8045F858FP17UnkClass_8045F564(
        regionGuard, (UnkClass_8045F564*)mMemRegion);

    // Build the tag processor on a 0x840-byte heap block (retail ctor is
    // C-ABI: __ct__CTagProcessorSE, no class-length mangling; it returns this
    // in r3, which the retail code reuses directly for the message setup).
    void* tagMem = mtl::MemManager::allocate(0x840, mem);
    void* tagProc = tagMem;
    if (tagMem != 0) {
        tagProc = __ct__CTagProcessorSE(tagMem);
    }
    field_88 = (nw4r::lyt::AnimTransform*)tagProc;

    reinterpret_cast<CSimpleTagProc*>(tagProc)
        ->setMessage(lbl_eu_80667CA0, lbl_eu_80667CA0, mMsgBuf, field_68);

    CSimpleTagProcFields* tagFields =
        reinterpret_cast<CSimpleTagProcFields*>(field_88);
    if (tagFields->field_0x814 == 1) {
        // Tag processor claims the message needs no layout: play its voice
        // and finish without building the window. Retail loads the 0x81C
        // float before the 0x81A byte.
        f32 voiceVol = tagFields->field_0x81C;
        u8 voiceId = tagFields->field_0x81A;
        func_80135464(voiceId, 0, voiceVol, lbl_eu_80667CA4,
                      lbl_eu_80667CA4);
        field_64 = 1;
        __dt__14Class_8045F858Fv(regionGuard, -1);
        return;
    }

    func_80136E84(&mpLayout, func_801355F4(), &lbl_eu_80503E14[0x12]);
    func_80136F08(mpLayout, &field_8C, func_801355F4(),
                  &lbl_eu_80503E14[0x2b]);
    func_80136F08(mpLayout, &field_90, func_801355F4(),
                  &lbl_eu_80503E14[0x47]);
    func_80136F08(mpLayout, &field_94, func_801355F4(),
                  &lbl_eu_80503E14[0x6a]);

    // Bind the font and hand the loaded font object over to the root pane.
    nw4r::lyt::Pane* rootPane = mpLayout->GetRootPane();
    u8* fontObj =
        (u8*)func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mpLayout);
    u32 fontResult =
        reinterpret_cast<CSimpleFontObj*>(fontObj)->getFontHandle();
    func_8013676C(rootPane, fontResult);

    // Co-op mode uses the 2P button texture.
    const char* texName;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0)
        texName = &lbl_eu_80503E14[0x8e];
    else
        texName = &lbl_eu_80503E14[0xaa];
    void* tex = func_801355F4()->GetResource(0x74696D67, texName, 0);
    if (tex != 0) {
        func_80137E7C(mpLayout, &lbl_eu_80503E14[0xc6], tex);
    }

    if (field_68 != 0) {
        // Real message: fill the five name fields from the talk source.
        char* msgText =
            reinterpret_cast<CSimpleTalkSrc*>(func_800B708C(field_68))
                ->getText();
        func_80136B4C(mpLayout, &lbl_eu_80503E14[0xd3], msgText, 0);
        func_80136B4C(mpLayout, &lbl_eu_80503E14[0xdc], msgText, 0);
        func_80136B4C(mpLayout, &lbl_eu_80503E14[0xe7], msgText, 0);
        func_80136B4C(mpLayout, &lbl_eu_80503E14[0xf2], msgText, 0);
        func_80136B4C(mpLayout, &lbl_eu_80503E14[0xfd], msgText, 0);
    } else {
        // No talk source: leave the name fields empty.
        func_80136B4C(mpLayout, &lbl_eu_80503E14[0xd3],
                      &lbl_eu_80503E14[0x108], 0);
        func_80136B4C(mpLayout, &lbl_eu_80503E14[0xdc],
                      &lbl_eu_80503E14[0x108], 0);
        func_80136B4C(mpLayout, &lbl_eu_80503E14[0xe7],
                      &lbl_eu_80503E14[0x108], 0);
        func_80136B4C(mpLayout, &lbl_eu_80503E14[0xf2],
                      &lbl_eu_80503E14[0x108], 0);
        func_80136B4C(mpLayout, &lbl_eu_80503E14[0xfd],
                      &lbl_eu_80503E14[0x108], 0);
    }

    // Attach the tag processor to the message text box and show it.
    func_80136B4C(mpLayout, &lbl_eu_80503E14[0x109], &lbl_eu_80503E14[0x108],
                  0);
    nw4r::lyt::Pane* textPane =
        mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_80503E14[0x109], 1);
    textPane->SetVisible(true);
    reinterpret_cast<nw4r::lyt::TextBox*>(textPane)->SetTagProcessor(
        reinterpret_cast<nw4r::ut::WideTagProcessor*>(field_88));
    func_801287BC(field_88, textPane, 0);

    // Without a talk source the window is a bare frame: pin it to its default
    // position.
    if (field_68 == 0) {
        mpLayout->GetRootPane()->SetTranslate(nw4r::math::VEC3(
            lbl_eu_80667CA8, lbl_eu_80667CAC, lbl_eu_80667CA8));
    }

    // No ctor-flag: start with the cursor animation disabled and the intro
    // animation at frame 1.
    if (field_AC == 0) {
        mpLayout->SetAnimationEnable(field_90, 0);
        mpLayout->SetAnimationEnable(field_94, 0);
        mpLayout->SetAnimationEnable(field_8C, 1);
        field_8C->SetFrame(lbl_eu_80667CA8);
    }

    mpLayout->Animate(0);
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mScnRender);
    }
    mScene->addRenderCB(render, 0xa, 0);
    ((UnkClass_8045F564*)mMemRegion)->func_8045F810();
    __dt__14Class_8045F858Fv(regionGuard, -1);
}

void CSimpleEveTalkWin::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;
    if (isInitialized__10CMenuPauseFv()) return;

    switch (field_AD) {
    case 1:
        // Intro animation finished: play the message voice, then cross-fade
        // to the cursor animation set and enter the advance state.
        if (func_80137444(field_8C, lbl_eu_80667CB0) != 0) {
            void* src = func_800B708C(field_68);
            if (src != 0) {
                reinterpret_cast<CSimpleTalkSrc*>(src)->field_0x98->play(1, 0);
            }
            mpLayout->SetAnimationEnable(field_94, 0);
            mpLayout->SetAnimationEnable(field_8C, 0);
            mpLayout->SetAnimationEnable(field_90, 1);
            field_AD = 2;
        }
        break;
    case 2:
        // Message advance (tag-processor page logic in func_801A2624).
        func_801A2624(this);
        break;
    case 3:
        // Closing animation finished: unregister the render callback and hand
        // control back to the caller.
        if (func_80137510(field_8C, lbl_eu_80667CB0) != 0) {
            CDeviceVI::waitForDrawDone();
            IScnRender* render = reinterpret_cast<IScnRender*>(this);
            if (this != 0) {
                render = reinterpret_cast<IScnRender*>(&mScnRender);
            }
            mScene->removeRenderCB(render);
            field_64 = 1;
            lbl_eu_80664320 = 0;
            field_AD = 0;
            lbl_eu_80664324 = 1;
        }
        break;
    }
    mpLayout->Animate(0);
}

void CSimpleEveTalkWin::Term() {
    if (lbl_eu_80664324 == 0) {
        CDeviceVI::waitForDrawDone();
        IScnRender* render = reinterpret_cast<IScnRender*>(this);
        if (this != 0) {
            render = reinterpret_cast<IScnRender*>(&mScnRender);
        }
        mScene->removeRenderCB(render);
        lbl_eu_80664320 = 0;
    } else {
        lbl_eu_80664324 = 0;
    }

    if (mpLayout != 0) {
        delete mpLayout;
        mpLayout = 0;
    }
    if (field_88 != 0) {
        delete field_88;
        field_88 = 0;
    }
    func_8045F778__17UnkClass_8045F564Fv(
        reinterpret_cast<UnkClass_8045F564*>(mMemRegion));
}

void CSimpleEveTalkWin::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0)
        return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls
    // to match the retail direct calls (a C++ local would virtual-dispatch its
    // scope-exit destructor and bloat the body).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137038(mpLayout, (nw4r::lyt::DrawInfo*)&drawInfo[0], 0, 1);
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
}

// Creates the singleton CSimpleEveTalkWin on the work heap and registers it as
// a CProcess under `parent`. If the singleton already exists the message is
// handed to func_801A2190 instead and 0 is returned.
CSimpleEveTalkWin* func_801A20DC(CProcess* parent, CScn* scene, u32 text,
                                 const u8* msgSrc, u8 flag) {
    u8 buf[0x800];
    memset(buf, 0, 0x800);
    func_80136400((const char*)msgSrc, (u16*)buf, 0x400);

    if (lbl_eu_80664320 != 0) {
        func_801A2190(lbl_eu_80664320, text, buf);
        return 0;
    }

    u32 workMem = CWorkThreadSystem::getWorkMem();
    CSimpleEveTalkWin* obj = (CSimpleEveTalkWin*)mtl::MemManager::allocate(
        0xb0, workMem);
    if (obj != 0) {
        obj = __ct__CSimpleEveTalkWin(obj, scene, text, buf, flag);
    }
    lbl_eu_80664320 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return lbl_eu_80664320;
}

// --- this-adjusting thunks (retail vtable dispatch: "this" lands at the
// embedded IWorkEvent/IScnRender subobject; back off to the object base and
// forward to the real member). func_801A2190 / func_801A2624 remain stubs.

extern "C" __declspec(noinline) void func_801A2190(CSimpleEveTalkWin* owner,
                                                    u32 textId, u8* msgBuf) {
    owner->field_67 = 1;
    owner->field_68 = textId;
    owner->mMsgBuf = msgBuf;

    // Drop the old layout and tag processor (nested null checks are the
    // retail D2-inline shape, same as Term).
    if (owner->mpLayout != 0) {
        delete owner->mpLayout;
        owner->mpLayout = 0;
    }
    if (owner->field_88 != 0) {
        delete owner->field_88;
        owner->field_88 = 0;
    }
    ((UnkClass_8045F564*)owner->mMemRegion)->func_8045F778();

    // --- rebuild: identical to Init() below this line ---
    mtl::ALLOC_HANDLE mem = mtl::MemManager::getHandleMEM2();
    ((UnkClass_8045F564*)owner->mMemRegion)
        ->createRegion((int)mem, 0x4000, lbl_eu_80503E14, 0);
    // Retail only ever touches the first word of the guard, so a 4-byte slot
    // keeps the SetTranslate VEC3 temp at r1+0xc like retail.
    u8 regionGuard[4];
    __ct__14Class_8045F858FP17UnkClass_8045F564(
        regionGuard, (UnkClass_8045F564*)owner->mMemRegion);

    void* tagMem = mtl::MemManager::allocate(0x840, mem);
    void* tagProc = tagMem;
    if (tagMem != 0) {
        tagProc = __ct__CTagProcessorSE(tagMem);
    }
    owner->field_88 = (nw4r::lyt::AnimTransform*)tagProc;

    reinterpret_cast<CSimpleTagProc*>(tagProc)
        ->setMessage(lbl_eu_80667CA0, lbl_eu_80667CA0, owner->mMsgBuf,
                     owner->field_68);

    CSimpleTagProcFields* tagFields =
        reinterpret_cast<CSimpleTagProcFields*>(owner->field_88);
    if (tagFields->field_0x814 == 1) {
        f32 voiceVol = tagFields->field_0x81C;
        u8 voiceId = tagFields->field_0x81A;
        func_80135464(voiceId, 0, voiceVol, lbl_eu_80667CA4,
                      lbl_eu_80667CA4);
        owner->field_64 = 1;
        __dt__14Class_8045F858Fv(regionGuard, -1);
        return;
    }

    owner->field_8C = 0;
    owner->field_90 = 0;
    owner->field_94 = 0;

    func_80136E84(&owner->mpLayout, func_801355F4(), &lbl_eu_80503E14[0x12]);
    func_80136F08(owner->mpLayout, &owner->field_8C, func_801355F4(),
                  &lbl_eu_80503E14[0x2b]);
    func_80136F08(owner->mpLayout, &owner->field_90, func_801355F4(),
                  &lbl_eu_80503E14[0x47]);
    func_80136F08(owner->mpLayout, &owner->field_94, func_801355F4(),
                  &lbl_eu_80503E14[0x6a]);

    // Bind the font and hand the loaded font object over to the root pane.
    nw4r::lyt::Pane* rootPane = owner->mpLayout->GetRootPane();
    u8* fontObj = (u8*)func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
        1, owner->mpLayout);
    u32 fontResult =
        reinterpret_cast<CSimpleFontObj*>(fontObj)->getFontHandle();
    func_8013676C(rootPane, fontResult);

    // Start with the cursor animation disabled and the cursor at frame 1.
    owner->mpLayout->SetAnimationEnable(owner->field_8C, 0);
    owner->mpLayout->SetAnimationEnable(owner->field_94, 0);
    owner->mpLayout->SetAnimationEnable(owner->field_90, 1);
    owner->field_90->SetFrame(lbl_eu_80667CA8);
    owner->field_AE = 4;

    // Co-op mode uses the 2P button texture.
    const char* texName;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0)
        texName = &lbl_eu_80503E14[0x8e];
    else
        texName = &lbl_eu_80503E14[0xaa];
    void* tex = func_801355F4()->GetResource(0x74696D67, texName, 0);
    if (tex != 0) {
        func_80137E7C(owner->mpLayout, &lbl_eu_80503E14[0xc6], tex);
    }

    if (owner->field_68 != 0) {
        // Real message: fill the five name fields from the talk source, then
        // start the voice line.
        char* msgText =
            reinterpret_cast<CSimpleTalkSrc*>(func_800B708C(owner->field_68))
                ->getText();
        func_80136B4C(owner->mpLayout, &lbl_eu_80503E14[0xd3], msgText, 0);
        func_80136B4C(owner->mpLayout, &lbl_eu_80503E14[0xdc], msgText, 0);
        func_80136B4C(owner->mpLayout, &lbl_eu_80503E14[0xe7], msgText, 0);
        func_80136B4C(owner->mpLayout, &lbl_eu_80503E14[0xf2], msgText, 0);
        func_80136B4C(owner->mpLayout, &lbl_eu_80503E14[0xfd], msgText, 0);
        void* src = func_800B708C(owner->field_68);
        if (src != 0) {
            reinterpret_cast<CSimpleTalkSrc*>(src)->field_0x98->play(1, 0);
        }
    } else {
        // No talk source: leave the name fields empty.
        func_80136B4C(owner->mpLayout, &lbl_eu_80503E14[0xd3],
                      &lbl_eu_80503E14[0x108], 0);
        func_80136B4C(owner->mpLayout, &lbl_eu_80503E14[0xdc],
                      &lbl_eu_80503E14[0x108], 0);
        func_80136B4C(owner->mpLayout, &lbl_eu_80503E14[0xe7],
                      &lbl_eu_80503E14[0x108], 0);
        func_80136B4C(owner->mpLayout, &lbl_eu_80503E14[0xf2],
                      &lbl_eu_80503E14[0x108], 0);
        func_80136B4C(owner->mpLayout, &lbl_eu_80503E14[0xfd],
                      &lbl_eu_80503E14[0x108], 0);
    }

    // Attach the tag processor to the message text box and show it.
    func_80136B4C(owner->mpLayout, &lbl_eu_80503E14[0x109],
                  &lbl_eu_80503E14[0x108], 0);
    nw4r::lyt::Pane* textPane = owner->mpLayout->GetRootPane()
                                    ->FindPaneByName(&lbl_eu_80503E14[0x109],
                                                     1);
    textPane->SetVisible(true);
    reinterpret_cast<nw4r::lyt::TextBox*>(textPane)->SetTagProcessor(
        reinterpret_cast<nw4r::ut::WideTagProcessor*>(owner->field_88));
    func_801287BC(owner->field_88, textPane, 0);

    // Without a talk source the window is a bare frame: pin it to its default
    // position.
    if (owner->field_68 == 0) {
        owner->mpLayout->GetRootPane()->SetTranslate(nw4r::math::VEC3(
            lbl_eu_80667CA8, lbl_eu_80667CAC, lbl_eu_80667CA8));
    }

    ((UnkClass_8045F564*)owner->mMemRegion)->func_8045F810();
    __dt__14Class_8045F858Fv(regionGuard, -1);
}

// Message-advance step (window state 2): re-fetch the message text box, ask
// the tag processor which page action the confirm button triggered and run it
// (4 = select accepted, 2 = page animation, 0/1/3 = page/close logic).
extern "C" void func_801A2624(CSimpleEveTalkWin* self) {
    nw4r::lyt::Pane* pane =
        self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_80503E14[0x109],
                                                      1);
    CPad* pad = cf::CfGameManager::getCurrentPad();

    // In co-op the confirm button is Classic A (bit 21), otherwise Wiimote A
    // (bit 4); the pad flag fields are 2-bit counters, so test the extracted
    // pair for non-zero.
    int confirmBtn;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0)
        confirmBtn = (pad->mPressedButtonFlags & 0x00600000) != 0;
    else
        confirmBtn = (pad->mPressedButtonFlags & 0x00000030) != 0;

    int state;
    if (confirmBtn != 0)
        state = func_801286E0(self->field_88, pane);
    else
        state = func_80127FB4(self->field_88, pane);

    if (state == 4) {
        // Selection accepted: play the confirm voice.
        func_80128740(self->field_88, pane);
        void* src = func_800B708C(self->field_68);
        if (src != 0) {
            reinterpret_cast<CSimpleTalkSrc*>(src)->field_0x98->play(1, 0);
        }
    } else if (state == 2) {
        // Page transition animations (4 -> 5 -> 6 -> 7 -> 4 loop).
        switch (self->field_AE) {
        case 4:
            self->mpLayout->SetAnimationEnable(self->field_8C, 0);
            self->mpLayout->SetAnimationEnable(self->field_94, 0);
            self->mpLayout->SetAnimationEnable(self->field_90, 1);
            self->field_90->SetFrame(lbl_eu_80667CA8);
            self->field_AE = 5;
            break;
        case 5:
            if (func_80137444(self->field_90, lbl_eu_80667CB0) != 0) {
                self->mpLayout->SetAnimationEnable(self->field_8C, 0);
                self->mpLayout->SetAnimationEnable(self->field_90, 0);
                self->mpLayout->SetAnimationEnable(self->field_94, 1);
                self->field_94->SetFrame(lbl_eu_80667CA8);
                self->field_AE = 6;
            }
            break;
        case 6:
            func_80137444(self->field_94, lbl_eu_80667CB0);
            break;
        case 7:
            if (func_80137510(self->field_90, lbl_eu_80667CB0) != 0) {
                self->mpLayout->SetAnimationEnable(self->field_8C, 0);
                self->mpLayout->SetAnimationEnable(self->field_94, 0);
                self->mpLayout->SetAnimationEnable(self->field_90, 1);
                self->field_90->SetFrame(lbl_eu_80667CA8);
                self->field_AE = 4;
            }
            break;
        }
    } else if (state == 0 || self->field_65 != 0 || self->field_66 != 0) {
        // Page advance: stop the current voice, switch to the next page's
        // animation set and let the window close when the page ends.
        void* src = func_800B708C(self->field_68);
        if (src != 0) {
            reinterpret_cast<CSimpleTalkSrc*>(src)->field_0x98->play(0, 0);
        }
        self->mpLayout->SetAnimationEnable(self->field_94, 0);
        self->mpLayout->SetAnimationEnable(self->field_90, 0);
        self->mpLayout->SetAnimationEnable(self->field_8C, 1);
        self->field_AD = 3;
    } else {
        // Still waiting on the page: re-arm the cursor (state 1) or close the
        // window (state 3). The redundant field_65 re-check is retail shape.
        if (state == 1) {
            if (self->field_65 == 0) {
                self->field_65 = 1;
            }
        } else if (state == 3) {
            self->field_67 = 0;
        }
    }
}

void func_801A29B4(u8* self) {
    reinterpret_cast<CSimpleEveTalkWin*>(self - 0x6c)->~CSimpleEveTalkWin();
}

void func_801A29BC(u8* self) {
    reinterpret_cast<CSimpleEveTalkWin*>(self - 0x70)->cbRenderBefore();
}

void func_801A29C4(u8* self) {
    reinterpret_cast<CSimpleEveTalkWin*>(self - 0x70)->~CSimpleEveTalkWin();
}
