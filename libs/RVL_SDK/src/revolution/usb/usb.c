#include <revolution/FS.h>
#include <revolution/OS.h>
#include <revolution/USB.h>

#include <stdio.h>
#include <string.h>
void OSReport(const char* msg, ...);

#define USB_HEAP_SIZE 0x4000

typedef enum {
    USB_NCLEAN_CLOSEDEVICE = 0,
    USB_NCLEAN_MSG2 = 2,
    USB_NCLEAN_BULKMSG = 3,
    USB_NCLEAN_MSG4 = 4,
    USB_NCLEAN_CTRLMSG = 7,
    USB_NCLEAN_MAX = 8
} USBNClean;

typedef enum {
    USB_IOCTLV_CTRLMSG,
    USB_IOCTLV_BLKMSG,
    USB_IOCTLV_INTRMSG
} USBIoctl;

typedef struct USBMsg {
    void* buffer; // at 0x0
    u16 length;   // at 0x4
} USBMsg;

typedef struct USBCommandBlock {
    USBCallback callback; // at 0x0
    USBISOCallback isoCallback; // at 0x4
    void* callbackArg;    // at 0x8
    void* isoCallbackArg; // at 0xc
    char UNK_0x10[0x4];
    void* clean[USB_NCLEAN_MAX]; // at 0x14
    u32 nclean;                  // at 0x34
    char UNK_0x38[0x40 - 0x38];
    union {
        char path[FS_MAX_PATH];
        USBMsg msg;
    }; // at 0x40
} USBCommandBlock;

static s32 hId = -1;

static void* lo = NULL;
static void* hi = NULL;

static u8 s_usb_log = FALSE;
static u8 s_usb_err = TRUE;

//unused
// unused in Xenoblade retail: _usb_enable_log, _usb_disable_log

static void USB_LOG(const char* fmt, ...) {
    va_list list;

    if (s_usb_log) {
        OSReport("USB: ");
        va_start(list, fmt);
        vprintf(fmt, list);
        va_end(list);
    }
}

static void USB_ERR(const char* fmt, ...) {
    va_list list;

    if (s_usb_err) {
        OSReport("USB ERR: ");
        va_start(list, fmt);
        vprintf(fmt, list);
        va_end(list);
    }
}

static void* IOSAlloc(size_t size) {
    void* mem;

    mem = iosAllocAligned(hId, size, 32);

    if (mem == NULL) {
        USB_ERR("iosAllocAligned(%d, %u) failed: %d\n", hId, size, mem);
    }

    return mem;
}

static void IOSFree(void* mem) {
    s32 result;

    if (mem != NULL) {
        result = iosFree(hId, mem);

        if (result < 0) {
            USB_ERR("iosFree(%d, 0x%x) failed: %d\n", hId, mem, result);
        }
    }
}

IPCResult IUSB_OpenLib(void) {
    IPCResult result;
    BOOL enabled;

    result = IPC_RESULT_OK;
    enabled = OSDisableInterrupts();

    if (hId != -1) {
        USB_LOG("Library is already initialized. Heap Id = %d\n", hId);
        goto end;
    }

    if (lo == NULL) {
        lo = IPCGetBufferLo();
        hi = IPCGetBufferHi();

        USB_LOG("iusb size: %d lo: %x hi: %x\n", sizeof(USBCommandBlock), lo,
                hi);

        if ((u8*)lo + USB_HEAP_SIZE > hi) {
            USB_ERR("Not enough IPC arena\n");
            result = IPC_RESULT_ALLOC_FAILED;
            goto end;
        }

        IPCSetBufferLo((u8*)lo + USB_HEAP_SIZE);
    }

    hId = iosCreateHeap(lo, USB_HEAP_SIZE);
    if (hId < 0) {
        USB_ERR("Not enough heaps\n");
        result = IPC_RESULT_ALLOC_FAILED;
    }

end:
    OSRestoreInterrupts(enabled);
    return result;
}

IPCResult IUSB_CloseLib(void) {
    return IPC_RESULT_OK;
}

