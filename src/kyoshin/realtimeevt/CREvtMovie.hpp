#pragma once

/**
 * @file CREvtMovie.hpp
 * CREvtMovie -- realtime event movie (SFD) playback class.
 *
 * Inherits from cf::CREvtObj (vtable 0x80532320, size 0x14)
 * Own vtable at 0x80538AA0
 * Size: 0x1A
 *
 * Virtual function table at 0x80538AA0 (8 slots):
 *   dtor, vfunc_04 (empty), vfunc_08 (stop before trigger), inherited
 *   vfunc_0C/vfunc_10/vfunc_14/isBusy, vfunc_1C (preload).
 *
 * The class constructs SFD movie file paths by concatenating
 *   "/ev/realtime/" + scriptName + ".sfd"
 * and passes them to the streaming/playback system.
 */

#include <types.h>
#include "kyoshin/realtimeevt/CREvtObj.hpp"

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

namespace cf {

// CREvtMovie - realtime event movie playback (RTTI name "cf::CREvtMovie").
// Vtable at 0x80538AA0. Size: 0x1A.
class CREvtMovie : public CREvtObj {
public:
    CREvtMovie(CREvtMovieScript* scriptData);
    virtual ~CREvtMovie();
    virtual void vfunc_04();  // empty override
    virtual void vfunc_08();  // stop playback one tick before the trigger
    virtual void vfunc_1C();  // preload the SFD ahead of its trigger tick

    /* 0x14 */ CREvtMovieScript* mScriptData;
    /* 0x18 */ u8 mFlag18;
    /* 0x19 */ u8 mFlag19;
};

} // namespace cf

using cf::CREvtMovie;

// C-linkage imports and TU targets (retail symbol names - keep
// linkage/signatures verbatim so the definitions emit the unmangled names).
extern "C" {

// Movie playback functions (CLibCri streaming family).
void evtStopMoviePlayback(void);
int evtIsMoviePaused(void);
void evtClearMoviePause(void);
void evtStartMoviePlayback(const char* path, int flag, void* handle);

// Counter/timing functions.
// EvtSeqGetWalkIndex is an unsigned tick counter; 8016A378/8016A35C return
// signed timestamps (compared with a signed `cmpi` in retail).
int EvtSeqGetCounter100(void);

// Memory availability probe (CLibCri member, defined in the CLibCri TU).
void* getMovieWorkSize__7CLibCriFv(void);

// Targets defined in this TU (declared here so the definitions keep C
// linkage and emit the retail unmangled symbol names).
void EvtMovie_StartPlayback_4980(CREvtMovie* self);
void EvtMovie_TogglePlayback_4BA4(CREvtMovie* self);

} // extern "C"

// Defined in CTaskREvent.cpp (C++ linkage).
int evtHasMoviePlayer(void);
