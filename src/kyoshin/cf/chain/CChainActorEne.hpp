#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActor.hpp"

namespace cf {
    class __declspec(novtable) CChainActorEne : public CChainActor {
    public:
        CChainActorEne();
        virtual ~CChainActorEne();
        virtual void _vf0C(int val) override;
        virtual void _vf10() override;
        virtual void _vf14() override;
        virtual void _vf18(int val) override;
        virtual void _vf1C(int val) override;
        virtual int _vf20(int arg) override;
        virtual int _vf24(int arg) override;
        virtual int _vf28() override;
        virtual int _vf2C() override;
        virtual void _vf30(int p1, cf::CChainActor* p2, int p3) override;
        virtual void _vf34() override;
        virtual void _vf38(int a, int b) override;
        virtual int _vf3C() override;
        virtual int _vf40() override;
        virtual int _vf44(void* arg) override;
        virtual int vf48(void* arg) override;
        virtual int _vf4C() override;
        virtual void _vf50() override;
        virtual int _vf54() override;
        virtual int _vf58() override;
        virtual int _vf5C() override;
        virtual int _vf60() override;
        virtual void _vf64(int key) override;
        virtual int _vf68() override;
        virtual void _vf6C(int val) override;
        virtual void _vf70() override;
        virtual int _vf74() override;
    };
}

