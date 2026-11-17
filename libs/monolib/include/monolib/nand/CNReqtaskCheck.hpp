#pragma once

#include "monolib/nand/CNReqtask.hpp"

// Asks the NAND whether the given number of blocks and files can be created.
class CNReqtaskCheck : public CNReqtask {
public:
    struct Param {
        u32 neededBlocks;
        u32 neededFiles;
        u32* answer;
        s8 state;
    };

    virtual int update(void* param);

    static CNReqtask* setup(void* param, u32 neededBlocks, u32 neededFiles, u32* answer);
};
