#pragma once

#include <types.h>

namespace cf {

class CPartsChange {
public:
    virtual ~CPartsChange();

    /* 0x04 */ u8 mField04;
    /* 0x08 */ u32 mField08;
    /* 0x0C */ u8 mData[0x20];
    /* 0x2C */ u8 mField2C;
};

// Struct filled by func_80198710 (likely CfPartyInfo)
struct CfPartyInfo {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 field_08;
    /* 0x0C */ s32 field_0C;
    /* 0x10 */ u32 field_10;
    /* 0x14 */ s32 field_14;
    /* 0x18 */ f32 field_18;
    /* 0x1C */ f32 field_1C;
    /* 0x20 */ f32 field_20;
    /* 0x24 */ u32 field_24;
    /* 0x28 */ f32 field_28;
    /* 0x2C */ u8 field_2C;
    /* 0x2D */ u8 field_2D;
    /* 0x2E */ u8 field_2E;

    void SetField2D(u8 val);
    void func_80198710(void* r4, float f1, int r5, int r6, float f2, float f3);
};

// Comparator key struct for CompareSortKey
struct CfPartyInfoSortKey {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ f32 sortKey;
};

// Wrapper for CActorParam fields accessed by CPartsChange accessors
struct CfActorAccessors {
    u8 pad_00[0x1E];
    /* 0x1E */ u16 mFlags1E;
    u8 pad_20[0x10];
    /* 0x30 */ void* mField30;
    u8 pad_34[0x58];
    /* 0x8C */ f32 mField8C;
    u8 pad_90[4];
    /* 0x94 */ u32 mField94;
    u8 pad_98[6];
    /* 0x9E */ u16 mField9E;

    void SetFlag400(int enable);
    u32 GetField94();
    u16 GetField9E();
    u32 TestFlag400();
    u32 TestFlag8();
    void SetField8C(float val);
    void* GetField30();
};

// Wrapper for CfObjectPc fields at 0x45C4+
struct CfObjectPcExt {
    u8 pad[0x45C4];
    /* 0x45C4 */ u16 mField45C4;
    /* 0x45C6 */ u16 mField45C6;
    /* 0x45C8 */ u16 mField45C8;

    void SetField45C4(u16 val);
    void SetField45C8(u16 val);
    u16 GetField45C6();
    void* GetField60C();
};

} // namespace cf


