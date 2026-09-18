#include <types.h>
#include <cstring>
#include "kyoshin/cf/object/CActorParam.hpp"
#include "monolib/util/MemManager.hpp"

extern "C" void dummy() {}

#pragma section data_type ".data" ".data"

// --- typified .data (layout from build/us/asm/kyoshin/cf/CfNandManager.s) ---
// Slots carrying relocs emit zero bytes + ADDR32 relocs in the .o, matching
// retail (the gate compares bytes; reloc names are ignored once bytes match).
// Slot targets owned by other TUs are extern (UNDEF here, resolved at link
// via the global symbol map). The four CTTask<cf::CfNandManager> template
// slots spell '<' and are not expressible in C++; they stay zero with the
// retail target in comments. The 0x90 switch jumptable targets local case
// labels of func_8023EABC (not in this stub TU), so it stays a named zero
// table. The trailing zero word is split off so MWCC's 8-alignment of
// size%8==0 .data objects cannot pad the section (retail packs at 4).
extern "C" {
void __dt__Q22cf13CfNandManagerFv();
void Reset__14CChildListNodeFv();
void Init__Q22cf13CfNandManagerFv();
void Term__Q22cf13CfNandManagerFv();
void Move__Q22cf13CfNandManagerFv();
void Tail__8CProcessFv();
void destroyViaWorkEvent__Q22cf13CfNandManagerFv();
void WorkEvent1__10IWorkEventFPvPCc();
void OnFileEvent__Q22cf13CfNandManagerFv();
void WorkEvent3__10IWorkEventFPv();
void WorkEvent4__10IWorkEventFv();
void OnPauseTrigger__10IWorkEventFb();
void WorkEvent6__10IWorkEventFv();
void WorkEvent7__10IWorkEventFv();
void WorkEvent8__10IWorkEventFv();
void WorkEvent9__10IWorkEventFv();
void WorkEvent10__10IWorkEventFv();
void WorkEvent11__10IWorkEventFv();
void WorkEvent12__10IWorkEventFv();
void WorkEvent13__10IWorkEventFv();
void WorkEvent14__10IWorkEventFv();
void WorkEvent15__10IWorkEventFv();
void WorkEvent16__10IWorkEventFv();
void WorkEvent17__10IWorkEventFv();
void WorkEvent18__10IWorkEventFv();
void WorkEvent19__10IWorkEventFv();
void WorkEvent20__10IWorkEventFv();
void WorkEvent21__10IWorkEventFv();
void WorkEvent22__10IWorkEventFv();
void WorkEvent23__10IWorkEventFv();
void WorkEvent24__10IWorkEventFv();
void WorkEvent25__10IWorkEventFv();
void WorkEvent26__10IWorkEventFv();
void WorkEvent27__10IWorkEventFv();
void WorkEvent28__10IWorkEventFv();
void WorkEvent29__10IWorkEventFv();
void WorkEvent30__10IWorkEventFv();
void WorkEvent31__10IWorkEventFv();
void destroyViaRender__Q22cf13CfNandManagerFv();
void handleRenderCallback__Q22cf13CfNandManagerFv();
void handleFileEvent__Q22cf13CfNandManagerFv();
void cbRenderBefore__Q22cf13CfNandManagerFv();
}
extern char lbl_eu_80661940[];
extern char lbl_eu_80661948[];
extern char lbl_eu_80661950[];
extern char lbl_eu_80661958[];
extern char __RTTI__10IWorkEvent[];
// this TU's own .sdata pair tables (defined below; vtable slots point at them)
extern const void* lbl_eu_806628C8[2];
extern const void* lbl_eu_806628D0[2];
// fwd decls so the .sdata initializers below see the tables (defs inherit
// external linkage, matching retail's global .obj bindings)
extern const void* lbl_eu_80536BBC[49];
extern const void* lbl_eu_80536C80[13];
extern const void* lbl_eu_80536CB4[9];
extern const void* lbl_eu_80536CD8[7];

