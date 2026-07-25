// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_buf
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803c1500
void SFBUF_Init() {}
// LLM-HARNESS-END: us-803c1500

// LLM-HARNESS-BEGIN: us-803c15a0
void SFBUF_InitHn() {}
// LLM-HARNESS-END: us-803c15a0

// LLM-HARNESS-BEGIN: us-803c176c
void sfbuf_InitRingSj() {}
// LLM-HARNESS-END: us-803c176c

// LLM-HARNESS-BEGIN: us-803c18d0
void SFBUF_DestroySj() {}
// LLM-HARNESS-END: us-803c18d0

// LLM-HARNESS-BEGIN: us-803c199c
void sfbuf_InitVfrmBuf() {}
// LLM-HARNESS-END: us-803c199c

// LLM-HARNESS-BEGIN: us-803c1a7c
void sfbuf_InitAringBuf() {}
// LLM-HARNESS-END: us-803c1a7c

// LLM-HARNESS-BEGIN: us-803c1af8
void SFBUF_SetSupplySj() {}
// LLM-HARNESS-END: us-803c1af8

// LLM-HARNESS-BEGIN: us-803c1cac
void fn_803C1CAC() {}
// LLM-HARNESS-END: us-803c1cac

// LLM-HARNESS-BEGIN: us-803c1d50
void SFBUF_SetUoch() {}
// LLM-HARNESS-END: us-803c1d50

// LLM-HARNESS-BEGIN: us-803c1d84
void SFBUF_GetUoch() {}
// LLM-HARNESS-END: us-803c1d84

// LLM-HARNESS-BEGIN: us-803c1db8
void SFBUF_RingGetWrite(void) {}
// LLM-HARNESS-END: us-803c1db8

// LLM-HARNESS-BEGIN: us-803c1dc0
void SFBUF_RingGetRead(void) {}
// LLM-HARNESS-END: us-803c1dc0

// LLM-HARNESS-BEGIN: us-803c1dc8
void sfbuf_RingGetSub() {}
// LLM-HARNESS-END: us-803c1dc8

// LLM-HARNESS-BEGIN: us-803c1f10
void sfbuf_RingAddSub(void* a, void* b, void* c, int mode);
void SFBUF_RingAddWrite(void* a, void* b, void* c) {
    sfbuf_RingAddSub(a, b, c, 0);
}
// LLM-HARNESS-END: us-803c1f10

// LLM-HARNESS-BEGIN: us-803c1f18
void SFBUF_RingAddRead(void* a, void* b, void* c) {
    sfbuf_RingAddSub(a, b, c, 1);
}
// LLM-HARNESS-END: us-803c1f18

// LLM-HARNESS-BEGIN: us-803c1f20
void sfbuf_RingAddSub(void* a, void* b, void* c, int mode) {}
// LLM-HARNESS-END: us-803c1f20

// LLM-HARNESS-BEGIN: us-803c2198
void SFBUF_RingGetDlm() {}
// LLM-HARNESS-END: us-803c2198

// LLM-HARNESS-BEGIN: us-803c21fc
extern void SFLIB_LockCs(void *);
extern void SFLIB_UnlockCs(void *);

void SFBUF_RingSetDlm(char *buf, int idx, void *dlm, int size) {
    void *cs;
    char *p = buf + idx * 0x74;
    SFLIB_LockCs(&cs);
    *(void **)(p + 0x13e0) = dlm;
    *(int *)(p + 0x13e4) = size;
    SFLIB_UnlockCs(&cs);
}
// LLM-HARNESS-END: us-803c21fc

// LLM-HARNESS-BEGIN: us-803c2258
u32 SFBUF_GetRingBufSiz(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0x13d4 + idx * 0x74);
}
// LLM-HARNESS-END: us-803c2258

// LLM-HARNESS-BEGIN: us-803c2268
u32 SFBUF_GetRTot(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0x13ec + idx * 0x74);
}
// LLM-HARNESS-END: us-803c2268

// LLM-HARNESS-BEGIN: us-803c2278
void SFBUF_GetWTot() {}
// LLM-HARNESS-END: us-803c2278

// LLM-HARNESS-BEGIN: us-803c230c
void SFBUF_RingGetSj() {}
// LLM-HARNESS-END: us-803c230c

// LLM-HARNESS-BEGIN: us-803c2344
void SFBUF_AddRtotSj(void* self, int idx, int addend) {
    u32* ptr = (u32*)((u8*)self + idx * 0x74 + 0x13ec);
    if ((s32)*ptr < 0) return;
    *ptr += addend;
}
// LLM-HARNESS-END: us-803c2344

// LLM-HARNESS-BEGIN: us-803c2364
void SFBUF_VfrmGetRead() {}
// LLM-HARNESS-END: us-803c2364

// LLM-HARNESS-BEGIN: us-803c2394
void SFBUF_VfrmAddRead() {}
// LLM-HARNESS-END: us-803c2394

// LLM-HARNESS-BEGIN: us-803c23f8
void SFBUF_SetPrepFlg(void* self, u32 idx, u32 val) {
    if (idx != 8) {
        *(u32*)((u8*)self + 0x13c0 + idx * 0x74) = val;
    }
}
// LLM-HARNESS-END: us-803c23f8

// LLM-HARNESS-BEGIN: us-803c2410
int SFBUF_GetPrepFlg(void* self, int idx) {
    if (idx == 8) return 0;
    return *(u32*)((u8*)self + idx * 0x74 + 0x13c0);
}
// LLM-HARNESS-END: us-803c2410

// LLM-HARNESS-BEGIN: us-803c2430
void SFBUF_SetTermFlg(void* buf, s32 idx, u32 flg) {
    if (idx == 8) return;
    *(u32*)((u8*)buf + idx * 0x74 + 0x13c4) = flg;
}
// LLM-HARNESS-END: us-803c2430

// LLM-HARNESS-BEGIN: us-803c2448
int SFBUF_GetTermFlg(void* self, int idx) {
    if (idx == 8) return 1;
    return *(u32*)((u8*)self + idx * 0x74 + 0x13c4);
}
// LLM-HARNESS-END: us-803c2448

// LLM-HARNESS-BEGIN: us-803c2468
void SFBUF_RingGetDataSiz() {}
// LLM-HARNESS-END: us-803c2468

// LLM-HARNESS-BEGIN: us-803c249c
void SFBUF_GetFlowCnt() {}
// LLM-HARNESS-END: us-803c249c

// LLM-HARNESS-BEGIN: us-803c259c
int SFBUF_UpdateFlowCnt(int count, int new_val, int old_val) {
    u32 diff = new_val ^ old_val;
    u32 leading = __cntlzw(diff);
    u32 shifted = new_val << leading;
    int bit = shifted >> 31;
    return count + bit;
}
// LLM-HARNESS-END: us-803c259c
