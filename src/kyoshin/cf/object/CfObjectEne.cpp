#include "kyoshin/harness_catalog.hpp"
#include <types.h>
#include "kyoshin/cf/object/CfObjectEne.hpp"


// Copy block: 0x00-0x78 (120 bytes), loaded from r4 then stored to self+0x17E4/0x1650
struct ParamCopyBlock {
    u32 f00;
    f32 f04, f08, f0C, f10, f14, f18;
    s16 f1C, f1E, f20;
    f32 f24, f28;
    s16 h2C, h2E, h30, h32, h34, h36, h38, h3A;
    u8 f3C;
    f32 f40, f44, f48, f4C, f50;
    u8 f54, f55, f56, f57, f58;
    f32 f5C;
    u32 f60, f64, f68, f6C, f70, f74;
};


struct BSIf {
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
    virtual void vf0038();
    virtual void vf003C();
    virtual void vf0040();
    virtual void vf0044();
};

void __ct__cf_CfObjectEne(){}

cf::CfObjectActor::~CfObjectActor() {}

cf::CAIAction::~CAIAction() {}

void __dt__800ADA08(){}

cf::CfObjectEne::~CfObjectEne() {}

void cf::CfObjectEne::func_800ADB2C() {}

void cf::CfObjectEne::func_800ADBD4() {}

void cf::CfObjectEne::func_800ADDA8() {}

void CActorParam_UnkVirtualFunc20__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x15E4) = val; }

void CActorParam_UnkVirtualFunc18__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x15EC) = val; }

void CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv(void* self, float val) { *(float*)((u8*)self + 0x6ec) = val; }

void CActorParam_UnkVirtualFunc112__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x161C) = val; }

void CActorParam_UnkVirtualFunc114__Q22cf11CActorParamFv(void* self, u8 val) { ((cf::CActorParam*)self)->unk1628 = val; }

void CActorParam_UnkVirtualFunc118__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1624) = val; }

void CActorParam_UnkVirtualFunc168__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1630) = val; }

void cf::CActorParam::CActorParam_UnkVirtualFunc98() {}

void cf::CfObjectEne::func_800AEC68() {}

void cf::CfObjectEne::CActorParam_UnkVirtualFunc166() {}

void cf::CfObjectEne::CActorParam_UnkVirtualFunc167() {}

void func_800AF7E4(){}

void func_800AF82C(){}

void func_800AF870(){}

int CfObjectActor_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void* self) { return 1; }

extern "C" void CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv(void) {}

extern "C" void CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void) {}

struct If38 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void vf38();
};
extern "C" void CActorState_UnkVirtualFunc1__Q22cf11CActorStateFv(void* self) {
    reinterpret_cast<If38*>(*(void**)((u8*)self + 4))->vf38();
}

extern "C" void* CBattleState_UnkVirtualFunc28__Q22cf12CBattleStateFv(void* self, unsigned long index) {
    return ((cf::CBattleState*)self)->unk152C + (index << 4);
}

extern "C" void* CBattleState_UnkVirtualFunc27__Q22cf12CBattleStateFv(void* self) { return (void*)((u8*)self + 0x152c); }

extern "C" void* CBattleState_UnkVirtualFunc25__Q22cf12CBattleStateFv(void* self) { return (void*)((u8*)self + 0x8); }

void cf::CBattleState::CBattleState_UnkVirtualFunc24() {}





extern "C" void CActorParam_UnkVirtualFunc181__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x2A80) = val; }

extern "C" int CActorParam_UnkVirtualFunc178__Q22cf11CActorParamFv(void* self) { return 0; }

extern "C" void CActorParam_UnkVirtualFunc173__Q22cf11CActorParamFv(void) {}

extern "C" u32 CActorParam_UnkVirtualFunc172__Q22cf11CActorParamFv(void* self) { return *(u32*)((u8*)self + 0x3370); }

extern "C" float CActorParam_UnkVirtualFunc171__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x1630); }

extern "C" float CActorParam_UnkVirtualFunc170__Q22cf11CActorParamFv(void* self) {
    extern float lbl_eu_8066A1F8;
    return lbl_eu_8066A1F8 - *(float*)((char*)self + 0x1630);
}

extern "C" float CActorParam_UnkVirtualFunc169__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x1630); }

extern "C" void* CActorParam_UnkVirtualFunc164__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x164a); }

extern "C" void* CActorParam_UnkVirtualFunc163__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1648); }

extern "C" u32 CActorParam_UnkVirtualFunc162__Q22cf11CActorParamFv(void* self) { return *(u32*)((u8*)self + 0x336C); }

extern "C" void CActorParam_UnkVirtualFunc161__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x336C) = val; }

extern "C" void* CActorParam_UnkVirtualFunc151__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x3358); }

void cf::CActorParam::CActorParam_UnkVirtualFunc148() {}

void cf::CActorParam::CActorParam_UnkVirtualFunc146() {}

extern "C" void* CActorParam_UnkVirtualFunc145__Q22cf11CActorParamFv(void* self, unsigned long idx) {
    return &((cf::CActorParam*)self)->unk1928[idx];
}

