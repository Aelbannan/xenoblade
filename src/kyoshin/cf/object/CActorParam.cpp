#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/CfGimmick.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfObjectEnumList.hpp"
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
    virtual void* vf4C();
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
// Retail symbol is Fv; the real ABI passes (self, delta). Adds a signed
// delta to the gauge at 0x160C, clamping to [0, 0x160E].
void CActorParam_UnkVirtualFunc54__Q22cf11CActorParamFv(cf::CActorParam* self, int delta) {
    cf::CActorParamGaugeView* f = reinterpret_cast<cf::CActorParamGaugeView*>(self);
    s16 cur = f->field_0x160C;
    s16 max = f->field_0x160E;
    // Explicit (s16) truncation of the int param -> retail's extsh r0, r4.
    int sum = cur + (s16)delta;
    f->field_0x160C = (u16)sum;
    if ((s16)sum < 0) {
        f->field_0x160C = 0;
    } else if ((s16)sum > max) {
        f->field_0x160C = (u16)max;
    }
}

// Retail symbol is Fv; the real ABI passes (self, delta). Same gauge update
// as CActorParam_UnkVirtualFunc54 but on 0x1614/[0, 0x1616].
void CActorParam_UnkVirtualFunc60__Q22cf11CActorParamFv(cf::CActorParam* self, int delta) {
    cf::CActorParamGaugeView* f = reinterpret_cast<cf::CActorParamGaugeView*>(self);
    s16 cur = f->field_0x1614;
    s16 max = f->field_0x1616;
    int sum = cur + (s16)delta;
    f->field_0x1614 = (u16)sum;
    if ((s16)sum < 0) {
        f->field_0x1614 = 0;
    } else if ((s16)sum > max) {
        f->field_0x1614 = (u16)max;
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc58() {
    reinterpret_cast<cf::CActorParamGaugeView*>(this)->field_0x160C = 0;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc64() {
    reinterpret_cast<cf::CActorParamGaugeView*>(this)->field_0x1614 = 0;
}
// Retail symbol is Fv; the real ABI passes (self, id). Scans the 8
// CActorParam_UnkStruct5 entries at +0x1928 for id; on hit, resets the
// entry's vector fields to the sdata2 defaults.
void CActorParam_UnkVirtualFunc141__Q22cf11CActorParamFv(cf::CActorParam* self, u32 id) {
    cf::CActorParamUnk1928View* view = reinterpret_cast<cf::CActorParamUnk1928View*>(self);
    for (int i = 0; i < 8; i++) {
        cf::CActorParam_UnkStruct5* e = &view->entries[i];
        if (e->unk14 == id) {
            float v0;
            float v;
            v = lbl_eu_806677E4;
            e->unk4 = v;
            v0 = lbl_eu_806677E0;
            e->unk0 = v;
            e->unkC = v;
            e->unk8 = v;
            e->unk10 = v0;
            return;
        }
    }
}
void cf::CActorParam::CActorParam_UnkVirtualFunc142() {
    cf::CActorParamUnk1928View* view = reinterpret_cast<cf::CActorParamUnk1928View*>(this);
    float v = lbl_eu_806677E4;
    float v0 = lbl_eu_806677E0;
    cf::CActorParam_UnkStruct5* e = view->entries;
    e[0].unk4 = v; e[0].unk0 = v; e[0].unkC = v; e[0].unk8 = v; e[0].unk10 = v0;
    e[1].unk4 = v; e[1].unk0 = v; e[1].unkC = v; e[1].unk8 = v; e[1].unk10 = v0;
    e[2].unk4 = v; e[2].unk0 = v; e[2].unkC = v; e[2].unk8 = v; e[2].unk10 = v0;
    e[3].unk4 = v; e[3].unk0 = v; e[3].unkC = v; e[3].unk8 = v; e[3].unk10 = v0;
    e[4].unk4 = v; e[4].unk0 = v; e[4].unkC = v; e[4].unk8 = v; e[4].unk10 = v0;
    e[5].unk4 = v; e[5].unk0 = v; e[5].unkC = v; e[5].unk8 = v; e[5].unk10 = v0;
    e[6].unk4 = v; e[6].unk0 = v; e[6].unkC = v; e[6].unk8 = v; e[6].unk10 = v0;
    e[7].unk4 = v; e[7].unk0 = v; e[7].unkC = v; e[7].unk8 = v; e[7].unk10 = v0;
}
// Retail symbol is Fv; the real ABI passes (self, id). Same scan as
// CActorParam_UnkVirtualFunc141 but zeroes the id and all vector fields.
void CActorParam_UnkVirtualFunc143__Q22cf11CActorParamFv(cf::CActorParam* self, u32 id) {
    cf::CActorParamUnk1928View* view = reinterpret_cast<cf::CActorParamUnk1928View*>(self);
    for (int i = 0; i < 8; i++) {
        cf::CActorParam_UnkStruct5* e = &view->entries[i];
        if (e->unk14 == id) {
            e->unk14 = 0;
            float v = lbl_eu_806677E4;
            e->unk4 = v;
            e->unk0 = v;
            e->unkC = v;
            e->unk8 = v;
            e->unk10 = v;
            return;
        }
    }
}
// us-8017f1d0: retail symbol is Fv; the real ABI passes (self, id).
// Returns a pointer to the CActorParam_UnkStruct5 entry at +0x1928 whose
// unk14 matches id, or NULL. Retail is a fully unrolled scan.
cf::CActorParam_UnkStruct5* CActorParam_UnkVirtualFunc144__Q22cf11CActorParamFv(cf::CActorParam* self, u32 id) {
    cf::CActorParamUnk1928View* view = reinterpret_cast<cf::CActorParamUnk1928View*>(self);
    for (int i = 0; i < 8; i++) {
        cf::CActorParam_UnkStruct5* e = &view->entries[i];
        if (e->unk14 == id) {
            return e;
        }
    }
    return NULL;
}
// us-8017f258: scan the 8 CActorParam_UnkStruct5 entries at 0x1928 (processed
// as 4 pairs per loop iteration) for the entry with the largest rounded sum
// unk4 + (unk10 + unk0); the rounding constant (+/-0.5) is chosen by whether
// the sum exceeds the sdata2 default. Returns the winner's unk14 pointer
// (the first entry to reach the strictly-maximum value), else NULL.
void* cf::CActorParam::CActorParam_UnkVirtualFunc149() {
    cf::CActorParamUnk1928View* view = reinterpret_cast<cf::CActorParamUnk1928View*>(this);
    void* best = NULL;
    int bestVal = 0;
    for (int i = 0; i < 4; i++) {
        cf::CActorParam_UnkStruct5* e = &view->entries[2 * i];
        if (e->unk14 != 0) {
            float sum = e->unk4 + (e->unk10 + e->unk0);
            double d;
            if (sum > lbl_eu_806677E4) {
                d = (double)sum + lbl_eu_80667848;
            } else {
                d = (double)sum + lbl_eu_80667850;
            }
            int v = (int)d;
            if (v > bestVal) {
                best = (void*)e->unk14;
                bestVal = v;
            }
        }
        cf::CActorParam_UnkStruct5* e2 = &view->entries[2 * i + 1];
        if (e2->unk14 != 0) {
            float sum2 = e2->unk4 + (e2->unk10 + e2->unk0);
            double d2;
            if (sum2 > lbl_eu_806677E4) {
                d2 = (double)sum2 + lbl_eu_80667848;
            } else {
                d2 = (double)sum2 + lbl_eu_80667850;
            }
            int v2 = (int)d2;
            if (v2 > bestVal) {
                best = (void*)e2->unk14;
                bestVal = v2;
            }
        }
    }
    return best;
}
// us-8017f334: scan the 8 CActorParam_UnkStruct5 entries at 0x1928 (as 4
// pairs per loop iteration) for the entry with the minimum rounded sum
// unk4 + (unk10 + unk0); the +/-0.5 rounding constant is chosen by whether
// the sum exceeds the sdata2 default, and the initial bestVal of 30000 acts
// as a sentinel (only candidates at/below it can win). Returns the winner's
// unk14 (the first entry to reach the strictly-minimum value), else NULL.
void* cf::CActorParam::CActorParam_UnkVirtualFunc150() {
    cf::CActorParamUnk1928View* view = reinterpret_cast<cf::CActorParamUnk1928View*>(this);
    void* best = NULL;
    int bestVal = 30000;
    int i;
    cf::CActorParam_UnkStruct5* p;
    for (i = 0, p = &view->entries[0]; i < 4; i++, p += 2) {
        cf::CActorParam_UnkStruct5* e = p;
        if (e->unk14 != 0) {
            float sum = e->unk4 + (e->unk10 + e->unk0);
            double d;
            if (sum > lbl_eu_806677E4) {
                d = (double)sum + lbl_eu_80667848;
            } else {
                d = (double)sum + lbl_eu_80667850;
            }
            int v = (int)d;
            if (v <= bestVal) {
                best = (void*)e->unk14;
                bestVal = v;
            }
        }
        cf::CActorParam_UnkStruct5* e2 = p + 1;
        if (e2->unk14 != 0) {
            float sum2 = e2->unk4 + (e2->unk10 + e2->unk0);
            double d2;
            if (sum2 > lbl_eu_806677E4) {
                d2 = (double)sum2 + lbl_eu_80667848;
            } else {
                d2 = (double)sum2 + lbl_eu_80667850;
            }
            int v2 = (int)d2;
            if (v2 <= bestVal) {
                best = (void*)e2->unk14;
                bestVal = v2;
            }
        }
    }
    return best;
}
s16 CActorParam_UnkVirtualFunc155__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x3358); }
s16 CActorParam_UnkVirtualFunc157__Q22cf11CActorParamFv(void* self) { return *(s16*)((u8*)self + 0x335a); }
void cf::CActorParam::CActorParam_UnkVirtualFunc160() {
    cf::CActorParamStatusView* v = reinterpret_cast<cf::CActorParamStatusView*>(this);
    // 0x335A = 2 is stored first; the rate byte (0x335E) is widened to
    // double via the 0x43300000 magic and scaled by the gauge max (0x3368);
    // the truncating half is stored at 0x3358.
    v->field_0x335A = 2;
    v->field_0x3358 = (u16)((int)(v->field_0x3368 * (float)(u32)v->field_0x335E) / 2);
}
float CActorParam_UnkVirtualFunc23__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x15e8); }
void CActorParam_UnkVirtualFunc21__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x15e8) = val; }

