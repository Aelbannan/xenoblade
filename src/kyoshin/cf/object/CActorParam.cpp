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
extern "C" void CActorParam_UnkVirtualFunc35__Q22cf11CActorParamFv(cf::CActorParam* self) {
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
// ==== CActorParam_UnkVirtualFunc177 (us-80177eec): battle action update ====

// Vtable call helpers (indirect calls through retail vtable offsets).
struct CActorParamVt {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098();
    virtual void* vf9C();              // 0x9C CActorParam_UnkVirtualFunc2
    virtual void _v0A0(); virtual void _v0A4(); virtual void _v0A8(); virtual void _v0AC();
    virtual void _v0B0(); virtual void _v0B4(); virtual void _v0B8(); virtual void _v0BC();
    virtual void _v0C0(); virtual void _v0C4(); virtual void _v0C8(); virtual void _v0CC();
    virtual void _v0D0(); virtual void _v0D4(); virtual void _v0D8(); virtual void _v0DC();
    virtual void _v0E0(); virtual void _v0E4(); virtual void _v0E8(); virtual void _v0EC();
    virtual void _v0F0(); virtual void _v0F4(); virtual void _v0F8(); virtual void _v0FC();
    virtual void _v100(); virtual void _v104(); virtual void _v108(); virtual void _v10C();
    virtual void _v110(); virtual void _v114();
    virtual void vf118(float);         // 0x118 CActorParam_UnkVirtualFunc33
    virtual void vf11C(float);         // 0x11C CActorParam_UnkVirtualFunc34
    virtual void vf120(float, int, int, int); // 0x120 CActorParam_UnkVirtualFunc35
    virtual void _v124();
    virtual float vf128();             // 0x128 CActorParam_UnkVirtualFunc37
    virtual float vf12C();             // 0x12C CActorParam_UnkVirtualFunc38
    virtual void _v130(); virtual void _v134(); virtual void _v138(); virtual void _v13C();
    virtual void _v140(); virtual void _v144(); virtual void _v148(); virtual void _v14C();
    virtual void _v150();
    virtual void vf154(float);         // 0x154 CActorParam_UnkVirtualFunc48
    virtual float vf158();             // 0x158 CActorParam_UnkVirtualFunc49
    virtual void _v15C(); virtual void _v160(); virtual void _v164(); virtual void _v168();
    virtual void _v16C(); virtual void _v170(); virtual void _v174(); virtual void _v178();
    virtual void _v17C(); virtual void _v180(); virtual void _v184(); virtual void _v188();
    virtual void _v18C(); virtual void _v190(); virtual void _v194(); virtual void _v198();
    virtual void _v19C(); virtual void _v1A0(); virtual void _v1A4(); virtual void _v1A8();
    virtual void _v1AC(); virtual void _v1B0(); virtual void _v1B4(); virtual void _v1B8();
    virtual void _v1BC(); virtual void _v1C0(); virtual void _v1C4(); virtual void _v1C8();
    virtual void _v1CC(); virtual void _v1D0(); virtual void _v1D4(); virtual void _v1D8();
    virtual void _v1DC(); virtual void _v1E0(); virtual void _v1E4(); virtual void _v1E8();
    virtual void _v1EC(); virtual void _v1F0(); virtual void _v1F4(); virtual void _v1F8();
    virtual void _v1FC(); virtual void _v200(); virtual void _v204(); virtual void _v208();
    virtual void _v20C(); virtual void _v210(); virtual void _v214(); virtual void _v218();
    virtual void _v21C(); virtual void _v220();
    virtual void* vf224();             // 0x224 CActorParam_UnkVirtualFunc100
    virtual void _v228(); virtual void _v22C(); virtual void _v230(); virtual void _v234();
    virtual void _v238(); virtual void _v23C(); virtual void _v240(); virtual void _v244();
    virtual void _v248(); virtual void _v24C(); virtual void _v250(); virtual void _v254();
    virtual void _v258(); virtual void _v25C(); virtual void _v260(); virtual void _v264();
    virtual void _v268(); virtual void _v26C(); virtual void _v270(); virtual void _v274();
    virtual void _v278(); virtual void _v27C(); virtual void _v280(); virtual void _v284();
    virtual void _v288(); virtual void _v28C();
    virtual void* vf290();             // 0x290 CActorParam_UnkVirtualFunc127
    virtual void _v294(); virtual void _v298(); virtual void _v29C(); virtual void _v2A0();
    virtual void _v2A4(); virtual void _v2A8(); virtual void _v2AC(); virtual void _v2B0();
    virtual void _v2B4(); virtual void _v2B8();
    virtual int vf2BC();               // 0x2BC CActorParam_UnkVirtualFunc138
    virtual void _v2C0(); virtual void _v2C4(); virtual void _v2C8(); virtual void _v2CC();
    virtual void _v2D0(); virtual void _v2D4(); virtual void _v2D8(); virtual void _v2DC();
    virtual void _v2E0(); virtual void _v2E4(); virtual void _v2E8(); virtual void _v2EC();
    virtual void _v2F0(); virtual void _v2F4(); virtual void _v2F8(); virtual void _v2FC();
    virtual void _v300(); virtual void _v304(); virtual void _v308(); virtual void _v30C();
    virtual void _v310(); virtual void _v314(); virtual void _v318(); virtual void _v31C();
    virtual void _v320(); virtual void _v324(); virtual void _v328(); virtual void _v32C();
    virtual void _v330(); virtual void _v334(); virtual void _v338(); virtual void _v33C();
    virtual void _v340(); virtual void _v344(); virtual void _v348();
    virtual void vf34C();              // 0x34C CActorParam_UnkVirtualFunc174
    virtual void _v350(); virtual void _v354();
    virtual void vf358();              // 0x358 CActorParam_UnkVirtualFunc177
};

struct CBattleStateVt {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C();
    virtual void vf20(int);            // 0x20 CBattleState_UnkVirtualFunc7
    virtual void vf24(void*);          // 0x24 CBattleState_UnkVirtualFunc8
    virtual void vf28(int);            // 0x28 CBattleState_UnkVirtualFunc9
};

struct CBattleMgrVt {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual int vf28(int);             // 0x28
};

struct Unk4Vt {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C();
    virtual void* vf30();              // 0x30
};

struct ActorVt {
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
    virtual void _v118(); virtual void _v11C(); virtual void _v120(); virtual void _v124();
    virtual void _v128(); virtual void _v12C(); virtual void _v130(); virtual void _v134();
    virtual void _v138(); virtual void _v13C(); virtual void _v140(); virtual void _v144();
    virtual void _v148(); virtual void _v14C(); virtual void _v150(); virtual void _v154();
    virtual void _v158(); virtual void _v15C(); virtual void _v160(); virtual void _v164();
    virtual void _v168(); virtual void _v16C(); virtual void _v170(); virtual void _v174();
    virtual void _v178(); virtual void _v17C(); virtual void _v180(); virtual void _v184();
    virtual void _v188(); virtual void _v18C(); virtual void _v190(); virtual void _v194();
    virtual void _v198(); virtual void _v19C(); virtual void _v1A0(); virtual void _v1A4();
    virtual void _v1A8(); virtual void _v1AC(); virtual void _v1B0(); virtual void _v1B4();
    virtual void _v1B8(); virtual void _v1BC(); virtual void _v1C0(); virtual void _v1C4();
    virtual void _v1C8(); virtual void _v1CC(); virtual void _v1D0(); virtual void _v1D4();
    virtual void _v1D8(); virtual void _v1DC(); virtual void _v1E0(); virtual void _v1E4();
    virtual void _v1E8(); virtual void _v1EC(); virtual void _v1F0(); virtual void _v1F4();
    virtual void _v1F8(); virtual void _v1FC(); virtual void _v200(); virtual void _v204();
    virtual void _v208(); virtual void _v20C(); virtual void _v210(); virtual void _v214();
    virtual void _v218(); virtual void _v21C(); virtual void _v220(); virtual void _v224();
    virtual void _v228(); virtual void _v22C(); virtual void _v230(); virtual void _v234();
    virtual void _v238(); virtual void _v23C(); virtual void _v240(); virtual void _v244();
    virtual void _v248(); virtual void _v24C(); virtual void _v250(); virtual void _v254();
    virtual void _v258(); virtual void _v25C(); virtual void _v260(); virtual void _v264();
    virtual void _v268(); virtual void _v26C(); virtual void _v270(); virtual void _v274();
    virtual void _v278(); virtual void _v27C(); virtual void _v280(); virtual void _v284();
    virtual void _v288(); virtual void _v28C(); virtual void _v290(); virtual void _v294();
    virtual void _v298(); virtual void _v29C(); virtual void _v2A0(); virtual void _v2A4();
    virtual void _v2A8(); virtual void _v2AC(); virtual void _v2B0(); virtual void _v2B4();
    virtual void _v2B8();
    virtual int vf2BC();              // 0x2BC
    virtual void _v2C0();
    virtual void vf2C4(void*, int, float, float, float); // 0x2C4
};

struct SubObjVt {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048();
    virtual int vf4C();               // 0x4C
    virtual void _v050(); virtual void _v054(); virtual void _v058(); virtual void _v05C();
    virtual void _v060(); virtual void _v064(); virtual void _v068(); virtual void _v06C();
    virtual void _v070(); virtual void _v074(); virtual void _v078(); virtual void _v07C();
    virtual void _v080(); virtual void _v084(); virtual void _v088(); virtual void _v08C();
    virtual void _v090(); virtual void _v094(); virtual void _v098(); virtual void _v09C();
    virtual void _v0A0(); virtual void _v0A4(); virtual void _v0A8();
    virtual void* vfAC();             // 0xAC
};

struct EnumListHolder { void* list; u32 handle; };

// C-linkage imports (retail symbol names).
extern "C" void* getInstance__Q22cf13CfGameManagerFv(void);
extern "C" void* getInstance__Q22cf14CBattleManagerFv(void);
extern "C" bool func_8006EF04__Fi(s32 mask);
extern "C" void func_802804F8(void*);
extern "C" void func_80280588(void*);
extern "C" void func_80280640(void*);
extern "C" int func_80148778(void*, int);
extern "C" void* func_80149154(void*, unsigned int);
extern "C" int func_801490A0(void*, int);
extern "C" int func_80145BC4(int);
extern "C" bool func_80146384(unsigned int);
extern "C" void* func_8016FE34(void*);
extern void* func_800B708C(int);            // C++ linkage -> func_800B708C__Fi
extern "C" void* func_80149330(void*, u32, u32, u32, u32);
extern "C" float func_800D81A8(void*, void*, void*);
extern "C" void func_800E9FE4(void*, void*, s32, s32, s32, s32, void*);
extern "C" void func_800EC8FC(void*, void*, void*, u32);
extern "C" void func_801A891C(int, int);
extern "C" u32 func_801B1DCC(u32);
extern "C" void func_80043D90(void*);
extern "C" void* func_80043F18(void*);
extern "C" void func_800F4A98(void*, int, int);
extern "C" void* func_800F6EAC(void*, u32);
extern "C" void __dt__80043E88(void*, int);
extern "C" void* __ct__800FB044(void*, void*, int, float);
extern "C" void func_800451D8(int, void*);
extern "C" int func_80260264(void*, int, void*);
extern "C" int rand(void);

extern "C" void CActorParam_UnkVirtualFunc177__Q22cf11CActorParamFv(cf::CActorParam* self, float dt) {
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x04000000)) return;
    if (reinterpret_cast<CBattleMgrVt*>(getInstance__Q22cf14CBattleManagerFv())->vf28(0x10)) return;
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x10000000)) return;

    bool flag = true;
    if (*(s16*)((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x20C8) == 0) {
        u8 b = *(u8*)((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x1AA);
        bool ok = (b >= 1 && b <= 0x18);
        if (!ok) flag = false;
    }
    func_802804F8(self);

    cf::CBattleStateEntry* entries = (cf::CBattleStateEntry*)((u8*)self + 0x10);
    for (int i = 0; i < 0x68; i++) {
        cf::CBattleStateEntry* e = &entries[i];
        if (e->unk0C == 0) continue;
        if (flag && !func_80146384(e->unk0C)) continue;

        if (!(e->unk30 & 1)) {
            if (!flag) {
                if (e->unk20 > 0.0f) {
                    if (e->unk0C == 0x10) {
                        u32 t = *(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30());
                        if ((t & 0x3F) == 0x16) goto unk20_done;
                        if (func_80148778((u8*)self + 8, 0xF) || func_80148778((u8*)self + 8, 9)) goto unk20_done;
                    }
                    e->unk20 -= dt;
                    if (e->unk20 < 0.0f) e->unk20 = 0.0f;
                }
            }
        }
unk20_done:
        if (e->unk28 > 0.0f) {
            if (e->unk0C == 0x10) {
                u32 t = *(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30());
                if ((t & 0x3F) == 0x16) goto unk28_done;
                if (func_80148778((u8*)self + 8, 0xF) || func_80148778((u8*)self + 8, 9)) goto unk28_done;
            }
            e->unk28 -= dt;
        }
unk28_done:
        if (e->unk28 <= 0.0f && e->unk24 > 0.0f) {
            e->unk28 += e->unk24;
            switch (e->unk0C) {
            case 0x35: {
                if (reinterpret_cast<CActorParamVt*>(self)->vf2BC()) break;
                if (reinterpret_cast<CActorParamVt*>(self)->vf12C() == reinterpret_cast<CActorParamVt*>(self)->vf128()) break;
                int a = (int)(0.05f * reinterpret_cast<CActorParamVt*>(self)->vf12C());
                int b = (int)((float)e->unk14 / 100.0f * reinterpret_cast<CActorParamVt*>(self)->vf12C());
                if (func_80148778((u8*)self + 8, 0xE7)) {
                    cf::CBattleStateEntry* d = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0xE7);
                    a += (int)((float)d->unk10 / 100.0f * reinterpret_cast<CActorParamVt*>(self)->vf12C());
                }
                a += b;
                int cl = func_801490A0((u8*)self + 8, 0x69);
                if (cl < 0) cl = 0;
                else if (cl > 3) cl = 3;
                double rate = 0.15 * (double)cl;
                a = (int)((double)a - (double)a * rate);
                if (func_80148778((u8*)self + 8, 0x8F)) {
                    cf::CBattleStateEntry* d = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x8F);
                    a *= d->unk10 / 100 + 1;
                } else if (func_80148778((u8*)self + 8, 0x8E)) {
                    cf::CBattleStateEntry* d = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x8E);
                    a *= 1 - d->unk10 / 100;
                }
                reinterpret_cast<CActorParamVt*>(self)->vf118((float)a + reinterpret_cast<CActorParamVt*>(self)->vf128());
                break;
            }
            case 0x34: {
                if (reinterpret_cast<CActorParamVt*>(self)->vf2BC()) break;
                if (reinterpret_cast<CActorParamVt*>(self)->vf128() == reinterpret_cast<CActorParamVt*>(self)->vf12C()) break;
                int a = e->unk10 + (int)((float)e->unk14 / 100.0f * reinterpret_cast<CActorParamVt*>(self)->vf12C());
                int cl = func_801490A0((u8*)self + 8, 0x69);
                if (cl < 0) cl = 0;
                else if (cl > 3) cl = 3;
                double rate = 0.15 * (double)cl;
                a = (int)((double)a - (double)a * rate);
                getInstance__Q22cf14CBattleManagerFv();
                float f = func_800D81A8(0, reinterpret_cast<CActorParamVt*>(self)->vf9C(), 0);
                a = (int)((float)a * f);
                if (a > 0) reinterpret_cast<CActorParamVt*>(self)->vf11C((float)a);
                break;
            }
            case 0x65: {
                if (reinterpret_cast<CActorParamVt*>(self)->vf2BC()) break;
                float f15 = (float)e->unk10;
                if (f15 < 1.0f) f15 = 1.0f;
                if (func_80148778((u8*)self + 8, 1)) f15 = 0.0f;
                int r4 = (f15 < 0.0f) ? 3 : 1;
                reinterpret_cast<CActorParamVt*>(self)->vf120((float)(-(int)f15), r4, 0, 0);
                func_80280588(self);
                break;
            }
            case 0x66: case 0x67: case 0x68: case 0x69: {
                if (reinterpret_cast<CActorParamVt*>(self)->vf2BC()) break;
                float f15 = (float)e->unk10;
                if (f15 < 1.0f) f15 = 1.0f;
                switch (e->unk0C) {
                case 0x66:
                    if (func_80148778((u8*)self + 8, 0x86)) {
                        cf::CBattleStateEntry* d = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x86);
                        f15 = f15 / 100.0f * (float)(100 - d->unk10);
                    }
                    break;
                case 0x67:
                    if (func_80148778((u8*)self + 8, 0x88)) {
                        cf::CBattleStateEntry* d = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x88);
                        f15 = f15 / 100.0f * (float)(100 - d->unk10);
                    }
                    break;
                case 0x68:
                    if (func_80148778((u8*)self + 8, 0x8A)) {
                        cf::CBattleStateEntry* d = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x8A);
                        f15 = f15 / 100.0f * (float)(100 - d->unk10);
                    }
                    break;
                case 0x69:
                    if (func_80148778((u8*)self + 8, 0x8C)) {
                        cf::CBattleStateEntry* d = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x8C);
                        f15 = f15 / 100.0f * (float)(100 - d->unk10);
                    }
                    break;
                default: break;
                }
                if (f15 <= 0.0f) break;
                if (func_80148778((u8*)self + 8, 1)) f15 = 0.0f;
                if (!func_80148778((u8*)self + 8, 0x13)) {
                int type = func_80145BC4(e->unk0C);
                u8* obj = (u8*)reinterpret_cast<CActorParamVt*>(self)->vf224();
                int v = 0;
                if ((u32)(type - 1) <= 2) {
                    switch (type) {
                    case 1: case 2: case 3: v = (obj[0x72] & 0x80) != 0; break;
                    case 4: v = (obj[0x72] & 0x01) != 0; break;
                    case 5: v = (obj[0x72] & 0x02) != 0; break;
                    case 6: v = (obj[0x72] & 0x04) != 0; break;
                    case 7: v = (obj[0x72] & 0x08) != 0; break;
                    case 8: v = (obj[0x72] & 0x10) != 0; break;
                    case 9: v = (obj[0x72] & 0x20) != 0; break;
                    default: v = 0; break;
                    }
                } else if (type == 0) {
                    v = 0;
                } else {
                    switch (type) {
                    case 4: v = *(s16*)(obj + 0x64); break;
                    case 5: v = *(s16*)(obj + 0x66); break;
                    case 6: v = *(s16*)(obj + 0x68); break;
                    case 7: v = *(s16*)(obj + 0x6A); break;
                    case 8: v = *(s16*)(obj + 0x6C); break;
                    case 9: v = *(s16*)(obj + 0x6E); break;
                    default: v = 0; break;
                    }
                    if (v >= 200) v = 1;
                    else {
                        switch (type) {
                        case 1: case 2: case 3: v = (obj[0x72] & 0x80) != 0; break;
                        case 4: v = (obj[0x72] & 0x01) != 0; break;
                        case 5: v = (obj[0x72] & 0x02) != 0; break;
                        case 6: v = (obj[0x72] & 0x04) != 0; break;
                        case 7: v = (obj[0x72] & 0x08) != 0; break;
                        case 8: v = (obj[0x72] & 0x10) != 0; break;
                        case 9: v = (obj[0x72] & 0x20) != 0; break;
                        default: v = 0; break;
                        }
                    }
                }
                    if (v) {
                        f15 *= -1.0f;
                    } else {                int type = func_80145BC4(e->unk0C);
                u8* obj = (u8*)reinterpret_cast<CActorParamVt*>(self)->vf224();
                int v = 0;
                if ((u32)(type - 1) <= 2) {
                    switch (type) {
                    case 1: case 2: case 3: v = (obj[0x70] & 0x80) != 0; break;
                    case 4: v = (obj[0x70] & 0x01) != 0; break;
                    case 5: v = (obj[0x70] & 0x02) != 0; break;
                    case 6: v = (obj[0x70] & 0x04) != 0; break;
                    case 7: v = (obj[0x70] & 0x08) != 0; break;
                    case 8: v = (obj[0x70] & 0x10) != 0; break;
                    case 9: v = (obj[0x70] & 0x20) != 0; break;
                    default: v = 0; break;
                    }
                } else if (type == 0) {
                    v = 0;
                } else {
                    switch (type) {
                    case 4: v = *(s16*)(obj + 0x64); break;
                    case 5: v = *(s16*)(obj + 0x66); break;
                    case 6: v = *(s16*)(obj + 0x68); break;
                    case 7: v = *(s16*)(obj + 0x6A); break;
                    case 8: v = *(s16*)(obj + 0x6C); break;
                    case 9: v = *(s16*)(obj + 0x6E); break;
                    default: v = 0; break;
                    }
                    if (v >= 100) v = 1;
                    else {
                        switch (type) {
                        case 1: case 2: case 3: v = (obj[0x70] & 0x80) != 0; break;
                        case 4: v = (obj[0x70] & 0x01) != 0; break;
                        case 5: v = (obj[0x70] & 0x02) != 0; break;
                        case 6: v = (obj[0x70] & 0x04) != 0; break;
                        case 7: v = (obj[0x70] & 0x08) != 0; break;
                        case 8: v = (obj[0x70] & 0x10) != 0; break;
                        case 9: v = (obj[0x70] & 0x20) != 0; break;
                        default: v = 0; break;
                        }
                    }
                }
                    if (v) {
                        f15 *= 0.5f;
                        if (f15 < 1.0f) f15 = 1.0f;
                    }
                }
                }
                int r4 = (f15 < 0.0f) ? 3 : 1;
                reinterpret_cast<CActorParamVt*>(self)->vf120((float)(-(int)f15), r4, 0, 0);
                func_80280588(self);
                if (func_80148778((u8*)self + 8, 9)) reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf20(9);
                break;
            }
            case 0x6A: case 0x6B: case 0x6C: case 0x6D: {
                if (reinterpret_cast<CActorParamVt*>(self)->vf2BC()) break;
                int a = e->unk10;
                float f15 = 1.0f;
                if (func_80148778((u8*)self + 8, 0x8D)) {
                    cf::CBattleStateEntry* d = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x8D);
                    f15 = 1.0f - 0.01f * (float)d->unk10;
                }
                if (reinterpret_cast<CActorParamVt*>(self)->vf290()) {
                    int sv;
                    if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x57, &sv)) {
                        f15 = f15 - 0.01f * (float)sv;
                    }
                }
                if (f15 < 0.0f) f15 = 0.0f;
                a = (int)((float)a * f15);
                if (a < 1) a = 0;
                int type = func_80145BC4(e->unk0C);
                u8* obj = (u8*)reinterpret_cast<CActorParamVt*>(self)->vf224();
                int v = 0;
                if ((u32)(type - 1) <= 2) {
                    switch (type) {
                    case 1: case 2: case 3: v = (obj[0x72] & 0x80) != 0; break;
                    case 4: v = (obj[0x72] & 0x01) != 0; break;
                    case 5: v = (obj[0x72] & 0x02) != 0; break;
                    case 6: v = (obj[0x72] & 0x04) != 0; break;
                    case 7: v = (obj[0x72] & 0x08) != 0; break;
                    case 8: v = (obj[0x72] & 0x10) != 0; break;
                    case 9: v = (obj[0x72] & 0x20) != 0; break;
                    default: v = 0; break;
                    }
                } else if (type == 0) {
                    v = 0;
                } else {
                    switch (type) {
                    case 4: v = *(s16*)(obj + 0x64); break;
                    case 5: v = *(s16*)(obj + 0x66); break;
                    case 6: v = *(s16*)(obj + 0x68); break;
                    case 7: v = *(s16*)(obj + 0x6A); break;
                    case 8: v = *(s16*)(obj + 0x6C); break;
                    case 9: v = *(s16*)(obj + 0x6E); break;
                    default: v = 0; break;
                    }
                    if (v >= 200) v = 1;
                    else {
                        switch (type) {
                        case 1: case 2: case 3: v = (obj[0x72] & 0x80) != 0; break;
                        case 4: v = (obj[0x72] & 0x01) != 0; break;
                        case 5: v = (obj[0x72] & 0x02) != 0; break;
                        case 6: v = (obj[0x72] & 0x04) != 0; break;
                        case 7: v = (obj[0x72] & 0x08) != 0; break;
                        case 8: v = (obj[0x72] & 0x10) != 0; break;
                        case 9: v = (obj[0x72] & 0x20) != 0; break;
                        default: v = 0; break;
                        }
                    }
                }
                if (v) {
                    a = -a;
                } else {
                    u8* obj = (u8*)reinterpret_cast<CActorParamVt*>(self)->vf224();
                    float ratio = (float)*(s16*)(obj + 0x62) / 100.0f;
                    if (!func_80148778((u8*)self + 8, 0x13)) {
                int type = func_80145BC4(e->unk0C);
                u8* obj = (u8*)reinterpret_cast<CActorParamVt*>(self)->vf224();
                int v = 0;
                if ((u32)(type - 1) <= 2) {
                    switch (type) {
                    case 1: case 2: case 3: v = (obj[0x70] & 0x80) != 0; break;
                    case 4: v = (obj[0x70] & 0x01) != 0; break;
                    case 5: v = (obj[0x70] & 0x02) != 0; break;
                    case 6: v = (obj[0x70] & 0x04) != 0; break;
                    case 7: v = (obj[0x70] & 0x08) != 0; break;
                    case 8: v = (obj[0x70] & 0x10) != 0; break;
                    case 9: v = (obj[0x70] & 0x20) != 0; break;
                    default: v = 0; break;
                    }
                } else if (type == 0) {
                    v = 0;
                } else {
                    switch (type) {
                    case 4: v = *(s16*)(obj + 0x64); break;
                    case 5: v = *(s16*)(obj + 0x66); break;
                    case 6: v = *(s16*)(obj + 0x68); break;
                    case 7: v = *(s16*)(obj + 0x6A); break;
                    case 8: v = *(s16*)(obj + 0x6C); break;
                    case 9: v = *(s16*)(obj + 0x6E); break;
                    default: v = 0; break;
                    }
                    if (v >= 100) v = 1;
                    else {
                        switch (type) {
                        case 1: case 2: case 3: v = (obj[0x70] & 0x80) != 0; break;
                        case 4: v = (obj[0x70] & 0x01) != 0; break;
                        case 5: v = (obj[0x70] & 0x02) != 0; break;
                        case 6: v = (obj[0x70] & 0x04) != 0; break;
                        case 7: v = (obj[0x70] & 0x08) != 0; break;
                        case 8: v = (obj[0x70] & 0x10) != 0; break;
                        case 9: v = (obj[0x70] & 0x20) != 0; break;
                        default: v = 0; break;
                        }
                    }
                }
                        if (v) {
                            a = (int)((float)a * 0.5f);
                            if (a < 1) a = 1;
                        }
                    }
                }
                if (a <= 0) break;
                if (func_80148778((u8*)self + 8, 1)) a = 0;
                int r4 = (a >= 0) ? 2 : 3;
                reinterpret_cast<CActorParamVt*>(self)->vf120((float)(-a), r4, 0, 0);
                func_80280588(self);
                int code;
                switch (func_80145BC4(e->unk0C)) {
                case 4: code = 0x42; break;
                case 5: code = 0x4C; break;
                case 6: code = 0x44; break;
                case 7: code = 0x46; break;
                case 8: code = 0x4A; break;
                case 9: code = 0x48; break;
                default: code = 0; break;
                }
                void* p = reinterpret_cast<CActorParamVt*>(self)->vf9C();
                if (p) p = (u8*)p + 0x3E9C;
                func_800451D8(code, p);
                if (func_80148778((u8*)self + 8, 9)) reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf20(9);
                break;
            }
            case 0x7D: {
                if (reinterpret_cast<CActorParamVt*>(self)->vf2BC()) break;
                float f15 = (float)e->unk10;
                if (f15 < 1.0f) f15 = 1.0f;
                if (func_80148778((u8*)self + 8, 1)) f15 = 0.0f;
                reinterpret_cast<CActorParamVt*>(self)->vf120((float)(-(int)f15), 0xA, 0, 0);
                func_80280640(self);
                if (func_80148778((u8*)self + 8, 9)) reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf20(9);
                break;
            }
            case 0x94: {
                reinterpret_cast<CActorParamVt*>(self)->vf154((float)(-e->unk10));
                if (reinterpret_cast<CActorParamVt*>(self)->vf158() > 0.0f) break;
                e->unk30 &= ~1;
                reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf24(e);
                break;
            }
            case 0x97: {
                reinterpret_cast<CActorParamVt*>(self)->vf154((float)e->unk14 / 10.0f * (float)e->unk16 + (float)e->unk10 / 10.0f);
                break;
            }
            case 0xC5: {
                void* p = reinterpret_cast<CActorParamVt*>(self)->vf9C();
                if (!p) break;
                void* base = (u8*)p + 0x3E9C;
                int v = reinterpret_cast<SubObjVt*>(base)->vf4C();
                void* actor = func_8016FE34(func_800B708C(v));
                if (!actor) break;
                if (reinterpret_cast<ActorVt*>(actor)->vf2BC()) break;
                reinterpret_cast<ActorVt*>(actor)->vf2C4(reinterpret_cast<CActorParamVt*>(self)->vf9C(), e->unk14, (float)e->unk10, (float)e->unk14, (float)e->unk16);
                break;
            }
            case 0xC6: {
                void* r16 = reinterpret_cast<CActorParamVt*>(self)->vf9C();
                func_800E9FE4(getInstance__Q22cf14CBattleManagerFv(), r16, e->unk10, e->unk14, e->unk16, 0, 0);
                break;
            }
            default: break;
            }
        } else {
            switch (e->unk0C) {
            case 0x11: {
                if (e->unk10 == -1) break;
                void* actor = func_8016FE34(func_800B708C(e->unk10));
                if (actor && !reinterpret_cast<ActorVt*>(actor)->vf2BC()) break;
                reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf20(0x11);
                break;
            }
            case 0x112: {
                void* actor = func_8016FE34(func_800B708C(e->unk10));
                if (actor && !reinterpret_cast<ActorVt*>(actor)->vf2BC()) break;
                EnumListHolder holder;
                func_80043D90(&holder);
                void* list = func_80043F18(&holder);
                func_800F4A98(list, 0x20, 0x800);
                u32 count = *(u32*)((u8*)func_80043F18(&holder) + 0x620);
                if (count != 0) {
                    u32 idx = (u32)rand() % count;
                    void* item = func_800F6EAC(func_80043F18(&holder), idx);
                    e->unk10 = *(u32*)((u8*)item + 0x74);
                }
                __dt__80043E88(&holder, -1);
                break;
            }
            case 0x10: {
                if (!reinterpret_cast<CActorParamVt*>(self)->vf9C()) break;
                if (*(u32*)((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3F60) == 0) break;
                void* q = *(void**)((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3F60);
                if (!(*(u32*)((u8*)q + 0x4EC) & 2)) break;
                if (!(*(u32*)((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3F00) & 2)) break;
                reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf24(e);
                break;
            }
            case 0x73: case 0x74: case 0x75: case 0x76:
            case 0x77: case 0x78: case 0x79: case 0x7A:
            case 0x7B: case 0x7C: {
                if (func_80148778((u8*)self + 8, 0x117)) break;
                void* p = reinterpret_cast<CActorParamVt*>(self)->vf9C();
                if (!p) break;
                if (*(u32*)((u8*)self + 0x3374) & 0x40000) {
                    u32 t = *(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30());
                    if ((t & 0x3F) == 0x16) break;
                }
                EnumListHolder holder;
                func_80043D90(&holder);
                void* list = func_80043F18(&holder);
                if (*(u32*)((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3F00) & 2) {
                    func_800F4A98(list, 0x4000, 0x800);
                    void* tgt = reinterpret_cast<SubObjVt*>((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3E9C)->vfAC();
                    __ct__800FB044(func_80043F18(&holder), tgt, 0, (float)e->unk14);
                } else {
                    func_800F4A98(list, 0x20, 0x800);
                    void* tgt = reinterpret_cast<SubObjVt*>((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3E9C)->vfAC();
                    __ct__800FB044(func_80043F18(&holder), tgt, 0, (float)e->unk14);
                }
                cf::CBattleStateEntry st;
                std::memset(&st, 0, sizeof(st));
                st.unk00 = *(u32*)((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3F10);
                st.unk04 = 0;
                u16 id = e->unk0C;
                if (id - 0x73 <= 3) {
                    st.unk0C = id - 9;
                    st.unk10 = e->unk10;
                    st.unk28 = 0.1f;
                    st.unk24 = (e->unk30 & 0x10000) ? 2.0f : 5.0f;
                    st.unk20 = st.unk24;
                } else if (id >= 0x78 && id <= 0x7B) {
                    st.unk0C = id - 0xE;
                    st.unk10 = e->unk10;
                    st.unk28 = 0.1f;
                    st.unk24 = (e->unk30 & 0x10000) ? 2.0f : 5.0f;
                    st.unk20 = st.unk24;
                }
                if (id == 0x74 || id == 0x79) {
                    if (reinterpret_cast<CActorParamVt*>(self)->vf290()) {
                        int sv;
                        if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x5B, &sv)) {
                            st.unk10 = (int)((float)st.unk10 * (1.0f + (float)sv / 100.0f));
                        }
                    }
                }
                u32 i2 = 0;
                goto check73;
            loop73:
                {
                    void* actor = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i2));
                    if (reinterpret_cast<CActorParamVt*>(self)->vf9C() != actor) {
                        if (!func_80148778((u8*)actor + 8, st.unk0C)) {
                            func_800EC8FC(getInstance__Q22cf14CBattleManagerFv(), actor, &st, 0);
                        }
                    }
                    i2++;
                }
            check73:
                {
                    if (i2 < *(u32*)((u8*)func_80043F18(&holder) + 0x620)) goto loop73;
                }
                __dt__80043E88(&holder, -1);
                break;
            }
            case 0x125: case 0x126: case 0x127: case 0x128: case 0x129: case 0x12A: {
                void* p = reinterpret_cast<CActorParamVt*>(self)->vf9C();
                if (!p) break;
                float f15 = (float)e->unk14;
                if (func_80148778((u8*)self + 8, 0x109)) f15 *= 2.5f;
                EnumListHolder holder;
                func_80043D90(&holder);
                void* list = func_80043F18(&holder);
                if (*(u32*)((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3F00) & 2) {
                    func_800F4A98(list, 0x20, 0x800);
                    void* tgt = reinterpret_cast<SubObjVt*>((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3E9C)->vfAC();
                    __ct__800FB044(func_80043F18(&holder), tgt, 0, f15);
                } else {
                    func_800F4A98(list, 0x4000, 0x800);
                    void* tgt = reinterpret_cast<SubObjVt*>((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3E9C)->vfAC();
                    __ct__800FB044(func_80043F18(&holder), tgt, 0, f15);
                }
                cf::CBattleStateEntry st;
                std::memset(&st, 0, sizeof(st));
                st.unk00 = *(u32*)((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3F10);
                st.unk20 = 1.0f;
                int r19 = 0;
                switch (e->unk0C) {
                case 0x125: st.unk0C = 0x5A; r19 = 6; st.unk10 = e->unk10; break;
                case 0x126: st.unk0C = 0x58; r19 = 4; st.unk10 = e->unk10; break;
                case 0x127: st.unk0C = 0x45; r19 = 7; st.unk10 = e->unk10; break;
                case 0x128: st.unk0C = 0x59; r19 = 8; st.unk10 = e->unk10; break;
                case 0x129: st.unk0C = 0x44; r19 = 9; st.unk10 = e->unk10; break;
                case 0x12A: st.unk0C = 0x34; r19 = 5; st.unk10 = e->unk10; st.unk24 = 4.0f; st.unk20 = 4.0f; break;
                default: break;
                }
                st.unk04 = ((u32)e->unk0C << 16) | st.unk0C;
                u32 i2 = 0;
                goto check125;
            loop125:
                {
                    void* actor = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i2));
                    cf::CBattleStateEntry* found = (cf::CBattleStateEntry*)func_80149330((u8*)actor + 8, st.unk0C, st.unk00, st.unk04, 0);
                    if (found) {
                        int nv = func_801B1DCC(r19) * st.unk10;
                        found->unk20 = st.unk20;
                        if (found->unk10 != nv) {
                            found->unk10 = nv;
                            if (nv == 0) reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf24(found);
                            func_801A891C((int)actor, 0);
                        }
                    } else {
                        if (func_801B1DCC(r19)) {
                            func_800EC8FC(getInstance__Q22cf14CBattleManagerFv(), actor, &st, 0);
                        }
                    }
                    i2++;
                }
            check125:
                {
                    if (i2 < *(u32*)((u8*)func_80043F18(&holder) + 0x620)) goto loop125;
                }
                __dt__80043E88(&holder, -1);
                break;
            }
            default: break;
            }
        }
        if (!(e->unk30 & 1) && e->unk20 <= 0.0f) {
            reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf28(i);
        }
    }
}
extern "C" int func_800B8B94(int);
extern "C" int func_8026178C(void*, int);
extern "C" void* func_8017389C(void*, void*, int);
extern "C" int func_8025FB10(void*, int);

struct Func4ItemVt {
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
    virtual void _v118(); virtual void _v11C(); virtual void _v120(); virtual void _v124();
    virtual void _v128(); virtual void _v12C(); virtual void _v130(); virtual void _v134();
    virtual void _v138(); virtual void _v13C(); virtual void _v140(); virtual void _v144();
    virtual void _v148(); virtual void _v14C(); virtual void _v150(); virtual void _v154();
    virtual void _v158(); virtual void _v15C(); virtual void _v160(); virtual void _v164();
    virtual void _v168(); virtual void _v16C(); virtual void _v170(); virtual void _v174();
    virtual void _v178(); virtual void _v17C(); virtual void _v180(); virtual void _v184();
    virtual void _v188(); virtual void _v18C(); virtual void _v190(); virtual void _v194();
    virtual void _v198(); virtual void _v19C(); virtual void _v1A0(); virtual void _v1A4();
    virtual void _v1A8(); virtual void _v1AC(); virtual void _v1B0(); virtual void _v1B4();
    virtual void _v1B8(); virtual void _v1BC(); virtual void _v1C0(); virtual void _v1C4();
    virtual void _v1C8(); virtual void _v1CC(); virtual void _v1D0(); virtual void _v1D4();
    virtual void _v1D8(); virtual void _v1DC(); virtual void _v1E0(); virtual void _v1E4();
    virtual void _v1E8(); virtual void _v1EC(); virtual void _v1F0(); virtual void _v1F4();
    virtual void _v1F8(); virtual void _v1FC(); virtual void _v200(); virtual void _v204();
    virtual void _v208(); virtual void _v20C();
    virtual void* vf210();             // 0x210
    virtual void* vf214();             // 0x214
};

extern "C" void CActorParam_UnkVirtualFunc4__Q22cf11CActorParamFv(cf::CActorParam* self, void* arts) {
    bool flag = false;
    if (arts != NULL) {
        if (func_800B8B94(*(u16*)arts)) flag = true;
    }
    if (reinterpret_cast<CActorParamVt*>(self)->vf9C()) flag = true;
    if (reinterpret_cast<CActorParamVt*>(self)->vf290() != NULL) {
        EnumListHolder holder;
        func_80043D90(&holder);
        void* list = func_80043F18(&holder);
        func_800F4A98(list, 32, 1);

        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 31) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 31);
            if (r != 0) {
                s16 delta = (s16)r;
                for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(&holder) + 0x620); i++) {
                    void* actor = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                    void* obj = reinterpret_cast<Func4ItemVt*>(actor)->vf210();
                    *(s16*)((u8*)obj + 0x60) += delta;
                    obj = reinterpret_cast<Func4ItemVt*>(actor)->vf210();
                    *(s16*)((u8*)obj + 0x62) += delta;
                }
            }
        }

        if (func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 150) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 150);
            if (r != 0 && flag) {
                s16 delta = (s16)r;
                for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(&holder) + 0x620); i++) {
                    void* actor = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                    void* obj = reinterpret_cast<Func4ItemVt*>(actor)->vf214();
                    *(s16*)((u8*)obj + 0x18) += delta;
                    obj = reinterpret_cast<Func4ItemVt*>(actor)->vf214();
                    *(s16*)((u8*)obj + 0x1C) += delta;
                }
            }
        }
        __dt__80043E88(&holder, -1);
    }

    float v17E8 = *(float*)((u8*)self + 0x17E8);
    float v17F4 = *(float*)((u8*)self + 0x17F4);
    float v17EC = *(float*)((u8*)self + 0x17EC);
    float v17F0 = *(float*)((u8*)self + 0x17F0);
    self->unk17E4 = self->unk1650;
    reinterpret_cast<CActorParamVt*>(self)->vf34C();
    if (reinterpret_cast<CActorParamVt*>(self)->vf290() != NULL) {
        float ratio = v17E8 / v17F4;
        void* gm = getInstance__Q22cf13CfGameManagerFv();
        bool bLT = ratio < 0.5f;
        (void)bLT;
        u32 t = *(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30());
        bool c = ((t & 0x3F) == 6) || ((t & 0x3F) == 7);
        if (!c) c = (t & 0x7C0) == 448;
        if (!c) c = func_8017389C(self, &t, 9) || func_8017389C(self, &t, 10) || func_8017389C(self, &t, 11);
        if (!c) c = ((t & 0x3F) == 19);
        if (!c) c = ((t & 0x3F) == 18);
        if (!c) c = ((t & 0x3F) == 20);
        if (!c) c = func_8017389C(self, &t, 22) || func_8017389C(self, &t, 23) || func_8017389C(self, &t, 15);
        if (!c) c = ((t & 0x3F) == 21);
        if (!c) c = ((t & 0x3F) == 24);
        if (!c) c = ((t & 0x3F) == 25);
        if (!c) c = ((t & 0x3F) == 26);
        if (!c) c = ((t & 0x3F) == 27);
        if (!c) c = ((t & 0x3F) == 16);
        if (!c) c = ((t & 0x3F) == 13);
        if (!c) c = ((t & 0x3F) == 15);
        if (!c) c = ((t & 0x3F) == 31);
        float f26 = 0.0f;
        if (c) f26 = 1.0f;
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 37) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 37);
            if (r != 0) *(s16*)((u8*)self + 0x1746) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 1) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 1);
            if (r != 0) *(s16*)((u8*)self + 0x16E4) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 16) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 16);
            if (r != 0) *(s16*)((u8*)self + 0x16E8) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 20) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 20);
            if (r != 0) *(s16*)((u8*)self + 0x16E6) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 152) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 152);
            if (r != 0 && bLT && f26 != 0.0f) *(s16*)((u8*)self + 0x174E) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 34) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 34);
            if (r != 0) *(s16*)((u8*)self + 0x16F6) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 35) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 35);
            if (r != 0) *(s16*)((u8*)self + 0x16FA) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 44) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 44);
            if (r != 0) *(s16*)((u8*)self + 0x181C) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 2) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 2);
            if (r != 0 && (u32)gm != 4) *(s16*)((u8*)self + 0x174C) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 3) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 3);
            if (r != 0 && (u32)gm == 4) *(s16*)((u8*)self + 0x174C) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 4) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 4);
            if (r != 0 && ratio >= 1.0f && f26 != 0.0f) *(s16*)((u8*)self + 0x174C) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 9) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 9);
            if (r != 0) *(s16*)((u8*)self + 0x1758) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 10) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 10);
            if (r != 0 && bLT && f26 != 0.0f) *(s16*)((u8*)self + 0x1758) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 149) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 149);
            if (r != 0) {
                *(s16*)((u8*)self + 0x1758) += (s16)r;
                *(s16*)((u8*)self + 0x175C) += (s16)r;
            }
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 17) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 17);
            if (r != 0 && bLT && f26 != 0.0f) *(s16*)((u8*)self + 0x1750) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 21) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 21);
            if (r != 0) *(s16*)((u8*)self + 0x16FE) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 22) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 22);
            if (r != 0 && bLT && f26 != 0.0f) *(s16*)((u8*)self + 0x16FE) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 27) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 27);
            if (r != 0) *(s16*)((u8*)self + 0x16FC) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 30) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 30);
            if (r != 0) {
                *(s16*)((u8*)self + 0x1728) += (s16)r;
                *(s16*)((u8*)self + 0x172A) += (s16)r;
            }
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 39) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 39);
            if (r != 0) *(u8*)((u8*)self + 0x171D) += (u8)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 40) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 40);
            if (r != 0 && bLT && f26 != 0.0f) *(u8*)((u8*)self + 0x171D) += (u8)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 41) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 41);
            if (r != 0) *(u8*)((u8*)self + 0x171E) += (u8)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 42) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 42);
            if (r != 0) *(u8*)((u8*)self + 0x171F) += (u8)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 43) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 43);
            if (r != 0 && bLT && f26 != 0.0f) *(u8*)((u8*)self + 0x171F) += (u8)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 114) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 114);
            if (r != 0) *(s16*)((u8*)self + 0x177C) += (s16)r;
        }
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 45) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 45);
            if (r != 0) {
                float f2 = *(float*)((u8*)self + 0x1808) * (1.0f + (float)r / 100.0f);
                float f0 = *(float*)((u8*)self + 0x180C) * (1.0f + (float)r / 100.0f);
                *(float*)((u8*)self + 0x1808) = f2;
                *(float*)((u8*)self + 0x180C) = f0;
                if (f2 > f0) *(float*)((u8*)self + 0x1808) = f0;
            }
        }

        *(u32*)((u8*)self + 0x185C) = *(u32*)((u8*)self + 0x17E4) + *(u32*)((u8*)self + 0x16C8);
        *(s16*)((u8*)self + 0x1878) = *(s16*)((u8*)self + 0x1800) + *(s16*)((u8*)self + 0x16E4);
        *(s16*)((u8*)self + 0x187A) = *(s16*)((u8*)self + 0x1802) + *(s16*)((u8*)self + 0x16E6);
        *(s16*)((u8*)self + 0x187C) = *(s16*)((u8*)self + 0x1804) + *(s16*)((u8*)self + 0x16E8);
        *(float*)((u8*)self + 0x1860) = *(float*)((u8*)self + 0x17E8);
        *(float*)((u8*)self + 0x1864) = *(float*)((u8*)self + 0x17EC);
        *(float*)((u8*)self + 0x1868) = *(float*)((u8*)self + 0x17F0);
        *(float*)((u8*)self + 0x186C) = *(float*)((u8*)self + 0x17F4);
        *(float*)((u8*)self + 0x1870) = *(float*)((u8*)self + 0x17F8);
        *(float*)((u8*)self + 0x1874) = *(float*)((u8*)self + 0x17FC);
        *(float*)((u8*)self + 0x1880) = *(float*)((u8*)self + 0x1808);
        *(float*)((u8*)self + 0x1884) = *(float*)((u8*)self + 0x180C);
        *(s16*)((u8*)self + 0x1888) = *(s16*)((u8*)self + 0x1810);
        *(s16*)((u8*)self + 0x188C) = *(s16*)((u8*)self + 0x1814);
        *(s16*)((u8*)self + 0x1894) = *(s16*)((u8*)self + 0x181C);
        *(s16*)((u8*)self + 0x1896) = *(s16*)((u8*)self + 0x181E);
        *(u8*)((u8*)self + 0x1898) = *(u8*)((u8*)self + 0x1820);
        *(float*)((u8*)self + 0x189C) = *(float*)((u8*)self + 0x1824);
        *(float*)((u8*)self + 0x18A0) = *(float*)((u8*)self + 0x1828);
        *(float*)((u8*)self + 0x18A4) = *(float*)((u8*)self + 0x182C);
        *(float*)((u8*)self + 0x18A8) = *(float*)((u8*)self + 0x1830);
        *(float*)((u8*)self + 0x18AC) = *(float*)((u8*)self + 0x1834);
        *(u8*)((u8*)self + 0x18B0) = *(u8*)((u8*)self + 0x1838);
        *(u8*)((u8*)self + 0x18B4) = *(u8*)((u8*)self + 0x183C);
        *(float*)((u8*)self + 0x18B8) = *(float*)((u8*)self + 0x1840);
        *(u32*)((u8*)self + 0x18BC) = *(u32*)((u8*)self + 0x1844);
        *(u32*)((u8*)self + 0x18C0) = *(u32*)((u8*)self + 0x1848);
        *(u32*)((u8*)self + 0x18C4) = *(u32*)((u8*)self + 0x184C);
        *(u32*)((u8*)self + 0x18C8) = *(u32*)((u8*)self + 0x1850);
        *(u32*)((u8*)self + 0x18CC) = *(u32*)((u8*)self + 0x1854);
        *(u32*)((u8*)self + 0x18D0) = *(u32*)((u8*)self + 0x1858);
        *(s16*)((u8*)self + 0x1890) = *(s16*)((u8*)self + 0x1818) + *(s16*)((u8*)self + 0x16FC);
        *(s16*)((u8*)self + 0x1892) = *(s16*)((u8*)self + 0x181A) + *(s16*)((u8*)self + 0x16FE);
        *(u8*)((u8*)self + 0x18B1) = *(u8*)((u8*)self + 0x1839) + *(u8*)((u8*)self + 0x171D);
        *(s16*)((u8*)self + 0x188A) = *(s16*)((u8*)self + 0x1812) + *(s16*)((u8*)self + 0x16F6);
        *(s16*)((u8*)self + 0x188E) = *(s16*)((u8*)self + 0x1816) + *(s16*)((u8*)self + 0x16FA);
        *(u8*)((u8*)self + 0x18B2) = *(u8*)((u8*)self + 0x183A) + *(u8*)((u8*)self + 0x171E);
        *(u8*)((u8*)self + 0x18B3) = *(u8*)((u8*)self + 0x183B) + *(u8*)((u8*)self + 0x171F);
        *(s16*)((u8*)self + 0x18BC) += *(s16*)((u8*)self + 0x1728);
        *(s16*)((u8*)self + 0x18BE) += *(s16*)((u8*)self + 0x172A);
        *(s16*)((u8*)self + 0x18E0) = *(s16*)((u8*)self + 0x174C);
        *(s16*)((u8*)self + 0x18E2) = *(s16*)((u8*)self + 0x174E);
        *(s16*)((u8*)self + 0x18E4) = *(s16*)((u8*)self + 0x1750);
        *(s16*)((u8*)self + 0x18EC) = *(s16*)((u8*)self + 0x1758);
        *(s16*)((u8*)self + 0x18F0) = *(s16*)((u8*)self + 0x175C);
        *(s16*)((u8*)self + 0x18DA) = *(s16*)((u8*)self + 0x1746);
    }
}
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
extern "C" void CBattleState_UnkVirtualFunc18__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv)((void*)((char*)self - 8));
}

