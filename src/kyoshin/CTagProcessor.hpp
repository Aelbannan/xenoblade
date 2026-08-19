#pragma once

#include <types.h>

#include <nw4r/db/db_assert.h>
#include <nw4r/ut/ut_TagProcessorBase.h>
#include <nw4r/ut/ut_Rect.h>

// Forward decls only: this TU defines its own byte-wise nw4r::ut::Color, so the
// real lyt headers (which pull ut_Color.h) cannot be included here.
namespace nw4r {
namespace lyt {
class AnimTransform;
class Pane;

/* Layout view of nw4r::lyt::Font used by func_80127D20: only the two virtual
 * slots the function calls are used (+0x34 int-returning height-ish getter,
 * +0x48 char-width getter). MWCC emits 2 hidden leading slots for
 * polymorphic classes in this TU (RTTI), so the first declared virtual sits
 * at +0x08 and the 12th/17th declared virtuals sit at +0x34/+0x48. */
#ifndef NW4R_LYT_RESOURCES_H
class Font {
public:
    virtual ~Font();   // +0x08
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
    virtual int v34();      // +0x34 returns int (font height-ish)
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual int v48(u16 c); // +0x48 returns int (char width)
};
#endif

/* Layout view of nw4r::lyt::TextBox for func_80127D20 (GetFont is the only
 * member used; mangled name GetFont__Q34nw4r3lyt7TextBoxCFv). Only defined
 * when the real lyt_textBox.h has not been included yet. */
#ifndef NW4R_LYT_TEXTBOX_H
class TextBox {
public:
    Font* GetFont() const;
};
#endif

}
}

