#include <revolution/ESP.h>
#include <revolution/NAND.h>
#include <revolution/OS.h>

#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define NAND_BANNER_MAGIC 'WIBN'

typedef enum {
    NAND_LIB_UNINITIALIZED,
    NAND_LIB_INITIALIZING,
    NAND_LIB_INITIALIZED
} NANDLibState;

void nandShutdownCallback(s32 result, void* arg);
void nandGetTypeCallback(s32 result, void* arg);
void nandChangeDirCallback(s32 result, void* arg);
s32 nandChangeDir(const char* path, NANDCommandBlock* block, BOOL async,
                  BOOL priv);
BOOL nandOnShutdown(BOOL final, u32 event);
static BOOL nandIsRelativePath(const char* path);
s32 nandGetType(const char* path, u8* type, NANDCommandBlock* block, BOOL async,
                BOOL priv);

/* Retail: 0x60-byte .data object; later strings are addressed as base+off. */
char __NANDVersion[0x60] =
    "<< RVL_SDK - NAND \trelease build: Feb 27 2009 10:05:16 (0x4302_145) >>";

static NANDLibState s_libState = NAND_LIB_UNINITIALIZED;
static char s_currentDir[64] ALIGN(32) = "/";
/* Must remain a real .data object at +0xA0 (NANDInit may address via strBase). */
#pragma force_active on
static OSShutdownFunctionInfo s_shutdownFuncInfo = {nandOnShutdown, 255};
#pragma force_active off

static char s_homeDir[64] ALIGN(32);

/* Contiguous after version: +0xB0 /shared2, +0xBC pooled strings (0x94). */
#pragma force_active on
static char s_shared2Prefix[12] = "/shared2";
static char s_nandStringPool[0x94] =
    "/shared2/\0\0\0"
    "ISFS error code: %d\0"
    "CAUTION!  Unexpected error code [%d] was found.\n\0\0\0\0"
    "ISFS unexpected error code: %d\0\0"
    "Failed to set home directory.\n\0";
#pragma force_active off

/* Retail .sdata 0x28; explicit_zero_data keeps "" / L"" out of .sbss. */
static char* s_nandVersion = __NANDVersion;
static char s_pathRoot[] = "/";
#pragma explicit_zero_data on
static char s_pathEmpty[4] = {0};
#pragma explicit_zero_data off
static char s_pathDot[] = ".";
static char s_pathDotDot[] = "..";
static char s_fmtAbsHead[] = "/%s";
static char s_fmtJoinPath[8] = "%s/%s";
#pragma explicit_zero_data on
static wchar_t s_wEmpty[2] = {0};
#pragma explicit_zero_data off
static wchar_t s_wSpace[] = L" ";

void nandRemoveTailToken(char* newp, const char* oldp) {
    int i;

    if (oldp[0] == '/' && oldp[1] == '\0') {
        newp[0] = '/';
        newp[1] = '\0';
    } else {
        for (i = strlen(oldp) - 1; i >= 0; i--) {
            if (oldp[i] == '/') {
                if (i != 0) {
                    strncpy(newp, oldp, i);
                    newp[i] = '\0';
                    break;
                } else {
                    newp[0] = '/';
                    newp[1] = '\0';
                    break;
                }
            }
        }
    }
}

void nandGetHeadToken(char* head, char* rest, const char* path) {
    int i;

    for (i = 0; i <= strlen(path); i++) {
        if (path[i] == '/') {
            strncpy(head, path, i);
            head[i] = '\0';

            if (path[i + 1] == '\0') {
                *rest = '\0';
            } else {
                strcpy(rest, path + i + 1);
            }
            break;
        } else if (path[i] == '\0') {
            strncpy(head, path, i);
            head[i] = '\0';
            *rest = '\0';
            break;
        }
    }
}

void nandGetRelativeName(char* name, const char* path) {
    int i;

    if (strcmp(s_pathRoot, path) == 0) {
        strcpy(name, s_pathEmpty);
    } else {
        for (i = strlen(path) - 1; i >= 0; i--) {
            if (path[i] == '/') {
                break;
            }
        }

        strcpy(name, path + i + 1);
    }
}

