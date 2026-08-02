#ifndef RVL_SDK_NDEV_EXI2_AD_DEBUGGER_DRIVER_H
#define RVL_SDK_NDEV_EXI2_AD_DEBUGGER_DRIVER_H
#include <types.h>

#include <NdevExi2A/exi2.h>

#include <revolution/OS.h>
#ifdef __cplusplus
extern "C" {
#endif

void DBInitComm(u8** flagOut, OSInterruptHandler handler);
void DBInitInterrupts(void);
u32 DBQueryData(void);
BOOL DBRead(void* dst, u32 size);
BOOL DBWrite(const void* src, u32 size);
void DBOpen(void);
void DBClose(void);

// EXI2 command words: bit 31 (0x80000000) marks the write direction; the
// remaining bits select the debugger device register/address to touch.
static BOOL __DBReadMailbox(u32* mailOut) {
    return __DBEXIReadReg(0x34000200, mailOut, sizeof(*mailOut));
}

// Translate the host offset into the EXI2 RAM command word (address field).
static BOOL __DBRead(u32 ofs, void* dest, u32 size) {
    return __DBEXIReadRam(((ofs + 0xD10000) * 0x40) & 0x3FFFFF00, dest, size);
}

static BOOL __DBWriteMailbox(u32 mail) {
    return __DBEXIWriteReg(0xB4000100, &mail, sizeof(mail));
}

static BOOL __DBWrite(u32 ofs, const void* src, u32 size) {
    return __DBEXIWriteRam(
        (((ofs + 0xD10000) * 0x40) & 0x3FFFFF00) | 0x80000000, src, size);
}

#ifdef __cplusplus
}
#endif
#endif
