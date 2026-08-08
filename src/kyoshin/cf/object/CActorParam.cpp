#include "kyoshin/cf/object/CActorParam.hpp"
#include "monolib/math.hpp"

struct APIf {
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
    virtual void vf0040();
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
    virtual void _v009C();
    virtual void _v00A0();
    virtual void _v00A4();
    virtual void _v00A8();
    virtual void _v00AC();
    virtual void _v00B0();
    virtual void _v00B4();
    virtual void _v00B8();
    virtual void _v00BC();
    virtual void _v00C0();
    virtual void _v00C4();
    virtual void _v00C8();
    virtual void _v00CC();
    virtual void _v00D0();
    virtual void _v00D4();
    virtual void _v00D8();
    virtual void _v00DC();
    virtual void _v00E0();
    virtual void _v00E4();
    virtual void _v00E8();
    virtual void _v00EC();
    virtual void _v00F0();
    virtual void _v00F4();
    virtual void _v00F8();
    virtual void _v00FC();
    virtual void _v0100();
    virtual void _v0104();
    virtual void _v0108();
    virtual void _v010C();
    virtual void _v0110();
    virtual void _v0114();
    virtual void _v0118();
    virtual void vf011C(float a);
    virtual void _v0120();
    virtual void _v0124();
    virtual void _v0128();
    virtual void _v012C();
    virtual void _v0130();
    virtual void _v0134();
    virtual void _v0138();
    virtual void vf013C(float a);
    virtual void _v0140();
    virtual void _v0144();
    virtual void _v0148();
    virtual void _v014C();
    virtual void _v0150();
    virtual void vf0154(float a);
};

namespace cf {
    CActorParam::CActorParam(UNKTYPE* r4, UNKTYPE* r5) : CActorState(r4),
    unk15DC(r4), unk15E0(reinterpret_cast<cf::UnkClass_CActorParam15E0*>(r5)), unk1648(0), unk164A(0), unk164C(0) {
        *(u32*)((u32)unk3298.unk8 + 4) = 0;
        std::memset(unk335C, 0, sizeof(unk335C));
        *(u32*)((u32)unk3298.unk8 + 8) = 0;
        unk3358 = 0;
        unk335A = 2;
        
        float idk = 2.0f * (ml::pi/3.0f);

        unk3364 = 0;
        unk3368 = 0;
        unk3374 = 0;
        
        unk15FC = 0;
        unk15E4 = 2;
        unk15E8 = 1;
        unk15EC = 0;
        unk15F0 = 0;
        unk1600 = 0;
        unk1604 = 0;
        unk1608 = 0;
        unk1610 = 0;
        unk160E = 0;
        unk160C = 0;
        unk1618 = 0;
        unk1616 = 0; //r0
        unk1614 = 0; //r0
        unk1650.unk44 = 2;
        unk1650.unk4C = idk;
        unk161C = 0;
        unk1620 = 40;
        unk1624 = idk;
        unk1628 = 0;
        unk1630 = 3;

        unk2A80 = 0;
        unk336C = 0;

        unk1629 = 0;
        unk162A = 0;
        unk162B = 0;
        unk162C = 0;
        unk1634 = 0;
        unk1638 = 0;
        unk163C = 0;
        unk1640 = 0;
        unk1644 = 0;

        unk3370 = 0;
        
        unk3298.init();
        std::memset(unk2A84, 0, sizeof(unk2A84)); //lol sure, go ahead and wipe everything
        unk31DC.init();

        unk3354 = 0;
        unk337C = 0;

        for(int i = 0; i < ARRAY_SIZE(unk1928); i++){
            unk1928[i].init();
        }
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc1() {
    ((void (*)(void*))(*(void***)this->unk15DC)[0x10])(this->unk15DC);
}
int CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv(void* self) { return 0x0; }
struct If11C {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
    virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
    virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
    virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4();
    virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4();
    virtual void _v0F8(); virtual void _v0FC(); virtual void _v100(); virtual void _v104();
    virtual void _v108(); virtual void _v10C(); virtual void _v110(); virtual void _v114();
    virtual void _v118();
    virtual void vf11C();
};
extern "C" void CActorParam_UnkVirtualFunc35__Q22cf11CActorParamFv(void* self) {
    reinterpret_cast<If11C*>(self)->vf11C();
}
void cf::CActorParam::CActorParam_UnkVirtualFunc54() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc60() {}
void CActorParam_UnkVirtualFunc58__Q22cf11CActorParamFv() {}
void CActorParam_UnkVirtualFunc64__Q22cf11CActorParamFv() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc141() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc142() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc143() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc144() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc149() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc150() {}
s16 CActorParam_UnkVirtualFunc155__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x3358); }
s16 CActorParam_UnkVirtualFunc157__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x335a); }
void cf::CActorParam::CActorParam_UnkVirtualFunc160() {}
float CActorParam_UnkVirtualFunc23__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x15e8); }
void CActorParam_UnkVirtualFunc21__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x15e8) = val; }

void func_80175A50(){}
void func_801765A4(){}
void cf::CActorParam::CActorParam_UnkVirtualFunc177() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc4() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc174() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc175() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc176() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc10() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc11() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc12() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc13() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc179() {}
// Forward declaration - body kept in separate section
void CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv();
void cf::CActorParam::CActorParam_UnkVirtualFunc5() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc6() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc9() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc48() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc67() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc70() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc7() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc8() {}
bool cf::CActorParam::CActorParam_UnkVirtualFunc138() { return false; }
void cf::CActorParam::CActorParam_UnkVirtualFunc140() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc153() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc154() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc156() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc158() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc159() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc86() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc87() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc135() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc136() {}
void cf::CActorParam::CActorParam_UnkVirtualFunc137() {}

// BattleState thunks: adjust `this` by -8 and tail-call CActorParam methods.
typedef void (*CActorFn)(void*);

void CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv();
void CBattleState_UnkVirtualFunc18__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv)((void*)((char*)self - 8));
}

// us-80180170
void CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv();
void CBattleState_UnkVirtualFunc17__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv)((void*)((char*)self - 8));
}

// us-80180178
extern "C" void CActorParam_UnkVirtualFunc1__Q22cf11CActorParamFv(void* self) { reinterpret_cast<APIf*>(*(void**)((u8*)self + 0x15dc))->vf0040(); }

// us-80180180
int CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv(void* self);
extern "C" int CBattleState_UnkVirtualFunc1__Q22cf11CActorParamFv(void* self) {
    return ((int(*)(void*))CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv)((char*)self - 8);
}

// us-80180188
extern "C" void CBattleState_UnkVirtualFunc2__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc3__Q22cf11CActorParamFv)((char*)self - 8);
}

extern "C" void CActorParam_UnkVirtualFunc40__Q22cf11CActorParamFv(void* self) { reinterpret_cast<APIf*>(self)->vf011C(*(float*)((u8*)self + 0x17f4)); }

extern "C" void CActorParam_UnkVirtualFunc46__Q22cf11CActorParamFv(void* self) { reinterpret_cast<APIf*>(self)->vf013C(*(float*)((u8*)self + 0x17f8)); }

extern "C" void CActorParam_UnkVirtualFunc52__Q22cf11CActorParamFv(void* self) { reinterpret_cast<APIf*>(self)->vf0154(*(float*)((u8*)self + 0x17fc)); }
