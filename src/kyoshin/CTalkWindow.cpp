// CTalkWindow - talk-window process (unit kyoshin/CTalkWindow).
//
// Real MI class (IUIWindow + IWorkEvent + IScnRender, see the header): the
// composite vtable __vt__11CTalkWindow (0x8052DFA8), its 0x40 base list, the
// weak RTTI record, the +0x6C/+0x70 adjusting thunks, the "CTalkWindow"
// type-name string and the TU static initializer that fills TalkWinPagePos
// are compiler output.

#include <nw4r/lyt/lyt_drawInfo.h>

// The retail CTalkWindow destructor calls __dt__8CProcessFv directly: the
// IUIWindow / CTTask<IUIWindow> / IWorkEvent destructor bodies are inlined
// into this TU (cs. CSysWinBuff / CSysWinSave).
#define IWORK_EVENT_INLINE_DTOR
#define CTTASK_INLINE_DTOR
#define IUIWINDOW_INLINE_DTOR
#include "kyoshin/CTalkWindow.hpp"
#include "monolib/lib/CLibLayout.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/device/CDeviceVI.hpp"

#include <revolution/gx/GXPixel.h>
#include <new>
#include <string.h>
#include "kyoshin/code_80135FDC_Lyt.hpp"
// .data (retail 0x8052DF70): page-pane names indexed by field_A4. The two
// short literals are also spelled directly in Init, hence the duplicate
// .sdata2 copies retail carries.
const char* TalkWinPagePaneName[4] = {"win_n", "win_a", "win_n", "win_n"};

// Page-position table (retail .bss 0x80573A70, 0xC0). VEC3 has a user ctor, so
// the initializer is a dynamic one: the compiler reserves .bss and emits the
// TU static initializer (retail sinit_8012DE98) that stores the 48 floats.
nw4r::math::VEC3 TalkWinPagePos[16] = {
    nw4r::math::VEC3(0.0f, 0.0f, 0.0f),
    nw4r::math::VEC3(-320.0f, 240.0f, 0.0f),
    nw4r::math::VEC3(0.0f, 240.0f, 0.0f),
    nw4r::math::VEC3(320.0f, 240.0f, 0.0f),
    nw4r::math::VEC3(-320.0f, 0.0f, 0.0f),
    nw4r::math::VEC3(0.0f, 0.0f, 0.0f),
    nw4r::math::VEC3(320.0f, 0.0f, 0.0f),
    nw4r::math::VEC3(-320.0f, -240.0f, 0.0f),
    nw4r::math::VEC3(0.0f, -240.0f, 0.0f),
    nw4r::math::VEC3(320.0f, -240.0f, 0.0f),
    nw4r::math::VEC3(0.0f, 0.0f, 0.0f),
    nw4r::math::VEC3(0.0f, 0.0f, 0.0f),
    nw4r::math::VEC3(0.0f, 0.0f, 0.0f),
    nw4r::math::VEC3(0.0f, 0.0f, 0.0f),
    nw4r::math::VEC3(0.0f, 0.0f, 0.0f),
    nw4r::math::VEC3(0.0f, 0.0f, 0.0f),
};

// .sbss (retail 0x80664040): page-mode flag byte + live talk-window counter.
u8 lbl_eu_80664040 = 0;
u32 lbl_eu_80664044 = 0;

// ---------------------------------------------------------------------------
// CTalkWindow::CTalkWindow (us-8012c710)
// CProcess base, then the inlined CTTask<IUIWindow>/IUIWindow base ctors (base
// vptr + null Move/Draw pmf hooks + layout/state members), the composite and
// sub-vptrs, the embedded layout-build region and the animation/state fields.
// The window id (IUIWindow::mField68) is stored last.
// ---------------------------------------------------------------------------
CTalkWindow::CTalkWindow(CScn* scene, u32 sourceId, u8* msg, u32 a0, u32 a1,
                         u32 b4)
    : mScene(scene), mMemRegion(), field_88(0), field_8C(0), field_90(0),
      field_94(0), field_98(0), field_9C(msg), field_A0(a0), field_A1(a1),
      field_A4(0), field_A8(0), field_AC(0), field_B0(1), field_B4(b4) {
    mField68 = sourceId;
}

// ---------------------------------------------------------------------------
// ~CTalkWindow (us-8012c828)
// Complete-object destructor. The embedded layout-build region is destroyed
// (flags -1), then the CProcess base guarded by the nested double null-check
// (an MWCC D2-inlined-into-D1 artifact; the IUIWindow/CTTask/IWorkEvent base
// dtors are the inline-empty variants selected by the macros at the top of
// this file), then the deleting path calls operator delete. Same shape as
// ~CSysWinSave.
// ---------------------------------------------------------------------------
CTalkWindow::~CTalkWindow() {}

// (lbl_eu_80664044 is the live talk-window counter)
extern "C" void TalkWin_ClearFlag_BDD0() { lbl_eu_80664044 = 0; }

