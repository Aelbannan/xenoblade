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
// extern "C" keeps the flat symbols.txt label (MWCC would otherwise append a
// `__FPvi` param suffix, and the substring resolver then confuses this stub
// with its ...Select2 twin below).
extern "C" void* __dt___unnamed_CTagProcessor_cpp_CTagCodeSelect(void* self, int deleting) {
    if (self && deleting > 0) {
        ::operator delete(self);
    }
    return self;
}

extern "C" void* __dt___unnamed_CTagProcessor_cpp_CTagCodeSelect2(void* self, int deleting) {
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
// is set (the retail shape). optimize_for_size merges the r30/r31 saves into
// stmw/lmw and fixes the copy order (func_801289B4 family pattern).
#pragma optimize_for_size on
extern "C" __declspec(noinline) CTagProcessorBase::~CTagProcessorBase() {}
#pragma optimize_for_size off

// Tag-code dispatch for Process: walk the handler table; a null handler
// falls back to the base TagProcessor, a matching tag dispatches to the
// handler object's Process override (vtable slot 3). The byte offset keeps
// MWCC's base+offset induction shape (retail: add r8,r7,r6 / addi r6,r6,0xC).
// Tag-code dispatch for Process: walk the handler table; a null handler
// falls back to the base TagProcessor, a matching tag dispatches to the
// handler object's Process override (vtable slot 3). The byte offset keeps
// MWCC's base+offset induction shape (retail: add r8,r7,r6 / addi r6,r6,0xC).
#pragma optimize_for_size on  // -O4,s keeps base+offset (retail); -O4,p walks the pointer
nw4r::ut::TagProcessorBase<wchar_t>::Operation func_80125AB8(
    nw4r::ut::TagProcessorBase<wchar_t>* self, u16 tag,
    nw4r::ut::PrintContext<wchar_t>* ctx) {
    u32 o = 0;
    const u8* base = (const u8*)lbl_eu_8052D478;
    for (;; o += 12) {
        const TagEntry* e = (const TagEntry*)(base + o);
        if (e->field_04 == 0) {
            return self->TagProcessorBase<wchar_t>::Process(tag, ctx);
        }
        if (tag == e->tag) {
            return e->obj->Process(tag, ctx);
        }
    }
}
#pragma optimize_for_size off



// Tag-code dispatch for CalcRect: walk the handler table; a null handler
// falls back to the base TagProcessor, a matching tag dispatches to the
// handler object's CalcRect override (vtable slot 4).
#pragma optimize_for_size on  // -O4,s keeps base+offset (retail); -O4,p walks the pointer
nw4r::ut::TagProcessorBase<wchar_t>::Operation func_80125B08(
    nw4r::ut::TagProcessorBase<wchar_t>* self, nw4r::ut::Rect* rect,
    u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx) {
    u32 o = 0;
    const u8* base = (const u8*)lbl_eu_8052D478;
    for (;; o += 12) {
        const TagEntry* e = (const TagEntry*)(base + o);
        if (e->field_04 == 0) {
            return self->TagProcessorBase<wchar_t>::CalcRect(rect, tag, ctx);
        }
        if (tag == e->tag) {
            return e->obj->CalcRect(rect, tag, ctx);
        }
    }
}
#pragma optimize_for_size off

extern "C" int defaultProcess(void* self) { return 0; }

// Message pre-processor: copy the raw message into the buffer, then walk it
// in place - a CRLF at the head is stripped (the tail is copied down over
// it), and a '<name=value>' tag is split on '=' by the tokenizer helpers,
// looked up by name in the tag dispatch table, and consumed by the matching
// handler object (vtable slot +0x14), which returns the new buffer position;
// the saved tail (everything past '>') is then copied down over the consumed
// text. An unmatched tag name is stripped the same way (the handler loop just
// falls through). Returns the buffer.
u16* func_80125B58(CTagProcessorBase* self, const void* src, f32 a, f32 b, u32 c) {
    wchar_t bigbuf[0x400];
    u16 scratchA[0x80];
    wchar_t* tokens[16];
    memcpy(self->mBuf, src, 0x800);
    self->field_804 = c;
    wchar_t* p = (wchar_t*)self->mBuf;
    self->field_808 = a;
    self->field_80c = b;
    self->field_810 = 0;
    while (*p != 0) {
        u32 count = 0;
        u32 off = 0;
        memset(bigbuf, 0, sizeof(bigbuf));
        memset(scratchA, 0, sizeof(scratchA));
        u16 ch = p[0];
        if (ch == 0xD && p[1] == 0xA) {
            wcscpy(bigbuf, p + 2);
            wcscpy(p, bigbuf);
            continue;
        }
        if (ch == 0x3C) {
            while ((ch = p[(off >> 1) + 1]) != 0x3E && ch != 0) {
                scratchA[off >> 1] = ch;
                count++;
                off += 2;
            }
            func_801365E4(scratchA, 0x3D, (u16**)tokens);
            func_801366F4((u16*)tokens[0]);
            wcscpy(bigbuf, p + count + 2);
            const wchar_t* name = tokens[0];
            for (const TagNameEntry* e = (const TagNameEntry*)lbl_eu_8052D478;; e++) {
                if (e->name == 0)
                    break;
                if (wcscmp(name, e->name) == 0) {
                    p = (wchar_t*)e->obj->v14((u16*)p, (const wchar_t*)tokens[1],
                                              &self->field_814);
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
// Control tags advance the buffer by their operand lengths.
// optimize_for_size: retail uses the -O4,s save shape (_savegpr_17) and the
// mtctr/bdnz pairs loop.
//
// RESIDUAL (open item): sizes now MATCH exactly (900B); shapes align
// through the prologue and all case blocks. What remains:
// 1. Register-allocation permutation: retail colors &vals LAST (r21,
//    lowest priority - the store loop even rematerializes r1+8), ours
//    ranks it TOP (r31), shifting msg/out/buf/font/i down one slot each,
//    permuting inner temps + the whole FPR map (lineH f30 vs f27 etc.).
//    Proven inert levers: declaration order/position, int-vs-unsigned
//    counters (retail is signed: srawi/addze ceil-div, cmpw loops),
//    pointer temps, indexed writes, renames, explicit loop-top pointer.
// 2. Conversion magic: retail references NAMED lbl_eu_80667200; implicit
//    casts emit private pooled literals (@7188). An explicit TagConvTemp
//    union (hi=0x43300000 seeded once, lo=w^0x80000000, then
//    d - lbl_eu_80667200) names the reloc but perturbed scheduling (+8B)
//    when tried standalone - revisit after (1) is solved.
// 3. Jump table: ours anchors a local object 32B below retail's
//    jumptable_eu_8052D5A4 despite equal size; likely follows from .data
//    layout, may resolve once (1) is fixed.
#pragma optimize_for_size on
__declspec(noinline) void func_80125D00(f32* out, CTagMsgView* msg, wchar_t* buf) {
    u32 vals[16];
    nw4r::lyt::Font* font = ((nw4r::lyt::TextBox*)msg)->GetFont();
    f32 lineH = lbl_eu_806671F4;
    f32 width = lbl_eu_806671F0;
    f32 lineStep = lineH;
    f32 max = width;
    f32 speed = lbl_eu_806671F8;
    f32 step = lineH;
    u32 i = 0;
    for (;;) {
        const u16* p = (const u16*)buf + i;
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
        // Case mapping mirrors the retail jump table (jumptable_eu_8052D5A4):
        // 3 -> plain advance, 4/5 -> skip one operand char, 6/7 -> the multi-
        // row block (which falls through into 8's +1), 8/9 -> skip one more,
        // B -> skip five, D -> full-width marker, everything else accumulates
        // width.
        switch (ch) {
        case 1: {
            // <1 skip,cnt>: advance past `skip` chars, then accumulate the
            // width of the following `cnt` chars.
            u16 v = p[1];
            int skip = v >> 8;
            int cnt = v & 0xFF;
            i += 2 + skip;
            const u16* p = (const u16*)buf + i;
            for (int j = 0; j < cnt; j++) {
                f32 sp = msg->field_F4;
                width += sp + (f32)font->v48(p[j]);
            }
            i += cnt - 1;
            break;
        }
        case 2:
            i += 2;
            break;
        case 3:
            break;
        case 4:
            i += 1;
            break;
        case 5:
            i += 1;
            break;
        case 6:
        case 7: {
            // <6/7 cnt,...>: each following char carries a per-char count in
            // its high/low byte; for each of `cnt` rows accumulate the width
            // of that many chars and step the line between rows.
            u16 v = buf[i + 1];
            int cnt = v & 0xFF;
            int x = cnt;
            if (v & 1)
                x = cnt + 1;
            int pairs = (x + 1) / 2;
            i += 2;
            const u16* p = (const u16*)buf + i;
            for (int j = 0; j < pairs; j++) {
                u16 c = p[j];
                vals[2 * j] = c >> 8;
                vals[2 * j + 1] = c & 0xFF;
            }
            i += pairs;
            for (int row = 0; row < cnt; row++) {
                int n = vals[row];
                const u16* q = (const u16*)buf + i;
                for (int k = 0; k < n; k++) {
                    f32 sp = msg->field_F4;
                    width += sp + (f32)font->v48(q[k]);
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
            /* fallthrough */
        case 8:
            i += 1;
            break;
        }
        case 9:
            i += 1;
            break;
        case 0xB:
            i += 5;
            break;
        case 0xD:
            // Full-width marker: width += speed * (glyph(0x2500) + spacing).
            width += speed * (msg->field_F4 + (f32)font->v48(0x2500));
            break;
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
// when the deleting flag is set (the retail shape). optimize_for_size merges
// the r30/r31 saves into stmw/lmw + fixes the copy order (family pattern).
#pragma optimize_for_size on
CTagProcessor::~CTagProcessor() {}
#pragma optimize_for_size off

// Message-pump step: dispatch the next tag code at buf[field_810+2]. The
// tag stream is the u16-encoded message written by the tag-writer family;
// most handlers push tag data to the text pane via its +0x7C SetText and
// advance the read counters. The name-display tag (codes 6/7) is multi-step:
// the first call builds the per-name rows and the combined string, later
// calls handle the directional input / accept (field_82c gate).
// Frame layout note: locals are assigned in declaration order from the top
// of the frame down (first declared = highest address), and the per-walk
// ctx/pair/color temps follow the walk order - the declaration order below
// mirrors the retail frame (0x8..0x480 locals, 0x480..0xC80 namebuf).
__declspec(noinline) int func_8012615C(nw4r::lyt::AnimTransform* tag,
                                        nw4r::lyt::Pane* a,
                                        nw4r::lyt::Pane* b,
                                        nw4r::lyt::Pane* c) {
    CTagProcMsg* msg = (CTagProcMsg*)tag;
    TalkMsgPane* pane = (TalkMsgPane*)a;
    // Big buffers first (highest frame addresses): the name display uses the
    // 0x480 combined-string buffer and the 0x1C0 per-name rows; the push
    // cases share the 0x280 output buffer.
    wchar_t namebuf[0x400];
    u16 outbuf[0x100];
    wchar_t rows[0x30];
    // Context-walk locals: six ctx slots for the find/highlight walk pairs,
    // then the per-walk pair arrays and the highlight color temps - declared
    // at the top in the retail's frame order (first declared = highest
    // address: 0xB8 ctxs, 0xB0..0x8 pairs/colors).
    u32* ctxA;
    u32* ctxB;
    u32* ctxC;
    u32* ctxD;
    u32* ctxE;
    u32* ctxF;
    u32 pair1[2];
    nw4r::ut::Color cW2a;
    nw4r::ut::Color cW2b;
    u32 pair2[2];
    u32 pair3[2];
    nw4r::ut::Color cW4a1;
    nw4r::ut::Color cW4a2;
    nw4r::ut::Color cW4b1;
    nw4r::ut::Color cW4b2;
    u32 pair4[2];
    u32 pair5[2];
    nw4r::ut::Color cW6a;
    nw4r::ut::Color cW6b;
    u32 pair6[2];
    u32 pair5p[2];
    nw4r::ut::Color cW6pa1;
    nw4r::ut::Color cW6pa2;
    nw4r::ut::Color cW6pb1;
    nw4r::ut::Color cW6pb2;
    u32 pair6p[2];
    u32 pair7[2];
    nw4r::ut::Color cW8a1;
    nw4r::ut::Color cW8a2;
    nw4r::ut::Color cW8b1;
    nw4r::ut::Color cW8b2;
    u32 pair8[2];
    u32 pair9[2];
    nw4r::ut::Color cW10a1;
    nw4r::ut::Color cW10a2;
    nw4r::ut::Color cW10b1;
    nw4r::ut::Color cW10b2;
    u32 pair10[2];

    u16 tagcode = (msg->buf + msg->field_810)[2];
    if (tagcode == 0)
        return 3;

    outbuf[0] = 0;
    // Talk-source / party-member / player lookups (r29/r28/r30).
    TagMemberObj* member = 0;
    TagTalkSrc* tsrc = (TagTalkSrc*)func_800BBC0C(findObjectById(msg->field_804));
    if (tsrc != 0)
        member = tsrc->field_98;
    TagTalkSrc* player = (TagTalkSrc*)func_800BBC0C(cf::CfGameManager::getPlayer(0));

    // Walk the tag dispatch table (25 x 12-byte entries).
    for (u32 o = 0; ; o += 12) {
        const TagEntry* e = (const TagEntry*)((const u8*)lbl_eu_8052D478 + o);
        if (e->field_04 == 0) {
            // No handler for this code: push <tag, 0> verbatim and advance
            // both counters, then run the icon-pending tail.
            outbuf[0] = tagcode;
            outbuf[1] = 0;
            u16 p = msg->field_810;
            msg->field_810 = p + 1;
            pane->v7C(outbuf, msg->field_820, p);
            msg->field_820 += 1;
            goto finish;
        }
        if (tagcode == e->tag) {
            switch (e->tag) {
            case 0:
            case 0xA:
            case 0xC:
            case 0xD:
                return 4;  // selection-accepted marker

            case 5: {
                // <5 v> text-speed / skip tag: v == -1 selects the button-
                // driven auto-advance, otherwise v is the skip counter.
                s16 v = (s16)msg->buf[msg->field_810 + 3];
                if (v == -1) {
                    // int->float via the retail 0x43300000 magic (named
                    // sdata2 constant): seed hi once, store (v ^ 0x8000)
                    // into lo, then subtract the magic double.
                    f32 speed = msg->field_824;
                    if (speed < lbl_eu_80667208) {
                        msg->field_824 += lbl_eu_806671F4;
                    } else {
                        TagPadView* pad =
                            (TagPadView*)cf::CfGameManager::getCurrentPad();
                        int pressed;
                        if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0)
                            pressed = (pad->field_04 & 0x00600000) != 0;
                        else
                            pressed = (pad->field_04 & 0x00000030) != 0;
                        if (pressed != 0) {
                            msg->field_824 = lbl_eu_806671F0;
                            msg->field_810 += 2;
                            msg->field_820 += 2;
                        }
                    }
                    return 2;
                }
                // Retail int->float lowering: a two-word temp on the stack
                // (hi = raw value, lo = (v << 16) ^ 0x80000000) minus the
                // named magic double lbl_eu_80667200.
                TagConvTemp conv;
                conv.w.hi = (u32)(s32)v;
                conv.w.lo = (((u32)v) << 16) ^ 0x80000000;
                f32 speed = msg->field_824;
                if (speed < (f32)(conv.d - lbl_eu_80667200)) {
                    if (speed < lbl_eu_806671F4 && member != 0)
                        member->v58(0, 0);
                    msg->field_824 += lbl_eu_806671F4;
                } else {
                    if (member != 0)
                        member->v58(1, 0);
                    msg->field_824 = lbl_eu_806671F0;
                    msg->field_810 += 2;
                    msg->field_820 += 2;
                }
                return 2;
            }

            case 4: {
                // <4 v> wait-for-click tag: v == 0xFFFF holds the page.
                s16 v = (s16)msg->buf[msg->field_810 + 3];
                msg->field_810 += 2;
                msg->field_820 = 0;
                return v == -1;
            }

            case 1: {
                // <1 len0|len1 data...> raw text-range tag: copy the range
                // (hi+lo count u16s) to the pane with the header preserved.
                u16 h = msg->buf[msg->field_810 + 3];
                u32 hi = h >> 8;
                u32 lo = h & 0xff;
                u32 count = hi + lo;
                outbuf[0] = tagcode;
                outbuf[1] = h;
                u32 j;
                for (j = 0; j < count; j++)
                    outbuf[2 + j] = msg->buf[msg->field_810 + 4 + j];
                outbuf[2 + count] = 0;
                u16 adv = (u16)(count + 2);
                msg->field_810 += adv;
                pane->v7C(outbuf, msg->field_820, adv);
                msg->field_820 += adv;
                return -1;
            }

            case 2: {
                // <2 hi lo> color tag: push the two data u16s plus a 0 tail.
                u16 d0 = msg->buf[msg->field_810 + 3];
                u16 d1 = msg->buf[msg->field_810 + 4];
                outbuf[0] = tagcode;
                outbuf[1] = d0;
                outbuf[2] = d1;
                outbuf[3] = 0;
                msg->field_810 += 3;
                pane->v7C(outbuf, msg->field_820, d1);
                msg->field_820 += 3;
                return -1;
            }

            case 0xB: {
                // <0xB ...> party-icon/event tag: 5 s16 operands, type = the
                // high byte of the first. Manipulates event bytes/words and
                // plays UI sounds; advances 6.
                s16 v3 = (s16)msg->buf[msg->field_810 + 3];
                s32 v4 = (s16)msg->buf[msg->field_810 + 4];
                s16 v5 = (s16)msg->buf[msg->field_810 + 5];
                s32 type = v3 >> 8;
                s32 v6 = (s16)msg->buf[msg->field_810 + 6];
                s32 v7 = (s16)msg->buf[msg->field_810 + 7];
                s16 v26 = 0;
                s16 v28 = v5;
                if (code80135FDC_getByte_6405A() != 0) {
                    if (code80135FDC_getByte_6405B() != 0) {
                        if (v28 < 0) {
                            v28 = (s16)-code80135FDC_getWord_6405C();
                            v26 = (s16)-code80135FDC_getWord_64060();
                            func_8013BE38();
                        }
                    } else {
                        int val = (int)func_8009CF8C(v4 + 0x29) + v28;
                        if (val < 0)
                            val = 0;
                        s32 r21;
                        if (val > 0x1388) {
                            r21 = v28 - (val - 0x1388);
                            if (r21 < 0)
                                r21 = 0;
                        } else {
                            r21 = v28;
                        }
                        r21 += code80135FDC_getWord_6405C();
                        code80135FDC_setPair_6405C_64060(
                            r21, code80135FDC_getWord_64060() + v28);
                    }
                }
                u32 v24 = 0;
                if (type == 1) {
                    if (v7 != func_8009CF8C(v6 + 0x608)) {
                        s32 r21 = v4 + 0x21;
                        int val = (int)func_8009CF8C(r21) + v28;
                        if (val < 0)
                            val = 0;
                        if (val > 0x2710)
                            val = 0x2710;
                        func_8009D018(r21, val);
                    } else {
                        v24 = 1;
                    }
                } else if (type == 2) {
                    s32 r21 = v4 + 0x29;
                    int val = (int)func_8009CF8C(r21) + v28;
                    if (val < 0)
                        val = 0;
                    if (val > 0x1388) {
                        v26 = v28;
                        v28 = v28 - (s16)(val - 0x1388);
                        if (v28 < 0)
                            v28 = 0;
                        val = 0x1388;
                    }
                    func_8009D018(r21, val);
                }
                if (v26 != 0)
                    v28 = v26;
                if (v28 == 0)
                    v24 = 1;
                if (v24 == 0) {
                    s32 sound = 0;
                    if (v28 > 0) {
                        sound = (type == 1) ? 0xc6 : 0xac;
                        func_80138078(0x34);
                    } else if (v28 < 0) {
                        sound = (type == 1) ? 0xc8 : 0xae;
                        func_80138078(0x36);
                    }
                    if (tsrc != 0)
                        func_800451D8(sound, tsrc);
                    if (player != 0)
                        func_800451D8(sound, player);
                }
                if (type == 1) {
                    if (player != 0)
                        func_8013DB6C(4, player->field_8C, v4, v28);
                } else if (type == 2) {
                    func_8013DB6C(5, 0, v4 + 1, v28);
                }
                msg->field_810 += 6;
                msg->field_820 += 6;
                return -1;
            }

            case 9: {
                // <9 v> party-member emphasis tag: v == 0 enables, v == -1
                // disables the member's follow highlight.
                s16 v = (s16)msg->buf[msg->field_810 + 3];
                if (member != 0) {
                    if (v == 0)
                        member->v58(1, 0);
                    else if (v == -1)
                        member->v58(0, 0);
                }
                msg->field_810 += 2;
                msg->field_820 += 2;
                return -1;
            }

            case 3: {
                // <3> plain text tag: push <tag, 0> and advance 1.
                outbuf[0] = tagcode;
                outbuf[1] = 0;
                u16 p = msg->field_810;
                msg->field_810 = p + 1;
                pane->v7C(outbuf, msg->field_820, p);
                msg->field_820 += 1;
                return -1;
            }

            case 6:
            case 7: {
                // Name-display tag: <7, count, len-pairs, name strings...>.
                // First call builds and displays the name list; later calls
                // handle the directional input / acceptance.
                if (msg->field_82c == 0) {
                    u16 h = msg->buf[msg->field_810 + 3];
                    u32 count = h & 0xff;
                    msg->field_830 = count;
                    u32 lcount = count;
                    if (h & 1)
                        lcount = (lcount + 1) & 0xff;
                    u32 pairs = (lcount >> 1) & 0x7f;
                    memset(msg->m838, 0, 0xc);
                    for (u32 i = 0; i < pairs; i++) {
                        u16 v = msg->buf[msg->field_810 + 4 + i];
                        msg->m838[2 * i] = v >> 8;
                        msg->m838[2 * i + 1] = v & 0xff;
                    }
                    memset(rows, 0, 0xc0);
                    u32 charIdx = 0;
                    for (u32 i = 0; i < msg->field_830; i++) {
                        wcscpy(
                            &rows[i * 0x20],
                            (const wchar_t*)&msg->buf[msg->field_810 + pairs +
                                                       charIdx + 4]);
                        charIdx += msg->m838[i];
                        rows[i * 0x20 + msg->m838[i]] = 0;
                    }
                    namebuf[0] = 0;
                    charIdx = 0;
                    for (u32 i = 0; i < msg->field_830; i++) {
                        wcscpy(&namebuf[charIdx], &rows[i * 0x20]);
                        charIdx += msg->m838[i];
                        if (i < msg->field_830 - 1)
                            namebuf[charIdx++] = 0xa;
                    }
                    namebuf[charIdx] = 0;
                    pane->v78();
                    pane->v74(0x40);
                    func_80127764(msg, (TalkPaneView*)pane, (TalkPaneView*)b,
                                   (TalkPaneView*)c, (const wchar_t*)namebuf);
                    u8* ctxBase = (u8*)pane->field_0C + 0x10;
                    // walk 1: find the context node matching the current text
                    ctxA = (u32*)getContextStr(ctxBase);
                    for (;;) {
                        u8* node = (u8*)func_80127670(&ctxA);
                        if (strcmp((char*)(node + 0xbc),
                                   (char*)pane->field_BC) == 0)
                            break;
                        ctxA = (u32*)*ctxA;
                        pair1[0] = (u32)getContextStrPtr(ctxBase);
                        pair1[1] = (u32)ctxA;
                        if (func_801276C8(&pair1[1], &pair1[0]) == 0)
                            break;
                    }
                    func_801276E0(&ctxA, 0);
                    // walk 2: create the per-name panes, set text + colors
                    u32 paneIdx = 0;
                    for (;;) {
                        pair2[0] = (u32)getContextStrPtr(ctxBase);
                        pair2[1] = (u32)ctxA;
                        if (func_801276C8(&pair2[1], &pair2[0]) == 0)
                            break;
                        if (paneIdx >= msg->field_830)
                            break;
                        TalkNamePane* np = pane->field_0C->v3C(
                            (const wchar_t*)((u8*)func_80127670(&ctxA) + 0xbc),
                            1);
                        copyVEC2(&np->field_4C[0], &pane->field_4C[0]);
                        np->v78();
                        np->v74(0x20);
                        np->v7C((const u16*)&rows[paneIdx * 0x20], 0);
                        cW2a.v = 0x5a5a5aff;
                        cW2b.v = 0x5a5a5aff;
                        np->field_DC = cW2a;
                        np->field_E0 = cW2b;
                        paneIdx++;
                        ctxA = (u32*)*ctxA;
                    }
                    msg->field_828 = 0;
                    msg->field_82c = 1;
                    msg->field_844 = 10;
                    if (e->tag == 7) {
                        msg->field_844 = 0;
                        msg->field_834 = msg->field_830 - 1;
                        // walk 3: find (second ctx)
                        ctxB = (u32*)getContextStr(ctxBase);
                        for (;;) {
                            u8* node = (u8*)func_80127670(&ctxB);
                            if (strcmp((char*)(node + 0xbc),
                                       (char*)pane->field_BC) == 0)
                                break;
                            ctxB = (u32*)*ctxB;
                            pair3[0] = (u32)getContextStrPtr(ctxBase);
                            pair3[1] = (u32)ctxB;
                            if (func_801276C8(&pair3[1], &pair3[0]) == 0)
                                break;
                        }
                        func_801276E0(&ctxB, 0);
                        // walk 4: highlight (last name selected)
                        paneIdx = 0;
                        for (;;) {
                            pair4[0] = (u32)getContextStrPtr(ctxBase);
                            pair4[1] = (u32)ctxB;
                            if (func_801276C8(&pair4[1], &pair4[0]) == 0)
                                break;
                            if (paneIdx >= msg->field_830)
                                break;
                            TalkNamePane* np = pane->field_0C->v3C(
                                (const wchar_t*)((u8*)func_80127670(&ctxB) +
                                                 0xbc),
                                1);
                            if (paneIdx == msg->field_834) {
                                cW4a1.v = 0x006400ff;
                                cW4a2.v = 0x006400ff;
                                np->field_DC = cW4a1;
                                np->field_E0 = cW4a2;
                            } else {
                                cW4b1.v = 0x5a5a5aff;
                                cW4b2.v = 0x5a5a5aff;
                                np->field_DC = cW4b1;
                                np->field_E0 = cW4b2;
                            }
                            paneIdx++;
                            ctxB = (u32*)*ctxB;
                        }
                    }
                    return 2;
                }
                // ---- subsequent calls: directional input handling ----
                TagPadDataView* pad =
                    (TagPadDataView*)cf::CfGameManager::getCfPadData();
                int btn;
                bool upDown;
                bool leftRight;
                if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
                    btn = (pad->field_04 >> 22) & 1;
                    upDown = (pad->field_104 & 0x8004) != 0;
                    leftRight = (pad->field_104 & 0x8000) != 0 ||
                                (pad->field_104 & 0x8) != 0;
                } else {
                    btn = (pad->field_04 >> 4) & 1;
                    upDown = (pad->field_104 & 0x8004) != 0;
                    leftRight = (pad->field_104 & 0x8000) != 0 ||
                                (pad->field_104 & 0x8) != 0;
                }
                u32 hold = msg->field_844 + 1;
                msg->field_844 = hold;
                if (btn != 0) {
                    if (hold > 10) {
                        if (msg->field_834 < 0) {
                            // button pressed with no selection yet: reset +
                            // highlight the first row
                            msg->field_844 = 0;
                            msg->field_834 = 0;
                            u8* ctxBase2 = (u8*)pane->field_0C + 0x10;
                            // walk 5': find
                            ctxD = (u32*)getContextStr(ctxBase2);
                            for (;;) {
                                u8* node = (u8*)func_80127670(&ctxD);
                                if (strcmp((char*)(node + 0xbc),
                                           (char*)pane->field_BC) == 0)
                                    break;
                                ctxD = (u32*)*ctxD;
                                pair5p[0] = (u32)getContextStrPtr(ctxBase2);
                                pair5p[1] = (u32)ctxD;
                                if (func_801276C8(&pair5p[1], &pair5p[0]) == 0)
                                    break;
                            }
                            func_801276E0(&ctxD, 0);
                            // walk 6': highlight
                            u32 paneIdx2 = 0;
                            for (;;) {
                                pair6p[0] = (u32)getContextStrPtr(ctxBase2);
                                pair6p[1] = (u32)ctxD;
                                if (func_801276C8(&pair6p[1], &pair6p[0]) == 0)
                                    break;
                                if (paneIdx2 >= msg->field_830)
                                    break;
                                TalkNamePane* np = pane->field_0C->v3C(
                                    (const wchar_t*)((u8*)func_80127670(&ctxD) +
                                                     0xbc),
                                    1);
                                if (paneIdx2 == msg->field_834) {
                                    cW6pa1.v = 0x006400ff;
                                    cW6pa2.v = 0x006400ff;
                                    np->field_DC = cW6pa1;
                                    np->field_E0 = cW6pa2;
                                } else {
                                    cW6pb1.v = 0x5a5a5aff;
                                    cW6pb2.v = 0x5a5a5aff;
                                    np->field_DC = cW6pb1;
                                    np->field_E0 = cW6pb2;
                                }
                                paneIdx2++;
                                ctxD = (u32*)*ctxD;
                            }
                            return 2;
                        }
                        // accept the current selection: re-highlight every
                        // row with the accept color, then consume the tag
                        u8* ctxBase3 = (u8*)pane->field_0C + 0x10;
                        // walk 5: find
                        ctxC = (u32*)getContextStr(ctxBase3);
                        for (;;) {
                            u8* node = (u8*)func_80127670(&ctxC);
                            if (strcmp((char*)(node + 0xbc),
                                       (char*)pane->field_BC) == 0)
                                break;
                            ctxC = (u32*)*ctxC;
                            pair5[0] = (u32)getContextStrPtr(ctxBase3);
                            pair5[1] = (u32)ctxC;
                            if (func_801276C8(&pair5[1], &pair5[0]) == 0)
                                break;
                        }
                        func_801276E0(&ctxC, 0);
                        // walk 6: accept highlight
                        u32 paneIdx3 = 0;
                        for (;;) {
                            pair6[0] = (u32)getContextStrPtr(ctxBase3);
                            pair6[1] = (u32)ctxC;
                            if (func_801276C8(&pair6[1], &pair6[0]) == 0)
                                break;
                            if (paneIdx3 >= msg->field_830)
                                break;
                            TalkNamePane* np = pane->field_0C->v3C(
                                (const wchar_t*)((u8*)func_80127670(&ctxC) +
                                                 0xbc),
                                1);
                            np->v78();
                            np->v74(0x20);
                            cW6a.v = 0x483a21ff;
                            cW6b.v = 0x483a21ff;
                            np->field_DC = cW6a;
                            np->field_E0 = cW6b;
                            paneIdx3++;
                            ctxC = (u32*)*ctxC;
                        }
                        // consume the name tag and reset the display state
                        u32 c2 = msg->field_830;
                        if (c2 & 1)
                            c2++;
                        u32 half = c2 / 2;
                        msg->field_810 += half + 2;
                        for (u32 i = 0; i < msg->field_830; i++)
                            msg->field_810 += (u16)msg->m838[i];
                        s32 f834 = msg->field_834;
                        msg->field_820 = 0;
                        msg->field_824 = lbl_eu_806671F0;
                        msg->field_830 = 0;
                        msg->field_828 = f834 + 1;
                        msg->field_834 = -1;
                        msg->field_82c = 0;
                    }
                    return 2;
                }
                if (upDown) {
                    // up/down: move the selection and re-highlight
                    msg->field_834--;
                    if (msg->field_834 < 0)
                        msg->field_834 = msg->field_830 - 1;
                    u8* ctxBase4 = (u8*)pane->field_0C + 0x10;
                    // walk 7: find
                    ctxE = (u32*)getContextStr(ctxBase4);
                    for (;;) {
                        u8* node = (u8*)func_80127670(&ctxE);
                        if (strcmp((char*)(node + 0xbc),
                                   (char*)pane->field_BC) == 0)
                            break;
                        ctxE = (u32*)*ctxE;
                        pair7[0] = (u32)getContextStrPtr(ctxBase4);
                        pair7[1] = (u32)ctxE;
                        if (func_801276C8(&pair7[1], &pair7[0]) == 0)
                            break;
                    }
                    func_801276E0(&ctxE, 0);
                    // walk 8: highlight
                    u32 paneIdx4 = 0;
                    for (;;) {
                        pair8[0] = (u32)getContextStrPtr(ctxBase4);
                        pair8[1] = (u32)ctxE;
                        if (func_801276C8(&pair8[1], &pair8[0]) == 0)
                            break;
                        if (paneIdx4 >= msg->field_830)
                            break;
                        TalkNamePane* np = pane->field_0C->v3C(
                            (const wchar_t*)((u8*)func_80127670(&ctxE) + 0xbc),
                            1);
                        if (paneIdx4 == msg->field_834) {
                            cW8a1.v = 0x006400ff;
                            cW8a2.v = 0x006400ff;
                            np->field_DC = cW8a1;
                            np->field_E0 = cW8a2;
                        } else {
                            cW8b1.v = 0x5a5a5aff;
                            cW8b2.v = 0x5a5a5aff;
                            np->field_DC = cW8b1;
                            np->field_E0 = cW8b2;
                        }
                        paneIdx4++;
                        ctxE = (u32*)*ctxE;
                    }
                    return 2;
                }
                if (leftRight) {
                    // left/right: move the selection and re-highlight
                    msg->field_834++;
                    if (msg->field_834 > msg->field_830 - 1)
                        msg->field_834 = 0;
                    u8* ctxBase5 = (u8*)pane->field_0C + 0x10;
                    // walk 9: find
                    ctxF = (u32*)getContextStr(ctxBase5);
                    for (;;) {
                        u8* node = (u8*)func_80127670(&ctxF);
                        if (strcmp((char*)(node + 0xbc),
                                   (char*)pane->field_BC) == 0)
                            break;
                        ctxF = (u32*)*ctxF;
                        pair9[0] = (u32)getContextStrPtr(ctxBase5);
                        pair9[1] = (u32)ctxF;
                        if (func_801276C8(&pair9[1], &pair9[0]) == 0)
                            break;
                    }
                    func_801276E0(&ctxF, 0);
                    // walk 10: highlight
                    u32 paneIdx5 = 0;
                    for (;;) {
                        pair10[0] = (u32)getContextStrPtr(ctxBase5);
                        pair10[1] = (u32)ctxF;
                        if (func_801276C8(&pair10[1], &pair10[0]) == 0)
                            break;
                        if (paneIdx5 >= msg->field_830)
                            break;
                        TalkNamePane* np = pane->field_0C->v3C(
                            (const wchar_t*)((u8*)func_80127670(&ctxF) + 0xbc),
                            1);
                        if (paneIdx5 == msg->field_834) {
                            cW10a1.v = 0x006400ff;
                            cW10a2.v = 0x006400ff;
                            np->field_DC = cW10a1;
                            np->field_E0 = cW10a2;
                        } else {
                            cW10b1.v = 0x5a5a5aff;
                            cW10b2.v = 0x5a5a5aff;
                            np->field_DC = cW10b1;
                            np->field_E0 = cW10b2;
                        }
                        paneIdx5++;
                        ctxF = (u32*)*ctxF;
                    }
                }
                return 2;
            }

            case 8: {
                // <8 ...> icon tag: map the three header bytes through the
                // 0x1B-entry flag table into icon ids (func_8004B9D4 call or
                // pending field_848/field_850 stores), then advance 3.
                // Scoped here: only this case touches the 0xD8-byte table.
                TagFlagTable flagTbl;
                u32 vals[3];
                s16 hdr[3];
                flagTbl = lbl_eu_804FF608;
                hdr[0] = (s16)((msg->buf[msg->field_810 + 3] >> 8) & 0xff);
                hdr[1] = (s16)(msg->buf[msg->field_810 + 3] & 0xff);
                hdr[2] = (s16)((msg->buf[msg->field_810 + 4] >> 8) & 0xff);
                vals[0] = 0;
                vals[1] = 0;
                vals[2] = 0;
                for (u32 k = 0; k < 3; k++) {
                    s16 id = hdr[k];
                    if (id == 0xff)
                        break;
                    u32 v = flagTbl.v[id];
                    vals[k] = v;
                    if (k == 0) {
                        if (tsrc != 0)
                            func_8004B9D4(tsrc->field_C4, v, 0, -1, 0);
                    } else if (k == 1) {
                        msg->field_848 = v;
                        msg->field_84c = 1;
                    } else {
                        msg->field_850 = v;
                        msg->field_854 = 1;
                    }
                }
                msg->field_810 += 3;
                msg->field_820 += 3;
                goto finish;
            }

            default:
                goto finish;
            }
        }
    }

finish:
    // Icon-pending tail: display the pending icon once its window is ready.
    if (msg->field_84c != 0) {
        if (tsrc != 0) {
            TagC4Obj* w = tsrc->field_C4;
            if (w->v80(0)) {
                func_8004B9D4(w, msg->field_848, 0, -1, 0);
                msg->field_84c = 0;
            }
        }
    }
    if (msg->field_854 != 0) {
        if (tsrc != 0) {
            TagC4Obj* w = tsrc->field_C4;
            if (w->v80(0)) {
                func_8004B9D4(w, msg->field_850, 0, -1, 0);
                msg->field_854 = 0;
            }
        }
    }
    return -1;
}

// noinline: func_80127BF4 keeps the bl copyVEC2 call in retail - a tiny
// same-TU body like this would otherwise be folded into the call site.
extern "C" __declspec(noinline) void copyVEC2(float *dst, const float *src) {
    dst[0] = src[0];
    dst[1] = src[1];
}
// noinline: the retail name-display walks call the operator out-of-line
// (each store emits a bl to __as__Q34nw4r2ut5ColorFRCQ34nw4r2ut5Color and
// the RHS color is materialized on the stack) - -ipa would otherwise fold
// the 4-byte copy into the call sites and drop the color temps.
__declspec(noinline) nw4r::ut::Color& nw4r::ut::Color::operator=(
    const nw4r::ut::Color& rhs) {
    comp.r = rhs.comp.r;
    comp.g = rhs.comp.g;
    comp.b = rhs.comp.b;
    comp.a = rhs.comp.a;
    return *this;
}

// noinline: the retail name-display walks call the operator out-of-line
// (each store emits a bl to __as__Q34nw4r2ut5ColorFRCQ34nw4r2ut5Color and
// the RHS color is materialized on the stack) - -ipa would otherwise fold
// the 4-byte copy into the call sites and drop the color temps.

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

// Fetch the tag chain head and return the first node's list header (4 bytes
// before the payload). Retail asserts when the chain is empty.
extern "C" void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
void* func_80127670(void* self) {
    extern u8 lbl_eu_8052CB40[], lbl_eu_8052CB1C[];
    void* node = *(void**)self;
    if (!node)
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 0x23D, (const char*)lbl_eu_8052CB1C);
    return (u8*)node - 4;
}

// Compare the first u32 of two blocks (used as a tag-identity test).
// Retail uses the -O4,s addic/subfe setnz idiom; optimize_for_size on
// forces the -O4,s lowering even though the unit compiles at -O4,p.
#pragma optimize_for_size on
u32 func_801276C8(const u32* a, const u32* b) {
    return *a != *b;
}
#pragma optimize_for_size off

extern "C" void* func_801276E0(void* self, int a) {
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

// Tag-processor page step: build the current page's text into a scratch
// buffer - either straight from the caller-supplied string, or by walking
// the message buffer tags from msg->field_810 (ruby tag 6 expands its packed
// per-name char counts; tag 9 skips one parameter word, 0xb skips three;
// codes 0/3/4 end the page and an out-of-window guard stops the walk).
// Then set the text on the talk textbox, measure the laid-out text and
// distribute positions/sizes across the message/name panes.
//
// MATCH STATUS (in progress): decomp 1112B vs retail 1116B; ALL relocs align
// except a uniform -4 shift starting at the lbl_eu_8066721C load in the
// second x-clamp below. Exactly ONE instruction missing there: retail emits
// `fcmpo; cror eq<-gt|eq; bne end` (merged FP compare) before the c21C load,
// while every source form tried so far yields either a single direct branch
// (`pos2[0] < lbl`, current form, -1 insn) or two extra insns
// (`!(x >= lbl)` / flipped-operand forms). Fix that one cror shape and the
// whole tail should realign byte-for-byte.
#pragma optimize_for_size on
void func_80127764(CTagProcMsg* msg, TalkPaneView* a, TalkPaneView* b,
                   TalkPaneView* c, const wchar_t* text) {
    wchar_t buf[0x400];   // page scratch (+0x190)
    u32 counts[0x10];     // ruby per-entry char counts (+0x50)
    wchar_t str[0x80];    // name string scratch (+0x90)

    memset(buf, 0, sizeof(buf));
    if (text == NULL) {
        u16 out = 0;
        u16 pos = msg->field_810;
        u16 code;
        // message chars live at msg+4+2*i (u16 buffer behind a header);
        // codes 0/3/4 end the page, the walk stops after 0x3ff chars
        while ((code = msg->buf[pos + 2]) != 0 && code != 3 && code != 4 &&
               (int)((u16)pos - msg->field_810) < 0x3ff) {
            if (code == 6) {
                // <6> name-list tag: hdr low byte = entry count; bit0 adds a
                // padding half-slot. The next ceil(m/2) chars pack two count
                // bytes each (hi then lo); each entry's chars follow.
                buf[out] = code;
                pos += 1;
                u16 hdr = msg->buf[pos + 2];
                out += 1;
                buf[out] = hdr;
                out += 1;
                pos += 1;
                u32 n = hdr & 0xff;
                s32 m = n;
                if (hdr & 1)
                    m = n + 1;
                u32 cnt = 0;
                u32 bi = 0;
                while ((s32)cnt < (m >> 1) + (m & 1)) {
                    u16 pair = msg->buf[pos + 2];
                    pos += 1;
                    buf[out] = pair;
                    out += 1;
                    counts[bi] = pair >> 8;
                    counts[bi + 1] = pair & 0xff;
                    bi += 2;
                    cnt += 1;
                }
                for (u32 j = 0; j < n; j++) {
                    u32 k = counts[j];
                    while (k > 0) {
                        buf[out] = msg->buf[pos + 2];
                        out += 1;
                        pos += 1;
                        k -= 1;
                    }
                }
            } else if (code == 0xb) {
                pos += 6;
            } else if (code == 9) {
                pos += 2;
            } else {
                buf[out] = code;
                out += 1;
                pos += 1;
            }
        }
        buf[out] = 0;
    } else {
        wcscpy(buf, text);
    }

    a->v7C((const u16*)buf, 0);
    a->field_F0 = lbl_eu_806671F8;

    f32 meas[2];  // +0x30 measured {x, y} from the layout step
    func_80125D00(meas, (CTagMsgView*)a, buf);
    meas[1] = lbl_eu_8066720C * meas[1];
    f32 pos2[2];  // +0x28 clamped cursor position
    pos2[0] = meas[0];
    pos2[1] = meas[1] + lbl_eu_80667210;
    f32 dim[2];   // +0x20 {width, height}; later reused as corner xy
    dim[0] = lbl_eu_806671F0;
    dim[1] = lbl_eu_806671F0;
    if (c != NULL) {
        memset(str, 0, sizeof(str));
        wcscpy(str, c->field_D8);
        f32 size[2];  // +0x10 measured name width/height
        func_80127D20(size, msg, (nw4r::lyt::TextBox*)c, str);
        dim[0] = size[0];
        dim[1] = size[1];
        f32 maxX = lbl_eu_80667214 + size[0];
        if (pos2[0] < maxX)
            pos2[0] = maxX;
    }
    pos2[0] = lbl_eu_806671F8 * msg->field_808 + pos2[0];
    if (pos2[0] <= lbl_eu_80667218) {
        pos2[0] = lbl_eu_80667218;
    } else if (pos2[0] < lbl_eu_80667220) {
        pos2[0] = lbl_eu_8066721C;
    }
    copyVEC2(b->field_4C, pos2);

    TagVec3 t = b->vec_2C;
    t.v[1] -= lbl_eu_80667224;
    copyVEC3(&a->vec_2C.v[0], &t.v[0]);
    copyVEC2(a->field_4C, meas);
    a->v78();
    a->v74(0x400);

    if (c != NULL) {
        f32 w[2];                  // +0x18 corner xy scratch
        TagVec3 s = a->vec_2C;     // dead in retail too; kept across the
                                   // opaque helper calls below
        func_80127BC4(w, a->field_4C);
        copyVEC3(&s.v[0], b->vec_2C.v);
        copyVEC2(w, b->field_4C);
        copyVEC3(c->vec_2C.v, &s.v[0]);
        copyVEC3(&s.v[0], c->vec_2C.v);
        copyVEC2(w, c->field_4C);
        f32 u[2];                  // +0x08
        u[0] = dim[0];
        u[1] = w[1];
        copyVEC2(c->field_4C, u);
        copyVEC2(dim, c->field_4C);
        copyVEC3(&s.v[0], b->vec_2C.v);
        copyVEC2(pos2, b->field_4C);
        // blend the b/c corners toward the measured box
        const f32 kBlend = lbl_eu_80667228;
        f32 nx = dim[0] * kBlend + (s.v[0] - pos2[0] * kBlend);
        f32 ny = (s.v[1] + pos2[1] * kBlend) - dim[1] * kBlend;
        s.v[0] = nx + lbl_eu_8066722C;
        s.v[1] = ny - lbl_eu_80667214;
        copyVEC3(c->vec_2C.v, &s.v[0]);
    }
}
#pragma optimize_for_size off

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
// running sum; when the terminator is hit the running sum becomes the max.
// The height getter (vtable +0x34) and each width+2 are converted with the
// builtin 0x4330/xoris/lfd/fsubs magic idiom (magic hoisted once into f31,
// shared by both sites).
// optimize_for_size: retail saves r27-r31 + f28-f31 with _savegpr_27 (the
// -O4,s save shape).
#pragma optimize_for_size on
void __declspec(noinline) func_80127D20(f32* out, void* unused,
                                        nw4r::lyt::TextBox* textbox,
                   const wchar_t* str) {
    wchar_t buf[0x400];
    memset(buf, 0, sizeof(buf));
    wcscpy(buf, str);
    nw4r::lyt::Font* font = textbox->GetFont();
    f32 sum = lbl_eu_806671F0;
    u32 i = 0;
    // Separate init from the same constant: retail materializes max via fmr
    // from the sum register and KEEPS the trailing `sum > max` compare (FP
    // compares are not folded). Merging the update into the loop condition
    // rotates the loop into a do-while the retail does not have.
    f32 max = lbl_eu_806671F0;
    // Plain (f32)(int) casts: MWCC emits the 0x4330/xoris/lfd/fsubs idiom
    // byte-identically. Hand-building the bit pattern against
    // lbl_eu_80667200 would name the sdata2 reloc but adds a rounding insn
    // and shifts regalloc - the builtin is the closest byte-identical state
    // (see func_80128C6C note / MWCC_CASES 7i).
    f32 height = (f32)font->v34();
    for (;;) {
        u16 c = str[i];
        if (c == 0) {
            if (sum > max)
                max = sum;
            break;
        }
        sum += (f32)(font->v48(c) + 2);
        i++;
    }
    out[0] = max;
    out[1] = height;
}
#pragma optimize_for_size off

// Message-pane text reset hook: free the old textbox string buffer, allocate
// a fresh 1024-char one, advance the tag-proc write position, run the message
// step (func_80127764) and clear the done flag. No explicit return: the
// retail leaves r3 as whatever func_80127764 left there.
#pragma push
#pragma optimize_for_size on
void func_80127E74(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                   nw4r::lyt::Pane* b, nw4r::lyt::Pane* c) {
    CTalkTextBoxVtbl* tb = (CTalkTextBoxVtbl*)a;
    tb->v78();          // FreeStringBuffer()
    tb->v74(0x400);     // AllocStringBuffer(0x400)
    CTagProcMsg* msg = (CTagProcMsg*)tag;
    msg->field_810++;
    func_80127764(msg, (TalkPaneView*)a, (TalkPaneView*)b, (TalkPaneView*)c,
                  0);
    msg->field_820 = 0;
}
#pragma pop

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
// operator delete when the deleting flag is set. optimize_for_size merges
// the r30/r31 saves into stmw/lmw; the base dtor is noinline so its guard
// isn't inlined into the derived dtor (single null-check, family pattern).
#pragma optimize_for_size on
CTagProcessorSE::~CTagProcessorSE() {}
#pragma optimize_for_size off

// SE message-pump step (CTagProcessorSE variant of func_8012615C, no name
// list): dispatch the next tag code at buf[field_810]. Handlers push tag
// data to the text pane via the 2-arg +0x7C SetText and advance the read
// counters. Tag 5 is the auto-advance speed gate; tags 8/9 drive icons and
// party-member emphasis. Frame layout: locals assigned from the top down
// (first declared = highest address: 0x2F8 conv temp, 0xF8 outbuf,
// 0x1C flag table, 0x10 vals, 0x8 hdr).
__declspec(noinline) int func_80127FB4(nw4r::lyt::AnimTransform* tag,
                                       nw4r::lyt::Pane* paneArg) {
    CTagProcessorSE* msg = (CTagProcessorSE*)tag;
    SeTalkPane* pane = (SeTalkPane*)paneArg;
    u16 outbuf[0x100];      // 0xF8
    TagFlagTable flagTbl;   // 0x1C
    u32 vals[3];            // 0x10
    s16 hdr[3];             // 0x08

    if (msg->mBuf[msg->field_810] == 0)
        return 3;
    // Talk-source / party-member lookups (r31 / r23).
    TagMemberObj* member = 0;
    TagTalkSrc* tsrc = (TagTalkSrc*)func_800BBC0C(findObjectById(msg->field_804));
    if (tsrc != 0)
        member = tsrc->field_98;

    outbuf[0] = 0;
    u16 idx = msg->field_810;
    u16 tagcode = msg->mBuf[idx];
    const u16* p = &msg->mBuf[idx];
    switch (tagcode) {
    case 1: {
        // <1 hi|lo data...> raw text-range tag: copy hi+lo chars to the
        // pane with the header preserved.
        u16 h = p[3];
        u32 count = (h >> 8) + (h & 0xff);
        outbuf[0] = tagcode;
        outbuf[1] = h;
        for (u32 j = 0; j < count; j++)
            outbuf[2 + j] = msg->mBuf[idx + 4 + j];
        outbuf[count + 2] = 0;
        u16 adv = count + 2;
        msg->field_810 += adv;
        pane->v7C(outbuf, msg->field_820);
        msg->field_820 += adv;
        goto finish;
    }

    case 2: {
        // <2 d0 d1> color tag: push both operands plus a 0 tail.
        u16 d0 = p[3];
        u16 d1 = p[4];
        outbuf[0] = tagcode;
        outbuf[1] = d0;
        outbuf[2] = d1;
        outbuf[3] = 0;
        msg->field_810 = idx + 3;
        pane->v7C(outbuf, msg->field_820);
        msg->field_820 += 3;
        goto finish;
    }

    case 3:
        return 4;  // selection-accepted marker

    case 4: {
        // <4 v> wait-for-click tag: v == -1 holds the page.
        s16 v = (s16)p[3];
        msg->field_810 = idx + 2;
        msg->field_820 = 0;
        return v == -1;
    }

    case 5: {
        // <5 v> text-speed / skip tag: v == -1 selects the button-driven
        // auto-advance, v == -2 skips unconditionally, otherwise v is the
        // per-char speed threshold.
        s16 v = (s16)p[3];
        if (v == -1) {
            // NOTE: keep field_824 reads adjacent to their compares (no
            // float local live across the calls below) so MWCC never
            // spills an FPR around the bl.
            if (msg->field_824 >= lbl_eu_80667208) {
                if (msg->field_824 < lbl_eu_80667250) {
                    if (func_eu_8013C8F4() == 0 &&
                        func_80189A04(0) != 0 && msg->field_804 != 0 &&
                        member != 0)
                        member->v58(0, 0);
                    msg->field_824 = lbl_eu_80667234;
                }
                // Button-driven auto-advance: poll the active pad style.
                TagPadView* pad =
                    (TagPadView*)cf::CfGameManager::getCurrentPad();
                member = (TagMemberObj*)pad;  // retail reuses r23 here
                u32 bits;
                if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0)
                    bits = pad->field_04 & 0x00600000;
                else
                    bits = pad->field_04 & 0x00000030;
                if (bits != 0) {
                    msg->field_824 = lbl_eu_806671F0;
                    msg->field_810 += 2;
                    msg->field_820 += 2;
                    func_8018986C(0, lbl_eu_806671F0);
                }
            } else {
                msg->field_824 += lbl_eu_806671F4;
            }
            return 2;
        }
        if (v == -2) {
            // Unconditional skip once the voice slot is free.
            if (func_80189A04(0) != 0) {
                msg->field_824 = lbl_eu_806671F0;
                msg->field_810 += 2;
                msg->field_820 += 2;
            }
            return 2;
        }
        // General path: convert v with the retail sdata2 magic idiom
        // (seed 0x43300000 hi, v^0x80000000 lo, subtract the shared
        // 2^52+2^31 blob) and compare in double.
        TagConvTemp conv;
        conv.w.hi = 0x43300000;
        conv.w.lo = (u32)v ^ 0x80000000;
        if ((f64)msg->field_824 < conv.d - lbl_eu_80667200) {
            if (msg->field_824 < lbl_eu_806671F4 && msg->field_804 != 0 &&
                member != 0)
                member->v58(0, 0);
            msg->field_824 += lbl_eu_806671F4;
        } else {
            if (msg->field_804 != 0 && member != 0)
                member->v58(1, 0);
            msg->field_824 = lbl_eu_806671F0;
            msg->field_810 += 2;
            msg->field_820 += 2;
        }
        return 2;
    }

    case 8: {
        // <8 b0 b1 ...> icon tag: map the three header bytes through the
        // 0x1B-entry flag table into icon ids (func_8004B9D4 call or
        // pending field_828/830/838 stores), then advance 3.
        u16 h0 = p[3];
        u16 h1 = p[4];
        hdr[0] = h0 >> 8;
        hdr[1] = h0 & 0xff;
        hdr[2] = h1 >> 8;
        if (msg->field_804 != 0 && tsrc != 0) {
            flagTbl = lbl_eu_804FF6E0;
            vals[0] = 0;
            vals[1] = 0;
            vals[2] = 0;
            for (int k = 0; k < 3; k++) {
                s16 id = hdr[k];
                if (id == 0xff)
                    break;
                u32 val = flagTbl.v[id];
                vals[k] = val;
                if (k == 0) {
                    TagC4Obj* w = tsrc->field_C4;
                    if (w->field_270 & 0x01000000) {
                        msg->field_828 = val;
                        msg->field_82c = 1;
                    } else {
                        func_8004B9D4(w, val, 0, -1, 0);
                    }
                } else if (k == 1) {
                    msg->field_830 = val;
                    msg->field_834 = 1;
                } else {
                    msg->field_838 = val;
                    msg->field_83c = 1;
                }
            }
        }
        msg->field_810 = idx + 3;
        goto finish;
    }

    case 9: {
        // <9 v> party-member emphasis tag: v == 0 enables, v == -1 disables
        // the member's follow highlight.
        s16 v = (s16)p[3];
        if (msg->field_804 != 0 && member != 0) {
            if (v == 0)
                member->v58(1, 0);
            else if (v == -1)
                member->v58(0, 0);
        }
        msg->field_810 = idx + 2;
        goto finish;
    }

    default:
        // Unknown code: push <code, 0> verbatim and advance both counters.
        outbuf[0] = tagcode;
        outbuf[1] = 0;
        msg->field_810 = idx + 1;
        pane->v7C(outbuf, msg->field_820);
        msg->field_820 += 1;
        goto finish;
    }

finish:
    // Icon-pending tail: display a pending icon once its window is ready.
    if (msg->field_82c != 0) {
        if (msg->field_804 != 0 && tsrc != 0) {
            TagC4Obj* w = tsrc->field_C4;
            u32 flags = w->field_270;
            // Display unless the hold bit (0x1000000) is set without the
            // wait-release bit (0x800000).
            if ((flags & 0x01000000) == 0 || (flags & 0x00800000) != 0) {
                func_8004B9D4(w, msg->field_828, 0, -1, 0);
                msg->field_82c = 0;
            }
        }
    } else if (msg->field_834 != 0) {
        if (msg->field_804 != 0 && tsrc != 0) {
            TagC4Obj* w = tsrc->field_C4;
            if (w->v80(0)) {
                func_8004B9D4(w, msg->field_830, 0, -1, 0);
                msg->field_834 = 0;
            }
        }
    } else if (msg->field_83c != 0) {
        if (msg->field_804 != 0 && tsrc != 0) {
            TagC4Obj* w = tsrc->field_C4;
            if (w->v80(0)) {
                func_8004B9D4(w, msg->field_838, 0, -1, 0);
                msg->field_83c = 0;
            }
        }
    }
    return -1;
}

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
    // Retail computes BOTH differences first, then both products; out2
    // entries forward the stored v3 values.
    f32 dy = v3[1] - out[1];
    f32 dx = v3[0] - out[0];
    v3[1] = dy * lbl_eu_80667228;
    v3[0] = dx * lbl_eu_80667228;
    out2[0] = v3[0];
    out2[1] = lbl_eu_80667258 - v3[1];
    copyVEC3(pv->vec_2C.v, out2);
    pv->v78();
    pv->v74(0x400);
}
#pragma optimize_for_size off

// Tag-writer family (small variants): init a TagParam block with a tag code
// NOTE: retail prologue is stwu; mflr; stw r0; stmw r30; or r30,r4,r4;
// or r31,r6,r6 — the optimize_for_size pragma merges the callee-saved saves
// into stmw/lmw and fixes the copy order (MWCC_CASES CPartyState
// func_801FD0A0 pattern); plain -O4,p emits reversed stw pairs + moves.
#pragma optimize_for_size on
extern "C" void* func_801289B4(void* a, void* b, u8 code, u8* dst) {
    TagParam p;
    func_80125944(&p, 7);
    copyTagParam(dst, (const u8*)&p);
    return b;
}
#pragma optimize_for_size off

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

// Same stmw/lmw save-merge + copy-order fix as func_801289B4.
#pragma optimize_for_size on
extern "C" void* func_80128A70(void* a, void* b, u8 code, u8* dst) {
    TagParam p;
    func_80125944(&p, 5);
    copyTagParam(dst, (const u8*)&p);
    return b;
}
#pragma optimize_for_size off

// Build a fresh tag-param (mode 8) on the stack, copy it onto the target,
// set its +1 flag, and return the ret pointer (retail keeps a1/a3 phantom).
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
extern "C" void* func_80128AB8(u8* a1, TagParam* ret, u8 a3, TagParam* target) {
    TagParam tmp;
    func_80125944(&tmp, 8);
    copyTagParam((u8*)target, (const u8*)&tmp);
    target->field_01 |= 1;
    return ret;
}
#pragma optimize_for_size off

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

// Same stmw/lmw save-merge + copy-order fix as func_801289B4.
#pragma optimize_for_size on
extern "C" void* func_80128BB0(void* a, void* b, u8 code, u8* dst) {
    TagParam p;
    func_80125944(&p, 3);
    copyTagParam(dst, (const u8*)&p);
    return b;
}
#pragma optimize_for_size off

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
        // the named sdata2 blob lbl_eu_80667200 (MWCC_CASES 7i - the
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
    TagColorValues values;
    TagColorNames names;
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
    if (wcscmp(str, &lbl_eu_80661FC8[0x12]) == 0)
        v = 0;
    else if (wcscmp(str, &lbl_eu_80661FC8[0x15]) == 0)
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
        // 'A'..'F' are valid nibble positions that contribute nothing; kept
        // as explicit cases so the jump table spans '1'..'F' like retail.
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
            // 'A'..'F' are valid hex positions but contribute nothing; kept
            // as explicit cases so the jump table spans '1'..'F' like retail.
            // NOTE: this MWCC build still folds them out of the emitted table
            // (cmpli bound 14 vs retail 21); retained for semantic fidelity.
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
                break;
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
    nw4r::ut::Color c = *arg->field_04;
    u32 hi = (u32)arg->field_00 & 0xFF000000;
    nw4r::ut::CharWriter* w = arg->field_00;
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
    c.comp.a = cur.comp.a;
    nw4r::ut::Color merged = c;
    validRegs2 = true;
    validRegs = true;
    validIo2 = true;
    validIo = true;
    validMem2 = true;
    validMem1 = true;
    w = arg->field_00;
    hi = (u32)arg->field_00 & 0xFF000000;
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
    const u8* buf = holder->field_04;
    nw4r::ut::TextWriterBase<wchar_t>* tw = holder->field_00;
    u16 h = *(const u16*)buf;
    u8 lenLo = h;
    u32 lenHi = (h >> 8) & 0xFF;
    const wchar_t* s2 = (const wchar_t*)(buf + 2);
    const wchar_t* s1 = s2 + ((h >> 8) & 0xFF);
    nw4r::ut::TextWriterBase<wchar_t> local;
    func_80129D1C((TagCopyBlock*)&local, (const TagCopyBlock*)tw);
    func_80129E20(&local, 0);
    func_8012B204(&local, lbl_eu_806671F0);
    f32 yscale = lbl_eu_80667264 * func_80129C04(&local);
    f32 xscale = func_80129AEC(&local);
    func_80129F3C(&local, lbl_eu_80667264 * xscale, yscale);
    func_8012930C(&local, lbl_eu_80667260);
    f32 wlimit = func_801291F4(tw);
    func_8012930C(tw, lbl_eu_80667260);
    // "no leading char space" flag, materialized before the width calls
    u32 noSpace = (holder->field_10 & 1) ^ 1;
    f32 w1 = tw->CalcStringWidth(s1, lenLo);
    f32 w2 = local.CalcStringWidth(s2, lenHi);
    if (w1 < w2)
        w1 = w2;
    f32 cs;
    if (noSpace != 0)
        cs = func_8012B328(tw);
    else
        cs = lbl_eu_806671F0;
    w1 += cs;
    func_8012930C(tw, wlimit);
    func_80129430(tw, w1);
    out->field_08 = out->field_00 + w1;
    addToCharSpace((u8*)out, tw->GetFontHeight());
    holder->field_04 = (const u8*)(s1 + lenLo);
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

// Tag-writer two-string layout: read the <lenHi|lenLo> header, validate the
// active writer, then copy it into a scratch writer (zero font size + char
// space, rescaled by the sdata2 factor). The first string (lenHi chars) is
// measured on the scratch writer; if the real writer's copy of that string
// is wider (excess > 0), the excess is distributed as scratch char space and
// folded into the saved cursor x. Both strings are printed at the adjusted
// cursor (y lifted by ascent+descent), the excess is walked back off the
// real writer's cursor twice (once per print), and the buffer pointer
// advances past both strings. Returns 2.
__declspec(noinline) int func_8012B440(void* unused, void* unused2,
                                       TagWriterHolder* holder) {
    // Long-lived locals declared up front in MWCC's callee-saved priority
    // order (first declared -> highest register); assigned in statement order
    // below, matching the retail allocation map (r31..r25).
    u8 lenLo;
    u32 savedFontSize;
    nw4r::ut::TextWriterBase<wchar_t>* msg;
    u8 lenHi;
    const u16* s1;
    const u16* s2;
    msg = holder->field_00;
    f32 savedWidth = func_801291F4(msg);
    func_8012930C(msg, lbl_eu_80667260);
    // "no leading char space" flag: re-apply the current char space when set.
    if ((holder->field_10 & 1) == 0)
        func_80129430(msg, func_8012B328(msg));

    // Validation-flag scratch: initialized before the header decode so the
    // allocator pins them to the low volatile window (retail r4..r9).
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;

    const u8* buf = holder->field_04;
    u32 h = ((const u16*)buf)[0];
    s1 = (const u16*)(buf + 2);
    lenHi = h >> 8;
    lenLo = h & 0xFF;
    s2 = s1 + lenHi;

    u32 hi = (u32)msg & 0xFF000000;
    if (hi != 0x80000000 && ((u32)msg & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)msg & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)msg & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)msg & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)msg & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DB08, 0x90, lbl_eu_8052DAD4, msg);
    }

    savedFontSize = msg->field_5C;
    f32 cursorX = func_801299D4(msg);
    f32 cursorY = func_80129564(msg);
    if (lenHi == 0) {
        nw4r::db::Panic(&lbl_eu_804FFC48[0x27], 0x120,
                        &lbl_eu_804FFC48[0x15]);
    }

    nw4r::ut::TextWriterBase<wchar_t> local;
    func_80129D1C((TagCopyBlock*)&local, (TagCopyBlock*)msg);
    func_80129E20(&local, 0x300);
    func_8012B204(&local, lbl_eu_806671F0);

    validRegs2 = true;
    validRegs = true;
    validIo2 = true;
    validIo = true;
    validMem2 = true;
    validMem1 = true;
    hi = (u32)&local & 0xFF000000;
    if (hi != 0x80000000 && ((u32)&local & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && ((u32)&local & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && hi != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && ((u32)&local & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && ((u32)&local & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && ((u32)&local & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        nw4r::db::Panic(lbl_eu_8052DAC0, 0x5d, lbl_eu_8052DA8C, &local);
    }

    local.field_5Cf = lbl_eu_806671F0;
    f32 yscale = lbl_eu_80667264 * func_80129C04(&local);
    f32 xscale = func_80129AEC(&local);
    func_80129F3C(&local, lbl_eu_80667264 * xscale, yscale);

    f32 wMsg = msg->CalcStringWidth((const wchar_t*)s1, lenHi);
    f32 wLocal = local.CalcStringWidth((const wchar_t*)s1, lenHi);
    f32 excess = wMsg - wLocal;
    if (excess > lbl_eu_806671F0) {
        // Distribute the excess width across lenHi chars: fold one char
        // space worth into the saved cursor x and give the scratch writer
        // the per-char share. int->float via the retail magic-constant idiom.
        // volatile keeps MWCC from scalar-replacing the union and folding
        // the subtract into an unsigned library conversion call.
        volatile TagConvTemp conv;
        conv.w.lo = (u32)lenHi ^ 0x80000000;
        conv.w.hi = 0x43300000;
        f32 lenF = (f32)(conv.d - lbl_eu_80667200);
        f32 share = excess / lenF;
        cursorX += share * lbl_eu_80667228;
        func_8012B204(&local, share);
    }

    f32 descent = local.GetFontDescent();
    f32 height = msg->GetFontAscent() + descent;
    func_8012A070(&local, cursorX, cursorY - height);
    local.Print((const wchar_t*)s1, lenHi);
    if (excess < lbl_eu_806671F0) {
        func_80129430(msg, -excess * lbl_eu_80667228);
    }
    func_80129E20(msg, 0x300);
    msg->Print((const wchar_t*)s2, lenLo);
    func_80129E20(msg, savedFontSize);
    if (excess < lbl_eu_806671F0) {
        func_80129430(msg, -excess * lbl_eu_80667228);
    }
    holder->field_04 = (const u8*)(s2 + lenLo);
    func_8012930C(msg, savedWidth);
    return 2;
}

// Tag-code writer (code 4): same shape as func_8012A1A4, but the first match
// selects -1 and the second selects 0 (strings at +0x17e / +0x186).
#pragma optimize_for_size on
u16* func_8012B8C4(void* a, u16* dst, wchar_t* str) {
    func_801366F4((u16*)str);
    s16 v = 0xff;
    if (wcscmp(str, &lbl_eu_80661FC8[0xbf]) == 0)
        v = -1;
    else if (wcscmp(str, &lbl_eu_80661FC8[0xc3]) == 0)
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
