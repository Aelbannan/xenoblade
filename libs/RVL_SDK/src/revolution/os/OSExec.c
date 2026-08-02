#include <revolution/ESP.h>
#include <revolution/IPC.h>
#include <revolution/OS.h>

#include <string.h>

#define MENU_TITLE_ID 0x0000000100000002
#define TICKET_VIEW_SIZE 0xD8

static u8 views[0xBD00] ALIGN(32);

BOOL __OSInReboot;

void Run(void (*func)(void)) {
    ICFlashInvalidate();
    __sync();
    __isync();
    func();
}

static s32 _ES_InitLib(s32* fd);
static s32 _ES_GetTicketViews(s32* fd, u64 tid, void* pViews, u32* count);
static s32 _ES_LaunchTitle(s32* fd, u64 tid, void* pViews) DECOMP_DONT_INLINE;

void __OSGetExecParams(OSExecParams* out) {
    if ((void*)OS_DOL_EXEC_PARAMS >= (void*)0x80000000) {
        memcpy(out, OS_DOL_EXEC_PARAMS, sizeof(OSExecParams));
    } else {
        out->WORD_0x0 = 0;
    }
}

//unused
void __OSSetExecParams(){
}

/**
 * Hex-encodes a UTF-16LE string (up to the first zero byte) into dst.
 * Returns 1 on success, 0 when src is NULL or contains an invalid digit.
 */
/**
 * Hex-encodes a UTF-16LE string (up to the first zero byte) into dst.
 * Returns TRUE on success, FALSE when srcName is NULL or contains an
 * invalid digit.
 */
BOOL Utf16ToArg(char* dstArg, u16* srcName) {
    char* srcPtr;
    char* dstPtr;
    u8 i;
    u8 mask;
    u8 shift;

    if (srcName != 0) {
        srcPtr = (char*)srcName;
        dstPtr = dstArg;

        while (*srcPtr || *(srcPtr + 1)) {
            for (i = 0; i < 4; i++) {
                mask = (u8)((i & 0x1) ? 0xF : 0xF0);
                shift = (u8)((i & 0x1) ? 0 : 4);

                if (0 <= ((*srcPtr & mask) >> shift) &&
                    ((*srcPtr & mask) >> shift) < 0xA) {
                    *dstPtr = (char)(((*srcPtr & mask) >> shift) + 0x30);
                } else if (0xA <= ((*srcPtr & mask) >> shift) &&
                           ((*srcPtr & mask) >> shift) < 0x10) {
                    *dstPtr = (char)(((*srcPtr & mask) >> shift) + 0x57);
                } else {
                    return FALSE;
                }

                dstPtr++;

                if (i & 1) {
                    srcPtr++;
                }
            }
        }

        *dstPtr++ = 0;
        return TRUE;
    }

    return FALSE;
}

void __OSLaunchMenu(void) {
    // This makes me feel sick
    s32 result;
    void* pviews = &views;
    u32 count = 1;
    s32 fd = -1;
    struct {
        u8 tmp[4];
    } unused = {0xFF, 0xFF, 0xFF, 0};

    if (_ES_InitLib(&fd) != IPC_RESULT_OK) {
        return;
    }

    // Get num ticket views
    result = _ES_GetTicketViews(&fd, MENU_TITLE_ID, NULL, &count);
    if (count != 1 || result != IPC_RESULT_OK) {
        return;
    }

    // Get ticket views
    if (_ES_GetTicketViews(&fd, MENU_TITLE_ID, pviews, &count) !=
        IPC_RESULT_OK) {
        return;
    }

    // Launch title
    if (_ES_LaunchTitle(&fd, MENU_TITLE_ID, pviews) != IPC_RESULT_OK) {
        return;
    }

    while (TRUE) {
        ;
    }
}

/**
 * These were actually re(?)implemented in NANDCore/OSExec according to BBA
 */

static s32 _ES_InitLib(s32* fd) {
    s32 result;

    // Had to remove fd initialization to match __OSLaunchMenu
    // *fd = -1;

    result = IPC_RESULT_OK;

    *fd = IOS_Open("/dev/es", IPC_OPEN_NONE);
    if (*fd < 0) {
        result = *fd;
    }

    return result;
}

static s32 _ES_GetTicketViews(s32* fd, u64 tid, void* pViews, u32* count) {
    s32 result;
    // TODO(kiwi) Hacky solution
    u8 work[0x120] ALIGN(32);
    IPCIOVector* pVectors = (IPCIOVector*)(work + 0x0);
    u64* pTid = (u64*)(work + 0x20);
    u32* pCount = (u32*)(work + 0x40);

    // Cast is necessary
    if (*fd < 0 || count == ((void*)NULL)) {
        return -0x3F9;
    }

    if ((u32)pViews % 32 != 0) {
        return -0x3F9;
    }

    *pTid = tid;

    // NULL views ptr = get num views
    if (pViews == (void*)NULL) {
        pVectors[0].base = pTid;
        pVectors[0].length = sizeof(u64);
        pVectors[1].base = pCount;
        pVectors[1].length = sizeof(u32);

        result =
            IOS_Ioctlv(*fd, ES_IOCTLV_GET_NUM_TICKET_VIEWS, 1, 1, pVectors);
        if (result == IPC_RESULT_OK) {
            *count = *pCount;
        }

        return result;
    }

    if (*count == 0) {
        return -0x3F9;
    }

    *pCount = *count;

    pVectors[0].base = pTid;
    pVectors[0].length = sizeof(u64);
    pVectors[1].base = pCount;
    pVectors[1].length = sizeof(u32);
    pVectors[2].base = pViews;
    pVectors[2].length = *count * TICKET_VIEW_SIZE;
    return IOS_Ioctlv(*fd, ES_IOCTLV_GET_TICKET_VIEWS, 2, 1, pVectors);
}

