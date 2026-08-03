// Decompiled adx_stmc.c — ADX stream management (CriWare)
#include <harness_catalog.h>

extern volatile s32 lbl_eu_805E3E98;
extern u8 lbl_eu_805E3EA8[0x1040];
extern volatile s32 lbl_eu_805E3E9C;
extern volatile s32 lbl_eu_805E3EA0;
extern volatile s32 lbl_eu_805E3EA4;
extern volatile s32 lbl_eu_805E4EE8;

typedef struct {
    /* 0x00 */ u8 active;
    /* 0x01 */ s8 stat;
    /* 0x02 */ u8 subState;
    /* 0x03 */ u8 slotType;        // 1 = default pool, 0 = extended pool
    /* 0x04 */ void* sj;
    /* 0x08 */ void* fileHandle;
    /* 0x0C */ u32 startOffset;
    /* 0x10 */ u32 fileSizeHi;
    /* 0x14 */ u32 fileSizeLo;
    /* 0x18 */ u32 fileSectors;
    /* 0x1C */ u32 bufFill;
    /* 0x20 */ u32 bufSize;
    /* 0x24 */ u32 unk24;
    /* 0x28 */ u32 reqId;
    /* 0x2C */ u32 rdAddr;
    /* 0x30 */ u32 reqRdSize;
    /* 0x34 */ u32 eosPos;
    /* 0x38 */ u32 written;
    /* 0x40 */ void* eosFunc;
    /* 0x44 */ void* eosCtx;
    /* 0x48 */ u32 sjBufSize;
    /* 0x4C */ u8 pause;
    /* 0x4D */ u8 openReq;
    /* 0x4E */ u8 closing;
    /* 0x4F */ u8 startPending;
    /* 0x50 */ u8 stopReq;
    /* 0x51 */ u8 opened;
    /* 0x52 */ u8 pad[2];
    /* 0x54 */ u32 retryCount;
    /* 0x54 */ void* fileName;
    /* 0x58 */ u32 fileSizeParam;
    /* 0x5C */ u32 seekPos;
    /* 0x60 */ u32 maxSectors;
    /* 0x64 */ u32 bufStartOff;
} ADXSTMHndl; // size 0x68

typedef struct {
    /* 0x00 */ s32 defCount;
    /* 0x04 */ s32 extStart;
    /* 0x08 */ s32 extCount;
} ADXSTMConfig;

extern ADXSTMConfig lbl_eu_80560030;

static inline s32 div_s2048(u32 val) {
    return (s32)val >> 11;
}

static inline u32 ceil_div2048(u32 size) {
    return (size + 0x7FF) >> 11;
}

s32 ADXSTM_Init(void) {
    s32 tmp = ++lbl_eu_805E3E98;
    if (tmp == 1) {
        memset(lbl_eu_805E3EA8, 0, 0x1040);
    }
    return 1;
}

void ADXSTM_Finish(void) {
    if (--lbl_eu_805E3E98 != 0) return;
    memset(lbl_eu_805E3EA8, 0, 0x1040);
}

void ADXSTMF_SetupHandleMember(ADXSTMHndl* h, void* fileName, u32 fileSizeParam,
                                u32 startOffsetLo, void* sj) {
    u32 sectors;
    u32 bufSize;

    ADXCRS_Lock();

    sectors = (u32)((s32)((s32)startOffsetLo / 2048) +
                      (s32)((-(u32)((s32)startOffsetLo % 2048) & ~(u32)((s32)startOffsetLo % 2048)) >> 31));

    h->stat = 1;
    h->subState = 0;
    h->sj = sj;
    h->fileHandle = fileName;
    h->startOffset = fileSizeParam;
    h->fileSizeLo = startOffsetLo;
    h->fileSizeHi = (s32)startOffsetLo >> 31;
    h->fileSectors = sectors;
    h->reqRdSize = 0x200;
    h->seekPos = 0;
    h->maxSectors = 0x100000 - 1;
    h->eosPos = sectors;

    if (sj != NULL) {
        typedef u32 (*GetLenFn)(void*, u32);
        u32 len1 = ((GetLenFn)*((u32*)*(void**)sj + 9))(sj, 1);
        u32 len0 = ((GetLenFn)*((u32*)*(void**)sj + 9))(sj, 0);
        bufSize = len0 + len1;
        h->sjBufSize = bufSize;
        h->bufFill = bufSize;
        h->bufSize = bufSize;
    }

    h->pause = 0;
    h->active = 1;

    ADXCRS_Unlock();
}