__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char jumptable_eu_80536B2C[0x90] = {0};
__declspec(section ".data") __attribute__((used)) const void* lbl_eu_80536BBC[49] = {
    lbl_eu_806628C8, 0,
    (const void*)__dt__Q22cf13CfNandManagerFv,
    (const void*)Reset__14CChildListNodeFv,
    (const void*)Init__Q22cf13CfNandManagerFv,
    (const void*)Term__Q22cf13CfNandManagerFv,
    (const void*)Move__Q22cf13CfNandManagerFv,
    0, // retail Draw__28CTTask<Q22cf13CfNandManager>Fv (unspellable '<')
    (const void*)Tail__8CProcessFv,
    lbl_eu_806628C8, (const void*)0xFFFFFFAC,
    (const void*)destroyViaWorkEvent__Q22cf13CfNandManagerFv,
    (const void*)WorkEvent1__10IWorkEventFPvPCc,
    (const void*)OnFileEvent__Q22cf13CfNandManagerFv,
    (const void*)WorkEvent3__10IWorkEventFPv,
    (const void*)WorkEvent4__10IWorkEventFv,
    (const void*)OnPauseTrigger__10IWorkEventFb,
    (const void*)WorkEvent6__10IWorkEventFv,
    (const void*)WorkEvent7__10IWorkEventFv,
    (const void*)WorkEvent8__10IWorkEventFv,
    (const void*)WorkEvent9__10IWorkEventFv,
    (const void*)WorkEvent10__10IWorkEventFv,
    (const void*)WorkEvent11__10IWorkEventFv,
    (const void*)WorkEvent12__10IWorkEventFv,
    (const void*)WorkEvent13__10IWorkEventFv,
    (const void*)WorkEvent14__10IWorkEventFv,
    (const void*)WorkEvent15__10IWorkEventFv,
    (const void*)WorkEvent16__10IWorkEventFv,
    (const void*)WorkEvent17__10IWorkEventFv,
    (const void*)WorkEvent18__10IWorkEventFv,
    (const void*)WorkEvent19__10IWorkEventFv,
    (const void*)WorkEvent20__10IWorkEventFv,
    (const void*)WorkEvent21__10IWorkEventFv,
    (const void*)WorkEvent22__10IWorkEventFv,
    (const void*)WorkEvent23__10IWorkEventFv,
    (const void*)WorkEvent24__10IWorkEventFv,
    (const void*)WorkEvent25__10IWorkEventFv,
    (const void*)WorkEvent26__10IWorkEventFv,
    (const void*)WorkEvent27__10IWorkEventFv,
    (const void*)WorkEvent28__10IWorkEventFv,
    (const void*)WorkEvent29__10IWorkEventFv,
    (const void*)WorkEvent30__10IWorkEventFv,
    (const void*)WorkEvent31__10IWorkEventFv,
    lbl_eu_806628C8, (const void*)0xFFFFFFA8,
    (const void*)destroyViaRender__Q22cf13CfNandManagerFv,
    (const void*)handleRenderCallback__Q22cf13CfNandManagerFv,
    (const void*)handleFileEvent__Q22cf13CfNandManagerFv,
    (const void*)cbRenderBefore__Q22cf13CfNandManagerFv,
};
__declspec(section ".data") __attribute__((used)) const void* lbl_eu_80536C80[13] = {
    lbl_eu_80661940, (const void*)0x58,
    __RTTI__10IWorkEvent, (const void*)0x54,
    lbl_eu_80661958, 0, lbl_eu_80661950, 0, lbl_eu_80661948, 0,
    lbl_eu_806628D0, 0, 0,
};
__declspec(section ".data") __attribute__((used)) const void* lbl_eu_80536CB4[9] = {
    lbl_eu_806628D0, 0,
    0, // retail __dt__28CTTask<Q22cf13CfNandManager>Fv (unspellable '<')
    (const void*)Reset__14CChildListNodeFv,
    0, 0,
    0, // retail Move__28CTTask<Q22cf13CfNandManager>Fv (unspellable '<')
    0, // retail Draw__28CTTask<Q22cf13CfNandManager>Fv (unspellable '<')
    (const void*)Tail__8CProcessFv,
};
__declspec(section ".data") __attribute__((used)) const void* lbl_eu_80536CD8[7] = {
    lbl_eu_80661958, 0, lbl_eu_80661950, 0, lbl_eu_80661948, 0, 0,
};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char s_nandTail[4] = {0}; // retail +0x1C8 null word
#pragma section data_type ".data" ".bss"
// typed .rodata strings (each sized to its retail gap).
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_8050B440[0x14] = "cf::CfNandManager";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_8050B454[0x1C] = "CTTask<cf::CfNandManager>";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_8050B470[0x18] = "%s%02d\0banner.bin\0name";
// typed .sdata pointer pairs (replaces old sdata blob which MWCC migrated
// to nobits as all-zero bytes): relocs to the string/pool labels keep the
// section live.
__declspec(section ".sdata") __attribute__((used)) const void* lbl_eu_806628C8[2] = { lbl_eu_8050B440, lbl_eu_80536C80 };
__declspec(section ".sdata") __attribute__((used)) const void* lbl_eu_806628D0[2] = { lbl_eu_8050B454, lbl_eu_80536CD8 };
// typed .bss/.sbss zero-fill (nobits compare sizes only; small sbss objects
// stay in .sbss automatically, no section attr needed).
char lbl_eu_80576CC0[0x38];
unsigned int lbl_eu_80664768;
unsigned char lbl_eu_8066476C;
unsigned char lbl_eu_8066476D;
unsigned short lbl_eu_8066476E;
unsigned short lbl_eu_80664770;
unsigned short lbl_eu_80664772;
unsigned short lbl_eu_80664774;
unsigned short lbl_eu_80664776_gap;
unsigned int lbl_eu_80664778;
void* lbl_eu_8066477C;
void* lbl_eu_80664780;
unsigned int lbl_eu_80664780_b;
unsigned int lbl_eu_80664788_a;
unsigned int lbl_eu_80664788_b;