void nandConvertPath(char* abs, const char* dir, const char* rel) {
    char head[128];
    char rest[128];
    char notail[128];
    char newdir[128];

    if (strlen(rel) == 0) {
        strcpy(abs, dir);
        return;
    }

    nandGetHeadToken(head, rest, rel);

    if (strcmp(head, s_pathDot) == 0) {
        nandConvertPath(abs, dir, rest);
    } else if (strcmp(head, s_pathDotDot) == 0) {
        nandRemoveTailToken(notail, dir);
        nandConvertPath(abs, notail, rest);
    } else if (head[0] != '\0') {
        if (strcmp(dir, s_pathRoot) == 0) {
            sprintf(newdir, s_fmtAbsHead, head);
        } else {
            sprintf(newdir, s_fmtJoinPath, dir, head);
        }
        nandConvertPath(abs, newdir, rest);
    } else {
        strcpy(abs, dir);
    }
}

static BOOL nandIsRelativePath(const char* path) {
    return *path == '/' ? FALSE : TRUE;
}

BOOL nandIsPrivatePath(const char* path) {
    size_t len = sizeof("/shared2") - 1;
    return strncmp(path, s_shared2Prefix, len) == 0;
}

BOOL nandIsUnderPrivatePath(const char* path) {
    size_t len = sizeof("/shared2/") - 1;

    if (strncmp(path, s_nandStringPool, len) == 0 && path[len] != '\0') {
        return TRUE;
    }

    return FALSE;
}

BOOL nandIsInitialized(void) {
    return s_libState == NAND_LIB_INITIALIZED ? TRUE : FALSE;
}

typedef void (*NANDLoggingCallback)(BOOL result);
BOOL NANDLoggingAddMessageAsync(NANDLoggingCallback cb, const char* fmt, ...);

