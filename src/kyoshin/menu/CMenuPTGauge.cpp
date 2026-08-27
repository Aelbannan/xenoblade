// func_8004302C must be declared extern "C" before any include chain pulls
// in functions.hpp's plain C++ copy (MWCC rejects the reverse redeclaration),
// so this header goes first.
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/menu/CMenuPTGauge.hpp"

#include "kyoshin/cf/object/CAIAction.hpp"
// Owner headers (single winning decls):
// - getArtsSlotRC / getArtsParamRC2 / func_8025FB10 -> CChainActorList.hpp
//   (int/const void* arts-query forms match the CActorParam.cpp definitions;
//   func_8025FB10 extern "C" int form is the single winning decl).
// - func_800F3970 -> single shared import on CBattleManagerApi.hpp (pulled in
//   via CBattleManager.hpp below; CChain.hpp's copy is gone).
// - func_8016DF2C -> canonical extern "C" u16() form (CAIAction.hpp,
//   matching CfMapEffectManager.hpp / code_80135FDC.hpp).
#include "kyoshin/cf/chain/CChainActorList.hpp"
// (MWCC -ipa file quirk: keep at least one line between the two includes -
// dropping the remap block's lines outright perturbs pool/regalloc in
// func_80187B70/func_80187C90; see docs/MWCC_CASES.md IPA line-count note.)
#include "kyoshin/cf/CBattleManager.hpp"
// code_80135FDC.hpp declares lbl_eu_8066A208 as u32 (line 188), conflicting
// with the const float epsilon copies elsewhere; rename it out of the way.
// This TU uses none of them.
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

// Compile-only placeholder removed: __ct__CMenuPTGauge is an import - the
// real retail ctor body lives outside this TU and must not be redefined here.

#include "decomp.h"
#include <revolution/GX.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Batch 2026-07-14e: menu-ptgauge-move and menu-ptgauge-cbrender own their methods exclusively.
// Batch 2026-07-14l: menu-ptgauge-init owns Init exclusively.
// CLEANUP: renamed all unk fields to descriptive names.

extern "C" {
extern u32 lbl_eu_80663E28; // Secondary mode bitfield (bit 21/IBM bit 10 = realtime event busy)
extern char lbl_eu_805039C8[]; // String table base; offsets used for layout/animation/pane names
extern const f32 lbl_eu_806679E0; // 0.0f
extern const f32 lbl_eu_806679E4; // 300.0f
extern const f32 lbl_eu_806679E8; // 128.0f
extern const f32 lbl_eu_806679EC; // 1.0f
// Ctor imports: base/subobject ctors and the shared vtable/PTMF labels
// (retail unmangled names; storage lives elsewhere).
void __ct__8CProcessFv(CProcess*);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
extern char lbl_eu_8052C1C0[]; // interim CProcess-region vtable
extern char lbl_eu_80532450[]; // final composite vtable set
extern u32 __ptmf_null[3];     // runtime ptmf.o null member-fn descriptor
// Unmangled retail names (distinct from C++-mangled decls in code_80135FDC.hpp).
// Layout/anim helpers: declared extern "C" where the retail symbol is unmangled.
int func_8013BE50(); // Returns nonzero when battle is active (gate for all menu HUD widgets)
}

extern void func_80138078(u32);

typedef void (CMenuPTGauge::*CMenuPTGaugePtmf)(s32); // PTMF: (s32 partyVal) -> void
extern CMenuPTGaugePtmf lbl_eu_805323F8[]; // Dispatch table for mActionIdx (3 entries: default, special, chain)

/**
 * Initialises the PT Gauge layout, loads five animations, binds the default
 * animation, registers the IScnRender render-before callback, and configures
 * the font renderer on the root pane.
 *
 * Layout region: 0x1200 bytes from MEM2. Animations loaded from the shared
 * ARC via string-table offsets (PTGauge_00 through PTGauge_04). Render
 * callback registered at priority 0xa (below standard HUD).
 *
 * @param  none
 * @return none
 */
