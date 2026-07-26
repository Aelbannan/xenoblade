#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__cf_CfObjectEne() {}

extern "C" void __dt__Q22cf13CfObjectActorFv() {}

extern "C" void __dt__Q22cf9CAIActionFv() {}

extern "C" void __dt__800ADA08() {}

extern "C" void __dt__Q22cf11CfObjectEneFv(void* self) {}

extern "C" void func_800ADB2C__Q22cf11CfObjectEneFv(void* self) {}

extern "C" void func_800ADBD4__Q22cf11CfObjectEneFv(void* self) {}

extern "C" void func_800ADDA8__Q22cf11CfObjectEneFv(void* self) {}

extern "C" void CActorParam_UnkVirtualFunc20__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x15E4) = val; }

extern "C" void CActorParam_UnkVirtualFunc18__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x15EC) = val; }

extern "C" void CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv(void* self, float val) { *(float*)((u8*)self + 0x6ec) = val; }

extern "C" void CActorParam_UnkVirtualFunc112__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x161C) = val; }

extern "C" void CActorParam_UnkVirtualFunc114__Q22cf11CActorParamFv(void* self, u8 val) { ((u8*)self)[0x1628] = val; }

extern "C" void CActorParam_UnkVirtualFunc118__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1624) = val; }

extern "C" void CActorParam_UnkVirtualFunc168__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1630) = val; }

extern "C" void CActorParam_UnkVirtualFunc98__Q22cf11CActorParamFv() {}

extern "C" void func_800AEC68__Q22cf11CfObjectEneFv() {}

extern "C" void CActorParam_UnkVirtualFunc166__Q22cf11CfObjectEneFv() {}

extern "C" void CActorParam_UnkVirtualFunc167__Q22cf11CfObjectEneFv() {}

extern "C" void func_800AF7E4() {}

extern "C" void func_800AF82C() {}

extern "C" void func_800AF870() {}

extern "C" int CfObjectActor_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void* self) { return 1; }

extern "C" void CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv(void) {}

extern "C" void CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void) {}

extern "C" void CActorState_UnkVirtualFunc1__Q22cf11CActorStateFv() {}

extern "C" void* CBattleState_UnkVirtualFunc28__Q22cf12CBattleStateFv(void* self, unsigned long index) {
    return (char*)self + (index << 4) + 0x152c;
}

extern "C" void* CBattleState_UnkVirtualFunc27__Q22cf12CBattleStateFv(void* self) { return (void*)((u8*)self + 0x152c); }

extern "C" void* CBattleState_UnkVirtualFunc25__Q22cf12CBattleStateFv(void* self) { return (void*)((u8*)self + 0x8); }

extern "C" void CBattleState_UnkVirtualFunc24__Q22cf12CBattleStateFv() {}

extern "C" void CBattleState_UnkVirtualFunc23__Q22cf12CBattleStateFv(void* self) {
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x44 / 4];
    func(self);
}

extern "C" void CBattleState_UnkVirtualFunc22__Q22cf12CBattleStateFv(void* self) {
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x40 / 4];
    func(self);
}

extern "C" void CBattleState_UnkVirtualFunc21__Q22cf12CBattleStateFv(void* self) {
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x3c / 4];
    func(self);
}

extern "C" void CBattleState_UnkVirtualFunc20__Q22cf12CBattleStateFv(void* self) {
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x38 / 4];
    func(self);
}

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

extern "C" void CActorParam_UnkVirtualFunc148__Q22cf11CActorParamFv() {}

extern "C" void CActorParam_UnkVirtualFunc146__Q22cf11CActorParamFv() {}

extern "C" void* CActorParam_UnkVirtualFunc145__Q22cf11CActorParamFv(void* self, unsigned long idx) {
    return (char*)self + (idx * 0x18) + 0x1928;
}

extern "C" void* CActorParam_UnkVirtualFunc147__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1928); }

extern "C" u32 CActorParam_UnkVirtualFunc139__Q22cf11CActorParamFv(void* self) { return *(u32*)((u8*)self + 0x2A80); }

extern "C" void CActorParam_UnkVirtualFunc134__Q22cf11CActorParamFv() {}

extern "C" u8 CActorParam_UnkVirtualFunc133__Q22cf11CActorParamFv(void* self) { return ((u8*)self)[0x3354]; }

extern "C" void* CActorParam_UnkVirtualFunc131__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x31dc); }

extern "C" void CActorParam_UnkVirtualFunc130__Q22cf11CActorParamFv() {}

extern "C" u32 CActorParam_UnkVirtualFunc128__Q22cf11CActorParamFv(void* self) { return *(u32*)((u8*)self + 0x15E0); }

extern "C" void* CActorParam_UnkVirtualFunc124__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x2740); }

extern "C" void CActorParam_UnkVirtualFunc123__Q22cf11CActorParamFv() {}

extern "C" void* CActorParam_UnkVirtualFunc121__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x19e8); }

extern "C" void CActorParam_UnkVirtualFunc120__Q22cf11CActorParamFv() {}

extern "C" u8 CActorParam_UnkVirtualFunc111__Q22cf11CActorParamFv(void* self) { return ((u8*)self)[0x183C]; }

extern "C" u8 CActorParam_UnkVirtualFunc110__Q22cf11CActorParamFv(void* self) { return ((u8*)self)[0x183B]; }

