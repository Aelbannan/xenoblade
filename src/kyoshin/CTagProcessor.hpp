#pragma once
#include "kyoshin/cf/CtrlRemoteApi.hpp"
#include <types.h>
#include "kyoshin/CTaskGameEff.hpp"
#include "kyoshin/TagProcCompareIds.hpp"

#include <nw4r/db/db_assert.h>
#include <nw4r/ut/ut_TagProcessorBase.h>
#include <nw4r/ut/ut_Rect.h>
#include <nw4r/ut/ut_Color.h>
#include <nw4r/ut/ut_Font.h>
#include <nw4r/ut/ut_CharWriter.h>
#include <nw4r/ut/ut_TextWriterBase.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_textBox.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

/* Table copies used by TagWriterColorBlock's color-tag writer: 55 value dwords and
 * 56 name pointers copied from retail .data (0x804FEFB8 / 0x804FF488) onto
 * the stack as struct assignments (MWCC emits the mtctr/bdnz copy loop for
 * these sizes - 220 and 224 bytes). */
struct TagColorValues {
    u32 values[55];  // +0x00 (220 bytes)
};
struct TagColorNames {
    const wchar_t* names[56];  // +0x00 (224 bytes)
};
extern const TagColorValues lbl_eu_804FEFB8;
extern const TagColorNames lbl_eu_804FF488;

/* int->float conversion scratch (retail tag-proc idiom): seed hi with
 * 0x43300000 once, store (w ^ 0x80000000) into lo per use, then
 * (f32)(d - lbl_eu_80667200). Big-endian: hi word sits at +0. */
union TagConvTemp {
    f64 d;
    struct {
        u32 hi;
        u32 lo;
    } w;
};

/* 6-byte color-tag scratch used by TagWriterColorBlock: the write side seeds it
 * from the sdata2 color constants (u32 + u16), the read side (loop matches +
 * final pack) sees three s16 slots. */
union TagColorOut {
    s16 s[3];  // read side (lha)
    struct {
        u32 c1;  // covers s[0..1]
        u16 c2;  // covers s[2]
    } w;  // write side
};

// sdata2 float constant used by the tag-param init functions
// (retail: .sdata2:0x806671F0).
extern const f32 lbl_eu_806671F0;

// sdata2 float constant used by the tag-writer family (TagWriterCharSpacePP sets
// the TextWriter font size from it; retail: .sdata2:0x80667260).
extern const f32 lbl_eu_80667260;

// sdata2 float constants used by the tag-writer position clamp
// (TagProcCalcPageLayout: line-height/offset math + the y snap thresholds;
// retail .sdata2:0x806671F4..0x8066724C).
extern const f32 lbl_eu_806671F4;
extern const f32 lbl_eu_80667230;
extern const f32 lbl_eu_80667234;
extern const f32 lbl_eu_80667238;
extern const f32 lbl_eu_8066723C;
extern const f32 lbl_eu_80667240;
extern const f32 lbl_eu_80667244;
extern const f32 lbl_eu_80667248;
extern const f32 lbl_eu_8066724C;

// sdata2 float constant used by TagProcStepMessage's auto-speed gate
// (retail .sdata2:0x80667208).
extern const f32 lbl_eu_80667208;

// sdata2 float constants used by the tag-writer family (TagProcBuildOpenText's
// message-speed select / line math; retail .sdata2:0x806671F8..0x80667258).
extern const f32 lbl_eu_806671F8;
extern const f32 lbl_eu_8066720C;
extern const f32 lbl_eu_80667210;
extern const f32 lbl_eu_80667214;
extern const f32 lbl_eu_80667218;
extern const f32 lbl_eu_8066721C;
extern const f32 lbl_eu_80667220;
extern const f32 lbl_eu_80667224;
extern const f32 lbl_eu_80667228;
extern const f32 lbl_eu_8066722C;
extern const f32 lbl_eu_80667254;
extern const f32 lbl_eu_80667258;

// f64 magic constant (0x4330000080000000 = 2^52 + 2^31) used by
// TagWriterParseColonParam's long->float conversion (retail .sdata2:0x80667200).
extern const f64 lbl_eu_80667200;

// f32 fallback value used by TagWriterParseColonParam's single-token tag-param init
// (retail .sdata2:0x8066725C).
extern const f32 lbl_eu_8066725C;

