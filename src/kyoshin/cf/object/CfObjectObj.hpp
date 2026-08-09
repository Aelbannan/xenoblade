#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"

namespace cf {
    //size 0x718
    class CfObjectObj : public CfObjectMove {
    public:
        //0x0: vtable 1
        //0x0-718: CfObjectMove

        CfObjectObj();
        virtual ~CfObjectObj();

        void func_800BFAAC();
        int func_800BFA88();
        void func_800BFB90();
        int func_800BFAB0(u32 arg4, u32 arg5);

        //0x718-0x71B
        u8 _718[4];
        u16 field_71C;      //0x71C helper field
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