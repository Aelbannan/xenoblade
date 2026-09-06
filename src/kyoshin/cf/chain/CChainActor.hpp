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

        // 28 entries (dt at +0x08 + 27) to reach +0x74 - covers full retail table lbl_eu_80538290 (0x78 bytes, header2+28).
        // Slot index == (offset-8)/4, same reckoning as CChainBattleObj vNNN in CChainTimer.hpp.
        // Each virtual below is named after its base-table word (lbl_eu_80538290); the Ene/Pc tables
        // (lbl_eu_80538458/84E0) override the same slots, noted per line as Ene/Pc impls:
        // +0x0C setFieldAndClear (Ene 81308 / Pc 81924), +0x10 79B34 (same / Pc 81958),
        // +0x14 79DC0 (Ene 8133C / Pc 81CB8), +0x18 79E48 (same / Pc 81CF0),
        // +0x1C 79F6C (same / Pc 81F38), +0x20 7A024 (Ene 81384 / Pc 81FA0),
        // +0x24/+0x28/+0x2C 7A338/7A58C/7A8C8 (shared), +0x30 noop_AA0C (Ene 81438 / Pc 82020),
        // +0x34 noop_795D0 (Ene 81460 / Pc 82048), +0x38 noop_78E00 (same / Pc 82490),
        // +0x3C getZero_78E04 (same / Pc 8248C), +0x40 getZero_AA04 (Ene 8146C / Pc 82054),
        // +0x44 getZero_A9F4 (Ene 818E4 / Pc 8246C), +0x48 getZero_A9FC (Ene 814E4 / Pc 82480),
        // +0x4C getZero_A9EC (Ene 815B8 / Pc 8209C), +0x50 noop_A9E8 (Ene 816FC / Pc 820D4),
        // +0x54 getZero_A9E0 (Ene 8183C / Pc 82100), +0x58 getChainCount (same / Pc 82174),
        // +0x5C getZero_A584 (same / Pc 82464), +0x60 getZero_A9D8 (Ene 818DC / Pc same-as-base),
        // +0x64 noop_79768 (Ene 818D4 / Pc 8245C), +0x68 78F70 (shared),
        // +0x6C 78F5C (same / Pc 822F8), +0x70 7A324 (shared), +0x74 getZero_A9D0 (same / Pc 821E0).
        virtual void CChain_setFieldAndClear(int val);
        virtual void func_80279B34();
        virtual void func_80279DC0();
        virtual void func_80279E48(int val);
        virtual void func_80279F6C(int val);
        virtual int func_8027A024(int arg);
        virtual int func_8027A338(int arg);
        virtual int func_8027A58C();
        virtual int func_8027A8C8();
        virtual void CChain_noop_AA0C(int p1, cf::CChainActor* p2, int p3);
        virtual void CChain_noop_795D0();
        virtual void CChain_noop_78E00(int a, int b);
        virtual int CChain_getZero_78E04();
        virtual int CChain_getZero_AA04();
        virtual int CChain_getZero_A9F4(void* arg);
        virtual int CChain_getZero_A9FC(void* arg);
        virtual int CChain_getZero_A9EC();
        virtual void CChain_noop_A9E8();
        virtual int CChain_getZero_A9E0();
        virtual int CChain_getChainCount();
        virtual int CChain_getZero_A584();
        virtual int CChain_getZero_A9D8();
        virtual void CChain_noop_79768(int key);
        virtual int func_80278F70();
        virtual void func_80278F5C(int val);
        virtual int func_8027A324();
        virtual int CChain_getZero_A9D0();

        u8 mChainEffectRaw[0xC]; // 0x74

        CChainActorVtbl*& vtbl() { return *reinterpret_cast<CChainActorVtbl**>(reinterpret_cast<u8*>(this) + 0x70); }
        CChainActorVtbl* vtbl() const { return *reinterpret_cast<CChainActorVtbl*const*>(reinterpret_cast<const u8*>(this) + 0x70); }
        u32& mVTable() { return *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x70); }
        const u32& mVTable() const { return *reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(this) + 0x70); }
    };
}
