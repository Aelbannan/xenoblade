#include "kyoshin/menu/CMenuArtsSelect.hpp"

#include "monolib/device/CDeviceVI.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include "decomp.h"
#include <revolution/GX.h>

extern "C" {
void* func_80110A70();
void* func_8010CE48();
void func_8010EDE4(void*);
void func_8010A8E4(void*);
void func_80133770();

// Batch 2026-07-14g: menu-arts-cbrender owns cbRenderBefore exclusively.
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
// Unmangled retail names; int (not u8) avoids clrlwi before cmpwi.
int func_8013BE50();
int func_8018A608();
int func_80122448();
}

void CMenuArtsSelect::Term() {
    // Retail clears lbl_eu_80663F24 (not a mangled spInstance).
    if (lbl_eu_80663F24 != NULL) {
        lbl_eu_80663F24->unkB0 = NULL;
        lbl_eu_80663F24->unk68 |= 0x40u;
        lbl_eu_80663F24 = NULL;
    }

    CDeviceVI::waitForDrawDone();

    // MI adjust: IScnRender at +0x5c (null-this safe).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(reinterpret_cast<u8*>(this) + 0x5c);
    }
    mScn->removeRenderCB(cb);

    // Sliding base matches retail r30 += 4 over 9 iterations.
    u8* base = reinterpret_cast<u8*>(this);
    s32 i = 0;
    do {
        nw4r::lyt::Layout*& a = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0x1b8);
        if (a != NULL) {
            delete a;
            a = NULL;
        }
        nw4r::lyt::Layout*& b = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0x170);
        if (b != NULL) {
            delete b;
            b = NULL;
        }
        nw4r::lyt::Layout*& c = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0x104);
        if (c != NULL) {
            delete c;
            c = NULL;
        }
        if (i < 8) {
            nw4r::lyt::Layout*& d = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0xa4);
            if (d != NULL) {
                delete d;
                d = NULL;
            }
        }
        i++;
        base += 4;
    } while (i < 9);

    if (unk98 != NULL) {
        delete unk98;
        unk98 = NULL;
    }
    if (unk8C != NULL) {
        delete unk8C;
        unk8C = NULL;
    }
    if (unk80 != NULL) {
        delete unk80;
        unk80 = NULL;
    }
    if (unk294 != NULL) {
        delete unk294;
        unk294 = NULL;
    }

    unk6C.func_8045F778();

    lbl_eu_80663F20 = NULL;

    // Retail calls each getter twice when non-null.
    if (func_80110A70() != NULL) {
        func_8010EDE4(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4);
    }
    if (func_8010CE48() != NULL) {
        func_8010A8E4(reinterpret_cast<u8*>(func_8010CE48()) + 0x7cc);
    }
    if (unk334 != 0) {
        func_80133770();
    }
}

void CMenuArtsSelect::cbRenderBefore() {
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
    if (func_8018A608()) {
        goto done;
    }
    if (func_80122448()) {
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);

        if (unk298 > 1 && !(unk308 & 0x80u)) {
            for (s32 i = 0; i < 9; i++) {
                if (unk30C & (1u << i)) {
                    func_80137038(unk1B8[i], &drawInfo, 0, 1);
                }
            }
        }

        {
            u32 flags = unk308;
            u32 notBit25 = !(flags & 0x40u);
            u32 merged = notBit25 | (flags & 0x30u);
            int visible = merged != 0;
            func_80137038(unk80, &drawInfo, 0, visible);
        }

        unk308 |= 0x40u;

        if (unk298 > 1) {
            if (unk328 == 4 && (unk308 & 0x2u)) {
                u32 bit0 = unk308 & 0x1u;
                u32 notBit28 = !(unk308 & 0x8u);
                u32 merged = bit0 | notBit28;
                int visible = merged != 0;
                func_80137038(unk98, &drawInfo, 0, visible);
                unk308 |= 0x8u;
            } else {
                unk308 &= ~0x8u;
            }

            if (!(unk308 & 0x80u)) {
                for (s32 i = 0; i < 8; i++) {
                    u32 v = unk318;
                    if (v & (1u << i)) {
                        u32 bit18 = 1u << (i + 18);
                        u32 notBit18 = !(v & bit18);
                        u32 merged = notBit18 | (v & (1u << (i + 9)));
                        int visible = merged != 0;
                        if (unk340 == 0) {
                            func_80137038(unkA4[i], &drawInfo, 0, visible);
                        }
                        unk318 |= bit18;
                    } else {
                        unk318 &= ~(1u << (i + 18));
                    }
                }

                for (s32 i = 0; i < 9; i++) {
                    if (unk200[(i + 1) % 9]->unkBB & 1) {
                        u32 v = unk310;
                        u32 bitI = 1u << i;
                        u32 bit18 = 1u << (i + 18);
                        if (v & bitI) {
                            u32 notBit18 = !(v & bit18);
                            u32 merged = notBit18 | (v & (1u << (i + 9)));
                            int visible = merged != 0;
                            func_80137038(unk104[i], &drawInfo, 0, visible);
                            unk310 |= bit18;
                        } else {
                            unk310 &= ~bit18;
                        }

                        if (unk2C0[i] == 0xC) {
                            u32 w = unk314;
                            if (w & bitI) {
                                u32 notBit18 = !(w & bit18);
                                u32 merged = notBit18 | (w & (1u << (i + 9)));
                                int visible = merged != 0;
                                func_80137038(unk170[i], &drawInfo, 0, visible);
                                unk314 |= bit18;
                            } else {
                                unk314 &= ~bit18;
                            }
                        } else {
                            unk314 &= ~bit18;
                        }
                    } else {
                        unk310 &= ~(1u << (i + 18));
                        unk314 &= ~(1u << (i + 18));
                    }
                }
            }
        }

        func_80137038(unk8C, &drawInfo, 0, 1);
    }
done:
    ;
}
