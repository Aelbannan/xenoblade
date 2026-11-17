#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"

class CFileHandle;
class CEventFile;

// Mirror of nw4r::lyt::Pane's size fields (mSize @ +0x4C: width, height).
// nw4r hides these members (protected); used for raw field access in matching.
struct PaneSizeRegion {
    f32 width;   // +0x4C
    f32 height;  // +0x50
};

// Whole-pane view so the size fields land at their real offsets (+0x4C/+0x50).
struct CSortMenuPaneSizeView {
    u8 _00[0x4c];
    f32 width;
    f32 height;
};

// Interface view of the font object returned by CDeviceFont::getFontInfo.
// Slot layout mirrors the retail vtable: destructor pair (+0x00/+0x04), six
// reserved slots (+0x08..+0x1c), then the resource getter at +0x24. Declared
// only (never defined here) so no vtable/RTTI is emitted into this TU.
class CDeviceFontItf {
public:
    virtual ~CDeviceFontItf();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual u32 getResource();  // +0x24
};

/* Pop-up sort-order list (up to 32 entries, 5 visible rows) with its own
layout archive and scroll bar. Receives the archive through
IWorkEvent::OnFileEvent (retail __RTTI__9CSortMenu, IWorkEvent base list).

mState: 0 closed, 1 opening, 2 open-info anim, 3 open/idle, 4 closing-info,
5 closing. */
class CSortMenu : public IWorkEvent {
public:
    UnkClass_8045F564 mMemRegion;               // +0x04
    CFileHandle* mFileHandle;                   // +0x14
    nw4r::lyt::ArcResourceAccessor* mArcResAcc; // +0x18
    nw4r::lyt::Layout* mpLayout;                // +0x1C
    nw4r::lyt::AnimTransform* mpAnimTrans0;     // +0x20 open/close
    nw4r::lyt::AnimTransform* mpAnimTrans1;     // +0x24 info in/out
    u8 mOpen;                                   // +0x28 visible / running
    u8 mLoaded;                                 // +0x29 layout built
    u8 mState;                                  // +0x2A
    u8 mIdle;                                   // +0x2B accepts input
    CScrollBar mScrollBar;                      // +0x2C
    const char* mEntries[32];                   // +0x6C
    u8 mCount;                                  // +0xEC
    u8 mCursor;                                 // +0xED visible row
    u8 mTop;                                    // +0xEE first visible entry

    CSortMenu();
    virtual ~CSortMenu();

    void Init();
    void Move();
    void Draw(nw4r::lyt::DrawInfo* drawInfo);
    void Term();
    int IsReady();
    int IsOpen();
    int IsIdle();
    void Open();
    void Close();
    void SetPos(const nw4r::math::VEC3* pos);
    nw4r::math::VEC3 GetCursorPos();
    void Clear();
    void Add(const char* entry);
    void SetSelect(u8 select);
    void CursorUp();
    void CursorDown();
    void PageUp();
    void PageDown();
    u8 GetSelect();
    int GetCursor();
    int GetTop();
    void CalcSelect(int category, u8* outCursor, u8* outTop);
    void StateOpen();
    void StateOpenInfo();
    void StateCloseInfo();
    void StateClose();
    void Refresh();

    virtual bool OnFileEvent(CEventFile* pEventFile);
};

