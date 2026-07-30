#include <harness_catalog.h>

extern int SFD_Start(void *);
extern int SFD_TermSupply(void *);
extern int SFD_Stop(void *);
extern int SFD_Pause(void *, int);
extern int SFD_SetFlowLimit(void *, int);
extern int SFD_RecordFname(void *, const char *);
extern int MWSFSVM_Error(const char *, ...);
extern int MWSFLIB_SetErrCode(int);
extern int mw_sfd_start_ex(void *, void *, void *);

extern char lbl_eu_8051B1A0[];

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

int mwPlyStartSj(void *h, int sj) {
    return mw_sfd_start_ex(h, NULL, (void *)(u32)sj);
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

int MWSFPLY_SetFlowLimit(void *h, int limit) {
    return SFD_SetFlowLimit(sfd(h), limit);
}

int mwPlyChkSupply(void *h) {
    /* Check supply state */
    return 0;
}

int MWSFPLY_RecordFname(void *h, const char *fname) {
    return SFD_RecordFname(sfd(h), fname);
}

int fn_803A537C(void *a, void *b) {
    /* Unknown sub-function */
    return 0;
}

int MWSFD_IsFsBdr(void*);
int MWSFSVR_IsSvrBdrHndl(void*);

int MWSFD_IsEndPrepareStop(void* h) {
    if (!MWSFD_IsFsBdr(h)) {
        return 0;
    }
    return !!MWSFSVR_IsSvrBdrHndl(h);
}