void* ADXSTM_Create(void* fileName, int type) {
    ADXSTMHndl* h;
    s32 i, count;
    ADXSTMHndl* pool;
    s32 start;

    ADXCRS_Enter();

    if (type < 0x100) {
        count = lbl_eu_80560030.defCount;
        pool = (ADXSTMHndl*)lbl_eu_805E3EA8;
        start = lbl_eu_805E3E9C * 0x68;
        h = NULL;
        for (i = 0; i < count; i++) {
            h = (ADXSTMHndl*)(lbl_eu_805E3EA8 + start);
            if ((s8)h->active == 0) break;
            start += 0x68;
        }
        if (i == count) {
            h = NULL;
        } else {
            ADXSTMF_SetupHandleMember(h, NULL, 0, 0, fileName);
            h->slotType = 1;
        }
    } else {
        start = lbl_eu_80560030.extStart * 0x68;
        count = lbl_eu_80560030.extCount;
        h = NULL;
        for (i = 0; i < count; i++) {
            h = (ADXSTMHndl*)(lbl_eu_805E3EA8 + start);
            if ((s8)h->active == 0) break;
            start += 0x68;
        }
        if (i == count) {
            h = NULL;
        } else {
            ADXSTMF_SetupHandleMember(h, NULL, 0, 0, fileName);
            h->slotType = 0;
        }
    }

    ADXCRS_Leave();
    return h;
}

void ADXSTM_Destroy(ADXSTMHndl* h) {
    ADXCRS_Enter();
    if (h == NULL) goto done;

    ADXCRS_Enter();
    if (h->fileHandle && (s8)h->closing == 0) {
        cvFsStopTr(h->fileHandle);
    }
    ADXCRS_Lock();
    h->stat = 1;
    h->subState = 0;
    *(u32*)((u8*)h + 0x28) = 0;
    ADXCRS_Unlock();
    ADXCRS_Enter();
    ADXCRS_Lock();
    if ((s32)h->stat == 2 && (s32)h->subState == 1) {
        h->stopReq = 1;
        if ((s32)h->startPending == 1) {
            h->startPending = 0;
        }
    } else {
        h->stat = 1;
    }
    ADXCRS_Unlock();
    ADXCRS_Leave();

    for (;;) {
        if (h->stat == 1 && *(u32*)((u8*)h + 0x28) == 0) break;
        ADXT_ExecFsSvr();
    }

    ADXCRS_Leave();
    ADXCRS_Enter();
    ADXCRS_Enter();
    if (h->fileHandle && (s8)h->closing == 0) {
        cvFsStopTr(h->fileHandle);
    }
    ADXCRS_Lock();
    h->stat = 1;
    h->subState = 0;
    *(u32*)((u8*)h + 0x28) = 0;
    ADXCRS_Unlock();
    ADXCRS_Enter();
    ADXCRS_Lock();
    if ((s32)h->stat == 2 && (s32)h->subState == 1) {
        h->stopReq = 1;
        if ((s32)h->startPending == 1) {
            h->startPending = 0;
        }
    } else {
        h->stat = 1;
    }
    ADXCRS_Unlock();
    ADXCRS_Leave();

    for (;;) {
        if (h->stat == 1 && *(u32*)((u8*)h + 0x28) == 0) break;
        ADXT_ExecFsSvr();
    }

    ADXCRS_Leave();
    ADXCRS_Enter();
    ADXCRS_Enter();
    ADXCRS_Lock();
    if ((s32)h->stat == 2 && (s32)h->subState == 1) {
        h->stopReq = 1;
        if ((s32)h->startPending == 1) {
            h->startPending = 0;
        }
    } else {
        h->stat = 1;
    }
    ADXCRS_Unlock();
    ADXCRS_Leave();
    ADXCRS_Lock();
    if ((s32)h->opened == 1) {
        h->closing = 1;
    }
    h->openReq = 0;
    ADXCRS_Unlock();
    ADXCRS_Leave();

    for (;;) {
        if ((s8)h->opened == 0 && (s8)h->closing == 0) break;
        ADXT_ExecFsSvr();
    }

    ADXCRS_Leave();
    h->active = 0;
    memset((void*)h, 0, sizeof(ADXSTMHndl));
done:
    ADXCRS_Leave();
}

