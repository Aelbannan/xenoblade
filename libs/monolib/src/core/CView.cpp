#include "monolib/core.hpp"
#include "monolib/core/CViewRectData.hpp"
#include "monolib/device.hpp"
#include "monolib/work/CWorkThread.hpp"

//Not yet decompiled (monolib/src/core/CSplitFrame.cpp); only its virtual destructor is
//referenced here (CView::wkUpdate deletes it once it goes inactive), so a minimal stub with a
//single virtual function is enough to reproduce the retail vtable-slot call.
class CSplitFrame {
public:
    virtual ~CSplitFrame();
};

extern "C" {
CView* lbl_eu_806655C8;
CViewRoot* getInstance__9CViewRootFv();
CView* getCurrent__9CViewRootFv();
void invalidCurrent__9CViewRootFP5CView(CView* view);
GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
CView* getView1__11CSplitFrameFv(void* splitFrame);
CView* getView2__11CSplitFrameFv(void* splitFrame);
s16 getSplitLine__11CSplitFrameFv(void* splitFrame);
void setSplitLine__11CSplitFrameFs(void* splitFrame, s16 line);
bool isActive__11CSplitFrameFv(void* splitFrame);
void apply__11CSplitFrameFv(void* splitFrame);
void setCurrent__9CViewRootFP5CView(CView* view);
CWorkThread* getWorkThread__9CWorkUtilFUl(u32 workId);
void getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(ml::CRect16* rect, CViewFrame* frame);
void func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(CViewRectDataCore* data, const ml::CPnt16& size);
void func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(CViewRectDataCore* data, const ml::CPnt16& maxSize);
void __ct__CViewFrame(CViewFrame* frame);
void func_8043FC60__10CViewFrameFUl(CViewFrame* frame, u32 owner);
u32 func_8044BE2C__8CGXCacheFv();
void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, mtl::ALLOC_HANDLE handle);
void func_8043E010__5CViewFv(CView* view);
CView* getFullScreenView__9CViewRootFv();
CProc* pssGetRoot__5CProcFP5CProc(CProc* proc);
void func_8044A578__8CGXCacheFv(CGXCache* cache, ml::CCol4* color, int flag);
void getScissorRect1__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(ml::CRect16* out, void* split);
void getScissorRect2__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(ml::CRect16* out, void* split);
void func_80442B54__9CViewRootFPvPv(void* viewRect, void* scissorRect, void* insetRect);
void func_80442C68__9CViewRootFv();
void func_8044BE38__8CGXCacheFv(CGXCache* cache);
void func_80442DA8__9CViewRootFv();
void fontFlush__10CFontLayerFi(CFontLayer* layer, int flag);
void render__10CViewFrameFv(CViewFrame* frame);
// Incomplete arrays force lis/addi (not SDA lwz) -- same as CViewRoot::create.
char lbl_eu_8056B298[];
char lbl_eu_8056B280[];
char lbl_eu_8056B6F0[];
char lbl_eu_8056B6D8[];
char lbl_eu_8056B6CC[];
ml::CCol4 lbl_8065A0C8;
}

ml::CCol4 CView::sFrameColor;

void CView::setDefaultFrameColor(const ml::CCol4& color) {
    sFrameColor = color;
}

CView* CView::getCurrentView() {
    return lbl_eu_806655C8;
}

// Enqueue tag 6 onto the context ring (CMsgParam<10>). High-level enqueue
// restores signed ring index + stwux; retail -0x40 spill interleave is closed
// Chaitin near-miss — keep iterating in high-level C. behaviour:view-set-current-ring.
void CView::setCurrent() {
    CMsgParam<10>& messages =
        *reinterpret_cast<CMsgParam<10>*>(&mContextMsgVtable);
    messages.enqueue(6);
}

void CView::setRect(const ml::CRect16& rect) {
    // All CPnt16 so MWCC packs 4-byte homes (CRect16 aligns to 8 and leaves a
    // hole at sp+0xC). Contiguous pos+size pairs cast to CRect16* for the helper.
    // Decl order (first = higher addr): normalSize@0x18 ... modeSize@0x8.
    // modeSize as u16 pair so fbWidth/efbHeight loads stay lhz (not lha via s16).
    ml::CPnt16 normalSize;
    ml::CPnt16 normalPos;
    ml::CPnt16 splitSize;
    ml::CPnt16 splitPos;
    struct {
        u16 x;
        u16 y;
    } modeSize;
    CWorkThread* parentSnap;
    CWorkThread* sourceParent;
    GXRenderModeObj* renderMode;

    if ((unk278 & 1) != 0) {
        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
            (ml::CRect16*)&splitPos, &mFrame);
        splitSize.x = -splitPos.x;
        parentSnap = mParent;
        splitSize.y = -splitPos.y;
        *(u32*)&mFrame.mContentX = *(u32*)&splitSize;

        if (getInstance__9CViewRootFv() == parentSnap) {
            sourceParent = nullptr;
        } else {
            sourceParent = mParent;
            if (sourceParent != nullptr) {
                goto setRect_parent_typed;
            }
            sourceParent = nullptr;
            goto setRect_parent_done;
        setRect_parent_typed:
            {
                int parentType = sourceParent->mType;
                if (THREAD_CVIEW > parentType || parentType >= THREAD_CVIEW_MAX) {
                    sourceParent = nullptr;
                }
            }
        setRect_parent_done:;
        }

        if (sourceParent != nullptr) {
            func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(
                &mRectData, *(const ml::CPnt16*)((u8*)sourceParent + 0x1CC));
        } else {
            {
                u16 modeHeight;
                u16 modeWidth;

                renderMode = getRenderModeObj__9CDeviceVIFv();
                modeHeight = renderMode->efbHeight;
                renderMode = getRenderModeObj__9CDeviceVIFv();
                modeWidth = renderMode->fbWidth;
                modeSize.x = modeWidth;
                modeSize.y = modeHeight;
                func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(
                    &mRectData, *(const ml::CPnt16*)&modeSize);
            }
        }
        goto setRect_tail;
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
        (ml::CRect16*)&normalPos, &mFrame);
    normalSize.x = rect.mPos.x - normalPos.x;
    normalSize.y = rect.mPos.y - normalPos.y;
    *(u32*)&mFrame.mContentX = *(u32*)&normalSize;
    func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(&mRectData, rect.mSize);

setRect_tail:
    if ((unk278 & 0x10) == 0) {
        func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(&mRectData, rect.mSize);
    }
}

bool CView::attachRenderWork(CWorkThread* pThread) {
    // Dual context-ring enqueue: tag0+WorkID then tag1+thread*. Typed
    // CMsgParam<10> restores exact -0x80 / stmw r21 / 0x1E0 / stwux; remaining
    // dual-inline snap schedule is a Chaitin near-miss — keep iterating in high-level C.
    CMsgParam<10>& messages =
        *reinterpret_cast<CMsgParam<10>*>(&mContextMsgVtable);

    messages.enqueue(0);
    messages.last().unk23 = 3;
    messages.last().wid = pThread->mWorkID;
    messages.enqueue(1);
    messages.last().unk23 = 3;
    messages.last().wid = (WORK_ID)pThread;
    return true;
}

