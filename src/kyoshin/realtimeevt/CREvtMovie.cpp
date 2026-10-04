// TU: kyoshin/realtimeevt/CREvtMovie
// Class: CREvtMovie (inherits from cf::CREvtObj)
// Own vtable at 0x80538AA0
// Size: 0x1A

#include <types.h>
#include <cstring>
#include "kyoshin/realtimeevt/CREvtMovie.hpp"
#include "monolib/util/MemManager.hpp"

// ============================================================================
// Constructor: __ct__CREvtMovie
// r3 = this, r4 = arg
// Calls CREvtObj(this, 4), sets own vtable, initializes fields
// ============================================================================
CREvtMovie* __ct__CREvtMovie(CREvtMovie* self, CREvtMovieScript* scriptData) {
    __ct__cf_CREvtObj(reinterpret_cast<cf::CREvtObj*>(self), 4);
    *(u32**)self = (u32*)(&lbl_eu_80538AA0[0]);
    self->mScriptData = scriptData;
    self->mFlag18 = 0;
    self->mFlag19 = 0;
    return self;
}

// ============================================================================
// __ct__802948D0: Destructor (vtable slot 0)
// NOTE (open item): retail compiles this as a REAL destructor of a class whose
// base cf::CREvtObj has a declared dtor, so its .extab entry carries a 0x1C
// DESTROYBASE action relocating __dt__Q22cf8CREvtObjFv. The C-linkage form
// below is byte-identical in .text but emits a plain 8-byte etb entry (no
// .relaextab), leaving objdiff's section metric at 0% (extab 0x30 vs retail
// 0x44). Proper fix needs ~CREvtObj() declared in CREvtObj.hpp + CREvtMovie
// inheriting cf::CREvtObj + a real ~CREvtMovie()/operator delete pair -- see
// session notes.
// ============================================================================
CREvtMovie* __ct__802948D0(CREvtMovie* self, int dealloc_flag) {
    if (self != 0) {
        *(u32**)self = (u32*)(&lbl_eu_80538AA0[0]);
        evtStopMoviePlayback();
        __dt__Q22cf8CREvtObjFv(reinterpret_cast<cf::CREvtObj*>(self), 0);

        if (dealloc_flag > 0) {
            __dt__80185754(self);
        }
    }
    return self;
}

// ============================================================================
// EvtMovie_MaybeAdvance_493C: Check counter and cleanup if matches
// If scriptData->counter == currentTick + 1, calls evtStopMoviePlayback()
// ============================================================================
void EvtMovie_MaybeAdvance_493C(CREvtMovie* self) {
    if (self->mScriptData->mCounter == EvtSeqGetWalkIndex() + 1) {
        evtStopMoviePlayback();
    }
}

// ============================================================================
// EvtMovie_StartPlayback_4980: Build SFD path and start playback
// If scriptData->counter == currentTick and not already playing,
// builds "/ev/realtime/" + scriptName + ".sfd" and starts playback
// opt_propagation off keeps `dir` as an opaque held base pointer (retail
// materializes it once into a callee-saved reg; ext = runtime dir + 14).
// ============================================================================
#pragma push
#pragma opt_propagation off
void EvtMovie_StartPlayback_4980(CREvtMovie* self) {
    if (self->mScriptData->mCounter != EvtSeqGetWalkIndex()) return;

    // Check if already playing or finished
    if (evtHasMoviePlayer() != 0) return;

    // Build file path on stack: "/ev/realtime/" + scriptName + ".sfd"
    // Suffix local assigned up front keeps the base pointer in a
    // callee-saved register across the strlen/strcpy/strcat calls.
    char* name;
    char* dir = lbl_eu_8050FD98;
    CREvtMoviePathBuf buf;
    buf.mLength = std::strlen(dir);
    std::strcpy(buf.mPath, dir);

    name = self->mScriptData->mScriptName;
    u32 sLen = std::strlen(name);
    std::strcat(buf.mPath, name);
    buf.mLength += sLen;

    name = dir + 14;  // extension string; reuses the (dead) name register
    u32 eLen = std::strlen(name);
    std::strcat(buf.mPath, name);
    buf.mLength += eLen;

    evtStartMoviePlayback(buf.mPath, 1, EvtSeqGetC4FlagBit1());
    self->mFlag19 = 0;
}
#pragma pop

