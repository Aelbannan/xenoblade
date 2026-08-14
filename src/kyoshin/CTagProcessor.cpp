// Auto-scaffolded catalog TU for kyoshin/CTagProcessor
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTagProcessor.hpp"

#include <wchar.h>  // wcstol (MSL, C-linkage)

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
// noinline + C linkage (declared in the header): the tag-proc ctors call it
// via bl and the retail reloc name is the flat func_801258D0.
__declspec(noinline) void func_801258D0(TagParam* p) {
    p->field_00 = 0;
    p->field_01 = 0;
    p->field_02 = 0;
    p->field_03 = 0;
    p->field_04 = 0;
    p->field_06 = 0;
    p->field_08 = lbl_eu_806671F0;
}

// Init a tag-param block from a byte code and a u16 value. The tag-writer
// family calls it via bl (retail kept it as a flat C-ABI symbol in a separate
// TU), so the body must stay out of line - noinline + the header's extern
// "C" declaration keep the symbol unmangled.
__declspec(noinline) void initTagParam(u8* obj, unsigned char a, unsigned char b, float c) {
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
// extern "C" + __declspec(noinline): the tag-writer family calls it via bl with
// the flat retail name func_8012591C - keep the symbol unmangled and the call
// out of line so the reloc matches retail (same convention as func_80125944).
extern "C" __declspec(noinline) void func_8012591C(TagParam* p, u8 a, u16 b) {
    p->field_00 = a;
    p->field_01 = 0;
    p->field_02 = 0;
    p->field_03 = 0;
    p->field_04 = b;
    p->field_06 = 0;
    p->field_08 = lbl_eu_806671F0;
}

// Init a tag-param block with a byte code (field_00) only.
// extern "C" + __declspec(noinline): retail kept this helper as a flat
// C-linkage symbol in a separate TU, so the tag-writer family calls it via
// bl with the unmangled name - do not let MWCC mangle/inline it here.
extern "C" __declspec(noinline) void func_80125944(TagParam* p, u8 a) {
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

// Tag-processor base constructor (retail __ct__CTagProcessorBase): the member
// ctor provides the compiler-generated nw4r base-ctor call — placement new
// would add a null beq the retail does not have. novtable: the retail vtable
// data lives in .data (lbl_eu_8052DA68), so the vptr is stored explicitly.
CTagProcessorBase::CTagProcessorBase() {
    field_804 = 0;
    *(void**)this = (void*)lbl_eu_8052DA68;
    field_808 = lbl_eu_806671F0;
    field_80c = lbl_eu_806671F0;
    field_810 = 0;
    func_801258D0(&field_814);
    memset(mBuf, 0, sizeof(mBuf));
    TagParam tmp;
    func_801258D0(&tmp);
    copyTagParam((u8*)&field_814, (const u8*)&tmp);
}

// Copy a 12-byte tag-param block byte-by-byte.
// extern "C" + __declspec(noinline): retail kept this helper as a flat
// C-linkage symbol in a separate TU, so the tag-writer family calls it via
// bl with the unmangled name - do not let MWCC mangle/inline it here.
extern "C" __declspec(noinline) void copyTagParam(u8* dst, const u8* src) {
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

// Deleting destructor for CTagProcessorBase (retail __dt__17CTagProcessorBaseFv):
// genuine C++ member dtor of the derived class — the compiler emits the nw4r
// base-dtor call with flag 0 and the operator delete when the deleting flag
// is set (the retail shape).
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

// Message pre-processor: copy the raw message into the buffer, then walk it
// in place — a CRLF at the head is stripped (the tail is copied down over
// it), and a '<name=value>' tag is looked up by name in the tag dispatch
// table; the matching handler (vtable slot +0x14) consumes the tag and
// returns the new buffer position, after which the saved tail is copied
// down over it. Returns the buffer.
u16* func_80125B58(CTagProcessorBase* self, const void* src, f32 a, f32 b, u32 c) {
    wchar_t bigbuf[0x400];
    wchar_t scratchA[0x80];
    wchar_t* tokens[16];
    memcpy(self->mBuf, src, 0x800);
    self->field_804 = c;
    self->field_808 = a;
    self->field_80c = b;
    self->field_810 = 0;
    wchar_t* p = (wchar_t*)self->mBuf;
    while (*p != 0) {
        u32 count = 0;
        u32 off = 0;
        memset(bigbuf, 0, sizeof(bigbuf));
        memset(scratchA, 0, sizeof(scratchA));
        if (p[0] == 0xD && p[1] == 0xA) {
            wcscpy(bigbuf, p + 2);
            wcscpy(p, bigbuf);
            continue;
        }
        if (p[0] == 0x3C) {
            u16 ch;
            while ((ch = p[off / 2 + 1]) != 0x3E && ch != 0) {
                scratchA[off / 2] = ch;
                count++;
                off += 2;
            }
            func_801365E4((u16*)scratchA, 0x3D, (u16**)tokens);
            func_801366F4((u16*)tokens[0]);
            wcscpy(bigbuf, p + count + 2);
            const wchar_t* name = tokens[0];
            for (const TagNameEntry* e = (const TagNameEntry*)lbl_eu_8052D478;; e++) {
                if (e->name == 0)
                    break;
                if (wcscmp(name, e->name) == 0) {
                    p = (wchar_t*)e->obj->v14((u16*)p, tokens[1], &self->field_814);
                    break;
                }
            }
            wcscpy(p, bigbuf);
            p--;
        }
        p++;
    }
    return self->mBuf;
}

// Text-position measurement for the tag-writer family: walk the message
// buffer from the TextBox, accumulating the max line width (per-glyph font
// width + the TextBox's per-char spacing at +0xF4) and the line height
// (0xA and the row-advance inside tag 5 step by the fixed sdata2 constant).
// Control tags 1..7 advance the buffer by their operand lengths.
// optimize_for_size: retail uses the -O4,s save shape (_savegpr_17) and the
// mtctr/bdnz pairs loop.
#pragma optimize_for_size on
__declspec(noinline) void func_80125D00(f32* out, CTagMsgView* msg, wchar_t* buf) {
    nw4r::lyt::Font* font = ((nw4r::lyt::TextBox*)msg)->GetFont();
    f32 lineH = lbl_eu_806671F4;
    f32 width = lbl_eu_806671F0;
    f32 lineStep = lineH;
    f32 max = width;
    f32 speed = lbl_eu_806671F8;
    f32 step = lineH;
    u32 i = 0;
    u32 vals[0x12];
    for (;;) {
        u16 ch = buf[i];
        if (ch == 0) {
            if (width > max)
                max = width;
            break;
        }
        if (ch == 0xA) {
            if (width > max)
                max = width;
            lineH += step;
            width = lbl_eu_806671F0;
            i++;
            continue;
        }
        switch (ch) {
        case 1: {
            // <1 skip,cnt>: advance past `skip` chars, then accumulate the
            // width of the following `cnt` chars.
            u16 v = buf[i + 1];
            u32 skip = v >> 8;
            u32 cnt = v & 0xFF;
            i += 2 + skip;
            for (u32 j = 0; j < cnt; j++) {
                f32 sp = msg->field_F4;
                width += sp + (f32)font->v48(buf[i + j]);
            }
            i += cnt - 1;
            break;
        }
        case 2:
            i += 2;
            break;
        case 3:
            i += 1;
            break;
        case 4:
            i += 1;
            break;
        case 5: {
            // <5 cnt,flags>: each following char carries a per-char count in
            // its high/low byte; for each of `cnt` rows accumulate the width
            // of that many chars and step the line between rows.
            u16 v = buf[i + 1];
            u32 cnt = v & 0xFF;
            u32 pairs = (cnt + (v & 1) + 1) >> 1;
            i += 2;
            for (u32 j = 0; j < pairs; j++) {
                u16 c = buf[i + j];
                vals[2 * j] = c >> 8;
                vals[2 * j + 1] = c & 0xFF;
            }
            i += pairs;
            for (u32 row = 0; row < cnt; row++) {
                u32 n = vals[row];
                for (u32 k = 0; k < n; k++) {
                    f32 sp = msg->field_F4;
                    width += sp + (f32)font->v48(buf[i + k]);
                }
                if (row < cnt - 1) {
                    lineH += lineStep;
                    if (width > max)
                        max = width;
                    width = lbl_eu_806671F0;
                }
                i += n;
            }
            i -= 1;
            break;
        }
        case 6:
            i += 1;
            break;
        case 7:
            // Full-width marker: width += speed * (glyph(0x2500) + spacing).
            width += speed * (msg->field_F4 + (f32)font->v48(0x2500));
            break;
        case 8:
        case 9:
        case 0xB:
        case 0xC:
        case 0xD:
        default:
            width += msg->field_F4 + (f32)font->v48(ch);
            break;
        }
        i++;
    }
    out[0] = max;
    out[1] = lineH;
}
#pragma optimize_for_size off

// Tag-proc constructor for the full processor: base-construct, then the
// CTagProcessor-specific fields. The 0x838 block is zeroed via memset.
void* __ct__CTagProcessor(void* self) {
    CTagProcessor* p = (CTagProcessor*)self;
    __ct__CTagProcessorBase(p);
    *(void**)p = (void*)lbl_eu_8052DA38;
    p->field_820 = 0;
    p->field_824 = lbl_eu_806671F0;
    p->field_828 = 0;
    p->field_82c = 0;
    p->field_830 = 0;
    p->field_834 = -1;
    p->field_844 = 10;
    p->field_848 = 0;
    p->field_84c = 0;
    p->field_850 = 0;
    p->field_854 = 0;
    memset(p->m838, 0, sizeof(p->m838));
    return p;
}

// Deleting destructor for CTagProcessor (retail __dt__13CTagProcessorFv):
// genuine C++ member dtor of the derived class — the compiler emits the base
// dtor call (__dt__17CTagProcessorBaseFv) with flag 0 and the operator delete
// when the deleting flag is set (the retail shape).
CTagProcessor::~CTagProcessor() {}

// Stub (retail body is 0x14D4 bytes, not matched yet). noinline keeps -ipa file
// from folding the empty body into the tag-writer family's bl sites.
__declspec(noinline) int func_8012615C(nw4r::lyt::AnimTransform* tag,
                                        nw4r::lyt::Pane* a,
                                        nw4r::lyt::Pane* b,
                                        nw4r::lyt::Pane* c){}

// noinline: func_80127BF4 keeps the bl copyVEC2 call in retail - a tiny
// same-TU body like this would otherwise be folded into the call site.
extern "C" __declspec(noinline) void copyVEC2(float *dst, const float *src) {
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

// Char-space accumulation helper (retail C-ABI symbol in this unit).
// noinline: the tag-writer family (func_80129128) calls it via bl in retail,
// so the tiny body must not be folded into call sites.
extern "C" __declspec(noinline) void addToCharSpace(u8* self, float val) { *(float*)(self + 0xC) = *(float*)(self + 0x4) + val; };

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

// Tag-processor message pump: check the next tag code at buf[field_810+2]; if
// the message is exhausted return 3, otherwise keep running the text-processing
// step func_8012615C until it reports <= 4 (page/section done).
#pragma optimize_for_size on
int func_801276F4(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                  nw4r::lyt::Pane* b, nw4r::lyt::Pane* c) {
    CTagProcMsg* msg = (CTagProcMsg*)tag;
    const u16* p = &msg->buf[msg->field_810];
    if (p[2] == 0)
        return 3;
    u32 r;
    do {
        r = (u32)func_8012615C(tag, a, b, c);
    } while (r > 4);
    return (int)r;
}
#pragma optimize_for_size off

// Stub (retail body 0x45C bytes, not matched yet). noinline keeps -ipa file
// from folding the empty body into the tag-writer family's bl sites.
__declspec(noinline) void func_80127764(void* tagProc, nw4r::lyt::Pane* a,
                                        nw4r::lyt::Pane* b, nw4r::lyt::Pane* c,
                                        int flag){}

// noinline: func_801287BC keeps the bl func_80127BC4 / copyVEC3 calls in
// retail - the tiny same-TU bodies would otherwise be folded into the call
// sites (same convention as copyVEC2 below).
extern "C" __declspec(noinline) void func_80127BC4(float* dst, const float* src) {
    dst[0] = src[0];
    dst[1] = src[1];
}

extern "C" __declspec(noinline) void copyVEC3(float* dst, const float* src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

// Tag-writer position clamp: copy the message string into a local buffer,
// measure the text position (func_80125D00), then y = c34*(y-cF4)+c30 with a
// low clamp and a snap-down to the fixed step table; write the VEC2 result
// to out->field_4C. The r3 arg is unused (family idiom).
// The y-transform and clamp must be written with the constants INLINE (no
// f32 temps): named temps shift MWCC's FPR birth order and swap c34/c30
// between f2/f3 (6 reg_swap).
void func_80127BF4(void* unused, CTagMsgView* msg, CTagOutView* out) {
    wchar_t buf[0x400];
    f32 xy[2];
    wcscpy(buf, msg->field_D8);
    func_80125D00(xy, msg, buf);
    xy[1] = lbl_eu_80667234 * (xy[1] - lbl_eu_806671F4) + lbl_eu_80667230;
    xy[0] += lbl_eu_80667238;
    if (xy[1] < lbl_eu_80667230)
        xy[1] = lbl_eu_80667230;
    f32 y = xy[1];
    if (y >= lbl_eu_8066723C)
        xy[1] = lbl_eu_8066723C;
    else if (y >= lbl_eu_80667240)
        xy[1] = lbl_eu_80667240;
    else if (y >= lbl_eu_80667244)
        xy[1] = lbl_eu_80667244;
    else if (y >= lbl_eu_80667248)
        xy[1] = lbl_eu_80667248;
    else if (y >= lbl_eu_8066724C)
        xy[1] = lbl_eu_8066724C;
    else if (y >= lbl_eu_80667230)
        xy[1] = lbl_eu_80667230;
    copyVEC2(&out->field_4C, xy);
}

// Tag-writer accessor: set CharWriter::mCursorPos (+0x2C/+0x30) with the
// same nw4r pointer validation as func_801291F4 (different Panic site/strings).
// Flags declared in reverse chain order, hi = 0xFF000000 mask reused by the
// 0x80/0xC0 checks only - the retail flag/mask colors.
void func_8012A070(nw4r::ut::TextWriterBase<wchar_t>* tw, float x, float y) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DE50, 0xf9, lbl_eu_8052DE1C, tw);
    }
    tw->cursorX = x;
    tw->cursorY = y;
}

// Tag-writer helper: measure a string's max line width and the font height.
// The string is copied into a scratch buffer (memset + wcscpy), then the
// ORIGINAL string is walked accumulating (font->GetCharWidth(c) + 2) into a
// running sum with a max tracked after the loop; the font height getter
// (vtable +0x34) is converted to float with the builtin 0x4330/xoris/lfd
// magic idiom (the magic pools to a TU-local @N label; retail shares it as
// the named sdata2 blob lbl_eu_80667200 - MWCC_REFERENCE 7i).
// optimize_for_size: retail saves r27-r31 + f28-f31 with _savegpr_27 (the
// -O4,s save shape).
#pragma optimize_for_size on
void func_80127D20(f32* out, void* unused, nw4r::lyt::TextBox* textbox,
                   const wchar_t* str) {
    wchar_t buf[0x400];
    memset(buf, 0, sizeof(buf));
    wcscpy(buf, str);
    nw4r::lyt::Font* font = textbox->GetFont();
    f32 sum = lbl_eu_806671F0;
    u32 i = 0;
    f32 max = sum;
    f32 height = (f32)font->v34();
    for (; ((const u16*)str)[i] != 0; i++) {
        sum += (f32)(font->v48(((const u16*)str)[i]) + 2);
    }
    if (sum > max)
        max = sum;
    out[0] = max;
    out[1] = height;
}
#pragma optimize_for_size off

// Message-pane text reset hook: free the old textbox string buffer, allocate
// a fresh 1024-char one, advance the tag-proc write position, run the message
// step (func_80127764) and clear the done flag. No explicit return: the
// retail leaves r3 as whatever func_80127764 left there.
void func_80127E74(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                   nw4r::lyt::Pane* b, nw4r::lyt::Pane* c) {
    CTalkTextBoxVtbl* tb = (CTalkTextBoxVtbl*)a;
    tb->v78();          // FreeStringBuffer()
    tb->v74(0x400);     // AllocStringBuffer(0x400)
    CTagProcMsg* msg = (CTagProcMsg*)tag;
    msg->field_810++;
    func_80127764(msg, a, b, c, 0);
    msg->field_820 = 0;
}

// Sound-effect variant of the tag processor (retail __ct__CTagProcessorSE):
// base-construct, then the SE-specific fields (byte at +0x834, u32 at +0x838).
void* __ct__CTagProcessorSE(void* self) {
    CTagProcessorSE* p = (CTagProcessorSE*)self;
    __ct__CTagProcessorBase(p);
    p->field_820 = 0;
    *(void**)p = (void*)lbl_eu_8052DA08;
    p->field_824 = lbl_eu_806671F0;
    p->field_828 = 0;
    p->field_82c = 0;
    p->field_830 = 0;
    p->field_834 = 0;
    p->field_838 = 0;
    p->field_83c = 0;
    return p;
}

// Deleting destructor for CTagProcessorSE (retail __dt__15CTagProcessorSEFv):
// member dtor of the derived class — base dtor call with flag 0, then
// operator delete when the deleting flag is set.
CTagProcessorSE::~CTagProcessorSE() {}

void func_80127FB4(){}

void func_801286E0(){}

// Talk-open hook: reset the message text pane's string buffer (free the old
// buffer, allocate a fresh 1024-char one), advance the tag-proc write position,
// run the message start step, and clear the done flag. No explicit return: the
// retail leaves r3 as whatever func_801287BC left there.
#pragma optimize_for_size on
int func_80128740(void* tagProc, nw4r::lyt::Pane* pane) {
    CTalkTextBoxVtbl* tb = (CTalkTextBoxVtbl*)pane;
    tb->v78();          // FreeStringBuffer()
    tb->v74(0x400);     // AllocStringBuffer(0x400)
    CTagProcMsg* msg = (CTagProcMsg*)tagProc;
    msg->field_810++;
    func_801287BC((CTagProcessorBase*)tagProc, pane, 0);
    msg->field_820 = 0;
}
#pragma optimize_for_size off

// Talk-open text setup: build the display string (either the caller's string
// or a scan of the message buffer from field_810 that stops at a control
// code (0/3/4) or 0x3FF chars, flagging a "1 after newline"), push it into
// the pane's +0x7C setter, pick the message-speed constant from the newline
// flag, measure the line position, and update the pane's position VEC3.
// -O4,s save shape (_savegpr_28).
#pragma optimize_for_size on
void func_801287BC(CTagProcessorBase* msg, nw4r::lyt::Pane* pane,
                   const wchar_t* str) {
    wchar_t buf2[0x400];
    f32 out[2];
    f32 out2[3];
    f32 v3[2];
    int newline = 0;
    memset(buf2, 0, sizeof(buf2));
    if (str == 0) {
        u16 ch;
        int nl = 0;
        u16 j = 0;
        u16 i = msg->field_810;
        while ((ch = msg->mBuf[i]) != 0 && ch != 3 && ch != 4) {
            if (i - msg->field_810 >= 0x3ff)
                break;
            if (ch == 0xA)
                nl++;
            if (ch == 1 && nl > 0)
                newline = 1;
            buf2[j] = ch;
            j++;
            i++;
        }
        buf2[j] = 0;
    } else {
        wcscpy(buf2, str);
    }
    TalkPaneView* pv = (TalkPaneView*)pane;
    pv->v7C((const u16*)buf2, 0);
    if (newline)
        pv->field_F0 = lbl_eu_80667254;
    else
        pv->field_F0 = lbl_eu_806671F8;
    func_80125D00(out, (CTagMsgView*)pane, buf2);
    if (newline)
        out[1] = lbl_eu_8066720C * out[1] + lbl_eu_80667254;
    else
        out[1] = lbl_eu_8066720C * out[1];
    func_801375A0(out2, pane);
    func_80127BC4(v3, pv->field_4C);
    f32 k = lbl_eu_80667228;
    v3[1] = (v3[1] - out[1]) * k;
    v3[0] = (v3[0] - out[0]) * k;
    out2[0] = v3[0];
    out2[1] = lbl_eu_80667258 - v3[1];
    copyVEC3(pv->vec_2C, out2);
    pv->v78();
    pv->v74(0x400);
}
#pragma optimize_for_size off

// Tag-writer family (small variants): init a TagParam block with a tag code
// and copy it into the output buffer, returning the r4 arg. The r3/r5 args
// are unused by these small variants (the larger siblings use the r5 code
// arg); the helpers are called via bl (extern "C" flat names, kept out of
// line with noinline - retail had them in a separate TU).
// NOTE: the r30/r31 saves come out as stw pairs + reversed moves under this
// unit's -O4,p; retail used -O4,s (stmw r30) - per-function opt-level wall
// (walls doc #13; same as func_801276C8's addic/subfe idiom).
extern "C" void* func_801289B4(void* a, void* b, u8 code, u8* dst) {
    TagParam p;
    func_80125944(&p, 7);
    copyTagParam(dst, (const u8*)&p);
    return b;
}

// Tag-writer variant with a numeric value: uppercase the value string, parse it
// with wcstol, store tag-param code 6 plus the value, copy the block into the
// output buffer, and thread the r4 arg through (same family as func_801289B4).
#pragma optimize_for_size on
extern "C" void* func_801289FC(void* a, void* b, u16* str, u8* dst) {
    func_801366F4(str);
    wchar_t* endptr = 0;
    TagParam p;
    long val = wcstol((const wchar_t*)str, &endptr, 10);
    func_8012591C(&p, 6, (u16)val);
    copyTagParam(dst, (const u8*)&p);
    return b;
}
#pragma optimize_for_size off

extern "C" void* func_80128A70(void* a, void* b, u8 code, u8* dst) {
    TagParam p;
    func_80125944(&p, 5);
    copyTagParam(dst, (const u8*)&p);
    return b;
}

void func_80128AB8(){}

// Same as func_801289FC but with tag-param code 4.
#pragma optimize_for_size on
extern "C" void* func_80128B0C(void* a, void* b, u16* str, u8* dst) {
    func_801366F4(str);
    wchar_t* endptr = 0;
    TagParam p;
    long val = wcstol((const wchar_t*)str, &endptr, 10);
    func_8012591C(&p, 4, (u16)val);
    copyTagParam(dst, (const u8*)&p);
    return b;
}
#pragma optimize_for_size off

void* callInitTagProc(void* arg1, void* arg2) {
    extern void func_8013BDE4(void*);
    func_8013BDE4(arg1);
    return arg2;
}

extern "C" void* func_80128BB0(void* a, void* b, u8 code, u8* dst) {
    TagParam p;
    func_80125944(&p, 3);
    copyTagParam(dst, (const u8*)&p);
    return b;
}

// Same as func_801289FC but with tag-param code 2.
#pragma optimize_for_size on
extern "C" void* func_80128BF8(void* a, void* b, u16* str, u8* dst) {
    func_801366F4(str);
    wchar_t* endptr = 0;
    TagParam p;
    long val = wcstol((const wchar_t*)str, &endptr, 10);
    func_8012591C(&p, 2, (u16)val);
    copyTagParam(dst, (const u8*)&p);
    return b;
}
#pragma optimize_for_size off

// Tag-param writer for a 1-value tag: split the arg string on ':' via
// func_801365E4, uppercase the first token and map it to a tag index via the
// tag-string table (offsets +0x00/+0x08/+0x14 select 0..2), then init a
// tag-param block: when a second token exists its decimal value is parsed
// with wcstol and converted to float (builtin s32->f32, byte-identical to
// retail's 0x4330/xoris/lfd/fsubs magic idiom - the magic pools to a TU-local
// @N label; retail shares it as lbl_eu_80667200, see the inline note),
// else the fixed f32 fallback constant is used. The block is copied into
// dst and the r4 arg is returned (family idiom).
// optimize_for_size: retail saves r26-r31 with stmw (the -O4,s save shape).
// tokens[16]: retail's frame places the token array at +0x28 and the
// conversion scratch at +0x68, which a 16-pointer array reproduces.
#pragma optimize_for_size on
void* func_80128C6C(void* unused, void* ret, wchar_t* str, TagParam* dst) {
    wchar_t* tokens[16];
    TagParam p;
    int count = func_801365E4((u16*)str, 0x3a, (u16**)tokens);
    u8 idx = 0;
    func_801366F4((u16*)tokens[0]);
    if (wcscmp(tokens[0], &lbl_eu_80661FC8[0]) == 0)
        idx = 0;
    else if (wcscmp(tokens[0], &lbl_eu_80661FC8[4]) == 0)
        idx = 1;
    else if (wcscmp(tokens[0], &lbl_eu_80661FC8[10]) == 0)
        idx = 2;
    if (count > 1) {
        func_801366F4((u16*)tokens[1]);
        wchar_t* endptr = 0;
        long val = wcstol(tokens[1], &endptr, 10);
        // Builtin s32->f32 conversion: byte-identical to retail's
        // lis/xoris/stw/lfd/fsubs magic idiom. The conversion's 2^52+2^31
        // magic constant pools to a TU-local @N label; retail shares it as
        // the named sdata2 blob lbl_eu_80667200 (MWCC_REFERENCE 7i - the
        // manual union form that names it breaks the schedule with an extra
        // fsub+frsp, so the builtin is the closest byte-identical state).
        initTagParam((u8*)&p, 1, idx, (f32)val);
        copyTagParam((u8*)dst, (const u8*)&p);
    } else {
        TagParam q;
        initTagParam((u8*)&q, 1, idx, lbl_eu_8066725C);
        copyTagParam((u8*)dst, (const u8*)&q);
    }
    return ret;
}
#pragma optimize_for_size off

// Color-tag writer: split the arg string on ':', copy the name/value tables
// from ROM (220/224-byte struct assignments - MWCC's mtctr/bdnz copy loop),
// match each token against the name table (max 0x40 entries, null-terminated)
// storing the matching value into the 3-slot scratch (seeded with the sdata2
// color defaults), then pack the scratch into a 3-u16 block <8, (b<<8)|r,
// (a<<8)|0xff> and return the position after the block.
// optimize_for_size: retail saves r21-r31 with stmw (the -O4,s save shape).
#pragma optimize_for_size on
u16* func_80128DA0(void* unused, u16* dst, wchar_t* str) {
    TagColorOut out;
    wchar_t* tokens[16];
    TagColorNames names;
    TagColorValues values;
    int count = func_801365E4((u16*)str, 0x3a, (u16**)tokens);
    values = lbl_eu_804FEFB8;
    names = lbl_eu_804FF488;
    out.w.c1 = lbl_eu_80667188;
    out.w.c2 = lbl_eu_8066718C;
    for (int i = 0; i < count; i++) {
        func_801366F4((u16*)tokens[i]);
        for (int j = 0; j < 0x40; j++) {
            if (names.names[j] == 0)
                break;
            if (wcscmp(tokens[i], names.names[j]) == 0) {
                out.s[i] = (s16)values.values[j];
                break;
            }
        }
    }
    dst[0] = 8;
    dst[1] = (u16)(((u8)out.s[0] << 8) | (u8)out.s[1]);
    dst[2] = (u16)(((u8)out.s[2] << 8) | 0xff);
    return dst + 3;
}
#pragma optimize_for_size off

// Tag-code writer (code 7): split the arg string on '/', record each token's
// length, write the 2-entry header (tag + token count), then a (count+1)/2
// block of length pairs (each u16 packs two consecutive token lengths), then
// copy every token into dst in order. Returns the end of the copied region.
// optimize_for_size: retail saves r26-r31 with stmw (the -O4,s save shape);
// the unsigned trip shift is what retail uses (rlwinm, not srawi).
#pragma optimize_for_size on
u16* func_80128EF8(void* unused, u16* dst, wchar_t* str) {
    wchar_t* tokens[16];
    int count = func_801365E4((u16*)str, 0x2f, (u16**)tokens);
    u8 lens[16];
    memset(lens, 0, sizeof(lens));
    int i;
    for (i = 0; i < count; i++)
        lens[i] = (u8)wcslen(tokens[i]);
    dst[0] = 7;
    dst[1] = (u16)(count & 0xff);
    dst += 2;
    // Step-2 loop over count: retail trips (count+1)>>1 with the count>0
    // guard (cmpwi count), and the byte index advancing by 2 per iter.
    for (i = 0; i < count; i += 2)
        *dst++ = (u16)((lens[i] << 8) | lens[i + 1]);
    for (i = 0; i < count; i++) {
        wcscpy((wchar_t*)dst, tokens[i]);
        dst += lens[i];
    }
    return dst;
}
#pragma optimize_for_size off

// Tag-code writer (code 6): same shape as func_80128EF8 but with tag code 6.
#pragma optimize_for_size on
u16* func_80129008(void* unused, u16* dst, wchar_t* str) {
    wchar_t* tokens[16];
    int count = func_801365E4((u16*)str, 0x2f, (u16**)tokens);
    u8 lens[16];
    memset(lens, 0, sizeof(lens));
    int i;
    for (i = 0; i < count; i++)
        lens[i] = (u8)wcslen(tokens[i]);
    dst[0] = 6;
    dst[1] = (u16)(count & 0xff);
    dst += 2;
    // Step-2 loop over count: retail trips (count+1)>>1 with the count>0
    // guard (cmpwi count), and the byte index advancing by 2 per iter.
    for (i = 0; i < count; i += 2)
        *dst++ = (u16)((lens[i] << 8) | lens[i + 1]);
    for (i = 0; i < count; i++) {
        wcscpy((wchar_t*)dst, tokens[i]);
        dst += lens[i];
    }
    return dst;
}
#pragma optimize_for_size off

extern "C" u16* writeTagCode0D(void* dummy, u16* buf) {
    *buf = 0xD;
    return buf + 1;
}

// Tag-writer: measure a message line. Reads the TextWriter from the holder,
// sizes the writer's font to the fixed sdata2 constant, measures the two-char
// speed-up string "++" (tag table +0x1a), restores the previous font size,
// and stores the result back into the char-space context. Returns 2.
#pragma optimize_for_size on
int func_80129128(void* unused, TagCharContext* ctx, void* unused2,
                  TagWriterHolder* holder) {
    nw4r::ut::TextWriterBase<wchar_t>* tw = holder->field_00;
    f32 sp = func_801291F4(tw);
    func_8012930C(tw, lbl_eu_80667260);
    f32 w = tw->CalcStringWidth(&lbl_eu_80661FC8[0xd], 2);
    func_8012930C(tw, sp);
    func_80129430(tw, w);
    f32 h = func_80129564(holder->field_00);
    ctx->field_04 = h;
    ctx->field_08 = ctx->field_00 + w;
    addToCharSpace((u8*)ctx, tw->GetFontHeight());
    return 2;
}
#pragma optimize_for_size off

// Tag-writer accessor: TextWriterBase::GetWidthLimit (+0x4C) with the nw4r
// pointer validation used across the tag-writer family - the writer must live
// in one of the game's known memory regions (MEM1 0x80..., MEM2 0x90..., I/O
// 0xC0/0xC1..., 0xD0..., 0xE0...), otherwise nw4r::db::Panic. The cascading
// flag chain (each region check gated on the previous one failing) is the
// retail shape. Two regalloc levers made it byte-identical: flags declared in
// REVERSE chain order (retail colors validRegs2=r5 .. validMem1=r10) and the
// 0xFF000000 mask kept in a short-lived `hi` local (only the 0x80/0xC0 checks
// use it - retail r11) while the other masks are recomputed from tw per check
// so they die per-condition.
__declspec(noinline) float func_801291F4(nw4r::ut::TextWriterBase<wchar_t>* tw) {
    // nw4r pointer validation: panic unless the writer lives in one of the
    // known memory regions. The cascading if-chain (each check gated on the
    // previous one failing) is the retail shape. Flags are declared in
    // reverse chain order (retail colors validRegs2=r5 .. validMem1=r10);
    // hi is the 0xFF000000 mask reused by the 0x80/0xC0 checks only (retail
    // keeps it in r11); the other masks are recomputed from tw so they die
    // per-condition.
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DBE0, 0x84, lbl_eu_8052DBAC, tw);
    }
    return tw->widthLimit;
}

