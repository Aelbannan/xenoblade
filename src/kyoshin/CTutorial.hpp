#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"

#include "monolib/work/IWorkEvent.hpp"

// Forward declarations for nw4r layout types used by CTutorial members.
namespace nw4r {
namespace lyt {
class Layout;
class DrawInfo;
class AnimTransform;
class ArcResourceAccessor;
class Pane;
}
}

class CEventFile;
class CFileHandle;

// (Font-handle objects from CDeviceFont::getFontInfo dispatch through
// IDeviceFontInfo::getFont at +0x24; see monolib/device/CDeviceFont.hpp.)

// Tutorial page viewer widget; receives file-load completion through IWorkEvent.
class CTutorial : public IWorkEvent {
public:
    CTutorial(u8 param_1, u8 param_2);
    virtual ~CTutorial();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    // Draw the tutorial layout when the active flag is set (tail-calls
    // drawLayout).
    void Tutorial_DrawLayoutGated(nw4r::lyt::DrawInfo* drawInfo);
    // Idempotent start: mark state 1 and fire the 0x8 UI event once.
    void Tutorial_StartIfIdle();
    // Current tutorial phase (0-3) derived from the page counters at 0x50/0x51.
    u8 Tutorial_GetPageState();
    // Advance-animation completion: state 3, visible.
    void Tutorial_FinishAdvanceAnim();
    // Rewind-animation completion: state 0, visible.
    void Tutorial_FinishRewindAnim();

    bool Tutorial_GetField46Mark();
    bool Tutorial_GetField47Mark();
    bool Tutorial_GetField52Mark();

    // Load / unload the three tutorial data files (retail Tutorial_LoadTutorialFiles /
    // Tutorial_TeardownTutorial).
    void Tutorial_LoadTutorialFiles();
    void Tutorial_TeardownTutorial();
    void Tutorial_RefreshPage();
    // Page-navigation tick (retail Tutorial_NextPage).
    void Tutorial_NextPage();
    // Advance-anim start handler (retail Tutorial_StartAdvanceAnim).
    void Tutorial_StartAdvanceAnim();
    // Rewind-anim start handler (retail Tutorial_StartRewindAnim).
    void Tutorial_StartRewindAnim();
    // Advance-anim fully finished (retail Tutorial_AdvanceState3To4).
    void Tutorial_AdvanceState3To4();
    // Reset the 0x3340..0x33BE UI flag block (retail Tutorial_ResetSharedFlags).
    void Tutorial_ResetSharedFlags();
    // Per-frame tick: drive the state machine's animation step, then animate.
    void Tutorial_Move();
    // Page-counter tick backwards (retail Tutorial_PrevPage).
    void Tutorial_PrevPage();
    // Validate the loaded resources and reset the page counters (retail
    // Tutorial_ReloadTutorialData).
    void Tutorial_ReloadTutorialData();

    // IWorkEvent vptr (+0x00), then the widget data:
    UnkClass_8045F564 mRegion0;         // 0x04 - layout build region (0x1800)
    UnkClass_8045F564 mRegion1;         // 0x14 - data region (0x1000)
    CFileHandle* mFileHandle0;          // 0x24 - tutorial layout arc
    CFileHandle* mFileHandle1;          // 0x28 - locale-specific data
    CFileHandle* mFileHandle2;          // 0x2C - game data
    nw4r::lyt::ArcResourceAccessor* mAccessor0; // 0x30
    nw4r::lyt::ArcResourceAccessor* mAccessor1; // 0x34
    nw4r::lyt::Layout* mpLayout;             // 0x38
    nw4r::lyt::AnimTransform* mpAnimTrans0;  // 0x3C
    nw4r::lyt::AnimTransform* mpAnimTrans1;  // 0x40
    u8 field_44;                              // 0x44: active/draw gate
    u8 field_45;                              // 0x45: state
    bool field_46;                              // 0x46
    bool field_47;                              // 0x47: visible flag
    u8 field_48;                              // 0x48: current locale id
                                              // 0x49-0x4B: alignment padding
    u8* field_4C;                             // 0x4C: data buffer (deallocated on unload)
    s8 field_50;                              // 0x50: current page counter
    s8 field_51;                              // 0x51: page counter bound
    bool field_52;                              // 0x52
    u8 field_53;                              // 0x53: region/flags byte
};

// ---- imports folded from the former CTutorialImports.hpp ----
// Unit-private import declarations split out of CTutorial.hpp so the class header
// stays includable next to other headers.
#include "kyoshin/CTutorial.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"


// Layout draw helper (retail reloc is the mangled
// drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii).
// Fire a per-window sound/effect event by id (retail reloc
// playUISound__FUl; declared with C++ linkage so MWCC mangles it).

// Shared loaded-BDAT pointer used by the tutorial data loader.
extern u32 lbl_eu_80664A30;

// Font/sound helpers (retail reloc getPackedFont__Fv; C++ linkage so MWCC
// mangles the free function).

// Remaining helpers from the MenuStateInitFlags translation unit (same signatures
// as code_80135FDC.hpp so the mangled/unmangled reloc names are unchanged).
extern "C" void CtrlRemote_SetSharedBit(u32 destination, u32 value);

// Unmangled retail callees - C ABI. The retail reloc names for these are the
// literal unmangled identifiers, so they must be declared with C linkage.
extern "C" void* Bdat_GetTable_AA34();


// Layout build helpers (retail relocs are the mangled C++ names).
// Root-pane font bind (retail reloc is the literal unmangled name).
// BDAT field reader (retail reloc is the literal unmangled name).

// Retail calls the language-flag query through its unmangled Fv symbol with a
// dummy -1 argument that the callee ignores.
