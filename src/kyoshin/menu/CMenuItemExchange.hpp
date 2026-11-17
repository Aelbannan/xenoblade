#pragma once

#include "kyoshin/CItemBoxLine.hpp"

#include <types.h>

#include <monolib/util/FixStr.hpp>

#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CItemBoxGrid.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

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
    u32 field_5C;                    // 0x5C
    s32 field_60;                    // 0x60 (ctor init -1)
    u8  field_64;                    // 0x64
    u8  field_65;                    // 0x65
    u8  field_66;                    // 0x66
    u8  field_67;                    // 0x67 (ctor init 1)
    u32 field_68;                    // 0x68
    u32 mOCVt;                       // 0x6C OC sub-object vtable
    CScn* mScene;                    // 0x70 owning scene
    u32 mField74;                    // 0x74 (u16 arg passed to item-box ctors)
    CBgTex mBgTex;                   // 0x78 background layout widget (0x20)
    CTitleAHelp mTitleAHelp;         // 0x98 (sizeof 0x38) -> 0xD0
    CItemBoxLine mItemBoxLine;       // 0xD0 -> 0x70C
    CItemBoxGrid mItemBoxGrid;       // 0x70C (vptr sub-object)
    u8  mPadGrid[0x5118 - 0x70C - sizeof(CItemBoxGrid)]; // 0x710
    u8  field_5118;                  // 0x5118
    u8  mPad5119[0x511C - 0x5119];   // 0x5119..0x511B
    f32 mFloat511C;                  // 0x511C (input-handler timer)
    f32 mFloat5120;                  // 0x5120 (state-4 fade float)
};

#include "monolib/scn/CScn.hpp"

/* Layout view of the CItemBoxLine body, used only by the copy helper
 * ItemEx_CopyLine (the full class lives in CItemBoxLine.hpp, which cannot be
 * included here: its CSysWin.hpp dependency redeclares
 * __dt__17UnkClass_8045F564Fv with a typed pointer that clashes with
 * CItemBoxGrid.hpp's void* form). Offsets match the retail class exactly. */
struct CItemBoxLineView {
    u8* mVtbl;                       // 0x00
    u8 region04[0x10];               // 0x04..0x13
    u8 region14[0x10];               // 0x14..0x23
    u32 field24;                     // 0x24
    u32 field28;                     // 0x28
    u32 field2C;                     // 0x2C
    u32 field30;                     // 0x30
    u8* field34;                     // 0x34
    u8* field38;                     // 0x38
    u8* field3C;                     // 0x3C
    u8* field40;                     // 0x40
    u8* field44;                     // 0x44
    u8* field48;                     // 0x48
    u8 field4C;                      // 0x4C
    u8 pad4D[3];
    int field50;                     // 0x50
    u8* field54;                     // 0x54
    u8 field58;                      // 0x58
    u8 unk59;                        // 0x59
    u8 tabEntries[9];                // 0x5A..0x62
    u8 tabCount;                     // 0x63
    u8 unk64[9];                     // 0x64..0x6C
    u8 field6D;                      // 0x6D
    u8 pad6E[2];
    u8 cur70[0x18];                  // 0x70 embedded cursor
    u8 cur88[0x18];                  // 0x88 embedded cursor
    u8 curA0[0x18];                  // 0xA0 embedded cursor
    u8 curB8[0x18];                  // 0xB8 embedded cursor
    u8 info2D0[0x20C];               // 0xD0..0x2DB item-box-info2 state
    u8 numSel2DC[0x2F];              // 0x2DC num-select
    u8 pad30B[5];
    u8 scrollBar310[0x40];           // 0x310 scroll bar
    u8 sysWin350[0x3C];              // 0x350 sys window
    s16 unk38C;                      // 0x38C
    s16 unk38E;                      // 0x38E
    u8 field390;                     // 0x390
    u8 pad391;                       // 0x391
    s16 field392;                    // 0x392
    u16 field394;                    // 0x394
    struct { s16 r, g, b, a; } field396; // 0x396 nameplate colour
    u8 unk39E;                       // 0x39E
    u8 field39F;                     // 0x39F
    u8 field3A0;                     // 0x3A0
    u8 field3A1;                     // 0x3A1
    u8 field3A2;                     // 0x3A2
    u8 field3A3;                     // 0x3A3
    struct CIBLTabEntryView {        // 0x3A4 tab entries (12-byte stride)
        u16 f0;
        u32 f4;
        u8 f8;
        u8 f9;
    } entries[12];
    u16 tabCount2;                   // 0x434
    u8 field92;                      // 0x436
    u8 field93;                      // 0x437
    ml::FixStr<32> str438;           // 0x438 tab name
    ml::FixStr<128> str45C;          // 0x45C second tab name
    u16 pageWords4E0[12];            // 0x4E0
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

// 0x10-byte region copy helper used by the CBgTex copy routine (and the other
// menu widget copy ctors); retail keeps the unmangled name.
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dest, void* src);

