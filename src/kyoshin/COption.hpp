#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CSysWin.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CEventFile.hpp"

class CFileHandle;

/* Option-row highlight cursor (retail RTTI "CCur19"; table overrides
   initLayout at +0x08 and checkDeactivate at +0x14). */
class CCur19 : public CBaseCur {
public:
    CCur19(nw4r::lyt::ArcResourceAccessor* = nullptr);
    ~CCur19();
    virtual void initLayout();
    void showPaneA();
    void showPaneB();
    virtual void checkDeactivate();
};

/* Settings list widget embedded in CMenuOption (retail RTTI "COption",
   IWorkEvent base). Only the class shape is recovered; the member bodies
   other than the ctor/dtor and the trivial getters are still retail asm. */
class COption : public IWorkEvent {
public:
    COption(u8 kind);
    virtual ~COption();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    void rebuildWidgets();
    void tickState();
    void draw(nw4r::lyt::DrawInfo* drawInfo);
    void teardown();
    int isWindowReady();
    int getLiveFlag();
    int getConfirmGate();
    int getSecondConfirm();
    void cursorUp();
    void cursorDown();
    void pageUp();
    void pageDown();
    void beginScrollSetup();
    void confirmSelection();
    void handleLeft();
    void handleRightAccept(int accept);
    void openConfigDialog();
    int getNavCode();
    void advanceState1To2();
    void advanceState2To3();
    void advanceState4To5();
    void advanceState5To0();
    void advanceState6To3();
    void tickConfigSync();
    void advanceState8To9();
    void advanceState10To3();
    void syncConfigAndRefresh();
    void refreshRows();
    u8 readConfigCell(u8 row);
    u8 readCurrentConfigCell();
    void writeConfigCell(u8 value);
    u8 getCellMax();
    void configIncrement();
    void applyConfig();
    void setupSubAnim20();
    void setupSubAnim24();
    void refreshCursorPanes();
    int isConfigDirty();

    UnkClass_8045F564 mMemRegion;                  // 0x04
    CFileHandle* mFileHandle;                      // 0x14
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;  // 0x18
    nw4r::lyt::Layout* mLayout;                    // 0x1C
    nw4r::lyt::AnimTransform* mAnimTransA;         // 0x20
    nw4r::lyt::AnimTransform* mAnimTransB;         // 0x24
    u8 field_0x28;                                 // 0x28
    u8 field_0x29;                                 // 0x29
    u8 field_0x2A;                                 // 0x2A
    u8 mLiveFlag;                                  // 0x2B
    s8 mRowTop;                                    // 0x2C
    s8 mRow;                                       // 0x2D
    u8 field_0x2E;                                 // 0x2E
    u8 field_0x2F;                                 // 0x2F
    u8 mConfirmGate;                               // 0x30
    u8 field_0x31;                                 // 0x31
    u8 mSecondConfirm;                             // 0x32
    u8 field_0x33;                                 // 0x33
    u8 mKind;                                      // 0x34 - window kind (ctor arg)
    CScrollBar mScrollBar;                         // 0x38
    CCur18 mCur78;                                 // 0x78
    CCur19 mCur90;                                 // 0x90
    CSysWin mSysWin;                               // 0xA8
    CCur18 mCurE4;                                 // 0xE4
    s32 field_0xFC;                                // 0xFC
    u32 field_0x100;                               // 0x100
};
