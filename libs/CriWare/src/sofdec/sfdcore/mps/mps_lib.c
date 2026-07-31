#include <harness_catalog.h>

extern int UTY_MemsetDword(u32 *, u32, int);
extern void MPSDEC_Init(void);
extern void MPSGET_Init(void);
extern void MPSDEC_Finish(void);
extern void MPSGET_Finish(void);
extern int MPSDEC_DecHdMpeg1(void);

void *mpslib_InitHn(u32 *entry);

extern u32 lbl_eu_8051C488[];
extern u32 lbl_eu_80606DD8;
extern u32 lbl_eu_80606DDC[];
extern u32 lbl_eu_80606DE0[];

/* MPS global state:
 *   lbl_eu_80606DD8: default error handler (set by MPS_Init)
 *   lbl_eu_80606DDC[0]: work buffer pointer
 *   lbl_eu_80606DE0[0]: last validated handle
 * Work buffer layout:
 *   +0x00: error callback
 *   +0x04: error callback arg
 *   +0x08: global error code
 *   +0x0C: max streams
 *   +0x10: stream entries[0..max-1] (each 0x130 bytes)
 */

#define MPS_ENTRY_SIZE 0x130

int MPS_Init(int max_streams, void *work_buf) {
    u32 magic = 0x01020304;
    u32 def_err = (u32)&lbl_eu_8051C488;
    int i;

    lbl_eu_80606DD8 = def_err;

    /* endianness check: high byte of 0x01020304 must be 1 */
    if (((u8 *)&magic)[0] != 1) {
        for (;;) ((void (*)(void))-1)();
    }

    lbl_eu_80606DDC[0] = (u32)work_buf;

    /* Clear the work buffer: (max_streams - 1) * 0x130 + 0x140 bytes */
    UTY_MemsetDword((u32 *)work_buf, 0,
                    (u32)((max_streams - 1) * MPS_ENTRY_SIZE + 0x140) / 4);

    /* Initialize global state */
    ((u32 *)lbl_eu_80606DDC[0])[0] = 0;
    ((u32 *)lbl_eu_80606DDC[0])[1] = 0;
    ((u32 *)lbl_eu_80606DDC[0])[2] = 0;
    ((u32 *)lbl_eu_80606DDC[0])[3] = max_streams;

    /* Mark all entries as free (state = 1) */
    {
        u32 *p = (u32 *)lbl_eu_80606DDC[0] + 4;
        for (i = 0; i < max_streams; i++) {
            *p = 1;
            p += MPS_ENTRY_SIZE / 4;
        }
    }

    MPSDEC_Init();
    MPSGET_Init();
    return 0;
}

void MPS_Finish(void) {
    int i = 0;
    u8 *entry = (u8 *)lbl_eu_80606DDC[0] + 0x10;
    int count = ((u32 *)lbl_eu_80606DDC[0])[3];

    for (; i < count; entry += MPS_ENTRY_SIZE, i++) {
        u32 *cur = (u32 *)entry;
        u32 err;

        /* Skip free entries (state == 1) */
        if (((s32 *)cur)[0] == 1) continue;

        /* Validate entry */
        if (cur == NULL) {
            err = (u32)-1;
        } else if (((s32 *)cur)[0] == 1) {
            err = (u32)-1;
        } else {
            lbl_eu_80606DE0[0] = (u32)cur;
            err = 0;
        }

        if (err != 0) {
            u32 *gbase = (u32 *)lbl_eu_80606DDC[0];
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
    if (handle == NULL) {
        u32 *base = (u32 *)lbl_eu_80606DDC[0];
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
    if (handle == NULL) {
        u32 *base = (u32 *)lbl_eu_80606DDC[0];
        base[0] = (u32)cb;
        base[1] = (u32)arg;
    } else {
        int err = 0;

        if (handle == NULL) {
            err = -1;
        } else if (((s32 *)handle)[0] == 1) {
            err = -1;
        } else {
            lbl_eu_80606DE0[0] = (u32)handle;
            err = 0;
        }

        if (err) {
            u32 *base = (u32 *)lbl_eu_80606DDC[0];
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
    u32 *base = (u32 *)lbl_eu_80606DDC[0];
    int count = base[3];
    u32 *entry = base + 4;
    int i;

    for (i = 0; i < count; i++, entry += MPS_ENTRY_SIZE / 4) {
        if (((s32 *)entry)[0] == 1)
            return mpslib_InitHn(entry);
    }
    return NULL;
}

void *mpslib_InitHn(u32 *entry) {
    int i;

    UTY_MemsetDword(entry, 0, 0x4C);

    entry[0] = 2;     /* +0x00: state = active */
    entry[1] = 0;     /* +0x04: callback = NULL */
    entry[2] = 0;     /* +0x08: callback arg = NULL */
    entry[3] = 0;     /* +0x0C: error code = 0 */
    entry[4] = 2;     /* +0x10: type = 2 */

    /* Initialize fields +0x18..+0xCC to -1 */
    for (i = 6; i <= 51; i++) {
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
    u32 err;

    if (handle == NULL) {
        err = (u32)-1;
    } else if (((s32 *)handle)[0] == 1) {
        err = (u32)-1;
    } else {
        lbl_eu_80606DE0[0] = (u32)handle;
        err = 0;
    }

    if (err) {
        u32 *base = (u32 *)lbl_eu_80606DDC[0];
        base[2] = 0xFF020103;
        if (base[0]) {
            ((void (*)(u32, u32))base[0])(base[1], 0xFF020103);
        }
        return 0xFF020103;
    }

    ((u32 *)handle)[0] = 1;
    return 0;
}