// Tag-writer accessor: TextWriterBase::SetWidthLimit (+0x4C) with the same
// nw4r pointer validation as func_801291F4 (different Panic site/strings).
// Flags declared in reverse chain order, hi = 0xFF000000 mask reused by the
// 0x80/0xC0 checks only - the retail flag/mask colors.
__declspec(noinline) void func_8012930C(nw4r::ut::TextWriterBase<wchar_t>* tw, float v) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DB50, 0x7b, lbl_eu_8052DB1C, tw);
    }
    tw->widthLimit = v;
}

// Tag-writer accessor: add to CharWriter::mCursorPos.x (+0x2C) with the same
// nw4r pointer validation as func_801291F4 (different Panic site/strings).
__declspec(noinline) void func_80129430(nw4r::ut::TextWriterBase<wchar_t>* tw, float v) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DED8, 0x12d, lbl_eu_8052DEA4, tw);
    }
    tw->cursorX += v;
}

// Tag-writer accessor: CharWriter::GetCursorY (+0x30) with the same pointer
// validation as func_801291F4 (different Panic site/strings). Flags declared
// in reverse chain order, hi = 0xFF000000 mask reused by the 0x80/0xC0 checks
// only - the retail flag/mask colors.
__declspec(noinline) float func_80129564(nw4r::ut::TextWriterBase<wchar_t>* tw) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DF60, 0x141, lbl_eu_8052DF2C, tw);
    }
    return tw->cursorY;
}

