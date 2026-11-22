#include <string.h>

#include "monolib/core/CRsrcData.hpp"
#include "monolib/core/CRsrc.hpp"
#include "monolib/util.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "monolib/work/CWorkUtil.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/data_vtables.hpp"

extern "C" {
// Retail's (unmangled) inlined-ctor entry point, defined below.
CRsrcData* __ct__CRsrcData(CRsrcData* ths, const char* name, CWorkThread* parent);
// Base ctor called on raw storage by __ct__CRsrcData.
CWorkThread* __ct__11CWorkThreadFPCcP11CWorkThreadi(CWorkThread* ths, const char* name,
                                                    CWorkThread* parent, int capacity);
}

// --- Blob monolibdata1.s dissolve: this TU owns .data 0x8056B360-0x8056B418,
// .rodata 0x80522528-0x80522540, .sdata 0x80663550-0x80663558. ---

// RTTI name strings (.rodata).
extern const char lbl_eu_80522528[];
// RTTI locator (.sdata, 8 bytes): { name, base-list }.
extern void* __RTTI__9CRsrcData[2];

const char lbl_eu_80522528[12] = {0x43,0x52,0x73,0x72,0x63,0x44,0x61,0x74,0x61,0x00,0x00,0x00};  /* "CRsrcData" + pad */

// CRsrcData vtable (.data, 0xA0): [rtti, 0, dtor, IWorkEvent 1-31, wkUpdate
// (CRsrcData), wkRender/wkRenderAfter (CWorkThread slots), wkStandbyLogin/Logout
// (CRsrcData), wkStandbyExceptionRetry].
u32 lbl_eu_8056B360[0xA0 / 4] = {
    (u32)&__RTTI__9CRsrcData, 0, (u32)&__dt__9CRsrcDataFv,
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

void* __RTTI__9CRsrcData[2] = { (void*)lbl_eu_80522528, (void*)lbl_eu_8056B400 };

extern "C" CRsrcData* __ct__CRsrcData(CRsrcData* ths, const char* pName,
                                          CWorkThread* pParent) {
    __ct__11CWorkThreadFPCcP11CWorkThreadi(ths, pName, pParent, 0);
    *(u32**)ths = lbl_eu_8056B360; // novtable class: vptr of the hand-laid vtable
    ths->mName[0] = 0;
    ths->mNameLength = 0;
    ths->mAltPath[0] = 0;
    ths->mAltPathLength = 0;
    ths->mPath[0] = 0;
    ths->mPathLength = 0;
    ths->mCacheData = nullptr;
    ths->mCacheLength = 0;
    ths->mRefCount = 0;
    ths->mFlags4DC = 0;
    ths->unk4E0 = 0;
    ths->unk4E2 = 0;
    ths->unk4E4 = 0;
    ths->mType = CWorkThread::THREAD_CRSRCDATA;
    return ths;
}

// Load-bearing: without optimize_for_size MWCC grows the dtor by 8 bytes (retail size 0x7C).
#pragma optimize_for_size on
CRsrcData::~CRsrcData() {
    // MWCC virtual dtors already null-check `this`; an extra guard adds a dead beq.
    void* cache = mCacheData;
    *(u32**)this = lbl_eu_8056B360;

    if (cache != nullptr) {
        mtl::MemManager::deallocate(cache);
        mCacheData = nullptr;
    }
}
#pragma optimize_for_size reset

// Load-bearing: retail wkUpdate calls destruct out of line; -ipa file would inline it.
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

void CRsrcData::build(void* pParent, const char* name, void* path, void* data, u32 length, bool flag) {
    const char* threadName = "CRsrcData";
    mtl::ALLOC_HANDLE handle = CWorkThreadSystem::getWorkMem();
    CRsrcData* rsrc = static_cast<CRsrcData*>(mtl::MemManager::allocate(sizeof(CRsrcData), handle));

    if (rsrc != nullptr) {
        rsrc = __ct__CRsrcData(rsrc, threadName, static_cast<CWorkThread*>(pParent));
    }

    CWorkUtil::entryWork(rsrc, static_cast<CWorkThread*>(pParent), false);
    rsrc->setRsrcFile(name, path, data, length, flag);
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
    if (!hasChild(this) && (mFlags4DC & 2) != 0) {
        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CRsrcData::wkStandbyLogout() {
    if (!hasChild(this) && (((mFlags4DC & 2) != 0 && mRefCount == 0) || (mFlags4DC & 1) != 0)) {
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}
