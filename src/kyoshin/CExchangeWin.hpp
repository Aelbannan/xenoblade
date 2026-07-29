#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

class CFileHandle;
class CEventFile;

// Vtable for CExchangeWin (split1 .data)
extern "C" void* lbl_eu_80536640[];

class CExchangeWin {
public:
    virtual ~CExchangeWin();
    void OnFileEvent(CEventFile* pEventFile);

    // vtable pointer at 0x00 (implicit, managed by compiler)
    UnkClass_8045F564 mMemRegion;               // 0x04
    CFileHandle* mFileHandle;                    // 0x14
    nw4r::lyt::ArcResourceAccessor* mAccessor;   // 0x18
    nw4r::lyt::Layout* mLayout;                  // 0x1C
    nw4r::lyt::AnimTransform* mAnimTransform;    // 0x20
    u8 field_24;                                  // 0x24
    u8 field_25;                                  // 0x25
    u8 _26;                                       // 0x26
    u8 field_27;                                  // 0x27
};

