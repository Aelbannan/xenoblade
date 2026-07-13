# cf::CTaskCulling::ICulling_UnkVirtualFunc1(ml::CFrustum*)
.fn ICulling_UnkVirtualFunc1__Q22cf12CTaskCullingFPQ22ml8CFrustum, global
/* 801A4340 0016D580  80 0D AA A8 */	lwz r0, lbl_eu_80664328@sda21(r0)
/* 801A4344 0016D584  2C 00 00 00 */	cmpwi r0, 0x0
/* 801A4348 0016D588  40 82 00 0C */	bne .L_801A4354
/* 801A434C 0016D58C  38 60 00 00 */	li r3, 0x0
/* 801A4350 0016D590  4E 80 00 20 */	blr
.L_801A4354:
/* 801A4354 0016D594  38 63 00 60 */	addi r3, r3, 0x60
/* 801A4358 0016D598  4B FF E2 C8 */	b func_801A0F04__11COccCullingFPQ22ml8CFrustum
/* 801A435C 0016D59C  4E 80 00 20 */	blr
.endfn ICulling_UnkVirtualFunc1__Q22cf12CTaskCullingFPQ22ml8CFrustum
