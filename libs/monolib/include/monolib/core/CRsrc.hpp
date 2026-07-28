#pragma once

#include <types.h>

#include "monolib/work/CWorkThread.hpp"

class CRsrcData;

class CRsrc {
public:
    static CRsrcData* convertToRsrcData(CWorkThread* pThread);
    static bool entry(void* parent, const char* name, void* arg2, void* data, u32 length, bool flag);
    static CRsrcData* getRsrc(u32 id);

    bool releaseCacheLocal(CWorkThread* parent, const void* data);
    bool isExistFile(CWorkThread* parent, const char* name, void** outData, u32* outLength);
    bool isExistDataLocal(CWorkThread* parent, const void* data);
    bool releaseCache(const void* data);
    bool isExistData(const void* data);
};

extern "C" {
bool releaseCacheLocal__5CRsrcFPCv(CWorkThread* parent, const void* data);
bool isExistFile__5CRsrcFPCcPPvPUi(CWorkThread* parent, const char* name, void** outData, u32* outLength);
bool isExistDataLocal__5CRsrcFPCv(CWorkThread* parent, const void* data);
bool releaseCache__5CRsrcFPCv(const void* data);
bool isExistData__5CRsrcFPCv(const void* data);
}