// ---------------------------------------------------------------------------
// CTalkWindow::Init (us-8012c8a8)
// Open a scratch region (Class_8045F858 RAII guard), allocate and prime the
// tag processor, then dispatch on its +0x814 mode byte. Mode 7 builds the
// full talk layout (5 animations, font binding, message panes, corner
// pictures), wires the page panes and registers the render callback.
//
// Matching notes from the retail ASM (for the next iteration):
//  - Retail holds `this` in r25 (_savegpr_25) and the tag processor in r29
//    across the whole case-7 body; the decomp currently copies this into
//    r29 first, shifting every later register assignment.
//  - Every pane show/hide compiles to `lbz 0xbb; rlwinm r0,r0,0,24,30
//    [; ori 1]; stb` - i.e. (old & 0x7F) [| 1]: bit 7 of the flag byte is
//    ALWAYS cleared. That shape needs two merged bitfield writes on CTalkWinPane
//    (a :7 field + a :1 field both assigned per site); plain `u8 mFlag |= 1`
//    gives lbz/ori/stb without the mask.
//  - Font binding order: retail loads GetRootPane() BEFORE calling
//    getFontInfo(1, layout) (hoist into its own statement).
//  - The literal pool base is materialised once into r30 before the
//    five bindLayoutAnimTransform calls, then rematerialised (r30/r28/r27/r30 again)
//    at each section boundary after Animate() - the source likely redeclares
//    a base pointer per scope rather than sharing one.
//  - Member calls on reinterpret_cast<UnkClass_8045F564*> (createRegion /
//    validateHeap) add null-check branches retail lacks; extern "C"
//    pre-mangled declarations avoid that but shift other allocation.
// ---------------------------------------------------------------------------
void CTalkWindow::Init() {
    mMemRegion.createRegion(
        mtl::MemManager::getHandleMEM2(), 0x4000, "CTalkWindow", 0);
    Class_8045F858 guard(&mMemRegion);

    lbl_eu_80664044++;

    void* tagMem = mtl::MemManager::allocate(
        0x858, CLibLayout::getAllocHandle());
    if (tagMem != 0) {
        tagMem = __ct__CTagProcessor(tagMem);
    }
    mField5C = reinterpret_cast<nw4r::lyt::AnimTransform*>(tagMem);
    CTalkWinTagProc* tagProc = reinterpret_cast<CTalkWinTagProc*>(tagMem);
    tagProc->Proc(reinterpret_cast<u8*>(field_9C), 38.0f,
                  38.0f, mField68);

    // Early-init dispatch on the tag-processor mode byte (+0x814). Modes 0-6
    // are small setups; mode 7 builds the layout; 3/8 just mark ready.
    u8 type = tagProc->field_0x814;
    switch (type) {
    case 0:
        CUICfManager_queueFadeMenu(tagProc->field_0x81A, 0, tagProc->field_0x81C,
                      -1.0f, -1.0f);
        mField64 = 1;
        break;
    case 1:
        UIWin_CreateQuestWin(tagProc->field_0x818, 0, 0);
        mField64 = 1;
        break;
    case 2: {
        cf::CfObject* s2 = reinterpret_cast<cf::CfObject*>(
            findObjectById(mField68));
        if (s2 != 0) {
            s2->setPointEnabled(0);
        }
        mField64 = 1;
        break;
    }
    case 3:
    case 8:
        mField64 = 1;
        break;
    case 4: {
        cf::CfObject* s4 = reinterpret_cast<cf::CfObject*>(
            findObjectById(mField68));
        UIWin_CreateBEDE0Win(
            reinterpret_cast<CTalkActorId*>(s4)->msgId8C);
        mField64 = 1;
        break;
    }
    case 5:
        UIWin_CreateShopWin(tagProc->field_0x818);
        mField64 = 1;
        break;
    case 6:
        CUICfManager_queueMakeCrystalMenu();
        mField64 = 1;
        break;
    case 7: {
        // Full layout build: page index, layout + 5 animations from the
        // shared arc (re-acquired per build call), font binding, then the
        // message / corner panes.
        field_A4 = tagProc->field_0x816;

        buildLayout(&mLayout, CUICfManager_getArcResourceAccessor(), "mf70_cf00_twin00.brlyt");
        bindLayoutAnimTransform(mLayout, &field_88, CUICfManager_getArcResourceAccessor(), "mf70_cf00_twin00_in.brlan");
        bindLayoutAnimTransform(mLayout, &field_8C, CUICfManager_getArcResourceAccessor(), "mf70_cf00_twin00_def.brlan");
        bindLayoutAnimTransform(mLayout, &field_90, CUICfManager_getArcResourceAccessor(), "mf70_cf00_twin00_out.brlan");
        bindLayoutAnimTransform(mLayout, &field_94, CUICfManager_getArcResourceAccessor(), "mf70_cf00_twin00_fadeout.brlan");
        bindLayoutAnimTransform(mLayout, &field_98, CUICfManager_getArcResourceAccessor(), "mf70_cf00_twin00_fadein.brlan");

        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
            1, mLayout);
        ApplyFontToTxtPanes(mLayout->GetRootPane(),
                      (const nw4r::ut::Font*)(reinterpret_cast<CTalkFontObj*>(fontObj)->getFontHandle()));

        // Prime the message-window pane and bind the four corner panes.
        CTalkWinPane* paneB0 = reinterpret_cast<CTalkWinPane*>(
            mLayout->GetRootPane()->FindPaneByName("txt_name", 1));
        paneB0->FreeStringBuffer();
        paneB0->AllocStringBuffer(0x20);

        char* msg = GetBdatNameByNumId(reinterpret_cast<char*>(
            reinterpret_cast<cf::CfObject*>(
                findObjectById(mField68))->CObjectParam_getParamPtr()));
        LayoutSetTextBoxFmtValue(mLayout, "txt_name", msg, 0);
        LayoutSetTextBoxFmtValue(mLayout, "txt_shadow_01", msg, 0);
        LayoutSetTextBoxFmtValue(mLayout, "txt_shadow_02", msg, 0);
        LayoutSetTextBoxFmtValue(mLayout, "txt_shadow_03", msg, 0);
        LayoutSetTextBoxFmtValue(mLayout, "txt_shadow_04", msg, 0);

        // Page-select pane: attach the tag processor and name the sub-panes.
        CTalkWinPane* paneF1 = reinterpret_cast<CTalkWinPane*>(
            mLayout->GetRootPane()->FindPaneByName("txt_mess00", 1));
        paneF1->FreeStringBuffer();
        paneF1->AllocStringBuffer(0x400);
        paneF1->mFlag |= 1;
        paneF1->field_F8 = reinterpret_cast<u32>(mField5C);
        LayoutSetTextBoxFmtValue(mLayout, "txt_mess01", "", 0);
        LayoutSetTextBoxFmtValue(mLayout, "txt_mess02", "", 0);
        LayoutSetTextBoxFmtValue(mLayout, "txt_mess03", "", 0);

        TagProcRenderPage(mField5C,
                      mLayout->GetRootPane()->FindPaneByName(
                          "txt_mess00", 1),
                      mLayout->GetRootPane()->FindPaneByName(
                          TalkWinPagePaneName[field_A4], 1),
                      mLayout->GetRootPane()->FindPaneByName(
                          "txt_name", 1), 0);

        updateTalkWindow(this);
        layoutTalkPanes(this);

        // Hide the arrow panes, then reveal the active side.
        reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
            FindPaneByName("pic_tail_over", 1))->mFlag &= ~1;
        reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
            FindPaneByName("pic_tail_under", 1))->mFlag &= ~1;
        if (field_A1 != 0) {
            reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                FindPaneByName("pic_tail_over", 1))->mFlag |= 1;
        } else {
            reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                FindPaneByName("pic_tail_under", 1))->mFlag |= 1;
        }

        // Default animation set: intro anim active, page anims off.
        if (field_A0 == 0) {
            lbl_eu_80664040 = 0;
            mLayout->SetAnimationEnable(field_8C, false);
            mLayout->SetAnimationEnable(field_90, false);
            mLayout->SetAnimationEnable(field_94, false);
            mLayout->SetAnimationEnable(field_98, false);
            mLayout->SetAnimationEnable(field_88, true);
            reinterpret_cast<CTalkAnimFrame*>(field_88)->mFrame =
                0.0f;
        }
        mLayout->Animate(0);

        // Page-mode banner panes + the shared 'timg' page texture.
        u8 opt = tagProc->field_0x815;
        reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
            FindPaneByName("win_n", 1))->mFlag |= 1;
        reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
            FindPaneByName("win_a", 1))->mFlag &= ~1;
        void* res = CUICfManager_getArcResourceAccessor()->GetResource(
            0x74696d67, "mf70_cf00_twin_tail_n.tpl", 0);
        CUICfManager_getArcResourceAccessor()->GetResource(
            0x74696d67, "mf70_cf00_twin_line_n.tpl", 0);
        playUISound(0x29);

        // Mode-specific page setup (retail keeps the type==0 guard as dead
        // code: type is always 7 on this path).
        if (type != 0) {
            if ((opt & 0x4) != 0) {
                if (field_A4 == 0) {
                    reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                        FindPaneByName("win_n", 1))->mFlag |= 1;
                    reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                        FindPaneByName("win_a", 1))->mFlag &= ~1;
                    res = CUICfManager_getArcResourceAccessor()->GetResource(
                        0x74696d67, "mf70_cf00_twin_tail_n.tpl", 0);
                    CUICfManager_getArcResourceAccessor()->GetResource(
                        0x74696d67, "mf70_cf00_twin_line_n.tpl", 0);
                    playUISound(0x29);
                } else if (field_A4 == 1) {
                    reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                        FindPaneByName("win_n", 1))->mFlag &= ~1;
                    reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                        FindPaneByName("win_a", 1))->mFlag |= 1;
                    res = CUICfManager_getArcResourceAccessor()->GetResource(
                        0x74696d67, "mf70_cf00_twin_tail_a.tpl", 0);
                    CUICfManager_getArcResourceAccessor()->GetResource(
                        0x74696d67, "mf70_cf00_twin_line_a.tpl", 0);
                    lbl_eu_80664040 = 1;
                    reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                        FindPaneByName("pic_tail_over", 1))->mFlag &= ~1;
                    reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                        FindPaneByName("pic_tail_under", 1))->mFlag &= ~1;
                    playUISound(0x72);
                } else if (field_A4 == 3) {
                    reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                        FindPaneByName("win_n", 1))->mFlag |= 1;
                    reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                        FindPaneByName("win_a", 1))->mFlag &= ~1;
                    res = CUICfManager_getArcResourceAccessor()->GetResource(
                        0x74696d67, "mf70_cf00_twin_tail_t.tpl", 0);
                    CUICfManager_getArcResourceAccessor()->GetResource(
                        0x74696d67, "mf70_cf00_twin_line_t.tpl", 0);
                    playUISound(0x74);
                }
            }
            if ((opt & 0x1) != 0) {
                lbl_eu_80664040 = 1;
                reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                    FindPaneByName("pic_tail_over", 1))->mFlag &= ~1;
                reinterpret_cast<CTalkWinPane*>(mLayout->GetRootPane()->
                    FindPaneByName("pic_tail_under", 1))->mFlag &= ~1;
            }
            if ((opt & 0x2) != 0) {
                field_A8 = tagProc->field_0x817;
            }
        }

        // Bind the page texture to the two arrow panes.
        if (res != 0) {
            PaneSetTexPaletteByName(mLayout, "pic_tail_over", (TPLPalette*)res);
            PaneSetTexPaletteByName(mLayout, "pic_tail_under", (TPLPalette*)res);
        }

        mScene->addRenderCB(this, 0xa, 0);
        break;
    }
    default:
        break;
    }

    mMemRegion.validateHeap();
}