#pragma dont_inline on
s32 nandConvertErrorCode(s32 result) {
    char* strBase = __NANDVersion;
    int i;

    // clang-format off
    const s32 errorMap[] = {IPC_RESULT_OK,                    NAND_RESULT_OK,
                            IPC_RESULT_ACCESS,                NAND_RESULT_ACCESS,
                            IPC_RESULT_CORRUPT,               NAND_RESULT_CORRUPT,
                            IPC_RESULT_ECC_CRIT,              NAND_RESULT_ECC_CRIT,
                            IPC_RESULT_EXISTS,                NAND_RESULT_EXISTS,
                            -116,                             NAND_RESULT_AUTHENTICATION,
                            IPC_RESULT_INVALID,               NAND_RESULT_INVALID,
                            IPC_RESULT_MAXBLOCKS,             NAND_RESULT_MAXBLOCKS,
                            IPC_RESULT_MAXFD,                 NAND_RESULT_MAXFD,
                            IPC_RESULT_MAXFILES,              NAND_RESULT_MAXFILES,
                            -110,                             NAND_RESULT_MAXDEPTH,
                            IPC_RESULT_NOEXISTS,              NAND_RESULT_NOEXISTS,
                            IPC_RESULT_NOTEMPTY,              NAND_RESULT_NOTEMPTY,
                            -104,                             NAND_RESULT_UNKNOWN,
                            IPC_RESULT_OPENFD,                NAND_RESULT_OPENFD,
                            -117,                             NAND_RESULT_UNKNOWN,
                            IPC_RESULT_BUSY,                  NAND_RESULT_BUSY,
                            IPC_RESULT_FATAL_ERROR,           NAND_RESULT_FATAL_ERROR,
                            IPC_RESULT_ACCESS_INTERNAL,       NAND_RESULT_ACCESS,
                            IPC_RESULT_EXISTS_INTERNAL,       NAND_RESULT_EXISTS,
                            -3,                               NAND_RESULT_UNKNOWN,
                            IPC_RESULT_INVALID_INTERNAL,      NAND_RESULT_INVALID,
                            -5,                               NAND_RESULT_UNKNOWN,
                            IPC_RESULT_NOEXISTS_INTERNAL,     NAND_RESULT_NOEXISTS,
                            -7,                               NAND_RESULT_UNKNOWN,
                            IPC_RESULT_BUSY_INTERNAL,         NAND_RESULT_BUSY,
                            -9,                               NAND_RESULT_UNKNOWN,
                            -10,                              NAND_RESULT_UNKNOWN,
                            -11,                              NAND_RESULT_UNKNOWN,
                            IPC_RESULT_ECC_CRIT_INTERNAL,     NAND_RESULT_ECC_CRIT,
                            -13,                              NAND_RESULT_UNKNOWN,
                            -14,                              NAND_RESULT_UNKNOWN,
                            -15,                              NAND_RESULT_UNKNOWN,
                            -16,                              NAND_RESULT_UNKNOWN,
                            -17,                              NAND_RESULT_UNKNOWN,
                            -18,                              NAND_RESULT_UNKNOWN,
                            -19,                              NAND_RESULT_UNKNOWN,
                            -20,                              NAND_RESULT_UNKNOWN,
                            -21,                              NAND_RESULT_UNKNOWN,
                            IPC_RESULT_ALLOC_FAILED,          NAND_RESULT_ALLOC_FAILED,
                            -23,                              NAND_RESULT_UNKNOWN};
    // clang-format on

    i = 0;

    if (result >= 0) {
        return result;
    }

    for (; i < ARRAY_SIZE(errorMap); i += 2) {
        if (result == errorMap[i]) {
            if (result == -0x72 || result == -0x74 || result == -0x75 ||
                result == -0x9 || result == -0xc) {
                char msg[128] ALIGN(64);
                sprintf(msg, strBase + 0xC8, result);
                NANDLoggingAddMessageAsync(NULL, msg);
            }
            /* Inlined nandReportErrorCode: cmpwi -23 / bge / cmpwi -100. */
            if (result < -23) {
                if (result < -100) {
                    i = i;
                }
            }
            return errorMap[i + 1];
        }
    }

    OSReport(strBase + 0xDC, result);
    {
        char msg[128] ALIGN(64);
        sprintf(msg, strBase + 0x110, result);
        NANDLoggingAddMessageAsync(NULL, msg);
    }
    {
        s32 mapped = NAND_RESULT_UNKNOWN;
        /* Inlined nandReportErrorCode: cmpwi -23 / bge / cmpwi -100. */
        if (result < -23) {
            if (result < -100) {
                mapped = mapped;
            }
        }
        return mapped;
    }
}
#pragma dont_inline off

void nandGenerateAbsPath(char* abs, const char* rel) {
    s32 len;

    if (strlen(rel) == 0) {
        strcpy(abs, s_pathEmpty);
    } else if (nandIsRelativePath(rel)) {
        nandConvertPath(abs, s_currentDir, rel);
    } else {
        strcpy(abs, rel);
        len = strlen(abs);
        if (len != 0 && abs[len - 1] == '/' && len - 1 != 0) {
            abs[len - 1] = '\0';
        }
    }
}

