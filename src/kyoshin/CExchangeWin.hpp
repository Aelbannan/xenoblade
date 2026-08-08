#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

class CFileHandle;
class CEventFile;

// Vtable for CExchangeWin (split1 .data)
extern "C" void* lbl_eu_80536640[];

/* CExchangeWin — layout-compatible with IWorkEvent (vptr @ 0) for
   CDeviceFile::readFile, but not a C++ IWorkEvent subclass.  Vtable is
   provided by split1; the constructor stores it manually. */
class CExchangeWin {
public:
    ~CExchangeWin();
    bool OnFileEvent(CEventFile* pEventFile);

    // field accessors — retail func_8022D094/08C/09C (recovered as members)
    u8 getField24();
    u8 getField25();
    u8 getField27();

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

