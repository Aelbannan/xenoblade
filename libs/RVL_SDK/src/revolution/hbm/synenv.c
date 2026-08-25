// Decompiled RVL_SDK/src/revolution/hbm/synenv - volume envelope setup/run
#include <harness_catalog.h>
#include <math.h>

extern const f32 __HBMSYNn128[];
extern const s32 __HBMSYNAttackAttnTable[];

// .rodata constant pool referenced by the envelope tick computations.
typedef struct HBMSYNEnvConsts {
    f64 powBase;   // 0x00 - pow() base
    f32 divA;      // 0x08
    f32 mulMs;     // 0x0c - seconds -> milliseconds
    f32 divB;      // 0x10
    f32 pad14;
    f64 convBias;  // 0x18 - s32->f64 conversion bias
} HBMSYNEnvConsts;
extern HBMSYNEnvConsts const lbl_80518B58;

// s32 -> f64 the way MWCC lowers it: 0x4330-prefixed double of the sign-flipped
// word minus the rodata bias constant.
typedef union HBMSYNCvt {
    f64 d;
    u32 w[2];
} HBMSYNCvt;

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

// Convert one envelope parameter (a biased exponent-style value, 0x80000000 =
// "unset") into a tick count: ms = mulMs * pow(powBase, linear combination of
// the decoded values divided by the divisor). Values are decoded through a
// 0x43300000-prefixed double and rebased by the rodata bias constant.
void __HBMSYNSetupVolumeEnvelope(void* voice)
{
    HBMSYNVoice* v = (HBMSYNVoice*)voice;
    HBMSYNEnvConsts const* tbl = &lbl_80518B58;
    HBMSYNCvt cvt1, cvt2;
    s32 ticks;

    cvt1.w[0] = 0x43300000;
    cvt2.w[0] = 0x43300000;

    if (((HBMSYNVolParams*)v->params)->attack == 0x80000000) {
        v->volPhase = 1;
        v->vol = 0;
        if (((HBMSYNVolParams*)v->params)->decay == 0x80000000) {
            v->volPhase = 2;
            v->vol = ((HBMSYNVolParams*)v->params)->sustain;
        }
    } else {
        u8 ch = v->idx;
        s32 attack = ((HBMSYNVolParams*)v->params)->attack;
        s32 extra = ((HBMSYNVolParams*)v->params)->attackExtra;

        // Dead gate: this re-tests the value the outer gate compared, so MWCC
        // folds it onto the stale CR0 — the ticks = 0 path costs no compare.
        if (attack == 0x80000000) {
            ticks = 0;
        } else if (extra == 0x80000000) {
            cvt1.w[1] = (u32)attack ^ 0x80000000;
            f32 exp = (f32)pow(tbl->powBase,
                (f64)((f32)(cvt1.d - tbl->convBias) / tbl->divA));
            ticks = tbl->mulMs * exp;
        } else {
            cvt1.w[1] = (u32)extra ^ 0x80000000;
            cvt2.w[1] = (u32)attack ^ 0x80000000;
            f32 exp = (f32)pow(tbl->powBase,
                (f64)((f32)(cvt1.d - tbl->convBias) * __HBMSYNn128[ch] +
                      (f32)(cvt2.d - tbl->convBias)) / tbl->divB);
            ticks = tbl->mulMs * exp;
        }

        {
            s32 div3 = ticks / 3;
            if (div3 != 0) {
                v->attackRate = 0x640000 / div3;
                v->attackTicks = 0;
                v->vol = 0xFC400000;
                v->volPhase = 0;
            } else {
                v->attackRate = 0x640000;
                v->attackTicks = 0;
                v->vol = (s32)0xFC400000;
                v->volPhase = 0;
            }
        }
    }

    if ((u32)v->volPhase < 2) {
        u8 ch = v->note;
        s32 decay = ((HBMSYNVolParams*)v->params)->decay;
        s32 extra2 = ((HBMSYNVolParams*)v->params)->decayExtra;

        if (decay == 0x80000000) {
            ticks = 0;
        } else if (extra2 == 0x80000000) {
            cvt2.w[1] = (u32)decay ^ 0x80000000;
            f32 exp = (f32)pow(tbl->powBase,
                (f64)((f32)(cvt2.d - tbl->convBias) / tbl->divA));
            ticks = tbl->mulMs * exp;
        } else {
            cvt1.w[1] = (u32)decay ^ 0x80000000;
            cvt2.w[1] = (u32)extra2 ^ 0x80000000;
            f32 exp = (f32)pow(tbl->powBase,
                (f64)((f32)(cvt2.d - tbl->convBias) * __HBMSYNn128[ch] +
                      (f32)(cvt1.d - tbl->convBias)) / tbl->divB);
            ticks = tbl->mulMs * exp;
        }

        {
            s32 div3 = ticks / 3;
            v->decayRate = (div3 != 0) ? (s32)0xFC400000 / div3 : (s32)0xFC400000;
        }
    }

    v->sustain = ((HBMSYNVolParams*)v->params)->sustain;
    v->release = ((HBMSYNVolParams*)v->params)->release;
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
