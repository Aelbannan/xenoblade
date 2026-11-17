#include "monolib/nand/CNReqtaskCreatedir.hpp"

#include <string.h>

static CNReqtaskCreatedir sTask;

CNReqtask* CNReqtaskCreatedir::setup(void* p, const char* name, u8 perm, bool share) {
    Param* param = static_cast<Param*>(p);

    strcpy(param->name, name);
    param->perm = perm;
    param->share = share;
    param->state = 0;
    return &sTask;
}

int CNReqtaskCreatedir::update(void* p) {
    Param* param = static_cast<Param*>(p);

    if (sBusy) {
        return 0;
    }

    switch (param->state) {
    case 0:
        if (createDir(getPath(param->name, param->share), param->perm, 0) != 0) {
            return 2;
        }
        param->state = 1;
        break;
    case 1:
        if (sResult != 0 && sResult != NAND_RESULT_EXISTS) {
            return 2;
        }
        param->state = 2;
        break;
    case 2:
        return 1;
    }
    return 0;
}
