// CSimpleEveTalkWin - simple event-talk window process (singleton factory
// eveTalkWinCreateRegister). See CSimpleEveTalkWin.hpp for the layout notes.

#include "kyoshin/CSimpleEveTalkWin.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

#include <revolution/gx/GXPixel.h>
#include <string.h>

// ----- typified retail data (replaces copy_data_sections) -----
// Composite vtable and RTTI dispatch block; the two this-adjust deltas are
// immediates (CSysWinSelect/CSysWinBuff/CSysWinSave absorb the same labels).
__attribute__((used, aligned(8)))
char lbl_eu_80532EE0[0xC0] = {
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0x94, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0xFF,0xFF,0xFF,0x90, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
};
// .data RTTI dispatch block 0x40 (subobject offsets 0x70/0x6C immediates).
__attribute__((used, aligned(8)))
char lbl_eu_80532FA0[0x40] = {
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x70, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x6C,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
};
// .sdata typeinfo pair (RTTI name + dispatch block).
__attribute__((used, aligned(8)))
const void* __RTTI__17CSimpleEveTalkWin[2] = { "CSimpleEveTalkWin", lbl_eu_80532FA0 };
// .sbss singletons (MWCC auto-routes zero-init small data here).
CSimpleEveTalkWin* spEveTalkWinInstance = 0;
u8 sEveTalkWinTerminated = 0;
// ----- end typified retail data -----

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

    // Copy the null member-function pointer into both callback slots. The
    // composite vtable is materialized once here (its +0x24/+0xac interface
    // pointers are computed mid-copy) and installed after the scalars.
    u32* ptmf = __ptmf_null;
    u32 compVt = (u32)lbl_eu_80532EE0;

    // Slot 0: retail preloads word 0, then stores word 1 before word 0.
    u32 w0 = ptmf[0];
    u32 evtVt = compVt + 0x24;
    _this->ptmf0[1] = ptmf[1];
    u32 scnVt = compVt + 0xac;
    _this->ptmf0[0] = w0;
    _this->ptmf0[2] = ptmf[2];

    // Slot 1: retail stores word 1 before word 0.
    u32 v0 = ptmf[0];
    u32 v1 = ptmf[1];
    _this->ptmf1[1] = v1;
    _this->ptmf1[0] = v0;
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

    _this->mVtable = compVt;
    _this->mWorkEvent = evtVt;
    _this->mScnRender = scnVt;

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
        ->createRegion((int)mem, 0x4000, "CSimpleEveTalkWin", 0);
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
        ->Proc(mMsgBuf, 38.0f, 38.0f, field_68);

    CSimpleTagProc* tagFields =
        reinterpret_cast<CSimpleTagProc*>(field_88);
    if (tagFields->field_0x814 == 1) {
        // Tag processor claims the message needs no layout: play its voice
        // and finish without building the window. Retail loads the 0x81C
        // float before the 0x81A byte.
        f32 voiceVol = tagFields->field_0x81C;
        u8 voiceId = tagFields->field_0x81A;
        CUICfManager_queueFadeMenu(voiceId, 0, voiceVol, -1.0f,
                      -1.0f);
        field_64 = 1;
        __dt__14Class_8045F858Fv(regionGuard, -1);
        return;
    }

    buildLayout(&mpLayout, CUICfManager_getArcResourceAccessor(), "mf70_cf01_esywin00.brlyt");
    bindLayoutAnimTransform(mpLayout, &field_8C, CUICfManager_getArcResourceAccessor(),
                  "mf70_cf01_esywin00_in.brlan");
    bindLayoutAnimTransform(mpLayout, &field_90, CUICfManager_getArcResourceAccessor(),
                  "mf70_cf01_esywin00_cursor_in.brlan");
    bindLayoutAnimTransform(mpLayout, &field_94, CUICfManager_getArcResourceAccessor(),
                  "mf70_cf01_esywin00_cursor_def.brlan");

    // Bind the font and hand the loaded font object over to the root pane.
    nw4r::lyt::Pane* rootPane = mpLayout->GetRootPane();
    u8* fontObj =
        (u8*)getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mpLayout);
    u32 fontResult =
        reinterpret_cast<CSimpleFontObj*>(fontObj)->getFontHandle();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);

    // Co-op mode uses the 2P button texture.
    const char* texName;
    if (cf::CfGameManager::isClassicController(-1) != 0)
        texName = "mf00_com00_btn30_anim00.tpl";
    else
        texName = "mf00_com00_btn00_anim00.tpl";
    void* tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, texName, 0);
    if (tex != 0) {
        PaneSetTexPaletteByName(mpLayout, (const char*)"pic_cursor00", (TPLPalette*)tex);
    }

    if (field_68 != 0) {
        // Real message: fill the five name fields from the talk source.
        char* msgText = reinterpret_cast<char*>(
            reinterpret_cast<cf::CfObject*>(findObjectById(field_68))
                ->CObjectParam_getParamPtr());
        LayoutSetTextBoxFmtValue(mpLayout, "txt_name", msgText, 0);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_name_r", msgText, 0);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_name_l", msgText, 0);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_name_o", msgText, 0);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_name_u", msgText, 0);
    } else {
        // No talk source: leave the name fields empty.
        LayoutSetTextBoxFmtValue(mpLayout, "txt_name",
                      "", 0);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_name_r",
                      "", 0);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_name_l",
                      "", 0);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_name_o",
                      "", 0);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_name_u",
                      "", 0);
    }

    // Attach the tag processor to the message text box and show it.
    LayoutSetTextBoxFmtValue(mpLayout, "txt_mes", "",
                  0);
    nw4r::lyt::Pane* textPane =
        mpLayout->GetRootPane()->FindPaneByName("txt_mes", 1);
    textPane->SetVisible(true);
    reinterpret_cast<nw4r::lyt::TextBox*>(textPane)->SetTagProcessor(
        reinterpret_cast<nw4r::ut::WideTagProcessor*>(field_88));
    TagProcBuildOpenText(field_88, textPane, 0);

    // Without a talk source the window is a bare frame: pin it to its default
    // position.
    if (field_68 == 0) {
        mpLayout->GetRootPane()->SetTranslate(nw4r::math::VEC3(
            0.0f, 193.0f, 0.0f));
    }

    // No ctor-flag: start with the cursor animation disabled and the intro
    // animation at frame 1.
    if (field_AC == 0) {
        mpLayout->SetAnimationEnable(field_90, 0);
        mpLayout->SetAnimationEnable(field_94, 0);
        mpLayout->SetAnimationEnable(field_8C, 1);
        field_8C->SetFrame(0.0f);
    }

    mpLayout->Animate(0);
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mScnRender);
    }
    mScene->addRenderCB(render, 0xa, 0);
    ((UnkClass_8045F564*)mMemRegion)->validateHeap();
    __dt__14Class_8045F858Fv(regionGuard, -1);
}

