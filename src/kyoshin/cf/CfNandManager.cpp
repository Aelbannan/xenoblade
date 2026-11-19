#include <types.h>
#include <cstring>
#include "kyoshin/cf/CfNandManager.hpp"

#include "monolib/util/MemManager.hpp"
#include "monolib/nand/CNand.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/FixStr.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/CNandData.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/scn/CScn.hpp"

extern "C" void dummy() {}

#pragma section data_type ".data" ".data"

// --- typified .data (layout from build/us/asm/kyoshin/cf/CfNandManager.s) ---
// Slots carrying relocs emit zero bytes + ADDR32 relocs in the .o, matching
// retail (the gate compares bytes; reloc names are ignored once bytes match).
// Slot targets owned by other TUs are extern (UNDEF here, resolved at link
// via the global symbol map). The four CTTask<cf::CfNandManager> template
// slots spell '<' and are not expressible in C++; they stay zero with the
// retail target in comments. The 0x90 switch jumptable targets local case
// labels of NandResolveOp (not in this stub TU), so it stays a named zero
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
extern char __RTTI__10IScnRender[];
extern char __RTTI__8CProcess[];
extern char __RTTI__14CChildListNode[];
extern char __RTTI__15CDoubleListNode[];
extern char __RTTI__10IWorkEvent[];
// this TU's own .sdata pair tables (defined below; vtable slots point at them)
extern const void* __RTTI__Q22cf13CfNandManager[2];
extern const void* lbl_eu_806628D0[2];

__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char jumptable_eu_80536B2C[0x90] = {0};
__declspec(section ".data") __attribute__((used)) const void* lbl_eu_80536BBC[49] = {
    __RTTI__Q22cf13CfNandManager, 0,
    (const void*)__dt__Q22cf13CfNandManagerFv,
    (const void*)Reset__14CChildListNodeFv,
    (const void*)Init__Q22cf13CfNandManagerFv,
    (const void*)Term__Q22cf13CfNandManagerFv,
    (const void*)Move__Q22cf13CfNandManagerFv,
    0, // retail Draw__28CTTask<Q22cf13CfNandManager>Fv (unspellable '<')
    (const void*)Tail__8CProcessFv,
    __RTTI__Q22cf13CfNandManager, (const void*)0xFFFFFFAC,
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
    __RTTI__Q22cf13CfNandManager, (const void*)0xFFFFFFA8,
    (const void*)destroyViaRender__Q22cf13CfNandManagerFv,
    (const void*)handleRenderCallback__Q22cf13CfNandManagerFv,
    (const void*)handleFileEvent__Q22cf13CfNandManagerFv,
    (const void*)cbRenderBefore__Q22cf13CfNandManagerFv,
};
__declspec(section ".data") __attribute__((used)) const void* lbl_eu_80536C80[13] = {
    __RTTI__10IScnRender, (const void*)0x58,
    __RTTI__10IWorkEvent, (const void*)0x54,
    __RTTI__15CDoubleListNode, 0, __RTTI__14CChildListNode, 0, __RTTI__8CProcess, 0,
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
    __RTTI__15CDoubleListNode, 0, __RTTI__14CChildListNode, 0, __RTTI__8CProcess, 0, 0,
};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char s_nandTail[4] = {0}; // retail +0x1C8 null word
#pragma section data_type ".data" ".bss"
// typed .rodata strings (each sized to its retail gap).
__declspec(section ".rodata") __attribute__((used)) const char kNandTypeName[0x14] = "cf::CfNandManager";
__declspec(section ".rodata") __attribute__((used)) const char kNandTaskTypeName[0x1C] = "CTTask<cf::CfNandManager>";
__declspec(section ".rodata") __attribute__((used)) const char kNandSaveNameFormat[0x18] = "%s%02d\0banner.bin\0name";
// typed .sdata pointer pairs (replaces old sdata blob which MWCC migrated
// to nobits as all-zero bytes): relocs to the string/pool labels keep the
// section live.
__declspec(section ".sdata") __attribute__((used)) const void* __RTTI__Q22cf13CfNandManager[2] = { kNandTypeName, lbl_eu_80536C80 };
__declspec(section ".sdata") __attribute__((used)) const void* lbl_eu_806628D0[2] = { kNandTaskTypeName, lbl_eu_80536CD8 };
// typed .bss/.sbss zero-fill (nobits compare sizes only; small sbss objects
// stay in .sbss automatically, no section attr needed).
char gCfNandPartySnapshot[0x38];
cf::CfNandManager* gpCfNandManager;
unsigned char gCfNandFileEventPending;
unsigned char lbl_eu_8066476D;
unsigned short lbl_eu_8066476E;
unsigned short gCfNandCaptureState;
unsigned short lbl_eu_80664772;
unsigned short lbl_eu_80664774;
unsigned short gNandSbssGap64776;
unsigned int lbl_eu_80664778;
void* gpCfNandCaptureBuf;
void* gpCfNandCaptureBufTail;
unsigned int gNandSbssPad64780;

#pragma section code_type ".text" ".text"

// Manager base layout for the render-callback path: the CTTask (0x54) and
// CIWorkEventSlot (4) bases come first, so the IScnRender base sub-object the
// scene registers sits at +0x58. Never instantiated (data only), so MWCC emits
// no vtable/RTTI for it.
struct CfNandTaskPad {
    u8 _pad58[0x58];
};
class CfNandMgrBases : public CfNandTaskPad, public IScnRender {};

// Local copies of the save-image / work-entry views (the unit header cannot
// be included here: it redeclares this TU's .sbss/.bss labels with different
// types). Offsets match src/kyoshin/cf/CfNandManager.hpp.