extern "C" u8 CActorParam_UnkVirtualFunc109__Q22cf11CActorParamFv(void* self) { return ((u8*)self)[0x183A]; }

extern "C" void CActorParam_UnkVirtualFunc108__Q22cf11CActorParamFv(void* self, u8 val) { ((u8*)self)[0x183B] = val; }

extern "C" void CActorParam_UnkVirtualFunc107__Q22cf11CActorParamFv(void* self, u8 val) { ((u8*)self)[0x183A] = val; }

extern "C" void CActorParam_UnkVirtualFunc105__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x15fc) = val; }

extern "C" void* CActorParam_UnkVirtualFunc104__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x15fc); }

extern "C" void CActorParam_UnkVirtualFunc103__Q22cf11CActorParamFv() {}

extern "C" void* CActorParam_UnkVirtualFunc102__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x18d4); }

extern "C" void* CActorParam_UnkVirtualFunc101__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x185c); }

extern "C" void* CActorParam_UnkVirtualFunc99__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x17e4); }

extern "C" void* CActorParam_UnkVirtualFunc97__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1792); }

extern "C" void* CActorParam_UnkVirtualFunc96__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1740); }

extern "C" void* CActorParam_UnkVirtualFunc95__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x16c8); }

extern "C" void* CActorParam_UnkVirtualFunc93__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1650); }

extern "C" void CActorParam_UnkVirtualFunc92__Q22cf11CActorParamFv() {}

extern "C" void CActorParam_UnkVirtualFunc88__Q22cf11CActorParamFv() {}

extern "C" void CActorParam_UnkVirtualFunc84__Q22cf11CActorParamFv() {}

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

extern "C" void CActorParam_UnkVirtualFunc51__Q22cf11CActorParamFv() {}

extern "C" float CActorParam_UnkVirtualFunc50__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x17fc); }

extern "C" float CActorParam_UnkVirtualFunc49__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x17f0); }

extern "C" void CActorParam_UnkVirtualFunc47__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x17f0) = val; }

extern "C" void CActorParam_UnkVirtualFunc45__Q22cf11CActorParamFv() {}

extern "C" float CActorParam_UnkVirtualFunc44__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x17f8); }

extern "C" float CActorParam_UnkVirtualFunc43__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x17ec); }

extern "C" void CActorParam_UnkVirtualFunc42__Q22cf11CActorParamFv() {}

extern "C" void CActorParam_UnkVirtualFunc41__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x17ec) = val; }

extern "C" void CActorParam_UnkVirtualFunc39__Q22cf11CActorParamFv() {}

extern "C" void CActorParam_UnkVirtualFunc36__Q22cf11CActorParamFv() {}

extern "C" void CActorParam_UnkVirtualFunc34__Q22cf11CActorParamFv() {}

extern "C" float CActorParam_UnkVirtualFunc32__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x1660); }

extern "C" void CActorParam_UnkVirtualFunc31__Q22cf11CActorParamFv() {}

extern "C" void CActorParam_UnkVirtualFunc30__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1660) = val; }

extern "C" void CActorParam_UnkVirtualFunc28__Q22cf11CActorParamFv() {}

extern "C" void CActorParam_UnkVirtualFunc27__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x17E4) = val; }

extern "C" void CActorParam_UnkVirtualFunc25__Q22cf11CActorParamFv() {}

extern "C" void CActorParam_UnkVirtualFunc24__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x1650) = val; }

extern "C" float CActorParam_UnkVirtualFunc17__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x15f8); }

extern "C" u8 CActorParam_UnkVirtualFunc15__Q22cf11CActorParamFv(void* self) { return ((u8*)self)[0x15F4]; }

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

extern "C" void CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv(void* self);
extern "C" void CBattleState_UnkVirtualFunc18__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv(void* self);
extern "C" void CBattleState_UnkVirtualFunc17__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv(void* self);
extern "C" void CBattleState_UnkVirtualFunc3__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CBattleState_UnkVirtualFunc1__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CBattleState_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv)((char*)self - 0x8); }

extern "C" void CObjectParam_UnkVirtualFunc4__Q22cf11CfObjectEneFv(void* self) { ((void(*)(void*))func_800ADDA8__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

extern "C" void CfObjectActor_UnkVirtualFunc6__Q22cf13CfObjectActorFv(void* self);
extern "C" void CfObject_UnkVirtualFunc31__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc6__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }


extern "C" void CObjectParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

extern "C" void CfObjectActor_UnkVirtualFunc11__Q22cf13CfObjectActorFv(void* self);
extern "C" void CfObjectMove_UnkVirtualFunc6__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc11__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

extern "C" void CfObjectActor_UnkVirtualFunc10__Q22cf13CfObjectActorFv(void* self);
extern "C" void CfObject_UnkVirtualFunc14__Q22cf13CfObjectActorFv(void* self) { ((void(*)(void*))CfObjectActor_UnkVirtualFunc10__Q22cf13CfObjectActorFv)((char*)self - 0x3e9c); }

extern "C" void CfObject_UnkVirtualFunc4__Q22cf11CfObjectEneFv(void* self) { ((void(*)(void*))func_800ADBD4__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }

extern "C" void func_800B069C__Q22cf11CfObjectEneFv(void* self) { ((void(*)(void*))__dt__Q22cf11CfObjectEneFv)((char*)self - 0x3e9c); }
