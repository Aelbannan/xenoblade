#pragma once

#include <types.h>
#include "monolib/work.hpp"
#include "monolib/math.hpp"
#include "monolib/util.hpp"
#include "monolib/core/CViewFrame.hpp"
#include "monolib/core/CViewRectData.hpp"

//size: 0x4
class CFontLayer {
    CFontLayer();
    virtual ~CFontLayer();
};

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
        getFrame2ViewOffset(tempRect, &unk1DC);

        rect.mPos.x = tempRect.mPos.x + unk1DC.unk54;
        rect.mPos.y = tempRect.mPos.y + unk1DC.unk56;
        rect.mSize.x = unk1C8.unk0;
        rect.mSize.y = unk1C8.unk2;
    }

    //0x0: vtable 1
    //0x4-1C4: CWorkThread
    //0x1C4: vtable 2
    CViewRectDataCore unk1C8;
    CViewFrame unk1DC;
    reslist<CWorkThread*> unk238; //0x238 - checked empty() in wkUpdate
    u8 unk258[0x278 - 0x258];
    u32 unk278;
    u8 unk27C[0x284 - 0x27C];
    u8 mContextRing[0x3EC - 0x284];
    void* mContextRingBase;
    u32 unk3F0;
    u32 mContextRingWriteIndex;
    u32 mContextRingCapacity;
    u32 unk3FC;
    ml::FixStr<64> mName;
    ml::CVec4 unk444;
    u8 unk454[0x45C - 0x454];
    void* unk45C;
    u32 unk460;
    s16 unk464;
    s16 unk466;
    s16 unk468;
    s16 unk46A;
    u8 unk46C[0x470 - 0x46C];
};
