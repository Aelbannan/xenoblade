#pragma once

#include <types.h>

class __declspec(novtable) CToken {
public:
    CToken();
    virtual ~CToken();

    u32 func_8043A8D8();
    void func_8043AA1C();
    void func_8043AA68(char* pOutBuffer, int bufferLen, const char* pName);
    char* func_8043AB14(char* pInput, int tokenIndex);
    bool func_8043ABD4(const char* pInput, const char* mode);

    //0x0: vtable
    u8 field_0x4[0x1100];     //0x4
    u32 m_data[16];           //0x1104
    u16 field_0x1144[16];     //0x1144
    u16 m_fieldCount;         //0x1164
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

// Retail vtable data symbol (vtable lives in monolibdata1.s at 0x8056B52C).
// The class is __declspec(novtable), so the ctor assigns the retail label
// explicitly instead of the compiler-generated __vt__6CToken. Defined in
// CToken.cpp as [&lbl_eu_80663560, 0, &__dt__6CTokenFv] (3 words).
extern "C" u32 lbl_eu_8056B52C[3];
