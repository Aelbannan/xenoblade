#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActor.hpp"

namespace cf {
    class CChainActorEne : public CChainActor {
    public:
        // No additional members beyond CChainActor
        void func_802818D4();
        s32 func_802818DC();
    };

    // Typed view of a CChainActor whose manually-managed vtable pointer lives at
    // +0x70 (CChainActor::mVTable). Never instantiated; used only so the manual
    // vtable dispatch at slot 0x50 emits as a real r12-bidirectional virtual call
    // (lwz r12,[self+0x70]; lwz r12,[r12+0x50]; mtctr; bctrl). 21 pure virtuals
    // position wf50 at vtable offset 0x50 (index 20).
    class CChainActorEneVtDispatch {
    public:
        u8 _pad[0x70];
        virtual void wf00() = 0;
        virtual void wf04() = 0;
        virtual void wf08() = 0;
        virtual void wf0C() = 0;
        virtual void wf10() = 0;
        virtual void wf14() = 0;
        virtual void wf18() = 0;
        virtual void wf1C() = 0;
        virtual void wf20() = 0;
        virtual void wf24() = 0;
        virtual void wf28() = 0;
        virtual void wf2C() = 0;
        virtual void wf30() = 0;
        virtual void wf34() = 0;
        virtual void wf38() = 0;
        virtual void wf3C() = 0;
        virtual int wf40() = 0;  // dispatched target (vtable offset 0x40) returns a truth flag
        virtual void wf44() = 0;
        virtual void wf50() = 0; // dispatched target (vtable offset 0x50)
    };

    // Dedicated dispatcher for the vtable offset 0x40 hook (func_8028183C).
    // With -RTTI on, MWCC reserves two leading vtable slots (offset-to-top +
    // typeinfo), so a virtual at index 14 lands at offset (14+2)*4 == 0x40.
    class CChainActorEneVtDispatch40 {
    public:
        u8 _pad[0x70];
        virtual void wf00() = 0;
        virtual void wf04() = 0;
        virtual void wf08() = 0;
        virtual void wf0C() = 0;
        virtual void wf10() = 0;
        virtual void wf14() = 0;
        virtual void wf18() = 0;
        virtual void wf1C() = 0;
        virtual void wf20() = 0;
        virtual void wf24() = 0;
        virtual void wf28() = 0;
        virtual void wf2C() = 0;
        virtual void wf30() = 0;
        virtual void wf34() = 0;
        virtual int target() = 0; // index 14 -> vtable offset 0x40
    };

    // Dispatcher for the standard object rooted at obj+4 (func_80281384). Its
    // vptr lives at +0 (normal C++ ABI). With -RTTI's two leading vtable slots,
    // a virtual at index 10 lands at vtable offset (10+2)*4 == 0x30; the hook
    // returns a pointer whose first field is an id.
    class ChainSubDispatch {
    public:
        virtual void f00() = 0;
        virtual void f04() = 0;
        virtual void f08() = 0;
        virtual void f0C() = 0;
        virtual void f10() = 0;
        virtual void f14() = 0;
        virtual void f18() = 0;
        virtual void f1C() = 0;
        virtual void f20() = 0;
        virtual void f24() = 0;
        virtual void* target() = 0; // index 10 -> vtable offset 0x30
    };
}