void CView::detachRenderWork(CWorkThread* pThread) {
    mFrame.detachRenderWork(pThread);
}

// Drain the context ring: classify each pending message and apply side effects.
void CView::updateMsg() {
#if 1
    struct CtxSnap {
        u32 w0;
        u32 w1;
        u32 w2;
        u32 w3;
        u32 w4;
        u32 w5;
        u32 w6;
        s16 half;
        u8 byte;
        u8 pad;
    };
    // Three uninit 0x24 snaps (retail -0x150 homes @ 0x48 / 0x28 / 0x08).
    // Keep only these — dual fan-copy volatiles force -0x140 / stmw r17.
    volatile CtxSnap snapFan0;
    volatile CtxSnap snapFan1;
    volatile CtxSnap snapTag1;
#endif
    u32 tag0;
    u32 tag1;
    u32 flag;
    u32 readIdx;
    u32 cap;
    u32 slotOff;
    u8* ringBase;
    u8* slot;
    u32 tag;
    void** listSentinel;
    void** listNode;
    CWorkThread* parentSnap;
    CWorkThread* parentView;
    CWorkThread* workThread;
    void** childSentinel;
    void** childNode;
    CWorkThread* childThread;
    CView* childView;
    u32 writeIdx;
    u32 prevIdx;
    u32 sumU;
    u32 slotU;
    s32 sumSigned;
    s32 capSigned;
    s32 slotIndex;
    u32 byteOff;
    u8* childRing;
    u8* childSlot;
    u32 workId;

    goto updateMsg_check;

updateMsg_loop:
    readIdx = unk3F0;
    cap = mContextRingCapacity;
    ringBase = (u8*)mContextRingBase;
    slotOff = (readIdx - (readIdx / cap) * cap) * 0x24u;
    slot = ringBase + slotOff;
    tag = *(u32*)slot;

    if (tag > 7) {
        goto updateMsg_advance;
    }

    switch (tag) {
        case 0: {
            // Attach WORK_ID to unk238; fan-out dual ring msgs to child views.
            u32 msgItem = *(u32*)(slot + 4);
            listSentinel = (void**)unk238.mStartNodePtr;
            listNode = (void**)*listSentinel;
            while (listNode != listSentinel) {
                if ((u32)listNode[2] == msgItem) {
                    break;
                }
                listNode = (void**)listNode[0];
            }

            if (listNode == listSentinel) {
                reinterpret_cast<reslist<u32>*>(&unk238)->push_back(msgItem);
            }

            parentSnap = mParent;
            if (getInstance__9CViewRootFv() == parentSnap) {
                parentView = nullptr;
            } else {
                parentView = mParent;
                if (parentView == nullptr) {
                    parentView = nullptr;
                } else if ((s32)parentView->mType < 0x30) {
                    parentView = nullptr;
                } else if ((s32)parentView->mType >= 0x35) {
                    parentView = nullptr;
                }
            }

            if (parentView == nullptr) {
                goto updateMsg_advance;
            }

            workThread = getWorkThread__9CWorkUtilFUl(*(u32*)(ringBase + slotOff + 4));
            childSentinel = (void**)mChildren.mStartNodePtr;
            childNode = (void**)childSentinel[0];
            while (childNode != childSentinel) {
                childThread = (CWorkThread*)childNode[2];
                childView = CView::convertToView(childThread);
                if (childView != nullptr) {
#if 1
                    CMsgParam<10>& childMessages =
                        *reinterpret_cast<CMsgParam<10>*>(&childView->mContextMsgVtable);
                    childMessages.enqueue(0);
                    childMessages.last().unk23 = 3;
                    childMessages.last().wid = workThread->mWorkID;
                    childMessages.enqueue(1);
                    childMessages.last().unk23 = 3;
                    childMessages.last().wid = (WORK_ID)workThread;
#else
                    tag0 = 0;
                    flag = 3;
                    tag1 = 1;
                    workId = workThread->mWorkID;

                    sumSigned = (s32)childView->unk3F0 + (s32)childView->mContextRingWriteIndex;
                    capSigned = (s32)childView->mContextRingCapacity;
                    slotIndex = sumSigned / capSigned;
                    byteOff = (u32)(sumSigned - slotIndex * capSigned) * 0x24u;
                    childRing = (u8*)childView->mContextRingBase;
                    childSlot = childRing + byteOff;
                    *(u32*)childSlot = tag0;
                    *(u32*)(childSlot + 0x4) = snapFan0.w0;
                    *(u32*)(childSlot + 0x8) = snapFan0.w1;
                    *(u32*)(childSlot + 0xC) = snapFan0.w2;
                    *(u32*)(childSlot + 0x10) = snapFan0.w3;
                    *(u32*)(childSlot + 0x14) = snapFan0.w4;
                    *(u32*)(childSlot + 0x18) = snapFan0.w5;
                    *(u32*)(childSlot + 0x1C) = snapFan0.w6;
                    *(s16*)(childSlot + 0x20) = snapFan0.half;
                    childSlot[0x22] = snapFan0.byte;
                    childSlot[0x23] = (u8)tag0;

                    writeIdx = childView->mContextRingWriteIndex + 1;
                    prevIdx = writeIdx - 1;
                    childView->mContextRingWriteIndex = writeIdx;
                    childView->unk3FC = prevIdx;

                    sumU = childView->unk3F0 + prevIdx;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    childSlot[0x23] = (u8)flag;

                    sumU = childView->unk3F0 + childView->unk3FC;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    *(u32*)(childSlot + 0x4) = workId;

                    sumSigned = (s32)childView->unk3F0 + (s32)childView->mContextRingWriteIndex;
                    capSigned = (s32)childView->mContextRingCapacity;
                    slotIndex = sumSigned / capSigned;
                    byteOff = (u32)(sumSigned - slotIndex * capSigned) * 0x24u;
                    childRing = (u8*)childView->mContextRingBase;
                    childSlot = childRing + byteOff;
                    *(u32*)childSlot = tag1;
                    *(u32*)(childSlot + 0x4) = snapFan1.w0;
                    *(u32*)(childSlot + 0x8) = snapFan1.w1;
                    *(u32*)(childSlot + 0xC) = snapFan1.w2;
                    *(u32*)(childSlot + 0x10) = snapFan1.w3;
                    *(u32*)(childSlot + 0x14) = snapFan1.w4;
                    *(u32*)(childSlot + 0x18) = snapFan1.w5;
                    *(u32*)(childSlot + 0x1C) = snapFan1.w6;
                    *(s16*)(childSlot + 0x20) = snapFan1.half;
                    childSlot[0x22] = snapFan1.byte;
                    childSlot[0x23] = (u8)tag0;

                    writeIdx = childView->mContextRingWriteIndex + 1;
                    childView->mContextRingWriteIndex = writeIdx;
                    prevIdx = writeIdx - 1;
                    childView->unk3FC = prevIdx;

                    sumU = childView->unk3F0 + prevIdx;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    childSlot[0x23] = (u8)flag;

                    sumU = childView->unk3F0 + childView->unk3FC;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    *(u32*)(childSlot + 0x4) = (u32)workThread;
#endif
                }
                childNode = (void**)childNode[0];
            }
            break;
        }
        case 1: {
            // Attach IWorkEvent* to unk258; fan-out tag-1 msgs to child views.
            u32 msgItem = *(u32*)(slot + 4);
            listSentinel = (void**)unk258.mStartNodePtr;
            listNode = (void**)*listSentinel;
            while (listNode != listSentinel) {
                if ((u32)listNode[2] == msgItem) {
                    break;
                }
                listNode = (void**)listNode[0];
            }

            if (listNode == listSentinel) {
                reinterpret_cast<reslist<void*>*>(&unk258)->push_back(
                    *reinterpret_cast<void**>(&msgItem));
            }

            parentSnap = mParent;
            if (getInstance__9CViewRootFv() == parentSnap) {
                parentView = nullptr;
            } else {
                parentView = mParent;
                if (parentView == nullptr) {
                    parentView = nullptr;
                } else if ((s32)parentView->mType < 0x30) {
                    parentView = nullptr;
                } else if ((s32)parentView->mType >= 0x35) {
                    parentView = nullptr;
                }
            }

            if (parentView == nullptr) {
                goto updateMsg_advance;
            }

            msgItem = *(u32*)(slot + 4);
            childSentinel = (void**)mChildren.mStartNodePtr;
            childNode = (void**)childSentinel[0];
            while (childNode != childSentinel) {
                childThread = (CWorkThread*)childNode[2];
                childView = CView::convertToView(childThread);
                if (childView != nullptr) {
#if 1
                    CMsgParam<10>& childMessages =
                        *reinterpret_cast<CMsgParam<10>*>(&childView->mContextMsgVtable);
                    childMessages.enqueue(1);
                    childMessages.last().unk23 = 3;
                    childMessages.last().wid = msgItem;
#else
                    tag1 = 1;
                    flag = 3;
                    tag0 = 0;

                    sumSigned = (s32)childView->unk3F0 + (s32)childView->mContextRingWriteIndex;
                    capSigned = (s32)childView->mContextRingCapacity;
                    slotIndex = sumSigned / capSigned;
                    byteOff = (u32)(sumSigned - slotIndex * capSigned) * 0x24u;
                    childRing = (u8*)childView->mContextRingBase;
                    childSlot = childRing + byteOff;
                    *(u32*)childSlot = tag1;
                    *(u32*)(childSlot + 0x4) = snapTag1.w0;
                    *(u32*)(childSlot + 0x8) = snapTag1.w1;
                    *(u32*)(childSlot + 0xC) = snapTag1.w2;
                    *(u32*)(childSlot + 0x10) = snapTag1.w3;
                    *(u32*)(childSlot + 0x14) = snapTag1.w4;
                    *(u32*)(childSlot + 0x18) = snapTag1.w5;
                    *(u32*)(childSlot + 0x1C) = snapTag1.w6;
                    *(s16*)(childSlot + 0x20) = snapTag1.half;
                    childSlot[0x22] = snapTag1.byte;
                    childSlot[0x23] = (u8)tag0;

                    writeIdx = childView->mContextRingWriteIndex + 1;
                    prevIdx = writeIdx - 1;
                    childView->mContextRingWriteIndex = writeIdx;
                    childView->unk3FC = prevIdx;

                    sumU = childView->unk3F0 + prevIdx;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    childSlot[0x23] = (u8)flag;

                    sumU = childView->unk3F0 + childView->unk3FC;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    *(u32*)(childSlot + 0x4) = msgItem;
#endif
                }
                childNode = (void**)childNode[0];
            }
            break;
        }
        case 2: {
            // Remove WORK_ID from unk238.
            u32 msgItem = *(u32*)(slot + 4);
            listSentinel = (void**)unk238.mStartNodePtr;
            listNode = (void**)listSentinel[0];
            while (listNode != listSentinel) {
                void** nextNode = (void**)listNode[0];
                if ((u32)listNode[2] == msgItem) {
                    void** prevNode = (void**)listNode[1];
                    prevNode[0] = nextNode;
                    nextNode[1] = prevNode;
                    listNode[0] = 0;
                }
                listNode = nextNode;
            }
            break;
        }
        case 3:
            unk278 |= 0x3;
            break;
        case 4:
            unk278 |= 0x4;
            break;
        case 5:
            unk278 &= ~0x4;
            break;
        case 6:
            setCurrent__9CViewRootFP5CView(this);
            break;
        case 7:
            if (*(u8*)(slot + 4) == 0) {
                unk278 |= 0x20;
            } else {
                unk278 &= ~0x20;
            }
            break;
        }

    updateMsg_advance:
        readIdx = unk3F0;
        cap = mContextRingCapacity;
        mContextRingWriteIndex -= 1;
        unk3F0 = (readIdx + 1) - ((readIdx + 1) / cap) * cap;

    updateMsg_check:
        if (mContextRingWriteIndex != 0) {
            goto updateMsg_loop;
        }
}

