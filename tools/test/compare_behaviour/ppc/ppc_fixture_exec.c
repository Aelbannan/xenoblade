#include <string.h>

#include "ppc_fixture_exec.h"

/*
 * Generic Broadway fixture executor for tools/ppc_equivalence.
 *
 * Each fixture’s instruction words are compiled into .text as
 * ppc_fixture_payload_N (see ppc_fixture_payloads.c). This harness loads the
 * shared corpus state, calls that payload via blrl (preserving CTR), and
 * captures finals.
 */

typedef struct PpcFixtureLiveState {
    u32 gpr[32];
    u64 fpr[32];
    u64 fpscr_image;
    u32 cr;
    u32 xer;
    u32 lr;
    u32 ctr;
} PpcFixtureLiveState;

static PpcFixtureLiveState s_live;

/*
 * r3 = live state, r4 = payload function
 * Loads architectural state from *r3, blrl to the payload, writes finals back.
 * Harness r1 is preserved. Fixture CTR is preserved across the call.
 */
static asm void ppc_fixture_enter(PpcFixtureLiveState* live, PpcFixturePayloadFn payload) {
    nofralloc

    mflr r0
    stw r0, 4(r1)
    stwu r1, -288(r1)
    stmw r14, 8(r1)
    mfcr r0
    stw r0, 80(r1)
    mfxer r0
    stw r0, 84(r1)
    mfctr r0
    stw r0, 88(r1)
    /* Preserve ABI SDA pointers; fixture payloads must not clobber harness C. */
    stw r2, 92(r1)
    stw r13, 96(r1)
    stfd f14, 104(r1)
    stfd f15, 112(r1)
    stfd f16, 120(r1)
    stfd f17, 128(r1)
    stfd f18, 136(r1)
    stfd f19, 144(r1)
    stfd f20, 152(r1)
    stfd f21, 160(r1)
    stfd f22, 168(r1)
    stfd f23, 176(r1)
    stfd f24, 184(r1)
    stfd f25, 192(r1)
    stfd f26, 200(r1)
    stfd f27, 208(r1)
    stfd f28, 216(r1)
    stfd f29, 224(r1)
    stfd f30, 232(r1)
    stfd f31, 240(r1)

    mr r14, r3
    mr r15, r4

    lfd f0, 384(r14)
    mtfsf 0xff, f0
    lwz r0, 392(r14)
    mtcrf 0xff, r0
    lwz r0, 396(r14)
    mtxer r0
    lwz r0, 404(r14)
    mtctr r0

    lfd f0, 128(r14)
    lfd f1, 136(r14)
    lfd f2, 144(r14)
    lfd f3, 152(r14)
    lfd f4, 160(r14)
    lfd f5, 168(r14)
    lfd f6, 176(r14)
    lfd f7, 184(r14)
    lfd f8, 192(r14)
    lfd f9, 200(r14)
    lfd f10, 208(r14)
    lfd f11, 216(r14)
    lfd f12, 224(r14)
    lfd f13, 232(r14)
    lfd f14, 240(r14)
    lfd f15, 248(r14)
    lfd f16, 256(r14)
    lfd f17, 264(r14)
    lfd f18, 272(r14)
    lfd f19, 280(r14)
    lfd f20, 288(r14)
    lfd f21, 296(r14)
    lfd f22, 304(r14)
    lfd f23, 312(r14)
    lfd f24, 320(r14)
    lfd f25, 328(r14)
    lfd f26, 336(r14)
    lfd f27, 344(r14)
    lfd f28, 352(r14)
    lfd f29, 360(r14)
    lfd f30, 368(r14)
    lfd f31, 376(r14)

    lwz r0, 0(r14)
    /* Keep harness r2/r13: do not load fixture values into them. */
    lwz r3, 12(r14)
    lwz r4, 16(r14)
    lwz r5, 20(r14)
    lwz r6, 24(r14)
    lwz r7, 28(r14)
    lwz r8, 32(r14)
    lwz r9, 36(r14)
    lwz r10, 40(r14)
    lwz r11, 44(r14)
    lwz r12, 48(r14)
    lwz r16, 64(r14)
    lwz r17, 68(r14)
    lwz r18, 72(r14)
    lwz r19, 76(r14)
    lwz r20, 80(r14)
    lwz r21, 84(r14)
    lwz r22, 88(r14)
    lwz r23, 92(r14)
    lwz r24, 96(r14)
    lwz r25, 100(r14)
    lwz r26, 104(r14)
    lwz r27, 108(r14)
    lwz r28, 112(r14)
    lwz r29, 116(r14)
    lwz r30, 120(r14)
    lwz r31, 124(r14)

    mtlr r15
    blrl

    stfd f0, 128(r14)
    stfd f1, 136(r14)
    stfd f2, 144(r14)
    stfd f3, 152(r14)
    stfd f4, 160(r14)
    stfd f5, 168(r14)
    stfd f6, 176(r14)
    stfd f7, 184(r14)
    stfd f8, 192(r14)
    stfd f9, 200(r14)
    stfd f10, 208(r14)
    stfd f11, 216(r14)
    stfd f12, 224(r14)
    stfd f13, 232(r14)
    stfd f14, 240(r14)
    stfd f15, 248(r14)
    stfd f16, 256(r14)
    stfd f17, 264(r14)
    stfd f18, 272(r14)
    stfd f19, 280(r14)
    stfd f20, 288(r14)
    stfd f21, 296(r14)
    stfd f22, 304(r14)
    stfd f23, 312(r14)
    stfd f24, 320(r14)
    stfd f25, 328(r14)
    stfd f26, 336(r14)
    stfd f27, 344(r14)
    stfd f28, 352(r14)
    stfd f29, 360(r14)
    stfd f30, 368(r14)
    stfd f31, 376(r14)
    mffs f0
    stfd f0, 384(r14)

    stw r0, 0(r14)
    stw r3, 12(r14)
    stw r4, 16(r14)
    stw r5, 20(r14)
    stw r6, 24(r14)
    stw r7, 28(r14)
    stw r8, 32(r14)
    stw r9, 36(r14)
    stw r10, 40(r14)
    stw r11, 44(r14)
    stw r12, 48(r14)
    stw r16, 64(r14)
    stw r17, 68(r14)
    stw r18, 72(r14)
    stw r19, 76(r14)
    stw r20, 80(r14)
    stw r21, 84(r14)
    stw r22, 88(r14)
    stw r23, 92(r14)
    stw r24, 96(r14)
    stw r25, 100(r14)
    stw r26, 104(r14)
    stw r27, 108(r14)
    stw r28, 112(r14)
    stw r29, 116(r14)
    stw r30, 120(r14)
    stw r31, 124(r14)

    mfcr r0
    stw r0, 392(r14)
    mfxer r0
    stw r0, 396(r14)
    mfctr r0
    stw r0, 404(r14)

    lwz r2, 92(r1)
    lwz r13, 96(r1)
    lfd f14, 104(r1)
    lfd f15, 112(r1)
    lfd f16, 120(r1)
    lfd f17, 128(r1)
    lfd f18, 136(r1)
    lfd f19, 144(r1)
    lfd f20, 152(r1)
    lfd f21, 160(r1)
    lfd f22, 168(r1)
    lfd f23, 176(r1)
    lfd f24, 184(r1)
    lfd f25, 192(r1)
    lfd f26, 200(r1)
    lfd f27, 208(r1)
    lfd f28, 216(r1)
    lfd f29, 224(r1)
    lfd f30, 232(r1)
    lfd f31, 240(r1)
    lwz r0, 80(r1)
    mtcrf 0xff, r0
    lwz r0, 84(r1)
    mtxer r0
    lwz r0, 88(r1)
    mtctr r0
    lmw r14, 8(r1)
    lwz r0, 292(r1)
    mtlr r0
    addi r1, r1, 288
    blr
}

