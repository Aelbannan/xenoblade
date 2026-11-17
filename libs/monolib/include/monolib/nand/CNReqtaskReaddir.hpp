#pragma once

#include "monolib/nand/CNReqtask.hpp"

// Lists the entries of the home (or share) directory.
class CNReqtaskReaddir : public CNReqtask {
public:
    struct Param {
        char* nameList;
        u32 size;
        u32* num;
        bool share;
        s8 state;
    };

    virtual int update(void* param);

    static CNReqtask* setup(void* param, char* nameList, u32 size, u32* num, bool share);
};