// ============================================================================
// EvtMovie_PreloadMovie_4A70: Build SFD path and start playback (with timing checks)
// Checks counter (currentTick + 1), timing, and memory availability
// Same held-base pattern as EvtMovie_StartPlayback_4980: opt_propagation off keeps `dir`
// materialized once in a callee-saved register; ext reuses the dead name reg.
// ============================================================================
#pragma push
#pragma opt_propagation off
void EvtMovie_PreloadMovie_4A70(CREvtMovie* self) {
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
    char* name;
    char* dir = lbl_eu_8050FD98;
    CREvtMoviePathBuf buf;
    buf.mLength = std::strlen(dir);
    std::strcpy(buf.mPath, dir);

    name = self->mScriptData->mScriptName;
    u32 sLen = std::strlen(name);
    std::strcat(buf.mPath, name);
    buf.mLength += sLen;

    name = dir + 14;  // ".sfd"; reuses the (dead) name register
    u32 eLen = std::strlen(name);
    std::strcat(buf.mPath, name);
    buf.mLength += eLen;

    evtStartMoviePlayback(buf.mPath, 1, EvtSeqGetC4FlagBit1());
    self->mFlag19 = 0;
}
#pragma pop

// ============================================================================
// EvtMovie_TogglePlayback_4BA4: Build SFD path and start/stop playback
// If playing, stops. Otherwise, starts with flag 0.
// Same held-base pattern as EvtMovie_StartPlayback_4980: opt_propagation off keeps `dir`
// materialized once in a callee-saved register; ext is runtime dir + 14.
// ============================================================================
#pragma push
#pragma opt_propagation off
void EvtMovie_TogglePlayback_4BA4(CREvtMovie* self) {
    if (self->mScriptData->mCounter != EvtSeqGetWalkIndex()) return;

    // If currently playing, stop
    if (evtIsMoviePaused() != 0) {
        evtClearMoviePause();
    }
    // Otherwise if not finished, start playback
    else if (evtHasMoviePlayer() == 0) {
        // Build file path on stack: "/ev/realtime/" + scriptName + ".sfd"
        char* name;
        char* dir = lbl_eu_8050FD98;
        CREvtMoviePathBuf buf;
        buf.mLength = std::strlen(dir);
        std::strcpy(buf.mPath, dir);

        name = self->mScriptData->mScriptName;
        u32 sLen = std::strlen(name);
        std::strcat(buf.mPath, name);
        buf.mLength += sLen;

        name = dir + 14;  // ".sfd"; reuses the (dead) name register
        u32 eLen = std::strlen(name);
        std::strcat(buf.mPath, name);
        buf.mLength += eLen;

        evtStartMoviePlayback(buf.mPath, 0, EvtSeqGetC4FlagBit1());
        self->mFlag19 = 1;
    }
    self->mFlag19 = 1;
}
#pragma pop

// ============================================================================
// EvtMovie_Noop_4CB0: Empty virtual function override (blr)
// ============================================================================
void EvtMovie_Noop_4CB0(void) {}

// ============================================================================
// Retail data owned by this TU (dissolved blob; decomp emitted .text only).
// Defined here at EOF so -ipa codegen for the functions above is unaffected.
// ============================================================================

// Cross-TU vtable slots (flat retail names).
extern "C" void EvtObj_ConstFalse();
extern "C" void CREvtObjVfunc10Default();
extern "C" void REvtCam_AlwaysTrue1();
extern "C" void CREvtObjIsBusyDefault();
extern u32 lbl_eu_80662B18[];  // sdata locator word (defined below).

// [.data] 0x80538AA0-0x80538AC8 (40B): CREvtMovie vtable (sdata locator,
// null word, dtor, and 7 virtual slots; retail lays one label over all 10).
u32 lbl_eu_80538AA0[10] = {
    (u32)&lbl_eu_80662B18, 0x00000000,
    (u32)&__ct__802948D0, (u32)&EvtMovie_Noop_4CB0,
    (u32)&EvtMovie_MaybeAdvance_493C, (u32)&EvtObj_ConstFalse,
    (u32)&CREvtObjVfunc10Default, (u32)&REvtCam_AlwaysTrue1,
    (u32)&CREvtObjIsBusyDefault, (u32)&EvtMovie_PreloadMovie_4A70,
};

// [.rodata] 0x8050FD88-0x8050FDB0 (40B): class-name string (unreferenced pool
// bytes, kept under the retail label) + the "/ev/realtime/" + ".sfd" pair
// the playback builders index as dir / dir+14.
__declspec(section ".rodata") __attribute__((used, aligned(4)))
char lbl_eu_8050FD88[0xF] = {
    0x63,0x66,0x3A,0x3A,0x43,0x52,0x45,0x76,0x74,0x4D,0x6F,0x76,0x69,0x65,0x00,
};
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_8050FD98[0x18] = {
    0x2F,0x65,0x76,0x2F,0x72,0x65,0x61,0x6C,0x74,0x69,0x6D,0x65,0x2F,0x00,
    0x2E,0x73,0x66,0x64,0x00,0x00,0x00,0x00,0x00,0x00,
};

// [.sdata] 0x80662B18-0x80662B20 (8B): null word pair referenced by vtable[0].
#pragma section data_type ".sdata" ".sdata"
extern __declspec(section ".sdata") u32 lbl_eu_80662B18[2] = { 0x00000000, 0x00000000 };
#pragma section data_type ".data" ".data"