void ADXSTM_BindFileNw(ADXSTMHndl* h, void* fileName, u32 fileSizeParam,
                        u32 startOffset, u32 sizeHi, u32 sizeLo) {
    u32 sectors;
    s64 len;

    ADXCRS_Enter();
    ADXCRS_Lock();

    len = ((s64)(u32)sizeHi << 32) | (u64)(u32)sizeLo;
    sectors = (u32)((s32)(((s64)len + 0x7FF) / 2048));

    h->startOffset = startOffset;
    h->fileSizeLo = sizeLo;
    h->fileSizeHi = sizeHi;
    h->fileSectors = sectors;
    h->fileName = fileName;
    h->fileSizeParam = fileSizeParam;
    h->openReq = 1;

    ADXCRS_Unlock();
    ADXCRS_Leave();
}

void ADXSTM_ReleaseFileNw(ADXSTMHndl* h) {
    ADXCRS_Enter();
    ADXCRS_Enter();
    ADXCRS_Lock();

    if ((s32)h->stat == 2 && (s32)h->subState == 1) {
        h->stopReq = 1;
        if ((s32)h->startPending == 1) {
            h->startPending = 0;
        }
    } else {
        h->stat = 1;
    }

    ADXCRS_Unlock();
    ADXCRS_Leave();
    ADXCRS_Lock();

    if ((s32)h->opened == 1) {
        h->closing = 1;
    }
    h->openReq = 0;

    ADXCRS_Unlock();
    ADXCRS_Leave();
}

void ADXSTM_ReleaseFile(ADXSTMHndl* h) {
    ADXCRS_Enter();
    ADXCRS_Enter();

    if (h->fileHandle && (s8)h->closing == 0) {
        cvFsStopTr(h->fileHandle);
    }

    ADXCRS_Lock();
    h->stat = 1;
    h->subState = 0;
    *(u32*)((u8*)h + 0x28) = 0;
    ADXCRS_Unlock();
    ADXCRS_Enter();
    ADXCRS_Lock();
    if ((s32)h->stat == 2 && (s32)h->subState == 1) {
        h->stopReq = 1;
        if ((s32)h->startPending == 1) {
            h->startPending = 0;
        }
    } else {
        h->stat = 1;
    }
    ADXCRS_Unlock();
    ADXCRS_Leave();

    for (;;) {
        if (h->stat == 1 && *(u32*)((u8*)h + 0x28) == 0) break;
        ADXT_ExecFsSvr();
    }

    ADXCRS_Leave();
    ADXCRS_Enter();
    ADXCRS_Enter();
    ADXCRS_Lock();
    if ((s32)h->stat == 2 && (s32)h->subState == 1) {
        h->stopReq = 1;
        if ((s32)h->startPending == 1) {
            h->startPending = 0;
        }
    } else {
        h->stat = 1;
    }
    ADXCRS_Unlock();
    ADXCRS_Leave();
    ADXCRS_Lock();
    if ((s32)h->opened == 1) {
        h->closing = 1;
    }
    h->openReq = 0;
    ADXCRS_Unlock();
    ADXCRS_Leave();

    for (;;) {
        if ((s8)h->opened == 0 && (s8)h->closing == 0) break;
        ADXT_ExecFsSvr();
    }

    ADXCRS_Leave();
}

int ADXSTM_IsOpened(const ADXSTMHndl* h) {
    if ((s8)h->closing != 0) return 1;
    if ((s8)h->openReq != 0) return 0;
    return (s8)h->opened ? 1 : 0;
}

s32 ADXSTM_IsOpenReq(ADXSTMHndl* h) {
    if (h->openReq || h->opened) return 1;
    return 0;
}

s32 ADXSTM_GetStat(ADXSTMHndl* h) {
    s32 result;
    ADXCRS_Enter();
    result = (s8)h->stat;
    ADXCRS_Leave();
    return result;
}

