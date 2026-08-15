#include <string.h>

#include "monolib/core/CRsrcData.hpp"
#include "monolib/core/CRsrc.hpp"
#include "monolib/util.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "monolib/work/CWorkUtil.hpp"

extern "C" {
bool hasChild__FP11CWorkThread(CWorkThread* pThread);
// Work-thread display name pool: "CRsrcData" (not empty string).
// Vtable/RTTI targets referenced by the CRsrcData data definitions below.
extern const char lbl_eu_80522534[];
extern void __dt__9CRsrcDataFv(void* self, int flags);
extern void wkUpdate__9CRsrcDataFv();
extern bool wkStandbyLogin__9CRsrcDataFv();
extern bool wkStandbyLogout__9CRsrcDataFv();
// IWorkEvent virtual handlers (weak defaults live in kyoshin/CGame.cpp).
extern int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
extern int OnFileEvent__10IWorkEventFP10CEventFile(void*);
extern int WorkEvent3__10IWorkEventFPv(void*);
extern int WorkEvent4__10IWorkEventFv();
extern void OnPauseTrigger__10IWorkEventFb(int);
extern int WorkEvent6__10IWorkEventFv();
extern int WorkEvent7__10IWorkEventFv();
extern int WorkEvent8__10IWorkEventFv();
extern int WorkEvent9__10IWorkEventFv();
extern int WorkEvent10__10IWorkEventFv();
extern int WorkEvent11__10IWorkEventFv();
extern int WorkEvent12__10IWorkEventFv();
extern int WorkEvent13__10IWorkEventFv();
extern int WorkEvent14__10IWorkEventFv();
extern int WorkEvent15__10IWorkEventFv();
extern int WorkEvent16__10IWorkEventFv();
extern int WorkEvent17__10IWorkEventFv();
extern int WorkEvent18__10IWorkEventFv();
extern int WorkEvent19__10IWorkEventFv();
extern int WorkEvent20__10IWorkEventFv();
extern int WorkEvent21__10IWorkEventFv();
extern int WorkEvent22__10IWorkEventFv();
extern int WorkEvent23__10IWorkEventFv();
extern int WorkEvent24__10IWorkEventFv();
extern int WorkEvent25__10IWorkEventFv();
extern int WorkEvent26__10IWorkEventFv();
extern int WorkEvent27__10IWorkEventFv();
extern int WorkEvent28__10IWorkEventFv();
extern int WorkEvent29__10IWorkEventFv();
extern int WorkEvent30__10IWorkEventFv();
extern void WorkEvent31__10IWorkEventFv();
// CWorkThread work-method vtable slots.
extern void wkRender__11CWorkThreadFv();
extern void wkRenderAfter__11CWorkThreadFv();
extern void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);
// RTTI locators (defined by kyoshin/CGame.cpp, .sdata 0x806618A0/0x806618A8).
extern void* __RTTI__10IWorkEvent;
extern void* __RTTI__11CWorkThread;
mtl::ALLOC_HANDLE getWorkMem__17CWorkThreadSystemFv();
void* allocate__Q23mtl10MemManagerFUlUl(u32 size, mtl::ALLOC_HANDLE handle);
void entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(CWorkThread* ths, CWorkThread* parent,
                                                         bool prepend);
void setRsrcFile__9CRsrcDataFPCcPvPvUlb(CRsrcData* ths, const char* name, void* path, void* data,
                                        u32 length, bool flag);
CRsrcData* __ct__CRsrcData(CRsrcData* ths, const char* name, CWorkThread* parent);
CWorkThread* __ct__11CWorkThreadFPCcP11CWorkThreadi(CWorkThread* ths, const char* name,
                                                    CWorkThread* parent, int capacity);
}

// --- Blob monolibdata1.s dissolve: this TU owns .data 0x8056B360-0x8056B418,
// .rodata 0x80522528-0x80522540, .sdata 0x80663550-0x80663558. ---

// RTTI name strings (.rodata).
extern const char lbl_eu_80522528[];
// RTTI locator (.sdata, 8 bytes): { name, base-list }.
extern void* lbl_eu_80663550[2];

const char lbl_eu_80522528[] = "CRsrcData";
const char lbl_eu_80522534[] = "CRsrcData";

// CRsrcData vtable (.data, 0xA0): [rtti, 0, dtor, IWorkEvent 1-31, wkUpdate
// (CRsrcData), wkRender/wkRenderAfter (CWorkThread slots), wkStandbyLogin/Logout
// (CRsrcData), wkStandbyExceptionRetry].
u32 lbl_eu_8056B360[0xA0 / 4] = {
    (u32)&lbl_eu_80663550, 0, (u32)&__dt__9CRsrcDataFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__9CRsrcDataFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__9CRsrcDataFv,
    (u32)&wkStandbyLogout__9CRsrcDataFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

// RTTI base list (.data, 0x18): [IWorkEvent, 0, CWorkThread, 0, 0, 0].
u32 lbl_eu_8056B400[6] = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};

void* lbl_eu_80663550[2] = { (void*)lbl_eu_80522528, (void*)lbl_eu_8056B400 };