// ---------------------------------------------------------------------------
// CTalkWindow::Term (us-8012d2c4)
// Wait for VI draw completion, drop the instance counter, detach the render
// callback, delete the two layout objects and release the layout-build
// scratch region.
// ---------------------------------------------------------------------------
void CTalkWindow::Term() {
    CDeviceVI::waitForDrawDone();
    lbl_eu_80664044--;
    mScene->removeRenderCB(this);
    if (mField5C != 0) {
        delete reinterpret_cast<nw4r::lyt::AnimTransform*>(mField5C);
        mField5C = 0;
    }
    if (mLayout != 0) {
        delete mLayout;
        mLayout = 0;
    }
    mMemRegion.deleteRegion();
}

// ---------------------------------------------------------------------------
// CTalkWindow::Move (us-8012d374)
// Per-frame driver: gate on the task/global flags, run the shared
// updateTalkWindow update, dispatch the state machine (1..5), then always
// advance the layout animations.
// ---------------------------------------------------------------------------
void CTalkWindow::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000)) return;
    if (IsMenuState621F0() == 0) return;
    if (cf::CfGameManager::isSceneLoading() != 0) return;
    updateTalkWindow(this);

    // Signed switch: retail emits a beq dispatch chain with signed cmpwi.
    switch ((int)field_B0) {
    case 1:
        driveTalkIntro(this);
        break;
    case 2:
        driveTalkMessage(this);
        break;
    case 3:
        if (advanceAnimTransform(field_90, 1.0f) != 0) {
            mScene->removeRenderCB(this);
            mField64 = 1;
            field_B0 = 0;
        }
        break;
    case 4: {
        if (advanceAnimTransform(field_94, 1.0f) == 0) break;
        mLayout->SetAnimationEnable(field_90, false);
        mLayout->SetAnimationEnable(field_94, false);
        mLayout->SetAnimationEnable(field_88, false);
        mLayout->SetAnimationEnable(field_8C, false);
        mLayout->SetAnimationEnable(field_98, true);
        reinterpret_cast<CTalkAnimFrame*>(field_98)->mFrame = 0.0f;
        // Declared p2-first: MWCC allocates the two pane locals in reverse
        // declaration order, matching retail's r28/r29 split.
        nw4r::lyt::Pane* p2;
        nw4r::lyt::Pane* p1;
        p1 = mLayout->GetRootPane()->
            FindPaneByName("txt_mess00", 1);
        p2 = mLayout->GetRootPane()->FindPaneByName(
            TalkWinPagePaneName[field_A4], 1);
        nw4r::lyt::Pane* p3 = mLayout->GetRootPane()->FindPaneByName(
            "txt_name", 1);
        TagProcResetPage(reinterpret_cast<nw4r::lyt::AnimTransform*>(mField5C), p1, p2, p3);
        layoutTalkPanes(this);
        field_B0 = 5;
        break;
    }
    case 5:
        if (advanceAnimTransform(field_98, 1.0f) != 0) {
            mLayout->SetAnimationEnable(field_90, false);
            mLayout->SetAnimationEnable(field_94, false);
            mLayout->SetAnimationEnable(field_88, false);
            mLayout->SetAnimationEnable(field_98, false);
            mLayout->SetAnimationEnable(field_8C, true);
            reinterpret_cast<CTalkAnimFrame*>(field_8C)->mFrame =
                0.0f;
            field_B0 = 2;
        }
        break;
    default:
        break;
    }

    mLayout->Animate(0);
}

