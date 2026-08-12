#include "monolib/work/IWorkEvent.hpp"

// IWorkEvent::~IWorkEvent is DECLARATION-ONLY in the header (it is the key
// function, so the IWorkEvent vtable is emitted in the TU that defines it and
// no weak copies leak into other TUs). The STRONG empty copy lives in
// kyoshin/CTaskGame.cpp (retail split places __dt__10IWorkEventFv at
// 0x80040858 in CTaskGame.o); being in the same TU as CTaskGame's dtor lets
// MWCC empty-function call elimination elide the base-dtor call there (the
// retail derived-dtor shape). Default virtual bodies (WorkEvent1..31,
// OnFileEvent, OnPauseTrigger) live in kyoshin/CGame.cpp for retail weak
// placement. This TU is NOT part of the build.
