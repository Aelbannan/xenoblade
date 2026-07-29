#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"

// Sub-entry for each of the 6 vision slots (array at CMenuVision+0x74, stride 0x30)
struct CMenuVisionEntry {
    nw4r::lyt::Layout* mLayout;                  // 0x00
    nw4r::lyt::AnimTransform* mAnim1;            // 0x04
    nw4r::lyt::AnimTransform* mAnim2;            // 0x08
    nw4r::lyt::AnimTransform* mAnim3;            // 0x0C
    nw4r::lyt::AnimTransform* mAnim4;            // 0x10
    nw4r::lyt::AnimTransform* mAnim5;            // 0x14
    nw4r::lyt::AnimTransform* mAnim6;            // 0x18
    nw4r::lyt::AnimTransform* mAnim7;            // 0x1C
    nw4r::lyt::AnimTransform* mAnim8;            // 0x20
    int mState;                                   // 0x24
    u8 field_0x28;                                // 0x28
    u8 _pad29[3];                                 // 0x29-0x2B
    f32 mTimer;                                   // 0x2C
};

// Pad struct to access pane visibility byte at offset 0xBB
struct PaneVisAccess {
    u8 _pad[0xBB];
    u8 visByte;
};

class CMenuVision {
public:
    CMenuVision();
    virtual ~CMenuVision();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x00: vtable + base classes (0x60 bytes; mirrors CMenuPTGauge layout)
    u8 unk00[0x60];
    CScn* mScn;                                 // 0x60 — owning scene; addRenderCB target
    UnkClass_8045F564 mLayoutMem;               // 0x64 — layout memory region (MEM2 alloc)
    CMenuVisionEntry mEntries[6];               // 0x74 — 6 vision slots (stride 0x30)
};