void CTalkWindow::Draw() {}

// ---------------------------------------------------------------------------
// CTalkWindow::cbRenderBefore (us-8012d6a0)
// Gate prefix (task busy / global bit / scene-active / talk source) then draw
// the layout through a stack DrawInfo.
// ---------------------------------------------------------------------------
void CTalkWindow::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0)
        return;
    if (findObjectById((int)mField68) == 0)
        return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    drawLayout(mLayout, &drawInfo, 0, 1);
}

// ---------------------------------------------------------------------------
// TalkWin_Create_CC78 (us-8012d744)
// Talk-window factory: copy the message source into a zeroed stack buffer,
// allocate the 0xB8-byte CTalkWindow on the work heap, run the ctor and
// register it under the caller's parent.
// ---------------------------------------------------------------------------
extern "C" CTalkWindow* TalkWin_Create_CC78(CProcess* parent, u32 arg1, u32 arg2,
                                      const u8* msgSrc, u32 arg3, u32 arg4,
                                      u32 arg5) {
    u8 buf[0x800];
    memset(buf, 0, 0x800);
    Utf8ToWcsCopyFix((const char*)msgSrc, (u16*)buf, 0x400);

    u32 workMem = CWorkThreadSystem::getWorkMem();
    CTalkWindow* obj = new (mtl::MemManager::allocate(0xb8, workMem))
        CTalkWindow((CScn*)arg1, arg2, buf, arg3, arg4, arg5);
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return obj;
}

