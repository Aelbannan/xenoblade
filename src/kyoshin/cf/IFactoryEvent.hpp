#pragma once

#include <types.h>

namespace cf{

    class CfObject;

    // Factory-manager callbacks; the manager passes the spawned/removed object
    // (retail reads it from r4, see the slot-4 call in teardown's queue drain).
    class IFactoryEvent{
    public:
        virtual ~IFactoryEvent(){}
        virtual void FactoryEvent1(){}
        virtual void FactoryEvent2(CfObject* obj) = 0;
        virtual void FactoryEvent3(){}
        virtual void FactoryEvent4(){}
    };

}