// sdata2 color defaults used by TagWriterColorBlock's color-tag writer
// (retail .sdata2:0x80667188 / 0x8066718C).
extern const u32 lbl_eu_80667188;
extern const u16 lbl_eu_8066718C;

// sdata2 float used by TagWriterMeasureStrings's scratch-writer scale setup
// (retail .sdata2:0x80667264).
extern const f32 lbl_eu_80667264;

// Tag-string table used by the tag-writer family (retail .sdata:0x80661FC8).
// Declared with a tiny size so MWCC treats it as sdata-eligible and emits the
// r13 sda21 access the retail uses (an unsized extern array gets HA/LO). The
// real table is much larger; indexes beyond [1] are accessed via the base
// address + constant offsets.
extern wchar_t lbl_eu_80661FC8[2];

// BDAT-style value table used by TagWriterCode2Lookup's tag-2 lookup chain
// (retail .rodata:0x804FFC48, 0x50 bytes): BdatGetS16ByTableKey(lbl_eu_804FFC48,
// &lbl_eu_804FFC48[0xc], idx) maps a name index to an s16 value.
extern char lbl_eu_804FFC48[];

// Retail vtable data symbols (.data) stored by the tag-proc ctors. The ctors
// write these pointers directly so the emitted relocs name the retail symbols.
extern u32 lbl_eu_8052DA08[];  // CTagProcessorSE vtable
extern u32 lbl_eu_8052DA38[];  // CTagProcessor vtable
extern u32 lbl_eu_8052DA68[];  // CTagProcessorBase vtable

// nw4r db Panic file/message strings used by the tag-writer accessors
// (retail .data:0x8052DBAC..0x8052DF60). First symbol of each pair is the
// Panic(file, ...) argument, second is the message (Panic(..., fmt) arg).
extern char lbl_eu_8052DBAC[];  // TagWriterGetWidthLimit message
extern char lbl_eu_8052DBE0[];  // TagWriterGetWidthLimit file
extern char lbl_eu_8052DF2C[];  // TagWriterGetCursorY message
extern char lbl_eu_8052DF60[];  // TagWriterGetCursorY file
extern char lbl_eu_8052DEE8[];  // TagWriterGetCursorX message
extern char lbl_eu_8052DF1C[];  // TagWriterGetCursorX file
extern char lbl_eu_8052DB64[];  // TagWriterGetCharSpace message
extern char lbl_eu_8052DB98[];  // TagWriterGetCharSpace file
extern char lbl_eu_8052DC3C[];  // TagWriterSetFontSize message
extern char lbl_eu_8052DC70[];  // TagWriterSetFontSize file
extern char lbl_eu_8052DD94[];  // TagWriterGetScaleX message
extern char lbl_eu_8052DDC8[];  // TagWriterGetScaleX file
extern char lbl_eu_8052DDD8[];  // TagWriterGetScaleY message
extern char lbl_eu_8052DE0C[];  // TagWriterGetScaleY file
extern char lbl_eu_8052DB50[];  // TagWriterSetWidthLimit file
extern char lbl_eu_8052DB1C[];  // TagWriterSetWidthLimit message
extern char lbl_eu_8052DC28[];  // TagWriterSetCharSpace file
extern char lbl_eu_8052DBF4[];  // TagWriterSetCharSpace message
extern char lbl_eu_8052DED8[];  // TagWriterAdvanceCursorX file
extern char lbl_eu_8052DEA4[];  // TagWriterAdvanceCursorX message
extern char lbl_eu_8052DD84[];  // TagWriterSetScale file
extern char lbl_eu_8052DD50[];  // TagWriterSetScale message
extern char lbl_eu_8052DE1C[];  // TagWriterSetCursorPos message
extern char lbl_eu_8052DE50[];  // TagWriterSetCursorPos file
extern char lbl_eu_8052DD40[];  // TagWriterApplyColor chain-1 file
extern char lbl_eu_8052DD0C[];  // TagWriterApplyColor chain-1 message
extern char lbl_eu_8052DCFC[];  // TagWriterApplyColor chain-2 file
extern char lbl_eu_8052DCC8[];  // TagWriterApplyColor chain-2 message
extern char lbl_eu_8052DCB8[];  // TagWriterMeasureScratch chain-1 file
extern char lbl_eu_8052DC84[];  // TagWriterMeasureScratch chain-1 message
extern char lbl_eu_8052DE94[];  // TagWriterMeasureScratch chain-2 file
extern char lbl_eu_8052DE60[];  // TagWriterMeasureScratch chain-2 message
extern char lbl_eu_8052DB08[];  // TagWriterJustifyPair chain-1 file
extern char lbl_eu_8052DAD4[];  // TagWriterJustifyPair chain-1 message
extern char lbl_eu_8052DAC0[];  // TagWriterJustifyPair chain-2 file
extern char lbl_eu_8052DA8C[];  // TagWriterJustifyPair chain-2 message

