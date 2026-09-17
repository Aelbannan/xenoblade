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
    virtual void CChain_setFieldAndClear(int val) override;
    virtual void func_80279B34() override;
    virtual void func_80279DC0() override;
    virtual void func_80279E48(int val) override;
    virtual void func_80279F6C(int val) override;
    virtual int func_8027A024(int arg) override;
    virtual int func_8027A338(int arg) override;
    virtual int func_8027A58C() override;
    virtual int func_8027A8C8() override;
    virtual void CChain_noop_AA0C(int p1, cf::CChainActor* p2, int p3) override;
    virtual void CChain_noop_795D0() override;
    virtual void CChain_noop_78E00(int a, int b) override;
    virtual int CChain_getZero_78E04() override;
    virtual int CChain_getZero_AA04() override;
    virtual int CChain_getZero_A9F4(void* arg) override;
    virtual int CChain_getZero_A9FC(void* arg) override;
    virtual int CChain_getZero_A9EC() override;
    virtual void CChain_noop_A9E8() override;
    virtual int CChain_getZero_A9E0() override;
    virtual int CChain_getChainCount() override;
    virtual int CChain_getZero_A584() override;
    virtual int CChain_getZero_A9D8() override;
    virtual void CChain_noop_79768(int key) override;
    virtual int func_80278F70() override;
    virtual void func_80278F5C(int val) override;
    virtual int func_8027A324() override;
    virtual int CChain_getZero_A9D0() override;
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
extern "C" int* CtrlObjectParam_GetSlotTableBase();

// Retail symbol: func_80279F6C
extern "C" void func_80279F6C(void*, int);
