// Decompiled RVL_SDK/src/revolution/hbm/synenv - volume envelope setup/run
#include <harness_catalog.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif
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

void __HBMSYNSetupVolumeEnvelope(void* voice)
{
    HBMSYNVoice* v = (HBMSYNVoice*)voice;
    HBMSYNVolParams* params = (HBMSYNVolParams*)v->params;
    s32 attack = params->attack;

    if (attack == 0x80000000) {
        v->volPhase = 1;
        v->vol = 0;
        s32 decay = params->decay;
        if (decay == 0x80000000) {
            v->volPhase = 2;
            v->vol = params->sustain;
        }
    } else {
        u8 ch = v->idx;
        s32 extra = params->attackExtra;
        s32 ticks;

        if (attack == 0x80000000) {
            ticks = 0;
        } else {
            if (extra == 0x80000000) {
                f32 f = (f32)pow(2.0, (f64)((f32)attack / 78643200.0f));
                ticks = (s32)(f * 1000.0f);
            } else {
                f32 f = (f32)pow(2.0, (f64)(((f32)extra * __HBMSYNn128[ch] + (f32)attack) / 78642000.0f));
                ticks = (s32)(f * 1000.0f);
            }
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
        HBMSYNVolParams* p2 = (HBMSYNVolParams*)v->params;
        u8 ch = v->note;
        s32 decay = p2->decay;
        s32 extra2 = p2->decayExtra;
        s32 ticks;

        if (decay == 0x80000000) {
            ticks = 0;
        } else {
            if (extra2 == 0x80000000) {
                f32 f = (f32)pow(2.0, (f64)((f32)decay / 78643200.0f));
                ticks = (s32)(f * 1000.0f);
            } else {
                f32 f = (f32)pow(2.0, (f64)(((f32)extra2 * __HBMSYNn128[ch] + (f32)decay) / 78642000.0f));
                ticks = (s32)(f * 1000.0f);
            }
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

    params = (HBMSYNVolParams*)v->params;
    v->sustain = params->sustain;
    v->release = params->release;
}

void __HBMSYNRunVolumeEnvelope(void* voice)
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
#ifdef __cplusplus
}
#endif
