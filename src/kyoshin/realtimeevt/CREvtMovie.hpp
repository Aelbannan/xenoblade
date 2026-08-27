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
#include "kyoshin/realtimeevt/CREvtObj.hpp"

// Own vtable (retail .data:0x80538AA0), referenced by the ctor/dtor.
extern void* lbl_eu_80538AA0[];

// String constant: "/ev/realtime/\0.sfd\0" packed together.
// [0x00 .. 0x0D] = "/ev/realtime/", [0x0E .. ] = ".sfd"
// Declared as an incomplete (unsized) array: MWCC cannot classify an
// unsized extern as SDA-eligible, so it forms the address with `lis`/`addi`
// (ADDR16_HA/LO) and holds the base in a callee-saved register (retail
// shape). Referencing via array decay keeps `dir + 14` as runtime pointer
// arithmetic from that held base (retail's `addi rN,rM,14`).
extern char lbl_eu_8050FD98[];

// Script data structure referenced by CREvtMovie::mScriptData.
// Layout: [0x08] = event counter, [0x0C] = SFD script name (no extension).
struct CREvtMovieScript {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 mCounter;               // trigger/iteration counter
    /* 0x0C */ char mScriptName[1];        // script name (flexible array)
};

// Path-building buffer used by CREvtMovie playback functions.
// Holds the concatenated SFD path in mPath and tracks its length in mLength;
// the struct is passed to opaque playback routines, so mLength stores are kept.
struct CREvtMoviePathBuf {
    /* 0x00 */ char mPath[0x20];
    /* 0x20 */ u32 mLength;
};

// CREvtMovie - realtime event movie playback
// Vtable at 0x80538AA0 (installed by hand by the ctor/dtor)
// Size: 0x1A
struct CREvtMovie : cf::CREvtObj {
    /* 0x14 */ CREvtMovieScript* mScriptData;  // Pointer to script data structure
    /* 0x18 */ u8 mFlag18;                     // Flag byte (initialized to 0)
    /* 0x19 */ u8 mFlag19;                     // Flag byte (loading/playback state)
};

// C-linkage imports and TU targets (retail symbol names - keep
// linkage/signatures verbatim so the definitions emit the unmangled names).
extern "C" {

// Base constructor/destructor -- defined in kyoshin/realtimeevt/CREvtObj.cpp.
cf::CREvtObj* __ct__cf_CREvtObj(cf::CREvtObj* self, int arg);
cf::CREvtObj* __dt__Q22cf8CREvtObjFv(cf::CREvtObj* self, int deleteFlag);
void __dt__80185754(void* ptr);

// Movie playback functions (CLibCri streaming family).
void func_80164F6C(void);
int func_80164FE8(void);
int func_80164FB4(void);
void func_80165014(void);
void func_80164ED0(const char* path, int flag, void* handle);
void* func_8016C3DC(void);

// Counter/timing functions.
// func_8016A3C4 is an unsigned tick counter; 8016A378/8016A35C return
// signed timestamps (compared with a signed `cmpi` in retail).
u32 func_8016A3C4(void);
int func_8016A378(void);
int func_8016A35C(void);

// Memory availability probe (CLibCri member, defined in the CLibCri TU).
void* getMovieWorkSize__7CLibCriFv(void);

// Targets defined in this TU (declared here so the definitions keep C
// linkage and emit the retail unmangled symbol names).
CREvtMovie* __ct__CREvtMovie(CREvtMovie* self, CREvtMovieScript* scriptData);
CREvtMovie* __ct__802948D0(CREvtMovie* self, int dealloc_flag);
void func_8029493C(CREvtMovie* self);
void func_80294980(CREvtMovie* self);
void func_80294A70(CREvtMovie* self);
void func_80294BA4(CREvtMovie* self);
void func_80294CB0(void);

} // extern "C"