extern "C" bool TalkWin_IsActive_CD24() {
    u32 v = lbl_eu_80664044;
    return ((-v) & ~v) >> 31;
}

extern "C" void updateTalkWindow(CTalkWindow* self) {
    cf::CfObject* src =
        reinterpret_cast<cf::CfObject*>(findObjectById(self->mField68));
    if (src == 0) {
        self->mScene->removeRenderCB(self);
        self->mField64 = 1;
        self->field_B0 = 0;
        return;
    }

    // Local declaration order mirrors the retail stack layout. MWCC assigns
    // ascending slots in reverse decl order: the corner-UV table (declared
    // last) takes the top region (+0x178..+0x274), then world down to sy2
    // (+0x16c..+0xc). Indices 0..7 = pane1-B1..B4, pane2-B1..B4.
    nw4r::math::VEC3 world;    // 0x16c
    nw4r::math::VEC3 screen;   // 0x160
    nw4r::math::VEC3 trans1;   // 0x154
    nw4r::math::VEC3 trans2;   // 0x148
    nw4r::math::VEC3 tmp;      // 0x13c
    nw4r::math::VEC3 table;    // 0x130
    f32 scaleY;                // 0x12c
    f32 scaleX;                // 0x128
    f32 sx1;                   // 0x120
    f32 sx2;                   // 0x118
    f32 sy1;                   // 0x14
    f32 sy2;                   // 0xc
    nw4r::math::VEC2 cornerUVs[8][4];  // 0x178
    f32 dx;

    // Resolve the talk-source anchor: the named-position lookup (0x120)
    // yields strided components (+0xc/+0x1c/+0x2c), else the +0xAC fallback.
    const nw4r::math::VEC3* wsrc;
    CTalkWinPosObj* obj = reinterpret_cast<CTalkWinPosObj*>(
        src->CfObject_findNodeMatrix("ep100"));
    if (obj != 0) {
        // Load the strided components in retail's z,y,x order.
        f32 tz = obj->field_0x2C;
        f32 ty = obj->field_0x1C;
        f32 tx = obj->field_0x0C;
        tmp.x = tx;
        tmp.y = ty;
        tmp.z = tz;
        wsrc = &tmp;
    } else {
        wsrc = reinterpret_cast<const nw4r::math::VEC3*>(
            src->CfObject_getPosVector());
    }
    world.x = wsrc->x;
    world.y = wsrc->y;
    world.z = wsrc->z;

    // Project the world anchor through the scene camera into screen space.
    CTalkWinPose* pose = Scn_FindCamItem(self->mScene, -1);
    CamMan_ProjectToScreen(&screen, pose, &world);

    screen.z = 0.0f;
    screen.x = screen.x - 320.0f;
    screen.y = (screen.y - 224.0f) * -1.0f;
    world = screen;  // pre-clamp copy (dx uses world.x later)

    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->
        FindPaneByName(TalkWinPagePaneName[self->field_A4], 1);
    screen.y = screen.y + pane->GetScale().y * 0.5f;
    screen.y = screen.y + 20.0f;
    scaleX = pane->GetScale().x;
    scaleY = pane->GetScale().y;

    if (self->field_A1 != 0) {
        screen.y *= -1.0f;
    }
    if (self->field_A8 != 0) {
        // Override the projected position with the page-position table entry.
        table = TalkWinPagePos[self->field_A8];
        screen = table;
    }

    // Clamp the projected point inside the pane bounds (expanded forms match
    // the retail fmadds/fnmsubs folds).
    if (screen.x - scaleX * 0.5f < -320.0f) {
        screen.x = screen.x +
                   (-320.0f - (screen.x - scaleX * 0.5f));
    } else if (screen.x + scaleX * 0.5f > 320.0f) {
        screen.x = screen.x -
                   ((screen.x + scaleX * 0.5f) - 320.0f);
    }
    if (screen.y + scaleY * 0.5f > 224.0f) {
        screen.y = screen.y -
                   ((screen.y + scaleY * 0.5f) - 224.0f);
    } else if (screen.y - scaleY * 0.5f < -224.0f) {
        screen.y = screen.y +
                   (-224.0f - (screen.y - scaleY * 0.5f));
    }

    if (self->field_A1 != 0 && self->field_A8 == 0) {
        screen.y += 50.0f;
    }

    self->mLayout->GetRootPane()->SetTranslate(screen);
    dx = world.x - screen.x;
    if (-200.0f <= world.x && world.x <= 200.0f) {
        dx = 0.0f;
    }

    // Corner-picture UV flips: the sign of dx (and the pane translate for the
    // centred case) picks one of the four corner rotations.
    nw4r::lyt::Pane* p1 = self->mLayout->GetRootPane()->
        FindPaneByName("pic_tail_over", 1);
    if (p1 != 0) {
        trans1 = p1->GetTranslate();
        sx1 = p1->GetScale().x;
        sy1 = p1->GetScale().y;
        if (dx > 0.0f) {
            cornerUVs[0][0] = nw4r::math::VEC2(1.0f, 1.0f);
            cornerUVs[0][1] = nw4r::math::VEC2(0.0f, 1.0f);
            cornerUVs[0][2] = nw4r::math::VEC2(1.0f, 0.0f);
            cornerUVs[0][3] = nw4r::math::VEC2(0.0f, 0.0f);
            reinterpret_cast<nw4r::lyt::Picture*>(p1)->SetTexCoord(0, cornerUVs[0]);
            goto p1done;
        }
        if (dx < 0.0f) {
            cornerUVs[1][0] = nw4r::math::VEC2(0.0f, 1.0f);
            cornerUVs[1][1] = nw4r::math::VEC2(1.0f, 1.0f);
            cornerUVs[1][2] = nw4r::math::VEC2(0.0f, 0.0f);
            cornerUVs[1][3] = nw4r::math::VEC2(1.0f, 0.0f);
            reinterpret_cast<nw4r::lyt::Picture*>(p1)->SetTexCoord(0, cornerUVs[1]);
            goto p1done;
        }
        if (trans1.x <= 0.0f) {
            cornerUVs[2][0] = nw4r::math::VEC2(1.0f, 1.0f);
            cornerUVs[2][1] = nw4r::math::VEC2(0.0f, 1.0f);
            cornerUVs[2][2] = nw4r::math::VEC2(1.0f, 0.0f);
            cornerUVs[2][3] = nw4r::math::VEC2(0.0f, 0.0f);
            reinterpret_cast<nw4r::lyt::Picture*>(p1)->SetTexCoord(0, cornerUVs[2]);
        } else {
            cornerUVs[3][0] = nw4r::math::VEC2(0.0f, 1.0f);
            cornerUVs[3][1] = nw4r::math::VEC2(1.0f, 1.0f);
            cornerUVs[3][2] = nw4r::math::VEC2(0.0f, 0.0f);
            cornerUVs[3][3] = nw4r::math::VEC2(1.0f, 0.0f);
            reinterpret_cast<nw4r::lyt::Picture*>(p1)->SetTexCoord(0, cornerUVs[3]);
        }
p1done:;
    }

    nw4r::lyt::Pane* p2 = self->mLayout->GetRootPane()->
        FindPaneByName("pic_tail_under", 1);
    if (p2 != 0) {
        trans2 = p2->GetTranslate();
        sx2 = p2->GetScale().x;
        sy2 = p2->GetScale().y;
        if (dx > 0.0f) {
            cornerUVs[4][0] = nw4r::math::VEC2(1.0f, 0.0f);
            cornerUVs[4][1] = nw4r::math::VEC2(0.0f, 0.0f);
            cornerUVs[4][2] = nw4r::math::VEC2(1.0f, 1.0f);
            cornerUVs[4][3] = nw4r::math::VEC2(0.0f, 1.0f);
            reinterpret_cast<nw4r::lyt::Picture*>(p2)->SetTexCoord(0, cornerUVs[4]);
            goto p2done;
        }
        if (dx < 0.0f) {
            cornerUVs[5][0] = nw4r::math::VEC2(0.0f, 0.0f);
            cornerUVs[5][1] = nw4r::math::VEC2(1.0f, 0.0f);
            cornerUVs[5][2] = nw4r::math::VEC2(0.0f, 1.0f);
            cornerUVs[5][3] = nw4r::math::VEC2(1.0f, 1.0f);
            reinterpret_cast<nw4r::lyt::Picture*>(p2)->SetTexCoord(0, cornerUVs[5]);
            goto p2done;
        }
        if (trans2.x <= 0.0f) {
            cornerUVs[6][0] = nw4r::math::VEC2(1.0f, 0.0f);
            cornerUVs[6][1] = nw4r::math::VEC2(0.0f, 0.0f);
            cornerUVs[6][2] = nw4r::math::VEC2(1.0f, 1.0f);
            cornerUVs[6][3] = nw4r::math::VEC2(0.0f, 1.0f);
            reinterpret_cast<nw4r::lyt::Picture*>(p2)->SetTexCoord(0, cornerUVs[6]);
        } else {
            cornerUVs[7][0] = nw4r::math::VEC2(0.0f, 0.0f);
            cornerUVs[7][1] = nw4r::math::VEC2(1.0f, 0.0f);
            cornerUVs[7][2] = nw4r::math::VEC2(0.0f, 1.0f);
            cornerUVs[7][3] = nw4r::math::VEC2(1.0f, 1.0f);
            reinterpret_cast<nw4r::lyt::Picture*>(p2)->SetTexCoord(0, cornerUVs[7]);
        }
p2done:;
    }
}