// Converted to TagContext::addToCharSpace

// Tag-writer: copy the active writer into a scratch, configure it (font-size
// field, width limit, scale, cursor), print the one-char marker at the tag
// table +0x20, then advance the real writer's cursor by the scratch's new
// cursor x plus the doubled font-slot value. Two nw4r pointer-validation
// chains (on the scratch and on the real writer) bracket the measurement.
// The scratch's dtor is emitted automatically at scope exit. Returns 2.
int func_8012968C(void* unused, void* unused2, TagWriterHolder* holder) {
    nw4r::ut::TextWriterBase<wchar_t>* tw = holder->field_00;
    f32 x = func_801299D4(tw);
    f32 y = func_80129564(tw);
    f32 sx = func_80129AEC(tw);
    f32 sy = func_80129C04(tw);
    nw4r::ut::TextWriterBase<wchar_t> local;
    func_80129D1C((TagCopyBlock*)&local, (const TagCopyBlock*)tw);
    func_80129E20(&local, 0x300);
    func_8012930C(&local, lbl_eu_80667260);
    func_80129F3C(&local, lbl_eu_806671F8 * sx, sy);
    func_8012A070(&local, x, y);
    local.Print(&lbl_eu_80661FC8[0x10], 1);
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)&local & 0xFF000000;
    if (hi != 0x80000000 && ((u32)&local & 0xFF800000) != 0x81000000)
        validMem1 = false;
    if (!validMem1 && ((u32)&local & 0xF8000000) != 0x90000000)
        validMem2 = false;
    if (!validMem2 && hi != 0xC0000000)
        validIo = false;
    if (!validIo && ((u32)&local & 0xFF800000) != 0xC1000000)
        validIo2 = false;
    if (!validIo2 && ((u32)&local & 0xF8000000) != 0xD0000000)
        validRegs = false;
    if (!validRegs && ((u32)&local & 0xFFFFC000) != 0xE0000000)
        validRegs2 = false;
    if (!validRegs2)
        nw4r::db::Panic(lbl_eu_8052DCB8, 0x47, lbl_eu_8052DC84, &local);
    int fw = local.mFont->v48(0x2500);
    f32 conv = (f32)(fw << 1);
    func_8012A070(&local, x + conv, y);
    f32 cx = func_801299D4(&local);
    bool validRegs2b = true;
    bool validRegsb = true;
    bool validIo2b = true;
    bool validIob = true;
    bool validMem2b = true;
    bool validMem1b = true;
    u32 hi2 = (u32)tw & 0xFF000000;
    if (hi2 != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000)
        validMem1b = false;
    if (!validMem1b && ((u32)tw & 0xF8000000) != 0x90000000)
        validMem2b = false;
    if (!validMem2b && hi2 != 0xC0000000)
        validIob = false;
    if (!validIob && ((u32)tw & 0xFF800000) != 0xC1000000)
        validIo2b = false;
    if (!validIo2b && ((u32)tw & 0xF8000000) != 0xD0000000)
        validRegsb = false;
    if (!validRegsb && ((u32)tw & 0xFFFFC000) != 0xE0000000)
        validRegs2b = false;
    if (!validRegs2b)
        nw4r::db::Panic(lbl_eu_8052DE94, 0x11e, lbl_eu_8052DE60, tw);
    tw->cursorX = cx;
    return 2;
}