#pragma section code_type ".text" ".text"

// Local copies of the save-image / work-entry views (the unit header cannot
// be included here: it redeclares this TU's .sbss/.bss labels with different
// types). Offsets match src/kyoshin/cf/CfNandManager.hpp.

struct __attribute__((packed)) CfNandEntryHead {
    u16 f000;
    u32 f002;
    u32 f006;
    u32 f00A;
    u32 f00E;
    u32 f012;
    u32 f016;
    u16 gap01A;
    u32 f01C;
    u32 f020;
    u32 f024;
};

struct CfNandWorkEntryDst {
    CfNandEntryHead head;
    u32 arr028[48];
    u32 arr0E8[36];
    u16 f178;
    u16 gap17A;
    u32 f17C;
    f32 f180;
    f32 f184;
    f32 f188;
    f32 f18C;
    f32 f190;
    f32 f194;
    s16 f198;
    s16 f19A;
    s16 f19C;
    u16 gap19E;
    f32 f1A0;
    f32 f1A4;
    s16 f1A8;
    s16 f1AA;
    s16 f1AC;
    s16 f1AE;
    s16 f1B0;
    s16 f1B2;
    s16 f1B4;
    s16 f1B6;
    u8 f1B8;
    u8 gap1B9[3];
    f32 f1BC;
    f32 f1C0;
    f32 f1C4;
    f32 f1C8;
    f32 f1CC;
    u8 f1D0;
    u8 f1D1;
    u8 f1D2;
    u8 f1D3;
    u8 f1D4;
    u8 gap1D5[3];
    f32 f1D8;
    u32 f1DC;
    u32 f1E0;
    u32 f1E4;
    u32 f1E8;
    u32 f1EC;
    u32 f1F0;
    u8 kizuna[0xAC];
    u32 f2A0;
    u32 f2A4;
    u32 f2A8;
    s16 f2AC;
    s16 f2AE;
    u32 f2B0;
    u8 f2B4;
    u8 gap2B5[3];
    f32 f2B8;
    f32 f2BC;
    u8 f2C0;
    u8 tail[0x40];
    u8 gapEnd[3];
};

struct CfNandWorkEntrySrc {
    CfNandEntryHead head;
    u32 arr028[48];
    u32 arr0E8[36];
    u16 f178;
    u16 gap17A;
    u32 subSlot;
    u8 gap180[0x159C];
    u32 f177C;
    u32 f1780;
    u32 f1784;
    u8 gap1788[0x1D4C];
    u32 f17CC;
    f32 f17D0;
    f32 f17D4;
    f32 f17D8;
    f32 f17DC;
    f32 f17E0;
    f32 f17E4;
    s16 f17E8;
    s16 f17EA;
    s16 f17EC;
    u16 gap17EE;
    f32 f17F0;
    f32 f17F4;
    s16 f17F8;
    s16 f17FA;
    s16 f17FC;
    s16 f17FE;
    s16 f1800;
    s16 f1802;
    s16 f1804;
    s16 f1806;
    u8 f1808;
    u8 gap1809[3];
    f32 f180C;
    f32 f1810;
    f32 f1814;
    f32 f1818;
    f32 f181C;
    u8 f1820;
    u8 f1821;
    u8 f1822;
    u8 f1823;
    u8 f1824;
    u8 gap1825[3];
    f32 f1828;
    u32 f182C;
    u32 f1830;
    u32 f1834;
    u32 f1838;
    u32 f183C;
    u32 f1840;
    s16 f34D4;
    s16 f34D6;
    u32 f34D8;
    u8 f34DC;
    u8 gap34DD[3];
    f32 f34E0;
    f32 f34E4;
    u8 gap34E8[0x4C];
    u8 kizunaSrc[0x8A0];
};

struct CfNandBattleParams {
    u32 p00;
    float p04;
    float p08;
    float p0C;
    float p10;
    float p14;
    float p18;
    s16 p1C;
    s16 p1E;
    s16 p20;
    float p24;
    float p28;
    s16 p2C;
    s16 p2E;
    s16 p30;
    s16 p32;
    s16 p34;
    s16 p36;
    s16 p38;
    s16 p3A;
    u8 p3C;
    u8 gap3D[3];
    float p40;
    float p44;
    float p48;
    float p4C;
    float p50;
    u8 p54;
    u8 p55;
    u8 p56;
    u8 p57;
    u8 p58;
    u8 gap59[3];
    float p5C;
    u32 p60;
    u32 p64;
    u32 p68;
    u32 p6C;
    u32 p70;
    u32 p74;
};

struct CfNandSaveTagBlock {
    u32 tag;
    u32 length;
    u32 unk08;
    u32 crc;
};

struct CfNandProgressArea {
    u32 field00;
    f32 f04;
    u16 f08;
    u16 f0A;
};

struct CfNandPartyEntry {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    f32 field_0C;
};

struct CfNandPartySnapshot {
    CfNandPartyEntry mEntry[3];
    u32 field_30;
};

