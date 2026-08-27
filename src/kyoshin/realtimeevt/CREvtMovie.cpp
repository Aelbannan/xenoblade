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
    __ct__cf_CREvtObj(self, 4);
    self->vtable = &lbl_eu_80538AA0[0];
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
        self->vtable = &lbl_eu_80538AA0[0];
        func_80164F6C();
        __dt__Q22cf8CREvtObjFv(self, 0);

        if (dealloc_flag > 0) {
            __dt__80185754(self);
        }
    }
    return self;
}

// ============================================================================
// func_8029493C: Check counter and cleanup if matches
// If scriptData->counter == currentTick + 1, calls func_80164F6C()
// ============================================================================
void func_8029493C(CREvtMovie* self) {
    if (self->mScriptData->mCounter == func_8016A3C4() + 1) {
        func_80164F6C();
    }
}

// ============================================================================
// func_80294980: Build SFD path and start playback
// If scriptData->counter == currentTick and not already playing,
// builds "/ev/realtime/" + scriptName + ".sfd" and starts playback
// opt_propagation off keeps `dir` as an opaque held base pointer (retail
// materializes it once into a callee-saved reg; ext = runtime dir + 14).
// ============================================================================
#pragma push
#pragma opt_propagation off
void func_80294980(CREvtMovie* self) {
    if (self->mScriptData->mCounter != func_8016A3C4()) return;

    // Check if already playing or finished
    if (func_80164FE8() != 0) return;

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

    func_80164ED0(buf.mPath, 1, func_8016C3DC());
    self->mFlag19 = 0;
}
#pragma pop

// ============================================================================
// func_80294A70: Build SFD path and start playback (with timing checks)
// Checks counter (currentTick + 1), timing, and memory availability
// Same held-base pattern as func_80294980: opt_propagation off keeps `dir`
// materialized once in a callee-saved register; ext reuses the dead name reg.
// ============================================================================
#pragma push
#pragma opt_propagation off
void func_80294A70(CREvtMovie* self) {
    if (self->mScriptData->mCounter != func_8016A3C4() + 1) return;

    // Check if already playing or finished
    if (func_80164FE8() != 0) return;

    // Check timing: elapsed time since last event
    if (func_8016A378() - func_8016A35C() >= 25) return;  // too early

    // Check memory availability
    u32 criSize = (u32)getMovieWorkSize__7CLibCriFv();
    if (mtl::MemManager::getMaxAllocSize(mtl::MemManager::getHandleMEM2()) + 0x200 <= criSize)
        return;

    // Double-check not playing
    if (func_80164FE8() != 0) return;

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

    func_80164ED0(buf.mPath, 1, func_8016C3DC());
    self->mFlag19 = 0;
}
#pragma pop

// ============================================================================
// func_80294BA4: Build SFD path and start/stop playback
// If playing, stops. Otherwise, starts with flag 0.
// Same held-base pattern as func_80294980: opt_propagation off keeps `dir`
// materialized once in a callee-saved register; ext is runtime dir + 14.
// ============================================================================
#pragma push
#pragma opt_propagation off
void func_80294BA4(CREvtMovie* self) {
    if (self->mScriptData->mCounter != func_8016A3C4()) return;

    // If currently playing, stop
    if (func_80164FB4() != 0) {
        func_80165014();
    }
    // Otherwise if not finished, start playback
    else if (func_80164FE8() == 0) {
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

        func_80164ED0(buf.mPath, 0, func_8016C3DC());
        self->mFlag19 = 1;
    }
    self->mFlag19 = 1;
}
#pragma pop

// ============================================================================
// func_80294CB0: Empty virtual function override (blr)
// ============================================================================
void func_80294CB0(void) {}