void CView::wkUpdate() {
    u32 hasView1;
    u32 hasView2;

    if (unk45C != nullptr && !isActive__11CSplitFrameFv(unk45C)) {
        if (unk45C != nullptr) {
            delete static_cast<CSplitFrame*>(unk45C);
            unk45C = nullptr;
        }
    }

    updateMsg();

    {
        u32 hasView1a;
        u32 hasView2a;

        hasView2a = 0;
        hasView1a = 0;
        if (unk45C == nullptr) {
            goto wkUpdate_no_apply;
        }
        if (getView1__11CSplitFrameFv(unk45C) == nullptr) {
            goto wkUpdate_no_apply;
        }
        hasView1a = 1;

    wkUpdate_no_apply:
        if (hasView1a == 0) {
            goto wkUpdate_apply_check;
        }
        if (getView2__11CSplitFrameFv(unk45C) == nullptr) {
            goto wkUpdate_apply_check;
        }
        hasView2a = 1;

    wkUpdate_apply_check:
        if (hasView2a != 0) {
            apply__11CSplitFrameFv(unk45C);
        }
    }

    if (mChildren.empty() && unk238.empty()) {
        hasView2 = 0;
        hasView1 = 0;
        if (unk45C == nullptr) {
            goto wkUpdate_no_setevent_check;
        }
        if (getView1__11CSplitFrameFv(unk45C) == nullptr) {
            goto wkUpdate_no_setevent_check;
        }
        hasView1 = 1;

    wkUpdate_no_setevent_check:
        if (hasView1 == 0) {
            goto wkUpdate_setevent;
        }
        if (getView2__11CSplitFrameFv(unk45C) == nullptr) {
            goto wkUpdate_setevent;
        }
        hasView2 = 1;

    wkUpdate_setevent:
        if (hasView2 == 0) {
            wkSetEvent(EVT_NONE);
        }
    }
}