// ---------------------------------------------------------------------------
// layoutTalkPanes (us-8012dea8)
// Place the four corner message panes around the reference pane (0xb0) and
// lay out the remaining page panes with scale-derived offsets, using the
// page-name table (lbl_eu_8052DF70) for the final pane.
// ---------------------------------------------------------------------------
void layoutTalkPanes(CTalkWindow* self) {
    nw4r::lyt::Pane* root = self->mLayout->GetRootPane();
    nw4r::lyt::Pane* ref =
        root->FindPaneByName("txt_name", 1);

    f32 tx = ref->GetTranslate().x;
    f32 ty = ref->GetTranslate().y;
    f32 tz = ref->GetTranslate().z;
    f32 sx = ref->GetScale().x;
    f32 sy = ref->GetScale().y;

    nw4r::lyt::Pane* p2 =
        root->FindPaneByName("txt_shadow_01", 1);
    p2->SetTranslate(nw4r::math::VEC3(tx - 0.8f,
                                      0.8f + ty, tz));
    p2->SetScale(nw4r::math::VEC2(sx, sy));

    nw4r::lyt::Pane* p3 =
        root->FindPaneByName("txt_shadow_02", 1);
    p3->SetTranslate(nw4r::math::VEC3(0.8f + tx,
                                      0.8f + ty, tz));
    p3->SetScale(nw4r::math::VEC2(sx, sy));

    nw4r::lyt::Pane* p4 =
        root->FindPaneByName("txt_shadow_03", 1);
    p4->SetTranslate(nw4r::math::VEC3(0.8f + tx,
                                      ty - 0.8f, tz));
    p4->SetScale(nw4r::math::VEC2(sx, sy));

    nw4r::lyt::Pane* p5 =
        root->FindPaneByName("txt_shadow_04", 1);
    p5->SetTranslate(nw4r::math::VEC3(tx - 0.8f,
                                      ty - 0.8f, tz));
    p5->SetScale(nw4r::math::VEC2(sx, sy));

    nw4r::lyt::Pane* p6 =
        root->FindPaneByName(TalkWinPagePaneName[self->field_A4], 1);
    p6->SetTranslate(nw4r::math::VEC3(tx - 0.8f,
                                      ty - 0.8f, tz));
    p6->SetScale(nw4r::math::VEC2(sx, sy));

    // Centre-pane scale: pane6's scale.x scaled to a layout offset, flipped
    // when the root pane sits on the negative side of the origin.
    f32 g = p6->GetScale().x / 3.0f * 0.5f;
    if (root->GetTranslate().x <= 0.0f) {
        g *= -1.0f;
    }

    nw4r::lyt::Pane* p7 =
        root->FindPaneByName("pic_tail_over", 1);
    p7->SetTranslate(nw4r::math::VEC3(
        g, p6->GetScale().y * 0.5f + p6->GetTranslate().y +
               p7->GetScale().y * 0.5f - 39.0f,
        p6->GetTranslate().z));

    nw4r::lyt::Pane* p8 =
        root->FindPaneByName("txt_mess00", 1);
    p8->SetTranslate(nw4r::math::VEC3(
        g, 39.0f + (p6->GetTranslate().y -
                               p6->GetScale().y * 0.5f -
                               p8->GetScale().y * 0.5f),
        p6->GetTranslate().z));

    nw4r::lyt::Pane* p9 =
        root->FindPaneByName("txt_mess01", 1);
    p9->SetTranslate(p8->GetTranslate());

    nw4r::lyt::Pane* p10 =
        root->FindPaneByName("txt_mess02", 1);
    p10->SetTranslate(nw4r::math::VEC3(
        p8->GetTranslate().x, p8->GetTranslate().y - 18.0f,
        p8->GetTranslate().z));

    nw4r::lyt::Pane* p11 =
        root->FindPaneByName("txt_mess03", 1);
    p11->SetTranslate(nw4r::math::VEC3(
        p8->GetTranslate().x, p8->GetTranslate().y - 36.0f,
        p8->GetTranslate().z));
}

