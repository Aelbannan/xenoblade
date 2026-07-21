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


// LLM-HARNESS-BEGIN: us-80499ca0
extern "C" void ct_CScn() {}
// LLM-HARNESS-END: us-80499ca0
// LLM-HARNESS-BEGIN: us-80499ed8
extern "C" void func_80495E60() {}
// LLM-HARNESS-END: us-80499ed8
// LLM-HARNESS-BEGIN: us-80499efc
extern "C" bool func_80495E84(void* self) {
    extern bool func_8048CB14(void*);
    return func_8048CB14(*(void**)((char*)self + 0x60));
}
// LLM-HARNESS-END: us-80499efc
// LLM-HARNESS-BEGIN: us-80499f04
extern "C" void* func_80486068(void*, void*, void*, void*, int);

extern "C" void* func_80495E8C(void* a, void* b, void* c, void* d) {
    return func_80486068(a, b, c, d, 0);
}
// LLM-HARNESS-END: us-80499f04
// LLM-HARNESS-BEGIN: us-80499f0c
extern "C" void func_80495E94(s32 param_1, s32 param_2) {
    func_80486068((void*)param_1, (void*)param_2, (void*)-1, (void*)0, 0);
}
// LLM-HARNESS-END: us-80499f0c
// LLM-HARNESS-BEGIN: us-80499f1c
extern "C" int func_80486070(int, int, int);

extern "C" int func_80495EA4(int a, int b) {
    return func_80486070(a, b, 8);
}
// LLM-HARNESS-END: us-80499f1c
// LLM-HARNESS-BEGIN: us-80499f24
extern "C" void __ct__8049E710();
extern "C" void func_80495EAC() {
    __ct__8049E710();
}
// LLM-HARNESS-END: us-80499f24
// LLM-HARNESS-BEGIN: us-80499f28
extern "C" void __ct__804820F8();
extern "C" void func_80495EB0() {
    __ct__804820F8();
}
// LLM-HARNESS-END: us-80499f28
// LLM-HARNESS-BEGIN: us-8049a040
extern "C" void func_80495FC8() {}
// LLM-HARNESS-END: us-8049a040
// LLM-HARNESS-BEGIN: us-8049a054
extern "C" void func_80495FDC() {}
// LLM-HARNESS-END: us-8049a054
// LLM-HARNESS-BEGIN: us-8049a068
extern "C" void func_80495FF0() {}
// LLM-HARNESS-END: us-8049a068
// LLM-HARNESS-BEGIN: us-8049a07c
extern "C" void func_80496004() {}
// LLM-HARNESS-END: us-8049a07c
// LLM-HARNESS-BEGIN: us-8049a090
extern "C" void func_80496018() {}
// LLM-HARNESS-END: us-8049a090
// LLM-HARNESS-BEGIN: us-8049a0a4
extern "C" void func_8049C72C(int);
extern "C" void func_8049602C(void* p) {
    func_8049C72C(*(int*)((u8*)p + 0x80));
}
// LLM-HARNESS-END: us-8049a0a4
// LLM-HARNESS-BEGIN: us-8049a0ac
extern "C" void* func_8049C794(void*);

extern "C" void* func_80496034(void* _this) {
    return func_8049C794(*(void**)((char*)_this + 0x80));
}
// LLM-HARNESS-END: us-8049a0ac
// LLM-HARNESS-BEGIN: us-8049a0b4
extern "C" u32 func_8049603C(void* self) {
    extern u32 func_8049C7A8(u32);
    return func_8049C7A8(*(u32*)((char*)self + 0x80));
}
// LLM-HARNESS-END: us-8049a0b4
// LLM-HARNESS-BEGIN: us-8049a0bc
extern "C" void func_80496044() {}
// LLM-HARNESS-END: us-8049a0bc
// LLM-HARNESS-BEGIN: us-8049a120
extern "C" void func_804960A8() {}
// LLM-HARNESS-END: us-8049a120
// LLM-HARNESS-BEGIN: us-8049a188
extern "C" void* func_8049C7B0(void*);

extern "C" void* func_80496110(void* self) {
    return func_8049C7B0(*(void**)((char*)self + 0x80));
}
// LLM-HARNESS-END: us-8049a188
// LLM-HARNESS-BEGIN: us-8049a190
extern "C" int func_8049AED4(int);