void CView::renderView() {
    // Retail r28: 0, then fs pssGetRoot, then 0/1.
    u32 crossRootFlag;
    CView* fullScreenView;
    CProc* thisRoot;
    s16 savedSizeX;
    s16 savedSizeY;
    void* listNode;
    u32 listLen;
    void* listStart;
    CView* parentView;
    CWorkThread* parentThread;
    u32 invalidRect;
    u32 hasView1;
    u32 hasView2;
    u32 colorUpdateOff;
    u32 flags278;
    // Decl order first = higher addr. Share scratch homes so the frame stays
    // -0x180 while preserving the retail rectangle lifetimes.
    volatile ml::CRect16 viewRect;
    ml::CRect16 accumRect;
    ml::CRect16 scissorOut;
    ml::CRect16 insetRect;
    ml::CRect16 clearRect;
    ml::CRect16 scissorSrc;
    volatile ml::CRect16 parentAccumRect;
    ml::CRect16 home34;
    ml::CRect16 home28;
    ml::CRect16 home30; // early / y-loop / clear
    ml::CRect16 home18; // also home20
    ml::CRect16 home14; // also home1C
    ml::CRect16 home08;
    s16 yAccum;
    void** attachNode;
    CWorkThread* attachWork;
    u32 msgQualified;
    u32 stateReady;
    u32 shouldRender;
    void** childNode;
    CView* childView;
    CView* frameParent;
    s32 accumX;
    s32 accumY;
    s32 parentAccumX;
    s32 parentAccumY;

    if ((unk278 & 0x40) != 0) {
        return;
    }

    func_8043E010__5CViewFv(this);

    crossRootFlag = 0;
    fullScreenView = getFullScreenView__9CViewRootFv();

    if ((fullScreenView->unk278 & 0x2) != 0) {
        goto renderView_after_cross;
    }
    if (fullScreenView == this) {
        goto renderView_after_cross;
    }

    listStart = unk238.mStartNodePtr;
    listLen = 0;
    listNode = *(void**)listStart;
    goto renderView_count_this_check;
renderView_count_this_body:
    listNode = *(void**)listNode;
    listLen = listLen + 1;
renderView_count_this_check:
    if (listNode != listStart) {
        goto renderView_count_this_body;
    }
    if (listLen == 0) {
        goto renderView_after_cross;
    }

    listStart = fullScreenView->unk238.mStartNodePtr;
    listLen = 0;
    listNode = *(void**)listStart;
    goto renderView_count_fs_check;
renderView_count_fs_body:
    listNode = *(void**)listNode;
    listLen = listLen + 1;
renderView_count_fs_check:
    if (listNode != listStart) {
        goto renderView_count_fs_body;
    }
    if (listLen == 0) {
        goto renderView_after_cross;
    }

    crossRootFlag = (u32)pssGetRoot__5CProcFP5CProc(CProc::convertToProc(
        getWorkThread__9CWorkUtilFUl(*(u32*)((u8*)*(void**)listStart + 8))));

    listStart = unk238.mStartNodePtr;
    thisRoot = pssGetRoot__5CProcFP5CProc(CProc::convertToProc(
        getWorkThread__9CWorkUtilFUl(*(u32*)((u8*)*(void**)listStart + 8))));

    {
        CProc* fullScreenRoot = (CProc*)crossRootFlag;
        crossRootFlag = 0;
        if (fullScreenRoot == thisRoot) {
            goto renderView_after_cross;
        }
        if (thisRoot == nullptr) {
            goto renderView_after_cross;
        }
        if ((s32)unk460 < 7) {
            goto renderView_after_cross;
        }
        crossRootFlag = 1;
    }

renderView_after_cross:
    if (fullScreenView != this) {
        goto renderView_after_cache_color;
    }

    {
        ml::CCol4 cacheColor;
        // Retail: lfs scale->f4, z, y, x; fmuls b; lfs w; fmuls g/r; stfs a,r,g,b
        float scale = mAlpha;
        float b = unk444.z * scale;
        float a = unk444.w;
        float g = unk444.y * scale;
        float r = unk444.x * scale;
        cacheColor.a = a;
        cacheColor.r = r;
        cacheColor.g = g;
        cacheColor.b = b;
        func_8044A578__8CGXCacheFv(CDeviceGX::getCacheInstance(), &cacheColor, 0);
    }

renderView_after_cache_color:
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home30,
                                                              &mFrame);
    savedSizeX = mRectData.mViewSize.x;
    savedSizeY = mRectData.mViewSize.y;
    yAccum = mFrame.mContentY;
    parentThread = mParent;

    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_y_accum_check;
    }
    parentView = CView::convertToView(mParent);
    goto renderView_y_accum_check;

renderView_y_accum_body:
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home30,
                                                              &parentView->mFrame);
    yAccum = (s16)(yAccum + (s16)(parentView->mFrame.mContentY + home30.mPos.y));
    parentThread = parentView->mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_y_accum_check;
    }
    parentView = CView::convertToView(parentView->mParent);

renderView_y_accum_check:
    if (parentView != nullptr) {
        goto renderView_y_accum_body;
    }
    *reinterpret_cast<volatile s16*>(&home08.mPos.y) = yAccum;

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home34,
                                                              &mFrame);
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home28,
                                                              &mFrame);

    invalidRect = 0;
    viewRect.mPos.y = (s16)(mFrame.mContentY + home28.mPos.y);
    viewRect.mPos.x = (s16)(mFrame.mContentX + home28.mPos.x);
    viewRect.mSize.x = mRectData.mViewSize.x;
    viewRect.mSize.y = mRectData.mViewSize.y;
    if (mRectData.mViewSize.x <= 0) {
        goto renderView_mark_invalid;
    }
    if (mRectData.mViewSize.y > 0) {
        goto renderView_after_size_gate1;
    }
renderView_mark_invalid:
    invalidRect = 1;
renderView_after_size_gate1:
    if (invalidRect != 0) {
        goto renderView_tail;
    }

    invalidRect = 0;
    if (savedSizeX <= 0) {
        goto renderView_mark_invalid2;
    }
    if (savedSizeY > 0) {
        goto renderView_after_size_gate2;
    }
renderView_mark_invalid2:
    invalidRect = 1;
