#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include <nw4r/lyt.h>
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

#include "kyoshin/CPresentWinClass.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Mirror of the font object returned by CDeviceFont::getFontInfo.
// vtable slot 9 (offset 0x24) yields the u32 bound into the layout's font
// pane. Never instantiated, so no vtable is emitted; a genuine virtual call
// makes MWCC emit the retail r12 dispatch sequence (same trick as
// CMenuPTGaugeFont / CFontPanel).
struct CPresentFontView {
    virtual void sf2() = 0;
    virtual void sf3() = 0;
    virtual void sf4() = 0;
    virtual void sf5() = 0;
    virtual void sf6() = 0;
    virtual void sf7() = 0;
    virtual void sf8() = 0;
    virtual u32 sf9() = 0; // vtable offset 0x24
};

// Texture object returned by ArcResourceAccessor::GetResource for the 'timg'
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
extern char lbl_eu_8050A84C[]; // layout path + pane-name + message string pool
extern const float lbl_eu_8066862C;    // animation advance delta (const -> sdata2 pool, lfs hoist)
extern f64 lbl_eu_80668620;    // 2^52 double used by the u32->float conversion
extern f32 lbl_eu_80668628;    // frame reset value written into animTrans1
extern u32 lbl_eu_80664104;    // BDAT table handle used by the time/rank lookup

// BDAT table handle (sda21) used by the item-name/message lookups
extern void* lbl_eu_80664090;

// Retail symbols that ARE the Itanium-mangled C++ names: declare under C
// linkage so calls bind to the literal mangled identifier.
extern "C" {
void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
}

// Layout teardown view: with -RTTI the virtual index N sits at vtable offset
// (N+2)*4, so index 0 lands exactly on 0x8. A genuine virtual call makes MWCC
// emit the retail r12-chain (`lwz r12,0(rN); lwz r12,0x8(r12); mtctr; bctrl`).
class CPresentLayoutSlot2View {
public:
    virtual void mSlot2(u32 arg); // index 0 -> vtable 0x8
};

// Unmangled retail helpers: the retail reloc at every call site is the bare
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