struct CfNandCamBlock {
    u8 _pad0[0xC];
    f32 f0C;
};

struct CfNandWthrBlock {
    u8 _pad0[0xE];
    u16 f0E;
};

struct CfNandSndArea {
    f32 f00;
    f32 f04;
    f32 f08;
    u32 f0C;
};

struct __attribute__((packed)) CfNandSaveNameEntry {
    u32 f04;
    u32 f08;
    u32 f0C;
    u32 f10;
    f32 f14;
    u32 f18;
    u16 f1C;
    u8 f1E;
    u8 f1F;
};

struct CfNandSaveNameTable {
    s32 count;
    CfNandSaveNameEntry entries[20];
};

struct CfNandSaveImage {
    u32 magic;
    u32 totalSize;
    u32 unk08;
    u32 version;
    CfNandSaveTagBlock slotTag;
    u8 slot[0xB8];
    u8 slotPad[0x9BE8];
    u8 pad9CC0[0x360];
    CfNandSaveTagBlock flagTag;
    u8 flagData[0x1220];
    CfNandSaveTagBlock gameTag;
    u8 workHead[0x41F0];
    CfNandWorkEntryDst workEntry[14];
    u8 pad11E88[0x18];
    CfNandSaveTagBlock timeTag;
    CfNandProgressArea progress;
    u8 pad11EBC[0x14];
    CfNandSaveTagBlock partyTag;
    CfNandPartySnapshot snapshot;
    u8 pad11F14[0xC];
    CfNandSaveTagBlock camTag;
    CfNandCamBlock camBlock;
    u8 pad11F40[0x10];
    CfNandSaveTagBlock itemTag;
    u8 itemBlob[0x12120];
    CfNandSaveTagBlock wthrTag;
    CfNandWthrBlock wthrBlock;
    u8 pad240A0[0x10];
    CfNandSaveTagBlock sndTag;
    CfNandSndArea sndArea;
    u8 pad240D0[0x10];
    CfNandSaveTagBlock mineTag;
    u8 mineRegion[0x384];
    u8 pad24474[0x1C];
    CfNandSaveTagBlock tboxTag;
    CfNandSaveNameTable names;
    u8 pad246D4[0x1CC];
    CfNandSaveTagBlock optdTag;
    u8 optdBlob[0x40];
};

// Version-0x70001 (and the 0x60002 apply path): larger ITEM region, tail
// shifted to 0x27740+.
struct CfNandSaveImageV1 {
    u32 magic;
    u32 totalSize;
    u32 unk08;
    u32 version;
    CfNandSaveTagBlock slotTag;
    u8 slot[0xB8];
    u8 slotPad[0x9BE8];
    u8 pad9CC0[0x360];
    CfNandSaveTagBlock flagTag;
    u8 flagData[0x1220];
    CfNandSaveTagBlock gameTag;
    u8 workHead[0x41F0];
    CfNandWorkEntryDst workEntry[14];
    u8 pad11E88[0x18];
    CfNandSaveTagBlock timeTag;
    CfNandProgressArea progress;
    u8 pad11EBC[0x14];
    CfNandSaveTagBlock partyTag;
    CfNandPartySnapshot snapshot;
    u8 pad11F14[0xC];
    CfNandSaveTagBlock camTag;
    CfNandCamBlock camBlock;
    u8 pad11F40[0x10];
    CfNandSaveTagBlock itemTag;
    u8 itemBlob[0x157D0];
    CfNandSaveTagBlock wthrTag;
    CfNandWthrBlock wthrBlock;
    u8 pad27760[0x10];
    CfNandSaveTagBlock sndTag;
    CfNandSndArea sndArea;
    u8 pad277A0[0x10];
    CfNandSaveTagBlock mineTag;
    u8 mineRegion[0x384];
    u8 pad27B44[0x1C];
    CfNandSaveTagBlock tboxTag;
    CfNandSaveNameTable names;
    u8 pad27F34[0x1CC];
    CfNandSaveTagBlock optdTag;
    u8 optdBlob[0x40];
};

struct CfNandNameNode {
    CfNandNameNode* mNext;
    CfNandNameNode* mPrev;
    u32 f08;
    float f0C;
    float f10;
    float f14;
    float f18;
    u32 f1C;
    u16 f20;
    u8 f22;
    u8 f23;
};

struct CfNandNameRoot {
    u8 _pad0[4];
    CfNandNameNode* mNext;
    u8 _08[0x24];
    CfNandNameNode* slots;
    s32 slotCount;
};

struct CfNandWorkBuf {
    u8 head[0x41F0];
    CfNandWorkEntryDst entry[14];
};

