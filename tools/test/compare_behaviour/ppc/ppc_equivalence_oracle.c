#include "ppc_equivalence_oracle.h"

/*
 * Independent Broadway oracle for tools/ppc_equivalence.
 *
 * This lives under tools/test rather than src/libs: exact instructions are
 * intentional here. r3 is the output array and r4 is naturally aligned RAM.
 * The caller's CR, XER, CTR, and r28-r31 are restored before returning. Keeping
 * the oracle ABI-clean is important because several cases deliberately modify
 * all CR fields rather than only volatile CR0/CR1.
 */
asm void ppc_equivalence_oracle(u32* output, u32* memory) {
    nofralloc

    stwu r1, -32(r1)
    mfcr r0
    stw r0, 0(r1)
    mfxer r0
    stw r0, 4(r1)
    mfctr r0
    stw r0, 8(r1)
    stmw r28, 16(r1)

    /* 0: addc. 0xffffffff + 1 -> 0, CA */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, -1
    li r6, 1
    addc. r7, r5, r6
    stw r7, 0(r3)
    mfcr r8
    stw r8, 4(r3)
    mfxer r8
    stw r8, 8(r3)

    /* 1: addeo. 0x7fffffff + 0 + CA -> 0x80000000, OV/SO */
    li r0, 0
    mtcrf 0xff, r0
    lis r0, 0x2000
    mtxer r0
    lis r5, 0x7fff
    ori r5, r5, 0xffff
    li r6, 0
    addeo. r7, r5, r6
    stw r7, 12(r3)
    mfcr r8
    stw r8, 16(r3)
    mfxer r8
    stw r8, 20(r3)

    /* 2: addmeo. 0x80000000 with CA=0 -> 0x7fffffff, CA/OV/SO */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0x8000
    addmeo. r7, r5
    stw r7, 24(r3)
    mfcr r8
    stw r8, 28(r3)
    mfxer r8
    stw r8, 32(r3)

    /* 3: subfc. 1 from 0 -> -1, no CA */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, 1
    li r6, 0
    subfc. r7, r5, r6
    stw r7, 36(r3)
    mfcr r8
    stw r8, 40(r3)
    mfxer r8
    stw r8, 44(r3)

    /* 4: subfeo. 0 from 0 with CA=0 -> -1 */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, 0
    li r6, 0
    subfeo. r7, r5, r6
    stw r7, 48(r3)
    mfcr r8
    stw r8, 52(r3)
    mfxer r8
    stw r8, 56(r3)

    /* 5: nego. INT_MIN -> INT_MIN, OV/SO; CA is preserved */
    li r0, 0
    mtcrf 0xff, r0
    lis r0, 0x2000
    mtxer r0
    lis r5, 0x8000
    nego. r7, r5
    stw r7, 60(r3)
    mfcr r8
    stw r8, 64(r3)
    mfxer r8
    stw r8, 68(r3)

    /* 6: mulhw. -2 * 3 -> high word -1 */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, -2
    li r6, 3
    mulhw. r7, r5, r6
    stw r7, 72(r3)
    mfcr r8
    stw r8, 76(r3)
    mfxer r8
    stw r8, 80(r3)

    /* 7: mulhwu. 0xffffffff * 2 -> high word 1 */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, -1
    li r6, 2
    mulhwu. r7, r5, r6
    stw r7, 84(r3)
    mfcr r8
    stw r8, 88(r3)
    mfxer r8
    stw r8, 92(r3)

    /* 8: mullwo. 0x40000000 * 4 -> 0 with OV/SO */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0x4000
    li r6, 4
    mullwo. r7, r5, r6
    stw r7, 96(r3)
    mfcr r8
    stw r8, 100(r3)
    mfxer r8
    stw r8, 104(r3)

    /* 9: divw. -7 / 3 -> -2 (truncates toward zero) */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, -7
    li r6, 3
    divw. r7, r5, r6
    stw r7, 108(r3)
    mfcr r8
    stw r8, 112(r3)
    mfxer r8
    stw r8, 116(r3)

    /* 10: divwu. UINT_MAX / 2 -> 0x7fffffff */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, -1
    li r6, 2
    divwu. r7, r5, r6
    stw r7, 120(r3)
    mfcr r8
    stw r8, 124(r3)
    mfxer r8
    stw r8, 128(r3)

    /* 11: slw. 1 << 31 */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, 1
    li r6, 31
    slw. r7, r5, r6
    stw r7, 132(r3)
    mfcr r8
    stw r8, 136(r3)
    mfxer r8
    stw r8, 140(r3)

    /* 12: slw. with bit 5 of shift count set -> zero */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, 1
    li r6, 32
    slw. r7, r5, r6
    stw r7, 144(r3)
    mfcr r8
    stw r8, 148(r3)
    mfxer r8
    stw r8, 152(r3)

    /* 13: srw. 0x80000000 >> 31 -> 1 */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0x8000
    li r6, 31
    srw. r7, r5, r6
    stw r7, 156(r3)
    mfcr r8
    stw r8, 160(r3)
    mfxer r8
    stw r8, 164(r3)

    /* 14: sraw. -1 by 1 -> -1 and CA */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, -1
    li r6, 1
    sraw. r7, r5, r6
    stw r7, 168(r3)
    mfcr r8
    stw r8, 172(r3)
    mfxer r8
    stw r8, 176(r3)

    /* 15: srawi. 0x80000001 by 1 -> 0xc0000000 and CA */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0x8000
    ori r5, r5, 1
    srawi. r7, r5, 1
    stw r7, 180(r3)
    mfcr r8
    stw r8, 184(r3)
    mfxer r8
    stw r8, 188(r3)

    /* 16: rlwinm. 3 * 4 -> 12 */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, 3
    rlwinm. r7, r5, 2, 0, 29
    stw r7, 192(r3)
    mfcr r8
    stw r8, 196(r3)
    mfxer r8
    stw r8, 200(r3)

    /* 17: rlwimi. inserts source bits into the existing destination */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0x1234
    ori r5, r5, 0x5678
    lis r7, 0xaabb
    ori r7, r7, 0xccdd
    rlwimi. r7, r5, 8, 8, 15
    stw r7, 204(r3)
    mfcr r8
    stw r8, 208(r3)
    mfxer r8
    stw r8, 212(r3)

    /* 18: rlwnm. rotate 0x80000001 left by 1 -> 3 */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0x8000
    ori r5, r5, 1
    li r6, 1
    rlwnm. r7, r5, r6, 0, 31
    stw r7, 216(r3)
    mfcr r8
    stw r8, 220(r3)
    mfxer r8
    stw r8, 224(r3)

    /* 19: andc. */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0xf0f0
    ori r5, r5, 0xf0f0
    lis r6, 0xff00
    ori r6, r6, 0xff00
    andc. r7, r5, r6
    stw r7, 228(r3)
    mfcr r8
    stw r8, 232(r3)
    mfxer r8
    stw r8, 236(r3)

    /* 20: eqv. */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0xaaaa
    ori r5, r5, 0xaaaa
    lis r6, 0x0f0f
    ori r6, r6, 0x0f0f
    eqv. r7, r5, r6
    stw r7, 240(r3)
    mfcr r8
    stw r8, 244(r3)
    mfxer r8
    stw r8, 248(r3)

    /* 21: nand. */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0xffff
    ori r5, r5, 0x0000
    lis r6, 0x0f0f
    ori r6, r6, 0x0f0f
    nand. r7, r5, r6
    stw r7, 252(r3)
    mfcr r8
    stw r8, 256(r3)
    mfxer r8
    stw r8, 260(r3)

    /* 22: nor. */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0xf000
    lis r6, 0x0f00
    nor. r7, r5, r6
    stw r7, 264(r3)
    mfcr r8
    stw r8, 268(r3)
    mfxer r8
    stw r8, 272(r3)

    /* 23: orc. */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0x1234
    ori r5, r5, 0x5678
    lis r6, 0xffff
    orc. r7, r5, r6
    stw r7, 276(r3)
    mfcr r8
    stw r8, 280(r3)
    mfxer r8
    stw r8, 284(r3)

    /* 24: xor. */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 0xaaaa
    ori r5, r5, 0x5555
    lis r6, 0xffff
    xor. r7, r5, r6
    stw r7, 288(r3)
    mfcr r8
    stw r8, 292(r3)
    mfxer r8
    stw r8, 296(r3)

    /* 25: extsb. 0x80 -> 0xffffff80 */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, 0x80
    extsb. r7, r5
    stw r7, 300(r3)
    mfcr r8
    stw r8, 304(r3)
    mfxer r8
    stw r8, 308(r3)

    /* 26: extsh. 0x8001 -> 0xffff8001 */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, 0
    ori r5, r5, 0x8001
    extsh. r7, r5
    stw r7, 312(r3)
    mfcr r8
    stw r8, 316(r3)
    mfxer r8
    stw r8, 320(r3)

    /* 27: cntlzw. 0x00010000 -> 15 */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    lis r5, 1
    cntlzw. r7, r5
    stw r7, 324(r3)
    mfcr r8
    stw r8, 328(r3)
    mfxer r8
    stw r8, 332(r3)

    /* 28: cmpw -1,1 -> LT */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, -1
    li r6, 1
    cmpw r5, r6
    li r7, 0
    stw r7, 336(r3)
    mfcr r8
    stw r8, 340(r3)
    mfxer r8
    stw r8, 344(r3)

    /* 29: cmplw UINT_MAX,1 -> GT */
    li r0, 0
    mtcrf 0xff, r0
    mtxer r0
    li r5, -1
    li r6, 1
    cmplw r5, r6
    li r7, 0
    stw r7, 348(r3)
    mfcr r8
    stw r8, 352(r3)
    mfxer r8
    stw r8, 356(r3)

    /* 30: CR logical operations with PowerPC bit numbering. */
    lis r0, 0x4000
    mtcrf 0xff, r0
    li r0, 0
    mtxer r0
    crand 0, 1, 2
    crorc 3, 1, 2
    creqv 4, 1, 2
    crxor 5, 1, 2
    mfcr r7
    stw r7, 360(r3)
    stw r7, 364(r3)
    mfxer r8
    stw r8, 368(r3)

    /* 31: explicit mtxer/mfxer transfer. */
    li r0, 0
    mtcrf 0xff, r0
    lis r5, 0xe000
    mtxer r5
    mfxer r7
    stw r7, 372(r3)
    mfcr r8
    stw r8, 376(r3)
    stw r7, 380(r3)

    /* 32: stw + lbz, big-endian high byte. */
    lis r5, 0x1122
    ori r5, r5, 0x3344
    stw r5, 0(r4)
    lbz r7, 0(r4)
    stw r7, 384(r3)
    li r0, 0
    stw r0, 388(r3)
    stw r0, 392(r3)

    /* 33: lhz, big-endian low halfword. */
    lhz r7, 2(r4)
    stw r7, 396(r3)
    stw r0, 400(r3)
    stw r0, 404(r3)

    /* 34: sth + lha sign extension. */
    ori r5, r0, 0x8001
    sth r5, 4(r4)
    lha r7, 4(r4)
    stw r7, 408(r3)
    stw r0, 412(r3)
    stw r0, 416(r3)

    /* 35: stwbrx + lwbrx round trip. */
    lis r5, 0x1122
    ori r5, r5, 0x3344
    li r6, 8
    stwbrx r5, r4, r6
    lwbrx r7, r4, r6
    stw r7, 420(r3)
    stw r0, 424(r3)
    stw r0, 428(r3)

    /* 36: stwu writes and updates the base register. */
    addi r10, r4, 16
    stwu r5, -4(r10)
    subf r7, r4, r10
    stw r7, 432(r3)
    stw r0, 436(r3)
    stw r0, 440(r3)

    /* 37: lmw/stmw preserve word order. */
    lmw r28, 16(r4)
    stmw r28, 48(r4)
    xor r7, r28, r29
    xor r7, r7, r30
    xor r7, r7, r31
    stw r7, 444(r3)
    li r0, 0
    stw r0, 448(r3)
    stw r0, 452(r3)

    /* 38: a CR-selected conditional branch is taken. */
    lis r0, 0x8000
    mtcrf 0xff, r0
    li r0, 0
    mtxer r0
    li r7, 0
    bc 12, 0, cr_branch_taken
    li r7, 1
cr_branch_taken:
    stw r7, 456(r3)
    mfcr r8
    stw r8, 460(r3)
    mfxer r8
    stw r8, 464(r3)

    /* 39: bdnz decrements CTR and takes the branch while CTR is nonzero. */
    li r0, 2
    mtctr r0
    li r7, 0
    bdnz ctr_branch_taken
    li r7, 1
ctr_branch_taken:
    slwi r7, r7, 8
    mfctr r8
    or r7, r7, r8
    stw r7, 468(r3)
    li r0, 0
    stw r0, 472(r3)
    stw r0, 476(r3)

    lmw r28, 16(r1)
    lwz r0, 8(r1)
    mtctr r0
    lwz r0, 4(r1)
    mtxer r0
    lwz r0, 0(r1)
    mtcrf 0xff, r0
    addi r1, r1, 32

    blr
}
