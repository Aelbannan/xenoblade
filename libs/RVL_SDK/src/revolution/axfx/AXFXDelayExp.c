#include <revolution/AXFX.h>
#include <revolution/OS.h>

#include <string.h>

static BOOL __InitParams(AXFX_DELAY_EXP* fx);

#pragma scheduling off
u32 AXFXDelayExpGetMemSize(const AXFX_DELAY_EXP* fx) {
    return (u32)(s32)(32.0f * fx->delayTimeMax) * 0xc;
}
#pragma scheduling on

BOOL AXFXDelayExpInit(AXFX_DELAY_EXP* fx) {
    BOOL ok;
    BOOL mask = OSDisableInterrupts();
    BOOL nested;

    fx->active = 1;

    if (fx->delayTimeMax <= 0.0f) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        if (fx->line[0] != NULL) {
            __AXFXFree(fx->line[0]);
            fx->line[0] = NULL;
        }
        if (fx->line[1] != NULL) {
            __AXFXFree(fx->line[1]);
            fx->line[1] = NULL;
        }
        if (fx->line[2] != NULL) {
            __AXFXFree(fx->line[2]);
            fx->line[2] = NULL;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    fx->maxLength = (u32)(32.0f * fx->delayTimeMax);
    if (fx->maxLength == 0) {
        fx->maxLength = 1;
    }

    fx->line[0] = (s32*)__AXFXAlloc(fx->maxLength * 4);
    if (fx->line[0] == NULL) {
        ok = FALSE;
    } else {
        fx->line[1] = (s32*)__AXFXAlloc(fx->maxLength * 4);
        if (fx->line[1] == NULL) {
            ok = FALSE;
        } else {
            fx->line[2] = (s32*)__AXFXAlloc(fx->maxLength * 4);
            ok = fx->line[2] != NULL;
        }
    }

    if (ok == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        if (fx->line[0] != NULL) {
            __AXFXFree(fx->line[0]);
            fx->line[0] = NULL;
        }
        if (fx->line[1] != NULL) {
            __AXFXFree(fx->line[1]);
            fx->line[1] = NULL;
        }
        if (fx->line[2] != NULL) {
            __AXFXFree(fx->line[2]);
            fx->line[2] = NULL;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    if (__InitParams(fx) == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        if (fx->line[0] != NULL) {
            __AXFXFree(fx->line[0]);
            fx->line[0] = NULL;
        }
        if (fx->line[1] != NULL) {
            __AXFXFree(fx->line[1]);
            fx->line[1] = NULL;
        }
        if (fx->line[2] != NULL) {
            __AXFXFree(fx->line[2]);
            fx->line[2] = NULL;
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

BOOL AXFXDelayExpSettings(AXFX_DELAY_EXP* fx) {
    BOOL result;
    BOOL mask = OSDisableInterrupts();
    BOOL nested;

    fx->active |= 1;

    nested = OSDisableInterrupts();
    fx->active |= 1;
    if (fx->line[0] != NULL) {
        __AXFXFree(fx->line[0]);
        fx->line[0] = NULL;
    }
    if (fx->line[1] != NULL) {
        __AXFXFree(fx->line[1]);
        fx->line[1] = NULL;
    }
    if (fx->line[2] != NULL) {
        __AXFXFree(fx->line[2]);
        fx->line[2] = NULL;
    }
    OSRestoreInterrupts(nested);

    result = AXFXDelayExpInit(fx);
    if (result == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        if (fx->line[0] != NULL) {
            __AXFXFree(fx->line[0]);
            fx->line[0] = NULL;
        }
        if (fx->line[1] != NULL) {
            __AXFXFree(fx->line[1]);
            fx->line[1] = NULL;
        }
        if (fx->line[2] != NULL) {
            __AXFXFree(fx->line[2]);
            fx->line[2] = NULL;
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

BOOL AXFXDelayExpSettingsUpdate(AXFX_DELAY_EXP* fx) {
    BOOL nested;
    BOOL mask;

    mask = OSDisableInterrupts();
    fx->active |= 1;
    if (__InitParams(fx) == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        if (fx->line[0] != NULL) {
            __AXFXFree(fx->line[0]);
            fx->line[0] = NULL;
        }
        if (fx->line[1] != NULL) {
            __AXFXFree(fx->line[1]);
            fx->line[1] = NULL;
        }
        if (fx->line[2] != NULL) {
            __AXFXFree(fx->line[2]);
            fx->line[2] = NULL;
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

void AXFXDelayExpShutdown(AXFX_DELAY_EXP* fx) {
    BOOL mask = OSDisableInterrupts();

    fx->active |= 1;
    if (fx->line[0] != NULL) {
        __AXFXFree(fx->line[0]);
        fx->line[0] = NULL;
    }
    if (fx->line[1] != NULL) {
        __AXFXFree(fx->line[1]);
        fx->line[1] = NULL;
    }
    if (fx->line[2] != NULL) {
        __AXFXFree(fx->line[2]);
        fx->line[2] = NULL;
    }
    OSRestoreInterrupts(mask);
}

void AXFXDelayExpCallback(AXFX_BUFFERUPDATE* update, AXFX_DELAY_EXP* fx) {
    s32* inputL;
    s32* inputR;
    s32* inputS;
    s32* inL;
    s32* inR;
    s32* inS;
    s32* outL;
    s32* outR;
    s32* outS;
    AXFX_BUS* busIn;
    AXFX_BUS* busOut;
    u32 samp;
    s32 delayedL;
    s32 delayedR;
    s32 delayedS;
    s32 mixedL;
    s32 mixedR;
    s32 mixedS;
    s32 coef;
    s32 invCoef;
    u32 pos;

    if (fx->active != 0) {
        fx->active &= ~2;
        return;
    }

    busIn = fx->busIn;
    coef = fx->iirGain;
    inputL = update->left;
    inputR = update->right;
    invCoef = 0x80 - coef;
    inputS = update->surround;

    if (busIn != NULL) {
        inL = busIn->left;
        inR = busIn->right;
        inS = busIn->surround;
    }

    busOut = fx->busOut;
    if (busOut != NULL) {
        outL = busOut->left;
        outR = busOut->right;
        outS = busOut->surround;
    }

    for (samp = 0; samp < 96; samp++) {
        pos = fx->curPos;
        delayedL = fx->line[0][pos];
        delayedR = fx->line[1][pos];
        delayedS = fx->line[2][pos];

        if (fx->busIn != NULL) {
            mixedL = invCoef * (*inputL + *inL++) + coef * fx->last[0];
            mixedR = invCoef * (*inputR + *inR++) + coef * fx->last[1];
            mixedS = invCoef * (*inputS + *inS++) + coef * fx->last[2];
        } else {
            mixedL = invCoef * (*inputL) + coef * fx->last[0];
            mixedR = invCoef * (*inputR) + coef * fx->last[1];
            mixedS = invCoef * (*inputS) + coef * fx->last[2];
        }

        mixedL >>= 7;
        mixedR >>= 7;
        mixedS >>= 7;

        fx->last[1] = mixedR;
        fx->last[2] = mixedS;
        fx->last[0] = mixedL;

        fx->line[0][pos] = mixedL + ((delayedL * fx->feedbackGain) >> 7);
        fx->line[1][pos] = mixedR + ((delayedR * fx->feedbackGain) >> 7);
        fx->line[2][pos] = mixedS + ((delayedS * fx->feedbackGain) >> 7);

        pos++;
        fx->curPos = pos;
        if (pos >= fx->length) {
            fx->curPos = 0;
        }

        *inputL++ = (delayedL * fx->outGainI) >> 7;
        *inputR++ = (delayedR * fx->outGainI) >> 7;
        *inputS++ = (delayedS * fx->outGainI) >> 7;

        if (fx->busOut != NULL) {
            *outL++ = (delayedL * fx->sendGainI) >> 7;
            *outR++ = (delayedR * fx->sendGainI) >> 7;
            *outS++ = (delayedS * fx->sendGainI) >> 7;
        }
    }
}

static BOOL __InitParams(AXFX_DELAY_EXP* fx) {
    f32 iir;

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

    if (fx->line[0] == NULL) {
        return FALSE;
    }
    if (fx->line[1] == NULL) {
        return FALSE;
    }
    if (fx->line[2] == NULL) {
        return FALSE;
    }

    memset(fx->line[0], 0, fx->maxLength * 4);
    memset(fx->line[1], 0, fx->maxLength * 4);
    memset(fx->line[2], 0, fx->maxLength * 4);

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

    fx->last[0] = 0;
    fx->last[1] = 0;
    fx->iirGain = (s32)(128.0f * iir);
    fx->last[2] = 0;
    fx->outGainI = (s32)(128.0f * fx->outGain);
    fx->sendGainI = (s32)(128.0f * fx->sendGain);
    return TRUE;
}
