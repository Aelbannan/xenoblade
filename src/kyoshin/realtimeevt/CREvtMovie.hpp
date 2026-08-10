#pragma once

/**
 * @file CREvtMovie.hpp
 * CREvtMovie -- realtime event movie (SFD) playback class.
 *
 * Inherits from cf::CREvtObj (vtable 0x80532320, size 0x14)
 * Own vtable at 0x80538AA0
 * Size: 0x1A
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

// Script data structure referenced by CREvtMovie::mScriptData.
// Layout: [0x08] = event counter, [0x0C] = SFD script name (no extension).
struct CREvtMovieScript {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 mCounter;               // trigger/iteration counter
    /* 0x0C */ char mScriptName[1];        // script name (flexible array)
};

// CREvtMovie - realtime event movie playback
// Vtable at 0x80538AA0
// Size: 0x1A
struct CREvtMovie {
    // Inherited from cf::CREvtObj (size 0x14)
    /* 0x00 */ void* vtable;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 ptmf[3];

    // CREvtMovie fields
    /* 0x14 */ CREvtMovieScript* mScriptData;  // Pointer to script data structure
    /* 0x18 */ u8 mFlag18;                     // Flag byte (initialized to 0)
    /* 0x19 */ u8 mFlag19;                     // Flag byte (loading/playback state)
};

// String constant: "/ev/realtime/\0.sfd\0" packed together.
// [0x00 .. 0x0D] = "/ev/realtime/", [0x0E .. ] = ".sfd"
// Declared as a single object (not an array) so that `&lbl + 14` is emitted
// as runtime pointer arithmetic from a held base pointer rather than being
// folded into a constant array index (matches retail's `addi rN,rM,14`).
extern const char lbl_eu_8050FD98;

// Path-building buffer used by CREvtMovie playback functions.
// Holds the concatenated SFD path in mPath and tracks its length in mLength;
// the struct is passed to opaque playback routines, so mLength stores are kept.
struct CREvtMoviePathBuf {
    /* 0x00 */ char mPath[0x20];
    /* 0x20 */ u32 mLength;
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void* lbl_eu_80538AA0[];