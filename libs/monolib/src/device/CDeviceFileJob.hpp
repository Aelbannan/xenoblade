#pragma once

#include <types.h>

class CDeviceFileJob {
public:
    virtual ~CDeviceFileJob();

    // TODO: add fields
    void cancel(const char*);
    void cancel(CFileHandle*);
    void CDeviceFileJob_UnkVirtualFunc1();
};

