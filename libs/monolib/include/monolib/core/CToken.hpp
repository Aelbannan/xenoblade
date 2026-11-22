#pragma once

#include <types.h>

class CToken {
public:
    CToken();
    virtual ~CToken();

    u32 parseHexToken();
    void func_8043AA1C();
    void clear();
    void findTokenName(char* pOutBuffer, int bufferLen, const char* pName);
    char* extractWordAt(char* pInput, int tokenIndex);
    bool matchKeyWords(const char* pInput, const char* mode);

    //0x0: vtable
    u8 field_0x4[0x1100];     //0x4
    u32 m_data[16];           //0x1104
    u16 field_0x1144[16];     //0x1144
    u16 m_fieldCount;         //0x1164
};
