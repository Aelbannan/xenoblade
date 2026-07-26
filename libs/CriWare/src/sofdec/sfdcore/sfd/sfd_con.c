// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_con
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803c25d8
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
// LLM-HARNESS-END: us-803c25d8

// LLM-HARNESS-BEGIN: us-803c2634
s32 SFSET_GetCond(void* h, u32 cond);
s32 SFCON_IsEndcodeSkip(void* h) {
    return SFSET_GetCond(h, 0x31) ? 1 : 0;
}
// LLM-HARNESS-END: us-803c2634

// LLM-HARNESS-BEGIN: us-803c2664
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
// LLM-HARNESS-END: us-803c2664

// LLM-HARNESS-BEGIN: us-803c26bc
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
// LLM-HARNESS-END: us-803c26bc

// LLM-HARNESS-BEGIN: us-803c2714
void SFLIB_LockCs(void* cs);
void SFLIB_UnlockCs(void* cs);
void SFCON_UpdateConcatTime(void* h, s32 delta) {
    u32 cs;
    SFLIB_LockCs(&cs);
    u8* base = (u8*)h + 0xd98;
    s32 idx = *(s32*)(base + 0x168);
    s32 time = *(s32*)(base + 0x164) + delta;
    *(s32*)(base + 0x164) = time;
    idx++;
    *(s32*)(base + 0x16c + (idx % 32) * 4) = time;
    *(s32*)(base + 0x168) = idx;
    SFLIB_UnlockCs(&cs);
}
// LLM-HARNESS-END: us-803c2714

// LLM-HARNESS-BEGIN: us-803c2790
void SFLIB_LockCs(void* cs);
void SFLIB_UnlockCs(void* cs);
s32 SFCON_WriteTotSmplQue(void* h, s32 lastSmpl, s32 value) {
    u32 cs;
    SFLIB_LockCs(&cs);
    u8* base = (u8*)h + 0xd98;
    s32 wr = *(s32*)(base + 0x1f8);
    s32 rd = *(s32*)(base + 0x1fc);
    if (wr - rd >= 32) {
        SFLIB_UnlockCs(&cs);
        return 0;
    }
    *(s32*)(base + 0x1ec) = value;
    *(s32*)(base + 0x200 + (wr % 32) * 4) = lastSmpl;
    *(s32*)(base + 0x1f8) = wr + 1;
    *(s32*)(base + 0x1f0) += lastSmpl;
    SFLIB_UnlockCs(&cs);
    return 1;
}
// LLM-HARNESS-END: us-803c2790

// LLM-HARNESS-BEGIN: us-803c2840
void SFLIB_LockCs(void* cs);
void SFLIB_UnlockCs(void* cs);
s32 SFCON_ReadTotSmplQue(void* h, s32* lastSmpl, s32* value) {
    u32 cs;
    SFLIB_LockCs(&cs);
    u8* base = (u8*)h + 0xd98;
    s32 rd = *(s32*)(base + 0x1fc);
    s32 wr = *(s32*)(base + 0x1f8);
    if (wr - rd <= 0) {
        *lastSmpl = -1;
        SFLIB_UnlockCs(&cs);
        return 0;
    }
    *value = *(s32*)(base + 0x1ec);
    *lastSmpl = *(s32*)(base + 0x200 + (rd % 32) * 4);
    *(s32*)(base + 0x1fc) = rd + 1;
    SFLIB_UnlockCs(&cs);
    return 1;
}
// LLM-HARNESS-END: us-803c2840