/* Table copies used by func_80128DA0's color-tag writer: 55 value dwords and
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

/* 6-byte color-tag scratch used by func_80128DA0: the write side seeds it
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

// sdata2 float constant used by the tag-writer family (func_80129128 sets
// the TextWriter font size from it; retail: .sdata2:0x80667260).
extern const f32 lbl_eu_80667260;

// sdata2 float constants used by the tag-writer position clamp
// (func_80127BF4: line-height/offset math + the y snap thresholds;
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

// sdata2 float constant used by func_8012615C's auto-speed gate
// (retail .sdata2:0x80667208).
extern const f32 lbl_eu_80667208;

// sdata2 float constants used by the tag-writer family (func_801287BC's
// message-speed select / line math; retail .sdata2:0x806671F8..0x80667258).
extern const f32 lbl_eu_806671F8;
extern const f32 lbl_eu_8066720C;
extern const f32 lbl_eu_80667228;
extern const f32 lbl_eu_80667254;
extern const f32 lbl_eu_80667258;

// f64 magic constant (0x4330000080000000 = 2^52 + 2^31) used by
// func_80128C6C's long->float conversion (retail .sdata2:0x80667200).
extern const f64 lbl_eu_80667200;

// f32 fallback value used by func_80128C6C's single-token tag-param init
// (retail .sdata2:0x8066725C).
extern const f32 lbl_eu_8066725C;

// sdata2 color defaults used by func_80128DA0's color-tag writer
// (retail .sdata2:0x80667188 / 0x8066718C).
extern const u32 lbl_eu_80667188;
extern const u16 lbl_eu_8066718C;

// sdata2 float used by func_8012B070's scratch-writer scale setup
// (retail .sdata2:0x80667264).
extern const f32 lbl_eu_80667264;

// Tag-string table used by the tag-writer family (retail .sdata:0x80661FC8).
// Declared with a tiny size so MWCC treats it as sdata-eligible and emits the
// r13 sda21 access the retail uses (an unsized extern array gets HA/LO). The
// real table is much larger; indexes beyond [1] are accessed via the base
// address + constant offsets.
extern wchar_t lbl_eu_80661FC8[2];

// BDAT-style value table used by func_8012A460's tag-2 lookup chain
// (retail .rodata:0x804FFC48, 0x50 bytes): func_80136130(lbl_eu_804FFC48,
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
extern char lbl_eu_8052DBAC[];  // func_801291F4 message
extern char lbl_eu_8052DBE0[];  // func_801291F4 file
extern char lbl_eu_8052DF2C[];  // func_80129564 message
extern char lbl_eu_8052DF60[];  // func_80129564 file
extern char lbl_eu_8052DEE8[];  // func_801299D4 message
extern char lbl_eu_8052DF1C[];  // func_801299D4 file
extern char lbl_eu_8052DB64[];  // func_8012B328 message
extern char lbl_eu_8052DB98[];  // func_8012B328 file
extern char lbl_eu_8052DC3C[];  // func_80129E20 message
extern char lbl_eu_8052DC70[];  // func_80129E20 file
extern char lbl_eu_8052DD94[];  // func_80129AEC message
extern char lbl_eu_8052DDC8[];  // func_80129AEC file
extern char lbl_eu_8052DDD8[];  // func_80129C04 message
extern char lbl_eu_8052DE0C[];  // func_80129C04 file
extern char lbl_eu_8052DB50[];  // func_8012930C file
extern char lbl_eu_8052DB1C[];  // func_8012930C message
extern char lbl_eu_8052DC28[];  // func_8012B204 file
extern char lbl_eu_8052DBF4[];  // func_8012B204 message
extern char lbl_eu_8052DED8[];  // func_80129430 file
extern char lbl_eu_8052DEA4[];  // func_80129430 message
extern char lbl_eu_8052DD84[];  // func_80129F3C file
extern char lbl_eu_8052DD50[];  // func_80129F3C message
extern char lbl_eu_8052DE1C[];  // func_8012A070 message
extern char lbl_eu_8052DE50[];  // func_8012A070 file
extern char lbl_eu_8052DD40[];  // func_8012AD40 chain-1 file
extern char lbl_eu_8052DD0C[];  // func_8012AD40 chain-1 message
extern char lbl_eu_8052DCFC[];  // func_8012AD40 chain-2 file
extern char lbl_eu_8052DCC8[];  // func_8012AD40 chain-2 message
extern char lbl_eu_8052DCB8[];  // func_8012968C chain-1 file
extern char lbl_eu_8052DC84[];  // func_8012968C chain-1 message
extern char lbl_eu_8052DE94[];  // func_8012968C chain-2 file
extern char lbl_eu_8052DE60[];  // func_8012968C chain-2 message

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
};

/* 0x68-byte view of the nw4r TextWriterBase used by func_80129D1C's copy
 * and the tag-writer scratch writers (func_8012B070 / func_8012968C). */
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

namespace nw4r {
namespace ut {

/* nw4r::ut::Color as seen by this TU: retail copy-assign is byte-wise,
 * so the class carries four plain u8 components (no u32 shortcut).
 * operator= is defined out-of-line in CTagProcessor.cpp so it is emitted
 * as the standalone retail symbol. Only defined when the real ut_Color.h
 * has not been included yet (other TUs include both). */
#ifndef NW4R_UT_COLOR_H
struct Color {
    union {
        u32 v;  // packed ARGB value (0xAARRGGBB)
        struct {
            u8 r;  // +0x00
            u8 g;  // +0x01
            u8 b;  // +0x02
            u8 a;  // +0x03
        } comp;
    };

    Color& operator=(const Color& rhs);
};
#endif

/* 4-byte color-value pack for the name-list highlight walks: the retail
 * constructs the packed ARGB u32 with lis/addi and stores it to the stack
 * slot that the out-of-line Color::operator= then reads. */

}  // namespace ut
}  // namespace nw4r

/* Minimal nw4r::ut::CharWriter / TextWriterBase views used by the tag-writer
 * family. The real ut_CharWriter.h / ut_TextWriterBase.h cannot be included
 * here (they pull ut_Color.h, which clashes with the byte-wise Color above),
 * so only the members called from this TU are declared. The class names and
 * signatures match the retail nw4r headers, so MWCC emits the same mangled
 * callee names (CalcStringWidth__Q34nw4r2ut17TextWriterBase<w>CFPCwi /
 * GetFontHeight__Q34nw4r2ut10CharWriterCFv). ut_TagProcessorBase.h already
 * forward-declares the template; this is the matching definition. */
