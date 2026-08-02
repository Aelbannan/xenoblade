// High-level C reconstruction of RVL_SDK/src/revolution/os/OSPlayTime
// All functions match the retail symbols exactly.

#include <types.h>
#include <string.h>
#include <revolution/OS.h>
#include <revolution/AI.h>
#include <revolution/VI.h>
#include <revolution/ESP.h>
#include <revolution/NAND.h>
#include <revolution/OS/OSStateTM.h>
#include <revolution/OS/OSReset.h>
#include <revolution/OS/OSInterrupt.h>
#include <revolution/OS/OSCache.h>
#include <revolution/OS/OSThread.h>
#include <revolution/OS/OSAlarm.h>
#include <revolution/OS/OSError.h>

// External globals from sbss/bss
extern void *volatile __OSExpireAIFade;
extern u32 __OSExpireSetExpiredFlag;
extern void (*__OSExpireCallback)(void);
extern u64 __OSExpireTime;
extern OSAlarm __OSExpireAlarm;

// Float/double constants in .sdata2 (loaded via extern to match retail relocs)
extern const f32 float_8066C1D8;
extern const f64 double_8066C1E0;

// Internal struct for the AI DMA fade double-buffer
typedef struct FadeAIData {
    u8 buf[0x480];             // 0x000: double-buffered audio data
    u32 bufIndex;              // 0x480: active buffer index (0 or 1)
    u32 ptr;                   // 0x484: current position in audio stream
    u32 state;                 // 0x488: fade state counter
    s16 fadeL;                 // 0x48C: left channel fade volume
    s16 fadeR;                 // 0x48E: right channel fade volume
    void (*callback)(void);    // 0x490: AI DMA callback
} FadeAIData;

BOOL OSPlayTimeIsLimited(void) {
    return __OSExpireTime != 0;
}

void __OSPlayTimeFadeLastAIDCallback(void) {
    FadeAIData* fade;
    u16* buf;
    s16* dst;
    u32 len;
    s32 rem;

    fade = (FadeAIData*)__OSExpireAIFade;
    if (fade->callback != NULL) {
        fade->callback();
    }

    fade = (FadeAIData*)__OSExpireAIFade;
    if (fade->state == 0) {
        ((FadeAIData*)__OSExpireAIFade)->ptr = AIGetDMAStartAddr() + 0x80000000;
    }

    fade = (FadeAIData*)__OSExpireAIFade;
    if (fade->state == 1) {
        DCInvalidateRange((void*)fade->ptr, 4);
        fade = (FadeAIData*)__OSExpireAIFade;
        {
            s16* p = (s16*)fade->ptr;
            fade->fadeL = p[0];
            fade->ptr = (u32)(p + 1);
        }
        fade = (FadeAIData*)__OSExpireAIFade;
        fade->fadeR = *(s16*)fade->ptr;
    }

    fade = (FadeAIData*)__OSExpireAIFade;
    if (fade->state >= 1) {
        buf = (u16*)((u8*)fade + fade->bufIndex * 0x240);
        dst = (s16*)buf;
        len = AIGetDMALength();
        rem = (s32)len;

        while (rem != 0) {
            rem -= 4;
            fade = (FadeAIData*)__OSExpireAIFade;
            dst[0] = fade->fadeL;
            fade = (FadeAIData*)__OSExpireAIFade;
            dst[1] = fade->fadeR;
            dst += 2;
            fade = (FadeAIData*)__OSExpireAIFade;
            {
                f32 mul = 0.995f;
                f32 x = (f32)fade->fadeL;
                fade->fadeL = (s16)(x * mul);
            }
            fade = (FadeAIData*)__OSExpireAIFade;
            {
                f32 mul = 0.995f;
                f32 x = (f32)fade->fadeR;
                fade->fadeR = (s16)(x * mul);
            }
        }

        DCFlushRange(buf, len);
        AIInitDMA(buf, len);
        ((FadeAIData*)__OSExpireAIFade)->bufIndex += 1;
        ((FadeAIData*)__OSExpireAIFade)->bufIndex &= 1;
    }

    ((FadeAIData*)__OSExpireAIFade)->state++;
}

