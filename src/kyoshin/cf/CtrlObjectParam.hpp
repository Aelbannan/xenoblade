#pragma once

#include <types.h>
#include "kyoshin/cf/object/CActorParam.hpp"
#include "monolib/util/FixStr.hpp"   // ml::FixStr<64> for the func_800AA33C import

// C-linkage import: bdat column lookup (defining TU: ocBdat.cpp).
extern "C" u32 getBdatStringColumnValue(void* pData, const char* pColumn, int index);

// C-ABI imports used by func_800A18A4 (bdat file lookup / bdat init hook).
extern "C" void* getFP__FPCc(const char* name);
extern "C" void func_8003AA34();

// Global data imports (MWCC does not mangle global-scope variable names).
extern u32 lbl_eu_80663E88;    // work-buffer pointer (.sbss, sda21)
extern u32 lbl_eu_80663E24;    // global state flag (.sbss, sda21, func_8009DBF4 bit-20 test)
extern u32 lbl_eu_8052E9B0[];   // 0x90-byte .data object; its address is snapshot to the stack (.data, __ct__8009ED08)
extern u32 lbl_eu_80664A10;     // CHelpManager singleton pointer (.sbss, func_800A18A4 byte flag at +0x16)

// ── func_800A18A4 view: 8-word offset table (lbl_eu_804FBC90, .rodata) ────
// Copied to a local as a struct so MWCC inlines the 8 lwz/stw pairs
// (retail copy shape) instead of emitting a memcpy call. Global-scope extern:
// namespace-scope globals would mangle to lbl_eu_804FBC90__2cf.
struct CtrlObjectParamOffsetTable {
    u32 words[8];
};
extern CtrlObjectParamOffsetTable lbl_eu_804FBC90;
extern u32 lbl_eu_804FBC70[8];   // arts-learn offset table A (.rodata, func_800A145C)
extern u32 lbl_eu_804FBC50[8];   // arts-learn offset table B (.rodata, func_800A145C)
extern u32 lbl_eu_806667C8;    // arts-learn table words (.sdata2, func_800A145C)
extern u32 lbl_eu_806667CC;    // arts-learn table words (.sdata2, func_800A145C)
extern u32 lbl_eu_806640F4;    // item/weapon bdat file pointer (.sbss, sda21)
extern u32 lbl_eu_806640D8;    // arts bdat file pointer (.sbss, sda21, func_800A2DE8)
extern void* lbl_eu_806640DC;   // arts bdat file pointer (.sbss, sda21, func_800A1E3C)
extern u32 lbl_eu_806640F8;    // armor bdat file pointer (.sbss, sda21, func_8009D7F4)
extern u32 lbl_eu_80664090;    // arts stat bdat file pointer (.sbss, sda21, func_800A0E64)
extern char lbl_eu_804FBCB0[]; // bdat column-name string table (.rodata)
extern f32 lbl_eu_806667A0;    // arts-set float constant (.sdata2, func_800A13C4)
extern f32 lbl_eu_80666780;    // f32 scale constant (.sdata2, func_800A11A4 field_D4)
extern f32 lbl_eu_80666784;    // f32 scale constant (.sdata2, func_800A11A4 field_D6)
extern f64 lbl_eu_80666788;    // 2^52 u32->f64 conversion constant (.sdata2, func_800A11A4)
extern f64 lbl_eu_80666778;    // 2^52 u32->f64 conversion constant (.sdata2, func_8009F6D4)
extern f32 lbl_eu_80666770;    // arts-level float scale (.sdata2, func_8009D7F4)
extern f32 lbl_eu_80666774;    // arts-level float scale (.sdata2, func_8009D7F4)
extern f32 lbl_eu_806667B8;    // arts-stat float constant (.sdata2, func_800A0E64)
extern f32 lbl_eu_806667BC;    // arts-stat float constant (.sdata2, func_800A0E64)
extern f32 lbl_eu_806667C0;    // arts-stat float constant (.sdata2, func_800A0E64)
extern f32 lbl_eu_80666790;    // rank*average-level divisor (.sdata2, func_800A1E3C)
extern f32 lbl_eu_80666794;    // (.sdata2, func_8009EF9C arts-set float)
extern f32 lbl_eu_80666798;    // (.sdata2, func_8009EF9C scale)
extern f32 lbl_eu_8066679C;    // (.sdata2, func_8009EF9C scale)
extern f32 lbl_eu_80663EC8;    // (.sdata2, func_8009EF9C arts-set float)
extern f32 lbl_eu_8066A1F8;    // (.sdata2, func_8009EF9C arts-set float)
extern u32 lbl_eu_806667A8;    // arts column-name prefix word 0 (.sdata2, func_800A03F4)
extern u32 lbl_eu_806667AC;    // arts column-name prefix word 1 (.sdata2, func_800A03F4)
extern f32 lbl_eu_8066A20C;    // arts display scale (.sdata2, func_800A03F4)
extern char lbl_eu_804FBF58[]; // arts-list sprintf format string (.rodata, func_800A3A6C)
extern char lbl_eu_805280E8[];  // vtable-like pointer stored at entry+0x34FC (work-buffer ctor); array type forces lis/addi @ha/@l (retail relocs are ADDR16_HA/LO)
extern u32 lbl_eu_805280D8[4];  // arts stat column-name pointer table (.data, func_800A0E64)

// C-ABI imports for the func_* stubs in CtrlObjectParam.cpp (defining TUs:
// CfBdat.cpp / CActorParam.cpp / CItemBoxInfo.cpp). extern "C" keeps the
// call-site relocs at the plain retail names (same convention as the
// CfResPcImpl.hpp / CAIAction.hpp import blocks).
extern "C" u32 func_80142074(u32 arg1, u16 arg2, u32 arg3);                     // CfBdat.cpp

extern "C" u32 func_80141E90(u32 param1, s16 param2, u32 param3, u32 param4);   // CfBdat.cpp

extern "C" void func_80175A50(u8* value, u8* obj);                              // CActorParam.cpp

// Three args: actor (r3, already the lookup result at retail call sites),
// f32 (f1) and u32 (r4). A 2-arg (f32, u32) declaration would put the u32
// in r3 (MWCC assigns integer args to GPRs in order, floats independently).
extern "C" void func_801765A4(void* actor, f32 value, u32 arg);                 // CActorParam.cpp

extern "C" int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);  // code_800AA008.cpp (token formatter)

extern "C" void* func_80157C4C(u32 index, s16 value);                           // CItemBoxInfo.cpp

extern "C" void* getArtsParamRC2(void* base, int row, int slot);               // CAttackSet.cpp (arts-slot record)
extern "C" void setArtsSlotRC(void* base, int artsId, int row, int slot);     // CAttackSet.cpp (arts-slot write)

extern "C" u16 func_80158018(u16 value);                                        // CItem.cpp (arts level lookup)

extern "C" void* CItem_initItemImplInstances(void* item);                       // CItem.cpp (per-category item-impl accessor)

extern "C" void* getAtkParam(void* base, int index);                           // CAttackSet.cpp (attack-parameter record lookup)

extern "C" void func_80157F04(u16 v, void* outA, void* outB);                  // CItemBoxInfo.cpp (item family resolver)

