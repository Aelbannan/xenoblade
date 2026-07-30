// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_buf
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFBUF_Init() {}

void SFBUF_InitHn() {}

void sfbuf_InitRingSj() {}

void SFBUF_DestroySj() {}

void sfbuf_InitVfrmBuf() {}

void sfbuf_InitAringBuf() {}

void SFBUF_SetSupplySj() {}

void fn_803C1CAC() {}

void SFBUF_SetUoch(void* self, int idx, int sub_idx, u32* src) {
    u32 s1 = src[1];
    u32 s0 = src[0];
    u8* dst = (u8*)self + idx * 0x74 + (sub_idx << 4) + 0x13c8;
    *(u32*)(dst + 0) = s0;
    *(u32*)(dst + 4) = s1;
    u32 s3 = src[3];
    u32 s2 = src[2];
    *(u32*)(dst + 8) = s2;
    *(u32*)(dst + 12) = s3;
}

void SFBUF_GetUoch(void* self, int idx, int sub_idx, u32* dst) {
    u32* src = (u32*)((u8*)self + idx * 0x74 + (sub_idx << 4) + 0x13c8);
    u32 s0 = src[0];
    u32 s1 = src[1];
    dst[1] = s1;
    dst[0] = s0;
    u32 s2 = src[2];
    u32 s3 = src[3];
    dst[3] = s3;
    dst[2] = s2;
}

void sfbuf_RingGetSub(void* self, void* a, void* b, int mode);
void SFBUF_RingGetWrite(void* self, void* a, void* b) { sfbuf_RingGetSub(self, a, b, 0); }

void SFBUF_RingGetRead(void* self, void* a, void* b) { sfbuf_RingGetSub(self, a, b, 1); }

void sfbuf_RingGetSub(void* self, void* a, void* b, int mode) { (void)self; (void)a; (void)b; (void)mode; }

void sfbuf_RingAddSub(void* a, void* b, void* c, int mode);
void SFBUF_RingAddWrite(void* a, void* b, void* c) {
    sfbuf_RingAddSub(a, b, c, 0);
}

void SFBUF_RingAddRead(void* a, void* b, void* c) {
    sfbuf_RingAddSub(a, b, c, 1);
}

void sfbuf_RingAddSub(void* a, void* b, void* c, int mode) {}

void SFBUF_RingGetDlm() {}

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

u32 SFBUF_GetRingBufSiz(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0x13d4 + idx * 0x74);
}

u32 SFBUF_GetRTot(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0x13ec + idx * 0x74);
}

void SFBUF_GetWTot() {}

void SFBUF_RingGetSj() {}

void SFBUF_AddRtotSj(void* self, int idx, int addend) {
    u32* ptr = (u32*)((u8*)self + idx * 0x74 + 0x13ec);
    if ((s32)*ptr < 0) return;
    *ptr += addend;
}

extern int SFTRN_CallTrtTrif(void *, int, int, int *, int);

int SFBUF_VfrmGetRead(void* self, int idx, int* a, int b) {
    u8* p = (u8*)self + idx * 0x74;
    if (*(u32*)(p + 0x13bc) == 0) {
        return SFTRN_CallTrtTrif(self, *(int*)(p + 0x1404), 0xb, a, b);
    }
    return 0;
}

void SFBUF_VfrmAddRead() {}

void SFBUF_SetPrepFlg(void* self, u32 idx, u32 val) {
    if (idx != 8) {
        *(u32*)((u8*)self + 0x13c0 + idx * 0x74) = val;
    }
}

int SFBUF_GetPrepFlg(void* self, int idx) {
    if (idx == 8) return 0;
    return *(u32*)((u8*)self + idx * 0x74 + 0x13c0);
}

void SFBUF_SetTermFlg(void* buf, s32 idx, u32 flg) {
    if (idx == 8) return;
    *(u32*)((u8*)buf + idx * 0x74 + 0x13c4) = flg;
}

int SFBUF_GetTermFlg(void* self, int idx) {
    if (idx == 8) return 1;
    return *(u32*)((u8*)self + idx * 0x74 + 0x13c4);
}

void SFBUF_RingGetDataSiz() {}

void SFBUF_GetFlowCnt() {}

int SFBUF_UpdateFlowCnt(int count, int new_val, int old_val) {
    u32 diff = new_val ^ old_val;
    u32 leading = __cntlzw(diff);
    u32 shifted = new_val << leading;
    u32 bit = shifted >> 31;
    return count + (int)bit;
}
