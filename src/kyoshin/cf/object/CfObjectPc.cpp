// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectPc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "kyoshin/CArtsInfo.hpp"   // declares extern "C" func_8009EC9C(u32)
#include "kyoshin/cf/CfMapMineManager.hpp"  // func_80061FFC + getBdatStringColumnValue

struct PCIf {
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
    virtual u32 _v00FC();
    virtual void _v0100();
    virtual void _v0104();
    virtual void _v0108();
    virtual void _v010C();
    virtual void _v0110();
    virtual void _v0114();
    virtual void _v0118();
    virtual void _v011C();
    virtual void _v0120();
    virtual void _v0124();
    virtual void _v0128();
    virtual void _v012C();
    virtual void _v0130();
    virtual void _v0134();
    virtual void _v0138();
    virtual void _v013C();
    virtual void _v0140();
    virtual void _v0144();
    virtual void _v0148();
    virtual void _v014C();
    virtual void _v0150();
    virtual void _v0154();
    virtual void _v0158();
    virtual void _v015C();
    virtual void _v0160();
    virtual void _v0164();
    virtual void _v0168();
    virtual void _v016C();
    virtual void _v0170();
    virtual void _v0174();
    virtual void _v0178();
    virtual void _v017C();
    virtual void _v0180();
    virtual void _v0184();
    virtual void _v0188();
    virtual void _v018C();
    virtual void _v0190();
    virtual void _v0194();
    virtual void _v0198();
    virtual void _v019C();
    virtual void _v01A0();
    virtual void _v01A4();
    virtual void _v01A8();
    virtual void _v01AC();
    virtual void _v01B0();
    virtual void _v01B4();
    virtual void _v01B8();
    virtual void _v01BC();
    virtual void _v01C0();
    virtual void _v01C4();
    virtual void _v01C8();
    virtual void _v01CC();
    virtual void _v01D0();
    virtual void _v01D4();
    virtual void _v01D8();
    virtual void _v01DC();
    virtual void _v01E0();
    virtual void _v01E4();
    virtual u32 _v01E8();
    virtual void _v01EC();
    virtual void _v01F0();
    virtual void _v01F4();
    virtual void _v01F8();
    virtual void _v01FC();
    virtual void _v0200();
    virtual void _v0204();
    virtual void _v0208();
    virtual void _v020C();
    virtual void _v0210();
    virtual void _v0214();
    virtual void _v0218();
    virtual void _v021C();
    virtual void _v0220();
    virtual void _v0224();
    virtual void _v0228();
    virtual void _v022C();
    virtual void _v0230();
    virtual void _v0234();
    virtual void _v0238();
    virtual void _v023C();
    virtual void _v0240();
    virtual void _v0244();
    virtual void _v0248();
    virtual void _v024C();
    virtual void _v0250();
    virtual void _v0254();
    virtual void _v0258();
    virtual void _v025C();
    virtual void _v0260();
    virtual void _v0264();
    virtual void _v0268();
    virtual void _v026C();
    virtual void _v0270();
    virtual void _v0274();
    virtual void _v0278();
    virtual void _v027C();
    virtual void _v0280();
    virtual void _v0284();
    virtual void _v0288();
    virtual u32 _v028C();
    virtual void _v0290();
    virtual void _v0294();
    virtual void _v0298();
    virtual void _v029C();
    virtual void _v02A0();
    virtual void _v02A4();
    virtual void _v02A8();
    virtual void _v02AC();
    virtual void _v02B0();
    virtual void _v02B4();
    virtual void _v02B8();
    virtual void _v02BC();
    virtual void _v02C0();
    virtual void _v02C4();
    virtual void _v02C8();
    virtual void _v02CC();
    virtual void _v02D0();
    virtual void _v02D4();
    virtual void _v02D8();
    virtual void _v02DC();
    virtual void _v02E0();
    virtual void _v02E4();
    virtual void _v02E8();
    virtual void _v02EC();
    virtual void _v02F0();
    virtual void _v02F4();
    virtual void _v02F8();
    virtual void _v02FC();
    virtual void _v0300();
    virtual void _v0304();
    virtual void _v0308();
    virtual void _v030C();
    virtual void _v0310();
    virtual void _v0314();
    virtual void _v0318();
    virtual void _v031C();
    virtual void _v0320();
    virtual void _v0324();
    virtual void _v0328();
    virtual void _v032C();
    virtual void _v0330();
    virtual void _v0334();
    virtual void _v0338();
    virtual void _v033C();
    virtual void _v0340();
    virtual void _v0344();
    virtual void _v0348();
    virtual void _v034C();
    virtual void _v0350();
    virtual void _v0354();
    virtual void _v0358();
    virtual void _v035C();
    virtual void _v0360();
    virtual void _v0364();
    virtual void _v0368();
    virtual void _v036C();
    virtual void _v0370();
    virtual void _v0374();
    virtual void _v0378();
    virtual void _v037C();
    virtual void _v0380();
    virtual void _v0384();
    virtual void _v0388();
    virtual void _v038C();
    virtual void _v0390();
    virtual void _v0394();
    virtual void _v0398();
    virtual void _v039C();
    virtual void _v03A0();
    virtual void _v03A4();
    virtual void _v03A8();
    virtual void _v03AC();
    virtual void _v03B0();
    virtual void _v03B4();
    virtual void _v03B8();
    virtual void _v03BC();
    virtual void _v03C0();
    virtual void _v03C4();
    virtual void _v03C8();
    virtual void _v03CC();
    virtual void _v03D0();
    virtual void _v03D4();
    virtual void _v03D8();
    virtual void _v03DC();
    virtual void _v03E0();
    virtual void _v03E4();
    virtual void _v03E8();
    virtual void _v03EC();
    virtual void _v03F0();
    virtual void _v03F4();
    virtual void _v03F8();
    virtual void _v03FC();
    virtual void _v0400();
    virtual void _v0404();
    virtual void _v0408();
    virtual void _v040C();
    virtual void _v0410();
    virtual void _v0414();
    virtual void _v0418();
    virtual void _v041C();
    virtual void _v0420();
    virtual void _v0424();
    virtual void _v0428();
    virtual void _v042C();
    virtual void _v0430();
    virtual void _v0434();
    virtual void _v0438();
    virtual void _v043C();
    virtual void _v0440();
    virtual void _v0444();
    virtual void _v0448();
    virtual void _v044C();
    virtual void _v0450();
    virtual void _v0454();
    virtual void _v0458();
    virtual void _v045C();
    virtual void _v0460();
    virtual void _v0464();
    virtual void _v0468();
    virtual void _v046C();
    virtual void _v0470();
    virtual void _v0474();
    virtual void _v0478();
    virtual void _v047C();
    virtual void _v0480();
    virtual void _v0484();
    virtual void _v0488();
    virtual void _v048C();
    virtual void _v0490();
    virtual void _v0494();
    virtual void _v0498();
    virtual void _v049C();
    virtual void _v04A0();
    virtual void _v04A4();
    virtual void _v04A8();
    virtual void _v04AC();
    virtual void _v04B0();
    virtual void _v04B4();
    virtual void _v04B8();
    virtual void _v04BC();
    virtual void _v04C0();
    virtual void _v04C4();
    virtual void _v04C8();
    virtual void _v04CC();
    virtual void _v04D0();
    virtual void _v04D4();
    virtual void _v04D8();
    virtual void _v04DC();
    virtual void _v04E0();
    virtual void _v04E4();
    virtual void _v04E8();
    virtual void _v04EC();
    virtual void _v04F0();
    virtual void _v04F4();
    virtual void _v04F8();
    virtual void _v04FC();
    virtual void _v0500();
    virtual void _v0504();
    virtual void _v0508();
    virtual void _v050C();
    virtual void _v0510();
    virtual void _v0514();
    virtual void _v0518();
    virtual void _v051C();
    virtual void _v0520();
    virtual void _v0524();
    virtual void _v0528();
    virtual void _v052C();
    virtual void _v0530();
    virtual void _v0534();
    virtual void _v0538();
    virtual void _v053C();
    virtual void _v0540();
    virtual void _v0544();
    virtual void _v0548();
    virtual void _v054C();
    virtual void _v0550();
    virtual void _v0554();
    virtual void _v0558();
    virtual void _v055C();
    virtual void _v0560();
    virtual void _v0564();
    virtual void _v0568();
    virtual void _v056C();
    virtual void _v0570();
    virtual void _v0574();
    virtual void _v0578();
    virtual void _v057C();
    virtual void _v0580();
    virtual void _v0584();
    virtual void _v0588();
    virtual void _v058C();
    virtual void _v0590();
    virtual void _v0594();
    virtual void _v0598();
    virtual void _v059C();
    virtual void _v05A0();
    virtual void _v05A4();
    virtual void _v05A8();
    virtual void _v05AC();
    virtual void _v05B0();
    virtual void _v05B4();
    virtual void _v05B8();
    virtual void _v05BC();
    virtual void _v05C0();
    virtual void _v05C4();
    virtual void _v05C8();
    virtual void _v05CC();
    virtual void _v05D0();
    virtual void _v05D4();
    virtual void _v05D8();
    virtual void _v05DC();
    virtual void _v05E0();
    virtual void _v05E4();
    virtual void _v05E8();
    virtual void _v05EC();
    virtual void _v05F0();
    virtual void vf05F4();
    virtual void vf05F8();
    virtual void vf05FC();
};