extern "C" u32 func_8008235C__Q22cf13CfGameManagerFv(u32 bit);               // CfGameManager.cpp (flag-bit test)
extern "C" u32 func_8009CF8C(u32 resourceId);                                // CfGameManager.cpp (resource lookup)

extern "C" int func_8026178C(void* data, u32 flag);                          // code_8025FB10.cpp (status lookup)
extern "C" u32 func_8025FB10(void* data, u32 flag);                          // code_8025FB10.cpp (status value)
extern "C" void func_802618AC(u8* obj, int value);                           // code_8025FB10.cpp (clamped counter set)
extern "C" void func_802617B8(u8* obj, u32 value, int flag);                // code_8025FB10.cpp (arts-slot write)
extern "C" void func_8026187C(u8* obj, int value);                          // code_8025FB10.cpp (arts-change gate)

extern "C" void func_80174AE8(void* self);                                  // CfMapItemManager.cpp (row-sync helper)
extern "C" void func_80174B3C(void* self, u8 a, u8 b, u8 c);               // CfMapItemManager.cpp (row-sync helper)
extern "C" void func_80174B4C(void* actor, u32 flags);                     // CfMapItemManager.cpp / CtrlAct.hpp (status write)

struct CPcKizunagramBig;  // defined in src/kyoshin/CPcKizunagram.hpp (only used through a pointer here)
struct CEventDataTable;   // defined in src/kyoshin/cf/CTaskREvent.hpp (global scope; only used through a pointer here)

extern "C" void func_8009E7C8(u8* self);                                  // (this unit) item-slot area init at work+0x1F98
extern "C" CPcKizunagramBig* func_8025EDC8(CPcKizunagramBig* self);      // CPcKizunagram.cpp (affinity-slot clear)
extern "C" void func_8016455C(CEventDataTable* self);                    // CTaskREvent.cpp (event data table fill)

namespace cf {

    // ── Opaque struct for func_8009D764 ─────────────────────────────────────
    //   6 s16s (set to -1) + 192-byte zeroed area.
    struct CtrlObjectParamInit {
        s16 field_00;
        s16 field_02;
        s16 field_04;
        s16 field_06;
        s16 field_08;
        s16 field_0A;
        u8  blob[192];
    };

    // ── 32-byte sub-struct entry (getSubStruct / getShortAt1C / getByteE4) ──
    struct CtrlObjectParamSubEntry {
        u8  pad_00[0x10];        // 0x00..0x0F
        s16 shortArr[8];         // 0x10..0x1F  - accessed at work+0x1C = entry[0].shortArr
    };

    // ── Opaque data block with sub-entries (getSubStruct, getShortAt1C, getByteE4) ──
    struct CtrlObjectParamData {
        u16                      field_00;               // 0x00..0x01  (u16 type tag read by func_8009E054)
        u8                       pad_02[0x0A];           // 0x02..0x0B
        CtrlObjectParamSubEntry  entries[6];            // 0x0C..0xCB  (6×32=192)
        u8                       pad_CC[0x18];           // 0xCC..0xE3
        u8                       field_E4;               // 0xE4

        CtrlObjectParamSubEntry* getSubStruct(unsigned long index);
        void setArgType2(void* arg);
        void setArgType3(void* arg);
        void setArgType5(void* arg);
        long getShortAt1C(unsigned long index);
    };

    // ── Swap view (swapIntFields): int arrays at +4 and +16 ─────────────────
    struct CtrlObjectParamSwap {
        u8  pad_00[4];       // 0x00..0x03
        int intArr1[3];      // 0x04..0x0F  (type 1, indices 0,1,2)
        int intArr2[3];      // 0x10..0x1B  (type 2, indices 0,1,2)
    };

    // ── Clear view (clearStruct): byte + 24 ints ───────────────────────────
    struct CtrlObjectParamClear {
        u8  firstByte;        // 0x00
        u32 words[24];        // 0x04..0x63

        int clearStruct();
    };

    // ── Clear 16 bytes view (clear16Bytes) ─────────────────────────────────
    struct CtrlObjectParamClear16 {
        u32 words[4];         // 0x00..0x0F

        void clear16Bytes();
    };

    // ── Byte-at-E4 view (getByteE4) ────────────────────────────────────────
    struct CtrlObjectParamByteE4 {
        u8  pad_00[0xE4];
        u8  field_E4;

        u8 getByteE4();
    };

    // ── Retail-layout view of cf::CActorParam (pointer-returning vfuncs) ──
    // The declared CActorParam bases (CActorState/CBattleState/CDebugState)
    // total 8 bytes more than the retail object, so member offsets computed
    // through the class are +8 off (e.g. &unk1650 -> 0x1658). This view
    // places the fields at their true retail offsets so the return-address
    // stubs in this TU compile to the exact retail addi immediates.
    struct CActorParamRetailView {
        u8   pad_0000[0x15F0];          // 0x0000..0x15EF
        u32  unk15F0;                   // 0x15F0  (func_8009EF9C field_176C)
        u8   pad_15F4[0x1604 - 0x15F4]; // 0x15F4..0x1603
        u32  field_1604;                // 0x1604  (UnkVirtualFunc85)
        u8   pad_1608[0x1629 - 0x1608]; // 0x1608..0x1628
        u8   unk1629;                   // 0x1629  (func_8009EF9C field_17A5)
        u8   unk162A;                   // 0x162A  (field_17A6)
        u8   unk162B;                   // 0x162B  (field_17A7)
        u8   unk162C;                   // 0x162C  (field_17A8)
        u8   pad_162D[0x164C - 0x162D]; // 0x162D..0x164B
        u16  field_164C;                // 0x164C  (UnkVirtualFunc165)
        u8   pad_164E[2];               // 0x164E..0x1650
        u8   field_1650[0x78];          // 0x1650..0x16C8 (UnkVirtualFunc94)
        u8   pad_16C8[0x17E4 - 0x16C8]; // 0x16C8..0x17E4
        u8   field_17E4[0x4C];          // 0x17E4..0x1830 (UnkVirtualFunc100)
        f32  field_1830;                // 0x1830  (UnkVirtualFunc76; unk17E4.unk4C)
        u8   pad_1834[0x2740 - 0x1834]; // 0x1834..0x2740
        u8   field_2740[0xC];           // 0x2740..0x274C (UnkVirtualFunc125)
        u8   pad_274C[0x3358 - 0x274C]; // 0x274C..0x3358
        u16  field_3358;                // 0x3358  (UnkVirtualFunc152)
    };

    // ── Owner view for func_800A082C: CActorParam sub-object at +0x17C ─────
    // UnkVirtualFunc94 sits at retail vtable slot 0x20C; calling it through
    // the embedded member reproduces the lwzu r12,0x17C / lwz r12,0x20C
    // dispatch sequence.
    struct CtrlObjectParamActorOwner {
        u8 pad_0000[0x17C];
        CActorParam mParam;   // vtable at 0x17C
    };

    // ── Word-at-offset-0 view of the object returned by UnkVirtualFunc94 ────
    // func_800A082C reads the low 16 bits of the first word.
    struct CtrlObjectParamWordView {
        u32 word0;
    };

