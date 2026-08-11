#pragma once

#include <types.h>

#include "kyoshin/CSysWin.hpp"
#include "kyoshin/cf/CtrlObjectParam.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>

class CProcess;
class CScn;
class IScnRender;
class UnkClass_8045F564;

/*
 * Game-clear menu screen process (singleton, built by the __ct__CMenuGameClear
 * factory).
 *
 * Manual CProcess-style layout (NOT a C++ subclass - the vtable is stored at
 * +0x10 by the factory):
 *   0x00  compiler vptr (never written; novtable)
 *   0x04  CDoubleListNode
 *   0x10  CProcess vtable (set by factory: temp CProcess vtable, then the
 *         composite CMenuGameClear vtable lbl_eu_8053AE20)
 *   0x3C  null pointer-to-member-function triple (2x)
 *   0x54/0x55  state bytes
 *   0x58  IScnRender vtable slot (lbl_eu_8053AE20 + 0x24) - render callback
 *   0x5C  owning CScn (stored by factory, used by Term removeRenderCB)
 *   0x60  UnkClass_8045F564 scratch region (0x10)
 *   0x70/0x71  state bytes
 *   0x74  CSysWin storage (0x3C)
 *   0xB0  CCur18 cursor storage (0x18)
 *   0x12338  cf::CtrlObjectParamInit entries (14 x 0xCC = 0xB28)
 *   size 0x12E60
 */
class __declspec(novtable) CMenuGameClear {
public:
    virtual ~CMenuGameClear();
    CMenuGameClear();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x04-0x0F: CDoubleListNode
    u8 _04[0x10 - 0x04];            // 0x04..0x0F
    u32 mVtable10;                  // 0x10: CProcess vtable (set in factory)
    u8 _14[0x3C - 0x14];            // 0x14..0x3B: rest of CProcess
    u32 mPtMf3C[3];                 // 0x3C..0x47: null PMF copy
    u32 mPtMf48[3];                 // 0x48..0x53: null PMF copy
    u8 mField54;                    // 0x54
    u8 mField55;                    // 0x55
    u8 _56[0x58 - 0x56];            // 0x56..0x57
    u32 mIScnRenderVtbl;            // 0x58: IScnRender vtable slot
    CScn* mScene;                   // 0x5C: owning scene
    u8 mMemRegion[0x10];            // 0x60: UnkClass_8045F564 storage
    u8 mField70;                    // 0x70
    u8 mField71;                    // 0x71
    u8 _72[0x74 - 0x72];            // 0x72..0x73
    u8 mSysWin[0x3C];               // 0x74: CSysWin storage (0x3C bytes)
    u8 mCursor[0x18];               // 0xB0: CCur18 cursor storage (0x18 bytes)
    u8 _C8[0x12338 - 0xC8];         // 0xC8..0x12337: uninitialized region
    cf::CtrlObjectParamInit mParams[14];  // 0x12338, 14 x 0xCC = 0xB28
    // size 0x12E60
};

// Abstract view into the embedded CCur18 vtable. MWCC inserts an offset-to-top
// + RTTI prefix (2 entries), so the second declared virtual lands at vtable
// +0x0C (slot 3) - the cursor update virtual invoked by Term.
class CCursor18 {
public:
    virtual void vf2(int) = 0;  // slot 2 (+0x08)
    virtual void vf3() = 0;     // slot 3 (+0x0C)
};

// Minimal CTaskGame decl (retail symbols getInstance__9CTaskGameFv /
// func_800426F0__9CTaskGameFv; same scheme as CSysWinSave.hpp).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// DrawInfo helper (retail mangled name func_80137250__FPQ34nw4r3lyt8DrawInfo).
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// C-ABI imports (retail symbols are unmangled / pre-mangled C names).
extern "C" {
void __ct__8CProcessFv(CProcess* self);
void __dt__8CProcessFv(CProcess* self, int flags);
void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent,
                                   bool insertTop);
void __ct__CSysWin(void* syswin, int arg);
void __dt__7CSysWinFv(void* syswin, int flags);
void __ct__CCur18(void* cursor, void* accessor);
void __dt__6CCur18Fv(void* cursor, int flags);
void* func_8009D764(cf::CtrlObjectParamInit* p);
void func_8022B7F4(void* syswin);
void func_8022B7C8(void* syswin, nw4r::lyt::DrawInfo* drawInfo);
void func_801D20B0(void* cursor, nw4r::lyt::DrawInfo* drawInfo);
void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo);
void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo, int flags);
int func_8013BE50();
void func_8008294C__Q22cf13CfGameManagerFv(int enable);
}

// Global data imports (MWCC does not mangle global-scope data names).
extern CMenuGameClear* lbl_eu_80664C08;  // singleton instance pointer (.sbss)
extern u32 lbl_eu_80664C10;              // game-clear state word (.sbss)
extern u32 lbl_eu_80663E28;              // task-state flag word (bit 21 gate)

extern u32 __ptmf_null[3];               // null pointer-to-member-function
extern char lbl_eu_8052BF70[];           // CProcess vtable (temp store)
extern char lbl_eu_8053AE20[];           // CMenuGameClear composite vtable
