#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "kyoshin/cf/IResInfo.hpp"

struct CfResBuffer {
    u8 buffer[0x400];
    u32 field_400;
    u32 field_404;

    int func_80061A80(unsigned char byte1, unsigned short halfword, unsigned int dataVal, unsigned int* src, int count, unsigned int headerBits);
};

struct CfRes_64994 {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u8 _0C[0x14];
    u32 field_20;
    u32 field_24;
    u32 field_28;

    void initStruct();
};

struct CfRes_65818 {
    u32 field_00;
    int field_04;

    int decRefCount();
};

namespace cf {

class CfRes {
public:
    CfRes();
    ~CfRes();
};

// CRTP task base (CTTask<CfResTask>) occupies 0x00-0x54; the embedded resource
// storage lives past the base. Term() tears it down via func_80063158 and
// func_80067D38 (the +0x58 region doubles as a ResInfoEntry array and a
// ResInfoStorage container).
class CfResTask : public CTTask<CfResTask> {
public:
    virtual ~CfResTask();
    void Term();

    virtual void Init();
    virtual void Move();
    virtual void Draw();

    /* 0x54 */ u8 mField54[4];              // header of the embedded storage object
    /* 0x58 */ ResInfoEntry mEntries[0x82]; // entry array (also a ResInfoStorage)
};
} // namespace cf

// The retail OnFileEvent symbol is a global-namespace CfRes member (MWCC
// emits the IWorkEvent override of the cf-namespace manager class with the
// base interface's global namespace context - same quirk as CTaskREvent).
// Declared here at global scope so the compiled mangled name matches retail.
class CEventFile;
class CfRes {
public:
    void OnFileEvent(CEventFile* ev);
};

// String-keyed record: 4-byte key string at +0 (compared via strcmp against
// lbl_eu_80661A24, e.g. "DAP1") with a count word at +4.
struct CfResNameRec {
    char mKey[4];   // 0x00 - inline key string
    int mCount;     // 0x04
};

// Name-key string pointer (in .sdata; points at "DAP1" etc.).
extern const char* lbl_eu_80661A24;

// Reset the ResInfoEntry array (defined in kyoshin/cf/IResInfo.cpp).
// C linkage so the retail unmangled symbol name is emitted (struct params
// would otherwise mangle it).
extern "C" void func_80067D38(ResInfoEntry* entry);

// Global CfRes manager state word (zeroed by CfResTask::Term).
extern u32 lbl_eu_80663D78;

// C-linkage runtime import (retail symbol name - keep linkage/signatures verbatim)
extern "C" long __ptmf_test(void* ptmf);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" int func_80068078(int);
extern "C" int func_80068254(int);
extern "C" int func_801BFA64(int);
extern "C" int func_800A9068();
extern "C" int func_800A9134();
extern "C" void func_800676F8(u8* self);
// Same-unit helpers under their retail unmangled names (CfRes.cpp stubs -
// linkage must stay C so call relocs carry the retail names).
extern "C" int CfResEntry_decRefCount(u8* entry);
extern "C" int func_80063A60(u8* res);
extern "C" void func_80064014(CfRes* self, CEventFile* evt, u32 field);
extern "C" void func_eu_80065590(int inst, int index, u8* ptr);

