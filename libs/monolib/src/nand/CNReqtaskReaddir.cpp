#include "monolib/nand/CNReqtaskReaddir.hpp"

static CNReqtaskReaddir sTask;

CNReqtask* CNReqtaskReaddir::setup(void* p, char* nameList, u32 size, u32* num, bool share) {
    Param* param = static_cast<Param*>(p);

    param->nameList = nameList;
    param->size = size;
    param->num = num;
    param->share = share;
    param->state = 0;
    *nameList = '\0';
    *num = 0;
    return &sTask;
}

int CNReqtaskReaddir::update(void* p) {
    Param* param = static_cast<Param*>(p);

    if (sBusy) {
        return 0;
    }

    int state = param->state;
    if (state > 0) {
        if (state != 1) {
            if (sResult < 0) {
                return 2;
            }
        } else {
            s32 result = sResult;
            if (result == NAND_RESULT_NOEXISTS) {
                param->state = 3;
            } else if (result < 0) {
                return 2;
            } else if (param->size < *param->num * 13) {
                return 2;
            }
        }
    }

    switch (param->state) {
    case 0:
        if (readDir(NULL, param->num, getHomeDir(param->share)) != 0) {
            return 2;
        }
        param->state = 1;
        break;
    case 1:
        if (readDir(param->nameList, param->num, getHomeDir(param->share)) != 0) {
            return 2;
        }
        param->state = 2;
        break;
    case 2:
        param->state = 3;
        break;
    case 3:
        return 1;
    }
    return 0;
}
