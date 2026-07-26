// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_sjd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADXSJD_Init() {}

void ADXSJD_Finish() {}

void ADXSJD_Create() {}

void ADXSJD_Destroy() {}

int ADXSJD_GetStat(void* self) { return (signed char)((u8*)self)[0]; }

void ADXB_SetAhxInSj(void* base);
void ADXSJD_SetInSj(void* self, void* sj) {
    *(void**)((u8*)self + 0x08) = sj;
    ADXB_SetAhxInSj(*(void**)((u8*)self + 0x04));
}

void ADXB_SetAhxDecSmpl(void* base, u32 val);
void ADXSJD_SetMaxDecSmpl(void* self, u32 val) {
    *(u32*)((u8*)self + 0x38) = val;
    ADXB_SetAhxDecSmpl(*(void**)((u8*)self + 0x04), val);
}

void ADXSJD_TermSupply(void) {}

void ADXSJD_Start() {}

void ADXSJD_Stop() {}

void adxsjd_decode_prep() {}

void adxsjd_get_wr() {}

void adxsjd_decexec_start() {}

void adxsjd_decexec_end() {}

void ADXSJD_ExecHndl() {}

void adxsjd_insert_proc() {}

void adxsjd_discard_proc() {}

void ADXSJD_ExecServer() {}

u32 ADXSJD_GetDecDtLen(void* self) { return *(u32*)((u8*)self + 0x30); }

u32 ADXSJD_GetDecNumSmpl(void* self) { return *(u32*)((u8*)self + 0x2c); }

void ADXSJD_SetDecPos(void* self, u32 val) { *(u32*)((u8*)self + 0x34) = val; }

void ADXB_SetLnkSw(void* base, u32 val);
void ADXSJD_SetLnkSw(void* self, u32 val) {
    *(u32*)((u8*)self + 0xa4) = val;
    ADXB_SetLnkSw(*(void**)((u8*)self + 0x04), val);
}

void ADXB_SetDefFmt();
void ADXSJD_SetDefFmt(void* self) {
    ADXB_SetDefFmt(*(void**)((u8*)self + 4));
}

void ADXSJD_EntryFltFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x50) = func;
    *(void**)((u8*)self + 0x54) = ctx;
}

void ADXSJD_EntryTrapFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x48) = func;
    *(void**)((u8*)self + 0x4c) = ctx;
}

void ADXSJD_SetTrapNumSmpl(void* self, u32 val) { *(u32*)((u8*)self + 0x3c) = val; }

void ADXSJD_SetTrapCnt(void* self, u32 val) { *(u32*)((u8*)self + 0x40) = val; }

void ADXSJD_SetTrapDtLen(void* self, u32 val) { *(u32*)((u8*)self + 0x44) = val; }

s16 ADXB_GetFormat(void* self);
s16 ADXSJD_GetFormat(void* self) {
    return ADXB_GetFormat(*(void**)((u8*)self + 4));
}

u32 ADXB_GetSfreq(void* self);
u32 ADXSJD_GetSfreq(void* self) {
    return ADXB_GetSfreq(*(void**)((u8*)self + 4));
}

void ADXB_GetNumChan();
void ADXSJD_GetNumChan(void* self) {
    ADXB_GetNumChan(*(void**)((u8*)self + 4));
}

void ADXB_GetOutBps();
void ADXSJD_GetOutBps(void* self) {
    ADXB_GetOutBps(*(void**)((u8*)self + 4));
}

u32 ADXB_GetBlkSmpl(void* self);
u32 ADXSJD_GetBlkSmpl(void* self) {
    return ADXB_GetBlkSmpl(*(void**)((u8*)self + 4));
}

u32 ADXB_GetTotalNumSmpl(void* self);
u32 ADXSJD_GetTotalNumSmpl(void* self) {
    return ADXB_GetTotalNumSmpl(*(void**)((u8*)self + 4));
}

s16 ADXB_GetNumLoop(void* self);
s16 ADXSJD_GetNumLoop(void* self) {
    return ADXB_GetNumLoop(*(void**)((u8*)self + 4));
}

u32 ADXB_GetLpStartPos(void* self);
u32 ADXSJD_GetLpStartPos(void* self) {
    return ADXB_GetLpStartPos(*(void**)((u8*)self + 4));
}

u32 ADXB_GetLpStartOfst(u32 a);
u32 ADXSJD_GetLpStartOfst(void* self) {
    if (self == NULL) return 0;
    return ADXB_GetLpStartOfst(*(u32*)((u8*)self + 4));
}

u32 ADXB_GetLpEndPos(void* self);
u32 ADXSJD_GetLpEndPos(void* self) {
    return ADXB_GetLpEndPos(*(void**)((u8*)self + 4));
}

u32 ADXB_GetLpEndOfst(void* self);
u32 ADXSJD_GetLpEndOfst(void* self) {
    return ADXB_GetLpEndOfst(*(void**)((u8*)self + 4));
}

void ADXSJD_GetDefOutVol() {}

void ADXSJD_GetDefPan() {}

void* ADXSJD_GetSpsdInfo(void* self) { return (void*)((u8*)self + 0x60); }

void ADXB_TakeSnapshot();
void ADXSJD_TakeSnapshot(void* self) {
    ADXB_TakeSnapshot(*(void**)((u8*)self + 4));
}

void ADXB_RestoreSnapshot();
void ADXSJD_RestoreSnapshot(void* self) {
    ADXB_RestoreSnapshot(*(void**)((u8*)self + 4));
}