extern "C" int func_80496118(void* _this) {
    return func_8049AED4(*(int*)((char*)_this + 0x68));
}
// LLM-HARNESS-END: us-8049a190
// LLM-HARNESS-BEGIN: us-8049a198
extern "C" void func_80496120() {}
// LLM-HARNESS-END: us-8049a198
// LLM-HARNESS-BEGIN: us-8049a24c
extern "C" void func_804961D4() {}
// LLM-HARNESS-END: us-8049a24c
// LLM-HARNESS-BEGIN: us-8049a294
extern "C" void func_8049621C() {}
// LLM-HARNESS-END: us-8049a294
// LLM-HARNESS-BEGIN: us-8049a2dc
extern "C" int func_8049B158(int);

extern "C" int func_80496264(void* _this) {
    return func_8049B158(*(int*)((char*)_this + 0x68));
}
// LLM-HARNESS-END: us-8049a2dc
// LLM-HARNESS-BEGIN: us-8049a2e4
extern "C" bool func_8049B240(int);
extern "C" bool func_8049626C(void* _this) {
    return func_8049B240(*(int*)((char*)_this + 0x68));
}
// LLM-HARNESS-END: us-8049a2e4
// LLM-HARNESS-BEGIN: us-8049a2ec
extern "C" void* func_8049B2C4(void*);

extern "C" void* func_80496274(void* _this) {
    return func_8049B2C4(*(void**)((char*)_this + 0x68));
}
// LLM-HARNESS-END: us-8049a2ec
// LLM-HARNESS-BEGIN: us-8049a2f4
extern "C" void func_8049627C(void* self, int arg) { func_8049B0A0(*(UnkScn68**)((char*)self + 0x68), (short)arg); }
// LLM-HARNESS-END: us-8049a2f4
// LLM-HARNESS-BEGIN: us-8049a300
extern "C" void func_80496288() {}
// LLM-HARNESS-END: us-8049a300
// LLM-HARNESS-BEGIN: us-8049a370
extern "C" void func_80496294() {}
// LLM-HARNESS-END: us-8049a370
// LLM-HARNESS-BEGIN: us-8049a37c
extern "C" void func_804962A0(void* p, unsigned char v) {
    ((unsigned char*)p)[0x3e5] = v;
}
// LLM-HARNESS-END: us-8049a37c
// LLM-HARNESS-BEGIN: us-8049a384
extern "C" bool func_804962A8() { return false; }
// LLM-HARNESS-END: us-8049a384
// LLM-HARNESS-BEGIN: us-8049aa38
extern "C" void func_8049695C() {}
// LLM-HARNESS-END: us-8049aa38
// LLM-HARNESS-BEGIN: us-8049aa4c
extern "C" void func_80496970() {}
// LLM-HARNESS-END: us-8049aa4c
// LLM-HARNESS-BEGIN: us-8049aa60
extern "C" void func_80496984(u32 value) {
    lbl_eu_80665908 = (CScn *)value;
}
// LLM-HARNESS-END: us-8049aa60
// LLM-HARNESS-BEGIN: us-8049aa68
extern "C" int func_8049698C() {
    return (int)lbl_eu_80665908;
}
// LLM-HARNESS-END: us-8049aa68
// LLM-HARNESS-BEGIN: us-8049aa70
void func_80496994() {}
// LLM-HARNESS-END: us-8049aa70
// LLM-HARNESS-BEGIN: us-8049aa74
void func_80496998() {}
// LLM-HARNESS-END: us-8049aa74
// LLM-HARNESS-BEGIN: us-8049aa78
extern "C" void func_8049699C() {}
// LLM-HARNESS-END: us-8049aa78
// LLM-HARNESS-BEGIN: us-8049ab50
extern "C" void func_eu_8049AB50(void* self, unsigned char byte) {
    *(unsigned char*)((uintptr_t)self + 0x3e9) = byte;
}
// LLM-HARNESS-END: us-8049ab50
// LLM-HARNESS-BEGIN: us-8049abe8
extern "C" void __dt__4CScnFv(void*, int);
extern "C" void func_80496B04(void* pThis, int r4) {
    __dt__4CScnFv((char*)pThis - 0x54, r4);
}
// LLM-HARNESS-END: us-8049abe8
