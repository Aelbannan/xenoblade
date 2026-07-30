#include <harness_catalog.h>
#include <string.h>

extern void LSC_Enter(void);
extern void LSC_Leave(void);
extern void LSC_LockCrs(void *);
extern void LSC_UnlockCrs(void *);
extern void LSC_CallErrFunc_(const char *, ...);
extern int ADXSTM_StopNw(void *);
extern int ADXSTM_Start(void *);
extern int ADXSTM_GetStat(void *);
extern int ADXSTM_ReleaseFileNw(void *);
extern void LSC_CallStatFunc(void);

extern char lbl_eu_80518478[];
extern u8 lbl_eu_805E7D40[];

#define LSE_MAX   0x20
#define LSE_SIZE  0x238

void *lsc_Create(void *);
void lsc_EntryFileRange(void *, const char *, int, int, int);
void LSC_Stop(void *);

void *LSC_Create(void *handle) {
    void *r;
    LSC_Enter();
    r = lsc_Create(handle);
    LSC_Leave();
    return r;
}

void *lsc_Create(void *handle) {
    u8 *entry;
    int i;
    u32 crs;
    void (*getsize)(void *, int, s32 *);
    s32 size1, size2, total, adjusted;

    if (handle == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478);
        return NULL;
    }

    LSC_LockCrs(&crs);

    entry = NULL;
    for (i = 0; i < LSE_MAX; i++) {
        u8 *e = lbl_eu_805E7D40 + i * LSE_SIZE;
        if ((s8)e[0] == 0) { entry = e; break; }
    }

    if (entry == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0x30);
        LSC_UnlockCrs(&crs);
        return NULL;
    }

    memset(entry, 0, LSE_SIZE);
    *(void **)(entry + 0x08) = handle;

    getsize = (void (*)(void *, int, s32 *))((u32 *)(*(void **)handle))[9];
    getsize(handle, 1, &size1);
    getsize(handle, 0, &size2);
    total = size1 + size2;
    *(s32 *)(entry + 0x18) = total;
    /* total * 2/3 roughly */
    adjusted = (total / 3) * 2;
    *(s32 *)(entry + 0x1C) = adjusted;

    entry[0] = 1;
    entry[0x01] = 0;
    LSC_UnlockCrs(&crs);
    return entry;
}

void LSC_Destroy(void *entry) {
    s32 idx;
    LSC_Enter();
    idx = (s32)(((u8 *)entry - lbl_eu_805E7D40) / LSE_SIZE);
    if (idx >= 0 && idx < LSE_MAX && entry != NULL) {
        ADXSTM_StopNw(*(void **)((u8 *)entry + 0x10));
        ADXSTM_ReleaseFileNw(*(void **)((u8 *)entry + 0x10));
        memset(entry, 0, LSE_SIZE);
    }
    LSC_Leave();
}

void LSC_SetStmHndl(void *entry, void *stm) {
    LSC_Enter();
    *(void **)((u8 *)entry + 0x28) = stm;
    LSC_Leave();
}

int LSC_EntryFname(void *entry, const char *fname) {
    int r;
    LSC_Enter();
    lsc_EntryFileRange(entry, fname, 0, 0, 0);
    r = 0;
    LSC_Leave();
    return r;
}

int LSC_EntryFileRange(void *entry, const char *fname, int a, int b, int c) {
    int r;
    LSC_Enter();
    lsc_EntryFileRange(entry, fname, a, b, c);
    r = 0;
    LSC_Leave();
    return r;
}

void lsc_EntryFileRange(void *entry, const char *fname, int off_lo, int off_hi, int size) {
    int i;
    u8 *e = (u8 *)entry;
    char *dst;

    LSC_LockCrs(entry);

    if ((s8)e[0x01] > 0)
        LSC_Stop(entry);

    dst = (char *)e + 0x3C;
    for (i = 0; i < 14 && fname[i]; i++)
        dst[i] = fname[i];
    dst[i] = 0;

    *(s32 *)(e + 0x44) = off_lo;
    *(s32 *)(e + 0x48) = off_hi;
    *(s32 *)(e + 0x4C) = size;

    LSC_UnlockCrs(entry);
}

int LSC_Start(void *entry) {
    u8 *e = (u8 *)entry;
    if ((s8)e[0x01] > 0) return -1;
    e[0x01] = 1;
    e[0x02] = 0;
    e[0x03] = 1;
    e[0x04] = 0;
    *(s32 *)(e + 0x20) = 0;
    *(s32 *)(e + 0x24) = *(s32 *)(e + 0x18);
    if (*(s32 *)(e + 0x1C) > 0)
        e[0x03] = 2;
    LSC_CallStatFunc();
    return 0;
}

void LSC_Stop(void *entry) {
    u8 *e = (u8 *)entry;
    LSC_Enter();
    if ((s8)e[0x01] != 0) {
        ADXSTM_StopNw(*(void **)(e + 0x10));
        ADXSTM_ReleaseFileNw(*(void **)(e + 0x10));
        e[0x01] = 0;
        e[0x02] = 0;
    }
    LSC_Leave();
}

int LSC_Pause(void *entry, int pause) {
    u8 *e = (u8 *)entry;
    LSC_Enter();
    if (pause) {
        ADXSTM_StopNw(*(void **)(e + 0x10));
        e[0x04] = 1;
    } else {
        ADXSTM_Start(*(void **)(e + 0x10));
        e[0x04] = 0;
    }
    LSC_Leave();
    return 0;
}

void LSC_ExecServer(void *entry) {
    u8 *e = (u8 *)entry;
    if (e[0x04] == 1) return;
    if ((s8)e[0x01] != 2) return;
    if (ADXSTM_GetStat(*(void **)(e + 0x10)) == 4)
        e[0x01] = 3;
}

int LSC_GetStat(void *entry) {
    return *(s8 *)((u8 *)entry + 0x01);
}

int LSC_GetNumStm(void) {
    int i, n = 0;
    for (i = 0; i < LSE_MAX; i++) {
        if (*(s8 *)(lbl_eu_805E7D40 + i * LSE_SIZE) != 0)
            n++;
    }
    return n;
}

void LSC_SetFlowLimit(void *entry, int limit) {
    u8 *e = (u8 *)entry;
    *(s32 *)(e + 0x18) = limit;
    *(s32 *)(e + 0x24) = limit;
    *(s32 *)(e + 0x1C) = (limit / 3) * 2;
}

extern u32 lbl_eu_805EC440[];

void LSC_CallStatFunc(void) {
    u32 fn = lbl_eu_805EC440[0];
    if (fn == 0) return;
    ((void (*)(u32, u32))fn)(lbl_eu_805EC440[1], lbl_eu_805EC440[2]);
}

void LSC_SetLpFlg(void *entry, int flag) {
    *(u8 *)((u8 *)entry + 0xA8) = (flag != 0) ? 1 : 0;
}