s32 __OSWriteExpiredFlag(void) {
    NANDFileInfo info;
    s32 result;
    BOOL opened = FALSE;
    ESTitleId titleId;
    s32 written;

    result = NANDPrivateCreate("/shared2/expired", 0x3F, 0);
    if (result == 0 || result == -6) {
        result = NANDPrivateOpen("/shared2/expired", &info, NAND_ACCESS_WRITE);
        if (result == 0) {
            opened = TRUE;

            ESP_InitLib();
            memset(&titleId, 0, sizeof(titleId));
            ESP_GetTitleId(&titleId);

            written = NANDWrite(&info, &titleId, sizeof(titleId));
            if (written < 0) {
                result = written;
            } else if (written == (s32)sizeof(titleId)) {
                result = 0;
            } else {
                result = -8;
            }
        }
    }

    if (opened) {
        NANDClose(&info);
    }

    return (result == 0) ? 1 : 0;
}

s32 __OSWriteExpiredFlagIfSet(void) {
    if (__OSExpireSetExpiredFlag) {
        return __OSWriteExpiredFlag();
    }
    return 0;
}

static u32 __OSPlayTimeRebootThread(void* arg) {
    BOOL enabled;
    u8 fadeBuf[0x494] __attribute__((aligned(32)));
    u32 i;
    u32 level;

    __OSExpireAIFade = (FadeAIData*)fadeBuf;
    memset(fadeBuf, 0, sizeof(FadeAIData));

    // Store the previous AI DMA callback, register the fade callback
    ((FadeAIData *)__OSExpireAIFade)->callback =
        AIRegisterDMACallback(__OSPlayTimeFadeLastAIDCallback);

    // Gradual dimming over 20 retrace cycles
    for (i = 0; i < 20; i++) {
        /* i / 5 (mulhwu magic 0xCCCCCCCD) + 1, clamped to 7 */
        level = i / 5 + 1;
        if (level > 7) {
            level = 7;
        }
        VIWaitForRetrace();
        __OSSetVIForceDimming(1, level, level);
    }

    AIRegisterDMACallback(NULL);
    VISetBlack(TRUE);
    VIFlush();

    // Write the expired flag atomically
    {
        enabled = OSDisableInterrupts();
        if (__OSExpireSetExpiredFlag) {
            __OSWriteExpiredFlag();
        }
        OSRestoreInterrupts(enabled);
    }

    OSReturnToMenu();

    (void)arg;
    return 0;
}

void __OSPlayTimeAlarmExpired(OSAlarm* alarm, OSContext* ctx) {
    OSThread* thread;
    OSThread* rebootThread;
    void* stack;
    s32 prio;
    u16 flags;
    BOOL created;

    if (__OSExpireCallback != NULL) {
        __OSExpireCallback();
        return;
    }

    // Suspend all active threads
    thread = *(OSThread**)0x800000DC;
    while (thread != NULL) {
        OSSuspendThread(thread);
        thread = thread->nextActive;
    }

    // Create and start the reboot thread
    rebootThread = (OSThread*)(*(u32*)0x80003128 - 0x1320);
    prio = 0;
    flags = 0;
    stack = (char*)rebootThread + 0x1320;
    created = OSCreateThread(rebootThread, (OSThreadFunc)__OSPlayTimeRebootThread,
                             NULL, stack, 0x1000, prio, flags);
    if (!created) {
        __OSHotResetForError();
    }
    OSResumeThread(rebootThread);

    (void)alarm;
    (void)ctx;
}