extern "C" CRsrcData* __ct__CRsrcData(CRsrcData* self, const char* pName,
                                          CWorkThread* pParent) {
    __ct__11CWorkThreadFPCcP11CWorkThreadi(self, pName, pParent, 0);
    *(void**)self = lbl_eu_8056B360;
    self->mName[0] = 0;
    self->mNameLength = 0;
    self->mAltPath[0] = 0;
    self->mAltPathLength = 0;
    self->mPath[0] = 0;
    self->mPathLength = 0;
    self->mCacheData = nullptr;
    self->mCacheLength = 0;
    self->mRefCount = 0;
    self->mFlags4DC = 0;
    self->unk4E0 = 0;
    self->unk4E2 = 0;
    self->unk4E4 = 0;
    self->mType = CWorkThread::THREAD_CRSRCDATA;
    return self;
}

#pragma optimize_for_size on
CRsrcData::~CRsrcData() {
    // MWCC virtual dtors already null-check `this`; an extra guard adds a dead beq.
    void* cache = mCacheData;
    *(void**)this = lbl_eu_8056B360;

    if (cache != nullptr) {
        mtl::MemManager::deallocate(cache);
        mCacheData = nullptr;
    }
}
#pragma optimize_for_size reset

#pragma dont_inline on
void CRsrcData::destruct(int arg) {
    if (mRefCount != 0) {
        mRefCount--;
    }

    if (mRefCount != 0) {
        return;
    }

    if (arg == 0) {
        return;
    }

    wkSetEvent(EVT_NONE);
}
#pragma dont_inline off

bool CRsrcData::releaseCache(const void* data) {
    if (mCacheData == data) {
        wkSetEvent(EVT_NONE);
        return true;
    }

    return false;
}

void CRsrcData::setRsrcFile(const char* name, void* path, void* data, u32 length, bool flag) {
    mNameLength = strlen(name);
    strcpy(mName, name);

    if (flag) {
        mAltPathLength = strlen(mName);
        strcpy(mAltPath, mName);
    } else {
        // Retail walks the already-copied mName buffer, not the original pointer.
        const char* filePtr = ml::CPathUtil::getFilePtrFromPath(mName);
        mAltPathLength = strlen(filePtr);
        strcpy(mAltPath, filePtr);
    }

    unk4E0 = flag;
    mPathLength = strlen(static_cast<const char*>(path));
    strcpy(mPath, static_cast<const char*>(path));

    mCacheData = data;
    mCacheLength = length;
    mRefCount = 0;
    mFlags4DC |= 2;
    if (data == nullptr) {
        mFlags4DC |= 1;
    }
}

// Retail symbol is void-returning build__9CRsrcDataFPvPCcPvPvUlb (no C++ static twin).
extern "C" void build__9CRsrcDataFPvPCcPvPvUlb(void* parent, const char* name, void* arg2, void* data,
                                                 u32 length, bool flag) {
    const char* threadName;
    mtl::ALLOC_HANDLE handle;
    CRsrcData* rsrc;

    threadName = lbl_eu_80522534;
    handle = getWorkMem__17CWorkThreadSystemFv();
    rsrc = static_cast<CRsrcData*>(allocate__Q23mtl10MemManagerFUlUl(0x4E8, handle));

    if (rsrc != nullptr) {
        // Explicit retail-named constructor entry point.
        rsrc = __ct__CRsrcData(rsrc, threadName, static_cast<CWorkThread*>(parent));
    }

    entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(rsrc, static_cast<CWorkThread*>(parent),
                                                        false);
    setRsrcFile__9CRsrcDataFPCcPvPvUlb(rsrc, name, arg2, data, length, flag);
}

// int (not bool): MWCC bool-return converts cntlzw/srwi via neg/or (+0x18).
int CRsrcData::isSameName(const char* name) const {
    int cmp;

    if (unk4E0 != 0) {
        cmp = strcmp(mAltPath, name);
        return static_cast<u32>(__cntlzw(cmp)) >> 5;
    }

    cmp = strcmp(mAltPath, ml::CPathUtil::getFilePtrFromPath(name));
    return static_cast<u32>(__cntlzw(cmp)) >> 5;
}

void CRsrcData::wkUpdate() {
    s16 t = unk4E2;

    if (t == 0) {
        return;
    }

    t = t - 1;
    unk4E2 = t;
    if (t > 0) {
        return;
    }

    destruct((mFlags4DC >> 3) & 1);
}

bool CRsrcData::wkStandbyLogin() {
    if (hasChild__FP11CWorkThread(this)) {
        goto fail;
    }

    if ((mFlags4DC & 2) == 0) {
        goto fail;
    }

    return CWorkThread::wkStandbyLogin();

fail:
    return false;
}

bool CRsrcData::wkStandbyLogout() {
    u32 flags;

    if (hasChild__FP11CWorkThread(this)) {
        goto fail;
    }

    flags = mFlags4DC;
    if ((flags & 2) != 0) {
        if (mRefCount == 0) {
            goto success;
        }
    }

    if ((flags & 1) == 0) {
        goto fail;
    }

success:
    return CWorkThread::wkStandbyLogout();

fail:
    return false;
}
