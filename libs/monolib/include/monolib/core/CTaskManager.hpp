#pragma once

#include "monolib/monolib_types.hpp"

/*
Manages the lifetime of the root tasks.
*/
class CTaskManager {
public:
    static void Create();
    static void Release();

    static void Move();
    static void Draw();
    static void Reset();

    static CProcess* GetRootProcGame();
    static CProcess* GetRootProcRealTime();
    static CProcess* GetRootProcScn();

private:
    static void Start();
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Root process pointers. Retail keeps them in the sbss data object
// (lbl_eu_806655B8/BC/C0/C4); declared extern so the SDA refs resolve to the
// retail slots at link. CRootProc itself is TU-local to CTaskManager.cpp; the
// forward declaration below is in this header's unnamed namespace so the TU's
// definition completes the same type.
namespace {
class CRootProc;
}

extern "C" CRootProc* lbl_eu_806655B8;  // lbl_eu_806655B8
extern "C" CRootProc* lbl_eu_806655BC;  // lbl_eu_806655BC
extern "C" CRootProc* lbl_eu_806655C0;  // lbl_eu_806655C0
extern "C" CRootProc* lbl_eu_806655C4;  // lbl_eu_806655C4