int ADXSTM_Seek(ADXSTMHndl* h, int pos) {
    ADXCRS_Enter();
    int max = (int)h->fileSectors;
    h->seekPos = pos;
    if (pos > max)
        h->seekPos = max;
    int result = (int)h->seekPos;
    ADXCRS_Leave();
    return result;
}

s32 ADXSTM_Tell(ADXSTMHndl* h) {
    ADXCRS_Enter();
    s32 result;
    if (h->fileHandle != NULL) {
        result = h->seekPos;
    } else {
        result = 0;
    }
    ADXCRS_Leave();
    return result;
}

s32 ADXSTM_Start(ADXSTMHndl* h) {
    ADXCRS_Enter();
    ADXCRS_Lock();
    h->written = 0;
    h->retryCount = 0;
    if (h->fileSectors == 0) {
        h->stat = 3;
    } else {
        h->stat = 2;
    }
    h->subState = 0;
    h->reqId = 0;
    h->rdAddr = 0;
    h->startPending = 1;
    h->maxSectors = 0x100000 - 1;
    ADXCRS_Unlock();
    ADXCRS_Leave();
    return 1;
}

s32 ADXSTM_Start2(ADXSTMHndl* h, u32 maxSectors) {
    ADXCRS_Enter();
    ADXCRS_Lock();
    h->written = 0;
    h->retryCount = 0;
    if (h->fileSectors == 0) {
        h->stat = 3;
    } else {
        h->stat = 2;
    }
    h->subState = 0;
    h->reqId = 0;
    h->rdAddr = 0;
    h->startPending = 1;
    h->maxSectors = maxSectors;
    ADXCRS_Unlock();
    ADXCRS_Leave();
    return 1;
}

void ADXSTM_StopNw(ADXSTMHndl* h) {
    ADXCRS_Enter();
    ADXCRS_Lock();
    if ((s32)h->stat == 2 && (s32)h->subState == 1) {
        h->stopReq = 1;
        if ((s32)h->startPending == 1) {
            h->startPending = 0;
        }
    } else {
        h->stat = 1;
    }
    ADXCRS_Unlock();
    ADXCRS_Leave();
}

void ADXSTM_Stop(ADXSTMHndl* h) {
    ADXCRS_Enter();

    if (h->fileHandle && (s8)h->closing == 0) {
        cvFsStopTr(h->fileHandle);
    }

    ADXCRS_Lock();
    h->stat = 1;
    h->subState = 0;
    *(u32*)((u8*)h + 0x28) = 0;
    ADXCRS_Unlock();

    ADXCRS_Enter();
    ADXCRS_Lock();
    if ((s32)h->stat == 2 && (s32)h->subState == 1) {
        h->stopReq = 1;
        if ((s32)h->startPending == 1) {
            h->startPending = 0;
        }
    } else {
        h->stat = 1;
    }
    ADXCRS_Unlock();
    ADXCRS_Leave();

    for (;;) {
        if (h->stat == 1 && *(u32*)((u8*)h + 0x28) == 0) break;
        ADXT_ExecFsSvr();
    }

    ADXCRS_Leave();
}

void ADXSTM_EntryEosFunc(ADXSTMHndl* h, void* func, void* ctx) {
    ADXCRS_Enter();
    h->eosFunc = func;
    h->eosCtx = ctx;
    ADXCRS_Leave();
}

void ADXSTM_SetEos(ADXSTMHndl* h, s32 val) {
    ADXCRS_Enter();
    if (val >= 0) {
        h->eosPos = val;
    } else {
        h->eosPos = h->fileSectors;
    }
    ADXCRS_Leave();
}