    // ── Opaque work-buffer type for the global destructor's typed delete ────
    // The empty inline dtor is elided by MWCC, but `delete` still emits its
    // null-guard, which merges with the outer if into the retail double-beq
    // (same shape as CfObjectSelectorObj's __dt__800FDC1C).
    struct CtrlObjectParamWorkBuffer {
        u8 data[0x10];
        ~CtrlObjectParamWorkBuffer() {}
    };

    // ── func_8009DBF4 view: per-slot equip arrays + arts fields ────────────
    // The equip slot at +0x1C (s16), the per-slot u16 pairs at +0x02 and
    // +0x0E (stride 2 per slot), the s16 display fields at +0xD4/+0xD6, the
    // u16 flag at +0xE6 and the embedded CActorParam at +0x17C.
    struct CtrlObjectParamSlotView {
        u16 field_00[7];        // +0x00..0x0D (field_00[i+1] = slot u16 at +0x02+i*2)
        u16 field_0E[6];        // +0x0E..0x19 (per-slot u16 at +0x0E+i*2)
        u8  pad_1A[2];          // +0x1A..0x1B
        s16 shortArr[6];        // +0x1C..0x27 (equip slot ids)
        u8  pad_28[0xD4 - 0x28];
        s16 field_D4;           // +0xD4
        s16 field_D6;           // +0xD6
        u8  pad_D8[0xE6 - 0xD8];
        u16 field_E6;           // +0xE6
        u8  pad_E8[0x17C - 0xE8];
        CActorParam mParam;     // +0x17C (vtable)
    };

    // ── func_8009DBF4 view: 0x49-stride arts row (copy source/dest) ────────
    // Eight interleaved (u16 at +0xE8+j*2, u8 at +0x119+j) pairs copied
    // between two rows selected by the bdat column bytes.
    struct CtrlObjectParamArtsRowCopy {
        u8  pad_00[0xE8];
        u16 field_E8[8];        // +0xE8..+0xF7
        u8  pad_F8[0x119 - 0xF8];
        u8  field_119[8];       // +0x119..+0x120
    };

    // ── func_8009DBF4 dispatch: vtable slot 0x14 on the sub-object at +0x184 ──
    // MWCC emits 2 leading vtable entries, so declared index N lands at slot
    // (N+2)*4: _v14 (index 3) sits at 0x14.
    struct CtrlObjectParamVt014If {
        virtual void _v08();
        virtual void _v0C();
        virtual void _v10();
        virtual void _v14(int val);
    };

    // ── func_800A03F4 view: type id + arts key + flags + CActorParam ───────
    struct CtrlObjectParamArtsInitView {
        u16 field_00;           // +0x00: type id
        u8  pad_02[0xA];
        u16 field_0C;           // +0x0C: arts bdat row key
        u8  pad_0E[0xE6 - 0x0E];
        u16 field_E6;           // +0xE6: flags
        u8  pad_E8[0x17C - 0xE8];
        CActorParam mParam;     // +0x17C (vtable)
    };

    // ── func_800A03F4 view: UnkVirtualFunc125 result ───────────────────────
    // Six u16 slots at +0 (written with constant offsets in the type-0 path
    // and indexed stores in the loop paths) and a vtable pointer at +0xC
    // used once for the opening slot-0x8 dispatch.
    struct CtrlObjectParamArtsDataV {
        u16 arr[6];             // +0x00..+0x0B
        u8  vtable[4];          // +0x0C (vtable pointer)
    };

    // ── func_800A03F4 dispatch: vtable slot 0x8 ────────────────────────────
    struct CtrlObjectParamVt008If {
        virtual void _v08();
    };

    // ── func_800A03F4 view: arts column-name buffer (two prefix words) ─────
    // The third byte of the first word is overwritten with the digit char.
    union CtrlObjectParamColName {
        u32 words[2];
        u8  bytes[8];
    };

    // ── func_800A03F4 view: attack-parameter record (getAtkParam result) ───
    struct CtrlObjectParamAtkView {
        char name[0x20];        // +0x00 (strcpy target)
        u32  field_20;          // +0x20 (strlen result)
        u8   pad_24[0x2C - 0x24];
        f32  field_2C;          // +0x2C
        f32  field_30;          // +0x30
        u8   pad_34[0x36 - 0x34];
        s16  field_36;          // +0x36 (sign-extended byte)
        s16  field_38;          // +0x38
        s16  field_3A;          // +0x3A
        s16  field_3C;          // +0x3C
        s16  field_40;          // +0x40
        u8   field_42;          // +0x42
        u8   field_43;          // +0x43
        u8   field_44;          // +0x44
        s16  field_46;          // +0x46
        u8   pad_48[0x58 - 0x48];
        s16  field_58;          // +0x58
        s16  field_5A;          // +0x5A
        s16  field_5C;          // +0x5C
        s16  field_5E;          // +0x5E
        u8   pad_60[0x64 - 0x60];
        s16  field_64;          // +0x64 (fctiwz float product)
        u8   pad_66[0x76 - 0x66];
        u8   field_76;          // +0x76
        u8   field_77;          // +0x77
        u32  field_78;          // +0x78 (flag word)
    };

    // ── func_8009EF9C view: full character entry + param tail ─────────────
    // The retail object is a 0x3DD4-stride character entry. The CActorParam
    // member drives the vtable-slot dispatches at +0x17C; the byte/u32 fields
    // at +0x176C / +0x17A5..0x17A8 sit inside the param (CActorParamRetailView
    // unk15F0 / unk1629..unk162C), and the sub-object at +0x34FC (vtable-like
    // word) plus the u32 at +0x3DA0 sit past the declared class size, so they
    // are accessed through a separate tail view.
    struct CtrlObjectParamEF9C {
        u16 field_00;            // +0x00: type id
        u8  pad_02[0x0A];        // 0x02..0x0B
        u16 field_0C;            // +0x0C: bdat row key
        u8  pad_0E[0x17C - 0x0E]; // 0x0E..0x17B
        CActorParam mParam;      // +0x17C (vtable)
    };

    // ── func_8009EF9C view: fields past the declared CActorParam span ──────
    // Cast of the mParam address: pad covers the CActorParam storage, then the
    // vtable-like word at entry+0x34FC and the u32 at +0x3DA0.
    struct CtrlObjectParamEF9CTail {
        u8  pad_00[0x3380];      // 0x00..0x337F (CActorParam storage)
        u32 field_34FC;          // +0x3380 (vtable-like object at entry+0x34FC)
        u8  pad_3500[0x3DA0 - 0x3500];
        u32 field_3DA0;          // +0x3DA0
    };

    // ── func_8009EF9C dispatch: vtable slot 0x3C on the +0x34FC object ─────
    // Declared index N lands at vtable offset (N+2)*4, so _v034 (index 13)
    // hits retail slot 0x3C. The +0x34FC word is the object's vtable pointer.
    struct CtrlObjectParamVt34FCIf {
        virtual void _v000(); virtual void _v004(); virtual void _v008(); virtual void _v00C();
        virtual void _v010(); virtual void _v014(); virtual void _v018(); virtual void _v01C();
        virtual void _v020(); virtual void _v024(); virtual void _v028(); virtual void _v02C();
        virtual void _v030();
        virtual void _v034(u32 value);   // vtable offset 0x3C
    };

