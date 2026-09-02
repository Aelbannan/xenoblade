#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActor.hpp"

namespace cf {
// Retail vt: lbl_eu_805384E0 (US) / __vt__Q22cf13CChainActorPc (JP, 0x8053BC98, size 0x78).
// 28 slots (header 2 + 26 funcs). Words after header: dt, 81924, 81958, 81CB8, 81CF0, 81F38, 81FA0, A338, A58C, A8C8, 82020, 82048, 82490, 8248C, 82054, 8246C, 82480, 8209C, 820D4, 82100, 82174, 82464, A9D8, 8245C, 78F70, 822F8, A324, 821E0.
// Slot at vtable offset +0x48 (index 18 overall, func-index 16) is func_80282480 (int(void*)).
// Owning class per linker symbol is cf::CChainActorPc (leaf overrides base CChain_getZero_A9FC).
// CHelp precedent: novtable, vptr at +0x70 after the 0x70 pad, ctor writes lbl_eu_805384E0.
struct CChainActorPcVtbl {
    void* mSlots[30];
};
extern "C" cf::CChainActorPcVtbl lbl_eu_805384E0;

class __declspec(novtable) CChainActorPc : public CChainActor {
public:
    CChainActorPc();
    virtual ~CChainActorPc();
    // filler virtuals to position +0x48 correctly (header 2 + 16 dummies = offset 0x48 is index 16)
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
    virtual int _vf44(void* arg); // slot +0x44 : func_8028246C (thunk) – takes void** arg, forwards to +0x48
    virtual int vf48(void* arg);  // slot +0x48 : func_80282480 – int(void* p) where p = *(void**)arg, tests bit at +0x3f00
    // overlay for manual vptr at +0x70 (CChainActor::mVTable)
    CChainActorPcVtbl*& vtbl() { return *reinterpret_cast<CChainActorPcVtbl**>(reinterpret_cast<u8*>(this) + 0x70); }
};
}


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

// Forward: free function operating on the CChain/CChainActor memory layout
extern "C" void CChain_setFieldAndClear(void*, int);

extern "C" void func_802A08F4(void*);
extern "C" void func_802A0904(void*);
extern "C" void func_80279DC0(void*);
extern "C" int func_80148778(void*, int);
extern "C" int func_8027A024(void*, void*);
extern "C" int* func_8009ECB0();

// Retail symbol: func_80279F6C
extern "C" void func_80279F6C(void*, int);
