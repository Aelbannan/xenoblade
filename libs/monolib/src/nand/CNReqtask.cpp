#include "monolib/nand/CNReqtask.hpp"

#include <stdio.h>
#include <string.h>

#include "monolib/core/CException.hpp"

static const wchar_t* sGeneralErrorMessage = L"";
static const wchar_t* sSaveErrorMessage = L"";
static const wchar_t* sExtraErrorMessage = L"";

bool CNReqtask::sBusy;
s32 CNReqtask::sResult;
bool CNReqtask::sOpen;

static NANDCommandBlock sCommandBlock;
static NANDFileInfo sFileInfo;

void CNReqtask::close(void* param) {
    if (sOpen) {
        closeFile();
    }
}

s32 CNReqtask::check(u32 neededBlocks, u32 neededFiles, u32* answer) {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDCheckAsync(neededBlocks, neededFiles, answer, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    }
    return ret;
}

s32 CNReqtask::open(const char* path, u8 access) {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDOpenAsync(path, &sFileInfo, access, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    } else {
        sOpen = true;
    }
    return ret;
}

s32 CNReqtask::read(void* buf, u32 size) {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDReadAsync(&sFileInfo, buf, size, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    }
    return ret;
}

s32 CNReqtask::write(const void* buf, u32 size) {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDWriteAsync(&sFileInfo, buf, size, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    }
    return ret;
}

s32 CNReqtask::closeFile() {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDCloseAsync(&sFileInfo, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    } else {
        sOpen = false;
    }
    return ret;
}

s32 CNReqtask::create(const char* path, u8 perm, u8 attr) {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDCreateAsync(path, perm, attr, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    }
    return ret;
}

s32 CNReqtask::remove(const char* path) {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDDeleteAsync(path, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    }
    return ret;
}

s32 CNReqtask::move(const char* from, const char* to) {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDMoveAsync(from, to, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    }
    return ret;
}

s32 CNReqtask::tell(u32* pos) {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDTellAsync(&sFileInfo, pos, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    }
    return ret;
}

s32 CNReqtask::readDir(char* nameList, u32* num, const char* path) {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDReadDirAsync(path, nameList, num, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    }
    return ret;
}

s32 CNReqtask::createDir(const char* path, u8 perm, u8 attr) {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDCreateDirAsync(path, perm, attr, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    }
    return ret;
}

s32 CNReqtask::changeDir(const char* path) {
    sBusy = true;
    sResult = 0;
    s32 ret = NANDChangeDirAsync(path, callback, &sCommandBlock);
    if (ret != 0) {
        error(ret);
        sBusy = false;
    }
    return ret;
}

void CNReqtask::callback(s32 result, NANDCommandBlock* block) {
    sBusy = false;
    sResult = result;
}

const char* CNReqtask::getHomeDir(bool share) {
    static char sHomeDir[68];

    error(NANDGetHomeDir(sHomeDir));
    if (share) {
        strcat(sHomeDir, "/share");
    }
    return sHomeDir;
}

const char* CNReqtask::getPath(const char* name, bool share) {
    static char sPath[68];

    sprintf(sPath, "%s/%s", getHomeDir(share), name);
    return sPath;
}

void CNReqtask::error(s32 code) {
    if (code == NAND_RESULT_CORRUPT) {
        CException::logExceptionMessage(sGeneralErrorMessage);
        return;
    }
    if (code == NAND_RESULT_BUSY || code == NAND_RESULT_ALLOC_FAILED) {
        CException::logExceptionMessage(sSaveErrorMessage);
        return;
    }
    if (code == NAND_RESULT_UNKNOWN) {
        CException::logExceptionMessage(sGeneralErrorMessage);
    }
}

void setGeneralNandErrorMessage(const wchar_t* msg) {
    sGeneralErrorMessage = msg;
}

void setSaveNandErrorMessage(const wchar_t* msg) {
    sSaveErrorMessage = msg;
}

void setExtraNandErrorMessage(const wchar_t* msg) {
    sExtraErrorMessage = msg;
}