    // ── func_800A145C view: type id + arts row/level keys + CActorParam ────
    struct CtrlObjectParamArtsLearnView {
        u16 field_00;           // +0x00: type id
        u8  pad_02[0x8];
        u16 field_0A;           // +0x0A (arts-level lookup key)
        u16 field_0C;           // +0x0C: bdat row key
        u8  pad_0E[0xE6 - 0x0E];
        u16 field_E6;           // +0xE6: flags
        u8  pad_E8[0x17C - 0xE8];
        CActorParam mParam;     // +0x17C (vtable)
    };

    // ── func_800A145C view: arts-slot record (getArtsParamRC2 result) ───────
    // The row pointer at +0x88 is stamped with the claimed arts-slot pointer.
    struct CtrlObjectParamArtsRecRC2 {
        u8 pad_00[0x88];
        u32 field_88;           // +0x88
    };

    // ── func_800A21F8 view: entry with arts rank/level fields ───────────────
    // The 0x49-stride arts rows are written through the item impls; the
    // arts-set storage at +0x3534 and the u32 at +0x3DD0 feed the
    // code_8025FB10.cpp helpers.
    struct CtrlObjectParamArtsRankView {
        u16 field_00;           // +0x00: type id
        u8  pad_02[0x1A];       // +0x02..0x1B
        s16 shortArr[6];        // +0x1C..0x27 (shortArr[5] = arts item at +0x26)
        u8  pad_28[0xD4 - 0x28];
        s16 field_D4;           // +0xD4
        s16 field_D6;           // +0xD6
        u8  pad_D8[0xE6 - 0xD8];
        u16 field_E6;           // +0xE6
        u8  pad_E8[0x17C - 0xE8];
        CActorParam mParam;     // +0x17C (vtable)
        u8  pad_3500[0x34];     // +0x3500..0x3533
        u8  big[0x89C];         // +0x3534..0x3DCF (arts-set storage)
        u32 field_3DD0;         // +0x3DD0
    };

    // ── func_800A2974 / func_800A26A4 / func_800A1E3C / func_800A21F8 view --
    // Full character-entry layout: u16 type id at +0, equip slot ids at
    // +0x1C, the s16 display fields at +0xD4/+0xD6, the u16 flag at +0xE6,
    // the embedded CActorParam at +0x17C, and the arts-slot area at +0x3534
    // (handed to func_802618AC / func_802617B8).
    struct CtrlObjectParamTypeView {
        u16 field_00;           // +0x00: type id
        u8  pad_02[0x1A];       // +0x02..0x1B
        s16 shortArr[6];        // +0x1C..0x27 (shortArr[5] = arts slot id at +0x26)
        u8  pad_28[0xD4 - 0x28];
        s16 field_D4;           // +0xD4
        s16 field_D6;           // +0xD6
        u8  pad_D8[0xE6 - 0xD8];
        u16 field_E6;           // +0xE6
        u8  pad_E8[0x17C - 0xE8];
        CActorParam mParam;     // +0x17C..0x34FF (vtable)
        u8  pad_3500[0x34];     // +0x3500..0x3533
        u8  big[0x8A0];         // +0x3534..0x3DD3
    };

    // -- func_800A1CA0 view: stride-4 u32 pair array at work+0x1F98 --------
    // The loop reads base[i].field_04 (cmpw + clrlwi), so the field must be
    // a 32-bit word read as signed for the > 0 test and masked to u16 for
    // the character-row offset.
    struct CtrlObjectParamSlotItem {
        u32 field_00;           // +0x00
        u32 field_04;           // +0x04 (character row id)
    };

    // -- func_800A1CA0 view: CActorParam at entry base +0x436C -------------
    // The loop loads the vtable word at base+0x436C and dispatches vtable
    // slot 0x20C (CActorParam_UnkVirtualFunc94). Calling through the embedded
    // member reproduces the retail lwzu r12,0x436c / lwz r12,0x20c sequence.
    struct CtrlObjectParamArtsOwner {
        u8 pad_0000[0x436C];
        CActorParam mParam;     // vtable at +0x436C
    };

    // ── Row-index view for func_800A32C4: u16 bdat row key at +0xC ─────────
    struct CtrlObjectParamBdatRow {
        u8  pad_00[0xC];
        u16 field_0C;
    };

    // ── Retail-layout view of cf::CObjectParam (UnkVirtualFunc3) ────────────
    // The declared CObjectParam class (CObjectState + mPtr10/unk14) places the
    // "field_30" word at 0x20, but the retail object reads it at 0x30. This
    // view places the field at its true retail offset so UnkVirtualFunc3
    // compiles to the exact retail lwz immediate.
    struct CObjectParamRetailView {
        u8   pad_0000[0x30];   // 0x00..0x2F
        u32  field_30;         // 0x30  (UnkVirtualFunc3)
        u8   pad_34[4];        // 0x34..0x37
    };

    // ── func_8009E120 view: row id + u16 table at +2 ──────────────────────
    // Retail indexes (u16)(value+1) into a u16 table starting at +0x02 and
    // reads the row id at +0x00 (the object is the arts-data block returned
    // by func_8009EC9C; CfResPcImpl.hpp declares the param as CfResPcCharData*).
    struct CtrlObjectParamRowView {
        u16 field_00;       // +0x00: row id (arg1 of func_80142074 / func_80141E90)
        u16 field_02[8];    // +0x02: u16 table, indexed by (u16)(value+1)
    };

    // ── func_800A1370 view: arts-data object ───────────────────────────────
    // u16 type id at +0x00 (fed to func_800B8B94) and a write target at
    // +0x17C (func_80175A50's obj argument).
    struct CtrlObjectParamArtsView {
        u16 field_00;         // +0x00: type id passed to func_800B8B94
        u8  pad_02[0x17A];    // +0x02..0x17B
        u8  field_17C;        // +0x17C: write target of func_80175A50
    };