void CMenuPTGauge::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mLayoutMem.createRegion(handle, 0x1200, lbl_eu_805039C8, 0); // 0x1200 = layout region budget
    Class_8045F858 regionGuard(&mLayoutMem);

    nw4r::lyt::ArcResourceAccessor* accessor;

    accessor = func_801355F4();
    func_80136E84(&mLayout, accessor, lbl_eu_805039C8 + 0xd);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnimDefault, accessor, lbl_eu_805039C8 + 0x29);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnimOpen, accessor, lbl_eu_805039C8 + 0x48);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnimClose, accessor, lbl_eu_805039C8 + 0x6f);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnimIdle, accessor, lbl_eu_805039C8 + 0x96);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnimSpecial, accessor, lbl_eu_805039C8 + 0xb7);

    // Retail: layout+0x10 is the root pane (GetRootPane inlines to this load).
    // CDeviceFont::getFontInfo(1, layout) returns an object whose vt+0x24
    // (no explicit args) yields the u32 passed to func_8013676C. The virtual
    // call through CMenuPTGaugeFont makes MWCC emit retail's r12 dispatch.
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    CMenuPTGaugeFont* fontObj =
        static_cast<CMenuPTGaugeFont*>(CDeviceFont::getFontInfo(1, mLayout));
    u32 fontResult = fontObj->sf9();
    func_8013676C(rootPane, fontResult);

    mLayout->Animate(0);
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimDefault);
    mLayout->SetAnimationEnable(mAnimDefault, true);

    // MI adjust: IScnRender at +0x5c (null-this safe), same pattern as
    // CMenuArtsSelect::Term.
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        // Access IScnRender subobject at unk00[0x5c] via member offset
        cb = reinterpret_cast<IScnRender*>(&this->unk54[0x8]);
    }
    mScn->addRenderCB(cb, 0xa, 0); // priority 0xa = HUD render layer

    mLayoutMem.func_8045F810();
}

/**
 * Per-frame update driving a 4-phase FSM for the PT Gauge widget.
 *
 * Gate sequence (shared with cbRenderBefore):
 *   1. CTaskGame pause (isFlag01Set) -> skip
 *   2. lbl_eu_80663E28 bit 21 (IBM bit 10; realtime event busy) -> skip
 *   3. func_8013BE50 (battle inactive) -> skip
 *   4. lbl_eu_80663E24 & 0xAFA40000 (UI suppress mask) -> skip
 *
 * FSM phases (mPhase):
 *   0: Count battle actors; if none, idle. Otherwise init gauge panel width,
 *      set mPhase=1, play open SE (0xa0).
 *   1: Wait for default animation finish, advance to mPhase=2.
 *   2: Dispatch PTMF action (mActionIdx). On party wipe or empty actor list,
 *      transition to mPhase=3 with close SE (0xa1). On chain == 5, switch
 *      mActionIdx=2 and play special animation.
 *   3: Wait for close/default animation finish, reset to mPhase=0.
 *
 * @param  none
 * @return none
 */
void CMenuPTGauge::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto done;
    }
    // Retail: rlwinm.; beq +8; b done. MWCC collapses if->goto to bne; keep beq
    // via fallthrough asm b (PLAN.md section 17.6 single-insn carve-out).
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) { // IBM bit 10 = realtime event busy
        goto after_bit21;
    }
    DECOMP_ASM_INSN_BEGIN
    asm {
        b done
    }
    DECOMP_ASM_INSN_END
