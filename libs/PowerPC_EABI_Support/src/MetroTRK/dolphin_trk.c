#include "PowerPC_EABI_Support/MetroTRK/trk.h"
#include "PowerPC_EABI_Support/MetroTRK/mem_TRK.h"
#include "PowerPC_EABI_Support/MetroTRK/dolphin_trk.h"
// Local declarations from dolphin_trk_glue.h (full header drags in
// <revolution/OS.h> -> OSFastCast.h statics that retail link-strips).
int InitMetroTRKCommTable(int);
void EnableEXI2Interrupts();
#include <revolution/os/OSInterrupt.h>
#include <revolution/os/OSReset.h>
#include "PowerPC_EABI_Support/MetroTRK/__exception.h"
#include "PowerPC_EABI_Support/MetroTRK/mpc_7xx_603e.h"
#include "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/ppc_targimpl.h"
#include "PowerPC_EABI_Support/MetroTRK/main_TRK.h"
#include "PowerPC_EABI_Support/MetroTRK/rvl_mem.h"

#define EXCEPTION_SIZE  0x100
#define NUM_EXCEPTIONS  15
#define LC_REGION_SIZE  0x1000     // locked-cache region size
#define MEM1_SIZE       0x03000000 // 48 MB MEM1
#define MEM1_MASK       0x3FFFFFFF
#define MEM2_LO         0x10000000 // MEM2 starts at 256 MB
#define MEM2_HI         0x1C000000 // MEM2 ends at 448 MB
#define LC_BASE_ADDR    0xE0000000 // locked-cache base in the CPU map

static ui32 TRK_ISR_OFFSETS[NUM_EXCEPTIONS] = {
    PPC_SYSTEMRESET,
    PPC_MACHINECHECK,
    PPC_DATAACCESSERROR,
    PPC_INSTACCESSERROR,
    PPC_EXTERNALINTERRUPT,
    PPC_ALIGNMENTERROR,
    PPC_PROGRAMERROR,
    PPC_FPUNAVAILABLE,
    PPC_DECREMENTERINTERRUPT,
    PPC_SYSTEMCALL,
    PPC_TRACE,
    PPC_PERFORMANCE_MONITOR,
    PPC7xx_603E_INSTR_ADDR_BREAK,
    PPC7xx_603E_SYS_MANAGE,
    PPC_THERMAL_MANAGE
};

enum { HARDWARE_BBA = 2 }; // from dolphin_trk_glue.h (see include note above)

static ui32* lc_base;

/*
 * Wii boot-entry vectors (policy exception, PLAN.md §17.6):
 * InitMetroTRK / InitMetroTRK_BBA are entered by the Wii boot code with a
 * non-standard ABI — no valid stack frame, no return address, hardware ID in
 * r5 — and their retail bodies hand-roll the GPR save/restore (stmw/lmw),
 * MSR/SRR1 and IABR/DABR SPR setup, and a fixed debug-stack switch. MWCC's
 * mandatory frame prologue makes a C reconstruction impossible. The matching
 * build therefore uses whole-function `asm void` + `nofralloc` transcribed
 * from retail for exactly these two boot vectors. Non-MWCC / PC builds select
 * the readable C fallback below (TRK is dead code on PC).
 */
#if defined(NONMATCHING) || defined(COMPAT_ANY) || !defined(__MWERKS__)

static void InitMetroTRK_Common(ui32 hwId) {
    if (InitMetroTRKCommTable(hwId) == 1) {
        return;
    }

    TRK_main();
}

void InitMetroTRK(void) {
    InitMetroTRK_Common(0);
}

void InitMetroTRK_BBA(void) {
    InitMetroTRK_Common(HARDWARE_BBA);
}

#else /* MWCC matching build — boot-entry vectors */

// r5: hardware id (non-standard boot ABI)
asm void InitMetroTRK(){
    nofralloc
    subi r1, r1, 4
    stw r3, 0(r1)
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    stmw r0, 0(r3)                  // Save all GPRs
    lwz r4, 0(r1)
    addi r1, r1, 4
    stw r1, 0x4(r3)                 // GPR[1] = entry stack pointer
    stw r4, 0xc(r3)                 // GPR[3] = original r3
    mflr r4
    stw r4, 0x84(r3)                // LR
    stw r4, 0x80(r3)                // PC
    mfcr r4
    stw r4, 0x88(r3)                // CR
    // Clear external interrupts (MSR_EE = 0), preserve original MSR in SRR1.
    mfmsr r4
    ori r3, r4, MSR_EE
    xori r3, r3, MSR_EE
    mtmsr r3
    mtsrr1 r4
    bl TRKSaveExtended1Block
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    lmw r0, 0(r3)                   // Restore all GPRs
    // Reset IABR and DABR, switch to the debug stack, init comm table.
    li r0, 0
    mtiabr r0
    mtdabr r0
    lis r1, 0x8067
    ori r1, r1, 0xd560
    mr r3, r5
    bl InitMetroTRKCommTable
    cmpwi r3, 1
    bne initCommTableSuccess
    // BUG (retail): reads the return value of InitMetroTRKCommTable as a
    // TRKCPUState pointer, returning to a garbage code address.
    lwz r4, 0x84(r3)
    mtlr r4
    lmw r0, 0(r3)
    blr
initCommTableSuccess:
    b TRK_main
    blr
}

