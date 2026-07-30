#include "PowerPC_EABI_Support/MetroTRK/trk.h"
#include "PowerPC_EABI_Support/MetroTRK/mem_TRK.h"
#include "PowerPC_EABI_Support/MetroTRK/dolphin_trk.h"
#include "PowerPC_EABI_Support/MetroTRK/dolphin_trk_glue.h"
#include "PowerPC_EABI_Support/MetroTRK/__exception.h"
#include "PowerPC_EABI_Support/MetroTRK/mpc_7xx_603e.h"
#include "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/ppc_targimpl.h"
#include "PowerPC_EABI_Support/MetroTRK/main_TRK.h"
#include "PowerPC_EABI_Support/MetroTRK/rvl_mem.h"

extern u8 _db_stack_addr[];

#define EXCEPTION_SIZE  0x100
#define NUM_EXCEPTIONS  15

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

static ui32* lc_base;

static void InitMetroTRK_Inner(ui32 hwId) {
    // Save the CPU state: all GPRs, LR, CR, and MSR are captured into
    // gTRKCPUState so the debugger can inspect the pre-exception context.
    DECOMP_ASM_INSN_BEGIN
    asm {
        subi r1, r1, 4
        stw r3, 0(r1)
        lis r3, gTRKCPUState@h
        ori r3, r3, gTRKCPUState@l
        stmw r0, 0x0(r3)
        lwz r4, 0x0(r1)
        addi r1, r1, 4
        stw r1, 0x4(r3)
        stw r4, 0xc(r3)
        mflr r4
        stw r4, 0x84(r3)
        stw r4, 0x80(r3)
        mfcr r4
        stw r4, 0x88(r3)
        // Clear external interrupts (MSR_EE = 0) and preserve original MSR in SRR1.
        mfmsr r4
        ori r3, r4, 0x8000
        xori r3, r3, 0x8000
        mtmsr r3
        mtsrr1 r4
    }
    DECOMP_ASM_INSN_END

    TRKSaveExtended1Block();

    // Restore GPRs, reset IABR/DABR, switch to the debug stack, then
    // initialize the communication hardware.
    DECOMP_ASM_INSN_BEGIN
    asm {
        lis r3, gTRKCPUState@h
        ori r3, r3, gTRKCPUState@l
        lmw r0, 0x0(r3)
        li r0, 0x0
        mtspr IABR, r0
        mtspr DABR, r0
        lis r1, _db_stack_addr@h
        ori r1, r1, _db_stack_addr@l
        mr r3, r5
    }
    DECOMP_ASM_INSN_END

    if (InitMetroTRKCommTable(hwId) == 1) {
        // BUG: the code originally reloaded gTRKCPUState here, but as-is it
        // reads the return value of InitMetroTRKCommTable as a TRKCPUState
        // pointer, causing the CPU to return to a garbage code address.
        DECOMP_ASM_INSN_BEGIN
        asm {
            lwz r4, 0x84(r3)
            mtlr r4
            lmw r0, 0x0(r3)
        }
        DECOMP_ASM_INSN_END
        return;
    }

    TRK_main();
}

// r5: hardware id
void InitMetroTRK(void) {
    ui32 hwId;
    // Hardware ID is passed in r5 at entry (non-standard ABI from boot code).
    asm {
        mr r3, r5
        stw r3, hwId
    }
    InitMetroTRK_Inner(hwId);
}