// Tag-writer accessor: CharWriter::GetCursorX (+0x2C) with the same pointer
// validation as func_801291F4 (different Panic site/strings). Flags declared
// in reverse chain order, hi = 0xFF000000 mask reused by the 0x80/0xC0 checks
// only - the retail flag/mask colors.
__declspec(noinline) float func_801299D4(nw4r::ut::TextWriterBase<wchar_t>* tw) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DF1C, 0x13c, lbl_eu_8052DEE8, tw);
    }
    return tw->cursorX;
}

// Tag-writer accessor: CharWriter scale-x (+0x24) with the same nw4r
// pointer validation as func_801291F4 (different Panic site/strings). Flags
// declared in reverse chain order, hi = 0xFF000000 mask reused by the
// 0x80/0xC0 checks only - the retail flag/mask colors.
__declspec(noinline) float func_80129AEC(nw4r::ut::TextWriterBase<wchar_t>* tw) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DDC8, 0xb8, lbl_eu_8052DD94, tw);
    }
    return tw->field_24;
}

// Tag-writer accessor: CharWriter scale-y (+0x28) with the same nw4r
// pointer validation as func_801291F4 (different Panic site/strings).
__declspec(noinline) float func_80129C04(nw4r::ut::TextWriterBase<wchar_t>* tw) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DE0C, 0xbd, lbl_eu_8052DDD8, tw);
    }
    return tw->field_28;
}

