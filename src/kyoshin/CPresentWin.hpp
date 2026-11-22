#pragma once

#include "kyoshin/cf/CfBdat.hpp"

#include <types.h>
#include "kyoshin/CPresentWinApi.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include <nw4r/lyt.h>
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

#include "kyoshin/CPresentWinClass.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"


// tag. mChain (+0x08) -> mDims (+0x00) carries the 2D dimension header.
struct CPresentTexDims;
struct CPresentTexChain {
    CPresentTexDims* mDims; // +0x00
};
struct CPresentTexDims {
    u16 mW; // +0x00
    u16 mH; // +0x02
};
struct CPresentTexObj {
    void* mVtable;            // +0x00
    void* _pad_04;            // +0x04
    CPresentTexChain* mChain; // +0x08
};

// --- imports (retail linker names) ---
extern const float CPresentAnimStepDelta;    // animation advance delta (const -> sdata2 pool, lfs hoist)
extern f64 CPresentU32ToFloatMagic;    // 2^52 double used by the u32->float conversion
extern f32 CPresentFrameResetValue;    // frame reset value written into animTrans1

// BDAT table handle (sda21) used by the item-name/message lookups

// Retail symbols that ARE the Itanium-mangled C++ names: declare under C
// linkage so calls bind to the literal mangled identifier.
extern "C" {
void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
}


// identifier, so declare C linkage (a C++-linkage decl would emit a mangled
// reloc and drift at the reloc gate).
extern "C" {
void waitForDrawDone__9CDeviceVIFv();
void writePanePos(nw4r::lyt::Pane*, float*);
void repaintPresentItemA(CPresentWin* self);
void repaintPresentItemB(CPresentWin* self);
void stepPresentCounterBack(CPresentWin* self);
u8 lookupPresentRank(CPresentWin* self, u32 r4);
}
