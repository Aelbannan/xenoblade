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

extern char lbl_eu_8051BE68[];
extern s32 MWSTM_GetStat(void* stm);
extern s32 MWSTM_ReqStart(void* stm);
extern void MWSFCRE_SetSupplySj(void* self);
extern void MWSFSVM_Error(const char* fmt, ...);
extern void MWSFLIB_SetErrCode(s32 code);

s32 mwsfsvr_StartStream(void* self) {
    if (MWSTM_GetStat(*(void**)((u8*)self + 0x5C)) == 2) return -1;
    if (*(s32*)((u8*)self + 0x678) == 1) return 1;
    if (*(void**)((u8*)self + 0x500) != NULL) {
        void* obj = *(void**)((u8*)self + 0x500);
        ((void (*)(void*))*(void**)((char*)*(void**)obj + 0x14))(obj);
    }
    MWSTM_SetFileRange(*(void**)((u8*)self + 0x5C), *(s32*)((u8*)self + 0x4E8),
                       *(s32*)((u8*)self + 0x4F4), *(s32*)((u8*)self + 0x4F8),
                       *(s32*)((u8*)self + 0x4FC));
    if (MWSTM_ReqStart(*(void**)((u8*)self + 0x5C)) == -1) {
        *(s32*)((u8*)self + 4) = 4;
        MWSFLIB_SetErrCode(-102);
        MWSFSVM_Error(lbl_eu_8051BE68, *(s32*)((u8*)self + 0x4E8));
        *(s32*)((u8*)self + 0x4F0) = 0;
        return -1;
    }
    MWSFCRE_SetSupplySj(self);
    return 1;
}

extern void* mwPlyGetSstCoreHnByIdx(void* self, s32 idx);
extern s32 MWSST_GetStat(void* sst);

void mwlSfdExecDecSvrPlaying(void* self) {
    void* sst = *(void**)((u8*)self + 0x58);
    if (*(s32*)((u8*)self + 0x678) == 0) {
        s32 s = SFD_GetHnStat(sst);
        s32 a = 4;
        s32 b = 4;
        if (mwPlyGetSstCoreHnByIdx(self, 0) != 0) {
            a = MWSST_GetStat((u8*)self + 0x5D8);
        }
        if (mwPlyGetSstCoreHnByIdx(self, 1) != 0) {
            b = MWSST_GetStat((u8*)self + 0x600);
        }
        if (s == 6 && a != 2 && b != 2) {
            *(s32*)((u8*)self + 4) = 3;
        }
    }
}

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

extern u8 lbl_eu_80566E18[];
extern u8 lbl_eu_80566EEC[];
extern void* MWSFLIB_GetLibWorkPtr(void);
extern s32 mwsfsvr_DecodeServer(void* self);

static s32 criware_803A2908(void* self) {
    s32 local;
    if (lbl_eu_805FF3A0 != NULL) {
        ((void (*)(void*, void*))*(void**)((char*)*(void**)lbl_eu_805FF3A0 + 0x24))(
            lbl_eu_805FF3A0, lbl_eu_80566E18 + 4);
    }
    local = 0;
    {
        void* lw = MWSFLIB_GetLibWorkPtr();
        if (*(s32*)((u8*)lw + 0x10) != 1) {
            if (lbl_eu_805FF3A0 != NULL) {
                ((void (*)(void*, void*))*(void**)((char*)*(void**)lbl_eu_805FF3A0 + 0x24))(
                    lbl_eu_805FF3A0, lbl_eu_80566EEC + 4);
            }
            {
                s32 r = mwsfsvr_DecodeServer(self);
                if (lbl_eu_805FF3A0 != NULL) {
                    *(void**)(lbl_eu_80566EEC + 0x74) = &r;
                    ((void (*)(void*, void*))*(void**)((char*)*(void**)lbl_eu_805FF3A0 + 0x24))(
                        lbl_eu_805FF3A0, lbl_eu_80566EEC + 0x6C);
                }
                local = r;
            }
        }
        if (lbl_eu_805FF3A0 != NULL) {
            *(void**)(lbl_eu_80566E18 + 0x74) = &local;
            ((void (*)(void*, void*))*(void**)((char*)*(void**)lbl_eu_805FF3A0 + 0x24))(
                lbl_eu_805FF3A0, lbl_eu_80566E18 + 0x6C);
        }
    }
    return local;
}

s32 mwsfsvr_DecodeServer(void* self) { return 0; }

extern u8 lbl_eu_80566FC0[];
extern s32 mwsfd_ExecSvrHndl(void* self);

s32 mwply_ExecSvrHndl(void* self) {
    if ((s32)lbl_eu_805FF39C != 1) return 0;
    if (self == NULL) {
        MWSFSVM_Error(lbl_eu_8051BE68 + 0x29);
        return 0;
    }
    if (*(s32*)self != 1) return 0;
    if (*(s32*)((u8*)self + 0x7C) == 1) return 0;
    if (*(s32*)((u8*)MWSFLIB_GetLibWorkPtr() + 0x24) == 1) return 0;
    if (*(s32*)((u8*)self + 0x678) == 1) return 0;
    if (lbl_eu_805FF3A0 != NULL) {
        *(void**)(lbl_eu_80566FC0 + 0xC) = self;
        ((void (*)(void*, void*))*(void**)((char*)*(void**)lbl_eu_805FF3A0 + 0x24))(
            lbl_eu_805FF3A0, lbl_eu_80566FC0 + 4);
    }
    {
        s32 local = mwsfd_ExecSvrHndl(self);
        if (lbl_eu_805FF3A0 != NULL) {
            *(void**)(lbl_eu_80566FC0 + 0x74) = &local;
            ((void (*)(void*, void*))*(void**)((char*)*(void**)lbl_eu_805FF3A0 + 0x24))(
                lbl_eu_805FF3A0, lbl_eu_80566FC0 + 0x6C);
        }
        return local;
    }
}

s32 mwsfd_ExecSvrHndl(void* self) { return 0; }

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
