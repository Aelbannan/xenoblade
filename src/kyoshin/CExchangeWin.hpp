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
    void OnFileEvent(CEventFile* pEventFile);

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