static s32 _ES_LaunchTitle(s32* fd, u64 tid, void* pViews) {
    // TODO(kiwi) Hacky solution
    u8 tidWork[256] ALIGN(32);
    u8 vectorWork[32] ALIGN(32);
    IPCIOVector* pVectors = (IPCIOVector*)vectorWork;
    u64* pTid = (u64*)tidWork;

    if (*fd < 0) {
        return -0x3F9;
    }

    if ((u32)pViews % 32 != 0) {
        return -0x3F9;
    }

    *pTid = tid;

    pVectors[0].base = pTid;
    pVectors[0].length = sizeof(u64);
    pVectors[1].base = pViews;
    pVectors[1].length = TICKET_VIEW_SIZE;

    return IOS_IoctlvReboot(*fd, ES_IOCTLV_LAUNCH_TITLE, 2, 0, pVectors);
}


/**
 * Packs argc/argv into the 0x2000-byte boot-args area at dst:
 * strings are copied back-to-front from the top of the area, and the
 * argv pointer table is written just below them.  *(dst+8) receives
 * the offset of the pointer-table count word.
 */
int PackArgs(char* dst, int argc, char** argv) {
    char* p;
    char** a;
    int i;

    memset(dst, 0, 0x2000);

    if (argc == 0) {
        *(int*)(dst + 8) = 0;
        return 1;
    }

    p = dst + 0x2000;
    a = argv + argc;
    i = argc;

    while (--i >= 0) {
        char* s;
        int len;

        a--;

        s = *a;
        len = (int)strlen(s);
        p -= len + 1;
        strcpy(p, s);
        *(int*)a = (int)(p - dst);
    }

    {
        unsigned int n = (unsigned int)argc + 1;
        int used = (int)(p - dst) & ~3;
        char* table = dst + used - (int)n * 4;

        if (n != 0) {
            unsigned int* src = (unsigned int*)argv;
            unsigned int* dstw = (unsigned int*)table;
            unsigned int j;

            for (j = 0; j < n; j++) {
                *dstw++ = *src++;
            }
        }

        *(int*)(table - 4) = argc;
        *(int*)(dst + 8) = (int)((table - 4) - dst);
    }

    return 1;
}

/**
 * PackInstallerArgs: like PackArgs, but the odd/even index selects
 * UTF-16 (wcslen + Utf16ToArg) vs UTF-8 (strlen + strcpy) packing.
 */
int PackInstallerArgs(char* dst, int argc, char** argv) {
    char* p;
    char** a;
    int i;

    memset(dst, 0, 0x2000);

    if (argc != 0) {
        p = dst + 0x2000;
        i = argc;
        a = argv + argc;
        while (--i >= 0) {
            if (i >= 2 && i % 2 == 0) {
                wchar_t* s;
                int len;

                s = (wchar_t*)*a;
                len = (int)wcslen(s);
                p -= len * 4 + 1;
                Utf16ToArg(p, s);
                *(int*)a = (int)(p - dst);
            } else {
                char* s;
                int len;

                s = *a;
                len = (int)strlen(s);
                p -= len + 1;
                strcpy(p, s);
                *(int*)a = (int)(p - dst);
            }
            a--;
        }

        {
            unsigned int n = (unsigned int)argc + 1;
            int used = (int)(p - dst) & ~3;
            char* table = dst + used - (int)n * 4;

            if (n != 0) {
                unsigned int* src = (unsigned int*)argv;
                unsigned int* dstw = (unsigned int*)table;
                unsigned int j;

                for (j = 0; j < n; j++) {
                    *dstw++ = *src++;
                }
            }

            *(int*)(table - 4) = argc;
            *(int*)(dst + 8) = (int)((table - 4) - dst);
        }
    } else {
        *(int*)(dst + 8) = 0;
    }

    return 1;
}

void __OSBootDolSimple(s32 param1, u32 param2, u32 regionStart, u32 regionEnd,
                       s32 param5, u32 argc, void* argv);

void __OSBootDol(u32 titleId, u32 param2, u32* argv) {
    char name[0x20];
    u32 saveStart;
    u32 saveEnd;
    void* newArgv;
    int count = 0;
    u32 n;
    u32* p;
    int j;

    OSGetSaveRegion(&saveStart, &saveEnd);

    sprintf(name, "%d", titleId);

    if (argv != NULL) {
        p = argv;
        while (*p != 0) {
            count++;
            p++;
        }
    }

    newArgv = OSAllocFromMEM1ArenaLo((count + 2) * 4, 1);
    *(u32*)newArgv = (u32)name;
    n = count + 1;

    if (n > 1) {
        int j;

        for (j = 1; j <= count; j++) {
            ((u32*)newArgv)[j] = ((u32*)argv)[j - 1];
        }
    }

    __OSBootDolSimple(-1, param2, saveStart, saveEnd, 0, n, newArgv);
}

extern int Prepared;
void Callback() {
    Prepared = 1;
}
extern unsigned long DVDLowIntType;
void callback(unsigned long type) {
    DVDLowIntType = type;
}
