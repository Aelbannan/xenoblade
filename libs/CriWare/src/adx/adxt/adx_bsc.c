// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bsc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SKG_GenerateKey() {}

extern u32 lbl_eu_805E5370;

u32 ADXB_GetDecErrMode(void) { return lbl_eu_805E5370; }

void ADXB_Init() {}

u32 adxb_DefGetWr(void* self, u32* out1, u32* out2, u32* out3) {
    *out1 = *(u32*)((u8*)self + 0x8C);
    *out2 = *(u32*)((u8*)self + 0x40) - *(u32*)((u8*)self + 0x8C);
    *out3 = *(u32*)((u8*)self + 0x18) - *(u32*)((u8*)self + 0x88);
    return *(u32*)((u8*)self + 0x3C);
}

void adxb_DefAddWr(void* self, u32 unused, u32 addend) {
    *(u32*)((u8*)self + 0x8c) += addend;
    *(u32*)((u8*)self + 0x88) += addend;
}

void ADXB_Create() {}

void ADXB_Destroy() {}

void ADXB_DecodeHeaderAdx() {}

void ADXB_SetDefFmt(void* self) {
    *(s16*)((u8*)self + 0x98) = 0;
}

void criware_eu_8038A864(void) {}

void ADXB_SetDefPrm() {}

void ADXB_DecodeHeader() {}

void ADXB_EntryGetWrFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x78) = func;
    *(void**)((u8*)self + 0x7c) = ctx;
}

u32 ADXB_GetPcmBuf(void* self) { return *(u32*)((u8*)self + 0x3c); }

s16 ADXB_GetFormat(void* self) { return *(s16*)((u8*)self + 0x98); }

u32 ADXB_GetSfreq(void* self) { return *(u32*)((u8*)self + 0x14); }

void ADXB_GetNumChan() {}

typedef struct ADXBFormatInfo {
    u8 _00[0x0d];
    s8 formatBitsPerSample;
} ADXBFormatInfo;

s16 ADXB_GetFmtBps(void* self) {
    return ((ADXBFormatInfo*)self)->formatBitsPerSample;
}

void ADXB_GetOutBps() {}

u32 ADXB_GetBlkSmpl(void* self) { return *(u32*)((u8*)self + 0x10); }

u32 ADXB_GetTotalNumSmpl(void* self) { return *(u32*)((u8*)self + 0x18); }

s16 ADXB_GetNumLoop(void* self) { return *(s16*)((u8*)self + 0x24); }

u32 ADXB_GetLpStartPos(void* self) { return *(u32*)((u8*)self + 0x28); }

u32 ADXB_GetLpStartOfst(void* self) {
    if (self == NULL) return 0;
    return *(u32*)((u8*)self + 0x2c);
}

u32 ADXB_GetLpEndPos(void* self) { return *(u32*)((u8*)self + 0x30); }

u32 ADXB_GetLpEndOfst(void* self) { return *(u32*)((u8*)self + 0x34); }

u32 ADXB_GetAinfLen(void* self) { return *(u32*)((u8*)self + 0xc4); }

s16 ADXB_GetDefOutVol(void* self) { return *(s16*)((u8*)self + 0xd8); }

s16 ADXB_GetDefPan(void* self, u32 idx) {
    return *(s16*)((u8*)self + 0xda + idx * 2);
}

void ADXB_TakeSnapshot(void* this_) {
    void* r31 = this_;
    ADXPD_GetDly(*(void**)((char*)r31 + 8), (char*)r31 + 0xb0, (char*)r31 + 0xb4);
    ADXPD_GetExtPrm(*(void**)((char*)r31 + 8), (char*)r31 + 0xa6, (char*)r31 + 0xa8, (char*)r31 + 0xaa);
}

void ADXB_RestoreSnapshot() {}

void adxb_get_key() {}

void ADXB_SetLnkSw(void* self, int val) {
    if (val == 0) {
        *(u32*)((u8*)self + 0xAC) = 0;
    } else {
        *(u32*)((u8*)self + 0xAC) = 1;
    }
}

u32 ADXB_GetStat(void* self) { return *(u32*)((u8*)self + 0x4); }

void ADXB_EntryData() {}

void ADXB_Start(void* self) {
    if (*(u32*)((u8*)self + 0x04) != 0) return;
    *(u32*)((u8*)self + 0x04) = 1;
}

void ADXB_Stop() {}

void ADXB_Reset() {}

u32 ADXB_GetDecDtLen(void* self) { return *(u32*)((u8*)self + 0x94); }

u32 ADXB_GetDecNumSmpl(void* self) { return *(u32*)((u8*)self + 0x90); }

void ADXB_EvokeDecode() {}

void ADXB_EndDecode() {}

void ADXB_ExecOneAdx() {}

void ADXB_ExecHndl() {}
