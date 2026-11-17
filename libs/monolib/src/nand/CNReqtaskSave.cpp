#include "monolib/nand/CNReqtaskSave.hpp"

#include <string.h>

#include "monolib/util/FixStr.hpp"

static CNReqtaskSave sTask;
static const char* sTempDir = "/tmp/";

CNReqtask* CNReqtaskSave::setup(void* p, const char* name, const void* buf, u32 size, u8 perm,
                                bool share) {
    Param* param = static_cast<Param*>(p);

    strcpy(param->name, name);
    param->buf = buf;
    param->size = size;
    param->perm = perm;
    param->share = share;
    param->state = 0;
    return &sTask;
}

int CNReqtaskSave::update(void* p) {
    Param* param = static_cast<Param*>(p);

    if (sBusy) {
        return 0;
    }
    if (param->state >= 2 && sResult < 0) {
        return 2;
    }

    switch (param->state) {
    case 0:
        if (open(getTempPath(param), NAND_ACCESS_WRITE) != 0) {
            return 2;
        }
        param->state = 1;
        break;
    case 1:
        if (sResult != 0) {
            if (sResult == NAND_RESULT_NOEXISTS) {
                if (create(getTempPath(param), param->perm, 0) != 0) {
                    return 2;
                }
                param->state = 6;
            } else {
                return 2;
            }
        } else {
            if (write(param->buf, param->size) != 0) {
                return 2;
            }
            param->state = 2;
        }
        break;
    case 2:
        if (closeFile() != 0) {
            return 2;
        }
        param->state = 3;
        break;
    case 3:
        if (move(getTempPath(param), getHomeDir(param->share)) != 0) {
            return 2;
        }
        param->state = 4;
        break;
    case 4:
        param->state = 5;
        break;
    case 5:
        return 1;
    case 6:
        param->state = 0;
        break;
    }
    return 0;
}

const char* CNReqtaskSave::getTempPath(Param* param) {
    static ml::FixStr<32> sPath;

    sPath.format("%s%s", sTempDir, param->name);
    return sPath.c_str();
}