    // ── func_800A1370 dispatch: vtable slot 0x28C ─────────────────────────
    // A real C++ virtual call at vtable offset 0x28C reproduces the retail
    // lwz r12,0(r3) / lwz r12,0x28C(r12) / mtctr / bctrl sequence (same
    // shape as the PCIf interface in CfObjectPc.cpp). The class is only used
    // through reinterpret_cast for calling — never instantiated, so no
    // vtable is emitted.
    struct CtrlObjectParamVt028CIf {
        virtual void _v000(); virtual void _v004(); virtual void _v008(); virtual void _v00C();
        virtual void _v010(); virtual void _v014(); virtual void _v018(); virtual void _v01C();
        virtual void _v020(); virtual void _v024(); virtual void _v028(); virtual void _v02C();
        virtual void _v030(); virtual void _v034(); virtual void _v038(); virtual void _v03C();
        virtual void _v040(); virtual void _v044(); virtual void _v048(); virtual void _v04C();
        virtual void _v050(); virtual void _v054(); virtual void _v058(); virtual void _v05C();
        virtual void _v060(); virtual void _v064(); virtual void _v068(); virtual void _v06C();
        virtual void _v070(); virtual void _v074(); virtual void _v078(); virtual void _v07C();
        virtual void _v080(); virtual void _v084(); virtual void _v088(); virtual void _v08C();
        virtual void _v090(); virtual void _v094(); virtual void _v098(); virtual void _v09C();
        virtual void _v0A0(); virtual void _v0A4(); virtual void _v0A8(); virtual void _v0AC();
        virtual void _v0B0(); virtual void _v0B4(); virtual void _v0B8(); virtual void _v0BC();
        virtual void _v0C0(); virtual void _v0C4(); virtual void _v0C8(); virtual void _v0CC();
        virtual void _v0D0(); virtual void _v0D4(); virtual void _v0D8(); virtual void _v0DC();
        virtual void _v0E0(); virtual void _v0E4(); virtual void _v0E8(); virtual void _v0EC();
        virtual void _v0F0(); virtual void _v0F4(); virtual void _v0F8(); virtual void _v0FC();
        virtual void _v100(); virtual void _v104(); virtual void _v108(); virtual void _v10C();
        virtual void _v110(); virtual void _v114(); virtual void _v118(); virtual void _v11C();
        virtual void _v120(); virtual void _v124(); virtual void _v128(); virtual void _v12C();
        virtual void _v130(); virtual void _v134(); virtual void _v138(); virtual void _v13C();
        virtual void _v140(); virtual void _v144(); virtual void _v148(); virtual void _v14C();
        virtual void _v150(); virtual void _v154(); virtual void _v158(); virtual void _v15C();
        virtual void _v160(); virtual void _v164(); virtual void _v168(); virtual void _v16C();
        virtual void _v170(); virtual void _v174(); virtual void _v178(); virtual void _v17C();
        virtual void _v180(); virtual void _v184(); virtual void _v188(); virtual void _v18C();
        virtual void _v190(); virtual void _v194(); virtual void _v198(); virtual void _v19C();
        virtual void _v1A0(); virtual void _v1A4(); virtual void _v1A8(); virtual void _v1AC();
        virtual void _v1B0(); virtual void _v1B4(); virtual void _v1B8(); virtual void _v1BC();
        virtual void _v1C0(); virtual void _v1C4(); virtual void _v1C8(); virtual void _v1CC();
        virtual void _v1D0(); virtual void _v1D4(); virtual void _v1D8(); virtual void _v1DC();
        virtual void _v1E0(); virtual void _v1E4(); virtual void _v1E8(); virtual void _v1EC();
        virtual void _v1F0(); virtual void _v1F4(); virtual void _v1F8(); virtual void _v1FC();
        virtual void _v200(); virtual void _v204(); virtual void _v208(); virtual void _v20C();
        virtual void _v210(); virtual void _v214(); virtual void _v218(); virtual void _v21C();
        virtual void _v220(); virtual void _v224(); virtual void _v228(); virtual void _v22C();
        virtual void _v230(); virtual void _v234(); virtual void _v238(); virtual void _v23C();
        virtual void _v240(); virtual void _v244(); virtual void _v248(); virtual void _v24C();
        virtual void _v250(); virtual void _v254(); virtual void _v258(); virtual void _v25C();
        virtual void _v260(); virtual void _v264(); virtual void _v268(); virtual void _v26C();
        virtual void _v270(); virtual void _v274(); virtual void _v278(); virtual void _v27C();
        virtual void _v280();
        virtual u8* _v028C();
    };

    // ── func_800A3998 view: arts list ─────────────────────────────────────
    // Linked list of rows; the type tag at +0x00 selects the handling:
    // 0x1111 = arts row (count at +0x04), 0xAAAA = pass-through, else abort.
    struct CtrlObjectParamArtsListEntry {
        u16 field_00;    // +0x00: type tag
        u8  pad_02[2];
        u32 field_04;    // +0x04: count (arts count, shifted left 5)
        u32 prev;        // +0x08: previous row
        CtrlObjectParamArtsListEntry* next;  // +0x0C
        u32 data[8];     // +0x10..0x2F (0xAAAA row: u32 slots; func_800A39E8 target = &data[4])
    };

    // ── func_800A3520 view: arts-slot header written before the entry memset ──
    struct CtrlObjectParamArtsSlot {
        CtrlObjectParamArtsListEntry* buf;  // +0x00: entry being initialized
        u32 field_04;                       // +0x04
        u32 field_08;                       // +0x08
        u32 field_0C;                       // +0x0C
    };

    // ── func_8009EB2C view: 36-byte block copy + name string ─────────────
    // Retail copies 36 bytes from work+0x00 to work+0x24 (one word, then
    // four 8-byte word pairs), writes two s16 headers at +0/+2, then
    // strncpy's a 31-char name into +0x04..+0x23.
    struct CtrlObjectParamCopyBlock {
        s16  field_00;     // +0x00
        s16  field_02;     // +0x02
        char str[0x20];    // +0x04..0x23
    };

    struct CtrlObjectParamCopyView {
        CtrlObjectParamCopyBlock src;   // +0x00..0x23
        CtrlObjectParamCopyBlock dst;   // +0x24..0x47
    };

    // ── func_8009E740 view: two int arrays cleared on value match ─────────
    struct CtrlObjectParamSlots {
        u8  pad_00[4];    // +0x00..0x03
        int arr1[3];      // +0x04..0x0F
        int arr2[6];      // +0x10..0x27
    };

    // -- func_800A33C8 view: u32 row-id slot table at work+0x1F9C ----------
    // Compared signed against the u16 type id in func_800A2974 /
    // func_800A26A4 (retail cmpw), so declared as plain int.
    struct CtrlObjectParamSlotTable {
        u8  pad_00[0x1F9C];
        union {
            int slots[10];   // +0x1F9C..0x1FC3 (signed row ids)
            u16 slots16[20]; // u16 view (cast source for the second base)
        };
    };

    // ── Work-buffer entry (stride 0x3DD4; 14 entries at work+0x41F0) ───────
    // Built by __ct__8009D604: shortArr = -1, two blobs memset, CActorParam
    // placement-new at +0x17C, vtable-like word at +0x34FC, and the
    // CPcKizunagramBig slot storage at +0x3534 (cleared by func_8025EDC8).
    struct CtrlObjectParamEntry {
        s16  field_00[7];        // 0x00..0x0D
        s16  field_0E[6];        // 0x0E..0x19 (equip slot ids, zeroed by func_8009E974)
        u8   pad_1A[2];          // 0x1A..0x1B
        s16  shortArr[6];        // 0x1C..0x27 (set to -1 by the work-buffer ctor)
        u8   blob_28[0xC0];      // 0x28..0xE7
        u8   blob_E8[0x92];      // 0xE8..0x179
        u8   pad_17A[2];         // 0x17A..0x17B
        u8   param_17C[0x3380];  // 0x17C..0x34FB (CActorParam storage)
        u32  field_34FC;         // 0x34FC..0x34FF (vtable-like pointer, lbl_eu_805280E8)
        u32  field_3500;         // 0x3500
        u32  field_3504;         // 0x3504
        u32  field_3508;         // 0x3508
        u8   field_350C;         // 0x350C
        u8   pad_350D[0x1F];     // 0x350D..0x352B
        u32  field_352C;         // 0x352C
        u32  field_3530;         // 0x3530
        u8   big[0x8A0];         // 0x3534..0x3DD3 (CPcKizunagramBig storage, cleared by func_8025EDC8)
    };  // total 0x3DD4

