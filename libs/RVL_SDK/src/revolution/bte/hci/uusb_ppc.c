// UUSB (USB-HCI) bridge for the Broadway BTE stack.
// Reconstruction of the retail RVL_SDK uusb_ppc translation unit.

#include <harness_catalog.h>

#include <string.h>

#include <revolution/bte/rvl/uusb_ppc.h>
#include <revolution/os/OSContext.h>

/* BTE stack helpers (defined in other BTE TUs, not declared in headers). */
extern void bta_ci_hci_msg_handler(void* p_data);
extern BT_HDR* l2cap_link_chk_pkt_start(BT_HDR* p_buf);
extern BOOLEAN l2cap_link_chk_pkt_end(void);

/* Global control block and trace flags (retail linker symbols). */
tUUSB_CB usb;
u8 uusb_g_usb_devid_found;
u8 uusb_g_trace_state_initialized;

/* Fiber stacks for the BTE HCI message dispatcher (retail linker symbols). */
u8 __uusb_ppc_stack1[0x1000];
u8 __uusb_ppc_stack2[0x1000];

/* Upper-layer wait-for-HCI flag (retail .sdata symbol). */
u32 wait4hci = 1;

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

/* uusb_ReadBulkDataCB - IUSB_ReadBlkMsgAsync completion callback.
 * If the UUSB state machine left the OPEN state the pool is torn down and
 * the buffer freed. Otherwise the received payload is repacked into a
 * fresh pool-3 buffer, checked through the L2CAP reassembly filter and,
 * when the packet is complete, handed to the BTE HCI message handler on a
 * fiber stack. A new bulk-in read is then always re-armed. */
void uusb_ReadBulkDataCB(IPCResult result, void* arg) {
    BT_HDR* buf;
    BT_HDR* pkt;
    u8* data;

    if (usb.state != UUSB_STATE_OPEN) {
        GKI_freebuf(arg);
        GKI_delete_pool(usb.pool_id_bulk);
        usb.pool_id_bulk = 0xff;
        return;
    }

    if (result <= 0) {
        GKI_freebuf(arg);
    } else {
        ((BT_HDR*)arg)->len = (u16)result;
        buf = (BT_HDR*)GKI_getpoolbuf(3);
        if (buf == NULL) {
            GKI_freebuf(arg);
        } else {
            memcpy(buf, arg,
                   (((u32)(((BT_HDR*)arg)->len + ((BT_HDR*)arg)->offset +
                           BT_HDR_SIZE)) &
                    ~3u) +
                       4);
            pkt = l2cap_link_chk_pkt_start(buf);
            if (pkt != NULL && l2cap_link_chk_pkt_end()) {
                OSSwitchFiberEx((u32)pkt, 0, 0, 0, bta_ci_hci_msg_handler,
                                __uusb_ppc_stack2 + 0x1000);
            }
            GKI_freebuf(arg);
        }
    }

    /* Re-arm the bulk-in read on a fresh pool buffer. */
    do {
        buf = (BT_HDR*)GKI_getpoolbuf(usb.pool_id_bulk);
    } while (buf == NULL);
    buf->event = 0x1100;
    buf->len = 0;
    data = (u8*)(((u32)buf + 0x27) & ~0x1Fu);
    buf->offset = (u16)(data - ((u8*)buf + BT_HDR_SIZE));
    if (IUSB_ReadBlkMsgAsync(usb.fd, usb.field_0x11,
                             (u16)GKI_get_buf_size(buf) - 0x28 - buf->offset,
                             data, uusb_ReadBulkDataCB, buf) != 0) {
        GKI_freebuf(buf);
    }
}

/* uusb_WriteCtrlDataCB - IUSB_WriteCtrlMsgAsync completion callback.
 * On entry `arg` is the BT_HDR buffer that was submitted. When the write
 * completes (arg != NULL) the buffer is returned to the GKI pool and the
 * outstanding-write counter is decremented. Then, while the in-flight window
 * (5) is not saturated and the queue is non-empty, the next queued buffer is
 * dispatched; a failed submit pushes the buffer back to the head. */
void uusb_WriteCtrlDataCB(IPCResult result, void* arg) {
    BT_HDR* buf;
    IPCResult rc;

    (void)result;

    if (arg != NULL) {
        GKI_freebuf(arg);
        GKI_disable();
        usb.ctrl_pending -= 1;
        GKI_enable();
    }

    /* While the in-flight window (5) is not saturated and the queue is
     * non-empty, dispatch the next queued control-write buffer. */
    if (usb.ctrl_pending < 5 && usb.ctrl_write_q.count != 0) {
        buf = (BT_HDR*)GKI_dequeue(&usb.ctrl_write_q);
        rc = IUSB_WriteCtrlMsgAsync(usb.fd, 0x20, 0, 0, 0, buf->len,
                                    (u8*)buf + buf->offset + BT_HDR_SIZE,
                                    uusb_WriteCtrlDataCB, buf);
        if (rc < IPC_RESULT_OK) {
            /* Submission rejected: restore the buffer to the queue head. */
            GKI_enqueue_head(&usb.ctrl_write_q, buf);
        } else {
            GKI_disable();
            usb.ctrl_pending += 1;
            GKI_enable();
        }
    }
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

/* UUSB_Open - bring the USB-HCI link up once UUSB_Register has built the
 * read pools (state READY). Installs the upper-layer close callback, then
 * arms the interrupt-in and bulk-in reads with fresh GKI buffers and clears
 * the wait-for-HCI flag so the stack init can proceed. */
void UUSB_Open(s32 fd, void (*close_cb)(int reason, s8 result)) {
    BT_HDR* buf;
    u8* data;

    (void)fd;

    if (usb.state != UUSB_STATE_READY) goto done;
    if (usb.pool_id_intr == 0xff) goto done;
    if (usb.pool_id_bulk == 0xff) goto done;
    goto body;
done:
    return;
body:
        GKI_disable();
        usb.close_cb = close_cb;
        usb.state = UUSB_STATE_OPEN;
        GKI_enable();

        /* Arm the interrupt-in read. */
        do {
            buf = (BT_HDR*)GKI_getpoolbuf(usb.pool_id_intr);
        } while (buf == NULL);
        buf->event = 0x1000;
        buf->len = 0;
        data = (u8*)(((u32)buf + 0x27) & ~0x1Fu);
        buf->offset = (u16)(data - ((u8*)buf + BT_HDR_SIZE));
        if (IUSB_ReadIntrMsgAsync(usb.fd, usb.field_0x12,
                                  (u16)GKI_get_buf_size(buf) - 0x28 - buf->offset,
                                  data, uusb_ReadIntrDataCB, buf) != 0) {
            GKI_freebuf(buf);
        }
        usb.field_0x2B = 1;

        /* Arm the bulk-in read. */
        do {
            buf = (BT_HDR*)GKI_getpoolbuf(usb.pool_id_bulk);
        } while (buf == NULL);
        buf->event = 0x1100;
        buf->len = 0;
        data = (u8*)(((u32)buf + 0x27) & ~0x1Fu);
        buf->offset = (u16)(data - ((u8*)buf + BT_HDR_SIZE));
        if (IUSB_ReadBlkMsgAsync(usb.fd, usb.field_0x11,
                                 (u16)GKI_get_buf_size(buf) - 0x28 - buf->offset,
                                 data, uusb_ReadBulkDataCB, buf) != 0) {
            GKI_freebuf(buf);
        }
        wait4hci = 0;
    return;
}

u16 UUSB_Read(u8 channel, void* p_buf, u16 len) {
    (void)channel;
    (void)p_buf;
    (void)len;
    return 0;
}

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
