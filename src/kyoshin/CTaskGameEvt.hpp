#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"

class CEventFile;

// Minimal CScn declaration local to this TU set: only the two OOL render-callback
// members are needed. Declared with the real class tag (CScn) and parameter
// list so member calls emit the retail mangled symbols
// removeRenderCB__4CScnFP10IScnRender / addRenderCB__4CScnFP10IScnRenderUlUl.
// (A full #include of monolib/scn/CScn.hpp is safe now that the CTTask template
// lives only in monolib/work/CTTask.hpp, but this minimal form keeps the TU
// lean.) Guarded so the kyoshin task headers can be included together.
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

// CProcess base constructor imported from another TU (C-ABI, retail C-linkage
// symbol name - do not let C++ mangle its parameter list).
extern "C" void __ct__8CProcessFv(CProcess* self);

// Retail vtable data / PTMF constant (imports; defined in .data/.rodata splits).
// lbl_eu_80538C00 is the CTaskGameEvt vtable region; the secondary sub-object
// vtables (field_54 at +0x24, the IScnRender member at +0xAC) live inside the
// same block, so the constructor references them as offsets off this label.
// lbl_eu_80538CE8 is the interim CTTask<CTaskGameEvt> vtable.
extern char lbl_eu_80538C00[];     // CTaskGameEvt vtable region
extern char lbl_eu_80538CE8[];     // CTTask<CTaskGameEvt> vtable
// null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// Generic task wrapper - canonical monolib template (declared-only members so
// the unit cpp can emit the retail out-of-line Move/Draw/dtor symbols via
// explicit `template<>` specializations).
#include "monolib/work/CTTask.hpp"

class CTaskGameEvt : public CTTask<CTaskGameEvt> {
public:
    CTaskGameEvt(int arg);
    virtual ~CTaskGameEvt();
    void Init();
    void Term();
    void Move();
    static CTaskGameEvt* create(CProcess* pParent, int arg);

    // 0x0-0x54: CTTask<CTaskGameEvt>
    u32 field_54;          // 0x54
    // 0x58 render callback subobject. Stored as raw bytes, not a typed
    // IScnRender member: a typed member makes MWCC emit a standalone
    // `__dt__10IScnRenderFv` strong copy (0x40) in every TU with an
    // out-of-line dtor (retail keeps it only in CTaskGame.o).
    u8 mRenderCB[4];     // 0x58
    CScn* mScene;          // 0x5C
    u32 mFlags;            // 0x60
}; // size: 0x64

// Minimal declaration for the cf event-task singleton query (avoids pulling
// in the full cf header).
namespace cf {
class CTaskGameCf {
public:
    static CTaskGameCf* getInstance();
};
} // namespace cf

// Free-function imports defined in sibling TUs (cf/CTaskREvent.cpp).
// These are genuine C-ABI functions (retail unmangled symbols), so they are
// declared extern "C" here: `decltype`/member-reference would otherwise mangle
// the call relocs to func_80164C48__Fv / func_80164954__Fv and drift from the
// retail reloc sites (func_80164C48 / func_80164954).
extern "C" {
void func_80165038();
void func_80164CFC();
int func_80164C48();
int func_80164954();
}