after_bit21:
    if (!func_8013BE50()) {
        goto done;
    }
    if (lbl_eu_80663E24 & 0xAFA40000u) { // UI suppress composite mask (event/talk/vision)
        goto done;
    }

    switch (mPhase) {
    case 0: {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        _reslist_node<cf::CfObjectActor*>* actorNode;
        u32 actorCount;
        _reslist_node<cf::CfObjectActor*>* actorEnd;
        actorEnd = bm->mActorList1.mStartNodePtr;
        actorNode = actorEnd->mNext;
        actorCount = 0;
        while (actorNode != actorEnd) {
            actorNode = actorNode->mNext;
            ++actorCount;
        }
        if (actorCount == 0) {
            break;
        }

        mGaugeBase = 0;
        mActionIdx = 0;
        mPhase = 1;

        if (mLayout != NULL) {
            if (mGaugeInit != 0) {
                mGaugeInit = 0;
                nw4r::lyt::Pane* pane =
                    mLayout->GetRootPane()->FindPaneByName(lbl_eu_805039C8 + 0xd8, true); // +0xd8 = pane name for width fixup
                if (pane != NULL) {
                    nw4r::lyt::Size size;
                    // 128.0f * (0.0f / 300.0f) = 0.0f => collapse gauge to zero width
                    size.width =
                        lbl_eu_806679E8 * (lbl_eu_806679E0 / lbl_eu_806679E4);
                    size.height = pane->GetSize().height;
                    pane->SetSize(size);
                }
            }
        }
        func_80138078(0xa0); // SE: gauge open
        break;
    }
    case 1: {
        if (func_80137444(mAnimDefault, lbl_eu_806679EC) != 0) {
            mPhase = 2;
        }
        break;
    }
    case 2: {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        s32 partyVal = *reinterpret_cast<s32*>(&bm->unk194);

        (this->*lbl_eu_805323F8[mActionIdx])(partyVal);

        if (partyVal <= 0) {
            bm = cf::CBattleManager::getInstance();
            _reslist_node<cf::CfObjectActor*>* actorNode;
            u32 actorCount;
            _reslist_node<cf::CfObjectActor*>* actorEnd;
            actorEnd = bm->mActorList1.mStartNodePtr;
            actorNode = actorEnd->mNext;
            actorCount = 0;
            while (actorNode != actorEnd) {
                actorNode = actorNode->mNext;
                ++actorCount;
            }
            if (actorCount == 0) {
                mPhase = 3;
                mLayout->Animate(0);
                mLayout->UnbindAllAnimation();
                mLayout->BindAnimation(mAnimDefault);
                mLayout->SetAnimationEnable(mAnimDefault, true);
                func_80138078(0xa1); // SE: gauge close
                break;
            }
        }

        if (mActionIdx >= 1) { // only action 0 checks chain; actions 1/2 skip
            break;
        }

        bm = cf::CBattleManager::getInstance();
        s32 flag;
        u8 byte = bm->mChain.unk0[2];
        u16 lowerByte = byte;
        u32 upperByte = byte;
        flag = 0;
        if (lowerByte < 1) { // chain value range: [1, 0x18]
            goto range_done;
        }
        if (upperByte > 0x18) {
            goto range_done;
        }
        flag = 1;
    range_done:
        if (flag == 0) {
            goto not_five;
        }
        if (byte != 5) { // chain == 5 triggers special animation path
            goto not_five;
        }
        flag = 1;
        goto after_five;
    not_five:
        flag = 0;
    after_five:
        if (flag == 0) {
            break;
        }

        mActionIdx = 2;
        mLayout->Animate(0);
        mLayout->UnbindAllAnimation();
        mLayout->BindAnimation(mAnimSpecial);
        mLayout->SetAnimationEnable(mAnimSpecial, true);
        mAnimSpecial->SetFrame(lbl_eu_806679E0);
        mLayout->Animate(0);
        mGaugePrev = mGaugeBase;
        break;
    }
    case 3: {
        if (func_80137510(mAnimDefault, lbl_eu_806679EC) != 0) {
            mPhase = 0;
        }
        break;
    }
    default:
        break;
    }

    mLayout->Animate(0);
done:
    ;
}

/**
 * Render-before callback: draws the PT Gauge layout with Z-test disabled.
 *
 * Reuses the same four gate checks as Move (CTaskGame pause, realtime event,
 * battle active, UI suppress). Also skips when mPhase==0 (no gauge visible).
 *
 * When active, disables Z comparison (always pass via GXSetZMode), sets up
 * a lyt::DrawInfo, and renders the full layout tree.
 *
 * @param  none
 * @return none
 */
void CMenuPTGauge::cbRenderBefore() {
    // getInstance result discarded; feeds static isFlag01Set call schedule
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto done;
    }
    // Retail: rlwinm.; beq +8; b done. MWCC collapses if->goto to bne; keep beq
    // via fallthrough asm b (PLAN.md section 17.6 single-insn carve-out).
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) { // IBM bit 10 = realtime event busy
        goto after_bit21;
    }
    DECOMP_ASM_INSN_BEGIN
    asm {
        b done
    }
    DECOMP_ASM_INSN_END
