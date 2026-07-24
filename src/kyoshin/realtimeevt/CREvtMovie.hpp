#pragma once

/**
 * @file CREvtMovie.hpp
 * CREvtMovie -- realtime event movie (SFD) playback class.
 *
 * Class hierarchy (namespace cf):
 *   cf::CREvtObj  (vtable 0x80532320, size 0x14)
 *     +0x00: vtable pointer
 *     +0x04: u32 field (set to 4 in CREvtMovie's constructor)
 *     +0x08: __ptmf (12 bytes, initialized to null)
 *     +0x14: .. derived class fields start here
 *   +-- cf::CREvtMovie  (vtable 0x80538AA0, size >= 0x1A)
 *
 * Virtual function table at 0x80538AA0 (7 virtual functions):
 *   vfunc 0: destructor  (__ct__802948D0)
 *   vfunc 1: func_80294CB0  -- empty override (just blr)
 *   vfunc 2: func_8029493C
 *   vfunc 3: inherited (func_80185758 -- returns 0)
 *   vfunc 4: inherited (func_80169048)
 *   vfunc 5: inherited (func_801809A8)
 *   vfunc 6: inherited (func_801696C4)
 *
 * The class constructs SFD movie file paths by concatenating
 *   "/ev/realtime/" + scriptName + ".sfd"
 * and passes them to the streaming/playback system.
 */

#include <types.h>

namespace cf {

// Forward declaration; parent class fields are opaque here.
// Full definition TBD during parent TU decomp.
struct CREvtMovie {
    // Inherited from cf::CREvtObj (size 0x14)
    u8 unk00[0x14];

    // Offset 0x14: pointer to script data structure.
    // Stores the second constructor argument.
    // In functions func_8029493C/80/A70/BA4 this is dereferenced
    // as a structure with:
    //   +0x00: header byte magic
    //   +0x08: u32 counter (compared with func_8016A3C4 result)
    //   +0x0C: char[] (script name string)
    u32 mScriptData;

    // Offset 0x18: flag byte.
    // Written during constructor, read in playback functions.
    u8 mFlag18;

    // Offset 0x19: flag byte.
    // Set to 0/1 by playback functions; may indicate loading state.
    u8 mFlag19;
};

} // namespace cf
