// Auto-scaffolded catalog TU for kyoshin/CTagProcessor
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void* __dt___unnamed_CTagProcessor_cpp_CTagCodeCL(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeLine(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeSelect(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeSelect2(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeAct(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeFade(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeQuest(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeInvisible(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeQstStart(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodePresent(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeTailOff(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeExchange(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeShop(){}

void __dt___unnamed_CTagProcessor_cpp_CTagCodeMakeCrystal(){}

void func_801258D0(){}

extern "C" void func_801258F8(void* obj, unsigned char a, unsigned char b, float c) {
    unsigned char* base = (unsigned char*)obj;
    base[0] = a;
    base[1] = 0;
    base[2] = 0;
    base[3] = 0;
    *(unsigned short*)(base + 4) = 0;
    base[6] = b;
    *(float*)(base + 8) = c;
}

void func_8012591C(){}

void func_80125944(){}

void __dt__8012596C(){}

void __ct__CTagProcessorBase(){}

extern "C" void func_80125A2C(void* r3, const void* r4) {
    unsigned char* dst = (unsigned char*)r3;
    const unsigned char* src = (const unsigned char*)r4;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    *(unsigned short*)(dst + 4) = *(const unsigned short*)(src + 4);
    dst[6] = src[6];
    *(float*)(dst + 8) = *(const float*)(src + 8);
}

void CTagProcessorBase::~CTagProcessorBase() {}

void func_80125AB8(){}



void func_80125B08(){}

extern "C" int func_80125B50(void* self) { return 0; }

void func_80125B58(){}

void func_80125D00(){}

void __ct__CTagProcessor(){}

void CTagProcessor::~CTagProcessor() {}

void func_8012615C(){}

extern "C" void func_80127630(float *dst, const float *src) {
    dst[0] = src[0];
    dst[1] = src[1];
}

void __as__Q34nw4r2ut5ColorFRCQ34nw4r2ut5Color(){}

extern "C" u32 func_80127668(void* self) { return *(u32*)((u8*)self + 0x4); }

void func_80127670(){}

extern "C" void* func_801276C0(void* self) { return (void*)((u8*)self + 0x4); }

void func_801276C8(){}

void func_801276E0(){}

void func_801276F4(){}

void func_80127764(){}

void func_80127BC4(){}

extern "C" void func_80127BD8(float* dst, const float* src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

void func_80127BF4(){}

void func_80127D20(){}

void func_80127E74(){}

void __ct__CTagProcessorSE(){}

void CTagProcessorSE::~CTagProcessorSE() {}

void func_80127FB4(){}

void func_801286E0(){}

void func_80128740(){}

void func_801287BC(){}

void func_801289B4(){}

void func_801289FC(){}

void func_80128A70(){}

void func_80128AB8(){}

void func_80128B0C(){}

extern "C" void* func_80128B80(void* arg1, void* arg2) {
    extern void func_8013BDE4(void*);
    func_8013BDE4(arg1);
    return arg2;
}

void func_80128BB0(){}

void func_80128BF8(){}

void func_80128C6C(){}

void func_80128DA0(){}

void func_80128EF8(){}

void func_80129008(){}

extern "C" void* func_80129118(void* dummy, void* r4) {
    *(u16*)r4 = 0xD;
    return (u16*)r4 + 1;
}

void func_80129128(){}

void func_801291F4(){}

void func_8012930C(){}

void func_80129430(){}

void func_80129564(){}

extern "C" void func_8012967C(void* self, float val) {
    *(float*)((char*)self + 0xc) = *(float*)((char*)self + 4) + val;
}

void func_8012968C(){}

void func_801299D4(){}

void func_80129AEC(){}

void func_80129C04(){}

void func_80129D1C(){}

void func_80129E20(){}

void func_80129F3C(){}

void func_8012A070(){}

void func_8012A1A4(){}

void func_8012A224(){}

void func_8012A388(){}

void func_8012A460(){}

void func_8012AAA4(){}

void func_8012AD2C(){}

void func_8012AD40(){}

void func_8012AF90(){}

void func_8012B070(){}

void func_8012B204(){}

void func_8012B328(){}

void func_8012B440(){}

void func_8012B8C4(){}

void func_8012B944(){}

extern "C" void* func_8012B9E0(void* unused, void* buf) {
    *(unsigned short*)buf = 3;
    return (char*)buf + 2;
}

extern "C" void* func_8012B9F0(void* unused, void* buf) {
    *(unsigned short*)buf = 0xa;
    return (char*)buf + 2;
}

extern u32 lbl_eu_8052D9E0[];
extern "C" void __ct__CTagCodeCL(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D9E0;
}

extern u32 lbl_eu_8052D9B8[];
extern "C" void __ct__CTagCodeCP(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D9B8;
}

extern u32 lbl_eu_8052D990[];
extern "C" void __ct__CTagCodeWait(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D990;
}

extern u32 lbl_eu_8052D968[];
extern "C" void __ct__CTagCodeDel(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D968;
}

extern u32 lbl_eu_8052D940[];
extern "C" void __ct__CTagCodeRuby(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D940;
}

extern u32 lbl_eu_8052D918[];
extern "C" void __ct__CTagCodeCol(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D918;
}

extern u32 lbl_eu_8052D898[];
extern "C" void __ct__CTagCodeTrust(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D898;
}

extern u32 lbl_eu_8052D870[];
extern "C" void __ct__CTagCodeType(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D870;
}

extern u32 lbl_eu_8052D848[];
extern "C" void __ct__CTagCodePos(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D848;
}

extern u32 lbl_eu_8052D820[];
extern "C" void __ct__CTagCodeMouth(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D820;
}

extern u32 lbl_eu_8052D7F8[];
extern "C" void __ct__CTagCodeLine(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D7F8;
}

extern u32 lbl_eu_8052D7D0[];
extern "C" void __ct__CTagCodeSelect(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D7D0;
}

extern u32 lbl_eu_8052D7A8[];
extern "C" void __ct__CTagCodeSelect2(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D7A8;
}

extern u32 lbl_eu_8052D780[];
extern "C" void __ct__CTagCodeAct(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D780;
}

extern u32 lbl_eu_8052D758[];
extern "C" void __ct__CTagCodeFade(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D758;
}

extern u32 lbl_eu_8052D730[];
extern "C" void __ct__CTagCodeQuest(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D730;
}

extern u32 lbl_eu_8052D708[];
extern "C" void __ct__CTagCodeInvisible(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D708;
}

extern u32 lbl_eu_8052D6E0[];
extern "C" void __ct__CTagCodeQstStart(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D6E0;
}

extern u32 lbl_eu_8052D6B8[];
extern "C" void __ct__CTagCodePresent(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D6B8;
}

extern u32 lbl_eu_8052D690[];
extern "C" void __ct__CTagCodeTailOff(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D690;
}

extern u32 lbl_eu_8052D668[];
extern "C" void __ct__CTagCodeExchange(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D668;
}

extern u32 lbl_eu_8052D640[];
extern "C" void __ct__CTagCodeShop(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D640;
}

extern u32 lbl_eu_8052D618[];
extern "C" void __ct__CTagCodeMakeCrystal(void* self) {
    *(u32*)self = (u32)lbl_eu_8052D618;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8012BA00(){}
