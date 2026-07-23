#include "monolib/core/CRsrc.hpp"
#include "monolib/core/CRsrcData.hpp"
#include "monolib/work/CWorkThread.hpp"
#include "monolib/work/CWorkUtil.hpp"
#include "monolib/work/UnkStruct_80438AF0.hpp"
#include "monolib/util/reslist.hpp"

extern "C" {
s16 lbl_eu_806655A8;
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
#pragma dont_inline on
extern "C" bool releaseCacheLocal__5CRsrcFPCv(CWorkThread* parent, const void* data) {
    _reslist_node<CWorkThread*>* head;
    _reslist_node<CWorkThread*>* node;

    head = parent->mChildren.mStartNodePtr;
    node = head->mNext;

    while (node != head) {
        if (releaseCache__9CRsrcDataFPCv(convertToRsrcData__5CRsrcFP11CWorkThread(node->mItem), data)) {
            return true;
        }
        node = node->mNext;
    }

    return false;
}
#pragma dont_inline off
#pragma optimize_for_size reset

#pragma dont_inline on
bool CRsrc::entry(void* parent, const char* name, void* arg2, void* data, u32 length, bool flag) {
    if (isExistData__5CRsrcFPCv(data)) {
        return true;
    }

    build__9CRsrcDataFPvPCcPvPvUlb(parent, name, arg2, data, length, flag);
    *(u32*)((u8*)parent + 0x2C8) = strlen(name);
    strcpy((char*)((u8*)parent + 0x1C8), name);

    return true;
}
#pragma dont_inline off

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

    head = *(_reslist_node<CWorkThread*>**)((u8*)pThread + 0x60);
    first = head->mNext;
    diff = (u32)head - (u32)first;
    diff2 = (u32)first - (u32)head;

    return (diff | diff2) >> 31;
}

CRsrcData* CRsrc::getRsrc(u32 id) {
    s16 index;
    s16 count;
    CRsrcData* entry;

    count = lbl_eu_806655A8;
    index = 0;

    while (index < count) {
        entry = sRsrcPointerList__5CRsrc[index];

        // List slots are parent work threads; retail compares u32 @ 0x1C4.
        if (*(u32*)((u8*)entry + 0x1C4) == id) {
            return entry;
        }

        index++;
    }

    return nullptr;
}

#pragma optimize_for_size on
#pragma dont_inline on
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
    if (index < lbl_eu_806655A8) {
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
    if (index < lbl_eu_806655A8) {
        goto loop;
    }

    return false;
}
#pragma dont_inline off
#pragma optimize_for_size reset

extern "C" bool isExistDataLocal__5CRsrcFPCv(CWorkThread* parent, const void* data) {
    _reslist_node<CWorkThread*>* node;
    CRsrcData* rsrcData;

    node = reinterpret_cast<_reslist_node<CWorkThread*>*>(
        CWorkUtil::func_80438AF0(reinterpret_cast<UnkStruct_80438AF0*>(&parent->mChildren)));

    while (node != parent->mChildren.mStartNodePtr) {
        rsrcData = convertToRsrcData__5CRsrcFP11CWorkThread(node->mItem);

        if (rsrcData->mCacheData == data) {
            if (data == nullptr || rsrcData->mRefCount == 0) {
                return true;
            }
        }

        node = node->mNext;
    }

    return false;
}