extern "C" int func_8023CD9C(void* block);
extern "C" u32 CtrlRemote_GetFixedSize1214();
extern "C" u8* CtrlRemote_GetSharedBufPtr();
extern "C" u8* CtrlRemote_GetFileEventIds();
extern "C" void func_8009EF9C(void* data, u32 value);
extern "C" void func_8025ECE4(void* src, void* dst);
extern "C" void queueEventId__Q22cf13CfGameManagerFv(u32 value);
extern "C" void CfT_FrameTimerSet(u32* self);
extern "C" void CfT_PlayClockReset(f32 v);
extern "C" void cfCam_storeFollowD(void* v, f32 f);
extern "C" void loadReloadInfo(void* p);
extern "C" void fadeOutGameEffects__Q22cf13CfGameManagerFv();
extern "C" void func_80207D2C(u8* rec);
extern "C" void rebuildTboxPool();
extern "C" CfNandNameRoot* getReslistC48();
extern "C" void updateConfig__FPUc(u8* src, int mode);
extern "C" u32 CtrlRemote_TouchBitByArg(u32 resourceId);
extern "C" void setEventCounterA__Q22cf13CfGameManagerFv(u32 value);
extern "C" void setMasterVolume__Fff(f32 a, f32 b);
extern "C" void MenuSnd_SetSeVolPush_95F4(f32 v);
extern "C" void MenuSnd_SetMasterAndPush_9510(f32 v);
extern u16 lbl_eu_80661AF4;
extern u16 lbl_eu_80661AF6;
extern char* lbl_eu_806641B8;
extern f32 lbl_eu_806686E0;
extern u8 lbl_eu_80663E5D;

#pragma inline_max_size(0x4000)
#pragma inline_max_total_size(0x8000)
inline static void expandWorkEntry(CfNandWorkEntrySrc* src, CfNandWorkEntryDst* dst) {
    src->head.f000 = dst->head.f000;
    src->head.f002 = dst->head.f002;
    src->head.f006 = dst->head.f006;
    src->head.f00A = dst->head.f00A;
    src->head.f00E = dst->head.f00E;
    src->head.f012 = dst->head.f012;
    src->head.f016 = dst->head.f016;
    src->head.f01C = dst->head.f01C;
    src->head.f020 = dst->head.f020;
    struct Arr48 { u32 w[48]; };
    struct Arr36 { u32 w[36]; };
    *(Arr48*)&src->head.f024 = *(Arr48*)&dst->head.f024;
    *(Arr36*)&src->arr028[47] = *(Arr36*)&dst->arr028[47];
    u16 t178 = dst->f178;
    cf::CActorParam* actor = reinterpret_cast<cf::CActorParam*>((u8*)src + 0x17C);
    src->f178 = t178;
    src->f17CC = dst->f17C;
    src->f17D0 = dst->f180;
    src->f17D4 = dst->f184;
    src->f17D8 = dst->f188;
    src->f17DC = dst->f18C;
    src->f17E0 = dst->f190;
    src->f17E4 = dst->f194;
    src->f17E8 = dst->f198;
    src->f17EA = dst->f19A;
    src->f17EC = dst->f19C;
    src->f17F0 = dst->f1A0;
    src->f17F4 = dst->f1A4;
    src->f17F8 = dst->f1A8;
    src->f17FA = dst->f1AA;
    src->f17FC = dst->f1AC;
    src->f17FE = dst->f1AE;
    src->f1800 = dst->f1B0;
    src->f1802 = dst->f1B2;
    src->f1804 = dst->f1B4;
    src->f1806 = dst->f1B6;
    src->f1808 = dst->f1B8;
    src->f180C = dst->f1BC;
    src->f1810 = dst->f1C0;
    src->f1814 = dst->f1C4;
    src->f1818 = dst->f1C8;
    src->f181C = dst->f1CC;
    src->f1820 = dst->f1D0;
    src->f1821 = dst->f1D1;
    src->f1822 = dst->f1D2;
    src->f1823 = dst->f1D3;
    src->f1824 = dst->f1D4;
    src->f1828 = dst->f1D8;
    src->f182C = dst->f1DC;
    src->f1830 = dst->f1E0;
    src->f1834 = dst->f1E4;
    src->f1838 = dst->f1E8;
    src->f183C = dst->f1EC;
    src->f1840 = dst->f1F0;

    CfNandBattleParams* bp = (CfNandBattleParams*)actor->CActorParam_getBattleParams();
    bp->p00 = src->f17CC;
    bp->p04 = src->f17D0;
    bp->p08 = src->f17D4;
    bp->p0C = src->f17D8;
    bp->p10 = src->f17DC;
    bp->p14 = src->f17E0;
    bp->p18 = src->f17E4;
    bp->p1C = src->f17E8;
    bp->p1E = src->f17EA;
    bp->p20 = src->f17EC;
    bp->p24 = src->f17F0;
    bp->p28 = src->f17F4;
    bp->p2C = src->f17F8;
    bp->p2E = src->f17FA;
    bp->p30 = src->f17FC;
    bp->p32 = src->f17FE;
    bp->p34 = src->f1800;
    bp->p36 = src->f1802;
    bp->p38 = src->f1804;
    bp->p3A = src->f1806;
    bp->p3C = src->f1808;
    bp->p40 = src->f180C;
    bp->p44 = src->f1810;
    bp->p48 = src->f1814;
    bp->p4C = src->f1818;
    bp->p50 = src->f181C;
    bp->p54 = src->f1820;
    bp->p55 = src->f1821;
    bp->p56 = src->f1822;
    bp->p57 = src->f1823;
    bp->p58 = src->f1824;
    bp->p5C = src->f1828;
    bp->p60 = src->f182C;
    bp->p64 = src->f1830;
    bp->p68 = src->f1834;
    bp->p6C = src->f1838;
    bp->p70 = src->f183C;
    bp->p74 = src->f1840;

    src->f177C = dst->f2A0;
    src->f1780 = dst->f2A4;
    src->f1784 = dst->f2A8;
    src->f34D4 = dst->f2AC;
    src->f34D6 = dst->f2AE;
    src->f34D8 = dst->f2B0;
    src->f34DC = dst->f2B4;
    src->f34E0 = dst->f2B8;
    src->f34E4 = dst->f2BC;

    u8 flag = dst->f2C0 != 0;
    lbl_eu_80663E5D = flag;
    lbl_eu_8066476D = flag;
    func_8025ECE4(dst->kizuna, src->kizunaSrc);
}