void CSimpleEveTalkWin::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    if (isInitialized__10CMenuPauseFv()) return;

    switch (field_AD) {
    case 1:
        // Intro animation finished: play the message voice, then cross-fade
        // to the cursor animation set and enter the advance state.
        if (advanceAnimTransform(field_8C, 1.0f) != 0) {
            void* src = findObjectById(field_68);
            if (src != 0) {
                reinterpret_cast<CSimpleTalkFields*>(src)->voice98->play(1, 0);
            }
            mpLayout->SetAnimationEnable(field_94, 0);
            mpLayout->SetAnimationEnable(field_8C, 0);
            mpLayout->SetAnimationEnable(field_90, 1);
            field_AD = 2;
        }
        break;
    case 2:
        // Message advance (tag-processor page logic in eveTalkWinAdvancePage).
        eveTalkWinAdvancePage(this);
        break;
    case 3:
        // Closing animation finished: unregister the render callback and hand
        // control back to the caller.
        if (AnimRewindFrame(field_8C, 1.0f) != 0) {
            CDeviceVI::waitForDrawDone();
            IScnRender* render = reinterpret_cast<IScnRender*>(this);
            if (this != 0) {
                render = reinterpret_cast<IScnRender*>(&mScnRender);
            }
            mScene->removeRenderCB(render);
            field_64 = 1;
            spEveTalkWinInstance = 0;
            field_AD = 0;
            sEveTalkWinTerminated = 1;
        }
        break;
    }
    mpLayout->Animate(0);
}

