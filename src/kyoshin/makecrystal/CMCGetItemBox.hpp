#pragma once

#include <types.h>

class CMCGetItemBox {
public:
    CMCGetItemBox();
    virtual ~CMCGetItemBox();
    void OnFileEvent();

    // +0x00: vtable
    u8 _pad_04[0x55 - 0x04]; // 0x04-0x54
    u8 mField55;             // 0x55
    u8 _pad_56[0x303 - 0x56];// 0x56-0x302
    u8 mField303;            // 0x303
};
