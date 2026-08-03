// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_dcd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SKG_GenerateKey() {}

void AHXSJD_SetupAtbl(u32 val) { AHXDCD_SetupAtbl(val); }

extern u32 lbl_eu_805E64D4;
void AHXSJD_SetupMtbl(u32 val) { lbl_eu_805E64D4 = val; }

void AHXSJD_SetupFtbl(u32 val) { AHXDCD_SetupFtbl(val); }

void AHXSJD_SetupWtbl(void) { AHXDCD_SetupWtbl(); }

extern s32 lbl_eu_805E64D8;
extern void AHXSBF_Finish(void);

void AHXDCD_Init(void) {
    if (lbl_eu_805E64D8 == 0) {
        AHXSBF_Init();
    }
    lbl_eu_805E64D8++;
}

void AHXDCD_Finish(void) {
    if (--lbl_eu_805E64D8 != 0) return;
    AHXSBF_Finish();
}

void AHXDCD_Create() {}

void AHXDCD_Destroy(void* p)
{
    void** sbf = (void**)((char*)p + 0x354);
    if (*sbf) {
        void* tmp = *sbf;
        *sbf = 0;
        AHXSBF_Destroy(tmp);
    }
    memset(p, 0, 0xbcc);
}

extern void AHXDCD_Reset(void* self);

void AHXDCD_SetBsr(void* self, void* bsr) {
    AHXDCD_Reset(self);
    *(void**)((u8*)self + 0x350) = bsr;
}

void AHXDCD_DecodeHeader() {}

void AHXDCD_DecodeFrmHdr() {}

typedef struct AHXDCDState {
    u8 _00[0x349];
    s8 outputReady;
    u8 _34A[2];
    u32 eofState;
    u8 _350[0x44];
    u32 totalNumSmpl;
} AHXDCDState;

u32 AHXDCD_IsEof(void* self) {
    return (((AHXDCDState*)self)->eofState == 0xc) ? 1 : 0;
}

extern void AHXDCD_GetSample_Dequantize_Denormalize(s32 a, s32* b, s32* c, s32* d, u32 e, s32 f, s32 g);
extern void AHXSBF_Synthesize(s32* base, s32 pcm, s32 idx, s32* out);

s32 AHXDCD_DecodeData(void* self, u8* out) {
    s32 i;
    if (*(s32*)((u8*)self + 844) == 12)
        return 0;
    {
        s32 nblk = *(s32*)((u8*)self + 844) >> 2;
        AHXDCD_GetSample_Dequantize_Denormalize(
            *(s32*)((u8*)self + 848), (s32*)((u8*)self + 904), (s32*)((u8*)self + 964),
            (s32*)((u8*)self + 1476), nblk,
            *(s32*)((u8*)self + 0), *(s32*)((u8*)self + 836));
    }
    for (i = 0; i < 3; i++) {
        AHXSBF_Synthesize(*(s32**)((u8*)self + 852), *(s32*)((u8*)self + 0) + i * 128, 0, (s32*)(out + i * 64));
    }
    *(s32*)((u8*)self + 844) += 1;
    return 96;
}

s16 AHXDCD_GetOutBps(void* self) {
    return ((AHXDCDState*)self)->outputReady;
}

int AHXDCD_GetOutSmpl(void) { return 0x60; }

u32 AHXDCD_GetTotalNumSmpl(void* self) {
    AHXDCDState* state = (AHXDCDState*)self;
    if (state->outputReady == 0) return 0;
    return state->totalNumSmpl;
}

void* memcpy(void* dest, const void* src, size_t n);
void AHXDCD_SetExtPrm(void* self, void* prm) {
    memcpy((u8*)self + 0xbc4, prm, 8);
}

void AHXDCD_DecodeBhdr() {}

void AHXDCD_BhdrToDinf() {}

void AHXDCD_DecodeBitalloc2() {}

void AHXDCD_DecodeScale2() {}