void CSimpleEveTalkWin::Term() {
    if (sEveTalkWinTerminated == 0) {
        CDeviceVI::waitForDrawDone();
        IScnRender* render = reinterpret_cast<IScnRender*>(this);
        if (this != 0) {
            render = reinterpret_cast<IScnRender*>(&mScnRender);
        }
        mScene->removeRenderCB(render);
        spEveTalkWinInstance = 0;
    } else {
        sEveTalkWinTerminated = 0;
    }

    if (mpLayout != 0) {
        delete mpLayout;
        mpLayout = 0;
    }
    if (field_88 != 0) {
        delete field_88;
        field_88 = 0;
    }
    deleteRegion__17UnkClass_8045F564Fv(
        reinterpret_cast<UnkClass_8045F564*>(mMemRegion));
}

void CSimpleEveTalkWin::cbRenderBefore(CScn*) {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0)
        return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    drawLayout(mpLayout, &drawInfo, 0, 1);
}

// Creates the singleton CSimpleEveTalkWin on the work heap and registers it as
// a CProcess under `parent`. If the singleton already exists the message is
// handed to eveTalkWinSetReuseText instead and 0 is returned.
CSimpleEveTalkWin* eveTalkWinCreateRegister(CProcess* parent, CScn* scene, u32 text,
                                 const u8* msgSrc, u8 flag) {
    u8 buf[0x800];
    memset(buf, 0, 0x800);
    Utf8ToWcsCopyFix((const char*)msgSrc, (u16*)buf, 0x400);

    if (spEveTalkWinInstance != 0) {
        eveTalkWinSetReuseText(spEveTalkWinInstance, text, buf);
        return 0;
    }

    u32 workMem = CWorkThreadSystem::getWorkMem();
    CSimpleEveTalkWin* obj = (CSimpleEveTalkWin*)mtl::MemManager::allocate(
        0xb0, workMem);
    if (obj != 0) {
        obj = __ct__CSimpleEveTalkWin(obj, scene, text, buf, flag);
    }
    spEveTalkWinInstance = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return spEveTalkWinInstance;
}

// --- this-adjusting thunks (retail vtable dispatch: "this" lands at the
// embedded IWorkEvent/IScnRender subobject; back off to the object base and
// forward to the real member). eveTalkWinSetReuseText / eveTalkWinAdvancePage remain stubs.

