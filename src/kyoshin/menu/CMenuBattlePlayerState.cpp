#include "kyoshin/menu/CMenuBattlePlayerState.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceVI.hpp"

#include "decomp.h"
#include <revolution/GX.h>

// Batch 2026-07-14g: menu-bps-cbrender owns cbRenderBefore exclusively.

extern "C" {
extern CMenuBattlePlayerState* lbl_eu_80663F48;
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
// Unmangled retail name; int (not u8) avoids clrlwi before cmpwi.
int func_8013BE50();
}

void CMenuBattlePlayerState::Term() {
    CDeviceVI::waitForDrawDone();
    mScn->removeRenderCB(this);

    for (u8 i = 0; i < 3; i++) {
        CMenuBattlePlayerStateSlot& slot = mSlots[i];

        if (slot.unk00 != NULL) {
            delete slot.unk00;
            slot.unk00 = NULL;
        }
        if (slot.unk08 != NULL) {
            delete slot.unk08;
            slot.unk08 = NULL;
        }
        if (slot.unk18 != NULL) {
            delete slot.unk18;
            slot.unk18 = NULL;
        }
        if (slot.unk20 != NULL) {
            delete slot.unk20;
            slot.unk20 = NULL;
        }
        if (slot.unk28 != NULL) {
            delete slot.unk28;
            slot.unk28 = NULL;
        }
        if (slot.unk78 != NULL) {
            delete slot.unk78;
            slot.unk78 = NULL;
        }
    }

    if (unk7E4 != NULL) {
        delete unk7E4;
        unk7E4 = NULL;
    }

    unk7D0.func_8045F778();
    unk64.func_8045F778();
    lbl_eu_80663F48 = NULL;
}

void CMenuBattlePlayerState::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    // Retail: rlwinm.; beq +8; b done. MWCC collapses if->goto to bne; keep beq
    // via fallthrough asm b (PLAN.md section 17.6). See MWCC_REFERENCE 8c9.
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
    if (unk7C9 != 0) {
        goto done;
    }
    if (lbl_eu_80663E24 & 0xAFE40000u) {
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);

        for (u8 i = 0; i < 3; i++) {
            CMenuBattlePlayerStateSlot& slot = mSlots[i];

            if (slot.unk244 == 0) {
                continue;
            }

            func_80137038(slot.unk00, &drawInfo, 0, (slot.unk25C & 0x40) != 0);

            if (slot.unk25C & 0x100) {
                func_80137038(slot.unk08, &drawInfo, 0, 1);
            }
            if (slot.unk25C & 0x200) {
                func_80137038(slot.unk28, &drawInfo, 0, 1);
            }
            func_80137038(slot.unk18, &drawInfo, 0, (slot.unk25C & 0x880) != 0);
            if (slot.unk25C & 0x400) {
                func_80137038(slot.unk20, &drawInfo, 0, 1);
            }

            if (slot.unk78 != NULL && slot.unk80 != 0) {
                func_80137038(slot.unk78, &drawInfo, 0, 1);
            }
        }

        if (unk7F8 != 0) {
            func_80137038(unk7E4, &drawInfo, 0, 1);
        }
    }
done:
    ;
}