namespace nw4r {
namespace ut {

/* Opaque CharWriter font (ut::detail::Font view) used by func_8012968C:
 * vtable slot +0x48 is a no-arg int getter whose value is doubled and
 * float-converted for the cursor-advance math. 2 hidden leading slots
 * (RTTI), so 16 declared virtuals precede the +0x48 slot. */
#ifndef NW4R_UT_FONT_H
class FontH {
public:
    virtual ~FontH();   // +0x08
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
    virtual int v48(u16 c);  // +0x48 char-width getter (func_8012968C passes U+2500)
};
#endif

/* Layout view of CharWriter for the tag-writer accessors (real
 * ut_CharWriter.h layout): mCursorPos is a math::VEC3 at +0x2C with x at
 * 0x2C and y at 0x30. Only the fields the tag-writer family reads are
 * named; the rest is padding so the offsets stay exact. */
#ifndef NW4R_UT_CHAR_WRITER_H
class CharWriter {
public:
    f32 GetFontHeight() const;
    void UpdateVertexColor();  // out-of-line nw4r method (retail bl)

    u8  pad_00[0x18];                    // +0x00 mColorMapping..mVertexColor-1
    nw4r::ut::Color mVertexColor;        // +0x18 (func_8012AD40 merges alpha here)
    u8  pad_1C[0x8];                     // +0x1C
    f32 field_24;                        // +0x24 (mScale region; func_80129AEC reads this)
    f32 field_28;                        // +0x28 (mScale region; func_80129C04 reads this)
    f32 cursorX;                         // +0x2C mCursorPos.x
    f32 cursorY;                         // +0x30 mCursorPos.y
    u8  pad_34[0x14];                    // +0x34 mCursorPos.z .. +0x47
    FontH* mFont;                        // +0x48 (func_8012968C reads the +0x48 getter)
};
#endif

/* Layout view of TextWriterBase for the tag-writer accessors: mWidthLimit
 * is the first TextWriterBase member, at +0x4C right after CharWriter. */
#ifndef NW4R_UT_TEXT_WRITER_BASE_H
template <typename T> class TextWriterBase : public CharWriter {
public:
    f32 CalcStringWidth(const T* pStr, int len) const;
    void Print(const T* pStr, int len);  // func_8012968C prints the 1-char marker
    ~TextWriterBase();  // __dt__Q34nw4r2ut17TextWriterBase<w>Fv (local scratch dtor)

    f32 widthLimit;    // +0x4C mWidthLimit
    f32 charSpace;     // +0x50 mCharSpace
    u8  pad_54[0x8];   // +0x54 mLineSpace / mFontSize.x
    u32 field_5C;      // +0x5C (mFontSize.y region; func_80129E20 stores here)
    u8  pad_60[0x8];   // +0x60 mTagProcessor
};
#endif

}  // namespace ut
}  // namespace nw4r

/* Tag-processor message object (retail tag-proc layout, 0x858 heap block):
 * 1024-wchar message buffer at +0x00, then u16 state fields. The tag-writer
 * family (func_801276F4 / func_80128740) reads/writes the position counters. */
