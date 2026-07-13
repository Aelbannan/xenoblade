#include "kyoshin/menu/CMenuEnemyState.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include "decomp.h"
#include <revolution/GX.h>

// Batch 2026-07-14f: menu-enemy-cbrender owns cbRenderBefore exclusively.

extern "C" {
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
// Unmangled retail name; int (not u8) avoids clrlwi before cmpwi.
int func_8013BE50();
}

void CMenuEnemyState::cbRenderBefore() {
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
    if (lbl_eu_80663E24 & 0xAFA40000u) {
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);

        u32 indices[0x18];
        u8 fill;
        for (fill = 0; fill < 0x18; fill++) {
            indices[fill] = fill;
        }

        // Outer for(left!=0) -> mtctr/bdnz. Inner bottom-tested goto keeps CTR outer.
        // Rematerialize &indices[j] each step (retail addi r1,0x8). Load depthB before
        // depthA so fcmpo uses f1/f0. XOR via store-reload of order[j]/pair[1].
        {
            u32* order = indices;
            u8 pass = 0;
            u32 left;
            for (left = 0x17; left != 0; left--) {
                u8 swapped = 0;
                u8 pass8 = pass;
                s32 limit = 0x17 - pass8;
                u8 j = 0;
                goto sort_test;
            sort_body: {
                    u32* pair = &indices[j];
                    u32 idxA = order[j];
                    u32 idxB = pair[1];
                    f32 depthB = *reinterpret_cast<f32*>(
                        reinterpret_cast<u8*>(this) + idxB * 0x4c + 0xbc);
                    f32 depthA = *reinterpret_cast<f32*>(
                        reinterpret_cast<u8*>(this) + idxA * 0x4c + 0xbc);
                    if (depthA > depthB) {
                        u32 tmp = order[j] ^ pair[1];
                        order[j] = tmp;
                        tmp = pair[1] ^ tmp;
                        pair[1] = tmp;
                        tmp = order[j] ^ tmp;
                        order[j] = tmp;
                        swapped = 1;
                    }
                    j++;
                }
            sort_test:
                if ((s32)j < limit) {
                    goto sort_body;
                }
                if (swapped == 0) {
                    break;
                }
                pass++;
            }
        }

        // NV decl order entry, order, i -> r30/r29/r28 (MWCC_REFERENCE 8c6).
        {
            u8* entry;
            u32* order;
            u8 i;
            order = indices;
            i = 0;
            do {
                entry = reinterpret_cast<u8*>(this) + order[i] * 0x4c;
                if (entry[0xb9] == 0) {
                    goto draw_next;
                }
                if (entry[0xcd] == 0) {
                    goto draw_next;
                }
                if (entry[0xcc] != 0) {
                    func_80137038(unk74, &drawInfo, 0, 1);
                }
                {
                    u8 cc = entry[0xcc];
                    int drawFlag = (cc == 0);
                    func_80137038(
                        *reinterpret_cast<nw4r::lyt::Layout**>(entry + 0xb0),
                        &drawInfo, drawFlag, 1);
                }
                if (entry[0xb8] == 0) {
                    func_80137038(
                        *reinterpret_cast<nw4r::lyt::Layout**>(entry + 0xa8),
                        &drawInfo, 0, 1);
                }
            draw_next:
                i++;
            } while (i < 0x18);
        }

        if (unk828 != 0) {
            func_80137038(unk800, &drawInfo, 0, 1);
        }
    }
done:
    ;
}