renderView_after_size_gate2:
    if (invalidRect != 0) {
        goto renderView_tail;
    }

    {
        u32 maxSizeBad;

        maxSizeBad = 0;
        if (mRectData.mBoundsSize.x > 0) {
            goto renderView_after_max_gate;
        }
        if (mRectData.mBoundsSize.y > 0) {
            goto renderView_after_max_gate;
        }
        maxSizeBad = 1;
    renderView_after_max_gate:
        if (maxSizeBad != 0) {
            goto renderView_tail;
        }
    }

    if (crossRootFlag != 0) {
        goto renderView_scissor_setup;
    }

    {
        u32 hasView2a;
        u32 hasView1a;

        hasView2a = 0;
        hasView1a = 0;
        if (unk45C == nullptr) {
            goto renderView_clear_no_view1;
        }
        if (getView1__11CSplitFrameFv(unk45C) == nullptr) {
            goto renderView_clear_no_view1;
        }
        hasView1a = 1;
    renderView_clear_no_view1:
        if (hasView1a == 0) {
            goto renderView_clear_split_check;
        }
        if (getView2__11CSplitFrameFv(unk45C) == nullptr) {
            goto renderView_clear_split_check;
        }
        hasView2a = 1;
    renderView_clear_split_check:
        if (hasView2a != 0) {
            goto renderView_scissor_setup;
        }

        flags278 = unk278;
        if ((flags278 & 1) != 0) {
            goto renderView_scissor_setup;
        }

        colorUpdateOff = (flags278 >> 2) & 1;
        {
            CDrawGX draw;
            draw.func_80456570(0);
            draw.func_8045657C(1);
            if (colorUpdateOff != 0) {
                GXSetColorUpdate(GX_FALSE);
                goto renderView_clear_begin;
            }
            {
                ml::CCol4 clearColor;
                clearColor = *reinterpret_cast<ml::CCol4*>(&unk444);
                clearColor.a *= mAlpha;
                draw.setCol(clearColor);
            }
        renderView_clear_begin:
            *(u32*)((u8*)&draw + 0x1C) = mGXCacheId;
            draw.begin(PRIM_QUADS, 1);
            getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home30,
                                                                      &mFrame);
            clearRect.mPos.y = (s16)(mFrame.mContentY + home30.mPos.y);
            clearRect.mPos.x = (s16)(mFrame.mContentX + home30.mPos.x);
            clearRect.mSize.x = mRectData.mViewSize.x;
            clearRect.mSize.y = mRectData.mViewSize.y;
            draw.add(clearRect);
            draw.end();
            if (colorUpdateOff != 0) {
                GXSetColorUpdate(GX_TRUE);
            }
        }
    }

renderView_scissor_setup:
    if (crossRootFlag != 0) {
        goto renderView_works_or_children;
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home18, &mFrame);
    accumRect.mPos.y = (s16)(mFrame.mContentY + home18.mPos.y);
    accumRect.mPos.x = (s16)(mFrame.mContentX + home18.mPos.x);
    accumRect.mSize.x = mRectData.mViewSize.x;
    accumRect.mSize.y = mRectData.mViewSize.y;

    accumX = mFrame.mContentX;
    accumY = mFrame.mContentY;

    parentThread = mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_xy_accum_check;
    }
    parentView = CView::convertToView(mParent);
    goto renderView_xy_accum_check;

renderView_xy_accum_body:
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home14,
                                                              &parentView->mFrame);
    accumX += (s16)(parentView->mFrame.mContentX + home14.mPos.x);
    accumY += (s16)(parentView->mFrame.mContentY + home14.mPos.y);
    parentThread = parentView->mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_xy_accum_check;
    }
    parentView = CView::convertToView(parentView->mParent);

renderView_xy_accum_check:
    if (parentView != nullptr) {
        goto renderView_xy_accum_body;
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home18, &mFrame);
    accumRect.mPos.x = (s16)(accumX + home18.mPos.x);
    accumRect.mPos.y = (s16)(accumY + home18.mPos.y);

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home08, &mFrame);
    parentAccumX = mFrame.mContentX;
    parentAccumY = mFrame.mContentY;
    parentAccumRect.mPos.x = (s16)(mFrame.mContentX + home08.mPos.x);
    parentAccumRect.mPos.y = (s16)(mFrame.mContentY + home08.mPos.y);
    parentAccumRect.mSize.x = mRectData.mViewSize.x;
    parentAccumRect.mSize.y = mRectData.mViewSize.y;

    parentThread = mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_parent_accum_check;
    }
    parentView = CView::convertToView(mParent);
    goto renderView_parent_accum_check;

renderView_parent_accum_body:
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home14,
                                                              &parentView->mFrame);
    parentAccumX += (s16)(parentView->mFrame.mContentX + home14.mPos.x);
    parentAccumY += (s16)(parentView->mFrame.mContentY + home14.mPos.y);
    parentThread = parentView->mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_parent_accum_check;
    }
    parentView = CView::convertToView(parentView->mParent);

renderView_parent_accum_check:
    if (parentView != nullptr) {
        goto renderView_parent_accum_body;
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home08, &mFrame);
    parentAccumRect.mPos.x = (s16)(parentAccumX + home08.mPos.x);
    parentAccumRect.mPos.y = (s16)(parentAccumY + home08.mPos.y);

    parentThread = mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        frameParent = nullptr;
        goto renderView_frame_parent_ready;
    }
    frameParent = CView::convertToView(mParent);

renderView_frame_parent_ready:
    if (frameParent == nullptr) {
        goto renderView_local_scissor;
    }

    {
        u32 hasView2b;
        u32 hasView1b;

        hasView2b = 0;
        hasView1b = 0;
        if (frameParent->unk45C == nullptr) {
            goto renderView_fp_no_view1;
        }
        if (getView1__11CSplitFrameFv(frameParent->unk45C) == nullptr) {
            goto renderView_fp_no_view1;
        }
        hasView1b = 1;
    renderView_fp_no_view1:
        if (hasView1b == 0) {
            goto renderView_fp_split_check;
        }
        if (getView2__11CSplitFrameFv(frameParent->unk45C) == nullptr) {
            goto renderView_fp_split_check;
        }
        hasView2b = 1;
    renderView_fp_split_check:
        if (hasView2b != 0) {
            goto renderView_parent_split_scissor;
        }
    }

renderView_local_scissor:
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home14, &mFrame);
    scissorSrc.mPos.y = (s16)(mFrame.mContentY + home14.mPos.y);
    scissorSrc.mPos.x = (s16)(mFrame.mContentX + home14.mPos.x);
    scissorSrc.mSize.x = mRectData.mViewSize.x;
    scissorSrc.mSize.y = mRectData.mViewSize.y;
    goto renderView_scissor_ready;

