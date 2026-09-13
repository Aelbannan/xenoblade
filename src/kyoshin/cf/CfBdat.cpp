#include "decomp.h"
#include "types.h"

// Typed .data (retail 0xC0): quest pointer table + IWorkEvent vtable block + RTTI tail.
// Uses u32 + (u32)& casts so function relocs emit as ADDR32 with zero file bytes (retail shape).
extern "C" void __dt__Q22cf6CfBdatFv();
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__Q22cf6CfBdatFP10CEventFile();
extern "C" void WorkEvent3__10IWorkEventFPv();
extern "C" void WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb();
extern "C" void WorkEvent6__10IWorkEventFv();
extern "C" void WorkEvent7__10IWorkEventFv();
extern "C" void WorkEvent8__10IWorkEventFv();
extern "C" void WorkEvent9__10IWorkEventFv();
extern "C" void WorkEvent10__10IWorkEventFv();
extern "C" void WorkEvent11__10IWorkEventFv();
extern "C" void WorkEvent12__10IWorkEventFv();
extern "C" void WorkEvent13__10IWorkEventFv();
extern "C" void WorkEvent14__10IWorkEventFv();
extern "C" void WorkEvent15__10IWorkEventFv();
extern "C" void WorkEvent16__10IWorkEventFv();
extern "C" void WorkEvent17__10IWorkEventFv();
extern "C" void WorkEvent18__10IWorkEventFv();
extern "C" void WorkEvent19__10IWorkEventFv();
extern "C" void WorkEvent20__10IWorkEventFv();
extern "C" void WorkEvent21__10IWorkEventFv();
extern "C" void WorkEvent22__10IWorkEventFv();
extern "C" void WorkEvent23__10IWorkEventFv();
extern "C" void WorkEvent24__10IWorkEventFv();
extern "C" void WorkEvent25__10IWorkEventFv();
extern "C" void WorkEvent26__10IWorkEventFv();
extern "C" void WorkEvent27__10IWorkEventFv();
extern "C" void WorkEvent28__10IWorkEventFv();
extern "C" void WorkEvent29__10IWorkEventFv();
extern "C" void WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" char lbl_eu_80662230[];

// Typed .rodata (retail 0x420): type name + bdat table-name pool + face names.
// Sizes are retail gaps so the section packs exactly.
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80500F98[0x0C] = "cf::CfBdat";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80500FA4[0x3E4] =
    "BTL_pclist\0"
    "BTL_enelist\0"
    "BTL_skilllist\0"
    "BTL_growlist\0"
    "BTL_bufflist\0"
    "FLD_pointlist\0"
    "FLD_tboxlist\0"
    "BTL_camlist\0"
    "BTL_camdatalist\0"
    "BTL_crystalnamelist\0"
    "MNU_item\0"
    "FLD_npclist\0"
    "FLD_maplist\0"
    "landmarklist\0"
    "FLD_valpoplist\0"
    "FLD_dmobjlist\0"
    "ITM_itemlist\0"
    "ITM_wpnlist\0"
    "ITM_equiplist\0"
    "ITM_crystallist\0"
    "ITM_dropcrystallist\0"
    "ITM_collectlist\0"
    "ITM_materiallist\0"
    "ITM_valuablelist\0"
    "ITM_artslist\0"
    "ITM_headlist\0"
    "ITM_bodylist\0"
    "ITM_armlist\0"
    "ITM_waistlist\0"
    "ITM_legglist\0"
    "BTL_PSVskill\0"
    "BTL_PSVlink\0"
    "BTL_PSSlist\0"
    "routelist%02d%02d\0"
    "mapefflist%02d%02d\0"
    "mapobjlist%02d%02d\0"
    "maplodlist%02d%02d\0"
    "mapselist%02d%02d\0"
    "FLD_GimCamList%02d%02d\0"
    "Litemlist%02d%02d\0"
    "extalklist%02d%02d\0"
    "minelist%02d%02d\0"
    "BTL_enelist%02d%02d\0"
    "Gimlist%02d%02d\0"
    "FieldLock%02d%02d\0"
    "ElvGm%02d%02d\0"
    "WarpGm%02d%02d\0"
    "JumpGm%02d%02d\0"
    "ItemGm%02d%02d\0"
    "GimSVoff%02d%02d\0"
    "GimEne%02d%02d\0"
    "GimMess%02d%02d\0"
    "drop_nmllist%02d%02d\0"
    "drop_rarlist%02d%02d\0"
    "drop_sprlist%02d%02d\0"
    "resource\0"
    "posX\0"
    "posY\0"
    "posZ\0"
    "pc%d\0"
    "parts\0"
    "bat_mount\0"
    "def_mount\0"
    "name\0"
    "itemType\0"
    "itemID\0"
    "\x96\xA2\x92\xE8\x8B\x60\x83\x41\x83\x43\x83\x65\x83\x80\0"
    "common/jp/bdat_eve.bin\0"
    "MNU_eve_start\0"
    "MNU_eve_table";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80501388[0x0C] = "face_n01";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80501394[0x0C] = "face_m01";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805013A0[0x0C] = "face_n02";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805013AC[0x0C] = "face_m02";