void func_80175A50(){}
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
    virtual void _v0A0(); virtual void _v0A4(int); virtual void _v0A8(int); virtual void _v0AC();
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
    virtual float vf130();             // 0x130 CActorParam_UnkVirtualFunc39
    virtual void _v134(); virtual void vf138(float); virtual void vf13C(float);
    virtual void _v140(); virtual void _v144(); virtual void _v148(); virtual void _v14C();
    virtual void vf150(float);         // 0x150 CActorParam_UnkVirtualFunc47 (takes float)
    virtual void vf154(float);         // 0x154 CActorParam_UnkVirtualFunc48
    virtual float vf158();             // 0x158 CActorParam_UnkVirtualFunc49
    virtual void _v15C(); virtual void _v160(); virtual void _v164(); virtual void _v168();
    virtual void vf16C(int);           // 0x16C CActorParam_UnkVirtualFunc54 (takes delta)
    virtual void _v170(); virtual int _v174(); virtual int vf178(); // 0x178 Func57 (getter)
    virtual void _v17C(); virtual void _v180(); virtual void vf184(int); virtual void _v188(); // 0x184 Func60 (takes delta)
    virtual int _v18C(); virtual int vf190(); virtual void _v194(); virtual void _v198(); // 0x190 Func63 (getter)
    virtual float _v19C(); virtual void _v1A0(); virtual void _v1A4(); virtual float _v1A8();
    virtual void _v1AC(); virtual void _v1B0(); virtual void _v1B4(); virtual void _v1B8();
    virtual void _v1BC(); virtual void _v1C0(); virtual void _v1C4(); virtual void _v1C8();
    virtual void _v1CC(); virtual void _v1D0(); virtual void _v1D4(); virtual void _v1D8();
    virtual void _v1DC(); virtual void _v1E0(); virtual void _v1E4(); virtual u32 vf1E8(); // 0x1E8 CActorParam_UnkVirtualFunc85
    virtual void _v1EC(); virtual u32 vf1F0(); // 0x1F0 CActorParam_UnkVirtualFunc87
    virtual void _v1F4(); virtual void _v1F8();
    virtual void _v1FC(); virtual void _v200(); virtual void _v204(); virtual void _v208();
    virtual void _v20C(); virtual void _v210(); virtual void _v214(); virtual void _v218();
    virtual void _v21C(); virtual void _v220();
    virtual void* vf224();             // 0x224 CActorParam_UnkVirtualFunc100
    virtual void _v228(); virtual void _v22C(); virtual void _v230(); virtual void _v234();
    virtual void _v238(); virtual void _v23C(); virtual void _v240(); virtual void _v244();
    virtual void _v248(); virtual void _v24C(); virtual void _v250(); virtual void _v254();
    virtual void _v258(); virtual void _v25C(); virtual void _v260(); virtual void _v264();
    virtual void _v268(); virtual void _v26C(); virtual void _v270(); virtual void _v274();
    virtual void _v278(); virtual void* vf27C(); virtual void _v280(); virtual void _v284();
    virtual void* vf288();            // 0x288 CActorParam_UnkVirtualFunc125
    virtual void _v28C();
    virtual void* vf290();             // 0x290 CActorParam_UnkVirtualFunc127
    virtual void _v294(); virtual void _v298(); virtual void _v29C(); virtual void _v2A0();
    virtual void* _v2A4(); virtual void* vf2A8(); virtual void _v2AC(); virtual void _v2B0();
    virtual void _v2B4(); virtual void _v2B8();
    virtual int vf2BC();               // 0x2BC CActorParam_UnkVirtualFunc138
    virtual void _v2C0(); virtual void _v2C4(); virtual void _v2C8(); virtual void _v2CC();
    virtual void _v2D0(); virtual void _v2D4(); virtual void _v2D8(); virtual void _v2DC();
    virtual void _v2E0(); virtual void _v2E4(); virtual void _v2E8(); virtual void _v2EC();
    virtual void _v2F0(); virtual void _v2F4();
    virtual void vf2F8(int);           // 0x2F8 CActorParam_UnkVirtualFunc153 (takes int)
    virtual void _v2FC();
    virtual void _v300(); virtual void _v304(); virtual void _v308(); virtual void _v30C();
    virtual void _v310(); virtual void _v314(); virtual void _v318(); virtual void _v31C();
    virtual void _v320(); virtual void _v324(); virtual void _v328(); virtual void _v32C();
    virtual void _v330(); virtual void _v334(); virtual void _v338(); virtual void _v33C();
    virtual void _v340(); virtual void _v344(); virtual void _v348();
    virtual void vf34C();              // 0x34C CActorParam_UnkVirtualFunc174
    virtual void vf350(float);         // 0x350 CActorParam_UnkVirtualFunc175
    virtual void _v354(float);         // 0x354 CActorParam_UnkVirtualFunc176
    virtual void vf358(float);         // 0x358 CActorParam_UnkVirtualFunc177
};

