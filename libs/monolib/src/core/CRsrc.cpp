#include "monolib/core/CRsrc.hpp"
#include "monolib/core/CRsrcData.hpp"
#include "monolib/work/CWorkThread.hpp"
#include "monolib/work/CWorkUtil.hpp"
#include "monolib/work/UnkStruct_80438AF0.hpp"
#include "monolib/util/reslist.hpp"

// Retail CRsrc::entry stores the name at +0x1C8 and length at +0x2C8
// (header CRsrcData::mName is documented at 0x1C4; keep a local view).
struct RsrcNameOverlay {
    u8 pad_00[0x1C4];
    u32 id;           // 0x1C4
    char name[0x100]; // 0x1C8
    u32 nameLen;      // 0x2C8
};

extern "C" {
// Data definitions moved into this TU (blob monolibdata1d dissolve):
// .sbss sRsrcCount (s16, 2 bytes) and .bss sRsrcPointerList (0x40).
s16 sRsrcCount;  // name-guess: loop bound for sRsrcPointerList__5CRsrc
s16 g_CRsrcSbssPad1;  /* pad to 0x806655A8-B0 */
s16 g_CRsrcSbssPad2;
s16 g_CRsrcSbssPad3;
CRsrcData* sRsrcPointerList__5CRsrc[16];

CRsrcData* convertToRsrcData__5CRsrcFP11CWorkThread(CWorkThread* pThread);
void build__9CRsrcDataFPvPCcPvPvUlb(void* parent, const char* name, void* arg2, void* data, u32 length,
                                    bool flag);
bool releaseCache__9CRsrcDataFPCv(CRsrcData* ths, const void* data);
int isSameName__9CRsrcDataCFPCc(CRsrcData* ths, const char* name);
bool isExistData__5CRsrcFPCv(const void* data);
bool releaseCache__5CRsrcFPCv(const void* data);
size_t strlen(const char* str);
char* strcpy(char* dst, const char* src);
UnkStruct_80438AF0* getData__9CWorkUtilFP18UnkStruct_80438AF0(UnkStruct_80438AF0* p);
}

CRsrcData* CRsrc::convertToRsrcData(CWorkThread* pThread) {
    if (pThread == nullptr) {
        return nullptr;
    }

    if (pThread->mType == CWorkThread::THREAD_CRSRCDATA) {
        return static_cast<CRsrcData*>(pThread);
    }

    return nullptr;
}

#pragma optimize_for_size on
extern "C" bool releaseCacheLocal__5CRsrcFPCv(CWorkThread* parent, const void* data) {
    _reslist_node<CWorkThread*>* node;

    // Retail: seed next from head->mNext, bottom-tested with per-iter lwz 0x60(parent).
    node = parent->mChildren.mStartNodePtr->mNext;
    goto check;
loop:
    if (releaseCache__9CRsrcDataFPCv(convertToRsrcData__5CRsrcFP11CWorkThread(node->mItem),
                                      data)) {
        return true;
    }
    node = node->mNext;
check:
    if (node != parent->mChildren.mStartNodePtr) {
        goto loop;
    }

    return false;
}
#pragma optimize_for_size reset

bool CRsrc::entry(void* parent, const char* name, void* arg2, void* data, u32 length, bool flag) {
    if (isExistData__5CRsrcFPCv(data)) {
        return true;
    }

    build__9CRsrcDataFPvPCcPvPvUlb(parent, name, arg2, data, length, flag);
    RsrcNameOverlay* rsrc = (RsrcNameOverlay*)parent;
    rsrc->nameLen = strlen(name);
    strcpy(rsrc->name, name);

    return true;
}

extern "C" bool isExistFile__5CRsrcFPCcPPvPUi(CWorkThread* parent, const char* name, void** outData,
                                              u32* outLength) {
    _reslist_node<CWorkThread*>* node;
    CRsrcData* rsrcData;

    node = parent->mChildren.mStartNodePtr->mNext;

    while (node != parent->mChildren.mStartNodePtr) {
        rsrcData = convertToRsrcData__5CRsrcFP11CWorkThread(node->mItem);

        if ((rsrcData->mFlags & CWorkThread::THREAD_FLAG_NO_EVENT) == 0) {
            if (isSameName__9CRsrcDataCFPCc(rsrcData, name)) {
                if (outData != nullptr) {
                    *outData = rsrcData->mCacheData;
                }

                if (outLength != nullptr) {
                    *outLength = rsrcData->mCacheLength;
                }

                return true;
            }
        }

        node = node->mNext;
    }

    return false;
}

bool hasChild(CWorkThread* pThread) {
    _reslist_node<CWorkThread*>* head;
    _reslist_node<CWorkThread*>* first;
    u32 diff;
    u32 diff2;

    head = pThread->mChildren.mStartNodePtr;
    first = head->mNext;
    diff = (u32)head - (u32)first;
    diff2 = (u32)first - (u32)head;

    return (diff | diff2) >> 31;
}

CRsrcData* CRsrc::getRsrc(u32 id) {
    CRsrcData* entry;
    s16 index;
    u32 off;

    // Decl order: entry before index -> retail r6/r7 coloring.
    index = 0;
    goto check;
loop:
    off = (u32)index;
    entry = sRsrcPointerList__5CRsrc[off];
    if (id == ((RsrcNameOverlay*)entry)->id) {
        return entry;
    }
    index++;
check:
    if (index < sRsrcCount) {
        goto loop;
    }

    return nullptr;
}

#pragma optimize_for_size on
extern "C" bool releaseCache__5CRsrcFPCv(const void* data) {
    CRsrcData** listBase;
    s16 index;

    listBase = sRsrcPointerList__5CRsrc;
    index = 0;
    goto check;
loop:
    if (releaseCacheLocal__5CRsrcFPCv((CWorkThread*)listBase[index], data)) {
        return true;
    }
    index++;
check:
    if (index < sRsrcCount) {
        goto loop;
    }

    return false;
}

extern "C" bool isExistData__5CRsrcFPCv(const void* data) {
    CRsrcData** listBase;
    s16 index;

    listBase = sRsrcPointerList__5CRsrc;
    index = 0;
    goto check;
loop:
    if (isExistDataLocal__5CRsrcFPCv((CWorkThread*)listBase[index], data)) {
        return true;
    }
    index++;
check:
    if (index < sRsrcCount) {
        goto loop;
    }

    return false;
}
#pragma optimize_for_size reset

#pragma optimize_for_size on
extern "C" bool isExistDataLocal__5CRsrcFPCv(CWorkThread* parent, const void* data) {
    _reslist_node<CWorkThread*>* node;
    CRsrcData* rsrcData;
    int flag;

    node = reinterpret_cast<_reslist_node<CWorkThread*>*>(
        getData__9CWorkUtilFP18UnkStruct_80438AF0(
            reinterpret_cast<UnkStruct_80438AF0*>(&parent->mChildren)));
    goto check;
loop:
    rsrcData = convertToRsrcData__5CRsrcFP11CWorkThread(node->mItem);
    if (rsrcData->mCacheData == data) {
        // Retail: match when cache==data and NOT (non-null && refCount==0).
        flag = 0;
        if (rsrcData->mCacheData != nullptr) {
            if (rsrcData->mRefCount == 0) {
                flag = 1;
            }
        }
        if (flag == 0) {
            return true;
        }
    }
    node = node->mNext;
check:
    if (node != parent->mChildren.mStartNodePtr) {
        goto loop;
    }

    return false;
}
#pragma optimize_for_size reset
