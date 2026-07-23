#include <revolution/NAND.h>
#include <revolution/OS.h>
#include <stdio.h>

void nandOpenCallback(s32 result, void* arg);
void nandCloseCallback(s32 result, void* arg);

/* Retail keeps nandOpen outlined; -ipa file would otherwise inline it. */
#pragma dont_inline on
s32 nandOpen(const char* path, u8 mode, NANDCommandBlock* block,
                    BOOL async, BOOL priv) {
    IPCOpenMode ipcMode;
    char absPath[64];

    MEMCLR(&absPath);
    ipcMode = IPC_OPEN_NONE;
    nandGenerateAbsPath(absPath, path);

    if (!priv && nandIsPrivatePath(absPath)) {
        return IPC_RESULT_ACCESS;
    }

    switch (mode) {
    case NAND_ACCESS_RW:
        ipcMode = IPC_OPEN_RW;
        break;
    case NAND_ACCESS_READ:
        ipcMode = IPC_OPEN_READ;
        break;
    case NAND_ACCESS_WRITE:
        ipcMode = IPC_OPEN_WRITE;
        break;
    }

    if (async) {
        return ISFS_OpenAsync(absPath, ipcMode, nandOpenCallback, block);
    } else {
        return ISFS_Open(absPath, ipcMode);
    }
}
#pragma dont_inline off

s32 NANDOpen(const char* path, NANDFileInfo* info, u8 mode) {
    s32 result;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    result = nandOpen(path, mode, NULL, FALSE, FALSE);
    if (result >= 0) {
        info->fd = result;
        info->mark = 1;
        return NAND_RESULT_OK;
    }

    return nandConvertErrorCode(result);
}

s32 NANDPrivateOpen(const char* path, NANDFileInfo* info, u8 mode) {
    s32 result;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    result = nandOpen(path, mode, NULL, FALSE, TRUE);
    if (result >= 0) {
        info->fd = result;
        info->mark = 1;
        return NAND_RESULT_OK;
    }

    return nandConvertErrorCode(result);
}

s32 NANDOpenAsync(const char* path, NANDFileInfo* info, u8 mode,
                  NANDAsyncCallback callback, NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    block->info = info;
    return nandConvertErrorCode(nandOpen(path, mode, block, TRUE, FALSE));
}

s32 NANDPrivateOpenAsync(const char* path, NANDFileInfo* info, u8 mode,
                         NANDAsyncCallback callback, NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    block->info = info;
    return nandConvertErrorCode(nandOpen(path, mode, block, TRUE, TRUE));
}

void nandOpenCallback(s32 result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;

    if (result >= 0) {
        block->info->fd = result;
        block->info->stage = 2;
        block->info->mark = 1;
        block->callback(NAND_RESULT_OK, block);
    } else {
        block->callback(nandConvertErrorCode(result), block);
    }
}

s32 NANDClose(NANDFileInfo* info) {
    s32 result;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    if (info->mark != 1) {
        return NAND_RESULT_INVALID;
    }

    result = ISFS_Close(info->fd);
    if (result == IPC_RESULT_OK) {
        info->mark = 2;
    }

    return nandConvertErrorCode(result);
}

s32 NANDCloseAsync(NANDFileInfo* info, NANDAsyncCallback callback,
                   NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    if (info->mark != 1) {
        return NAND_RESULT_INVALID;
    }

    block->callback = callback;
    block->info = info;
    return nandConvertErrorCode(
        ISFS_CloseAsync(info->fd, nandCloseCallback, block));
}

void nandCloseCallback(s32 result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;

    if (result == IPC_RESULT_OK) {
        block->info->stage = 7;
        block->info->mark = 2;
    }

    block->callback(nandConvertErrorCode(result), block);
}

