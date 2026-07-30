#pragma once

#include <types.h>

/* System window process. Inherits IUIWindow (0x0), IWorkEvent (0x6C),
   and IScnRender (0x70). Singleton created by func_80124AEC.
   Full class definition pending IUIWindow recovery. */
class CSystemWindow {
public:
    CSystemWindow();
    ~CSystemWindow();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x0: IUIWindow base
    // 0x6C: IWorkEvent base
    // 0x70: IScnRender base
    // 0x78: CSysWin subobject or similar
};