namespace cf {
// Minimal view of CfObjectPoint for func_800C1244: only the vtable layout
// matters (retail dispatches slot 0x70 = CfObject_UnkVirtualFunc8). With RTTI
// on, declared virtual N sits at vtable offset (N+2)*4, so 26 padding slots
// put the callee at 0x70. The class is never instantiated in this TU, so the
// padding virtuals need no definitions and no vtable is emitted.
class CfObjectPoint {
public:
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
    virtual void CfObject_UnkVirtualFunc8();  // vtable offset 0x70
    int func_800C1244();
};
}

cf::CfObjectPc::CfObjectPc() : CfObjectActor() {
    // novtable class: write the four retail sub-vtable pointers manually
    // (base labels 0x0 / +0xC / +0x36C / +0x37C at offsets 0x0 / 0x8 /
    // 0x3380 / 0x3E9C).
    *(void**)this = (void*)lbl_eu_80529DA0;
    *(void**)((u8*)this + 0x8) = (void*)(lbl_eu_80529DA0 + 0xC);
    *(void**)((u8*)this + 0x3380) = (void*)(lbl_eu_80529DA0 + 0x36C);
    *(void**)((u8*)this + 0x3E9C) = (void*)(lbl_eu_80529DA0 + 0x37C);

    // Allocate the 0x44-byte CfResPcImpl resource object and construct it in
    // place, passing the player sub-object (this + 0x3E9C, null-checked). The
    // ctor returns the object in r3, so no extra callee-saved register is
    // needed for the final store.
    u8* res = (u8*)allocate__Q23mtl10MemManagerFUlUl(0x44, func_80061FFC());
    if (res != NULL) {
        // Player sub-object (this + 0x3E9C), null-checked like a secondary-
        // base conversion; the declared base sizes are 8 bytes off retail.
        u8* parent = (u8*)this;
        if (this != NULL) {
            parent += 0x3E9C;
        }
        res = (u8*)__ct__cf_CfResPcImpl(res, (cf::CfObjectMove*)parent);
    }
    ((CfObjectPcSubFields*)this)->mPtr3F4C = res;
    reinterpret_cast<PCIf*>(this)->_v05D8();
}