s32 __OSGetPlayTime(ESTicketView* ticket, s32* outType, u32* outRemaining) {
    ESTicketView aligned __attribute__((aligned(32)));
    ESTicketView* view;
    u32 hasConsumed = 0;
    ESLpEntry entries[8] __attribute__((aligned(32)));
    s32 result;
    u32 lastUnknown = 0;
    u32 i;
    view = ticket;
    if (((u32)ticket & 31) != 0) {
        memcpy(&aligned, ticket, sizeof(ESTicketView));
        view = &aligned;
    }

    // Probe for consumption data (NULL entries = probe only)
    result = ESP_GetConsumption(view->ticketID, NULL, &hasConsumed);
    if (result > 0) {
        goto search_limits;
    }
    if (result == 0) {
        goto fetch_consumption;
    }
    if (result != 0) {
        goto search_limits;
    }

fetch_consumption:
    if (hasConsumed == 0) {
        goto search_limits;
    }
    result = ESP_GetConsumption(view->ticketID, entries, &hasConsumed);

search_limits:
    if (result == 0) {
        // Search the 8-element limits array
        for (i = 0; i < 8; i++) {
            u32 code = view->limits[i].code;
            if (code == 1) {
                *outType = 1;
                if (hasConsumed == 0) {
                    *outRemaining = view->limits[i].limit;
                } else {
                    u32 consumed = entries[i].limit;
                    u32 limit = view->limits[i].limit;
                    if (consumed >= limit) {
                        *outRemaining = 0;
                    } else {
                        *outRemaining = limit - consumed;
                    }
                }
                return result;
            }
            if (code != 0) {
                lastUnknown = i + 1;
            }
        }

        if (lastUnknown == 0) {
            *outType = 0;
            *outRemaining = (u32)-1;
            return result;
        }

        // Fallback: use the last non-zero, non-one limit entry
        i = lastUnknown - 1;
        if (view->limits[i].code == 4) {
            *outType = 4;
            {
                u32 limit = view->limits[i].limit;
                *outRemaining = limit;
                if (hasConsumed != 0) {
                    *outRemaining = limit - entries[i].limit;
                }
            }
        } else {
            *outType = 9;
        }

        return result;
    }

    return result;
}

void __OSInitPlayTime(void) {
    ESTicketView ticketView __attribute__((aligned(32)));
    u32 remaining;
    s32 resultType;
    s32 result;

    __OSExpireTime = 0;
    __OSExpireCallback = NULL;
    __OSExpireSetExpiredFlag = TRUE;

    result = ESP_InitLib();
    switch (result) {
    case 0:
        goto get_ticket_view;
    default:
        goto close_lib;
    }

get_ticket_view:
    result = ESP_DiGetTicketView(NULL, &ticketView);
    if (result == -0x3F9) {
        goto check_result;
    }
    if (result == 0) {
        goto get_play_time;
    }
    if (result != 0) {
        goto check_result;
    }

get_play_time:
    result = __OSGetPlayTime(&ticketView, &resultType, &remaining);

check_result:
    if (result == 0) {
        goto type_checks;
    }
    if (result == -0x3F9) {
        goto close_lib;
    }
    if (result != 0) {
        goto close_lib;
    }

type_checks:
    if (resultType == 0 || resultType != 1) {
        goto close_lib;
    }

    if (remaining == 0) {
        OSPanic("OSPlayTime.c", 0x2E1, "Expired");
    }

    // Set up an alarm that fires when the play time expires
    {
        u32 busClock;
        s64 tick;

        OSCreateAlarm(&__OSExpireAlarm);

        busClock = *(u32*)0x800000F8;
        tick = ((s64)remaining + 20) * (s64)(busClock >> 2);

        OSSetAlarm(&__OSExpireAlarm, tick, __OSPlayTimeAlarmExpired);

        // Store the alarm end time for OSPlayTimeIsLimited
        __OSExpireTime = __OSExpireAlarm.end;

        OSReport("PlayTime: %d seconds left\n", remaining);
    }

close_lib:
    ESP_CloseLib();
}
