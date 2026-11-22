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

class CSplitFrame;

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

//size: 0x470
class CView : public CWorkThread, public CFontLayer {
public:
    CView(const char* pName, CWorkThread* pParent);
    virtual ~CView();
    
    DECL_WORKTHREAD_CREATE(CView);

    virtual void wkUpdate();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    // Virtuals appended after the CWorkThread/IWorkEvent slots land in the
    // CFontLayer secondary sub-vtable (retail: 0xAC..0xCC, see the retail
    // vtable at .data:0x8056B5E0+0xA0).
    virtual void CView_UnkVirtualFunc1();
    virtual void CView_UnkVirtualFunc2(CWorkThread* pThread);
    virtual bool detachRenderWork(CWorkThread* pThread);
    virtual bool CView_UnkVirtualFunc4(IWorkEvent* event);
    virtual bool CView_UnkVirtualFunc5();
    virtual bool CView_UnkVirtualFunc6();
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
    void updateViewRect(ml::CRect16* rect);
    bool enqueueContextMsg(u32 param);
    void sortChildViews();
    u8 getSplitFrameFlag();
    s32 checkSplitActive();
    s32 hasSplitViews();
    CView* updateViewState();

    static ml::CCol4 sFrameColor;
    static CView* sCurrentView;

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

    // Rect helpers: retail passes the target rect in r3 and the source view in
    // r4, so they are class-scope statics (no `this`).
    static void fitRectToView(ml::CRect& rect, CView* other);
    static void accumRectToView(ml::CRect& rect, CView* other);
    static void clipRectToView(ml::CRect& rect, CView* other);
    static void adjustRectForView(ml::CRect& rect, CView* other);
    static void applyRectToView(ml::CRect& rect, CView* other);
    static void insetRectToView(ml::CRect& rect, CView* other);

    //0x0: vtable 1
    //0x4-1C4: CWorkThread
    //0x1C4: vtable 2
    CViewRectDataCore mRectData; //0x1C8
    CViewFrame mFrame; //0x1DC
    reslist<WORK_ID> mWorkIDList; //0x238 (work ids attached to this view)
    reslist<IWorkEvent*> mEventList; //0x258 (events attached to this view)
    u32 unk278; //0x278
    u32 unk27C; //0x27C
    CMsgParam<10> mContextMsg; //0x280
    ml::FixStr<64> mName; //0x400
    ml::CVec4 unk444; //0x444
    u32 mGXCacheId; //0x454
    float mAlpha; //0x458
    CSplitFrame* unk45C; //0x45C split-frame layout attached to this view
    u32 unk460; //0x460
    s16 unk464;
    s16 unk466;
    s16 unk468;
    s16 unk46A;
    u8 unk46C[0x470 - 0x46C];
};