    // ── Full work-buffer layout (0x3A38C bytes, allocated by __ct__8009D604) ──
    struct CtrlObjectParamWork {
        CtrlObjectParamCopyBlock copy;       // 0x0000..0x0023 (func_8009EB2C source block)
        CtrlObjectParamCopyBlock copyDst;    // 0x0024..0x0047 (func_8009EB2C destination block)
        u32  field_48;                       // 0x0048..0x004B
        u8   pad_4C[4];                      // 0x004C..0x004F
        s16  field_50;                       // 0x0050
        s16  field_52;                       // 0x0052
        s16  field_54;                       // 0x0054
        s16  field_56;                       // 0x0056
        u8   slots_58[0x3E8][8];             // 0x0058..0x1F97 (1000 x 8-byte item slots)
        u8   field_1F98;                     // 0x1F98 (item-slot area head, func_8009E7C8)
        u32  slotTable[9];                   // 0x1F9C..0x1FBF (row-id slots, func_800A33C8)
        u8   pad_1FC0[0x3C];                 // 0x1FC0..0x1FFB
        u8   bitmap_1FFC[0x186A];            // 0x1FFC..0x3865 (50000-bit bitmap)
        u8   bitmap_3866[0x28];              // 0x3866..0x388D (320-bit bitmap)
        u8   pad_388E[2];                    // 0x388E..0x388F
        u8   eventTable[0x960];              // 0x3890..0x41EF (CEventDataTable storage, func_8016455C)
        CtrlObjectParamEntry entries[14];    // 0x41F0..0x3A387
        u32  field_3A388;                    // 0x3A388..0x3A38B (func_8009ECD0 / func_8009ECE0)
    };  // total 0x3A38C

    // ── Equip-row view (func_800A33C8 / func_800A3304 / func_8009DB28) ────
    // base = work + row*0x3DD4 + 0x41F0 (func_8009EC9C(row)) - the same base
    // as CtrlObjectParamEntry; parallel s16 arrays at +0x0E and +0x1C indexed
    // by the 6-slot index.
    struct CtrlObjectParamEquipRow {
        u8  pad_00[0x0E];
        s16 field_0E[6];   // +0x0E..0x19
        u8  pad_1A[2];     // +0x1A..0x1B
        s16 shortArr[6];   // +0x1C..0x27
    };

    // ── func_800A3594 view: node header written into the data area ────────
    // The split writes the new node's header through the row pointer plus
    // +0x20 displacement (fields at +0x20..0x2F of the 0x1111 row).
    struct CtrlObjectParamArtsNode {
        u8  pad_00[0x20];
        u16 field_20;      // +0x20: tag (0x1111)
        u8  pad_22[2];
        u32 field_24;      // +0x24: count
        u32 field_28;      // +0x28: prev (owning row)
        u32 field_2C;      // +0x2C: next
    };

    struct CtrlObjectParamArtsList {
        CtrlObjectParamArtsListEntry* head;  // +0x00
        u32 field_04;                        // +0x04 (selection bound)
    };

    // ── func_800A3304/8009DB28 dispatch: item-impl vtable slots 0x10/0x48 ──
    // Real C++ virtual calls on the object returned by
    // CItem_initItemImplInstances. MWCC emits 2 leading vtable entries
    // (offset-to-top + typeinfo), so declared index N lands at vtable offset
    // (N+2)*4: _v10 at 0x10 (index 2), _v48 at 0x48 (index 16).
    struct CtrlObjectParamItemImplIf {
        virtual void _v08(); virtual void _v0C(); virtual void _v10(void* arg);
        virtual void _v14(); virtual void _v18(); virtual void _v1C();
        virtual void _v20(); virtual void _v24(); virtual void _v28();
        virtual void _v2C(); virtual void _v30(); virtual void _v34();
        virtual void _v38(); virtual void _v3C(); virtual void _v40();
        virtual void _v44();
        virtual void _v48(void* arg);
    };

    // ── func_8009DFC8 dispatch: vtable slots 0xA4 / 0xA8 ───────────────────
    // Real C++ virtual calls at vtable offsets 0xA4/0xA8 through the param
    // embedded at +0x17C reproduce the retail lwz r12,0x17C / lwz r12,0xA4
    // dispatch sequences (same shape as the PCIf interface in CfObjectPc.cpp).
    // Only used through reinterpret_cast for calling - never instantiated.
    struct CtrlObjectParamVt0A8If {
        // MWCC emits 2 leading vtable entries (offset-to-top + typeinfo), so a
        // virtual declared at index N lands at vtable offset (N+2)*4: the two
        // slots below (declared indices 39/40) hit retail slots 0xA4 / 0xA8.
        virtual void _v000(); virtual void _v004(); virtual void _v008(); virtual void _v00C();
        virtual void _v010(); virtual void _v014(); virtual void _v018(); virtual void _v01C();
        virtual void _v020(); virtual void _v024(); virtual void _v028(); virtual void _v02C();
        virtual void _v030(); virtual void _v034(); virtual void _v038(); virtual void _v03C();
        virtual void _v040(); virtual void _v044(); virtual void _v048(); virtual void _v04C();
        virtual void _v050(); virtual void _v054(); virtual void _v058(); virtual void _v05C();
        virtual void _v060(); virtual void _v064(); virtual void _v068(); virtual void _v06C();
        virtual void _v070(); virtual void _v074(); virtual void _v078(); virtual void _v07C();
        virtual void _v080(); virtual void _v084(); virtual void _v088(); virtual void _v08C();
        virtual void _v090(); virtual void _v094(); virtual void _v098();
        virtual void _v0A4(cf::CtrlObjectParamActorOwner* self);
        virtual void _v0A8(int val);
    };

    // ── func_800A11A4 dispatch: item-impl vtable slot 0x28 ────────────────
    // Real C++ virtual call at vtable offset 0x28 through the object returned
    // by CItem_initItemImplInstances. Declared index N lands at vtable offset
    // (N+2)*4 (two hidden typeinfo entries), so _v028 (index 8) sits at 0x28.
    struct CtrlObjectParamVt028If {
        virtual void _v08(); virtual void _v0C(); virtual void _v10();
        virtual void _v14(); virtual void _v18(); virtual void _v1C();
        virtual void _v20(); virtual void _v24();
        virtual u16 _v028(void* item, const char* str);
    };

    // ── func_800A11A4 view: s16/u16 fields + CActorParam at +0x17C ────────
    // Reads shortArr[5] at +0x26, the s16 display fields at +0xD4/+0xD6 and
    // the u16 flag at +0xE6, and dispatches UnkVirtualFunc94 through the
    // embedded CActorParam (retail lwzu r12,0x17c / slot 0x20C sequence).
    struct CtrlObjectParamEntry11A4 {
        u8  pad_00[0x1C];
        s16 shortArr[6];           // +0x1C..0x27 (shortArr[5] at +0x26)
        u8  pad_28[0xD4 - 0x28];
        s16 field_D4;              // +0xD4
        s16 field_D6;              // +0xD6
        u8  pad_D8[0xE6 - 0xD8];
        u16 field_E6;              // +0xE6
        u8  pad_E8[0x17C - 0xE8];
        CActorParam mParam;        // +0x17C (vtable)
    };