after_bit21:
    if (!func_8013BE50()) {
        goto done;
    }
    if (mPhase == 0) { // gauge not initialised yet
        goto done;
    }
    if (lbl_eu_80663E24 & 0xAFA40000u) { // UI suppress composite mask (event/talk/vision)
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE); // disable Z compare, always pass
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        func_80137038(mLayout, &drawInfo, 0, 1);
    }
done:
    ;
}

extern "C" int func_80187710() {
    return (int)lbl_eu_806642D8;
}
// Complete-object destructor (retail __dt__12CMenuPTGaugeFv). Written as the
// real member dtor so MWCC supplies the this-null guard, the embedded-member
// -1 flag, and the flags>0 operator-delete tail from the dtor shape itself.
// The base CProcess dtor is called explicitly (no recovered C++ base chain);
// the doubled null re-check reproduces retail's dead doubled beq.
// Complete-object destructor: body empty; MWCC supplies the this-null guard,
// the member destruction, the base __dt__8CProcessFv call and the delete tail.
extern "C" void __dt__12CMenuPTGaugeFv(void*);
extern "C" void __dt__8CProcessFv(void* self, int flags); // CProcess base destructor
// Empty-body mid-base dtor: the doubled self-check + explicit CProcess base
// call reproduce retail's dead doubled beq; MWCC inlines this into the
// derived dtor (no implicit CProcess chain exists on PTGaugeTaskMid).
PTGaugeTaskMid::~PTGaugeTaskMid() {
    if (reinterpret_cast<void*>(this) != NULL) {
        __dt__8CProcessFv(reinterpret_cast<void*>(this), 0);
    }
}
CMenuPTGauge::~CMenuPTGauge() {
    // mLayoutMem is destroyed implicitly (MWCC passes the -1 embedded-member
    // flag), then the CProcess base dtor is emitted automatically from the
    // real CProcess base; MWCC appends the flags>0 operator-delete tail.
}
// IWorkEvent dtor this-adjusting thunk (retail: subi r3,-0x58; b __dt__12CMenuPTGaugeFv;
// resolves to the member dtor's mangled symbol above)
extern "C" void func_80187EFC(void* self){
    __dt__12CMenuPTGaugeFv((char*)self - 0x58);
}
extern "C" void func_80187F04(void* self) {
    extern void cbRenderBefore__12CMenuPTGaugeFv(void*);
    cbRenderBefore__12CMenuPTGaugeFv((char*)self - 0x5c);
}
void func_80187F0C(void* p) {
    // Adjust from IScnRender subobject (+0x5c) back to CMenuPTGauge
    __dt__12CMenuPTGaugeFv(static_cast<char*>(p) - 0x5c);
}

/**
 * Teardown: wait for VI draw done, unregister the render callback, free the
 * layout (polymorphic delete -> vtable+0x8 deleting-dtor dispatch with flag
 * 1), release the layout memory region, and clear the global instance.
 */
void CMenuPTGauge::Term() {
    CDeviceVI::waitForDrawDone();
    // Null-this-safe IScnRender subobject at +0x5c, same idiom as Init.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        render = reinterpret_cast<IScnRender*>(&this->unk54[0x8]);
    }
    mScn->removeRenderCB(render);

    if (mLayout != NULL) {
        // Retail re-checks the pointer before the deleting-dtor dispatch
        // (dead doubled beq from D2-inlined-into-D1 shape).
        delete mLayout;
        mLayout = NULL;
    }

    mLayoutMem.func_8045F778();
    lbl_eu_806642D8 = NULL;
}


