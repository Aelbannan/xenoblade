// UUSB (USB-HCI) bridge for the Broadway BTE stack.
// Reconstruction of the retail RVL_SDK uusb_ppc translation unit.

#include <harness_catalog.h>

#include <string.h>

/* usb.h declares the 4-arg implementation form of IUSB_OpenDeviceIds
 * (resultOut); the retail uusb_ppc TU was compiled against the 3-arg form
 * (result returned in r3, r6 left untouched). Rename the header declaration
 * aside and declare the local form below. */
#define IUSB_OpenDeviceIds IUSB_OpenDeviceIds_usb_h
#include <revolution/bte/rvl/uusb_ppc.h>
#undef IUSB_OpenDeviceIds
#include <revolution/os/OSContext.h>

/* BTE stack helpers (defined in other BTE TUs, not declared in headers). */
extern void bta_ci_hci_msg_handler(void* p_data);
extern BT_HDR* l2cap_link_chk_pkt_start(BT_HDR* p_buf);
extern BOOLEAN l2cap_link_chk_pkt_end(void);

/* Retail uusb_ppc prototype of IUSB_OpenDeviceIds: no resultOut, the file
 * descriptor comes back in r3. */
extern IPCResult IUSB_OpenDeviceIds(const char* interface, u16 vid, u16 pid);

/* Global control block and trace flags (retail linker symbols). */
tUUSB_CB usb;
u8 uusb_g_usb_devid_found;
u8 uusb_g_trace_state_initialized;

/* NTD (Nintendo Test/Dev?) USB bridge globals shared with other BTE TUs. */
s32 __ntd_ios_file_descriptor = -1;
u32 __ntd_ohci;
u32 __ntd_ohci_init_flag;
u8 __ntd_pid_vid_specified;
u32 __ntd_vid;
u32 __ntd_pid;

/* Device interface names for IUSB_OpenDeviceIds (retail .sdata labels). */
static char lbl_806658D0[] = "oh0";
static char lbl_806658D4[] = "oh1";

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

/* uusb_ReadIntrDataCB - IUSB_ReadIntrMsgAsync completion callback.
 * Interrupt-in mirror of uusb_ReadBulkDataCB: clears the read-restart flag,
 * repacks a successful payload into a fresh pool-2 buffer and hands it
 * straight to the BTE HCI message handler on a fiber stack (the intr pipe
 * carries whole HCI events, so no L2CAP reassembly filter is needed), then
 * re-arms the interrupt-in read. */
