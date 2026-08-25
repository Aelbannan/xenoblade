// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_frm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPVFRM_Init(void) {}

extern s32 MPVM2V_DecodeFrm(void* h, void* a, void* b);
extern void MPVUMC_InitOutRfb(void* h);
extern void MPVCMC_InitMcOiRt(void* h);
extern void MPVCMC_SetCcnt(void* h);
extern void MPVCDEC_StartFrame(void* h);
extern void MPVBDEC_StartFrame(void* h);
extern s32 MPVSL_DecPicture(void* h, void* a);
extern void MPVUMC_EndOfFrame(void* h);

/* 128B decoded-stream block copied to the caller's output buffer. */
typedef struct MpvDecBuf {
    u32 w[32];              /* 0xb58 */
} MpvDecBuf;

/* SJ frame info passed in/out of MPV_DecodeFrmSj. */
typedef struct MpvFrmInfo {
    u32 field_0x00[13];     /* 0x00 */
    u32* outBuf;            /* 0x34: output buffer for the decoded stream */
    u32 field_0x38;         /* 0x38: byte-delta outputs */
    u32 field_0x3c;         /* 0x3c */
    s16 field_0x40;         /* 0x40 */
    u8 pad_0x42[2];
} MpvFrmInfo;

/* MPV decode handle - only the fields this TU touches are modelled. */
typedef struct MpvHn {
    u8 pad_0x00[0xb58];
    MpvDecBuf decBuf;       /* 0xb58: 128B decoded-stream copy source */
    u8 pad_0xbd8[0x10];
    u32 field_0xbe8;        /* 0xbe8 */
    u32 field_0xbec;        /* 0xbec */
    MpvFrmInfo savedFrm;    /* 0xbf0: 68B copied from the SJ frame info */
    u8 pad_0xc34[0xc8];
    s32 mode;               /* 0xcfc: codec mode (2 = MPEG2 video) */
} MpvHn;

s32 MPV_DecodeFrmSj(MpvHn* h, void* a2, MpvFrmInfo* a3) {
    s32 r;
    u32 sv2;
    u32 sv1;
    if (MPVLIB_CheckHn(h))
        return MPVERR_SetCode(NULL, 0xFF030209);
    if (h->mode == 2)
        return MPVM2V_DecodeFrm(h, a2, a3);
    sv2 = h->field_0xbec;
    sv1 = h->field_0xbe8;
    /* Copy the 68B SJ frame info into the handle output area. */
    h->savedFrm = *a3;
    MPVUMC_InitOutRfb(h);
    MPVCMC_InitMcOiRt(h);
    MPVCMC_SetCcnt(h);
    MPVCDEC_StartFrame(h);
    MPVBDEC_StartFrame(h);
    r = MPVSL_DecPicture(h, a2);
    MPVUMC_EndOfFrame(h);
    /* Copy 128B of decoded stream from the handle to a3->outBuf. */
    *(MpvDecBuf*)a3->outBuf = h->decBuf;
    a3->field_0x38 = h->field_0xbe8 - sv1;
    a3->field_0x3c = h->field_0xbec - sv2;
    a3->field_0x40 = h->savedFrm.field_0x40;
    return r;
}

extern s32 MPV_GoNextDelimSj(void* a);
extern s32 MPV_MoveChunk(void* a, s32 b, s32 c);

s32 MPV_SkipFrmSj(void* hn, void* sj) {
    void* p = sj;
    s32 code;
    /* Walk SJ delimiters, skipping chunks, until an error/end condition. */
    if (MPVLIB_CheckHn(hn) != 0)
        return MPVERR_SetCode(NULL, 0xFF03020A);
    code = 0xFF030305;
    for (;;) {
        int r = MPV_GoNextDelimSj(p);
        if (r == 0)
            break;
        if (r & 0xCC) {
            code = 0;
            break;
        }
        if (MPV_MoveChunk(p, 1, 4) == 4)
            continue;
        break;
    }
    return MPVERR_SetCode(hn, code);
}

extern int MPVLIB_CheckHn(void*);
extern int MPVERR_SetCode(void*, int);
extern int MPVM2V_RequestStop(void*);

s32 MPV_RequestStop(void* handle) {
    if (MPVLIB_CheckHn(handle)) {
        return MPVERR_SetCode(NULL, 0xFF030211);
    }
    *(u32*)((u8*)handle + 0xDA8) = 1;
    if ((s32)*(u32*)((u8*)handle + 0xCFC) == 2) {
        return MPVM2V_RequestStop(handle);
    }
    return 1;
}
