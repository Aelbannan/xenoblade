#pragma once

#include <types.h>

namespace cf {

class CfGameManager;

class CfGimmick {
public:
    virtual ~CfGimmick();
    CfGimmick();

    // 0x00 vtable (implicit - first member is virtual dtor)

    /* 0x04 */ u8 gap04[0x30 - 0x04];
    /* 0x30 */ f32 field_30;   // 0x30 - horizontal radius / half-extent
    /* 0x34 */ f32 field_34;   // 0x34 - vertical extent (low)
    /* 0x38 */ f32 field_38;   // 0x38 - vertical extent (high)
    /* 0x3C */ f32 field_3C;   // 0x3C - depth half-extent
    /* 0x40 */ f32 field_40;   // 0x40 - interaction distance
    /* 0x44 */ u8 gap44[0x64 - 0x44];
    /* 0x64 */ u16 field_64;
    /* 0x66 */ u16 field_66;
    /* 0x68 */ u16 field_68;
    /* 0x6A */ u16 field_6A;
    /* 0x6C */ u16 field_6C;
    /* 0x6E */ u16 field_6E;
    /* 0x70 */ u8 field_70;
    /* 0x71 */ u8 field_71;
    /* 0x72 */ u8 field_72;
    /* 0x73 */ u8 field_73;
    /* 0x74 */ u32 field_74;
    /* 0x78 */ CfGameManager* field_78;
    /* 0x7C */ u32 field_7C;
    /* 0x80 */ u16 field_80;

    void func_8020896C(void* other);
    void func_80208988();
    int func_8020A8AC();
};

} // namespace cf

// Global settings object returned by getUnk80664658 (field_214 flag word).
struct CfGimmickGlobal {
    u8 pad[0x210];
    u32 field_210;   // 0x210 - value set by func_8020A068
    u32 field_214;   // 0x214 - flag bits set by several setters
};

// Minimal 3-component vector used by the CfGimmick collision helpers.
struct CfGimmickVec3 {
    f32 x;  // 0x00
    f32 y;  // 0x04
    f32 z;  // 0x08
};

// Container whose first member (0x00) is a registered-object pointer used by
// func_8020A434 to unregister from the global resource manager.
struct CfGimmickReg {
    void* field_00;
};