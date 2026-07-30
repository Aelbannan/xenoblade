#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/code_801862C0.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

#include <types.h>

extern "C" {
float func_80496288(void*);
u32 func_8049E51C(void*);
void* func_804BE398(float*, u32, u32, u32, float, float);
void func_804BE4B4(float*, u32);
void func_804BE4E0(float*, u32);
u32 func_8013EB90(u32);
extern void* lbl_eu_80663E14;
extern u32 lbl_eu_80663E24;
extern float lbl_eu_80666B48;
extern float lbl_eu_80666B4C;
extern float lbl_eu_80666B50;
extern void* lbl_eu_804FC648;
}

namespace cf {

class CfObjectPoint : public CfObject {
public:
    virtual ~CfObjectPoint();
    void func_800C1324();
    void func_800C136C();
    void func_800C1444();
    void func_800C14CC();
    void func_800C1638();
    void func_800C1658(void* obj);
    void func_800C16F4(u32 val);
    void func_800C171C(float x, float z);

    float mPosX;   // 0x3C
    float mPosY;   // 0x40
    float mPosZ;   // 0x44
    u8 _pad48[0x70 - 0x48];
    void* mPtr70;  // 0x70
    u8 _pad74[0x78 - 0x74];
    char mName[0x14]; // 0x78
    u16 mIndex8C; // 0x8C
    u8 _pad8E[2];
    u8 mFlag90;   // 0x90
    u8 mFlag91;   // 0x91
};

} // namespace cf

extern "C" void func_800C13FC(cf::CfObjectPoint* self, u32 a, u8 val) {
    self->CObjectParam_UnkVirtualFunc1(a, val);
    self->mFlag91 = val;
}

namespace cf {

CfObjectPoint::~CfObjectPoint() {
    if (mFlags68 & 0x40000000) {
        func_801862C0((VMThread*)this);
        func_80186474(this);
    }
}

void CfObjectPoint::func_800C1324() {
    CfObject_UnkVirtualFunc66();
    mFlag91 = 0;
    mFlag90 = 0;
}

void CfObjectPoint::func_800C136C() {
    void* bdat = CBdat::func_8003AA34();
    void* fp = getFP(mName);
    const char* name = CBdat::getBdatStringColumnValue(bdat, (const char*)lbl_eu_804FC648, mIndex8C);
    CObjectParam_UnkVirtualFunc1(0, 0);
    const char* val = CBdat::getBdatStringColumnValue(fp, (const char*)lbl_eu_804FC648 + 5, mIndex8C);
    mFlag91 = *(u8*)val;
}

void CfObjectPoint::func_800C1444() {
    if (mSubObj38 != nullptr) {
        ((CfObject*)mSubObj38)->CfObject_UnkVirtualFunc23();
        if (mSubObj38 != nullptr) {
            ((CfObject*)mSubObj38)->CObjectState_UnkVirtualFunc1(1);
        }
        mSubObj38 = nullptr;
    }
    mPtr70 = nullptr;
    mFlags68 &= 0x40000000;
}

void CfObjectPoint::func_800C14CC() {
    CObjectState_UnkVirtualFunc13();

    if ((lbl_eu_80663E24 & 0x01000000) != 0) {
        if (CObjectState_UnkVirtualFunc8(1) != 0) {
            goto main_body;
        }
        if (CObjectState_UnkVirtualFunc2(1) == 0) {
            goto done;
        }
    }

main_body:
    {
        float dist = func_80496288(lbl_eu_80663E14);
        if (dist > lbl_eu_80666B48) {
            if (CObjectState_UnkVirtualFunc8(1) != 0) {
                if (CObjectState_UnkVirtualFunc2(1) == 0) {
                    if (func_8013EB90(1) == 0) {
                        CObjectState_UnkVirtualFunc7(1);
                    }
                }
            }
        }
    }

    if (CObjectState_UnkVirtualFunc2(1) != 0) {
        CObjectState_UnkVirtualFunc5(1);
        CObjectState_UnkVirtualFunc3(1);
    }

    if (mSubObj38 != nullptr) {
        ((CfObject*)mSubObj38)->CfObject_UnkVirtualFunc21();
    }

done:
    return;
}

void CfObjectPoint::func_800C1638() {
    if (mSubObj38 == nullptr) return;
    ((CfObject*)mSubObj38)->CfObject_UnkVirtualFunc22();
}

void CfObjectPoint::func_800C1658(void* obj) {
    if (mSubObj38 != nullptr) {
        ((CfObject*)mSubObj38)->CObjectState_UnkVirtualFunc1(1);
        mSubObj38 = nullptr;
    }
    mSubObj38 = obj;
    if (obj != nullptr) {
        ((CfObject*)obj)->CObjectState_UnkVirtualFunc10(this);
        ((CfObject*)mSubObj38)->CfObject_UnkVirtualFunc20(0.0f, 0.0f);
    }
}

void CfObjectPoint::func_800C16F4(u32 val) {
    if (val != 0) {
        mFlags68 |= 0x00100000;
    } else {
        mFlags68 &= ~0x00100000u;
    }
}

void CfObjectPoint::func_800C171C(float x, float z) {
    float pos[3];
    pos[0] = x;
    pos[1] = lbl_eu_80666B4C;
    pos[2] = z;

    void* scene = lbl_eu_80663E14;
    if (scene == nullptr) {
        if (func_8049E51C(*(void**)((char*)scene + 0x74)) != 0) {
            if (func_804BE398(pos, 0, 0, 0, lbl_eu_80666B50, lbl_eu_80666B48) != nullptr) {
                float out1[3];
                float out2[3];
                func_804BE4B4(out1, 0);
                func_804BE4E0(out2, 0);
                pos[0] = out1[0];
                pos[1] = out1[1];
                pos[2] = out1[2];
            }
        }
    }

    mPosX = pos[0];
    mPosY = pos[1];
    mPosZ = pos[2];
}

} // namespace cf
