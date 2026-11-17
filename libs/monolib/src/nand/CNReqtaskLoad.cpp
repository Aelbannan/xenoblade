#include "monolib/nand/CNReqtaskLoad.hpp"

#include <string.h>

#include <revolution/OS.h>

static CNReqtaskLoad sTask;

CNReqtask* CNReqtaskLoad::setup(void* p, const char* name, void* buf, u32 size, bool share) {
    Param* param = static_cast<Param*>(p);

    strcpy(param->name, name);
    param->buf = buf;
    param->size = size;
    param->share = share;
    param->state = 0;
    return &sTask;
}

int CNReqtaskLoad::update(void* p) {
    Param* param = static_cast<Param*>(p);

    if (sBusy) {
        return 0;
    }
    if (param->state > 0 && sResult < 0) {
        return 2;
    }

    switch (param->state) {
    case 0:
        if (open(getPath(param->name, param->share), NAND_ACCESS_READ) != 0) {
            return 2;
        }
        param->state = 1;
        break;
    case 1:
        if (read(param->buf, param->size) != 0) {
            return 2;
        }
        param->state = 2;
        break;
    case 2:
        if (closeFile() != 0) {
            return 2;
        }
        param->state = 3;
        break;
    case 3:
        DCFlushRange(param->buf, param->size);
        param->state = 4;
        break;
    case 4:
        return 1;
    }
    return 0;
}
