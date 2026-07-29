// UUSB (USB-HCI) bridge for the Broadway BTE stack.
// Reconstruction of the retail RVL_SDK uusb_ppc translation unit.

#include <harness_catalog.h>

#include <revolution/bte/rvl/uusb_ppc.h>

/* Global control block and trace flags (retail linker symbols). */
tUUSB_CB usb;
u8 uusb_g_usb_devid_found;
u8 uusb_g_trace_state_initialized;

/* uusb_CloseDeviceCB - completion callback for IUSB_CloseDeviceAsync.
 * Tears down the control block, closes the IUSB library, resets the UUSB
 * state machine to CLOSED, then notifies the upper layer (if it registered
 * a close callback) with the IPC result. */
void uusb_CloseDeviceCB(IPCResult result, void* arg) {
    (void)arg;

    /* Forget the device and all endpoint/pool bookkeeping. */
    usb.fd = 0;
    usb.field_0x10 = 0;
    usb.field_0x11 = 0;
    usb.field_0x12 = 0;
    usb.field_0x13 = 0;
    uusb_g_usb_devid_found = 0;

    IUSB_CloseLib();

    /* Reset the UUSB state machine under the GKI lock. */
    GKI_disable();
    usb.state = UUSB_STATE_CLOSED;
    usb.trace_state = 0;
    uusb_g_trace_state_initialized = 0;
    GKI_enable();

    /* Notify the upper layer that the device has closed. */
    if (usb.close_cb != NULL) {
        usb.close_cb(UUSB_CLOSE_REASON_CLOSED, (s8)result);
    }
}

void uusb_ReadIntrDataCB(IPCResult result, void* arg) {
    (void)result;
    (void)arg;
}

void uusb_ReadBulkDataCB(IPCResult result, void* arg) {
    (void)result;
    (void)arg;
}

void uusb_WriteCtrlDataCB(IPCResult result, void* arg) {
    (void)result;
    (void)arg;
}

/* uusb_WriteBulkDataCB - IUSB_WriteBlkMsgAsync completion callback.
 * On entry `arg` is the BT_HDR buffer that was submitted. When the write
 * completes (arg != NULL) the buffer is returned to the GKI pool and the
 * outstanding-write counter is decremented. Then, while the in-flight window
 * (5) is not saturated and the queue is non-empty, the next queued buffer is
 * dispatched; a failed submit pushes the buffer back to the head. */
void uusb_WriteBulkDataCB(IPCResult result, void* arg) {
    BT_HDR* buf;
    IPCResult rc;

    (void)result;

    if (arg != NULL) {
        GKI_freebuf(arg);
        GKI_disable();
        usb.bulk_pending -= 1;
        GKI_enable();
    }

    /* While the in-flight window (5) is not saturated and the queue is
     * non-empty, dispatch the next queued bulk-write buffer. */
    if (usb.bulk_pending < 5 && usb.bulk_write_q.count != 0) {
        buf = (BT_HDR*)GKI_dequeue(&usb.bulk_write_q);
        rc = IUSB_WriteBlkMsgAsync(usb.fd, usb.field_0x10, buf->len,
                                   (u8*)buf + buf->offset + BT_HDR_SIZE,
                                   uusb_WriteBulkDataCB, buf);
        if (rc < IPC_RESULT_OK) {
            /* Submission rejected: restore the buffer to the queue head. */
            GKI_enqueue_head(&usb.bulk_write_q, buf);
        } else {
            GKI_disable();
            usb.bulk_pending += 1;
            GKI_enable();
        }
    }
}

void UUSB_Register(void* cb_arg) {
    (void)cb_arg;
}

void UUSB_Open(s32 fd, void (*close_cb)(int reason, s8 result)) {
    (void)fd;
    (void)close_cb;
}

s32 UUSB_Read(void) { return 0; }

s32 UUSB_Write(s32 type, void* data, u16 length) {
    (void)type;
    (void)data;
    (void)length;
    return 0;
}

/* UUSB_Close - mark the control block idle, drain any queued bulk/ctrl
 * write buffers back to the GKI pool, then kick off an async device close.
 * The actual teardown (state -> CLOSED, close notify) happens in the
 * uusb_CloseDeviceCB completion callback. */
void UUSB_Close(void) {
    GKI_disable();
    usb.state = UUSB_STATE_IDLE;
    GKI_enable();

    /* Free any bulk-write buffers still pending in the queue. */
    if (usb.bulk_write_q.count != 0) {
        while (usb.bulk_write_q.count != 0) {
            GKI_freebuf(GKI_dequeue(&usb.bulk_write_q));
        }
    }

    /* Free any control-write buffers still pending in the queue. */
    if (usb.ctrl_write_q.count != 0) {
        while (usb.ctrl_write_q.count != 0) {
            GKI_freebuf(GKI_dequeue(&usb.ctrl_write_q));
        }
    }

    IUSB_CloseDeviceAsync(usb.fd, uusb_CloseDeviceCB, NULL);
}

/* UUSB_Unregister - release the IUSB library and return the UUSB state
 * machine to CLOSED so a subsequent UUSB_Register can reinitialise it. */
void UUSB_Unregister(void) {
    IUSB_CloseLib();

    GKI_disable();
    usb.state = UUSB_STATE_CLOSED;
    usb.trace_state = 0;
    uusb_g_trace_state_initialized = 0;
    GKI_enable();
}
