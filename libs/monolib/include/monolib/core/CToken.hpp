#pragma once

#include <types.h>

class CToken {
public:
    CToken();
    virtual ~CToken();

    u32 func_8043A8D8();
    void func_8043AA1C();
    void func_8043AA68(char* pOutBuffer, const char* pName, int bufferLen);
    void func_8043AB14();
    void func_8043ABD4();

    //0x0: vtable
    u8 field_0x4[0x1100];     //0x4
    u32 m_data[16];           //0x1104
    u8 field_0x1144[0x1E];    //0x1144
    u16 m_fieldCount;         //0x1164
};