extern "C" void eveTalkWinSetReuseText(CSimpleEveTalkWin* owner,
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
    ((UnkClass_8045F564*)owner->mMemRegion)->deleteRegion();

    // --- rebuild: identical to Init() below this line ---
    mtl::ALLOC_HANDLE mem = mtl::MemManager::getHandleMEM2();
    ((UnkClass_8045F564*)owner->mMemRegion)
        ->createRegion((int)mem, 0x4000, "CSimpleEveTalkWin", 0);
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
        ->Proc(owner->mMsgBuf, 38.0f, 38.0f,
               owner->field_68);

    CSimpleTagProc* tagFields =
        reinterpret_cast<CSimpleTagProc*>(owner->field_88);
    if (tagFields->field_0x814 == 1) {
        f32 voiceVol = tagFields->field_0x81C;
        u8 voiceId = tagFields->field_0x81A;
        CUICfManager_queueFadeMenu(voiceId, 0, voiceVol, -1.0f,
                      -1.0f);
        owner->field_64 = 1;
        __dt__14Class_8045F858Fv(regionGuard, -1);
        return;
    }

    owner->field_8C = 0;
    owner->field_90 = 0;
    owner->field_94 = 0;

    buildLayout(&owner->mpLayout, CUICfManager_getArcResourceAccessor(), "mf70_cf01_esywin00.brlyt");
    bindLayoutAnimTransform(owner->mpLayout, &owner->field_8C, CUICfManager_getArcResourceAccessor(),
                  "mf70_cf01_esywin00_in.brlan");
    bindLayoutAnimTransform(owner->mpLayout, &owner->field_90, CUICfManager_getArcResourceAccessor(),
                  "mf70_cf01_esywin00_cursor_in.brlan");
    bindLayoutAnimTransform(owner->mpLayout, &owner->field_94, CUICfManager_getArcResourceAccessor(),
                  "mf70_cf01_esywin00_cursor_def.brlan");

    // Bind the font and hand the loaded font object over to the root pane.
    nw4r::lyt::Pane* rootPane = owner->mpLayout->GetRootPane();
    u8* fontObj = (u8*)getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
        1, owner->mpLayout);
    u32 fontResult =
        reinterpret_cast<CSimpleFontObj*>(fontObj)->getFontHandle();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);

    // Start with the cursor animation disabled and the cursor at frame 1.
    owner->mpLayout->SetAnimationEnable(owner->field_8C, 0);
    owner->mpLayout->SetAnimationEnable(owner->field_94, 0);
    owner->mpLayout->SetAnimationEnable(owner->field_90, 1);
    owner->field_90->SetFrame(0.0f);
    owner->field_AE = 4;

    // Co-op mode uses the 2P button texture.
    const char* texName;
    if (cf::CfGameManager::isClassicController(-1) != 0)
        texName = "mf00_com00_btn30_anim00.tpl";
    else
        texName = "mf00_com00_btn00_anim00.tpl";
    void* tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, texName, 0);
    if (tex != 0) {
        PaneSetTexPaletteByName(owner->mpLayout, (const char*)"pic_cursor00", (TPLPalette*)tex);
    }

    if (owner->field_68 != 0) {
        // Real message: fill the five name fields from the talk source, then
        // start the voice line.
        char* msgText = reinterpret_cast<char*>(
            reinterpret_cast<cf::CfObject*>(findObjectById(owner->field_68))
                ->CObjectParam_getParamPtr());
        LayoutSetTextBoxFmtValue(owner->mpLayout, "txt_name", msgText, 0);
        LayoutSetTextBoxFmtValue(owner->mpLayout, "txt_name_r", msgText, 0);
        LayoutSetTextBoxFmtValue(owner->mpLayout, "txt_name_l", msgText, 0);
        LayoutSetTextBoxFmtValue(owner->mpLayout, "txt_name_o", msgText, 0);
        LayoutSetTextBoxFmtValue(owner->mpLayout, "txt_name_u", msgText, 0);
        void* src = findObjectById(owner->field_68);
        if (src != 0) {
            reinterpret_cast<CSimpleTalkFields*>(src)->voice98->play(1, 0);
        }
    } else {
        // No talk source: leave the name fields empty.
        LayoutSetTextBoxFmtValue(owner->mpLayout, "txt_name",
                      "", 0);
        LayoutSetTextBoxFmtValue(owner->mpLayout, "txt_name_r",
                      "", 0);
        LayoutSetTextBoxFmtValue(owner->mpLayout, "txt_name_l",
                      "", 0);
        LayoutSetTextBoxFmtValue(owner->mpLayout, "txt_name_o",
                      "", 0);
        LayoutSetTextBoxFmtValue(owner->mpLayout, "txt_name_u",
                      "", 0);
    }

    // Attach the tag processor to the message text box and show it.
    LayoutSetTextBoxFmtValue(owner->mpLayout, "txt_mes",
                  "", 0);
    nw4r::lyt::Pane* textPane = owner->mpLayout->GetRootPane()
                                    ->FindPaneByName("txt_mes",
                                                     1);
    textPane->SetVisible(true);
    reinterpret_cast<nw4r::lyt::TextBox*>(textPane)->SetTagProcessor(
        reinterpret_cast<nw4r::ut::WideTagProcessor*>(owner->field_88));
    TagProcBuildOpenText(owner->field_88, textPane, 0);

    // Without a talk source the window is a bare frame: pin it to its default
    // position.
    if (owner->field_68 == 0) {
        owner->mpLayout->GetRootPane()->SetTranslate(nw4r::math::VEC3(
            0.0f, 193.0f, 0.0f));
    }

    ((UnkClass_8045F564*)owner->mMemRegion)->validateHeap();
    __dt__14Class_8045F858Fv(regionGuard, -1);
}