    // ── func_8009F6D4 dispatch: vtable slots 0xA4..0x304 ──────────────────
    // Real C++ virtual calls at vtable offsets 0xA4/0xAC/0xB0/0xB8/0x28C/
    // 0x2F4/0x2FC/0x304 through the actor (func_800B8B94) or the embedded
    // CActorParam at +0x17C. Declared index N lands at (N+2)*4; the arg-
    // taking slots below reproduce the retail li r4, imm before bctrl.
    struct CtrlObjectParamVt0A4If {
        virtual void _v000(); virtual void _v004(); virtual void _v008(); virtual void _v00C();
        virtual void _v010(); virtual void _v014(); virtual void _v018(); virtual void _v01C();
        virtual void _v020(); virtual void _v024(); virtual void _v028(); virtual void _v02C();
        virtual void _v030(); virtual void _v034(); virtual void _v038(); virtual void _v03C();
        virtual void _v040(); virtual void _v044(); virtual void _v048(); virtual void _v04C();
        virtual void _v050(); virtual void _v054(); virtual void _v058(); virtual void _v05C();
        virtual void _v060(); virtual void _v064(); virtual void _v068(); virtual void _v06C();
        virtual void _v070(); virtual void _v074(); virtual void _v078(); virtual void _v07C();
        virtual void _v080(); virtual void _v084(); virtual void _v088(); virtual void _v08C();
        virtual void _v090(); virtual void _v094(); virtual void _v098();
        virtual void _v0A4(void* self);
        virtual void _v0A8(int val);
        virtual void _v0AC(int val);
        virtual void _v0B0();
        virtual void _v0B4();
        virtual void _v0B8();
        virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4(); virtual void _v0C8();
        virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4(); virtual void _v0D8();
        virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4(); virtual void _v0E8();
        virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4(); virtual void _v0F8();
        virtual void _v0FC(); virtual void _v100(); virtual void _v104(); virtual void _v108();
        virtual void _v10C(); virtual void _v110(); virtual void _v114(); virtual void _v118();
        virtual void _v11C(); virtual void _v120(); virtual void _v124(); virtual void _v128();
        virtual void _v12C(); virtual void _v130(); virtual void _v134(); virtual void _v138();
        virtual void _v13C(); virtual void _v140(); virtual void _v144(); virtual void _v148();
        virtual void _v14C(); virtual void _v150(); virtual void _v154(); virtual void _v158();
        virtual void _v15C(); virtual void _v160(); virtual void _v164(); virtual void _v168();
        virtual void _v16C(); virtual void _v170(); virtual void _v174(); virtual void _v178();
        virtual void _v17C(); virtual void _v180(); virtual void _v184(); virtual void _v188();
        virtual void _v18C(); virtual void _v190(); virtual void _v194(); virtual void _v198();
        virtual void _v19C(); virtual void _v1A0(); virtual void _v1A4(); virtual void _v1A8();
        virtual void _v1AC(); virtual void _v1B0(); virtual void _v1B4(); virtual void _v1B8();
        virtual void _v1BC(); virtual void _v1C0(); virtual void _v1C4(); virtual void _v1C8();
        virtual void _v1CC(); virtual void _v1D0(); virtual void _v1D4(); virtual void _v1D8();
        virtual void _v1DC(); virtual void _v1E0(); virtual void _v1E4(); virtual void _v1E8();
        virtual void _v1EC(); virtual void _v1F0(); virtual void _v1F4(int value, int a, int b); virtual void _v1F8();
        virtual void _v1FC(); virtual void _v200(); virtual void _v204(); virtual void _v208();
        virtual void* _v20C(); virtual void _v210(); virtual void _v214(); virtual void _v218();
        virtual void _v21C(); virtual void _v220(); virtual void* _v224(); virtual void _v228();
        virtual void _v22C(); virtual void _v230(); virtual void _v234(); virtual void _v238();
        virtual void _v23C(); virtual void _v240(); virtual void _v244(); virtual void _v248();
        virtual void _v24C(); virtual void _v250(); virtual void _v254(); virtual void _v258();
        virtual void _v25C(); virtual void _v260(); virtual void _v264(); virtual void _v268();
        virtual void _v26C(); virtual void _v270(); virtual void _v274(); virtual void _v278();
        virtual void _v27C(); virtual void _v280(); virtual void _v284(); virtual void _v288();
        virtual u8* _v028C();
        virtual void _v0290(); virtual void _v0294(); virtual void _v0298(); virtual void _v029C();
        virtual void _v02A0(); virtual void _v02A4(); virtual void _v02A8(); virtual void _v02AC();
        virtual void _v02B0(); virtual void _v02B4(); virtual void _v02B8(); virtual void _v02BC();
        virtual void _v02C0(); virtual void _v02C4(); virtual void _v02C8(); virtual void _v02CC();
        virtual void _v02D0(); virtual void _v02D4(); virtual void _v02D8(); virtual void _v02DC();
        virtual void _v02E0(); virtual void _v02E4(); virtual void _v02E8(); virtual void _v02EC();
        virtual void _v02F0();
        virtual void* _v02F4();
        virtual void _v02F8();
        virtual void _v02FC(int val);
        virtual void _v0300();
        virtual void _v0304(int val);
        virtual void _v0308(); virtual void _v030C(); virtual void _v0310(); virtual void _v0314();
        virtual void _v0318(); virtual void _v031C(); virtual void _v0320(); virtual void _v0324();
        virtual void _v0328();
        virtual void _v032C();
    };

    // ── func_8009F6D4 view: u16 type id at +0, CActorParam at +0x17C ──────
    // lhz reads the u16 type at +0 (fed to func_800B8B94); the embedded
    // CActorParam drives the vtable-slot dispatches at +0x17C.
    struct CtrlObjectParamEntry9F6D4 {
        u16 field_00;           // +0x00: type id
        u8  pad_04[0x17C - 0x02];
        CActorParam mParam;     // +0x17C (vtable)
    };

    // ── func_8009F6D4 view: vt[0x2F4] result (relative-offset record) ──────
    // field_02 is a signed byte offset added to the record base; field_10 is
    // the f32 scale multiplied by the byte at (record + offset)[4].
    struct CtrlObjectParamF2F4View {
        u8  pad_00[2];
        s16 field_02;           // +0x02: relative offset to a sub-record
        u8  pad_04[0x10 - 0x04];
        f32 field_10;           // +0x10: f32 scale
    };

    // ── func_8009E0C4 view: u16 row table with 2-byte header ───────────────
    // The value is written to row [index] (u16 rows starting at +2).
    struct CtrlObjectParamU16RowTable {
        u8  pad_00[2];
        u16 rows[1];
    };

    // ── func_800A18A4 view: CHelpManager byte flag at +0x16 ────────────────
    // The retail loads the CHelpManager singleton pointer (lbl_eu_80664A10,
    // .sbss) and sets the byte at +0x16 (CHelpManager::mLearnArtsFlag). The
    // extern pointer itself is declared at global scope (see top of header).
    struct CtrlObjectParamHelpFlag {
        u8 pad_00[0x16];
        u8 field_16;   // +0x16: learn-arts flag
    };

