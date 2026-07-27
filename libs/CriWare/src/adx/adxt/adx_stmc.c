// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_stmc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADXSTM_Init() {}

extern s32 lbl_eu_805E3E98;
void ADXSTM_Finish(void) {
    if (--lbl_eu_805E3E98 != 0) return;
    // would call ADXSTM_DestroyAll() here but that's in another TU
}

void ADXSTMF_SetupHandleMember() {}

void ADXSTM_Create() {}

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
    return ((signed char*)((u8*)self + 0x49))[0] ? 1 : 0;
}

void ADXSTM_GetStat() {}

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

void ADXSTM_Tell() {}

void ADXSTM_Start() {}

void ADXSTM_Start2() {}

void ADXSTM_StopNw() {}

void ADXSTM_Stop() {}

void ADXSTM_EntryEosFunc() {}

void ADXSTM_SetEos() {}

void adxstmf_stat_exec() {}

void ADXSTMF_ExecHndl() {}

void ADXSTM_ExecServer() {}

void ADXSTM_ExecFsSvr(void) {
    ADXCRS_Enter();
    cvFsExecServer();
    ADXCRS_Leave();
}

void ADXSTM_ExecFsIdle() {}

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

void ADXSTM_SetReqRdSize() {}

void ADXSTM_GetFileLen() {}

void ADXSTM_GetFileLen64() {}

void ADXSTM_GetFileSct() {}

void ADXSTM_SetPause() {}

void ADXSTM_SetSj() {}

int ADXSTM_IsOpenedFile(void* self) { return *(signed char*)self; }