cf::CfObjectPc::~CfObjectPc() {}

void func_800BFDE0(){}

void cf::CfObjectPc::func_800BFF20() {}

void cf::CfObjectPc::func_800BFFEC() {
    reinterpret_cast<PCIf*>(this)->_v00B0();
    reinterpret_cast<PCIf*>(this)->_v00B8();
    reinterpret_cast<PCIf*>(this)->_v0314();
    // Look up the arts data object for this PC's index, then write the
    // per-entry value at +0x17C from the CActorParam virtual 0x28C result.
    CfObjectPcArtsData* artsData = (CfObjectPcArtsData*)func_8009EC9C(
        ((CfObjectPcSubFields*)this)->field_0x3F28);
    func_80175A50(&artsData->field_0x17C,
        reinterpret_cast<PCIf*>(this)->_v028C());
}

// NOTE: func_800C0080 is defined as the extern "C" wrapper below (not as a
// member): the adjuster thunks reference the global identifier, and the
// wrapper also satisfies the vtable slot for the virtual member.
extern "C" void func_800C0080__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    ((CfObjectPcSubFields*)self)->field_0x4568 = 1;
    func_80174B4C((u8*)self, 0x00100000);
    ((CfObjectPcSubFields*)self)->field_0x45C0 = lbl_eu_80666B14;
}

