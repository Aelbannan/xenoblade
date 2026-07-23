#include <revolution/AX.h>
#include <revolution/AXFX.h>
#include <revolution/OS.h>

#include <string.h>

static BOOL __InitParams(AXFX_CHORUS_EXP_DPL2* fx);
static void __CalcLFO(s32* dst, AXFX_CHORUS_EXP_LFO* lfo);

u32 AXFXChorusExpGetMemSizeDpl2(const AXFX_CHORUS_EXP_DPL2* fx) {
#pragma unused(fx)
    return 0xc80 * 4 * sizeof(f32);
}

BOOL AXFXChorusExpInitDpl2(AXFX_CHORUS_EXP_DPL2* fx) {
    u32 i;
    BOOL ok;
    BOOL mask = OSDisableInterrupts();
    BOOL nested;

    if (AXGetMode() != AX_OUTPUT_DPL2) {
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    fx->active = 1;
    fx->delay.size = 0xc80;

    for (i = 0; i < 4; i++) {
        fx->delay.line[i] = (f32*)__AXFXAlloc(fx->delay.size * 4);
        if (fx->delay.line[i] == NULL) {
            ok = FALSE;
            goto alloc_done;
        }
    }
    ok = TRUE;
alloc_done:
    if (ok == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        for (i = 0; i < 4; i++) {
            if (fx->delay.line[i] != NULL) {
                __AXFXFree(fx->delay.line[i]);
            }
            fx->delay.line[i] = NULL;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    for (i = 0; i < 4; i++) {
        if (fx->delay.line[i] == NULL) {
            ok = FALSE;
            goto memset_done;
        }
        memset(fx->delay.line[i], 0, fx->delay.size * 4);
    }
    fx->delay.inPos = 0;
    fx->delay.outPos = (fx->delay.size - (u32)(32.0f * fx->delayTime)) << 16;
    fx->delay.lastPos = fx->delay.outPos;
    fx->delay.sizeFP = fx->delay.size << 16;
    ok = TRUE;
memset_done:
    if (ok == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        for (i = 0; i < 4; i++) {
            if (fx->delay.line[i] != NULL) {
                __AXFXFree(fx->delay.line[i]);
            }
            fx->delay.line[i] = NULL;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    if (__InitParams(fx) == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        for (i = 0; i < 4; i++) {
            if (fx->delay.line[i] != NULL) {
                __AXFXFree(fx->delay.line[i]);
            }
            fx->delay.line[i] = NULL;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    fx->active &= ~1;
    OSRestoreInterrupts(mask);
    return TRUE;
}

BOOL AXFXChorusExpSettingsUpdateDpl2(AXFX_CHORUS_EXP_DPL2* fx) {
    u32 i;
    BOOL ok;
    BOOL result;
    BOOL mask = OSDisableInterrupts();
    BOOL nested;

    fx->active |= 1;

    for (i = 0; i < 4; i++) {
        if (fx->delay.line[i] == NULL) {
            ok = FALSE;
            goto memset_done;
        }
        memset(fx->delay.line[i], 0, fx->delay.size * 4);
    }
    fx->delay.inPos = 0;
    fx->delay.outPos = (fx->delay.size - (u32)(32.0f * fx->delayTime)) << 16;
    fx->delay.lastPos = fx->delay.outPos;
    fx->delay.sizeFP = fx->delay.size << 16;
    ok = TRUE;
memset_done:
    if (ok == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        for (i = 0; i < 4; i++) {
            if (fx->delay.line[i] != NULL) {
                __AXFXFree(fx->delay.line[i]);
            }
            fx->delay.line[i] = NULL;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    result = __InitParams(fx);
    if (result == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        for (i = 0; i < 4; i++) {
            if (fx->delay.line[i] != NULL) {
                __AXFXFree(fx->delay.line[i]);
            }
            fx->delay.line[i] = NULL;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    fx->active |= 2;
    fx->active &= ~1;
    OSRestoreInterrupts(mask);
    return result;
}

void AXFXChorusExpShutdownDpl2(AXFX_CHORUS_EXP_DPL2* fx) {
    u32 i;
    BOOL mask = OSDisableInterrupts();

    fx->active |= 1;
    for (i = 0; i < 4; i++) {
        if (fx->delay.line[i] != NULL) {
            __AXFXFree(fx->delay.line[i]);
        }
        fx->delay.line[i] = NULL;
    }
    OSRestoreInterrupts(mask);
}

void AXFXChorusExpCallbackDpl2(AXFX_BUFFERUPDATE_DPL2* update, AXFX_CHORUS_EXP_DPL2* fx) {
    s32 lfo[96];
    s32* input[4];
    s32* inBus[4];
    s32* outBus[4];
    u32 samp;
    u32 ch;
    s32 pos;
    s32 diff;
    u32 steps;
    u32 frac;
    u32 histPos;
    u32 histIndex;
    SrcCoef* coef;
    f32 out;
    f32 data;

    if (fx->active != 0) {
        fx->active &= ~2;
        return;
    }

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

    __CalcLFO(lfo, &fx->lfo);

    for (samp = 0; samp < 96; samp++) {
        pos = (s32)fx->delay.outPos + lfo[samp];
        if (pos >= (s32)fx->delay.sizeFP) {
            pos -= (s32)fx->delay.sizeFP;
        } else if (pos < 0) {
            pos += (s32)fx->delay.sizeFP;
        }

        diff = pos - (s32)fx->delay.lastPos;
        if (diff < 0) {
            diff += (s32)fx->delay.sizeFP;
        }

        steps = (u32)diff >> 16;
        frac = (u32)diff & 0xFFFF;
        histPos = fx->delay.lastPos >> 16;
        histIndex = fx->histIndex;

        if (steps != 0) {
            do {
                fx->history[0][histIndex] = fx->delay.line[0][histPos];
                fx->history[1][histIndex] = fx->delay.line[1][histPos];
                fx->history[2][histIndex] = fx->delay.line[2][histPos];
                fx->history[3][histIndex] = fx->delay.line[3][histPos];
                histIndex = (histIndex + 1) & 3;
                histPos++;
                if (histPos >= fx->delay.size) {
                    histPos = 0;
                }
            } while (--steps != 0);
        }

        fx->delay.lastPos = (u32)pos & ~0xFFFFu;
        coef = __AXFXGetSrcCoef((frac >> 9) & 0x7F);

        for (ch = 0; ch < 4; ch++) {
            out = 0.0f;
            out += coef->unk0 * fx->history[ch][histIndex];
            histIndex = (histIndex + 1) & 3;
            out += coef->unk4 * fx->history[ch][histIndex];
            histIndex = (histIndex + 1) & 3;
            out += coef->unk8 * fx->history[ch][histIndex];
            histIndex = (histIndex + 1) & 3;
            out += coef->unkC * fx->history[ch][histIndex];
            histIndex = (histIndex + 1) & 3;

            if (fx->busIn != NULL) {
                data = (f32)(*input[ch] + *inBus[ch]++);
            } else {
                data = (f32)(*input[ch]);
            }

            fx->delay.line[ch][fx->delay.inPos] = data + out * fx->feedback;
            *input[ch]++ = (s32)(out * fx->outGain);

            if (fx->busOut != NULL) {
                *outBus[ch]++ = (s32)(out * fx->sendGain);
            }
        }

        fx->histIndex = histIndex;

        fx->delay.inPos++;
        if (fx->delay.inPos >= fx->delay.size) {
            fx->delay.inPos = 0;
        }

        fx->delay.outPos += 0x10000;
        if (fx->delay.outPos >= fx->delay.sizeFP) {
            fx->delay.outPos = 0;
        }
    }
}

static BOOL __InitParams(AXFX_CHORUS_EXP_DPL2* fx) {
    f32 base;
    f32 depthSamp;
    f32 step;
    f32 phaseAdd;
    f32 gradFactor;
    u32 i;
    u32 j;

    if (fx->delayTime < 0.1f || fx->delayTime > 50.0f) {
        return FALSE;
    }
    if (fx->depth < 0.0f || fx->depth > 1.0f) {
        return FALSE;
    }
    if (fx->rate < 0.1f || fx->rate > 2.0f) {
        return FALSE;
    }
    if (fx->feedback < 0.0f || fx->feedback >= 1.0f) {
        return FALSE;
    }
    if (fx->outGain < 0.0f || fx->outGain > 1.0f) {
        return FALSE;
    }
    if (fx->sendGain < 0.0f || fx->sendGain > 1.0f) {
        return FALSE;
    }

    fx->lfo.table = __AXFXGetLfoSinTable();

    base = 32.0f * fx->delayTime;
    depthSamp = base * fx->depth;
    if (depthSamp >= base) {
        depthSamp -= 1.0f;
        if (depthSamp < 0.0f) {
            depthSamp = 0.0f;
        }
    }

    step = (32000.0f / fx->rate) * 0.00390625f;
    phaseAdd = (256.0f * fx->rate) / 32000.0f;
    gradFactor = depthSamp / step;

    fx->lfo.lastNum = (u32)-1;
    fx->lfo.phase = 0;
    fx->lfo.sign = 0;
    fx->lfo.lastValue = 0;
    fx->lfo.grad = 0;

    fx->lfo.depthSamp = (s32)(65536.0f * depthSamp);
    fx->lfo.phaseAdd = (s32)(65536.0f * phaseAdd);
    fx->lfo.stepSamp = (s32)(65536.0f * step);
    fx->lfo.gradFactor = (s32)(65536.0f * gradFactor);

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            fx->history[i][j] = 0.0f;
        }
    }
    fx->histIndex = 0;

    return TRUE;
}

static void __CalcLFO(s32* dst, AXFX_CHORUS_EXP_LFO* lfo) {
    u32 i;
    u32 currNum;
    u32 idx;
    s64 value;
    s32 curr;
    s32 next;
    s32 delta;

    for (i = 0; i < 0x60; i++) {
        currNum = lfo->phase & ~0xFFFFu;

        if (currNum != lfo->lastNum) {
            lfo->lastNum = currNum;
            idx = currNum >> 16;
            curr = lfo->table[idx];
            next = lfo->table[(idx + 1) & 0x7F];
            delta = next - curr;
            value = ((s64)curr * lfo->depthSamp) >> 24;
            lfo->grad = (s32)(((s64)delta * lfo->gradFactor) >> 24);
        } else {
            value = (s64)lfo->lastValue + lfo->grad;
        }

        lfo->lastValue = (s32)value;
        if (lfo->sign >= 1) {
            value = -value;
        }

        lfo->phase += lfo->phaseAdd;
        if ((lfo->phase & 0xFF800000u) != 0) {
            lfo->phase &= 0x7FFFFFu;
            lfo->sign ^= 1;
        }

        *dst++ = (s32)value;
    }
}
