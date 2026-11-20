// Auto-scaffolded catalog TU for kyoshin/CTagProcessor
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"

#include "kyoshin/CTagProcessor.hpp"

#include <wchar.h>  // wcstol (MSL, C-linkage)

/* Tag-code handler hierarchy.
 *
 * Retail .data carries 23 vtable + base-list pairs (0x8052D618..0x8052DA08)
 * and .sdata one 8-byte type_info per class (0x80661EE8..0x80661FA8), both in
 * *reverse* declaration order (first declared = highest address), so the
 * classes are declared in tag order, CL -> MakeCrystal, inside one anonymous
 * namespace and the compiler emits the whole block. Slot order is fixed by
 * ITagCode: [0] dtor, [1] Process, [2] CalcRect, [3] v14. The three tags that
 * need a custom Process/CalcRect (Col, Ruby, Line) override them; the rest
 * inherit the ITagCode defaults, and every class overrides v14 (its
 * tag-body writer, whose trailing slots in the table are those addresses).
 */
namespace {

class __declspec(novtable) ITagCode {
public:
    typedef nw4r::ut::TagProcessorBase<wchar_t>::Operation Operation;

    virtual ~ITagCode();
    virtual Operation Process(u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx);
    virtual Operation CalcRect(nw4r::ut::Rect* rect, u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx);
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm) = 0;
};