// Typed .sdata (retail 0x8): "pc1" + pad to section size.
__declspec(section ".sdata") __attribute__((used, aligned(8))) char lbl_eu_80662228[8] = "pc1";

// Typed .sdata2 (retail 0x60): floats, EXwp tags, face pointers, 2^52, 4.0, 1.0.
// Struct freezes retail declaration order (MWCC reorders loose floats by first-use).
struct Sdata2_CfBdat {
    float f0;          // +0x00 lbl_eu_80667368 0.0f (lfs pool)
    char empty[4];     // +0x04 lbl_eu_8066736C "" (empty string for quest table slot 0)
    char exL[8];        // +0x08 lbl_eu_80667370 "EXwp_L"
    char exR[8];        // +0x10 lbl_eu_80667378 "EXwp_R"
    char exS[8];        // +0x18 lbl_eu_80667380 "EXwp_S"
    char exH[8];        // +0x20 lbl_eu_80667388 "EXwp_H"
    char exHL[8];       // +0x28 lbl_eu_80667390 "EXwp_HL"
    char exHR[8];       // +0x30 lbl_eu_80667398 "EXwp_HR"
    float zero2[2];     // +0x38 lbl_eu_806673A0 {0.0f, 0.0f}
    const char* face[4];// +0x40 lbl_eu_806673A8..B4 -> face_n01/m01/n02/m02
    double d52;         // +0x50 lbl_eu_806673B8 2^52 (int->double magic)
    float f4;           // +0x58 lbl_eu_806673C0 4.0f
    float f1;           // +0x5C lbl_eu_806673C4 1.0f
};
__declspec(section ".sdata2") __attribute__((used, aligned(8))) const Sdata2_CfBdat sdata2_CfBdat = {
    0.0f, "", "EXwp_L", "EXwp_R", "EXwp_S", "EXwp_H", "EXwp_HL", "EXwp_HR",
    {0.0f, 0.0f}, {lbl_eu_80501388, lbl_eu_80501394, lbl_eu_805013A0, lbl_eu_805013AC},
    4503599627370496.0, 4.0f, 1.0f
};
#define lbl_eu_80667368 sdata2_CfBdat.f0
#define lbl_eu_8066736C sdata2_CfBdat.empty
#define lbl_eu_80667370 sdata2_CfBdat.exL
#define lbl_eu_80667378 sdata2_CfBdat.exR
#define lbl_eu_80667380 sdata2_CfBdat.exS
#define lbl_eu_80667388 sdata2_CfBdat.exH
#define lbl_eu_80667390 sdata2_CfBdat.exHL
#define lbl_eu_80667398 sdata2_CfBdat.exHR
#define lbl_eu_806673A0 sdata2_CfBdat.zero2
#define lbl_eu_806673B8 sdata2_CfBdat.d52
#define lbl_eu_806673C0 sdata2_CfBdat.f4
#define lbl_eu_806673C4 sdata2_CfBdat.f1