// ---------------------------------------------------------------------------
// driveTalkIntro (us-8012e390)
// Window-state 1 driver: play the intro anim (field_88); when the talk-enable
// flag (field_B4) is set, run the character-voice playback through the talk
// source (with the page-id / talk-source checks), then switch the enabled
// animations to state 2.
// ---------------------------------------------------------------------------
void driveTalkIntro(CTalkWindow* self) {
    f32 frame = 1.0f;
    if (advanceAnimTransform(self->field_88, frame) == 0) return;
    if (self->field_B4 != 0) {
        if (code80135FDC_getByte_64058() == 0) {
            if (findObjectById(self->mField68) != 0) {
                CTalkWinTalkSrc* src =
                    reinterpret_cast<CTalkWinTalkSrc*>(func_800BBC0C());
                // Re-read mTalkC4 at each use: retail reloads the field
                // around the voice call (memory-clobber), and keeping it in
                // a local would shrink the body by two loads.
                if (src->mTalkC4 != 0) {
                    // Talk-stop flag: leave the intro playing.
                    if ((src->mTalkC4->field_270 & 0x80) != 0) return;
                    src->mVoice->play(1, 0);
                    int page = getAnimModelId(src->mTalkC4);
                    // Two separate compares branching to a shared call site
                    // (not a fused range test): retail keeps
                    // `cmpwi 0x21 / cmpwi 0x26` with blt/ble gates.
                    if (page < 0x21) goto do_b9d4;
                    if (page <= 0x26) goto skip_b9d4;
do_b9d4:
                    func_8004B9D4(src->mTalkC4, 0x1f, 0, -1, 0);
skip_b9d4:;
                }
            }
        } else if (code80135FDC_getByte_64059() != 0) {
            if (findObjectById(self->mField68) != 0) {
                CTalkWinTalkSrc* src =
                    reinterpret_cast<CTalkWinTalkSrc*>(func_800BBC0C());
                src->mVoice->play(1, 0);
            }
        }
    }
    self->mLayout->SetAnimationEnable(self->field_90, false);
    self->mLayout->SetAnimationEnable(self->field_94, false);
    self->mLayout->SetAnimationEnable(self->field_98, false);
    self->mLayout->SetAnimationEnable(self->field_88, false);
    self->mLayout->SetAnimationEnable(self->field_8C, true);
    self->field_B0 = 2;
}

