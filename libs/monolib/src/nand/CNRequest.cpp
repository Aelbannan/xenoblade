#include "monolib/nand/CNRequest.hpp"

#include <string.h>

#include "monolib/nand/CNReqtaskCheck.hpp"
#include "monolib/nand/CNReqtaskCreatedir.hpp"
#include "monolib/nand/CNReqtaskLoad.hpp"
#include "monolib/nand/CNReqtaskReaddir.hpp"
#include "monolib/nand/CNReqtaskRemove.hpp"
#include "monolib/nand/CNReqtaskSave.hpp"
#include "monolib/nand/CNReqtaskSaveBanner.hpp"

CNRequest::CNRequest() {
    mTask = NULL;
    memset(mParam, 0, sizeof(mParam));
}

bool CNRequest::update(bool* success) {
    int result = mTask->update(mParam);
    if (result != 0) {
        *success = result == 1;
        mTask->close(mParam);
        mTask = NULL;
        return true;
    }
    return false;
}

bool CNRequest::check(u32 neededBlocks, u32 neededFiles, u32* answer) {
    mTask = CNReqtaskCheck::setup(mParam, neededBlocks, neededFiles, answer);
    return mTask != NULL;
}

bool CNRequest::save(const char* name, const void* buf, u32 size, u8 perm, bool share) {
    mTask = CNReqtaskSave::setup(mParam, name, buf, size, perm, share);
    return mTask != NULL;
}

bool CNRequest::load(const char* name, void* buf, u32 size, bool share) {
    mTask = CNReqtaskLoad::setup(mParam, name, buf, size, share);
    return mTask != NULL;
}

bool CNRequest::remove(const char* name, bool share) {
    mTask = CNReqtaskRemove::setup(mParam, name, share);
    return mTask != NULL;
}

bool CNRequest::readdir(char* nameList, u32 size, u32* num, bool share) {
    mTask = CNReqtaskReaddir::setup(mParam, nameList, size, num, share);
    return mTask != NULL;
}

bool CNRequest::mkdir(const char* name, u8 perm, bool share) {
    mTask = CNReqtaskCreatedir::setup(mParam, name, perm, share);
    return mTask != NULL;
}

bool CNRequest::saveBanner(CNBanner* banner, bool share) {
    mTask = CNReqtaskSaveBanner::setup(mParam, banner, share);
    return mTask != NULL;
}