// us-80180170
void CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv();
extern "C" void CBattleState_UnkVirtualFunc17__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv)((void*)((char*)self - 8));
}

// us-80180178
extern "C" void CActorParam_UnkVirtualFunc1__Q22cf11CActorParamFv(cf::CActorParam* self) { reinterpret_cast<APIf*>(*(void**)((u8*)self + 0x15dc))->vf0040(); }

// us-80180180
int CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv(void* self);
extern "C" int CBattleState_UnkVirtualFunc1__Q22cf11CActorParamFv(cf::CActorParam* self) {
    return ((int(*)(void*))CActorParam_UnkVirtualFunc2__Q22cf11CActorParamFv)((char*)self - 8);
}

// us-80180188
extern "C" void CBattleState_UnkVirtualFunc2__Q22cf11CActorParamFv(cf::CActorParam* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc3__Q22cf11CActorParamFv)((char*)self - 8);
}

extern "C" void CActorParam_UnkVirtualFunc40__Q22cf11CActorParamFv(cf::CActorParam* self) { reinterpret_cast<APIf*>(self)->vf011C(*(float*)((u8*)self + 0x17f4)); }

extern "C" void CActorParam_UnkVirtualFunc46__Q22cf11CActorParamFv(cf::CActorParam* self) { reinterpret_cast<APIf*>(self)->vf013C(*(float*)((u8*)self + 0x17f8)); }

extern "C" void CActorParam_UnkVirtualFunc52__Q22cf11CActorParamFv(cf::CActorParam* self) { reinterpret_cast<APIf*>(self)->vf0154(*(float*)((u8*)self + 0x17fc)); }
