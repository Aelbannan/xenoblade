#include "kyoshin/CUICfManager.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"

#include <decomp.h>

// §17.6 whole-function asm: packed tail copy + -0x1A0/stmw frame not recoverable
// in high-level C++ (60% STRUCTURAL). User-approved "Fix it".

extern "C" {
CProcess* lbl_eu_80664054;
const char* lbl_eu_806621A8;
char lbl_eu_8052E404[];
char lbl_eu_8052E3BC[];
u32 __ptmf_null[3];
void __ct__8CProcessFv(CProcess*);
void* getWorkMem__17CWorkThreadSystemFv(void);
void* allocate__Q23mtl10MemManagerFUlUl(u32, u32);
CFileHandle* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, IWorkEvent*, int, int);
void func_8044F154__11CDeviceFileFP11CFileHandlei(CFileHandle*, int);
void Regist__8CProcessFP8CProcessb(CProcess*, CProcess*, bool);
void func_8015704C(void*, const void*);
void func_8009D0B4();
void func_8009D514(cf::IFlagEvent*);
}

asm void CUICfManager::Init() {
    nofralloc
    stwu r1, -0x1a0(r1)
    mflr r0
    cmpwi r3, 0x0
    stw r0, 0x1a4(r1)
    stmw r22, 0x178(r1)
    mr r23, r3
    mr r5, r23
    beq lbl_80132EEC
    addi r5, r3, 0x54
lbl_80132EEC:
    lwz r3, 0x118(r3)
    li r6, 0x0
    lwz r4, lbl_eu_806621A8
    li r7, 0x0
    bl readFile__11CDeviceFileFUlPCcP10IWorkEventii
    stw r3, 0x114(r23)
    li r4, 0x3
    bl func_8044F154__11CDeviceFileFP11CFileHandlei
    bl getWorkMem__17CWorkThreadSystemFv
    mr r4, r3
    li r3, 0x54
    bl allocate__Q23mtl10MemManagerFUlUl
    cmpwi r3, 0x0
    mr r22, r3
    beq lbl_80132F7C
    bl __ct__8CProcessFv
    lis r3, lbl_eu_8052E404@ha
    lis r5, __ptmf_null@ha
    addi r3, r3, lbl_eu_8052E404@l
    stw r3, 0x10(r22)
    addi r5, r5, __ptmf_null@l
    lwz r0, 0x4(r5)
    lis r3, lbl_eu_8052E3BC@ha
    lwz r4, 0x0(r5)
    addi r3, r3, lbl_eu_8052E3BC@l
    stw r4, 0x3c(r22)
    stw r0, 0x40(r22)
    lwz r0, 0x8(r5)
    stw r0, 0x44(r22)
    lwz r0, 0x4(r5)
    lwz r4, 0x0(r5)
    stw r4, 0x48(r22)
    stw r0, 0x4c(r22)
    lwz r0, 0x8(r5)
    stw r0, 0x50(r22)
    stw r3, 0x10(r22)
lbl_80132F7C:
    stw r22, 0x144(r23)
    mr r3, r22
    li r5, 0x0
    lwz r4, lbl_eu_80664054
    bl Regist__8CProcessFP8CProcessb
    addi r27, r1, 0xdc
    addi r26, r1, 0x40
    addi r25, r1, 0x74
    addi r24, r1, 0xa8
    li r28, 0x0
    li r30, 0x0
    li r31, 0x7
    li r22, 0x34
lbl_80132FB0:
    clrlwi r0, r28, 24
    addi r3, r1, 0x40
    mulli r0, r0, 0x168
    sth r30, 0x10(r1)
    cmplw r3, r27
    stw r30, 0xc(r1)
    add r29, r23, r0
    bge lbl_80132FF4
    addi r0, r27, 0x33
    subf r0, r3, r0
    divwu r0, r0, r22
    mtctr r0
    bge lbl_80132FF4
lbl_80132FE4:
    sth r30, 0x4(r3)
    stw r30, 0x0(r3)
    addi r3, r3, 0x34
    bdnz lbl_80132FE4
lbl_80132FF4:
    stb r30, 0x9(r1)
    addi r3, r29, 0x150
    addi r4, r1, 0xc
    stw r30, 0xdc(r1)
    sth r30, 0xe0(r1)
    sth r30, 0xe2(r1)
    sth r30, 0xe4(r1)
    sth r30, 0xe6(r1)
    sth r30, 0xe8(r1)
    sth r30, 0xea(r1)
    stb r30, 0xec(r1)
    stb r30, 0xee(r1)
    stb r30, 0x12e(r1)
    stb r31, 0x8(r1)
    stb r31, 0x14c(r29)
    lbz r0, 0x9(r1)
    stb r0, 0x14d(r29)
    bl func_8015704C
    mr r4, r26
    addi r3, r29, 0x184
    bl func_8015704C
    mr r4, r25
    addi r3, r29, 0x1b8
    bl func_8015704C
    mr r4, r24
    addi r3, r29, 0x1ec
    bl func_8015704C
    lwz r0, 0xdc(r1)
    addi r28, r28, 0x1
    stw r0, 0x220(r29)
    cmplwi r28, 0x8
    lwz r0, 0xe4(r1)
    lwz r3, 0xe0(r1)
    stw r3, 0x224(r29)
    stw r0, 0x228(r29)
    lhz r0, 0xe8(r1)
    sth r0, 0x22c(r29)
    lwz r0, 0xea(r1)
    stw r0, 0x22e(r29)
    lwz r0, 0xf2(r1)
    lwz r3, 0xee(r1)
    stw r3, 0x232(r29)
    stw r0, 0x236(r29)
    lwz r0, 0xfa(r1)
    lwz r3, 0xf6(r1)
    stw r3, 0x23a(r29)
    stw r0, 0x23e(r29)
    lwz r0, 0x102(r1)
    lwz r3, 0xfe(r1)
    stw r3, 0x242(r29)
    stw r0, 0x246(r29)
    lwz r0, 0x10a(r1)
    lwz r3, 0x106(r1)
    stw r3, 0x24a(r29)
    stw r0, 0x24e(r29)
    lwz r0, 0x112(r1)
    lwz r3, 0x10e(r1)
    stw r3, 0x252(r29)
    stw r0, 0x256(r29)
    lwz r0, 0x11a(r1)
    lwz r3, 0x116(r1)
    stw r3, 0x25a(r29)
    stw r0, 0x25e(r29)
    lwz r0, 0x122(r1)
    lwz r3, 0x11e(r1)
    stw r3, 0x262(r29)
    stw r0, 0x266(r29)
    lwz r0, 0x12a(r1)
    lwz r3, 0x126(r1)
    stw r3, 0x26a(r29)
    stw r0, 0x26e(r29)
    lwz r0, 0x132(r1)
    lwz r3, 0x12e(r1)
    stw r3, 0x272(r29)
    stw r0, 0x276(r29)
    lwz r0, 0x13a(r1)
    lwz r3, 0x136(r1)
    stw r3, 0x27a(r29)
    stw r0, 0x27e(r29)
    lwz r0, 0x142(r1)
    lwz r3, 0x13e(r1)
    stw r3, 0x282(r29)
    stw r0, 0x286(r29)
    lwz r0, 0x14a(r1)
    lwz r3, 0x146(r1)
    stw r3, 0x28a(r29)
    stw r0, 0x28e(r29)
    lwz r0, 0x152(r1)
    lwz r3, 0x14e(r1)
    stw r3, 0x292(r29)
    stw r0, 0x296(r29)
    lwz r0, 0x15a(r1)
    lwz r3, 0x156(r1)
    stw r3, 0x29a(r29)
    stw r0, 0x29e(r29)
    lwz r0, 0x162(r1)
    lwz r3, 0x15e(r1)
    stw r3, 0x2a2(r29)
    stw r0, 0x2a6(r29)
    lwz r0, 0x16a(r1)
    lwz r3, 0x166(r1)
    stw r3, 0x2aa(r29)
    stw r0, 0x2ae(r29)
    blt lbl_80132FB0
    lmw r22, 0x178(r1)
    lwz r0, 0x1a4(r1)
    mtlr r0
    addi r1, r1, 0x1a0
    blr
}

void CUICfManager::Term() {
    func_801390E0(&mFileHandle);

    mtl::MemManager::deallocate(mPackedFont60.Destroy());
    mtl::MemManager::deallocate(mPackedFont9C.Destroy());
    mtl::MemManager::deallocate(mPackedFontD8.Destroy());

    func_80139124(mArcResourceAccessor);
    mArcResourceAccessor = NULL;
    unk118 = -1;

    cf::IFlagEvent* flagEvent = this;
    func_8009D0B4();
    func_8009D514(flagEvent);

    unk144->unk39 = 1;
    lbl_eu_80664054 = NULL;
}
