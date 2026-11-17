#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"

namespace cf {
    // 0x71C CfObjectMove base (mField718 at 0x718-0x71B) + u16 field_71C
    // The retail vtable lives in a data TU (lbl_eu_80529B4C); the
    // compiler-generated __vt__Q22cf11CfObjectObj reloc name would drift, so
    // novtable suppresses the implicit vptr store and the ctor/dtor assign
    // the retail label explicitly (same scheme as CfObjectMove.hpp).
    class __declspec(novtable) CfObjectObj : public CfObjectMove {
    public:
        //0x0: vtable 1
        //0x0-71C: CfObjectMove (mField718 is the base's last field)

        void reset();
        int initialize();
        void update();

        CfObjectObj();
        virtual ~CfObjectObj();

        //0x71C helper field (first field past the 0x71C base)
        u16 field_71C;
    };
}

// CfObjectObj retail vtable (data TU; the ctor/dtor store it explicitly, same
// scheme as CfObjectMove.hpp's lbl_eu_80529690).
extern u8 lbl_eu_80529B4C[];

// RTTI typeinfo pair for the ctor's __dynamic_cast guard: the 0x1C-byte
// CfResObjImpl resource object is allocated only when the cast fails. Passed
// by address (&lbl) so MWCC emits li rN, lbl@sda21 (CBattleManager pattern);
// fixed-size decls (8-byte typeinfo objects) keep them sdata-eligible
// (MWCC_CASES: incomplete-array externs fall back to lis/addi).
extern char lbl_eu_80661D18[8];
extern char lbl_eu_80661D20[8];

// Runtime RTTI helper (retail unmangled symbol; MWCC emits the plain name).
void* __dynamic_cast(void* obj, long offset, const void* src_type,
                     const void* dst_type, void* src2dst);

// C-ABI imports (retail unmangled names): ObjImplSyncSlotState dispatches a helper
// id on the +0x38 sub-object (ABI void* - shared with CfObjectImplObj stub);
// __ct__cf_CfResObjImpl constructs the 0x1C-byte resource object in place.
namespace cf { struct CfResObjImpl; }
extern "C" void ObjImplSyncSlotState(void* self, u16 id);
extern "C" cf::CfResObjImpl* __ct__cf_CfResObjImpl(cf::CfResObjImpl* self,
                                                   void* parent);