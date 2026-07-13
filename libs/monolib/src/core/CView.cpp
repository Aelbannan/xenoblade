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
void updateMsg__5CViewFv(CView* pThis);
void getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(ml::CRect16* rect, CViewFrame* frame);
void func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(CViewRectDataCore* data, const ml::CPnt16& size);
void func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(CViewRectDataCore* data, const ml::CPnt16& maxSize);
}

ml::CCol4 CView::sFrameColor;

void CView::setDefaultFrameColor(const ml::CCol4& color) {
    sFrameColor = color;
}

CView* CView::getCurrentView() {
    return lbl_eu_806655C8;
}

void CView::setCurrent() {
    u32 ctx0;
    u32 ctx1;
    u32 ctx2;
    u32 ctx3;
    u32 ctx4;
    u32 ctx5;
    u32 ctx6;
    s16 ctxHalf;
    u8 ctxByte;
    u32 tag;
    s32 sumSigned;
    s32 capSigned;
    s32 slotIndex;
    u32 byteOff;
    u8* ringBase;

    tag = 6;
    ctx0 = *(const u32*)&unk1C8.unk0;
    ctx1 = *(const u32*)&unk1C8.unk4;
    ctx2 = *(const u32*)&unk1C8.unk8;
    ctx3 = *(const u32*)&unk1C8.unkC;
    ctx4 = *(const u32*)&unk1C8.unk10;
    ctx5 = *(const u32*)&unk1DC.unk0[0];
    ctx6 = *(const u32*)&unk1DC.unk0[4];
    ctxHalf = unk1DC.unk54;
    ctxByte = (u8)unk1DC.unk56;

    sumSigned = (s32)unk3F0 + (s32)mContextRingWriteIndex;
    capSigned = (s32)mContextRingCapacity;
    slotIndex = sumSigned / capSigned;
    byteOff = (u32)(sumSigned - slotIndex * capSigned) * 0x24u;
    ringBase = (u8*)mContextRingBase;

    *(u32*)(ringBase + byteOff) = tag;
    *(u32*)(ringBase + 0x4) = ctx0;
    *(u32*)(ringBase + 0x8) = ctx1;
    *(u32*)(ringBase + 0xC) = ctx2;
    *(u32*)(ringBase + 0x10) = ctx3;
    *(u32*)(ringBase + 0x14) = ctx4;
    *(u32*)(ringBase + 0x18) = ctx5;
    *(u32*)(ringBase + 0x1C) = ctx6;
    *(s16*)(ringBase + 0x20) = ctxHalf;
    ringBase[0x22] = ctxByte;
    ringBase[0x23] = 0;

    mContextRingWriteIndex += 1;
    unk3FC = mContextRingWriteIndex - 1;
}

void CView::setRect(const ml::CRect16& rect) {
    ml::CRect16 frameOffset;
    ml::CPnt16 stackPos;
    CWorkThread* parentSnap;
    CWorkThread* sourceParent;

    if ((unk278 & 1) != 0) {
        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&frameOffset, &unk1DC);
        stackPos.x = -frameOffset.mPos.x;
        parentSnap = mParent;
        stackPos.y = -frameOffset.mPos.y;
        *(u32*)&unk1DC.unk54 = *(u32*)&stackPos;

        if (getInstance__9CViewRootFv() == parentSnap) {
            sourceParent = nullptr;
        } else {
            sourceParent = mParent;
            if (sourceParent == nullptr) {
                goto setRect_parent_null;
            }
            if ((s32)sourceParent->mType < 0x30) {
                goto setRect_parent_null;
            }
            if ((s32)sourceParent->mType < 0x35) {
                goto setRect_parent_done;
            }
            goto setRect_parent_null;
        setRect_parent_null:
            sourceParent = nullptr;
        setRect_parent_done:;
        }

        if (sourceParent != nullptr) {
            func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(&unk1C8, *(const ml::CPnt16*)((u8*)sourceParent + 0x1CC));
        } else {
            ml::CPnt16 modeSize;

            modeSize.y = getRenderModeObj__9CDeviceVIFv()->efbHeight;
            modeSize.x = getRenderModeObj__9CDeviceVIFv()->fbWidth;
            func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(&unk1C8, modeSize);
        }
        goto setRect_tail;
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&frameOffset, &unk1DC);
    {
        s16 deltaX;
        s16 deltaY;

        deltaX = rect.mPos.x - frameOffset.mPos.x;
        deltaY = rect.mPos.y - frameOffset.mPos.y;
        stackPos.x = deltaX;
        stackPos.y = deltaY;
    }
    *(u32*)&unk1DC.unk54 = *(u32*)&stackPos;
    func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(&unk1C8, rect.mSize);

setRect_tail:
    if ((unk278 & 0x10) == 0) {
        func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(&unk1C8, rect.mSize);
    }
}

