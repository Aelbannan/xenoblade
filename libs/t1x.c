#include <types.h>
typedef struct SvmSvrEntry { u32 (*func)(void*); void* object; const char* name; } SvmSvrEntry;
typedef struct SvmCtrl { u32 a,b,c,d; SvmSvrEntry svr_tbl[8][6]; u32 exec_flags[8]; u32 exec_counts[8]; } SvmCtrl;
extern SvmCtrl lbl_eu_805F26F0;

static u32 svm_exec_svr(u32 svtype) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    for (i = 0; i < 6; i++) {
        SvmSvrEntry* p = &ctrl->svr_tbl[svtype][i];
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            ctrl->exec_flags[svtype] = 1;
            result |= fn(obj);
            ctrl->exec_flags[svtype] = 0;
        }
    }
    ctrl->exec_counts[svtype] += 1;
    return result;
}
u32 vint(void) { return svm_exec_svr(0); }
u32 usrvsync(void) { return svm_exec_svr(1); }
u32 vsync2(void) { return svm_exec_svr(2); }
