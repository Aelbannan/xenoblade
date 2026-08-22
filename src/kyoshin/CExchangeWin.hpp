#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

class CFileHandle;
class CEventFile;

// Vtable for CExchangeWin (split1 .data)
extern "C" void* lbl_eu_80536640[];

/* CExchangeWin - layout-compatible with IWorkEvent (vptr @ 0) for
   CDeviceFile::readFile, but not a C++ IWorkEvent subclass.  Vtable is
   provided by split1; the constructor stores it manually. */
class CExchangeWin {
public:
    ~CExchangeWin();
    bool OnFileEvent(CEventFile* pEventFile);

    // field accessors - retail func_8022D094/08C/09C (recovered as members)
    int getField24();  // retail caller (CItemBoxGrid func_801CB1E4) compares the FULL word, no rlwinm mask
    int getField25();  // retail caller (CItemBoxGrid func_801CB038) compares the FULL word, no rlwinm mask
    // Retail callers compare the FULL word (cmpi r3,0, no rlwinm mask), so the
    // recovered return type is int even though the body returns a byte.
    int getField27();

    void* mVtbl;                                  // 0x00
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


// Shared string pool (split1 .rodata): arc path, pane names, msg-ids.
extern char lbl_eu_8050A740[];

// POD mirror of nw4r::lyt::Size (two f32): using the real type would emit
// the default-constructor zeroing, which retail does not have. Cast to the
// nw4r type at the SetSize call site (layout-identical).
struct CExchangeWinSize {
    f32 width;
    f32 height;
};

// Mirror of the font object returned by CDeviceFont::func_80452C10: its
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
extern "C" void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
// Class_8045F858 scratch-region RAII guard ctor/dtor (retail C-ABI names; the
// explicit dtor call is required so the -1 dealloc flag matches retail).
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void* self, void* base);
extern "C" void __dt__14Class_8045F858Fv(void* self, int dealloc);
// Retail symbol is the pre-mangled Fv name but the call passes -1 in r3.
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
// Retail reloc is the UNMANGLED name; the C++ member in CUICfManager.hpp
// would mangle it, so keep C linkage here.
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
