#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
class CBaseCur;
#include <nw4r/lyt.h>

#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CSysWin.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CEventFile.hpp"

class CFileHandle;

/* Save-slot highlight cursor (retail RTTI "CSLCur"; vtable has a single
virtual, createLayout, and no destructor slot). */
class CSLCur {
public:
    CSLCur(nw4r::lyt::ArcResourceAccessor* accessor);
    ~CSLCur();
    virtual void createLayout();
    void update();
    void draw(nw4r::lyt::DrawInfo* drawInfo);
    void destroy();
    void setPos(const nw4r::math::VEC3* pos);
    void tickAnim();
    void bindPane();

    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x04
    nw4r::lyt::Layout* mLayout;                // 0x08
    nw4r::lyt::AnimTransform* mAnimLoop;       // 0x0C
    nw4r::lyt::AnimTransform* mAnimPush;       // 0x10
    u8 mVisible;                               // 0x14
    u8 mState;                                 // 0x15 - 0 loop, 1 push
};

/* "Now saving" sub-window layout (no vtable; CSaveLoad +0x110). */
class CSLSub {
public:
    CSLSub(nw4r::lyt::ArcResourceAccessor* accessor);
    ~CSLSub();
    void create();
    void update();
    void destroy();
    void setBusy();
    void tickIn();
    void tickOut();
    void bindPane();

    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x00
    nw4r::lyt::Layout* mLayout;                // 0x04
    nw4r::lyt::AnimTransform* mAnimTrans;      // 0x08
    u8 mState;                                 // 0x0C - 0 idle, 1 in, 2 shown, 3 out
    u8 mShown;                                 // 0x0D
    u8 mIdle;                                  // 0x0E
};

/* Save-slot record stored at +0x20 of each NAND header buffer. The
unknown bytes are real fields of the on-disk record whose meaning has not
been recovered yet. */
struct CSLSaveSlot {
    u8 mUnk00;                 // 0x00
    u8 mUnk01;                 // 0x01
    u8 mPriority02;            // 0x02
    u8 mPriority03;            // 0x03
    u16 mPriority04;           // 0x04
    u16 mPriority06;           // 0x06
    u8 mPriority08;            // 0x08
    u8 mPriority09;            // 0x09
    u16 mPlayTime;             // 0x0A
    u16 mMinutes;              // 0x0C
    u16 mStartPlace;           // 0x0E
    u8 mUnk10;                 // 0x10
    u8 mGender;                // 0x11
    u16 mUnk12;                // 0x12
    u32 mAwardRecord[7];       // 0x14
    u8 mUnk30[0x14];           // 0x30
    char mPlayerName[0x20];    // 0x44
    u16 mLevel;                // 0x64
    u8 mClassFlag;             // 0x66
    u8 mClassIndex;            // 0x67
    u8 mUnk68[0x18];           // 0x68
    u8 mThumbnail[0x9C00];     // 0x80
};

/* NAND header buffer: the first 0x20 bytes are the transfer header, then
 the save record. */
struct CSLBuffer {
    u8 mNandHeader[0x20];      // 0x00
    CSLSaveSlot mSlot;         // 0x20
};

/* Save-slot header buffers read through the NAND queue (no vtable;
CSaveLoad +0x13C). */
class CSLData {
public:
    CSLData();
    ~CSLData();
    void allocBufs();
    void freeBufs();
    void bindSlots();
    int isReady();
    CSLSaveSlot* getSlot(u8 index);
    static void onSlotRead(int, int index, u8 valid);

    CSLBuffer* mBuf[3]; // 0x00
    u8 mRead[3];     // 0x0C - header read finished
    u8 mValid[3];    // 0x0F - header valid
};

class CSaveLoad : public IWorkEvent {
public:
    CSaveLoad(u8 load, u8 arg);
    virtual ~CSaveLoad();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    void loadSaveData();
    void update();
    void draw(nw4r::lyt::DrawInfo* drawInfo);
    void reset();
    int isReady();
    int getIdle();
    int isBusy();
    void open();
    void close();
    void curUp();
    void curDown();
    void pageUp();
    void pageDown();
    void confirm();
    void cancel();
    int getDone();
    void openDel();
    u8 getHelpKind();
    void tickOpenA();
    void tickOpenB();
    void tickCloseB();
    void tickCloseA();
    void tickConf();
    void execOp();
    void finish();
    void tickWait();
    void tickWindow();
    void tickSlot();
    void enAnimA();
    void enAnimB();
    void refresh();
    void renderSlots();
    void bestSlot();
    static void onFile(int, int, int completed, int msgKind);

    UnkClass_8045F564 mMemRegion;                  // 0x04
    CFileHandle* mFileHandle;                      // 0x14
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;  // 0x18
    nw4r::lyt::Layout* mLayout;                    // 0x1C
    nw4r::lyt::AnimTransform* mAnimTransA;         // 0x20
    nw4r::lyt::AnimTransform* mAnimTransB;         // 0x24
    CSLCur mCur;                                   // 0x28
    CCur18 mCur40;                                 // 0x40
    CScrollBar mScrollBar;                         // 0x58
    CSysWin mSysWin98;                             // 0x98 - confirm window
    CSysWin mSysWinD4;                             // 0xD4 - result window
    CSLSub mSub;                                   // 0x110
    u8 mField120;                                  // 0x120
    u8 mField121;                                  // 0x121 - state
    u8 mField122;                                  // 0x122
    u8 mField123;                                  // 0x123
    u8 mField124;                                  // 0x124 - cursor row
    s16 mField126;                                 // 0x126 - scroll top
    u8 mField128;                                  // 0x128
    u8 mField129;                                  // 0x129 - load mode
    u8 mField12A;                                  // 0x12A
    u8 mField12B;                                  // 0x12B
    u8 mField12C;                                  // 0x12C
    u8 mField12D;                                  // 0x12D
    u8 mField12E;                                  // 0x12E
    u8 mField12F;                                  // 0x12F
    void* mSlotTex[3];                             // 0x130 - per-slot thumbnail buffers
    CSLData mData;                                 // 0x13C
};

namespace cf {

struct CfAward {
    void* mVtbl;       // +0x00 - vtable pointer (points to lbl_eu_80538858 + 8)
    void* mSecondBase; // +0x04 - second vtable base (points to lbl_eu_80538858)

    // No virtual functions - vtable managed manually.
    // The destructor is a regular member function (not virtual).
    // MWCC generates the delete flag handling automatically.
    ~CfAward();
    void processAward(int val);
    // getAwardState is implemented as an extern "C" function (unmangled retail symbol)
};

} // namespace cf

extern "C" u32 getSaveLoadOp(void);