extern "C" u32 lbl_eu_8052E6F0[10] = {
    (u32)&sdata2_CfBdat.empty, (u32)&sdata2_CfBdat.exL, (u32)&sdata2_CfBdat.exR,
    (u32)&sdata2_CfBdat.exR, (u32)&sdata2_CfBdat.exS, (u32)&sdata2_CfBdat.exH,
    (u32)&sdata2_CfBdat.exHL, (u32)&sdata2_CfBdat.exHR, (u32)&sdata2_CfBdat.exHR, 0x00000000,
};
extern "C" u32 lbl_eu_8052E718[34] = {
    (u32)&lbl_eu_80662230, 0x00000000, (u32)&__dt__Q22cf6CfBdatFv, (u32)&WorkEvent1__10IWorkEventFPvPCc,
    (u32)&OnFileEvent__Q22cf6CfBdatFP10CEventFile, (u32)&WorkEvent3__10IWorkEventFPv,
    (u32)&WorkEvent4__10IWorkEventFv, (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
};
extern "C" u32 lbl_eu_8052E7A0[4] = {
    (u32)&__RTTI__10IWorkEvent, 0x00000000, 0x00000000, 0x00000000,
};

// Typed .sbss (retail 0x98 = 38 words): bdat file-data pointers 80664090..8124.
// Zero-fill section: only size (0x98) and align (8) are compared.
// First object carries aligned(8) so the section keeps retail align 8.
void* lbl_eu_80664090 __attribute__((aligned(8)));
void* lbl_eu_80664094;
void* lbl_eu_80664098;
void* lbl_eu_8066409C;
void* lbl_eu_806640A0;
void* lbl_eu_806640A4;
void* lbl_eu_806640A8;
void* lbl_eu_806640AC;
void* lbl_eu_806640B0;
void* lbl_eu_806640B4;
void* lbl_eu_806640B8;
void* lbl_eu_806640BC;
void* lbl_eu_806640C0;
void* lbl_eu_806640C4;
void* lbl_eu_806640C8;
void* lbl_eu_806640CC;
void* lbl_eu_806640D0;
void* lbl_eu_806640D4;
void* lbl_eu_806640D8;
void* lbl_eu_806640DC;
void* lbl_eu_806640E0;
void* lbl_eu_806640E4;
void* lbl_eu_806640E8;
void* lbl_eu_806640EC;
void* lbl_eu_806640F0;
void* lbl_eu_806640F4;
void* lbl_eu_806640F8;
void* lbl_eu_806640FC;
void* lbl_eu_80664100;
void* lbl_eu_80664104;
void* lbl_eu_80664108;
void* lbl_eu_8066410C;
void* lbl_eu_80664110;
void* lbl_eu_80664114;
void* lbl_eu_80664118;
void* lbl_eu_8066411C;
void* lbl_eu_80664120;
void* lbl_eu_80664124;

extern "C" u32 func_8003B1EC(void* bdat);
extern "C" u32 func_8003B41C(void* bdat);
extern "C" u32 func_800AA714(const char* path);
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index);

namespace cf {
class CfBdat {
public:
    static u32 func_801422A8(u32 param1);
};
}

// Weapon-id translator: ids with top-5-bit tag 5 pass through; other
// nonzero ids index the ITM_wpnlist table via its name column.
u32 cf::CfBdat::func_801422A8(u32 param1) {
    if (param1 != 0) {
        u32 result;
        void* data;
        if ((param1 >> 27) == 5) {
            return param1;
        }
        result = 0;
        if (param1 & 0xFFFF) {
            data = lbl_eu_806640F4;
            int total = (int)(func_8003B1EC(data) + func_8003B41C(data));
            if ((u16)param1 < total) {
                result = func_800AA714((const char*)getBdatStringColumnValue(
                    data, &lbl_eu_80500FA4[0x34f], (u16)param1));
            }
        }
        return result;
    }
    return 0;
}
