#pragma once

#include <types.h>

namespace cf {

class CTaskREvtSequence {
    u8 gap00[0x20];
    u32 field_0x20;  // at 0x20, accessed by func_8016A354
    u8 gap24[0x34];
public:
    virtual ~CTaskREvtSequence();
    void Init();
    void Term();
    void cbRenderBefore();

    void Draw();
    void OnFileEvent();
    u32 func_8016A354();
};
} // namespace cf

