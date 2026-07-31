#include <harness_catalog.h>

extern int UTY_MemsetDword(u32 *, u32, int);
extern void MPSDEC_Init(void);
extern void MPSGET_Init(void);
extern void MPSDEC_Finish(void);
extern void MPSGET_Finish(void);
extern int MPSDEC_DecHdMpeg1(void);

void *mpslib_InitHn(u32 *entry);

extern u32 lbl_eu_80606DDC[];
extern u32 lbl_eu_80606DE0[];

/* MPS global state at lbl_eu_80606DDC:
 *   +0x00: error callback
 *   +0x04: error callback arg
 *   +0x08: global error code
 *   +0x0C: max streams
 *   +0x10: stream entries[0..max-1] (each 0x130 bytes)
 */

#define MPS_ENTRY_SIZE 0x130

int MPS_Init(int max_streams, void *work_buf) {
    int i;
    u32 *entry;
    u32 *p;
    u32 magic;

    /* endianness check: high byte of 0x01020304 must be 1 */
    magic = 0x01020304;
    if (((u8 *)&magic)[0] != 1) {
        for (;;) ((void (*)(void))-1)();
    }

    lbl_eu_80606DDC[3] = (u32)work_buf;
    entry = &lbl_eu_80606DDC[4];

    /* Clear the work buffer: (max_streams - 1) * 0x130 + 0x140 bytes */
    p = (u32 *)work_buf;
    UTY_MemsetDword(p, 0, ((max_streams - 1) * MPS_ENTRY_SIZE + 0x140) / 4);

    /* Initialize global state */
    p = (u32 *)lbl_eu_80606DDC[3];
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;

    lbl_eu_80606DDC[3] = (u32)work_buf;
    ((u32 *)lbl_eu_80606DDC[3])[3] = max_streams;

    entry = (u32 *)((u32)lbl_eu_80606DDC[3] + 0x10);

    /* Mark all entries as free (state = 1) */
    for (i = 0; i < max_streams; i++) {
        entry[i * (MPS_ENTRY_SIZE / 4)] = 1;
    }

    MPSDEC_Init();
    MPSGET_Init();
    return 0;
}

void MPS_Finish(void) {
    int i;
    u32 *base = (u32 *)lbl_eu_80606DDC[3];
    int count = base[3];
    u32 *entry = base + 4;

    for (i = 0; i < count; i++) {
        u32 *cur = &entry[i * (MPS_ENTRY_SIZE / 4)];
        u32 err;

        /* Skip free entries (state == 1) */
        if (cur[0] == 1) continue;

        /* Validate entry */
        if (cur == NULL) {
            err = (u32)-1;
        } else if (cur[0] == 1) {
            err = (u32)-1;
        } else {
            lbl_eu_80606DE0[0] = (u32)cur;
            err = 0;
        }

        if (err != 0) {
            u32 *gbase = (u32 *)lbl_eu_80606DDC[3];
            gbase[2] = 0xFF020103;
            if (gbase[0]) {
                ((void (*)(u32, u32))gbase[0])(gbase[1], 0xFF020103);
            }
        } else {
            cur[0] = 1;
        }
    }

    MPSDEC_Finish();
    MPSGET_Finish();
}

int MPSLIB_SetErr(void *handle, u32 err_code) {
    u32 *base = (u32 *)lbl_eu_80606DDC[3];

    if (handle == NULL) {
        base[2] = err_code;
        if (err_code && base[0]) {
            ((void (*)(u32, u32))base[0])(base[1], err_code);
        }
    } else {
        ((u32 *)handle)[3] = err_code;
        if (err_code) {
            u32 *hb = (u32 *)handle;
            if (hb[1]) {
                ((void (*)(u32, u32))hb[1])(hb[2], err_code);
            }
        }
    }
    return err_code;
}

int MPS_GetErrInf(void *handle, void *cb, void *arg) {
    u32 *base = (u32 *)lbl_eu_80606DDC[3];

    if (handle == NULL) {
        base[0] = (u32)cb;
        base[1] = (u32)arg;
    } else {
        int err = 0;

        if (handle == NULL) {
            err = -1;
        } else if (((u32 *)handle)[0] == 1) {
            err = -1;
        } else {
            lbl_eu_80606DE0[0] = (u32)handle;
            err = 0;
        }

        if (err) {
            base[2] = 0xFF020101;
            if (base[0]) {
                ((void (*)(u32, u32))base[0])(base[1], 0xFF020101);
            }
            return 0xFF020101;
        }

        ((u32 *)handle)[1] = (u32)cb;
        ((u32 *)handle)[2] = (u32)arg;
    }
    return 0;
}

int MPSLIB_CheckHn(void *handle) {
    if (handle == NULL) return -1;
    if (((s32 *)handle)[0] == 1) return -1;
    lbl_eu_80606DE0[0] = (u32)handle;
    return 0;
}

void *MPS_Create(void) {
    u32 *base = (u32 *)lbl_eu_80606DDC[3];
    int count = base[3];
    u32 *entry = base + 4;
    int i;

    for (i = 0; i < count; i++) {
        if (entry[i * (MPS_ENTRY_SIZE / 4)] == 1)
            break;
    }
    if (i >= count) return NULL;

    return mpslib_InitHn(&entry[i * (MPS_ENTRY_SIZE / 4)]);
}

void *mpslib_InitHn(u32 *entry) {
    int i;

    UTY_MemsetDword(entry, 0, 0x4C / 4);

    entry[0] = 2;     /* +0x00: state = active */
    entry[1] = 0;     /* +0x04: callback = NULL */
    entry[2] = 0;     /* +0x08: callback arg = NULL */
    entry[3] = 0;     /* +0x0C: error code = 0 */
    entry[4] = 2;     /* +0x10: type = 2 */

    /* Initialize fields +0x14..+0xCC to -1 */
    for (i = 5; i <= 51; i++) {
        entry[i] = (u32)-1;
    }

    entry[52] = 0;                       /* +0xD0 */
    entry[53] = (u32)MPSDEC_DecHdMpeg1;  /* +0xD4: dec_hd function */

    /* Clear +0xD8..+0xFC */
    for (i = 54; i <= 63; i++) {
        entry[i] = 0;
    }

    return entry;
}

int MPS_Destroy(void *handle) {
    u32 *base = (u32 *)lbl_eu_80606DDC[3];
    u32 err;

    if (handle == NULL) {
        err = (u32)-1;
    } else if (((u32 *)handle)[0] == 1) {
        err = (u32)-1;
    } else {
        lbl_eu_80606DE0[0] = (u32)handle;
        err = 0;
    }

    if (err) {
        base[2] = 0xFF020103;
        if (base[0]) {
            ((void (*)(u32, u32))base[0])(base[1], 0xFF020103);
        }
        return 0xFF020103;
    }

    ((u32 *)handle)[0] = 1;
    return 0;
}