/* Tag parameter block (12 bytes, retail tag-proc layout). */
struct TagParam {
    u8 field_00;   // +0x00
    u8 field_01;   // +0x01
    u8 field_02;   // +0x02
    u8 field_03;   // +0x03
    u16 field_04;  // +0x04
    u8 field_06;   // +0x06
    u8 field_07;   // +0x07 (unused/padding)
    f32 field_08;  // +0x08

    ~TagParam();
};

/* 0x68-byte view of the nw4r TextWriterBase used by TagWriterCopyBlock's copy
 * and the tag-writer scratch writers (TagWriterMeasureStrings / TagWriterMeasureScratch). */
struct TagCopyBlock {
    u32 w00, w04, w08, w0C, w10, w14, w18, w1C;
    u32 w20, w24, w28, w2C, w30, w34, w38, w3C;
    u16 h40;
    u8 b42, b43;
    f32 f44;
    u32 w48;   // CharWriter mFont (+0x48)
    f32 f4C, f50, f54;
    u32 w58, w5C, w60;
};

/* Tag-processor message object (retail tag-proc layout, 0x858 heap block):
 * 1024-wchar message buffer at +0x00, then u16 state fields. The tag-writer
 * family (TagProcPumpMessage / TagProcStartMessage) reads/writes the position counters. */
struct CTagProcMsg {
    u16 buf[0x400];    // +0x000 message buffer (0x800 bytes)
    u8  pad_800[0x4];  // +0x800
    u32 field_804;     // +0x804 talk-source index (findObjectById arg)
    f32 field_808;     // +0x808 current char scale
    u8  pad_80c[0x4];  // +0x80C
    u16 field_810;     // +0x810 next-tag write position / index
    u8  pad_812[0xE];  // +0x812
    u16 field_820;     // +0x820 text-done flag
    u8  pad_822[0x2];  // +0x822
    f32 field_824;     // +0x824 auto-advance speed (negative steps)
    u32 field_828;     // +0x828
    u8  field_82c;     // +0x82c name-display active flag
    u8  pad_82d[0x3];  // +0x82d
    u32 field_830;     // +0x830 name count
    s32 field_834;     // +0x834 current name index (-1 = none)
    u32 m838[3];       // +0x838 per-name char counts (0xC bytes)
    u32 field_844;     // +0x844 input hold counter
    u32 field_848;     // +0x848 icon 1 id
    u8  field_84c;     // +0x84c icon 1 pending flag
    u8  pad_84d[0x3];  // +0x84d
    u32 field_850;     // +0x850 icon 2 id
    u8  field_854;     // +0x854 icon 2 pending flag
    u8  pad_855[0x3];  // +0x855
};

/* Local view of the message text pane vtable (nw4r::lyt::TextBox layout):
 * MWCC emits 2 hidden leading slots, Pane's 27 declared virtuals fill
 * slots 2..28 (0x08..0x70), then the two TextBox-only slots used by
 * TagProcStartMessage: AllocStringBuffer at 0x74 and FreeStringBuffer at 0x78.
 * The real TextBox header cannot be included here (ut_Color.h clash). */
struct CTalkTextBoxVtbl {
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6C();
    virtual void v70();        // +0x70 (Pane tail)
    virtual void v74(u16 len); // +0x74 AllocStringBuffer(u16 len)
    virtual void v78();        // +0x78 FreeStringBuffer()
};

/* Talk-pane view for TagProcBuildOpenText: extends the TextBox vtable view with
 * the +0x7C text-setter (next slot after FreeStringBuffer) and the data
 * fields the function touches (position VEC3 at +0x2C, VEC2 at +0x4C,
 * message-speed f32 at +0xF0). */
/* 12-byte vector copied as an aggregate (MWCC emits lwz/stw pairs) by
 * TagProcRenderPage's pane-position shuffling. */
