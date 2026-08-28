#pragma once

#include <types.h>

struct WarpVec3 {
    f32 x; f32 y; f32 z;
};
struct WarpVec4 {
    f32 x; f32 y; f32 z; f32 w;
};
struct WarpObject;

namespace cf {

// Retail __vt__ for CfGimmickWarp is lbl_eu_805359B0 (size 0x24: RTTI + 0 + dtor + 6 virtuals).
// The class is novtable: the ctor writes the retail label explicitly (like CHelp / CToken),
// so the compiler does not emit its own __vt. Hierarchy lbl_eu_805359D4 -> CfGimmick.
class __declspec(novtable) CfGimmickWarp {
public:
    virtual ~CfGimmickWarp();                        // +0x08 dtor
    virtual void unk_0C();                           // +0x0C  func_8020896C / func_8020D7BC
    virtual void unk_10();                           // +0x10  func_801F4994 / func_8020D6FC
    virtual UNKWORD unk_14();                        // +0x14  stub func_801F4B64
    virtual UNKWORD unk_18();                        // +0x18  stub func_801F4BF8
    virtual UNKWORD unk_1C();                        // +0x1C  stub func_801F4C8C
    virtual void unk_20();                           // +0x20  func_80208988 / func_8020D7DC

    CfGimmickWarp();
    // Overlay on vptr at +0x00 so ctor can install retail table.
    void**& vtbl() { return *reinterpret_cast<void***>(this); }

    // Layout mirrors WarpData / CfGimmick base (implicit vptr at +0x00, set via vtbl() = lbl_eu_805359B0).
    /* 0x04 */ WarpVec3 position;                    // 0x04
    /* 0x10 */ WarpVec3 rotation;                    // 0x10
    /* 0x1C */ WarpVec3 scale;                       // 0x1c
    /* 0x28 */ u8 basePad28[0x38];                   // 0x28
    /* 0x60 */ u32 stateIndex;                       // 0x60 (dispatchIndex alias for jump table)
    /* 0x64 */ u16 rowId;                            // 0x64 CfGimmick field_64
    /* 0x66 */ u16 configFlags;                      // 0x66 field_66
    /* 0x68 */ u16 unk68;                            // 0x68 field_68
    /* 0x6A */ u16 resourceId;                       // 0x6a field_6A
    /* 0x6C */ u16 minValue;                         // 0x6c field_6C
    /* 0x6E */ u16 maxValue;                         // 0x6e field_6E
    /* 0x70 */ u8 lod[4];                            // 0x70
    /* 0x74 */ u32 flags;                            // 0x74 field_74
    /* 0x78 */ ::WarpObject* object7c;             // 0x78 field_78 (WarpObject* in warp)
    /* 0x7C */ ::WarpObject* object7cState;          // 0x7c field_7C
    /* 0x80 */ u16 soundHandle;                      // 0x80 field_80
    /* 0x82 */ u16 typeId;                           // 0x82
    /* 0x84 */ WarpVec3 destination;                 // 0x84
    /* 0x90 */ WarpVec3 destination2;                // 0x90
    /* 0x9C */ WarpVec3 destination3;                // 0x9c
    /* 0xA8 */ u8 derivedPadA8[0x38];                // 0xa8
    /* 0xE0 */ u32 dispatchIndex;                    // 0xe0
    /* 0xE4 */ u16 unkE4;                            // 0xe4
    /* 0xE6 */ u8 flagE6;                            // 0xe6
    /* 0xE7 */ u8 flagE7;                            // 0xe7
    /* 0xE8 */ u16 unkE8;                            // 0xe8
    /* 0xEA */ s16 unkEA;                            // 0xea
    /* 0xEC */ s16 unkEC;                            // 0xec
    /* 0xEE */ u16 unkEE;                            // 0xee
    /* 0xF0 */ u16 unkF0;                            // 0xf0
    /* 0xF2 */ u8 padF2[4];                          // 0xf2
    /* 0xF6 */ u16 state;                            // 0xf6
    /* 0xF8 */ u16 phase;                            // 0xf8
    /* 0xFC */ f32 timer;                            // 0xfc
    /* 0x100 */ ::WarpObject* object100;           // 0x100
    /* 0x104 */ ::WarpObject* object104;            // 0x104
    /* 0x108 */ ::WarpObject* object108;            // 0x108
};

} // namespace cf

extern "C" u8 lbl_eu_805359B0[];
// Hierarchy + RTTI (for reference): lbl_eu_805359D4 / lbl_eu_806627A8 ("cf::CfGimmickWarp" -> CfGimmick)