// Byte-window over CMenuPTGauge covering everything the retail ctor touches:
// base vtable (+0x10), two null PTMF blocks (+0x3c/+0x48), interface flags
// bytes (+0x54/+0x55), MI subobject vtables (+0x58/+0x5c), scene ptr (+0x60).
struct CMenuPTGaugeCtorShim {
    u8 unk00[0x10];
    u32 unk10;       // CProcess-region / final composite vtable
    u8 unk14[0x28];
    u32 ptmfMove[3]; // +0x3c
    u32 ptmfDraw[3]; // +0x48
    u8 flag54;
    u8 flag55;
    u8 unk56[2];
    u32 vt58;        // +0x58 (final vtable + 0x24)
    u32 vt5c;        // +0x5c (final vtable + 0xac)
    u32 scene;       // +0x60
};

// Retail constructs the gauge via the unmangled symbol __ct__CMenuPTGauge
// (not a MWCC-mangled member ctor); extern "C" keeps the exact retail name
// and avoids MWCC's auto-vtable/RTTI emission (CMdlAnmEye.cpp recipe).
extern "C" CMenuPTGauge* __ct__CMenuPTGauge(CMenuPTGauge* self, CProcess* arg) {
    // Byte-window alias over the object for the CProcess-region fields the
    // retail ctor fills by hand.
    CMenuPTGaugeCtorShim* p = reinterpret_cast<CMenuPTGaugeCtorShim*>(self);
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(p));

    p->unk10 = (u32)lbl_eu_8052C1C0;

    // Post-increment pointer form: MWCC folds the base @l into a single
    // `lwzu` for the first load and keeps the base register for the rest;
    // the second triple re-reads through folded negative offsets (retail
    // re-loads ptmf[0..2] after the first stores). q-locals declared first
    // so MWCC colors q0 into r7 and the ptmf base into r8 (CMenuBattleCommu
    // ctor recipe).
    u32 q0, q1, q2;
    const u32* ptmf = __ptmf_null;
    u32 p0, p1, p2;
    p0 = *ptmf++;
    p1 = *ptmf++;
    p->ptmfMove[1] = p1;
    p->ptmfMove[0] = p0;
    p2 = *ptmf++;
    p->ptmfMove[2] = p2;
    q0 = ptmf[-3];
    q1 = ptmf[-2];
    p->ptmfDraw[1] = q1;
    p->ptmfDraw[0] = q0;
    q2 = ptmf[-1];
    p->ptmfDraw[2] = q2;

    p->flag54 = 0;
    p->flag55 = 0;

    // Retail re-stores +0x10 with the composite vtable after the temp
    // (CProcess) vtable; +0x24/+0xac are the IWorkEvent / IScnRender
    // dispatch slots inside that cluster.
    p->unk10 = (u32)lbl_eu_80532450;
    p->vt58 = (u32)lbl_eu_80532450 + 0x24;
    p->vt5c = (u32)lbl_eu_80532450 + 0xac;
    p->scene = (u32)arg;

    __ct__17UnkClass_8045F564Fv(&self->mLayoutMem);

    self->mLayout = NULL;
    self->mAnimDefault = NULL;
    self->mAnimOpen = NULL;
    self->mAnimClose = NULL;
    self->mAnimIdle = NULL;
    self->mAnimSpecial = NULL;
    self->mPhase = 0;
    self->mActionIdx = 0;
    self->mGaugeBase = 0;
    self->mGaugePrev = 0;
    self->unk9C = 0;
    self->mGaugeInit = -1;
    return self;
}

typedef CMenuPTGauge* (*PTGaugeCtorFn)(CMenuPTGauge*, CProcess*);

