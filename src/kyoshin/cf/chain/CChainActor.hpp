#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainTemp.hpp"
#include "kyoshin/cf/chain/CChainEffect.hpp"
#include <cstring>

namespace cf {
    struct CChainActorData {
        u32 unk0;
        CChainTemp mChainTemp;
        u16 unk6C;
        u8 _pad6E[2];
    };
    // CChainActorData is 0x70 bytes (4 + 0x68 + 2 +2)

    struct CChainActorVtbl { void* slots[30]; };
    extern "C" CChainActorVtbl lbl_eu_80538290;

    class __declspec(novtable) CChainActor : public CChainActorData {
    public:
        // vptr at 0x70 (after base), retail table at lbl_eu_80538290
        CChainActor();
        virtual ~CChainActor();

        // 17 virtuals to reach +0x48 (dt + 14 dummies + _vf44 + vf48) – rest of table beyond 0x48 is manual (0x4C..0x74)
        virtual void _vf0C(int val);
        virtual void _vf10();
        virtual void _vf14();
        virtual void _vf18();
        virtual void _vf1C();
        virtual int _vf20(int arg);
        virtual void _vf24();
        virtual void _vf28();
        virtual void _vf2C();
        virtual void _vf30(int p1,int p2,int p3);
        virtual void* _vf34();
        virtual void _vf38();
        virtual void _vf3C();
        virtual int _vf40();
        virtual int _vf44(void* arg);
        virtual int vf48(void* arg);

        u8 mChainEffectRaw[0xC]; // 0x74

        CChainActorVtbl*& vtbl() { return *reinterpret_cast<CChainActorVtbl**>(reinterpret_cast<u8*>(this) + 0x70); }
        CChainActorVtbl* vtbl() const { return *reinterpret_cast<CChainActorVtbl*const*>(reinterpret_cast<const u8*>(this) + 0x70); }
        u32& mVTable() { return *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x70); }
        const u32& mVTable() const { return *reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(this) + 0x70); }
    };
}
