#include "monolib/nand/CNReqtaskSaveBanner.hpp"

#include "monolib/nand/CNBanner.hpp"
#include "monolib/util/FixStr.hpp"

static CNReqtaskSaveBanner sTask;
static const char* sTempDir = "/tmp/";
static const char* sBannerName = "banner.bin";

CNReqtask* CNReqtaskSaveBanner::setup(void* p, CNBanner* banner, bool share) {
    Param* param = static_cast<Param*>(p);

    param->banner = banner;
    param->share = share;
    param->state = 0;
    return &sTask;
}

int CNReqtaskSaveBanner::update(void* p) {
    Param* param = static_cast<Param*>(p);
    CNBanner* banner = param->banner;

    if (banner->mBusy) {
        return 0;
    }
    if (sBusy) {
        return 0;
    }

    if (param->state > 0) {
        if (param->state == 2) {
            if (sResult == NAND_RESULT_NOEXISTS) {
                param->state = 5;
            } else if (sResult == NAND_RESULT_AUTHENTICATION || sResult == NAND_RESULT_ECC_CRIT) {
                param->state = 5;
            } else if (sResult < 0) {
                return 2;
            }
        } else if (param->state == 7) {
            if (banner->mFlag) {
                return 2;
            }
        } else if (sResult < 0) {
            return 2;
        }
    }

    switch (param->state) {
    case 0: {
        const char* dir = getHomeDir(param->share);
        if (dir == NULL) {
            return 2;
        }
        if (changeDir(dir) != 0) {
            return 2;
        }
        param->state = 1;
        break;
    }
    case 1:
        if (open(sBannerName, NAND_ACCESS_READ) != 0) {
            return 2;
        }
        param->state = 2;
        break;
    case 2:
        if (tell(&param->length) != 0) {
            return 2;
        }
        param->state = 3;
        break;
    case 3:
        if (closeFile() != 0) {
            return 2;
        }
        if (param->length == 0) {
            param->state = 5;
        } else {
            param->state = 4;
        }
        break;
    case 4:
        return 1;
    case 5: {
        s32 result = create(getTempPath(param), NAND_PERM_RUSR | NAND_PERM_WUSR | NAND_PERM_RGRP, 0);
        if (result != 0 && result != NAND_RESULT_EXISTS) {
            return 2;
        }
        param->state = 6;
        break;
    }
    case 6:
        if (!param->banner->load()) {
            return 2;
        }
        param->state = 7;
        break;
    case 7:
        if (open(getTempPath(param), NAND_ACCESS_WRITE) != 0) {
            return 2;
        }
        param->state = 8;
        break;
    case 8:
        if (write(param->banner->mData, param->banner->mSize) != 0) {
            return 2;
        }
        param->state = 9;
        break;
    case 9:
        if (closeFile() != 0) {
            return 2;
        }
        param->state = 10;
        break;
    case 10:
        param->banner->unload();
        if (move(getTempPath(param), getHomeDir(param->share)) != 0) {
            return 2;
        }
        param->state = 11;
        break;
    case 11:
        param->state = 4;
        break;
    }
    return 0;
}

void CNReqtaskSaveBanner::close(void* p) {
    Param* param = static_cast<Param*>(p);

    if (param != NULL && param->banner != NULL) {
        param->banner->unload();
    }
    CNReqtask::close(p);
}

const char* CNReqtaskSaveBanner::getTempPath(Param* param) {
    static ml::FixStr<32> sPath;

    sPath.format("%s%s", sTempDir, sBannerName);
    return sPath.c_str();
}
