#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

/*
 * Land-telop menu screen process (CSysWinSave / CMenuQstCnt family).
 *
 * MI layout:
 *   0x00  CProcess (0x3C bytes)
 *   0x3C  IUIWindow/Move region (48 bytes: ptmf slots + misc u32s + flags)
 *   0x6C  IWorkEvent vtable slot
 *   0x70  IScnRender vtable slot (render-callback subobject)
 *   0x74  CScn* owning scene
 *   0x78  UnkClass_8045F564 embedded region (0x10 bytes)
 *   0xE0  bool flag (restored from func_80226B94 in Init)
 */
class CMenuLandTelop : public CProcess {
public:
    CMenuLandTelop();
    virtual ~CMenuLandTelop();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x3C IUIWindow/Move region (same as CSysWinSave / CMenuQstCnt).
    u32 ptmf0[3];                 // 0x3C null pointer-to-member-function
    u32 ptmf1[3];                 // 0x48 null pointer-to-member-function
    u32 field_54;                 // 0x54
    u32 field_58;                 // 0x58
    u32 field_5C;                 // 0x5C
    s32 field_60;                 // 0x60
    u8  field_64;                 // 0x64
    u8  field_65;                 // 0x65
    u8  field_66;                 // 0x66
    u8  field_67;                 // 0x67 (cleared in Init)
    u32 field_68;                 // 0x68
    u32 mWorkEvent;               // 0x6C IWorkEvent vtable slot
    u32 mScnRender;               // 0x70 IScnRender vtable slot
    CScn* mScene;                 // 0x74 owning scene
    UnkClass_8045F564 mMemRegion; // 0x78 embedded scratch region
    u8  _88[0xE0 - 0x88];         // 0x88 unmodelled region
    u8  field_E0;                 // 0xE0 bool
};

// Object behind the global lbl_eu_806641A0 pointer (real type not yet
// recovered). Only the byte offsets touched by this TU are modelled.
struct CLandTelopGlobal {
    u8 _00[0x64];            // 0x00
    u8 field_0x64;           // 0x64
    u8 _65[0x8E - 0x65];     // 0x65
    u8 field_0x8E;           // 0x8E state flag (0/1)
    u8 _8F[0xE0 - 0x8F];     // 0x8F
    u8 field_0xE0;           // 0xE0 bool
};

// Minimal CScn declaration (guarded so the kyoshin menu/task headers can be
// included together; same declaration appears in CTaskGameEff/Evt/Pic.hpp).
// Member calls emit the retail mangled addRenderCB__4CScnFP10IScnRenderUlUl.
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

// C-linkage imports/definitions: retail symbols in this TU are unmangled, so
// they are declared `extern "C"` to stop MWCC suffixing the reloc name
// (CSysWinSave.hpp / CMenuQstCnt.hpp idiom). The cpp definitions inherit the
// linkage; __dt__14CMenuLandTelopFv satisfies the member dtor at link time.
extern "C" {
void __ct__CMenuLandTelop();
void __dt__17UnkClass_8045F564Fv(void* _this, int flags);
void __dt__8CProcessFv(CProcess* _this, int flags);
CMenuLandTelop* __dt__14CMenuLandTelopFv(CMenuLandTelop* _this, int flags);
void cbRenderBefore__14CMenuLandTelopFv(void* _this);
int func_80226B94();
const char* getBdatStringColumnValue(void* bdat, const char* column, int index);
void func_80144EE4();
int func_80144FC8();
int func_80144FF0();
void func_80145018();
unsigned char func_80145030(void);
void func_8014504C();
void func_801453B8();
void func_8014548C();
void func_80145A90(void* _this);
void func_80145A98(void* _this);
void func_80145AA0(void* _this);
const char* func_80145AA8(int index);
}

// Global data imports (sbss/rodata; unmangled at global scope).
extern CLandTelopGlobal* lbl_eu_806641A0;
extern u8* lbl_eu_806640E0;
extern char lbl_eu_805018A8[];
