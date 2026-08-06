#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

class CSysWinScenarioLog : public CProcess {
public:
    CSysWinScenarioLog();
    virtual ~CSysWinScenarioLog();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void OnFileEvent();

    /* 0x3c-0x7c: region owned by layout / IWorkEvent sub-object */
    u8 _3C[0x7C - 0x3C];
    /* 0x7c: UnkClass_8045F564 embedded region (dtor calls ~UnkClass_8045F564) */
    UnkClass_8045F564 mMemRegion;
    /* 0x8c-0xa4 */
    u8 _8C[0xA4 - 0x8C];
};