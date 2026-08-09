#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"

/*
Generic task object.

Provides a way to implement Move/Draw behavior without needing to work with the
CProcess api.

Derived classes must inherit using CRTP to allow binding the move/draw functions.

Move/Draw/~CTTask are DECLARED ONLY (not defined inline) so that the owning
translation unit of each derived class can emit the retail out-of-line
`Move__..CTTask<X>Fv` / `Draw__..CTTask<X>Fv` / `__dt__..CTTask<X>Fv` bodies as
explicit `template<>` specializations.  Defining them inline here would make
MWCC silently drop those specializations (see TextWriterBase precedent in
docs/MWCC_REFERENCE.md) and no standalone retail-matching symbols would exist.
The ctor stays inline (retail emits no out-of-line CTTask ctor).
*/
template <typename TDerived>
class CTTask : public CProcess {
public:
    typedef void (TDerived::*MoveFunc)();
    typedef void (TDerived::*DrawFunc)();

public:
    CTTask() : mMoveFunc(nullptr), mDrawFunc(nullptr) {}

    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();

protected:
    //0x0-10: CDoubleListNode
    //0x10: vtable
    //0x14-3C: CProcess
    MoveFunc mMoveFunc; //0x3C
    DrawFunc mDrawFunc; //0x48
}; // size: 0x54
