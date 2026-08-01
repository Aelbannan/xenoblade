// High-level C reconstruction of RVL_SDK/src/revolution/mix
//
// MIX mixer (predecessor of the HBM fork in revolution/hbm/mix.c).
// Maintains 96 internal mixer channels (0x70 bytes each) backed by a
// static array; __MIXChannel / __MIXRmtChannel are pointers into it.

#include <types.h>
#include <revolution/ax/AXVPB.h>

typedef struct MIXChannel {
    AXVPB* vpb;      // at 0x00 — attached AX voice (NULL = free slot)
    u32 flags;       // at 0x04 — pending update flags
    s32 input;       // at 0x08 — main input volume (dB x10)
    s32 auxA;        // at 0x0C — AuxA send volume (dB x10)
    s32 auxB;        // at 0x10 — AuxB send volume (dB x10)
    s32 pan;         // at 0x14 — pan [0, 127]
    s32 panL;        // at 0x18 — left pan attenuation (dB x10)
    s32 panR;        // at 0x1C — right pan attenuation (dB x10)
    s32 fader;       // at 0x20 — fader volume (dB x10)
    u32 _rest[0x70 / 4 - 9]; // at 0x24..0x6C — volume targets
} MIXChannel; // size 0x70

// Channel update flags (MIXChannel.flags)
#define MIX_FLAG_INPUT 0x10000000u // input volume changed
#define MIX_FLAG_MIX   0x40000000u // auxA/pan/fader changed

// Mixer state (sbss globals)
extern MIXChannel* __MIXChannel;   // active channel table
extern MIXChannel* __MIXRmtChannel; // remote channel table
extern s32 __init;                 // mixer initialized flag

void __MIXSetPan() {}

void __MIXGetVolume() {}

void MIXInit() {}

void MIXQuit(void) {
    __MIXChannel = NULL;
    __MIXRmtChannel = NULL;
    __init = 0;
}

void MIXInitChannel() {}

void MIXReleaseChannel(AXVPB* vpb) {
    __MIXChannel[vpb->index].vpb = NULL;
}

void MIXSetInput(AXVPB* vpb, s32 input) {
    MIXChannel* ch = &__MIXChannel[vpb->index];

    ch->input = input;
    ch->flags |= MIX_FLAG_INPUT;
}

void MIXSetPan() {}

void MIXSetFader(AXVPB* vpb, s32 fader) {
    MIXChannel* ch = &__MIXChannel[vpb->index];

    ch->fader = fader;
    ch->flags |= MIX_FLAG_MIX;
}

void MIXUpdateSettings() {}
