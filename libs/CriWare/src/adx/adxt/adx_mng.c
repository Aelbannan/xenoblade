// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_mng
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

u32 adxm_framework = 0xFFFFFFFF;

void ADXMNG_SetFramework(u32 val) { adxm_framework = val; }

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
    if ((s32)adxm_framework == -1) {
        s32 r = ADXM_IsSetupThrd();
        state = 1;
        if (r == 1) {
            state = 2;
        }
    } else {
        state = adxm_framework;
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
