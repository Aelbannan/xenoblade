// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_sjd
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803827c0
void ADXSJD_Init() {}
// LLM-HARNESS-END: us-803827c0

// LLM-HARNESS-BEGIN: us-80382814
void ADXSJD_Finish() {}
// LLM-HARNESS-END: us-80382814

// LLM-HARNESS-BEGIN: us-80382848
void ADXSJD_Create() {}
// LLM-HARNESS-END: us-80382848

// LLM-HARNESS-BEGIN: us-80382ae0
void ADXSJD_Destroy() {}
// LLM-HARNESS-END: us-80382ae0

// LLM-HARNESS-BEGIN: us-80382b44
int ADXSJD_GetStat(void* self) { return (signed char)((u8*)self)[0]; }
// LLM-HARNESS-END: us-80382b44

// LLM-HARNESS-BEGIN: us-80382b50
void ADXB_SetAhxInSj(void* base);
void ADXSJD_SetInSj(void* self, void* sj) {
    *(void**)((u8*)self + 0x08) = sj;
    ADXB_SetAhxInSj(*(void**)((u8*)self + 0x04));
}
// LLM-HARNESS-END: us-80382b50

// LLM-HARNESS-BEGIN: us-80382b5c
void ADXB_SetAhxDecSmpl(void* base, u32 val);
void ADXSJD_SetMaxDecSmpl(void* self, u32 val) {
    *(u32*)((u8*)self + 0x38) = val;
    ADXB_SetAhxDecSmpl(*(void**)((u8*)self + 0x04), val);
}
// LLM-HARNESS-END: us-80382b5c

// LLM-HARNESS-BEGIN: us-80382b68
void ADXSJD_TermSupply(void) {}
// LLM-HARNESS-END: us-80382b68

// LLM-HARNESS-BEGIN: us-80382b70
void ADXSJD_Start() {}
// LLM-HARNESS-END: us-80382b70

// LLM-HARNESS-BEGIN: us-80382bb8
void ADXSJD_Stop() {}
// LLM-HARNESS-END: us-80382bb8

// LLM-HARNESS-BEGIN: us-80382bf0
void adxsjd_decode_prep() {}
// LLM-HARNESS-END: us-80382bf0

// LLM-HARNESS-BEGIN: us-80382f3c
void adxsjd_get_wr() {}
// LLM-HARNESS-END: us-80382f3c

// LLM-HARNESS-BEGIN: us-80383034
void adxsjd_decexec_start() {}
// LLM-HARNESS-END: us-80383034

// LLM-HARNESS-BEGIN: us-80383458
void adxsjd_decexec_end() {}
// LLM-HARNESS-END: us-80383458

// LLM-HARNESS-BEGIN: us-80383610
void ADXSJD_ExecHndl() {}
// LLM-HARNESS-END: us-80383610

// LLM-HARNESS-BEGIN: us-80383740
void adxsjd_insert_proc() {}
// LLM-HARNESS-END: us-80383740

// LLM-HARNESS-BEGIN: us-80383878
void adxsjd_discard_proc() {}
// LLM-HARNESS-END: us-80383878

// LLM-HARNESS-BEGIN: us-803839a0
void ADXSJD_ExecServer() {}
// LLM-HARNESS-END: us-803839a0

// LLM-HARNESS-BEGIN: us-80383b20
u32 ADXSJD_GetDecDtLen(void* self) { return *(u32*)((u8*)self + 0x30); }
// LLM-HARNESS-END: us-80383b20

// LLM-HARNESS-BEGIN: us-80383b28
u32 ADXSJD_GetDecNumSmpl(void* self) { return *(u32*)((u8*)self + 0x2c); }
// LLM-HARNESS-END: us-80383b28

// LLM-HARNESS-BEGIN: us-80383b30
void ADXSJD_SetDecPos(void* self, u32 val) { *(u32*)((u8*)self + 0x34) = val; }
// LLM-HARNESS-END: us-80383b30

// LLM-HARNESS-BEGIN: us-80383b38
void ADXB_SetLnkSw(void* base, u32 val);
void ADXSJD_SetLnkSw(void* self, u32 val) {
    *(u32*)((u8*)self + 0xa4) = val;
    ADXB_SetLnkSw(*(void**)((u8*)self + 0x04), val);
}
// LLM-HARNESS-END: us-80383b38

// LLM-HARNESS-BEGIN: us-80383b44
void ADXB_SetDefFmt();
void ADXSJD_SetDefFmt(void* self) {
    ADXB_SetDefFmt(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383b44

// LLM-HARNESS-BEGIN: us-80383b4c
void ADXSJD_EntryFltFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x50) = func;
    *(void**)((u8*)self + 0x54) = ctx;
}
// LLM-HARNESS-END: us-80383b4c

