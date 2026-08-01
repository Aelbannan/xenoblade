// Decompiled: WPADMem.c - Wii Remote memory block / extended register helpers
// High-level C reconstruction of RVL_SDK/src/revolution/wpad/WPADMem

#include <revolution/OS.h>
#include <revolution/wpad/WPAD.h>
#include <revolution/wpad/WPADInternal.h>

/* Defined in WPAD.c (retail bss object). */
extern WPADCB* __rvl_p_wpadcb[WPAD_MAX_CONTROLLERS];

void WPADiClearMemBlock(int index) {
    WPADCB* block = __rvl_p_wpadcb[index];
    block->memBlock_0x00 = 0;
    block->memBlock_0x04 = 0;
    block->memBlock_0x08 = 0;
    block->memBlock_0x0C = 0;
    block->memBlock_0x10 = NULL;
}

s32 WPADWriteExtReg(s32 chan, const void* pData, u32 len, u32 offset,
                    WPADCallback callback) {
    WPADCB* p_wpadcb = __rvl_p_wpadcb[chan];
    s32 status;
    BOOL handshakeFinished;
    s32 result;

    {
        BOOL enabled = OSDisableInterrupts();
        status = p_wpadcb->status;
        handshakeFinished = p_wpadcb->handshakeFinished;
        OSRestoreInterrupts(enabled);
    }

    result = status;
    if (status != -1) {
        if (handshakeFinished == 0) {
            result = WPAD_ERR_COMMUNICATION_ERROR;
        } else {
            BOOL sent = WPADiSendWriteData(&p_wpadcb->stdCmdQueue, pData, len,
                                           0x04A40000 | offset, callback);
            result = sent ? 0 : WPAD_ERR_COMMUNICATION_ERROR;
        }
    }

    if (result != WPAD_ERR_OK && callback != NULL) {
        callback(chan, result);
    }

    return result;
}
