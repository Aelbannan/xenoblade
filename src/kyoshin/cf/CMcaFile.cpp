// CMcaFile — MCA file container

#include "kyoshin/harness_catalog.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "kyoshin/cf/CMcaFile.hpp"

extern "C" void __ct__CMcaFile(void* self, void* data) {
    extern u8 lbl_eu_80526348[];
    u8* s = (u8*)self;
    u8* d = (u8*)data;

    *(void**)self = (void*)lbl_eu_80526348;
    s[4] = 0;
    *(void**)(s + 8) = data;
    *(void**)(s + 12) = data;

    if ((s8)d[0] == 0x6D && (s8)d[1] == 0x63 && (s8)d[2] == 0x61 && (s8)d[3] == 0) {
        s[4] = 1;
        *(void**)(s + 12) = d + *(u32*)(d + 4);
    }
}

CMcaFile::~CMcaFile() {
    // Nothing to destroy; deletion is handled by MWCC ABI flag
}

extern "C" void func_800584B8(void* self, void* data, const char* name) {
    u8* d = (u8*)data;
    s32 hasMagic = 0;
    void* dataAdj = data;

    if ((s8)d[0] == 0x6D && (s8)d[1] == 0x63 && (s8)d[2] == 0x61 && (s8)d[3] == 0) {
        hasMagic = 1;
        dataAdj = (u8*)data + *(u32*)(d + 4);
    }

    if (hasMagic != 0) {
        u8 buf[68];
        ml::CPathUtil::getNoPathExtName(*(ml::FixStr<64>*)buf, name);
        func_80495EAC(self, dataAdj, *(ml::FixStr<64>*)buf);
    } else {
        u8 buf[68];
        ml::CPathUtil::getNoPathExtName(*(ml::FixStr<64>*)buf, name);
        func_80495EAC(self, data, *(ml::FixStr<64>*)buf);
    }
}
