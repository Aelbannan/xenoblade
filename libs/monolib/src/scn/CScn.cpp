#include "monolib/scn/CScn.hpp"
#include "monolib/core/CView.hpp"

extern "C" {
s32 func_8049AF80(UnkScn68* cam);
CView* func_8049B0A0(UnkScn68* cam, s16 idx);
CScn* lbl_eu_80665908;
}

void CScn::addRenderCB(IScnRender* cb, u32 prio, u32 flag) {
    mRenderCBs[mRenderCBCount].cb = cb;
    mRenderCBs[mRenderCBCount].prio = prio;
    mRenderCBs[mRenderCBCount].flag = flag;

    u32 count = mRenderCBCount + 1;
    mRenderCBCount = count;

    s32 i = count - 2;
    CScn* p = (CScn*)((u8*)this + i * sizeof(ScnRenderCB));
    for (; i >= 0; i--) {
        ScnRenderCB& a = p->mRenderCBs[0];
        ScnRenderCB& b = ((CScn*)((u8*)p + sizeof(ScnRenderCB)))->mRenderCBs[0];
        if ((s32)a.prio > (s32)b.prio) {
            IScnRender* tmpCb = a.cb;
            u32 tmpPrio = a.prio;
            u8 tmpFlag = a.flag;

            a.cb = b.cb;
            a.prio = b.prio;
            a.flag = b.flag;

            b.cb = tmpCb;
            b.prio = tmpPrio;
            b.flag = tmpFlag;
        }
        p = (CScn*)((u8*)p - sizeof(ScnRenderCB));
    }
}

void CScn::removeRenderCB(IScnRender* cb) {
    s32 count = mRenderCBCount;
    CScn* p = this;
    u32 i = 0;

    for (; count > 0; count--) {
        if (p->mRenderCBs[0].cb == cb) {
            CScn* slot = (CScn*)((u8*)this + i * sizeof(ScnRenderCB));
            for (; (s32)i < (s32)(mRenderCBCount - 1);) {
                IScnRender* nextCb = slot->mRenderCBs[1].cb;
                i++;
                slot->mRenderCBs[0].cb = nextCb;
                slot->mRenderCBs[0].prio = slot->mRenderCBs[1].prio;
                slot->mRenderCBs[0].flag = slot->mRenderCBs[1].flag;
                slot = (CScn*)((u8*)slot + sizeof(ScnRenderCB));
            }
            mRenderCBCount = mRenderCBCount - 1;
            return;
        }
        p = (CScn*)((u8*)p + sizeof(ScnRenderCB));
        i++;
    }
}

void CScn::Draw() {
    if (func_8049AF80(mCamWork) != 0) {
        UnkScn68* cam = mCamWork;
        CView* view = func_8049B0A0(cam, (s16)cam->unk34);
        if (view == CView::getCurrentView()) {
            if (unk_3E4 == 0) {
                lbl_eu_80665908 = this;

                if (unk_3E5 != 0) {
                    if (unk_3E6 != 0) {
                        mUnk8C->vf18();
                    }
                }

                CScn* p = this;
                s32 i = 0;
                for (; i < (s32)mRenderCBCount;) {
                    if (unk_3E5 != 0 || p->mRenderCBs[0].flag != 0) {
                        reinterpret_cast<IScnRenderDraw*>(p->mRenderCBs[0].cb)->onRender(this);
                    }
                    p = (CScn*)((u8*)p + sizeof(ScnRenderCB));
                    i += 1;
                }

                lbl_eu_80665908 = 0;
                unk_3E8 = 1;
                mUnk8C->vf1C();
            }
        }
    }
    unk_3E8 = 1;
}