static s32 _intrBlkCtrlIsoCb(s32 result, void* arg) {
    int i;
    USBCommandBlock* block = (USBCommandBlock*)arg;

    USB_LOG("_intrBlkCtrlIsoCb returned: %d\n", result);
    USB_LOG("_intrBlkCtrlIsoCb: nclean = %d\n", block->nclean);

    if (block->nclean != USB_NCLEAN_CTRLMSG &&
        block->nclean != USB_NCLEAN_BULKMSG &&
        block->nclean != USB_NCLEAN_CLOSEDEVICE &&
        block->nclean != USB_NCLEAN_MSG4 &&
        block->nclean != USB_NCLEAN_MSG2) {
        USB_ERR("__intrBlkCtrlIsoCb: got invalid nclean\n");
    } else {
        for (i = 0; i < block->nclean; i++) {
            USB_LOG("Freeing clean[%d] = %x\n", i, block->clean[i]);
            IOSFree(block->clean[i]);
        }
        block->nclean = 0;
    }

    USB_LOG("cb = %x cbArg = %x\n", block->callback, block->callbackArg);

    if (block->callback != NULL) {
        block->callback(result, block->callbackArg);
    } else if(block->isoCallback != NULL) {
        USB_LOG("calling iso callback\n");
        block->isoCallback(result, block->isoCallbackArg, block->callbackArg);
    }

    IOSFree(block);
    return result;
}


IPCResult IUSB_OpenDeviceIds(const char* interface, u16 vid, u16 pid,
                             IPCResult* resultOut) {
    IPCResult result;
    USBCommandBlock* block;

    block = NULL;

    if (resultOut == NULL) {
        result = IPC_RESULT_INVALID_INTERNAL;
        goto end;
    }

    block = IOSAlloc(sizeof(USBCommandBlock));
    if (block == NULL) {
        USB_ERR("OpenDeviceIds: Not enough memory\n");
        result = IPC_RESULT_ALLOC_FAILED;
        goto end;
    }

    memset(block, 0, sizeof(USBCommandBlock));
    snprintf(block->path, sizeof(block->path), "/dev/usb/%s/%x/%x", interface,
             vid, pid);
    USB_LOG("OpenDevice - %s\n", block->path);

    result = IOS_Open(block->path, IPC_OPEN_NONE);
    USB_LOG("OpenDevice returned: %d\n", result);

    *resultOut = result;

end:
    IOSFree(block);
    return result;
}



// unused in Xenoblade retail: IUSB_OpenDeviceIdsAsync, IUSB_CloseDevice

// Retail keeps these strings although the function using them is absent
// (IUSB_OpenDeviceIdsAsync). Explicit .data bytes (OSError/dsp_task model),
// positioned so the section comes out byte-identical to retail.
extern __declspec(section ".data") const unsigned char sdata_keepalive_usb_A[52] = {
    0x4F, 0x70, 0x65, 0x6E, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x0A, 0x00, 0x4F, 0x70, 0x65, 0x6E,
    0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x49, 0x64, 0x73, 0x41, 0x73, 0x79, 0x6E, 0x63, 0x3A, 0x20,
    0x4E, 0x6F, 0x74, 0x20, 0x65, 0x6E, 0x6F, 0x75, 0x67, 0x68, 0x20, 0x6D, 0x65, 0x6D, 0x6F, 0x72,
    0x79, 0x0A, 0x00, 0x00,
};

// IUSB_CloseDeviceAsync's own log strings, held as static objects (not literals):
// MWCC emits function literals in source-token order, which would place
// "CloseDevice returned" after "CloseDeviceAsync: ..."; retail has it before,
// so the bytes live here at retail offsets and the function references them.
static __declspec(section ".data") const char sCloseDevice[13] = {
    0x43, 0x6C, 0x6F, 0x73, 0x65, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x0A, 0x00,
};

static __declspec(section ".data") const char sCloseDeviceReturned[26] = {
    0x43, 0x6C, 0x6F, 0x73, 0x65, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x20, 0x72, 0x65, 0x74, 0x75,
    0x72, 0x6E, 0x65, 0x64, 0x3A, 0x20, 0x25, 0x64, 0x0A, 0x00,
};

IPCResult IUSB_CloseDeviceAsync(s32 fd, USBCallback callback,
                                void* callbackArg) {
    IPCResult result;
    USBCommandBlock* block;

    USB_LOG(sCloseDevice);

    block = IOSAlloc(sizeof(USBCommandBlock));
    if (block == NULL) {
        USB_ERR("CloseDeviceAsync: Not enough memory\n");
        result = IPC_RESULT_ALLOC_FAILED;
        goto end;
    }
    memset(block, 0, sizeof(USBCommandBlock));
    block->callback = callback;
    block->callbackArg = callbackArg;
    block->nclean = USB_NCLEAN_CLOSEDEVICE;

    result = IOS_CloseAsync(fd, _intrBlkCtrlIsoCb, block);
    USB_LOG(sCloseDeviceReturned, result);

    if (result < 0) {
        IOSFree(block);
    }

end:
    return result;
}



