#include <harness_catalog.h>
#include <string.h>

extern void ADXSTM_StopNw(void *);
extern void ADXSTM_ReleaseFileNw(void *);
extern int ADXSTM_BindFileNw(void *, const char *, int, int, int);
extern void ADXSTM_SetEos(void *, int);
extern void ADXSTM_SetBufSize(void *, int, int);
extern void ADXSTM_Seek(void *, int);
extern void ADXSTM_Start(void *);
extern int ADXSTM_GetStat(void *);
extern int ADXSTM_Tell(void *);
extern void LSC_CallErrFunc_(const char *, ...);
extern void LSC_CallStatFunc(void *);
extern int LSC_EntryFileRange(void *, const char *, int, int, int);

extern char lbl_eu_80518420[];

/* LSC stream server entry: each entry is 0x20 (32) bytes
 *   +0x00: handle ptr? 
 *   +0x3C: filename string ptr (relative to lsc handle base)
 *   +0x40: filename checksum
 *   +0x44: offset low
 *   +0x48: offset high
 *   +0x4C: size (shifted by 11 for 64-bit)
 *   +0x50: state (0=idle, 1=binding, 2=done)
 *   +0x54: current position
 */

#define ENTRY_SIZE 0x20

/* Get entry pointer from handle and index */
u8 *get_entry(u8 *h, int idx) {
    return h + (idx << 5);
}

/* Wait for a stream operation to complete, then bind next file */
void lsc_StatWait(u8 *h);
void lsc_StatEnd(u8 *h);

void lsc_StatWait(u8 *h) {
    int retry = *(s32 *)(h + 0x24);
    int idx = *(s32 *)(h + 0x20);

    if (retry <= 0) return;

    ADXSTM_StopNw(*(void **)(h + 0x28));
    ADXSTM_ReleaseFileNw(*(void **)(h + 0x28));

    {
        u8 *entry = get_entry(h, idx);
        const char *fname = *(const char **)(entry + 0x3C);
        int flen = (int)strlen(fname);
        int sum = 0;

        if (flen > 0) {
            int i;
            for (i = 0; i < flen; i++) {
                sum += (unsigned char)fname[i];
            }
        }

        if (sum != *(int *)(entry + 0x40)) {
            LSC_CallErrFunc_(lbl_eu_80518420, fname);
            return;
        }

        {
            int off_low = *(int *)(entry + 0x44);
            int off_high = *(int *)(entry + 0x48);
            int size = *(int *)(entry + 0x4C);
            int size_hi = (size >> 31) & 1; /* sign extend */
            int shift_lo = size << 11;
            int shift_hi = (size_hi << 11) | ((unsigned int)size >> 21);
            /* pack into single 64-bit value, pass high and low separately */
            ADXSTM_BindFileNw(*(void **)(h + 0x28), fname, off_low, off_high, size);
            ADXSTM_SetEos(*(void **)(h + 0x28), size);
        }

        *(int *)(h + 0x2C) = *(int *)(entry + 0x4C);
        *(int *)(entry + 0x54) = 0;
        h[0x02] = 0;

        ADXSTM_SetBufSize(*(void **)(h + 0x28), *(int *)(h + 0x14), *(int *)(h + 0x18));
        ADXSTM_Seek(*(void **)(h + 0x28), 0);
        ADXSTM_Start(*(void **)(h + 0x28));

        h[0x02] = 1;
        *(int *)(entry + 0x50) = 1;
    }
}

/* End a stat operation: advance to next entry */
void lsc_StatEnd(u8 *h) {
    int idx, count;
    const char *fname = NULL;
    int off_low = 0, off_high = 0, size = 0;

    if (*(void **)(h + 0x28) == NULL) goto skip_state;

    if (h[0x03] == 1) {
        u8 *entry = get_entry(h, *(int *)(h + 0x20));
        fname = *(const char **)(entry + 0x3C);
        off_low = *(int *)(entry + 0x44);
        off_high = *(int *)(entry + 0x48);
        size = *(int *)(entry + 0x4C);
    }

    /* advance to next entry (circular, 4-bit) */
    idx = *(int *)(h + 0x20);
    count = *(int *)(h + 0x24) - 1;
    idx = (idx + 1) & 0x0FFFFFFF;
    *(int *)(h + 0x24) = count;
    *(int *)(h + 0x20) = idx;

    if (count <= 0) {
        LSC_CallStatFunc(h);
        h[0x01] = 1;
    }

skip_state:
    if (h[0x03] == 1) {
        LSC_EntryFileRange(h, fname, off_low, off_high, size);
    }
}

/* Execute handler for active stream */
void lsc_ExecHndl(u8 *h) {
    if (h[0x04] == 1) return;
    if (h[0x01] != 2) return;
    if (*(int *)(h + 0x24) <= 0) return;

    {
        u8 *entry = get_entry(h, *(int *)(h + 0x20));

        if (*(int *)(entry + 0x50) == 1) {
            void *stream = *(void **)(h + 0x28);
            if (stream == NULL) {
                LSC_CallErrFunc_(lbl_eu_80518420 + 0x40);
            } else {
                int stat = ADXSTM_GetStat(stream);
                switch (stat) {
                case 4: /* finished */
                    h[0x01] = 3;
                    break;
                case 2: /* paused */
                    *(int *)(entry + 0x54) = ADXSTM_Tell(stream);
                    break;
                case 3: /* completed */
                    *(int *)(entry + 0x54) = *(int *)(h + 0x2C);
                    *(int *)(entry + 0x50) = 2;
                    break;
                }
            }
        }

        if (*(int *)(entry + 0x50) == 2) {
            lsc_StatEnd(h);
        }
        if (*(int *)(entry + 0x50) == 0) {
            lsc_StatWait(h);
        }
    }
}
