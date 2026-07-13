
# .text:0x0 | 0x80447250 | size: 0x60
# CWorkSystemMem::CWorkSystemMem(const char*, CWorkThread*)
.fn __ct__14CWorkSystemMemFPCcP11CWorkThread, global
/* 80447250 00410490  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 80447254 00410494  7C 08 02 A6 */	mflr r0
/* 80447258 00410498  38 C0 00 01 */	li r6, 0x1
/* 8044725C 0041049C  90 01 00 14 */	stw r0, 0x14(r1)
/* 80447260 004104A0  93 E1 00 0C */	stw r31, 0xc(r1)
/* 80447264 004104A4  7C 7F 1B 78 */	mr r31, r3
/* 80447268 004104A8  4B FF 2C ED */	bl __ct__11CWorkThreadFPCcP11CWorkThreadi
/* 8044726C 004104AC  3C 60 80 57 */	lis r3, lbl_eu_8056BAA8@ha
/* 80447270 004104B0  38 00 FF FF */	li r0, -0x1
/* 80447274 004104B4  38 63 BA A8 */	addi r3, r3, lbl_eu_8056BAA8@l
/* 80447278 004104B8  90 7F 00 00 */	stw r3, 0x0(r31)
/* 8044727C 004104BC  90 1F 01 C4 */	stw r0, 0x1c4(r31)
/* 80447280 004104C0  93 ED BD A0 */	stw r31, lbl_eu_80665620@sda21(r0)
/* 80447284 004104C4  4B FE FA B5 */	bl getHandleMEM2__Q23mtl10MemManagerFv
/* 80447288 004104C8  38 BF 00 04 */	addi r5, r31, 0x4
/* 8044728C 004104CC  38 80 0F E0 */	li r4, 0xfe0
/* 80447290 004104D0  4B FE F7 49 */	bl create__Q23mtl10MemManagerFUlUlPCc
/* 80447294 004104D4  90 7F 01 C4 */	stw r3, 0x1c4(r31)
/* 80447298 004104D8  7F E3 FB 78 */	mr r3, r31
/* 8044729C 004104DC  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 804472A0 004104E0  80 01 00 14 */	lwz r0, 0x14(r1)
/* 804472A4 004104E4  7C 08 03 A6 */	mtlr r0
/* 804472A8 004104E8  38 21 00 10 */	addi r1, r1, 0x10
/* 804472AC 004104EC  4E 80 00 20 */	blr
.endfn __ct__14CWorkSystemMemFPCcP11CWorkThread
