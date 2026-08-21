#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectObj.hpp"

namespace cf {
    //size: 0x738
    class CfObjectTbox : public CfObjectObj {
    public:
        //0x0: vtable
        //0x0-71F: CfObjectObj (0x71C base + u16 field_71C, padded to 0x720)
        u32 field_720;      // 0x720
        u8 _724[0x10];      // 0x724-0x733
        u32 field_734;      // 0x734
        u16 field_738;      // 0x738
        u8 _73A[2];         // 0x73A-0x73B
        u32 field_73C;      // 0x73C
    CfObjectTbox();
    virtual ~CfObjectTbox();
    void func_801F8DD0();
    };
}

// Retail vtable data for CfObjectTbox (.data @0x80534FB8); stored at +0x00 by
// the ctor over the base's novtable placeholder.
extern u8 lbl_eu_80534FB8[];

namespace cf {
struct CfResTboxImpl;  // full def in kyoshin/cf/CfResTboxImpl.hpp
}

// Cast-only view of the +0xB0 sub-object: its vtable pointer lives at object
// offset 0x10 (manual-vtable scheme, cf. CfResTboxShift), and slot +0x08
// releases the resource (called with the delete-flag argument 1). MWCC adds
// two overhead slots when lowering the view's virtual call, so no dummy
// slots are needed to land on retail's 0x8 (cf. CfFn144View).
struct CfTboxSubB0Vt {
    u8 pad[0x10];
    virtual void _v008(u32 flag);
};

// C-ABI import (retail unmangled name): the CfObjectObj base constructor.
// Retail names it with the flat cf_ scheme (like __ct__cf_CfResObjImpl), so
// the Tbox ctor calls it directly instead of through a C++ base initializer
// (which would emit a mangled Q2- reloc the retail side doesn't carry).
extern "C" void* __ct__cf_CfObjectObj(cf::CfObjectObj* self);

// C-ABI import (retail unmangled name): constructs the 0x1C-byte textbox
// resource object in place and returns it (defined in CfResTboxImpl.cpp).
extern "C" void* __ct__cf_CfResTboxImpl(cf::CfResTboxImpl* self, void* parent);
