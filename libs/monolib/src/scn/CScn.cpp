// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).
struct VTarget {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual ~VTarget() {}
};

#include "monolib/scn/CScn.hpp"
#include "monolib/core/CView.hpp"

// --- CTTask<CScn> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move__13CTTask<4CScn>Fv /
// Draw__13CTTask<4CScn>Fv / __dt__13CTTask<4CScn>Fv symbols.
template<>
void CTTask<CScn>::Move() {
    if (mMoveFunc) {
        (static_cast<CScn*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CScn>::Draw() {
    if (mDrawFunc) {
        (static_cast<CScn*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<CScn>::~CTTask() {}

extern "C" {
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

// CScn::Init() = empty (retail blr)
void CScn::Init() {}

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


extern "C" bool func_80495E60(u8* self) {
    extern bool func_8048C8C4(void*, void*);
    if (!self) return 1;
    return func_8048C8C4(*(void**)((char*)*(void**)(self + 4) + 0x60), self);
}
extern "C" bool func_80495E84(u8* self) {
    extern bool func_8048CB14(void*);
    return func_8048CB14(*(void**)((char*)self + 0x60));
}
extern "C" void* func_80495E8C(void* a, void* b, void* c, void* d) {
    return func_80486068(a, b, c, d, 0);
}
extern "C" void func_80495E94(s32 param_1, s32 param_2) {
    func_80486068((void*)param_1, (void*)param_2, (void*)-1, (void*)0, 0);
}
extern "C" int func_80495EA4(int a, int b) {
    return func_80486070(a, b, 8);
}
extern "C" void func_80495EAC() {
    __ct__8049E710();
}
extern "C" void func_80495EB0() {
    __ct__804820F8();
}
extern "C" void func_80495FC8(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 140);
    obj->v6();
}
extern "C" void func_80495FDC(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 140);
    obj->v7();
}
extern "C" void func_80495FF0(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 140);
    obj->v9();
}
extern "C" void func_80496004(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 140);
    obj->v10();
}
extern "C" void func_80496018(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 140);
    obj->v8();
}
extern "C" void func_8049602C(void* p) {
    func_8049C72C(*(int*)((u8*)p + 0x80));
}
extern "C" void* func_80496034(void* _this) {
    return func_8049C794(*(void**)((char*)_this + 0x80));
}
extern "C" u32 func_8049603C(u8* self) {
    extern u32 func_8049C7A8(u32);
    return func_8049C7A8(*(u32*)((char*)self + 0x80));
}
void func_80496044(){}
void func_804960A8(){}
extern "C" void* func_80496110(u8* self) {
    return func_8049C7B0(*(void**)((char*)self + 0x80));
}
extern "C" int func_80496118(void* _this) {
    return func_8049AED4(*(int*)((char*)_this + 0x68));
}
void func_80496120(){}
// Get the camera item handle, lazily creating id -1 when missing; returns
// the item payload +0x9C (the sibling +0xCC variant mirrors it).
extern "C" void* func_8049B1CC(void*);
extern "C" void* func_804961D4(void* self) {
    void* r = func_8049B1CC(*(void**)((u8*)self + 0x68));
    if (!r)
        r = ((void* (*)(void*, int))func_8049B1CC)(*(void**)((u8*)self + 0x68), -1);
    return (u8*)r + 0x9C;
}

// Sibling with the +0xCC payload offset.
extern "C" void* func_8049621C(void* self) {
    void* r = func_8049B1CC(*(void**)((u8*)self + 0x68));
    if (!r)
        r = ((void* (*)(void*, int))func_8049B1CC)(*(void**)((u8*)self + 0x68), -1);
    return (u8*)r + 0xCC;
}
extern "C" int func_80496264(void* _this) {
    return func_8049B158(*(int*)((char*)_this + 0x68));
}
extern "C" bool func_8049626C(void* _this) {
    return func_8049B240(*(int*)((char*)_this + 0x68));
}
extern "C" void* func_80496274(void* _this) {
    return func_8049B2C4(*(void**)((char*)_this + 0x68));
}
extern "C" void func_8049627C(u8* self, int arg) { func_8049B0A0(*(UnkScn68**)((char*)self + 0x68), (short)arg); }
void func_80496288(){}
extern "C" void func_80496294() {}
extern "C" void func_804962A0(void* p, unsigned char v) {
    ((unsigned char*)p)[0x3e5] = v;
}
// retail: stb r4,0x3e6(r3); blr
extern "C" void func_804962A8(u8* self, u8 v) { self[0x3E6] = v; }
extern "C" void func_8049695C(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 104);
    obj->v3();
}
extern "C" void func_80496970(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 96);
    obj->v4();
}
extern "C" void func_80496984(u32 value) {
    lbl_eu_80665908 = (CScn *)value;
}
extern "C" int func_8049698C() {
    return (int)lbl_eu_80665908;
}
extern "C" void func_8049B3FC();
extern "C" void func_8049B408();
void func_80496994__Fv(void) { func_8049B3FC(); }
void func_80496998__Fv(void) { func_8049B408(); }
// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).


extern "C" void func_eu_8049AB50(u8* self, unsigned char byte) {
    *(unsigned char*)((uintptr_t)self + 0x3e9) = byte;
}
void __dt__4CScnFv(void*, int);
extern "C" void func_80496B04(void* pThis, int r4) {
    __dt__4CScnFv((char*)pThis - 0x54, r4);
}

extern "C" void func_8049699C(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 0x8C);
    obj->v11();
}


extern "C" void func_804962B0__4CScnFUlUlPvUl() {}
extern "C" void Term__4CScnFv() {}
extern "C" void Move__4CScnFv() {}
extern "C" void create__8CScnNw4rFv() {}