// extern "C" + noinline: the tag-writer family (func_8012B070) calls it via
// bl in retail; noinline keeps -ipa from folding the 0x68-byte struct copy
// into the call site (the retail kept it as a real 0x104-byte call).
extern "C" __declspec(noinline) void func_80129D1C(TagCopyBlock* dst, const TagCopyBlock* src) {
    *dst = *src;
}

// Tag-writer accessor: store into TextWriterBase +0x5C (mFontSize.y region)
// with the same nw4r pointer validation as func_801291F4. This one keeps
// both tw and value live across the Panic call, so MWCC saves r30/r31
// (stmw prologue) instead of just r31.
// optimize_for_size: retail saves r30/r31 with a single stmw/lmw (the -O4,s
// save shape); under the unit's -O4,p they split into stw pairs with
// reversed moves (same artifact as func_801289B4).
#pragma optimize_for_size on
__declspec(noinline) void func_80129E20(nw4r::ut::TextWriterBase<wchar_t>* tw, u32 value) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DC70, 0x8b, lbl_eu_8052DC3C, tw);
    }
    tw->field_5C = value;
}
#pragma optimize_for_size off

// Tag-writer accessor: CharWriter scale set (+0x24/+0x28, mScale region)
// with the same nw4r pointer validation as func_801291F4 (different Panic
// site/strings).
__declspec(noinline) void func_80129F3C(nw4r::ut::TextWriterBase<wchar_t>* tw, float x, float y) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DD84, 0xab, lbl_eu_8052DD50, tw);
    }
    tw->field_24 = x;
    tw->field_28 = y;
}

