#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "kyoshin/CScrollBar.hpp"       // CScrollBar (real class: ctor/dtor emitted by the compiler)
#include "kyoshin/CBaseCur.hpp"         // CCur18 cursor member
#include "monolib/device/CDeviceFont.hpp" // IDeviceFontInfo / CDeviceFont
#include "kyoshin/code_80135FDC.hpp"  // AnimRewindFrame, advanceAnimTransform, BdatTouchStringCell, LayoutSetTextBoxFmtValue
#include "kyoshin/cf/voice/cvsys/CVS_THREAD_REVIVE.hpp"  // CVoiceHandle (TutorialList_PlayTutorialVoice)

// UI sound: retail symbol is the C++-mangled playUISound__FUl.
void playUISound(u32);

// Minimal CSortMenu: the full class (CSortMenu.hpp) cannot be included here
// because its extern "C" UnkClass_8045F564 stubs collide with the real
// UnkClass header. Field layout matches CSortMenu.hpp (0xF0 bytes). The retail
// ctor symbol is the unmangled __ct__CSortMenu (called from the inline ctor);
// the dtor is the out-of-line retail __dt__9CSortMenuFv.
extern "C" void __ct__CSortMenu(void* self);
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* self, const void* src);

class CSortMenu {
public:
    CSortMenu() { __ct__CSortMenu(this); }
    ~CSortMenu();

    void* mVtbl;                // 0x00
    u8 mRegion[0x10];           // 0x04: UnkClass_8045F564
    void* mFileHandle;          // 0x14
    void* mArcResAcc;           // 0x18
    void* mLayout;              // 0x1C
    void* mAnim0;               // 0x20
    void* mAnim1;               // 0x24
    u8 mFlag28;                 // 0x28
    u8 mFlag29;                 // 0x29
    u8 mFlag2A;                 // 0x2A
    u8 mFlag2B;                 // 0x2B
    u8 mScrollBar[0x40];        // 0x2C: CScrollBar body
    u32 mArray[32];             // 0x6C
    u8 mCount;                  // 0xEC
    u8 mPage;                   // 0xED
    u8 mSubPage;                // 0xEE
    u8 mPad;                    // 0xEF
};

// Tutorial id table: the ids of every unlocked tutorial in display order plus
// the page/scroll split of the selected entry.
struct CTutorialIdTable {
    u16 mIds[0x80];   // 0x00: collected tutorial ids
    u16 mCount;       // 0x100: number of entries
    s16 mSelRow;      // 0x102: row of the selected entry within its page
    s16 mScrollBase;  // 0x104: scroll offset of the selected entry's page

    CTutorialIdTable();
    void Rebuild(u16 target, int filter);
    u16 GetEntryIdAt(u16 index);
};

/* CTutorialList -- tutorial list widget (IWorkEvent: receives archive
   completion through OnFileEvent). Retail layout (ctor 802AF55C):
   +0x00 vptr | +0x04 UnkClass_8045F564 | +0x14 two CFileHandle*, accessor,
   layout, two AnimTransform* | +0x2C CCur18 | +0x44 CScrollBar (0x40)
   | +0x84 CSortMenu (0xF0) | +0x174 control bytes | +0x180 id table
   | +0x288 total size (the embedded object in CMenuTutorialList). */
class CTutorialList : public IWorkEvent {
public:
    CTutorialList(u16 count);
    virtual ~CTutorialList();
    virtual bool OnFileEvent(CEventFile* event);

    void TutorialList_LoadLayoutFiles();
    void TutorialList_UpdatePerFrame();
    void TutorialList_DrawVisible(nw4r::lyt::DrawInfo* drawInfo);
    void TutorialList_ReleaseResources();
    int TutorialList_IsListVisible();
    int TutorialList_IsInitialized();
    void TutorialList_OpenListInit();
    void TutorialList_BeginClose();
    void TutorialList_PageUpIdle();
    void TutorialList_PageUpWrap();
    void TutorialList_PageDownStep();
    void TutorialList_PageDownBlock();
    u16 TutorialList_GetSelectedEntryId();
    void TutorialList_RefreshRowTexts();
    void TutorialList_ConfirmOpenMenu();
    int TutorialList_IsSortMenuActive();
    void TutorialList_AdvanceSelection(int arg);
    void TutorialList_RebuildAfterLoad();
    int TutorialList_GetMenuStateCode();
    void TutorialList_OnOpenAnimDone();
    void TutorialList_OnIdleAnimDone();
    void TutorialList_OnCloseAnimRewind();
    void TutorialList_OnShutdownAnimRewind();
    void TutorialList_EnableOpenAnims();
    void TutorialList_EnableCloseAnims();
    void TutorialList_UpdateRowTexts();
    void TutorialList_MoveCursorToRow();
    void TutorialList_RebuildSortEntries();
    void TutorialList_UpdateTitleText();
    void TutorialList_OpenAfterFilesReady();

    // Fields are public: the owning menu embeds the widget by value and the
    // retail callers poke the state bytes directly.
    UnkClass_8045F564 mRegion;            // 0x04: layout build region (0x10000)
    CFileHandle* mFileHandle0;            // 0x14: list layout arc
    CFileHandle* mFileHandle1;            // 0x18: common bdat archive
    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x1C
    nw4r::lyt::Layout* mLayout;           // 0x20
    nw4r::lyt::AnimTransform* mAnim24;    // 0x24
    nw4r::lyt::AnimTransform* mAnim28;    // 0x28
    CCur18 mCursor;                       // 0x2C
    CScrollBar mScrollBar;                // 0x44
    CSortMenu mSortMenu;         // 0x84: CSortMenu (0xF0)
    u8 mField174;                         // 0x174: run/visible gate byte
    u8 mState175;                         // 0x175: anim/state byte
    u8 mField176;                         // 0x176: visible-selection byte
    u8 mInitialized;                      // 0x177: non-zero after construction
    s8 mField178;                         // 0x178: sign-extended page byte
    u16 mField17A;                        // 0x17A: selection id (halfword)
    u16 mField17C;                        // 0x17C: count/limit (halfword)
    s8 mField17E;                         // 0x17E: sign-extended counter base
    CTutorialIdTable mIdTable;            // 0x180: id table (0x106 bytes, padded to 0x288 total)
};
