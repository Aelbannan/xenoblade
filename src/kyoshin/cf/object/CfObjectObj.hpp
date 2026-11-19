#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"

namespace cf {
    class CfObjectPc;

    // Find the containing CfObjectPc for a CfObjectMove subobject.
    CfObjectPc* getCfObjectPc(CfObjectMove* objMove);

    // 0x71C CfObjectMove base (mField718 at 0x718-0x71B) + u16 field_71C
    // This TU owns __vt__Q22cf11CfObjectObj and its RTTI (key function:
    // the out-of-line virtual destructor).
    class CfObjectObj : public CfObjectMove {
    public:
        //0x0: vtable 1
        //0x0-71C: CfObjectMove (mField718 is the base's last field)

        void reset();
        int initialize();
        void update();

        CfObjectObj();
        virtual ~CfObjectObj();

        int setup(u32 flagA, u32 flagB);
        void setMoveId(u16 id);

        //0x71C helper field (first field past the 0x71C base)
        u16 field_71C;
    };
}

// C-ABI imports (retail unmangled names): ObjImplSyncSlotState dispatches a helper
// id on the +0x38 sub-object (ABI void* - shared with CfObjectImplObj stub);
// __ct__cf_CfResObjImpl constructs the 0x1C-byte resource object in place.
namespace cf { struct CfResObjImpl; }
extern "C" void ObjImplSyncSlotState(void* self, u16 id);
extern "C" cf::CfResObjImpl* __ct__cf_CfResObjImpl(cf::CfResObjImpl* self,
                                                   void* parent);