struct TagVec3 {
    f32 v[3];
};
struct TalkPaneView : public CTalkTextBoxVtbl {
    virtual void v7C(const u16* text, int flag);  // +0x7C
    u8 pad_04[0x28];      // +0x04 (after vptr)
    TagVec3 vec_2C;       // +0x2C
    u8 pad_38[0x14];      // +0x38
    f32 field_4C[2];      // +0x4C
    u8 pad_54[0x84];      // +0x54
    wchar_t* field_D8;    // +0xD8 message string pointer
    u8 pad_E0[0x10];      // +0xE0
    f32 field_F0;         // +0xF0
};

/* The tag dispatch table (retail .data 0x8052D478, 25 x 12B) and its ITagCode
 * handler hierarchy are file-local to CTagProcessor.cpp (anonymous namespace). */

/* Text-pane view for TagProcStepSEMessage (SE message pump): only the 2-arg
 * SetText at +0x7C is called (next slot after FreeStringBuffer). */
struct SeTalkPane : public CTalkTextBoxVtbl {
    virtual void v7C(const u16* text, u16 flag);  // +0x7C SetText (2-arg)
};

/* Icon-flag tables copied to the stack by the icon-tag handlers
 * (retail .rodata:0x804FF608 / 0x804FF6E0, 0x1B x 8 bytes each; MWCC
 * emits the mtctr/bdnz copy loop for these sizes). */
struct TagFlagTable {
    u32 v[54];  // +0x00
};
extern const TagFlagTable lbl_eu_804FF608;
extern const TagFlagTable lbl_eu_804FF6E0;

// sdata2 float constants used by TagProcStepSEMessage's tag-5 auto-speed gate
// (retail .sdata2:0x80667250).
extern const f32 lbl_eu_80667250;

/* Text-pane view for TagProcStepMessage's message pane (`a`) and its context
 * node list (a->field_0C): the vtable slots called are the context-pane
 * factory (+0x3C, returns a per-name pane), string-buffer alloc/free
 * (+0x74/+0x78) and the 3-arg SetText at +0x7C. MWCC emits 2 hidden
 * leading slots, so 30 declared virtuals fill slots 0x08..0x7C. */
struct TalkNamePane;
struct TalkMsgPane {
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual TalkNamePane* v3C(const wchar_t* text, int flag);  // +0x3C
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6C();
    virtual void v70();
    virtual void v74(u16 len);               // +0x74 AllocStringBuffer
    virtual void v78();                      // +0x78 FreeStringBuffer
    virtual void v7C(const u16* text, u16 a, u16 b);  // +0x7C SetText
    u8   pad_04[0x8];   // +0x04
    TalkMsgPane* field_0C;  // +0x0C context node list
    u8   pad_10[0x3C];  // +0x10
    f32  field_4C[2];   // +0x4C
    u8   pad_54[0x68];  // +0x54
    u8*  field_BC;      // +0xBC current text
};

/* Per-name pane created by the context list's +0x3C factory: vtable slots
 * +0x74/+0x78/+0x7C are string-buffer alloc/free and the 2-arg SetText.
 * field_DC/field_E0 are the two text colors written by the name-list
 * highlight walks. */
struct TalkNamePane {
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6C();
    virtual void v70();
    virtual void v74(u16 len);               // +0x74 AllocStringBuffer
    virtual void v78();                      // +0x78 FreeStringBuffer
    virtual void v7C(const u16* text, int flag);  // +0x7C SetText (2-arg)
    u8   pad_04[0x48];  // +0x04
    f32  field_4C[2];   // +0x4C
    u8   pad_54[0x88];  // +0x54
    nw4r::ut::Color field_DC;  // +0xDC
    nw4r::ut::Color field_E0;  // +0xE0
};

/* Party-member object (TagProcStepMessage's r29->field_98): the +0x58 virtual
 * toggles the member's auto-follow/emphasis state (called with (this, on, 0)). */
struct TagMemberObj {
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58(int a, int b);  // +0x58
};

/* Talk-window/icon object (TagProcStepMessage's r29->field_C4): the +0x80
 * virtual is a busy/accept query; the object is passed to func_8004B9D4
 * for icon display. field_270 is the event-flag word polled by
 * TagProcStepSEMessage's icon handler and pending-icon tail. */
struct TagC4Obj {
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6C();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7C();
    virtual int v80(int a);  // +0x80
    u8  pad_04[0x26C];       // +0x04
    u32 field_270;           // +0x270 event-flag word
};

