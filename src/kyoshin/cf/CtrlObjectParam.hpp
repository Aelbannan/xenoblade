#pragma once

#include <types.h>
#include "kyoshin/cf/object/CActorParam.hpp"
#include "monolib/util/FixStr.hpp"   // ml::FixStr<64> for the func_800AA33C import

// C-linkage import: bdat column lookup (defining TU: ocBdat.cpp).
extern "C" u32 getBdatStringColumnValue(void* pData, const char* pColumn, int index);

// Global data imports (MWCC does not mangle global-scope variable names).
extern u32 lbl_eu_80663E88;    // work-buffer pointer (.sbss, sda21)
extern u32 lbl_eu_806640F4;    // item/weapon bdat file pointer (.sbss, sda21)
extern char lbl_eu_804FBCB0[]; // bdat column-name string table (.rodata)
extern f32 lbl_eu_806667A0;    // arts-set float constant (.sdata2, func_800A13C4)
extern char lbl_eu_804FBF58[]; // arts-list sprintf format string (.rodata, func_800A3A6C)
extern char lbl_eu_805280E8[];  // vtable-like pointer stored at entry+0x34FC (work-buffer ctor); array type forces lis/addi @ha/@l (retail relocs are ADDR16_HA/LO)

// C-ABI imports for the func_* stubs in CtrlObjectParam.cpp (defining TUs:
// CfBdat.cpp / CActorParam.cpp / CItemBoxInfo.cpp). extern "C" keeps the
// call-site relocs at the plain retail names (same convention as the
// CfResPcImpl.hpp / CAIAction.hpp import blocks).
extern "C" u32 func_80142074(u32 arg1, u16 arg2, u32 arg3);                     // CfBdat.cpp

extern "C" u32 func_80141E90(u32 param1, s16 param2, u32 param3, u32 param4);   // CfBdat.cpp

extern "C" void func_80175A50(u8* value, u8* obj);                              // CActorParam.cpp

extern "C" void func_801765A4(f32 value, u32 arg);                              // CActorParam.cpp

extern "C" int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);  // code_800AA008.cpp (token formatter)

extern "C" void* func_80157C4C(u32 index, s16 value);                           // CItemBoxInfo.cpp

extern "C" void* CItem_initItemImplInstances(void* item);                       // CItem.cpp (per-category item-impl accessor)

extern "C" void func_80157F04(u16 v, void* outA, void* outB);                  // CItemBoxInfo.cpp (item family resolver)

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
        u8   pad_0000[0x1604];          // 0x0000..0x1603
        u32  field_1604;                // 0x1604  (UnkVirtualFunc85)
        u8   pad_1608[0x164C - 0x1608]; // 0x1608..0x164B
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

    // ── func_800A33C8 view: u32 row-id slot table at work+0x1F9C ──────────
    struct CtrlObjectParamSlotTable {
        u8  pad_00[0x1F9C];
        u32 slots[10];    // +0x1F9C..0x1FC3
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

    // ── func_8009E0C4 view: u16 row table with 2-byte header ───────────────
    // The value is written to row [index] (u16 rows starting at +2).
    struct CtrlObjectParamU16RowTable {
        u8  pad_00[2];
        u16 rows[1];
    };

} // namespace cf
