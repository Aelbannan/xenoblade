#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CKizunaTalkList.hpp"

class CPad;

// Pad accessors on cf::CfGameManager used by Move()/KizunaList_Phase2Input.
// Declared member-for-member here (full definition: kyoshin/cf/CfGameManager.hpp)
// because that header's lbl_eu_80664090 declaration conflicts with the one in
// CKizunaTalkList.hpp above; merge back once those types are reconciled.
namespace cf {
class CfPadData;
class CfGameManager {
public:
    static CPad* getCurrentPad();
    static CfPadData* getCfPadData();
};
} // namespace cf


// Shared string pool (.rodata) used by Init.
extern char lbl_eu_8050E970[];

// Global data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_80663E28;            // render-gate mode bitfield (.sbss; bit 21)
extern u32 __ptmf_null[3];             // null pointer-to-member-function constant
// CProcess vtable (written at +0x10 first, then overwritten by the
// CMenuKizunaTalkList composite vtable lbl_eu_80537CB8, whose IScnRender
// sub-vtable lives at +0x24).
extern char lbl_eu_8052BF70[];
extern char lbl_eu_80537CB8[];

// Forward decls so the in-class friends below bind the retail C-linkage
// symbols (no new C++-linkage decl is introduced by the friend):
class CMenuKizunaTalkList;
// Retail ctor symbol (unmangled global): the factory binds the bare reloc
// (same pattern as CMenuSelectShop.hpp).
extern "C" CMenuKizunaTalkList* __ct__CMenuKizunaTalkList(CMenuKizunaTalkList* self,
                                                           CProcess* parent);
// Retail beginClose symbol (unmangled global): free-call form so the reloc
// is bare (same pattern as CMenuShopBuy.hpp).
extern "C" void beginClose(CTitleAHelp* self);

/*
 * Kizuna (Friendship) Talk List screen process.
 *
 * Layout (constructor/Init):
 *   0x00: CProcess          -- task-system base (0x3C) + vtable PMF data
 *   0x58: IScnRender        -- render-callback interface
 *   0x5C: CProcess*         -- parent process reference
 *   0x60: CBgTex            -- background layout widget (0x20 bytes)
 *   0x80: CTitleAHelp       -- title/help bar (0x38 bytes)
 *   0xB8: CKizunaTalkList   -- core talk-list data/logic (0x1494 bytes)
 *  0x154C: u8               -- phase/state byte
 */
class CMenuKizunaTalkList : public CProcess {
public:
    CMenuKizunaTalkList();
    virtual ~CMenuKizunaTalkList();

    // CProcess overrides
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void KizunaList_ThunkRender58();
    void KizunaList_ThunkDtor58();

private:
    friend CMenuKizunaTalkList* __ct__CMenuKizunaTalkList(CMenuKizunaTalkList* self,
                                                         CProcess* parent);
    friend void KizunaList_Phase2Input(CMenuKizunaTalkList* self);
    friend void KizunaList_Phase1Ready(CMenuKizunaTalkList* self);
    friend void KizunaList_Phase3Flag(CMenuKizunaTalkList* self);
    friend void KizunaList_Phase0Show(CMenuKizunaTalkList* self);

    u32 ptmf0[3];               // 0x3C-0x47: vtable pointer-to-member-function data
    u32 ptmf1[3];               // 0x48-0x53: vtable pointer-to-member-function data
    u8 mUnknown54;              // 0x54 - phase flag (written by KizunaList_Phase3Flag)
    u8 mUnknown55;              // 0x55
    u8 _pad56[2];               // 0x56-0x57: padding
    IScnRender mIScnRender;     // 0x58
    CProcess* mParentRef;       // 0x5C
    CBgTex mBgTex;              // 0x60
    CTitleAHelp mTitleAHelp;    // 0x80
    CKizunaTalkList mKizunaTalkList; // 0xB8
    u8 mState;                  // 0x154C
};