// CBgTex field-by-field copy helper (retail func_ name; symbol map resolves
// the C-linkage name back to the unmangled ItemEx_CopyBgTex).
extern "C" CBgTex* ItemEx_CopyBgTex(CBgTex* dest, CBgTex* src);
// CTitleAHelp field-by-field copy helper (retail func_ name).
extern "C" CTitleAHelp* ItemEx_CopyTitle(CTitleAHelp* dest, CTitleAHelp* src);
// CItemBoxLine body copy helper (retail func_ name).
extern "C" void ItemEx_CopyLine(CItemBoxLine* dest, CItemBoxLine* src);
// CItemBoxGrid body copy helper (retail func_ name).
extern "C" void ItemEx_CopyGrid(CItemBoxGrid* dest, CItemBoxGrid* src);

// Subobject ctors/dtors/load hooks (retail keeps the unmangled ctor names for
// these menu widgets; the dtors keep their C++ manglings).
extern "C" void __ct__6CBgTexFUc(CBgTex* self, u8 arg);
extern "C" void __dt__6CBgTexFv(CBgTex* self, int flags);
extern "C" bool BgTex_Acquire_3C14(CBgTex* self);
extern "C" void __ct__11CTitleAHelpFPcUc(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" void beginClose(CTitleAHelp* self);
extern "C" void markReplayClose(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
extern "C" void __dt__12CItemBoxLineFv(CItemBoxLine* self, int flags);
extern "C" void __ct__12CItemBoxGridFUcUsUlUc(CItemBoxGrid* self, u32 a, u32 b, u32 c, u32 d);
extern "C" void __dt__12CItemBoxGridFv(CItemBoxGrid* self, int flags);

// Load/anim hooks used by the phase gates (retail unmangled names).
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);            // bg layout load-done query
extern "C" void reopenFromClose(CTitleAHelp* self);      // stop help-bar animation
extern "C" int isInitialized(CTitleAHelp* self);       // help-bar ready query
extern "C" void func_801C412C(CTitleAHelp* self);      // hide prompt

// Factory support (retail unmangled symbols; sibling-menu-TU scheme).
extern "C" void __ct__8CProcessFv(CMenuItemExchange* _this);
extern u32 lbl_eu_8052D238;   // temp (CProcess) vtable (.data)
extern u32 lbl_eu_80533BC0;   // composite vtable (.data)
extern u32 __ptmf_null[3];    // null pointer-to-member-function triple
class CProcess;
extern "C" void* getWorkMem__17CWorkThreadSystemFv();
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 heap);
extern "C" void Regist__8CProcessFP8CProcessb(void* _this, void* parent, bool insertTop);
extern "C" CMenuItemExchange* __ct__CMenuItemExchange(CMenuItemExchange* self, CScn* scene, u32 arg);

// cbRenderBefore gates / draw pipeline (retail verbatim names).
namespace nw4r { namespace lyt { class DrawInfo; } }
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* di);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* di);
extern u32 lbl_eu_80663E28;   // cbRenderBefore mode-flag word (.sbss)

// Input-handler callees (retail unmangled names). isClassicController keeps the
// u32 return declared by CItemBoxGrid.hpp (included above) - do not redeclare.
extern "C" CMIExPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8 enable);
extern "C" int MenuTutorialIsCreated();

// Move()/phase-handler callees (retail unmangled names).
extern "C" void BgTex_Tick_3D54(CBgTex* self);          // bg per-frame update
extern "C" void updateHelp(CTitleAHelp* self);     // help-bar per-frame update
extern "C" void InfoCfCopyObjD4(void*, void*);          // CNumSelectFull copy

// Shared data imports (MWCC does not mangle global-scope data names).
extern char lbl_eu_80505324[];   // split1 .rodata string pool (Init title name)
extern u8 lbl_eu_8066442C;       // .sbss (item-box line tab mode latch)
extern const f32 lbl_eu_80667E78; // .sdata2 (fade-out/state float)
extern const f32 lbl_eu_80667E7C; // .sdata2 (timer increment)
extern const f32 lbl_eu_80667E80; // .sdata2 (timer clamp)
