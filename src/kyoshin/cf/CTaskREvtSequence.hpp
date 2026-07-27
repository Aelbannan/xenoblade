#pragma once

#include <types.h>

namespace cf {

class CTaskREvtSequence {
public:
    virtual ~CTaskREvtSequence();
    void Init();
    void Term();
    void cbRenderBefore();

    // TODO: add fields
    void Draw();
    void OnFileEvent();
};
} // namespace cf

