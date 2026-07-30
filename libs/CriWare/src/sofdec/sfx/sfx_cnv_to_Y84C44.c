#include <harness_catalog.h>

extern int CFT_Ycc420plnToY84C44(void);
extern void SFX_SetCnvFrmCbFunc(void *ctx, void *cb);
extern void SFX_SetCopyAlphaCbFunc(void *ctx, void *cb);
extern void SFX_SetMakeLumiTableCbFunc(void *ctx, void *cb);
extern void SFX_SetMakeAlp3TableCbFunc(void *ctx, void *cb);
extern void SFX_SetMakeAlp3110TableCbFunc(void *ctx, void *cb);
extern void SFX_SetMakeColAdjTableCbFunc(void *ctx, void *cb);
extern int SFX_CnvFrmByCbFunc(void *ctx, void *arg1, void *arg2);

int SFX_CnvFrmY84C44ByCbFunc(void *ctx, void *arg1, void *arg2) {
    *(void **)((u8 *)ctx + 0x50) = NULL;
    SFX_SetCnvFrmCbFunc(ctx, CFT_Ycc420plnToY84C44);
    SFX_SetCopyAlphaCbFunc(ctx, NULL);
    SFX_SetMakeLumiTableCbFunc(ctx, NULL);
    SFX_SetMakeAlp3TableCbFunc(ctx, NULL);
    SFX_SetMakeAlp3110TableCbFunc(ctx, NULL);
    SFX_SetMakeColAdjTableCbFunc(ctx, NULL);
    return SFX_CnvFrmByCbFunc(ctx, arg1, arg2);
}