    // ── func_800A0E64 view: arts stat object (UnkVirtualFunc94 result) ────
    // The 0x78-byte stat record copied to the slot-0x224 result: u32 head,
    // f32s at +4..+0x18, s16s at +0x1C/+0x1E/+0x20, more f32/s16/u8 fields
    // and a u32 tail.
    struct CtrlObjectParamStats {
        u32  field_00;   // +0x00
        f32  field_04;   // +0x04
        f32  field_08;   // +0x08
        f32  field_0C;   // +0x0C
        f32  field_10;   // +0x10
        f32  field_14;   // +0x14
        f32  field_18;   // +0x18
        s16  field_1C;   // +0x1C
        s16  field_1E;   // +0x1E
        s16  field_20;   // +0x20
        u8   pad_22[2];
        f32  field_24;   // +0x24
        f32  field_28;   // +0x28
        s16  field_2C;   // +0x2C
        s16  field_2E;   // +0x2E
        s16  field_30;   // +0x30
        s16  field_32;   // +0x32
        s16  field_34;   // +0x34
        s16  field_36;   // +0x36
        s16  field_38;   // +0x38
        s16  field_3A;   // +0x3A
        u8   field_3C;   // +0x3C
        u8   pad_3D[3];
        f32  field_40;   // +0x40
        f32  field_44;   // +0x44
        f32  field_48;   // +0x48
        f32  field_4C;   // +0x4C
        f32  field_50;   // +0x50
        u8   field_54;   // +0x54
        u8   field_55;   // +0x55
        u8   field_56;   // +0x56
        u8   field_57;   // +0x57
        u8   field_58;   // +0x58
        u8   pad_59[3];
        f32  field_5C;   // +0x5C
        u32  field_60;   // +0x60
        u32  field_64;   // +0x64
        u32  field_68;   // +0x68
        u32  field_6C;   // +0x6C
        u32  field_70;   // +0x70
        u32  field_74;   // +0x74
    };

    // ── func_800A0E64 view: owner (u16 type id + CActorParam at +0x17C) ────
    struct CtrlObjectParamArtsOwner0E64 {
        u16 field_00;             // +0x00
        u8  pad_02[0x17A];        // +0x02..0x17B
        CActorParam mParam;       // +0x17C (vtable)
    };

    // ── func_8009D7F4 view: 0x20-byte arts/equip write block ──────────────
    // Written at &arr[index*0x20 + 0xC] (the r31 base of the retail). f32
    // fields come from the u32->f64->f32 conversions, s16/u16 from column
    // lookups, bytes from byte columns.
    struct CtrlObjectParamArtsBlock {
        f32  field_00;   // +0x00
        f32  field_04;   // +0x04
        f32  field_08;   // +0x08
        s16  field_0C;   // +0x0C
        s16  field_0E;   // +0x0E
        u16  field_10;   // +0x10
        u16  field_12;   // +0x12
        u16  field_14;   // +0x14
        u16  field_16;   // +0x16
        u16  field_18;   // +0x18
        u16  field_1A;   // +0x1A
        u8   field_1C;   // +0x1C (stb)
        u8   field_1D;   // +0x1D (stb)
        u16  field_1E;   // +0x1E (sth)
        u8   field_1F;   // +0x1F
    };

    // ── func_8009D7F4 view: item instance word (arts id in bits 15..5) ─────
    // The retail extracts (word0 >> 5) & 0x7FF from the first word.
    struct CtrlObjectParamItemWord {
        u32 word0;
    };

    // ── func_800A2DE8 view: 8-slot arts table (self) ──────────────────────
    // Each slot is 0x10 bytes: u16 id at +0 (0 = empty), byte/level fields at
    // +2/+4/+6/+8/+0xA, u16 level at +0xC, u16 bit flags at +0xE.
    struct CtrlObjectParamArtsSlotItem {
        u16 field_00;   // +0x00: arts id (0 = empty)
        u16 field_02;   // +0x02
        u16 field_04;   // +0x04
        u16 field_06;   // +0x06
        s16 field_08;   // +0x08 (signed: lha in retail)
        s16 field_0A;   // +0x0A (signed: lha in retail)
        u16 field_0C;   // +0x0C
        u16 field_0E;   // +0x0E: bit15 = value bit, bit14 = src row bit0
    };
    struct CtrlObjectParamArtsTable8 {
        CtrlObjectParamArtsSlotItem slots[8];   // 0x00..0x7F
    };

    // ── func_800A2DE8 view: source row (stride 0x40, 4 slots of 0x10) ──────
    // u32 header at +0 (level + 11-bit signed fields), u16 row id at +4 whose
    // top 12 bits carry the arts type id; slot ids at +0x4/+0x14/+0x24/+0x34
    // and slot flag words at +0xE/+0x1E/+0x2E/+0x3E (bit 15 = compare bit).
    struct CtrlObjectParamArtsSrcRow {
        u32  field_00;      // +0x00
        u16  field_04;      // +0x04: slot0 id (top 12 bits = arts type)
        u8   pad_06[0x8];   // +0x06..0x0D
        u16  field_0E;      // +0x0E: slot0 flags
        u16  field_10;      // +0x10: slot1 id
        u8   pad_12[0xC];   // +0x12..0x1D
        u16  field_1E;      // +0x1E: slot1 flags
        u16  field_20;      // +0x20: slot2 id
        u8   pad_22[0xC];   // +0x22..0x2D
        u16  field_2E;      // +0x2E: slot2 flags
        u16  field_30;      // +0x30: slot3 id
        u8   pad_32[0xC];   // +0x32..0x3D
        u16  field_3E;      // +0x3E: slot3 flags
    };

    // ── func_800A18A4 view: 8-word offset table (lbl_eu_804FBC90) ─────────
    // The struct/extern live at global scope (see top of header) so the
    // symbol stays unmangled.
    // ── func_800A18A4 view: arts-slot owner ────────────────────────────────
    // u16 type id at +0 (fed to func_800B8B94), u16 bdat row key at +0xC, a
    // 0x49-stride byte-row table at +0xE8 (indexed by the +0x58 column
    // value), and the embedded CActorParam at +0x17C (vtable slots 0xA4 /
    // 0x28C / 0x32C).
    struct CtrlObjectParamArtsSlotOwner {
        u16 field_00;           // +0x00: type id
        u8  pad_02[0xA];        // +0x02..0x0B
        u16 field_0C;           // +0x0C: bdat row key
        u8  pad_0E[0xE8 - 0x0E];
        u8  rows[0x17C - 0xE8]; // +0xE8..0x17B: byte-row table (stride 0x49)
        CActorParam mParam;     // +0x17C (vtable)
    };

    // ── __ct__8009ED08 stack-snapshot blocks (entryB param sub-region) ─────
    // The constructor snapshots three blocks of entryB's param area to stack
    // locals across the func_80175A50 call, then restores them. The field
    // types drive the retail copy shapes: block1 = 2 u16s + a word-pair
    // counted loop; block2 = u32 + 8 u16s + a u16[0x38] loop (save) / word
    // pairs (restore); block3 = 12 words.
    struct CtrlObjectParamCopyBlock1 {   // 0x1524 bytes
        u16 field_00;
        u16 field_02;
        u8  field_04[0x1520];
    };
    struct CtrlObjectParamCopyBlock2 {   // 0x84 bytes
        u32 field_00;
        u16 field_04;
        u16 field_06;
        u16 field_08;
        u16 field_0A;
        s16 field_0C;
        s16 field_0E;
        u16 field_10;
        u16 field_12;
        u16 field_14[0x38];
    };
    struct CtrlObjectParamCopyBlock3 {   // 0x30 bytes
        u32 field_00[0xC];
    };

} // namespace cf
