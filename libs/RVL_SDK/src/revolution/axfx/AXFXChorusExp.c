#include <revolution/AXFX.h>
#include <revolution/OS.h>

#include <string.h>

// Float/double constants in .sdata2 (loaded via extern to match retail relocs)
extern const f32 float_8066BED0; // 32.0f
extern const f32 float_8066BED4; // 0.0f
extern const f32 float_8066BEE0; // 0.1f
extern const f32 float_8066BEE4; // 50.0f
extern const f32 float_8066BEE8; // 1.0f
extern const f32 float_8066BEEC; // 2.0f
extern const f32 float_8066BEF0; // 65536.0f
extern const f32 float_8066BEF4; // 256.0f
extern const f32 float_8066BEF8; // 32000.0f
extern const f32 float_8066BEFC; // 0.00390625f

static BOOL __InitParams(AXFX_CHORUS_EXP* fx);
static void __CalcLFO(s32* dst, AXFX_CHORUS_EXP_LFO* lfo);

u32 AXFXChorusExpGetMemSize(const AXFX_CHORUS_EXP* fx) {
#pragma unused(fx)
    return 0xc80 * 3 * sizeof(f32);
}

BOOL AXFXChorusExpInit(AXFX_CHORUS_EXP* fx) {
    f32** walk;
    u32 i;
    u32 k;
    u32 j;
    BOOL ok;
    BOOL mask = OSDisableInterrupts();
    BOOL nested;

    fx->active = 1;
    fx->delay.size = 0xc80;

    walk = fx->delay.line;
    for (i = 0; i < 3; i++) {
        *walk = (f32*)__AXFXAlloc(fx->delay.size * 4);
        if (*walk == NULL) {
            ok = FALSE;
            goto alloc_done;
        }
        walk++;
    }
    ok = TRUE;
alloc_done:
    if (ok == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        for (i = 0; i < 3; i++) {
            if (fx->delay.line[i] != NULL) {
                __AXFXFree(fx->delay.line[i]);
            }
            fx->delay.line[i] = NULL;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    {
        f32** walkM;
        walkM = fx->delay.line;
        for (k = 0; k < 3; k++) {
            if (*walkM == NULL) {
                ok = FALSE;
                goto memset_done;
            }
            memset(*walkM, 0, fx->delay.size * 4);
            walkM++;
        }
    }
    fx->delay.inPos = 0;
    fx->delay.outPos = (fx->delay.size - (u32)(float_8066BED0 * fx->delayTime)) << 16;
    fx->delay.lastPos = fx->delay.outPos;
    fx->delay.sizeFP = fx->delay.size << 16;
    ok = TRUE;
memset_done:
    if (ok == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        for (j = 0; j < 3; j++) {
            if (fx->delay.line[j] != NULL) {
                __AXFXFree(fx->delay.line[j]);
            }
            fx->delay.line[j] = NULL;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    if (__InitParams(fx) == FALSE) {
        nested = OSDisableInterrupts();
        fx->active |= 1;
        for (j = 0; j < 3; j++) {
            if (fx->delay.line[j] != NULL) {
                __AXFXFree(fx->delay.line[j]);
            }
            fx->delay.line[j] = NULL;
        }
        OSRestoreInterrupts(nested);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    fx->active &= ~1;
    OSRestoreInterrupts(mask);
    return TRUE;
}

BOOL AXFXChorusExpSettingsUpdate(AXFX_CHORUS_EXP* fx) {
    f32** walk;
    u32 i;
    u32 j;
    BOOL ok;
    BOOL result;
    BOOL mask = OSDisableInterrupts();
    BOOL nested;

    fx->active |= 1;

    walk = fx->delay.line;
    for (i = 0; i < 3; i++) {
        if (*walk == NULL) {
            ok = FALSE;
            goto memset_done;
        }
        memset(*walk, 0, fx->delay.size * 4);
        walk++;
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
        for (i = 0; i < 3; i++) {
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
        for (j = 0; j < 3; j++) {
            if (fx->delay.line[j] != NULL) {
                __AXFXFree(fx->delay.line[j]);
            }
            fx->delay.line[j] = NULL;
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

void AXFXChorusExpShutdown(AXFX_CHORUS_EXP* fx) {
    u32 i;
    BOOL mask = OSDisableInterrupts();

    fx->active |= 1;
    for (i = 0; i < 3; i++) {
        if (fx->delay.line[i] != NULL) {
            __AXFXFree(fx->delay.line[i]);
        }
        fx->delay.line[i] = NULL;
    }
    OSRestoreInterrupts(mask);
}

void AXFXChorusExpCallback(AXFX_BUFFERUPDATE* update, AXFX_CHORUS_EXP* fx) {
    s32 lfo[96];
    s32* input[3];
    s32* inBus[3];
    s32* outBus[3];
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
    input[2] = update->surround;

    if (fx->busIn != NULL) {
        inBus[0] = fx->busIn->left;
        inBus[1] = fx->busIn->right;
        inBus[2] = fx->busIn->surround;
    }

    if (fx->busOut != NULL) {
        outBus[0] = fx->busOut->left;
        outBus[1] = fx->busOut->right;
        outBus[2] = fx->busOut->surround;
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
                histIndex = (histIndex + 1) & 3;
                histPos++;
                if (histPos >= fx->delay.size) {
                    histPos = 0;
                }
            } while (--steps != 0);
        }

        fx->delay.lastPos = (u32)pos & ~0xFFFFu;
        coef = __AXFXGetSrcCoef((frac >> 9) & 0x7F);

        for (ch = 0; ch < 3; ch++) {
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

static BOOL __InitParams(AXFX_CHORUS_EXP* fx) {
    f32 base;
    f32 depthSamp;
    f32 step;
    f32 phaseAdd;
    u32 i;
    u32 j;

    if (fx->delayTime < float_8066BEE0 || fx->delayTime > float_8066BEE4) {
        return FALSE;
    }
    if (fx->depth < float_8066BED4 || fx->depth > float_8066BEE8) {
        return FALSE;
    }
    if (fx->rate < float_8066BEE0 || fx->rate > float_8066BEEC) {
        return FALSE;
    }
    if (fx->feedback < float_8066BED4 || fx->feedback >= float_8066BEE8) {
        return FALSE;
    }
    if (fx->outGain < float_8066BED4 || fx->outGain > float_8066BEE8) {
        return FALSE;
    }
    if (fx->sendGain < float_8066BED4 || fx->sendGain > float_8066BEE8) {
        return FALSE;
    }

    fx->lfo.table = __AXFXGetLfoSinTable();

    base = float_8066BED0 * fx->delayTime;
    depthSamp = base * fx->depth;
    if (depthSamp >= base) {
        depthSamp -= float_8066BEE8;
        if (depthSamp < float_8066BED4) {
            depthSamp = float_8066BED4;
        }
    }

    {
        f32 rate = fx->rate;
        step = (float_8066BEF8 / rate) * float_8066BEFC;
        phaseAdd = (float_8066BEF4 * rate) / float_8066BEF8;
    }

    fx->lfo.lastNum = (u32)-1;
    fx->lfo.phase = 0;
    fx->lfo.sign = 0;
    fx->lfo.lastValue = 0;
    fx->lfo.grad = 0;

    fx->lfo.depthSamp = (s32)(float_8066BEF0 * depthSamp);
    fx->lfo.phaseAdd = (s32)(float_8066BEF0 * phaseAdd);
    fx->lfo.gradFactor = (s32)(float_8066BEF0 * (depthSamp / step));
    fx->lfo.stepSamp = (s32)(float_8066BEF0 * step);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            fx->history[i][j] = float_8066BED4;
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
    s64 delta;
    s32 curr;
    s32 next;

    for (i = 0; i < 0x60; i++) {
        currNum = lfo->phase & ~0xFFFFu;

        if (currNum != lfo->lastNum) {
            lfo->lastNum = currNum;
            idx = currNum >> 16;
            curr = lfo->table[idx];
            next = lfo->table[(idx + 1) & 0x7F];
            delta = (s64)next - (s64)curr;
            value = ((s64)curr * lfo->depthSamp) >> 24;
            lfo->grad = (s32)((delta * (s64)lfo->gradFactor) >> 24);
        } else {
            value = (s64)(lfo->lastValue + lfo->grad);
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
