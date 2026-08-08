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
u32 func_8016A3C4(void);
u32 func_8016A378(void);
u32 func_8016A35C(void);

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

    // Build file path on stack
    const char* basePath = &lbl_eu_8050FD98[0];     // "/ev/realtime/"
    const char* scriptName = self->mScriptData->mScriptName;
    const char* extension = &lbl_eu_8050FD98[0x0E]; // ".sfd"

    char buf[0x20];
    s32 totalLen = strlen(basePath);
    strcpy(buf, basePath);
    totalLen += strlen(scriptName);
    strcat(buf, scriptName);
    totalLen += strlen(extension);
    strcat(buf, extension);

    void* handle = func_8016C3DC();
    func_80164ED0(buf, 1, handle);
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
    s32 t1 = func_8016A378();
    s32 t2 = func_8016A35C();
    if (t1 - t2 >= 25) return;  // too early

    // Check memory availability
    u32 criSize = (u32)func_80459AD0__7CLibCriFv();
    void* memHandle = getHandleMEM2__Q23mtl10MemManagerFv();
    u32 maxSize = getMaxAllocSize__Q23mtl10MemManagerFUl(memHandle);
    if (maxSize + 0x200 <= criSize) return;  // not enough memory

    // Double-check not playing
    if (func_80164FE8() != 0) return;

    // Build file path
    const char* basePath = &lbl_eu_8050FD98[0];
    const char* scriptName = self->mScriptData->mScriptName;
    const char* extension = &lbl_eu_8050FD98[0x0E];

    char buf[0x20];
    s32 totalLen = strlen(basePath);
    strcpy(buf, basePath);
    totalLen += strlen(scriptName);
    strcat(buf, scriptName);
    totalLen += strlen(extension);
    strcat(buf, extension);

    void* handle = func_8016C3DC();
    func_80164ED0(buf, 1, handle);
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
        const char* basePath = &lbl_eu_8050FD98[0];
        const char* scriptName = self->mScriptData->mScriptName;
        const char* extension = &lbl_eu_8050FD98[0x0E];

        char buf[0x20];
        s32 totalLen = strlen(basePath);
        strcpy(buf, basePath);
        totalLen += strlen(scriptName);
        strcat(buf, scriptName);
        totalLen += strlen(extension);
        strcat(buf, extension);

        void* handle = func_8016C3DC();
        func_80164ED0(buf, 0, handle);
        self->mFlag19 = 1;
    }
    self->mFlag19 = 1;
}

// ============================================================================
// func_80294CB0: Empty virtual function override (blr)
// ============================================================================
extern "C" void func_80294CB0(void) {}