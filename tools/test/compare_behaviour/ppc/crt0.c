#include <string.h>

#include <PowerPC_EABI_Support/Runtime/__ppc_eabi_linker.h>

#include "behaviour_result.h"

int behaviour_main(void);

extern void exit(int code);

DECL_SECTION(".init") static asm void __init_registers(void) {
    nofralloc
    li r0, 0
    li r3, 0
    li r4, 0
    li r5, 0
    li r6, 0
    li r7, 0
    li r8, 0
    li r9, 0
    li r10, 0
    li r11, 0
    li r12, 0
    li r14, 0
    li r15, 0
    li r16, 0
    li r17, 0
    li r18, 0
    li r19, 0
    li r20, 0
    li r21, 0
    li r22, 0
    li r23, 0
    li r24, 0
    li r25, 0
    li r26, 0
    li r27, 0
    li r28, 0
    li r29, 0
    li r30, 0
    li r31, 0
    lis r1, _stack_addr@h
    ori r1, r1, _stack_addr@l
    lis r2, _SDA2_BASE_@h
    ori r2, r2, _SDA2_BASE_@l
    lis r13, _SDA_BASE_@h
    ori r13, r13, _SDA_BASE_@l
    blr
}

DECL_SECTION(".init") static void __copy_rom_section(void* dst, const void* src, size_t size) {
    if (size == 0 || dst == src) {
        return;
    }
    memcpy(dst, src, size);
}

DECL_SECTION(".init") static void __init_bss_section(void* dst, size_t size) {
    if (size == 0) {
        return;
    }
    memset(dst, 0, size);
}

DECL_SECTION(".init") static void __init_data(void) {
    const __rom_copy_info* rs;
    const __bss_init_info* bs;

    for (rs = _rom_copy_info; rs->size != 0; rs++) {
        __copy_rom_section(rs->addr, rs->rom, rs->size);
    }

    for (bs = _bss_init_info; bs->size != 0; bs++) {
        __init_bss_section(bs->addr, bs->size);
    }
}

DECL_SECTION(".init") asm void __start(void) {
    nofralloc
    bl __init_registers
    li r0, -1
    stwu r1, -8(r1)
    stw r0, 4(r1)
    stw r0, 0(r1)
    bl __init_data
    bl behaviour_main
    bl exit
}
