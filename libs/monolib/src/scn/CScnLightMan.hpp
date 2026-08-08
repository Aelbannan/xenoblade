#pragma once

#include <types.h>

// A node in the light-manager's circular linked list.
// +0x00: next link, +0x08: the item.
struct CScnLightNode {
    CScnLightNode* next;           // +0x00
    u32 _04;                       // +0x04
    struct CScnItemLight* item;    // +0x08
};

// List handle returned by the scene-item-pool accessor (func_8048C698).
// +0x04 holds the circular-list sentinel.
struct CScnLightList {
    u32 _00;                       // +0x00
    CScnLightNode* sentinel;       // +0x04
};

// A light item walked by the constructor and consulted by func_8048D160.
// id10 is a signed id; the flag at +0x121c gates func_8048D160.
struct CScnItemLight {
    u8 _00[0x10];
    s16 id10;          // +0x10
    u8 _12[2];         // +0x12
    u8 _14[0x1208];    // +0x14
    u8 flag121C;       // +0x121c
};

// Parameter object for the light manager; pool handle at +0x60.
struct CScnLightParam {
    u8 _00[0x60];                 // +0x00
    void* mPool;                  // +0x60
};

// Scene light manager: selects an active light item from the scene pool.
class __declspec(novtable) CScnLightMan {
public:
    CScnLightMan(CScnLightParam* param);
    virtual ~CScnLightMan();

    // +0x04
    CScnLightParam* mParam;    // +0x04
    CScnItemLight* mLight;     // +0x08
};