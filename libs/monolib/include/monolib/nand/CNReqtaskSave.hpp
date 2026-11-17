#pragma once

#include "monolib/nand/CNReqtask.hpp"

// Writes a buffer to a temporary file and moves it over the real one.
class CNReqtaskSave : public CNReqtask {
public:
    struct Param {
        char name[16];
        const void* buf;
        u32 size;
        u8 perm;
        bool share;
        s8 state;
    };

    virtual int update(void* param);

    static CNReqtask* setup(void* param, const char* name, const void* buf, u32 size, u8 perm,
                            bool share);

private:
    static const char* getTempPath(Param* param);
};
