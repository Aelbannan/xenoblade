// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectEff
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectEff.hpp"

struct CfObjIf {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void _v0048();
    virtual void _v004C();
    virtual void _v0050();
    virtual void _v0054();
    virtual void _v0058();
    virtual void _v005C();
    virtual void _v0060();
    virtual void _v0064();
    virtual void _v0068();
    virtual void _v006C();
    virtual void _v0070();
    virtual void _v0074();
    virtual void _v0078();
    virtual void _v007C();
    virtual void _v0080();
    virtual void _v0084();
    virtual void _v0088();
    virtual void _v008C();
    virtual void _v0090();
    virtual void _v0094();
    virtual void _v0098();
    virtual void vf009C();
    virtual void _v00A0();
    virtual void _v00A4();
    virtual void _v00A8();
    virtual void _v00AC();
    virtual void _v00B0();
    virtual void vf00B4();
    virtual void _v00B8();
    virtual void _v00BC();
    virtual void _v00C0();
    virtual void vf00C4();
    virtual void _v00C8();
    virtual void _v00CC();
    virtual void vf00D0();
};

namespace cf {
CfObjectEff::CfObjectEff() {}
} // namespace cf

void CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv() {}

namespace cf {
CfObject::~CfObject() {}
} // namespace cf

void CfObject_UnkVirtualFunc6__Q22cf8CfObjectFv() {}

namespace cf {
void CfObjectEff::func_800AC7CC() {}
extern "C" u32 func_800AC7FC__Q22cf11CfObjectEffFv(void* self) { return *(u32*)((u8*)self + 0x94) != 0; }
void CfObjectEff::func_800AC810() {}
} // namespace cf

void CfObjectModel_UnkVirtualFunc18__Q22cf13CfObjectModelFv() {}

namespace cf {
void CfObjectEff::func_800AC990() {}
void CfObjectEff::func_800ACA58() {}
void CfObjectEff::func_800ACAE8() {}
void CfObjectEff::func_800ACB08() {}
void CfObjectEff::func_800ACBA4() {}
void CfObjectEff::func_800ACBCC() {}
} // namespace cf

void func_800ACC14(void* self, unsigned char val) {
    cf::CfObjectEff* obj = static_cast<cf::CfObjectEff*>(self);
    if (obj->mChildEff != nullptr)
        obj->mChildEff->unk59 = val;
}

void func_800ACC28(){}

namespace cf {
extern "C" void func_800ACC3C__Q22cf11CfObjectEffFv(void* self, float v) {
    void* o = *(void**)((u8*)self + 0x94);
    if (o) *(float*)((u8*)o + 0x50) = v;
}
} // namespace cf

extern "C" void func_800ACC50(void* self, float v) {
    void* o = *(void**)((u8*)self + 0x94);
    if (o) *(float*)((u8*)o + 0x50) = v;
}

void func_800ACC64(void* obj, const void* src) {
    cf::CfObjectEff* eff = static_cast<cf::CfObjectEff*>(obj);
    cf::CfObjectEffChild* child = eff->mChildEff;
    if (child == nullptr) return;
    const u32* s = static_cast<const u32*>(src);
    child->unk40[0] = s[0];
    child->unk40[1] = s[1];
    child->unk40[2] = s[2];
    child->unk40[3] = s[3];
}

namespace cf {
void CfObjectEff::func_800ACC94() {}


void CfObjectEff::func_800ACCE4() {}


void CfObjectEff::func_800ACD5C() {}
void CfObjectEff::func_800ACDA0() {}
void CfObjectEff::func_800ACDE0() {}
void CfObjectEff::func_800ACDFC() {}
} // namespace cf

float func_800ACE44__Q22cf11CfObjectEffFv(void* self) {
    cf::CfObjectEff* obj = static_cast<cf::CfObjectEff*>(self);
    if (obj->mChildEff != nullptr) {
        return obj->mChildEff->unk2C;
    }
    return reinterpret_cast<float&>(static_cast<cf::CfObject*>(obj)->mField4C);
}

