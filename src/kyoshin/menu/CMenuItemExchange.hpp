#pragma once

#include <types.h>

#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CItemBoxGrid.hpp"

class CItemBoxLine;
class CScn;
class IScnRender;

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
 *   0x70 CScn* mScene (owning scene, addRenderCB target in Init)
 *   0x74 u32 mField74 (u16 arg passed to the item-box ctors)
 *   0x78 CBgTex mBgTex (0x20 bytes)
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
    CScn* mScene;                    // 0x70 owning scene
    u32 mField74;                    // 0x74 (u16 arg passed to item-box ctors)
    CBgTex mBgTex;                   // 0x78 background layout widget (0x20)
    CTitleAHelp mTitleAHelp;         // 0x98 (sizeof 0x38) -> 0xD0
    u8  mItemBoxLine[0x63C];         // 0xD0 opaque CItemBoxLine storage -> 0x70C
    CItemBoxGrid mItemBoxGrid;       // 0x70C (vptr sub-object)
    u8  mPadGrid[0x5118 - 0x70C - sizeof(CItemBoxGrid)]; // 0x710
    u8  field_5118;                  // 0x5118
    u8  mPad5119[0x511C - 0x5119];   // 0x5119..0x511B
    f32 mFloat511C;                  // 0x511C (input-handler timer)
    f32 mFloat5120;                  // 0x5120 (state-4 fade float)
};

// Minimal CScn view exposing the render-callback registration used by Init
// (retail mangled name addRenderCB__4CScnFP10IScnRenderUlUl; defined in the
// monolib/scn CScn TU).
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
};

// cf::CfPadData view: only the flag words the menu input handlers read
// (+0x00 held, +0x04 pressed, +0x104 turbo/short-press).
struct CMIExPadData {
    u32 mHeldFlags;                  // +0x00
    u32 mPressedFlags;               // +0x04
    u8 _pad08[0x104 - 0x08];         // 0x08..0x103
    u32 mTurboFlags;                 // +0x104
};

// Retail-unmangled callee names (US strips the member manglings for the
// func_ helpers; isIdle keeps its C++ mangling). int returns keep the caller's
// `!= 0` as a plain cmpwi (no u8 mask) like retail. These names ARE the
// retail linker symbols - do not rename or "fix" the mangled forms.
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* self);
extern "C" int func_801ED800(CItemBoxLine* self);
extern "C" int GetField61(CItemBoxGrid* self);

// 0x10-byte region copy helper used by the CBgTex copy routine (and the other
// menu widget copy ctors); retail keeps the unmangled name.
extern "C" void __ct__UnkClass_8011C974(void* dest, void* src);

// CBgTex field-by-field copy helper (retail func_ name; symbol map resolves
// the C-linkage name back to the unmangled func_801BE108).
extern "C" CBgTex* func_801BE108(CBgTex* dest, CBgTex* src);
// CTitleAHelp field-by-field copy helper (retail func_ name).
extern "C" void func_801BE16C(CTitleAHelp* dest, CTitleAHelp* src);
// CItemBoxLine body copy helper (retail func_ name).
extern "C" void func_801BE208(CItemBoxLine* dest, CItemBoxLine* src);
// CItemBoxGrid body copy helper (retail func_ name).
extern "C" void func_801BE590(CItemBoxGrid* dest, CItemBoxGrid* src);

// Subobject ctors/dtors/load hooks (retail keeps the unmangled ctor names for
// these menu widgets; the dtors keep their C++ manglings).
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __dt__6CBgTexFv(CBgTex* self, int flags);
extern "C" bool func_801C3C14(CBgTex* self);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);
extern "C" void func_801C414C(CTitleAHelp* self);
extern "C" void func_801C4198(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
extern "C" void __ct__CItemBoxLine(CItemBoxLine* self, u32 a, u32 b);
extern "C" void __dt__12CItemBoxLineFv(CItemBoxLine* self, int flags);
extern "C" void func_801EDA08(CItemBoxLine* self);
extern "C" void func_801EDA4C(CItemBoxLine* self, u8 val);
extern "C" void func_801ED31C(CItemBoxLine* self);
extern "C" void __ct__CItemBoxGrid(CItemBoxGrid* self, u32 a, u32 b, u32 c, u32 d);
extern "C" void __dt__12CItemBoxGridFv(CItemBoxGrid* self, int flags);
extern "C" void func_801CB480(CItemBoxGrid* self);
extern "C" void PushToList(CItemBoxGrid* self, u8 val);

// Input-handler callees (retail unmangled names). func_80086F9C keeps the
// u32 return declared by CItemBoxGrid.hpp (included above) - do not redeclare.
extern "C" CMIExPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u8 enable);
extern "C" int func_8029A658();
extern "C" int func_801ED808(CItemBoxLine* self);
extern "C" void func_801EECE0(CItemBoxLine* self, u32 arg);
extern "C" void func_801EECE8(CItemBoxLine* self);
extern "C" void func_801EED6C(CItemBoxLine* self);
extern "C" void func_801EE788(CItemBoxLine* self);
extern "C" int func_801EECC0(CItemBoxLine* self);
extern "C" void func_801ED97C(CItemBoxLine* self);
extern "C" void func_801EE684(CItemBoxLine* self);
extern "C" void func_801EDC94(CItemBoxLine* self);
extern "C" void func_801EDF40(CItemBoxLine* self);
extern "C" void func_801EE228(CItemBoxLine* self);
extern "C" void func_801EE448(CItemBoxLine* self);
extern "C" void func_801EEDF8(CItemBoxLine* self);
extern "C" void func_801EF050(CItemBoxLine* self);
extern "C" u8 func_801EF0EC(CItemBoxLine* self);
extern "C" int func_801CCAF0(CItemBoxGrid* self);
extern "C" int func_801CB0FC(CItemBoxGrid* self);
extern "C" void func_801CC7B0(CItemBoxGrid* self, int arg);
extern "C" int func_801CDBE0(CItemBoxGrid* self);
extern "C" int func_801CB1E4(CItemBoxGrid* self);
extern "C" void func_801CB38C(CItemBoxGrid* self);
extern "C" void func_801CB5F0(CItemBoxGrid* self);
extern "C" void func_801CBA04(CItemBoxGrid* self);
extern "C" void func_801CBDE8(CItemBoxGrid* self);
extern "C" void func_801CC0EC(CItemBoxGrid* self);
extern "C" void func_801CC5DC(CItemBoxGrid* self);
extern "C" void func_801CDEE8(CItemBoxGrid* self);
extern "C" u8 func_801CDFB4(CItemBoxGrid* self);
extern "C" int func_801CB184(CItemBoxGrid* self);
extern "C" int GetField52D(CItemBoxGrid* self);

// Shared data imports (MWCC does not mangle global-scope data names).
extern char lbl_eu_80505324[];   // split1 .rodata string pool (Init title name)
extern u8 lbl_eu_8066442C;       // .sbss (item-box line tab mode latch)
extern const f32 lbl_eu_80667E78; // .sdata2 (fade-out/state float)
extern const f32 lbl_eu_80667E7C; // .sdata2 (timer increment)
extern const f32 lbl_eu_80667E80; // .sdata2 (timer clamp)