void cf::CfObjectPc::func_800C00C0() {}

extern "C" void func_800C00C0__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    static_cast<cf::CfObjectPc*>(self)->func_800C00C0();
}

extern "C" void func_800BFF20__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    static_cast<cf::CfObjectPc*>(self)->func_800BFF20();
}

extern "C" void func_800C0524__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    static_cast<cf::CfObjectPc*>(self)->func_800C0524();
}

// Vtable slot 16 override (CfObjectMove vtable): the adjuster thunk passes the
// CfObjectMove sub-object, and the vtable caller supplies a second int in r4
// even though the retail mangled name is arg-less. Small args delegate to the
// base CfObjectMove implementation; larger ones forward a sub-object pointer
// to func_800CA42C.
void func_800C11CC__Q22cf10CfObjectPcFv(cf::CfObjectPc* self, int arg) {
    if (arg < 0x2c) {
        // CfObjectMove sub-object lives at this+0x3E9C (manual offset: the
        // declared base-class sizes in the headers do not match the retail
        // layout). Direct (non-virtual) call to the base implementation.
        u8* subObj = reinterpret_cast<u8*>(self) + 0x3e9c;
        ((cf::CfObjectMove*)subObj)->CfObjectMove::CfObjectMove_UnkVirtualFunc16();
    } else {
        u8* obj = ((CfObjectPcSubFields*)self)->mPtr3ED4;
        if (obj != NULL) {
            func_800CA42C(obj);
        }
    }
}

extern "C" void __dt__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    static_cast<cf::CfObjectPc*>(self)->~CfObjectPc();
}

// Scans the first six arts-data entries (indices 0..5) for one whose u16
// flag at +0x1A is non-zero ("occupied"); stops at the first hit.
void cf::CfObjectPc::func_800C0174() {
    CfObjectPcArtsData* data = (CfObjectPcArtsData*)func_8009EC9C(
        ((CfObjectPcSubFields*)this)->field_0x3F28);
    for (int i = 0; i <= 5; i++) {
        if (func_8009D7E4(&data->mEntries, i)->field_0x1A != 0) break;
    }
}

void func_800C01D4(){}

// Looks up the data object for the index at +0x3F28, then runs its ctor.
void CActorParam_UnkVirtualFunc166__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    __ct__8009F8B8((u8*)func_8009EC9C(((CfObjectPcSubFields*)self)->field_0x3F28));
}

void CActorParam_UnkVirtualFunc167__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    func_800A03F4((u8*)func_8009EC9C(((CfObjectPcSubFields*)self)->field_0x3F28));
}

void func_800C02C4__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    func_800A145C((u8*)func_8009EC9C(((CfObjectPcSubFields*)self)->field_0x3F28));
}

// Adjuster-style override (retail mangled name is arg-less Fv but the vtable
// caller supplies the parameter in r4): applies the arts data entry identified
// by the index at +0x3F28, passing the extra value through.
void func_800C02EC__Q22cf10CfObjectPcFv(cf::CfObjectPc* self, int param) {
    if (param == 0) return;
    func_800A18A4((u8*)func_8009EC9C(((CfObjectPcSubFields*)self)->field_0x3F28), param);
}

void cf::CfObjectPc::func_800C032C() {
    // getPlayer(0) returns the player's CfObjectMove sub-object; compare it
    // against this object's CfObjectMove base (null-checked pointer adjust)
    // and dispatch to the matching per-frame handler. Dispatches go through
    // PCIf (fresh vtable class, retail slot offsets) because the shared base
    // headers declare four extra virtuals.
    u8* target = (u8*)this;
    if (this != NULL) {
        target += 0x3E9C;
    }
    if (target == (u8*)cf::CfGameManager::getPlayer(0)) {
        reinterpret_cast<PCIf*>(this)->vf05F8();
    } else {
        reinterpret_cast<PCIf*>(this)->vf05FC();
    }
}

void cf::CfObjectPc::func_800C03A8() {}