// Tag-code writer (code 9): uppercase the arg string, compare it against the
// tag-string table entries at +0x24 / +0x2a, write <9, value> into dst, and
// return the position after the block. The r3 arg is unused (family idiom).
// optimize_for_size: retail saves r28-r31 with stmw (the -O4,s save shape).
#pragma optimize_for_size on
u16* func_8012A1A4(void* a, u16* dst, wchar_t* str) {
    func_801366F4((u16*)str);
    s16 v = 0xff;
    const wchar_t* tbl = lbl_eu_80661FC8;
    if (wcscmp(str, tbl + 0x12) == 0)
        v = 0;
    else if (wcscmp(str, tbl + 0x15) == 0)
        v = -1;
    dst[0] = 9;
    dst[1] = (u16)v;
    return dst + 2;
}
#pragma optimize_for_size off

// Tag-code writer (code 8, name selection): uppercase the arg string, map it
// to a value 1..9 via the tag-string table, init a tag-param block with code
// 8, copy it into dst, then tag the block with the selected value and the
// "has value" bit. Returns the r4 arg.
// optimize_for_size: retail saves r27-r31 with stmw (the -O4,s save shape).
#pragma optimize_for_size on
void* func_8012A224(void* unused, void* ret, wchar_t* str, TagParam* dst) {
    u8 v = 0;
    func_801366F4((u16*)str);
    if (wcscmp(str, &lbl_eu_80661FC8[0x19]) == 0)
        v = 1;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x1f]) == 0)
        v = 2;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x27]) == 0)
        v = 3;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x2e]) == 0)
        v = 4;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x34]) == 0)
        v = 5;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x3c]) == 0)
        v = 6;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x43]) == 0)
        v = 7;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x49]) == 0)
        v = 8;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x51]) == 0)
        v = 9;
    TagParam p;
    func_80125944(&p, 8);
    copyTagParam((u8*)dst, (const u8*)&p);
    dst->field_03 = v;
    dst->field_01 |= 0x2;
    return ret;
}
#pragma optimize_for_size off

// Tag-code writer (code 8): uppercase the arg string, pick a value from the
// tag-string table by comparison (offsets +0xb0/+0xb8/+0xc4/+0xcc select
// 0..3), init a tag-param block with code 8, copy it into dst, then tag the
// block with the selected value and the "has value" bit. Returns the r4 arg.
#pragma optimize_for_size on
void* func_8012A388(void* unused, void* ret, wchar_t* str, TagParam* dst) {
    u8 v = 0;
    func_801366F4((u16*)str);
    if (wcscmp(str, &lbl_eu_80661FC8[0x58]) == 0)
        v = 0;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x5c]) == 0)
        v = 1;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x62]) == 0)
        v = 2;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x66]) == 0)
        v = 3;
    TagParam p;
    func_80125944(&p, 8);
    copyTagParam((u8*)dst, (const u8*)&p);
    dst->field_02 = v;
    dst->field_01 |= 0x4;
    return ret;
}
#pragma optimize_for_size off

// Tag-code writer (code 0xB): split the arg string on ':', uppercase token 0
// and map it to a 1..2 tag id via the tag-string table (+0xd8/+0xdc), parse
// token 1 as the numeric value, then map token 2 through the 26-entry BDAT
// lookup chain (each match yields func_80136130(lbl_eu_804FFC48,
// &lbl_eu_804FFC48[0xc], n)) or a wcstol fallback. Tokens 3/4 (when present)
// are decimal values. Writes <0xb, (tag1<<8)|0xff, val, tag2, tag3, tag4>
// when the split produced >= 3 tokens and returns the position after the
// block.
// optimize_for_size: retail saves r23-r31 with stmw (the -O4,s save shape).
#pragma optimize_for_size on
u16* func_8012A460(void* unused, u16* out, wchar_t* str) {
    wchar_t* tokens[16];
    int count = func_801365E4((u16*)str, 0x3a, (u16**)tokens);
    u8 tag1 = 0xff;
    int tag3 = 0xff;
    int tag4 = 0xff;
    func_801366F4((u16*)tokens[0]);
    if (wcscmp(tokens[0], &lbl_eu_80661FC8[0x6c]) == 0)
        tag1 = 1;
    else if (wcscmp(tokens[0], &lbl_eu_80661FC8[0x6e]) == 0)
        tag1 = 2;
    func_801366F4((u16*)tokens[1]);
    wchar_t* end1 = 0;
    int val = (int)wcstol(tokens[1], &end1, 10);
    func_801366F4((u16*)tokens[2]);
    int tag2;
    if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x70]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 1);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x72]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 2);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x74]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 3);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x76]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 4);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x78]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 5);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x6e]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 6);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x7a]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 7);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x7c]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 8);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x7e]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 9);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x80]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xa);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x82]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xb);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x84]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xc);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x86]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xd);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x88]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xe);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x8a]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xf);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x6c]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x10);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x8c]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x11);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x8e]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x12);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x90]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x13);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x92]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x14);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x94]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x15);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x96]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x16);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x98]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x17);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x9a]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x18);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x9c]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x19);
    else if (wcscmp(tokens[2], &lbl_eu_80661FC8[0x9e]) == 0)
        tag2 = func_80136130(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x1a);
    else {
        wchar_t* end2 = 0;
        tag2 = wcstol(tokens[2], &end2, 10);
    }
    if (count > 3) {
        func_801366F4((u16*)tokens[3]);
        wchar_t* end3 = 0;
        tag3 = (int)wcstol(tokens[3], &end3, 10);
    }
    if (count > 4) {
        func_801366F4((u16*)tokens[4]);
        wchar_t* end4 = 0;
        tag4 = (int)wcstol(tokens[4], &end4, 10);
    }
    if (count >= 3) {
        out[0] = 0xb;
        out[1] = (u16)((tag1 << 8) | 0xff);
        out[2] = val;
        out[3] = (u16)tag2;
        out[4] = tag3;
        out[5] = tag4;
        out += 6;
    }
    return out;
}
#pragma optimize_for_size off

