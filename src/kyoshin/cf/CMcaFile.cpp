// CMcaFile - MCA file container

#include "kyoshin/harness_catalog.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "kyoshin/cf/CMcaFile.hpp"

extern "C" void __ct__CMcaFile(u8* self, void* data) {
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

extern "C" void func_800584B8(u8* self, void* data, const char* name) {
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

// absorb: split1 retail data sections
// generated from retail build/us/asm via split1_spec.txt
__declspec(section ".data") __attribute__((aligned(8))) __attribute__((used)) unsigned char __absorb_kyoshin_cf_CMcaFile_data[0xC] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
__declspec(section ".rodata") __attribute__((aligned(8))) __attribute__((used)) const unsigned char __absorb_kyoshin_cf_CMcaFile_rodata[0x10] = {
    0x43, 0x4D, 0x63, 0x61, 0x46, 0x69, 0x6C, 0x65, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) unsigned char __absorb_kyoshin_cf_CMcaFile_sdata[0x8] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
