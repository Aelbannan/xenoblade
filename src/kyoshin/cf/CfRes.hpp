#pragma once

#include <types.h>
struct CfResBuffer {
    u8 buffer[0x400];
    u32 field_400;
    u32 field_404;

    int func_80061A80(unsigned char byte1, unsigned short halfword, unsigned int dataVal, unsigned int* src, int count, unsigned int headerBits);
};

struct CfRes_64994 {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u8 _0C[0x14];
    u32 field_20;
    u32 field_24;
    u32 field_28;

    void initStruct();
};

struct CfRes_65818 {
    u32 field_00;
    int field_04;

    int decRefCount();
};

namespace cf {

class CfRes {
public:
    CfRes();
    ~CfRes();
    void OnFileEvent();
};

class CfResTask {
public:
    virtual ~CfResTask();
    void Term();

    // TODO: add fields
    void Init();
    void Move();
    void Draw();
};
} // namespace cf

