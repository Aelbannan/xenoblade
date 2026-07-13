.fn getInstance__Q22cf13CfGameManagerFv, global
/* 8007E418 00047658  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8007E41C 0004765C  7C 08 02 A6 */	mflr r0
/* 8007E420 00047660  90 01 00 14 */	stw r0, 0x14(r1)
/* 8007E424 00047664  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8007E428 00047668  88 0D A5 F0 */	lbz r0, lbl_eu_80663E70@sda21(r0)
/* 8007E42C 0004766C  7C 00 07 75 */	extsb. r0, r0
/* 8007E430 00047670  40 82 00 30 */	bne .L_8007E460
/* 8007E434 00047674  3F E0 80 57 */	lis r31, lbl_eu_80571758@ha
/* 8007E438 00047678  38 7F 17 58 */	addi r3, r31, lbl_eu_80571758@l
/* 8007E43C 0004767C  4B FF EA 19 */	bl __ct__Q22cf13CfGameManagerFv
/* 8007E440 00047680  3C 80 80 08 */	lis r4, __dt__Q22cf13CfGameManagerFv@ha
/* 8007E444 00047684  3C A0 80 57 */	lis r5, lbl_eu_80571748@ha
/* 8007E448 00047688  38 7F 17 58 */	addi r3, r31, lbl_eu_80571758@l
/* 8007E44C 0004768C  38 84 CE A8 */	addi r4, r4, __dt__Q22cf13CfGameManagerFv@l
/* 8007E450 00047690  38 A5 17 48 */	addi r5, r5, lbl_eu_80571748@l
/* 8007E454 00047694  48 23 DC B9 */	bl __register_global_object
/* 8007E458 00047698  38 00 00 01 */	li r0, 0x1
/* 8007E45C 0004769C  98 0D A5 F0 */	stb r0, lbl_eu_80663E70@sda21(r0)
.L_8007E460:
/* 8007E460 000476A0  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8007E464 000476A4  3C 60 80 57 */	lis r3, lbl_eu_80571758@ha
/* 8007E468 000476A8  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8007E46C 000476AC  38 63 17 58 */	addi r3, r3, lbl_eu_80571758@l
/* 8007E470 000476B0  7C 08 03 A6 */	mtlr r0
/* 8007E474 000476B4  38 21 00 10 */	addi r1, r1, 0x10
/* 8007E478 000476B8  4E 80 00 20 */	blr
.endfn getInstance__Q22cf13CfGameManagerFv