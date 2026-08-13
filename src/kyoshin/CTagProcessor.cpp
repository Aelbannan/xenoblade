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

// Deleting destructor for the anonymous-namespace tag code classes.
// MWCC emits the `if (this && deletingFlag) operator delete(this)` idiom
// for an empty destructor; retail symbol is the annotation name below.
void* __dt___unnamed_CTagProcessor_cpp_CTagCodeCP(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeWait(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeDel(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeRuby(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeCol(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeTrust(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeType(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodePos(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeMouth(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeLine(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

// Deleting destructor for the anonymous-namespace tag code classes.
// MWCC emits the `if (this && deletingFlag) operator delete(this)` idiom
// for an empty destructor; retail symbol is the annotation name below.
void* __dt___unnamed_CTagProcessor_cpp_CTagCodeSelect(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeSelect2(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeAct(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeFade(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeQuest(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

// Deleting destructor for the anonymous-namespace tag code classes.
// MWCC emits the `if (this && deletingFlag) operator delete(this)` idiom
// for an empty destructor; retail symbol is the annotation name below.
void* __dt___unnamed_CTagProcessor_cpp_CTagCodeInvisible(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeQstStart(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodePresent(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeTailOff(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeExchange(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

// Deleting destructor for the anonymous-namespace tag code classes.
// MWCC emits the `if (this && deletingFlag) operator delete(this)` idiom
// for an empty destructor; retail symbol is the annotation name below.
void* __dt___unnamed_CTagProcessor_cpp_CTagCodeShop(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

void* __dt___unnamed_CTagProcessor_cpp_CTagCodeMakeCrystal(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

// Reset a tag-param block to all zero (float field takes the sdata2 constant).
void func_801258D0(TagParam* p) {
    p->field_00 = 0;
    p->field_01 = 0;
    p->field_02 = 0;
    p->field_03 = 0;
    p->field_04 = 0;
    p->field_06 = 0;
    p->field_08 = lbl_eu_806671F0;
}

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

// Init a tag-param block with a byte code (field_00) and a u16 value (field_04).
void func_8012591C(TagParam* p, u8 a, u16 b) {
    p->field_00 = a;
    p->field_01 = 0;
    p->field_02 = 0;
    p->field_03 = 0;
    p->field_04 = b;
    p->field_06 = 0;
    p->field_08 = lbl_eu_806671F0;
}

// Init a tag-param block with a byte code (field_00) only.
void func_80125944(TagParam* p, u8 a) {
    p->field_00 = a;
    p->field_01 = 0;
    p->field_02 = 0;
    p->field_03 = 0;
    p->field_04 = 0;
    p->field_06 = 0;
    p->field_08 = lbl_eu_806671F0;
}

// Deleting destructor for the tag-param code object at 8012596C.
void* __dt__8012596C(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

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

// Tag-code dispatch for Process: walk the handler table; a null handler
// falls back to the base TagProcessor, a matching tag dispatches to the
// handler object's Process override (vtable slot 3). The byte offset keeps
// MWCC's base+offset induction shape (retail: add r8,r7,r6 / addi r6,r6,0xC).
nw4r::ut::TagProcessorBase<wchar_t>::Operation func_80125AB8(
    nw4r::ut::TagProcessorBase<wchar_t>* self, u16 tag,
    nw4r::ut::PrintContext<wchar_t>* ctx) {
    for (u32 o = 0; ; o += 12) {
        const TagEntry* e = (const TagEntry*)((const u8*)lbl_eu_8052D478 + o);
        if (e->field_04 == 0) {
            return self->TagProcessorBase<wchar_t>::Process(tag, ctx);
        }
        if (tag == e->tag) {
            return e->obj->Process(tag, ctx);
        }
    }
}



// Tag-code dispatch for CalcRect: walk the handler table; a null handler
// falls back to the base TagProcessor, a matching tag dispatches to the
// handler object's CalcRect override (vtable slot 4).
nw4r::ut::TagProcessorBase<wchar_t>::Operation func_80125B08(
    nw4r::ut::TagProcessorBase<wchar_t>* self, nw4r::ut::Rect* rect,
    u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx) {
    for (u32 o = 0; ; o += 12) {
        const TagEntry* e = (const TagEntry*)((const u8*)lbl_eu_8052D478 + o);
        if (e->field_04 == 0) {
            return self->TagProcessorBase<wchar_t>::CalcRect(rect, tag, ctx);
        }
        if (tag == e->tag) {
            return e->obj->CalcRect(rect, tag, ctx);
        }
    }
}

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

nw4r::ut::Color& nw4r::ut::Color::operator=(const nw4r::ut::Color& rhs) {
    r = rhs.r;
    g = rhs.g;
    b = rhs.b;
    a = rhs.a;
    return *this;
}

// TagContext struct for the u8*-self functions
struct TagContext {
    u8 _pad00[0x04];
    const wchar_t* contextStr;
    u8 _pad08[0x0C - 0x08];
    float charSpace;

    void addToCharSpace(float val) { charSpace = *(float*)((u8*)this + 4) + val; }
};

extern "C" const wchar_t* getContextStr(u8* self) { return *(const wchar_t**)(self + 0x4); }

extern "C" const wchar_t** getContextStrPtr(u8* self) { return (const wchar_t**)(self + 0x4); }

extern "C" void addToCharSpace(u8* self, float val) { *(float*)(self + 0xC) = *(float*)(self + 0x4) + val; };

void func_80127670(){}

// Compare the first u32 of two blocks (used as a tag-identity test).
// NOTE: retail uses the -O4,s addic/subfe setnz idiom; under this unit's
// -O4,p the same expression lowers to the neg/or/rlwinm form (5 mismatches).
// Needs a unit-level extra_cflags=["-O4,s"] flip (wall: per-function opt level).
u32 func_801276C8(const u32* a, const u32* b) {
    return *b != *a;
}

extern "C" void* func_801276E0(void* self) {
    void* p = *(void**)self;
    *(u32*)self = *(u32*)p;
    return p;
}

void func_801276F4(){}

void func_80127764(){}

extern "C" void func_80127BC4(float* dst, const float* src) {
    dst[0] = src[0];
    dst[1] = src[1];
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

extern "C" void func_80129D1C(TagCopyBlock* dst, const TagCopyBlock* src) {
    *dst = *src;
}

void func_80129E20(){}

void func_80129F3C(){}

void func_8012A070(){}

void func_8012A1A4(){}

void func_8012A224(){}

void func_8012A388(){}

void func_8012A460(){}

void func_8012AAA4(){}

extern "C" u32 func_8012AD2C(void* a, void* b, void* c, u8* p) {
    void* base = *(void**)(p + 4);
    *(void**)(p + 4) = (u8*)base + 4;
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
