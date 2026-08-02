// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsvr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern s32 SFD_GetHnStat(void* self);
extern s32 MWSST_GetStat(void* sst);
extern void MWSST_GetTime(void* sst, s32 flag);
extern void mwPlyPause(void* self, s32 flag);
extern void mwPlySfdStart(void* self);

typedef s32 (*SyncStartFn)(void*, s32);
typedef s32 (*VtableFn)(void*, void*);

void mwsfsvr_SyncStartSst(void* self) {
    void* sync;

    if (SFD_GetHnStat(*(void**)((u8*)self + 0x58)) != 3)
        return;

    sync = (u8*)self + 0x5D8;
    if (*(s32*)sync == 1) {
        if (MWSST_GetStat(sync) != 2) {
            void* obj = *(void**)((u8*)sync + 0x14);
            if (((SyncStartFn)(*(void***)obj)[9])(obj, 1) != 0)
                return;
        }
    }
    sync = (u8*)self + 0x600;
    if (*(s32*)sync == 1) {
        if (MWSST_GetStat(sync) != 2) {
            void* obj = *(void**)((u8*)sync + 0x14);
            if (((SyncStartFn)(*(void***)obj)[9])(obj, 1) != 0)
                return;
        }
    }

    mwPlySfdStart(self);
    if ((s8)*(u8*)((u8*)self + 0x92) != 0) {
        mwPlyPause(self, 0);
        if ((s8)*(u8*)((u8*)self + 0x92) != 0) {
            MWSST_GetTime((u8*)self + 0x5D8, 0);
            MWSST_GetTime((u8*)self + 0x600, 0);
        }
    }
}

void mwsfsvr_StartStream() {}

void mwlSfdExecDecSvrPlaying() {}

extern void* lbl_eu_805FF3A0;
extern u8 lbl_eu_80566C70[];
extern u8 lbl_eu_80567094[];
extern u32 lbl_eu_805FF39C;
extern u32 lbl_eu_806029F0;
extern u32 lbl_eu_806029F4;
extern void* MWSFLIB_GetLibWorkPtr(void);
extern s32 MWSFSVM_TestAndSet(void* p);
extern void SFD_VbIn(void);

void MWSFSVR_VsyncThrdProc(void) {
    u32 local = 0;
    if (lbl_eu_805FF3A0 != NULL) {
        void* obj = lbl_eu_805FF3A0;
        ((VtableFn)(*(void***)obj)[9])(obj, lbl_eu_80566C70 + 4);
    }
    if (lbl_eu_805FF3A0 != NULL) {
        void* obj = lbl_eu_805FF3A0;
        ((VtableFn)(*(void***)obj)[9])(obj, lbl_eu_80567094 + 4);
    }
    if (lbl_eu_805FF39C == 1) {
        void* w;
        lbl_eu_806029F0++;
        lbl_eu_806029F4++;
        w = MWSFLIB_GetLibWorkPtr();
        if (MWSFSVM_TestAndSet((u8*)w + 0x5C) == 1) {
            if (lbl_eu_805FF39C == 1)
                SFD_VbIn();
            *(u32*)((u8*)w + 0x5C) = 0;
        }
    }
    if (lbl_eu_805FF3A0 != NULL) {
        void* obj = lbl_eu_805FF3A0;
        ((VtableFn)(*(void***)obj)[9])(obj, lbl_eu_80567094 + 0x6C);
    }
    if (lbl_eu_805FF3A0 != NULL) {
        void* obj = lbl_eu_805FF3A0;
        *(void**)((u8*)lbl_eu_80566C70 + 0x74) = &local;
        ((VtableFn)(*(void***)obj)[9])(obj, (u8*)lbl_eu_80566C70 + 0x6C);
    }
}

void MWSFSVR_MainThrdProc() {}

void criware_803A2908() {}

void mwsfsvr_DecodeServer() {}

void mwply_ExecSvrHndl() {}

void mwsfd_ExecSvrHndl() {}

typedef struct MWSFDServerWork {
    u8 _00[0x58];
    int serverFlag;
} MWSFDServerWork;

void MWSFSVR_SetMwsfdSvrFlg(int flag) {
    ((MWSFDServerWork*)MWSFLIB_GetLibWorkPtr())->serverFlag = flag;
}

void MWSFSVR_SetHnMwplySvrFlg(void* self, u32 val) { *(u32*)((u8*)self + 0x7c) = val; }

void MWSFSVR_SetHnSfdSvrFlg(void* self, u32 val) { *(u32*)((u8*)self + 0x80) = val; }

extern int MWSFD_IsEnableHndl(void *);
extern void MWSFSVM_Error(const char *, ...);
extern char lbl_eu_8051BE68[];

int MWSFSVR_IsSvrBdrHndl(void *h) {
    if (MWSFD_IsEnableHndl(h) != 1) {
        MWSFSVM_Error(lbl_eu_8051BE68 + 0x51);
        return 0;
    }
    return *(u32 *)((u8 *)h + 0x7C) != 1;
}

void mwlSfdSleepDecSvr() {}

void MWSFLSC_Pause(void* self, int flag);
void* MWSFD_GetStmHn(void* self);
void MWSTM_ReqStop(void* h);
void MWSFD_RequestStopRead(void* self) {
    MWSFLSC_Pause(self, 1);
    MWSTM_ReqStop(MWSFD_GetStmHn(self));
}

s32 MWSFD_IsFsBdr(void* self) {
    s32 r = MWSTM_GetReadFlg(MWSFD_GetStmHn(self));
    return r != 1;
}

void MWSFD_SetProhibitServer(int val) {
    *(int*)((u8*)MWSFLIB_GetLibWorkPtr() + 0x34f0) = val;
}

void mwPlyExecRequestServer() {}