extern "C" void* CActorParam_UnkVirtualFunc147__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1928); }

extern "C" u32 CActorParam_UnkVirtualFunc139__Q22cf11CActorParamFv(void* self) { return *(u32*)((u8*)self + 0x2A80); }

void cf::CActorParam::CActorParam_UnkVirtualFunc134() {}

extern "C" u8 CActorParam_UnkVirtualFunc133__Q22cf11CActorParamFv(void* self) { return ((cf::CActorParam*)self)->unk3354; }

extern "C" void* CActorParam_UnkVirtualFunc131__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x31dc); }

extern "C" void* CActorParam_UnkVirtualFunc130__Q22cf11CActorParamFv(void* self, unsigned long index) { return (u8*)self + index * 0xBC + 0x2A84; }

extern "C" u32 CActorParam_UnkVirtualFunc128__Q22cf11CActorParamFv(void* self) { return *(u32*)((u8*)self + 0x15E0); }

extern "C" void* CActorParam_UnkVirtualFunc124__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x2740); }

void cf::CActorParam::CActorParam_UnkVirtualFunc123() {}

extern "C" void* CActorParam_UnkVirtualFunc121__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x19e8); }

void cf::CActorParam::CActorParam_UnkVirtualFunc120() {}

extern "C" u8 CActorParam_UnkVirtualFunc111__Q22cf11CActorParamFv(void* self) { return ((cf::CActorParam*)self)->unk17E4.unk50[8]; }

extern "C" u8 CActorParam_UnkVirtualFunc110__Q22cf11CActorParamFv(void* self) { return ((cf::CActorParam*)self)->unk17E4.unk50[7]; }

extern "C" u8 CActorParam_UnkVirtualFunc109__Q22cf11CActorParamFv(void* self) { return ((cf::CActorParam*)self)->unk17E4.unk50[6]; }

extern "C" void CActorParam_UnkVirtualFunc108__Q22cf11CActorParamFv(void* self, u8 val) { ((cf::CActorParam*)self)->unk17E4.unk50[7] = val; }

extern "C" void CActorParam_UnkVirtualFunc107__Q22cf11CActorParamFv(void* self, u8 val) { ((cf::CActorParam*)self)->unk17E4.unk50[6] = val; }

extern "C" void CActorParam_UnkVirtualFunc105__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x15fc) = val; }

extern "C" void* CActorParam_UnkVirtualFunc104__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x15fc); }

void cf::CActorParam::CActorParam_UnkVirtualFunc103() {}

extern "C" void* CActorParam_UnkVirtualFunc102__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x18d4); }

extern "C" void* CActorParam_UnkVirtualFunc101__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x185c); }

extern "C" void* CActorParam_UnkVirtualFunc99__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x17e4); }

extern "C" void* CActorParam_UnkVirtualFunc97__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1792); }

extern "C" void* CActorParam_UnkVirtualFunc96__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1740); }

extern "C" void* CActorParam_UnkVirtualFunc95__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x16c8); }

extern "C" void* CActorParam_UnkVirtualFunc93__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1650); }

void cf::CActorParam::CActorParam_UnkVirtualFunc92() {}

void cf::CActorParam::CActorParam_UnkVirtualFunc88() {}

extern "C" u32 CActorParam_UnkVirtualFunc84__Q22cf11CActorParamFv(void* self) { return *(u32*)((u8*)self + 0x1600) + *(u32*)((u8*)self + 0x1604); }

extern "C" void* CActorParam_UnkVirtualFunc80__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1834); }

extern "C" void* CActorParam_UnkVirtualFunc79__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x182c); }

extern "C" void CActorParam_UnkVirtualFunc78__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1834) = val; }

extern "C" void CActorParam_UnkVirtualFunc77__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x182c) = val; }

extern "C" void* CActorParam_UnkVirtualFunc75__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1828); }

extern "C" void CActorParam_UnkVirtualFunc73__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1828) = val; }

extern "C" float CActorParam_UnkVirtualFunc72__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x1824); }

extern "C" void CActorParam_UnkVirtualFunc71__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1824) = val; }

extern "C" float CActorParam_UnkVirtualFunc69__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x1618); }

extern "C" float CActorParam_UnkVirtualFunc66__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x1610); }

extern "C" s16 CActorParam_UnkVirtualFunc63__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x1616); }

extern "C" s16 CActorParam_UnkVirtualFunc62__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x1614); }

extern "C" void CActorParam_UnkVirtualFunc59__Q22cf11CActorParamFv(void* self, u16 val) { *(u16*)((u8*)self + 0x1614) = val; }

extern "C" s16 CActorParam_UnkVirtualFunc57__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x160e); }

extern "C" s16 CActorParam_UnkVirtualFunc56__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x160c); }

extern "C" void CActorParam_UnkVirtualFunc53__Q22cf11CActorParamFv(void* self, u16 val) { *(u16*)((u8*)self + 0x160C) = val; }

