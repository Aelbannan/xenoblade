#pragma once

#include "monolib/nand/CNReqtask.hpp"

// Creates a directory; an already existing one is not an error.
class CNReqtaskCreatedir : public CNReqtask {
public:
    struct Param {
        char name[13];
        u8 perm;
        bool share;
        s8 state;
    };

    virtual int update(void* param);

    static CNReqtask* setup(void* param, const char* name, u8 perm, bool share);
};