renderView_parent_split_scissor:
    {
        u32 hasView2c;
        u32 hasView1c;
        CView* splitView1;

        hasView2c = 0;
        hasView1c = 0;
        if (frameParent->unk45C == nullptr) {
            goto renderView_fp2_no_view1;
        }
        if (getView1__11CSplitFrameFv(frameParent->unk45C) == nullptr) {
            goto renderView_fp2_no_view1;
        }
        hasView1c = 1;
    renderView_fp2_no_view1:
        if (hasView1c == 0) {
            goto renderView_fp2_split_check;
        }
        if (getView2__11CSplitFrameFv(frameParent->unk45C) == nullptr) {
            goto renderView_fp2_split_check;
        }
        hasView2c = 1;
    renderView_fp2_split_check:
        if (hasView2c == 0) {
            splitView1 = nullptr;
            goto renderView_split_which;
        }
        splitView1 = getView1__11CSplitFrameFv(frameParent->unk45C);
    renderView_split_which:
        if (splitView1 == this) {
            getScissorRect1__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(
                &scissorSrc, frameParent->unk45C);
            goto renderView_scissor_ready;
        }
        getScissorRect2__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(
            &scissorSrc, frameParent->unk45C);
        goto renderView_scissor_ready;
    }

renderView_scissor_ready:
    scissorOut.mPos.x = parentAccumRect.mPos.x;
    scissorOut.mPos.y = parentAccumRect.mPos.y;
    scissorOut.mSize.x = scissorSrc.mSize.x;
    scissorOut.mSize.y = scissorSrc.mSize.y;
    insetRect.mPos.x = mRectData.mScrollX;
    insetRect.mPos.y = mRectData.mScrollY;
    insetRect.mSize.x = mRectData.mBoundsSize.x;
    insetRect.mSize.y = mRectData.mBoundsSize.y;
    func_80442B54__9CViewRootFPvPv(&accumRect, &scissorOut, &insetRect);

renderView_works_or_children:
    if (crossRootFlag != 0) {
        goto renderView_children;
    }

    hasView2 = 0;
    hasView1 = 0;
    if (unk45C == nullptr) {
        goto renderView_wk_no_view1;
    }
    if (getView1__11CSplitFrameFv(unk45C) == nullptr) {
        goto renderView_wk_no_view1;
    }
    hasView1 = 1;
renderView_wk_no_view1:
    if (hasView1 == 0) {
        goto renderView_wk_split_check;
    }
    if (getView2__11CSplitFrameFv(unk45C) == nullptr) {
        goto renderView_wk_split_check;
    }
    hasView2 = 1;
renderView_wk_split_check:
    if (hasView2 != 0) {
        goto renderView_children;
    }

    lbl_eu_806655C8 = this;

    if (isRunning()) {
        wkRender();
    }
#if 0
    if ((mFlags & THREAD_FLAG_EXCEPTION) != 0) {
        msgQualified = 1;
        goto renderView_self_qualified;
    }

    {
        u32 msgIndex;
        u32 msgFront;
        u32 msgModulus;
        u32 msgSum;
        u32 msgRemainder;
        u32 msgEntryOffset;
        CMsgParamEntry* msgArray;
        u32 msgCount;

        msgCount = *(u32*)((u8*)this + 0x1AC);

        for (msgIndex = 0; msgIndex < msgCount; msgIndex++) {
            msgFront = *(u32*)((u8*)this + 0x1A8);
            msgModulus = *(u32*)((u8*)this + 0x1B0);
            msgSum = msgFront + msgIndex;
            msgArray = *(CMsgParamEntry**)((u8*)this + 0x1A4);
            msgRemainder = msgSum / msgModulus;
            msgRemainder = msgRemainder * msgModulus;
            msgRemainder = msgSum - msgRemainder;
            msgEntryOffset = msgRemainder * 0x24;

            if (*(u32*)((u8*)msgArray + msgEntryOffset) != 2) {
                continue;
            }

            goto renderView_self_msg_eval;
        }

        msgIndex = -1;

    renderView_self_msg_eval:
        msgQualified = (msgIndex >> 31) ^ 1;
    }

renderView_self_qualified:
    shouldRender = 0;
    if (msgQualified != 0) {
        goto renderView_self_state_end;
    }
    stateReady = 1;
    if (mState == THREAD_STATE_LOGIN) {
        goto renderView_self_state_ready;
    }
    if (mState == THREAD_STATE_RUN) {
        goto renderView_self_state_ready;
    }
    stateReady = 0;
renderView_self_state_ready:
    if (stateReady == 0) {
        goto renderView_self_state_end;
    }
    shouldRender = 1;
renderView_self_state_end:
    if (shouldRender != 0) {
        wkRender();
    }
#endif

    attachNode = (void**)unk238.mStartNodePtr;
    attachNode = (void**)*attachNode;
    goto renderView_attach_wk_check;

renderView_attach_wk_body:
    attachWork = getWorkThread__9CWorkUtilFUl(*(u32*)((u8*)attachNode + 8));
    if (attachWork == nullptr) {
        goto renderView_attach_wk_next;
    }
    lbl_eu_806655C8 = this;
    if (attachWork == this) {
        goto renderView_attach_wk_next;
    }

    if (attachWork->isRunning()) {
        goto renderView_attach_do_render_inline;
    }
    if (!attachWork->isException()) {
        goto renderView_attach_wk_next;
    }
renderView_attach_do_render_inline:
    attachWork->wkRender();
#if 0
    {
        u32 exceptFlag;

        exceptFlag = (attachWork->mFlags & THREAD_FLAG_EXCEPTION) != 0 ? 1 : 0;
        if (exceptFlag != 0) {
            msgQualified = 1;
            goto renderView_attach_qualified;
        }

        {
            u32 msgIndex;
            u32 msgFront;
            u32 msgModulus;
            u32 msgSum;
            u32 msgRemainder;
            u32 msgEntryOffset;
            CMsgParamEntry* msgArray;
            u32 msgCount;

            msgCount = *(u32*)((u8*)attachWork + 0x1AC);

            for (msgIndex = 0; msgIndex < msgCount; msgIndex++) {
                msgFront = *(u32*)((u8*)attachWork + 0x1A8);
                msgModulus = *(u32*)((u8*)attachWork + 0x1B0);
                msgSum = msgFront + msgIndex;
                msgArray = *(CMsgParamEntry**)((u8*)attachWork + 0x1A4);
                msgRemainder = msgSum / msgModulus;
                msgRemainder = msgRemainder * msgModulus;
                msgRemainder = msgSum - msgRemainder;
                msgEntryOffset = msgRemainder * 0x24;

                if (*(u32*)((u8*)msgArray + msgEntryOffset) != 2) {
                    continue;
                }

                goto renderView_attach_msg_eval;
            }

            msgIndex = -1;

        renderView_attach_msg_eval:
            msgQualified = (msgIndex >> 31) ^ 1;
        }

    renderView_attach_qualified:
        shouldRender = 0;
        if (msgQualified != 0) {
            goto renderView_attach_state_end;
        }
        stateReady = 1;
        if (attachWork->mState == THREAD_STATE_LOGIN) {
            goto renderView_attach_state_ready;
        }
        if (attachWork->mState == THREAD_STATE_RUN) {
            goto renderView_attach_state_ready;
        }
        stateReady = 0;
    renderView_attach_state_ready:
        if (stateReady == 0) {
            goto renderView_attach_state_end;
        }
        shouldRender = 1;
    renderView_attach_state_end:
        if (shouldRender != 0) {
            goto renderView_attach_do_render;
        }

        if (exceptFlag != 0) {
            msgQualified = 1;
            goto renderView_attach_ex_done;
        }

        {
            u32 msgIndex;
            u32 msgFront;
            u32 msgModulus;
            u32 msgSum;
            u32 msgRemainder;
            u32 msgEntryOffset;
            CMsgParamEntry* msgArray;
            u32 msgCount;

            msgCount = *(u32*)((u8*)attachWork + 0x1AC);

            for (msgIndex = 0; msgIndex < msgCount; msgIndex++) {
                msgFront = *(u32*)((u8*)attachWork + 0x1A8);
                msgModulus = *(u32*)((u8*)attachWork + 0x1B0);
                msgSum = msgFront + msgIndex;
                msgArray = *(CMsgParamEntry**)((u8*)attachWork + 0x1A4);
                msgRemainder = msgSum / msgModulus;
                msgRemainder = msgRemainder * msgModulus;
                msgRemainder = msgSum - msgRemainder;
                msgEntryOffset = msgRemainder * 0x24;

                if (*(u32*)((u8*)msgArray + msgEntryOffset) != 2) {
                    continue;
                }

                goto renderView_attach_ex_msg_eval;
            }

            msgIndex = -1;

        renderView_attach_ex_msg_eval:
            msgQualified = (msgIndex >> 31) ^ 1;
        }

    renderView_attach_ex_done:
        if (msgQualified == 0) {
            goto renderView_attach_wk_next;
        }

    renderView_attach_do_render:
        attachWork->wkRender();
    }
