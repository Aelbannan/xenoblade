// Auto-scaffolded catalog TU for kyoshin/CQstLogInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CQstLogInfo.hpp"
#include "kyoshin/code_80135FDC.hpp"

extern void func_80138078(u32);

u8 CQstLogInfo::func_802296D0() { return mField38; }

u8 CQstLogInfo::func_802296D8() { return mField39; }

void CQstLogInfo::func_80229768(u16 val) { mField3A = val; }


void func_802297BC(){}

void func_80229808(){}

void func_80229854(){}

#pragma push
#pragma auto_inline off
void CQstLogInfo::func_802298A0(){
    // Prevent call elimination
    mUnk20 = mUnk20;
}

void CQstLogInfo::func_80229900(){
    // Prevent call elimination
    mUnk20 = mUnk20;
}
#pragma pop

void func_80229960(){}

void func_80229A0C(){}

void func_80229B54(){}

void func_80229CA0(){}

void func_80229CF0(){}

void func_8022A904(){}

void func_8022AFF8(){}

void CQstLogInfo::OnFileEvent() {}

void CQstLogInfo::func_80229600(nw4r::lyt::DrawInfo* drawInfo){
    if (mUnk30 == 0) return;
    func_80137038(mUnk20, drawInfo, 0, 1);
}

void CQstLogInfo::func_802296E0(){
    if (mUnk34 != 0) return;
    mUnk34 = 1;
    mField39 = 0;
    func_802298A0();
    func_80138078(0x6d);
}

void CQstLogInfo::func_80229724(){
    if (mUnk34 != 3) return;
    mUnk34 = 4;
    mField39 = 0;
    func_80229900();
    func_80138078(6);
}