inline static void restoreNameTable(CfNandSaveNameTable* names) {
    CfNandSaveNameEntry cur;
    rebuildTboxPool();
    CfNandNameRoot* root = getReslistC48();
    u8* rec = (u8*)names;
    u8* recF = (u8*)names + 4;
    for (s32 i = 0; i < names->count; i++) {
        CfNandNameNode* slots = root->slots;
        s32 n = 0;
        u32 off = 0;
        while (n < root->slotCount) {
            if (*(u32*)((u8*)slots + off) == 0) {
                break;
            }
            off += 0x24;
            n++;
        }
        CfNandNameNode* slot = (CfNandNameNode*)((u8*)slots + n * 0x24);
        u8* body = (u8*)slot + 8;
        CfNandSaveNameEntry* pCur = &cur;
        if (body != 0) {
            *(u32*)(body + 0) = *(u32*)(rec + 4);
            *(float*)(body + 4) = *(float*)(recF + 4);
            *(float*)(body + 8) = *(float*)(recF + 8);
            *(float*)(body + 12) = *(float*)(recF + 12);
            *(float*)(body + 16) = *(float*)(rec + 20);
            *(u32*)(body + 20) = *(u32*)(rec + 24);
            *(u16*)(body + 24) = *(u16*)(rec + 28);
            body[26] = rec[30];
            pCur->f04 = 0;
            body[27] = rec[31];
        }
        CfNandNameNode* head = root->mNext;
        slot->mNext = head;
        slot->mPrev = head->mPrev;
        head->mPrev->mNext = slot;
        head->mPrev = slot;
        rec += 0x1C;
        recF += 0x1C;
    }
}