void cf::CfObjectPc::func_800C0474() {
    // The three uses of this+0x3380 are written in different syntaxes so MWCC
    // does not CSE the address into a callee-saved register (retail recomputes
    // addi r3, r31, 0x3380 at each call site).
    func_8014B7B0((u8*)this + 0x3380);
    if (((CfObjectPcSubFields*)this)->field_0x3F28 == 4) {
        // Arts-count bdat row: enable the art column when the string value's
        // first byte is '1' (the value is returned packed; read its low byte).
        CfObjectPcArtsData* artsData = (CfObjectPcArtsData*)func_8009EC9C(
            ((CfObjectPcSubFields*)this)->field_0x3F28);
        u32 val = getBdatStringColumnValue(lbl_eu_806640F4,
            (const char*)lbl_eu_804FC5EC + 0x24, artsData->field_0xC);
        if (*(s8*)&val == 1) {
            func_8015396C(&((u8*)this)[0x3380], 0x70, 0xd);
            return;
        }
    }
    func_8015396C((u8*)((u32)this + 0x3380), 0, 0);
}



void cf::CfObjectPc::func_800C0524() {}

void cf::CfObjectPc::CActorParam_UnkVirtualFunc4() {}

void CActorParam_UnkVirtualFunc173__Q22cf10CfObjectPcFv(void) {}

// CActorParam virtual 0x354 override. The retail mangled name is arg-less
// (Fv) but the dispatcher passes a float in f1; forward it to the base
// implementation (same naming quirk) after touching the battle singleton.
void CActorParam_UnkVirtualFunc176__Q22cf10CfObjectPcFv(cf::CfObjectPc* self, float value) {
    getInstance__Q22cf14CBattleManagerFv();
    CActorParam_UnkVirtualFunc176__Q22cf11CActorParamFv(self, value);
}

// Retail symbol CActorParam_UnkVirtualFunc86__Q22cf10CfObjectPcFv (vtable slot
// 0x1EC override). Written as a global with the mangled name: the base
// CActorParam override returns u32 in retail but the base header declares it
// void, so a real member override would fail to compile.
u32 CActorParam_UnkVirtualFunc86__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) {
    // Arts count from the param object (vtable 0xFC); the arts table index is
    // count+1, capped at 99 entries. Returns 1 when over the cap.
    void* bdat = lbl_eu_806640DC;
    u32 idx = reinterpret_cast<PCIf*>(self)->_v00FC() + 1;
    if (idx > 0x63) return 1;
    u32 sval = getBdatStringColumnValue(bdat,
        (const char*)lbl_eu_804FC5EC + 0x34, idx);
    return sval - reinterpret_cast<PCIf*>(self)->_v01E8();
}

void cf::CfObjectPc::CActorParam_UnkVirtualFunc88() {}

void cf::CfObjectPc::CActorParam_UnkVirtualFunc178() {}

void func_800C0DD4(){}

void CObjectParam_UnkVirtualFunc4__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))func_800C00C0__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }

void CfObject_UnkVirtualFunc3__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))func_800C0080__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }

void CfObject_UnkVirtualFunc2__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))func_800BFF20__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }

void CfObjectMove_UnkVirtualFunc16__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))func_800C11CC__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }

// Global-scope name for the func_800BFFEC symbol: the member definition below
// mangles to the same identifier, and the adjuster thunks reference it as a
// plain global (C linkage keeps the reloc name exact).
extern "C" void func_800BFFEC__Q22cf10CfObjectPcFv(cf::CfObjectPc* self);

void CfObject_UnkVirtualFunc6__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))func_800BFFEC__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }

void CfObject_UnkVirtualFunc4__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))func_800C0524__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }

void func_800C1220__Q22cf10CfObjectPcFv(void* self) { ((void(*)(void*))__dt__Q22cf10CfObjectPcFv)((char*)self - 0x3e9c); }

// Returns the object only when its flags word at +0x64 has bit 0x4000 set
// (the "valid" mark); otherwise returns NULL. The C++ name mangles to the
// retail symbol func_800C1228__FPv.
UNKTYPE* func_800C1228(UNKTYPE* r3) {
    if (r3 != NULL) {
        if (((FlagsObj800C1228*)r3)->field_0x64 & 0x4000) {
            return r3;
        }
    }
    return NULL;
}

int cf::CfObjectPoint::func_800C1244() {
    CfObject_UnkVirtualFunc8();
    return 1;
}

extern "C" void func_800C0504__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) { reinterpret_cast<PCIf*>(self)->vf05F4(); }

extern "C" void func_800C0514__Q22cf10CfObjectPcFv(cf::CfObjectPc* self) { reinterpret_cast<PCIf*>(self)->vf05F4(); }
