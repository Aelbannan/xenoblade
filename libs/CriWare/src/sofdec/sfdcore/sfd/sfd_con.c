// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_con
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef struct {
    u8 _pad00[0x164];
    s32 time;
    s32 idx;
    s32 timeQueue[32];
    s32 value;
    s32 totalSmpl;
    s32 _pad1f4;
    s32 wr;
    s32 rd;
    s32 smplQueue[32];
} SfdConcatContext;

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
void SFCON_UpdateConcatTime(void* h, s32 delta) {
    u32 cs;
    SfdConcatContext* ctx = (SfdConcatContext*)((u8*)h + 0xd98);
    SFLIB_LockCs(&cs);
    s32 idx = ctx->idx;
    s32 time = ctx->time;
    s32 new_time = time + delta;
    ctx->time = new_time;
    s32 new_idx = idx + 1;
    ctx->timeQueue[new_idx % 32] = new_time;
    ctx->idx = new_idx;
    SFLIB_UnlockCs(&cs);
}

void SFLIB_LockCs(void* cs);
void SFLIB_UnlockCs(void* cs);
s32 SFCON_WriteTotSmplQue(void* h, s32 lastSmpl, s32 value) {
    u32 cs;
    SFLIB_LockCs(&cs);
    SfdConcatContext* ctx = (SfdConcatContext*)((u8*)h + 0xd98);
    s32 wr = ctx->wr;
    s32 rd = ctx->rd;
    if (wr - rd >= 32) {
        SFLIB_UnlockCs(&cs);
        return 0;
    }
    ctx->value = value;
    ctx->smplQueue[wr % 32] = lastSmpl;
    ctx->wr = wr + 1;
    ctx->totalSmpl += lastSmpl;
    SFLIB_UnlockCs(&cs);
    return 1;
}

void SFLIB_LockCs(void* cs);
void SFLIB_UnlockCs(void* cs);
s32 SFCON_ReadTotSmplQue(void* h, s32* lastSmpl, s32* value) {
    u32 cs;
    SFLIB_LockCs(&cs);
    SfdConcatContext* ctx = (SfdConcatContext*)((u8*)h + 0xd98);
    s32 rd = ctx->rd;
    s32 wr = ctx->wr;
    if (wr - rd <= 0) {
        *lastSmpl = -1;
        SFLIB_UnlockCs(&cs);
        return 0;
    }
    *value = ctx->value;
    *lastSmpl = ctx->smplQueue[rd % 32];
    ctx->rd = rd + 1;
    SFLIB_UnlockCs(&cs);
    return 1;
}
