#include <harness_catalog.h>
#include <string.h>

extern void LSC_Enter(void);
extern void LSC_Leave(void);
extern void LSC_LockCrs(void *);
extern void LSC_UnlockCrs(void *);
extern void LSC_CallErrFunc_(const char *, ...);
extern void LSC_CallStatFunc(void);
extern void lsc_ExecHndl(u8 *);
extern int ADXSTM_StopNw(void *);
extern void ADXSTM_Stop(void *);
extern int ADXSTM_Start(void *);
extern int ADXSTM_GetStat(void *);
extern int ADXSTM_ReleaseFileNw(void *);

extern char lbl_eu_80518478[];
extern u8 lbl_eu_805E7D40[];

#define LSE_MAX   0x20
#define LSE_SIZE  0x238

void *lsc_Create(void *);
int lsc_EntryFileRange(void *, const char *, int, int, int);
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
    r = lsc_EntryFileRange(entry, fname, 0, 0, 0x100000 - 1);
    LSC_Leave();
    return r;
}

int LSC_EntryFileRange(void *entry, const char *fname, int off_lo, int off_hi, int size) {
    int r;
    LSC_Enter();
    r = lsc_EntryFileRange(entry, fname, off_lo, off_hi, size);
    LSC_Leave();
    return r;
}

int lsc_EntryFileRange(void *entry, const char *fname, int off_lo, int off_hi, int size) {
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
    return 0;
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
    LSC_Enter();
    if (entry == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0x5F);
    } else {
        if (*(void **)((u8 *)entry + 0x28) != NULL) {
            ADXSTM_Stop(*(void **)((u8 *)entry + 0x28));
            ((u8 *)entry)[0x02] = 0;
        }
        if ((s8)((u8 *)entry)[0x01] != 0) {
            ((u8 *)entry)[0x01] = 0;
            *(s32 *)((u8 *)entry + 0x2C) = 0;
            LSC_Enter();
            if (entry == NULL) {
                LSC_CallErrFunc_(lbl_eu_80518478 + 0x88);
            } else if ((s8)((u8 *)entry)[0x01] == 0) {
                *(s32 *)((u8 *)entry + 0x1C) = 0;
                *(s32 *)((u8 *)entry + 0x20) = 0;
                *(s32 *)((u8 *)entry + 0x24) = 0;
            }
            LSC_Leave();
            *(s32 *)((u8 *)entry + 0x34) = 0;
        }
    }
    LSC_Leave();
}

void LSC_Pause(void *entry, int pause) {
    LSC_Enter();
    if (entry == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0x12C);
    } else if (pause == 1) {
        ((u8 *)entry)[0x04] = 1;
    } else {
        ((u8 *)entry)[0x04] = 0;
    }
    LSC_Leave();
}

void LSC_ExecServer(void) {
    int i;
    s8 *e;
    void *crs;

    LSC_Enter();
    LSC_LockCrs(&crs);

    e = (s8 *)lbl_eu_805E7D40;
    for (i = 0; i < LSE_MAX; i++, e += LSE_SIZE) {
        if (e[0x00] == 1)
            lsc_ExecHndl((u8 *)e);
    }

    LSC_UnlockCrs(&crs);
    LSC_Leave();
}

int LSC_GetStat(void *entry) {
    int r;
    LSC_Enter();
    if (entry == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0x155);
        r = -1;
    } else {
        r = *(s8 *)((u8 *)entry + 0x01);
    }
    LSC_Leave();
    return r;
}

int LSC_GetNumStm(void *entry) {
    int r;
    LSC_Enter();
    if (entry == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0x17E);
        r = -1;
    } else {
        r = *(s32 *)((u8 *)entry + 0x24);
    }
    LSC_Leave();
    return r;
}

void LSC_SetFlowLimit(void *entry, int limit) {
    LSC_Enter();
    if (entry == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0x2EC);
    } else if (limit < 0 || limit > *(s32 *)((u8 *)entry + 0x18)) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0x315, limit);
    } else {
        *(s32 *)((u8 *)entry + 0x14) = limit;
    }
    LSC_Leave();
}

extern u32 lbl_eu_805EC440[];

void LSC_CallStatFunc(void) {
    u32 *p = (u32 *)lbl_eu_805EC440;
    u32 fn = *p;
    if (fn == 0)
        return;
    ((void (*)(u32, u32))fn)(p[1], p[2]);
}

void LSC_SetLpFlg(void *entry, int flag) {
    LSC_Enter();
    if (entry == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0x369);
    } else {
        ((u8 *)entry)[0x03] = (u8)flag;
    }
    LSC_Leave();
}
