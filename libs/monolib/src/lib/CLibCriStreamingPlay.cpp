// CLibCriStreamingPlay - CRI streaming audio playback manager
// Inherits from CLibCri (which inherits CWorkThread, CDeviceVICb, IErrorWii)

#include "monolib/lib/CLibCriStreamingPlay.hpp"
#include <harness_catalog.h>
#include <stdint.h>

// Extern "C" stub declarations for functions not yet decompiled
// These will be replaced with proper member functions during full decompilation
extern "C" {
void func_8045CF30__20CLibCriStreamingPlayFv();
void __dt__20CLibCriStreamingPlayFv();
void func_8045CFDC__20CLibCriStreamingPlayFv();
void func_8045D03C__20CLibCriStreamingPlayFv();
void func_8045C67C__20CLibCriStreamingPlayFv();
void func_8045C700__20CLibCriStreamingPlayFv();
void func_8045C8B0__20CLibCriStreamingPlayFv();
void func_8045CA4C__20CLibCriStreamingPlayFv();
void func_8045CCFC__20CLibCriStreamingPlayFv();
}

bool CLibCriStreamingPlay::wkStandbyLogin() { return false; }

void CLibCriStreamingPlay::OnPauseTrigger(bool paused) {}

// ============================================================================
// FULL_MATCH functions (us-80461184, us-8046118c)
// Virtual thunks for CDeviceVICb overrides in CLibCriStreamingPlay.
// When called through the CDeviceVICb vtable, 'this' points to the
// CDeviceVICb subobject at offset 0x1c4 from the CLibCriStreamingPlay base.
// These thunks adjust 'this' back to the CLibCriStreamingPlay base and
// tail-call the implementation.
// ============================================================================

// Thunk: adjusts this from CDeviceVICb subobject (+0x1c4) to CLibCriStreamingPlay base,
// then calls the streaming cleanup function.
void CLibCriStreamingPlay::func_8045D140() {
    typedef void (*CleanupFn)(CLibCriStreamingPlay*);
    CleanupFn cleanup = reinterpret_cast<CleanupFn>(func_8045CF30__20CLibCriStreamingPlayFv);
    CLibCriStreamingPlay* base = reinterpret_cast<CLibCriStreamingPlay*>(
        reinterpret_cast<uintptr_t>(this) - 0x1c4
    );
    cleanup(base);
}

// Thunk: adjusts this from CDeviceVICb subobject (+0x1c4) to CLibCriStreamingPlay base,
// then calls the destructor.
void CLibCriStreamingPlay::func_8045D148() {
    typedef void (*DtorFn)(CLibCriStreamingPlay*);
    DtorFn dtor = reinterpret_cast<DtorFn>(__dt__20CLibCriStreamingPlayFv);
    CLibCriStreamingPlay* base = reinterpret_cast<CLibCriStreamingPlay*>(
        reinterpret_cast<uintptr_t>(this) - 0x1c4
    );
    dtor(base);
}
