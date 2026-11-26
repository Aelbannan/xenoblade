#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/cf/CfBdat.hpp"
#include <types.h>
#include "kyoshin/cf/CtrlRemoteApi.hpp"
#include <cstring>
#include "kyoshin/cf/CfNandManager.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/CfTFile.hpp"

#include "monolib/util/MemManager.hpp"
#include "monolib/nand/CNand.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/FixStr.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CtrlObjectParamSymbols.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/CNandData.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/scn/CScn.hpp"

namespace cf { class CfGameManager; }

#pragma section data_type ".data" ".data"

// Switch cases for NandResolveOp; the table itself is owned by this TU.
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const unsigned char jumptable_eu_80536B2C[0x90] = {0};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const unsigned char s_nandTail[4] = {0}; // retail +0x1C8 null word
#pragma section data_type ".data" ".bss"
extern const char kNandSaveNameFormat[0x18];
// typed .bss/.sbss zero-fill (nobits compare sizes only; small sbss objects
// stay in .sbss automatically, no section attr needed).
char gCfNandPartySnapshot[0x38];
cf::CfNandManager* gpCfNandManager;
unsigned char gCfNandFileEventPending;
unsigned char gNandTeardownByte;
unsigned short gNandEventStatus; // name-guess: u16 NAND file-event status bitset; no retail name at 0x8066476E
unsigned short gCfNandCaptureState;
unsigned short gNandTeardownFlag;
// name-guess: loaded from save image slot[0x66] in NandRestore, mirrored into gmPauseFlags 0x8000 and cleared by CfGameManager after load; no retail name found (map/KB searched).
unsigned short gCfNandRestoredEventFlag;
unsigned short gNandSbssGap64776;
unsigned int gNandDirEntryCount; // name-guess: nand.readdir entry count; no retail name at 0x80664778
void* gpCfNandCaptureBuf;
void* gpCfNandCaptureBufTail;
unsigned int gNandSbssPad64780;

#pragma section code_type ".text" ".text"

// Local copies of the save-image / work-entry views (the unit header cannot
// be included here: it redeclares this TU's .sbss/.bss labels with different
// types). Offsets match src/kyoshin/cf/CfNandManager.hpp.

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

struct CfNandCamBlock_Local {
    u8 _pad0[0xC];
    f32 f0C;
};

