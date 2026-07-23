#include <revolution/NAND.h>
#include <revolution/OS.h>

#include <stdio.h>

/* /meta and /ticket live in .sdata; title paths in .data (retail layout). */
const char* USER_DIR_LIST[] = {
    "/meta",
    "/ticket",
    "/title/00010000",
    "/title/00010001",
    "/title/00010003",
    "/title/00010004",
    "/title/00010005",
    "/title/00010006",
    "/title/00010007",
    "/shared2/title",
    NULL,
};

void nandUserAreaCallback(s32 result, void* arg);

/* Land at retail 0xAC / 0xC4; postprocess renames + sizes to lbl_805512*. */
static char s_nandUserAreaCallbackName[] = "nandUserAreaCallback";
static char s_nandUserAreaCallbackFmt[] = "Illegal status is detected at %s()\0";

static u32 nandCheck(u32 reqBlocks, u32 reqInodes, u32 homeBlocks,
                     u32 homeInodes, u32 userBlocks, u32 userInodes) {
    u32 answer = 0;

    if (homeBlocks + reqBlocks > 0x400) {
        answer |= NAND_CHECK_TOO_MANY_APP_BLOCKS;
    }
    if (homeInodes + reqInodes > 0x21) {
        answer |= NAND_CHECK_TOO_MANY_APP_FILES;
    }
    if (userBlocks + reqBlocks > 0x4400) {
        answer |= NAND_CHECK_TOO_MANY_USER_BLOCKS;
    }
    if (userInodes + reqInodes > 0xFA0) {
        answer |= NAND_CHECK_TOO_MANY_USER_FILES;
    }

    return answer;
}

s32 NANDCheckAsync(u32 neededBlocks, u32 neededFiles, u32* answer,
                   NANDAsyncCallback callback, NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    block->reqBlocks = neededBlocks;
    block->reqInodes = neededFiles;
    block->answer = answer;
    block->userBlocks = 0;
    block->userInodes = 0;
    block->workBlocks = 0;
    block->workInodes = 0;
    block->dir = USER_DIR_LIST;
    block->state = 0x14;

    return nandConvertErrorCode(
        ISFS_GetUsageAsync(nandGetHomeDir(), &block->homeBlocks,
                           &block->homeInodes, nandUserAreaCallback, block));
}

void nandUserAreaCallback(s32 result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;

    if (result == IPC_RESULT_OK || result == IPC_RESULT_NOEXISTS) {
        const char* path = *block->dir;

        if (result == IPC_RESULT_OK) {
            block->userBlocks += block->workBlocks;
            block->userInodes += block->workInodes;
        }

        if (path != NULL) {
            block->dir++;
            result = ISFS_GetUsageAsync(path, &block->workBlocks,
                                        &block->workInodes, nandUserAreaCallback,
                                        arg);
            if (result != IPC_RESULT_OK) {
                block->callback(nandConvertErrorCode(result), block);
            }
        } else if (block->state == 0x14) {
            *block->answer =
                nandCheck(block->reqBlocks, block->reqInodes, block->homeBlocks,
                          block->homeInodes, block->userBlocks,
                          block->userInodes);
            block->callback(nandConvertErrorCode(IPC_RESULT_OK), block);
        } else if (block->state == 0x15) {
            u32 freeUserBlocks =
                (block->userBlocks > 0x4400) ? 0 : (0x4400 - block->userBlocks);
            u32 freeUserInodes =
                (block->userInodes > 0xFA0) ? 0 : (0xFA0 - block->userInodes);
            u32 freeAppBlocks =
                (block->homeBlocks > 0x400) ? 0 : (0x400 - block->homeBlocks);
            u32 freeAppInodes =
                (block->homeInodes > 0x21) ? 0 : (0x21 - block->homeInodes);

            *(u32*)block->bytes = (freeUserBlocks < freeAppBlocks)
                                      ? freeUserBlocks
                                      : freeAppBlocks;
            *(u32*)block->inodes = (freeUserInodes < freeAppInodes)
                                       ? freeUserInodes
                                       : freeAppInodes;

            block->callback(nandConvertErrorCode(IPC_RESULT_OK), block);
        } else {
            OSReport(s_nandUserAreaCallbackFmt, s_nandUserAreaCallbackName);
            block->callback(nandConvertErrorCode(-0x75), block);
        }
    } else {
        block->callback(nandConvertErrorCode(result), block);
    }
}