// Color-name tag writer: uppercase the arg string, map known color names
// from the tag table to packed RGB values, else parse a '#'-prefixed digit
// string (digits '1'..'?' accumulate value nibbles, 'A'..'F' are skipped,
// alpha is forced to 0xFF), then write the <2, hi, lo> block.
// -O4,s save shape (stmw r28).
#pragma optimize_for_size on
u16* func_8012AAA4(void* a, u16* out, wchar_t* str) {
    func_801366F4((u16*)str);
    u32 v = 0;
    if (wcscmp(str, &lbl_eu_80661FC8[0x58]) == 0)
        v = 0x483A21FF;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x76]) == 0)
        v = 0x483A21FF;
    else if (wcscmp(str, &lbl_eu_80661FC8[0xa0]) == 0)
        v = -0x501;
    else if (wcscmp(str, &lbl_eu_80661FC8[0xa5]) == 0)
        v = -0x501;
    else if (wcscmp(str, &lbl_eu_80661FC8[0xa8]) == 0)
        v = 0x483A21FF;
    else if (wcscmp(str, &lbl_eu_80661FC8[0xad]) == 0)
        v = 0x483A21FF;
    else if (wcscmp(str, &lbl_eu_80661FC8[0xb0]) == 0)
        v = 0xFF0000FF;
    else if (wcscmp(str, &lbl_eu_80661FC8[0xb4]) == 0)
        v = 0xC81E1EFF;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x7e]) == 0)
        v = 0xC81E1EFF;
    else if (wcscmp(str, &lbl_eu_80661FC8[0xb9]) == 0)
        v = 0xC81E1EFF;
    else if (wcscmp(str, &lbl_eu_80661FC8[0xbc]) == 0)
        v = 0x509CCCFF;
    else if (str[0] == 0x23) {
        int len = wcslen(str + 1);
        for (int i = 0; i < len; i++) {
            u16 ch = str[i + 1];
            switch (ch) {
            case '1': v += 1; break;
            case '2': v += 2; break;
            case '3': v += 3; break;
            case '4': v += 4; break;
            case '5': v += 5; break;
            case '6': v += 6; break;
            case '7': v += 7; break;
            case '8': v += 8; break;
            case '9': v += 9; break;
            case ':': v += 10; break;
            case ';': v += 11; break;
            case '<': v += 12; break;
            case '=': v += 13; break;
            case '>': v += 14; break;
            case '?': v += 15; break;
            case 'A': v += 0; break;
            case 'B': v += 0; break;
            case 'C': v += 0; break;
            case 'D': v += 0; break;
            case 'E': v += 0; break;
            case 'F': v += 0; break;
            default: break;
            }
            v <<= 4;
        }
        v <<= 4;
        v += 0xFF;
    }
    out[0] = 2;
    out[1] = (u16)(v >> 16);
    out[2] = (u16)v;
    return out + 3;
}
#pragma optimize_for_size off

extern "C" u32 func_8012AD2C(void* a, void* b, void* c, u8* p) {
    void* base = *(void**)(p + 4);
    *(void**)(p + 4) = (u8*)base + 4;
    return 0;
}

// Color-tag writer: apply the source color to the CharWriter, preserving the
// writer's current alpha. Two nw4r-style pointer-validation chains (each
// with its own Panic site) sandwich the alpha merge; the vertex color is
// committed via the out-of-line Color::operator= and
// CharWriter::UpdateVertexColor, then the source pointer advances.
// -O4,s save shape (stmw r30).
#pragma optimize_for_size on
u32 func_8012AD40(void* a, void* b, TagColorArg* arg) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    nw4r::ut::CharWriter* w = arg->field_00;
    nw4r::ut::Color c = *arg->field_04;
    u32 hi = (u32)w & 0xFF000000;
    if (hi != 0x80000000 && ((u32)w & 0xFF800000) != 0x81000000)
        validMem1 = false;
    if (!validMem1 && ((u32)w & 0xF8000000) != 0x90000000)
        validMem2 = false;
    if (!validMem2 && hi != 0xC0000000)
        validIo = false;
    if (!validIo && ((u32)w & 0xFF800000) != 0xC1000000)
        validIo2 = false;
    if (!validIo2 && ((u32)w & 0xF8000000) != 0xD0000000)
        validRegs = false;
    if (!validRegs && ((u32)w & 0xFFFFC000) != 0xE0000000)
        validRegs2 = false;
    if (!validRegs2)
        nw4r::db::Panic(lbl_eu_8052DD40, 0x98, lbl_eu_8052DD0C, w);
    nw4r::ut::Color cur = w->mVertexColor;
    c.a = cur.a;
    nw4r::ut::Color merged = c;
    validRegs2 = true;
    validRegs = true;
    validIo2 = true;
    validIo = true;
    validMem2 = true;
    validMem1 = true;
    w = arg->field_00;
    hi = (u32)w & 0xFF000000;
    if (hi != 0x80000000 && ((u32)w & 0xFF800000) != 0x81000000)
        validMem1 = false;
    if (!validMem1 && ((u32)w & 0xF8000000) != 0x90000000)
        validMem2 = false;
    if (!validMem2 && hi != 0xC0000000)
        validIo = false;
    if (!validIo && ((u32)w & 0xFF800000) != 0xC1000000)
        validIo2 = false;
    if (!validIo2 && ((u32)w & 0xF8000000) != 0xD0000000)
        validRegs = false;
    if (!validRegs && ((u32)w & 0xFFFFC000) != 0xE0000000)
        validRegs2 = false;
    if (!validRegs2)
        nw4r::db::Panic(lbl_eu_8052DCFC, 0x87, lbl_eu_8052DCC8, w);
    w->mVertexColor = merged;
    w->UpdateVertexColor();
    arg->field_04++;
    return 0;
}
#pragma optimize_for_size off

// Tag-code writer (code 1): split the arg string on '/' via func_801365E4,
// record each token's length, write a 2-entry header (count + combined first
// two lengths), then copy every token into dst in order, advancing by each
// token's stored length. Returns the end of the copied region.
#pragma optimize_for_size on
u16* func_8012AF90(void* unused, u16* dst, wchar_t* str) {
    wchar_t* tokens[16];
    int count = func_801365E4((u16*)str, 0x2f, (u16**)tokens);
    u8 lens[16];
    memset(lens, 0, sizeof(lens));
    int i;
    for (i = 0; i < count; i++)
        lens[i] = (u8)wcslen(tokens[i]);
    dst[0] = 1;
    dst[1] = (u16)((lens[0] << 8) | lens[1]);
    dst += 2;
    for (i = 0; i < count; i++) {
        wcscpy((wchar_t*)dst, tokens[i]);
        dst += lens[i];
    }
    return dst;
}
#pragma optimize_for_size off

// Tag-writer line layout: copy the active TextWriter into a local scratch
// writer (no ctor - -ipa drops the dead __ct__ since func_80129D1C
// overwrites every byte), zero its font-size field, scale it by the sdata2
// factor, measure two adjacent strings from the tag buffer (lengths are the
// low/high bytes of the u16 header; the second string starts 2*len2 bytes in),
// add the max width (plus the writer's char space unless bit0 of field_10 is
// set) to the real writer's cursor, write out->field_08, advance the buffer
// pointer past both strings, destroy the scratch, and return 2.
// optimize_for_size: retail saves r24-r31 + f30/f31 with _savegpr_24 (the
// -O4,s save shape).
#pragma optimize_for_size on
int func_8012B070(void* unused, TagLineOutView* out, void* unused2,
                  TagWriterHolder* holder) {
    nw4r::ut::TextWriterBase<wchar_t>* tw = holder->field_00;
    const u8* buf = holder->field_04;
    u16 h = *(const u16*)buf;
    const wchar_t* s2 = (const wchar_t*)(buf + 2);
    u32 off = (h >> 7) & 0x1FE;
    const wchar_t* s1 = (const wchar_t*)(buf + 2 + off);
    u32 len1 = h & 0xFF;
    u32 len2 = (h >> 8) & 0xFF;
    nw4r::ut::TextWriterBase<wchar_t> local;
    func_80129D1C((TagCopyBlock*)&local, (const TagCopyBlock*)tw);
    func_80129E20(&local, 0);
    func_8012B204(&local, lbl_eu_806671F0);
    f32 sy = func_80129C04(&local);
    f32 ys = lbl_eu_80667264 * sy;
    f32 sx = func_80129AEC(&local);
    func_80129F3C(&local, lbl_eu_80667264 * sx, ys);
    func_8012930C(&local, lbl_eu_80667260);
    f32 wlimit = func_801291F4(tw);
    func_8012930C(tw, lbl_eu_80667260);
    f32 w1 = tw->CalcStringWidth(s1, (int)len1);
    f32 w2 = local.CalcStringWidth(s2, (int)len2);
    if (w2 > w1)
        w1 = w2;
    f32 cs;
    if (!(holder->field_10 & 1))
        cs = func_8012B328(tw);
    else
        cs = lbl_eu_806671F0;
    w1 += cs;
    func_8012930C(tw, wlimit);
    func_80129430(tw, w1);
    out->field_08 = out->field_00 + w1;
    f32 fh = tw->GetFontHeight();
    addToCharSpace((u8*)out, fh);
    holder->field_04 = (const u8*)s1 + 2 * len1;
    return 2;
}
#pragma optimize_for_size off

