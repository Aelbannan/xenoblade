// CMcaFile - MCA file container

#include "kyoshin/harness_catalog.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "kyoshin/cf/CMcaFile.hpp"

struct McaHeadView { u8 magic[4]; u32 dataOff; };
extern "C" void __ct__CMcaFile(u8* self, void* data) {
    u8* s = (u8*)self;
    u8* d = (u8*)data;

    *(void**)self = (void*)lbl_eu_80526348;
    s[4] = 0;
    *(void**)(s + 8) = data;
    *(void**)(s + 12) = data;

    if ((s8)d[0] == 0x6D && (s8)d[1] == 0x63 && (s8)d[2] == 0x61 && (s8)d[3] == 0) {
        s[4] = 1;
        *(void**)(s + 12) = d + ((McaHeadView*)d)->dataOff;
    }
}

CMcaFile::~CMcaFile() {
    // Nothing to destroy; deletion is handled by MWCC ABI flag
}

// Compiler-generated deleting destructor (defined in this TU's .text);
// referenced by the hand-built vtable below.
extern "C" void __dt__8CMcaFileFv();

// Retail string label (.rodata @0x804FB088, 0x10): typeinfo name.
const char lbl_eu_804FB088[0x10] = "CMcaFile";

// Retail RTTI descriptor pair (.sdata @0x806619C0): {name, 0}.
__attribute__((used))
const void* __RTTI__8CMcaFile[2] = { lbl_eu_804FB088, 0 };

// Retail vtable data (.data @0x80526348: RTTI + 0 + dtor); the ctor stores
// it at +0x0 (novtable: the TU declares the class without a compiler
// vtable and assigns the retail label explicitly).
__attribute__((used))
void* lbl_eu_80526348[3] = {
    (void*)__RTTI__8CMcaFile,
    0,
    (void*)__dt__8CMcaFileFv,
};

extern "C" void initMcaFile(u8* self, void* data, const char* name) {
    u8* d = (u8*)data;
    s32 hasMagic = 0;
    void* dataAdj = data;

    if ((s8)d[0] == 0x6D && (s8)d[1] == 0x63 && (s8)d[2] == 0x61 && (s8)d[3] == 0) {
        hasMagic = 1;
        dataAdj = (u8*)data + ((McaHeadView*)d)->dataOff;
    }

    if (hasMagic != 0) {
        u8 buf[68];
        ml::CPathUtil::getNoPathExtName(*(ml::FixStr<64>*)buf, name);
        Scn_InitGlobalA(self, dataAdj, *(ml::FixStr<64>*)buf);
    } else {
        u8 buf[68];
        ml::CPathUtil::getNoPathExtName(*(ml::FixStr<64>*)buf, name);
        Scn_InitGlobalA(self, data, *(ml::FixStr<64>*)buf);
    }
}