// Message-advance step (window state 2): re-fetch the message text box, ask
// the tag processor which page action the confirm button triggered and run it
// (4 = select accepted, 2 = page animation, 0/1/3 = page/close logic).
extern "C" void eveTalkWinAdvancePage(CSimpleEveTalkWin* self) {
    nw4r::lyt::Pane* pane =
        self->mpLayout->GetRootPane()->FindPaneByName("txt_mes",
                                                      1);
    CPad* pad = cf::CfGameManager::getCurrentPad();

    // In co-op the confirm button is Classic A (bit 21), otherwise Wiimote A
    // (bit 4); the pad flag fields are 2-bit counters, so test the extracted
    // pair for non-zero.
    int confirmBtn;
    if (cf::CfGameManager::isClassicController(-1) != 0)
        confirmBtn = (pad->mPressedButtonFlags & 0x00600000) != 0;
    else
        confirmBtn = (pad->mPressedButtonFlags & 0x00000030) != 0;

    int state;
    if (confirmBtn != 0)
        state = TagProcPumpSEMessage(self->field_88, pane);
    else
        state = TagProcStepSEMessage(self->field_88, pane);

    if (state == 4) {
        // Selection accepted: play the confirm voice.
        TagProcStartMessage(self->field_88, pane);
        void* src = findObjectById(self->field_68);
        if (src != 0) {
            reinterpret_cast<CSimpleTalkFields*>(src)->voice98->play(1, 0);
        }
    } else if (state == 2) {
        // Page transition animations (4 -> 5 -> 6 -> 7 -> 4 loop).
        switch (self->field_AE) {
        case 4:
            self->mpLayout->SetAnimationEnable(self->field_8C, 0);
            self->mpLayout->SetAnimationEnable(self->field_94, 0);
            self->mpLayout->SetAnimationEnable(self->field_90, 1);
            self->field_90->SetFrame(0.0f);
            self->field_AE = 5;
            break;
        case 5:
            if (advanceAnimTransform(self->field_90, 1.0f) != 0) {
                self->mpLayout->SetAnimationEnable(self->field_8C, 0);
                self->mpLayout->SetAnimationEnable(self->field_90, 0);
                self->mpLayout->SetAnimationEnable(self->field_94, 1);
                self->field_94->SetFrame(0.0f);
                self->field_AE = 6;
            }
            break;
        case 6:
            advanceAnimTransform(self->field_94, 1.0f);
            break;
        case 7:
            if (AnimRewindFrame(self->field_90, 1.0f) != 0) {
                self->mpLayout->SetAnimationEnable(self->field_8C, 0);
                self->mpLayout->SetAnimationEnable(self->field_94, 0);
                self->mpLayout->SetAnimationEnable(self->field_90, 1);
                self->field_90->SetFrame(0.0f);
                self->field_AE = 4;
            }
            break;
        }
    } else if (state == 0 || self->field_65 != 0 || self->field_66 != 0) {
        // Page advance: stop the current voice, switch to the next page's
        // animation set and let the window close when the page ends.
        void* src = findObjectById(self->field_68);
        if (src != 0) {
            reinterpret_cast<CSimpleTalkFields*>(src)->voice98->play(0, 0);
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

extern "C" void __dt__17CSimpleEveTalkWinFv(void*);
extern "C" void cbRenderBefore__17CSimpleEveTalkWinFP4CScn(void*);

// Spoof thunks -- retail `subi/b` are compiler thunks for the
// IWorkEvent@0x6C / IScnRender@0x70 bases. Plain `p->~Class()` lowers to
// `stwu/mfspr/addic./beq/bl` (0x50), not 0x8. The cast tail-calls the
// flat `__dt__`/`cbRenderBefore` and is §17.6 high-level C++ (extern "C"
// REL24, no asm). With real bases the ideal would be
// `static_cast<CSimpleEveTalkWin*>(base)->~...`, today 0x20/0x9c.
void eveTalkWinDtorThunk6C(u8* self) {
    ((void (*)(void*))__dt__17CSimpleEveTalkWinFv)((char*)self - 0x6c);
}

void eveTalkWinRenderBeforeThunk(u8* self) {
    ((void (*)(void*))cbRenderBefore__17CSimpleEveTalkWinFP4CScn)((char*)self - 0x70);
}

void eveTalkWinDtorThunk70(u8* self) {
    ((void (*)(void*))__dt__17CSimpleEveTalkWinFv)((char*)self - 0x70);
}