extern "C" float CActorParam_UnkVirtualFunc51__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x17F0) / *(float*)((u8*)self + 0x17FC); }

extern "C" float CActorParam_UnkVirtualFunc50__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x17fc); }

extern "C" float CActorParam_UnkVirtualFunc49__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x17f0); }

extern "C" void CActorParam_UnkVirtualFunc47__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x17f0) = val; }

extern "C" float CActorParam_UnkVirtualFunc45__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x17EC) / *(float*)((u8*)self + 0x17F8); }

extern "C" float CActorParam_UnkVirtualFunc44__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x17f8); }

extern "C" float CActorParam_UnkVirtualFunc43__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x17ec); }

void cf::CActorParam::CActorParam_UnkVirtualFunc42() {}

extern "C" void CActorParam_UnkVirtualFunc41__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x17ec) = val; }

extern "C" float CActorParam_UnkVirtualFunc39__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x17E8) / *(float*)((u8*)self + 0x17F4); }

void cf::CActorParam::CActorParam_UnkVirtualFunc36() {}

void cf::CActorParam::CActorParam_UnkVirtualFunc34() {}

extern "C" float CActorParam_UnkVirtualFunc32__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x1660); }

void cf::CActorParam::CActorParam_UnkVirtualFunc31() {}

extern "C" void CActorParam_UnkVirtualFunc30__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1660) = val; }

extern "C" void CActorParam_UnkVirtualFunc28__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x17E4) += val; }

extern "C" void CActorParam_UnkVirtualFunc27__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x17E4) = val; }

extern "C" void CActorParam_UnkVirtualFunc25__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x1650) += val; }

extern "C" void CActorParam_UnkVirtualFunc24__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x1650) = val; }

extern "C" float CActorParam_UnkVirtualFunc17__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x15f8); }

extern "C" u8 CActorParam_UnkVirtualFunc15__Q22cf11CActorParamFv(void* self) { return ((cf::CActorParam*)self)->unk15F4[0]; }

extern "C" u32 CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv(void* self) { return (*(u32*)((u8*)self + 104) >> 21) & 0x1u; }

extern "C" u32 CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv(void* self) { return *(u32*)((u8*)self + 0xB4); }

extern "C" float CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv(void* self) { return *(float*)((u8*)self + 0xac); }

extern "C" float CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv(void* self) { return *(float*)((u8*)self + 0xa8); }

extern "C" u32 CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv(void* self) { return *(u32*)((u8*)self + 0x704); }

extern "C" u32 CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(void* self) { return *(u32*)((u8*)self + 0x708); }

extern "C" void CfObjectActor_UnkVirtualFunc13__Q22cf13CfObjectActorFv(void* self) {
    *(long*)((char*)self + 0x45bc) = -1;
}

extern "C" u32 CfObjectActor_UnkVirtualFunc12__Q22cf13CfObjectActorFv(void* self) { return *(u32*)((u8*)self + 0x45BC); }

extern "C" void CBattleState_UnkVirtualFunc18__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CBattleState_UnkVirtualFunc17__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CBattleState_UnkVirtualFunc3__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CBattleState_UnkVirtualFunc1__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CBattleState_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CObjectParam_UnkVirtualFunc4__Q22cf11CfObjectEneFv(void* self) { ((void(*)(void*))func_800ADDA8__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

extern "C" void CfObject_UnkVirtualFunc31__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc6__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }


extern "C" void CObjectParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

extern "C" void CfObjectMove_UnkVirtualFunc6__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc11__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

extern "C" void CfObject_UnkVirtualFunc14__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc10__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

extern "C" void CfObject_UnkVirtualFunc4__Q22cf11CfObjectEneFv(void* self) { ((void(*)(void*))func_800ADBD4__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

extern "C" void func_800B069C__Q22cf11CfObjectEneFv(void* self) { ((void(*)(void*))__dt__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

extern "C" void CBattleState_UnkVirtualFunc23__Q22cf12CBattleStateFv(void* self) { reinterpret_cast<BSIf*>(self)->vf0044(); }

extern "C" void CBattleState_UnkVirtualFunc22__Q22cf12CBattleStateFv(void* self) { reinterpret_cast<BSIf*>(self)->vf0040(); }

extern "C" void CBattleState_UnkVirtualFunc21__Q22cf12CBattleStateFv(void* self) { reinterpret_cast<BSIf*>(self)->vf003C(); }

extern "C" void CBattleState_UnkVirtualFunc20__Q22cf12CBattleStateFv(void* self) { reinterpret_cast<BSIf*>(self)->vf0038(); }

extern "C" void CActorParam_UnkVirtualFunc98__Q22cf11CActorParamFv(void* self, const void* src) {
    *(ParamCopyBlock*)((u8*)self + 0x17e4) = *(const ParamCopyBlock*)src;
}

extern "C" void CActorParam_UnkVirtualFunc92__Q22cf11CActorParamFv(void* self, const void* src) {
    *(ParamCopyBlock*)((u8*)self + 0x1650) = *(const ParamCopyBlock*)src;
}
