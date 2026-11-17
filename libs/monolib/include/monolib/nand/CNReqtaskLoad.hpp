#pragma once

#include "monolib/nand/CNReqtask.hpp"

// Reads a whole file into a buffer.
class CNReqtaskLoad : public CNReqtask {
public:
    struct Param {
        char name[16];
        void* buf;
        u32 size;
        bool share;
        s8 state;
    };

    virtual int update(void* param);

    static CNReqtask* setup(void* param, const char* name, void* buf, u32 size, bool share);
};
