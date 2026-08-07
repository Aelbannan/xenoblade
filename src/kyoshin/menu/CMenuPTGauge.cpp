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
// Layout/anim helpers: declared extern "C" where the retail symbol is
// unmangled (func_801355F4, func_8013676C, func_80137510 in code_80135FDC.hpp).
int func_8013BE50(); // Returns nonzero when battle is active (gate for all menu HUD widgets)
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4(); // shared ARC accessor (retail unmangled)
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
        cb = reinterpret_cast<IScnRender*>(&this->unk00[0x5c]);
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

extern "C" int lbl_eu_806642D8;

extern "C" int func_80187710() {
    return lbl_eu_806642D8;
}
void __dt__12CMenuPTGaugeFv(void*);
bool func_80187EFC(void* self){
    __dt__12CMenuPTGaugeFv((char*)self - 0x58);
    return false;
}
extern "C" void func_80187F04(void* self) {
    extern void cbRenderBefore__12CMenuPTGaugeFv(void*);
    cbRenderBefore__12CMenuPTGaugeFv((char*)self - 0x5c);
}
void __dt__12CMenuPTGaugeFv(void*); void func_80187F0C(void* p) {
    // Adjust from IScnRender subobject (+0x5c) back to CMenuPTGauge
    __dt__12CMenuPTGaugeFv(static_cast<char*>(p) - 0x5c);
}

void __ct__CMenuPTGauge(){}
void func_80187694(){}
void func_80187718(){}
void func_80187778(){}
extern double lbl_eu_806679F0;
extern float lbl_eu_806679F8;
struct PTGaugeIf { virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014(); virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024(); virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034(); virtual void _v038(); virtual void* vf3C(void* a, u32 b); };

extern "C" void func_80187858(void* self) {
    s32 v = (s32)((float)*(s32*)((u8*)self + 0x9c) - 1.0f);
    *(u32*)((u8*)self + 0x9c) = (u32)v;
    if (v < 0) {
        *(u32*)((u8*)self + 0x90) = 0;
        *(u32*)((u8*)self + 0x94) = 0;
        *(u32*)((u8*)self + 0x9c) = 0;
    }
    u32 cur = *(u32*)((u8*)self + 0x9c);
    void* sub = *(void**)((u8*)self + 0x74);
    if (sub) {
        if (*(u32*)((u8*)self + 0xa0) != cur) {
            *(u32*)((u8*)self + 0xa0) = cur;
            void* r = ((PTGaugeIf*)*(void**)((u8*)sub + 0x10))->vf3C(lbl_eu_805039C8 + 0xd8, 1);
            if (r) {
                float old = *(float*)((u8*)r + 0x50);
                *(float*)((u8*)r + 0x4c) = lbl_eu_806679E8 * ((float)(s32)cur / lbl_eu_806679E4);
                *(float*)((u8*)r + 0x50) = old;
            }
        }
    }
}
void func_80187958(){}
void func_80187A88(){}
void func_80187B70(){}
void func_80187C90(){}
void func_80187E28(){}