struct CBattleStateVt {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C();
    virtual void vf20(int);            // 0x20 CBattleState_UnkVirtualFunc7
    virtual void vf24(void*);          // 0x24 CBattleState_UnkVirtualFunc8
    virtual void vf28(int);            // 0x28 CBattleState_UnkVirtualFunc9
    virtual void _v2C(); virtual void _v30(); virtual void _v34(); virtual void _v38();
    virtual void _v3C(); virtual void _v40(); virtual void _v44(); virtual void _v48();
    virtual void _v4C(); virtual void _v50(); virtual void _v54();
    virtual cf::CBattleStateEntry* vf58(int); // 0x58 CBattleState_UnkVirtualFunc21
    virtual cf::CBattleStateEntry* vf5C(int); // 0x5C CBattleState_UnkVirtualFunc22
    virtual cf::CBattleStateEntry* vf60(int); // 0x60 CBattleState_UnkVirtualFunc23
    virtual void _v64(); virtual void _v68();
    virtual void _v6C();
    virtual void* vf70();             // 0x70
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
    virtual void _v034(); virtual void _v038();
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
extern "C" int func_80145C00(int); // battle-state status-id classifier (CBattleState.cpp)
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
extern "C" int func_80260A6C(void*, int, s32*, s32*); // dual accessor (CUnkObj8025FB10.cpp)
extern "C" int rand(void);
extern "C" void func_802808AC(s32 mode);
extern "C" void func_802A28C4(int a, int b, int c);
extern "C" void func_801748B8(void*, int); // map-item position update (CfMapItemManager TU)

// Map-item timer advance (CfMapItemManager TU; unmangled global). Its body
// reads the same lbl_eu_806677EC sdata2 divisor as func_801765A4.
extern "C" void func_801746B4(u8*, float);

// Arts-stats helpers (unmangled globals). func_8009D7E4 walks the sub-row at
// +0x1C of the char-data object returned by func_8009EC9C; getAtkParam and
// func_8015403C resolve the attack-parameter records.
extern "C" void* func_8009EC9C(u16 index);
extern "C" void* func_8009D7E4(void* base, u32 idx);
extern "C" void* getAtkParam(void* base, int index);
extern "C" int func_8015403C(int x);

// CArtsSet arts-slot helpers (retail: unmangled global symbols defined in
// CArtsSet.cpp; CChainActorList.hpp declares the same pair but cannot be
// included here because CChainEffect.hpp re-declares func_800B3A88 with a
// different signature than CfGimmick.hpp).
extern "C" int getArtsSlotRC(const void* arts, short index, short subindex);
extern "C" void* getArtsParamRC2(const void* arts, int index, int subindex);

// CCharVoiceMan voice-event push (retail: unmangled global symbol; the
// CCharVoiceMan.cpp definition is `void func_802A2C88()`). The retail
// Func48 caller passes two floats the callee ignores, so declare them here
// for the ABI.
extern "C" void func_802A2C88(float a, float b);

// us-801779a0: arts/tension update tick. Divides the delta by the sdata2
// divisor, feeds the actor-id check (unk15DC vtable slot 0x4C) into
// unk2A80, applies battle-state probes and dispatch blocks, and ends by
// calling Func176 (0x354) with the scaled delta.
void func_801765A4(cf::CActorParam* self, int arg, float f1) {
    float f31 = f1 / lbl_eu_806677EC;
    float f30 = f1;
    reinterpret_cast<cf::CActorParam765View*>(self)->field_0x15FC += f31;
    void* r = reinterpret_cast<APIf*>(reinterpret_cast<cf::CActorParam765View*>(self)->field_0x15DC)->vf4C();
    if ((u32)r != reinterpret_cast<cf::CActorParam765View*>(self)->field_0x2A80) {
        reinterpret_cast<cf::CActorParam765View*>(self)->field_0x2A80 =
            (u32)reinterpret_cast<APIf*>(reinterpret_cast<cf::CActorParam765View*>(self)->field_0x15DC)->vf4C();
    }
    u32 t = *(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30()) & 0x3F;
    if (t == 3 || t == 4 || t == 8 || t == 0xE || t == 5) {
        if ((*(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30()) & 0x3F) != 8) {
            func_801746B4((u8*)self + 0x3358, f30);
        }
    }
    reinterpret_cast<CActorParamVt*>(self)->vf358(f31);
    if (arg != 0) {
        reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->_v038();
    }
    reinterpret_cast<CActorParamVt*>(self)->_v0A4(0);
    if ((*(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30()) & 0x3F) == 1 ||
        (*(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30()) & 0x3F) == 2) {
        return;
    }
    reinterpret_cast<CActorParamVt*>(self)->vf350(f30);
    if (reinterpret_cast<CActorParamVt*>(self)->vf2BC() == 0) {
        if (reinterpret_cast<CActorParamVt*>(self)->vf178() != 0 &&
            reinterpret_cast<CActorParamVt*>(self)->_v19C() != lbl_eu_806677E4 &&
            !func_80148778((u8*)self + 8, 0x10) &&
            reinterpret_cast<CActorParamVt*>(self)->vf178() == reinterpret_cast<CActorParamVt*>(self)->_v174()) {
            cf::CBattleStateEntry st;
            std::memset(&st, 0, sizeof(st));
            st.unk00 = 0;
            st.unk04 = 0;
            st.unk0C = 0x10;
            st.unk10 = reinterpret_cast<CActorParamVt*>(self)->vf178();
            st.unk20 = reinterpret_cast<CActorParamVt*>(self)->_v19C();
            st.unk24 = lbl_eu_806677E4;
            func_800EC8FC(getInstance__Q22cf14CBattleManagerFv(), reinterpret_cast<CActorParamVt*>(self)->vf9C(), &st, 0);
            if (!func_80148778((u8*)self + 8, 0x10)) {
                reinterpret_cast<CActorParamVt*>(self)->_v17C();
            }
        }
    }
    if (reinterpret_cast<CActorParamVt*>(self)->vf190() != 0 &&
        reinterpret_cast<CActorParamVt*>(self)->_v1A8() != lbl_eu_806677E4 &&
        !func_80148778((u8*)self + 8, 0xF)) {
        u32 t3;
        if ((*(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30()) & 0x3F) == 6 ||
            (*(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30()) & 0x3F) == 0x12) {
            goto secondBlock;
        }
        t3 = *(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30()) & 0x3F;
        if (t3 == 9 || t3 == 0xA || t3 == 0xB) {
        secondBlock:
            if (reinterpret_cast<CActorParamVt*>(self)->vf190() == reinterpret_cast<CActorParamVt*>(self)->_v18C()) {
                cf::CBattleStateEntry st;
                std::memset(&st, 0, sizeof(st));
                st.unk00 = 0;
                st.unk04 = 0;
                st.unk0C = 0xF;
                st.unk10 = reinterpret_cast<CActorParamVt*>(self)->vf190();
                st.unk20 = reinterpret_cast<CActorParamVt*>(self)->_v1A8();
                st.unk24 = lbl_eu_806677E4;
                st.unk30 |= 0x80;
                func_800EC8FC(getInstance__Q22cf14CBattleManagerFv(), reinterpret_cast<CActorParamVt*>(self)->vf9C(), &st, 0);
                if (!func_80148778((u8*)self + 8, 0x10)) {
                    reinterpret_cast<CActorParamVt*>(self)->_v194();
                }
            }
        }
    }
    reinterpret_cast<CActorParamVt*>(self)->_v354(f31);
}

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
        int r45 = 0;
        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 45) != 0) {
            r45 = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 45);
            if (r45 != 0) {
                float f2 = *(float*)((u8*)self + 0x1808) * (1.0f + (float)r45 / 100.0f);
                float f0 = *(float*)((u8*)self + 0x180C) * (1.0f + (float)r45 / 100.0f);
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

        s16 t45 = (s16)r45;
        if ((float)t45 < 0.7f * (float)*(s16*)((u8*)self + 0x166C)) {
            *(s16*)((u8*)self + 0x1878) = (int)(0.7f * (float)*(s16*)((u8*)self + 0x166C));
        }
        if ((float)*(s16*)((u8*)self + 0x187C) < 0.7f * (float)*(s16*)((u8*)self + 0x1670)) {
            *(s16*)((u8*)self + 0x187C) = (int)(0.7f * (float)*(s16*)((u8*)self + 0x1670));
        }
        if ((float)*(s16*)((u8*)self + 0x187A) < 0.7f * (float)*(s16*)((u8*)self + 0x166E)) {
            *(s16*)((u8*)self + 0x187A) = (int)(0.7f * (float)*(s16*)((u8*)self + 0x166E));
        }
        *(s16*)((u8*)self + 0x1894) += *(u8*)((u8*)self + 0x1898);

        if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 48) != 0) {
            int r = func_8025FB10(reinterpret_cast<CActorParamVt*>(self)->vf290(), 48);
            if (r != 0) {
                void* o = reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf70();
                bool ok = (*(u16*)((u8*)o + 4) == 0) && (*(u16*)((u8*)o + 0x14) == 0) &&
                          (*(u16*)((u8*)o + 0x24) == 0) && (*(u16*)((u8*)o + 0x34) == 0) &&
                          (*(u16*)((u8*)o + 0x44) == 0) && (*(u16*)((u8*)o + 0x54) == 0) &&
                          (*(u16*)((u8*)o + 0x64) == 0) && (*(u16*)((u8*)o + 0x74) == 0);
                if (ok) {
                    *(s16*)((u8*)self + 0x174C) += (s16)r;
                    *(s16*)((u8*)self + 0x1750) += (s16)r;
                    *(s16*)((u8*)self + 0x174E) += (s16)r;
                    *(s16*)((u8*)self + 0x1746) += (s16)r;
                }
            }
        }
    }
}
// us-8017c700: retail symbol is Fv; the real ABI passes (self, arg). When
// arg is NULL the actor's own char-data (0x3F28) is used. Adds each of the
// six arts-stats rows (func_8009D7E4 at +0x1C) into the stat fields, applies
// the 0x1A-type arts-param blend, then updates 0x181A/0x174E from arts
// params and refreshes the 6-slot attack-parameter records.
void CActorParam_UnkVirtualFunc174__Q22cf11CActorParamFv(cf::CActorParam* self, cf::CActorParam174Arg* arg) {
    if (arg == NULL) {
        if (reinterpret_cast<CActorParamVt*>(self)->vf9C() == NULL) return;
        if (!(*(u32*)((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3F00) & 2)) return;
        arg = reinterpret_cast<cf::CActorParam174Arg*>(
            func_8009EC9C(*(u16*)((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3F28)));
    }
    reinterpret_cast<cf::CActorParamArtsStatView*>(self)->field_0x3378 = 0;
    // Reference alias so MWCC folds field access onto `self` (no extra saved reg).
    cf::CActorParamArtsStatView& v = *reinterpret_cast<cf::CActorParamArtsStatView*>(self);
    // Hoisted conversion magics (retail keeps them in f29/f30 across the loop).
    double sm = lbl_eu_806677F8;
    double um = lbl_eu_806677F0;
    float scale = lbl_eu_80667818;
    bool flag = true;
    for (int i = 0; i <= 5; i++) {
        cf::CActorParamArtsRow* row = (cf::CActorParamArtsRow*)func_8009D7E4(&arg->field_0x1C, i);
        v.field_0x1828 += row->field_0x0;
        u32 masked = v.field_0x3374 & 0xFFFC0000;
        v.field_0x182C += row->field_0x4;
        cf::CActorParamF64Conv uc;
        uc.w[0] = 0x43300000;
        uc.w[1] = (u32)(s16)row->field_0xC ^ 0x80000000;
        v.field_0x1808 += (float)(uc.d - sm);
        uc.w[0] = 0x43300000;
        uc.w[1] = (u32)(s16)row->field_0xE ^ 0x80000000;
        v.field_0x180C += (float)(uc.d - sm);
        v.field_0x1820 = (u8)(v.field_0x1820 + (u8)row->field_0x10);
        v.field_0x1824 += row->field_0x8;
        v.field_0x3378 = (s16)(v.field_0x3378 + row->field_0x12);
        v.field_0x1839 = (u8)(v.field_0x1839 + (u8)row->field_0x14);
        v.field_0x1812 = (s16)(v.field_0x1812 + row->field_0x16);
        v.field_0x1816 = (s16)(v.field_0x1816 + row->field_0x18);
        v.field_0x3374 = masked;
        if (row->field_0x1E & 1) v.field_0x3374 |= 0x4000;
        if (row->field_0x1E & 2) v.field_0x3374 |= 0x8000;
        if (reinterpret_cast<CActorParamVt*>(self)->vf290() != NULL && i != 5) {
            int sv;
            cf::CActorParamF64Conv ux;
            switch (row->field_0x1A) {
            case 3:
                if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0xD, &sv)) {
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)sv ^ 0x80000000;
                    float f1 = (float)(ux.d - sm);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(u16)row->field_0x16;
                    float f2 = (float)(ux.d - um);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(s16)v.field_0x1812 ^ 0x80000000;
                    float f0 = (float)(ux.d - sm);
                    v.field_0x1812 = (s16)(int)(f2 * (f1 / scale) + f0);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(u16)row->field_0x18;
                    float g2 = (float)(ux.d - um);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(s16)v.field_0x1816 ^ 0x80000000;
                    float g0 = (float)(ux.d - sm);
                    v.field_0x1816 = (s16)(int)(g2 * (f1 / scale) + g0);
                }
                if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x24, &sv)) {
                    v.field_0x1816 = (s16)(v.field_0x1816 + (s16)sv);
                }
                break;
            case 2:
                if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0xC, &sv)) {
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)sv ^ 0x80000000;
                    float f1 = (float)(ux.d - sm);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(u16)row->field_0x16;
                    float f2 = (float)(ux.d - um);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(s16)v.field_0x1812 ^ 0x80000000;
                    float f0 = (float)(ux.d - sm);
                    v.field_0x1812 = (s16)(int)(f2 * (f1 / scale) + f0);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(u16)row->field_0x18;
                    float g2 = (float)(ux.d - um);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(s16)v.field_0x1816 ^ 0x80000000;
                    float g0 = (float)(ux.d - sm);
                    v.field_0x1816 = (s16)(int)(g2 * (f1 / scale) + g0);
                }
                break;
            case 1:
                if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0xB, &sv)) {
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)sv ^ 0x80000000;
                    float f1 = (float)(ux.d - sm);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(u16)row->field_0x16;
                    float f2 = (float)(ux.d - um);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(s16)v.field_0x1812 ^ 0x80000000;
                    float f0 = (float)(ux.d - sm);
                    v.field_0x1812 = (s16)(int)(f2 * (f1 / scale) + f0);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(u16)row->field_0x18;
                    float g2 = (float)(ux.d - um);
                    ux.w[0] = 0x43300000;
                    ux.w[1] = (u32)(s16)v.field_0x1816 ^ 0x80000000;
                    float g0 = (float)(ux.d - sm);
                    v.field_0x1816 = (s16)(int)(g2 * (f1 / scale) + g0);
                }
                break;
            default: break;
            }
        }
        if (row->field_0x1A != 0) flag = false;
    }
    v.field_0x3374 &= 0xFFFFFE00;
    if (reinterpret_cast<CActorParamVt*>(self)->vf290() != NULL) {
        if (flag) {
            int sv;
            if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x1A, &sv)) {
                v.field_0x181A = (s16)(v.field_0x181A + (s16)sv);
            }
            if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x99, &sv)) {
                v.field_0x174E = (s16)(v.field_0x174E + (s16)sv);
            }
            v.field_0x3374 |= 0x00200000;
        }
        int sv;
        if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x97, &sv)) {
            s16 nv = (s16)(v.field_0x3378 - (s16)sv);
            v.field_0x3378 = nv;
            if (nv < 0) {
                v.field_0x3378 = 0;
            } else if (nv > 100) {
                v.field_0x3378 = 100;
            }
            v.field_0x181A = (s16)(v.field_0x181A + v.field_0x3378);
        }
    }
    cf::CActorParamArtsRow* row5 = (cf::CActorParamArtsRow*)func_8009D7E4(&arg->field_0x1C, 5);
    float def = lbl_eu_806677E4;
    for (int i = 0; i < 6; i++) {
        void* arts = reinterpret_cast<CActorParamVt*>(self)->vf288();
        if (((u16*)arts)[i] == 0) continue;
        void* arts2 = reinterpret_cast<CActorParamVt*>(self)->vf288();
        cf::CAttackParam* atk = (cf::CAttackParam*)getAtkParam(arts2, i);
        atk->unk40 = row5->field_0x1D;
        atk->unk3C = (u16)func_8015403C(row5->field_0x1D);
        float gauge = v.field_0x182C;
        if (gauge == def) {
            gauge = v.field_0x1828;
        }
        atk->unk60 = gauge;
    }
}
// us-8017cdb0: retail symbol is Fv; the real ABI passes (self, dt). Under a
// non-0x04000000 game flag, decay the 8 CActorParam_UnkStruct5 gauges at
// 0x1928: entries whose actor (unk14 -> func_800B708C/8016FE34) is gone or
// busy (Func138 / battle state 0xF8) are reset to the sdata2 default; live
// entries decay unk0 by dt*unk8 and unk4 by dt*unkC, clamped to
// [lbl_eu_806677E8, lbl_eu_80667864].
void CActorParam_UnkVirtualFunc175__Q22cf11CActorParamFv(cf::CActorParam* self, float dt) {
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x04000000)) return;
    cf::CActorParamUnk1928View* view = reinterpret_cast<cf::CActorParamUnk1928View*>(self);
    for (int i = 0; i < 8; i++) {
        cf::CActorParam_UnkStruct5* e = &view->entries[i];
        if (e->unk14 == 0) continue;
        void* actor = func_8016FE34(func_800B708C((int)e->unk14));
        if (actor == NULL || reinterpret_cast<ActorVt*>(actor)->vf2BC() != 0 ||
            func_80148778((u8*)actor + 8, 0xF8) != 0) {
            e->unk14 = 0;
            float def = lbl_eu_806677E4;
            e->unk4 = def;
            e->unk0 = def;
            e->unkC = def;
            e->unk8 = def;
            e->unk10 = def;
            continue;
        }
        float v = e->unk0;
        if (v > lbl_eu_806677E4) {
            float nv = v - dt * e->unk8;
            e->unk0 = nv;
            if (nv < lbl_eu_806677E8) {
                e->unk0 = lbl_eu_806677E8;
            } else if (e->unk0 > lbl_eu_80667864) {
                e->unk0 = lbl_eu_80667864;
            }
        }
        float v2 = e->unk4;
        if (v2 > lbl_eu_806677E4) {
            float nv2 = v2 - dt * e->unkC;
            e->unk4 = nv2;
            if (nv2 < lbl_eu_806677E8) {
                e->unk4 = lbl_eu_806677E8;
            } else if (e->unk4 > lbl_eu_80667864) {
                e->unk4 = lbl_eu_80667864;
            }
        }
    }
}
// us-8017cf48: retail symbol is Fv; the real ABI passes (self, f1). Sums the
// battle-state entry pairs at self+0x10 (0x2A/0x2C ops into one counter,
// 0x2B/0x2D into another), clamps the sums, derives per-axis gauge deltas
// from f1, applies arts-param percentage scaling (0x5C/0x5D/0x9D/0x5E/0x5F),
// then decays the 3x8 arts matrix (unk7C/unk80) with the scaled deltas.
void CActorParam_UnkVirtualFunc176__Q22cf11CActorParamFv(cf::CActorParam* self, float f1) {
    void* bm = getInstance__Q22cf14CBattleManagerFv();
    u8 ch = *(u8*)((u8*)bm + 0x1AA);
    if (ch >= 1 && ch <= 0x18) return;
    int sum4 = 0;
    int sum5 = 0;
    cf::CBattleStateEntry* es = reinterpret_cast<cf::CBattleStateEntry*>((u8*)self + 0x10);
    for (int i = 0; i < 0x34; i++) {
        cf::CBattleStateEntry* e = &es[2 * i];
        switch (e->unk0C) {
        case 0x2A: sum4 += e->unk10; break;
        case 0x2C: sum4 -= e->unk10; break;
        case 0x2B: sum5 += e->unk10; break;
        case 0x2D: sum5 -= e->unk10; break;
        default: break;
        }
        cf::CBattleStateEntry* e2 = &es[2 * i + 1];
        switch (e2->unk0C) {
        case 0x2A: sum4 += e2->unk10; break;
        case 0x2C: sum4 -= e2->unk10; break;
        case 0x2B: sum5 += e2->unk10; break;
        case 0x2D: sum5 -= e2->unk10; break;
        default: break;
        }
    }
    if (sum4 < -0x32) sum4 = -0x32;
    else if (sum4 > 0x32) sum4 = 0x32;
    if (sum5 < -0x64) sum5 = -0x64;
    else if (sum5 > 0x64) sum5 = 0x64;
    // s32->float via the named 0x4330000080000000 magic (union pins the reloc).
    cf::CActorParamF64Conv uc;
    uc.w[0] = 0x43300000;
    uc.w[1] = (u32)(sum4 + 100) ^ 0x80000000;
    float f31 = f1 / ((float)(uc.d - lbl_eu_806677F8) / lbl_eu_80667818);
    cf::CActorParamF64Conv uc2;
    uc2.w[0] = 0x43300000;
    uc2.w[1] = (u32)(sum5 + 100) ^ 0x80000000;
    float f30 = f1 / ((float)(uc2.d - lbl_eu_806677F8) / lbl_eu_80667818);
    float f29 = f31;
    float f28 = f30;
    if (reinterpret_cast<CActorParamVt*>(self)->vf290() != NULL) {
        int sv;
        cf::CActorParamF64Conv ux;
        if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x5C, &sv)) {
            ux.w[0] = 0x43300000;
            ux.w[1] = (u32)(100 - sv) ^ 0x80000000;
            f31 /= (float)(ux.d - lbl_eu_806677F8) / lbl_eu_80667818;
        }
        if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x5D, &sv)) {
            ux.w[0] = 0x43300000;
            ux.w[1] = (u32)(100 - sv) ^ 0x80000000;
            f29 = f31 / ((float)(ux.d - lbl_eu_806677F8) / lbl_eu_80667818);
        }
        if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x9D, &sv)) {
            ux.w[0] = 0x43300000;
            ux.w[1] = (u32)(100 - sv) ^ 0x80000000;
            f28 = f30 / ((float)(ux.d - lbl_eu_806677F8) / lbl_eu_80667818);
        }
        if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x5E, &sv) &&
            cf::CfGameManager::func_80086DBC() != 4) {
            ux.w[0] = 0x43300000;
            ux.w[1] = (u32)(100 - sv) ^ 0x80000000;
            f30 /= (float)(ux.d - lbl_eu_806677F8) / lbl_eu_80667818;
            ux.w[0] = 0x43300000;
            ux.w[1] = (u32)(100 - sv) ^ 0x80000000;
            f1 /= (float)(ux.d - lbl_eu_806677F8) / lbl_eu_80667818;
        }
        if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x5F, &sv) &&
            cf::CfGameManager::func_80086DBC() == 4) {
            ux.w[0] = 0x43300000;
            ux.w[1] = (u32)(100 - sv) ^ 0x80000000;
            f30 /= (float)(ux.d - lbl_eu_806677F8) / lbl_eu_80667818;
            ux.w[0] = 0x43300000;
            ux.w[1] = (u32)(100 - sv) ^ 0x80000000;
            f1 /= (float)(ux.d - lbl_eu_806677F8) / lbl_eu_80667818;
        }
    }
    float f26 = lbl_eu_806677E4;
    for (int r29 = 0; r29 < 3; r29++) {
        for (int r28 = 0; r28 < 8; r28++) {
            void* arts = reinterpret_cast<CActorParamVt*>(self)->vf27C();
            if (!getArtsSlotRC(arts, r29, r28)) continue;
            void* arts2 = reinterpret_cast<CActorParamVt*>(self)->vf27C();
            cf::CAttackParam* p = (cf::CAttackParam*)getArtsParamRC2(arts2, r29, r28);
            u32 t = *(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30()) & 0x3F;
            if (t == 0x16 || t == 0x17 || t == 0xF) {
                // no unk7C decay for these actor states
            } else {
                u32 t2 = *(u32*)(reinterpret_cast<Unk4Vt*>(self->CActorState::unk4)->vf30()) & 0x3F;
                if (t2 != 0x18 && !func_80148778((u8*)self + 8, 0xF)) {
                    switch (p->unk48) {
                    case 0xD5: case 0x11E: case 0x107: case 0xFA:
                        if (p->unk7C > f26) {
                            p->unk7C -= f29;
                            if (p->unk7C < f26) p->unk7C = f26;
                        }
                        break;
                    default:
                        if (p->unk7C > f26) {
                            p->unk7C -= f31;
                            if (p->unk7C < f26) p->unk7C = f26;
                        }
                        break;
                    }
                }
            }
            if (!func_80148778((u8*)self + 8, 0xF) && !func_80148778((u8*)self + 8, 0xC) &&
                !func_80148778((u8*)self + 8, 0xF6)) {
                switch (p->unk48) {
                case 0xD5: case 0x11E: case 0x107: case 0xFA:
                    if (p->unk80 > f26) {
                        p->unk80 -= f28;
                        if (p->unk80 < f26) p->unk80 = f26;
                    }
                    break;
                case 0xEF:
                    if (p->unk80 > f26) {
                        p->unk80 -= f1;
                        if (p->unk80 < f26) p->unk80 = f26;
                    }
                    break;
                default:
                    if (p->unk80 > f26) {
                        p->unk80 -= f30;
                        if (p->unk80 < f26) p->unk80 = f26;
                    }
                    break;
                }
            }
        }
    }
}
// us-8017d594: retail symbol is Fv; the real ABI passes (self, arg). Refreshes
// the arts gauge (Func48) from a target object: gates on the target pointer,
// its bit-0 status flag and (unless mode 1) the Func133 gate; scales the
// 0x98/0xC9 battle-state entries into gauge deltas, then pushes either the
// 0x162A/0x1629 byte gauges or the target's own slot-0xC value through
// Func48.
void CActorParam_UnkVirtualFunc10__Q22cf11CActorParamFv(cf::CActorParam* self, cf::CActorParam10Arg* arg) {
    cf::CActorParam10Target* tgt = (cf::CActorParam10Target*)arg->field_0x50;
    if (tgt == NULL) return;
    if (!(arg->field_0x74 & 1)) return;
    if (tgt->field_0x44 != 1) {
        if (reinterpret_cast<CActorParamVt*>(self)->vf2A8() != 0) return;
    }
    float cur = lbl_eu_806677E8;
    float max = lbl_eu_806677E4;
    if (func_80148778((u8*)self + 8, 0x98) != 0) {
        cur = lbl_eu_806677E8 + (float)(u32)*(u32*)((u8*)func_80149154((u8*)self + 8, 0x98) + 0x10) / lbl_eu_80667818;
        max = lbl_eu_806677E4 + (float)(u32)*(s16*)((u8*)func_80149154((u8*)self + 8, 0x98) + 0x14);
    }
    if (tgt->field_0x78 & 0x40000000) {
        if (reinterpret_cast<CActorParamVt*>(self)->vf9C() != NULL) {
            void* o1 = reinterpret_cast<CActorParamVt*>(self)->vf9C();
            if (*(u32*)((u8*)o1 + 0x3F00) & 2) {
                void* o2 = reinterpret_cast<CActorParamVt*>(self)->vf9C();
                if (*(u16*)((u8*)o2 + 0x3F28) == 5) {
                    if (func_80148778((u8*)self + 8, 0xC9) != 0) {
                        cur = cur * (lbl_eu_806677E8 - (float)(u32)*(u32*)((u8*)func_80149154((u8*)self + 8, 0xC9) + 0x10) / lbl_eu_80667818);
                    }
                }
            }
        }
        if (!(tgt->field_0x78 & 0x4000)) {
            reinterpret_cast<CActorParamVt*>(self)->vf154((float)self->unk162A * cur + max);
        }
        if (tgt->field_0x78 & 0x20) {
            reinterpret_cast<CActorParamVt*>(self)->vf154((float)(u32)tgt->vt->vf0C());
        }
    } else {
        reinterpret_cast<CActorParamVt*>(self)->vf154((float)self->unk1629 * cur + max);
        if (func_80148778((u8*)self + 8, 0xC8) != 0) {
            reinterpret_cast<CActorParamVt*>(self)->vf154((float)(u32)*(u32*)((u8*)func_80149154((u8*)self + 8, 0xC8) + 0x10));
        }
    }
}
// us-8017d83c: retail symbol is Fv; the real ABI passes (self, arg) where
// arg carries a u32 status word at +0x74 (bit 0x100 gate). When the actor
// state (Func127) is active, the battle-state gate (Func138) is clear and
// the flag is set: read the arts-slot value (func_80260264 id 0x45), scale
// it by the battle-rate factor (func_800D81A8 over the actor), the actor
// gauge (Func38) and an sdata2 constant, round to int, and feed the result
// back through the gauge setter Func34 (slot 0x11C).
void CActorParam_UnkVirtualFunc11__Q22cf11CActorParamFv(cf::CActorParam* self, cf::CActorParam11Arg* arg) {
    if (reinterpret_cast<CActorParamVt*>(self)->vf290() == NULL) return;
    if (reinterpret_cast<CActorParamVt*>(self)->vf2BC() != 0) return;
    if ((arg->field_0x74 & 0x100) == 0) return;
    int val;
    if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x45, &val) == 0) return;
    getInstance__Q22cf14CBattleManagerFv();
    void* actor = reinterpret_cast<CActorParamVt*>(self)->vf9C();
    float a = func_800D81A8(reinterpret_cast<CActorParamVt*>(self)->vf9C(), actor, NULL);
    float b = reinterpret_cast<CActorParamVt*>(self)->vf12C();
    double magic = lbl_eu_806677F8;
    cf::CActorParamF64Conv u;
    u.w[0] = 0x43300000;
    u.w[1] = (u32)val ^ 0x80000000;
    float conv = (float)(u.d - magic);
    float v = a * (lbl_eu_80667830 * (conv * b));
    int iv = (int)v;
    cf::CActorParamF64Conv u2;
    u2.w[0] = 0x43300000;
    u2.w[1] = (u32)iv ^ 0x80000000;
    reinterpret_cast<CActorParamVt*>(self)->vf11C((float)(u2.d - magic));
}
// us-8017d9a0: retail symbol is Fv; the real ABI passes (self, arg) where
// arg carries a flags block (+0x50 -> +0x78), a u16 dispatch id (+0x80) and
// an int roll value (+0xA8). Battle-action tick: gauge sets (Func48), battle-
// state dispatch blocks 0x121/0x6/0x9/0xF8/0x36/0x37/0x11E (rand-gated,
// battle-rate-scaled damage/SP with the CBattleState slot-0x20 trigger), then
// the unk15E0 (Func127) block: 0x37/0x38 stat transfers, 0x46 arts-gauge roll
// and an actor sweep (list type 0x20, filter 0x800) applying the arts
// magnitude to the target.
void CActorParam_UnkVirtualFunc12__Q22cf11CActorParamFv(cf::CActorParam* self, cf::CActorParam12Arg* arg) {
    u32 flags = ((cf::CActorParam12Flags*)arg->field_0x50)->field_0x78;
    if (flags & 0x40000000) {
        if (!(flags & 0x4000)) {
            reinterpret_cast<CActorParamVt*>(self)->vf154((float)((cf::CActorParam162BView*)self)->field_0x162C);
        }
    } else {
        reinterpret_cast<CActorParamVt*>(self)->vf154((float)((cf::CActorParam162BView*)self)->field_0x162B);
    }

    if (func_80148778((u8*)self + 8, 0x121)) {
        cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x121);
        reinterpret_cast<CActorParamVt*>(self)->vf154((float)e->unk10);
    }

    if (func_80148778((u8*)self + 8, 0x6)) {
        if (rand() % 100 < 0x19) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x6);
            if (e->unk2C != arg->field_0x80) {
                reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf20(0x6);
            }
        }
    }

    if (func_80148778((u8*)self + 8, 0x9)) {
        cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x9);
        if (e->unk2C != arg->field_0x80) {
            reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf20(0x9);
        }
    }

    if (func_80148778((u8*)self + 8, 0xf8)) {
        if (reinterpret_cast<CActorParamVt*>(self)->vf2BC() == 0) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0xf8);
            if (e->unk2C != arg->field_0x80) {
                reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf20(0xf8);
            }
        }
    }

    if (func_80148778((u8*)self + 8, 0x36)) {
        cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x36);
        if (e != NULL) {
            if (reinterpret_cast<CActorParamVt*>(self)->vf2BC() == 0) {
                float f31 = reinterpret_cast<CActorParamVt*>(self)->vf12C();
                if (reinterpret_cast<CActorParamVt*>(self)->vf128() != f31) {
                    s32 unk10 = e->unk10;
                    getInstance__Q22cf14CBattleManagerFv();
                    float rate = func_800D81A8(0, reinterpret_cast<CActorParamVt*>(self)->vf9C(), 0);
                    int v = (int)((float)unk10 * rate);
                    func_800E9FE4(getInstance__Q22cf14CBattleManagerFv(),
                                  reinterpret_cast<CActorParamVt*>(self)->vf9C(),
                                  (int)(lbl_eu_80667868 * (float)v), 0, 0, 1,
                                  (void*)(uintptr_t)((cf::CActorParam12ActorView*)reinterpret_cast<CActorParamVt*>(self)->vf9C())->field_0x3F10);
                    reinterpret_cast<CActorParamVt*>(self)->vf11C((float)v);
                    if (e->unk14 > 0) {
                        e->unk14--;
                        if (e->unk14 == 0) {
                            reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf20(0x36);
                        }
                    }
                }
            }
        }
    }

    if (func_80148778((u8*)self + 8, 0x37)) {
        u32 r26 = 0;
        if (func_80148778((u8*)self + 8, 0xa1)) {
            cf::CBattleStateEntry* ea = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0xa1);
            r26 = ea->unk10;
        }
        cf::CBattleStateEntry* e37 = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x37);
        if (e37 != NULL) {
            if (reinterpret_cast<CActorParamVt*>(self)->vf2BC() == 0) {
                float f31 = reinterpret_cast<CActorParamVt*>(self)->vf12C();
                if (reinterpret_cast<CActorParamVt*>(self)->vf128() != f31) {
                    u32 unk8 = e37->unk08;
                    int unk1A = e37->unk1A;
                    if (unk8 == 0x2000) {
                        unk1A += r26;
                    }
                    if (arg->field_0xA8 % 100 < unk1A) {
                        s32 unk10 = e37->unk10;
                        getInstance__Q22cf14CBattleManagerFv();
                        float rate = func_800D81A8(0, reinterpret_cast<CActorParamVt*>(self)->vf9C(), 0);
                        int v = (int)((float)unk10 * rate);
                        reinterpret_cast<CActorParamVt*>(self)->vf11C((float)v);
                    }
                }
            }
        }
    }

    if (func_80148778((u8*)self + 8, 0x11e)) {
        cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)func_80149154((u8*)self + 8, 0x11e);
        if (e != NULL) {
            if (reinterpret_cast<CActorParamVt*>(self)->vf2BC() == 0) {
                float f31 = reinterpret_cast<CActorParamVt*>(self)->vf12C();
                if (reinterpret_cast<CActorParamVt*>(self)->vf128() != f31) {
                    int v = (int)((float)e->unk14 * reinterpret_cast<CActorParamVt*>(self)->vf12C() / lbl_eu_80667818);
                    getInstance__Q22cf14CBattleManagerFv();
                    float rate = func_800D81A8(0, reinterpret_cast<CActorParamVt*>(self)->vf9C(), 0);
                    func_800E9FE4(getInstance__Q22cf14CBattleManagerFv(),
                                  reinterpret_cast<CActorParamVt*>(self)->vf9C(),
                                  (int)(lbl_eu_80667868 * (float)v), 0, 0, 1,
                                  (void*)(uintptr_t)((cf::CActorParam12ActorView*)reinterpret_cast<CActorParamVt*>(self)->vf9C())->field_0x3F10);
                    reinterpret_cast<CActorParamVt*>(self)->vf11C((float)v);
                    if (e->unk10 > 0) {
                        e->unk10--;
                        if (e->unk10 == 0) {
                            reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf20(0x11e);
                        }
                    }
                }
            }
        }
    }

    if (reinterpret_cast<CActorParamVt*>(self)->vf290() != NULL) {
        if (((cf::CActorParam12ActorView*)reinterpret_cast<CActorParamVt*>(self)->vf9C())->field_0x3F00 & 2) {
            if (reinterpret_cast<CActorParamVt*>(self)->vf2BC() == 0) {
                s32 v;
                if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x37, &v)) {
                    reinterpret_cast<CActorParamVt*>(self)->vf2F8(v);
                }
                if (func_80260264(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x38, &v)) {
                    reinterpret_cast<CActorParamVt*>(self)->vf154((float)v);
                }
                s32 v2;
                if (func_80260A6C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x46, &v, &v2)) {
                    if (ml::math::mtRand(100) < v2) {
                        if (reinterpret_cast<CActorParamVt*>(self)->vf130() < lbl_eu_8066786C) {
                            u8 holder[8];
                            func_80043D90(holder);
                            func_800F4A98(func_80043F18(holder), 0x20, 0x800);
                            for (u32 i = 0; i < ((cf::CfObjEnumList*)func_80043F18(holder))->mPtrCount; i++) {
                                cf::CActorParam12ActorView* actor = (cf::CActorParam12ActorView*)func_8016FE34(func_800F6EAC(func_80043F18(holder), i));
                                if (actor->field_0x3F28 == 1) {
                                    int vv = (int)((float)v * (reinterpret_cast<CActorParamVt*>(self)->vf12C() / lbl_eu_80667818));
                                    getInstance__Q22cf14CBattleManagerFv();
                                    float rate = func_800D81A8(0, reinterpret_cast<CActorParamVt*>(self)->vf9C(), 0);
                                    int vv2 = (int)((float)vv * rate);
                                    reinterpret_cast<CActorParamVt*>(self)->vf11C((float)vv2);
                                    break;
                                }
                            }
                            __dt__80043E88(holder, -1);
                        }
                    }
                }
            }
        }
    }
}
// us-8017e2fc: four chained battle/status updates - reset the two gauges
// (Func58/64), then refresh the battle action (Func103) and status (Func9).
void cf::CActorParam::CActorParam_UnkVirtualFunc13() {
    CActorParam_UnkVirtualFunc58();
    CActorParam_UnkVirtualFunc64();
    CActorParam_UnkVirtualFunc103();
    CActorParam_UnkVirtualFunc9();
}
// us-8017e370: retail symbol is Fv; the real ABI passes (self, arg) where
// arg carries a u16 dispatch id at +0xC (CfObjectActor's override calls this
// base with the same arg). Id 0x10 -> getter slot 0x178 then setter 0x16C;
// id 0xF -> getter slot 0x190 then setter 0x184.
void CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv(cf::CActorParam* self, cf::CActorParam179Arg* arg) {
    switch ((int)arg->field_0xC) {
    case 0x10: {
        int v = reinterpret_cast<CActorParamVt*>(self)->vf178();
        reinterpret_cast<CActorParamVt*>(self)->vf16C(v);
        break;
    }
    case 0xF: {
        int v = reinterpret_cast<CActorParamVt*>(self)->vf190();
        reinterpret_cast<CActorParamVt*>(self)->vf184(v);
        break;
    }
    default:
        break;
    }
}
// us-8017e474: retail symbol is Fv; the real ABI passes (self, arg) where
// arg carries a u16 dispatch id at +0xC (same arg family as Func179). The
// getInstance pair is a no-op guard; then, when func_80145C00 classifies the
// id and battle state 0x90 is active, re-queues an id-0x34 entry carrying
// the actor's 0x3F10 id (func_80149330) into the CBattleState subobject
// (slot 0x24). Finally dispatches on the id: 0x10 -> Func58, 0xF -> Func64,
// 0x11 -> re-raise battle state 0x11 on the target actor (id from arg+0x10,
// s16 gate at arg+0x14).
void CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv(cf::CActorParam* self, cf::CActorParam180Arg* arg) {
    if (getInstance__Q22cf14CBattleManagerFv() != NULL) {
        getInstance__Q22cf14CBattleManagerFv();
    }
    if (func_80145C00(arg->field_0xC) != 0) {
        if (func_80148778((u8*)self + 8, 0x90) != 0) {
            void* r = func_80149330((u8*)self + 8, 0x34,
                *(u32*)((u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C() + 0x3F10), 0x90, 0);
            if (r != NULL) {
                reinterpret_cast<CBattleStateVt*>((u8*)self + 8)->vf24(r);
            }
        }
    }
    switch ((int)arg->field_0xC) {
    case 0x10:
        reinterpret_cast<CActorParamVt*>(self)->_v17C();
        break;
    case 0xF:
        reinterpret_cast<CActorParamVt*>(self)->_v194();
        break;
    case 0x11:
        if (arg->field_0x10 != NULL) {
            if (arg->field_0x14 == 0) {
                void* actor = func_8016FE34(func_800B708C((int)arg->field_0x10));
                if (actor != NULL) {
                    reinterpret_cast<CBattleStateVt*>((u8*)actor + 8)->vf20(0x11);
                }
            }
        }
        break;
    default:
        break;
    }
}
// us-8017e5b8: retail symbol is Fv; the real ABI passes (self, flag). With
// flag!=0 feeds the 0x17F4/0x17F8 floats to vtable slots 0x118/0x138
// (Func33/41); with flag==0 to slots 0x11C/0x13C (Func34/42).
void CActorParam_UnkVirtualFunc5__Q22cf11CActorParamFv(cf::CActorParam* self, int flag) {
    if (flag != 0) {
        reinterpret_cast<CActorParamVt*>(self)->vf118(*(float*)((u8*)self + 0x17f4));
        reinterpret_cast<CActorParamVt*>(self)->vf138(*(float*)((u8*)self + 0x17f8));
    } else {
        reinterpret_cast<CActorParamVt*>(self)->vf11C(*(float*)((u8*)self + 0x17f4));
        reinterpret_cast<CActorParamVt*>(self)->vf13C(*(float*)((u8*)self + 0x17f8));
    }
}
// us-8017e644: retail symbol is Fv; the real ABI passes (self, val).
// Clears the bit-25 status flag at 0x3374, then with val==0 triggers the
// battle-start cue Func5(1), otherwise feeds val (as float) to Func34.
void CActorParam_UnkVirtualFunc6__Q22cf11CActorParamFv(cf::CActorParam* self, int val) {
    reinterpret_cast<cf::CActorParamFlagsView*>(self)->field_0x3374 &= ~0x02000000;
    if (val == 0) {
        reinterpret_cast<CActorParamVt*>(self)->_v0A8(1);
    } else {
        reinterpret_cast<CActorParamVt*>(self)->vf11C((float)val);
    }
}
// us-8017e6b8: raise the battle-state cue (slot 0x20) for the 0x1530 arts
// id, then sweep the three arts-slot groups (CBattleState slots 0x5C/0x58/
// 0x60): re-queue (slot 0x24) every entry whose id is set and that is not
// idle at the default gauge with the 0x1C busy mask clear. Finally, when
// the actor's arts-gauge type is the 0x3F28==5 group, feed the default
// gauge value to slot 0x150.
void cf::CActorParam::CActorParam_UnkVirtualFunc9() {
    u32 cue = *(u32*)((u8*)this + 0x1530);
    if (cue != 0) {
        reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf20(cue);
    }
    for (int i = 0; i < 0x20; i++) {
        cf::CBattleStateEntry* e = reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf5C(i);
        if (e->unk0C != 0) {
            cf::CBattleStateEntry* e2 = reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf5C(i);
            if (!(e2->unk08 & 0x1C) || reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf5C(i)->unk20 != lbl_eu_806677E4) {
                reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf24(reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf5C(i));
            }
        }
    }
    for (int i = 0; i < 0x20; i++) {
        cf::CBattleStateEntry* e = reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf58(i);
        if (e->unk0C != 0) {
            cf::CBattleStateEntry* e2 = reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf58(i);
            if (!(e2->unk08 & 0x1C) || reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf58(i)->unk20 != lbl_eu_806677E4) {
                reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf24(reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf58(i));
            }
        }
    }
    for (int i = 0; i < 0x20; i++) {
        cf::CBattleStateEntry* e = reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf60(i);
        if (e->unk0C != 0) {
            cf::CBattleStateEntry* e2 = reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf60(i);
            if (!(e2->unk08 & 0x1C) || reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf60(i)->unk20 != lbl_eu_806677E4) {
                reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf24(reinterpret_cast<CBattleStateVt*>((u8*)this + 8)->vf60(i));
            }
        }
    }
    if (reinterpret_cast<CActorParamVt*>(this)->vf9C() != NULL) {
        void* o1 = reinterpret_cast<CActorParamVt*>(this)->vf9C();
        if (*(u32*)((u8*)o1 + 0x3F00) & 2) {
            void* o2 = reinterpret_cast<CActorParamVt*>(this)->vf9C();
            if (*(u16*)((u8*)o2 + 0x3F28) == 5) {
                reinterpret_cast<CActorParamVt*>(this)->vf150(lbl_eu_806677E4);
            }
        }
    }
}
// us-8017e9d4: retail symbol is Fv; the real ABI passes (self, delta). Adds
// delta to the arts gauge at 0x17F0, clamps it to [default, 0x17FC], then
// pushes the two Func49 gauge readings (second first) to func_802A2C88.
void CActorParam_UnkVirtualFunc48__Q22cf11CActorParamFv(cf::CActorParam* self, float delta) {
    cf::CActorParamArtsGaugeView* v = reinterpret_cast<cf::CActorParamArtsGaugeView*>(self);
    float b;
    float a = self->CActorParam_UnkVirtualFunc49();
    float cur = v->field_0x17F0 + delta;
    float max = v->field_0x17FC;
    v->field_0x17F0 = cur;
    if (cur < lbl_eu_806677E4) {
        v->field_0x17F0 = lbl_eu_806677E4;
    } else if (cur > max) {
        v->field_0x17F0 = max;
    }
    b = self->CActorParam_UnkVirtualFunc49();
    self->CActorParam_UnkVirtualFunc2();
    func_802A2C88(b, a);
}
// us-8017eab4: find the battle-state entry with id 0x10 in the CBattleState
// subobject (this+8) and reset its gauge (unk20) to the sdata2 default.
void cf::CActorParam::CActorParam_UnkVirtualFunc67() {
    cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)func_80149154((u8*)this + 8, 0x10);
    if (e != NULL) {
        e->unk20 = lbl_eu_806677E4;
    }
}
// Finds the battle-state entry with id 0xF in the CBattleState subobject
// (this+8) and resets its gauge (unk20) to the sdata2 default (sibling of
// CActorParam_UnkVirtualFunc67 which uses id 0x10).
void cf::CActorParam::CActorParam_UnkVirtualFunc70() {
    cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)func_80149154((u8*)this + 8, 0xF);
    if (e != NULL) {
        e->unk20 = lbl_eu_806677E4;
    }
}
// us-8017eb24: reset every arts-slot record's unk80 (the 3x8 arts matrix)
// to the sdata2 default. The arts set comes from the Func122 vtable slot
// (called twice per slot: once for the slot probe, once for the record).
void cf::CActorParam::CActorParam_UnkVirtualFunc7() {
    const float def = lbl_eu_806677E4;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
            void* arts = reinterpret_cast<CActorParamVt*>(this)->vf27C();
            if (getArtsSlotRC(arts, i, j) != 0) {
                void* p = getArtsParamRC2(reinterpret_cast<CActorParamVt*>(this)->vf27C(), i, j);
                ((cf::CAttackParam*)p)->unk80 = def;
            }
        }
    }
}

