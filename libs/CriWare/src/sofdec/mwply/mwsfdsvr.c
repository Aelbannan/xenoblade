// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsvr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
__declspec(section ".rodata") __attribute__((aligned(8))) char lbl_eu_8051BE68[0xC0] = {
    0x45,0x32,0x31,0x31,0x31,0x34,0x31,0x20,0x4D,0x57,0x53,0x54,
    0x4D,0x5F,0x52,0x65,0x71,0x53,0x74,0x61,0x72,0x74,0x3A,0x20,
    0x63,0x61,0x6E,0x27,0x74,0x20,0x73,0x74,0x61,0x72,0x74,0x20,
    0x27,0x25,0x73,0x27,0x00,0x45,0x31,0x30,0x37,0x31,0x39,0x30,
    0x31,0x20,0x6D,0x77,0x50,0x6C,0x79,0x45,0x78,0x65,0x63,0x53,
    0x76,0x72,0x48,0x6E,0x64,0x6C,0x3A,0x20,0x4E,0x55,0x4C,0x4C,
    0x20,0x68,0x61,0x6E,0x64,0x6C,0x65,0x2E,0x00,0x45,0x32,0x30,
    0x31,0x31,0x31,0x30,0x31,0x20,0x4D,0x57,0x53,0x46,0x53,0x56,
    0x52,0x5F,0x49,0x73,0x53,0x76,0x72,0x42,0x64,0x72,0x48,0x6E,
    0x64,0x6C,0x3A,0x20,0x68,0x61,0x6E,0x64,0x6C,0x65,0x20,0x69,
    0x73,0x20,0x69,0x6E,0x76,0x61,0x6C,0x69,0x64,0x2E,0x00,0x45,
    0x32,0x31,0x31,0x31,0x34,0x32,0x20,0x6D,0x77,0x50,0x6C,0x79,
    0x49,0x73,0x44,0x65,0x63,0x6F,0x64,0x65,0x72,0x57,0x61,0x69,
    0x74,0x69,0x6E,0x67,0x3A,0x20,0x63,0x61,0x6E,0x27,0x74,0x20,
    0x75,0x73,0x65,0x20,0x6F,0x6E,0x20,0x6D,0x75,0x6C,0x74,0x69,
    0x20,0x74,0x68,0x72,0x65,0x61,0x64,0x2E,0x00,0x00,0x00,0x00
};


extern s32 SFD_GetHnStat(void* self);
extern s32 MWSST_GetStat(void* sst);
extern void MWSST_GetTime(void* sst, s32 flag);
extern void mwPlyPause(void* self, s32 flag);
extern int mwPlySfdStart(void* self);

typedef s32 (*SyncStartFn)(void*, s32);
typedef s32 (*VtableFn)(void*, void*);

// Sound stream handle owned by a player; obj at 0x14 gets the sync-start request.
typedef struct MWSstHn {
    s32 state;  // 0x00
    u8 _04[0x10];
    void* obj;  // 0x14
    u8 _18[0x10];
} MWSstHn;

typedef struct MwPlySvr {
    u8 _00[0x58];
    void* hn;       // 0x58
    u8 _5C[0x36];
    s8 muteFlag;    // 0x92
    u8 _93[0x545];
    MWSstHn sst0;   // 0x5D8
    MWSstHn sst1;   // 0x600
} MwPlySvr;

// Start both sound streams once the decoder handle is ready; pause immediately
// if the player is muted, and reset stream times on unmute.
void mwsfsvr_SyncStartSst(MwPlySvr* self) {
    MWSstHn* sync;
    void* obj;

    if (SFD_GetHnStat(self->hn) != 3)
        return;

    sync = &self->sst0;
    if (sync->state == 1) {
        if (MWSST_GetStat(sync) != 2) {
            obj = sync->obj;
            if (((SyncStartFn)(*(void***)obj)[9])(obj, 1) != 0)
                return;
        }
    }
    sync = &self->sst1;
    if (sync->state == 1) {
        if (MWSST_GetStat(sync) != 2) {
            obj = sync->obj;
            if (((SyncStartFn)(*(void***)obj)[9])(obj, 1) != 0)
                return;
        }
    }

    mwPlySfdStart(self);
    if (self->muteFlag == 0) {
        mwPlyPause(self, 0);
    }
    if (self->muteFlag == 0) {
        MWSST_GetTime(&self->sst0, 0);
        MWSST_GetTime(&self->sst1, 0);
    }
}

