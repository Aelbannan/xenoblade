#include "monolib/nand/CNReqtaskCheck.hpp"

static CNReqtaskCheck sTask;

CNReqtask* CNReqtaskCheck::setup(void* p, u32 neededBlocks, u32 neededFiles, u32* answer) {
    Param* param = static_cast<Param*>(p);

    param->neededBlocks = neededBlocks;
    param->neededFiles = neededFiles;
    param->answer = answer;
    param->state = 0;
    return &sTask;
}

int CNReqtaskCheck::update(void* p) {
    Param* param = static_cast<Param*>(p);

    if (sBusy) {
        return 0;
    }
    if (param->state > 0 && sResult < 0) {
        return 2;
    }

    switch (param->state) {
    case 0:
        if (check(param->neededBlocks, param->neededFiles, param->answer) != 0) {
            return 2;
        }
        param->state = 1;
        break;
    case 1:
        param->state = 2;
        break;
    case 2:
        return 1;
    }
    return 0;
}
