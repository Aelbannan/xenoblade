#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/code_801862C0.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

#include <types.h>
#include <decomp.h>

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

// Retail keeps these BDAT entry points as their short/unmangled linker names.
extern "C" u32 getBdatStringColumnValue(void*, const char*, int);

extern void* lbl_eu_8052A3B0[];
extern void* lbl_eu_805294E0[];
} 

namespace {
// Cast-only interfaces retain the retail virtual-call slots without adding
// storage or compiler-owned vtables to CfObjectPoint.
struct CfObjectPointParamIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void setName(const char*);
};

struct CfObjectPointChildIf {
    virtual void _v008(int);
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void setOwner(void*);
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void afterAttach();
};
}

namespace cf {

class __declspec(novtable) CfObjectPoint : public CfObject {
public:
    virtual ~CfObjectPoint();
    void func_800C1324();
    void func_800C136C();
    void func_800C1444();
    void func_800C14CC();
    void func_800C1638();

    // CfObject ends at 0x70
    void* mPtr70;     // 0x70
    u8 _pad74[4];     // 0x74-0x77
    char mName[0x14]; // 0x78-0x8B
    u16 mIndex8C;     // 0x8C-0x8D
    u8 _pad8E[2];     // 0x8E-0x8F
    u8 mFlag90;       // 0x90
    u8 mFlag91;       // 0x91
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
        ((void* (*)(void*, void*))func_80186474)(this, this);
    }
    CfObject_UnkVirtualFunc6();
}

void CfObjectPoint::func_800C1324() {
    CfObject_UnkVirtualFunc66(1);
    mFlag91 = 0;
    mFlag90 = 0;
}

void CfObjectPoint::func_800C136C() {
    void* bdat = CBdat::func_8003AA34();
    void* fp = getFP(mName);
    u32 name = getBdatStringColumnValue(
        bdat, (const char*)lbl_eu_804FC648, mIndex8C);
    CObjectParam_UnkVirtualFunc1(0, 0);
    u32 val = getBdatStringColumnValue(
        fp, (const char*)lbl_eu_804FC648 + 5, mIndex8C);
    mFlag91 = *(u8*)&val;
}

void CfObjectPoint::func_800C1444() {
    if (mSubObj38 != nullptr) {
        CfObjectPoint* child = (CfObjectPoint*)mSubObj38;
        child->CfObject_UnkVirtualFunc23();
        if (mSubObj38 != nullptr) {
            reinterpret_cast<CfObjectPointChildIf*>(mSubObj38)->_v008(1);
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

} // namespace cf

extern "C" void func_800C1658__Q22cf13CfObjectPointFv(
    cf::CfObjectPoint* self, void* child) {
    void* old = self->mSubObj38;
    if (old != nullptr) {
        if (old != nullptr) {
            reinterpret_cast<CfObjectPointChildIf*>(old)->_v008(1);
        }
        self->mSubObj38 = nullptr;
    }

    self->mSubObj38 = child;
    if (child != nullptr) {
        reinterpret_cast<CfObjectPointChildIf*>(child)->setOwner(self);
        reinterpret_cast<CfObjectPointChildIf*>(self->mSubObj38)->afterAttach();
    }
}

extern "C" void func_800C16F4__Q22cf13CfObjectPointFv(
    cf::CfObjectPoint* self, int enable) {
    if (enable != 0) {
        self->mFlags68 |= 0x00100000;
    } else {
        self->mFlags68 = DECOMP_PPC_RLWINM(self->mFlags68, 0, 12, 10);
    }
}

extern "C" void func_800C171C__Q22cf13CfObjectPointFv(
    cf::CfObjectPoint* self, float x, float z) {
    float pos[3];
    float first[3];
    float second[3];
    pos[0] = x;
    pos[1] = lbl_eu_80666B4C;
    pos[2] = z;

    if (lbl_eu_80663E14 == nullptr) {
        void* source = *(void**)((u8*)lbl_eu_80663E14 + 0x74);
        if (func_8049E51C(source) != 0) {
            if (func_804BE398(pos, 0, 0, 0,
                              lbl_eu_80666B50, lbl_eu_80666B48) != nullptr) {
                func_804BE4B4(first, 0);
                func_804BE4E0(second, 0);
                ((u32*)pos)[0] = ((u32*)first)[0];
                ((u32*)pos)[1] = ((u32*)first)[1];
                ((u32*)pos)[2] = ((u32*)first)[2];
            }
        }
    }

    ((u32*)self)[0x3c / 4] = ((u32*)pos)[0];
    ((u32*)self)[0x40 / 4] = ((u32*)pos)[1];
    ((u32*)self)[0x44 / 4] = ((u32*)pos)[2];
}

namespace cf {

} // namespace cf