s32 NANDInit(void) {
    /* Keep .data base in one reg (r31+off) like retail, not __NANDVersion+off. */
    char* strBase = __NANDVersion;
    s32 result;
    u64 tid;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if (s_libState == NAND_LIB_INITIALIZING) {
        OSRestoreInterrupts(enabled);
        return NAND_RESULT_BUSY;
    } else if (s_libState == NAND_LIB_INITIALIZED) {
        OSRestoreInterrupts(enabled);
        return NAND_RESULT_OK;
    }

    s_libState = NAND_LIB_INITIALIZING;
    OSRestoreInterrupts(enabled);

    result = ISFS_OpenLib();
    if (result == IPC_RESULT_OK) {
        result = ESP_InitLib();

        if (result == IPC_RESULT_OK) {
            result = ESP_GetTitleId(&tid);
        }

        if (result == IPC_RESULT_OK) {
            result = ESP_GetDataDir(tid, s_homeDir);
        }

        if (result == IPC_RESULT_OK) {
            strcpy(strBase + 0x60, s_homeDir);
        }

        ESP_CloseLib();

        if (result != IPC_RESULT_OK) {
            OSReport(strBase + 0x130);
        }

        OSRegisterShutdownFunction((OSShutdownFunctionInfo*)(strBase + 0xa0));
        enabled = OSDisableInterrupts();
        s_libState = NAND_LIB_INITIALIZED;
        OSRestoreInterrupts(enabled);
        OSRegisterVersion(s_nandVersion);
        return NAND_RESULT_OK;
    } else {
        enabled = OSDisableInterrupts();
        s_libState = NAND_LIB_UNINITIALIZED;
        OSRestoreInterrupts(enabled);
        return nandConvertErrorCode(result);
    }
}

BOOL nandOnShutdown(BOOL final, u32 event) {
    if (!final) {
        if (event == OS_SD_EVENT_SHUTDOWN) {
            volatile BOOL shutdown = FALSE;
            s64 start = OSGetTime();
            ISFS_ShutdownAsync(nandShutdownCallback, (void*)&shutdown);

            while (OS_TICKS_TO_MSEC(OSGetTime() - start) < 500) {
                if (shutdown) {
                    break;
                }
            }
        }

        return TRUE;
    }

    return TRUE;
}

void nandShutdownCallback(s32 result, void* arg) {
#pragma unused(result)

    *(BOOL*)arg = TRUE;
}

s32 nandChangeDir(const char* path, NANDCommandBlock* block, BOOL async,
                  BOOL priv) {
    // Retail stack: numFiles @ sp+8, path @ sp+10 (gap @ sp+0xC uncleared).
    u32 numFiles;
    char stackPath[64];
    s32 result;
    s32 len;
    BOOL enabled;
    BOOL isPrivate;

    // Retail duplicates absolute-path construction for the async/sync arms.
    if (async) {
        if (strlen(path) == 0) {
            strcpy(block->path, s_pathEmpty);
        } else if (nandIsRelativePath(path)) {
            nandConvertPath(block->path, s_currentDir, path);
        } else {
            strcpy(block->path, path);
            len = strlen(block->path);
            if (len != 0 && block->path[len - 1] == '/' && len - 1 != 0) {
                block->path[len - 1] = '\0';
            }
        }

        // Inline private check: if/else TRUE/FALSE (not cntlzw from == 0).
        if (!priv) {
            if (strncmp(block->path, s_shared2Prefix, sizeof("/shared2") - 1) ==
                0) {
                isPrivate = TRUE;
            } else {
                isPrivate = FALSE;
            }
            if (isPrivate) {
                return IPC_RESULT_ACCESS;
            }
        }

        return ISFS_ReadDirAsync(block->path, NULL, &block->dirFileCount,
                                 nandChangeDirCallback, block);
    }

    // Coalesce numFiles clear with path MEMCLR (retail: one li r0 + stws).
    numFiles = 0;
    MEMCLR(&stackPath);

    if (strlen(path) == 0) {
        strcpy(stackPath, s_pathEmpty);
    } else if (nandIsRelativePath(path)) {
        nandConvertPath(stackPath, s_currentDir, path);
    } else {
        strcpy(stackPath, path);
        len = strlen(stackPath);
        if (len != 0 && stackPath[len - 1] == '/' && len - 1 != 0) {
            stackPath[len - 1] = '\0';
        }
    }

    if (!priv) {
        if (strncmp(stackPath, s_shared2Prefix, sizeof("/shared2") - 1) == 0) {
            isPrivate = TRUE;
        } else {
            isPrivate = FALSE;
        }
        if (isPrivate) {
            return IPC_RESULT_ACCESS;
        }
    }

    result = ISFS_ReadDir(stackPath, NULL, &numFiles);
    if (result == IPC_RESULT_OK) {
        enabled = OSDisableInterrupts();
        strcpy(s_currentDir, stackPath);
        OSRestoreInterrupts(enabled);
    }

    return result;
}

