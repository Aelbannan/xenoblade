// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bsc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SKG_GenerateKey() {}

extern u32 lbl_eu_805E5370;

u32 ADXB_GetDecErrMode(void) { return lbl_eu_805E5370; }

extern volatile u32 lbl_eu_805E5358;
extern u8 lbl_eu_805E5378[0x1000];

void ADXB_Init(void) {
    u8* p = (u8*)&lbl_eu_805E5358;
    ADXPD_Init();
    (*(volatile u32*)p)++;
    memset(p + 0x20, 0, 0x1000);
    *(u32*)(p + 0x18) = 0;
}

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

void ADXB_SetDefPrm(void* self);
void criware_eu_8038A864(void* self) {
    *(s16*)((u8*)self + 0x98) = *(s16*)((u8*)self + 0x9a);
    ADXB_SetDefPrm(self);
}

void ADXB_SetDefPrm(void* self) {
    u8* p = (u8*)self;
    u32 v3c = *(u32*)(p + 0x3c);
    u32 v40 = *(u32*)(p + 0x40);
    u32 v44 = *(u32*)(p + 0x44);

    *(u16*)(p + 0x02) = 1;
    *(u32*)(p + 0x14) = 48000;
    *(u8*)(p + 0x0e) = 2;
    *(u8*)(p + 0x0d) = 16;
    *(u32*)(p + 0x18) = 0x7fffffff;
    *(u8*)(p + 0x0f) = 0x7f;
    *(u32*)(p + 0x10) = 0x400;
    *(u32*)(p + 0x50) = 2;
    *(u32*)(p + 0x54) = 0x7f;
    *(u32*)(p + 0x58) = 0x400;
    *(u32*)(p + 0x5c) = v3c;
    *(u32*)(p + 0x60) = v40;
    *(u32*)(p + 0x64) = v44;
    *(u32*)(p + 0x8c) = 0;
    *(u16*)(p + 0x1c) = 0;
    *(u16*)(p + 0x24) = 0;
    *(u16*)(p + 0x26) = 0;
    *(u32*)(p + 0x20) = 0;
    *(u32*)(p + 0x28) = 0;
    *(u32*)(p + 0x2c) = 0;
    *(u32*)(p + 0x30) = 0;
    *(u32*)(p + 0x34) = 0;
    *(u32*)(p + 0x88) = 0;
    *(u8*)(p + 0xec) = 0;
    *(u8*)(p + 0xed) = 0;
}

void ADXB_DecodeHeader() {}

void ADXB_EntryGetWrFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x78) = func;
    *(void**)((u8*)self + 0x7c) = ctx;
}

u32 ADXB_GetPcmBuf(void* self) { return *(u32*)((u8*)self + 0x3c); }

s16 ADXB_GetFormat(void* self) { return *(s16*)((u8*)self + 0x98); }

u32 ADXB_GetSfreq(void* self) { return *(u32*)((u8*)self + 0x14); }

extern char lbl_eu_80517330[];
extern void ADXERR_CallErrFunc1_(const char* msg);

s32 ADXB_GetNumChan(void* self) {
    int ch = *(u8*)((u8*)self + 0x0E);
    if (ch == 1 && *(u32*)((u8*)self + 0xE0) != 0)
        return 2;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_80517330 + 0x81);
        return -1;
    }
    return (s8)ch;
}

typedef struct ADXBFormatInfo {
    u8 _00[0x0d];
    s8 formatBitsPerSample;
} ADXBFormatInfo;

s16 ADXB_GetFmtBps(void* self) {
    return ((ADXBFormatInfo*)self)->formatBitsPerSample;
}

s32 ADXB_GetOutBps(void* self) {
    s16 val = *(s16*)((u8*)self + 0x98);
    if (val == 0) return 0x10;
    if (val == 2) {
        s16 sub = *(s16*)((u8*)self + 0x9c);
        if (sub == 2) return 4;
        if (sub == 1) return 8;
        return 0x10;
    }
    if (val == 1) {
        s16 sub = *(s16*)((u8*)self + 0x9c);
        if (sub == 2) return 4;
        return 0x10;
    }
    return 0x10;
}

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

void ADXB_RestoreSnapshot(void* self) {
    ADXPD_SetDly(*(void**)((u8*)self + 0x08), (u32*)((u8*)self + 0xB0), (u32*)((u8*)self + 0xB4));
    ADXPD_SetExtPrm(*(void**)((u8*)self + 0x08),
        *(s16*)((u8*)self + 0xA6),
        *(s16*)((u8*)self + 0xA8),
        *(s16*)((u8*)self + 0xAA));
}

void adxb_get_key() {}

void ADXB_SetLnkSw(void* self, int val) {
    if (val == 0) {
        *(u32*)((u8*)self + 0xAC) = 0;
    } else {
        *(u32*)((u8*)self + 0xAC) = 1;
    }
}

u32 ADXB_GetStat(void* self) { return *(u32*)((u8*)self + 0x4); }

void ADXB_EntryData(void* self, void* data, int size) {
    if (*(s16*)((u8*)self + 0x98) == 0) {
        *(void**)((u8*)self + 0x48) = data;
        int ch = (s8)*(u8*)((u8*)self + 0x0F);
        *(u32*)((u8*)self + 0x74) = 0;
        *(u32*)((u8*)self + 0x4C) = size / ch;
    } else {
        *(void**)((u8*)self + 0x48) = data;
        int t = (s8)*(u8*)((u8*)self + 0x0D) / 8;
        *(u32*)((u8*)self + 0x74) = 0;
        *(u32*)((u8*)self + 0x4C) = size / (t * (s8)*(u8*)((u8*)self + 0x0E));
    }
    *(u32*)((u8*)self + 0x90) = 0;
    *(u32*)((u8*)self + 0x94) = 0;
    *(u32*)((u8*)self + 0xF4) = 0;
    *(u32*)((u8*)self + 0xF0) = 0;
}

void ADXB_Start(void* self) {
    if (*(u32*)((u8*)self + 0x04) != 0) return;
    *(u32*)((u8*)self + 0x04) = 1;
}

extern void (*lbl_eu_805E5364)(void*);

void ADXB_Stop(void* self) {
    if (*(u32*)((u8*)self + 0xE0) != 0) {
        lbl_eu_805E5364(self);
    }
    ADXPD_Stop(*(void**)((u8*)self + 0x08));
    *(u32*)((u8*)self + 0x04) = 0;
}

void ADXB_Reset(void* self) {
    if (*(u32*)((u8*)self + 0x04) == 3) {
        ADXPD_Reset(*(void**)((u8*)self + 0x08));
        *(u32*)((u8*)self + 0x8C) = 0;
        *(u32*)((u8*)self + 0x04) = 0;
    }
}

u32 ADXB_GetDecDtLen(void* self) { return *(u32*)((u8*)self + 0x94); }

u32 ADXB_GetDecNumSmpl(void* self) { return *(u32*)((u8*)self + 0x90); }

void ADXB_EvokeDecode() {}

void ADXB_EndDecode() {}

void ADXB_ExecOneAdx() {}

void ADXB_ExecHndl() {}
