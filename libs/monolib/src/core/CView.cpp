#include "monolib/core.hpp"
#include "monolib/core/CViewRectData.hpp"
#include "monolib/device.hpp"
#include "monolib/work/CWorkThread.hpp"

extern "C" {
void getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame();
CViewRoot* getInstance__9CViewRootFv();
GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
void func_804592F0__17UnkClass_8043C59CFv();
void func_80459384__17UnkClass_8043C59CFv();
}

static CView* spCurrentView;
ml::CCol4 CView::sFrameColor;

void CView::setDefaultFrameColor(const ml::CCol4& color) {
    sFrameColor = color;
}

CView* CView::getCurrentView() {
    return spCurrentView;
}

asm void CView::setCurrent() {
    stwu r1, -0x40(r1)
    li r0, 0
    stw r31, 0x3c(r1)
    li r31, 6
    lwz r12, 0xc(r1)
    stw r30, 0x38(r1)
    lwz r11, 0x10(r1)
    stw r29, 0x34(r1)
    lwz r10, 0x14(r1)
    stw r28, 0x30(r1)
    lwz r9, 0x18(r1)
    lwz r6, 0x3f0(r3)
    lwz r4, 0x3f4(r3)
    lwz r5, 0x3f8(r3)
    add r29, r6, r4
    lwz r28, 0x3ec(r3)
    divw r4, r29, r5
    lwz r8, 0x1c(r1)
    lwz r7, 0x20(r1)
    lwz r6, 0x24(r1)
    mullw r30, r4, r5
    lhz r5, 0x28(r1)
    lbz r4, 0x2a(r1)
    subf r30, r30, r29
    mulli r30, r30, 0x24
    stwux r31, r28, r30
    stw r12, 4(r28)
    stw r11, 8(r28)
    stw r10, 0xc(r28)
    stw r9, 0x10(r28)
    stw r8, 0x14(r28)
    stw r7, 0x18(r28)
    stw r6, 0x1c(r28)
    sth r5, 0x20(r28)
    stb r4, 0x22(r28)
    stb r0, 0x23(r28)
    lwz r4, 0x3f4(r3)
    addi r4, r4, 1
    stw r4, 0x3f4(r3)
    subi r0, r4, 1
    stw r0, 0x3fc(r3)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    lwz r28, 0x30(r1)
    addi r1, r1, 0x40
    blr
}

asm void CView::setRect(const ml::CRect16& rect) {
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    mr r31, r4
    stw r30, 0x28(r1)
    mr r30, r3
    stw r29, 0x24(r1)
    lwz r0, 0x278(r3)
    clrlwi. r0, r0, 31
    beq setRect_normal
    addi r3, r1, 0xc
    addi r4, r30, 0x1dc
    bl getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame
    lha r3, 0xc(r1)
    lha r0, 0xe(r1)
    neg r3, r3
    sth r3, 0x10(r1)
    neg r0, r0
    lwz r29, 0x58(r30)
    sth r0, 0x12(r1)
    lwz r0, 0x10(r1)
    stw r0, 0x230(r30)
    bl getInstance__9CViewRootFv
    cmplw r3, r29
    bne setRect_parent
    li r4, 0
    b setRect_parent_check
setRect_parent:
    lwz r4, 0x58(r30)
    cmpwi r4, 0
    bne setRect_type
    li r4, 0
    b setRect_parent_check
setRect_type:
    lwz r0, 0x50(r4)
    cmpwi r0, 0x30
    blt setRect_parent_null
    cmpwi r0, 0x35
    blt setRect_parent_check
setRect_parent_null:
    li r4, 0
setRect_parent_check:
    cmpwi r4, 0
    beq setRect_mode
    addi r3, r30, 0x1c8
    addi r4, r4, 0x1cc
    bl func_804592F0__17UnkClass_8043C59CFv
    b setRect_tail
setRect_mode:
    bl getRenderModeObj__9CDeviceVIFv
    lhz r29, 0x6(r3)
    bl getRenderModeObj__9CDeviceVIFv
    lhz r0, 0x4(r3)
    addi r3, r30, 0x1c8
    sth r0, 0x8(r1)
    addi r4, r1, 0x8
    sth r29, 0xa(r1)
    bl func_804592F0__17UnkClass_8043C59CFv
    b setRect_tail
setRect_normal:
    addi r3, r1, 0x14
    addi r4, r30, 0x1dc
    bl getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame
    lha r7, 0x14(r1)
    addi r3, r30, 0x1c8
    lha r6, 0(r31)
    addi r4, r31, 4
    lha r5, 0x16(r1)
    lha r0, 2(r31)
    subf r6, r7, r6
    sth r6, 0x18(r1)
    subf r0, r5, r0
    sth r0, 0x1a(r1)
    lwz r0, 0x18(r1)
    stw r0, 0x230(r30)
    bl func_804592F0__17UnkClass_8043C59CFv
setRect_tail:
    lwz r0, 0x278(r30)
    rlwinm. r0, r0, 0, 27, 27
    bne setRect_epilogue
    addi r3, r30, 0x1c8
    addi r4, r31, 4
    bl func_80459384__17UnkClass_8043C59CFv
setRect_epilogue:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
}

void CView::detachRenderWork(CWorkThread* pThread) {
    unk1DC.detachRenderWork(pThread);
}
