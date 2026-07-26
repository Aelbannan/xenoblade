// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsvr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void mwsfsvr_SyncStartSst() {}

void mwsfsvr_StartStream() {}

void mwlSfdExecDecSvrPlaying() {}

void MWSFSVR_VsyncThrdProc() {}

void MWSFSVR_MainThrdProc() {}

void criware_803A2908() {}

void mwsfsvr_DecodeServer() {}

void mwply_ExecSvrHndl() {}

void mwsfd_ExecSvrHndl() {}

void MWSFSVR_SetMwsfdSvrFlg(int flag) { *(int*)((char*)MWSFLIB_GetLibWorkPtr() + 0x58) = flag; }

void MWSFSVR_SetHnMwplySvrFlg(void* self, u32 val) { *(u32*)((u8*)self + 0x7c) = val; }

void MWSFSVR_SetHnSfdSvrFlg(void* self, u32 val) { *(u32*)((u8*)self + 0x80) = val; }

void MWSFSVR_IsSvrBdrHndl() {}

void mwlSfdSleepDecSvr() {}

void MWSFD_RequestStopRead() {}

void MWSFD_IsFsBdr() {}

void MWSFD_SetProhibitServer() {}

void mwPlyExecRequestServer() {}
