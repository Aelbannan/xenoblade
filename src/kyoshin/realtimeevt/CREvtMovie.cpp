// TU: kyoshin/realtimeevt/CREvtMovie
// Class: CREvtMovie (inherits from cf::CREvtObj)
// Own vtable at 0x80538AA0
// Size: 0x1A

#include <types.h>
#include <cstring>
#include "kyoshin/realtimeevt/CREvtMovie.hpp"
#include "kyoshin/cf/EvtSeqApi.hpp"
#include "monolib/util/MemManager.hpp"

namespace cf {

// ============================================================================
// Constructor: base CREvtObj(4) (movie kind), then the movie fields.
// ============================================================================
CREvtMovie::CREvtMovie(CREvtMovieScript* scriptData) : CREvtObj(4) {
    mScriptData = scriptData;
    mFlag18 = 0;
    mFlag19 = 0;
}

// ============================================================================
// Destructor (vtable slot 0): stop any running playback; the CREvtObj base
// dtor and the class operator delete (CREvtMem arena) follow implicitly.
// ============================================================================
CREvtMovie::~CREvtMovie() {
    evtStopMoviePlayback();
}

// ============================================================================
// vfunc_08: if scriptData->counter == currentTick + 1, stop playback.
// ============================================================================
void CREvtMovie::vfunc_08() {
    if (mScriptData->mCounter == EvtSeqGetWalkIndex() + 1) {
        evtStopMoviePlayback();
    }
}

} // namespace cf

// ============================================================================
// EvtMovie_StartPlayback_4980: Build SFD path and start playback
// If scriptData->counter == currentTick and not already playing,
// builds "/ev/realtime/" + scriptName + ".sfd" and starts playback
// ============================================================================
void EvtMovie_StartPlayback_4980(CREvtMovie* self) {
    if (self->mScriptData->mCounter != EvtSeqGetWalkIndex()) return;

    // Check if already playing or finished
    if (evtHasMoviePlayer() != 0) return;

    // Build file path on stack: "/ev/realtime/" + scriptName + ".sfd"
    // Suffix local assigned up front keeps the base pointer in a
    // callee-saved register across the strlen/strcpy/strcat calls.
    const char* name;
    const char* dir = "/ev/realtime/";
    CREvtMoviePathBuf buf;
    buf.mLength = std::strlen(dir);
    std::strcpy(buf.mPath, dir);

    name = self->mScriptData->mScriptName;
    u32 sLen = std::strlen(name);
    std::strcat(buf.mPath, name);
    buf.mLength += sLen;

    name = ".sfd";
    u32 eLen = std::strlen(name);
    std::strcat(buf.mPath, name);
    buf.mLength += eLen;

    evtStartMoviePlayback(buf.mPath, 1, (void*)EvtSeqGetC4FlagBit1());
    self->mFlag19 = 0;
}

// ============================================================================
// CREvtMovie::vfunc_1C: Build SFD path and start playback (with timing checks)
// Checks counter (currentTick + 1), timing, and memory availability
// ============================================================================
void cf::CREvtMovie::vfunc_1C() {
    CREvtMovie* self = this;
    if (self->mScriptData->mCounter != EvtSeqGetWalkIndex() + 1) return;

    // Check if already playing or finished
    if (evtHasMoviePlayer() != 0) return;

    // Check timing: elapsed time since last event
    if (EvtSeqGetEntryLimit() - EvtSeqGetCounter100() >= 25) return;  // too early

    // Check memory availability
    u32 criSize = (u32)getMovieWorkSize__7CLibCriFv();
    if (mtl::MemManager::getMaxAllocSize(mtl::MemManager::getHandleMEM2()) + 0x200 <= criSize)
        return;

    // Double-check not playing
    if (evtHasMoviePlayer() != 0) return;

    // Build file path on stack: "/ev/realtime/" + scriptName + ".sfd"
    const char* name;
    const char* dir = "/ev/realtime/";
    CREvtMoviePathBuf buf;
    buf.mLength = std::strlen(dir);
    std::strcpy(buf.mPath, dir);

    name = self->mScriptData->mScriptName;
    u32 sLen = std::strlen(name);
    std::strcat(buf.mPath, name);
    buf.mLength += sLen;

    name = ".sfd";
    u32 eLen = std::strlen(name);
    std::strcat(buf.mPath, name);
    buf.mLength += eLen;

    evtStartMoviePlayback(buf.mPath, 1, (void*)EvtSeqGetC4FlagBit1());
    self->mFlag19 = 0;
}

// ============================================================================
// EvtMovie_TogglePlayback_4BA4: Build SFD path and start/stop playback
// If playing, stops. Otherwise, starts with flag 0.
// ============================================================================
void EvtMovie_TogglePlayback_4BA4(CREvtMovie* self) {
    if (self->mScriptData->mCounter != EvtSeqGetWalkIndex()) return;

    // If currently playing, stop
    if (evtIsMoviePaused() != 0) {
        evtClearMoviePause();
    }
    // Otherwise if not finished, start playback
    else if (evtHasMoviePlayer() == 0) {
        // Build file path on stack: "/ev/realtime/" + scriptName + ".sfd"
        const char* name;
        const char* dir = "/ev/realtime/";
        CREvtMoviePathBuf buf;
        buf.mLength = std::strlen(dir);
        std::strcpy(buf.mPath, dir);

        name = self->mScriptData->mScriptName;
        u32 sLen = std::strlen(name);
        std::strcat(buf.mPath, name);
        buf.mLength += sLen;

        name = ".sfd";
        u32 eLen = std::strlen(name);
        std::strcat(buf.mPath, name);
        buf.mLength += eLen;

        evtStartMoviePlayback(buf.mPath, 0, (void*)EvtSeqGetC4FlagBit1());
        self->mFlag19 = 1;
    }
    self->mFlag19 = 1;
}

// ============================================================================
// CREvtMovie::vfunc_04: empty override (blr)
// ============================================================================
void cf::CREvtMovie::vfunc_04() {}

// ============================================================================
// Retail data owned by this TU. The vtable (0x80538AA0) and its RTTI record /
// "cf::CREvtMovie" name string are compiler-emitted for the class above.
// ============================================================================

