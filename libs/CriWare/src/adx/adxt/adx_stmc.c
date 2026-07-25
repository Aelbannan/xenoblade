// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_stmc
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-80383cc8
void ADXSTM_Init() {}
// LLM-HARNESS-END: us-80383cc8

// LLM-HARNESS-BEGIN: us-80383d14
extern s32 lbl_eu_805E3E98;
void ADXSTM_Finish(void) {
    if (--lbl_eu_805E3E98 != 0) return;
    // would call ADXSTM_DestroyAll() here but that's in another TU
}
// LLM-HARNESS-END: us-80383d14

// LLM-HARNESS-BEGIN: us-80383d40
void ADXSTMF_SetupHandleMember() {}
// LLM-HARNESS-END: us-80383d40

// LLM-HARNESS-BEGIN: us-80383e50
void ADXSTM_Create() {}
// LLM-HARNESS-END: us-80383e50

// LLM-HARNESS-BEGIN: us-80383f98
void ADXSTM_Destroy() {}
// LLM-HARNESS-END: us-80383f98

// LLM-HARNESS-BEGIN: us-803841ec
void ADXSTM_BindFileNw() {}
// LLM-HARNESS-END: us-803841ec

// LLM-HARNESS-BEGIN: us-80384280
void ADXSTM_ReleaseFileNw() {}
// LLM-HARNESS-END: us-80384280

// LLM-HARNESS-BEGIN: us-80384324
void ADXSTM_ReleaseFile() {}
// LLM-HARNESS-END: us-80384324

// LLM-HARNESS-BEGIN: us-803844a8
int ADXSTM_IsOpened(const void* self) {
    const signed char* b = (const signed char*)self;
    if (b[0x4a] != 0) return 1;
    if (b[0x49] != 0) return 0;
    return b[0x4d] != 0 ? 1 : 0;
}
// LLM-HARNESS-END: us-803844a8

// LLM-HARNESS-BEGIN: us-803844e8
s32 ADXSTM_IsOpenReq(void* self) {
    return ((signed char*)((u8*)self + 0x49))[0] ? 1 : 0;
}
// LLM-HARNESS-END: us-803844e8

// LLM-HARNESS-BEGIN: us-80384510
void ADXSTM_GetStat() {}
// LLM-HARNESS-END: us-80384510

// LLM-HARNESS-BEGIN: us-8038454c
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
// LLM-HARNESS-END: us-8038454c

// LLM-HARNESS-BEGIN: us-803845a4
void ADXSTM_Tell() {}
// LLM-HARNESS-END: us-803845a4

// LLM-HARNESS-BEGIN: us-803845f0
void ADXSTM_Start() {}
// LLM-HARNESS-END: us-803845f0

// LLM-HARNESS-BEGIN: us-8038467c
void ADXSTM_Start2() {}
// LLM-HARNESS-END: us-8038467c

// LLM-HARNESS-BEGIN: us-8038470c
void ADXSTM_StopNw() {}
// LLM-HARNESS-END: us-8038470c

// LLM-HARNESS-BEGIN: us-80384784
void ADXSTM_Stop() {}
// LLM-HARNESS-END: us-80384784

// LLM-HARNESS-BEGIN: us-80384864
void ADXSTM_EntryEosFunc() {}
// LLM-HARNESS-END: us-80384864

// LLM-HARNESS-BEGIN: us-803848b4
void ADXSTM_SetEos() {}
// LLM-HARNESS-END: us-803848b4

// LLM-HARNESS-BEGIN: us-80384908
void adxstmf_stat_exec() {}
// LLM-HARNESS-END: us-80384908

// LLM-HARNESS-BEGIN: us-80384d68
void ADXSTMF_ExecHndl() {}
// LLM-HARNESS-END: us-80384d68

// LLM-HARNESS-BEGIN: us-803850ac
void ADXSTM_ExecServer() {}
// LLM-HARNESS-END: us-803850ac

// LLM-HARNESS-BEGIN: us-80385130
void ADXSTM_ExecFsSvr(void) {
    ADXCRS_Enter();
    cvFsExecServer();
    ADXCRS_Leave();
}
// LLM-HARNESS-END: us-80385130

// LLM-HARNESS-BEGIN: us-80385158
void ADXSTM_ExecFsIdle() {}
// LLM-HARNESS-END: us-80385158

// LLM-HARNESS-BEGIN: us-8038515c
int ADXSTM_SetBufSize(void *obj, int a, int b) {
    ADXCRS_Enter();
    ((int*)obj)[8] = a;
    ((int*)obj)[7] = b;
    ADXCRS_Leave();
    return 1;
}
// LLM-HARNESS-END: us-8038515c

// LLM-HARNESS-BEGIN: us-803851b0
void ADXSTM_SetReqRdSize() {}
// LLM-HARNESS-END: us-803851b0

// LLM-HARNESS-BEGIN: us-803851f4
void ADXSTM_GetFileLen() {}
// LLM-HARNESS-END: us-803851f4

// LLM-HARNESS-BEGIN: us-8038522c
void ADXSTM_GetFileLen64() {}
// LLM-HARNESS-END: us-8038522c

// LLM-HARNESS-BEGIN: us-80385274
void ADXSTM_GetFileSct() {}
// LLM-HARNESS-END: us-80385274

// LLM-HARNESS-BEGIN: us-803852ac
void ADXSTM_SetPause() {}
// LLM-HARNESS-END: us-803852ac

// LLM-HARNESS-BEGIN: us-803852ec
void ADXSTM_SetSj() {}
// LLM-HARNESS-END: us-803852ec

// LLM-HARNESS-BEGIN: us-80385384
int ADXSTM_IsOpenedFile(void* self) { return (signed char)((u8*)self)[0]; }
// LLM-HARNESS-END: us-80385384
