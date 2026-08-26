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

        // Retail constructs this class only through the forced-name free
        // function __ct__cf_CfObjectObj (defined in the .cpp); there is no
        // mangled member ctor symbol in retail.

        void func_800BFAAC();
        int func_800BFA88();
        void func_800BFB90();
        int func_800BFAB0(u32 arg4, u32 arg5);

        // Declared but NOT defined in this fork: retail's CfObjectObj ctor
        // exists under the forced flat name __ct__cf_CfObjectObj (defined as
        // a free function in CfObjectObj.cpp). Derived ctors synthesize an
        // UNDEF bl that postprocess exact_renames maps onto it.
        CfObjectObj();

        // Declared but NOT defined in this fork: retail's CfObjectObj dtor
        // exists under the forced flat name __dt__800BFA14 (defined as a
        // free function in CfObjectObj.cpp). Derived dtors (CfObjectTbox)
        // synthesize an UNDEF bl to the mangled name, which postprocess
        // exact_renames maps onto that retail-named definition so both the
        // call target and codegen shape match retail's dtor-chain.
        ~CfObjectObj();

        //0x71C helper field (first field past the 0x71C base)
        u16 field_71C;
    };

    // Virtual-dispatch view used only to reproduce MWCC's r12 vtable call for
    // the 0x144 helper slot, which retail invokes with a 2-arg signature that
    // the locked CfObject header declares 0-arg. The dummy-vtable slot count is
    // calibrated so MWCC lays fn144() out at vtable offset 0x144 (MWCC adds a
    // small slot overhead, so this is 79 dummies, not a naive 81). Calling via
    // this view dispatches through whatever object actually sits behind `this`
    // at runtime (a plain vtable deref would codegen a temp base reg instead).
    struct CfFn144View {
        virtual void vt0() = 0;  virtual void vt1() = 0;
        virtual void vt2() = 0;  virtual void vt3() = 0;
        virtual void vt4() = 0;  virtual void vt5() = 0;
        virtual void vt6() = 0;  virtual void vt7() = 0;
        virtual void vt8() = 0;  virtual void vt9() = 0;
        virtual void vt10() = 0; virtual void vt11() = 0;
        virtual void vt12() = 0; virtual void vt13() = 0;
        virtual void vt14() = 0; virtual void vt15() = 0;
        virtual void vt16() = 0; virtual void vt17() = 0;
        virtual void vt18() = 0; virtual void vt19() = 0;
        virtual void vt20() = 0; virtual void vt21() = 0;
        virtual void vt22() = 0; virtual void vt23() = 0;
        virtual void vt24() = 0; virtual void vt25() = 0;
        virtual void vt26() = 0; virtual void vt27() = 0;
        virtual void vt28() = 0; virtual void vt29() = 0;
        virtual void vt30() = 0; virtual void vt31() = 0;
        virtual void vt32() = 0; virtual void vt33() = 0;
        virtual void vt34() = 0; virtual void vt35() = 0;
        virtual void vt36() = 0; virtual void vt37() = 0;
        virtual void vt38() = 0; virtual void vt39() = 0;
        virtual void vt40() = 0; virtual void vt41() = 0;
        virtual void vt42() = 0; virtual void vt43() = 0;
        virtual void vt44() = 0; virtual void vt45() = 0;
        virtual void vt46() = 0; virtual void vt47() = 0;
        virtual void vt48() = 0; virtual void vt49() = 0;
        virtual void vt50() = 0; virtual void vt51() = 0;
        virtual void vt52() = 0; virtual void vt53() = 0;
        virtual void vt54() = 0; virtual void vt55() = 0;
        virtual void vt56() = 0; virtual void vt57() = 0;
        virtual void vt58() = 0; virtual void vt59() = 0;
        virtual void vt60() = 0; virtual void vt61() = 0;
        virtual void vt62() = 0; virtual void vt63() = 0;
        virtual void vt64() = 0; virtual void vt65() = 0;
        virtual void vt66() = 0; virtual void vt67() = 0;
        virtual void vt68() = 0; virtual void vt69() = 0;
        virtual void vt70() = 0; virtual void vt71() = 0;
        virtual void vt72() = 0; virtual void vt73() = 0;
        virtual void vt74() = 0; virtual void vt75() = 0;
        virtual void vt76() = 0; virtual void vt77() = 0;
        virtual void vt78() = 0;
        virtual void fn144(u32 value, u32 src) = 0;  // vtable slot 0x144
    };
}

// CfObjectObj retail vtable (data TU; the ctor/dtor store it explicitly, same
// scheme as CfObjectMove.hpp's lbl_eu_80529690).
extern u8 lbl_eu_80529B4C[];

// Base-class constructor import (defined in CfObjectMove.cpp as the real
// member ctor): called directly by the forced-name __ct__cf_CfObjectObj.
extern "C" void __ct__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self);

// Base-class destructor chain import (defined in CfObjectMove.cpp): called
// with flag 0 from the __dt__800BFA14 deleting destructor below.
extern "C" void __dt__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int flag);

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

// C-ABI imports (retail unmangled names): func_800CA580 dispatches a helper
// id on the +0x38 sub-object; __ct__cf_CfResObjImpl constructs the 0x1C-byte
// resource object in place and returns it (CfResObjImpl.hpp's member ctor
// would mangle to __ct__Q22cf10CfResObjImplFv - same scheme as CfObjectPc.hpp's
// __ct__cf_CfResPcImpl).
extern "C" void func_800CA580(void* self, u16 id);
extern "C" void* __ct__cf_CfResObjImpl(void* self, void* parent);