#pragma once

#include <types.h>

namespace cf {

class CfObject; // forward decl (effect object array elements)

// Scene object referenced by CfMapEffectManager::field_0x134 (released via
// gmFileObject once lookupWorkAtAddr validates it).
struct CfMapEffectHandle {
    u8 field_0x0[0xB0];
    u32 field_0xB0;
};

// Effect-object view exposing the release flag at 0xB0 used by
// MapFx_ResetSlots / MapFx_ReleaseObject (the concrete effect subclasses are opaque
// here).
struct CfMapEffectObject {
    u8 field_0x0[0xB0];
    u32 field_0xB0; // 0xB0
};

// Map-effect manager (RTTI "cf::CfMapEffectManager", no bases). The game
// manager owns the single instance (CfGameManager +0xA0); the static
// accessors work on sInstance. Vtable: ~CfMapEffectManager, releaseObject.
class CfMapEffectManager {
public:
    CfMapEffectManager();
    virtual ~CfMapEffectManager();
    virtual void releaseObject(CfObject* target);

    void releaseHandle();
    void resetSlots();
    int checkRowRange(int index);
    void forwardReload();
    void updateSceneFx();
    void updateRow(int index, u8* bdat);
    void reloadFromBdat();
    int matchKind(int b, int c, int d);
    void setSlotFlag(int flag);
    static void setPointEnabled(u8 enable);
    static void pushFlash(float first, float second);
    static void setGlobalFloat(float val);

    static CfMapEffectManager* sInstance;

    s16 field_0x4;                    // 0x04
    s16 field_0x6;                    // 0x06
    s16 field_0x8;                    // 0x08
    s16 field_0xA;                    // 0x0A
    CfObject* field_0x0C[0x24];       // 0x0C effect objects (36 entries)
    u16 field_0x9C[0x24];             // 0x9C per-slot u16 counter bank 1
    u16 field_0xE4[0x24];             // 0xE4 per-slot u16 counter bank 2
    s16 field_0x12C;                  // 0x12C
    s16 field_0x12E;                  // 0x12E
    s16 field_0x130;                  // 0x130
    CfMapEffectHandle* field_0x134;   // 0x134 scene object
    u8 field_0x138;                   // 0x138 active flag
};

// u16/u32 -> float conversion helper: the retail code builds the
// 0x43300000-prefixed bit pattern in memory and subtracts the shared sdata2
// magic double (CfTFile.cpp convention) so the pool reloc matches the retail
// constant instead of an MWCC-synthesised @N entry.
union F64Conv {
    u32 w[2];
    double d;
};

class CfObjectEff; // concrete effect object (defined in CfObjectEff.hpp)

// Owner-named view of the object returned by
// cf::CfGameManager::getGameSubManager (UnkClass_80083298, owned by the
// CfGameManager TU): retail dispatches vtable slot 0x194 for the
// effect-enable flag consumed by setChild5CFl_. Foreign object, so the slot
// lives on this tiny owner-named view, never on a caller-named pad. 99
class __declspec(novtable) GameSubManagerCamState {
public:
    virtual void _q008(); // +0x008
    virtual void _q00C(); // +0x00C
    virtual void _q010(); // +0x010
    virtual void _q014(); // +0x014
    virtual void _q018(); // +0x018
    virtual void _q01C(); // +0x01C
    virtual void _q020(); // +0x020
    virtual void _q024(); // +0x024
    virtual void _q028(); // +0x028
    virtual void _q02C(); // +0x02C
    virtual void _q030(); // +0x030
    virtual void _q034(); // +0x034
    virtual void _q038(); // +0x038
    virtual void _q03C(); // +0x03C
    virtual void _q040(); // +0x040
    virtual void _q044(); // +0x044
    virtual void _q048(); // +0x048
    virtual void _q04C(); // +0x04C
    virtual void _q050(); // +0x050
    virtual void _q054(); // +0x054
    virtual void _q058(); // +0x058
    virtual void _q05C(); // +0x05C
    virtual void _q060(); // +0x060
    virtual void _q064(); // +0x064
    virtual void _q068(); // +0x068
    virtual void _q06C(); // +0x06C
    virtual void _q070(); // +0x070
    virtual void _q074(); // +0x074
    virtual void _q078(); // +0x078
    virtual void _q07C(); // +0x07C
    virtual void _q080(); // +0x080
    virtual void _q084(); // +0x084
    virtual void _q088(); // +0x088
    virtual void _q08C(); // +0x08C
    virtual void _q090(); // +0x090
    virtual void _q094(); // +0x094
    virtual void _q098(); // +0x098
    virtual void _q09C(); // +0x09C
    virtual void _q0A0(); // +0x0A0
    virtual void _q0A4(); // +0x0A4
    virtual void _q0A8(); // +0x0A8
    virtual void _q0AC(); // +0x0AC
    virtual void _q0B0(); // +0x0B0
    virtual void _q0B4(); // +0x0B4
    virtual void _q0B8(); // +0x0B8
    virtual void _q0BC(); // +0x0BC
    virtual void _q0C0(); // +0x0C0
    virtual void _q0C4(); // +0x0C4
    virtual void _q0C8(); // +0x0C8
    virtual void _q0CC(); // +0x0CC
    virtual void _q0D0(); // +0x0D0
    virtual void _q0D4(); // +0x0D4
    virtual void _q0D8(); // +0x0D8
    virtual void _q0DC(); // +0x0DC
    virtual void _q0E0(); // +0x0E0
    virtual void _q0E4(); // +0x0E4
    virtual void _q0E8(); // +0x0E8
    virtual void _q0EC(); // +0x0EC
    virtual void _q0F0(); // +0x0F0
    virtual void _q0F4(); // +0x0F4
    virtual void _q0F8(); // +0x0F8
    virtual void _q0FC(); // +0x0FC
    virtual void _q100(); // +0x100
    virtual void _q104(); // +0x104
    virtual void _q108(); // +0x108
    virtual void _q10C(); // +0x10C
    virtual void _q110(); // +0x110
    virtual void _q114(); // +0x114
    virtual void _q118(); // +0x118
    virtual void _q11C(); // +0x11C
    virtual void _q120(); // +0x120
    virtual void _q124(); // +0x124
    virtual void _q128(); // +0x128
    virtual void _q12C(); // +0x12C
    virtual void _q130(); // +0x130
    virtual void _q134(); // +0x134
    virtual void _q138(); // +0x138
    virtual void _q13C(); // +0x13C
    virtual void _q140(); // +0x140
    virtual void _q144(); // +0x144
    virtual void _q148(); // +0x148
    virtual void _q14C(); // +0x14C
    virtual void _q150(); // +0x150
    virtual void _q154(); // +0x154
    virtual void _q158(); // +0x158
    virtual void _q15C(); // +0x15C
    virtual void _q160(); // +0x160
    virtual void _q164(); // +0x164
    virtual void _q168(); // +0x168
    virtual void _q16C(); // +0x16C
    virtual void _q170(); // +0x170
    virtual void _q174(); // +0x174
    virtual void _q178(); // +0x178
    virtual void _q17C(); // +0x17C
    virtual void _q180(); // +0x180
    virtual void _q184(); // +0x184
    virtual void _q188(); // +0x188
    virtual void _q18C(); // +0x18C
    virtual void _q190(); // +0x190
    virtual int getEffectEnableFlag(); // +0x194 effect-enable flag
};

} // namespace cf