void ppc_fixture_run_case(const PpcFixtureCase* fixture, u32* sandbox, PpcFixtureActual* actual) {
    u32 index;

    memcpy(s_live.gpr, fixture->gpr, sizeof(s_live.gpr));
    memcpy(s_live.fpr, fixture->fpr, sizeof(s_live.fpr));
    s_live.fpscr_image = fixture->fpscr_image;
    s_live.cr = fixture->cr;
    s_live.xer = fixture->xer;
    s_live.lr = fixture->lr;
    s_live.ctr = fixture->ctr;

    if (fixture->uses_memory) {
        memcpy(sandbox, fixture->mem, sizeof(u32) * PPC_FIXTURE_MEM_WORDS);
        s_live.gpr[4] = (u32)sandbox;
        if (s_live.gpr[10] >= 0x00001000u && s_live.gpr[10] < 0x00001000u + (PPC_FIXTURE_MEM_WORDS * 4u)) {
            s_live.gpr[10] = (u32)sandbox + (s_live.gpr[10] - 0x00001000u);
        }
    } else {
        memset(sandbox, 0, sizeof(u32) * PPC_FIXTURE_MEM_WORDS);
    }

    ppc_fixture_enter(&s_live, fixture->payload);

    actual->result = s_live.gpr[fixture->result_reg];
    actual->cr = s_live.cr;
    actual->xer = s_live.xer;
    memcpy(actual->fpr, s_live.fpr, sizeof(actual->fpr));
    actual->fpscr = (u32)s_live.fpscr_image;
    for (index = 0; index < PPC_FIXTURE_MEM_WORDS; ++index) {
        actual->mem[index] = sandbox[index];
    }
}