// Apply a NAND save image back into live game state (load/teardown path).
extern "C" int func_8023D3D8(CfNandSaveImage* img) {
    (void)__alloca(0);
    CfNandSaveImage* image = img;
    int ok = 1;
    u8* live;
    CfNandWorkEntrySrc* src;
    CfNandWorkEntryDst* dst;
    u32 ver = image->version - 0x70001;
    if (ver <= 1) {
        int valid = func_8023CD9C(image);
        if (valid == 0) {
            return 0;
        }
        u32 flagLen = CtrlRemote_GetFixedSize1214();
        memcpy(CtrlRemote_GetSharedBufPtr(), image->flagData, flagLen);
        lbl_eu_80664774 = image->slot[0x66];
        live = CtrlRemote_GetFileEventIds();
        memcpy(live, image->workHead, 0x41F0);
        src = (CfNandWorkEntrySrc*)(live + 0x7FC4);
        dst = &image->workEntry[1];
        for (u32 i = 1; i < 14; i++) {
            expandWorkEntry(src, dst);
            func_8009EF9C(src, 0);
            src++;
            dst++;
        }
        u8* q = image->workHead + 0x44B1;
        if (((u32)q & 1) != 0) {
            q = image->workHead + 0x44B2;
        }
        u16* ev = (u16*)q;
        int n = 0;
        u16 evCur = ev[0];
        while (n < evCur && n < 16) {
            queueEventId__Q22cf13CfGameManagerFv(evCur);
            ev++;
            n++;
            evCur = ev[0];
        }
        CfT_FrameTimerSet(&image->progress.field00);
        CfT_PlayClockReset(image->progress.f04);
        lbl_eu_80661AF4 = image->progress.f08;
        lbl_eu_80661AF6 = image->progress.f0A;
        CfNandPartySnapshot* snapDst = (CfNandPartySnapshot*)lbl_eu_80576CC0;
        *snapDst = image->snapshot;
        memcpy(lbl_eu_806641B8, image->itemBlob, 0x12120);
        cfCam_storeFollowD(&image->camBlock, image->camBlock.f0C);
        loadReloadInfo(&image->wthrBlock);
        fadeOutGameEffects__Q22cf13CfGameManagerFv();
        func_80207D2C(image->mineRegion);
        restoreNameTable(&image->names);
        if (valid != 0) {
            updateConfig__FPUc(image->optdBlob, 1);
        }
        u32 cnt = CtrlRemote_TouchBitByArg(0x3F);
        if ((cnt & 0xFFFF) == 0) {
            cnt = image->wthrBlock.f0E;
        }
        if ((cnt & 0xFFFF) != 0) {
            setEventCounterA__Q22cf13CfGameManagerFv(cnt);
        }
        return ok;
    }

    CfNandSaveImageV1* v1 = (CfNandSaveImageV1*)image;
    if ((mtl::MemManager::calculateCrc(v1->flagData, CtrlRemote_GetFixedSize1214()) & 0xFFFF) != v1->flagTag.crc) {
        ok = 0;
    }
    if ((mtl::MemManager::calculateCrc(v1->workHead, 0x6C28) & 0xFFFF) != v1->gameTag.crc) {
        ok = 0;
    } else if (v1->gameTag.length != 0x6C50) {
        ok = 0;
    }
    if ((mtl::MemManager::calculateCrc((const u8*)&v1->progress, 0xC) & 0xFFFF) != v1->timeTag.crc) {
        ok = 0;
    } else if (v1->timeTag.length != 0x30) {
        ok = 0;
    }
    if ((mtl::MemManager::calculateCrc((const u8*)&v1->snapshot, 0x34) & 0xFFFF) != v1->partyTag.crc) {
        ok = 0;
    } else if (v1->partyTag.length != 0x50) {
        ok = 0;
    }
    if ((mtl::MemManager::calculateCrc((const u8*)&v1->camBlock, 0x10) & 0xFFFF) != v1->camTag.crc) {
        ok = 0;
    } else if (v1->camTag.length != 0x30) {
        ok = 0;
    }
    if ((mtl::MemManager::calculateCrc(v1->itemBlob, 0x157D0) & 0xFFFF) != v1->itemTag.crc) {
        ok = 0;
    } else if (v1->itemTag.length != 0x157F0) {
        ok = 0;
    }
    if ((mtl::MemManager::calculateCrc((const u8*)&v1->wthrBlock, 0x10) & 0xFFFF) != v1->wthrTag.crc) {
        ok = 0;
    } else if (v1->wthrTag.length != 0x30) {
        ok = 0;
    }
    if ((mtl::MemManager::calculateCrc((const u8*)&v1->sndArea, 0x10) & 0xFFFF) != v1->sndTag.crc) {
        ok = 0;
    } else if (v1->sndTag.length != 0x30) {
        ok = 0;
    }
    if ((mtl::MemManager::calculateCrc(v1->mineRegion, 0x384) & 0xFFFF) != v1->mineTag.crc) {
        ok = 0;
    } else if (v1->mineTag.length != 0x3B0) {
        ok = 0;
    }
    if ((mtl::MemManager::calculateCrc((const u8*)&v1->names, 0x234) & 0xFFFF) != v1->tboxTag.crc) {
        ok = 0;
    } else if (v1->tboxTag.length != 0x410) {
        ok = 0;
    }
    if ((mtl::MemManager::calculateCrc(v1->optdBlob, 0x40) & 0xFFFF) != v1->optdTag.crc) {
        ok = 0;
    } else if (v1->optdTag.length != 0x50) {
        ok = 0;
    }

    u32 flagLen = CtrlRemote_GetFixedSize1214();
    memcpy(CtrlRemote_GetSharedBufPtr(), v1->flagData, flagLen);
    live = CtrlRemote_GetFileEventIds();
    memcpy(live, v1->workHead, 0x41F0);
    src = (CfNandWorkEntrySrc*)(live + 0x7FC4);
    dst = &v1->workEntry[1];
    for (u32 i = 1; i < 14; i++) {
        expandWorkEntry(src, dst);
        src++;
        dst++;
    }
    CfT_FrameTimerSet(&v1->progress.field00);
    CfT_PlayClockReset(v1->progress.f04);
    lbl_eu_80661AF4 = v1->progress.f08;
    lbl_eu_80661AF6 = v1->progress.f0A;
    CfNandPartySnapshot* snapDst = (CfNandPartySnapshot*)lbl_eu_80576CC0;
    *snapDst = v1->snapshot;

    u8* item = (u8*)lbl_eu_806641B8;
    u8* blob = v1->itemBlob;
    memset(item, 0, 0x12120);
    memcpy(item + 0x0000, blob + 0x0000, 0x211C);
    memcpy(item + 0x211C, blob + 0x2B44, 0x211C);
    memcpy(item + 0x4238, blob + 0x5688, 0x211C);
    memcpy(item + 0x6354, blob + 0x81CC, 0x211C);
    memcpy(item + 0x8470, blob + 0xAD10, 0x211C);
    memcpy(item + 0xA58C, blob + 0xD854, 0x211C);
    memcpy(item + 0xC6A8, blob + 0x10398, 0x20D0);
    memcpy(item + 0xE778, blob + 0x12468, 0x12C0);
    memcpy(item + 0xFA38, blob + 0x13728, 0x780);
    memcpy(item + 0x101B8, blob + 0x13EA8, 0x960);
    memcpy(item + 0x10B18, blob + 0x14808, 0x960);
    memcpy(item + 0x11478, blob + 0x15168, 0x640);
    memcpy(item + 0x120EC, blob + 0x157AC, 0x1C);
    *(u32*)(item + 0x120E8) = *(u32*)(blob + 0x157A8);
    *(u32*)(item + 0x12108) = *(u32*)(blob + 0x157C8);
    *(u32*)(item + 0x1210C) = *(u32*)(blob + 0x157CC);
    *(u32*)(item + 0x12110) = 0;
    *(u16*)(item + 0x12114) = (u16)*(u32*)(blob + 0x157CC);
    *(u16*)(item + 0x12116) = (u16)*(u32*)(blob + 0x157CC);
    *(u16*)(item + 0x12118) = (u16)*(u32*)(blob + 0x157CC);
    *(u16*)(item + 0x1211A) = (u16)*(u32*)(blob + 0x157CC);
    *(u16*)(item + 0x1211C) = (u16)*(u32*)(blob + 0x157CC);

    cfCam_storeFollowD(&v1->camBlock, v1->camBlock.f0C);
    loadReloadInfo(&v1->wthrBlock);
    setMasterVolume__Fff(v1->sndArea.f00, lbl_eu_806686E0);
    MenuSnd_SetSeVolPush_95F4(v1->sndArea.f04);
    MenuSnd_SetMasterAndPush_9510(v1->sndArea.f08);
    func_80207D2C(v1->mineRegion);
    restoreNameTable(&v1->names);
    if (ok != 0) {
        updateConfig__FPUc(v1->optdBlob, 1);
    }
    return ok;
}