// ---------------------------------------------------------------------------
// driveTalkMessage (us-8012e53c)
// Window-state 2 driver: find the three page panes, blend the tag-processor
// animation (TagProcPumpMessage / TagProcStepMessage depending on the confirm button),
// then dispatch the result: state 4 accepts the selection, otherwise the
// voice/page logic runs (or the page-loop flag flips).
// ---------------------------------------------------------------------------
void driveTalkMessage(CTalkWindow* self) {
    nw4r::lyt::Pane* root = self->mLayout->GetRootPane();
    nw4r::lyt::Pane* p1 =
        root->FindPaneByName("txt_mess00", 1);
    nw4r::lyt::Pane* p2 =
        root->FindPaneByName(TalkWinPagePaneName[self->field_A4], 1);
    nw4r::lyt::Pane* p3 =
        root->FindPaneByName("txt_name", 1);

    int state;
    if (GetSysStateFlag31() != 0) {
        state = TagProcPumpMessage(reinterpret_cast<nw4r::lyt::AnimTransform*>(self->mField5C), p1, p2, p3);
    } else {
        CTalkPadView* pad = reinterpret_cast<CTalkPadView*>(
            cf::CfGameManager::getCurrentPad());
        int confirm;
        // Co-op (Classic) vs single-player (Wiimote) confirm-press bits.
        if (cf::CfGameManager::isClassicController(-1) != 0)
            confirm = (pad->field_04 & 0x00600000) != 0;
        else
            confirm = (pad->field_04 & 0x00000030) != 0;
        if (confirm != 0)
            state = TagProcPumpMessage(reinterpret_cast<nw4r::lyt::AnimTransform*>(self->mField5C), p1, p2, p3);
        else
            state = TagProcStepMessage(reinterpret_cast<nw4r::lyt::AnimTransform*>(self->mField5C), p1, p2, p3);
    }

    if (state == 4) {
        // Selection accepted: play the confirm voice and switch to state 4.
        self->mLayout->SetAnimationEnable(self->field_88, false);
        self->mLayout->SetAnimationEnable(self->field_8C, false);
        self->mLayout->SetAnimationEnable(self->field_98, false);
        self->mLayout->SetAnimationEnable(self->field_90, false);
        self->mLayout->SetAnimationEnable(self->field_94, true);
        reinterpret_cast<CTalkAnimFrame*>(self->field_94)->mFrame =
            0.0f;
        playUISound(0x2b);
        self->field_B0 = 4;
    } else if (state == 0 || self->mField65 != 0 || self->mField66 != 0) {
        // Page-advance path: clear the pending flag, run the character-voice
        // playback, then switch the enabled animations to state 3.
        if (self->mField65 != 0) self->mField65 = 0;
        if (self->field_B4 != 0) {
            if (code80135FDC_getByte_64058() == 0) {
                if (findObjectById(self->mField68) != 0) {
                    CTalkWinTalkSrc* src =
                        reinterpret_cast<CTalkWinTalkSrc*>(func_800BBC0C());
                    CTalkWinTalkC4* talkC4 = src->mTalkC4;
                    if (talkC4 != 0) {
                        src->mVoice->play(0, 0);
                        int page = getAnimModelId(talkC4);
                        if (page < 0x21 || page > 0x26) {
                            func_8004B9D4(talkC4, 1, 0, -1, 0);
                        }
                    }
                }
            } else if (code80135FDC_getByte_64059() != 0) {
                if (findObjectById(self->mField68) != 0) {
                    CTalkWinTalkSrc* src =
                        reinterpret_cast<CTalkWinTalkSrc*>(func_800BBC0C());
                    src->mVoice->play(0, 0);
                }
            }
        }
        self->mLayout->SetAnimationEnable(self->field_94, false);
        self->mLayout->SetAnimationEnable(self->field_88, false);
        self->mLayout->SetAnimationEnable(self->field_8C, false);
        self->mLayout->SetAnimationEnable(self->field_98, false);
        self->mLayout->SetAnimationEnable(self->field_90, true);
        reinterpret_cast<CTalkAnimFrame*>(self->field_90)->mFrame =
            0.0f;
        switch (self->field_A4) {
        case 0:
            playUISound(0x2a);
            break;
        case 1:
            playUISound(0x73);
            break;
        case 3:
            playUISound(0x75);
            break;
        default:
            break;
        }
        self->field_B0 = 3;
    } else {
        // Page-loop flag flips (only reached when mField65/mField66 are clear).
        if (state == 1) {
            if (self->mField65 == 0) self->mField65 = 1;
        } else if (state == 3) {
            self->mField67 = 0;
            if (self->field_AC != 0) self->mField65 = 1;
        }
    }

    advanceAnimTransform(self->field_8C, 1.0f);
}