// Tag-writer accessor: TextWriterBase::SetCharSpace (+0x50) with the same
// nw4r pointer validation as func_801291F4 (different Panic site/strings).
__declspec(noinline) void func_8012B204(nw4r::ut::TextWriterBase<wchar_t>* tw, float v) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DC28, 0x62, lbl_eu_8052DBF4, tw);
    }
    tw->charSpace = v;
}

// Tag-writer accessor: TextWriterBase::GetCharSpace (+0x50) with the same
// nw4r pointer validation as func_801291F4 (different Panic site/strings).
__declspec(noinline) float func_8012B328(nw4r::ut::TextWriterBase<wchar_t>* tw) {
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)tw & 0xFF000000;
    if (hi != 0x80000000 && ((u32)tw & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)tw & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)tw & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)tw & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)tw & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DB98, 0x6c, lbl_eu_8052DB64, tw);
    }
    return tw->charSpace;
}

void func_8012B440(){}

// Tag-code writer (code 4): same shape as func_8012A1A4, but the first match
// selects -1 and the second selects 0 (strings at +0x17e / +0x186).
#pragma optimize_for_size on
u16* func_8012B8C4(void* a, u16* dst, wchar_t* str) {
    func_801366F4((u16*)str);
    s16 v = 0xff;
    const wchar_t* tbl = lbl_eu_80661FC8;
    if (wcscmp(str, tbl + 0xbf) == 0)
        v = -1;
    else if (wcscmp(str, tbl + 0xc3) == 0)
        v = 0;
    dst[0] = 4;
    dst[1] = (u16)v;
    return dst + 2;
}
#pragma optimize_for_size off

// Tag-code writer (code 5): uppercase the arg string, then map it to a value
// via the tag-string table (+0x190 -> -1, +0x198 -> -2, otherwise the
// decimal parse of the string). Writes <5, value> into dst and returns the
// position after the block. The r3 arg is unused (family idiom).
#pragma optimize_for_size on
u16* func_8012B944(void* unused, u16* dst, wchar_t* str) {
    func_801366F4((u16*)str);
    s32 v;
    if (wcscmp(str, &lbl_eu_80661FC8[0xc8]) == 0)
        v = -1;
    else if (wcscmp(str, &lbl_eu_80661FC8[0xcc]) == 0)
        v = -2;
    else {
        wchar_t* endptr = 0;
        v = wcstol(str, &endptr, 10);
    }
    dst[0] = 5;
    dst[1] = (u16)v;
    return dst + 2;
}
#pragma optimize_for_size off

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


// Tag-code singleton ctors (retail C-ABI names, declared in the header):
// each stores its class vtable (retail .data:0x8052D618..0x8052D9E0) into
// the 4-byte sbss singleton. The classes themselves are the anonymous-
// namespace CTagCode subclasses whose deleting dtors are the
// __dt___unnamed_CTagProcessor_cpp_CTagCode* stubs at the top of the file.
// noinline: sinit_8012BA00 calls each via bl in retail - the tiny bodies
// must not be folded into the sinit's call sites.
__declspec(noinline) void __ct__CTagCodeCL(void* self) { *(void**)self = (void*)lbl_eu_8052D9E0; }
__declspec(noinline) void __ct__CTagCodeCP(void* self) { *(void**)self = (void*)lbl_eu_8052D9B8; }
__declspec(noinline) void __ct__CTagCodeWait(void* self) { *(void**)self = (void*)lbl_eu_8052D990; }
__declspec(noinline) void __ct__CTagCodeDel(void* self) { *(void**)self = (void*)lbl_eu_8052D968; }
__declspec(noinline) void __ct__CTagCodeRuby(void* self) { *(void**)self = (void*)lbl_eu_8052D940; }
__declspec(noinline) void __ct__CTagCodeCol(void* self) { *(void**)self = (void*)lbl_eu_8052D918; }
__declspec(noinline) void __ct__CTagCodeTrust(void* self) { *(void**)self = (void*)lbl_eu_8052D898; }
__declspec(noinline) void __ct__CTagCodeType(void* self) { *(void**)self = (void*)lbl_eu_8052D870; }
__declspec(noinline) void __ct__CTagCodePos(void* self) { *(void**)self = (void*)lbl_eu_8052D848; }
__declspec(noinline) void __ct__CTagCodeMouth(void* self) { *(void**)self = (void*)lbl_eu_8052D820; }
__declspec(noinline) void __ct__CTagCodeLine(void* self) { *(void**)self = (void*)lbl_eu_8052D7F8; }
__declspec(noinline) void __ct__CTagCodeSelect(void* self) { *(void**)self = (void*)lbl_eu_8052D7D0; }
__declspec(noinline) void __ct__CTagCodeSelect2(void* self) { *(void**)self = (void*)lbl_eu_8052D7A8; }
__declspec(noinline) void __ct__CTagCodeAct(void* self) { *(void**)self = (void*)lbl_eu_8052D780; }
__declspec(noinline) void __ct__CTagCodeFade(void* self) { *(void**)self = (void*)lbl_eu_8052D758; }
__declspec(noinline) void __ct__CTagCodeQuest(void* self) { *(void**)self = (void*)lbl_eu_8052D730; }
__declspec(noinline) void __ct__CTagCodeInvisible(void* self) { *(void**)self = (void*)lbl_eu_8052D708; }
__declspec(noinline) void __ct__CTagCodeQstStart(void* self) { *(void**)self = (void*)lbl_eu_8052D6E0; }
__declspec(noinline) void __ct__CTagCodePresent(void* self) { *(void**)self = (void*)lbl_eu_8052D6B8; }
__declspec(noinline) void __ct__CTagCodeTailOff(void* self) { *(void**)self = (void*)lbl_eu_8052D690; }
__declspec(noinline) void __ct__CTagCodeExchange(void* self) { *(void**)self = (void*)lbl_eu_8052D668; }
__declspec(noinline) void __ct__CTagCodeShop(void* self) { *(void**)self = (void*)lbl_eu_8052D640; }
__declspec(noinline) void __ct__CTagCodeMakeCrystal(void* self) { *(void**)self = (void*)lbl_eu_8052D618; }

// Static initializer: construct the 23 tag-code singletons in tag order
// (retail .ctors registration; retail C-ABI ctor names).
void sinit_8012BA00() {
    __ct__CTagCodeCL(&lbl_eu_80663FE0);
    __ct__CTagCodeCP(&lbl_eu_80663FE4);
    __ct__CTagCodeWait(&lbl_eu_80663FE8);
    __ct__CTagCodeDel(&lbl_eu_80663FEC);
    __ct__CTagCodeRuby(&lbl_eu_80663FF0);
    __ct__CTagCodeCol(&lbl_eu_80663FF4);
    __ct__CTagCodeTrust(&lbl_eu_80663FF8);
    __ct__CTagCodeType(&lbl_eu_80663FFC);
    __ct__CTagCodePos(&lbl_eu_80664000);
    __ct__CTagCodeMouth(&lbl_eu_80664004);
    __ct__CTagCodeLine(&lbl_eu_80664008);
    __ct__CTagCodeSelect(&lbl_eu_8066400C);
    __ct__CTagCodeSelect2(&lbl_eu_80664010);
    __ct__CTagCodeAct(&lbl_eu_80664014);
    __ct__CTagCodeFade(&lbl_eu_80664018);
    __ct__CTagCodeQuest(&lbl_eu_8066401C);
    __ct__CTagCodeInvisible(&lbl_eu_80664020);
    __ct__CTagCodeQstStart(&lbl_eu_80664024);
    __ct__CTagCodePresent(&lbl_eu_80664028);
    __ct__CTagCodeTailOff(&lbl_eu_8066402C);
    __ct__CTagCodeExchange(&lbl_eu_80664030);
    __ct__CTagCodeShop(&lbl_eu_80664034);
    __ct__CTagCodeMakeCrystal(&lbl_eu_80664038);
}
