#pragma once

#include <types.h>
#include "monolib/core/CRsrcData.hpp"
#include "monolib/work/CWorkThread.hpp"
#include "monolib/work/CWorkUtil.hpp"

class CRsrc {
public:
    void convertToRsrcData();
    void releaseCacheLocal();
    void entry();
    void isExistFile();
    void getRsrc();
    void releaseCache();
    void isExistData();
    void isExistDataLocal();

    // TODO: add fields
};

