// Auto-scaffolded catalog TU for kyoshin/CTagProcessor
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTagProcessor.hpp"

struct TagCopyBlock {
    u32 w00, w04, w08, w0C, w10, w14, w18, w1C;
    u32 w20, w24, w28, w2C, w30, w34, w38, w3C;
    u16 h40;
    u8 b42, b43;
    f32 f44;
    u32 w48;
    f32 f4C, f50, f54;
    u32 w58, w5C, w60;
};

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeCL(void* self, int deleting) {
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

void initTagParam(u8* obj, unsigned char a, unsigned char b, float c) {
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

void copyTagParam(u8* dst, const u8* src) {
    unsigned char* d = (unsigned char*)dst;
    const unsigned char* s = (const unsigned char*)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
    *(unsigned short*)(d + 4) = *(const unsigned short*)(s + 4);
    d[6] = s[6];
    *(float*)(d + 8) = *(const float*)(s + 8);
}

CTagProcessorBase::~CTagProcessorBase() {}

void func_80125AB8(){}



void func_80125B08(){}

extern "C" int defaultProcess(void* self) { return 0; }

void func_80125B58(){}

void func_80125D00(){}

void __ct__CTagProcessor(){}

CTagProcessor::~CTagProcessor() {}

void func_8012615C(){}

extern "C" void copyVEC2(float *dst, const float *src) {
    dst[0] = src[0];
    dst[1] = src[1];
}

void __as__Q34nw4r2ut5ColorFRCQ34nw4r2ut5Color(){}

// TagContext struct for the u8*-self functions
struct TagContext {
    u8 _pad00[0x04];
    const wchar_t* contextStr;
    u8 _pad08[0x0C - 0x08];
    float charSpace;

    void addToCharSpace(float val) { charSpace = *(float*)((u8*)this + 4) + val; }
};

extern "C" const wchar_t* getContextStr(void* self) { return *(const wchar_t**)((u8*)self + 0x4); }

extern "C" const wchar_t** getContextStrPtr(void* self) { return (const wchar_t**)((u8*)self + 0x4); }

extern "C" void addToCharSpace(void* self, float val) { *(float*)((u8*)self + 0xC) = *(float*)((u8*)self + 0x4) + val; };

void func_80127670(){}

void func_801276C8(){}

extern "C" void* func_801276E0(void* self) {
    void* p = *(void**)self;
    *(u32*)self = *(u32*)p;
    return p;
}

void func_801276F4(){}

void func_80127764(){}

extern "C" void func_80127BC4(void* dst, void* src) {
    *(float*)dst = *(float*)src;
    *(float*)((u8*)dst + 4) = *(float*)((u8*)src + 4);
}

extern "C" void copyVEC3(float* dst, const float* src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

void func_80127BF4(){}

void func_80127D20(){}

void func_80127E74(){}

void __ct__CTagProcessorSE(){}

CTagProcessorSE::~CTagProcessorSE() {}

void func_80127FB4(){}

void func_801286E0(){}

void func_80128740(){}

void func_801287BC(){}

void func_801289B4(){}

void func_801289FC(){}

void func_80128A70(){}

void func_80128AB8(){}

void func_80128B0C(){}

void* callInitTagProc(void* arg1, void* arg2) {
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

extern "C" u16* writeTagCode0D(void* dummy, u16* buf) {
    *buf = 0xD;
    return buf + 1;
}

void func_80129128(){}

void func_801291F4(){}

void func_8012930C(){}

void func_80129430(){}

void func_80129564(){}

// Converted to TagContext::addToCharSpace

void func_8012968C(){}

void func_801299D4(){}

void func_80129AEC(){}

void func_80129C04(){}

extern "C" void func_80129D1C(void* dst, const void* src) {
    *(TagCopyBlock*)dst = *(const TagCopyBlock*)src;
}

void func_80129E20(){}

void func_80129F3C(){}

void func_8012A070(){}

void func_8012A1A4(){}

void func_8012A224(){}

void func_8012A388(){}

void func_8012A460(){}

void func_8012AAA4(){}

extern "C" u32 func_8012AD2C(void* a, void* b, void* c, void* p) {
    void* base = *(void**)((u8*)p + 4);
    *(void**)((u8*)p + 4) = (u8*)base + 4;
    return 0;
}

void func_8012AD40(){}

void func_8012AF90(){}

void func_8012B070(){}

void func_8012B204(){}

void func_8012B328(){}

void func_8012B440(){}

void func_8012B8C4(){}

void func_8012B944(){}

extern "C" u16* writeTagCode03(void* unused, u16* buf) {
    *buf = 3;
    return buf + 1;
}

extern "C" u16* writeTagCode0A(void* unused, u16* buf) {
    *buf = 0xa;
    return buf + 1;
}

extern u32 lbl_eu_8052D9E0[];


extern u32 lbl_eu_8052D9B8[];


extern u32 lbl_eu_8052D990[];


extern u32 lbl_eu_8052D968[];


extern u32 lbl_eu_8052D940[];


extern u32 lbl_eu_8052D918[];


extern u32 lbl_eu_8052D898[];


extern u32 lbl_eu_8052D870[];


extern u32 lbl_eu_8052D848[];


extern u32 lbl_eu_8052D820[];


extern u32 lbl_eu_8052D7F8[];


extern u32 lbl_eu_8052D7D0[];


extern u32 lbl_eu_8052D7A8[];


extern u32 lbl_eu_8052D780[];


extern u32 lbl_eu_8052D758[];


extern u32 lbl_eu_8052D730[];


extern u32 lbl_eu_8052D708[];


extern u32 lbl_eu_8052D6E0[];


extern u32 lbl_eu_8052D6B8[];


extern u32 lbl_eu_8052D690[];


extern u32 lbl_eu_8052D668[];


extern u32 lbl_eu_8052D640[];


extern u32 lbl_eu_8052D618[];


// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8012BA00(){}
