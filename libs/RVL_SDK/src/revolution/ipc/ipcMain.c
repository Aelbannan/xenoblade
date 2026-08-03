#include <revolution/IPC.h>
#include <revolution/OS.h>

static u8 Initialized = FALSE;
static void* IPCCurrentBufferLo = NULL;
static void* IPCCurrentBufferHi = NULL;
/* Retail .sbss slice is 0x18: the buffers then 4 zero pad bytes; a plain
   4-byte zero-init global lands in .sbss. */
u32 __IpcMainSbssPad;
static void* IPCBufferLo = NULL;
static void* IPCBufferHi = NULL;

void IPCInit(void) {
    if (!Initialized) {
        IPCBufferHi = __OSGetIPCBufferHi();
        IPCBufferLo = __OSGetIPCBufferLo();
        IPCCurrentBufferHi = IPCBufferHi;
        IPCCurrentBufferLo = IPCBufferLo;
        Initialized = TRUE;
    }
}

void IPCReInit(void) {
    Initialized = FALSE;
    IPCBufferHi = __OSGetIPCBufferHi();
    IPCBufferLo = __OSGetIPCBufferLo();
    IPCCurrentBufferHi = IPCBufferHi;
    IPCCurrentBufferLo = IPCBufferLo;
    Initialized = TRUE;
}

u32 IPCReadReg(s32 index) {
    return IPC_PPC_HW_REGS[index];
}

void IPCWriteReg(s32 index, u32 value) {
    IPC_PPC_HW_REGS[index] = value;
}

void* IPCGetBufferHi(void) {
    return IPCCurrentBufferHi;
}

void* IPCGetBufferLo(void) {
    return IPCCurrentBufferLo;
}

// unused in Xenoblade retail: IPCSetBufferHi

void IPCSetBufferLo(void* lo) {
    IPCCurrentBufferLo = lo;
}
