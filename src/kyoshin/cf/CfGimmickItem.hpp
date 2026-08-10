#pragma once

/**
 * @file CfGimmickItem.hpp
 * @brief cf::CfGimmickItem -- item gimmick entity class.
 *
 * CfGimmickItem extends CfGimmick to represent items that can be
 * picked up / collected in the game world (recovery items, key items,
 * etc.). Fields are populated from BDAT columns at construction.
 *
 * Like the other gimmick variants (CfGimmickElv/CfGimmickSaveOff) the class
 * carries a MANUAL vtable pointer at +0x00 (there is no C++ inheritance --
 * the shared CfGimmick base layout is replicated inline) so the retail
 * vtable `lbl_eu_80535A98` is stored verbatim instead of a compiler
 * generated vtable.
 */

#include "kyoshin/cf/CfGimmick.hpp"
#include "types.h"

namespace cf {

class CfGimmickItem {
public:
    ~CfGimmickItem();

    /* 0x00 */ void* vtable;             // manual vptr (lbl_eu_80535A98)
    /* 0x04 */ CfGimmickVec3 vvec04;     // base placement/sub-object 3-vector
    /* 0x10 */ u8 pad10[0x0C];
    /* 0x1C */ void* vobj;               // base sub-object slot
    /* 0x20 */ u8 pad20[0x10];
    /* 0x30 */ f32 field_30;
    /* 0x34 */ f32 field_34;
    /* 0x38 */ f32 field_38;
    /* 0x3C */ f32 field_3C;
    /* 0x40 */ f32 field_40;
    /* 0x44 */ s32 field_44;
    /* 0x48 */ u8 pad48[0x18];           // 0x48-0x5F
    /* 0x60 */ s32 mType;                // jumptable_eu_80535830 dispatch index
    /* 0x64 */ u16 field_64;
    /* 0x66 */ u16 field_66;             // bit0 toggled state
    /* 0x68 */ u16 field_68;
    /* 0x6A */ u16 field_6A;
    /* 0x6C */ u16 field_6C;
    /* 0x6E */ u16 field_6E;
    /* 0x70 */ u8 field_70;
    /* 0x71 */ u8 field_71;
    /* 0x72 */ u8 field_72;
    /* 0x73 */ u8 field_73;
    /* 0x74 */ u32 field_74;             // state flag word (0x10 / 0x20 bits)
    /* 0x78 */ u32 field_78;
    /* 0x7C */ CfGimmickReg field_7C;    // gimmick-object registration slot
    /* 0x80 */ u16 field_80;
    /* 0x82 */ u8 pad82[2];
    /* 0x84 */ u16 field_84[3];          // per-slot item ids
    /* 0x8A */ u16 field_8A;
    /* 0x8C */ u8 pad8C[2];
    /* 0x8E */ u16 field_8E;             // effect id triggered while working (func_80208C48)
    /* 0x90 */ u8 pad90[4];
    /* 0x94 */ u16 field_94;             // resource id passed to func_80082354
    /* 0x96 */ u8 field_96;              // lower bound of the respawn-count window
    /* 0x97 */ u8 field_97;              // upper bound of the respawn-count window
    /* 0x98 */ u8 pad98[4];
    /* 0x9C */ u8 field_9C;              // mode byte (2 or 3 => state 5)
    /* 0x9E */ u16 field_9E;             // current state (PTMF table index)
};

// Pointer-to-member state table indexed by field_9E (12-byte entries).
typedef void (CfGimmickItem::*CfGimmickItemState)();

} // namespace cf

// ---------------------------------------------------------------------------
// C-linkage imports (unmangled retail symbols used by this TU).  These are
// CfGimmick-family helpers / item engine calls defined in other units; they
// have C linkage in retail so extern "C" is required to emit the raw name.
// ---------------------------------------------------------------------------
extern "C" {
void __dt__Q22cf9CfGimmickFv(void* self, int mode);
void func_80208EE4(void* self);
void func_8020A434(void* self);                     // unregister field_7C object
void func_8020A484(void);
unsigned int func_8020A5DC(void);
void func_80209F2C(void);
void func_8020A6B0(void* reg, const CfGimmickVec3* point,
                   unsigned short c, float d, int e, int g);
void func_80159C04(unsigned int a, int b);
void func_801586D4(unsigned int a, unsigned int b);
void func_8020974C(unsigned int a, int b);
int func_80209754(unsigned short flag, void* a, void* b, void* c,
                  unsigned int d);
void func_80208C48(void* self, void* arg);
unsigned int func_800822F4__Q22cf13CfGameManagerFv(void);
unsigned int func_80082354__Q22cf13CfGameManagerFv(unsigned int a);
unsigned int func_801587E8(unsigned short id);
}

// Data symbols (global-scope objects, not mangled by MWCC).
extern f32 lbl_eu_8066844C;            // spawn distance constant (sdata2)
extern u8 lbl_eu_80535A98[];           // CfGimmickItem vtable (0x24)
extern cf::CfGimmickItemState lbl_eu_80535A50[];  // 6-entry PTMF state table