#pragma once

#include "monolib/nand/CNReqtask.hpp"

// Deletes a file.
class CNReqtaskRemove : public CNReqtask {
public:
    struct Param {
        char name[13];
        s8 state;
        bool share;
    };

    virtual int update(void* param);

    static CNReqtask* setup(void* param, const char* name, bool share);
};
