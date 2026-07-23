#include <string.h>

#include "monolib/core/CRsrcData.hpp"
#include "monolib/core/CRsrc.hpp"
#include "monolib/util.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "monolib/work/CWorkUtil.hpp"

extern "C" {
bool hasChild__FP11CWorkThread(CWorkThread* pThread);
// Work-thread display name pool: "CRsrcData" (not empty string).
char lbl_eu_80522534[];
mtl::ALLOC_HANDLE getWorkMem__17CWorkThreadSystemFv();
void* allocate__Q23mtl10MemManagerFUlUl(u32 size, mtl::ALLOC_HANDLE handle);
void entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(CWorkThread* ths, CWorkThread* parent,
                                                         bool prepend);
void setRsrcFile__9CRsrcDataFPCcPvPvUlb(CRsrcData* ths, const char* name, void* path, void* data,
                                        u32 length, bool flag);
// Pre-postprocess ctor name; reloc rename maps this to retail __ct__CRsrcData.
CRsrcData* __ct__9CRsrcDataFPCcP11CWorkThread(CRsrcData* ths, const char* name,
                                              CWorkThread* parent);
}

CRsrcData::CRsrcData(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 0) {
    mName[0] = 0;
    mNameLength = 0;
    mAltPath[0] = 0;
    mAltPathLength = 0;
    mPath[0] = 0;
    mPathLength = 0;
    mCacheData = nullptr;
    mCacheLength = 0;
    mRefCount = 0;
    mFlags4DC = 0;
    unk4E0 = 0;
    unk4E2 = 0;
    unk4E4 = 0;
    mType = THREAD_CRSRCDATA;
}

#pragma optimize_for_size on
CRsrcData::~CRsrcData() {
    // MWCC virtual dtors already null-check `this`; an extra guard adds a dead beq.
    void* cache = mCacheData;

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
        // Explicit ctor (not placement new) - avoids MWCC's extra null check.
        rsrc = __ct__9CRsrcDataFPCcP11CWorkThread(rsrc, threadName,
                                                  static_cast<CWorkThread*>(parent));
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
