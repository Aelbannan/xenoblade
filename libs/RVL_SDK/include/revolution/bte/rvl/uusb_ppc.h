#ifndef RVL_SDK_BTE_RVL_UUSB_PPC_H
#define RVL_SDK_BTE_RVL_UUSB_PPC_H
#include <types.h>

#include <revolution/bte/gki/common/gki.h>
#include <revolution/usb/usb.h>

#ifdef __cplusplus
extern "C" {
#endif

u32 __ntd_get_allocated_mem_size(void);

/* UUSB private control block (retail global `usb`, size 0x4C).
 * Field names are recovered where semantics are understood; unknown
 * members keep `field_0xNN` offsets. */
typedef struct tUUSB_CB {
    s32 fd;                       /* 0x00 device file descriptor */
    u8 field_0x04[0xC];          /* 0x04 unused in matched functions */
    u8 field_0x10;                /* 0x10 endpoint (intr/bulk ctrl) */
    u8 field_0x11;                /* 0x11 endpoint */
    u8 field_0x12;                /* 0x12 endpoint */
    u8 field_0x13;                /* 0x13 endpoint */
    u32 vid;                      /* 0x14 USB vendor id */
    u32 pid;                      /* 0x18 USB product id */
    u8 pool_id_intr;             /* 0x1C intr read pool id */
    u8 pool_id_bulk;             /* 0x1D bulk read pool id */
    u8 field_0x1E[2];            /* 0x1E */
    void (*close_cb)(int reason, s8 result); /* 0x20 upper-layer close notify */
    u32 close_cb_arg;            /* 0x24 close callback argument */
    u8 trace_state;             /* 0x28 trace init state */
    u8 state;                    /* 0x29 UUSB state machine */
    u8 field_0x2A;              /* 0x2A */
    u8 field_0x2B;              /* 0x2B read-restart flag */
    BUFFER_Q bulk_write_q;       /* 0x2C bulk write buffer queue */
    u8 bulk_pending;            /* 0x38 outstanding bulk writes */
    u8 field_0x39[3];           /* 0x39 */
    BUFFER_Q ctrl_write_q;       /* 0x3C ctrl write buffer queue */
    u8 ctrl_pending;            /* 0x48 outstanding ctrl writes */
    u8 field_0x49[3];           /* 0x49 */
} tUUSB_CB;

/* UUSB state values (usb.state). */
enum {
    UUSB_STATE_IDLE = 0,
    UUSB_STATE_OPEN = 2,
    UUSB_STATE_READY = 4,
    UUSB_STATE_CLOSED = 5,
};

/* Close notification reason passed to tUUSB_CB.close_cb. */
#define UUSB_CLOSE_REASON_CLOSED 4

/* Global control block and trace flags (retail linker names). */
extern tUUSB_CB usb;
extern u8 uusb_g_usb_devid_found;
extern u8 uusb_g_trace_state_initialized;
extern u32 wait4hci;

/* Public UUSB API. */
void UUSB_Register(void* cb_arg);
void UUSB_Open(s32 fd, void (*close_cb)(int reason, s8 result));
u16 UUSB_Read(u8 channel, void* p_buf, u16 len);
s32 UUSB_Write(s32 type, void* data, u16 length);
void UUSB_Close(void);
void UUSB_Unregister(void);

/* USB close completion callback (IUSB_CloseDeviceAsync). */
void uusb_CloseDeviceCB(IPCResult result, void* arg);
void uusb_ReadIntrDataCB(IPCResult result, void* arg);
void uusb_ReadBulkDataCB(IPCResult result, void* arg);
void uusb_WriteCtrlDataCB(IPCResult result, void* arg);
void uusb_WriteBulkDataCB(IPCResult result, void* arg);

#ifdef __cplusplus
}
#endif
#endif
