#pragma once

#include <types.h>
#include "monolib/core/CRsrc.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "monolib/work/CWorkUtil.hpp"

class CRsrcData {
public:
    CRsrcData();
    virtual ~CRsrcData();
    void destruct();
    void releaseCache();
    void setRsrcFile();
    void build();
    void isSameName() const;
    void wkUpdate();
    void wkStandbyLogin();
    void wkStandbyLogout();

    // TODO: add fields
};