void adxstmf_stat_exec(ADXSTMHndl* h) {
    u32 avail;
    u32 rdSectors;
    s32 stat;
    u32 tmp[2]; // stack buffer for cvFs

    void* sj = h->sj;
    void* fh = h->fileHandle;

    stat = cvFsGetStat(fh);
    ADXCRS_Lock();

    if (h->subState != 1) {
        ADXCRS_Unlock();
        goto epilogue;
    }

    if (stat == 1) {
        /* Transfer complete */
        h->subState = 0;
        ADXCRS_Unlock();

        u32 chunkSize = h->bufSize << 11;
        u32 addr[2];
        u32 size[2];
        SJ_SplitChunk(&h->reqId, chunkSize, addr, size);

        /* Notify SJ of read completion */
        typedef void (*NotifyFn)(void*, u32, u32*);
        NotifyFn notify;
        void* vtbl = *(void**)sj;
        notify = (NotifyFn)*((u32*)vtbl + 8);
        notify(sj, 1, addr);

        NotifyFn notify2 = (NotifyFn)*((u32*)vtbl + 7);
        notify2(sj, 0, size);

        h->seekPos += h->bufSize;
        h->written += chunkSize;
        h->reqId = 0;
        h->rdAddr = 0;

        /* Check EOS */
        if (h->seekPos == h->eosPos && h->eosFunc != NULL) {
            typedef void (*EosFn)(void*);
            ((EosFn)h->eosFunc)(h->eosCtx);
        }

        /* Check if done */
        if (h->seekPos >= h->fileSectors) {
            h->stat = 3;
        } else if (h->written >> 11 >= h->maxSectors && h->maxSectors < 0xFFFFF) {
            h->stat = 3;
        }
        h->retryCount = 0;
    } else if (stat == 3) {
        /* Error — retry */
        h->subState = 0;
        ADXCRS_Unlock();

        void* vtbl = *(void**)sj;
        typedef void (*NotifyFn)(void*, u32, u32*);
        NotifyFn notify = (NotifyFn)*((u32*)vtbl + 7);
        notify(sj, 0, &h->reqId);

        h->reqId = 0;
        h->rdAddr = 0;

        if (!cvFsIsAvailableRetry(h->fileHandle)) {
            h->stat = 4;
            goto epilogue;
        }

        s32 maxRetry = lbl_eu_805E3EA4;
        if (maxRetry >= 0 && h->retryCount >= maxRetry) {
            h->stat = 4;
            goto epilogue;
        }

        if (h->retryCount < 0x7FFFFFFF) {
            h->retryCount++;
        }
    } else {
        ADXCRS_Unlock();
    }

epilogue:
    if (h->stat == 4) return;

    ADXCRS_Lock();
    if (h->subState != 0) {
        ADXCRS_Unlock();
        return;
    }

    h->subState = 1;
    h->reqId = 0;
    h->rdAddr = 0;
    ADXCRS_Unlock();

    if (h->pause != 1 && h->stopReq == 1) {
        h->subState = 0;
        return;
    }

    if (h->fileSectors == 0) {
        h->subState = 0;
        h->stat = 3;
        lbl_eu_805E4EE8++;
        return;
    }

    /* Check available buffer space */
    u32 freeSpace;
    void* vtbl = *(void**)sj;
    typedef u32 (*GetFreeFn)(void*, u32);
    GetFreeFn getFree = (GetFreeFn)*((u32*)vtbl + 9);
    freeSpace = getFree(sj, 0);

    avail = h->sjBufSize - freeSpace;
    if (avail >= h->bufStartOff) {
        h->subState = 0;
        return;
    }

    /* Get read address */
    typedef void (*GetAddrFn)(void*, u32, u32*, u32*);
    GetAddrFn getAddr = (GetAddrFn)*((u32*)vtbl + 6);
    getAddr(sj, 0, &h->bufFill, tmp);

    u32 sectorsAvail = (s32)tmp[1] >> 11;
    sectorsAvail += (tmp[1] < 0 && (tmp[1] & 0x7FF) != 0) ? 1 : 0;

    s32 eosRemain = h->eosPos - h->seekPos;
    if (sectorsAvail > eosRemain) sectorsAvail = eosRemain;
    u32 fileRemain = h->fileSectors - h->seekPos;
    if (sectorsAvail > fileRemain) sectorsAvail = fileRemain;
    u32 reqSize = h->reqRdSize;
    if (sectorsAvail > reqSize) sectorsAvail = reqSize;

    if (h->maxSectors != 0xFFFFF) {
        u32 maxRemain = h->maxSectors - (h->written >> 11);
        if (sectorsAvail > maxRemain) sectorsAvail = maxRemain;
    }

    cvFsSeek(h->fileHandle, h->startOffset + h->seekPos, 0);
    h->reqId = cvFsReqRd(h->fileHandle, sectorsAvail, tmp[0]);
    h->reqId = (h->reqId > 0) ? h->reqId : 0;
    h->rdAddr = tmp[0];
    *(u32*)((u8*)h + 0x2C) = tmp[1];

    if (h->reqId <= 0) {
        /* Request failed — notify SJ */
        getAddr(sj, 0, &h->rdAddr, NULL);
        h->reqId = 0;
        h->rdAddr = 0;
        h->subState = 0;

        if (cvFsGetStat(h->fileHandle) == 3) {
            if (!cvFsIsAvailableRetry(h->fileHandle)) {
                h->stat = 4;
            } else {
                s32 maxRetry = lbl_eu_805E3EA4;
                if (maxRetry >= 0 && h->retryCount >= maxRetry) {
                    h->stat = 4;
                } else if (h->retryCount < 0x7FFFFFFF) {
                    h->retryCount++;
                }
            }
        }
    }
}

