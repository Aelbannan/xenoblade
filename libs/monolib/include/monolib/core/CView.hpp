#pragma once

#include <types.h>
#include "monolib/work.hpp"
#include "monolib/math.hpp"
#include "monolib/util.hpp"
#include "monolib/core/CViewFrame.hpp"
#include "monolib/core/CViewRectData.hpp"
#include "monolib/core/CFontLayer.hpp"

// Context ring slot written by setCurrent (0x24 bytes).
struct CViewContextRingEntry {
    u32 tag;
    u8 payload[0x1C];
    s16 unk54;
    u8 unk56Hi;
    u8 pad;
};

struct CView_UnkStruct1 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
};

// POD reslist header (size 0x20) so CView::__ct__ can init after ViewFrame.
struct CViewResList {
    void* vtable;
    void* mStartNodePtr;
    void* mSentinelNext;
    void* mSentinelPrev;
    void* mSentinelItem;
    void* mList;
    int mCapacity;
    u8 unk1C;
    u8 pad[3];

    bool empty() const {
        return *(void**)mStartNodePtr == mStartNodePtr;
    }
};

//size: 0x470
class CView : public CWorkThread, public CFontLayer {
public:
    CView(const char* pName, CWorkThread* pParent);
    virtual ~CView();
    
    DECL_WORKTHREAD_CREATE(CView);

    virtual void CView_UnkVirtualFunc0();
    virtual void CView_UnkVirtualFunc1();
    virtual void detachRenderWork(CWorkThread* pThread);
    virtual void wkUpdate();
    virtual void CView_UnkVirtualFunc3();
    virtual void CView_UnkVirtualFunc4();
    virtual void CView_UnkVirtualFunc5();
    virtual void CView_UnkVirtualFunc6();
    virtual void CView_UnkVirtualFunc7();
    virtual void CView_UnkVirtualFunc8();
    virtual void CView_UnkVirtualFunc9();

    static void setDefaultFrameColor(const ml::CCol4& color);
    static CView* getCurrentView();
    void setRect(const ml::CRect16& rect);
    bool attachRenderWork(CWorkThread* pThread);
    void setDisp(bool r4, bool r5);
    s16 getSplitLine();
    void setSplitLine(s16 line);
    void setCurrent();
    bool hasCurrent() const;
    void updateMsg();
    void renderView();

    static ml::CCol4 sFrameColor;

    //Tries to cast the given thread to CView.
    static CView* convertToView(CWorkThread* pThread) {
        if(pThread == nullptr){
            return nullptr;
        }

        int type = pThread->mType;

        //Check if the thread's type is in the CView range
        if(THREAD_CVIEW > type || type >= THREAD_CVIEW_MAX) return nullptr;
        return static_cast<CView*>(pThread);
    }

    
    void getRect(ml::CRect16& rect){
        ml::CRect16 tempRect;
        getFrame2ViewOffset(tempRect, &mFrame);

        rect.mPos.x = tempRect.mPos.x + mFrame.mContentX;
        rect.mPos.y = tempRect.mPos.y + mFrame.mContentY;
        rect.mSize.x = mRectData.mViewSize.x;
        rect.mSize.y = mRectData.mViewSize.y;
    }

    //0x0: vtable 1
    //0x4-1C4: CWorkThread
    //0x1C4: vtable 2
    CViewRectDataCore mRectData; //0x1C8
    CViewFrame mFrame; //0x1DC
    CViewResList unk238; //0x238 reslist<WORK_ID>
    CViewResList unk258; //0x258 reslist<IWorkEvent*>
    u32 unk278; //0x278
    u32 unk27C; //0x27C
    u32 mContextMsgVtable; //0x280
    u8 mContextRing[0x3EC - 0x284]; //0x284
    void* mContextRingBase; //0x3EC
    u32 unk3F0; //0x3F0
    u32 mContextRingWriteIndex; //0x3F4
    u32 mContextRingCapacity; //0x3F8
    u32 unk3FC; //0x3FC
    ml::FixStr<64> mName; //0x400
    ml::CVec4 unk444; //0x444
    u32 mGXCacheId; //0x454
    float mAlpha; //0x458
    void* unk45C; //0x45C
    u32 unk460; //0x460
    s16 unk464;
    s16 unk466;
    s16 unk468;
    s16 unk46A;
    u8 unk46C[0x470 - 0x46C];
};
