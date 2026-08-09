#pragma once

#include <types.h>
#include <revolution/GX.h>

#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"

// Minimal CScn declaration local to this TU set: only the render-callback
// members used by Init/Term are needed. Declared with the real class tag
// (CScn) and parameter list so member calls emit the retail mangled symbols
// addRenderCB__4CScnFP10IScnRenderUlUl / removeRenderCB__4CScnFP10IScnRender.
// Guarded so the kyoshin task headers can be included together.
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

// CProcess base constructor imported from another TU (retail C-linkage symbol
// name - do not let C++ mangle its parameter list).
extern "C" void __ct__8CProcessFv(CProcess* self);

// null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// Retail vtable regions for CTaskGamePic (imports; .data). lbl_eu_80538AD8 is
// the final CTaskGamePic vtable; lbl_eu_80538BC0 is the interim
// CTTask<CTaskGamePic> vtable (overwritten by the derived vptr in the ctor).
extern char lbl_eu_80538AD8[];
extern char lbl_eu_80538BC0[];

// Color/vec4 helper imported from another TU. Retail symbol is the *unmangled*
// `func_800407C8` (see include/functions.hpp), so it must be declared extern
// "C" here; including functions.hpp instead mangles it to
// func_800407C8__FP17func_800407C8_tmpffff and drifts the reloc sites.
struct func_800407C8_tmp {
    f32 unk00[4];
};
extern "C" func_800407C8_tmp* func_800407C8(func_800407C8_tmp*, f32, f32, f32, f32);

// Shared .sdata2 float used as the GX texture LOD bias in the file-event
// handler (func_8029539C).
extern f32 lbl_eu_80668BB0;

// Generic task wrapper - canonical monolib template (declared-only members so
// the unit cpp can emit the retail out-of-line Move/Draw/dtor symbols via
// explicit `template<>` specializations).
#include "monolib/work/CTTask.hpp"

class CTaskGamePic : public CTTask<CTaskGamePic> {
public:
    CTaskGamePic();
    virtual ~CTaskGamePic();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void create();
    void Draw();
    void OnFileEvent();

    // 0x0-0x54: CTTask<CTaskGamePic>
    u32 field_54;                     // 0x54 - file-event object vptr slot
    IScnRender mRenderCB;             // 0x58 render callback subobject
    CScn* mScene;                     // 0x5C
    CFileHandle* mFileHandle;         // 0x60 async file handle
    void* field_64;                   // 0x64 palette / loaded data
    void* field_68;                   // 0x68 palette / loaded data (alias of 0x64)
    GXTexObj mTexObj;                 // 0x6C loaded GX texture object (0x20: 0x6C..0x8B)
    u32 field_8C;                     // 0x8C..0x8F gap after GXTexObj (ctor zeroes only byte 0x8C)
    // 0x90-0xC4: texture/palette parameter block (read/written by func_80294E58)
    u32 param_90;
    u32 param_94;
    u32 param_98;
    u32 param_9C;
    u32 param_A0;
    u32 param_A4;
    u32 param_A8;
    u32 param_AC;
    u32 param_B0;
    u32 param_B4;
    u32 param_B8;
    u32 param_BC;
    u32 param_C0;
    u32 param_C4;
}; // size: 0xC8

// Retail ctor symbol is the *stripped* single-word form (no class-length
// prefix), so it is written as a plain global free function instead of a C++
// member ctor (cf. CTaskGameEvt / CTaskGameCf). Global free-function names are
// not mangled, so no `extern "C"` is needed. Placed after the class so the
// return type is visible.
CTaskGamePic* __ct__CTaskGamePic(CTaskGamePic* self, int arg);