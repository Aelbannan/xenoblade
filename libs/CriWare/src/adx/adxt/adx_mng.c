// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_mng
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

__declspec(section ".data") __attribute__((aligned(8))) u32 lbl_eu_80560028[2] = {0xFFFFFFFF, 0};

void ADXMNG_SetFramework(u32 val) { lbl_eu_80560028[0] = val; }

extern int ADXM_IsSetupThrd(void);
extern void SVM_ExecSvrVint(void);
extern void SVM_ExecSvrUsrVsync(void);
extern void SVM_ExecSvrVsync(void);
extern void SVM_ExecSvrUhigh(void);
extern void SVM_ExecSvrMain(void);
extern void SVM_ExecSvrMwIdle(void);
extern void SVM_ExecSvrUsrIdle(void);
extern void SVM_ExecSvrFs(void);

int ADXMNG_CallMainServerFunctions(void) {
    s32 state;
    if ((s32)lbl_eu_80560028[0] == -1) {
        s32 r = ADXM_IsSetupThrd();
        state = 1;
        if (r == 1) {
            state = 2;
        }
    } else {
        state = lbl_eu_80560028[0];
    }
    if (state == 1) goto case1;
    if (state == 2) goto case2;
    if (state == 3) goto case3;
    goto out;
case1:
    SVM_ExecSvrVint();
    SVM_ExecSvrUsrVsync();
    SVM_ExecSvrVsync();
    SVM_ExecSvrUhigh();
    SVM_ExecSvrMain();
    SVM_ExecSvrMwIdle();
    SVM_ExecSvrUsrIdle();
    SVM_ExecSvrFs();
    goto out;
case2:
    SVM_ExecSvrMain();
    goto out;
case3:
    SVM_ExecSvrMwIdle();
    SVM_ExecSvrUsrIdle();
out:
    return 0;
}
