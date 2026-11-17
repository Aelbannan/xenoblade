#include "monolib/nand/CNReqtaskRemove.hpp"

#include <string.h>

static CNReqtaskRemove sTask;

CNReqtask* CNReqtaskRemove::setup(void* p, const char* name, bool share) {
    Param* param = static_cast<Param*>(p);

    strcpy(param->name, name);
    param->share = share;
    param->state = 0;
    return &sTask;
}

int CNReqtaskRemove::update(void* p) {
    Param* param = static_cast<Param*>(p);

    if (sBusy) {
        return 0;
    }
    if (param->state > 0 && sResult < 0) {
        return 2;
    }

    switch (param->state) {
    case 0:
        if (remove(getPath(param->name, param->share)) != 0) {
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