/* Talk-source object (func_800BBC0C result): the member object at +0x98
 * and the icon window at +0xC4; the player variant carries a u16 at +0x8C. */
struct TagTalkSrc {
    u8 pad_00[0x8C];
    u16 field_8C;          // +0x8C
    u8 pad_8E[0xA];
    TagMemberObj* field_98;  // +0x98
    u8 pad_9C[0x28];
    TagC4Obj* field_C4;    // +0xC4
};

/* Tag-processor objects (0x858 heap block). Retail layout: vtable pointer at
 * +0x00 (the nw4r TagProcessorBase subobject), then the 0x800-byte message
 * buffer, then state fields. The classes are real C++ classes deriving from
 * the nw4r tag processor, but marked __declspec(novtable): the retail vtable
 * data lives in the retail .data (annotated lbl_eu_8052DA68 / lbl_eu_8052DA38
 * / lbl_eu_8052DA08), so MWCC must not emit its own __vt__* table or
 * auto-store it -- the ctor stores the retail vtable pointer explicitly. The
 * ctor entry points are the short C-ABI names (__ct__17CTagProcessorBaseFv /
 * __ct__CTagProcessor / __ct__CTagProcessorSE); the base ctor is referenced
 * by its C-ABI name from the derived ctor functions, while the dtors are
 * genuine C++ virtual dtors (full mangled names) so the base-dtor chain is
 * compiler-generated (MWCC_CASES "novtable + extern "C" ctor + virtual
 * dtor"). */
struct __declspec(novtable) CTagProcessorBase
    : public nw4r::ut::TagProcessorBase<wchar_t> {
    CTagProcessorBase();
    virtual ~CTagProcessorBase();
    virtual const wchar_t* Proc(wchar_t* text, int param, float a, float b); // 0x14 retail TagProcPreprocessMessage

    u16 mBuf[0x400];    // +0x04 message buffer (0x800 bytes)
    u32 field_804;      // +0x804
    f32 field_808;      // +0x808
    f32 field_80c;      // +0x80c
    u16 field_810;      // +0x810 next-tag write position
    u8  pad_812[2];     // +0x812
    TagParam field_814; // +0x814
};

struct __declspec(novtable) CTagProcessor : public CTagProcessorBase {
    virtual ~CTagProcessor();

    u16 field_820;       // +0x820 text-done flag
    u8  pad_822[2];      // +0x822
    f32 field_824;       // +0x824
    u32 field_828;       // +0x828
    u8  field_82c;       // +0x82c
    u8  pad_82d[3];      // +0x82d
    u32 field_830;       // +0x830
    u32 field_834;       // +0x834 (init -1)
    u8  m838[0xC];       // +0x838 zeroed block (memset 0xc)
    u32 field_844;       // +0x844 (init 10)
    u32 field_848;       // +0x848
    u8  field_84c;       // +0x84c
    u8  pad_84d[3];      // +0x84d
    u32 field_850;       // +0x850
    u8  field_854;       // +0x854
    u8  pad_855[3];      // +0x855
};

/* Char-space/position context updated by the tag-writer family
 * (TagWriterCharSpacePP writes field_04/field_08, addToCharSpace reads field_04
 * and writes field_0c). */
struct TagCharContext {
    f32 field_00;  // +0x00
    f32 field_04;  // +0x04
    f32 field_08;  // +0x08
    f32 field_0c;  // +0x0C
};

/* Holder for the active nw4r text writer (TagWriterCharSpacePP reads field_00
 * twice - once into a local, once for the TagWriterGetCursorY argument;
 * TagWriterMeasureStrings also reads field_04/field_10 and rewrites field_04). */
struct TagWriterHolder {
    nw4r::ut::TextWriterBase<wchar_t>* field_00;  // +0x00
    const u8* field_04;                           // +0x04 tag string buffer
    u8 pad_08[0x8];                               // +0x08
    u32 field_10;                                 // +0x10 flags
};

/* Arg block for the color-tag writer TagWriterApplyColor: the target CharWriter
 * and the source color. The writer's current alpha is preserved (the merge
 * writes src.a = writer.mVertexColor.a before the out-of-line assign). */
struct TagColorArg {
    nw4r::ut::CharWriter* field_00;  // +0x00
    nw4r::ut::Color* field_04;       // +0x04
};

struct __declspec(novtable) CTagProcessorSE : public CTagProcessorBase {
    virtual ~CTagProcessorSE();

