// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_con
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(s32 val, u32 code);
void SFSET_SetCond(void* h, u32 cond, u32 sw);
s32 SFD_SetConcatPlay(void* h) {
    if (SFLIB_CheckHn(h) != 0) {
        return SFLIB_SetErr(0, 0xFF000000u + 0x161u);
    }
    SFSET_SetCond(h, 0x31, 1);
    return 0;
}

s32 SFSET_GetCond(void* h, u32 cond);
s32 SFCON_IsEndcodeSkip(void* h) {
    return SFSET_GetCond(h, 0x31) ? 1 : 0;
}

s32 SFSET_GetCond(void* h, u32 cond);
s32 SFCON_IsSystemEndcodeSkip(void* h) {
    s32 r = SFSET_GetCond(h, 0x31);
    if (r != 0) goto ret1;
    r = SFSET_GetCond(h, 0x38);
    if (r == 0) goto ret0;
ret1:
    return 1;
ret0:
    return 0;
}

s32 SFSET_GetCond(void* h, u32 cond);
s32 SFCON_IsVideoEndcodeSkip(void* h) {
    s32 r = SFSET_GetCond(h, 0x31);
    if (r != 0) goto ret1;
    r = SFSET_GetCond(h, 0x39);
    if (r == 0) goto ret0;
ret1:
    return 1;
ret0:
    return 0;
}

void SFLIB_LockCs(void* cs);
void SFLIB_UnlockCs(void* cs);
void SFCON_UpdateConcatTime(SfdHandle* h, s32 delta) {
    u32 cs;
    SfdTim* tim = &h->tim;
    SFLIB_LockCs(&cs);
    s32 idx = tim->concat.idx;
    s32 time = tim->concat.time;
    idx++;
    s32 new_time = time + delta;
    tim->concat.time = new_time;
    tim->concat.timeQueue[idx % 32] = new_time;
    tim->concat.idx = idx;
    SFLIB_UnlockCs(&cs);
}

void SFLIB_LockCs(void* cs);
void SFLIB_UnlockCs(void* cs);
s32 SFCON_WriteTotSmplQue(SfdHandle* h, s32 lastSmpl, s32 value) {
    u32 cs;
    SfdTim* tim = &h->tim;
    s32 rd;
    s32 wr;
    s32 r;
    SFLIB_LockCs(&cs);
    wr = tim->concat.wr;
    rd = tim->concat.rd;
    /* ring full check: writer has lapped the reader by 32 entries */
    if (tim->concat.wr - tim->concat.rd >= 32) {
        r = 0;
    } else {
        tim->concat.value = value;
        tim->concat.smplQueue[tim->concat.wr % 32] = lastSmpl;
        tim->concat.wr += 1;
        tim->concat.totalSmpl += lastSmpl;
        r = 1;
    }
    SFLIB_UnlockCs(&cs);
    return r;
}

void SFLIB_LockCs(void* cs);
void SFLIB_UnlockCs(void* cs);
s32 SFCON_ReadTotSmplQue(SfdHandle* h, s32* lastSmpl, s32* value) {
    u32 cs;
    SfdTim* tim = &h->tim;
    s32 rd;
    s32 wr;
    s32 r;
    SFLIB_LockCs(&cs);
    rd = tim->concat.rd;
    wr = tim->concat.wr;
    if (wr - rd <= 0) {
        *lastSmpl = -1;
        r = 0;
    } else {
        *value = tim->concat.value;
        *lastSmpl = tim->concat.smplQueue[tim->concat.rd % 32];
        tim->concat.rd += 1;
        r = 1;
    }
    SFLIB_UnlockCs(&cs);
    return r;
}
