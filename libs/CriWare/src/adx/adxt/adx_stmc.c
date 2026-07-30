// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_stmc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern volatile s32 lbl_eu_805E3E98;
extern u8 lbl_eu_805E3EA8[0x1040];

s32 ADXSTM_Init(void) {
    if (++lbl_eu_805E3E98 == 1) {
        memset(lbl_eu_805E3EA8, 0, sizeof(lbl_eu_805E3EA8));
    }
    return 1;
}

void ADXSTM_Finish(void) {
    if (--lbl_eu_805E3E98 != 0) return;
    memset(lbl_eu_805E3EA8, 0, sizeof(lbl_eu_805E3EA8));
}

void ADXSTMF_SetupHandleMember() {}

void* ADXSTM_Create(void* a, int b) { return NULL; }

void ADXSTM_Destroy() {}

void ADXSTM_BindFileNw() {}

void ADXSTM_ReleaseFileNw() {}

void ADXSTM_ReleaseFile() {}

int ADXSTM_IsOpened(const void* self) {
    const signed char* b = (const signed char*)self;
    if (b[0x4a] != 0) return 1;
    if (b[0x49] != 0) return 0;
    return b[0x4d] != 0 ? 1 : 0;
}

s32 ADXSTM_IsOpenReq(void* self) {
    return (*(s8*)((u8*)self + 0x49) || *(s8*)((u8*)self + 0x4D)) ? 1 : 0;
}

s32 ADXSTM_GetStat(void* self) {
    ADXCRS_Enter();
    s32 result = *(s8*)((u8*)self + 1);
    ADXCRS_Leave();
    return result;
}

int ADXSTM_Seek(void* this_, int pos)
{
    int* pThis = (int*)this_;
    ADXCRS_Enter();
    int max = pThis[6];
    pThis[23] = pos;
    if (pos > max)
        pThis[23] = max;
    int result = pThis[23];
    ADXCRS_Leave();
    return result;
}

s32 ADXSTM_Tell(void* self) {
    ADXCRS_Enter();
    s32 result;
    if (*(u32*)((u8*)self + 8) != 0) {
        result = *(u32*)((u8*)self + 0x5C);
    } else {
        result = 0;
    }
    ADXCRS_Leave();
    return result;
}

s32 ADXSTM_Start(void* self) {
    ADXCRS_Enter();
    ADXCRS_Lock();
    *(u32*)((u8*)self + 0x38) = 0;
    *(u32*)((u8*)self + 0x50) = 0;
    if (*(u32*)((u8*)self + 0x18) == 0) {
        *(u8*)((u8*)self + 1) = 3;
    } else {
        *(u8*)((u8*)self + 1) = 2;
    }
    *(u8*)((u8*)self + 2) = 0;
    *(u32*)((u8*)self + 0x28) = 0;
    *(u32*)((u8*)self + 0x2C) = 0;
    *(u8*)((u8*)self + 0x4B) = 1;
    *(u32*)((u8*)self + 0x60) = 0xFFFFF;
    ADXCRS_Unlock();
    ADXCRS_Leave();
    return 1;
}

s32 ADXSTM_Start2(void* self, u32 param) {
    ADXCRS_Enter();
    ADXCRS_Lock();
    *(u32*)((u8*)self + 0x38) = 0;
    *(u32*)((u8*)self + 0x50) = 0;
    if (*(u32*)((u8*)self + 0x18) == 0) {
        *(u8*)((u8*)self + 1) = 3;
    } else {
        *(u8*)((u8*)self + 1) = 2;
    }
    *(u8*)((u8*)self + 2) = 0;
    *(u32*)((u8*)self + 0x28) = 0;
    *(u32*)((u8*)self + 0x2C) = 0;
    *(u8*)((u8*)self + 0x4B) = 1;
    *(u32*)((u8*)self + 0x60) = param;
    ADXCRS_Unlock();
    ADXCRS_Leave();
    return 1;
}

void ADXSTM_StopNw() {}

void ADXSTM_Stop() {}

void ADXSTM_EntryEosFunc(void* self, void* func, void* ctx) {
    ADXCRS_Enter();
    *(void**)((u8*)self + 0x3C) = func;
    *(void**)((u8*)self + 0x40) = ctx;
    ADXCRS_Leave();
}

void ADXSTM_SetEos(void* self, s32 val) {
    ADXCRS_Enter();
    if (val >= 0) {
        *(s32*)((u8*)self + 0x34) = val;
    } else {
        *(s32*)((u8*)self + 0x34) = *(s32*)((u8*)self + 0x18);
    }
    ADXCRS_Leave();
}

void adxstmf_stat_exec() {}

void ADXSTMF_ExecHndl() {}

void ADXSTM_ExecServer() {}

void ADXSTM_ExecFsSvr(void) {
    ADXCRS_Enter();
    cvFsExecServer();
    ADXCRS_Leave();
}

void ADXSTM_ExecFsIdle(void) {}

typedef struct ADXSTMBufferState {
    u8 _00[0x1c];
    int bufferSize;
    int _20;
    int _24;
    int _28;
    int _2c;
    int _30;
    int _34;
    int _38;
} ADXSTMBufferState;

int ADXSTM_SetBufSize(void *obj, int a, int b) {
    ADXSTMBufferState* state = (ADXSTMBufferState*)obj;
    ADXCRS_Enter();
    state->_20 = a;
    state->bufferSize = b;
    ADXCRS_Leave();
    return 1;
}

s32 ADXSTM_SetReqRdSize(void* self, u32 val) {
    ADXCRS_Enter();
    *(u32*)((u8*)self + 0x30) = val;
    ADXCRS_Leave();
    return 1;
}

u32 ADXSTM_GetFileLen(void* self) {
    ADXCRS_Enter();
    u32 result = *(u32*)((u8*)self + 0x14);
    ADXCRS_Leave();
    return result;
}

u64 ADXSTM_GetFileLen64(void* self) {
    ADXCRS_Enter();
    u32 hi = *(u32*)((u8*)self + 0x10);
    u32 lo = *(u32*)((u8*)self + 0x14);
    ADXCRS_Leave();
    return ((u64)hi << 32) | lo;
}

u32 ADXSTM_GetFileSct(void* self) {
    ADXCRS_Enter();
    u32 result = *(u32*)((u8*)self + 0x18);
    ADXCRS_Leave();
    return result;
}

void ADXSTM_SetPause(void* self, u32 val) {
    ADXCRS_Enter();
    *(u8*)((u8*)self + 0x48) = (u8)val;
    ADXCRS_Leave();
}

void ADXSTM_SetSj() {}

int ADXSTM_IsOpenedFile(void* self) { return *(s8*)((u8*)self + 0x4D); }
