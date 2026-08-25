#include <harness_catalog.h>
#include <string.h>

extern void LSC_Enter(void);
extern void LSC_Leave(void);
extern void LSC_LockCrs(void *);
extern void LSC_UnlockCrs(void *);
extern void LSC_CallErrFunc_(const char *, ...);
extern void *LSC_CallStatFunc(void *);
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
void LSC_Stop(void *);

void *LSC_Create(void *handle) {
    void *r;
    LSC_Enter();
    r = lsc_Create(handle);
    LSC_Leave();
    return r;
}

typedef struct LscStm {
    void *hndl;             /* +0x00 */
    char pad[0x20 - 4];     /* +0x04 */
} LscStm;

/* Per-file slot record (0x20 bytes each, 16 slots at entry + 0x38) */
typedef struct LscSlot {
    s32         cnt;    /* +0x00 playback counter, wraps at 0x7FFFFFFF */
    char        *fname;  /* +0x04 */
    s32         sum;    /* +0x08 checksum: sum of fname bytes */
    s32         f44;    /* +0x0C */
    s32         f48;    /* +0x10 */
    s32         f4c;    /* +0x14 */
    void       *hndl;   /* +0x18 (cleared by lsc_Create) */
    s32         f54;    /* +0x1C */
} LscSlot;

typedef struct LscEntry {
    u8     flag;            /* +0x00 */
    s8     stat;            /* +0x01 */
    u8     pause;           /* +0x02 */
    u8     loop;            /* +0x03 */
    u8     dummy;           /* +0x04 */
    char   pad5[3];         /* +0x05 */
    void  *hndl;            /* +0x08 */
    char   padc[0x14 - 0x0C]; /* +0x0C */
    s32    limit;           /* +0x14 */
    s32    total;           /* +0x18 */
    s32    cur;             /* +0x1C */
    s32    idx;             /* +0x20 */
    s32    num;             /* +0x24 */
    void  *stm;             /* +0x28 */
    s32    f2c;             /* +0x2C */
    char   pad30[0x34 - 0x30]; /* +0x30 */
    s32    f34;             /* +0x34 */
    LscSlot slots[16];      /* +0x38 */
} LscEntry;

int lsc_EntryFileRange(LscEntry *, char *, int, int, int);

void *lsc_Create(void *handle) {
    s32 crs;
    s32 size1, size2, total;
    int i;
    u8 *p;
    LscEntry *entry;

    if (handle == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478);
        return NULL;
    }

    LSC_LockCrs(&crs);

    entry = NULL;
    for (i = 0, p = lbl_eu_805E7D40; i < LSE_MAX; p += LSE_SIZE, i++) {
        if ((s8)p[0] == 0) {
            entry = (LscEntry *)&lbl_eu_805E7D40[i * LSE_SIZE];
            break;
        }
    }

    if (entry == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0x30);
    } else {
        memset(entry, 0, LSE_SIZE);
        entry->hndl = handle;
        entry->stat = 0;

        size1 = ((s32 (**)(void *, int))(*(void **)handle))[9](handle, 1);
        size2 = ((s32 (**)(void *, int))(*(void **)handle))[9](handle, 0);
        total = size2 + size1;
        entry->total = total;
        entry->limit = (total * 8) / 10;

        for (i = 0; i < 0x10; i++)
            entry->slots[i].hndl = NULL;

        entry->flag = 1;
    }

    LSC_UnlockCrs(&crs);
    return entry;
}

void LSC_Destroy(LscEntry *entry) {
    LSC_Enter();
    if (entry != NULL) {
        LSC_Enter();
        if (entry == NULL) {
            LSC_CallErrFunc_(lbl_eu_80518478 + 0x5F);
        } else {
            if (entry->stm != NULL) {
                ADXSTM_Stop(entry->stm);
                entry->pause = 0;
            }
            if ((s8)entry->stat != 0) {
                entry->stat = 0;
                entry->f2c = 0;
                LSC_Enter();
                if (entry == NULL) {
                    LSC_CallErrFunc_(lbl_eu_80518478 + 0x88);
                } else if ((s8)entry->stat == 0) {
                    entry->cur = 0;
                    entry->idx = 0;
                    entry->num = 0;
                }
                LSC_Leave();
                entry->f34 = 0;
            }
        }
        LSC_Leave();
        entry->flag = 0;
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
    r = lsc_EntryFileRange(entry, (char *)fname, 0, 0, 0x100000 - 1);
    LSC_Leave();
    return r;
}

int LSC_EntryFileRange(void *entry, const char *fname, int off_lo, int off_hi, int size) {
    int r;
    LSC_Enter();
    r = lsc_EntryFileRange(entry, (char *)fname, off_lo, off_hi, size);
    LSC_Leave();
    return r;
}

int lsc_EntryFileRange(LscEntry *e, char *fname, int off_lo, int off_hi, int size) {
    LscSlot *slot;
    int cnt;
    int i;
    int sum;
    u32 len;

    if (e == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0xB1);
        return -1;
    }
    if (e->num >= 0x10)
        return -1;
    if (fname == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0xDA);
        return -1;
    }

    /* Slots form a ring; the new slot inherits the previous slot's counter. */
    slot = &e->slots[e->cur];
    i = (e->cur + 15) % 16;
    cnt = e->slots[i].cnt;
    if (cnt == 0x7FFFFFFF)
        cnt = 0;
    else
        cnt = cnt + 1;
    slot->cnt = cnt;
    slot->fname = fname;

    len = strlen(fname);
    slot->sum = 0;
    for (i = 0; i < len; i++) {
        sum = slot->sum + fname[i];
        slot->sum = sum;
    }

    slot->f48 = off_hi;
    slot->f4c = size;
    slot->f44 = off_lo;
    slot->hndl = NULL;
    slot->f54 = 0;

    e->num++;
    if (e->stat == 1)
        e->stat = 2;
    e->cur = e->cur + 1;
    return cnt;
}

void LSC_Start(void *entry) {
    u8 *e = (u8 *)entry;
    s32 crs;

    LSC_Enter();
    if (entry == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0x103);
    } else {
        LSC_LockCrs(&crs);
        if ((s8)e[0x01] != 0) {
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
        if (*(s32 *)(e + 0x24) > 0)
            e[0x01] = 2;
        else
            e[0x01] = 1;
        LSC_UnlockCrs(&crs);
    }
    LSC_Leave();
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

// LSC_CallStatFunc: dispatch to the stat hook. The void* return + `return h`
// null path are REQUIRED for retail codegen: the return-value liveness keeps
// the address base in r4 (retail lis r4; addi r4; lwz r12,0(r4)), and
// `#pragma peephole off` blocks MWCC's lwzu fold of the addi+lwz pair
// (peephole on always emits `lwzu r12, LO(r4)` — 0x24 vs retail 0x28).
// The final `return fn(a1,a2)` passes the callee's r3 through (retail
// bctrl; blr with no extra move).
#pragma peephole off
void *LSC_CallStatFunc(void *h) {
    u32 *p = (u32 *)lbl_eu_805EC440;
    u32 fn = p[0];
    if (fn == 0)
        return h;
    return ((void *(*)(u32, u32))fn)(p[1], p[2]);
}
#pragma peephole on

void LSC_SetLpFlg(void *entry, int flag) {
    LSC_Enter();
    if (entry == NULL) {
        LSC_CallErrFunc_(lbl_eu_80518478 + 0x369);
    } else {
        ((u8 *)entry)[0x03] = (u8)flag;
    }
    LSC_Leave();
}
