#include <harness_catalog.h>

#define ADX_FS_WORK  lbl_eu_805E0630
#define ADX_FS_FLAG  lbl_eu_805E0624

extern void ADXCRS_Enter(void);
extern void ADXCRS_Leave(void);
extern void ADXERR_CallErrFunc1_(const char *);
extern char lbl_eu_805157E0[];
extern u32 lbl_eu_805E0624;
extern u32 lbl_eu_805E0630;

/* adxf_LoadPtBothNw - internal */
int adxf_LoadPtBothNw(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);
/* adxf_GetPtStat - internal */
int adxf_GetPtStat(int a);

/* ADXF_LoadPartitionNw - external wrapper */
int ADXF_LoadPartitionNw(int a, int b, int c, int d, int e, int f) {
    int r;
    ADXCRS_Enter();
    r = adxf_LoadPtBothNw(a, 0, 0, b, c, 0, d, 0, 0xFFFF, e);
    ADXCRS_Leave();
    return r;
}

/* adxf_LoadPtBothNw - internal implementation */
int adxf_LoadPtBothNw(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10) {
    if (lbl_eu_805E0624 != 2) {
        /* check initialization */
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x61);
        return -1;
    }
    /* Partition loading logic (0x68C bytes in retail) */
    /* Placeholder for the actual implementation */
    return 0;
}

int ADXF_GetPtStat(int a) {
    int r;
    ADXCRS_Enter();
    r = adxf_GetPtStat(a);
    ADXCRS_Leave();
    return r;
}

int adxf_GetPtStat(int a) {
    /* Get partition status (0xE3C bytes in retail - huge!) */
    return 0;
}

int adxf_CreateAdxFs(void) {
    return 0;
}

int ADXF_OpenNw(int a, int b, int c, int d) {
    int r;
    ADXCRS_Enter();
    r = 0;
    ADXCRS_Leave();
    return r;
}

int ADXF_IsOpened(int a) {
    int r;
    ADXCRS_Enter();
    r = 0;
    ADXCRS_Leave();
    return r;
}

int ADXF_Close(int a) {
    int r;
    ADXCRS_Enter();
    r = 0;
    ADXCRS_Leave();
    return r;
}

int ADXF_CloseAll(void) {
    int r = 0;
    /* close all handles */
    return r;
}

int adxf_read_sj32(int a, int b) { return 0; }
int adxf_ReadNw32(int a, int b, int c, int d) { return 0; }
int ADXF_ReadNw(int a, int b, int c, int d, int e) { int r; ADXCRS_Enter(); r = 0; ADXCRS_Leave(); return r; }
int ADXF_Stop(int a) { int r; ADXCRS_Enter(); r = adxf_Stop(a); ADXCRS_Leave(); return r; }
int adxf_Stop(int a) { return 0; }
int adxf_ExecOne(int a) { return 0; }
int ADXF_ExecServer(int a) { int r; ADXCRS_Enter(); r = 0; ADXCRS_Leave(); return r; }
int ADXF_Seek(int a, int b, int c) { int r; ADXCRS_Enter(); r = 0; ADXCRS_Leave(); return r; }
int adxf_Seek(int a, int b, int c) { return 0; }
int ADXF_GetFsizeSct(int a) { int r; ADXCRS_Enter(); r = 0; ADXCRS_Leave(); return r; }
int ADXF_GetFsizeByte(int a) { int r; ADXCRS_Enter(); r = 0; ADXCRS_Leave(); return r; }
int ADXF_GetNumReqSct(int a) { return 0; }
int adxf_ChkPrmGfr(int a) { return 0; }
int ADXF_GetFnameRangeEx(int a, int b, int c, int d, int e, int f) { return 0; }
int adxf_GetFnameRangeEx(int a, int b, int c, int d, int e, int f) { return 0; }
extern u32 lbl_eu_805E00F0[];

char* ADXF_GetFnameFromPt(int idx) {
    char* p;
    ADXCRS_Enter();
    p = (char*)lbl_eu_805E00F0[idx];
    ADXCRS_Leave();
    return p + 0x10;
}
void ADXT_ExecFsSvr(void) { ADXCRS_Enter(); ADXCRS_Leave(); }
void adxt_ExecFsSvr(void) {}
