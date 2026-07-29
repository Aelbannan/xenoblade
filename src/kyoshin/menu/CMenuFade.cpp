#include "kyoshin/menu/CMenuFade.hpp"
#include "monolib/device/CDeviceVI.hpp"

extern int lbl_eu_80663FA0;

CMenuFade::CMenuFade(){

}

CMenuFade::~CMenuFade() {

}

void CMenuFade::Draw() {

}

void CMenuFade::Term() {
    CDeviceVI::waitForDrawDone();
    // IScnRender vtable lives at offset 0x5c — pass the raw address
    mScn->removeRenderCB((IScnRender*)((u8*)this + 0x5c));
    if (mLayout) {
        delete mLayout;
    }
    mLayout = 0;
    mLayoutMem.func_8045F778();
    lbl_eu_80663FA0 = 0;
}

int func_80113E1C() {
    return lbl_eu_80663FA0;
}
u8 func_80113E24(void* pthis) {
    return *(u8*)((char*)pthis + 0x94);
}
// Converted to inline member function in header
void __dt__9CMenuFadeFv(CMenuFade*);
void func_80113E38(CMenuFade* p) {
    __dt__9CMenuFadeFv((CMenuFade*)((char*)p - 0x58));
}
void cbRenderBefore__9CMenuFadeFv(void* self);
void func_80113E40(void* self) { ((void(*)(void*))cbRenderBefore__9CMenuFadeFv)((char*)self - 0x5c); }
void func_80113E48(void* arg0) {
    __dt__9CMenuFadeFv((struct CMenuFade*)((char*)arg0 - 0x5C));
}

void func_80113C84(){}