    u16 field_820;       // +0x820 text-done flag
    u8  pad_822[2];      // +0x822
    f32 field_824;       // +0x824
    u32 field_828;       // +0x828
    u8  field_82c;       // +0x82c
    u8  pad_82d[3];      // +0x82d
    u32 field_830;       // +0x830
    u8  field_834;       // +0x834
    u8  pad_835[3];      // +0x835
    u32 field_838;       // +0x838
    u8  field_83c;       // +0x83c
    u8  pad_83d[3];      // +0x83d
};

// C-ABI imports used by the tag-writer family. These retail symbols are
// unmangled, so they must stay C-linkage or MWCC appends a __F<argtypes>
// suffix to the emitted reloc names (MWCC_CASES core pattern 2).
// TagProcPumpMessage / TagProcStepMessage / TagProcStartMessage / TagProcBuildOpenText are also
// declared in CTalkWindow.hpp / CSimpleEveTalkWin.hpp with these signatures.
extern "C" {
void TagParamClear(TagParam* p);  // reset a tag-param block
void initTagParam(u8* obj, unsigned char a, unsigned char b, float c);  // 12-byte tag-param init
void copyTagParam(u8* dst, const u8* src);  // 12-byte tag-param copy
int TagProcPumpMessage(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                  nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);
int TagProcStepMessage(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                  nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);
// SE message pump (CTagProcessorSE variant of TagProcStepMessage, no name list).
int TagProcStepSEMessage(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* pane);
// Voice/sound helpers used by TagProcStepSEMessage's auto-advance gate.
s32 MenuSnd_QuerySlotLive_89A04(s32 index);                                // sound-slot busy check
void MenuSnd_SilenceByName_8986C(int handle, f32 fadeTime);                 // stop voice
// Plugin UI mode query (pluginUi.cpp).
int TagProcStartMessage(void* tagProc, nw4r::lyt::Pane* pane);
// Retail r5 is a string (wcscpy source), not a flag. The CSimpleEveTalkWin.hpp
// copy still declares the older int-flag shape; the two headers are never
// included together, and extern "C" keeps the symbol flat either way.
void TagProcBuildOpenText(CTagProcessorBase* tagProc, nw4r::lyt::Pane* pane,
                   const wchar_t* str);
// Same signatures as the CTalkWindow.hpp declarations (shared C-ABI symbols).
void TagProcRenderPage(CTagProcMsg* msg, nw4r::lyt::TextBox* a, nw4r::lyt::TextBox* b,
                   nw4r::lyt::TextBox* c, const wchar_t* text);
void TagProcResetPage(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                   nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);

// Tag-proc ctors: retail symbols are the short C-ABI names (no class-length
// mangling). __ct__17CTagProcessorBaseFv is referenced (not defined) here -- its
// bytes come from the member ctor CTagProcessorBase::CTagProcessorBase() in
// the .cpp, which needs the compiler-generated base-ctor call (placement new
// would add a null beq the retail does not have). The derived ctors store the
// retail vtable symbols by hand (see the class comment above).
void* __ct__17CTagProcessorBaseFv(void* self);
void* __ct__CTagProcessor(void* self);
void* __ct__CTagProcessorSE(void* self);

// Tag-code singleton ctors (retail C-ABI names). Each stores its class
// vtable into the sbss singleton at the given address; sinit_8012BA00 calls
// them in tag order. The anonymous-namespace classes themselves are not
// declared here - only the C-ABI ctor entry points are needed by sinit (the
// class dtors are the __dt___unnamed_CTagProcessor_cpp_CTagCode* stubs).
void __ct__CTagCodeCL(void* self);
void __ct__CTagCodeCP(void* self);
void __ct__CTagCodeWait(void* self);
void __ct__CTagCodeDel(void* self);
void __ct__CTagCodeRuby(void* self);
void __ct__CTagCodeCol(void* self);
void __ct__CTagCodeTrust(void* self);
void __ct__CTagCodeType(void* self);
void __ct__CTagCodePos(void* self);
void __ct__CTagCodeMouth(void* self);
void __ct__CTagCodeLine(void* self);
void __ct__CTagCodeSelect(void* self);
void __ct__CTagCodeSelect2(void* self);
void __ct__CTagCodeAct(void* self);
void __ct__CTagCodeFade(void* self);
void __ct__CTagCodeQuest(void* self);
void __ct__CTagCodeInvisible(void* self);
void __ct__CTagCodeQstStart(void* self);
void __ct__CTagCodePresent(void* self);
void __ct__CTagCodeTailOff(void* self);
void __ct__CTagCodeExchange(void* self);
void __ct__CTagCodeShop(void* self);
void __ct__CTagCodeMakeCrystal(void* self);

// Tag-writer family entries (unmangled retail names; the arg-taking members
// must stay C-linkage or MWCC appends a __F<argtypes> suffix to the emitted
// reloc names - MWCC_CASES core pattern 2).
float TagWriterGetWidthLimit(nw4r::ut::TextWriterBase<wchar_t>* tw);
void TagWriterSetWidthLimit(nw4r::ut::TextWriterBase<wchar_t>* tw, float v);
void TagWriterAdvanceCursorX(nw4r::ut::TextWriterBase<wchar_t>* tw, float v);
float TagWriterGetCursorY(nw4r::ut::TextWriterBase<wchar_t>* tw);
float TagWriterGetCursorX(nw4r::ut::TextWriterBase<wchar_t>* tw);
// Same nw4r-pointer-validation accessor family (GetScale-ish / GetCharSpace
// / font-size store wrappers; unmangled retail names).
float TagWriterGetScaleX(nw4r::ut::TextWriterBase<wchar_t>* tw);
float TagWriterGetScaleY(nw4r::ut::TextWriterBase<wchar_t>* tw);
float TagWriterGetCharSpace(nw4r::ut::TextWriterBase<wchar_t>* tw);
void TagWriterSetFontSize(nw4r::ut::TextWriterBase<wchar_t>* tw, u32 value);
void TagWriterSetCharSpace(nw4r::ut::TextWriterBase<wchar_t>* tw, float v);
void TagWriterSetScale(nw4r::ut::TextWriterBase<wchar_t>* tw, float x, float y);
// Tag-param writer: split the arg string on ':', map the first token to a
// tag index, and init/copy a 1-tag-param block (numeric value when a second
// token exists, else the fixed f32 fallback). Returns the r4 arg.
void* TagWriterParseColonParam(void* unused, void* ret, wchar_t* str, TagParam* dst);
// Text-position measurement for the tag-writer position clamp (TagProcCalcPageLayout):
// out = (x, y) computed from the message context + string.
void TagProcMeasureText(f32* out, nw4r::lyt::TextBox* msg, wchar_t* buf);
// Tag-writer position clamp: copy the message into a local buffer, measure
// it, clamp/snap the y position to the fixed step table, and write the
// resulting VEC2 to out->field_4C.
void TagProcCalcPageLayout(void* unused, nw4r::lyt::TextBox* msg, nw4r::lyt::Pane* out);
int TagWriterCharSpacePP(void* unused, TagCharContext* ctx, void* unused2,
                  TagWriterHolder* holder);
void* TagWriterSelectParam8b(void* unused, void* ret, wchar_t* str, TagParam* dst);
// Tag-writer cursor setter: validate the writer, then store mCursorPos.x/y
// (CharWriter +0x2C/+0x30). Same pointer validation as TagWriterGetWidthLimit.
void TagWriterSetCursorPos(nw4r::ut::TextWriterBase<wchar_t>* tw, float x, float y);
// Tag-writer helper: max line width + font height of a string (uses the
// TextBox's font height getter and char-width getter).
void TagMeasureTextWidth(f32* out, void* unused,
                                        nw4r::lyt::TextBox* textbox,
                   const wchar_t* str);
// Color-tag writer: match tokens against the copied name table and pack the
// result block. Returns the position after the block.
u16* TagWriterColorBlock(void* unused, u16* dst, wchar_t* str);
// Tag-code writer (code 8, name selection): map the string to a value 1..9.
void* TagWriterSelectParam8a(void* unused, void* ret, wchar_t* str, TagParam* dst);
// Tag-writer line layout: measure two adjacent strings from the tag buffer
// into a scratch writer, advance the buffer pointer, return 2.
int TagWriterMeasureStrings(void* unused, nw4r::ut::Rect* out, void* unused2,
                  TagWriterHolder* holder);
// Tag-writer two-string layout: measure the first string in a scratch
// writer, distribute any excess width of the real writer's copy as char
// space, print both strings, and advance the buffer pointer. Returns 2.
int TagWriterJustifyPair(void* unused, void* unused2, TagWriterHolder* holder);
u16* TagWriterCode1List(void* unused, u16* dst, wchar_t* str);
u16* TagWriterCode5Value(void* unused, u16* dst, wchar_t* str);
// Tag-code writer (code 0xB): split the arg string on ':', map token 0 to a
// 1..2 tag id and tokens 1..4 to numeric values (tag 2 via the BDAT s16
// lookup chain), write the 6-u16 block, return the advanced position.
u16* TagWriterCode2Lookup(void* unused, u16* out, wchar_t* str);
u16* TagWriterCode7List(void* unused, u16* dst, wchar_t* str);
u16* TagWriterCode6List(void* unused, u16* dst, wchar_t* str);
// String splitter (code_80135FDC.cpp): split src on delim into outTokens,
// returns the token count.
// BDAT s16-keyed lookup (code_80135FDC.cpp): maps a key into the table pair
// to an s16 value (TagWriterCode2Lookup's tag-2 name chain).
// Message pre-processor: copy the raw message, normalize/compact the buffer
// (CRLF strip + '<name=value>' tag dispatch), return the buffer.
u16* TagProcPreprocessMessage(CTagProcessorBase* self, const void* src, f32 a, f32 b,
                   u32 c);
// Talk-open text setup (TagProcStartMessage's callee): build the display string from
// the message buffer (or the caller's string) and lay out the pane.
void TagWriterCopyBlock(TagCopyBlock* dst, const TagCopyBlock* src);
// Color-name tag writer: map the uppercased string to a packed RGB value.
u16* TagParseColor(void* unused, u16* out, wchar_t* str);
// Color-tag writer: apply the source color to the CharWriter (alpha kept).
u32 TagWriterApplyColor(void* unused, void* ret, TagColorArg* arg);
// Tag-writer: print a marker into a scratch writer and advance the cursor.
int TagWriterMeasureScratch(void* unused, void* unused2, TagWriterHolder* holder);
// Accumulated pane translate (code_80135FDC.cpp).

// TagProcStepMessage's C-ABI imports: flat (or pre-mangled) retail names.
extern "C" void* func_800BBC0C(void* obj);  // talk-source getter (retail r3 = prior call's result)
extern "C" void func_8004B9D4(void* w, int a, int b, int c, int d);

void CtrlRemote_SetSharedBit(u32 owner, u32 value);
void UIWin_CreateMenuUpdate(u32 a, u32 b, u32 c, u32 d);
// cf::CfGameManager controller-mode query. Retail name keeps the Fv suffix
// but the call passes -1 in r3 (same convention as CTalkWindow.hpp).
// Local context-walk helpers (defined in CTagProcessor.cpp).
void* TagCtxGetNodeHeader(void* self);
// TagProcCompareIds is declared once in kyoshin/TagProcCompareIds.hpp.
void* TagCtxAdvanceHead(void* self, int a);
const wchar_t* getContextStr(u8* self);
const wchar_t** getContextStrPtr(u8* self);
void copyVEC2(float* dst, const float* src);
}