extern "C" void __dl__FPv(void*);

// Retail US 0x8024058C: apply the +0x5C save image (func_8023D3D8), then
// operator-delete it. The in-progress flag at lbl_eu_80664772 is armed
// around the apply so CSysWinScenarioLog can poll the teardown.
extern "C" int __dt__8023E448() {
    u8* none = 0;
    u8* img = *(u8**)(lbl_eu_80664768 + 0x5C);
    if (img == none) {
        img = none;
    }
    if (img == none) {
        return 0;
    }
    u16 clear = 0;
    lbl_eu_80664772 = 1;
    int result = func_8023D3D8((CfNandSaveImage*)img);
    lbl_eu_80664772 = clear;
    u8* still = *(u8**)(lbl_eu_80664768 + 0x5C);
    if (still != 0) {
        __dl__FPv(still);
        *(u8**)(lbl_eu_80664768 + 0x5C) = (u8*)(u32)clear;
    }
    return result;
}

// Local GX/VI surface so this data-first TU does not include CDeviceVI.hpp
// (IWorkEvent / 10322). Offsets match GXRenderModeObj (fbWidth@+4, efbHeight@+6).
struct NandGXRenderMode {
    u32 viTVmode;
    u16 fbWidth;
    u16 efbHeight;
};
class CDeviceVI {
public:
    static NandGXRenderMode* getRenderModeObj();
};
extern "C" {
void GXSetTexCopySrc(u16 x, u16 y, u16 w, u16 h);
void GXSetTexCopyDst(u16 w, u16 h, u32 fmt, u8 mipmap);
void GXCopyTex(void* dest, u8 clear);
void GXPixModeSync(void);
void GXInvalidateTexAll(void);
u32 GXGetTexBufferSize(u16 w, u16 h, u32 fmt, u8 mipmap, u8 max_lod);
void DCFlushRange(void* addr, u32 nBytes);
}

// While the counter is armed (1), copy the EFB into the shared texture
// buffer and flush it, then disarm. Body recovered from CfNandManager.ctx.c.
extern "C" void cbRenderBefore__Q22cf13CfNandManagerFv() {
    if (lbl_eu_80664770 == 1) {
        u16 h1;
        u16 h2;
        u32 size1;
        h1 = CDeviceVI::getRenderModeObj()->efbHeight;
        GXSetTexCopySrc(0, 0, CDeviceVI::getRenderModeObj()->fbWidth, h1);
        h2 = CDeviceVI::getRenderModeObj()->efbHeight;
        GXSetTexCopyDst(CDeviceVI::getRenderModeObj()->fbWidth, h2, 6, 0);
        GXCopyTex(lbl_eu_80664780, 0);
        GXPixModeSync();
        GXInvalidateTexAll();
        size1 = GXGetTexBufferSize(0xa4, 0x74, 4, 0, 0);
        h1 = CDeviceVI::getRenderModeObj()->efbHeight;
        u32 size2 = GXGetTexBufferSize(CDeviceVI::getRenderModeObj()->fbWidth, h1, 6, 0, 0);
        DCFlushRange(lbl_eu_8066477C, size1 + size2);
        lbl_eu_80664770 = 2;
    }
}