namespace cf {

// Vtable-slot view on the CPartsChange manager object. With -RTTI the two-word
// header shifts declared slot index N to vtable offset (N+2)*4, so index 173
// lands on 0x2bc -- the slot retail reads in CPartsChange_ResetBattleEntry.
// Only pure virtuals, so no vtable is emitted by this unit.
class CPartsChangeDevView {
public:
    virtual void v000() = 0;
    virtual void v001() = 0;
    virtual void v002() = 0;
    virtual void v003() = 0;
    virtual void v004() = 0;
    virtual void v005() = 0;
    virtual void v006() = 0;
    virtual void v007() = 0;
    virtual void v008() = 0;
    virtual void v009() = 0;
    virtual void v010() = 0;
    virtual void v011() = 0;
    virtual void v012() = 0;
    virtual void v013() = 0;
    virtual void v014() = 0;
    virtual void v015() = 0;
    virtual void v016() = 0;
    virtual void v017() = 0;
    virtual void v018() = 0;
    virtual void v019() = 0;
    virtual void v020() = 0;
    virtual void v021() = 0;
    virtual void v022() = 0;
    virtual void v023() = 0;
    virtual void v024() = 0;
    virtual void v025() = 0;
    virtual void v026() = 0;
    virtual void v027() = 0;
    virtual void v028() = 0;
    virtual void v029() = 0;
    virtual void v030() = 0;
    virtual void v031() = 0;
    virtual void v032() = 0;
    virtual void v033() = 0;
    virtual void v034() = 0;
    virtual void v035() = 0;
    virtual void v036() = 0;
    virtual void mAt9C(f32* v) = 0;   // index 37 -> vtable 0x94
    virtual void v038() = 0;
    virtual void v039() = 0;
    virtual void mAtA8(int arg) = 0;   // index 40 -> vtable 0xa8
    virtual void* mAtAC() = 0;   // index 41 -> vtable 0xac
    virtual void v042() = 0;
    virtual void v043() = 0;
    virtual void v044() = 0;
    virtual void v045() = 0;
    virtual void v046() = 0;
    virtual void mAtC4(f32 v) = 0;   // index 47 -> vtable 0xBC
    virtual void v048() = 0;
    virtual void v049() = 0;
    virtual void v050() = 0;
    virtual void v051() = 0;
    virtual void v052() = 0;
    virtual void mAtDC(f32 v) = 0;   // index 53 -> vtable 0xD4
    virtual f32 mAtE0() = 0;   // index 54 -> vtable 0xe0
    virtual void v055() = 0;
    virtual void v056() = 0;
    virtual void v057() = 0;
    virtual void v058() = 0;
    virtual void v059() = 0;
    virtual void v060() = 0;
    virtual void v061() = 0;
    virtual void v062() = 0;
    virtual void v063() = 0;
    virtual void v064() = 0;
    virtual void v065() = 0;
    virtual void v066() = 0;
    virtual void v067() = 0;
    virtual void v068() = 0;
    virtual void v069() = 0;
    virtual void v070() = 0;
    virtual void v071() = 0;
    virtual void v072() = 0;
    virtual void v073() = 0;
    virtual void v074() = 0;
    virtual void v075() = 0;
    virtual void v076() = 0;
    virtual void v077() = 0;
    virtual void v078() = 0;
    virtual void v079() = 0;
    virtual void v080() = 0;
    virtual void v081() = 0;
    virtual void v082() = 0;
    virtual void v083() = 0;
    virtual void v084() = 0;
    virtual void v085() = 0;
    virtual void v086() = 0;
    virtual void v087() = 0;
    virtual void v088() = 0;
    virtual void v089() = 0;
    virtual void v090() = 0;
    virtual void v091() = 0;
    virtual void v092() = 0;
    virtual void v093() = 0;
    virtual void v094() = 0;
    virtual void v095() = 0;
    virtual void v096() = 0;
    virtual void v097() = 0;
    virtual void v098() = 0;
    virtual void v099() = 0;
    virtual void v100() = 0;
    virtual void v101() = 0;
    virtual void v102() = 0;
    virtual void v103() = 0;
    virtual void v104() = 0;
    virtual void v105() = 0;
    virtual void v106() = 0;
    virtual void v107() = 0;
    virtual void v108() = 0;
    virtual void v109() = 0;
    virtual void v110() = 0;
    virtual void v111() = 0;
    virtual void v112() = 0;
    virtual void v113() = 0;
    virtual void v114() = 0;
    virtual void v115() = 0;
    virtual void v116() = 0;
    virtual void v117() = 0;
    virtual void v118() = 0;
    virtual void v119() = 0;
    virtual void v120() = 0;
    virtual void v121() = 0;
    virtual void v122() = 0;
    virtual void v123() = 0;
    virtual void v124() = 0;
    virtual void v125() = 0;
    virtual void v126() = 0;
    virtual void v127() = 0;
    virtual void v128() = 0;
    virtual void v129() = 0;
    virtual void v130() = 0;
    virtual void v131() = 0;
    virtual void v132() = 0;
    virtual void v133() = 0;
    virtual void v134() = 0;
    virtual void v135() = 0;
    virtual void v136() = 0;
    virtual void v137() = 0;
    virtual void v138() = 0;
    virtual void v139() = 0;
    virtual void v140() = 0;
    virtual void v141() = 0;
    virtual void v142() = 0;
    virtual void v143() = 0;
    virtual void v144() = 0;
    virtual void v145() = 0;
    virtual void v146() = 0;
    virtual void v147() = 0;
    virtual void v148() = 0;
    virtual void v149() = 0;
    virtual void v150() = 0;
    virtual void v151() = 0;
    virtual void v152() = 0;
    virtual void v153() = 0;
    virtual void v154() = 0;
    virtual void v155() = 0;
    virtual void v156() = 0;
    virtual void v157() = 0;
    virtual void v158() = 0;
    virtual void v159() = 0;
    virtual void v160() = 0;
    virtual void v161() = 0;
    virtual void v162() = 0;
    virtual void v163() = 0;
    virtual void v164() = 0;
    virtual void v165() = 0;
    virtual void v166() = 0;
    virtual void v167() = 0;
    virtual void v168() = 0;
    virtual void v169() = 0;
    virtual void v170() = 0;
    virtual void v171() = 0;
    virtual void v172() = 0;
    virtual int mAt2BC() = 0;   // index 173 -> vtable 0x2bc
};

} // namespace cf