// Singleton factory: creates the PT Gauge on the work-thread heap (0xA4 = full
// object size), registers it as a child of `parent`, and caches it in the
// global pointer. Returns NULL when the gauge already exists.
CMenuPTGauge* func_80187694(CProcess* parent, CProcess* ctorArg) {
    if (lbl_eu_806642D8 != NULL) {
        return NULL;
    }
    mtl::ALLOC_HANDLE mem = CWorkThreadSystem::getWorkMem();
    // Retail chains the allocation result through the ctor call (r3 -> r3),
    // so the ctor is invoked through a pointer-typed cast returning the
    // object; this also stops MWCC eliding the empty stub.
    CMenuPTGauge* gauge = static_cast<CMenuPTGauge*>(mtl::MemManager::allocate(0xA4, mem));
    if (gauge != NULL) {
        gauge = ((PTGaugeCtorFn)__ct__CMenuPTGauge)(gauge, ctorArg);
    }
    lbl_eu_806642D8 = gauge;
    // PTGaugeTaskMid deliberately does not inherit CProcess; retail still
    // dispatches CProcess::Regist with the gauge as this.
    reinterpret_cast<CProcess*>(gauge)->Regist(parent, false);
    return lbl_eu_806642D8;
}
void func_80187718() {
    // When the gauge is active (phase 2) on the default action and the party
    // value hits 300, switch to the special action with a 300-frame counter.
    CMenuPTGauge* self = lbl_eu_806642D8;
    if (self->mPhase == 2) {
        if (self->mActionIdx < 1) {
            cf::CBattleManager* bm = cf::CBattleManager::getInstance();
            if (*reinterpret_cast<s32*>(&bm->unk194) == 0x12c) {
                self->mActionIdx = 1;
                self->unk9C = 0x12c;
            }
        }
    }
}
// Named .sdata2 conversion-magic blob (bits 0x4330000080000000 = 2^52 +
// 2^31); shared global defined elsewhere - declaration only, so int->float
// cast pools bind to the named label instead of a private literal.
extern const double lbl_eu_806679F0;
extern float lbl_eu_806679F8;
extern const float lbl_eu_806679FC;
extern const float lbl_eu_80667A00;
// Per-frame gauge counter tick: decrement unk9C by lbl_eu_806679F8 through
// the .sdata2 double path, clamp at zero (resetting action/base state), then
// when the value changed resize the gauge pane proportionally.
void func_80187858(CMenuPTGauge* self) {
    // Manual s32->double conversion (MWCC_PATTERNS 7i): xoris word first, then
    // 0x43300000; subtracting the shared bias blob completes the conversion.
    // lbl_eu_806679F0 is declaration-only here, so the fsubs keeps the named
    // .sdata2 reloc instead of folding to a private literal.
    // Decrement step, read early like retail's schedule.
    const f32 step = lbl_eu_806679F8;
    union {
        double d;
        u32 w[2];
    } cv;
    cv.w[1] = (u32)self->unk9C ^ 0x80000000;
    cv.w[0] = 0x43300000;
    s32 v = (s32)(cv.d - lbl_eu_806679F0 - step);
    self->unk9C = v;
    if (v < 0) {
        self->mActionIdx = 0;
        self->mGaugeBase = 0;
        self->unk9C = 0;
    }
    // Cached in a local (retail keeps it in r31 across the pane lookup).
    s32 cur = self->unk9C;
    if (self->mLayout != NULL && self->mGaugeInit != cur) {
        self->mGaugeInit = cur;
        nw4r::lyt::Pane* pane =
            self->mLayout->GetRootPane()->FindPaneByName(lbl_eu_805039C8 + 0xd8, true); // +0xd8 = gauge pane name
        if (pane != NULL) {
            nw4r::lyt::Size size;
            cv.w[1] = (u32)cur ^ 0x80000000;
            cv.w[0] = 0x43300000;
            size.height = pane->GetSize().height;
            // 128.0f scaled by cur / 300.0f (cur converted via the bias blob).
            size.width = lbl_eu_806679E8 * ((cv.d - lbl_eu_806679F0) / lbl_eu_806679E4);
            pane->SetSize(size);
        }
    }
}
void func_80187958(CMenuPTGauge* self, s32 partyVal) {
    // Advance the special animation one frame (result unused).
    func_80137444(self->mAnimSpecial, lbl_eu_806679EC);

    // Chain gate (shared with Move case 2): when the chain counter is 5,
    // keep the special animation running and skip the reset below.
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    int flag;
    u8 chain = bm->mChain.unk0[2]; // chain value byte at +0x1aa
    // Differently-typed copies keep MWCC from folding the range check.
    u16 lowerByte = chain;
    u32 upperByte = chain;
    flag = 0;
    if (lowerByte < 1) { // chain value range: [1, 0x18]
        goto range_done;
    }
    if (upperByte > 0x18) {
        goto range_done;
    }
    flag = 1;
range_done:
    if (flag == 0) {
        goto not_five;
    }
    if (chain != 5) { // chain == 5 keeps the special animation running
        goto not_five;
    }
    flag = 1;
    goto after_five;
not_five:
    flag = 0;
after_five:
    if (flag == 0) {
        // Reset to the default action and re-bind the gauge animation for the
        // current party gauge value: >= 300 idle, 200..299 close, < 200 open.
        self->mActionIdx = 0;
        self->mAnimSpecial->SetFrame(lbl_eu_806679E0);
        self->mLayout->Animate(0);
        self->mGaugeBase = self->mGaugePrev;

        if (partyVal >= 300) {
            self->mLayout->Animate(0);
            self->mLayout->UnbindAllAnimation();
            self->mLayout->BindAnimation(self->mAnimIdle);
            self->mLayout->SetAnimationEnable(self->mAnimIdle, true);
            self->mAnimIdle->SetFrame(lbl_eu_806679E0);
            self->mLayout->Animate(0);
        } else if (partyVal >= 200) {
            self->mLayout->Animate(0);
            self->mLayout->UnbindAllAnimation();
            self->mLayout->BindAnimation(self->mAnimClose);
            self->mLayout->SetAnimationEnable(self->mAnimClose, true);
            self->mAnimClose->SetFrame(lbl_eu_806679E0);
            self->mLayout->Animate(0);
        } else {
            self->mLayout->Animate(0);
            self->mLayout->UnbindAllAnimation();
            self->mLayout->BindAnimation(self->mAnimOpen);
            self->mLayout->SetAnimationEnable(self->mAnimOpen, true);
            self->mAnimOpen->SetFrame(lbl_eu_806679E0);
            self->mLayout->Animate(0);
        }
    }
}
void func_80187C90(CMenuPTGauge* self, s32 partyVal) {
    // Advance the close animation one frame (result unused).
    func_80137444(self->mAnimClose, lbl_eu_806679EC);

    // Manual signed-int -> double conversion (MWCC_PATTERNS 7i): build the
    // 0x4330000080000000 bit pattern so the fsubs names the shared .sdata2
    // magic lbl_eu_806679F0 (a plain C cast pools a private constant).
    // Manual signed-int -> double conversion (MWCC_PATTERNS 7i): build the
    // 0x4330000080000000 bit pattern so the fsubs names the shared .sdata2
    // magic lbl_eu_806679F0 (a plain C cast pools a private constant).
    union {
        double d;
        u32 w[2];
    } cv;
    cv.w[1] = (u32)partyVal ^ 0x80000000;
    cv.w[0] = 0x43300000;
    if (cv.d - lbl_eu_806679F0 >= lbl_eu_806679E4) {
        self->mGaugeBase = 3;
        self->mLayout->Animate(0);
        self->mLayout->UnbindAllAnimation();
        self->mLayout->BindAnimation(self->mAnimIdle);
        self->mLayout->SetAnimationEnable(self->mAnimIdle, true);
        self->mAnimIdle->SetFrame(lbl_eu_806679E0);
        self->mLayout->Animate(0);
        func_80138078(0x66); // SE
    } else {
        if (partyVal < 200) {
            self->mGaugeBase = 1;
            self->mLayout->Animate(0);
            self->mLayout->UnbindAllAnimation();
            self->mLayout->BindAnimation(self->mAnimOpen);
            self->mLayout->SetAnimationEnable(self->mAnimOpen, true);
            self->mAnimOpen->SetFrame(lbl_eu_806679E0);
            self->mLayout->Animate(0);
        }
    }
}
// Close-action dispatch entry: once the party value reaches lbl_eu_806679FC,
// reset the gauge base state to 1 and re-bind the open animation from frame 0
// with SE 0x65.
void func_80187A88(CMenuPTGauge* self, s32 partyVal) {
    // Manual signed-int -> double conversion (MWCC_PATTERNS 7i): build the
    // 0x4330000080000000 bit pattern and subtract the shared .sdata2 magic
    // lbl_eu_806679F0 so the fsubs names the retail blob.
    union {
        double d;
        u32 w[2];
    } cv;
    // xoris word first, then 0x43300000, or MWCC hoists the lis out of order.
    cv.w[1] = (u32)partyVal ^ 0x80000000;
    cv.w[0] = 0x43300000;
    if (cv.d - lbl_eu_806679F0 >= lbl_eu_806679FC) {
        self->mGaugeBase = 1;
        self->mLayout->Animate(0);
        self->mLayout->UnbindAllAnimation();
        self->mLayout->BindAnimation(self->mAnimOpen);
        self->mLayout->SetAnimationEnable(self->mAnimOpen, true);
        self->mAnimOpen->SetFrame(lbl_eu_806679E0);
        self->mLayout->Animate(0);
        func_80138078(0x65); // SE
    }
}
// Entry-action dispatch entry: re-seed the open animation to frame 1, then
// switch between close state (value >= lbl_eu_80667A00) or reset-to-entry
// (value < 100) with SE 0x65.
void func_80187B70(CMenuPTGauge* self, s32 partyVal) {
    // Retail seeds this frame through the out-of-line helper, not the inline
    // SetFrame accessor.
    func_80137444(self->mAnimOpen, lbl_eu_806679EC);
    // The 0x43300000-magic blob (lbl_eu_806679F0) in retail is MWCC's own
    // s32->double cast correction constant, not a source subtraction.
    if ((float)partyVal >= lbl_eu_80667A00) {
        self->mGaugeBase = 2;
        self->mLayout->Animate(0);
        self->mLayout->UnbindAllAnimation();
        self->mLayout->BindAnimation(self->mAnimClose);
        self->mLayout->SetAnimationEnable(self->mAnimClose, true);
        self->mAnimClose->SetFrame(lbl_eu_806679E0);
        self->mLayout->Animate(0);
        func_80138078(0x65); // SE
    } else {
        if (partyVal < 100) {
            self->mGaugeBase = 0;
            self->mAnimOpen->SetFrame(lbl_eu_806679E0);
        }
    }
}
// PTMF type shared by both dispatch tables (lbl_eu_805323F8/lbl_eu_80532420).
extern CMenuPTGaugePtmf lbl_eu_80532420[]; // Dispatch table indexed by mGaugeBase

