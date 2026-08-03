#include <harness_catalog.h>

extern int SFD_Start(void *);
extern int SFD_TermSupply(void *);
extern int SFD_Stop(void *);
extern int SFD_Pause(void *, int);
extern int SFD_RecordFname(void *, const char *);
extern int MWSFSVM_Error(const char *, ...);
extern int MWSFLIB_SetErrCode(int);
extern int mw_sfd_start_ex(void *, void *, void *);
extern void MWSFD_SetFlowLimit(void *, u32);
extern int MWSTM_GetStat(void *);
extern size_t strlen(const char *);
extern char *strncpy(char *, const char *, size_t);
extern char *strcpy(char *, const char *);
extern int MWSFD_IsFsBdr(void *);
extern int MWSFSVR_IsSvrBdrHndl(void *);

extern char lbl_eu_8051B1A0[];
extern double lbl_eu_8051B190;

/* Get SFD handle from player handle (+0x58) */
static void *sfd(void *h) { return *(void **)((u8 *)h + 0x58); }

int mwPlySfdStart(void *h) {
    if (SFD_Start(sfd(h))) {
        MWSFLIB_SetErrCode(-0x133);
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x1B);
        return -0x133;
    }
    return 0;
}

int mwPlyTermSupply(void *h) {
    if (SFD_TermSupply(sfd(h)))
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x62);
}

int mw_sfd_start_ex(void *a, void *b, void *c) {
    /* Complex start logic (0x298 bytes in retail) */
    return 0;
}

int mwPlyStartFname(void *h, const char *fname) {
    return mw_sfd_start_ex(h, (void *)fname, NULL);
}


int mwSfdStopDec(void *h) {
    SFD_Stop(sfd(h));
    return 0;
}

int mwPlyStop(void *h) {
    SFD_Stop(sfd(h));
    return 0;
}

int mwPlyPause(void *h, int pause) {
    return SFD_Pause(sfd(h), pause);
}

int MWSFPLY_SetFlowLimit(void *h) {
    MWSFD_SetFlowLimit(h,
        (u32)(s32)(lbl_eu_8051B190 * (double)(s32)*(s32 *)((u8 *)h + 0x50C)));
}

int mwPlyChkSupply(void *h) {
    void *stm = *(void **)((u8 *)h + 0x5C);
    if (stm && MWSTM_GetStat(stm) == 3 && SFD_TermSupply(*(void **)((u8 *)h + 0x58)))
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x62);
}

int MWSFPLY_RecordFname(void *h, const char *fname) {
    if ((s32)strlen(fname) > *(s32 *)((u8 *)h + 0x4EC)) {
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x185);
        strncpy(*(char **)((u8 *)h + 0x4E8), fname, *(u32 *)((u8 *)h + 0x4EC));
    } else {
        strcpy(*(char **)((u8 *)h + 0x4E8), fname);
    }
}

extern u8 lbl_eu_805669F4[];
extern void* lbl_eu_805FF3A0;
extern s32 MWSFD_IsEnableHndl(void* self);
extern void SFD_RequestStop(void* sst);
extern void MWSFD_RequestStopRead(void* self);
extern void MWSFD_SetProhibitServer(int on);

void mwPlyStartSj(void *h, int sj) {
    if (MWSFD_IsEnableHndl(h) != 1) {
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x336);
        return;
    }
    MWSFD_SetProhibitServer(1);
    mwSfdStopDec(h);
    *(s32*)((u8*)h + 0x500) = sj;
    *(s32*)((u8*)h + 0x514) = 2;
    *(s32*)((u8*)h + 0x518) = 0;
    *(s32*)((u8*)h + 0x51c) = 0;
    *(s32*)((u8*)h + 0x520) = 0;
    mw_sfd_start_ex(h, (void*)2, NULL);
    MWSFD_SetProhibitServer(0);
}

void fn_803A537C(void* self) {
    if (lbl_eu_805FF3A0 != NULL) {
        *(void**)(lbl_eu_805669F4 + 0xC) = self;
        ((void (*)(void*, void*))*(void**)((char*)*(void**)lbl_eu_805FF3A0 + 0x24))(
            lbl_eu_805FF3A0, lbl_eu_805669F4 + 4);
    }
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x380);
        return;
    }
    if (*(s32*)((u8*)self + 4) != 0 && *(s32*)((u8*)self + 0x678) != 1) {
        *(s32*)((u8*)self + 0x678) = 1;
        SFD_RequestStop(*(void**)((u8*)self + 0x58));
        MWSFD_RequestStopRead(self);
    }
    if (lbl_eu_805FF3A0 != NULL) {
        ((void (*)(void*, void*))*(void**)((char*)*(void**)lbl_eu_805FF3A0 + 0x24))(
            lbl_eu_805FF3A0, lbl_eu_805669F4 + 0x6C);
    }
}

int MWSFD_IsEndPrepareStop(void *h) {
    if (MWSFD_IsFsBdr(h) == 0)
        return 0;
    return !!MWSFSVR_IsSvrBdrHndl(h);
}
