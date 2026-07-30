// Decompiled RVL_SDK/src/revolution/hbm/synenv
// High-level C reconstruction — no inline asm, no register tricks

#include <harness_catalog.h>
#include <math.h>

extern const f32 __HBMSYNn128[];
extern const s32 __HBMSYNAttackAttnTable[];

void __HBMSYNSetupVolumeEnvelope(void* voice)
{
    void* params = *(void**)((u8*)voice + 0x14);
    s32 attack = *(s32*)((u8*)params + 0x18);
    s32 decay = *(s32*)((u8*)params + 0x1C);

    if (attack != (s32)0x80000000) {
        s32 extra, ticks, div3;
        f32 arg;

        extra = *(s32*)((u8*)params + 0x28);
        if (extra == (s32)0x80000000) {
            arg = (f32)(s32)attack / 78643200.0f;
        } else {
            arg = ((f32)(s32)extra * __HBMSYNn128[*(u8*)((u8*)voice + 0x0E)] + (f32)(s32)attack) / 78642000.0f;
        }

        ticks = (s32)(1000.0f * (f32)pow(2.0, (f64)arg));
        div3 = ticks / 3;
        if (div3 != 0) {
            *(s32*)((u8*)voice + 0x38) = 0;
            *(s32*)((u8*)voice + 0x34) = (s32)0xFC400000;
            *(s32*)((u8*)voice + 0x30) = 0;
            *(s32*)((u8*)voice + 0x3C) = 0x640000 / div3;
        } else {
            *(s32*)((u8*)voice + 0x38) = 0;
            *(s32*)((u8*)voice + 0x34) = (s32)0xFC400000;
            *(s32*)((u8*)voice + 0x30) = 0;
            *(s32*)((u8*)voice + 0x3C) = 0x640000;
        }
    } else {
        *(s32*)((u8*)voice + 0x30) = 1;
        *(s32*)((u8*)voice + 0x34) = 0;
        if (decay == (s32)0x80000000) {
            *(s32*)((u8*)voice + 0x30) = 2;
            *(s32*)((u8*)voice + 0x34) = *(s32*)((u8*)params + 0x20);
        }
    }

    if (*(u32*)((u8*)voice + 0x30) < 2) {
        params = *(void**)((u8*)voice + 0x14);
        s32 decay2 = *(s32*)((u8*)params + 0x1C);
        s32 extra2 = *(s32*)((u8*)params + 0x2C);
        s32 ticks, div3;
        f32 arg;

        if (extra2 == (s32)0x80000000) {
            arg = (f32)(s32)decay2 / 78643200.0f;
        } else {
            arg = ((f32)(s32)extra2 * __HBMSYNn128[*(u8*)((u8*)voice + 0x0D)] + (f32)(s32)decay2) / 78642000.0f;
        }

        ticks = (s32)(1000.0f * (f32)pow(2.0, (f64)arg));
        div3 = ticks / 3;

        if (div3 != 0) {
            *(s32*)((u8*)voice + 0x40) = (s32)0xFC400000 / div3;
        } else {
            *(s32*)((u8*)voice + 0x40) = (s32)0xFC400000;
        }
    }

    params = *(void**)((u8*)voice + 0x14);
    *(s32*)((u8*)voice + 0x44) = *(s32*)((u8*)params + 0x20);
    *(s32*)((u8*)voice + 0x48) = *(s32*)((u8*)params + 0x24);
}

void __HBMSYNRunVolumeEnvelope(void* voice)
{
    u8* v = (u8*)voice;
    u32 phase = *(u32*)(v + 0x30);

    switch (phase) {
    case 0:
    {
        s32 cnt = *(s32*)(v + 0x38) + *(s32*)(v + 0x3C);
        *(s32*)(v + 0x38) = cnt;
        if (cnt >= 0x630000) {
            *(s32*)(v + 0x34) = 0;
        } else {
            *(s32*)(v + 0x34) = __HBMSYNAttackAttnTable[cnt >> 16];
        }
        if (*(s32*)(v + 0x34) == 0) {
            *(s32*)(v + 0x30) = 1;
        }
        break;
    }
    case 1:
    {
        s32 vol = *(s32*)(v + 0x34);
        s32 drate = *(s32*)(v + 0x40);
        s32 sust = *(s32*)(v + 0x44);
        vol += drate;
        *(s32*)(v + 0x34) = vol;
        if (vol > sust) {
            *(s32*)(v + 0x30) = 2;
            *(s32*)(v + 0x34) = sust;
        }
        if (*(s32*)(v + 0x34) > (s32)0xFD300000) {
            break;
        }
        {
            u8 ch = *(u8*)(v + 0x0C);
            u8 note = *(u8*)(v + 0x0D);
            void* base = *(void**)(v + 0x08);
            void* addr = (u8*)base + ((s32)ch << 9) + ((s32)note << 2);
            *(s32*)(v + 0x30) = 4;
            *(s32*)((u8*)addr + 0x408) = 0;
        }
        break;
    }
    case 3:
    {
        s32 vol = *(s32*)(v + 0x34);
        if (vol > (s32)0xFD300000) {
            *(s32*)(v + 0x34) = vol + *(s32*)(v + 0x48);
            break;
        }
        *(s32*)(v + 0x30) = 4;
        break;
    }
    }
}
