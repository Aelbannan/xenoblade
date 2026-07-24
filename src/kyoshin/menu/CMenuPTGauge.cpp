#include "kyoshin/menu/CMenuPTGauge.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/util/MemManager.hpp"

#include "decomp.h"
#include <revolution/GX.h>

// Batch 2026-07-14e: menu-ptgauge-move and menu-ptgauge-cbrender own their methods exclusively.
// Batch 2026-07-14l: menu-ptgauge-init owns Init exclusively.
// CLEANUP: renamed all unk fields to descriptive names.

extern "C" {
extern u32 lbl_eu_80663E24; // Primary event/presentation bitfield (0xAFA40000 = UI suppress)
extern u32 lbl_eu_80663E28; // Secondary mode bitfield (bit 21/IBM bit 10 = realtime event busy)
extern char lbl_eu_805039C8[]; // String table base; offsets used for layout/animation/pane names
extern const f32 lbl_eu_806679E0; // 0.0f
extern const f32 lbl_eu_806679E4; // 300.0f
extern const f32 lbl_eu_806679E8; // 128.0f
extern const f32 lbl_eu_806679EC; // 1.0f
// Unmangled retail names (distinct from C++-mangled decls in code_80135FDC.hpp).
// void* params avoid C++ name-mangling conflict with typed decls in
// code_80135FDC.hpp (MWCC extern "C" would otherwise collide).
int func_8013BE50(); // Returns nonzero when battle is active (gate for all menu HUD widgets)
u32 func_80137510(void* anim, float frame); // Checks if animation has reached/exceeded given frame
nw4r::lyt::ArcResourceAccessor* func_801355F4(); // Returns shared ARC resource accessor
void func_8013676C(void* pane, u32 val); // Configures font renderer on a pane
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
    // CDeviceFont::func_80452C10(1, layout) returns an object whose vt+0x24
    // (no explicit args) yields the u32 passed to func_8013676C. Pass void*
    // so the call binds the unmangled reloc (not Pane*-mangled from
    // code_80135FDC.hpp).
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    void* fontObj = CDeviceFont::func_80452C10(1, mLayout);
    typedef u32 (*FontVFn)(void*);
    u32 fontResult = (*reinterpret_cast<FontVFn**>(fontObj))[0x24 / 4](fontObj);
    func_8013676C(static_cast<void*>(rootPane), fontResult);

    mLayout->Animate(0);
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimDefault);
    mLayout->SetAnimationEnable(mAnimDefault, true);

    // MI adjust: IScnRender at +0x5c (null-this safe), same pattern as
    // CMenuArtsSelect::Term.
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(reinterpret_cast<u8*>(this) + 0x5c);
    }
    mScn->addRenderCB(cb, 0xa, 0); // priority 0xa = HUD render layer

    mLayoutMem.func_8045F810();
}

/**
 * Per-frame update driving a 4-phase FSM for the PT Gauge widget.
 *
 * Gate sequence (shared with cbRenderBefore):
 *   1. CTaskGame pause (func_800426F0) -> skip
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
    if (CTaskGame::func_800426F0()) {
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
        if (func_80137510(static_cast<void*>(mAnimDefault), lbl_eu_806679EC) != 0) {
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
    // getInstance result discarded; feeds static func_800426F0 call schedule
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
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

// LLM-HARNESS-BEGIN: us-80188b98
extern "C" int lbl_eu_806642D8;

extern "C" int func_80187710() {
    return lbl_eu_806642D8;
}
// LLM-HARNESS-END: us-80188b98
// LLM-HARNESS-BEGIN: us-801894b0
extern "C" bool func_80187EFC() { return false; }
// LLM-HARNESS-END: us-801894b0
// LLM-HARNESS-BEGIN: us-801894b8
extern "C" bool func_80187F04() { return false; }
// LLM-HARNESS-END: us-801894b8
// LLM-HARNESS-BEGIN: us-801894c0
extern "C" void __dt__12CMenuPTGaugeFv(void*);

extern "C" void func_80187F0C(void* p) {
    __dt__12CMenuPTGaugeFv(reinterpret_cast<char*>(p) - 0x5c);
}
// LLM-HARNESS-END: us-801894c0

// LLM-HARNESS-BEGIN: us-80188388
extern "C" void __ct__CMenuPTGauge() {}
// LLM-HARNESS-END: us-80188388
// LLM-HARNESS-BEGIN: us-80188b1c
extern "C" void func_80187694() {}
// LLM-HARNESS-END: us-80188b1c
// LLM-HARNESS-BEGIN: us-80188ba0
extern "C" void func_80187718() {}
// LLM-HARNESS-END: us-80188ba0
// LLM-HARNESS-BEGIN: us-80188c00
extern "C" void func_80187778() {}
// LLM-HARNESS-END: us-80188c00
// LLM-HARNESS-BEGIN: us-80188ce0
extern "C" void func_80187858() {}
// LLM-HARNESS-END: us-80188ce0
// LLM-HARNESS-BEGIN: us-80188de0
extern "C" void func_80187958() {}
// LLM-HARNESS-END: us-80188de0
// LLM-HARNESS-BEGIN: us-8018903c
extern "C" void func_80187A88() {}
// LLM-HARNESS-END: us-8018903c
// LLM-HARNESS-BEGIN: us-80189124
extern "C" void func_80187B70() {}
// LLM-HARNESS-END: us-80189124
// LLM-HARNESS-BEGIN: us-80189244
extern "C" void func_80187C90() {}
// LLM-HARNESS-END: us-80189244
// LLM-HARNESS-BEGIN: us-801893dc
extern "C" void func_80187E28() {}
// LLM-HARNESS-END: us-801893dc
