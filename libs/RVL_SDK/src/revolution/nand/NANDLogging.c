#include <revolution/FS.h>
#include <revolution/NAND.h>
#include <revolution/OS.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef void (*NANDLoggingCallback)(BOOL result);

/* Retail: global .bss / .sdata / .sbss. */
char s_message[256] ALIGN(64);

/* Retail .sdata object is 8 bytes: -255 + pad. */
#pragma explicit_zero_data on
s32 s_fd[2] = {-255, 0};
#pragma explicit_zero_data off

NANDLoggingCallback s_callback;
int s_stage;

/* Retail .data @0x0 / @0x1C; postprocess renames to lbl_805512*. */
static char s_nanderrPath[] = "/shared2/test2/nanderr.log\0";
static char s_lineFmt[] =
    "%d %04d/%02d/%02d %02d:%02d:%02d %s %s\0\0\0\0\0\0\0\0\0\0";

void asyncRoutine(s32 result, void* ctxt);

#pragma dont_inline on
BOOL reserveFileDescriptor(void) {
    BOOL enabled;
    BOOL busy;

    enabled = OSDisableInterrupts();

    if (s_fd[0] == -255) {
        s_fd[0] = -254;
        busy = FALSE;
    } else {
        busy = TRUE;
    }

    OSRestoreInterrupts(enabled);
    return busy ? FALSE : TRUE;
}
#pragma dont_inline off

BOOL NANDLoggingAddMessageAsync(NANDLoggingCallback cb, const char* fmt, ...) {
    va_list ap;
    s32 err;

    if (!reserveFileDescriptor()) {
        return FALSE;
    }

    va_start(ap, fmt);
    vsnprintf(s_message, 256, fmt, ap);
    va_end(ap);

    s_callback = cb;
    s_stage = 1;
    err = ISFS_OpenAsync(s_nanderrPath, IPC_OPEN_RW, asyncRoutine, NULL);
    if (err == IPC_RESULT_OK) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void asyncRoutine(s32 result, void* ctxt) {
    static char s_rBuf[256] ALIGN(64);
    static char s_wBuf[256] ALIGN(64);
    char* base = s_message;

    (void)ctxt;

    s_stage = s_stage + 1;

    if (s_stage == 2) {
        if (result >= 0) {
            s_fd[0] = result;
            if (ISFS_SeekAsync(result, 0, IPC_SEEK_BEG, asyncRoutine, NULL) !=
                IPC_RESULT_OK) {
                if (s_callback != NULL) {
                    s_callback(FALSE);
                }
            }
        } else if (s_callback != NULL) {
            s_callback(FALSE);
        }
    } else if (s_stage == 3) {
        if (result == 0) {
            if (ISFS_ReadAsync(s_fd[0], base + 0x100, 256, asyncRoutine, NULL) !=
                IPC_RESULT_OK) {
                if (s_callback != NULL) {
                    s_callback(FALSE);
                }
            }
        } else if (s_callback != NULL) {
            s_callback(FALSE);
        }
    } else if (s_stage == 4) {
        if (result == 256) {
            if (ISFS_SeekAsync(s_fd[0], 0, IPC_SEEK_BEG, asyncRoutine, NULL) !=
                IPC_RESULT_OK) {
                if (s_callback != NULL) {
                    s_callback(FALSE);
                }
            }
        } else if (s_callback != NULL) {
            s_callback(FALSE);
        }
    } else if (s_stage == 5) {
        if (result == 0) {
            OSCalendarTime cal;
            char titleID[64];
            int end;
            char* rbuf;
            char* line;
            char* msg;

            rbuf = base + 0x100;
            {
                /* Short-lived zero so result is not kept in an extra NV. */
                s32 z = 0;
                rbuf[255] = z;
            }
            result = atoi(rbuf);
            memset(base + 0x200, ' ', 254);
            OSTicksToCalendarTime(OSGetTime(), &cal);
            strncpy(titleID, nandGetHomeDir() + 7, 8 + 1 + 8);
            titleID[8] = '-';
            titleID[8 + 1 + 8] = '\0';
            msg = base + 0;
            end = snprintf(base + 0x200, 256, s_lineFmt,
                           result % (64 - 1) + 1, cal.year, cal.month + 1, cal.mday,
                           cal.hour, cal.min, cal.sec, titleID, msg);
            if (end < 256) {
                line = base + 0x200;
                line[end] = ' ';
            }
            line = base + 0x200;
            line[254] = '\r';
            line[255] = '\n';
            if (ISFS_WriteAsync(s_fd[0], line, 256, asyncRoutine, NULL) !=
                IPC_RESULT_OK) {
                if (s_callback != NULL) {
                    s_callback(FALSE);
                }
            }
        } else if (s_callback != NULL) {
            s_callback(FALSE);
        }
    } else if (s_stage == 6) {
        if (result == 256) {
            s32 n;
            s32 fd;

            n = atoi(base + 0x100);
            fd = s_fd[0];
            if (ISFS_SeekAsync(fd, n * 256, IPC_SEEK_BEG, asyncRoutine, NULL) !=
                IPC_RESULT_OK) {
                if (s_callback != NULL) {
                    s_callback(FALSE);
                }
            }
        } else if (s_callback != NULL) {
            s_callback(FALSE);
        }
    } else if (s_stage == 7) {
        if (result == atoi(base + 0x100) * 256) {
            if (ISFS_WriteAsync(s_fd[0], base + 0x200, 256, asyncRoutine, NULL) !=
                IPC_RESULT_OK) {
                if (s_callback != NULL) {
                    s_callback(FALSE);
                }
            }
        } else if (s_callback != NULL) {
            s_callback(FALSE);
        }
    } else if (s_stage == 8) {
        if (result == 256) {
            if (ISFS_CloseAsync(s_fd[0], asyncRoutine, NULL) != IPC_RESULT_OK) {
                if (s_callback != NULL) {
                    s_callback(FALSE);
                }
            }
        } else if (s_callback != NULL) {
            s_callback(FALSE);
        }
    } else if (s_stage == 9) {
        if (result == IPC_RESULT_OK) {
            s_fd[0] = -255;
            if (s_callback != NULL) {
                s_callback(TRUE);
            }
        } else if (s_callback != NULL) {
            s_callback(FALSE);
        }
    }

    (void)s_rBuf;
    (void)s_wBuf;
}