void uusb_ReadIntrDataCB(IPCResult result, void* arg) {
    u8* data;
    BT_HDR* buf;

    usb.field_0x2B = 0;

    if (usb.state != UUSB_STATE_OPEN) {
        GKI_freebuf(arg);
        GKI_delete_pool(usb.pool_id_intr);
        usb.pool_id_intr = 0xff;
        return;
    }

    if (arg != NULL) {
        if (result <= 0) {
            GKI_freebuf(arg);
        } else {
            buf = (BT_HDR*)GKI_getpoolbuf(2);
            if (buf == NULL) {
                GKI_freebuf(arg);
            } else {
                ((BT_HDR*)arg)->event = 0x1000;
                ((BT_HDR*)arg)->len = (u16)result;
                memcpy(buf, arg,
                       (((u32)(((BT_HDR*)arg)->len + ((BT_HDR*)arg)->offset +
                               BT_HDR_SIZE)) &
                        ~3u) +
                           4);
                OSSwitchFiberEx((u32)buf, 0, 0, 0, bta_ci_hci_msg_handler,
                                __uusb_ppc_stack1 + 0x1000);
                GKI_freebuf(arg);
            }
        }
    }

    /* Re-arm the interrupt-in read on a fresh pool buffer. */
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

/* UUSB_Register - initialise the UUSB bridge: zero the control block, take
 * the state machine to CLOSED, init the IPC client and IUSB library, pick
 * the vid/pid (NTD overrides or the Bluetooth class defaults 0x057E/0x0305),
 * open the device ("oh0" / "oh1" interface), build the write queues and the
 * intr/bulk read pools, then move to READY and clear the wait-for-HCI flag. */
void UUSB_Register(void* cb_arg) {
    IPCResult fd;

    memset(&usb, 0, sizeof(usb));

    GKI_disable();
    usb.state = UUSB_STATE_CLOSED;
    GKI_enable();

    if (uusb_g_trace_state_initialized == 0) {
        usb.trace_state = 0;
        uusb_g_trace_state_initialized = 1;
    }

    if (IPCCltInit() != 0) {
        return;
    }
    if (IUSB_OpenLib() != 0) {
        return;
    }

    usb.close_cb_arg = (u32)cb_arg;

    if (__ntd_pid_vid_specified == 1) {
        usb.vid = __ntd_vid;
        usb.pid = __ntd_pid;
    } else {
        usb.vid = 0x57e;
        usb.pid = 0x305;
    }

    usb.field_0x10 = 0;
    usb.field_0x11 = 0;
    usb.field_0x12 = 0;
    usb.field_0x13 = 0;

    if (__ntd_ohci_init_flag == 1) {
        if (__ntd_ohci == 0) {
            fd = IUSB_OpenDeviceIds(lbl_806658D0, (u16)usb.vid, (u16)usb.pid);
        } else if (__ntd_ohci == 1) {
            fd = IUSB_OpenDeviceIds(lbl_806658D4, (u16)usb.vid, (u16)usb.pid);
        }
    } else {
        fd = IUSB_OpenDeviceIds(lbl_806658D4, (u16)usb.vid, (u16)usb.pid);
    }

    if (fd >= 0) {
        __ntd_ios_file_descriptor = fd;
    }
    if (fd < 0) {
        return;
    }

    usb.field_0x10 = 2;
    usb.field_0x11 = 0x82;
    usb.field_0x12 = 0x81;
    usb.field_0x13 = 0;
    GKI_init_q(&usb.bulk_write_q);
    usb.bulk_pending = 0;
    GKI_init_q(&usb.ctrl_write_q);
    usb.ctrl_pending = 0;
    usb.pool_id_intr = GKI_create_pool(0x294, 0x2d, 1, NULL);
    usb.pool_id_bulk = GKI_create_pool(0x708, 0x1e, 1, NULL);
    if (usb.pool_id_intr == 0xff) {
        return;
    }
    if (usb.pool_id_bulk == 0xff) {
        return;
    }

    GKI_disable();
    usb.state = UUSB_STATE_READY;
    GKI_enable();
    wait4hci = 1;
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

/* UUSB_Write - submit an HCI command (type 0) or ACL (type 2) packet to the
 * USB HCI transport. The payload is copied into a fresh pool buffer (intr
 * pool for commands, bulk pool for ACL) and either submitted directly when
 * the in-flight window (5) is not saturated and the queue is empty, or
 * queued for uusb_WriteCtrlDataCB / uusb_WriteBulkDataCB to dispatch.
 * Returns the submit result (0 on queue). */
s32 UUSB_Write(s32 type, void* data, u16 length) {
    BT_HDR* buf;
    u8* p;
    IPCResult rc = 0;

    if (usb.state != UUSB_STATE_OPEN) {
        return 0;
    }

    if (type == 0) {
        buf = (BT_HDR*)GKI_getpoolbuf(usb.pool_id_intr);
        if (buf == NULL) {
            return 0;
        }
        p = (u8*)(((u32)buf + 0x27) & ~0x1Fu);
        buf->len = length;
        buf->offset = (u16)(p - ((u8*)buf + BT_HDR_SIZE));
        memcpy(p, data, length);
        if (usb.ctrl_pending < 5 && usb.ctrl_write_q.count == 0) {
            rc = IUSB_WriteCtrlMsgAsync(usb.fd, 0x20, 0, 0, 0, length, p,
                                        uusb_WriteCtrlDataCB, buf);
            if (rc == 0) {
                GKI_disable();
                usb.ctrl_pending += 1;
                GKI_enable();
            } else {
                GKI_freebuf(buf);
            }
        } else {
            GKI_enqueue(&usb.ctrl_write_q, buf);
            return 0;
        }
    } else if (type == 2) {
        buf = (BT_HDR*)GKI_getpoolbuf(usb.pool_id_bulk);
        if (buf == NULL) {
            return 0;
        }
        p = (u8*)(((u32)buf + 0x27) & ~0x1Fu);
        buf->len = length;
        buf->offset = (u16)(p - ((u8*)buf + BT_HDR_SIZE));
        memcpy(p, data, length);
        if (usb.bulk_pending < 5 && usb.bulk_write_q.count == 0) {
            rc = IUSB_WriteBlkMsgAsync(usb.fd, usb.field_0x10, length, p,
                                       uusb_WriteBulkDataCB, buf);
            if (rc == 0) {
                GKI_disable();
                usb.bulk_pending += 1;
                GKI_enable();
            } else {
                GKI_freebuf(buf);
            }
        } else {
            GKI_enqueue(&usb.bulk_write_q, buf);
            return 0;
        }
    }

    return (u16)rc;
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
