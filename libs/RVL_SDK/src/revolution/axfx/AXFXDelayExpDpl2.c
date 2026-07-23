#include <revolution/AX.h>
#include <revolution/AXFX.h>
#include <revolution/OS.h>

#include <string.h>

static BOOL __InitParams(AXFX_DELAY_EXP_DPL2* fx);

#pragma scheduling off
u32 AXFXDelayExpGetMemSizeDpl2(const AXFX_DELAY_EXP_DPL2* fx) {
    return (u32)(s32)(32.0f * fx->delayTimeMax) << 4;
}
#pragma scheduling on

BOOL AXFXDelayExpInitDpl2(AXFX_DELAY_EXP_DPL2* fx) {
    u32 i;
    BOOL ok;
    BOOL nested;
    BOOL mask = OSDisableInterrupts();
    s32** p;

    if (AXGetMode() != AX_OUTPUT_DPL2) {
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    fx->active = 1;

    if (fx->delayTimeMax <= 0.0f) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        p = fx->line;
        for (i = 0; i < 4; i++) {
            if (*p != NULL) {
                __AXFXFree(*p);
            }
            *p = NULL;
            p++;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    fx->maxLength = (u32)(32.0f * fx->delayTimeMax);
    if (fx->maxLength == 0) {
        fx->maxLength = 1;
    }

    p = fx->line;
    for (i = 0; i < 4; i++) {
        *p = (s32*)__AXFXAlloc(fx->maxLength * 4);
        if (*p == NULL) {
            ok = FALSE;
            goto alloc_done;
        }
        p++;
    }
    ok = TRUE;
alloc_done:
    if (ok == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        p = fx->line;
        for (i = 0; i < 4; i++) {
            if (*p != NULL) {
                __AXFXFree(*p);
            }
            *p = NULL;
            p++;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    if (__InitParams(fx) == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        p = fx->line;
        for (i = 0; i < 4; i++) {
            if (*p != NULL) {
                __AXFXFree(*p);
            }
            *p = NULL;
            p++;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    fx->active |= 2;
    fx->active &= ~1;
    OSRestoreInterrupts(mask);
    return TRUE;
}

BOOL AXFXDelayExpSettingsDpl2(AXFX_DELAY_EXP_DPL2* fx) {
    u32 i;
    BOOL result;
    BOOL mask = OSDisableInterrupts();
    BOOL nested;
    s32** p;

    fx->active |= 1;

    nested = OSDisableInterrupts();
    fx->active |= 1;
    p = fx->line;
    for (i = 0; i < 4; i++) {
        if (*p != NULL) {
            __AXFXFree(*p);
        }
        *p = NULL;
        p++;
    }
    OSRestoreInterrupts(nested);

    result = AXFXDelayExpInitDpl2(fx);
    if (result == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        p = fx->line;
        for (i = 0; i < 4; i++) {
            if (*p != NULL) {
                __AXFXFree(*p);
            }
            *p = NULL;
            p++;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    fx->active |= 2;
    fx->active &= ~1;
    OSRestoreInterrupts(mask);
    return TRUE;
}

BOOL AXFXDelayExpSettingsUpdateDpl2(AXFX_DELAY_EXP_DPL2* fx) {
    u32 i;
    BOOL nested;
    BOOL mask;
    s32** p;

    mask = OSDisableInterrupts();
    fx->active |= 1;
    if (__InitParams(fx) == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        p = fx->line;
        for (i = 0; i < 4; i++) {
            if (*p != NULL) {
                __AXFXFree(*p);
            }
            *p = NULL;
            p++;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    fx->active |= 2;
    fx->active &= ~1;
    OSRestoreInterrupts(mask);
    return TRUE;
}

void AXFXDelayExpShutdownDpl2(AXFX_DELAY_EXP_DPL2* fx) {
    u32 i;
    BOOL mask = OSDisableInterrupts();
    s32** p;

    fx->active |= 1;
    p = fx->line;
    for (i = 0; i < 4; i++) {
        if (*p != NULL) {
            __AXFXFree(*p);
        }
        *p = NULL;
        p++;
    }
    OSRestoreInterrupts(mask);
}

void AXFXDelayExpCallbackDpl2(AXFX_BUFFERUPDATE_DPL2* update, AXFX_DELAY_EXP_DPL2* fx) {
    s32 coef;
    s32 invCoef;
    s32* input[4];
    s32* inBus[4];
    s32* outBus[4];
    s32 delayed[4];
    s32 mixed[4];
    u32 samp;
    u32 ch;
    u32 pos;
    s32 in;

    if (fx->active != 0) {
        fx->active &= ~2;
        return;
    }

    coef = fx->iirGain;
    invCoef = 0x80 - coef;
    input[0] = update->left;
    input[1] = update->right;
    input[2] = update->left_surround;
    input[3] = update->right_surround;

    if (fx->busIn != NULL) {
        inBus[0] = fx->busIn->left;
        inBus[1] = fx->busIn->right;
        inBus[2] = fx->busIn->left_surround;
        inBus[3] = fx->busIn->right_surround;
    }

    if (fx->busOut != NULL) {
        outBus[0] = fx->busOut->left;
        outBus[1] = fx->busOut->right;
        outBus[2] = fx->busOut->left_surround;
        outBus[3] = fx->busOut->right_surround;
    }

    for (samp = 0; samp < 96; samp++) {
        for (ch = 0; ch < 4; ch++) {
            pos = fx->curPos;
            delayed[ch] = fx->line[ch][pos];

            if (fx->busIn != NULL) {
                in = *input[ch] + *inBus[ch]++;
                mixed[ch] = invCoef * in + coef * fx->last[ch];
            } else {
                mixed[ch] = invCoef * (*input[ch]) + coef * fx->last[ch];
            }

            mixed[ch] >>= 7;
            fx->last[ch] = mixed[ch];

            fx->line[ch][pos] = mixed[ch] + ((delayed[ch] * fx->feedbackGain) >> 7);

            *input[ch]++ = (delayed[ch] * fx->outGainI) >> 7;

            if (fx->busOut != NULL) {
                *outBus[ch]++ = (delayed[ch] * fx->sendGainI) >> 7;
            }
        }

        pos = fx->curPos + 1;
        fx->curPos = pos;
        if (pos >= fx->length) {
            fx->curPos = 0;
        }
    }
}

static BOOL __InitParams(AXFX_DELAY_EXP_DPL2* fx) {
    f32 iir;
    u32 i;

    if (fx->delayTime > fx->delayTimeMax) {
        return FALSE;
    }

    if (fx->feedback < 0.0f || fx->feedback >= 1.0f) {
        return FALSE;
    }

    if (fx->iir < 0.0f || fx->iir > 1.0f) {
        return FALSE;
    }

    if (fx->outGain < 0.0f || fx->outGain > 1.0f) {
        return FALSE;
    }

    if (fx->sendGain < 0.0f || fx->sendGain > 1.0f) {
        return FALSE;
    }

    fx->length = (u32)(32.0f * fx->delayTime);
    if (fx->length == 0) {
        fx->length = 1;
    }

    fx->curPos = 0;
    fx->feedbackGain = (s32)(128.0f * fx->feedback);

    iir = 1.0f - fx->iir;
    if (iir > 0.95f) {
        iir = 0.95f;
    }
    fx->iirGain = (s32)(128.0f * iir);

    for (i = 0; i < 4; i++) {
        memset(fx->line[i], 0, fx->maxLength * 4);
        fx->last[i] = 0;
    }

    fx->outGainI = (s32)(128.0f * fx->outGain);
    fx->sendGainI = (s32)(128.0f * fx->sendGain);
    return TRUE;
}
