#pragma once

#include "types.h"
#include "revolution/arc/arc.h"
#include "revolution/ax/AXOut.h"
#include "revolution/os/OSMessage.h"
#include "revolution/os/OSThread.h"

// Sequence player state used by the HBM sound sequencer.
// Layout must match the retail workspace.
struct SequencePlayer {
    u8 field_0x000[0x418];
    u32 field_0x418;
    u8 field_0x41c[0x2a00]; // pads to 0x2e1c

    u8 active;              // 0x2e1c
    u8 field_0x2e1d[3];

    SequencePlayer* next; // 0x2e20
    SequencePlayer* prev; // 0x2e24
    s32 seqId;            // 0x2e28
};

namespace homebutton {

void SetSoundMode(u32 mode);
void ShutdownAxSound();
void SetVolumeAllSeq(f32 volume);
void PlaySeq(int id);
void StopAllSeq();
void InitAxSound(const u8* arcData, u8* mem, u32 memSize);
void AxSoundMain();

} // namespace homebutton
