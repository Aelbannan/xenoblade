// Decompilation of RVL_SDK/src/revolution/mix/remote
// Remote channel volume management for Wii Remote audio

#include <types.h>
#include <revolution/ax/AXVPB.h>

// ----- Forward declarations -----

// __MIXGetVolume converts a raw s32 volume parameter to a u16 volume level
// using the __MIXVolumeTable lookup table.
extern u16 __MIXGetVolume(s32 value);

// ----- Global state -----

// __MIXRmtChannel is a global pointer (sbss) to the remote channel array.
// Initialized during MIXInit to point to __s_MIXRmtChannel in BSS.
// Defined here to reproduce the retail .bss (0x1988) / .sbss allocations.
// retail .sbss symbol is 8 bytes; u64 keeps the size (and keeps the pointer at 0x0)
u64 __MIXRmtChannel;
u8 __s_MIXRmtChannel[0x1988];

// ----- Remote channel structure -----

// Each channel entry is 0x44 bytes with interleaved current/dest pairs.
typedef struct MIXRmtChannel {
    u32 flags;       // 0x00: status/control flags
    s32 vol[8];      // 0x04-0x23: 8 raw volume parameters (signed)
    u16 cur0, dst0;  // 0x24-0x27: pair 0 (main volume)
    u16 cur1, dst1;  // 0x28-0x2B: pair 1 (panning)
    u16 cur2, dst2;  // 0x2C-0x2F: pair 2 (aux A)
    u16 cur3, dst3;  // 0x30-0x33: pair 3 (aux B)
    u16 cur4, dst4;  // 0x34-0x37: pair 4 (aux C)
    u16 cur5, dst5;  // 0x38-0x3B: pair 5 (aux D)
    u16 cur6, dst6;  // 0x3C-0x3F: pair 6 (aux E)
    u16 cur7, dst7;  // 0x40-0x43: pair 7 (aux F)
} MIXRmtChannel;     // total 0x44

// Remote channel info block: index field at 0x18 (caller-provided).
typedef struct MIXRmtChanInfo {
    u8  _pad[0x18];
    s32 index;       // 0x18 - remote channel index
} MIXRmtChanInfo;

// Flag bit definitions
#define MIX_RMT_FLAG_COPY_DEST  0x80000000u  // copy dst[] -> cur[] on next update
#define MIX_RMT_FLAG_UPDATE     0x40000000u  // recalculate dst[] from vol[] params
#define MIX_RMT_FLAG_AUX0       0x00000001u  // bit 31: aux vol[4] instead of vol[0]+vol[4]
#define MIX_RMT_FLAG_AUX1       0x00000002u  // bit 30: aux vol[5] instead of vol[1]+vol[5]
#define MIX_RMT_FLAG_AUX2       0x00000004u  // bit 29: aux vol[6] instead of vol[2]+vol[6]
#define MIX_RMT_FLAG_AUX3       0x00000008u  // bit 28: aux vol[7] instead of vol[3]+vol[7]

// ---- Functions ----

void MIXRmtSetVolumes(void* chanInfo, u32 mask, s32 v0, s32 v1, s32 v2, s32 v3,
                      s32 v4, s32 v5, s32 v6, s32 v7)
{
    s32 idx = ((MIXRmtChanInfo*)chanInfo)->index;
    u32 bit = mask & 0xF;
    MIXRmtChannel* ch = (MIXRmtChannel*)__MIXRmtChannel + idx;
    u32 tmp;

    tmp = ch->flags | bit;
    ch->vol[0] = v0;
    ch->vol[1] = v1;
    ch->vol[2] = v2;
    ch->vol[3] = v3;
    ch->vol[4] = v4;
    ch->vol[5] = v5;
    ch->vol[6] = v6;
    ch->vol[7] = v7;
    ch->flags = tmp | MIX_RMT_FLAG_UPDATE;
}

void MIXRmtSetFader(void* chanInfo, s32 selector, s32 value)
{
    s32 idx = ((MIXRmtChanInfo*)chanInfo)->index;
    MIXRmtChannel* ch = (MIXRmtChannel*)__MIXRmtChannel + idx;

    switch (selector) {
    case 0:
        ch->vol[0] = value;
        break;
    case 1:
        ch->vol[1] = value;
        break;
    case 2:
        ch->vol[2] = value;
        break;
    case 3:
        ch->vol[3] = value;
        break;
    }
    ch->flags |= MIX_RMT_FLAG_UPDATE;
}