class CTagCodeCL : public ITagCode {
public:
    CTagCodeCL() {}
    virtual ~CTagCodeCL();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeCP : public ITagCode {
public:
    CTagCodeCP() {}
    virtual ~CTagCodeCP();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeWait : public ITagCode {
public:
    CTagCodeWait() {}
    virtual ~CTagCodeWait();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeDel : public ITagCode {
public:
    CTagCodeDel() {}
    virtual ~CTagCodeDel();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeRuby : public ITagCode {
public:
    CTagCodeRuby() {}
    virtual ~CTagCodeRuby();
    virtual Operation Process(u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx);
    virtual Operation CalcRect(nw4r::ut::Rect* rect, u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx);
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeCol : public ITagCode {
public:
    CTagCodeCol() {}
    virtual ~CTagCodeCol();
    virtual Operation Process(u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx);
    virtual Operation CalcRect(nw4r::ut::Rect* rect, u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx);
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeTrust : public ITagCode {
public:
    CTagCodeTrust() {}
    virtual ~CTagCodeTrust();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeType : public ITagCode {
public:
    CTagCodeType() {}
    virtual ~CTagCodeType();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodePos : public ITagCode {
public:
    CTagCodePos() {}
    virtual ~CTagCodePos();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeMouth : public ITagCode {
public:
    CTagCodeMouth() {}
    virtual ~CTagCodeMouth();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeLine : public ITagCode {
public:
    CTagCodeLine() {}
    virtual ~CTagCodeLine();
    virtual Operation Process(u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx);
    virtual Operation CalcRect(nw4r::ut::Rect* rect, u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx);
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeSelect : public ITagCode {
public:
    CTagCodeSelect() {}
    virtual ~CTagCodeSelect();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeSelect2 : public ITagCode {
public:
    CTagCodeSelect2() {}
    virtual ~CTagCodeSelect2();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeAct : public ITagCode {
public:
    CTagCodeAct() {}
    virtual ~CTagCodeAct();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeFade : public ITagCode {
public:
    CTagCodeFade() {}
    virtual ~CTagCodeFade();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeQuest : public ITagCode {
public:
    CTagCodeQuest() {}
    virtual ~CTagCodeQuest();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeInvisible : public ITagCode {
public:
    CTagCodeInvisible() {}
    virtual ~CTagCodeInvisible();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeQstStart : public ITagCode {
public:
    CTagCodeQstStart() {}
    virtual ~CTagCodeQstStart();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodePresent : public ITagCode {
public:
    CTagCodePresent() {}
    virtual ~CTagCodePresent();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeTailOff : public ITagCode {
public:
    CTagCodeTailOff() {}
    virtual ~CTagCodeTailOff();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeExchange : public ITagCode {
public:
    CTagCodeExchange() {}
    virtual ~CTagCodeExchange();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeShop : public ITagCode {
public:
    CTagCodeShop() {}
    virtual ~CTagCodeShop();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

class CTagCodeMakeCrystal : public ITagCode {
public:
    CTagCodeMakeCrystal() {}
    virtual ~CTagCodeMakeCrystal();
    virtual u16* v14(u16* buf, const wchar_t* arg, TagParam* prm);
};

/* Empty destructors: MWCC emits the retail `if (this && deleting) operator
 * delete(this)` deleting-destructor body for each (0x40 B, one per class). */
ITagCode::~ITagCode() {}
CTagCodeCL::~CTagCodeCL() {}
CTagCodeCP::~CTagCodeCP() {}
CTagCodeWait::~CTagCodeWait() {}
CTagCodeDel::~CTagCodeDel() {}
CTagCodeRuby::~CTagCodeRuby() {}
CTagCodeCol::~CTagCodeCol() {}
CTagCodeTrust::~CTagCodeTrust() {}
CTagCodeType::~CTagCodeType() {}
CTagCodePos::~CTagCodePos() {}
CTagCodeMouth::~CTagCodeMouth() {}
CTagCodeLine::~CTagCodeLine() {}
CTagCodeSelect::~CTagCodeSelect() {}
CTagCodeSelect2::~CTagCodeSelect2() {}
CTagCodeAct::~CTagCodeAct() {}
CTagCodeFade::~CTagCodeFade() {}
CTagCodeQuest::~CTagCodeQuest() {}
CTagCodeInvisible::~CTagCodeInvisible() {}
CTagCodeQstStart::~CTagCodeQstStart() {}
CTagCodePresent::~CTagCodePresent() {}
CTagCodeTailOff::~CTagCodeTailOff() {}
CTagCodeExchange::~CTagCodeExchange() {}
CTagCodeShop::~CTagCodeShop() {}
CTagCodeMakeCrystal::~CTagCodeMakeCrystal() {}

CTagCodeCL s_CL;
CTagCodeCP s_CP;
CTagCodeWait s_Wait;
CTagCodeDel s_Del;
CTagCodeRuby s_Ruby;
CTagCodeCol s_Col;
CTagCodeTrust s_Trust;
CTagCodeType s_Type;
CTagCodePos s_Pos;
CTagCodeMouth s_Mouth;
CTagCodeLine s_Line;
CTagCodeSelect s_Select;
CTagCodeSelect2 s_Select2;
CTagCodeAct s_Act;
CTagCodeFade s_Fade;
CTagCodeQuest s_Quest;
CTagCodeInvisible s_Invisible;
CTagCodeQstStart s_QstStart;
CTagCodePresent s_Present;
CTagCodeTailOff s_TailOff;
CTagCodeExchange s_Exchange;
CTagCodeShop s_Shop;
CTagCodeMakeCrystal s_MakeCrystal;

struct TagTableEntry {
    u16 tag;              // +0x00 tag code
    const wchar_t* name;  // +0x04 tag name (<NAME=value>); null terminates
    ITagCode* obj;        // +0x08 handler singleton
};

TagTableEntry s_tagTable[] = {
    { 0, L"N", &s_CL },
    { 3, L"P", &s_CP },
    { 5, L"WAIT", &s_Wait },
    { 4, L"DEL", &s_Del },
    { 1, L"RUBY", &s_Ruby },
    { 2, L"COL", &s_Col },
    { 2, L"C", &s_Col },
    { 11, L"TRUST", &s_Trust },
    { 0, L"TYPE", &s_Type },
    { 0, L"POS", &s_Pos },
    { 9, L"JAW", &s_Mouth },
    { 13, L"LINE", &s_Line },
    { 6, L"SEL", &s_Select },
    { 7, L"SEL2", &s_Select2 },
    { 8, L"ACT", &s_Act },
    { 0, L"FADE", &s_Fade },
    { 0, L"QST", &s_Quest },
    { 0, L"INV", &s_Invisible },
    { 0, L"QST_START", &s_QstStart },
    { 0, L"PRESENT", &s_Present },
    { 0, L"TAIL_OFF", &s_TailOff },
    { 0, L"EX", &s_Exchange },
    { 0, L"SHP", &s_Shop },
    { 0, L"CRYSTAL", &s_MakeCrystal },
    { 0, 0, 0 }
};
}  // namespace

// Reset a tag-param block to all zero (float field takes the sdata2 constant).
// via bl and the retail reloc name is the flat TagParamClear.
void TagParamClear(TagParam* p) {
    p->field_00 = 0;
    p->field_01 = 0;
    p->field_02 = 0;
    p->field_03 = 0;
    p->field_04 = 0;
    p->field_06 = 0;
    p->field_08 = lbl_eu_806671F0;
}

// Init a tag-param block from a byte code and a u16 value.
void initTagParam(u8* obj, unsigned char a, unsigned char b, float c) {
    unsigned char* base = (unsigned char*)obj;
    base[0] = a;
    base[1] = 0;
    base[2] = 0;
    base[3] = 0;
    ((TagParam*)base)->field_04 = 0;
    base[6] = b;
    ((TagParam*)base)->field_08 = c;
}

// Init a tag-param block with a byte code (field_00) and a u16 value (field_04).
extern "C" void TagParamInitCodeU16(TagParam* p, u8 a, u16 b) {
    p->field_00 = a;
    p->field_01 = 0;
    p->field_02 = 0;
    p->field_03 = 0;
    p->field_04 = b;
    p->field_06 = 0;
    p->field_08 = lbl_eu_806671F0;
}

// Init a tag-param block with a byte code (field_00) only.
extern "C" void TagParamInitCode(TagParam* p, u8 a) {
    p->field_00 = a;
    p->field_01 = 0;
    p->field_02 = 0;
    p->field_03 = 0;
    p->field_04 = 0;
    p->field_06 = 0;
    p->field_08 = lbl_eu_806671F0;
}

// Empty out-of-line destructor: auto-inlined at every local/temporary below,
// but kept as a real function because ~CTagProcessorBase's exception-cleanup
// record destroys field_814 through it.
TagParam::~TagParam() {}

// Tag-processor base constructor (retail __ct__17CTagProcessorBaseFv): the member
// ctor provides the compiler-generated nw4r base-ctor call and the vptr store
// to the compiler-emitted vtable -- placement new would add a null beq the
// retail does not have.
CTagProcessorBase::CTagProcessorBase() {
    field_804 = 0;
    field_808 = lbl_eu_806671F0;
    field_80c = lbl_eu_806671F0;
    field_810 = 0;
    TagParamClear(&field_814);
    memset(mBuf, 0, sizeof(mBuf));
    TagParam tmp;
    TagParamClear(&tmp);
    copyTagParam((u8*)&field_814, (const u8*)&tmp);
}

// Copy a 12-byte tag-param block byte-by-byte.
extern "C" void copyTagParam(u8* dst, const u8* src) {
    unsigned char* d = (unsigned char*)dst;
    const unsigned char* s = (const unsigned char*)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
    ((TagParam*)d)->field_04 = ((const TagParam*)s)->field_04;
    d[6] = s[6];
    ((TagParam*)d)->field_08 = ((const TagParam*)s)->field_08;
}

// Deleting destructor for CTagProcessorBase (retail __dt__17CTagProcessorBaseFv):
// genuine C++ member dtor of the derived class -- the compiler emits the nw4r
// base-dtor call with flag 0 and the operator delete when the deleting flag is set
// (the retail shape). Derived-class dtors must keep a real `bl` to this (retail:
// __dt__13CTagProcessorFv 0x80126BE8 and __dt__15CTagProcessorSEFv 0x80128A34 both
// emit `li r4,0; bl` with no extra null guard). Definition order already matches
// retail (this dtor sits above both), -O4,s / -ipa off do not stop the front end
// folding the empty body, so the declspec is the only lever.
__declspec(noinline) CTagProcessorBase::~CTagProcessorBase() {}  // noinline-ok: TU boundary ok (this split), definition order already retail, -O4,p/-O4,s/-ipa off tried; front end folds the empty member dtor

// Tag-code dispatch for Process: walk the handler table; a null handler
// falls back to the base TagProcessor, a matching tag dispatches to the
// handler object's Process override (vtable slot 3). The byte offset keeps
// MWCC's base+offset induction shape (retail: add r8,r7,r6 / addi r6,r6,0xC).
// Tag-code dispatch for Process: walk the handler table; a null handler
// falls back to the base TagProcessor, a matching tag dispatches to the
// handler object's Process override (vtable slot 3). The byte offset keeps
// MWCC's base+offset induction shape (retail: add r8,r7,r6 / addi r6,r6,0xC).
nw4r::ut::TagProcessorBase<wchar_t>::Operation TagProcDispatchProcess(
    nw4r::ut::TagProcessorBase<wchar_t>* self, u16 tag,
    nw4r::ut::PrintContext<wchar_t>* ctx) {
    for (int i = 0;; i++) {
        const TagTableEntry* e = &s_tagTable[i];
        if (e->name == 0) {
            return self->TagProcessorBase<wchar_t>::Process(tag, ctx);
        }
        if (tag == e->tag) {
            return e->obj->Process(tag, ctx);
        }
    }
}

// ITagCode's default slots: the two shared 8-byte `li r3,0; blr` handlers
// (vtable slots 1/2 for the 20 tag classes that do not override them).
nw4r::ut::TagProcessorBase<wchar_t>::Operation ITagCode::Process(u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx) { return nw4r::ut::TagProcessorBase<wchar_t>::OPERATION_DEFAULT; }

// Tag-code dispatch for CalcRect:
// falls back to the base TagProcessor, a matching tag dispatches to the
// handler object's CalcRect override (vtable slot 4).
nw4r::ut::TagProcessorBase<wchar_t>::Operation TagProcDispatchCalcRect(
    nw4r::ut::TagProcessorBase<wchar_t>* self, nw4r::ut::Rect* rect,
    u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx) {
    for (int i = 0;; i++) {
        const TagTableEntry* e = &s_tagTable[i];
        if (e->name == 0) {
            return self->TagProcessorBase<wchar_t>::CalcRect(rect, tag, ctx);
        }
        if (tag == e->tag) {
            return e->obj->CalcRect(rect, tag, ctx);
        }
    }
}

nw4r::ut::TagProcessorBase<wchar_t>::Operation ITagCode::CalcRect(nw4r::ut::Rect* rect, u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx) { return nw4r::ut::TagProcessorBase<wchar_t>::OPERATION_DEFAULT; }

// Message pre-processor: copy the raw message into the buffer, then walk it
// in place - a CRLF at the head is stripped (the tail is copied down over
// it), and a '<name=value>' tag is split on '=' by the tokenizer helpers,
// looked up by name in the tag dispatch table, and consumed by the matching
// handler object (vtable slot +0x14), which returns the new buffer position;
// the saved tail (everything past '>') is then copied down over the consumed
// text. An unmatched tag name is stripped the same way (the handler loop just
// falls through). Returns the buffer.
u16* TagProcPreprocessMessage(CTagProcessorBase* self, const void* src, f32 a, f32 b, u32 c) {
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
            SplitWStringTrim(scratchA, 0x3D, (u16**)tokens);
            WcsToUpperInPlace((u16*)tokens[0]);
            wcscpy(bigbuf, p + count + 2);
            const wchar_t* name = tokens[0];
            for (const TagTableEntry* e = s_tagTable;; e++) {
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

// Text-position measurement for the tag-writer family: walk the message buffer from
// the TextBox, accumulating the max line width (per-glyph font width + the TextBox's
// per-char spacing at +0xF4) and the line height (0xA and the row-advance inside tag 5
// step by the fixed sdata2 constant). Control tags advance the buffer by their operand
// lengths. RESIDUAL (open item): sizes now MATCH exactly (900B); shapes align through
// the prologue and all case blocks. What remains: 1. Register-allocation permutation:
// retail colors &vals LAST (r21, lowest priority - the store loop even rematerializes
// r1+8), ours ranks it TOP (r31), shifting msg/out/buf/font/i down one slot each,
// permuting inner temps + the whole FPR map (lineH f30 vs f27 etc.). Proven inert
// levers: declaration order/position, int-vs-unsigned counters (retail is signed:
// srawi/addze ceil-div, cmpw loops), pointer temps, indexed writes, renames, explicit
// loop-top pointer. 2. Conversion magic: retail references NAMED lbl_eu_80667200;
// implicit casts emit private pooled literals (@7188). An explicit TagConvTemp union
// (hi=0x43300000 seeded once, lo=w^0x80000000, then d - lbl_eu_80667200) names the
// reloc but perturbed scheduling (+8B) when tried standalone - revisit after (1) is
// solved. 3. Jump table: ours anchors a local object 32B below retail's
// jumptable_eu_8052D5A4 despite equal size; likely follows from .data layout, may
// resolve once (1) is fixed.
void TagProcMeasureText(f32* out, nw4r::lyt::TextBox* msg, wchar_t* buf) {
    u32 vals[16];
    const nw4r::ut::Font* font = msg->GetFont();
    f32 lineH = 1.0f;
    f32 width = 0.0f;
    f32 lineStep = lineH;
    f32 max = width;
    f32 speed = 2.0f;
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
            width = 0.0f;
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
                f32 sp = msg->GetCharSpace();
                width += sp + (f32)font->GetCharWidth(p[j]);
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
                    f32 sp = msg->GetCharSpace();
                    width += sp + (f32)font->GetCharWidth(q[k]);
                }
                if (row < cnt - 1) {
                    lineH += lineStep;
                    if (width > max)
                        max = width;
                    width = 0.0f;
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
            width += speed * (msg->GetCharSpace() + (f32)font->GetCharWidth(0x2500));
            break;
        default:
            width += msg->GetCharSpace() + (f32)font->GetCharWidth(ch);
            break;
        }
        i++;
    }
    out[0] = max;
    out[1] = lineH;
}

// Tag-proc constructor for the full processor: base-construct, then the
// CTagProcessor-specific fields. The 0x838 block is zeroed via memset.
void* __ct__CTagProcessor(void* self) {
    CTagProcessor* p = (CTagProcessor*)self;
    __ct__17CTagProcessorBaseFv(p);
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

// Deleting destructor for CTagProcessor (retail __dt__13CTagProcessorFv): genuine C++
// member dtor of the derived class -- the compiler emits the base dtor call
// (__dt__17CTagProcessorBaseFv) with flag 0 and the operator delete when the deleting
// flag is set (the retail shape).
CTagProcessor::~CTagProcessor() {}

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
int TagProcStepMessage(nw4r::lyt::AnimTransform* tag,
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
    for (int i = 0;; i++) {
        const TagTableEntry* e = &s_tagTable[i];
        if (e->name == 0) {
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
                    if (speed < 3.0f) {
                        msg->field_824 += 1.0f;
                    } else {
                        CPad* pad = cf::CfGameManager::getCurrentPad();
                        int pressed;
                        if (cf::CfGameManager::isClassicController(-1) != 0)
                            pressed = (pad->mPressedButtonFlags & 0x00600000) != 0;
                        else
                            pressed = (pad->mPressedButtonFlags & 0x00000030) != 0;
                        if (pressed != 0) {
                            msg->field_824 = 0.0f;
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
                    if (speed < 1.0f && member != 0)
                        member->v58(0, 0);
                    msg->field_824 += 1.0f;
                } else {
                    if (member != 0)
                        member->v58(1, 0);
                    msg->field_824 = 0.0f;
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
                            MenuStateClear6405A();
                        }
                    } else {
                        int val = (int)CtrlRemote_TouchBitByArg(v4 + 0x29) + v28;
                        if (val < 0)
                            val = 0;
                        s32 bitIdx;
                        if (val > 0x1388) {
                            bitIdx = v28 - (val - 0x1388);
                            if (bitIdx < 0)
                                bitIdx = 0;
                        } else {
                            bitIdx = v28;
                        }
                        bitIdx += code80135FDC_getWord_6405C();
                        code80135FDC_setPair_6405C_64060(
                            bitIdx, code80135FDC_getWord_64060() + v28);
                    }
                }
                u32 v24 = 0;
                if (type == 1) {
                    if (v7 != CtrlRemote_TouchBitByArg(v6 + 0x608)) {
                        s32 bitIdx = v4 + 0x21;
                        int val = (int)CtrlRemote_TouchBitByArg(bitIdx) + v28;
                        if (val < 0)
                            val = 0;
                        if (val > 0x2710)
                            val = 0x2710;
                        CtrlRemote_SetSharedBit(bitIdx, val);
                    } else {
                        v24 = 1;
                    }
                } else if (type == 2) {
                    s32 bitIdx = v4 + 0x29;
                    int val = (int)CtrlRemote_TouchBitByArg(bitIdx) + v28;
                    if (val < 0)
                        val = 0;
                    if (val > 0x1388) {
                        v26 = v28;
                        v28 = v28 - (s16)(val - 0x1388);
                        if (v28 < 0)
                            v28 = 0;
                        val = 0x1388;
                    }
                    CtrlRemote_SetSharedBit(bitIdx, val);
                }
                if (v26 != 0)
                    v28 = v26;
                if (v28 == 0)
                    v24 = 1;
                if (v24 == 0) {
                    s32 sound = 0;
                    if (v28 > 0) {
                        sound = (type == 1) ? 0xc6 : 0xac;
                        playUISound(0x34);
                    } else if (v28 < 0) {
                        sound = (type == 1) ? 0xc8 : 0xae;
                        playUISound(0x36);
                    }
                    if (tsrc != 0)
                        bindIndexedEffect(sound, tsrc);
                    if (player != 0)
                        bindIndexedEffect(sound, player);
                }
                if (type == 1) {
                    if (player != 0)
                        UIWin_CreateMenuUpdate(4, player->field_8C, v4, v28);
                } else if (type == 2) {
                    UIWin_CreateMenuUpdate(5, 0, v4 + 1, v28);
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
                    TagProcRenderPage(msg, (nw4r::lyt::TextBox*)pane, (nw4r::lyt::TextBox*)b,
                                   (nw4r::lyt::TextBox*)c, (const wchar_t*)namebuf);
                    u8* ctxBase = (u8*)pane->field_0C + 0x10;
                    // walk 1: find the context node matching the current text
                    ctxA = (u32*)getContextStr(ctxBase);
                    for (;;) {
                        u8* node = (u8*)TagCtxGetNodeHeader(&ctxA);
                        if (strcmp(((nw4r::lyt::Pane*)node)->GetName(),
                                   ((nw4r::lyt::Pane*)pane)->GetName()) == 0)
                            break;
                        ctxA = (u32*)*ctxA;
                        pair1[0] = (u32)getContextStrPtr(ctxBase);
                        pair1[1] = (u32)ctxA;
                        if (TagProcCompareIds(&pair1[1], &pair1[0]) == 0)
                            break;
                    }
                    TagCtxAdvanceHead(&ctxA, 0);
                    // walk 2: create the per-name panes, set text + colors
                    u32 paneIdx = 0;
                    for (;;) {
                        pair2[0] = (u32)getContextStrPtr(ctxBase);
                        pair2[1] = (u32)ctxA;
                        if (TagProcCompareIds(&pair2[1], &pair2[0]) == 0)
                            break;
                        if (paneIdx >= msg->field_830)
                            break;
                        TalkNamePane* np = pane->field_0C->v3C(
                            (const wchar_t*)((u8*)TagCtxGetNodeHeader(&ctxA) + 0xbc),
                            1);
                        copyVEC2(&np->field_4C[0], &pane->field_4C[0]);
                        np->v78();
                        np->v74(0x20);
                        np->v7C((const u16*)&rows[paneIdx * 0x20], 0);
                        cW2a = nw4r::ut::Color(0x5a5a5aff);
                        cW2b = nw4r::ut::Color(0x5a5a5aff);
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
                            u8* node = (u8*)TagCtxGetNodeHeader(&ctxB);
                            if (strcmp(((nw4r::lyt::Pane*)node)->GetName(),
                                       ((nw4r::lyt::Pane*)pane)->GetName()) == 0)
                                break;
                            ctxB = (u32*)*ctxB;
                            pair3[0] = (u32)getContextStrPtr(ctxBase);
                            pair3[1] = (u32)ctxB;
                            if (TagProcCompareIds(&pair3[1], &pair3[0]) == 0)
                                break;
                        }
                        TagCtxAdvanceHead(&ctxB, 0);
                        // walk 4: highlight (last name selected)
                        paneIdx = 0;
                        for (;;) {
                            pair4[0] = (u32)getContextStrPtr(ctxBase);
                            pair4[1] = (u32)ctxB;
                            if (TagProcCompareIds(&pair4[1], &pair4[0]) == 0)
                                break;
                            if (paneIdx >= msg->field_830)
                                break;
                            TalkNamePane* np = pane->field_0C->v3C(
                                (const wchar_t*)((u8*)TagCtxGetNodeHeader(&ctxB) +
                                                 0xbc),
                                1);
                            if (paneIdx == msg->field_834) {
                                cW4a1 = nw4r::ut::Color(0x006400ff);
                                cW4a2 = nw4r::ut::Color(0x006400ff);
                                np->field_DC = cW4a1;
                                np->field_E0 = cW4a2;
                            } else {
                                cW4b1 = nw4r::ut::Color(0x5a5a5aff);
                                cW4b2 = nw4r::ut::Color(0x5a5a5aff);
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
                cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
                int btn;
                bool upDown;
                bool leftRight;
                if (cf::CfGameManager::isClassicController(-1) != 0) {
                    btn = (pad->mPad.mPressedButtonFlags >> 22) & 1;
                    upDown = (pad->mTurboPressButtonFlags & 0x8004) != 0;
                    leftRight = (pad->mTurboPressButtonFlags & 0x8000) != 0 ||
                                (pad->mTurboPressButtonFlags & 0x8) != 0;
                } else {
                    btn = (pad->mPad.mPressedButtonFlags >> 4) & 1;
                    upDown = (pad->mTurboPressButtonFlags & 0x8004) != 0;
                    leftRight = (pad->mTurboPressButtonFlags & 0x8000) != 0 ||
                                (pad->mTurboPressButtonFlags & 0x8) != 0;
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
                                u8* node = (u8*)TagCtxGetNodeHeader(&ctxD);
                                if (strcmp(((nw4r::lyt::Pane*)node)->GetName(),
                                           ((nw4r::lyt::Pane*)pane)->GetName()) == 0)
                                    break;
                                ctxD = (u32*)*ctxD;
                                pair5p[0] = (u32)getContextStrPtr(ctxBase2);
                                pair5p[1] = (u32)ctxD;
                                if (TagProcCompareIds(&pair5p[1], &pair5p[0]) == 0)
                                    break;
                            }
                            TagCtxAdvanceHead(&ctxD, 0);
                            // walk 6': highlight
                            u32 paneIdx2 = 0;
                            for (;;) {
                                pair6p[0] = (u32)getContextStrPtr(ctxBase2);
                                pair6p[1] = (u32)ctxD;
                                if (TagProcCompareIds(&pair6p[1], &pair6p[0]) == 0)
                                    break;
                                if (paneIdx2 >= msg->field_830)
                                    break;
                                TalkNamePane* np = pane->field_0C->v3C(
                                    (const wchar_t*)((u8*)TagCtxGetNodeHeader(&ctxD) +
                                                     0xbc),
                                    1);
                                if (paneIdx2 == msg->field_834) {
                                    cW6pa1 = nw4r::ut::Color(0x006400ff);
                                    cW6pa2 = nw4r::ut::Color(0x006400ff);
                                    np->field_DC = cW6pa1;
                                    np->field_E0 = cW6pa2;
                                } else {
                                    cW6pb1 = nw4r::ut::Color(0x5a5a5aff);
                                    cW6pb2 = nw4r::ut::Color(0x5a5a5aff);
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
                            u8* node = (u8*)TagCtxGetNodeHeader(&ctxC);
                            if (strcmp(((nw4r::lyt::Pane*)node)->GetName(),
                                       ((nw4r::lyt::Pane*)pane)->GetName()) == 0)
                                break;
                            ctxC = (u32*)*ctxC;
                            pair5[0] = (u32)getContextStrPtr(ctxBase3);
                            pair5[1] = (u32)ctxC;
                            if (TagProcCompareIds(&pair5[1], &pair5[0]) == 0)
                                break;
                        }
                        TagCtxAdvanceHead(&ctxC, 0);
                        // walk 6: accept highlight
                        u32 paneIdx3 = 0;
                        for (;;) {
                            pair6[0] = (u32)getContextStrPtr(ctxBase3);
                            pair6[1] = (u32)ctxC;
                            if (TagProcCompareIds(&pair6[1], &pair6[0]) == 0)
                                break;
                            if (paneIdx3 >= msg->field_830)
                                break;
                            TalkNamePane* np = pane->field_0C->v3C(
                                (const wchar_t*)((u8*)TagCtxGetNodeHeader(&ctxC) +
                                                 0xbc),
                                1);
                            np->v78();
                            np->v74(0x20);
                            cW6a = nw4r::ut::Color(0x483a21ff);
                            cW6b = nw4r::ut::Color(0x483a21ff);
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
                        msg->field_824 = 0.0f;
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
                        u8* node = (u8*)TagCtxGetNodeHeader(&ctxE);
                        if (strcmp(((nw4r::lyt::Pane*)node)->GetName(),
                                   ((nw4r::lyt::Pane*)pane)->GetName()) == 0)
                            break;
                        ctxE = (u32*)*ctxE;
                        pair7[0] = (u32)getContextStrPtr(ctxBase4);
                        pair7[1] = (u32)ctxE;
                        if (TagProcCompareIds(&pair7[1], &pair7[0]) == 0)
                            break;
                    }
                    TagCtxAdvanceHead(&ctxE, 0);
                    // walk 8: highlight
                    u32 paneIdx4 = 0;
                    for (;;) {
                        pair8[0] = (u32)getContextStrPtr(ctxBase4);
                        pair8[1] = (u32)ctxE;
                        if (TagProcCompareIds(&pair8[1], &pair8[0]) == 0)
                            break;
                        if (paneIdx4 >= msg->field_830)
                            break;
                        TalkNamePane* np = pane->field_0C->v3C(
                            (const wchar_t*)((u8*)TagCtxGetNodeHeader(&ctxE) + 0xbc),
                            1);
                        if (paneIdx4 == msg->field_834) {
                            cW8a1 = nw4r::ut::Color(0x006400ff);
                            cW8a2 = nw4r::ut::Color(0x006400ff);
                            np->field_DC = cW8a1;
                            np->field_E0 = cW8a2;
                        } else {
                            cW8b1 = nw4r::ut::Color(0x5a5a5aff);
                            cW8b2 = nw4r::ut::Color(0x5a5a5aff);
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
                        u8* node = (u8*)TagCtxGetNodeHeader(&ctxF);
                        if (strcmp(((nw4r::lyt::Pane*)node)->GetName(),
                                   ((nw4r::lyt::Pane*)pane)->GetName()) == 0)
                            break;
                        ctxF = (u32*)*ctxF;
                        pair9[0] = (u32)getContextStrPtr(ctxBase5);
                        pair9[1] = (u32)ctxF;
                        if (TagProcCompareIds(&pair9[1], &pair9[0]) == 0)
                            break;
                    }
                    TagCtxAdvanceHead(&ctxF, 0);
                    // walk 10: highlight
                    u32 paneIdx5 = 0;
                    for (;;) {
                        pair10[0] = (u32)getContextStrPtr(ctxBase5);
                        pair10[1] = (u32)ctxF;
                        if (TagProcCompareIds(&pair10[1], &pair10[0]) == 0)
                            break;
                        if (paneIdx5 >= msg->field_830)
                            break;
                        TalkNamePane* np = pane->field_0C->v3C(
                            (const wchar_t*)((u8*)TagCtxGetNodeHeader(&ctxF) + 0xbc),
                            1);
                        if (paneIdx5 == msg->field_834) {
                    cW10a1 = nw4r::ut::Color(0x006400ff);
                    cW10a2 = nw4r::ut::Color(0x006400ff);
                            np->field_DC = cW10a1;
                            np->field_E0 = cW10a2;
                        } else {
                    cW10b1 = nw4r::ut::Color(0x5a5a5aff);
                    cW10b2 = nw4r::ut::Color(0x5a5a5aff);
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

// same-TU body like this would otherwise be folded into the call site.
extern "C" void copyVEC2(float *dst, const float *src) {
    dst[0] = src[0];
    dst[1] = src[1];
}
// (each store emits a bl to __as__Q34nw4r2ut5ColorFRCQ34nw4r2ut5Color and
// the RHS color is materialized on the stack) - -ipa would otherwise fold
// the 4-byte copy into the call sites and drop the color temps.

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

extern "C" const wchar_t* getContextStr(u8* self) { return ((const TagContext*)self)->contextStr; }

extern "C" const wchar_t** getContextStrPtr(u8* self) { return (const wchar_t**)(self + 0x4); }

// Fetch the tag chain head and return the first node's list header (4 bytes
// before the payload). Retail asserts when the chain is empty.
void* TagCtxGetNodeHeader(void* self) {
    extern u8 lbl_eu_8052CB40[], lbl_eu_8052CB1C[];
    void* node = *(void**)self;
    if (!node)
        nw4r::db::Panic((const char*)lbl_eu_8052CB40, 0x23D, (const char*)lbl_eu_8052CB1C);
    return (u8*)node - 4;
}

// Compare the first u32 of two blocks (used as a tag-identity test).
int TagProcCompareIds(const void* a, const void* b) {
    return *(const u32*)a != *(const u32*)b;
}

extern "C" void* TagCtxAdvanceHead(void* self, int a) {
    void* p = *(void**)self;
    *(u32*)self = *(u32*)p;
    return p;
}

// Tag-processor message pump: check the next tag code at buf[field_810+2]; if
// the message is exhausted return 3, otherwise keep running the text-processing
// step TagProcStepMessage until it reports <= 4 (page/section done).
int TagProcPumpMessage(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                  nw4r::lyt::Pane* b, nw4r::lyt::Pane* c) {
    CTagProcMsg* msg = (CTagProcMsg*)tag;
    const u16* p = &msg->buf[msg->field_810];
    if (p[2] == 0)
        return 3;
    u32 r;
    do {
        r = (u32)TagProcStepMessage(tag, a, b, c);
    } while (r > 4);
    return (int)r;
}

// retail - the tiny same-TU bodies would otherwise be folded into the call
// sites (same convention as copyVEC2 below).
extern "C" void TagCopyVec2f(float* dst, const float* src) {
    dst[0] = src[0];
    dst[1] = src[1];
}

extern "C" void copyVEC3(float* dst, const float* src) {
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
void TagProcRenderPage(CTagProcMsg* msg, nw4r::lyt::TextBox* a, nw4r::lyt::TextBox* b,
                   nw4r::lyt::TextBox* c, const wchar_t* text) {
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

    a->SetString((const wchar_t*)buf, 0);
    a->SetLineSpace(2.0f);

    f32 meas[2];  // +0x30 measured {x, y} from the layout step
    TagProcMeasureText(meas, (nw4r::lyt::TextBox*)a, buf);
    meas[1] = 16.0f * meas[1];
    f32 pos2[2];  // +0x28 clamped cursor position
    pos2[0] = meas[0];
    pos2[1] = meas[1] + 112.0f;
    f32 dim[2];   // +0x20 {width, height}; later reused as corner xy
    dim[0] = 0.0f;
    dim[1] = 0.0f;
    if (c != NULL) {
        memset(str, 0, sizeof(str));
        wcscpy(str, c->GetStringBuffer());
        f32 size[2];  // +0x10 measured name width/height
        TagMeasureTextWidth(size, msg, (nw4r::lyt::TextBox*)c, str);
        dim[0] = size[0];
        dim[1] = size[1];
        f32 maxX = 44.0f + size[0];
        if (pos2[0] < maxX)
            pos2[0] = maxX;
    }
    pos2[0] = 2.0f * msg->field_808 + pos2[0];
    if (pos2[0] <= 160.0f) {
        pos2[0] = 160.0f;
    } else if (pos2[0] < 432.0f) {
        pos2[0] = 440.0f;
    }
    b->SetSize(*(const nw4r::lyt::Size*)pos2);

    nw4r::math::VEC3 t = b->GetTranslate();
    t.y -= 6.0f;
    a->SetTranslate(t);
    a->SetSize(*(const nw4r::lyt::Size*)meas);
    a->FreeStringBuffer();
    a->AllocStringBuffer(0x400);

    if (c != NULL) {
        f32 w[2];                  // +0x18 corner xy scratch
        nw4r::math::VEC3 s = a->GetTranslate();  // dead in retail too; kept across the
                                                // opaque helper calls below
        nw4r::lyt::Size aSize = a->GetSize();
        TagCopyVec2f(w, (f32*)&aSize);
        nw4r::math::VEC3 bVec = b->GetTranslate();
        nw4r::lyt::Size bSize = b->GetSize();
        copyVEC3((f32*)&s, (f32*)&bVec);
        copyVEC2(w, (f32*)&bSize);
        c->SetTranslate(s);
        nw4r::math::VEC3 cVec = c->GetTranslate();
        copyVEC3((f32*)&s, (f32*)&cVec);
        nw4r::lyt::Size cSize = c->GetSize();
        TagCopyVec2f(w, (f32*)&cSize);
        f32 u[2];                  // +0x08
        u[0] = dim[0];
        u[1] = w[1];
        c->SetSize(*(const nw4r::lyt::Size*)u);
        cSize = c->GetSize();
        copyVEC2(dim, (f32*)&cSize);
        copyVEC3((f32*)&s, (f32*)&bVec);
        copyVEC2(pos2, (f32*)&bSize);
        // blend the b/c corners toward the measured box
        const f32 kBlend = 0.5f;
        f32 nx = dim[0] * kBlend + (s.x - pos2[0] * kBlend);
        f32 ny = (s.y + pos2[1] * kBlend) - dim[1] * kBlend;
        s.x = nx + 46.0f;
        s.y = ny - 44.0f;
        c->SetTranslate(s);
    }
}

// Tag-writer position clamp: copy the message string into a local buffer,
// measure the text position (TagProcMeasureText), then y = c34*(y-cF4)+c30 with a
// low clamp and a snap-down to the fixed step table; write the VEC2 result
// to out->field_4C. The r3 arg is unused (family idiom).
// The y-transform and clamp must be written with the constants INLINE (no
// f32 temps): named temps shift MWCC's FPR birth order and swap c34/c30
// between f2/f3 (6 reg_swap).
void TagProcCalcPageLayout(void* unused, nw4r::lyt::TextBox* msg, nw4r::lyt::Pane* out) {
    wchar_t buf[0x400];
    f32 xy[2];
    wcscpy(buf, msg->GetStringBuffer());
    TagProcMeasureText(xy, msg, buf);
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
    out->SetSize(*(const nw4r::lyt::Size*)xy);
}

// Tag-writer accessor: set CharWriter::mCursorPos (+0x2C/+0x30) with the
// same nw4r pointer validation as TagWriterGetWidthLimit (different Panic site/strings).
// Flags declared in reverse chain order, hi = 0xFF000000 mask reused by the
// 0x80/0xC0 checks only - the retail flag/mask colors.
void TagWriterSetCursorPos(nw4r::ut::TextWriterBase<wchar_t>* tw, float x, float y) {
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
    tw->SetCursorX(x);
    tw->SetCursorY(y);
}

// Tag-writer helper: measure a string's max line width and the font height. The string
// is copied into a scratch buffer (memset + wcscpy), then the ORIGINAL string is
// walked accumulating (font->GetCharWidth(c) + 2) into a running sum; when the
// terminator is hit the running sum becomes the max. The height getter (vtable +0x34)
// and each width+2 are converted with the builtin 0x4330/xoris/lfd/fsubs magic idiom
// (magic hoisted once into f31, shared by both sites).
void TagMeasureTextWidth(f32* out, void* unused,
                                        nw4r::lyt::TextBox* textbox,
                   const wchar_t* str) {
    wchar_t buf[0x400];
    memset(buf, 0, sizeof(buf));
    wcscpy(buf, str);
    const nw4r::ut::Font* font = textbox->GetFont();
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
    // (see TagWriterParseColonParam note / MWCC_CASES 7i).
    f32 height = (f32)font->GetLineFeed();
    for (;;) {
        u16 c = str[i];
        if (c == 0) {
            if (sum > max)
                max = sum;
            break;
        }
        sum += (f32)(font->GetCharWidth(c) + 2);
        i++;
    }
    out[0] = max;
    out[1] = height;
}

// Message-pane text reset hook: free the old textbox string buffer, allocate
// a fresh 1024-char one, advance the tag-proc write position, run the message
// step (TagProcRenderPage) and clear the done flag. No explicit return: the
// retail leaves r3 as whatever TagProcRenderPage left there.
void TagProcResetPage(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                   nw4r::lyt::Pane* b, nw4r::lyt::Pane* c) {
    CTalkTextBoxVtbl* tb = (CTalkTextBoxVtbl*)a;
    tb->v78();          // FreeStringBuffer()
    tb->v74(0x400);     // AllocStringBuffer(0x400)
    CTagProcMsg* msg = (CTagProcMsg*)tag;
    msg->field_810++;
    TagProcRenderPage(msg, (nw4r::lyt::TextBox*)a, (nw4r::lyt::TextBox*)b, (nw4r::lyt::TextBox*)c,
                  0);
    msg->field_820 = 0;
}

// Sound-effect variant of the tag processor (retail __ct__CTagProcessorSE):
// base-construct, then the SE-specific fields (byte at +0x834, u32 at +0x838).
void* __ct__CTagProcessorSE(void* self) {
    CTagProcessorSE* p = (CTagProcessorSE*)self;
    __ct__17CTagProcessorBaseFv(p);
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

// Deleting destructor for CTagProcessorSE (retail __dt__15CTagProcessorSEFv): member
// dtor of the derived class -- base dtor call with flag 0, then operator delete when
// the deleting flag is set.
CTagProcessorSE::~CTagProcessorSE() {}

// SE message-pump step (CTagProcessorSE variant of TagProcStepMessage, no name
// list): dispatch the next tag code at buf[field_810]. Handlers push tag
// data to the text pane via the 2-arg +0x7C SetText and advance the read
// counters. Tag 5 is the auto-advance speed gate; tags 8/9 drive icons and
// party-member emphasis. Frame layout: locals assigned from the top down
// (first declared = highest address: 0x2F8 conv temp, 0xF8 outbuf,
// 0x1C flag table, 0x10 vals, 0x8 hdr).
int TagProcStepSEMessage(nw4r::lyt::AnimTransform* tag,
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
            if (msg->field_824 >= 3.0f) {
                if (msg->field_824 < 10.0f) {
                    if (MenuStateCheck64064or30() == 0 &&
                        MenuSnd_QuerySlotLive_89A04(0) != 0 && msg->field_804 != 0 &&
                        member != 0)
                        member->v58(0, 0);
                    msg->field_824 = 20.0f;
                }
                // Button-driven auto-advance: poll the active pad style.
                CPad* pad = cf::CfGameManager::getCurrentPad();
                member = (TagMemberObj*)pad;  // retail reuses r23 here
                u32 bits;
                if (cf::CfGameManager::isClassicController(-1) != 0)
                    bits = pad->mPressedButtonFlags & 0x00600000;
                else
                    bits = pad->mPressedButtonFlags & 0x00000030;
                if (bits != 0) {
                    msg->field_824 = 0.0f;
                    msg->field_810 += 2;
                    msg->field_820 += 2;
                    MenuSnd_SilenceByName_8986C(0, 0.0f);
                }
            } else {
                msg->field_824 += 1.0f;
            }
            return 2;
        }
        if (v == -2) {
            // Unconditional skip once the voice slot is free.
            if (MenuSnd_QuerySlotLive_89A04(0) != 0) {
                msg->field_824 = 0.0f;
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
            if (msg->field_824 < 1.0f && msg->field_804 != 0 &&
                member != 0)
                member->v58(0, 0);
            msg->field_824 += 1.0f;
        } else {
            if (msg->field_804 != 0 && member != 0)
                member->v58(1, 0);
            msg->field_824 = 0.0f;
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

void TagProcPumpSEMessage(){}

// Talk-open hook: reset the message text pane's string buffer (free the old
// buffer, allocate a fresh 1024-char one), advance the tag-proc write position,
// run the message start step, and clear the done flag. No explicit return: the
// retail leaves r3 as whatever TagProcBuildOpenText left there.
extern "C" int TagProcStartMessage(void* tagProc, nw4r::lyt::Pane* pane) {
    CTalkTextBoxVtbl* tb = (CTalkTextBoxVtbl*)pane;
    tb->v78();          // FreeStringBuffer()
    tb->v74(0x400);     // AllocStringBuffer(0x400)
    CTagProcMsg* msg = (CTagProcMsg*)tagProc;
    msg->field_810++;
    TagProcBuildOpenText((CTagProcessorBase*)tagProc, pane, 0);
    msg->field_820 = 0;
}

// Talk-open text setup: build the display string (either the caller's string
// or a scan of the message buffer from field_810 that stops at a control
// code (0/3/4) or 0x3FF chars, flagging a "1 after newline"), push it into
// the pane's +0x7C setter, pick the message-speed constant from the newline
// flag, measure the line position, and update the pane's position VEC3.
// -O4,s save shape (_savegpr_28).
extern "C" void TagProcBuildOpenText(CTagProcessorBase* msg, nw4r::lyt::Pane* pane,
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
    nw4r::lyt::TextBox* pv = (nw4r::lyt::TextBox*)pane;
    pv->SetString((const wchar_t*)buf2, 0);
    if (newline)
        pv->SetLineSpace(15.0f);
    else
        pv->SetLineSpace(2.0f);
    TagProcMeasureText(out, (nw4r::lyt::TextBox*)pane, buf2);
    if (newline)
        out[1] = 16.0f * out[1] + 15.0f;
    else
        out[1] = 16.0f * out[1];
    AccumPaneWorldPos((nw4r::math::VEC3*)out2, pane);
    nw4r::lyt::Size pvSize = pv->GetSize();
    TagCopyVec2f(v3, (f32*)&pvSize);
    // Retail computes BOTH differences first, then both products; out2
    // entries forward the stored v3 values.
    f32 dy = v3[1] - out[1];
    f32 dx = v3[0] - out[0];
    v3[1] = dy * 0.5f;
    v3[0] = dx * 0.5f;
    out2[0] = v3[0];
    out2[1] = -193.0f - v3[1];
    pv->SetTranslate(*(const nw4r::math::VEC3*)out2);
    pv->FreeStringBuffer();
    pv->AllocStringBuffer(0x400);
}

u16* CTagCodeMakeCrystal::v14(u16* buf, const wchar_t* arg, TagParam* prm) {
    u8* dst = (u8*)prm;
    TagParam p;
    TagParamInitCode(&p, 7);
    copyTagParam(dst, (const u8*)&p);
    return buf;
}

// Tag-writer variant with a numeric value: uppercase the value string, parse it
// with wcstol, store tag-param code 6 plus the value, copy the block into the
// output buffer, and thread the r4 arg through (same family as TagWriterCode7).
u16* CTagCodeShop::v14(u16* buf, const wchar_t* str, TagParam* prm) {
    WcsToUpperInPlace((u16*)str);
    wchar_t* endptr = 0;
    TagParam p;
    long val = wcstol((const wchar_t*)str, &endptr, 10);
    TagParamInitCodeU16(&p, 6, (u16)val);
    copyTagParam((u8*)prm, (const u8*)&p);
    return buf;
}

// Same stmw/lmw save-merge + copy-order fix as TagWriterCode7.
u16* CTagCodeExchange::v14(u16* buf, const wchar_t* arg, TagParam* prm) {
    u8* dst = (u8*)prm;
    TagParam p;
    TagParamInitCode(&p, 5);
    copyTagParam(dst, (const u8*)&p);
    return buf;
}

// Build a fresh tag-param (mode 8) on the stack, copy it onto the target,
// set its +1 flag, and return the ret pointer (retail keeps a1/a3 phantom).
u16* CTagCodeTailOff::v14(u16* buf, const wchar_t* arg, TagParam* prm) {
    TagParam* ret = (TagParam*)buf;
    TagParam* target = (TagParam*)prm;
    TagParam tmp;
    TagParamInitCode(&tmp, 8);
    copyTagParam((u8*)target, (const u8*)&tmp);
    target->field_01 |= 1;
    return (u16*)ret;
}

// Same as TagWriterCode6Value but with tag-param code 4.
u16* CTagCodePresent::v14(u16* buf, const wchar_t* str, TagParam* prm) {
    WcsToUpperInPlace((u16*)str);
    wchar_t* endptr = 0;
    TagParam p;
    long val = wcstol((const wchar_t*)str, &endptr, 10);
    TagParamInitCodeU16(&p, 4, (u16)val);
    copyTagParam((u8*)prm, (const u8*)&p);
    return buf;
}

u16* CTagCodeQstStart::v14(u16* buf, const wchar_t* arg, TagParam* prm) {
    void* arg1 = (void*)this;
    void* arg2 = (void*)buf;
    extern void MenuStateSet6405A(void*);
    MenuStateSet6405A(arg1);
    return (u16*)arg2;
}

// Same stmw/lmw save-merge + copy-order fix as TagWriterCode7.
u16* CTagCodeInvisible::v14(u16* buf, const wchar_t* arg, TagParam* prm) {
    u8* dst = (u8*)prm;
    TagParam p;
    TagParamInitCode(&p, 3);
    copyTagParam(dst, (const u8*)&p);
    return buf;
}

// Same as TagWriterCode6Value but with tag-param code 2.
u16* CTagCodeQuest::v14(u16* buf, const wchar_t* str, TagParam* prm) {
    WcsToUpperInPlace((u16*)str);
    wchar_t* endptr = 0;
    TagParam p;
    long val = wcstol((const wchar_t*)str, &endptr, 10);
    TagParamInitCodeU16(&p, 2, (u16)val);
    copyTagParam((u8*)prm, (const u8*)&p);
    return buf;
}

// Tag-param writer for a 1-value tag: split the arg string on ':' via
// SplitWStringTrim, uppercase the first token and map it to a tag index via the
// tag-string table (offsets +0x00/+0x08/+0x14 select 0..2), then init a tag-param
// block: when a second token exists its decimal value is parsed with wcstol and
// converted to float (builtin s32->f32, byte-identical to retail's
// 0x4330/xoris/lfd/fsubs magic idiom - the magic pools to a TU-local @N label; retail
// shares it as lbl_eu_80667200, see the inline note), else the fixed f32 fallback
// constant is used. The block is copied into dst and the r4 arg is returned (family
// idiom). tokens[16]: retail's frame places the token array at +0x28 and the
// conversion scratch at +0x68, which a 16-pointer array reproduces.
u16* CTagCodeFade::v14(u16* ret, const wchar_t* str, TagParam* dst) {
    wchar_t* tokens[16];
    TagParam p;
    int count = SplitWStringTrim((u16*)str, 0x3a, (u16**)tokens);
    u8 idx = 0;
    WcsToUpperInPlace((u16*)tokens[0]);
    if (wcscmp(tokens[0], L"OUT") == 0)
        idx = 0;
    else if (wcscmp(tokens[0], L"OUTIN") == 0)
        idx = 1;
    else if (wcscmp(tokens[0], L"IN") == 0)
        idx = 2;
    if (count > 1) {
        WcsToUpperInPlace((u16*)tokens[1]);
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
        initTagParam((u8*)&q, 1, idx, -1.0f);
        copyTagParam((u8*)dst, (const u8*)&q);
    }
    return (u16*)ret;
}

// Color-tag writer: split the arg string on ':', copy the name/value tables from ROM
// (220/224-byte struct assignments - MWCC's mtctr/bdnz copy loop), match each token
// against the name table (max 0x40 entries, null-terminated) storing the matching
// value into the 3-slot scratch (seeded with the sdata2 color defaults), then pack the
// scratch into a 3-u16 block <8, (b<<8)|r, (a<<8)|0xff> and return the position after
// the block.
u16* CTagCodeAct::v14(u16* buf, const wchar_t* arg, TagParam* prm) {
    u16* dst = buf;
    wchar_t* str = (wchar_t*)arg;
    TagColorOut out;
    wchar_t* tokens[16];
    TagColorValues values;
    TagColorNames names;
    int count = SplitWStringTrim((u16*)str, 0x3a, (u16**)tokens);
    values = lbl_eu_804FEFB8;
    names = lbl_eu_804FF488;
    out.w.c1 = lbl_eu_80667188;
    out.w.c2 = lbl_eu_8066718C;
    for (int i = 0; i < count; i++) {
        WcsToUpperInPlace((u16*)tokens[i]);
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

// Tag-code writer (code 7): split the arg string on '/', record each token's length,
// write the 2-entry header (tag + token count), then a (count+1)/2 block of length
// pairs (each u16 packs two consecutive token lengths), then copy every token into dst
// in order. Returns the end of the copied region.
u16* CTagCodeSelect2::v14(u16* dst, const wchar_t* str, TagParam* prm) {
    wchar_t* tokens[16];
    int count = SplitWStringTrim((u16*)str, 0x2f, (u16**)tokens);
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

// Tag-code writer (code 6): same shape as TagWriterCode7List but with tag code 6.
u16* CTagCodeSelect::v14(u16* dst, const wchar_t* str, TagParam* prm) {
    wchar_t* tokens[16];
    int count = SplitWStringTrim((u16*)str, 0x2f, (u16**)tokens);
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
inline float TagWriterGetWidthLimit(nw4r::ut::TextWriterBase<wchar_t>* tw) {
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
    return tw->GetWidthLimit();
}

// Tag-writer accessor: TextWriterBase::SetWidthLimit (+0x4C) with the same
// nw4r pointer validation as TagWriterGetWidthLimit (different Panic site/strings).
// Flags declared in reverse chain order, hi = 0xFF000000 mask reused by the
// 0x80/0xC0 checks only - the retail flag/mask colors.
inline void TagWriterSetWidthLimit(nw4r::ut::TextWriterBase<wchar_t>* tw, float v) {
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
    tw->SetWidthLimit(v);
}

// Tag-writer accessor: add to CharWriter::mCursorPos.x (+0x2C) with the same
// nw4r pointer validation as TagWriterGetWidthLimit (different Panic site/strings).
inline void TagWriterAdvanceCursorX(nw4r::ut::TextWriterBase<wchar_t>* tw, float v) {
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
    tw->MoveCursorX(v);
}

// Tag-writer accessor: CharWriter::GetCursorY (+0x30) with the same pointer
// validation as TagWriterGetWidthLimit (different Panic site/strings). Flags declared
// in reverse chain order, hi = 0xFF000000 mask reused by the 0x80/0xC0 checks
// only - the retail flag/mask colors.
inline float TagWriterGetCursorY(nw4r::ut::TextWriterBase<wchar_t>* tw) {
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
    return tw->GetCursorY();
}

// Converted to TagContext::addToCharSpace

u16* CTagCodeLine::v14(u16* buf, const wchar_t* arg, TagParam* prm) {
    u16* out = buf;
    *out = 0xD;
    return out + 1;
}

// Tag-writer: measure a message line. Reads the TextWriter from the holder,
// sizes the writer's font to the fixed sdata2 constant, measures the two-char
// speed-up string "++" (tag table +0x1a), restores the previous font size,
// and stores the result back into the char-space context. Returns 2.
nw4r::ut::TagProcessorBase<wchar_t>::Operation CTagCodeLine::CalcRect(nw4r::ut::Rect* rect, u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx) {
    nw4r::ut::TextWriterBase<wchar_t>* tw = ctx->writer;
    f32 sp = TagWriterGetWidthLimit(tw);
    TagWriterSetWidthLimit(tw, lbl_eu_80667260);
    f32 w = tw->CalcStringWidth(L"──", 2);
    TagWriterSetWidthLimit(tw, sp);
    TagWriterAdvanceCursorX(tw, w);
    f32 h = TagWriterGetCursorY(ctx->writer);
    rect->top = h;
    rect->right = rect->left + w;
    rect->SetHeight(tw->GetFontHeight());
    return nw4r::ut::TagProcessorBase<wchar_t>::OPERATION_CHAR_SPACE;
}

// Tag-writer: copy the active writer into a scratch, configure it (font-size
// field, width limit, scale, cursor), print the one-char marker at the tag
// table +0x20, then advance the real writer's cursor by the scratch's new
// cursor x plus the doubled font-slot value. Two nw4r pointer-validation
// chains (on the scratch and on the real writer) bracket the measurement.
// The scratch's dtor is emitted automatically at scope exit. Returns 2.
nw4r::ut::TagProcessorBase<wchar_t>::Operation CTagCodeLine::Process(u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx) {
    nw4r::ut::TextWriterBase<wchar_t>* tw = ctx->writer;
    f32 x = TagWriterGetCursorX(tw);
    f32 y = TagWriterGetCursorY(tw);
    f32 sx = TagWriterGetScaleX(tw);
    f32 sy = TagWriterGetScaleY(tw);
    nw4r::ut::TextWriterBase<wchar_t> local;
    TagWriterCopyBlock((TagCopyBlock*)&local, (const TagCopyBlock*)tw);
    TagWriterSetFontSize(&local, 0x300);
    TagWriterSetWidthLimit(&local, 3.4028235e+38f);
    TagWriterSetScale(&local, 2.0f * sx, sy);
    TagWriterSetCursorPos(&local, x, y);
    local.Print(L"─", 1);
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
    int fw = local.GetFont()->GetCharWidth(0x2500);
    f32 conv = (f32)(fw << 1);
    TagWriterSetCursorPos(&local, x + conv, y);
    f32 cx = TagWriterGetCursorX(&local);
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
    tw->SetCursorX(cx);
    return nw4r::ut::TagProcessorBase<wchar_t>::OPERATION_CHAR_SPACE;
}

// Tag-writer accessor: CharWriter::GetCursorX (+0x2C) with the same pointer
// validation as TagWriterGetWidthLimit (different Panic site/strings). Flags declared
// in reverse chain order, hi = 0xFF000000 mask reused by the 0x80/0xC0 checks
// only - the retail flag/mask colors.
inline float TagWriterGetCursorX(nw4r::ut::TextWriterBase<wchar_t>* tw) {
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
    return tw->GetCursorX();
}

// Tag-writer accessor: CharWriter scale-x (+0x24) with the same nw4r
// pointer validation as TagWriterGetWidthLimit (different Panic site/strings). Flags
// declared in reverse chain order, hi = 0xFF000000 mask reused by the
// 0x80/0xC0 checks only - the retail flag/mask colors.
inline float TagWriterGetScaleX(nw4r::ut::TextWriterBase<wchar_t>* tw) {
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
    return tw->GetScaleH();
}

// Tag-writer accessor: CharWriter scale-y (+0x28) with the same nw4r
// pointer validation as TagWriterGetWidthLimit (different Panic site/strings).
inline float TagWriterGetScaleY(nw4r::ut::TextWriterBase<wchar_t>* tw) {
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
    return tw->GetScaleV();
}

extern "C" void TagWriterCopyBlock(TagCopyBlock* dst, const TagCopyBlock* src) {
    *dst = *src;
}

// Tag-writer accessor: store into TextWriterBase +0x5C (mFontSize.y region) with the
// same nw4r pointer validation as TagWriterGetWidthLimit. This one keeps both tw and
// value live across the Panic call, so MWCC saves r30/r31 (stmw prologue) instead of
// just r31.
inline void TagWriterSetFontSize(nw4r::ut::TextWriterBase<wchar_t>* tw, u32 value) {
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
    tw->SetDrawFlag(value);
}

// Tag-writer accessor: CharWriter scale set (+0x24/+0x28, mScale region)
// with the same nw4r pointer validation as TagWriterGetWidthLimit (different Panic
// site/strings).
inline void TagWriterSetScale(nw4r::ut::TextWriterBase<wchar_t>* tw, float x, float y) {
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
    tw->SetScale(x, y);
}

// Tag-code writer (code 9): uppercase the arg string, compare it against the
// tag-string table entries at +0x24 / +0x2a, write <9, value> into dst, and return the
// position after the block. The r3 arg is unused (family idiom).
u16* CTagCodeMouth::v14(u16* buf, const wchar_t* arg, TagParam* prm) {
    WcsToUpperInPlace((u16*)arg);
    s16 v = 0xff;
    if (wcscmp(arg, L"ON") == 0)
        v = 0;
    else if (wcscmp(arg, L"OFF") == 0)
        v = -1;
    buf[0] = 9;
    buf[1] = (u16)v;
    return buf + 2;
}

// Tag-code writer (code 8, name selection): uppercase the arg string, map it to a
// value 1..9 via the tag-string table, init a tag-param block with code 8, copy it
// into dst, then tag the block with the selected value and the "has value" bit.
// Returns the r4 arg.
u16* CTagCodePos::v14(u16* ret, const wchar_t* str, TagParam* dst) {
    u8 v = 0;
    WcsToUpperInPlace((u16*)str);
    if (wcscmp(str, L"TLEFT") == 0)
        v = 1;
    else if (wcscmp(str, L"TCENTER") == 0)
        v = 2;
    else if (wcscmp(str, L"TRIGHT") == 0)
        v = 3;
    else if (wcscmp(str, L"CLEFT") == 0)
        v = 4;
    else if (wcscmp(str, L"CCENTER") == 0)
        v = 5;
    else if (wcscmp(str, L"CRIGHT") == 0)
        v = 6;
    else if (wcscmp(str, L"BLEFT") == 0)
        v = 7;
    else if (wcscmp(str, L"BCENTER") == 0)
        v = 8;
    else if (wcscmp(str, L"BRIGHT") == 0)
        v = 9;
    TagParam p;
    TagParamInitCode(&p, 8);
    copyTagParam((u8*)dst, (const u8*)&p);
    dst->field_03 = v;
    dst->field_01 |= 0x2;
    return (u16*)ret;
}

// Tag-code writer (code 8): uppercase the arg string, pick a value from the
// tag-string table by comparison (offsets +0xb0/+0xb8/+0xc4/+0xcc select
// 0..3), init a tag-param block with code 8, copy it into dst, then tag the
// block with the selected value and the "has value" bit. Returns the r4 arg.
u16* CTagCodeType::v14(u16* ret, const wchar_t* str, TagParam* dst) {
    u8 v = 0;
    WcsToUpperInPlace((u16*)str);
    if (wcscmp(str, L"DEF") == 0)
        v = 0;
    else if (wcscmp(str, L"ANGER") == 0)
        v = 1;
    else if (wcscmp(str, L"SAD") == 0)
        v = 2;
    else if (wcscmp(str, L"THINK") == 0)
        v = 3;
    TagParam p;
    TagParamInitCode(&p, 8);
    copyTagParam((u8*)dst, (const u8*)&p);
    dst->field_02 = v;
    dst->field_01 |= 0x4;
    return (u16*)ret;
}

// Tag-code writer (code 0xB): split the arg string on ':', uppercase token 0 and map
// it to a 1..2 tag id via the tag-string table (+0xd8/+0xdc), parse token 1 as the
// numeric value, then map token 2 through the 26-entry BDAT lookup chain (each match
// yields BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], n)) or a wcstol
// fallback. Tokens 3/4 (when present) are decimal values. Writes <0xb, (tag1<<8)|0xff,
// val, tag2, tag3, tag4> when the split produced >= 3 tokens and returns the position
// after the block.
u16* CTagCodeTrust::v14(u16* out, const wchar_t* str, TagParam* prm) {
    wchar_t* tokens[16];
    int count = SplitWStringTrim((u16*)str, 0x3a, (u16**)tokens);
    u8 tag1 = 0xff;
    int tag3 = 0xff;
    int tag4 = 0xff;
    WcsToUpperInPlace((u16*)tokens[0]);
    if (wcscmp(tokens[0], L"P") == 0)
        tag1 = 1;
    else if (wcscmp(tokens[0], L"F") == 0)
        tag1 = 2;
    WcsToUpperInPlace((u16*)tokens[1]);
    wchar_t* end1 = 0;
    int val = (int)wcstol(tokens[1], &end1, 10);
    WcsToUpperInPlace((u16*)tokens[2]);
    int tag2;
    if (wcscmp(tokens[2], L"A") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 1);
    else if (wcscmp(tokens[2], L"B") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 2);
    else if (wcscmp(tokens[2], L"C") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 3);
    else if (wcscmp(tokens[2], L"D") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 4);
    else if (wcscmp(tokens[2], L"E") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 5);
    else if (wcscmp(tokens[2], L"F") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 6);
    else if (wcscmp(tokens[2], L"G") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 7);
    else if (wcscmp(tokens[2], L"H") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 8);
    else if (wcscmp(tokens[2], L"I") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 9);
    else if (wcscmp(tokens[2], L"J") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xa);
    else if (wcscmp(tokens[2], L"K") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xb);
    else if (wcscmp(tokens[2], L"L") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xc);
    else if (wcscmp(tokens[2], L"M") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xd);
    else if (wcscmp(tokens[2], L"N") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xe);
    else if (wcscmp(tokens[2], L"O") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0xf);
    else if (wcscmp(tokens[2], L"P") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x10);
    else if (wcscmp(tokens[2], L"Q") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x11);
    else if (wcscmp(tokens[2], L"R") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x12);
    else if (wcscmp(tokens[2], L"S") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x13);
    else if (wcscmp(tokens[2], L"T") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x14);
    else if (wcscmp(tokens[2], L"U") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x15);
    else if (wcscmp(tokens[2], L"V") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x16);
    else if (wcscmp(tokens[2], L"W") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x17);
    else if (wcscmp(tokens[2], L"X") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x18);
    else if (wcscmp(tokens[2], L"Y") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x19);
    else if (wcscmp(tokens[2], L"Z") == 0)
        tag2 = BdatGetS16ByTableKey(lbl_eu_804FFC48, &lbl_eu_804FFC48[0xc], 0x1a);
    else {
        wchar_t* end2 = 0;
        tag2 = wcstol(tokens[2], &end2, 10);
    }
    if (count > 3) {
        WcsToUpperInPlace((u16*)tokens[3]);
        wchar_t* end3 = 0;
        tag3 = (int)wcstol(tokens[3], &end3, 10);
    }
    if (count > 4) {
        WcsToUpperInPlace((u16*)tokens[4]);
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

// Color-name tag writer: uppercase the arg string, map known color names
// from the tag table to packed RGB values, else parse a '#'-prefixed digit
// string (digits '1'..'?' accumulate value nibbles, 'A'..'F' are skipped,
// alpha is forced to 0xFF), then write the <2, hi, lo> block.
// -O4,s save shape (stmw r28).
inline u16* CTagCodeCol::v14(u16* out, const wchar_t* str, TagParam* prm) {
    WcsToUpperInPlace((u16*)str);
    u32 v = 0;
    if (wcscmp(str, L"DEF") == 0)
        v = 0x483A21FF;
    else if (wcscmp(str, L"D") == 0)
        v = 0x483A21FF;
    else if (wcscmp(str, L"DEF2") == 0)
        v = -0x501;
    else if (wcscmp(str, L"D2") == 0)
        v = -0x501;
    else if (wcscmp(str, L"DEF3") == 0)
        v = 0x483A21FF;
    else if (wcscmp(str, L"D3") == 0)
        v = 0x483A21FF;
    else if (wcscmp(str, L"RED") == 0)
        v = 0xFF0000FF;
    else if (wcscmp(str, L"ITEM") == 0)
        v = 0xC81E1EFF;
    else if (wcscmp(str, L"I") == 0)
        v = 0xC81E1EFF;
    else if (wcscmp(str, L"S1") == 0)
        v = 0xC81E1EFF;
    else if (wcscmp(str, L"S2") == 0)
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
            case 'A': v += 10; break;
            case 'B': v += 11; break;
            case 'C': v += 12; break;
            case 'D': v += 13; break;
            case 'E': v += 14; break;
            case 'F': v += 15; break;
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

inline nw4r::ut::TagProcessorBase<wchar_t>::Operation CTagCodeCol::CalcRect(nw4r::ut::Rect* rect, u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx) {
    ctx->str += 2;  // skip the packed RGBA argument
    return nw4r::ut::TagProcessorBase<wchar_t>::OPERATION_DEFAULT;
}

// Color-tag writer: apply the source color to the CharWriter, preserving the
// writer's current alpha. Two nw4r-style pointer-validation chains (each
// with its own Panic site) sandwich the alpha merge; the vertex color is
// committed via the out-of-line Color::operator= and
// CharWriter::UpdateVertexColor, then the source pointer advances.
// -O4,s save shape (stmw r30).
inline nw4r::ut::TagProcessorBase<wchar_t>::Operation CTagCodeCol::Process(u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx) {
    // Retail keeps the context in its own callee-saved register (r31) ahead
    // of the writer; binding it through the writer/color arg view reproduces
    // that colouring (direct ctx-> accesses swap r30/r31, 85%).
    nw4r::ut::CharWriter* w = ((TagColorArg*)ctx)->field_00;
    TagColorArg* arg = (TagColorArg*)ctx;
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;
    u32 hi = (u32)w & 0xFF000000;
    nw4r::ut::Color c = *arg->field_04;
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
    c.a = w->GetTextColor().a;
    validRegs2 = true;
    validRegs = true;
    validIo2 = true;
    validIo = true;
    validMem2 = true;
    validMem1 = true;
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
    w->SetTextColor(c);
    arg->field_04++;
    return nw4r::ut::TagProcessorBase<wchar_t>::OPERATION_DEFAULT;
}

// Tag-code writer (code 1): split the arg string on '/' via SplitWStringTrim,
// record each token's length, write a 2-entry header (count + combined first
// two lengths), then copy every token into dst in order, advancing by each
// token's stored length. Returns the end of the copied region.
inline u16* CTagCodeRuby::v14(u16* dst, const wchar_t* str, TagParam* prm) {
    wchar_t* tokens[16];
    int count = SplitWStringTrim((u16*)str, 0x2f, (u16**)tokens);
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

// Tag-writer line layout: copy the active TextWriter into a local scratch writer (no
// ctor - -ipa drops the dead __ct__ since TagWriterCopyBlock overwrites every byte),
// zero its font-size field, scale it by the sdata2 factor, measure two adjacent
// strings from the tag buffer (lengths are the low/high bytes of the u16 header; the
// second string starts 2*len2 bytes in), add the max width (plus the writer's char
// space unless bit0 of field_10 is set) to the real writer's cursor, write
// out->field_08, advance the buffer pointer past both strings, destroy the scratch,
// and return 2.
inline nw4r::ut::TagProcessorBase<wchar_t>::Operation CTagCodeRuby::CalcRect(nw4r::ut::Rect* rect, u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx) {
    const u8* buf = (const u8*)ctx->str;
    nw4r::ut::TextWriterBase<wchar_t>* tw = ctx->writer;
    u16 h = *(const u16*)buf;
    u8 lenLo = h;
    u32 lenHi = (h >> 8) & 0xFF;
    const wchar_t* s2 = (const wchar_t*)(buf + 2);
    const wchar_t* s1 = s2 + ((h >> 8) & 0xFF);
    nw4r::ut::TextWriterBase<wchar_t> local;
    TagWriterCopyBlock((TagCopyBlock*)&local, (const TagCopyBlock*)tw);
    TagWriterSetFontSize(&local, 0);
    TagWriterSetCharSpace(&local, 0.0f);
    f32 yscale = lbl_eu_80667264 * TagWriterGetScaleY(&local);
    f32 xscale = TagWriterGetScaleX(&local);
    TagWriterSetScale(&local, lbl_eu_80667264 * xscale, yscale);
    TagWriterSetWidthLimit(&local, lbl_eu_80667260);
    f32 wlimit = TagWriterGetWidthLimit(tw);
    TagWriterSetWidthLimit(tw, lbl_eu_80667260);
    // "no leading char space" flag, materialized before the width calls
    u32 noSpace = (ctx->flags & 1) ^ 1;
    f32 w1 = tw->CalcStringWidth(s1, lenLo);
    f32 w2 = local.CalcStringWidth(s2, lenHi);
    if (w1 < w2)
        w1 = w2;
    f32 cs;
    if (noSpace != 0)
        cs = TagWriterGetCharSpace(tw);
    else
        cs = 0.0f;
    w1 += cs;
    TagWriterSetWidthLimit(tw, wlimit);
    TagWriterAdvanceCursorX(tw, w1);
    rect->right = rect->left + w1;
    rect->SetHeight(tw->GetFontHeight());
    ctx->str = (const wchar_t*)(s1 + lenLo);
    return nw4r::ut::TagProcessorBase<wchar_t>::OPERATION_CHAR_SPACE;
}

// Tag-writer accessor: TextWriterBase::SetCharSpace (+0x50) with the same
// nw4r pointer validation as TagWriterGetWidthLimit (different Panic site/strings).
inline void TagWriterSetCharSpace(nw4r::ut::TextWriterBase<wchar_t>* tw, float v) {
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
    tw->SetCharSpace(v);
}

// Tag-writer accessor: TextWriterBase::GetCharSpace (+0x50) with the same
// nw4r pointer validation as TagWriterGetWidthLimit (different Panic site/strings).
inline float TagWriterGetCharSpace(nw4r::ut::TextWriterBase<wchar_t>* tw) {
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
    return tw->GetCharSpace();
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
inline nw4r::ut::TagProcessorBase<wchar_t>::Operation CTagCodeRuby::Process(u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx) {
    // Long-lived locals declared up front in MWCC's callee-saved priority
    // order (first declared -> highest register); assigned in statement order
    // below, matching the retail allocation map (r31..r25).
    u8 lenLo;
    u32 savedFontSize;
    nw4r::ut::TextWriterBase<wchar_t>* msg;
    u8 lenHi;
    const u16* s1;
    const u16* s2;
    msg = ctx->writer;
    f32 savedWidth = TagWriterGetWidthLimit(msg);
    TagWriterSetWidthLimit(msg, 3.4028235e+38f);
    // "no leading char space" flag: re-apply the current char space when set.
    if ((ctx->flags & 1) == 0)
        TagWriterAdvanceCursorX(msg, TagWriterGetCharSpace(msg));

    // Validation-flag scratch: initialized before the header decode so the
    // allocator pins them to the low volatile window (retail r4..r9).
    bool validRegs2 = true;
    bool validRegs = true;
    bool validIo2 = true;
    bool validIo = true;
    bool validMem2 = true;
    bool validMem1 = true;

    const u8* buf = (const u8*)ctx->str;
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

    savedFontSize = msg->GetDrawFlag();
    f32 cursorX = TagWriterGetCursorX(msg);
    f32 cursorY = TagWriterGetCursorY(msg);
    if (lenHi == 0) {
        nw4r::db::Panic(&lbl_eu_804FFC48[0x27], 0x120,
                        &lbl_eu_804FFC48[0x15]);
    }

    nw4r::ut::TextWriterBase<wchar_t> local;
    TagWriterCopyBlock((TagCopyBlock*)&local, (TagCopyBlock*)msg);
    TagWriterSetFontSize(&local, 0x300);
    TagWriterSetCharSpace(&local, 0.0f);

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

    local.SetDrawFlag(0);
    f32 yscale = 0.6f * TagWriterGetScaleY(&local);
    f32 xscale = TagWriterGetScaleX(&local);
    TagWriterSetScale(&local, 0.6f * xscale, yscale);

    f32 wMsg = msg->CalcStringWidth((const wchar_t*)s1, lenHi);
    f32 wLocal = local.CalcStringWidth((const wchar_t*)s1, lenHi);
    f32 excess = wMsg - wLocal;
    if (excess > 0.0f) {
        // Distribute the excess width across lenHi chars: fold one char
        // space worth into the saved cursor x and give the scratch writer
        // the per-char share. int->float via the retail magic-constant idiom.
        // The union carries the int->float conversion bits directly, so
        // the subtract stays a float operation rather than a library call.
        TagConvTemp conv;
        conv.w.lo = (u32)lenHi ^ 0x80000000;
        conv.w.hi = 0x43300000;
        f32 lenF = (f32)(conv.d - lbl_eu_80667200);
        f32 share = excess / lenF;
        cursorX += share * 0.5f;
        TagWriterSetCharSpace(&local, share);
    }

    f32 descent = local.GetFontDescent();
    f32 height = msg->GetFontAscent() + descent;
    TagWriterSetCursorPos(&local, cursorX, cursorY - height);
    local.Print((const wchar_t*)s1, lenHi);
    if (excess < 0.0f) {
        TagWriterAdvanceCursorX(msg, -excess * 0.5f);
    }
    TagWriterSetFontSize(msg, 0x300);
    msg->Print((const wchar_t*)s2, lenLo);
    TagWriterSetFontSize(msg, savedFontSize);
    if (excess < 0.0f) {
        TagWriterAdvanceCursorX(msg, -excess * 0.5f);
    }
    ctx->str = (const wchar_t*)(s2 + lenLo);
    TagWriterSetWidthLimit(msg, savedWidth);
    return nw4r::ut::TagProcessorBase<wchar_t>::OPERATION_CHAR_SPACE;
}

// Tag-code writer (code 4): same shape as TagWriterCode9Switch, but the first match
// selects -1 and the second selects 0 (strings at +0x17e / +0x186).
inline u16* CTagCodeDel::v14(u16* dst, const wchar_t* str, TagParam* prm) {
    WcsToUpperInPlace((u16*)str);
    s16 v = 0xff;
    if (wcscmp(str, L"ALL") == 0)
        v = -1;
    else if (wcscmp(str, L"THIS") == 0)
        v = 0;
    dst[0] = 4;
    dst[1] = (u16)v;
    return dst + 2;
}

// Tag-code writer (code 5): uppercase the arg string, then map it to a value
// via the tag-string table (+0x190 -> -1, +0x198 -> -2, otherwise the
// decimal parse of the string). Writes <5, value> into dst and returns the
// position after the block. The r3 arg is unused (family idiom).
inline u16* CTagCodeWait::v14(u16* buf, const wchar_t* arg, TagParam* prm) {
    u16* dst = buf;
    wchar_t* str = (wchar_t*)arg;
    WcsToUpperInPlace((u16*)str);
    s32 v;
    if (wcscmp(str, L"KEY") == 0)
        v = -1;
    else if (wcscmp(str, L"VOICE") == 0)
        v = -2;
    else {
        wchar_t* endptr = 0;
        v = wcstol(str, &endptr, 10);
    }
    dst[0] = 5;
    dst[1] = (u16)v;
    return dst + 2;
}

inline u16* CTagCodeCP::v14(u16* buf, const wchar_t* arg, TagParam* prm) {
    u16* out = buf;
    *out = 3;
    return out + 1;
}

inline u16* CTagCodeCL::v14(u16* buf, const wchar_t* arg, TagParam* prm) {
    u16* out = buf;
    *out = 0xa;
    return out + 1;
}

namespace {

/* Tag-code singletons: retail .sbss 0x80663FE0..0x80664038, one vtable
 * pointer each, in tag order. MWCC emits their constructors as
 * __ct__Q227@unnamed@CTagProcessor_cpp@<Class>Fv and __sinit_CTagProcessor_cpp
 * installs them through the r13 sda21 slots. */


}  // namespace