struct CfNandSaveImage_Local {
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
    CfNandCamBlock_Local camBlock;
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
    CfNandCamBlock_Local camBlock;
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

extern "C" int NandCheckSave(void* block);
// The pending-event handler consumes one dispatched NAND request: it applies
// the per-type result, resolves the callback arguments and finally invokes
// the queued completion callback. The flush pump prepends the request to the
// ring before calling it.
extern "C" void NandVerifyBlk(void* self, u16 type, u8* ok, s32* result);
extern "C" void NandResolveOp(void* self, u16 type, u8* ok, s32* result);
extern "C" void NandDownsample(void* self);
extern "C" void NandImageMake(void* buf, bool flag);

extern "C" int CfRes_getD80Flag();
extern "C" u32 CtrlObjectParam_GetArtsDataWord(u32);
extern char lbl_eu_8050B470[];
extern "C" u8* CtrlRemote_GetFileEventIds();
extern "C" void CtrlObjectParam_RefreshCharEntry(void* data, u32 value);
extern "C" void KizunagramUnpackChart(void* src, void* dst);
namespace cf { struct CfGameManager {
    static void resetBattlePresentation();
    static void drainEventQueue();
    static u32 getQueuedFileEventCount();
    static void queueEventId(u32);
    static void fadeOutGameEffects();
    static void setEventCounterA(u32);
    static struct CfObjectMove* getPlayer(int);
}; }

extern "C" void cfCam_storeFollowD(void* v, f32 f);
extern "C" void loadReloadInfo(void* p);
extern "C" void mineLoadFromSave(u8* rec);
extern "C" void rebuildTboxPool();
extern "C" CfNandNameRoot* getReslistC48();
extern "C" void updateConfig__FPUc(u8* src, int mode);
#include "kyoshin/cf/CtrlRemoteApi.hpp"
extern "C" void setMasterVolume__Fff(f32 a, f32 b);
extern "C" void MenuSnd_SetSeVolPush_95F4(f32 v);
extern "C" void MenuSnd_SetMasterAndPush_9510(f32 v);

extern char* lbl_eu_806628C0;
extern u8 gmVoiceFlag5D;

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
    gmVoiceFlag5D = flag;
    gNandTeardownByte = flag;
    KizunagramUnpackChart(dst->kizuna, src->kizunaSrc);
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
extern "C" int NandApplySave(CfNandSaveImage_Local* img) {
    (void)__alloca(0);
    CfNandSaveImage_Local* image = img;
    int ok = 1;
    u8* live;
    CfNandWorkEntrySrc* src;
    CfNandWorkEntryDst* dst;
    u32 ver = image->version - 0x70001;
    if (ver <= 1) {
        int valid = NandCheckSave(image);
        if (valid == 0) {
            return 0;
        }
        u32 flagLen = CtrlRemote_GetFixedSize1214((void*)0);
        memcpy((u8*)(u8*)(u8*)(u8*)(u8*)(u8*)(u8*)(u8*)CtrlRemote_GetSharedBufPtr(), image->flagData, flagLen);
        gCfNandRestoredEventFlag = image->slot[0x66];
        live = CtrlRemote_GetFileEventIds();
        memcpy(live, image->workHead, 0x41F0);
        src = (CfNandWorkEntrySrc*)(live + 0x7FC4);
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
        gCfTCalendarDay = image->progress.f08;
        gCfTCalendarYear = image->progress.f0A;
        CfNandPartySnapshot* snapDst = (CfNandPartySnapshot*)gCfNandPartySnapshot;
        *snapDst = image->snapshot;
        memcpy(spItemBlock, image->itemBlob, 0x12120);
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
    if ((mtl::MemManager::calculateCrc(v1->flagData, CtrlRemote_GetFixedSize1214((void*)0)) & 0xFFFF) != v1->flagTag.crc) {
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

    u32 flagLen = CtrlRemote_GetFixedSize1214((void*)0);
    memcpy((u8*)(u8*)(u8*)(u8*)(u8*)(u8*)(u8*)(u8*)CtrlRemote_GetSharedBufPtr(), v1->flagData, flagLen);
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
    gCfTCalendarDay = v1->progress.f08;
    gCfTCalendarYear = v1->progress.f0A;
    CfNandPartySnapshot* snapDst = (CfNandPartySnapshot*)gCfNandPartySnapshot;
    *snapDst = v1->snapshot;

    u8* item = (u8*)spItemBlock;
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
// operator-delete it. The in-progress flag at gNandTeardownFlag is armed
// around the apply so CSysWinScenarioLog can poll the teardown.
// The retail symbol name is an address-derived placeholder, not a destructor
// mangling (this TU declares no class whose dtor it could be), so it stays a
// flat function name.
extern "C" int __dt__8023E448() {
    u8* none = 0;
    u8* img = gpCfNandManager->field_0x5C ? gpCfNandManager->field_0x5C : none;
    if (img == none) {
        return 0;
    }
    u16 clear = 0;
    gNandTeardownFlag = 1;
    int result = NandApplySave((CfNandSaveImage_Local*)img);
    gNandTeardownFlag = clear;
    u8* still = gpCfNandManager->field_0x5C;
    if (still != 0) {
        operator delete(still);
        gpCfNandManager->field_0x5C = (u8*)(u32)clear;
    }
    return result;
}


// GX framebuffer geometry comes from the canonical CDeviceVI declaration.
// GX / cache entry points come from the SDK headers pulled in by CScn.hpp.

// While the counter is armed (1), copy the EFB into the shared texture
// buffer and flush it, then disarm. Body recovered from CfNandManager.ctx.c.
void cf::CfNandManager::cbRenderBefore(CScn*) {
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
extern "C" CfNandPartySnapshot* NandPartySnap() {
    return (CfNandPartySnapshot*)gCfNandPartySnapshot;
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
extern "C" CfNandPartySnapshot* NandCopyParty(CfNandPartySnapshot* snapshot) {
    snapshot->field_30 = 0;
    if (cf::CBattleManager::getInstance() != 0) {
        snapshot->field_30 = *(u32*)((u8*)cf::CBattleManager::getInstance() + 0x194);
    }
    CfNandPartyEntry* entry = snapshot->mEntry;
    for (u32 i = 0; i < 3; i++) {
        void* player = cf::CfGameManager::getPlayer(i);
        if (player != 0) {
            ml::CVec3* pos = ((cf::CfObjectMove*)player)->CfObject_getPosVector();
            entry->mPos = *pos;
            entry->field_0C = ((cf::CfObjectMove*)player)->CfObject_getScaledAngle();
        } else {
            entry->mPos = gmAimPoint;
            entry->field_0C = gmNandFloat54;
        }
        entry++;
    }
    snapshot->field_30 = OSCalcCRC16((const u8*)snapshot, 0x34);
    return snapshot;
}
// LLM-HARNESS-END: us-8023e320

// LLM-HARNESS-BEGIN: us-8023e414
// Compact the live 0x3DD4-byte work entries into the 0x304-byte save records.
// The destination starts with the 0x41F0-byte bulk header copied from the
// file-event id table; each entry ends with its compacted kizuna block.
extern "C" u16 NandPackEntry(u8* dst) {
    u8* live = (u8*)CtrlRemote_GetFileEventIds();
    std::memcpy(dst, live, 0x41F0);

    u8 placeFlag = 0;
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player != 0) {
        CfNandPlayerC4* c4 = (CfNandPlayerC4*)player;
        if (c4->fieldC4 != 0) {
            placeFlag = (u8)((c4->fieldC4->field4EC >> 30) & 1);
        }
    }

    struct Arr48 { u32 w[48]; };
    struct Arr36 { u32 w[36]; };
    CfNandWorkEntrySrc* src = (CfNandWorkEntrySrc*)(live + 0x41F0);
    CfNandWorkEntryDst* out = (CfNandWorkEntryDst*)(dst + 0x41F0);
    for (u32 i = 0; i < 14; i++) {
        cf::CActorParam* actor = (cf::CActorParam*)src->subSlot;
        CfNandBattleParams* bp = (CfNandBattleParams*)actor->CActorParam_getBattleParams();

        out->head.f000 = src->head.f000;
        out->head.f002 = src->head.f002;
        out->head.f006 = src->head.f006;
        out->head.f00A = src->head.f00A;
        out->head.f00E = src->head.f00E;
        out->head.f012 = src->head.f012;
        out->head.f016 = src->head.f016;
        out->head.f01C = src->head.f01C;
        out->head.f020 = src->head.f020;
        *(Arr48*)&out->head.f024 = *(Arr48*)&src->head.f024;
        *(Arr36*)&out->arr028[47] = *(Arr36*)&src->arr028[47];
        out->f178 = src->f178;

        out->f17C = src->f177C;
        out->f180 = src->f17D0;
        out->f184 = src->f17D4;
        out->f188 = src->f17D8;
        out->f18C = src->f17DC;
        out->f190 = src->f17E0;
        out->f194 = src->f17E4;
        out->f198 = src->f17E8;
        out->f19A = src->f17EA;
        out->f19C = src->f17EC;
        out->f1A0 = src->f17F0;
        out->f1A4 = src->f17F4;
        out->f1A8 = src->f17F8;
        out->f1AA = src->f17FA;
        out->f1AC = src->f17FC;
        out->f1AE = src->f17FE;
        out->f1B0 = src->f1800;
        out->f1B2 = src->f1802;
        out->f1B4 = src->f1804;
        out->f1B6 = src->f1806;
        out->f1B8 = src->f1808;
        out->f1BC = src->f180C;
        out->f1C0 = src->f1810;
        out->f1C4 = src->f1814;
        out->f1C8 = src->f1818;
        out->f1CC = src->f181C;
        out->f1D0 = src->f1820;
        out->f1D1 = src->f1821;
        out->f1D2 = src->f1822;
        out->f1D3 = src->f1823;
        out->f1D4 = src->f1824;
        out->f1D8 = src->f1828;
        out->f1DC = src->f182C;
        out->f1E0 = src->f1830;
        out->f1E4 = src->f1834;
        out->f1E8 = src->f1838;
        out->f1EC = src->f183C;
        out->f1F0 = src->f1840;

        if (bp->p04 > out->f18C) out->f180 = bp->p04;
        if (bp->p0C <= out->f194) out->f188 = bp->p0C;
        if (bp->p08 <= out->f190) out->f184 = bp->p08;

        ItemWorkState scratch;
        initItemWorkState(&scratch);
        scratch.field_00 = src->f34D4;
        scratch.field_02 = src->f34D6;
        scratch.field_04 = src->f34D8;
        scratch.field_08 = src->f34DC;
        scratch.field_0C = src->f34E0;
        scratch.field_10 = src->f34E4;
        out->f2A0 = src->f177C;
        out->f2A4 = src->f1780;
        out->f2A8 = src->f1784;
        out->f2AC = scratch.field_00;
        out->f2AE = scratch.field_02;
        out->f2B0 = scratch.field_04;
        out->f2B4 = scratch.field_08;
        out->f2B8 = scratch.field_0C;
        out->f2BC = scratch.field_10;
        out->f2C0 = placeFlag;
        std::memset(out->tail, 0, sizeof(out->tail));
        KizunagramPackChart(out->kizuna, src->kizunaSrc);

        src++;
        out++;
    }

    u8* ev = dst + 0x44B1;
    if (((u32)ev & 1) != 0) ev++;
    cf::CfGameManager::drainEventQueue();
    return OSCalcCRC16(dst, 0x6C28);
}
// LLM-HARNESS-END: us-8023e414

// LLM-HARNESS-BEGIN: us-8023e7bc
// Build the texture descriptor followed by a 32-byte-aligned copy of the
// captured EFB tile. The caller's flag selects a zero fill over the capture.
extern "C" void NandTileDescr(CfNandTexBlock* block, bool useCapture) {
    u32 size = GXGetTexBufferSize(0xA4, 0x74, (GXTexFmt)4, (GXBool)0, (GXBool)0);
    u32 subOffset = (u8*)&block->sub - (u8*)block;
    u32 payloadOffset = (u8*)&block->sub.field34 - (u8*)block;
    if ((payloadOffset & 0x1F) != 0) {
        payloadOffset = payloadOffset + 0x20 - (payloadOffset & 0x1F);
    }
    block->field00 = 0x20AF30;
    block->field04 = 1;
    block->field08 = 0xC;
    block->field0C = subOffset;
    block->field10 = 0;
    block->sub.height = 0x74;
    block->sub.width = 0xA4;
    block->sub.format = 4;
    block->sub.offset = payloadOffset;
    u8* payload = (u8*)block + payloadOffset;
    block->sub.field20 = 0;
    block->sub.field24 = 0;
    block->sub.field28 = 1;
    block->sub.field2C = 1;
    block->sub.field30 = lbl_eu_806686E0;
    std::memset(block->sub.field34, 0, sizeof(block->sub.field34));
    if (!useCapture && gpCfNandCaptureBuf != 0) {
        std::memcpy(payload, gpCfNandCaptureBuf, size);
    } else {
        std::memset(payload, 0, size);
    }
    DCStoreRangeNoSync(payload, size);
}
// LLM-HARNESS-END: us-8023e7bc

// LLM-HARNESS-BEGIN: us-8023e8f4
// Assemble the 0x9C80-byte save slot from the live calendar, party records,
// character table and the captured texture tile.
extern "C" u16 NandSaveBuild(CfNandSaveSource* source, CfNandSaveBuf* buf, u8 slot) {
    std::memset(buf, 0, 0x9C80);
    OSCalendarTime time;
    buf->field00 = (s16)cf::CfGameManager::getQueuedFileEventCount();
    OSTicksToCalendarTime(OSGetTime(), &time);
    buf->field04 = (u16)time.yday;
    buf->field06 = (u16)(time.wday + 1);
    buf->field09 = (u8)time.year;
    buf->field03 = (u8)time.hour;
    buf->field02 = (u8)time.mday;
    buf->field08 = (u8)time.month;
    buf->field67 = (u8)CtrlRemote_TouchBitByArg(0x3508);
    buf->field10 = (u8)(buf->field10 & ~0xFF);
    u32 timer = CfT_FrameTimerGet();
    buf->field0A = (u16)((timer >> 4) & 0xFFFF);
    buf->field0C = (u16)((timer >> 20) & 0x3F);
    buf->field0E = source->fieldB262;
    buf->field11 = (u8)((buf->field11 & 0xFF00) | (u8)source->fieldB260);
    for (u32 i = 0; i < 2; i++) {
        buf->mRecord[i] = source->mRecord[i];
    }
    u32 charEntry = (u32)CtrlObjectParam_GetCharEntry(buf->mRecord[0].w[0] >> 16);
    buf->field64 = (u16)CtrlObjectParam_GetArtsDataWord(charEntry);
    buf->field66 = slot;
    const char* col = lbl_eu_8050B470 + 0x12;
    const u8* name = (const u8*)getBdatStringColumnValue(::cf::CfBdat::spBtlPcListFileData, col, buf->mRecord[0].w[0]);
    std::memcpy(buf->mName, name, 0x20);
    NandTileDescr(&buf->tex, slot != 0);
    return OSCalcCRC16((const u8*)buf, 0x9C80);
}
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
    NandCopyParty((CfNandPartySnapshot*)&image->snapshot);
    image->partyTag.tag = 0x50415254;
    image->partyTag.length = sizeof(CfNandPartySnapshot);
    image->partyTag.unk08 = 0x10;
    image->partyTag.crc = OSCalcCRC16((const u8*)&image->snapshot, sizeof(CfNandPartySnapshot));
    image->progress.field00 = CfT_FrameTimerGet();
    image->progress.f04 = gCfTPlayClockSeconds;
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
// Handle the IWorkEvent file event delivered through the +0x54 vtable thunk.
bool cf::CfNandManager::handleFileEvent(CEventFile* event) {
    s32 mode = event->unk0;
    gCfNandFileEventPending = 0;
    if (mode == 1) {
        if (event->field_14 != 0) {
            cf::CfGameManager::resetBattlePresentation();
        }
    } else if (mode == 4) {
        if (gpCfNandManager != 0 && gpCfNandManager->field_0x5C != 0) {
            operator delete(gpCfNandManager->field_0x5C);
            gpCfNandManager->field_0x5C = 0;
        }
    }
    return true;
}
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
extern "C" void NandDispatchPendingEvent(void* self) {
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
            gNandEventStatus = 0;
            if (ok != 0) {
                u8 sub = mgr->mPending.mSubtype;
                u32 count = gNandDirEntryCount;
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
                    gNandEventStatus = gNandEventStatus | (u16)(1 << mgr->mPending.mSubtype);
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
                        gNandEventStatus = gNandEventStatus |
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
            gNandDirEntryCount = 0;
            mgr->mPending.mPayload = (u32)lbl_eu_80576AC0;
            nand.readdir((char*)lbl_eu_80576AC0, 0x200, (u32*)&gNandDirEntryCount);
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
                        } else if (type == 12 && (gmPauseFlags & 0x200000) == 0) {
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
        NandDispatchPendingEvent(self);
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
    if ((gmGameFlags & 0xafa40000) != 0) return 0;
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
    gNandEventStatus = 0;
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
    cf::CfNandManager* mgr =
        new (mtl::MemManager::allocate(0x194, CWorkThreadSystem::getWorkMem()))
            cf::CfNandManager(arg);
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
    if ((gmPauseFlags & 0x200000) != 0) return;
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
    double yOffset = -lbl_eu_806686E8 + scale;
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
    ::CScn* scn = (::CScn*)CfRes_getD80Flag();
    scn->addRenderCB(gpCfNandManager, 4, 0);
}

// Release the capture state: unregister the render callback from the D80
// scene, free the shared texture buffer, drop the capture counter.
extern "C" void NandTeardown() {
    ::CScn* scn = (::CScn*)CfRes_getD80Flag();
    if (scn != 0) {
        scn->removeRenderCB(gpCfNandManager);
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

// The retail constructor is emitted at the head of the unit; its flat name is
// the MWCC spelling of CfNandManager().  Keep the member-initialization order
// of the original: queue storage, ring metadata, pending event, status words,
// the move hook, then the manager/capture globals.
extern u32 lbl_eu_80536B20[3];

cf::CfNandManager::CfNandManager(void* arg)
    : CTTask<cf::CfNandManager>(), IWorkEvent(), IScnRender(),
      field_0x5C(0) {

    mEventQueue.mRingBase = &mEventQueue.mFirst;
    mEventQueue.mHead = 0;
    mEventQueue.mTail = 0;
    mEventQueue.mSize = 0x10;
    mPending.mCb = 0;
    mPending.mPayload = 0;
    mPending.mType = 0;
    mPending.mSubtype = 0;
    mPending.mFlag = 0;
    field_180 = 0;
    field_184 = 0;
    field_186 = 0;
    typedef void (CfNandManager::*MoveHook)();
    mMoveHook = *reinterpret_cast<MoveHook*>(__ptmf_null);
    gpCfNandManager = this;
    gCfNandFileEventPending = 0;
    gNandTeardownByte = 0;
    gCfNandCaptureState = 0;
    gNandTeardownFlag = 0;
    gCfNandRestoredEventFlag = 0;
    gpCfNandCaptureBuf = 0;
    gpCfNandCaptureBufTail = 0;
    gNandEventStatus = 0;
    mMoveHook = *reinterpret_cast<MoveHook*>(lbl_eu_80536B20);
    mPending.mType = 0;
    mPending.mPayload = 0;
    mPending.mFlag = 0;
}

// The retail virtual destructor unregisters the render callback, frees the
// capture buffer and clears the manager's global state before the base-class
// teardown emitted by the compiler runs.
cf::CfNandManager::~CfNandManager() {
    ::CScn* scn = (::CScn*)CfRes_getD80Flag();
    if (scn != 0) {
        scn->removeRenderCB(gpCfNandManager);
    }
    if (gpCfNandCaptureBuf != 0) {
        mtl::MemManager::deallocate(gpCfNandCaptureBuf);
        gpCfNandCaptureBuf = 0;
    }
    gpCfNandManager = 0;
    gCfNandFileEventPending = 0;
    gNandTeardownByte = 0;
    gCfNandCaptureState = 0;
    gNandTeardownFlag = 0;
    gCfNandRestoredEventFlag = 0;
}


__attribute__((used)) const char kNandSaveNameFormat[0x18] = "%s%02d\0banner.bin\0name";
