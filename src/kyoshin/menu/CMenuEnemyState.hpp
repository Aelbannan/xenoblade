#pragma once

#include <types.h>
#include <nw4r/lyt.h>

// Minimal layout for batch-14f matching (cbRenderBefore only).
// Panel slots: this + idx * 0x4c, fields at +0xa8..+0xcd.
class CMenuEnemyState {
public:
    void cbRenderBefore();

    u8 unk00[0x74];
    nw4r::lyt::Layout* unk74; // 0x74 - shared layout draw target
    u8 unk78[0x800 - 0x78];
    nw4r::lyt::Layout* unk800; // 0x800
    u8 unk804[0x828 - 0x804];
    s32 unk828; // 0x828 - draw unk800 when nonzero
};