// unused in Xenoblade retail: __openDevice, IUSB_GetDeviceList



// Retail keeps these strings although the function using them is absent.
extern __declspec(section ".data") const unsigned char sdata_keepalive_usb_B[72] = {
    0x6F, 0x70, 0x65, 0x6E, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x3A, 0x20, 0x4E, 0x6F, 0x74, 0x20,
    0x65, 0x6E, 0x6F, 0x75, 0x67, 0x68, 0x20, 0x6D, 0x65, 0x6D, 0x6F, 0x72, 0x79, 0x0A, 0x00, 0x00,
    0x67, 0x65, 0x74, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x4C, 0x69, 0x73, 0x74, 0x3A, 0x20, 0x4E,
    0x6F, 0x74, 0x20, 0x65, 0x6E, 0x6F, 0x75, 0x67, 0x68, 0x20, 0x6D, 0x65, 0x6D, 0x6F, 0x72, 0x79,
    0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static IPCResult __LongBlkMsgInt(s32 fd, u32 endpoint, u32 length, void* buffer,
                                 USBCallback callback, void* callbackArg,
                                 u8 async) {
    IPCResult result;
    IPCIOVector* vectors;
    u8* endpointWork;
    u32* lengthWork;
    USBCommandBlock* block;

    vectors = (IPCIOVector*)IOSAlloc(0x60);
    endpointWork = (u8*)IOSAlloc(32);
    lengthWork = (u32*)IOSAlloc(32);

    if (vectors == NULL || endpointWork == NULL || lengthWork == NULL) {
        USB_ERR("__LongBlkMsgInt: Not enough memory\n");
        result = IPC_RESULT_ALLOC_FAILED;
        goto end;
    }

    *endpointWork = (u8)endpoint;
    *lengthWork = (u32)length;

    // Input vector 1: Transfer endpoint
    vectors[0].base = endpointWork;
    vectors[0].length = sizeof(u8);

    // Input vector 2: Transfer length
    vectors[1].base = lengthWork;
    vectors[1].length = sizeof(u32);

    // Output vector 1: Transfer buffer
    vectors[2].base = buffer;
    vectors[2].length = length;

    DCFlushRange(endpointWork, 32);
    DCFlushRange(lengthWork, 32);
    DCFlushRange(vectors, 0x60);

    if (!async) {
        result = IOS_Ioctlv(fd, 10, 2, 1, vectors);
        USB_LOG("Long bulk ioctl returned: %d\n", result);
        goto end;
    }

    block = IOSAlloc(sizeof(USBCommandBlock));
    if (block == NULL) {
        USB_ERR("LongBlkMsgInt (async): Not enough memory\n");
        result = IPC_RESULT_ALLOC_FAILED;
        goto end;
    }
    memset(block, 0, 0x80);

    block->callback = callback;
    block->callbackArg = callbackArg;
    USB_LOG("longblkmsg: cb = 0x%x cbArg = 0x%x\n", block->callback,
            block->callbackArg);

    // Mark memory for deletion
    block->nclean = USB_NCLEAN_BULKMSG;
    block->clean[0] = endpointWork;
    block->clean[1] = lengthWork;
    block->clean[2] = vectors;

    block->msg.buffer = buffer;
    block->msg.length = length;

    result =
        IOS_IoctlvAsync(fd, 10, 2, 1, vectors, _intrBlkCtrlIsoCb, block);
    if (result >= IPC_RESULT_OK) {
        goto end_async;
    }
    if (result == IPC_RESULT_ALLOC_FAILED) {
        OSReport("%s: IoctlvAsync returned error %d\n", __FUNCTION__, result);
    }


    IOSFree(block);

// Non-async (or unsuccessful async) means we must manually free memory
end:
    IOSFree(endpointWork);
    IOSFree(lengthWork);
    IOSFree(vectors);

// Async callback automatically freed the memory marked in block->clean
end_async:
    return result;
}




static IPCResult __IntrBlkMsgInt(s32 fd, u32 endpoint, u32 length, void* buffer,
                                 u8 ioctl, USBCallback callback,
                                 void* callbackArg, u8 async) {
    IPCResult result;
    IPCIOVector* vectors;
    u8* endpointWork;
    u16* lengthWork;
    USBCommandBlock* block;

    vectors = (IPCIOVector*)IOSAlloc(0x60);
    endpointWork = (u8*)IOSAlloc(32);
    lengthWork = (u16*)IOSAlloc(32);

    if (vectors == NULL || endpointWork == NULL || lengthWork == NULL) {
        USB_ERR("__IntrBlkMsgInt: Not enough memory\n");
        result = IPC_RESULT_ALLOC_FAILED;
        goto end;
    }

    *endpointWork = (u8)endpoint;
    *lengthWork = (u16)length;

    // Input vector 1: Transfer endpoint
    vectors[0].base = endpointWork;
    vectors[0].length = sizeof(u8);

    // Input vector 2: Transfer length
    vectors[1].base = lengthWork;
    vectors[1].length = sizeof(u16);

    // Output vector 1: Transfer buffer
    vectors[2].base = buffer;
    vectors[2].length = length;

    DCFlushRange(endpointWork, 32);
    DCFlushRange(lengthWork, 32);
    DCFlushRange(vectors, 0x60);

    if (!async) {
        result = IOS_Ioctlv(fd, ioctl, 2, 1, vectors);
        USB_LOG("intr/blk ioctl returned: %d\n", result);
        goto end;
    }

    block = IOSAlloc(sizeof(USBCommandBlock));
    if (block == NULL) {
        USB_ERR("IntBlkMsgInt (async): Not enough memory\n");
        result = IPC_RESULT_ALLOC_FAILED;
        goto end;
    }
    memset(block, 0, 0x80);

    block->callback = callback;
    block->callbackArg = callbackArg;
    USB_LOG("intrblkmsg: cb = 0x%x cbArg = 0x%x\n", block->callback,
            block->callbackArg);

    // Mark memory for deletion
    block->nclean = USB_NCLEAN_BULKMSG;
    block->clean[0] = endpointWork;
    block->clean[1] = lengthWork;
    block->clean[2] = vectors;

    block->msg.buffer = buffer;
    block->msg.length = length;

    result = IOS_IoctlvAsync(fd, ioctl, 2, 1, vectors, _intrBlkCtrlIsoCb, block);
    if (result >= IPC_RESULT_OK) {
        goto end_async;
    }
    if (result == IPC_RESULT_ALLOC_FAILED) {
        OSReport("%s: IoctlvAsync returned error %d\n", __FUNCTION__,
                 result);
    }

    IOSFree(block);

// Non-async (or unsuccessful async) means we must manually free memory
end:
    IOSFree(endpointWork);
    IOSFree(lengthWork);
    IOSFree(vectors);

// Async callback automatically freed the memory marked in block->clean
end_async:
    return result;
}




// unused in Xenoblade retail: IUSB_ReadIntrMsg

IPCResult IUSB_ReadIntrMsgAsync(s32 fd, u32 endpoint, u32 length, void* buffer,
                                USBCallback callback, void* callbackArg) {
    DCInvalidateRange(buffer, length);
    return __IntrBlkMsgInt(fd, endpoint, length, buffer, USB_IOCTLV_INTRMSG,
                           callback, callbackArg, TRUE);
}




// unused in Xenoblade retail: IUSB_WriteIntrMsg, IUSB_WriteIntrMsgAsync, IUSB_ReadBlkMsg

IPCResult IUSB_ReadBlkMsgAsync(s32 fd, u32 endpoint, u32 length, void* buffer,
                               USBCallback callback, void* callbackArg) {
    DCInvalidateRange(buffer, length);
    if (length <= 0xffff)
        return __IntrBlkMsgInt(fd, endpoint, length, (void*)buffer,
                               USB_IOCTLV_BLKMSG, callback, callbackArg, TRUE);
    return __LongBlkMsgInt(fd, endpoint, length, (void*)buffer, callback,
                           callbackArg, TRUE);
}


// unused in Xenoblade retail: IUSB_WriteBlkMsg

IPCResult IUSB_WriteBlkMsgAsync(s32 fd, u32 endpoint, u32 length,
                                const void* buffer, USBCallback callback,
                                void* callbackArg) {
    DCFlushRange(buffer, length);
    if(length <= 0xffff)
        return __IntrBlkMsgInt(fd, endpoint, length, (void*)buffer,
                               USB_IOCTLV_BLKMSG, callback, callbackArg, TRUE);
    return __LongBlkMsgInt(fd, endpoint, length, (void*)buffer, callback,
                           callbackArg, TRUE);
}




// Retail keeps these strings although the function using them is absent.
extern __declspec(section ".data") const unsigned char sdata_keepalive_usb_C[110] = {
    0x63, 0x61, 0x6C, 0x6C, 0x69, 0x6E, 0x67, 0x20, 0x73, 0x68, 0x6F, 0x72, 0x74, 0x20, 0x62, 0x6C,
    0x6B, 0x20, 0x74, 0x72, 0x61, 0x6E, 0x73, 0x66, 0x65, 0x72, 0x20, 0x66, 0x6E, 0x3A, 0x20, 0x62,
    0x75, 0x66, 0x6C, 0x65, 0x6E, 0x20, 0x3D, 0x20, 0x25, 0x75, 0x20, 0x6C, 0x69, 0x6D, 0x69, 0x74,
    0x20, 0x3D, 0x20, 0x25, 0x75, 0x0A, 0x00, 0x00, 0x63, 0x61, 0x6C, 0x6C, 0x69, 0x6E, 0x67, 0x20,
    0x6C, 0x6F, 0x6E, 0x67, 0x20, 0x62, 0x6C, 0x6B, 0x20, 0x74, 0x72, 0x61, 0x6E, 0x73, 0x66, 0x65,
    0x72, 0x20, 0x66, 0x6E, 0x3A, 0x20, 0x62, 0x75, 0x66, 0x6C, 0x65, 0x6E, 0x20, 0x3D, 0x20, 0x25,
    0x75, 0x20, 0x6C, 0x69, 0x6D, 0x69, 0x74, 0x20, 0x3D, 0x20, 0x25, 0x75, 0x0A, 0x00,
};

static IPCResult __CtrlMsgInt(s32 fd, u8 requestType, u8 request, u16 value,
                              u16 index, u16 length, void* buffer,
                              USBCallback callback, void* callbackArg,
                              u8 async) {
    IPCResult result;
    IPCIOVector* vectors;
    u8* requestTypeWork;
    u8* requestWork;
    u8* unkWork;
    u16* valueWork;
    u16* indexWork;
    u16* lengthWork;
    USBCommandBlock* block;

    if ((buffer == NULL && length != 0) || (u32)buffer % 32 != 0) {
        result = IPC_RESULT_INVALID_INTERNAL;
        USB_ERR("ctrlmsg: bad data buffer\n");
        goto end_async;
    }

    vectors = (IPCIOVector*)IOSAlloc(0xE0);
    requestTypeWork = (u8*)IOSAlloc(32);
    requestWork = (u8*)IOSAlloc(32);
    unkWork = (u8*)IOSAlloc(32);
    valueWork = (u16*)IOSAlloc(32);
    indexWork = (u16*)IOSAlloc(32);
    lengthWork = (u16*)IOSAlloc(32);

    if (requestTypeWork == NULL || requestWork == NULL || unkWork == NULL ||
        valueWork == NULL || indexWork == NULL || lengthWork == NULL ||
        vectors == NULL) {
        USB_ERR("Ctrl Msg: Not enough memory\n");
        result = IPC_RESULT_ALLOC_FAILED;
        goto end;
    }

    *requestTypeWork = requestType;
    *requestWork = request;
    *valueWork = (value & 0xFF) << 8 | value >> 8 & 0xFF;
    *indexWork = (index & 0xFF) << 8 | index >> 8 & 0xFF;
    *lengthWork = (length & 0xFF) << 8 | length >> 8 & 0xFF;
    *unkWork = 0;

    // Input vector 1: bmRequestType
    vectors[0].base = requestTypeWork;
    vectors[0].length = sizeof(u8);

    // Input vector 2: bmRequest
    vectors[1].base = requestWork;
    vectors[1].length = sizeof(u8);

    // Input vector 3: wValue
    vectors[2].base = valueWork;
    vectors[2].length = sizeof(u16);

    // Input vector 4: wIndex
    vectors[3].base = indexWork;
    vectors[3].length = sizeof(u16);

    // Input vector 5: wLength
    vectors[4].base = lengthWork;
    vectors[4].length = sizeof(u16);

    // Input vector 6: Unknown data
    vectors[5].base = unkWork;
    vectors[5].length = sizeof(u8);

    // Output vector 1: Transfer buffer
    vectors[6].base = buffer;
    vectors[6].length = length;

    DCFlushRange(requestTypeWork, 32);
    DCFlushRange(requestWork, 32);
    DCFlushRange(unkWork, 32);
    DCFlushRange(valueWork, 32);
    DCFlushRange(indexWork, 32);
    DCFlushRange(lengthWork, 32);
    DCFlushRange(vectors, 0xE0);

    if (!async) {
        result = IOS_Ioctlv(fd, USB_IOCTLV_CTRLMSG, 6, 1, vectors);
        goto end;
    }

    block = IOSAlloc(sizeof(USBCommandBlock));
    if (block == NULL) {
        USB_ERR("CtrlMsgInt (async): Not enough memory\n");
        result = IPC_RESULT_ALLOC_FAILED;
        goto end;
    }
    memset(block, 0, sizeof(USBCommandBlock));

    block->callback = callback;
    block->callbackArg = callbackArg;
    USB_LOG("ctrlmsgint: cb = 0x%x cbArg = 0x%x\n", block->callback,
            block->callbackArg);

    // Mark memory for deletion
    block->nclean = USB_NCLEAN_CTRLMSG;
    block->clean[0] = requestTypeWork;
    block->clean[1] = requestWork;
    block->clean[2] = valueWork;
    block->clean[3] = indexWork;
    block->clean[4] = lengthWork;
    block->clean[5] = unkWork;
    block->clean[6] = vectors;

    block->msg.buffer = buffer;
    block->msg.length = length;

    result = IOS_IoctlvAsync(fd, USB_IOCTLV_CTRLMSG, 6, 1, vectors,
                             _intrBlkCtrlIsoCb, block);
    USB_LOG("Ctrl Msg async returned: %d\n", result);

    if (result >= IPC_RESULT_OK) {
        goto end_async;
    }

    IOSFree(block);

// Non-async (or unsuccessful async) means we must manually free memory
end:
    IOSFree(requestTypeWork);
    IOSFree(requestWork);
    IOSFree(valueWork);
    IOSFree(indexWork);
    IOSFree(lengthWork);
    IOSFree(unkWork);
    IOSFree(vectors);

// Async callback automatically freed the memory marked in block->clean
end_async:
    return result;
}

// unused in Xenoblade retail: IUSB_ReadCtrlMsg, IUSB_ReadCtrlMsgAsync, IUSB_WriteCtrlMsg

IPCResult IUSB_WriteCtrlMsgAsync(s32 fd, u8 requestType, u8 request, u16 value,
                                 u16 index, u16 length, void* buffer,
                                 USBCallback callback, void* callbackArg) {
    DCFlushRange(buffer, length);
    return __CtrlMsgInt(fd, requestType, request, value, index, length, buffer,
                        callback, callbackArg, TRUE);
}

// unused in Xenoblade retail: unicode2ascii, _GetStrCb, IUSB_GetAsciiStr, IUSB_GetAsciiStrAsync,
// _GetDescrCb, IUSB_GetDevDescr, IUSB_GetDevDescrAsync, IUSB_SuspendDevice, IUSB_ResumeDevice,
// IUSB_ResetDevice, IUSB_GetRhDesca, __getsetRhPortStat, IUSB_GetRhPortStatus, IUSB_SetRhPortStatus,
// IUSB_DeviceRemovalNotifyAsync, __checkIsoArgs, IUSB_IsoMsgAsync, IUSB_DeviceInsertionNotifyAsync,
// IUSB_DeviceInsertionNotifyAsyncWithTimeout, IUSB_DeviceClassInsertionNotifyAsync, IUSB_SetIsoAlt,
// IUSB_RegisterInsertionNotifyWithIdAsync, IUSB_CancelInsertionNotify

// Retail keeps these strings although the functions using them are absent
// (unicode2ascii/_GetStrCb/GetAsciiStr/_GetDescrCb/GetDevDescr/Iso/Notify
// family, IUSB_RegisterInsertionNotifyWithIdAsync). Explicit .data bytes
// (OSError/dsp_task model), positioned so the section comes out
// byte-identical to retail.
/* last .data slot is exactly 0x17 (retail 0x82F); no pad. */
// Retail keeps these strings although the function using them is absent.
extern __declspec(section ".data") const unsigned char sdata_keepalive_usb_D[332] = {
    0x47, 0x65, 0x74, 0x53, 0x74, 0x72, 0x43, 0x62, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x65,
    0x64, 0x3A, 0x20, 0x25, 0x64, 0x0A, 0x00, 0x00, 0x47, 0x65, 0x74, 0x53, 0x74, 0x72, 0x43, 0x62,
    0x3A, 0x20, 0x62, 0x75, 0x66, 0x20, 0x3D, 0x20, 0x30, 0x78, 0x25, 0x78, 0x20, 0x62, 0x75, 0x66,
    0x6C, 0x65, 0x6E, 0x20, 0x3D, 0x20, 0x25, 0x75, 0x0A, 0x00, 0x00, 0x00, 0x46, 0x61, 0x69, 0x6C,
    0x65, 0x64, 0x20, 0x74, 0x6F, 0x20, 0x63, 0x6F, 0x6E, 0x76, 0x65, 0x72, 0x74, 0x20, 0x62, 0x75,
    0x66, 0x66, 0x65, 0x72, 0x20, 0x66, 0x72, 0x6F, 0x6D, 0x20, 0x75, 0x6E, 0x69, 0x63, 0x6F, 0x64,
    0x65, 0x20, 0x32, 0x20, 0x61, 0x73, 0x63, 0x69, 0x69, 0x0A, 0x00, 0x00, 0x63, 0x61, 0x6C, 0x6C,
    0x69, 0x6E, 0x67, 0x20, 0x63, 0x62, 0x20, 0x30, 0x78, 0x25, 0x78, 0x20, 0x77, 0x69, 0x74, 0x68,
    0x20, 0x61, 0x72, 0x67, 0x20, 0x30, 0x78, 0x25, 0x78, 0x0A, 0x00, 0x00, 0x46, 0x61, 0x69, 0x6C,
    0x65, 0x64, 0x20, 0x5F, 0x5F, 0x43, 0x74, 0x72, 0x6C, 0x4D, 0x73, 0x67, 0x3A, 0x20, 0x25, 0x64,
    0x00, 0x00, 0x00, 0x00, 0x46, 0x61, 0x69, 0x6C, 0x65, 0x64, 0x20, 0x74, 0x6F, 0x20, 0x63, 0x6F,
    0x6E, 0x76, 0x65, 0x72, 0x74, 0x20, 0x75, 0x6E, 0x69, 0x63, 0x6F, 0x64, 0x65, 0x20, 0x32, 0x20,
    0x61, 0x73, 0x63, 0x69, 0x69, 0x0A, 0x00, 0x00, 0x47, 0x65, 0x74, 0x53, 0x74, 0x72, 0x20, 0x2D,
    0x20, 0x5F, 0x47, 0x65, 0x74, 0x53, 0x74, 0x72, 0x43, 0x62, 0x0A, 0x00, 0x20, 0x47, 0x65, 0x74,
    0x41, 0x73, 0x63, 0x69, 0x69, 0x53, 0x74, 0x72, 0x41, 0x73, 0x79, 0x6E, 0x63, 0x3A, 0x20, 0x4E,
    0x6F, 0x74, 0x20, 0x65, 0x6E, 0x6F, 0x75, 0x67, 0x68, 0x20, 0x6D, 0x65, 0x6D, 0x6F, 0x72, 0x79,
    0x0A, 0x00, 0x00, 0x00, 0x5F, 0x5F, 0x43, 0x74, 0x72, 0x6C, 0x4D, 0x73, 0x67, 0x49, 0x6E, 0x74,
    0x20, 0x66, 0x61, 0x69, 0x6C, 0x65, 0x64, 0x20, 0x25, 0x64, 0x0A, 0x00, 0x47, 0x65, 0x74, 0x44,
    0x65, 0x73, 0x63, 0x72, 0x43, 0x62, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x65, 0x64, 0x3A,
    0x20, 0x25, 0x64, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x47, 0x65, 0x74, 0x44, 0x65, 0x76, 0x44, 0x65,
    0x73, 0x63, 0x72, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// Retail keeps these strings although the function using them is absent.
extern __declspec(section ".data") const unsigned char sdata_keepalive_usb_E[344] = {
    0x47, 0x65, 0x74, 0x44, 0x65, 0x76, 0x44, 0x65, 0x73, 0x63, 0x72, 0x3A, 0x20, 0x4E, 0x6F, 0x74,
    0x20, 0x65, 0x6E, 0x6F, 0x75, 0x67, 0x68, 0x20, 0x6D, 0x65, 0x6D, 0x6F, 0x72, 0x79, 0x0A, 0x00,
    0x47, 0x65, 0x74, 0x44, 0x65, 0x76, 0x44, 0x65, 0x73, 0x63, 0x72, 0x3A, 0x20, 0x25, 0x64, 0x0A,
    0x00, 0x00, 0x00, 0x00, 0x47, 0x65, 0x74, 0x44, 0x65, 0x76, 0x44, 0x65, 0x73, 0x63, 0x72, 0x20,
    0x2D, 0x20, 0x5F, 0x47, 0x65, 0x74, 0x44, 0x65, 0x73, 0x63, 0x72, 0x43, 0x62, 0x0A, 0x00, 0x00,
    0x47, 0x65, 0x74, 0x44, 0x65, 0x76, 0x44, 0x65, 0x73, 0x63, 0x72, 0x41, 0x73, 0x79, 0x6E, 0x63,
    0x3A, 0x20, 0x4E, 0x6F, 0x74, 0x20, 0x65, 0x6E, 0x6F, 0x75, 0x67, 0x68, 0x20, 0x6D, 0x65, 0x6D,
    0x6F, 0x72, 0x79, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x52, 0x65,
    0x6D, 0x6F, 0x76, 0x61, 0x6C, 0x4E, 0x6F, 0x74, 0x69, 0x66, 0x79, 0x41, 0x73, 0x79, 0x6E, 0x63,
    0x0A, 0x00, 0x00, 0x00, 0x70, 0x61, 0x63, 0x6B, 0x65, 0x74, 0x20, 0x25, 0x75, 0x20, 0x74, 0x6F,
    0x6F, 0x20, 0x62, 0x69, 0x67, 0x3A, 0x20, 0x25, 0x75, 0x0A, 0x00, 0x00, 0x49, 0x6E, 0x76, 0x61,
    0x6C, 0x69, 0x64, 0x20, 0x70, 0x61, 0x72, 0x61, 0x6D, 0x65, 0x74, 0x65, 0x72, 0x73, 0x20, 0x66,
    0x6F, 0x72, 0x20, 0x49, 0x53, 0x4F, 0x20, 0x74, 0x72, 0x61, 0x6E, 0x73, 0x66, 0x65, 0x72, 0x20,
    0x72, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x49, 0x55, 0x53, 0x42,
    0x5F, 0x49, 0x73, 0x6F, 0x4D, 0x73, 0x67, 0x41, 0x73, 0x79, 0x6E, 0x63, 0x3A, 0x20, 0x4E, 0x6F,
    0x74, 0x20, 0x65, 0x6E, 0x6F, 0x75, 0x67, 0x68, 0x20, 0x6D, 0x65, 0x6D, 0x6F, 0x72, 0x79, 0x0A,
    0x00, 0x00, 0x00, 0x00, 0x4F, 0x70, 0x65, 0x6E, 0x28, 0x25, 0x73, 0x29, 0x20, 0x66, 0x61, 0x69,
    0x6C, 0x65, 0x64, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x49, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20,
    0x70, 0x61, 0x74, 0x68, 0x20, 0x6F, 0x72, 0x20, 0x64, 0x65, 0x76, 0x43, 0x6C, 0x61, 0x73, 0x73,
    0x20, 0x69, 0x6E, 0x20, 0x69, 0x6E, 0x73, 0x65, 0x72, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x6E, 0x6F,
    0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x63, 0x61, 0x6C, 0x6C, 0x0A,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// Retail keeps these strings although the function using them is absent.
extern __declspec(section ".data") const unsigned char sdata_keepalive_usb_F[64] = {
    0x49, 0x55, 0x53, 0x42, 0x5F, 0x52, 0x65, 0x67, 0x69, 0x73, 0x74, 0x65, 0x72, 0x49, 0x6E, 0x73,
    0x65, 0x72, 0x74, 0x69, 0x6F, 0x6E, 0x4E, 0x6F, 0x74, 0x69, 0x66, 0x79, 0x57, 0x69, 0x74, 0x68,
    0x49, 0x64, 0x41, 0x73, 0x79, 0x6E, 0x63, 0x00, 0x25, 0x73, 0x3A, 0x20, 0x4E, 0x6F, 0x74, 0x20,
    0x65, 0x6E, 0x6F, 0x75, 0x67, 0x68, 0x20, 0x6D, 0x65, 0x6D, 0x6F, 0x72, 0x79, 0x0A, 0x00, 0x00,
};

char lbl_usb_last[0x17] = "Failed to open %s: %d\n";