bool CView::attachRenderWork(CWorkThread* pThread) {
    u32 a0;
    u32 a1;
    u32 a2;
    u32 a3;
    u32 a4;
    u32 a5;
    u32 a6;
    s16 aHalf;
    u8 aByte;
    u32 b0;
    u32 b1;
    u32 b2;
    u32 b3;
    u32 b4;
    u32 b5;
    u32 b6;
    s16 bHalf;
    u8 bByte;
    u32 tag0;
    u32 tag1;
    u32 flag;
    u32 workId;
    s32 sumSigned;
    s32 capSigned;
    s32 slotIndex;
    u32 byteOff;
    u8* ringBase;
    u32 writeIdx;
    u32 prevIdx;
    u32 sumU;
    u32 slotU;
    u8* slot;

    tag0 = 0;
    flag = 3;
    tag1 = 1;

    // Two uninitialized 0x24-byte context payloads on the stack (retail reads
    // them without stores). Tag constants are written separately.
    workId = pThread->mWorkID;

    sumSigned = (s32)unk3F0 + (s32)mContextRingWriteIndex;
    capSigned = (s32)mContextRingCapacity;
    slotIndex = sumSigned / capSigned;
    byteOff = (u32)(sumSigned - slotIndex * capSigned) * 0x24u;
    ringBase = (u8*)mContextRingBase;

    slot = ringBase + byteOff;
    *(u32*)slot = tag0;
    *(u32*)(slot + 0x4) = a0;
    *(u32*)(slot + 0x8) = a1;
    *(u32*)(slot + 0xC) = a2;
    *(u32*)(slot + 0x10) = a3;
    *(u32*)(slot + 0x14) = a4;
    *(u32*)(slot + 0x18) = a5;
    *(u32*)(slot + 0x1C) = a6;
    *(s16*)(slot + 0x20) = aHalf;
    slot[0x22] = aByte;
    slot[0x23] = (u8)tag0;

    writeIdx = mContextRingWriteIndex + 1;
    prevIdx = writeIdx - 1;
    mContextRingWriteIndex = writeIdx;
    unk3FC = prevIdx;

    sumU = unk3F0 + prevIdx;
    slotU = sumU / mContextRingCapacity;
    slot = (u8*)mContextRingBase + (sumU - slotU * mContextRingCapacity) * 0x24u;
    slot[0x23] = (u8)flag;

    sumU = unk3F0 + unk3FC;
    slotU = sumU / mContextRingCapacity;
    slot = (u8*)mContextRingBase + (sumU - slotU * mContextRingCapacity) * 0x24u;
    *(u32*)(slot + 0x4) = workId;

    sumSigned = (s32)unk3F0 + (s32)mContextRingWriteIndex;
    capSigned = (s32)mContextRingCapacity;
    slotIndex = sumSigned / capSigned;
    byteOff = (u32)(sumSigned - slotIndex * capSigned) * 0x24u;
    ringBase = (u8*)mContextRingBase;

    slot = ringBase + byteOff;
    *(u32*)slot = tag1;
    *(u32*)(slot + 0x4) = b0;
    *(u32*)(slot + 0x8) = b1;
    *(u32*)(slot + 0xC) = b2;
    *(u32*)(slot + 0x10) = b3;
    *(u32*)(slot + 0x14) = b4;
    *(u32*)(slot + 0x18) = b5;
    *(u32*)(slot + 0x1C) = b6;
    *(s16*)(slot + 0x20) = bHalf;
    slot[0x22] = bByte;
    slot[0x23] = (u8)tag0;

    writeIdx = mContextRingWriteIndex + 1;
    mContextRingWriteIndex = writeIdx;
    prevIdx = writeIdx - 1;
    unk3FC = prevIdx;

    sumU = unk3F0 + prevIdx;
    slotU = sumU / mContextRingCapacity;
    slot = (u8*)mContextRingBase + (sumU - slotU * mContextRingCapacity) * 0x24u;
    slot[0x23] = (u8)flag;

    sumU = unk3F0 + unk3FC;
    slotU = sumU / mContextRingCapacity;
    slot = (u8*)mContextRingBase + (sumU - slotU * mContextRingCapacity) * 0x24u;
    *(u32*)(slot + 0x4) = (u32)pThread;

    return true;
}

void CView::detachRenderWork(CWorkThread* pThread) {
    unk1DC.detachRenderWork(pThread);
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

    updateMsg__5CViewFv(this);

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

s16 CView::getSplitLine() {
    void* splitFrame = unk45C;
    u32 hasView2;
    u32 hasView1;

    hasView2 = 0;
    hasView1 = 0;
    if (splitFrame == nullptr) {
        goto getSplitLine_no_view1;
    }
    if (getView1__11CSplitFrameFv(splitFrame) == nullptr) {
        goto getSplitLine_no_view1;
    }
    hasView1 = 1;

getSplitLine_no_view1:
    if (hasView1 == 0) {
        goto getSplitLine_no_view2;
    }
    splitFrame = unk45C;
    if (getView2__11CSplitFrameFv(splitFrame) == nullptr) {
        goto getSplitLine_no_view2;
    }
    hasView2 = 1;

getSplitLine_no_view2:
    if (hasView2 != 0) {
        goto getSplitLine_call;
    }
    return 0;

getSplitLine_call:
    splitFrame = unk45C;
    return getSplitLine__11CSplitFrameFv(splitFrame);
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
