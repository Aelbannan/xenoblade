#pragma once

#include <types.h>

#include <nw4r/ut/ut_TagProcessorBase.h>
#include <nw4r/ut/ut_Rect.h>

// sdata2 float constant used by the tag-param init functions
// (retail: .sdata2:0x806671F0).
extern const f32 lbl_eu_806671F0;

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

namespace nw4r {
namespace ut {

/* nw4r::ut::Color as seen by this TU: retail copy-assign is byte-wise,
 * so the class carries four plain u8 components (no u32 shortcut).
 * operator= is defined out-of-line in CTagProcessor.cpp so it is emitted
 * as the standalone retail symbol. */
struct Color {
    u8 r;  // +0x00
    u8 g;  // +0x01
    u8 b;  // +0x02
    u8 a;  // +0x03

    Color& operator=(const Color& rhs);
};

}  // namespace ut
}  // namespace nw4r

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
};

class CTagProcessorBase {
public:
    CTagProcessorBase();
    virtual ~CTagProcessorBase();

    // TODO: add fields
};

class CTagProcessor {
public:
    CTagProcessor();
    virtual ~CTagProcessor();

    // TODO: add fields
};

class CTagProcessorSE {
public:
    CTagProcessorSE();
    virtual ~CTagProcessorSE();

    // TODO: add fields
};

