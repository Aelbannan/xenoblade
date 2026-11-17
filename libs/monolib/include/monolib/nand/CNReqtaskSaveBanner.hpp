#pragma once

#include "monolib/nand/CNReqtask.hpp"

class CNBanner;

// Writes banner.bin, unless the home directory already has one.
class CNReqtaskSaveBanner : public CNReqtask {
public:
    struct Param {
        CNBanner* banner;
        s8 state;
        u32 length;
        bool share;
    };

    virtual int update(void* param);
    virtual void close(void* param);

    static CNReqtask* setup(void* param, CNBanner* banner, bool share);

private:
    static const char* getTempPath(Param* param);
};