extern s32 MWSTM_GetStat(void* stm);
extern s32 MWSTM_ReqStart(void* stm);
extern void MWSTM_SetFileRange(void* stm, s32 a, s32 b, s32 c, s32 d);
extern void MWSFCRE_SetSupplySj(void* self);
extern void MWSFSVM_Error(const char* fmt, ...);
extern s32 MWSFLIB_SetErrCode(s32 code);

// Player stream server handle: start/stop request path.
// NOTE: error-path scheduling (call-arg li floated above the preceding
// field store) is fixed by the unit's -O4 scheduler; see session notes.
typedef struct MwSvrHndl {
    u8 _00[0x04];
    s32 state;               // 0x04
    u8 _08[0x54];
    void* stm;               // 0x5C
    u8 _60[0x488];
    s32 filePos;             // 0x4E8
    u8 _4EC[0x04];
    s32 queuedFlag;          // 0x4F0
    s32 rangeStart;          // 0x4F4
    s32 rangeEnd;            // 0x4F8
    s32 rangeSize;           // 0x4FC
    void* obj;               // 0x500
    u8 _508[0x174];
    s32 stopFlag;            // 0x678
} MwSvrHndl;

s32 mwsfsvr_StartStream(MwSvrHndl* self) {
    if (MWSTM_GetStat(self->stm) == 2) return -1;
    if (self->stopFlag == 1) return 1;
    if (self->obj != NULL) {
        void* obj = self->obj;
        ((void (*)(void*))*(void**)((char*)*(void**)obj + 0x14))(obj);
    }
    MWSTM_SetFileRange(self->stm, self->filePos, self->rangeStart,
                       self->rangeEnd, self->rangeSize);
    if (MWSTM_ReqStart(self->stm) == -1) {
        self->state = 4;
        MWSFLIB_SetErrCode(-102);
        MWSFSVM_Error(lbl_eu_8051BE68, self->filePos);
        self->queuedFlag = 0;
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
extern u8 lbl_eu_80566D44[];
extern u8 lbl_eu_80566EEC[];
extern s32 lbl_eu_805FF39C;
extern void mwPlyExecInfiniteLoopHandle(void* h);
extern void mwPlyExecRequestServer(void* self);
extern void* MWSFLIB_GetLibWorkPtr(void);
extern u32 lbl_eu_806029F0;
extern u32 lbl_eu_806029F4;
extern void* MWSFLIB_GetLibWorkPtr(void);
extern s32 MWSFSVM_TestAndSet(void* p);
extern void SFD_VbIn(void);

// Shared library work area for the sofdec player servers.
typedef void (*MWSFSVR_Callback)(void*);

typedef struct MWSFLibWork {
    u8 _00[0x10];
    s32 state10;                // 0x10
    u8 _14[0x14];
    s32 pauseFlag;              // 0x24
    u8 _28[0x18];
    MWSFSVR_Callback preCb;     // 0x40
    void* preCbArg;             // 0x44
    MWSFSVR_Callback execCb;    // 0x48
    void* execCbArg;            // 0x4C
    MWSFSVR_Callback postCb;    // 0x50
    void* postCbArg;            // 0x54
    s32 serverLock;             // 0x58
    u8 _5C[0x14];
    u8 svrHndls[0x690 * 8];     // 0x70: 8 server handles, ends 0x34F0
    s32 prohibitFlag;           // 0x34F0
} MWSFLibWork;

s32 MWSFSVR_VsyncThrdProc(void) {
    s32 local;
    void* h;

    h = lbl_eu_805FF3A0;
    if (h != NULL) {
        /* Handle object: method table slot 9 dispatches a player callback. */
        ((VtableFn)(*(void***)h)[9])(h, lbl_eu_80566C70 + 4);
    }
    local = 0;
    h = lbl_eu_805FF3A0;
    if (h != NULL) {
        ((VtableFn)(*(void***)h)[9])(h, lbl_eu_80567094 + 4);
    }
    {
        /* Per-frame tick counters; the vsync lock guards SFD_VbIn re-entry. */
        MWSFLibWork* w;
        /* Non-short-circuit &: both sides always evaluate, so the tick
           counters update every frame while the body runs only on vsync. */
        if ((lbl_eu_805FF39C == 1) & (lbl_eu_806029F0++, lbl_eu_806029F4++, 1)) {
            w = MWSFLIB_GetLibWorkPtr();
            if (MWSFSVM_TestAndSet((u8*)w + 0x5C) == 1) {
                if (lbl_eu_805FF39C == 1)
                    SFD_VbIn();
                *(u32*)((u8*)w + 0x5C) = 0;
            }
        }
    }
    h = lbl_eu_805FF3A0;
    if (h != NULL) {
        ((VtableFn)(*(void***)h)[9])(h, lbl_eu_80567094 + 0x6C);
    }
    local = 0;
    h = lbl_eu_805FF3A0;
    if (h != NULL) {
        *(void**)((u8*)lbl_eu_80566C70 + 0x74) = &local;
        ((VtableFn)(*(void***)h)[9])(h, (u8*)lbl_eu_80566C70 + 0x6C);
    }
    return local;
}

// Player stream handle entry within a server handle (stride 0x690).
typedef struct MwPlySvrHndl {
    u8 _00[0x524];
    void* obj;      // 0x524
    u8 _528[0x08];
    u8 timeBuf;     // 0x530: time buffer passed to the setter below
    u8 _531[0x07];
    s32 statFlag;   // 0x538
} MwPlySvrHndl;

// Main sound-server thread: run pre-callback, service all 8 handles, tick the
// infinite-loop handlers, process start/stop requests, then run the decoder.
s32 MWSFSVR_MainThrdProc(void* self) {
    s32 local_c;
    s32 local_10;
    s32 local_8;

    if (lbl_eu_805FF3A0 != NULL) {
        ((void (*)(void*, void*))(*(void***)lbl_eu_805FF3A0)[9])(
            lbl_eu_805FF3A0, lbl_eu_80566D44 + 4);
    }
    local_c = 0;
    if (lbl_eu_805FF39C == 1) {
        MWSFLibWork* wk = MWSFLIB_GetLibWorkPtr();
        if (MWSFLIB_GetLibWorkPtr()->prohibitFlag != 1) {
            MwPlySvrHndl* h = (MwPlySvrHndl*)((u8*)wk + 0x70);
            s32 i;
            for (i = 0; i < 8; i++) {
                if (h != NULL && h->obj != NULL) {
                    void* obj = h->obj;
                    /* Query current time into local_10 (max sentinel arg). */
                    ((void (*)(void*, s32, u32, void*))*(void**)((char*)*(void**)obj + 0x18))(
                        obj, 0, 0x7FFFFFFF, &local_10);
                    if (h->statFlag == 1) {
                        s32 st = ((s32 (*)(void*, s32))*(void**)((char*)*(void**)obj + 0x24))(
                            obj, 1);
                        if (st <= 3) {
                            ((void (*)(void*, s32, void*))*(void**)((char*)*(void**)obj + 0x20))(
                                obj, 1, &h->timeBuf);
                        }
                    }
                }
                h = (MwPlySvrHndl*)((u8*)h + 0x690);
            }
        }
    }
    if (lbl_eu_805FF39C == 1) {
        MwPlySvrHndl* h = (MwPlySvrHndl*)((u8*)MWSFLIB_GetLibWorkPtr() + 0x70);
        s32 i;
        for (i = 0; i < 8; i++) {
            if (h != NULL) {
                mwPlyExecInfiniteLoopHandle(h);
            }
            h = (MwPlySvrHndl*)((u8*)h + 0x690);
        }
    }
    mwPlyExecRequestServer(self);
    if (MWSFLIB_GetLibWorkPtr()->state10 == 1) {
        if (lbl_eu_805FF3A0 != NULL) {
            ((void (*)(void*, void*))(*(void***)lbl_eu_805FF3A0)[9])(
                lbl_eu_805FF3A0, lbl_eu_80566EEC + 4);
        }
        local_8 = mwsfsvr_DecodeServer(self);
        if (lbl_eu_805FF3A0 != NULL) {
            *(void**)((u8*)lbl_eu_80566EEC + 0x74) = &local_8;
            ((void (*)(void*, void*))(*(void***)lbl_eu_805FF3A0)[9])(
                lbl_eu_805FF3A0, (u8*)lbl_eu_80566EEC + 0x6C);
        }
        local_c = local_8;
    }
    if (lbl_eu_805FF3A0 != NULL) {
        *(void**)((u8*)lbl_eu_80566D44 + 0x74) = &local_c;
        ((void (*)(void*, void*))(*(void***)lbl_eu_805FF3A0)[9])(
            lbl_eu_805FF3A0, (u8*)lbl_eu_80566D44 + 0x6C);
    }
    return local_c;
}

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

extern s32 MWSFSVM_TestAndSet(void* flag);
extern s32 mwply_ExecSvrHndl(void* self);

// Runs under vsync lock: services all 8 player server handles once per frame,
// bracketed by the library's pre/exec/post callbacks.
s32 mwsfsvr_DecodeServer(void* self) {
    MWSFLibWork* w;
    void* cbArg;
    s32 i;
    s32 found;

    if ((s32)lbl_eu_805FF39C != 1) return 0;
    w = MWSFLIB_GetLibWorkPtr();
    if (MWSFSVM_TestAndSet(&w->serverLock) != 1) return 0;
    {
        /* Fresh work-pointer read for the pre-callback; w stays live below. */
        MWSFLibWork* wk = MWSFLIB_GetLibWorkPtr();
        cbArg = wk->preCbArg;
        if (wk->preCb != NULL) {
            wk->preCb(cbArg);
        }
    }
    found = 0;
    if (((MWSFLibWork*)MWSFLIB_GetLibWorkPtr())->prohibitFlag != 1) {
        /* Advance onto the 8 server-handle array and walk it in place. */
        w = (MWSFLibWork*)w->svrHndls;
        for (i = 0; i < 8; i++) {
            if (w != NULL) {
                if (mwply_ExecSvrHndl(w) == 1) {
                    found = 1;
                }
            }
            w = (MWSFLibWork*)((u8*)w + 0x690);
        }
    }
    w = MWSFLIB_GetLibWorkPtr();
    w->serverLock = 0;
    w = MWSFLIB_GetLibWorkPtr();
    cbArg = w->execCbArg;
    if (w->execCb != NULL) {
        w->execCb(cbArg);
    }
    if (found != 1) {
        w = MWSFLIB_GetLibWorkPtr();
        if (w->pauseFlag != 1) {
            w = MWSFLIB_GetLibWorkPtr();
            cbArg = w->postCbArg;
            if (w->postCb != NULL) {
                w->postCb(cbArg);
            }
        }
    }
    return found;
}

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

int MWSFSVR_IsSvrBdrHndl(void *h) {
    if (MWSFD_IsEnableHndl(h) != 1) {
        MWSFSVM_Error(lbl_eu_8051BE68 + 0x51);
        return 0;
    }
    return *(u32 *)((u8 *)h + 0x7C) != 1;
}

extern void* MWSFLIB_GetLibWorkPtr(void);
extern void ADXM_WaitVsync(void);
extern void MWSFSVM_GotoIdleBorder(void);
extern void mwPlySaveRsc(void);
extern void mwPlyRestoreRsc(void);

void mwlSfdSleepDecSvr(void* self) {
    void* w;
    int i;

    mwPlySaveRsc();
    w = MWSFLIB_GetLibWorkPtr();
    if (self != NULL) {
        *(u32*)((u8*)self + 0x78) = 1;
    }
    *(u32*)((u8*)w + 0x24) = 1;
    MWSFSVM_GotoIdleBorder();
    w = MWSFLIB_GetLibWorkPtr();
    if (self != NULL) {
        *(u32*)((u8*)self + 0x78) = 0;
    }
    *(u32*)((u8*)w + 0x24) = 0;
    mwPlyRestoreRsc();
    if ((s32)*(u32*)((u8*)self + 0x7C) == 1) {
        for (i = 0; i < 10; i++) {
            w = MWSFLIB_GetLibWorkPtr();
            if (self != NULL) {
                *(u32*)((u8*)self + 0x78) = 1;
            }
            *(u32*)((u8*)w + 0x24) = 1;
            ADXM_WaitVsync();
            w = MWSFLIB_GetLibWorkPtr();
            if (self != NULL) {
                *(u32*)((u8*)self + 0x78) = 0;
            }
            *(u32*)((u8*)w + 0x24) = 0;
            if ((s32)*(u32*)((u8*)self + 0x7C) != 1) {
                break;
            }
        }
    }
}

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

extern s32 MWSFD_IsEndPrepareStop(void* self);
extern void mwPlyStop(void* self);

// Services queued start/stop requests on all 8 player streams while the
// server is running and not prohibited.
void mwPlyExecRequestServer() {
    u8* w;
    u8* h;
    s32 i;

    if ((s32)lbl_eu_805FF39C != 1) return;
    w = (u8*)MWSFLIB_GetLibWorkPtr();
    if (*(s32*)((u8*)MWSFLIB_GetLibWorkPtr() + 0x34F0) == 1) return;
    h = w + 0x70;
    for (i = 0; i < 8; i++) {
        if (h != NULL) {
            if (*(s32*)(h + 0x4F0) == 1) {
                if (mwsfsvr_StartStream((MwSvrHndl*)h) == 1) {
                    *(s32*)(h + 0x4F0) = 0;
                }
            }
            if (*(s32*)(h + 0x678) == 1) {
                if (MWSFD_IsEndPrepareStop(h) == 1) {
                    mwPlyStop(h);
                }
            }
        }
        h += 0x690;
    }
}
