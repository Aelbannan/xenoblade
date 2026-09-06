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

        // 28 entries (dt at +0x08 + 27) to reach +0x74 - covers full retail table lbl_eu_80538290 (0x78 bytes, header2+28)
        virtual void _vf0C(int val);
        virtual void _vf10();
        virtual void _vf14();
        virtual void _vf18(int val);
        virtual void _vf1C(int val);
        virtual int _vf20(int arg);
        virtual int _vf24(int arg);
        virtual int _vf28();
        virtual int _vf2C();
        virtual void _vf30(int p1, cf::CChainActor* p2, int p3);
        virtual void _vf34();
        virtual void _vf38(int a, int b);
        virtual int _vf3C();
        virtual int _vf40();
        virtual int _vf44(void* arg);
        virtual int vf48(void* arg);
        virtual int _vf4C();
        virtual void _vf50();
        virtual int _vf54();
        virtual int _vf58();
        virtual int _vf5C();
        virtual int _vf60();
        virtual void _vf64(int key);
        virtual int _vf68();
        virtual void _vf6C(int val);
        virtual int _vf70();
        virtual int _vf74();

        u8 mChainEffectRaw[0xC]; // 0x74

        CChainActorVtbl*& vtbl() { return *reinterpret_cast<CChainActorVtbl**>(reinterpret_cast<u8*>(this) + 0x70); }
        CChainActorVtbl* vtbl() const { return *reinterpret_cast<CChainActorVtbl*const*>(reinterpret_cast<const u8*>(this) + 0x70); }
        u32& mVTable() { return *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x70); }
        const u32& mVTable() const { return *reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(this) + 0x70); }
    };
}
