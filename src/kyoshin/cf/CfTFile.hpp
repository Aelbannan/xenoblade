#pragma once

#include <types.h>
#include "monolib/work/CEventFile.hpp"

namespace cf {

class CfTFile {
public:
    CfTFile();
    virtual ~CfTFile();
    virtual void OnFileEvent(CEventFile* pEventFile);

    //0x0: vtable
};

} // namespace cf

// File event frame timer (sbss, packed layout):
//   bits 0-5:   60 Hz sub-second counter (0-59)
//   bits 6-11:  second counter (0-59)
//   bits 12-31: minute counter (0-9999)
extern u32 lbl_eu_80663DA8;
