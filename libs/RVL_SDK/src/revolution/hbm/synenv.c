// Decompiled RVL_SDK/src/revolution/hbm/synenv - volume envelope setup/run
#include <harness_catalog.h>
#include <math.h>

extern const f32 __HBMSYNn128[];
extern const s32 __HBMSYNAttackAttnTable[];

typedef struct HBMSYNVoice {
    u8  _pad0[0x08];
    void* base;        // 0x08 - synth base pointer
    u8  ch;            // 0x0C - column index
    u8  note;          // 0x0D - note index
    u8  idx;           // 0x0E
    u8  _pad0F;
    u8  _pad10[0x04];  // 0x10-0x13
    void* params;      // 0x14 - volume envelope params
    u8  _pad18[0x18];  // 0x18-0x2F
    s32 volPhase;      // 0x30 - 0=attack, 1=decay, 2=sustain, 3=release
    s32 vol;           // 0x34
    s32 attackTicks;   // 0x38
    s32 attackRate;    // 0x3C
    s32 decayRate;     // 0x40
    s32 sustain;       // 0x44
    s32 release;       // 0x48
} HBMSYNVoice;

typedef struct HBMSYNVolParams {
    u8  _pad0[0x18];
    s32 attack;        // 0x18
    s32 decay;         // 0x1C
    s32 sustain;       // 0x20
    s32 release;       // 0x24
    s32 attackExtra;   // 0x28
    s32 decayExtra;    // 0x2C
} HBMSYNVolParams;

// Volume envelope setup: attack/decay tick conversion via pow(2.0, ...).
// int->float uses plain (f32) builtin casts (MWCC's xoris/lfd-pool/fsubs
// idiom); 2.0 pools first via the (void)2.0 anchor (single r31 base, exact
// retail pool layout [2.0, divA, mulMs, divB, magic]). A const-qualified or
// function-scope powBase variable instead breaks pooling or frame shape.
extern "C" void __HBMSYNSetupVolumeEnvelope(void* voice)
{
    HBMSYNVoice* v = (HBMSYNVoice*)voice;
    s32 ticks;
    HBMSYNVolParams* pp0 = (HBMSYNVolParams*)v->params;

    // Pool anchor only: pools 2.0 at the head of the compiler literal pool
    // (exact retail layout). A bare literal statement keeps 2.0 first with
    // no variable, no code, and no virtual-register numbering of its own.
    // The arms below use literal 2.0 at the pow sites (deduped here).
    (void)2.0;
    if (pp0->attack == 0x80000000) {
        v->volPhase = 1;
        v->vol = 0;
        if (pp0->decay == 0x80000000) {
            v->volPhase = 2;
            v->vol = pp0->sustain;
        }
    } else {
        s32 extra;
        u8 ch;
        ch = v->idx;
        extra = pp0->attackExtra;

        // Dead gate: re-tests the outer value, so MWCC folds it onto the
        // stale CR0 -- the ticks = 0 path costs no compare.
        if (pp0->attack == 0x80000000) {
            ticks = 0;
        } else if (extra == 0x80000000) {
            f32 a = (f32)pp0->attack;
            f32 arg = a / 78643200.0f;
            f32 e = (f32)pow(2.0, (f64)arg);
            ticks = (s32)(1000.0f * e);
        } else {
            f32 arg = ((f32)pp0->attack + (f32)extra * __HBMSYNn128[ch]) / 78642000.0f;
            f32 e = (f32)pow(2.0, (f64)arg);
            ticks = (s32)(1000.0f * e);
        }

        {
            s32 div3 = ticks / 3;
            if (div3 != 0) {
                v->attackTicks = 0;
                v->attackRate = 0x640000 / div3;
                v->vol = 0xFC400000;
                v->volPhase = 0;
            } else {
                v->attackTicks = 0;
                v->attackRate = 0x640000;
                v->vol = (s32)0xFC400000;
                v->volPhase = 0;
            }
        }
    }

    if ((u32)v->volPhase < 2) {
        s32 decay;
        s32 extra2;
        HBMSYNVolParams* pp = (HBMSYNVolParams*)v->params;
        u8 ch = v->note;
        decay = pp->decay;
        extra2 = pp->decayExtra;

        if (decay == 0x80000000) {
            ticks = 0;
        } else if (extra2 == 0x80000000) {
            f32 d = (f32)decay;
            f32 arg = d / 78643200.0f;
            f32 e = (f32)pow(2.0, (f64)arg);
            ticks = (s32)(1000.0f * e);
        } else {
            f32 arg = ((f32)decay + (f32)extra2 * __HBMSYNn128[ch]) / 78642000.0f;
            f32 e = (f32)pow(2.0, (f64)arg);
            ticks = (s32)(1000.0f * e);
        }

        {
            s32 div3 = ticks / 3;
            if (div3 != 0) {
                v->decayRate = (s32)0xFC400000 / div3;
            } else {
                v->decayRate = (s32)0xFC400000;
            }
        }
    }

    v->sustain = ((HBMSYNVolParams*)v->params)->sustain;
    v->release = ((HBMSYNVolParams*)v->params)->release;
}

extern "C" void __HBMSYNRunVolumeEnvelope(void* voice)
{
    HBMSYNVoice* v = (HBMSYNVoice*)voice;
    u32 phase = (u32)v->volPhase;

    switch (phase) {
    case 0:
    {
        s32 cnt = v->attackTicks + v->attackRate;
        v->attackTicks = cnt;
        if (cnt >= 0x630000) {
            v->vol = 0;
        } else {
            v->vol = __HBMSYNAttackAttnTable[cnt >> 16];
        }
        if (v->vol == 0) {
            v->volPhase = 1;
        }
        break;
    }
    case 1:
    {
        s32 vol = v->vol;
        s32 drate = v->decayRate;
        s32 sust = v->sustain;
        s32 nvol = vol + drate;
        v->vol = nvol;
        if (nvol <= sust) {
            v->vol = sust;
            v->volPhase = 2;
        }
        if (v->vol > (s32)0xFD300000) {
            break;
        }
        {
            v->volPhase = 4;
            u8 ch = v->ch;
            u8 note = v->note;
            void* base = v->base;
            s32* p = (s32*)((u8*)base + ((s32)ch << 9));
            p[(s32)note + 0x102] = 0;
        }
        break;
    }
    case 3:
    {
        s32 vol = v->vol;
        if (vol <= (s32)0xFD300000) {
            v->volPhase = 4;
            break;
        }
        v->vol = vol + v->release;
        break;
    }
    }
}