asm void InitMetroTRK_BBA(){
    nofralloc
    subi r1, r1, 4
    stw r3, 0(r1)
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    stmw r0, 0(r3)                  // Save all GPRs
    lwz r4, 0(r1)
    addi r1, r1, 4
    stw r1, 0x4(r3)                 // GPR[1] = entry stack pointer
    stw r4, 0xc(r3)                 // GPR[3] = original r3
    mflr r4
    stw r4, 0x84(r3)                // LR
    stw r4, 0x80(r3)                // PC
    mfcr r4
    stw r4, 0x88(r3)                // CR
    // Turn on external interrupts (MSR_EE = 1), preserve original MSR in SRR1.
    mfmsr r4
    ori r3, r4, MSR_EE
    mtmsr r3
    mtsrr1 r4
    bl TRKSaveExtended1Block
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    lmw r0, 0(r3)                   // Restore all GPRs
    // Reset IABR and DABR, switch to the debug stack, init comm table as BBA.
    li r0, 0
    mtiabr r0
    mtdabr r0
    lis r1, 0x8067
    ori r1, r1, 0xd560
    li r3, 2 // HARDWARE_BBA
    bl InitMetroTRKCommTable
    cmpwi r3, 1
    bne initCommTableSuccess
    // BUG (retail): see InitMetroTRK.
    lwz r4, 0x84(r3)
    mtlr r4
    lmw r0, 0(r3)
    blr
initCommTableSuccess:
    b TRK_main
    blr
}

#endif /* NONMATCHING / COMPAT_ANY / !__MWERKS__ */

void EnableMetroTRKInterrupts(){
    EnableEXI2Interrupts();
}

void* TRKTargetTranslate(ui32* addr) {
    if (addr >= lc_base && addr < &lc_base[LC_REGION_SIZE]) {
        if (gTRKCPUState.Extended1.DBAT2L & 3) return addr;
    }
    if ((ui32)addr < MEM1_SIZE) {
        return (void*)(((ui32)addr & MEM1_MASK) | BOOTINFO);
    }
    if ((ui32)addr >= MEM2_LO && MEM2_HI > (ui32)addr) {
        return (void*)(((ui32)addr & MEM1_MASK) | MEM2_CACHED);
    }
    return addr;
}

void __TRK_copy_vectors(){
    ui32* data_ptr;
    ui32* isrOffsetPtr;
    int i;
    ui32 data;

    if((ui32)lc_base <= DB_EXCEPTION_MASK && (ui32)&lc_base[LC_REGION_SIZE] > DB_EXCEPTION_MASK && gTRKCPUState.Extended1.DBAT2L & 0x3){
        data_ptr = (ui32*)DB_EXCEPTION_MASK;
    }else{
        data_ptr = (ui32*)(BOOTINFO + DB_EXCEPTION_MASK);
    }

    isrOffsetPtr = TRK_ISR_OFFSETS;
    i = 0;
    data = *data_ptr;

    do{
        if((data & (1 << i)) != 0 && i != 4){
            {
                ui32 offset;
                void* destPtr;
                offset = *isrOffsetPtr;
                destPtr = (void*)TRKTargetTranslate((ui32*)offset);
                TRK_memcpy(destPtr, (void*)(gTRKInterruptVectorTable + offset), EXCEPTION_SIZE);
                TRK_flush_cache(destPtr, EXCEPTION_SIZE);
            }
        }

        i++;
        isrOffsetPtr++;
    }while(i <= 14);
}

DSError TRKInitializeTarget(){
    gTRKState.stopped = true;
    gTRKState.MSR = __TRK_get_MSR();
    lc_base = (ui32*)LC_BASE_ADDR;
    return kNoError;
}

void __TRKreset() {
    OSResetSystem(0, 0, 0);
}