void __MIXRmtUpdateSettings(s32 idx, AXVPB* out)
{
    MIXRmtChannel* ch = (MIXRmtChannel*)__MIXRmtChannel + idx;
    u32 flags = ch->flags;

    if (!(flags & (MIX_RMT_FLAG_COPY_DEST | MIX_RMT_FLAG_UPDATE)))
        return;

    if (flags & MIX_RMT_FLAG_COPY_DEST) {
        ch->cur0 = ch->dst0;
        ch->cur1 = ch->dst1;
        ch->cur2 = ch->dst2;
        ch->cur3 = ch->dst3;
        ch->cur4 = ch->dst4;
        ch->cur5 = ch->dst5;
        ch->cur6 = ch->dst6;
        ch->cur7 = ch->dst7;
        ch->flags &= ~MIX_RMT_FLAG_COPY_DEST;
    }

    if (ch->flags & MIX_RMT_FLAG_UPDATE) {
        ch->dst0 = __MIXGetVolume(ch->vol[0]);
        ch->dst2 = __MIXGetVolume(ch->vol[1]);
        ch->dst4 = __MIXGetVolume(ch->vol[2]);
        ch->dst6 = __MIXGetVolume(ch->vol[3]);

        if (ch->flags & MIX_RMT_FLAG_AUX0)
            ch->dst1 = __MIXGetVolume(ch->vol[4]);
        else
            ch->dst1 = __MIXGetVolume(ch->vol[0] + ch->vol[4]);

        if (ch->flags & MIX_RMT_FLAG_AUX1)
            ch->dst3 = __MIXGetVolume(ch->vol[5]);
        else
            ch->dst3 = __MIXGetVolume(ch->vol[1] + ch->vol[5]);

        if (ch->flags & MIX_RMT_FLAG_AUX2)
            ch->dst5 = __MIXGetVolume(ch->vol[6]);
        else
            ch->dst5 = __MIXGetVolume(ch->vol[2] + ch->vol[6]);

        if (ch->flags & MIX_RMT_FLAG_AUX3)
            ch->dst7 = __MIXGetVolume(ch->vol[7]);
        else
            ch->dst7 = __MIXGetVolume(ch->vol[3] + ch->vol[7]);

        ch->flags = (ch->flags & ~MIX_RMT_FLAG_UPDATE) | MIX_RMT_FLAG_COPY_DEST;
    }

    // Phase 3: output interpolation data for DSP
    {
        u16 bm;
        u16 cur;
        s32 step;
        u16* q = (u16*)&out->pb.rmtMix;

        cur = ch->cur0;
        *q++ = cur;
        bm = 0;
        if (cur != 0) bm |= 1;
        step = (ch->dst0 - ch->cur0) / 18;
        *q++ = (u16)step;
        if ((u16)step != 0) bm |= 2;

        cur = ch->cur1;
        *q++ = cur;
        if (cur != 0) bm |= 4;
        step = (ch->dst1 - ch->cur1) / 18;
        *q++ = (u16)step;
        if ((u16)step != 0) bm |= 8;

        cur = ch->cur2;
        *q++ = cur;
        if (cur != 0) bm |= 0x10;
        step = (ch->dst2 - ch->cur2) / 18;
        *q++ = (u16)step;
        if ((u16)step != 0) bm |= 0x20;

        cur = ch->cur3;
        *q++ = cur;
        if (cur != 0) bm |= 0x40;
        step = (ch->dst3 - ch->cur3) / 18;
        *q++ = (u16)step;
        if ((u16)step != 0) bm |= 0x80;

        cur = ch->cur4;
        *q++ = cur;
        if (cur != 0) bm |= 0x100;
        step = (ch->dst4 - ch->cur4) / 18;
        *q++ = (u16)step;
        if ((u16)step != 0) bm |= 0x200;

        cur = ch->cur5;
        *q++ = cur;
        if (cur != 0) bm |= 0x400;
        step = (ch->dst5 - ch->cur5) / 18;
        *q++ = (u16)step;
        if ((u16)step != 0) bm |= 0x800;

        cur = ch->cur6;
        *q++ = cur;
        if (cur != 0) bm |= 0x1000;
        step = (ch->dst6 - ch->cur6) / 18;
        *q++ = (u16)step;
        if ((u16)step != 0) bm |= 0x2000;

        cur = ch->cur7;
        *q++ = cur;
        if (cur != 0) bm |= 0x4000;
        step = (ch->dst7 - ch->cur7) / 18;
        *q++ = (u16)step;
        if ((u16)step != 0) bm |= 0x8000;

        out->pb.rmtMixerCtrl = bm;
    }

    out->sync |= 0x03000000;
}

void __MIXRmtResetChannel(s32 idx)
{
    MIXRmtChannel* ch = (MIXRmtChannel*)__MIXRmtChannel + idx;

    ch->flags = 0;
    ch->vol[0] = 0;
    ch->vol[1] = 0;
    ch->vol[2] = 0;
    ch->vol[3] = 0;
    ch->vol[4] = -0x3C0;
    ch->vol[5] = -0x3C0;
    ch->vol[6] = -0x3C0;
    ch->vol[7] = -0x3C0;

    ch->cur7 = 0;
    ch->cur6 = 0;
    ch->cur5 = 0;
    ch->cur4 = 0;
    ch->cur3 = 0;
    ch->cur2 = 0;
    ch->cur1 = 0;
    ch->cur0 = 0;
}
