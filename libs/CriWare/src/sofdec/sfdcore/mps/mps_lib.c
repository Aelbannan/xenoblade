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

/* MPS work struct at lbl_eu_80606DDC:
 *   +0x00: callback (error handler)
 *   +0x04: callback arg
 *   +0x08: error code
 *   +0x0C: max streams
 *   +0x10: stream entries[0..max-1] (each 0x130 bytes)
 */

#define MPS_ENTRY_SIZE 0x130

int MPS_Init(int max_streams, void *work_buf) {
    int i;
    u32 *entry;
    int remaining;
    u32 *p;

    /* version check: high byte of 0x01020304 must be 1 */
    /* This is encoded as: ((u8*)(&some_magic))[0] == 1 */
    {
        u32 magic = 0x01020304;
        if (((u8 *)&magic)[0] != 1) {
            for (;;) (* (void (*)(void)) -1)();
        }
    }

    lbl_eu_80606DDC[3] = (u32)work_buf;
    entry = &lbl_eu_80606DDC[4]; /* +0x10 */

    /* Clear the work buffer */
    UTY_MemsetDword((u32 *)work_buf, 0, ((max_streams - 1) * MPS_ENTRY_SIZE + 0x140 + 3) / 4);

    lbl_eu_80606DDC[0] = 0;
    lbl_eu_80606DDC[1] = 0;
    lbl_eu_80606DDC[2] = 0;
    lbl_eu_80606DDC[3] = max_streams;

    for (i = 0; i < max_streams; i++) {
        entry[i * (MPS_ENTRY_SIZE / 4)] = 1;
    }

    MPSDEC_Init();
    MPSGET_Init();
    return 0;
}

void MPS_Finish(void) {
    int i;
    u32 *base = lbl_eu_80606DDC;
    int count = base[3];
    u32 *entry = &base[4];

    for (i = 0; i < count; i++) {
        u32 *cur = entry + i * (MPS_ENTRY_SIZE / 4);
        if (cur[0] == 1) continue;

        /* error handling */
        {
            u32 err = 0;
            if (cur == NULL) err = (u32)-1;
            else if (cur[0] != 1) {
                lbl_eu_80606DE0[0] = (u32)cur;
                err = 0;
            } else err = (u32)-1;

            if (err != 0) {
                base[2] = 0xFF020103;
                if (base[0]) {
                    ((void (*)(u32, u32))base[0])(base[1], 0xFF020103);
                }
            } else {
                cur[0] = 1;
            }
        }
    }

    MPSDEC_Finish();
    MPSGET_Finish();
}

int MPSLIB_SetErr(void *handle, u32 err_code) {
    u32 *base = lbl_eu_80606DDC;

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
    int err = 0;
    u32 *base = lbl_eu_80606DDC;

    if (handle == NULL) {
        base[0] = (u32)cb;
        base[1] = (u32)arg;
    } else {
        if ((handle == NULL) || (((u32 *)handle)[0] == 1)) {
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
    u32 *base = lbl_eu_80606DDC;
    int count = base[3];
    u32 *entry = &base[4];
    int i;

    for (i = 0; i < count; i++) {
        if (entry[i * (MPS_ENTRY_SIZE / 4)] == 1)
            break;
    }
    if (i >= count) return NULL;

    return mpslib_InitHn(&entry[i * (MPS_ENTRY_SIZE / 4)]);
}

void *mpslib_InitHn(u32 *entry) {
    UTY_MemsetDword(entry, 0, 0x4C / 4);
    entry[0] = 2;
    entry[1] = 0;
    entry[2] = 0;
    entry[3] = 0;
    entry[4] = 2;
    entry[7] = (u32)-1;
    entry[6] = (u32)-1;
    entry[8] = (u32)-1;
    entry[9] = (u32)-1;
    entry[10] = (u32)-1;
    entry[11] = (u32)-1;
    entry[12] = (u32)-1;
    entry[13] = (u32)-1;
    entry[14] = (u32)-1;
    entry[15] = (u32)-1;
    entry[16] = (u32)-1;
    entry[17] = (u32)-1;
    entry[18] = (u32)-1;
    entry[19] = (u32)-1;
    entry[20] = (u32)-1;
    entry[21] = (u32)-1;
    entry[22] = (u32)-1;
    entry[23] = (u32)-1;
    entry[24] = (u32)-1;
    entry[25] = (u32)-1;
    entry[26] = (u32)-1;
    entry[27] = (u32)-1;
    entry[28] = (u32)-1;
    entry[29] = (u32)-1;
    entry[30] = (u32)-1;
    entry[31] = (u32)-1;
    entry[32] = (u32)-1;
    entry[33] = (u32)-1;
    entry[34] = (u32)-1;
    entry[35] = (u32)-1;
    entry[36] = (u32)-1;
    entry[37] = (u32)-1;
    entry[38] = (u32)-1;
    entry[39] = (u32)-1;
    entry[40] = (u32)-1;
    entry[41] = (u32)-1;
    entry[42] = (u32)-1;
    entry[43] = (u32)-1; /* 0xAC */
    entry[42] = (u32)-1; /* 0xA8 */
    entry[45] = (u32)-1; /* 0xB4 */
    entry[44] = (u32)-1; /* 0xB0 */
    entry[46] = (u32)-1; /* 0xB8 */
    entry[47] = (u32)-1; /* 0xBC */
    entry[48] = (u32)-1; /* 0xC0 */
    entry[49] = (u32)-1; /* 0xC4 */
    entry[50] = (u32)-1; /* 0xC8 */
    entry[51] = (u32)-1; /* 0xCC */
    entry[52] = 0;       /* 0xD0 */
    entry[53] = (u32)MPSDEC_DecHdMpeg1; /* 0xD4 */
    entry[54] = 0;       /* 0xD8 */
    entry[55] = 0;       /* 0xDC */
    entry[56] = 0;       /* 0xE0 */
    entry[57] = 0;       /* 0xE4 */
    entry[58] = 0;       /* 0xE8 */
    entry[59] = 0;       /* 0xEC */
    entry[60] = 0;       /* 0xF0 */
    entry[61] = 0;       /* 0xF4 */
    entry[62] = 0;       /* 0xF8 */
    entry[63] = 0;       /* 0xFC */
    return entry;
}

int MPS_Destroy(void *handle) {
    u32 err = 0;
    u32 *base = lbl_eu_80606DDC;

    if (handle == NULL) err = (u32)-1;
    else if (((u32 *)handle)[0] == 1) err = (u32)-1;
    else {
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
