#pragma once

/*
 * CKizunaTalkList.hpp
 * Kizuna (Friendship) Talk List manager.
 * Displays affinity talk messages between characters.
 *
 * Layout (size 0x1494):
 *   +0x00: IWorkEvent (vtable + virtual event handlers)
 *   +0x04: UnkClass_8045F564 - memory region for entries
 *   +0x14: mEntryCount (or part of header)
 *   +0x18: mUnknown18
 *   +0x1C: mUnknown1C
 *   +0x20: mUnknown20
 *   +0x24: mUnknown24
 *   +0x28: mUnknown28
 *   +0x2C: mCursor - CCur18 cursor widget (0x18 bytes)
 *   +0x44: mScrollBar - CScrollBar widget (0x40 bytes)
 *   +0x84: mState84
 *   +0x85: mState85 (state machine progression)
 *   +0x86: mUnknown86
 *   +0x87: mNeedsRebuild - 1 = needs (re)build, 0 = current
 *   +0x88: mUnknown88
 *   +0x8A: mUnknown8A (short)
 *   +0x8C: mEntryArray - array of talk entries (0x1408 bytes)
 */

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"

// Forward declare with C linkage for friend
class CKizunaTalkList;
extern "C" u8 func_8027355C(CKizunaTalkList* self);

// Talk list entry (0x14 bytes each, 256 max)
struct TalkListEntry {
    u32 field_00;   // 0x00
    u32 field_04;   // 0x04
    u32 field_08;   // 0x08
    u32 field_0C;   // 0x0C
    s16 field_10;   // 0x10
    s8 field_12;    // 0x12, initialised to -1
    u8 field_13;    // 0x13
};

// Array container holding up to 256 entries
struct TalkListEntryArray {
    TalkListEntry mEntries[256]; // 0x000-0x13FF
    u8 mCount;                   // 0x1400
    u32 mParent;                 // 0x1404
};

class CKizunaTalkList : public IWorkEvent {
    friend u8 func_8027355C(CKizunaTalkList* self);
private:
    /* 0x04 */ u8 _pad04[0x10];     // UnkClass_8045F564 (stub)
    /* 0x14 */ u32 mEntryCount;     // number of valid entries
    /* 0x18 */ u32 mUnknown18;
    /* 0x1C */ u32 mUnknown1C;
    /* 0x20 */ u32 mUnknown20;
    /* 0x24 */ u32 mUnknown24;
    /* 0x28 */ u32 mUnknown28;
    /* 0x2C */ u8 _pad2C[0x18];     // CCur18 cursor (stub, 0x18 bytes)
    /* 0x44 */ u8 _pad44[0x40];     // CScrollBar (stub, 0x40 bytes)
    /* 0x84 */ u8 mState84;
    /* 0x85 */ u8 mState85;         // state machine progression flag
    /* 0x86 */ u8 mUnknown86;
    /* 0x87 */ u8 mNeedsRebuild;    // 1 = pending rebuild, 0 = built
    /* 0x88 */ u8 mUnknown88;
    /* 0x89 */ u8 _pad89;           // padding
    /* 0x8A */ u16 mUnknown8A;
    /* 0x8C */ TalkListEntryArray mEntryArray;
};