#endif

renderView_attach_wk_next:
    attachNode = (void**)*attachNode;

renderView_attach_wk_check:
    if (attachNode != (void**)unk238.mStartNodePtr) {
        goto renderView_attach_wk_body;
    }

    lbl_eu_806655C8 = nullptr;
    func_8044BE38__8CGXCacheFv(CDeviceGX::getCacheInstance());
    func_80442DA8__9CViewRootFv();
    fontFlush__10CFontLayerFi((CFontLayer*)((u8*)this + 0x1C4), 1);

renderView_children:
    childNode = (void**)mChildren.mStartNodePtr;
    childNode = (void**)*childNode;
    goto renderView_child_check;

renderView_child_body:
    childView = CView::convertToView(reinterpret_cast<CWorkThread*>(childNode[2]));
    childView->renderView();
    childNode = (void**)*childNode;

renderView_child_check:
    if (childNode != (void**)mChildren.mStartNodePtr) {
        goto renderView_child_body;
    }

    attachNode = (void**)unk238.mStartNodePtr;
    attachNode = (void**)*attachNode;
    goto renderView_attach_after_check;

renderView_attach_after_body:
    attachWork = getWorkThread__9CWorkUtilFUl(*(u32*)((u8*)attachNode + 8));
    if (attachWork == nullptr) {
        goto renderView_attach_after_next;
    }
    lbl_eu_806655C8 = this;
    if (attachWork == this) {
        goto renderView_attach_after_next;
    }

    if (attachWork->isRunning()) {
        attachWork->wkRenderAfter();
    }
#if 0
    if ((attachWork->mFlags & THREAD_FLAG_EXCEPTION) != 0) {
        msgQualified = 1;
        goto renderView_after_qualified;
    }

    {
        u32 msgIndex;
        u32 msgFront;
        u32 msgModulus;
        u32 msgSum;
        u32 msgRemainder;
        u32 msgEntryOffset;
        CMsgParamEntry* msgArray;
        u32 msgCount;

        msgCount = *(u32*)((u8*)attachWork + 0x1AC);

        for (msgIndex = 0; msgIndex < msgCount; msgIndex++) {
            msgFront = *(u32*)((u8*)attachWork + 0x1A8);
            msgModulus = *(u32*)((u8*)attachWork + 0x1B0);
            msgSum = msgFront + msgIndex;
            msgArray = *(CMsgParamEntry**)((u8*)attachWork + 0x1A4);
            msgRemainder = msgSum / msgModulus;
            msgRemainder = msgRemainder * msgModulus;
            msgRemainder = msgSum - msgRemainder;
            msgEntryOffset = msgRemainder * 0x24;

            if (*(u32*)((u8*)msgArray + msgEntryOffset) != 2) {
                continue;
            }

            goto renderView_after_msg_eval;
        }

        msgIndex = -1;

    renderView_after_msg_eval:
        msgQualified = (msgIndex >> 31) ^ 1;
    }

renderView_after_qualified:
    shouldRender = 0;
    if (msgQualified != 0) {
        goto renderView_after_state_end;
    }
    stateReady = 1;
    if (attachWork->mState == THREAD_STATE_LOGIN) {
        goto renderView_after_state_ready;
    }
    if (attachWork->mState == THREAD_STATE_RUN) {
        goto renderView_after_state_ready;
    }
    stateReady = 0;
renderView_after_state_ready:
    if (stateReady == 0) {
        goto renderView_after_state_end;
    }
    shouldRender = 1;
renderView_after_state_end:
    if (shouldRender != 0) {
        attachWork->wkRenderAfter();
    }
#endif

renderView_attach_after_next:
    attachNode = (void**)*attachNode;

renderView_attach_after_check:
    if (attachNode != (void**)unk238.mStartNodePtr) {
        goto renderView_attach_after_body;
    }

    lbl_eu_806655C8 = nullptr;
    func_8044BE38__8CGXCacheFv(CDeviceGX::getCacheInstance());
    func_80442DA8__9CViewRootFv();
    fontFlush__10CFontLayerFi((CFontLayer*)((u8*)this + 0x1C4), 1);

    if (crossRootFlag == 0) {
        func_80442C68__9CViewRootFv();
    }

renderView_tail:
    if (crossRootFlag != 0) {
        return;
    }

    parentThread = mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        frameParent = nullptr;
        goto renderView_frame_call;
    }
    frameParent = CView::convertToView(mParent);

renderView_frame_call:
    // Keep frameParent live in r4 by passing as extra param to render
    ((void(*)(CViewFrame*, CView*))render__10CViewFrameFv)(&mFrame, frameParent);
}

void CView::setDisp(bool r4, bool r5) {
    if (!r4) {
        unk278 |= 0x40;
    } else {
        unk278 &= ~0x40;
    }

    if (r5 && !r4) {
        CView* current = getCurrent__9CViewRootFv();
        if (current == this) {
            invalidCurrent__9CViewRootFP5CView(this);
        }
    }
}

// True when this view is neither the root current nor under it (inactive chrome).
bool CView::hasCurrent() const {
    int result = 0;
    if (CViewRoot::isCurrent(this)) {
        goto done;
    }
    if (CViewRoot::isCurrentChild(this, CViewRoot::getCurrent())) {
        goto done;
    }
    result = 1;
done:
    return (bool)result;
}

