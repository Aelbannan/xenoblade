// TU: kyoshin/realtimeevt/CREvtMovie
// Class: CREvtMovie (inherits from cf::CREvtObj)
// Own vtable at 0x80538AA0
// Size: 0x1A

#include <types.h>
#include <cstring>
#include "kyoshin/realtimeevt/CREvtMovie.hpp"
#include "kyoshin/realtimeevt/CREvtObj.hpp"

extern "C" {
// Base constructor/destructor
void __ct__cf_CREvtObj(cf::CREvtObj* self, int arg);
void __dt__Q22cf8CREvtObjFv(cf::CREvtObj* self, int dealloc_flag);
void __dt__80185754(void* ptr);

// Movie playback functions
void func_80164F6C(void);
int func_80164FE8(void);
int func_80164FB4(void);
void func_80165014(void);
void func_80164ED0(const char* path, int flag, void* handle);
void* func_8016C3DC(void);

// Counter/timing functions
// func_8016A3C4 is an unsigned tick counter; 8016A378/8016A35C return
// signed timestamps (compared with a signed `cmpi` in retail).
u32 func_8016A3C4(void);
int func_8016A378(void);
int func_8016A35C(void);

// Memory functions
void* func_80459AD0__7CLibCriFv(void);
void* getHandleMEM2__Q23mtl10MemManagerFv(void);
u32 getMaxAllocSize__Q23mtl10MemManagerFUl(void* handle);

// String functions
using std::strlen;
using std::strcpy;
using std::strcat;
}

// Vtable for CREvtMovie lives in kyoshin/realtimeevt/CREvtMovie.hpp
// (C-linkage imports section).

// ============================================================================
// Constructor: __ct__CREvtMovie
// r3 = this, r4 = arg
// Calls CREvtObj(this, 4), sets own vtable, initializes fields
// ============================================================================
extern "C" CREvtMovie* __ct__CREvtMovie(CREvtMovie* self, CREvtMovieScript* scriptData) {
    __ct__cf_CREvtObj((cf::CREvtObj*)self, 4);
    self->vtable = (void*)lbl_eu_80538AA0;
    self->mScriptData = scriptData;
    self->mFlag18 = 0;
    self->mFlag19 = 0;
    return self;
}

// ============================================================================
// __ct__802948D0: Constructor with cleanup (called during destruction)
// r3 = this, r4 = dealloc_flag
// ============================================================================
extern "C" CREvtMovie* __ct__802948D0(CREvtMovie* self, int dealloc_flag) {
    if (self != 0) {
        self->vtable = (void*)lbl_eu_80538AA0;
        func_80164F6C();
        __dt__Q22cf8CREvtObjFv((cf::CREvtObj*)self, 0);

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
extern "C" void func_8029493C(CREvtMovie* self) {
    if (self->mScriptData->mCounter == func_8016A3C4() + 1) {
        func_80164F6C();
    }
}

// ============================================================================
// func_80294980: Build SFD path and start playback
// If scriptData->counter == currentTick and not already playing,
// builds "/ev/realtime/" + scriptName + ".sfd" and starts playback
// ============================================================================
extern "C" void func_80294980(CREvtMovie* self) {
    if (self->mScriptData->mCounter != func_8016A3C4()) return;

    // Check if already playing or finished
    if (func_80164FE8() != 0) return;

    // Build file path on stack: "/ev/realtime/" + scriptName + ".sfd"
    const char* dir = lbl_eu_8050FD98;           // "/ev/realtime/"
    CREvtMoviePathBuf buf;
    buf.mLength = strlen(dir);
    strcpy(buf.mPath, dir);

    const char* name = self->mScriptData->mScriptName;
    u32 sLen = strlen(name);
    strcat(buf.mPath, name);
    buf.mLength += sLen;

    const char* ext = dir + 14;                     // ".sfd"
    u32 eLen = strlen(ext);
    strcat(buf.mPath, ext);
    buf.mLength += eLen;

    func_80164ED0(buf.mPath, 1, func_8016C3DC());
    self->mFlag19 = 0;
}

// ============================================================================
// func_80294A70: Build SFD path and start playback (with timing checks)
// Checks counter (currentTick + 1), timing, and memory availability
// ============================================================================
extern "C" void func_80294A70(CREvtMovie* self) {
    if (self->mScriptData->mCounter != func_8016A3C4() + 1) return;

    // Check if already playing or finished
    if (func_80164FE8() != 0) return;

    // Check timing: elapsed time since last event
    if (func_8016A378() - func_8016A35C() >= 25) return;  // too early

    // Check memory availability
    u32 criSize = (u32)func_80459AD0__7CLibCriFv();
    if (getMaxAllocSize__Q23mtl10MemManagerFUl(getHandleMEM2__Q23mtl10MemManagerFv()) + 0x200 <= criSize)
        return;

    // Double-check not playing
    if (func_80164FE8() != 0) return;

    // Build file path
    const char* dir = lbl_eu_8050FD98;           // "/ev/realtime/"
    CREvtMoviePathBuf buf;
    buf.mLength = strlen(dir);
    strcpy(buf.mPath, dir);

    const char* name = self->mScriptData->mScriptName;
    u32 sLen = strlen(name);
    strcat(buf.mPath, name);
    buf.mLength += sLen;

    const char* ext = dir + 14;                     // ".sfd"
    u32 eLen = strlen(ext);
    strcat(buf.mPath, ext);
    buf.mLength += eLen;

    func_80164ED0(buf.mPath, 1, func_8016C3DC());
    self->mFlag19 = 0;
}

// ============================================================================
// func_80294BA4: Build SFD path and start/stop playback
// If playing, stops. Otherwise, starts with flag 0.
// ============================================================================
extern "C" void func_80294BA4(CREvtMovie* self) {
    if (self->mScriptData->mCounter != func_8016A3C4()) return;

    // If currently playing, stop
    if (func_80164FB4() != 0) {
        func_80165014();
    }
    // Otherwise if not finished, start playback
    else if (func_80164FE8() == 0) {
        // Build file path on stack: "/ev/realtime/" + scriptName + ".sfd"
        const char* dir = lbl_eu_8050FD98;       // "/ev/realtime/"
        CREvtMoviePathBuf buf;
        buf.mLength = strlen(dir);
        strcpy(buf.mPath, dir);

        const char* name = self->mScriptData->mScriptName;
        u32 sLen = strlen(name);
        strcat(buf.mPath, name);
        buf.mLength += sLen;

        const char* ext = dir + 14;                 // ".sfd"
        u32 eLen = strlen(ext);
        strcat(buf.mPath, ext);
        buf.mLength += eLen;

        func_80164ED0(buf.mPath, 0, func_8016C3DC());
        self->mFlag19 = 1;
    }
    self->mFlag19 = 1;
}

// ============================================================================
// func_80294CB0: Empty virtual function override (blr)
// ============================================================================
extern "C" void func_80294CB0(void) {}