namespace cf {
void CfObjectEff::func_800ACE60() {}
void CfObjectEff::func_800ACE78() {}
void CfObjectEff::func_800ACEAC() {}
} // namespace cf

void func_800ACEF8(void* obj, const void* src) {
    cf::CfObjectEff* eff = static_cast<cf::CfObjectEff*>(obj);
    if (eff->mFlagsA4 & 0x4) return;
    cf::CfObjectEffChild* child = eff->mChildEff;
    if (child != nullptr) {
        const u32* s = static_cast<const u32*>(src);
        child->unk34[0] = s[0];
        child->unk34[1] = s[1];
        child->unk34[2] = s[2];
    }
    float f = *static_cast<const float*>(src);
    static_cast<cf::CfObject*>(eff)->mFloat60 = f;
}

float func_800ACF34__Q22cf11CfObjectEffFv(char* self) {
    cf::CfObjectEff* obj = reinterpret_cast<cf::CfObjectEff*>(self);
    if (obj->mChildEff != nullptr) {
        return reinterpret_cast<float&>(obj->mChildEff->unk34[1]);
    }
    return static_cast<cf::CfObject*>(obj)->mFloat60;
}

namespace cf {
void CfObjectEff::func_800ACF50() const {}
} // namespace cf

void func_800ACF78(){}
void func_800ACFD8(){}

void func_800AD040(char* obj, int flag) {
    cf::CfObjectEff* eff = reinterpret_cast<cf::CfObjectEff*>(obj);
    if (eff->mChildEff == nullptr) return;
    eff->mChildEff->unk5C = (flag != 0);
}

void func_800AD060(){}

void func_800AD378(void* obj) {
    cf::CfObjectEff* eff = static_cast<cf::CfObjectEff*>(obj);
    if (eff->mCountA6 == 0) return;
    eff->mCountA6--;
    if (eff->mCountA6 != 0) return;
    static_cast<cf::CfObject*>(eff)->mFlags68 |= 0x40;
}

namespace cf {
void CfObjectEff::func_800AD3A4() {}
void CfObjectEff::func_800AD4A4() {}
} // namespace cf

void func_800AD4B0(){}
void func_800AD558(){}
void func_800AD5EC(){}

namespace cf {
void CfObjectEff::func_800AD68C() {}
CfObjectEff::~CfObjectEff() {}
void CfObjectEff::func_800AD818() {}


} // namespace cf

// Forward declarations for symbols used by thunks
void func_800AD68C__Q22cf11CfObjectEffFv();
void __dt__Q22cf11CfObjectEffFv();

void func_800AD850__Q22cf11CfObjectEffFv(void* self) {
    ((void(*)(void*))func_800AD68C__Q22cf11CfObjectEffFv)((char*)self - 0x90);
}

void func_800AD858__Q22cf11CfObjectEffFv(void* self) {
    ((void(*)(void*))__dt__Q22cf11CfObjectEffFv)((char*)self - 0x90);
}

void func_800AD860__FPv(){}

extern "C" void func_800ACCD4__Q22cf11CfObjectEffFv(void* self) { reinterpret_cast<CfObjIf*>(self)->vf009C(); }

extern "C" void func_800ACD4C__Q22cf11CfObjectEffFv(void* self) { reinterpret_cast<CfObjIf*>(self)->vf00B4(); }

extern "C" void func_800AD830__Q22cf11CfObjectEffFv(void* self) { reinterpret_cast<CfObjIf*>(self)->vf00D0(); }

extern "C" void func_800AD840__Q22cf11CfObjectEffFv(void* self) { reinterpret_cast<CfObjIf*>(self)->vf00C4(); }


extern "C" void func_800AC86C__Q22cf11CfObjectEffFv() {}