// LLM-HARNESS-BEGIN: us-80383b58
void ADXSJD_EntryTrapFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x48) = func;
    *(void**)((u8*)self + 0x4c) = ctx;
}
// LLM-HARNESS-END: us-80383b58

// LLM-HARNESS-BEGIN: us-80383b64
void ADXSJD_SetTrapNumSmpl(void* self, u32 val) { *(u32*)((u8*)self + 0x3c) = val; }
// LLM-HARNESS-END: us-80383b64

// LLM-HARNESS-BEGIN: us-80383b6c
void ADXSJD_SetTrapCnt(void* self, u32 val) { *(u32*)((u8*)self + 0x40) = val; }
// LLM-HARNESS-END: us-80383b6c

// LLM-HARNESS-BEGIN: us-80383b74
void ADXSJD_SetTrapDtLen(void* self, u32 val) { *(u32*)((u8*)self + 0x44) = val; }
// LLM-HARNESS-END: us-80383b74

// LLM-HARNESS-BEGIN: us-80383b7c
s16 ADXB_GetFormat(void* self);
s16 ADXSJD_GetFormat(void* self) {
    return ADXB_GetFormat(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383b7c

// LLM-HARNESS-BEGIN: us-80383b84
u32 ADXB_GetSfreq(void* self);
u32 ADXSJD_GetSfreq(void* self) {
    return ADXB_GetSfreq(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383b84

// LLM-HARNESS-BEGIN: us-80383b8c
void ADXB_GetNumChan();
void ADXSJD_GetNumChan(void* self) {
    ADXB_GetNumChan(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383b8c

// LLM-HARNESS-BEGIN: us-80383b94
void ADXB_GetOutBps();
void ADXSJD_GetOutBps(void* self) {
    ADXB_GetOutBps(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383b94

// LLM-HARNESS-BEGIN: us-80383b9c
u32 ADXB_GetBlkSmpl(void* self);
u32 ADXSJD_GetBlkSmpl(void* self) {
    return ADXB_GetBlkSmpl(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383b9c

// LLM-HARNESS-BEGIN: us-80383ba4
u32 ADXB_GetTotalNumSmpl(void* self);
u32 ADXSJD_GetTotalNumSmpl(void* self) {
    return ADXB_GetTotalNumSmpl(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383ba4

// LLM-HARNESS-BEGIN: us-80383bac
s16 ADXB_GetNumLoop(void* self);
s16 ADXSJD_GetNumLoop(void* self) {
    return ADXB_GetNumLoop(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383bac

// LLM-HARNESS-BEGIN: us-80383bb4
u32 ADXB_GetLpStartPos(void* self);
u32 ADXSJD_GetLpStartPos(void* self) {
    return ADXB_GetLpStartPos(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383bb4

// LLM-HARNESS-BEGIN: us-80383bbc
u32 ADXB_GetLpStartOfst(u32 a);
u32 ADXSJD_GetLpStartOfst(void* self) {
    if (self == NULL) return 0;
    return ADXB_GetLpStartOfst(*(u32*)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383bbc

// LLM-HARNESS-BEGIN: us-80383bd8
u32 ADXB_GetLpEndPos(void* self);
u32 ADXSJD_GetLpEndPos(void* self) {
    return ADXB_GetLpEndPos(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383bd8

// LLM-HARNESS-BEGIN: us-80383be0
u32 ADXB_GetLpEndOfst(void* self);
u32 ADXSJD_GetLpEndOfst(void* self) {
    return ADXB_GetLpEndOfst(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383be0

// LLM-HARNESS-BEGIN: us-80383be8
void ADXSJD_GetDefOutVol() {}
// LLM-HARNESS-END: us-80383be8

// LLM-HARNESS-BEGIN: us-80383c44
void ADXSJD_GetDefPan() {}
// LLM-HARNESS-END: us-80383c44

// LLM-HARNESS-BEGIN: us-80383cb0
void* ADXSJD_GetSpsdInfo(void* self) { return (void*)((u8*)self + 0x60); }
// LLM-HARNESS-END: us-80383cb0

// LLM-HARNESS-BEGIN: us-80383cb8
void ADXB_TakeSnapshot();
void ADXSJD_TakeSnapshot(void* self) {
    ADXB_TakeSnapshot(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383cb8

// LLM-HARNESS-BEGIN: us-80383cc0
void ADXB_RestoreSnapshot();
void ADXSJD_RestoreSnapshot(void* self) {
    ADXB_RestoreSnapshot(*(void**)((u8*)self + 4));
}
// LLM-HARNESS-END: us-80383cc0