struct CTagProcMsg {
    u16 buf[0x400];    // +0x000 message buffer (0x800 bytes)
    u8  pad_800[0x4];  // +0x800
    u32 field_804;     // +0x804 talk-source index (func_800B708C arg)
    u8  pad_808[0x8];  // +0x808
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
 * func_80128740: AllocStringBuffer at 0x74 and FreeStringBuffer at 0x78.
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

/* Talk-pane view for func_801287BC: extends the TextBox vtable view with
 * the +0x7C text-setter (next slot after FreeStringBuffer) and the data
 * fields the function touches (position VEC3 at +0x2C, VEC2 at +0x4C,
 * message-speed f32 at +0xF0). */
struct TalkPaneView : public CTalkTextBoxVtbl {
    virtual void v7C(const u16* text, int flag);  // +0x7C
    u8 pad_04[0x28];      // +0x04 (after vptr)
    f32 vec_2C[3];        // +0x2C
    u8 pad_38[0x14];      // +0x38
    f32 field_4C[2];      // +0x4C
    u8 pad_54[0x9C];      // +0x54
    f32 field_F0;         // +0xF0
};

/* Tag-code dispatch table entry (retail .data:0x8052D478; 25 entries x 12B).
 * func_80125AB8 / func_80125B08 walk this table; field_04 == 0 marks the
 * terminal "no handler" entry that falls back to the base TagProcessor. */
struct TagEntry {
    u16 tag;       // +0x00 tag-code value
    u16 pad;       // +0x02
    u32 field_04;  // +0x04 nonzero = entry has a handler
    class CTagCode* obj;  // +0x08 handler object
};

extern TagEntry lbl_eu_8052D478[];

/* Name-table view of the tag dispatch table used by func_80125B58: same
 * 12-byte entries as TagEntry, but +4 is the handler's name string
 * (wcscmp argument) and +8 is the handler object whose vtable slot +0x14
 * processes the tag body and returns the new buffer position. */
struct TagNameEntry {
    u32 field_00;          // +0x00 (tag code, unused here)
    const wchar_t* name;   // +0x04 handler name string
    class CTagCode* obj;   // +0x08 handler object
};

/* Per-tag handler object referenced by the dispatch table (retail vtable
 * size 0x18 = 6 slots, MWCC emits 2 hidden leading slots). Slot 2 = dtor,
 * slot 3 (0x0C) = Process, slot 4 (0x10) = CalcRect. */
class CTagCode {
public:
    virtual ~CTagCode();  // slot 2
    virtual nw4r::ut::TagProcessorBase<wchar_t>::Operation Process(
        u16 tag, nw4r::ut::PrintContext<wchar_t>* ctx);  // slot 3
    virtual nw4r::ut::TagProcessorBase<wchar_t>::Operation CalcRect(
        nw4r::ut::Rect* rect, u16 tag,
        nw4r::ut::PrintContext<wchar_t>* ctx);  // slot 4
    // slot 5 @ +0x14: tag-body processor called by func_80125B58's name
    // dispatch (consumes the '<name=value>' tag, returns the new position).
    virtual u16* v14(u16* p, const wchar_t* value, TagParam* param);
};

/* 216-byte flag/icon table copied to the stack by func_8012615C's icon-tag
 * handler (retail .rodata:0x804FF608, 0x1B x 8 bytes; MWCC emits the
 * mtctr/bdnz copy loop for this size, same as func_80128DA0's tables). */
struct TagFlagTable {
    u32 v[54];  // +0x00
};
extern const TagFlagTable lbl_eu_804FF608;

/* Text-pane view for func_8012615C's message pane (`a`) and its context
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

/* Party-member object (func_8012615C's r29->field_98): the +0x58 virtual
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

/* Talk-window/icon object (func_8012615C's r29->field_C4): the +0x80
 * virtual is a busy/accept query; the object is passed to func_8004B9D4
 * for icon display. */
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

/* Pad-data views for the CfGameManager pad getters used by func_8012615C. */
struct TagPadView {
    u8  pad[0x4];
    u32 field_04;   // +0x04 button bits
};
struct TagPadDataView {
    u8  pad[0x4];
    u32 field_04;    // +0x04 button bits
    u8  pad_08[0xFC];
    u32 field_104;   // +0x104
};

/* Tag-processor objects (0x858 heap block). Retail layout: vtable pointer at
 * +0x00 (the nw4r TagProcessorBase subobject), then the 0x800-byte message
 * buffer, then state fields. The classes are real C++ classes deriving from
 * the nw4r tag processor, but marked __declspec(novtable): the retail vtable
 * data lives in the retail .data (annotated lbl_eu_8052DA68 / lbl_eu_8052DA38
 * / lbl_eu_8052DA08), so MWCC must not emit its own __vt__* table or
 * auto-store it — the ctor stores the retail vtable pointer explicitly. The
 * ctor entry points are the short C-ABI names (__ct__CTagProcessorBase /
 * __ct__CTagProcessor / __ct__CTagProcessorSE); the base ctor is referenced
 * by its C-ABI name from the derived ctor functions, while the dtors are
 * genuine C++ virtual dtors (full mangled names) so the base-dtor chain is
 * compiler-generated (MWCC_CASES "novtable + extern "C" ctor + virtual
 * dtor"). */
struct __declspec(novtable) CTagProcessorBase
    : public nw4r::ut::TagProcessorBase<wchar_t> {
    CTagProcessorBase();
    virtual ~CTagProcessorBase();

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
 * (func_80129128 writes field_04/field_08, addToCharSpace reads field_04
 * and writes field_0c). */
struct TagCharContext {
    f32 field_00;  // +0x00
    f32 field_04;  // +0x04
    f32 field_08;  // +0x08
    f32 field_0c;  // +0x0C
};

/* Holder for the active nw4r text writer (func_80129128 reads field_00
 * twice - once into a local, once for the func_80129564 argument;
 * func_8012B070 also reads field_04/field_10 and rewrites field_04). */
struct TagWriterHolder {
    nw4r::ut::TextWriterBase<wchar_t>* field_00;  // +0x00
    const u8* field_04;                           // +0x04 tag string buffer
    u8 pad_08[0x8];                               // +0x08
    u32 field_10;                                 // +0x10 flags
};

/* Arg block for the color-tag writer func_8012AD40: the target CharWriter
 * and the source color. The writer's current alpha is preserved (the merge
 * writes src.a = writer.mVertexColor.a before the out-of-line assign). */
struct TagColorArg {
    nw4r::ut::CharWriter* field_00;  // +0x00
    nw4r::ut::Color* field_04;       // +0x04
};

/* Message-context view used by the tag-writer position clamp
 * (func_80127BF4): wchar_t* message string at +0xD8, copied into a local
 * buffer before func_80125D00 measures it. func_80125D00 reads the float
 * at +0xF4 (per-char spacing added to each glyph width). */
struct CTagMsgView {
    u8 pad[0xD8];        // +0x00
    wchar_t* field_D8;   // +0xD8 message string pointer
    u8 pad_E0[0x14];     // +0xE0
    f32 field_F4;        // +0xF4 per-char spacing
};

/* Output view used by the tag-writer position clamp (func_80127BF4):
 * VEC2 destination at +0x4C (copyVEC2 target). */
struct CTagOutView {
    u8 pad[0x4C];    // +0x00
    f32 field_4C;    // +0x4C
    f32 field_50;    // +0x50
};

/* Output view used by func_8012B070: reads field_00, writes field_08
 * (addToCharSpace additionally touches +0x04/+0x0C via the u8* helper). */
struct TagLineOutView {
    f32 field_00;  // +0x00
    f32 field_04;  // +0x04
    f32 field_08;  // +0x08
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
// func_801276F4 / func_8012615C / func_80128740 / func_801287BC are also
// declared in CTalkWindow.hpp / CSimpleEveTalkWin.hpp with these signatures.
extern "C" {
void func_801366F4(u16* str);  // uppercase wide string (code_80135FDC.cpp)
void func_801258D0(TagParam* p);  // reset a tag-param block
void initTagParam(u8* obj, unsigned char a, unsigned char b, float c);  // 12-byte tag-param init
void copyTagParam(u8* dst, const u8* src);  // 12-byte tag-param copy
int func_801276F4(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                  nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);
int func_8012615C(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                  nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);
int func_80128740(void* tagProc, nw4r::lyt::Pane* pane);
// Retail r5 is a string (wcscpy source), not a flag. The CSimpleEveTalkWin.hpp
// copy still declares the older int-flag shape; the two headers are never
// included together, and extern "C" keeps the symbol flat either way.
void func_801287BC(CTagProcessorBase* tagProc, nw4r::lyt::Pane* pane,
                   const wchar_t* str);
// Same signatures as the CTalkWindow.hpp declarations (shared C-ABI symbols).
void func_80127764(void* tagProc, nw4r::lyt::Pane* a, nw4r::lyt::Pane* b,
                   nw4r::lyt::Pane* c, int flag);
void func_80127E74(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                   nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);

// Tag-proc ctors: retail symbols are the short C-ABI names (no class-length
// mangling). __ct__CTagProcessorBase is referenced (not defined) here — its
// bytes come from the member ctor CTagProcessorBase::CTagProcessorBase() in
// the .cpp, which needs the compiler-generated base-ctor call (placement new
// would add a null beq the retail does not have). The derived ctors store the
// retail vtable symbols by hand (see the class comment above).
void* __ct__CTagProcessorBase(void* self);
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
float func_801291F4(nw4r::ut::TextWriterBase<wchar_t>* tw);
void func_8012930C(nw4r::ut::TextWriterBase<wchar_t>* tw, float v);
void func_80129430(nw4r::ut::TextWriterBase<wchar_t>* tw, float v);
float func_80129564(nw4r::ut::TextWriterBase<wchar_t>* tw);
float func_801299D4(nw4r::ut::TextWriterBase<wchar_t>* tw);
// Same nw4r-pointer-validation accessor family (GetScale-ish / GetCharSpace
// / font-size store wrappers; unmangled retail names).
float func_80129AEC(nw4r::ut::TextWriterBase<wchar_t>* tw);
float func_80129C04(nw4r::ut::TextWriterBase<wchar_t>* tw);
float func_8012B328(nw4r::ut::TextWriterBase<wchar_t>* tw);
void func_80129E20(nw4r::ut::TextWriterBase<wchar_t>* tw, u32 value);
void func_8012B204(nw4r::ut::TextWriterBase<wchar_t>* tw, float v);
void func_80129F3C(nw4r::ut::TextWriterBase<wchar_t>* tw, float x, float y);
// Tag-param writer: split the arg string on ':', map the first token to a
// tag index, and init/copy a 1-tag-param block (numeric value when a second
// token exists, else the fixed f32 fallback). Returns the r4 arg.
void* func_80128C6C(void* unused, void* ret, wchar_t* str, TagParam* dst);
// Text-position measurement for the tag-writer position clamp
// (func_80127BF4): out = (x, y) computed from the message context + string.
// noinline: the retail kept this as a real call (0x384-byte body); the local
// stub body must not be folded into func_80127BF4's call site.
void func_80125D00(f32* out, CTagMsgView* msg, wchar_t* buf);
// Tag-writer position clamp: copy the message into a local buffer, measure
// it, clamp/snap the y position to the fixed step table, and write the
// resulting VEC2 to out->field_4C.
void func_80127BF4(void* unused, CTagMsgView* msg, CTagOutView* out);
int func_80129128(void* unused, TagCharContext* ctx, void* unused2,
                  TagWriterHolder* holder);
void* func_8012A388(void* unused, void* ret, wchar_t* str, TagParam* dst);
// Tag-writer cursor setter: validate the writer, then store mCursorPos.x/y
// (CharWriter +0x2C/+0x30). Same pointer validation as func_801291F4.
void func_8012A070(nw4r::ut::TextWriterBase<wchar_t>* tw, float x, float y);
// Tag-writer helper: max line width + font height of a string (uses the
// TextBox's font height getter and char-width getter).
void func_80127D20(f32* out, void* unused, nw4r::lyt::TextBox* textbox,
                   const wchar_t* str);
// Color-tag writer: match tokens against the copied name table and pack the
// result block. Returns the position after the block.
u16* func_80128DA0(void* unused, u16* dst, wchar_t* str);
// Tag-code writer (code 8, name selection): map the string to a value 1..9.
void* func_8012A224(void* unused, void* ret, wchar_t* str, TagParam* dst);
// Tag-writer line layout: measure two adjacent strings from the tag buffer
// into a scratch writer, advance the buffer pointer, return 2.
int func_8012B070(void* unused, TagLineOutView* out, void* unused2,
                  TagWriterHolder* holder);
u16* func_8012AF90(void* unused, u16* dst, wchar_t* str);
u16* func_8012B944(void* unused, u16* dst, wchar_t* str);
// Tag-code writer (code 0xB): split the arg string on ':', map token 0 to a
// 1..2 tag id and tokens 1..4 to numeric values (tag 2 via the BDAT s16
// lookup chain), write the 6-u16 block, return the advanced position.
u16* func_8012A460(void* unused, u16* out, wchar_t* str);
u16* func_80128EF8(void* unused, u16* dst, wchar_t* str);
u16* func_80129008(void* unused, u16* dst, wchar_t* str);
// String splitter (code_80135FDC.cpp): split src on delim into outTokens,
// returns the token count.
int func_801365E4(u16* src, u16 delim, u16** outTokens);
// BDAT s16-keyed lookup (code_80135FDC.cpp): maps a key into the table pair
// to an s16 value (func_8012A460's tag-2 name chain).
s16 func_80136130(const void* a, const void* b, u8 key);
// Message pre-processor: copy the raw message, normalize/compact the buffer
// (CRLF strip + '<name=value>' tag dispatch), return the buffer.
u16* func_80125B58(CTagProcessorBase* self, const void* src, f32 a, f32 b,
                   u32 c);
// Talk-open text setup (func_80128740's callee): build the display string
// from the message buffer (or the caller's string) and lay out the pane.
// Tag-writer scratch-writer copy (retail 0x104-byte call; noinline keeps
// -ipa from folding it into the call sites).
void func_80129D1C(TagCopyBlock* dst, const TagCopyBlock* src);
// Color-name tag writer: map the uppercased string to a packed RGB value.
u16* func_8012AAA4(void* unused, u16* out, wchar_t* str);
// Color-tag writer: apply the source color to the CharWriter (alpha kept).
u32 func_8012AD40(void* unused, void* ret, TagColorArg* arg);
// Tag-writer: print a marker into a scratch writer and advance the cursor.
int func_8012968C(void* unused, void* unused2, TagWriterHolder* holder);
// Accumulated pane translate (code_80135FDC.cpp).
void func_801375A0(f32* out, nw4r::lyt::Pane* pane);

// func_8012615C's C-ABI imports: flat (or pre-mangled) retail names.
void* func_800BBC0C();  // talk-source getter (r3 = prior call's result)
void func_8004B9D4(void* w, int a, int b, int c, int d);  // icon display
u32 func_8009CF8C(u32 id);
void func_8009D018(u32 owner, u32 value);
void* func_800451D8(u32 cls, void* param);
void func_8013DB6C(u32 a, u32 b, u32 c, u32 d);
void func_8013BE38();
int code80135FDC_getByte_6405A();
int code80135FDC_getByte_6405B();
int code80135FDC_getWord_6405C();
int code80135FDC_getWord_64060();
void code80135FDC_setPair_6405C_64060(int a, int b);
// cf::CfGameManager controller-mode query. Retail name keeps the Fv suffix
// but the call passes -1 in r3 (same convention as CTalkWindow.hpp).
int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
// Local context-walk helpers (defined in CTagProcessor.cpp).
void* func_80127670(void* self);
u32 func_801276C8(const u32* a, const u32* b);
void* func_801276E0(void* self, int a);
const wchar_t* getContextStr(u8* self);
const wchar_t** getContextStrPtr(u8* self);
void copyVEC2(float* dst, const float* src);
}

// C++-linkage imports (retail emits the mangled forms).
void* func_800B708C(int id);  // func_800B708C__Fi (talk-source index lookup)
void func_80138078(u32 op);   // func_80138078__FUl (UI sound effect)

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

