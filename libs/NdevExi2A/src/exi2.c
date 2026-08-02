// Low-level EXI channel 2 primitives for the Ndev debugger: immediate
// transfers and register/RAM read-write helpers used by DebuggerDriver.c.
#include <revolution/EXI.h>
#include "NdevExi2A/exi2.h"
#include <revolution/OS.h>

// EXI_CHAN_PARAMS (EXIHardware.h) is at 0xCD006800 on Wii; on GameCube the
// EXI registers live at 0xCC006800.

static BOOL __EXI2Select(void) {
    // Keep the interrupt-mask bits (0x405 = EXIINTMASK | TCINTMASK |
    // EXTINTMASK) while raising chip select + clock (0xC0).
    u32 temp = EXI_CHAN_PARAMS[EXI_CHAN_2].cpr;
    EXI_CHAN_PARAMS[EXI_CHAN_2].cpr = ((temp & 0x405) | 0xC0);
    return TRUE;
}

static BOOL __EXI2Deselect(void) {
    // Drop chip select / clock, keep the interrupt-mask bits.
    u32 temp = EXI_CHAN_PARAMS[EXI_CHAN_2].cpr;
    EXI_CHAN_PARAMS[EXI_CHAN_2].cpr = temp & 0x405;
    return TRUE;
}

static BOOL __EXI2Sync(void) {
    // Wait until the transfer-in-progress bit (EXI_CR_TSTART) clears.
    while (EXI_CHAN_PARAMS[EXI_CHAN_2].cr & 0x1) {
    }
    return TRUE;
}

BOOL __EXI2Imm(void* mem, s32 size, u32 type) {
    int i;

    if (type != EXI_READ) {
        u32 imm = 0;

        for (i = 0; i < size; i++) {
            // Immediate data is 4 bytes, MSB first: byte i -> bits (3-i)*8.
            const u8* bmem = (const u8*)mem;
            imm |= bmem[i] << (3 - i) * 8;
        }

        EXI_CHAN_PARAMS[EXI_CHAN_2].data = imm;
    }

    // CR fields: EXI_CR_RW = type<<2, EXI_CR_TSTART = 1, EXI_CR_TLEN = (size-1)*16.
    EXI_CHAN_PARAMS[EXI_CHAN_2].cr = type << 2 | 1 | (size - 1) * 16;
    __EXI2Sync();

    if (type == EXI_READ) {
        u32 imm = EXI_CHAN_PARAMS[EXI_CHAN_2].data;

        u8* bmem = (u8*)mem;
        for (i = 0; i < size; i++, bmem++) {
            *bmem = imm >> (3 - i) * 8;
        }
    }

    return TRUE;
}

void __DBEXIInit(void) {
    u32 val0, val1;

    __OSMaskInterrupts(OS_INTR_MASK(OS_INTR_EXI_2_EXI) |
                       OS_INTR_MASK(OS_INTR_EXI_2_TC));
    // Wait for any in-flight transfer, then release the bus.
    while ((EXI_CHAN_PARAMS[EXI_CHAN_2].cr & 1) == 1U) {
    }
    EXI_CHAN_PARAMS[EXI_CHAN_2].cpr = 0;

    // Debugger command words (bit 31 set = write direction).
    val0 = 0xB4000000;
    val1 = 0xD4000000;

    __EXI2Select();
    __EXI2Imm(&val0, 4, EXI_WRITE);
    __EXI2Sync();

    __EXI2Imm(&val1, 4, EXI_WRITE);
    __EXI2Sync();
    __EXI2Deselect();
}

BOOL __DBEXIReadReg(u32 cmd, void* mem, s32 size) {
    BOOL error = FALSE;
    u32 read_val = 0;

    error = error | !__EXI2Select();
    error = error | !__EXI2Imm(&cmd, sizeof(cmd), EXI_WRITE);
    error = error | !__EXI2Sync();
    error = error | !__EXI2Imm(&read_val, sizeof(read_val), EXI_READ);
    error = error | !__EXI2Sync();
    error = error | !__EXI2Deselect();

    switch (size) {
    case 1:
        *(u8*)mem = read_val >> 24;
        break;
    case 2:
        *(u16*)mem = read_val >> 24 | read_val >> 8 & 0xFF00;
        break;
    default: // size 4
        *(u32*)mem = __EXISwap32(read_val);
        break;
    }

    return error == FALSE;
}

BOOL __DBEXIWriteReg(u32 cmd, const void* mem, s32 size) {
    BOOL error = FALSE;
    u32 write_val = 0;

    switch (size) {
    case 1:
        write_val = *(u8*)mem << 24;
        break;
    case 2:
        write_val = *(u16*)mem << 24 | (*(u16*)mem & 0xFF00) << 8;
        break;
    default:
        write_val = __EXISwap32(*(u32*)mem);
        break;
    }

    error = error | !__EXI2Select();
    error = error | !__EXI2Imm(&cmd, sizeof(cmd), EXI_WRITE);
    error = error | !__EXI2Sync();
    error = error | !__EXI2Imm(&write_val, sizeof(write_val), EXI_WRITE);
    error = error | !__EXI2Sync();
    error = error | !__EXI2Deselect();

    return error == FALSE;
}

BOOL __DBEXIReadRam(u32 cmd, void* mem, s32 size) {
    BOOL error = FALSE;
    u32 read_val; // Filled by the EXI_READ immediate transfer below.
    u32* lmem = (u32*)mem;

    error = error | !__EXI2Select();
    error = error | !__EXI2Imm(&cmd, sizeof(cmd), EXI_WRITE);
    error = error | !__EXI2Sync();

    // Read 32-bit words (size must be a multiple of 4).
    for (; size > 0; size -= 4) {
        error = error | !__EXI2Imm(&read_val, sizeof(read_val), EXI_READ);
        error = error | !__EXI2Sync();
        *lmem++ = read_val;
    }

    error = error | !__EXI2Deselect();

    return error == FALSE;
}

BOOL __DBEXIWriteRam(u32 cmd, const void* mem, s32 size) {
    BOOL error = FALSE;
    u32* lmem = (u32*)mem;

    error = error | !__EXI2Select();
    error = error | !__EXI2Imm(&cmd, sizeof(cmd), EXI_WRITE);
    error = error | !__EXI2Sync();

    // Write 32-bit words (size must be a multiple of 4).
    for (; size > 0; size -= 4) {
        u32 write_val = *lmem++;
        error = error | !__EXI2Imm(&write_val, sizeof(write_val), EXI_WRITE);
        error = error | !__EXI2Sync();
    }

    error = error | !__EXI2Deselect();

    return error == FALSE;
}
