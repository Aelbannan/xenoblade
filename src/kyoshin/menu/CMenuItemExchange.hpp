#pragma once

#include <types.h>

#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CItemBoxGrid.hpp"

class CItemBoxLine;
struct CBgTex;

/**
 * OC (object-component) interface embedded at offset 0x6c within
 * CMenuItemExchange. Used for virtual dispatch through the OC vtable;
 * thunks adjust back to the containing object before delegating.
 */
struct CMenuItemExchangeOC {
    virtual ~CMenuItemExchangeOC() {}
};

/**
 * Menu item exchange / equipment screen controller.
 *
 * MI layout (explicit CProcess-style raw region; IScnRender + OC are
 * sub-object vtable slots, not C++ bases, matching the retail layout):
 *   0x00 CProcess header (link nodes + vtable + flags)
 *   0x3C ptmf (Move hook), 0x48 ptmf (Draw hook)
 *   0x58 IScnRender dispatch vtable ptr
 *   0x64 u8 field_64
 *   0x6C CMenuItemExchangeOC sub-object vptr
 *   0x98 CTitleAHelp mTitleAHelp (0x38 bytes)
 *   0xD0 CItemBoxLine storage (opaque, 0x63C bytes)
 *   0x70C CItemBoxGrid mItemBoxGrid
 *   0x5118 u8 field_5118
 */
class CMenuItemExchange {
public:
    CMenuItemExchange();
    ~CMenuItemExchange();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    u8  mPad0[0x10];                 // 0x00 link-list nodes
    u32 mVtab;                       // 0x10 vtable ptr
    u8  mPad14[0x3C - 0x14];         // 0x14 rest of the CProcess header
    u32 mPtMf3C[3];                  // 0x3C Move hook ptmf (__ptmf_null)
    u32 mPtMf48[3];                  // 0x48 Draw hook ptmf (__ptmf_null)
    u8  mPad54[0x58 - 0x54];         // 0x54
    u32 mScnRenderVt;                // 0x58 IScnRender dispatch vtable
    u8  mPad5C[0x64 - 0x5C];         // 0x5C
    u8  field_64;                    // 0x64
    u8  mPad65[0x6C - 0x65];         // 0x65
    u32 mOCVt;                       // 0x6C OC sub-object vtable
    u8  mPad70[0x98 - 0x70];         // 0x70
    CTitleAHelp mTitleAHelp;         // 0x98 (sizeof 0x38) -> 0xD0
    u8  mItemBoxLine[0x63C];         // 0xD0 opaque CItemBoxLine storage -> 0x70C
    CItemBoxGrid mItemBoxGrid;       // 0x70C (vptr sub-object)
    u8  mPadGrid[0x5118 - 0x70C - sizeof(CItemBoxGrid)]; // 0x710
    u8  field_5118;                  // 0x5118
};

// CBgTex field-by-field copy helper (retail func_ name, C++ linkage; the
// symbol map resolves the mangled form back to the unmangled func_801BE108).
CBgTex* func_801BE108(CBgTex* dest, CBgTex* src);

// Retail-unmangled C-linkage callees (US strips the member manglings for the
// func_ helpers; isIdle keeps its C++ mangling). int returns keep the caller's
// `!= 0` as a plain cmpwi (no u8 mask) like retail.
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* self);
extern "C" int func_801ED800(CItemBoxLine* self);
extern "C" int GetField61(CItemBoxGrid* self);

// 0x10-byte region copy helper used by the CBgTex copy routine (and the other
// menu widget copy ctors); retail keeps the unmangled name.
extern "C" void __ct__UnkClass_8011C974(void* dest, void* src);
