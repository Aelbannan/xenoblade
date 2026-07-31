#include <harness_catalog.h>

extern void ADXCRS_Enter(void);
extern void ADXCRS_Leave(void);
extern void ADXT_Stop(void *);
extern int ADXF_GetFnameRangeEx(const char *, int, void *, u32 *, u32 *, u32 *);
extern void ADXERR_CallErrFunc1_(const char *);
extern void ADXERR_CallErrFunc2_(const char *, const char *);
extern void ADXERR_ItoA2(int, int, char *, int);
extern char *CRICRW_Strcpy(char *, int, const char *);
extern void ADXT_SetLnkSw(void *, int);

extern char lbl_eu_805161A0[];

void adxt_StartAfs(void *handle, const char *filename, int flags);

void ADXT_StartAfs(void *handle, const char *filename, int flags) {
    ADXCRS_Enter();
    adxt_StartAfs(handle, filename, flags);
    ADXCRS_Leave();
}

void adxt_StartAfs(void *handle, const char *filename, int flags) {
    char tmp[16];
    u32 range_end, range_size, range_start;

    if (handle == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805161A0);
        return;
    }

    ADXT_Stop(handle);

    if (ADXF_GetFnameRangeEx(filename, flags, *(void **)((u8 *)handle + 0xAC),
        &range_start, &range_end, &range_size) != 0)
        return;

    if (*(u32 *)((u8 *)handle + 0x08) == 0) {
        ADXERR_ItoA2((s32)filename, flags, tmp, 16);
        ADXERR_CallErrFunc2_(lbl_eu_805161A0 + 0x29, tmp);
        *(s16 *)((u8 *)handle + 0x60) = -1;
        *(u8 *)((u8 *)handle + 0x01) = 0x06;
        return;
    }

    void *lnkH = handle;
    u32 lnkV = 0;
    *(u32 *)((u8 *)handle + 0xB0) = *(u32 *)((u8 *)handle + 0xAC);
    *(u32 *)((u8 *)handle + 0xB4) = range_start;
    *(u32 *)((u8 *)handle + 0xB8) = range_end;
    *(u32 *)((u8 *)handle + 0xBC) = range_size;
    *(u8 *)((u8 *)handle + 0x01) = 1;
    *(u8 *)((u8 *)handle + 0xA8) = 1;
    *(u8 *)((u8 *)handle + 0x02) = 1;
    ADXT_SetLnkSw(lnkH, lnkV);
}

int ADXT_StartFnameRange(void *handle, const char *fname) {
    ADXCRS_Enter();

    if (handle == NULL || fname == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805161A0 + 0x4D);
    } else {
        ADXT_Stop(handle);
        CRICRW_Strcpy(*(char **)((u8 *)handle + 0xAC), 0x100, fname);
        *(u32 *)((u8 *)handle + 0xB0) = *(u32 *)((u8 *)handle + 0xAC);
        *(u32 *)((u8 *)handle + 0xB4) = 0;
        *(u32 *)((u8 *)handle + 0xB8) = 0;
        *(u32 *)((u8 *)handle + 0xBC) = 0x000FFFFF;
        *(u8 *)((u8 *)handle + 0x01) = 1;
        *(u8 *)((u8 *)handle + 0xA8) = 1;
        *(u8 *)((u8 *)handle + 0x02) = 0;
        ADXT_SetLnkSw(handle, 0);
    }

    ADXCRS_Leave();
}