void InitMetroTRK_BBA(void) {
    // BBA mode: hardware ID is fixed at HARDWARE_BBA (2).
    // Save the CPU state: all GPRs, LR, CR, and MSR are captured into
    // gTRKCPUState so the debugger can inspect the pre-exception context.
    DECOMP_ASM_INSN_BEGIN
    asm {
        subi r1, r1, 4
        stw r3, 0(r1)
        lis r3, gTRKCPUState@h
        ori r3, r3, gTRKCPUState@l
        stmw r0, 0x0(r3)
        lwz r4, 0x0(r1)
        addi r1, r1, 4
        stw r1, 0x4(r3)
        stw r4, 0xc(r3)
        mflr r4
        stw r4, 0x84(r3)
        stw r4, 0x80(r3)
        mfcr r4
        stw r4, 0x88(r3)
        // Turn on external interrupts (MSR_EE = 1) and preserve original MSR in SRR1.
        mfmsr r4
        ori r3, r4, 0x8000
        mtmsr r3
        mtsrr1 r4
    }
    DECOMP_ASM_INSN_END

    TRKSaveExtended1Block();

    // Restore GPRs, reset IABR/DABR, switch to the debug stack, then
    // initialize the communication hardware as BBA.
    DECOMP_ASM_INSN_BEGIN
    asm {
        lis r3, gTRKCPUState@h
        ori r3, r3, gTRKCPUState@l
        lmw r0, 0x0(r3)
        li r0, 0x0
        mtspr IABR, r0
        mtspr DABR, r0
        lis r1, _db_stack_addr@h
        ori r1, r1, _db_stack_addr@l
        li r3, 0x2
    }
    DECOMP_ASM_INSN_END

    if (InitMetroTRKCommTable(HARDWARE_BBA) == 1) {
        // BUG: the code originally reloaded gTRKCPUState here, but as-is it
        // reads the return value of InitMetroTRKCommTable as a TRKCPUState
        // pointer, causing the CPU to return to a garbage code address.
        DECOMP_ASM_INSN_BEGIN
        asm {
            lwz r4, 0x84(r3)
            mtlr r4
            lmw r0, 0x0(r3)
        }
        DECOMP_ASM_INSN_END
        return;
    }

    TRK_main();
}

void EnableMetroTRKInterrupts(){
    EnableEXI2Interrupts();
}

void* TRKTargetTranslate(ui32* addr) {
    if (addr >= lc_base && addr < &lc_base[0x1000]) {
        if (gTRKCPUState.Extended1.DBAT2L & 3) return addr;
    }
    if ((ui32)addr < 0x3000000) {
        return (void*)(((ui32)addr & 0x3FFFFFFF) | BOOTINFO);
    }
    if ((ui32)addr >= 0x10000000 && 0x1C000000 > (ui32)addr) {
        return (void*)(((ui32)addr & 0x3FFFFFFF) | MEM2_CACHED);
    }
    return addr;
}

static void TRK_copy_vector(ui32 offset){
    void* destPtr = (void*)TRKTargetTranslate((ui32*)offset);
    TRK_memcpy(destPtr, (void*)(gTRKInterruptVectorTable + offset), EXCEPTION_SIZE);
    TRK_flush_cache(destPtr, EXCEPTION_SIZE);
}

void __TRK_copy_vectors(){
    ui32* data_ptr;
    ui32* isrOffsetPtr;
    int i;
    ui32 data;

    if((ui32)lc_base <= DB_EXCEPTION_MASK && (ui32)&lc_base[0x1000] > DB_EXCEPTION_MASK && gTRKCPUState.Extended1.DBAT2L & 0x3){
        data_ptr = (ui32*)DB_EXCEPTION_MASK;
    }else{
        data_ptr = (ui32*)(BOOTINFO + DB_EXCEPTION_MASK);
    }

    isrOffsetPtr = TRK_ISR_OFFSETS;
    i = 0;
    data = *data_ptr;

    do{
        if((data & (1 << i)) != 0 && i != 4){
            TRK_copy_vector(*isrOffsetPtr);
        }

        i++;
        isrOffsetPtr++;
    }while(i <= 14);
}

DSError TRKInitializeTarget(){
    gTRKState.stopped = true;
    gTRKState.MSR = __TRK_get_MSR();
    lc_base = (ui32*)0xE0000000;
    return kNoError;
}

void __TRKreset() {
    OSResetSystem(0, 0, 0);
}