// PLAN.md 17.6: whole-function asm. MWCC C++ spills then lwz unk45C; retail
// interleaves lwz/cmpwi into the callee-save prologue (89.2% C++ cap).
asm s16 CView::getSplitLine() {
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    lwz r0, 0x45c(r3)
    stw r31, 0x1c(r1)
    li r31, 0
    cmpwi r0, 0
    stw r30, 0x18(r1)
    li r30, 0
    stw r29, 0x14(r1)
    mr r29, r3
    beq getSplitLine_no_view1
    mr r3, r0
    bl getView1__11CSplitFrameFv
    cmpwi r3, 0
    beq getSplitLine_no_view1
    li r30, 1
getSplitLine_no_view1:
    cmpwi r30, 0
    beq getSplitLine_no_view2
    lwz r3, 0x45c(r29)
    bl getView2__11CSplitFrameFv
    cmpwi r3, 0
    beq getSplitLine_no_view2
    li r31, 1
getSplitLine_no_view2:
    cmpwi r31, 0
    beq getSplitLine_zero
    lwz r3, 0x45c(r29)
    bl getSplitLine__11CSplitFrameFv
    b getSplitLine_epilogue
getSplitLine_zero:
    li r3, 0
getSplitLine_epilogue:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
}

void CView::setSplitLine(s16 line) {
    u32 hasView2;
    u32 hasView1;

    hasView2 = 0;
    hasView1 = 0;
    if (unk45C == nullptr) {
        goto setSplitLine_no_view1;
    }
    if (getView1__11CSplitFrameFv(unk45C) == nullptr) {
        goto setSplitLine_no_view1;
    }
    hasView1 = 1;

setSplitLine_no_view1:
    if (hasView1 == 0) {
        goto setSplitLine_no_view2;
    }
    if (getView2__11CSplitFrameFv(unk45C) == nullptr) {
        goto setSplitLine_no_view2;
    }
    hasView2 = 1;

setSplitLine_no_view2:
    if (hasView2 == 0) {
        return;
    }
    setSplitLine__11CSplitFrameFs(unk45C, line);
}

CView::CView(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 2),
      mName(false) {
    // The typed reserve calls below recover retail's shared 0x10 capacity
    // lifetime; guarded postprocessing closes the final register permutation.
    u32 zero;
    void* sentinel1;
    void* sentinel0;

    zero = 0;
    mRectData.func_80459270();
    __ct__CViewFrame(&mFrame);

    *(void**)&unk238 = lbl_eu_8056B298;
    sentinel0 = &unk238.mSentinelNext;
    unk238.mList = (void*)zero;
    unk238.mCapacity = (int)zero;
    unk238.unk1C = (u8)zero;
    unk238.mStartNodePtr = sentinel0;
    ((void**)sentinel0)[0] = sentinel0;
    sentinel0 = unk238.mStartNodePtr;
    ((void**)sentinel0)[1] = sentinel0;
    *(void**)&unk238 = lbl_eu_8056B280;

    *(void**)&unk258 = lbl_eu_8056B6F0;
    sentinel1 = &unk258.mSentinelNext;
    unk258.mList = (void*)zero;
    unk258.mCapacity = (int)zero;
    unk258.unk1C = (u8)zero;
    unk258.mStartNodePtr = sentinel1;
    ((void**)sentinel1)[0] = sentinel1;
    sentinel1 = unk258.mStartNodePtr;
    ((void**)sentinel1)[1] = sentinel1;
    *(void**)&unk258 = lbl_eu_8056B6D8;

    unk278 = zero;
    unk27C = zero;
    mContextMsgVtable = (u32)lbl_eu_8056B6CC;
    mContextRingCapacity = 10;
    mContextRingBase = mContextRing;
    mContextRingWriteIndex = zero;
    unk3F0 = zero;
    unk3FC = zero;
    mName.clear();

    mGXCacheId = func_8044BE2C__8CGXCacheFv();
    mAlpha = 1.0f;
    unk45C = (void*)zero;
    unk460 = 0xb;
    mType = THREAD_CVIEW;
    func_8043FC60__10CViewFrameFUl(&mFrame, (u32)this);

    reinterpret_cast<reslist<u32>*>(&unk238)->reserve(mAllocHandle, 0x10);
    reinterpret_cast<reslist<void*>*>(&unk258)->reserve(mAllocHandle, 0x10);

    unk444.x = 0.6f;
    unk444.y = 0.6f;
    unk444.z = 0.6f;
    unk444.w = 1.0f;
    unk464 = (s16)zero;
    unk466 = (s16)zero;
    unk468 = (s16)zero;
    unk46A = (s16)zero;

    *(ml::CCol4*)((u8*)&mFrame + 0x8) = sFrameColor;
    *(ml::CCol4*)((u8*)&mFrame + 0x28) = lbl_8065A0C8;
}

// LLM-HARNESS-BEGIN: us-8043f350
extern "C" void CView_UnkVirtualFunc7__5CViewFv() {}
// LLM-HARNESS-END: us-8043f350
// LLM-HARNESS-BEGIN: us-80440380
extern "C" void CView_UnkVirtualFunc3__5CViewFv() {}
// LLM-HARNESS-END: us-80440380
// LLM-HARNESS-BEGIN: us-804406b8
extern "C" void CView_UnkVirtualFunc4__5CViewFv() {}
// LLM-HARNESS-END: us-804406b8
// LLM-HARNESS-BEGIN: us-80441460
extern "C" void CView_UnkVirtualFunc9__5CViewFv() {}
// LLM-HARNESS-END: us-80441460
// LLM-HARNESS-BEGIN: us-80441468
extern "C" void CView_UnkVirtualFunc8__5CViewFv() {}
// LLM-HARNESS-END: us-80441468
// LLM-HARNESS-BEGIN: us-804424f0
extern "C" int CView_UnkVirtualFunc6__5CViewFv() { return 0; }
// LLM-HARNESS-END: us-804424f0
// LLM-HARNESS-BEGIN: us-804424f8
extern "C" int CView_UnkVirtualFunc5__5CViewFv() { return 0; }
// LLM-HARNESS-END: us-804424f8
// LLM-HARNESS-BEGIN: us-80442500
extern "C" void CView_UnkVirtualFunc1__5CViewFv() {}
// LLM-HARNESS-END: us-80442500
// LLM-HARNESS-BEGIN: us-80442508
extern "C" void sinit_8043FB70() {}
// LLM-HARNESS-END: us-80442508
// LLM-HARNESS-BEGIN: us-8044255c
extern "C" bool func_8043FBC4() { return false; }
// LLM-HARNESS-END: us-8044255c

// LLM-HARNESS-BEGIN: us-8043f178
extern "C" void __dt__5CViewFv() {}
// LLM-HARNESS-END: us-8043f178
