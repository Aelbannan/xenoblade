#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CEventFile.hpp"

namespace nw4r { namespace lyt { class Layout; class AnimTransform; class DrawInfo; class ArcResourceAccessor; } }
struct CFileHandle;

/* Map selection UI widget (retail RTTI "CMapSel", IWorkEvent base). Shows a
   10-row list of the maps the player may jump to.

   State machine (mState):
     0 = hidden, 1 = opening, 2 = open-in animation, 3 = active,
     4 = closing, 5 = post-close cleanup */
class CMapSel : public IWorkEvent {
public:
    CMapSel();
    virtual ~CMapSel();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    void startLoad();
    void update();
    void draw(nw4r::lyt::DrawInfo* drawInfo);
    void term();
    int getVisibleFlag();
    int getInitFlag();
    void init();
    void beginClose();
    void moveUp();
    void moveDown();
    void movePageUp();
    void movePageDown();
    u8 getMapGridCell();
    void tickOpen();
    void tickOpenIn();
    void tickClose();
    void tickCloseEnd();
    void activate();
    void refreshGridText();
    void refreshImage();
    void loadSelectedMapResource();
    void requestImage();
    void updateCursor();

    UnkClass_8045F564 mMemRegion;                  // 0x04
    CFileHandle* mFileHandle;                      // 0x14 - layout archive
    CFileHandle* mFileHandle2;                     // 0x18 - map image
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;  // 0x1C
    void* mAllocatedMem;                           // 0x20 - loaded map image
    nw4r::lyt::Layout* mLayout;                    // 0x24
    nw4r::lyt::AnimTransform* mAnimTransform1;     // 0x28
    nw4r::lyt::AnimTransform* mAnimTransform2;     // 0x2C
    u8 mActive;                                    // 0x30
    u8 mState;                                     // 0x31
    u8 mVisible;                                   // 0x32
    u8 mInitFlag;                                  // 0x33
    CScrollBar mScrollBar;                         // 0x34
    CCur18 mCursor;                                // 0x74
    s8 mSelY;                                      // 0x8C
    s8 mSelX;                                      // 0x8D
    u8 mGrid[0x20];                                // 0x8E - selectable map ids
    u8 mCount;                                     // 0xAE
    u8 mImagePending;                              // 0xAF
    float mTimer;                                  // 0xB0 - image load delay
};
