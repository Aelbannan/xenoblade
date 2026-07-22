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

extern "C" {
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
extern char lbl_eu_805039C8[];
extern const f32 lbl_eu_806679E0; // 0.0f
extern const f32 lbl_eu_806679E4; // 300.0f
extern const f32 lbl_eu_806679E8; // 128.0f
extern const f32 lbl_eu_806679EC; // 1.0f
// Unmangled retail names (distinct from C++-mangled decls in code_80135FDC.hpp).
int func_8013BE50();
u32 func_80137510(void* anim, float frame);
// Retail links this unmangled (not CUICfManager::func_801355F4) -- see
// MWCC_REFERENCE.md 8c19.
nw4r::lyt::ArcResourceAccessor* func_801355F4();
// Retail links this unmangled (not the FPQ34nw4r3lyt4PaneUl-mangled decl in
// code_80135FDC.hpp); void* param (same overload trick as func_80137510
// above) selects this bare-name overload -- cast the Pane* arg at call sites.
void func_8013676C(void* pane, u32 val);
}

extern void func_80138078(u32);

typedef void (CMenuPTGauge::*CMenuPTGaugePtmf)(s32);
extern CMenuPTGaugePtmf lbl_eu_805323F8[];

void CMenuPTGauge::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    unk64.createRegion(handle, 0x1200, lbl_eu_805039C8, 0);
    Class_8045F858 regionGuard(&unk64);

    nw4r::lyt::ArcResourceAccessor* accessor;

    accessor = func_801355F4();
    func_80136E84(&unk74, accessor, lbl_eu_805039C8 + 0xd);

    accessor = func_801355F4();
    func_80136F08(unk74, &unk78, accessor, lbl_eu_805039C8 + 0x29);

    accessor = func_801355F4();
    func_80136F08(unk74, &unk7C, accessor, lbl_eu_805039C8 + 0x48);

    accessor = func_801355F4();
    func_80136F08(unk74, &unk80, accessor, lbl_eu_805039C8 + 0x6f);

    accessor = func_801355F4();
    func_80136F08(unk74, &unk84, accessor, lbl_eu_805039C8 + 0x96);

    accessor = func_801355F4();
    func_80136F08(unk74, &unk88, accessor, lbl_eu_805039C8 + 0xb7);

    // Retail: layout+0x10 is the root pane (GetRootPane inlines to this load).
    // CDeviceFont::func_80452C10(1, layout) returns an object whose vt+0x24
    // (no explicit args) yields the u32 passed to func_8013676C. Pass void*
    // so the call binds the unmangled reloc (not Pane*-mangled from
    // code_80135FDC.hpp).
    nw4r::lyt::Pane* rootPane = unk74->GetRootPane();
    void* fontObj = CDeviceFont::func_80452C10(1, unk74);
    typedef u32 (*FontVFn)(void*);
    u32 fontResult = (*reinterpret_cast<FontVFn**>(fontObj))[0x24 / 4](fontObj);
    func_8013676C(static_cast<void*>(rootPane), fontResult);

    unk74->Animate(0);
    unk74->UnbindAllAnimation();
    unk74->BindAnimation(unk78);
    unk74->SetAnimationEnable(unk78, true);

    // MI adjust: IScnRender at +0x5c (null-this safe), same pattern as
    // CMenuArtsSelect::Term.
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(reinterpret_cast<u8*>(this) + 0x5c);
    }
    mScn->addRenderCB(cb, 0xa, 0);

    unk64.func_8045F810();
}

void CMenuPTGauge::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    // Retail: rlwinm.; beq +8; b done. MWCC collapses if->goto to bne; keep beq
    // via fallthrough asm b (PLAN.md section 17.6 single-insn carve-out).
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) {
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
    if (lbl_eu_80663E24 & 0xAFA40000u) {
        goto done;
    }

    switch (unk8C) {
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

        unk94 = 0;
        unk90 = 0;
        unk8C = 1;

        if (unk74 != NULL) {
            if (unkA0 != 0) {
                unkA0 = 0;
                nw4r::lyt::Pane* pane =
                    unk74->GetRootPane()->FindPaneByName(lbl_eu_805039C8 + 0xd8, true);
                if (pane != NULL) {
                    nw4r::lyt::Size size;
                    size.width =
                        lbl_eu_806679E8 * (lbl_eu_806679E0 / lbl_eu_806679E4);
                    size.height = pane->GetSize().height;
                    pane->SetSize(size);
                }
            }
        }
        func_80138078(0xa0);
        break;
    }
    case 1: {
        if (func_80137444(unk78, lbl_eu_806679EC) != 0) {
            unk8C = 2;
        }
        break;
    }
    case 2: {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        s32 partyVal = *reinterpret_cast<s32*>(reinterpret_cast<u8*>(bm) + 0x194);

        (this->*lbl_eu_805323F8[unk90])(partyVal);

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
                unk8C = 3;
                unk74->Animate(0);
                unk74->UnbindAllAnimation();
                unk74->BindAnimation(unk78);
                unk74->SetAnimationEnable(unk78, true);
                func_80138078(0xa1);
                break;
            }
        }

        if (unk90 >= 1) {
            break;
        }

        bm = cf::CBattleManager::getInstance();
        s32 flag;
        u8 byte = *(reinterpret_cast<u8*>(bm) + 0x1aa);
        u16 lowerByte = byte;
        u32 upperByte = byte;
        flag = 0;
        if (lowerByte < 1) {
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
        if (byte != 5) {
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

        unk90 = 2;
        unk74->Animate(0);
        unk74->UnbindAllAnimation();
        unk74->BindAnimation(unk88);
        unk74->SetAnimationEnable(unk88, true);
        unk88->SetFrame(lbl_eu_806679E0);
        unk74->Animate(0);
        unk98 = unk94;
        break;
    }
    case 3: {
        if (func_80137510(static_cast<void*>(unk78), lbl_eu_806679EC) != 0) {
            unk8C = 0;
        }
        break;
    }
    default:
        break;
    }

    unk74->Animate(0);
done:
    ;
}

void CMenuPTGauge::cbRenderBefore() {
    // getInstance result discarded; feeds static func_800426F0 call schedule
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    // Retail: rlwinm.; beq +8; b done. MWCC collapses if->goto to bne; keep beq
    // via fallthrough asm b (PLAN.md section 17.6 single-insn carve-out).
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) {
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
    if (unk8C == 0) {
        goto done;
    }
    if (lbl_eu_80663E24 & 0xAFA40000u) {
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        func_80137038(unk74, &drawInfo, 0, 1);
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
    __dt__12CMenuPTGaugeFv((char*)p - 0x5c);
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