// us-8017ebdc: same arts-matrix reset as CActorParam_UnkVirtualFunc7 but on
// each slot record's unk7C field instead of unk80.
void cf::CActorParam::CActorParam_UnkVirtualFunc8() {
    float def = lbl_eu_806677E4;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
            void* arts = reinterpret_cast<CActorParamVt*>(this)->vf27C();
            if (getArtsSlotRC(arts, i, j) != 0) {
                void* p = getArtsParamRC2(reinterpret_cast<CActorParamVt*>(this)->vf27C(), i, j);
                ((cf::CAttackParam*)p)->unk7C = def;
            }
        }
    }
}
// us-8017ec94: returns true while the arts gauge (Func37) is at/below the
// sdata2 default, or when the actor-state id (obj at +4, vtable slot 0x30,
// masked 0x3F) is 0x1C or 0x1E.
bool cf::CActorParam::CActorParam_UnkVirtualFunc138() {
    if (reinterpret_cast<CActorParamVt*>(this)->vf128() <= lbl_eu_806677E4) {
        return true;
    }
    u32 t = *(u32*)(reinterpret_cast<Unk4Vt*>(CActorState::unk4)->vf30());
    if ((t & 0x3F) == 0x1C) return true;
    t = *(u32*)(reinterpret_cast<Unk4Vt*>(CActorState::unk4)->vf30());
    if ((t & 0x3F) == 0x1E) return true;
    return false;
}
// us-8017ed30: retail symbol is Fv; the real ABI passes (self, arg, f1, f2,
// f3). Adds the three deltas to the 8-entry arts gauge block at 0x1928 for
// the entry tracking the actor (id from arg+0x3F10), clamping to
// [lbl_eu_806677E8, lbl_eu_80667864]; if no entry matches, a fresh slot is
// initialised with the deltas (f1 clamped, 0x10 = f2/f3 spread).
void CActorParam_UnkVirtualFunc140__Q22cf11CActorParamFv(cf::CActorParam* self, cf::CActorParam140Target* arg, float f1, float f2, float f3) {
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x04000000)) return;
    if (arg == NULL) return;
    u32 actorId = arg->field_0x3F10;
    int firstFree = -1; // live across the probes below (retail keeps it in a saved reg)
    if (func_80148778((u8*)self + 8, 0xF)) return;
    if (func_80148778(arg->cbstate, 0xC7)) return;
    if (func_80148778(arg->cbstate, 0x101)) {
        if (f1 != lbl_eu_806677E4) f1 = lbl_eu_806677E8;
        if (f2 != lbl_eu_806677E4) f2 = lbl_eu_806677E8;
        if (f3 != lbl_eu_806677E4) f3 = lbl_eu_806677E8;
    }
    cf::CActorParamUnk1928View* view = reinterpret_cast<cf::CActorParamUnk1928View*>(self);
    for (int i = 0; i < 8; i++) {
        cf::CActorParam_UnkStruct5* e = &view->entries[i];
        if (firstFree == -1 && e->unk14 == 0) {
            firstFree = i;
        }
        if (e->unk14 == actorId) {
            e->unk10 = e->unk10 + f1;
            if (e->unk10 < lbl_eu_806677E8) {
                e->unk10 = lbl_eu_806677E8;
            } else if (e->unk10 > lbl_eu_80667864) {
                e->unk10 = lbl_eu_80667864;
            }
            if (f2 != lbl_eu_806677E4) {
                e->unk0 = e->unk0 + f2;
                e->unk8 = lbl_eu_80667800;
                if (e->unk0 < lbl_eu_806677E8) {
                    e->unk0 = lbl_eu_806677E8;
                } else if (e->unk0 > lbl_eu_80667864) {
                    e->unk0 = lbl_eu_80667864;
                }
                if (e->unk8 < lbl_eu_806677E8) {
                    e->unk8 = lbl_eu_806677E8;
                } else if (e->unk8 > lbl_eu_80667864) {
                    e->unk8 = lbl_eu_80667864;
                }
            }
            if (f3 != lbl_eu_806677E4) {
                e->unk4 = e->unk4 + f3;
                e->unkC = (f3 / lbl_eu_806677EC) / lbl_eu_806677EC;
                if (e->unk4 < lbl_eu_806677E8) {
                    e->unk4 = lbl_eu_806677E8;
                } else if (e->unk4 > lbl_eu_80667864) {
                    e->unk4 = lbl_eu_80667864;
                }
                if (e->unkC < lbl_eu_806677E8) {
                    e->unkC = lbl_eu_806677E8;
                } else if (e->unkC > lbl_eu_80667864) {
                    e->unkC = lbl_eu_80667864;
                }
            }
            return;
        }
    }
    if (firstFree == -1) return;
    cf::CActorParam_UnkStruct5* e = &view->entries[firstFree];
    e->unk14 = actorId;
    e->unk0 = f2;
    e->unk4 = f3;
    e->unk10 = f1;
    e->unk8 = lbl_eu_80667804;
    e->unkC = lbl_eu_80667804;
    // float->double->float round-trip pins retail's frsp on the param compare
    // (MWCC_REFERENCE func_800B06A4 pattern).
    if ((float)(double)f1 < lbl_eu_806677E8) {
        e->unk10 = lbl_eu_806677E8;
    } else if (e->unk10 > lbl_eu_80667864) {
        e->unk10 = lbl_eu_80667864;
    }
    if (e->unk0 < lbl_eu_806677E8) {
        e->unk0 = lbl_eu_806677E8;
    } else if (e->unk0 > lbl_eu_80667864) {
        e->unk0 = lbl_eu_80667864;
    }
    if (e->unk4 < lbl_eu_806677E8) {
        e->unk4 = lbl_eu_806677E8;
    } else if (e->unk4 > lbl_eu_80667864) {
        e->unk4 = lbl_eu_80667864;
    }
    if (e->unk8 < lbl_eu_806677E8) {
        e->unk8 = lbl_eu_806677E8;
    } else if (e->unk8 > lbl_eu_80667864) {
        e->unk8 = lbl_eu_80667864;
    }
    if (e->unkC < lbl_eu_806677E8) {
        e->unkC = lbl_eu_806677E8;
    } else if (e->unkC > lbl_eu_80667864) {
        e->unkC = lbl_eu_80667864;
    }
}
// us-8017f410: retail symbol is Fv; the real ABI passes (self, arg). Pushes
// arg through func_801748B8 into the 0x3358 status field, then, unless the
// 0x335A counter is already positive, runs the id-0x73 arts scan: a hit sets
// the counter to 1 (clamped), notifies func_802808AC and clears 0x3358.
// Finally forwards (actor, clamped, original) to func_802A28C4.
void CActorParam_UnkVirtualFunc153__Q22cf11CActorParamFv(cf::CActorParam* self, int arg) {
    cf::CActorParamStatusView* v = reinterpret_cast<cf::CActorParamStatusView*>(self);
    if (arg < 0) {
        void* obj = reinterpret_cast<CActorParamVt*>(self)->vf9C();
        if (obj != NULL) {
            void* obj2 = reinterpret_cast<CActorParamVt*>(self)->vf9C();
            if (func_80148778((u8*)obj2 + 8, 0xBF) != 0) {
                return;
            }
        }
    }
    s16 orig = v->field_0x335A;
    func_801748B8(&v->field_0x3358, arg);
    if (v->field_0x335A <= 0) {
        if (reinterpret_cast<CActorParamVt*>(self)->vf290() != NULL) {
            void* obj = reinterpret_cast<CActorParamVt*>(self)->vf9C();
            if (*(u32*)((u8*)obj + 0x3F00) & 2) {
                if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x73) != 0) {
                    EnumListHolder holder;
                    func_80043D90(&holder);
                    void* list = func_80043F18(&holder);
                    func_800F4A98(list, 0x20, 0);
                    for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(&holder) + 0x620); i++) {
                        void* actor = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                        if (*(u16*)((u8*)actor + 0x3F28) == 1) {
                            s16 sv = 1;
                            v->field_0x335A = (u16)sv;
                            if (sv < 0) {
                                v->field_0x335A = 0;
                            } else if (sv > 4) {
                                v->field_0x335A = 4;
                            }
                            func_802808AC(1);
                            v->field_0x3358 = 0;
                            __dt__80043E88(&holder, -1);
                            return;
                        }
                    }
                    __dt__80043E88(&holder, -1);
                }
            }
        }
    }
    s16 clamped = v->field_0x335A;
    u8* obj = (u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C();
    if (obj != NULL) {
        func_802A28C4((int)obj, clamped, orig);
    }
}
// us-8017f5f0: retail symbol is Fv; the real ABI passes (self, val).
// Raises the 0x3358 status field up to val, unless the actor (Func2) exists
// and reports an active blocker (func_80148778 at obj+8 with id 0xBF).
void CActorParam_UnkVirtualFunc154__Q22cf11CActorParamFv(cf::CActorParam* self, int val) {
    cf::CActorParamStatusView* v = reinterpret_cast<cf::CActorParamStatusView*>(self);
    if (val < (s16)v->field_0x3358) {
        void* obj = reinterpret_cast<CActorParamVt*>(self)->vf9C();
        if (obj != NULL) {
            void* obj2 = reinterpret_cast<CActorParamVt*>(self)->vf9C();
            if (func_80148778((u8*)obj2 + 8, 0xBF) != 0) {
                return;
            }
        }
    }
    v->field_0x3358 = (s16)val;
}
// us-8017f67c: retail symbol is Fv; the real ABI passes (self, arg). Sets the
// 0x335A status counter to (s16)arg clamped to [0, 4], zeroes 0x3358, then
// forwards (actor, clamped, original) to func_802A28C4. A blocker on the
// actor (func_80148778 id 0xBF) aborts the update; the id-0x73 arts scan
// (arg <= 0) cancels it for dead arts.
void CActorParam_UnkVirtualFunc156__Q22cf11CActorParamFv(cf::CActorParam* self, int arg) {
    cf::CActorParamStatusView* v = reinterpret_cast<cf::CActorParamStatusView*>(self);
    s16 orig = v->field_0x335A;
    if (arg < orig) {
        void* obj = reinterpret_cast<CActorParamVt*>(self)->vf9C();
        if (obj != NULL) {
            void* obj2 = reinterpret_cast<CActorParamVt*>(self)->vf9C();
            if (func_80148778((u8*)obj2 + 8, 0xBF) != 0) {
                return;
            }
        }
    }
    if (arg <= 0) {
        if (reinterpret_cast<CActorParamVt*>(self)->vf290() != NULL) {
            void* obj = reinterpret_cast<CActorParamVt*>(self)->vf9C();
            if (*(u32*)((u8*)obj + 0x3F00) & 2) {
                if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x73) != 0) {
                    EnumListHolder holder;
                    func_80043D90(&holder);
                    void* list = func_80043F18(&holder);
                    func_800F4A98(list, 0x20, 0);
                    for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(&holder) + 0x620); i++) {
                        void* actor = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                        if (*(u16*)((u8*)actor + 0x3F28) == 1) {
                            __dt__80043E88(&holder, -1);
                            return;
                        }
                    }
                    __dt__80043E88(&holder, -1);
                }
            }
        }
    }
    s16 s = (s16)arg;
    v->field_0x335A = (u16)arg;
    if (s < 0) {
        v->field_0x335A = 0;
    } else if (s > 4) {
        v->field_0x335A = 4;
    }
    func_802808AC(arg);
    v->field_0x3358 = 0;
    s16 clamped = v->field_0x335A;
    u8* obj = (u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C();
    if (obj != NULL) {
        func_802A28C4((int)obj, clamped, orig);
    }
}
// us-8017f858: retail symbol is Fv; the real ABI passes (self, delta). Adds
// (s16)delta to the 0x335A status counter clamped to [0, 4], zeroes 0x3358,
// then if the actor (Func2) exists forwards (actor, clamped, original) to
// func_802A28C4.
void CActorParam_UnkVirtualFunc158__Q22cf11CActorParamFv(cf::CActorParam* self, int delta) {
    cf::CActorParamStatusView* v = reinterpret_cast<cf::CActorParamStatusView*>(self);
    s16 cur = v->field_0x335A;
    s16 orig = v->field_0x335A;
    s16 sum = (s16)(cur + (s16)delta);
    v->field_0x335A = (u16)sum;
    if (sum < 0) {
        v->field_0x335A = 0;
    } else if (sum > 4) {
        v->field_0x335A = 4;
    }
    func_802808AC(delta);
    v->field_0x3358 = 0;
    s16 clamped = v->field_0x335A;
    u8* obj = (u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C();
    if (obj != NULL) {
        func_802A28C4((int)obj, clamped, orig);
    }
}
// us-8017f8fc: retail symbol is Fv; the real ABI passes (self, arg).
// Subtracts (s16)arg from the 0x335A counter, clamps to [0, 4], then
// recomputes 0x3358 from the 0x335C status table row indexed by the clamped
// counter scaled by the 0x3368 rate. The id-0x73 arts scan (counter <= 0)
// sets the counter to 1, notifies func_802808AC and clears 0x3358. Finally
// forwards (actor, clamped, original) to func_802A28C4.
void CActorParam_UnkVirtualFunc159__Q22cf11CActorParamFv(cf::CActorParam* self, int arg) {
    cf::CActorParamStatusView* v = reinterpret_cast<cf::CActorParamStatusView*>(self);
    void* obj = reinterpret_cast<CActorParamVt*>(self)->vf9C();
    if (obj != NULL) {
        void* obj2 = reinterpret_cast<CActorParamVt*>(self)->vf9C();
        if (func_80148778((u8*)obj2 + 8, 0xBF) != 0) {
            return;
        }
    }
    s16 cur = v->field_0x335A;
    s16 orig = v->field_0x335A;
    int raw = cur - (s16)arg;
    s16 sum = (s16)raw;
    v->field_0x335A = (u16)raw;
    if (sum < 0) {
        v->field_0x335A = 0;
    } else if (sum > 4) {
        v->field_0x335A = 4;
    }
    func_802808AC(arg);
    s16 idx = v->field_0x335A;
    v->field_0x3358 = (u16)(int)(v->field_0x3368 * (float)self->unk335C[idx]);
    if (v->field_0x335A <= 0) {
        if (reinterpret_cast<CActorParamVt*>(self)->vf290() != NULL) {
            void* o = reinterpret_cast<CActorParamVt*>(self)->vf9C();
            if (*(u32*)((u8*)o + 0x3F00) & 2) {
                if (func_8026178C(reinterpret_cast<CActorParamVt*>(self)->vf290(), 0x73) != 0) {
                    EnumListHolder holder;
                    func_80043D90(&holder);
                    void* list = func_80043F18(&holder);
                    func_800F4A98(list, 0x20, 0);
                    for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(&holder) + 0x620); i++) {
                        void* actor = func_8016FE34(func_800F6EAC(func_80043F18(&holder), i));
                        if (*(u16*)((u8*)actor + 0x3F28) == 1) {
                            s16 sv = 1;
                            v->field_0x335A = (u16)sv;
                            if (sv < 0) {
                                v->field_0x335A = 0;
                            } else if (sv > 4) {
                                v->field_0x335A = 4;
                            }
                            func_802808AC(1);
                            v->field_0x3358 = 0;
                            __dt__80043E88(&holder, -1);
                            return;
                        }
                    }
                    __dt__80043E88(&holder, -1);
                }
            }
        }
    }
    s16 clamped = v->field_0x335A;
    u8* o = (u8*)reinterpret_cast<CActorParamVt*>(self)->vf9C();
    if (o != NULL) {
        func_802A28C4((int)o, clamped, orig);
    }
}
// Returns (vtable slot 0x1F0) - (vtable slot 0x1E8): CActorParam_UnkVirtualFunc87
// minus CActorParam_UnkVirtualFunc85 (per the header's vtable offset map).
int cf::CActorParam::CActorParam_UnkVirtualFunc86() {
    return reinterpret_cast<CActorParamVt*>(this)->vf1F0() - reinterpret_cast<CActorParamVt*>(this)->vf1E8();
}
// us-8017fbf0: arts-table row lookup for the current state counter
// (Func26) + 1; returns 1 when the row index exceeds the 0x63-row table.
// The bdat pointer is read before the virtual call (retail keeps it in r31
// across the call).
u32 cf::CActorParam::CActorParam_UnkVirtualFunc87() {
    void* bdat = lbl_eu_806640DC;
    u32 row = CActorParam_UnkVirtualFunc26() + 1;
    if (row > 0x63) {
        return 1;
    }
    return getBdatStringColumnValue(bdat, &lbl_eu_80503438[1], row);
}
// us-8017fc50: fetch the CActorParam_UnkStruct1 from vtable slot 0x2A4
// (Func132) and (re)initialize it exactly like CActorParam_UnkVirtualFunc136
// does for each 0x2A84 entry (gauge floats stamped with the sdata2 default
// instead of 0), then zero unk3354 and re-run the full init loop (slot 0x2B4).
void cf::CActorParam::CActorParam_UnkVirtualFunc135() {
    cf::CActorParam_UnkStruct1* s = reinterpret_cast<cf::CActorParam_UnkStruct1*>(reinterpret_cast<CActorParamVt*>(this)->_v2A4());
    s->unk0 = 0;
    s->unk4 = 0;
    s->unk48 = 0;
    s->unk4C = -1;
    s->unk50 = 0;
    s->unk54 = lbl_eu_806677E4;
    s->unk58 = lbl_eu_806677E4;
    s->unk5C = lbl_eu_806677E4;
    s->unk60 = lbl_eu_806677E4;
    s->unk64 = lbl_eu_806677E4;
    s->unk7C = 0;
    s->unk80 = 0;
    s->unkB8 = 0;
    s->unk68 = lbl_eu_806677E4;
    s->unk6C = lbl_eu_806677E4;
    s->unk70 = 0;
    s->unk72 = 0;
    std::memset(s->unk8, 0, sizeof(s->unk8));
    std::memset(s->unk84, 0, sizeof(s->unk84));
    s->mFlagsArray[0].flags = 0;
    s->mFlagsArray[1].flags = 0;
    reinterpret_cast<cf::CActorParam3354View*>(this)->field_0x3354 = 0;
    reinterpret_cast<CActorParamVt*>(this)->_v2B4();
}
// us-8017fd2c: (re)initialize the 10 CActorParam_UnkStruct1 entries at
// 0x2A84: zero the int/flag fields, -1 the unk4C id, and stamp the float
// gauge fields (unk54..unk6C) with the sdata2 default instead of 0.
void cf::CActorParam::CActorParam_UnkVirtualFunc136() {
    for (int i = 0; i < 10; i++) {
        unk2A84[i].unk0 = 0;
        std::memset(unk2A84[i].unk8, 0, sizeof(unk2A84[i].unk8));
        unk2A84[i].unk4 = 0;
        unk2A84[i].unk48 = 0;
        unk2A84[i].unk4C = -1;
        unk2A84[i].unk50 = 0;
        unk2A84[i].unk54 = lbl_eu_806677E4;
        unk2A84[i].unk58 = lbl_eu_806677E4;
        unk2A84[i].unk5C = lbl_eu_806677E4;
        unk2A84[i].unk60 = lbl_eu_806677E4;
        unk2A84[i].unk64 = lbl_eu_806677E4;
        unk2A84[i].unk7C = 0;
        unk2A84[i].unk80 = 0;
        unk2A84[i].unkB8 = 0;
        unk2A84[i].unk68 = lbl_eu_806677E4;
        unk2A84[i].unk6C = lbl_eu_806677E4;
        unk2A84[i].unk70 = 0;
        unk2A84[i].unk72 = 0;
        std::memset(unk2A84[i].unk84, 0, sizeof(unk2A84[i].unk84));
        unk2A84[i].mFlagsArray[0].flags = 0;
        unk2A84[i].mFlagsArray[1].flags = 0;
    }
}
// us-8017fe00: push a new CActorParam_UnkStruct1 slot onto the unk2A84
// stack. A fresh slot (unk4 == 0) gets a full inline copy of the previous
// entry plus flag-word pruning; an in-use slot only inherits unk4, the unk8
// name block, the 0x70/0x72 ids and (unless the 0x10 flag is set) the
// 0x68/0x6C floats plus a merged flag word.
void CActorParam_UnkVirtualFunc137__Q22cf11CActorParamFv(cf::CActorParam* self) {
    cf::CActorParam2A84View* arr = reinterpret_cast<cf::CActorParam2A84View*>(self);
    cf::CActorParam3354View* st = reinterpret_cast<cf::CActorParam3354View*>(self);
    u8 idx = st->field_0x3354;
    if (idx >= 10) return;
    u8 n = (u8)(idx + 1);
    st->field_0x3354 = n;
    if (arr->entries[n].unk4 == 0) {
        arr->entries[n] = arr->entries[n - 1];
        // rlwinm wrap-masks clear individual flag bits (retail 28,26 / 5,3 / 26,24).
        arr->entries[st->field_0x3354].mFlagsArray[1].flags &= ~cf::CActorParam_UnkStruct1::FLAG_BIT_4;
        arr->entries[st->field_0x3354].mFlagsArray[1].flags &= ~cf::CActorParam_UnkStruct1::FLAG_BIT_27;
        arr->entries[st->field_0x3354].mFlagsArray[1].flags &= ~cf::CActorParam_UnkStruct1::FLAG_BIT_6;
        arr->entries[st->field_0x3354].mFlagsArray[0].flags &= 0xC0000000;
    } else {
        arr->entries[n].unk4 = arr->entries[n - 1].unk4;
        std::memcpy(arr->entries[st->field_0x3354].unk8, arr->entries[st->field_0x3354 - 1].unk8, 0x40);
        arr->entries[st->field_0x3354].unk70 = arr->entries[st->field_0x3354 - 1].unk70;
        arr->entries[st->field_0x3354].unk72 = arr->entries[st->field_0x3354 - 1].unk72;
        if (!(arr->entries[st->field_0x3354].mFlagsArray[1].flags & 0x10)) {
            arr->entries[st->field_0x3354].unk68 = arr->entries[st->field_0x3354 - 1].unk68;
            arr->entries[st->field_0x3354].unk6C = arr->entries[st->field_0x3354 - 1].unk6C;
            cf::CActorParam_UnkStruct1* d = &arr->entries[st->field_0x3354];
            // The non-contiguous 0x4000F000 mask splits into rlwinm+rlwimi;
            // a separate (f&X)|(f&Y)|X OR-chain would fold the redundant bit.
            u32 bits = (arr->entries[st->field_0x3354 - 1].mFlagsArray[1].flags & (0x40000000 | 0x0000F000)) | 0x40000000;
            if (bits & 0x80000000) {
                d->mFlagsArray[0].flags |= bits;
            } else {
                d->mFlagsArray[1].flags |= bits;
            }
        }
    }
}

// BattleState thunks: adjust `this` by -8 and tail-call CActorParam methods.
typedef void (*CActorFn)(void*);

void CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv(cf::CActorParam* self, cf::CActorParam180Arg* arg);
extern "C" void CBattleState_UnkVirtualFunc18__Q22cf11CActorParamFv(void* self) {
    ((CActorFn)CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv)((void*)((char*)self - 8));
}

// us-80180170
void CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv(cf::CActorParam* self, cf::CActorParam179Arg* arg);
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
