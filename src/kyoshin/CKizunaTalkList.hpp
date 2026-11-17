#pragma once

/*
 * CKizunaTalkList.hpp
 * Kizuna (Friendship) Talk List manager.
 * Displays affinity talk messages between characters.
 *
 * Layout (size 0x1494):
 *   +0x00: IWorkEvent (vtable + virtual event handlers)
 *   +0x04: UnkClass_8045F564 - memory region for entries
 *   +0x14: mFileHandle - talk-list layout archive handle
 *   +0x18: mBdatHandle - shared BDAT archive handle
 *   +0x1C: mAccessor - layout ArcResourceAccessor
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
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CScrollBar.hpp"

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"

// Talk list entry (0x14 bytes each, 256 max)
struct TalkListEntry {
    u32 field_00;   // 0x00
    u32 field_04;   // 0x04
    u32 field_08;   // 0x08
    u32 field_0C;   // 0x0C
    s16 field_10;   // 0x10
    s8 field_12;    // 0x12, initialised to -1
    u8 field_13;    // 0x13

    TalkListEntry() {
        field_00 = 0;
        field_04 = 0;
        field_08 = 0;
        field_0C = 0;
        field_10 = 0;
        field_12 = -1;
        field_13 = 0;
    }
    ~TalkListEntry() {}
};

// Array container holding up to 256 entries; mParent is the layout's
// resource accessor used to resolve the entry icons.
struct TalkListEntryArray {
    TalkListEntry mEntries[256];                  // 0x000-0x13FF
    u8 mCount;                                    // 0x1400
    nw4r::lyt::ArcResourceAccessor* mParent;      // 0x1404

    TalkListEntryArray(nw4r::lyt::ArcResourceAccessor* parent);
};

// Kizuna (affinity) talk list widget; receives archive completion through IWorkEvent.
class CKizunaTalkList : public IWorkEvent {
public:
    /* 0x04 */ UnkClass_8045F564 mMemRegion;
    /* 0x14 */ CFileHandle* mFileHandle;            // talk-list layout archive
    /* 0x18 */ CFileHandle* mBdatHandle;            // shared BDAT archive
    /* 0x1C */ nw4r::lyt::ArcResourceAccessor* mAccessor;
    /* 0x20 */ nw4r::lyt::Layout* mpLayout20;
    /* 0x24 */ nw4r::lyt::AnimTransform* mpAnim24;
    /* 0x28 */ nw4r::lyt::AnimTransform* mpAnim28;
    /* 0x2C */ CCur18 mCursor;
    /* 0x44 */ CScrollBar mScrollBar;
    /* 0x84 */ u8 mState84;
    /* 0x85 */ u8 mState85;         // state machine progression flag
    /* 0x86 */ bool mUnknown86;   // talk data built and visible
    /* 0x87 */ u8 mNeedsRebuild;    // 1 = pending rebuild, 0 = built
    u8 needsRebuild() const { return mNeedsRebuild; }
    /* 0x88 */ s8 mUnknown88;
    /* 0x8A */ s16 mUnknown8A;
    /* 0x8C */ TalkListEntryArray mEntryArray;

    CKizunaTalkList(int);
    virtual ~CKizunaTalkList();
    virtual bool OnFileEvent(CEventFile* pEventFile);
    void hideTalkList();
    void scrollTalkListUp();
    void scrollTalkListDown();
    void pageTalkListUp();
    void pageTalkListDown();
    void initTalkList();
    int needsTalkRebuild();
    void teardownTalkList();
    bool hasVisibleTalks();
    void showTalkList();
    void drawTalkList(nw4r::lyt::DrawInfo* pDrawInfo);
    void updateTalkList();
};
