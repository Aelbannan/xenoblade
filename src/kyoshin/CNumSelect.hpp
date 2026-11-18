#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"

namespace nw4r {
namespace lyt {
class Layout;
class DrawInfo;
class AnimTransform;
class ArcResourceAccessor;
}
}

class CFileHandle;
class CEventFile;

/* Quantity selector popup used by the shop/exchange item lists (retail RTTI
 * "CNumSelect", base list IWorkEvent; vtable slot +0x10 = OnFileEvent). */
class CNumSelect : public IWorkEvent {
public:
    CNumSelect(u8 region);
    virtual ~CNumSelect();

    void LoadFiles();
    void UpdateStates();
    void DrawLayout(nw4r::lyt::DrawInfo* drawInfo);
    void UnloadFiles();
    int IsLoaded();
    int IsOpen();
    int IsIdle();
    void SetItemName(char* name);
    void SetCount(u8 count);
    void SetTotal(int value);
    void Open();
    void Close();
    void CountUp();
    void CountDown();
    void SetArrowVisible(bool visible);
    void UpdateOpen();
    void UpdateClose();
    void UpdateCountUp();
    void UpdateCountDown();

    virtual bool OnFileEvent(CEventFile* evt);

    /* 0x04 */ UnkClass_8045F564 mMemRegion;
    /* 0x14 */ CFileHandle* mFileHandle;
    /* 0x18 */ nw4r::lyt::ArcResourceAccessor* mArcResAcc;
    /* 0x1C */ nw4r::lyt::Layout* mpLayout;
    /* 0x20 */ nw4r::lyt::AnimTransform* mAnimIn;
    /* 0x24 */ nw4r::lyt::AnimTransform* mAnimLoop;
    /* 0x28 */ nw4r::lyt::AnimTransform* mAnimOn;
    /* 0x2C */ u8 mOpen;
    /* 0x2D */ u8 mLoaded;
    /* 0x2E */ u8 mIdle;
    /* 0x2F */ u8 mState;
    /* 0x30 */ u8 mRegion;
};
