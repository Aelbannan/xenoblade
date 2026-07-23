#pragma once

#include <types.h>
#include "monolib/monolib_types.hpp"

/* Interface for work events, which provides a set of 32 event handler functions that
get triggered when a certain event happens (such as when loading a file for OnFileEvent).
Deriving classes can override any of these functions to run their own code when the
corresponding event happens.

Of the 32 available event slots, however, only events 1-5 are ever overriden, with the rest
being empty slots that were left in for some dumb reason (tysm monolithsoft <3). Additionally,
out of the 5 overriden events, only OnFileEvent and OnPauseTrigger seem to be used,
with no apparent calls to the other 3 (possibly debug only).

In XC3D, all instances of the unused event functions (including events 1, 3, and 4) are absent,
with the entries for each instead just being 0 in the vtable. This points to the extra 3 overridden
events being unused as well.

Default bodies are out-of-line (IWorkEvent.cpp) so TUs that override a subset of these
do not emit a full set of weak stubs into their .text (retail keeps those in CGame / CDevice_vt). */
class IWorkEvent {
public:
    virtual ~IWorkEvent();
    virtual bool WorkEvent1(UNKTYPE* r4, const char* r5);
    virtual bool OnFileEvent(CEventFile* pEventFile);
    virtual bool WorkEvent3(UNKTYPE* r4);
    virtual bool WorkEvent4();
    virtual void OnPauseTrigger(bool paused);
    // Completely unused, but still left in...
    virtual bool WorkEvent6();
    virtual bool WorkEvent7();
    virtual bool WorkEvent8();
    virtual bool WorkEvent9();
    virtual bool WorkEvent10();
    virtual bool WorkEvent11();
    virtual bool WorkEvent12();
    virtual bool WorkEvent13();
    virtual bool WorkEvent14();
    virtual bool WorkEvent15();
    virtual bool WorkEvent16();
    virtual bool WorkEvent17();
    virtual bool WorkEvent18();
    virtual bool WorkEvent19();
    virtual bool WorkEvent20();
    virtual bool WorkEvent21();
    virtual bool WorkEvent22();
    virtual bool WorkEvent23();
    virtual bool WorkEvent24();
    virtual bool WorkEvent25();
    virtual bool WorkEvent26();
    virtual bool WorkEvent27();
    virtual bool WorkEvent28();
    virtual bool WorkEvent29();
    virtual bool WorkEvent30();
    virtual void WorkEvent31();
};