extern void* cvFsOpen(const char* name, s32 size, s32 mode);
extern void cvFsClose(void* fh);
extern s32 cvFsGetFileSize(const char* name);

void ADXSTMF_ExecHndl(ADXSTMHndl* h) {
    if (h->subState != 0) goto try_stat_exec;

    /* Handle stop request */
    if (h->stopReq == 1) {
        u8 prev = h->startPending;
        h->stopReq = 0;
        if (prev == 0) {
            h->stat = 1;
        }
    }

    /* Handle close request */
    if (h->closing == 1) {
        if (h->fileHandle) {
            void* fh = h->fileHandle;
            h->fileHandle = NULL;
            cvFsClose(fh);
        }
        ADXCRS_Lock();
        h->closing = 0;
        h->opened = 0;
        ADXCRS_Unlock();
    }

    /* Handle open request */
    ADXCRS_Lock();
    if (h->openReq != 1) {
        ADXCRS_Unlock();
        goto try_stat_exec;
    }
    if (h->closing == 1) goto unlock_and_return;
    int needUnlock = 0;

    if (h->opened == 0) {
        h->opened = 1;
        ADXCRS_Unlock();
        needUnlock = 1;

        if (h->fileHandle == NULL) {
            void* fh = cvFsOpen(h->fileName, h->fileSizeParam, 0);
            h->fileHandle = fh;
            if (fh == NULL) {
                ADXERR_CallErrFunc2_("ADXF: can't open file", h->fileName);
                h->stat = 4;
                h->opened = 0;
                h->openReq = 0;
                goto unlock_and_return;
            }
        }
    }

    if (h->opened != 1) goto try_start_pending;
    if (!needUnlock) ADXCRS_Unlock();

    /* Determine file size */
    if (h->openReq == 1 && h->closing == 1) goto unlock_and_return;

    u32 sectors;
    u32 fileSizeHi, fileSizeLo;

    if (h->fileSizeParam == 0) {
        u32 size = cvFsGetFileSize(h->fileName);
        fileSizeLo = size;
        fileSizeHi = (s32)size >> 31;
        if ((s32)fileSizeHi < 0 || ((s32)fileSizeHi == 0 && (s32)fileSizeLo < 0)) {
            size = cvFsGetFileSize(h->fileName);
            fileSizeLo = size;
            fileSizeHi = (s32)size >> 31;
        }
        sectors = ceil_div2048(fileSizeLo);
    } else {
        cvFsSeek(h->fileHandle, 0, 2);
        u32 pos = cvFsTell(h->fileHandle);
        sectors = pos;
        fileSizeLo = pos << 11;
        fileSizeHi = (s32)fileSizeLo >> 31;
        cvFsSeek(h->fileHandle, 0, 0);
    }

    /* Validate file size */
    if (h->fileSizeHi == 0x80000000 && h->fileSizeLo == 0) {
        h->fileSizeLo = fileSizeLo;
        h->fileSizeHi = fileSizeHi;
        h->fileSectors = sectors;
    }

    if (h->startOffset > sectors) {
        h->startOffset = sectors;
    }

    u32 endPos = h->startOffset + h->fileSectors;
    if (endPos > sectors) {
        h->fileSectors = sectors - h->startOffset;
        u32 s = h->fileSectors;
        h->fileSizeLo = s << 11;
        h->fileSizeHi = ((s32)s >> 31) << 11 | (s >> 21);
    }

    ADXCRS_Enter();
    h->seekPos = 0;
    if (0 > (int)h->fileSectors) {
        h->seekPos = h->fileSectors;
    }
    ADXCRS_Leave();

    h->openReq = 0;

    if (cvFsGetStat(h->fileHandle) == 3) {
        ADXERR_CallErrFunc2_("ADXF: can't open file (stat err)", h->fileName);
        if (h->fileHandle) {
            void* fh = h->fileHandle;
            h->fileHandle = NULL;
            cvFsClose(fh);
        }
        h->stat = 4;
        h->opened = 0;
        h->openReq = 0;
        goto unlock_and_return;
    }

try_start_pending:
    if (h->startPending == 1) {
        h->startPending = 0;
    }

try_stat_exec:
    if (h->stat == 2 && h->opened == 1 && h->openReq == 0) {
        adxstmf_stat_exec(h);
    }
    return;

unlock_and_return:
    return;
}