// C++-linkage imports (retail emits the mangled forms).
void* findObjectById(int id);  // findObjectById__Fi (talk-source index lookup)

// Tag-code singleton instances (retail .sbss:0x80663FE0..0x80664038, 4 bytes
// each - just the vtable pointer). Declared with plain u32 so MWCC treats
// them as sbss-eligible and emits the r13 sda21 access sinit uses; the symbol
// map places them at the retail addresses.
extern u32 lbl_eu_80663FE0;
extern u32 lbl_eu_80663FE4;
extern u32 lbl_eu_80663FE8;
extern u32 lbl_eu_80663FEC;
extern u32 lbl_eu_80663FF0;
extern u32 lbl_eu_80663FF4;
extern u32 lbl_eu_80663FF8;
extern u32 lbl_eu_80663FFC;
extern u32 lbl_eu_80664000;
extern u32 lbl_eu_80664004;
extern u32 lbl_eu_80664008;
extern u32 lbl_eu_8066400C;
extern u32 lbl_eu_80664010;
extern u32 lbl_eu_80664014;
extern u32 lbl_eu_80664018;
extern u32 lbl_eu_8066401C;
extern u32 lbl_eu_80664020;
extern u32 lbl_eu_80664024;
extern u32 lbl_eu_80664028;
extern u32 lbl_eu_8066402C;
extern u32 lbl_eu_80664030;
extern u32 lbl_eu_80664034;
extern u32 lbl_eu_80664038;