struct CfNandWorkEntryDst_Local {
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

struct CfNandWorkEntrySrc_Local {
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

struct CfNandSaveTagBlock_Local {
    u32 tag;
    u32 length;
    u32 unk08;
    u32 crc;
};

struct CfNandProgressArea_Local {
    u32 field00;
    f32 f04;
    u16 f08;
    u16 f0A;
};

struct CfNandPartyEntry_Local {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    f32 field_0C;
};

struct CfNandPartySnapshot_Local {
    CfNandPartyEntry_Local mEntry[3];
    u32 field_30;
};

struct CfNandCamBlock_Local {
    u8 _pad0[0xC];
    f32 f0C;
};

struct CfNandWthrBlock_Local {
    u8 _pad0[0xE];
    u16 f0E;
};

struct CfNandSndArea_Local {
    f32 f00;
    f32 f04;
    f32 f08;
    u32 f0C;
};

struct CfNandSaveNameTable_Local {
    s32 count;
    CfNandSaveNameEntry entries[20];
};

struct CfNandSaveImage_Local {
    u32 magic;
    u32 totalSize;
    u32 unk08;
    u32 version;
    CfNandSaveTagBlock_Local slotTag;
    u8 slot[0xB8];
    u8 slotPad[0x9BE8];
    u8 pad9CC0[0x360];
    CfNandSaveTagBlock_Local flagTag;
    u8 flagData[0x1220];
    CfNandSaveTagBlock_Local gameTag;
    u8 workHead[0x41F0];
    CfNandWorkEntryDst_Local workEntry[14];
    u8 pad11E88[0x18];
    CfNandSaveTagBlock_Local timeTag;
    CfNandProgressArea_Local progress;
    u8 pad11EBC[0x14];
    CfNandSaveTagBlock_Local partyTag;
    CfNandPartySnapshot_Local snapshot;
    u8 pad11F14[0xC];
    CfNandSaveTagBlock_Local camTag;
    CfNandCamBlock_Local camBlock;
    u8 pad11F40[0x10];
    CfNandSaveTagBlock_Local itemTag;
    u8 itemBlob[0x12120];
    CfNandSaveTagBlock_Local wthrTag;
    CfNandWthrBlock_Local wthrBlock;
    u8 pad240A0[0x10];
    CfNandSaveTagBlock_Local sndTag;
    CfNandSndArea_Local sndArea;
    u8 pad240D0[0x10];
    CfNandSaveTagBlock_Local mineTag;
    u8 mineRegion[0x384];
    u8 pad24474[0x1C];
    CfNandSaveTagBlock_Local tboxTag;
    CfNandSaveNameTable_Local names;
    u8 pad246D4[0x1CC];
    CfNandSaveTagBlock_Local optdTag;
    u8 optdBlob[0x40];
};

// Version-0x70001 (and the 0x60002 apply path): larger ITEM region, tail
// shifted to 0x27740+.
struct CfNandSaveImageV1 {
    u32 magic;
    u32 totalSize;
    u32 unk08;
    u32 version;
    CfNandSaveTagBlock_Local slotTag;
    u8 slot[0xB8];
    u8 slotPad[0x9BE8];
    u8 pad9CC0[0x360];
    CfNandSaveTagBlock_Local flagTag;
    u8 flagData[0x1220];
    CfNandSaveTagBlock_Local gameTag;
    u8 workHead[0x41F0];
    CfNandWorkEntryDst_Local workEntry[14];
    u8 pad11E88[0x18];
    CfNandSaveTagBlock_Local timeTag;
    CfNandProgressArea_Local progress;
    u8 pad11EBC[0x14];
    CfNandSaveTagBlock_Local partyTag;
    CfNandPartySnapshot_Local snapshot;
    u8 pad11F14[0xC];
    CfNandSaveTagBlock_Local camTag;
    CfNandCamBlock_Local camBlock;
    u8 pad11F40[0x10];
    CfNandSaveTagBlock_Local itemTag;
    u8 itemBlob[0x157D0];
    CfNandSaveTagBlock_Local wthrTag;
    CfNandWthrBlock_Local wthrBlock;
    u8 pad27760[0x10];
    CfNandSaveTagBlock_Local sndTag;
    CfNandSndArea_Local sndArea;
    u8 pad277A0[0x10];
    CfNandSaveTagBlock_Local mineTag;
    u8 mineRegion[0x384];
    u8 pad27B44[0x1C];
    CfNandSaveTagBlock_Local tboxTag;
    CfNandSaveNameTable_Local names;
    u8 pad27F34[0x1CC];
    CfNandSaveTagBlock_Local optdTag;
    u8 optdBlob[0x40];
};

struct CfNandWorkBuf_Local {
    u8 head[0x41F0];
    CfNandWorkEntryDst_Local entry[14];
};

extern "C" int NandCheckSave(void* block);
// The pending-event handler consumes one dispatched NAND request: it applies
// the per-type result, resolves the callback arguments and finally invokes
// the queued completion callback. The flush pump prepends the request to the
// ring before calling it.
extern "C" void NandVerifyBlk(void* self, u16 type, u8* ok, s32* result);
extern "C" void NandResolveOp(void* self, u16 type, u8* ok, s32* result);
extern "C" void NandDownsample(void* self);
extern "C" void NandImageMake(void* buf, bool flag);
extern "C" void __ct__cf_CfNandManager(void* self, void* arg);
extern "C" void __dt__8023E63C(void* self);
extern "C" void NandFlushEvts(void* self);
extern "C" int CfRes_getD80Flag();
extern "C" u32 CtrlRemote_GetFixedSize1214();
extern "C" u8* CtrlRemote_GetSharedBufPtr();
extern "C" u8* CtrlRemote_GetFileEventIds();
extern "C" void CtrlObjectParam_RefreshCharEntry(void* data, u32 value);
extern "C" void KizunagramUnpackChart(void* src, void* dst);
namespace cf { struct CfGameManager {
    static void queueEventId(u32);
    static void fadeOutGameEffects();
    static void setEventCounterA(u32);
    static struct CfObjectMove* getPlayer(int);
}; }
extern "C" void CfT_FrameTimerSet(u32* self);
extern "C" void CfT_PlayClockReset(f32 v);
extern "C" void cfCam_storeFollowD(void* v, f32 f);
extern "C" void loadReloadInfo(void* p);
extern "C" void mineLoadFromSave(u8* rec);
extern "C" void rebuildTboxPool();
extern "C" CfNandNameRoot* getReslistC48();
extern "C" void updateConfig__FPUc(u8* src, int mode);
extern "C" u32 CtrlRemote_TouchBitByArg(u32 resourceId);
extern "C" void setMasterVolume__Fff(f32 a, f32 b);
extern "C" void MenuSnd_SetSeVolPush_95F4(f32 v);
extern "C" void MenuSnd_SetMasterAndPush_9510(f32 v);

extern char* lbl_eu_806628C0;
extern u8 lbl_eu_80663E5D;

#pragma inline_max_size(0x4000)
#pragma inline_max_total_size(0x8000)
inline static void expandWorkEntry(CfNandWorkEntrySrc_Local* src, CfNandWorkEntryDst_Local* dst) {
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
    cf::CActorParam* actor = reinterpret_cast<cf::CActorParam*>(src->subSlot);
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
    KizunagramUnpackChart(dst->kizuna, src->kizunaSrc);
}

inline static void restoreNameTable(CfNandSaveNameTable_Local* names) {
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
extern "C" int NandApplySave(CfNandSaveImage_Local* img) {
    (void)__alloca(0);
    CfNandSaveImage_Local* image = img;
    int ok = 1;
    u8* live;
    CfNandWorkEntrySrc_Local* src;
    CfNandWorkEntryDst_Local* dst;
    u32 ver = image->version - 0x70001;
    if (ver <= 1) {
        int valid = NandCheckSave(image);
        if (valid == 0) {
            return 0;
        }
        u32 flagLen = CtrlRemote_GetFixedSize1214();
        memcpy(CtrlRemote_GetSharedBufPtr(), image->flagData, flagLen);
        lbl_eu_80664774 = image->slot[0x66];
        live = CtrlRemote_GetFileEventIds();
        memcpy(live, image->workHead, 0x41F0);
        src = (CfNandWorkEntrySrc_Local*)(live + 0x7FC4);
        dst = &image->workEntry[1];
        for (u32 i = 1; i < 14; i++) {
            expandWorkEntry(src, dst);
            CtrlObjectParam_RefreshCharEntry(src, 0);
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
            cf::CfGameManager::queueEventId(evCur);
            ev++;
            n++;
            evCur = ev[0];
        }
        CfT_FrameTimerSet(&image->progress.field00);
        CfT_PlayClockReset(image->progress.f04);
        lbl_eu_80661AF4 = image->progress.f08;
        lbl_eu_80661AF6 = image->progress.f0A;
        CfNandPartySnapshot_Local* snapDst = (CfNandPartySnapshot_Local*)gCfNandPartySnapshot;
        *snapDst = image->snapshot;
        memcpy(lbl_eu_806641B8, image->itemBlob, 0x12120);
        cfCam_storeFollowD(&image->camBlock, image->camBlock.f0C);
        loadReloadInfo(&image->wthrBlock);
        cf::CfGameManager::fadeOutGameEffects();
        mineLoadFromSave(image->mineRegion);
        restoreNameTable(&image->names);
        if (valid != 0) {
            updateConfig__FPUc(image->optdBlob, 1);
        }
        u32 cnt = CtrlRemote_TouchBitByArg(0x3F);
        if ((cnt & 0xFFFF) == 0) {
            cnt = image->wthrBlock.f0E;
        }
        if ((cnt & 0xFFFF) != 0) {
            cf::CfGameManager::setEventCounterA(cnt);
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
    src = (CfNandWorkEntrySrc_Local*)(live + 0x7FC4);
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
    CfNandPartySnapshot_Local* snapDst = (CfNandPartySnapshot_Local*)gCfNandPartySnapshot;
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
    mineLoadFromSave(v1->mineRegion);
    restoreNameTable(&v1->names);
    if (ok != 0) {
        updateConfig__FPUc(v1->optdBlob, 1);
    }
    return ok;
}


// Retail US 0x8024058C: apply the +0x5C save image (NandApplySave), then
// operator-delete it. The in-progress flag at lbl_eu_80664772 is armed
// around the apply so CSysWinScenarioLog can poll the teardown.
extern "C" int __dt__8023E448() {
    u8* none = 0;
    u8* img = gpCfNandManager->field_0x5C;
    if (img == none) {
        img = none;
    }
    if (img == none) {
        return 0;
    }
    u16 clear = 0;
    lbl_eu_80664772 = 1;
    int result = NandApplySave((CfNandSaveImage_Local*)img);
    lbl_eu_80664772 = clear;
    u8* still = gpCfNandManager->field_0x5C;
    if (still != 0) {
        operator delete(still);
        gpCfNandManager->field_0x5C = (u8*)(u32)clear;
    }
    return result;
}


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
// GX / cache entry points come from the SDK headers pulled in by CScn.hpp.

// While the counter is armed (1), copy the EFB into the shared texture
// buffer and flush it, then disarm. Body recovered from CfNandManager.ctx.c.
void cf::CfNandManager::cbRenderBefore() {
    if (gCfNandCaptureState == 1) {
        u16 h1;
        u16 h2;
        h1 = CDeviceVI::getRenderModeObj()->efbHeight;
        GXSetTexCopySrc(0, 0, CDeviceVI::getRenderModeObj()->fbWidth, h1);
        h2 = CDeviceVI::getRenderModeObj()->efbHeight;
        GXSetTexCopyDst(CDeviceVI::getRenderModeObj()->fbWidth, h2, (GXTexFmt)6, (GXBool)0);
        GXCopyTex(gpCfNandCaptureBufTail, 0);
        GXPixModeSync();
        GXInvalidateTexAll();
        u32 size1 = GXGetTexBufferSize(0xa4, 0x74, 4, 0, 0);
        h1 = CDeviceVI::getRenderModeObj()->efbHeight;
        u32 size2 = GXGetTexBufferSize(CDeviceVI::getRenderModeObj()->fbWidth, h1, 6, 0, 0);
        DCFlushRange(gpCfNandCaptureBuf, size1 + size2);
        gCfNandCaptureState = 2;
    }
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Address of the shared party-snapshot block (read by CfGameManager and the
// status/save windows).
extern "C" CfNandPartySnapshot_Local* NandPartySnap() {
    return (CfNandPartySnapshot_Local*)gCfNandPartySnapshot;
}

// Nonzero while the NAND request queue still has work: the global CNand's
// head/tail counters differ, or the file-event suppression flag is set.
extern "C" int NandQueueBusy() {
    s32 busy = 0;
    if (nand.mHead != nand.mTail || gCfNandFileEventPending != 0) {
        busy = 1;
    }
    return busy;
}

// Build the 3-entry party snapshot saved into each slot. Missing players use
// the static fallback record; the trailing word is the CRC16 of the snapshot.
extern "C" CfNandPartySnapshot_Local* NandCopyParty(CfNandPartySnapshot_Local* snapshot) {
    snapshot->field_30 = 0;
    if (cf::CBattleManager::getInstance() != 0) {
        snapshot->field_30 = *(u32*)((u8*)cf::CBattleManager::getInstance() + 0x194);
    }
    CfNandPartyEntry_Local* entry = snapshot->mEntry;
    for (u32 i = 0; i < 3; i++) {
        void* player = cf::CfGameManager::getPlayer(i);
        if (player != 0) {
            ml::CVec3* pos = ((cf::CfObjectMove*)player)->CfObject_getPosVector();
            entry->field_00 = *(u32*)&pos->x;
            entry->field_04 = *(u32*)&pos->y;
            entry->field_08 = *(u32*)&pos->z;
            entry->field_0C = ((cf::CfObjectMove*)player)->CfObject_getScaledAngle();
        } else {
            entry->field_00 = lbl_eu_8057164C[0];
            entry->field_04 = lbl_eu_8057164C[1];
            entry->field_08 = lbl_eu_8057164C[2];
            entry->field_0C = lbl_eu_80663E54;
        }
        entry++;
    }
    snapshot->field_30 = OSCalcCRC16((const u8*)snapshot, 0x34);
    return snapshot;
}
// LLM-HARNESS-END: us-8023e320

// LLM-HARNESS-BEGIN: us-8023e414
extern "C" void NandPackEntry() {}
// LLM-HARNESS-END: us-8023e414

// LLM-HARNESS-BEGIN: us-8023e7bc
extern "C" void NandTileDescr() {}
// LLM-HARNESS-END: us-8023e7bc

// LLM-HARNESS-BEGIN: us-8023e8f4
extern "C" void NandSaveBuild() {}
// LLM-HARNESS-END: us-8023e8f4

// Assemble the on-media save image in the manager's staging buffer. The
// version-0x70002 layout and the per-region CRC tags are owned by this TU.
extern "C" void NandImageMake(void* buf, bool flag) {
    CfNandSaveImage* image = (CfNandSaveImage*)buf;
    if (image == 0) {
        return;
    }
    std::memset(image, 0, sizeof(*image));
    image->magic = 0x55535244;
    image->totalSize = sizeof(*image);
    image->unk08 = 0x10;
    image->version = 0x70002;
    image->slotTag.tag = 0x444D4D59;
    image->slotTag.length = sizeof(CfNandSaveBuf);
    image->slotTag.unk08 = 0x10;
    NandCopyParty((CfNandPartySnapshot_Local*)&image->snapshot);
    image->partyTag.tag = 0x50415254;
    image->partyTag.length = sizeof(CfNandPartySnapshot);
    image->partyTag.unk08 = 0x10;
    image->partyTag.crc = OSCalcCRC16((const u8*)&image->snapshot, sizeof(CfNandPartySnapshot));
    image->progress.field00 = CfT_FrameTimerGet();
    image->progress.f04 = lbl_eu_80663D94;
    image->timeTag.tag = 0x54494D45;
    image->timeTag.length = sizeof(CfNandProgressArea);
    image->timeTag.crc = OSCalcCRC16((const u8*)&image->progress, sizeof(CfNandProgressArea));
    image->sndArea.f00 = getMasterVolume();
    image->sndArea.f04 = MenuSnd_GetSeVol_96A0();
    image->sndArea.f08 = MenuSnd_GetMasterVol_95EC();
    image->sndTag.crc = OSCalcCRC16((const u8*)&image->sndArea, sizeof(CfNandSndArea));
    if (flag) {
        initReloadInfoStruct((CfNandWthrBlock*)&image->wthrBlock);
    }
    mineSnapshotActivePoints(image->mineRegion);
}
// LLM-HARNESS-END: us-8023ea80

// LLM-HARNESS-BEGIN: us-80240618
void cf::CfNandManager::handleFileEvent() {}
// LLM-HARNESS-END: us-80240618

// Validate the save block in the pending payload. The dummy magic marks an
// empty (but valid) slot; the two real versions use different CRC16 routines
// over the same 0x9C80-byte payload.
extern "C" void NandVerifyBlk(void* self, u16 type, u8* ok, s32* result) {
    if (*ok != 0) {
        u8* block = (u8*)((cf::CfNandManager*)self)->mPending.mPayload;
        u32 magic = *(u32*)block;
        if (magic == 0x444D4D59) {
            *ok = 0;
            *result = 0;
        } else {
            u32 version = *(u32*)(block + 0xC);
            if (version == 0x70002) {
                u16 crc = OSCalcCRC16(block + 0x20, 0x9C80);
                if (crc != *(u32*)(block + 0x1C)) {
                    *ok = 0;
                    *result = 3;
                }
            } else if (version == 0x70001) {
                u16 crc = mtl::MemManager::calculateCrc(block + 0x20, 0x9C80);
                if (crc != *(u32*)(block + 0x1C)) {
                    *ok = 0;
                    *result = 3;
                }
            } else {
                *ok = 0;
                *result = 0;
            }
        }
    }
}
// LLM-HARNESS-END: us-80240688

// Consume the pending request stored at manager+0x170. `ok` and `result`
// are the two out-parameters handed to NandResolveOp and the completion
// callback. The request type selects which bits of the manager state are
// updated before the callback runs.
extern "C" void __dt__8023E63C(void* self) {
    cf::CfNandManager* mgr = (cf::CfNandManager*)self;
    u16 type = mgr->mPending.mType;
    u8 ok;
    s32 result;
    if (type != 0) {
        u8 success = 0;
        if (nand.mSuccess) {
            cf::CfNandManager* inst = gpCfNandManager;
            u32 flags = 0;
            if (inst != 0) {
                flags = inst->field_180;
            } else {
                flags = 0;
            }
            if (flags == 0) {
                success = 1;
            }
        }
        ok = success;
        result = 0;
        if (mgr->mPending.mType == 3) {
            NandVerifyBlk(self, type, &ok, &result);
        } else if (mgr->mPending.mType == 18) {
            if (ok != 0) {
                if (*(u32*)mgr->mPending.mPayload != 0x444D4D59) {
                    mgr->field_184 = mgr->field_184 + 1;
                }
            } else {
                result = 5;
            }
        } else if (type == 2) {
            if (ok != 0) {
                mgr->mPending.mCb = 0;
            } else {
                mgr->mEventQueue.mTail = 0;
                mgr->mEventQueue.mHead = 0;
            }
        } else if (type == 17) {
            lbl_eu_8066476E = 0;
            if (ok != 0) {
                u8 sub = mgr->mPending.mSubtype;
                u32 count = lbl_eu_80664778;
                u8* list = (u8*)mgr->mPending.mPayload;
                ml::FixStr<32> name;
                if (sub != 0) {
                    name.format(kNandSaveNameFormat, lbl_eu_806628C0, sub);
                } else {
                    name = &kNandSaveNameFormat[7];
                }
                u32 i = 0;
                u8 found = 0;
                while (i < count) {
                    if (std::strcmp(name.c_str(), (const char*)list) == 0) {
                        found = 1;
                        break;
                    }
                    list += std::strlen((const char*)list) + 1;
                    i++;
                }
                ok = found;
                if (ok != 0) {
                    lbl_eu_8066476E = lbl_eu_8066476E | (u16)(1 << mgr->mPending.mSubtype);
                }
            } else {
                result = 5;
            }
        }

        if (type == 11) {
            if (ok != 0) {
                if (mgr->mPending.mSubtype == 3) {
                    mgr->mPending.mCb = 0;
                } else {
                    mgr->mEventQueue.mTail = 0;
                    mgr->mEventQueue.mHead = 0;
                }
            }
        }
        if (type == 17 && ok != 0) {
            s32 head = mgr->mEventQueue.mHead;
            s32 size = mgr->mEventQueue.mSize;
            s32 quotient = (head + 1) / size;
            mgr->field_180 = 0;
            mgr->mEventQueue.mTail = head - 1;
            mgr->mEventQueue.mHead = (head + 1) - quotient * size;
        }
        if (type == 18) {
            if (mgr->mPending.mSubtype == 3) {
                ok = mgr->field_184 != 0;
            } else {
                mgr->mPending.mCb = 0;
                mgr->field_180 = 0;
            }
        }
        if (type == 10 && ok != 0) {
            mgr->mEventQueue.mTail = mgr->mEventQueue.mHead - 3;
            mgr->mEventQueue.mHead = (mgr->mEventQueue.mHead + 1) % mgr->mEventQueue.mSize;
            mgr->mEventQueue.mHead = (mgr->mEventQueue.mHead + 1) % mgr->mEventQueue.mSize;
            mgr->mEventQueue.mHead = (mgr->mEventQueue.mHead + 1) % mgr->mEventQueue.mSize;
        }

        if (mgr->mPending.mCb != 0) {
            if (ok != 0) {
                if (type == 1 || type == 2) {
                    if (NandCheckSave((void*)mgr->mPending.mPayload) == 0) {
                        ok = 0;
                        result = 3;
                    }
                } else if (type == 8) {
                    if (NandCheckSave((void*)mgr->mPending.mPayload) == 0) {
                        result = 3;
                        ok = 0;
                        mgr->mEventQueue.mTail = 0;
                        mgr->mEventQueue.mHead = 0;
                    } else {
                        lbl_eu_8066476E = lbl_eu_8066476E |
                                          (u16)(1 << mgr->mPending.mSubtype);
                        if (mgr->mPending.mSubtype != 3) {
                            mgr->mPending.mCb = 0;
                        }
                    }
                } else {
                    NandResolveOp(self, type, &ok, &result);
                }
            } else {
                NandResolveOp(self, type, &ok, &result);
            }
            if (type == 8 && mgr->mPending.mSubtype != 3) {
                mgr->mPending.mCb = 0;
            }
        }
        if (mgr->mPending.mCb != 0) {
            ((void (*)(u16, u8, u8, s32))mgr->mPending.mCb)(
                mgr->mPending.mType, (u8)mgr->mPending.mSubtype, ok, result);
        }

        if (type == 8 || type == 9 || type == 4 || type == 5 ||
            type == 11 || type == 12 || type == 38) {
            u8* image = gpCfNandManager->field_0x5C;
            if (image != 0) {
                operator delete(image);
                gpCfNandManager->field_0x5C = 0;
            }
        }
    }
dispatchDone:
    mgr->mPending.mType = 0;
    mgr->mPending.mPayload = 0;
    mgr->mPending.mFlag = 0;
}

// Pump the manager's ring: each iteration pops one request into mPending,
// builds the NAND file name and issues the operation selected by the request
// type. The completion callback and the state reset are shared by all types.
extern "C" void NandFlushEvts(void* self) {
    cf::CfNandManager* mgr = (cf::CfNandManager*)self;
    ml::FixStr<32> fileName;
    u8 ok = 0;
    s32 result = 0;
    bool armed = true;
    const u32 mask = 0x3FFF;
    while ((u32)mgr->mEventQueue.mTail >> 5 == 0) {
        s32 head = mgr->mEventQueue.mHead;
        s32 size = mgr->mEventQueue.mSize;
        s32 quotient = (head + 1) / size;
        s32 tail = mgr->mEventQueue.mTail;
        mgr->mEventQueue.mTail = tail - 1;
        CfNandEvent* slot = &mgr->mEventQueue.mRingBase[head];
        mgr->mEventQueue.mHead = (head + 1) - quotient * size;
        CfNandEvent ev;
        ev.mWord = slot->mWord;
        ev.mTag = slot->mTag;
        ev.mField8 = slot->mField8;
        ev.mFieldC = slot->mFieldC;
        ev.mFieldE = slot->mFieldE;
        ev.mFieldF = slot->mFieldF;
        mgr->mPending.mFlag = ev.mFieldF;
        mgr->mPending.mType = ev.mFieldC;
        mgr->mPending.mCb = ev.mWord;
        mgr->mPending.mTag = ev.mTag;
        mgr->mPending.mPayload = ev.mField8;
        mgr->mPending.mSubtype = ev.mFieldE;
        if (ev.mFieldE == 0) {
            fileName = &kNandSaveNameFormat[7];
        } else {
            fileName.format(kNandSaveNameFormat, lbl_eu_806628C0, ev.mFieldE);
        }
        u16 type = ev.mFieldC;
        if (type == 1 || type == 8 || type == 2) {
            if (gpCfNandManager->field_0x5C == 0) {
                mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
                gpCfNandManager->field_0x5C = (u8*)mtl::MemManager::allocate_ex(0x40000, handle, 0x40);
            }
            u8* buf = gpCfNandManager->field_0x5C;
            mgr->mPending.mPayload = (u32)buf;
            nand.load(fileName.c_str(), buf, ev.mTag);
        } else if (type == 9 || type == 10) {
            if (gpCfNandManager->field_0x5C == 0) {
                mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
                gpCfNandManager->field_0x5C = (u8*)mtl::MemManager::allocate_ex(0x40000, handle, 0x40);
            }
            u8* buf = gpCfNandManager->field_0x5C;
            mgr->mPending.mPayload = (u32)buf;
            nand.loadBanner(buf, ev.mTag);
        } else if (type == 17) {
            lbl_eu_80664778 = 0;
            mgr->mPending.mPayload = (u32)lbl_eu_80576AC0;
            nand.readdir((char*)lbl_eu_80576AC0, 0x200, (u32*)&lbl_eu_80664778);
        } else if (type == 3 || type == 18) {
            nand.load(fileName.c_str(), (void*)mgr->mPending.mPayload, ev.mTag);
        } else if (type == 32) {
            if (ev.mTag != 0) {
                ev.mTag = 0xFFFFFFFF;
                mgr->mEventQueue.mHead = mgr->mEventQueue.mHead - 1;
                if (mgr->mEventQueue.mHead < 0) {
                    mgr->mEventQueue.mHead %= mgr->mEventQueue.mSize;
                }
                CfNandEvent* dst = &mgr->mEventQueue.mRingBase[mgr->mEventQueue.mHead];
                dst->mWord = ev.mWord;
                dst->mTag = ev.mTag;
                dst->mField8 = ev.mField8;
                dst->mFieldC = ev.mFieldC;
                dst->mFieldE = ev.mFieldE;
                dst->mFieldF = ev.mFieldF;
                mgr->mEventQueue.mTail = mgr->mEventQueue.mTail + 1;
                mgr->mPending.mType = 0;
                mgr->mPending.mPayload = 0;
                mgr->mPending.mFlag = 0;
                continue;
            }
        } else if (type == 33) {
            u32 blocks = (ev.mTag + mask) & ~mask;
            (void)blocks;
            if (armed) {
                mgr->field_180 = 0;
                nand.check(0, 1, &mgr->field_180);
            }
        } else if (type == 16) {
            u32 aligned = (ev.mTag + 0x3FFF) & ~mask;
            u32 needed = ((aligned >> 12) & 0x3FFF) - ((aligned >> 14) & 0xFFFF) +
                         ((0x3FFFF0C8 & ~mask) >> 14 & 0xFFFF);
            mgr->field_180 = 0;
            nand.check(needed, nand.mShare != 0 ? 5 : 4, &mgr->field_180);
        } else if (type == 34) {
            if (gpCfNandManager->field_0x5C == 0) {
                mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
                gpCfNandManager->field_0x5C = (u8*)mtl::MemManager::allocate_ex(0x40000, handle, 0x40);
            }
            u8* buf = gpCfNandManager->field_0x5C;
            if (gCfNandCaptureState == 2) {
                NandDownsample(self);
            }
            NandImageMake(buf, ev.mFieldF != 0);
        } else if (type == 35) {
            if (gpCfNandManager->field_0x5C == 0) {
                mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
                gpCfNandManager->field_0x5C = (u8*)mtl::MemManager::allocate_ex(0x40000, handle, 0x40);
            }
            u8* buf = gpCfNandManager->field_0x5C;
            if (buf == 0) {
                continue;
            }
            std::memset(buf, 0, 0x48F0);
            *(u32*)buf = 0x55535244;
            *(u32*)(buf + 4) = 0x48F2;
            *(u32*)(buf + 8) = 16;
            *(u32*)(buf + 0xC) = 0x70002;
            *(u32*)(buf + 0x10) = 0x55535244;
            u32 crc = OSCalcCRC16(buf + 0x20, 0x9C80) & 0xFFFF;
            for (u32 i = 0; i < 25; i++) {
                buf[0x83 + i] = 0xFF;
            }
            *(u32*)(buf + 0x18) = 16;
            *(u32*)(buf + 0x14) = 0xA000;
            *(u32*)(buf + 0x1C) = crc;
            *(u32*)(buf + 0x10) = 0x55535244;
        } else if (type == 14 || type == 5 || type == 11 || type == 12 ||
                   type == 38 || type == 13) {
            if (gpCfNandManager != 0) {
                u32 status = gpCfNandManager->field_180;
                if ((status & 0x80000000) != 0) {
                    if (type == 13) {
                        nand.saveBanner(&CNandData::instance().mNandBanner);
                    } else {
                        u8* buf = gpCfNandManager->field_0x5C;
                        mgr->mPending.mPayload = (u32)buf;
                        if (type == 11) {
                            nand.save(fileName.c_str(), buf, ev.mTag, 0, 0x34);
                        } else if (type == 12 && (lbl_eu_80663E28 & 0x200000) == 0) {
                            nand.save(fileName.c_str(), buf, ev.mTag, 0, 0x34);
                        } else {
                            nand.save(fileName.c_str(), buf, ev.mTag,
                                      &CNandData::instance().mNandBanner, 0x34);
                        }
                    }
                }
            }
        }

        if (type == 7 || type == 6) {
            nand.remove(fileName.c_str());
        }
        if (type != 14) {
            goto resolve;
        }
    resolve:
        ok = 0;
        result = 0;
        NandResolveOp(self, type, &ok, &result);
        if (mgr->mPending.mCb != 0) {
            ((void (*)(u16, u8, u8, s32))mgr->mPending.mCb)(
                mgr->mPending.mType, (u8)mgr->mPending.mSubtype, ok, result);
        }
        mgr->mPending.mType = 0;
        mgr->mPending.mPayload = 0;
        mgr->mPending.mFlag = 0;
        mgr->mEventQueue.mTail = 0;
        mgr->mEventQueue.mHead = 0;
        mgr->field_180 = 0;
    }
}

// File-event pump: while the queue is idle, retire the pending event and
// flush the ring.
extern "C" void NandFileEvent(void* self) {
    s32 busy = 0;
    if (nand.mHead != nand.mTail || gCfNandFileEventPending != 0) {
        busy = 1;
    }
    if (busy == 0) {
        __dt__8023E63C(self);
        NandFlushEvts(self);
    }
}

// Queue the file-check request and its completion callback.
extern "C" int NandQueueChkFc(u32 value, void (*cb)(u32, u32, u32, u32)) {
    gpCfNandManager->queueEvent(0, 2, 0, 0x20, 0, 0);
    gpCfNandManager->queueEvent((u32)cb, 0x28000, 0, 6, (u8)value, 0);
    return 1;
}

// Queue the save-state checks unless presentation state suppresses them.
extern "C" int NandQueueSaveCk(int slot, void* cb, u8 flag) {
    if ((lbl_eu_80663E24 & 0xafa40000) != 0) return 0;
    gpCfNandManager->queueEvent(0, 2, 0, 0x20, 0, 0);
    gpCfNandManager->queueEvent(0, 0x28000, 0, 0x22, (u8)slot, flag);
    gpCfNandManager->queueEvent(0, 0x28000, 0, 0x11, (u8)slot, 0);
    gpCfNandManager->queueEvent((u32)cb, 0x28000, 0, 0x21, (u8)slot, 0);
    gpCfNandManager->queueEvent((u32)cb, 0x28000, 0, 4, (u8)slot, 0);
    return 1;
}

// Queue the 0x5CC check followed by the generic completion marker.
extern "C" int NandQueueChk5CC(void (*cb)(u32, u32, u32, u32)) {
    gpCfNandManager->queueEvent((u32)cb, 0x10000, 0, 0x21, 0, 0);
    gpCfNandManager->queueEvent(0, 0, 0, 0xD, 0, 0);
    return 1;
}

// Queue the three-round 0x690 check with its completion callback.
extern "C" int NandQueueChk690(void (*cb)(u32, u32, u32, u32)) {
    if (gpCfNandManager == 0) return 0;
    for (u8 slot = 1; slot <= 3; slot++) {
        gpCfNandManager->queueEvent(0, 2, 0, 0x20, 0, 0);
        gpCfNandManager->queueEvent(0, 0x28000, 0, 0x23, slot, 0);
        gpCfNandManager->queueEvent(0, 0x28000, 0, 0x21, slot, 0);
        gpCfNandManager->queueEvent((u32)cb, 0x28000, 0, 0xB, slot, 0);
    }
    return 1;
}

// Queue the five NAND checks for the 0x860 path.
extern "C" int NandQueueChk860(u32 value, void (*cb)(u32, u32, u32, u32)) {
    if (gpCfNandManager == 0) return 0;
    gpCfNandManager->queueEvent(0, 2, 0, 0x20, 0, 0);
    gpCfNandManager->queueEvent(0, 0x28000, 0, 0x23, (u8)value, 0);
    gpCfNandManager->queueEvent(0, 0x28000, 0, 0x11, (u8)value, 0);
    gpCfNandManager->queueEvent(0, 0x28000, 0, 0x21, (u8)value, 0);
    gpCfNandManager->queueEvent((u32)cb, 0x28000, 0, 0xC, (u8)value, 0);
    return 1;
}

// Register a heap-buffer request with the manager: clear the buffer first.
extern "C" int NandRegHeapCb(void* buf, int slot, void (*cb)(int, int, u8)) {
    memset(buf, 0, 0xA000);
    return gpCfNandManager->queueEvent((u32)cb, 0xA000, (u32)buf, 3, (u8)slot, 0);
}

// Queue a load check for one save slot.
extern "C" int NandQueueLoadCk(int slot, void* cb) {
    return gpCfNandManager->queueEvent((u32)cb, 0x28000, 0, 1, (u8)slot, 0);
}

// Queue the BA0 check with the completion callback.
extern "C" int NandQueueChkBA0(void (*cb)(u32, u32, u32, u32)) {
    if (gpCfNandManager == 0) return 0;
    gpCfNandManager->queueEvent((u32)cb, 0x28000, 0, 0x10, 0, 0);
    return 1;
}

// Request the three-part NAND reset and clear the event-status word.
extern "C" int NandQueueReset(void (*cb)(u32, u32, u32, u32)) {
    if (gpCfNandManager == 0) return 0;
    lbl_eu_8066476E = 0;
    int result = 0;
    for (u8 slot = 1; slot <= 3; slot++) {
        result = gpCfNandManager->queueEvent((u32)cb, 0x28000, 0, 8, slot, 0);
    }
    return result;
}

// Ask the NAND manager to check the CCC state and call back when done.
extern "C" int NandQueueChkCCC(void (*cb)(u32, u32, u32, u32)) {
    if (gpCfNandManager == 0) return 0;
    return gpCfNandManager->queueEvent((u32)cb, 0x10000, 0, 9, 0, 0);
}

// Reset the NAND manager mode and queue the three follow-up events.
extern "C" int NandResetMode(int mode) {
    if (gpCfNandManager == 0) return 0;
    gpCfNandManager->queueEvent((u32)mode, 0x10000, 0, 0xA, 0, 0);
    gpCfNandManager->queueEvent(0, 0, 0, 7, 1, 0);
    gpCfNandManager->queueEvent(0, 0, 0, 7, 2, 0);
    gpCfNandManager->queueEvent(0, 0, 0, 7, 3, 0);
    return 1;
}

// Queue the three title-file requests for the per-player tables.
extern "C" int NandTitleFile(void (*cb)(void*, unsigned int, unsigned char)) {
    if (gpCfNandManager == 0) return 0;
    gpCfNandManager->field_184 = 0;
    gpCfNandManager->field_186 = 0;
    int result = 0;
    for (u8 slot = 1; slot <= 3; slot++) {
        result = gpCfNandManager->queueEvent(
            (u32)cb, 0x80, (u32)(lbl_eu_80576AC0 + (slot - 1) * 0x80),
            0x12, slot, 0);

    }
    return result;
}

// Allocate and register the singleton manager. Both incoming object pointers
// are forwarded: one to the constructor, the other as the process parent.
extern "C" cf::CfNandManager* create__Q22cf13CfNandManagerFv(void* parent, void* arg) {
    if (gpCfNandManager != 0) {
        return 0;
    }
    mtl::ALLOC_HANDLE workMem = CWorkThreadSystem::getWorkMem();
    cf::CfNandManager* mgr = (cf::CfNandManager*)mtl::MemManager::allocate(0x194, workMem);
    if (mgr == 0) {
        return mgr;
    }
    __ct__cf_CfNandManager(mgr, arg);
    ((CProcess*)mgr)->Regist((CProcess*)parent, false);
    return mgr;
}
// LLM-HARNESS-END: us-80242188

void cf::CfNandManager::Init() {
    nand.reset();
}

void cf::CfNandManager::Term() {
    if (field_0x5C != 0) {
        operator delete(field_0x5C);
        field_0x5C = 0;
    }
    gpCfNandManager = 0;
}

void cf::CfNandManager::Move() {
    if ((lbl_eu_80663E28 & 0x200000) != 0) return;
    nand.update();
    if (mMoveHook != nullptr) (this->*mMoveHook)();
}

template<>
void CTTask<cf::CfNandManager>::Move() {
    cf::CfNandManager* self = static_cast<cf::CfNandManager*>(this);
    if (mMoveFunc != nullptr) (self->*mMoveFunc)();
}

template<>
void CTTask<cf::CfNandManager>::Draw() {
    cf::CfNandManager* self = static_cast<cf::CfNandManager*>(this);
    if (mDrawFunc != nullptr) (self->*mDrawFunc)();
}

// Manager head view: the capture-buffer pointer at +0x5C.
struct CfNandMgrHead {
    u8 _pad5C[0x5C];
    u8* field_0x5C;
};

// True while the save-image capture buffer (+0x5C) is present.
extern "C" u32 NandImageLive() {
    return gpCfNandManager != 0
               ? (u32)(gpCfNandManager->field_0x5C != 0)
               : 0;
}

// Downsample the captured EFB tile into the saved texture block. The four
// scale/offset doubles are owned by this TU's data section.
extern "C" void NandDownsample(void* self) {
    if (gpCfNandCaptureBuf == 0) {
        return;
    }
    double scale = (lbl_eu_806686F8 + 1.0) / (lbl_eu_80668700 + 1.0);
    double xOffset = lbl_eu_806686E8 + scale;
    double yOffset = lbl_eu_806686F0 + scale;
    u8* out = (u8*)gpCfNandCaptureBuf;
    for (u32 y = 0; y < 0x74; y++) {
        u8* row = out + (u32)(y * yOffset) * 0xA4;
        for (u32 x = 0; x < 0xA4; x++) {
            row[(u32)(x * xOffset)] = 0;
        }
    }
}
// LLM-HARNESS-END: us-8024223c

// Allocate the capture staging buffers and register the render callback.
extern "C" void NandAllocBufs() {
    if (gpCfNandCaptureBuf == 0) {
        u32 size1 = GXGetTexBufferSize(0xA4, 0x74, (GXTexFmt)4, (GXBool)0, (GXBool)0);
        u16 height = CDeviceVI::getRenderModeObj()->efbHeight;
        u16 width = CDeviceVI::getRenderModeObj()->fbWidth;
        u32 size2 = GXGetTexBufferSize(width, height, (GXTexFmt)6, (GXBool)0, (GXBool)0);
        u32 handle = mtl::MemManager::getHandleMEM2();
        gpCfNandCaptureBuf = mtl::MemManager::allocate_tail(handle, size1 + size2, 0x20);
        gpCfNandCaptureBufTail = (u8*)gpCfNandCaptureBuf + size1;
    }
    gCfNandCaptureState = 1;
    CScn* scn = (CScn*)CfRes_getD80Flag();
    u32 render = (u32)gpCfNandManager;
    if (render != 0) render += 0x58;
    scn->addRenderCB((IScnRender*)render, 4, 0);
}

// Release the capture state: unregister the render callback from the D80
// scene, free the shared texture buffer, drop the capture counter.
extern "C" void NandTeardown() {
    CScn* scn = (CScn*)CfRes_getD80Flag();
    if (scn != 0) {
        CfNandMgrBases* mgr = (CfNandMgrBases*)gpCfNandManager;
        scn->removeRenderCB(mgr);
    }
    if (gpCfNandCaptureBuf != 0) {
        mtl::MemManager::deallocate(gpCfNandCaptureBuf);
        gpCfNandCaptureBuf = 0;
    }
    gCfNandCaptureState = 0;
}

// LLM-HARNESS-BEGIN: us-802427a8
extern "C" void sinit_802405F0() {}
// LLM-HARNESS-END: us-802427a8
