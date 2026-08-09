#pragma once

#include <types.h>

// ---------------------------------------------------------------------------
// C-linkage imports
// ---------------------------------------------------------------------------

// External runtime / game functions (retail-unmangled symbols)
extern "C" void __ct__cf_CfGimmick(void*);
extern "C" void __dt__Q22cf9CfGimmickFv(void*, int);
extern "C" void* func_8003AA34();
extern "C" void func_80208F34(void*, void*, void*, void*);
extern "C" void func_80209020(void*, void*, void*, void*);
extern "C" void func_80209288(void*, void*, void*, void*);
extern "C" const char* getBdatStringColumnValue(void*, const char*, int);
extern "C" u32 func_800822F4__Q22cf13CfGameManagerFv();
extern "C" void func_8020A03C();
extern "C" void func_8020A0CC();
extern "C" void* func_800B6BC8();
extern "C" void func_8008B95C(void*);

// External data (retail linker symbols)
// State dispatch table (jumptable_eu_80535830, 5 entries).
extern "C" int (*jumptable_eu_80535830[])(void*, void*, void*);

namespace cf {

class CfGimmickSaveOff {
public:
    CfGimmickSaveOff(s32 param);
    virtual ~CfGimmickSaveOff();          // vtable index 0
    virtual void virt01();                // index 1
    virtual void virt02();                // index 2
    virtual void virt03();                // index 3
    virtual void virt04();                // index 4
    virtual void virt05();                // index 5
    virtual void vinit();                 // dispatch offset 0x20 (slot 8)
    u8 _04[0x5C];       // 0x04-0x5F: CfGimmick base class fields
    /* 0x60 */ u32 mState;      // 0x60: jump table index
    /* 0x64 */ u16 mParam;      // 0x64: saved constructor param
    /* 0x66 */ u8 _66[6];       // 0x66-0x6B
    /* 0x6C */ u16 mMinVal;     // 0x6C: lower bound
    /* 0x6E */ u16 mMaxVal;     // 0x6E: upper bound
    /* 0x70 */ u8 _70[0x12];    // 0x70-0x81
    /* 0x82 */ u16 mFlag;       // 0x82: = 8
    /* 0x84 */ u32 mType;       // 0x84: type (byte-extended)
};

} // namespace cf