void ADXSTM_ExecServer(void) {
    ADXCRS_Enter();
    if (SVM_TestAndSet(&lbl_eu_805E3EA0)) {
        s32 i;
        ADXSTMHndl* h = (ADXSTMHndl*)lbl_eu_805E3EA8;
        for (i = 0; i < 40; i++) {
            if ((s32)h->active == 1) {
                ADXSTMF_ExecHndl(h);
            }
            h++;
        }
        lbl_eu_805E3EA0 = 0;
    }
    ADXCRS_Leave();
}

void ADXSTM_ExecFsSvr(void) {
    ADXCRS_Enter();
    cvFsExecServer();
    ADXCRS_Leave();
}

void ADXSTM_ExecFsIdle(void) {
}

int ADXSTM_SetBufSize(ADXSTMHndl* h, int a, int b) {
    ADXCRS_Enter();
    h->bufStartOff = a;
    h->bufFill = b;
    ADXCRS_Leave();
    return 1;
}

s32 ADXSTM_SetReqRdSize(ADXSTMHndl* h, u32 val) {
    ADXCRS_Enter();
    h->reqRdSize = val;
    ADXCRS_Leave();
    return 1;
}

u32 ADXSTM_GetFileLen(ADXSTMHndl* h) {
    u32 result;
    ADXCRS_Enter();
    result = h->fileSizeLo;
    ADXCRS_Leave();
    return result;
}

u64 ADXSTM_GetFileLen64(ADXSTMHndl* h) {
    u32 hi, lo;
    ADXCRS_Enter();
    hi = h->fileSizeHi;
    lo = h->fileSizeLo;
    ADXCRS_Leave();
    return ((u64)hi << 32) | lo;
}

u32 ADXSTM_GetFileSct(ADXSTMHndl* h) {
    u32 result;
    ADXCRS_Enter();
    result = h->fileSectors;
    ADXCRS_Leave();
    return result;
}

void ADXSTM_SetPause(ADXSTMHndl* h, u32 val) {
    ADXCRS_Enter();
    h->pause = val;
    ADXCRS_Leave();
}

void ADXSTM_SetSj(ADXSTMHndl* h, void* sj) {
    u32 bufSize;

    ADXCRS_Enter();
    h->sj = sj;
    ADXCRS_Lock();

    typedef u32 (*GetLenFn)(void*, u32);
    u32 len1 = ((GetLenFn)*((u32*)*(void**)sj + 9))(sj, 1);
    u32 len0 = ((GetLenFn)*((u32*)*(void**)sj + 9))(sj, 0);
    bufSize = len0 + len1;
    h->sjBufSize = bufSize;

    ADXCRS_Unlock();

    h->bufFill = h->sjBufSize;
    h->bufSize = h->sjBufSize;

    ADXCRS_Leave();
}

int ADXSTM_IsOpenedFile(ADXSTMHndl* h) {
    return (s8)h->opened;
}