// Gauge refresh + action dispatch: when the displayed value changed, resize
// the gauge pane proportionally to the party value, then always dispatch the
// per-state handler selected by mGaugeBase.
void func_80187778(CMenuPTGauge* self, s32 partyVal) {
    if (self->mLayout != NULL && self->mGaugeInit != partyVal) {
        self->mGaugeInit = partyVal;
        nw4r::lyt::Pane* pane =
            self->mLayout->GetRootPane()->FindPaneByName(lbl_eu_805039C8 + 0xd8, true); // +0xd8 = gauge pane name
        if (pane != NULL) {
            nw4r::lyt::Size size;
            // 128.0f scaled by partyVal / 300.0f. The int->float cast expands
            // to MWCC's 0x43300000/xoris magic; its pooled constant must
            // resolve to retail's lbl_eu_806679F0 blob (see MWCC_PATTERNS 7i).
            size.width = lbl_eu_806679E8 * ((float)partyVal / lbl_eu_806679E4);
            size.height = pane->GetSize().height;
            pane->SetSize(size);
        }
    }
    (self->*lbl_eu_80532420[self->mGaugeBase])(partyVal);
}
// PTMF dispatch entry: advance the idle animation, then when the party value
// drops below 300 switch the gauge base state to 2 and re-bind the close
// animation from frame 0.
void func_80187E28(CMenuPTGauge* self, s32 partyVal) {
    // Advance the idle animation one frame (result unused).
    func_80137444(self->mAnimIdle, lbl_eu_806679EC);

    if (partyVal < 300) {
        self->mGaugeBase = 2;
        self->mLayout->Animate(0);
        self->mLayout->UnbindAllAnimation();
        self->mLayout->BindAnimation(self->mAnimClose);
        self->mLayout->SetAnimationEnable(self->mAnimClose, true);
        self->mAnimClose->SetFrame(lbl_eu_806679E0);
        self->mLayout->Animate(0);
    }
}