s32 NANDChangeDirAsync(const char* path, NANDAsyncCallback callback,
                       NANDCommandBlock* block) {
    if (s_libState != NAND_LIB_INITIALIZED) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    return nandConvertErrorCode(nandChangeDir(path, block, TRUE, FALSE));
}

void nandChangeDirCallback(s32 result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;
    BOOL enabled;

    if (result == IPC_RESULT_OK) {
        enabled = OSDisableInterrupts();
        strcpy(s_currentDir, block->path);
        OSRestoreInterrupts(enabled);
    }

    block->callback(nandConvertErrorCode(result), block);
}

s32 NANDGetHomeDir(char* out) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    strcpy(out, s_homeDir);
    return NAND_RESULT_OK;
}

void nandCallback(s32 result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;
    block->callback(nandConvertErrorCode(result), block);
}

s32 nandGetType(const char* path, u8* type, NANDCommandBlock* block, BOOL async,
                BOOL priv) {
    char absPath[64];
    u32 numFiles;
    s32 result;

    if (strlen(path) == 0) {
        return IPC_RESULT_INVALID;
    }

    if (async) {
        nandGenerateAbsPath(block->path, path);

        if (!priv && nandIsUnderPrivatePath(block->path)) {
            return IPC_RESULT_ACCESS;
        }

        block->type = type;
        return ISFS_ReadDirAsync(block->path, NULL, &block->dirFileCount,
                                 nandGetTypeCallback, block);
    } else {
        MEMCLR(&absPath);
        nandGenerateAbsPath(absPath, path);

        if (!priv && nandIsUnderPrivatePath(absPath)) {
            return IPC_RESULT_ACCESS;
        }

        numFiles = 0;
        result = ISFS_ReadDir(absPath, NULL, &numFiles);
        if (result == IPC_RESULT_OK || result == IPC_RESULT_ACCESS) {
            *type = NAND_FILE_TYPE_DIR;
            return IPC_RESULT_OK;
        } else if (result == IPC_RESULT_INVALID) {
            *type = NAND_FILE_TYPE_FILE;
            return IPC_RESULT_OK;
        }

        return result;
    }
}

s32 NANDPrivateGetTypeAsync(const char* path, u8* type,
                            NANDAsyncCallback callback,
                            NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    return nandConvertErrorCode(nandGetType(path, type, block, TRUE, TRUE));
}

void nandGetTypeCallback(s32 result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;

    if (result == IPC_RESULT_OK || result == IPC_RESULT_ACCESS) {
        *block->type = NAND_FILE_TYPE_DIR;
        result = IPC_RESULT_OK;
    } else if (result == IPC_RESULT_INVALID) {
        *block->type = NAND_FILE_TYPE_FILE;
        result = IPC_RESULT_OK;
    }

    block->callback(nandConvertErrorCode(result), block);
}

const char* nandGetHomeDir(void) {
    return s_homeDir;
}

void NANDInitBanner(NANDBanner* banner, u32 flags, const wchar_t* title,
                    const wchar_t* subtitle) {
    memset(banner, 0, sizeof(NANDBanner));
    banner->flags = flags;
    banner->magic = NAND_BANNER_MAGIC;

    if (wcscmp(title, s_wEmpty) == 0) {
        wcsncpy(banner->title, s_wSpace, NAND_BANNER_TITLE_MAX);
    } else {
        wcsncpy(banner->title, title, NAND_BANNER_TITLE_MAX);
    }

    if (wcscmp(subtitle, s_wEmpty) == 0) {
        wcsncpy(banner->subtitle, s_wSpace, NAND_BANNER_TITLE_MAX);
    } else {
        wcsncpy(banner->subtitle, subtitle, NAND_BANNER_TITLE_MAX);
    }
}

