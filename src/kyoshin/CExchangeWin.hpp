#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

class CFileHandle;
class CExchangeWin;
extern "C" void ExWin_CopyPanePair(void* dst, CExchangeWin* src, u8 val);
class CEventFile;

#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

/* CExchangeWin - item-exchange confirmation window (retail vtable
   __vt__12CExchangeWin, IWorkEvent base list): receives its layout archive
   through IWorkEvent::OnFileEvent. */
class CExchangeWin : public IWorkEvent {
public:
    CExchangeWin();
    virtual ~CExchangeWin();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    // field accessors - retail func_8022D094/08C/09C (recovered as members)
    int getField24();  // retail caller (CItemBoxGrid IsSubWinActive) compares the FULL word, no rlwinm mask
    int getField25();  // retail caller (CItemBoxGrid IsItemBoxReady) compares the FULL word, no rlwinm mask
    // Retail callers compare the FULL word (cmpi r3,0, no rlwinm mask), so the
    // recovered return type is int even though the body returns a byte.
    int getField27();

    void ExWin_LoadFile();
    void ExWin_TickUpdate();
    void ExWin_DrawLayout(nw4r::lyt::DrawInfo* drawInfo);
    void ExWin_Teardown();
    void ExWin_RequestOpen();
    void ExWin_RequestClose();
    void ExWin_SetTwoTexts(char* text1, char* text2);
    void ExWin_AdvanceEnter();
    void ExWin_AdvanceExit();

    UnkClass_8045F564 mMemRegion;                 // 0x04
    CFileHandle* mFileHandle;                      // 0x14
    nw4r::lyt::ArcResourceAccessor* mAccessor;     // 0x18
    nw4r::lyt::Layout* mLayout;                    // 0x1C
    nw4r::lyt::AnimTransform* mAnimTransform;      // 0x20
    u8 field_24;                                    // 0x24
    u8 field_25;                                    // 0x25
    u8 _26;                                         // 0x26
    u8 field_27;                                    // 0x27
};


// POD mirror of nw4r::lyt::Size (two f32): using the real type would emit
// the default-constructor zeroing, which retail does not have. Cast to the
// nw4r type at the SetSize call site (layout-identical).
struct CExchangeWinSize {
    f32 width;
    f32 height;
};

// Mirror of the font object returned by CDeviceFont::getFontInfo: its
// vtable slot 9 (+0x24) yields the u32 font handle bound into the layout.
// Never instantiated, so no vtable is emitted; a genuine virtual call makes
// MWCC emit the retail r12 dispatch sequence.
struct CExchangeWinFontView {
    virtual void sf2() = 0;
    virtual void sf3() = 0;
    virtual void sf4() = 0;
    virtual void sf5() = 0;
    virtual void sf6() = 0;
    virtual void sf7() = 0;
    virtual void sf8() = 0;
    virtual u32 sf9() = 0; // vtable offset 0x24
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail symbol is the pre-mangled Fv name but the call passes -1 in r3.
// Retail reloc is the UNMANGLED name; the C++ member in CUICfManager.hpp
// would mangle it, so keep C linkage here.
