// Auto-scaffolded catalog TU for kyoshin/cf/CfNandManager
// Replace stubs with high-level C/C++ during decomp.

// CfGameManager.hpp declares the teardown entry __dt__8023E448 as void(), but
// retail returns func_8023D3D8's result in r3 (mr r3,r30 epilogue). Rename the
// stale void declaration away while including (the include guard makes the
// later transitive inclusion a no-op); the properly-typed import is declared
// below.
#define __dt__8023E448 dt_8023E448_stale_void_decl
#define getEventCounterA cf_getEventCounterA_stale_void_decl
#include "kyoshin/cf/CfGameManager.hpp"
#undef __dt__8023E448
#undef getEventCounterA
#include "kyoshin/cf/CfNandManager.hpp"
#include "kyoshin/harness_catalog.hpp"
// (CSaveLoad.hpp intentionally not included: its void return for
// func_8023F860 conflicts with the retail int-returning implementation.)
#include "monolib/work/CEventFile.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/FixStr.hpp"
#include <revolution/os/OSCrc.h>
#include <cstring>
#include <revolution/os/OSTime.h>
#include <revolution/os/OSCache.h>

// Minimal view of cf::CBattleManager (the full CBattleManager.hpp pulls
// CSuddenCommu.hpp, which fails to compile in this TU's include chain). The
// party snapshot only calls getInstance() and reads the +0x194 word.
namespace cf {
class CBattleManager {
public:
    static CBattleManager* getInstance();
};
}

// forward declarations for scaffold thunk references
void __dt__Q22cf13CfNandManagerFv(void*);
void cbRenderBefore__Q22cf13CfNandManagerFv(void*);

// defined later in this TU (retail symbol names are unmangled)
extern "C" int func_8023CD9C(CfNandSaveBlock* block);
// NOTE: retail keeps the raw parameter in r3 for the null/version/magic
// checks and the first region-1 argument, and only materializes the
// full-image pointer (r30) once the CRC chain starts.
extern "C" void func_8023E544(cf::CfNandManager* self, u32 type, u8* flag,
                               u32* out);
extern "C" void func_8023EABC(cf::CfNandManager* self, u32 type, u8* status,
                              u32* out);

// Constructor (retail flat symbol __ct__cf_CfNandManager). Takes the scene
// pointer create() forwards in r4, but the body ignores it.
// Event-ring slots are constructed through a TU-local wrapper object so
// MWCC expands the 15-element array construction inline.
struct CfNandEventArrayInit {
    CfNandEvent m[15];
    CfNandEventArrayInit() : m() {}
};

extern "C" void* __ct__cf_CfNandManager(void* self, CScn* pScene) {
    (void)pScene;
    cf::CfNandManager* mgr = (cf::CfNandManager*)self;
    __ct__8CProcessFv(mgr);

    u32* w = reinterpret_cast<u32*>(mgr);
    const u32* nullPtmf = __ptmf_null;

    // CTTask base vtable first, then the composite/derived vtables and the
    // two interface subobject slots (+0x54 IWorkEvent, +0x58 IScnRender).
    w[4] = reinterpret_cast<u32>(lbl_eu_80536CB4);   // +0x10

    const u32* vt = reinterpret_cast<const u32*>(lbl_eu_80536BBC);

    // +0x3C mMoveFunc / +0x48 mDrawFunc: default null ptmf copies.
    u32* moveFunc = &w[0xF];
    moveFunc[0] = nullPtmf[0];
    moveFunc[1] = nullPtmf[1];
    moveFunc[2] = nullPtmf[2];
    u32* drawFunc = &w[0x12];
    drawFunc[0] = nullPtmf[0];
    drawFunc[1] = nullPtmf[1];
    drawFunc[2] = nullPtmf[2];

    w[4] = reinterpret_cast<u32>(vt);                 // +0x10 derived
    w[0x15] = reinterpret_cast<u32>(vt) + 0x24;       // +0x54 IWorkEvent
    w[0x16] = reinterpret_cast<u32>(vt) + 0xac;       // +0x58 IScnRender
    mgr->field_0x5C = 0;

    // Event-ring init: slot 0 inline, slots 1..15 through the wrapper's
    // inlined array-construction expansion (tags left untouched), then the
    // ring metadata.
    CfNandEvent* e0 = &mgr->mEventQueue.mFirst;
    e0->mWord = 0;
    e0->mField8 = 0;
    e0->mFieldC = 0;
    e0->mFieldE = 0;
    e0->mFieldF = 0;

    new (&mgr->mEventQueue.mRest) CfNandEventArrayInit;

    mgr->mEventQueue.mSize = 0x10;
    mgr->mEventQueue.mRingBase = &mgr->mEventQueue.mFirst;
    mgr->mEventQueue.mTail = 0;
    mgr->mEventQueue.mHead = 0;

    CfNandPendingEvent* p = &mgr->mPending;
    p->mCb = 0;
    p->mPayload = 0;
    p->mType = 0;
    p->mSubtype = 0;
    p->mFlag = 0;
    mgr->field_180 = 0;
    mgr->field_184 = 0;
    mgr->field_186 = 0;

    // Default null move hook (+0x188 ptmf), then register the singleton and
    // clear the NAND state globals, then install the static default hook.
    u32* hook = reinterpret_cast<u32*>(&mgr->mMoveHook);
    hook[0] = nullPtmf[0];
    hook[1] = nullPtmf[1];
    hook[2] = nullPtmf[2];

    lbl_eu_80664768 = mgr;
    lbl_eu_8066476C = 0;
    lbl_eu_8066476D = 0;
    lbl_eu_80664770 = 0;
    lbl_eu_80664772 = 0;
    lbl_eu_8066477C = 0;
    lbl_eu_80664780 = 0;
    lbl_eu_8066476E = 0;

    hook[0] = lbl_eu_80536B20[0];
    hook[1] = lbl_eu_80536B20[1];
    hook[2] = lbl_eu_80536B20[2];

    lbl_eu_80664774 = 0;
    mgr->mPending.mType = 0;
    mgr->mPending.mPayload = 0;
    mgr->mPending.mFlag = 0;
    return mgr;
}

// Destructor of the CfNandEventQueue member embedded in CfNandManager at +0x60
// (retail placeholder name __dt__8023BFCC): zeroes the queue head/tail
// counters, then frees the block when the deleting flag is set.
extern "C" void* __dt__8023BFCC(CfNandEventQueue* self, int dealloc_flag) {
    if (self != nullptr) {
        self->mTail = 0;
        self->mHead = 0;
        if (dealloc_flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

cf::CfNandManager::~CfNandManager() {
    // Restore the three subobject vtables (retail stores them explicitly in
    // the ctor and dtor; MWCC emits no vtable stores for this class). Keep the
    // label pointer live so MWCC emits a single @ha/@l pair for all three.
    u8* vt = lbl_eu_80536BBC;
    reinterpret_cast<u32*>(this)[4] = reinterpret_cast<u32>(vt);   // +0x10 CTTask
    reinterpret_cast<u32*>(this)[0x15] = reinterpret_cast<u32>(vt + 0x24);  // +0x54 IWorkEvent
    reinterpret_cast<u32*>(this)[0x16] = reinterpret_cast<u32>(vt + 0xac);  // +0x58 IScnRender

    // Drop the D80 scene render callback while the singleton is live.
    // Retail calls removeRenderCB whenever the scene pointer is non-null,
    // passing a null renderer when the singleton is already gone.
    CScn* scene = reinterpret_cast<CScn*>(CfRes_getD80Flag());
    if (scene != 0) {
        // Retail passes the singleton +0x58 when live, else the (null)
        // pointer itself - one register, conditionally offset.
        u8* p = reinterpret_cast<u8*>(lbl_eu_80664768);
        scene->removeRenderCB(reinterpret_cast<IScnRender*>(p != nullptr ? p + 0x58 : p));
    }
    // Release the shared NAND resource buffer.
    if (lbl_eu_8066477C != 0) {
        mtl::MemManager::deallocate(lbl_eu_8066477C);
        lbl_eu_8066477C = 0;
    }
    // Clear the NAND state globals; the embedded event queue is torn down
    // automatically (inline ~CfNandEventQueue, null-checked member call),
    // then the CProcess base.
    lbl_eu_80664768 = 0;
    lbl_eu_8066476C = 0;
    lbl_eu_8066476D = 0;
    lbl_eu_80664770 = 0;
    lbl_eu_80664772 = 0;
    lbl_eu_80664774 = 0;
}

void Init__Q22cf13CfNandManagerFv(void) {
    extern void func_804DA1B4(void*);
    func_804DA1B4(&lbl_eu_8065FD00);
}

void cf::CfNandManager::Term() {
    // Free the heap buffer, then clear the singleton pointer.
    if (field_0x5C != nullptr) {
        __dl__FPv(field_0x5C);
        field_0x5C = nullptr;
    }
    lbl_eu_80664768 = nullptr;
}

void cf::CfNandManager::Move() {
    // Skip the NAND completion pump while the global mode-flag bit is set.
    if (lbl_eu_80663E28 & 0x200000) {
        return;
    }
    func_804DA1CC(&lbl_eu_8065FD00);
    // Dispatch the +0x188 move hook through __ptmf_test/__ptmf_scall when set.
    if (__ptmf_test(&mMoveHook)) {
        (this->*mMoveHook)();
    }
}

extern char lbl_eu_80576CC0[];
extern "C" void* func_8023C1B4() { return lbl_eu_80576CC0; }

// Returns 1 while the NAND request queue is busy (head != tail) or the async
// flag is set; the result variable shape reproduces the retail li r3,0 / beqlr.
int func_8023C1C0() {
    u32 result = 0;
    if (lbl_eu_8065FD00.mHead != lbl_eu_8065FD00.mTail || lbl_eu_8066476C != 0) {
        result = 1;
    }
    return result;
}

// Target us-8023e320: build a 3-member party snapshot (player info + scale
// via the player vtable, static fallback data for missing slots) and run
// OSCalcCRC16 over the whole struct.
extern "C" u32 func_8023C1F0(CfNandPartySnapshot* snapshot) {
    snapshot->field_30 = 0;
    if (cf::CBattleManager::getInstance() != 0) {
        snapshot->field_30 = ((CfNandBmView*)cf::CBattleManager::getInstance())->field_194;
    }
    // Keep the fallback base and the write cursor in locals so MWCC holds them
    // in preserved regs across the virtual calls (retail r31/r30 shape).
    u32* fallback = lbl_eu_8057164C;
    CfNandPartyEntry* entry = snapshot->mEntry;
    for (s32 i = 0; i < 3; i++) {
        CfNandPlayerVt* player = (CfNandPlayerVt*)cf::CfGameManager::getPlayer(i);
        if (player != 0) {
            CfNandPartyInfo* info = player->vf0AC();
            // Both word loads are scheduled before the stores.
            u32 w0 = info->mPair.field_00;
            u32 w4 = info->mPair.field_04;
            entry->mPair.field_00 = w0;
            entry->mPair.field_04 = w4;
            entry->field_08 = info->field_08;
            entry->field_0C = player->vf0D8();
        } else {
            u32 w0 = fallback[0];
            u32 w4 = fallback[1];
            entry->mPair.field_00 = w0;
            entry->mPair.field_04 = w4;
            entry->field_08 = fallback[2];
            entry->field_0C = lbl_eu_80663E54;
        }
        entry++;
    }
    return OSCalcCRC16((const u8*)snapshot, 0x34);
}

// Target us-8023e414: build the 0x6C28-byte NAND work buffer from the shared
// character blob: bulk-copy the 0x41F0 head, then per entry (14 iterations)
// copy fields/arrays, fetch clamp bounds through the +0x17C subobject's
// virtual +0x224 slot, clamp three floats, build the 0x14 scratch and the
// compact kizuna block. Afterwards drain the party-slot queue into the tail
// (aligned up to even) and CRC16 the whole buffer.
extern "C" u32 func_8023C2E4(CfNandWorkBuf* work) {
    // Local alias born at entry keeps the parameter web first in MWCC's
    // allocation order (retail parks it in r25 before the first call).
    CfNandWorkBuf* dst = work;
    u8* blob = (u8*)func_8009D5FC();
    memcpy(dst, blob, 0x41F0);

    // Placement bit: getPlayer(0)+0xC4 -> +0x4EC bit30, copied into every
    // entry at +0x2C0.
    CfNandPlayerC4* player = (CfNandPlayerC4*)cf::CfGameManager::getPlayer(0);
    int ok = 0;
    if (player != 0) {
        if (player->fieldC4 != 0) {
            ok = 1;
        }
    }
    u32 flagBit;
    if (ok != 0) {
        flagBit = (player->fieldC4->field4EC >> 30) & 1;
    } else {
        flagBit = 0;
    }

    // Trip counts hoisted so MWCC parks them in preserved registers across
    // the entry loop (retail r30/r31).
    u32 n1 = 48;
    u32 n2 = 36;
    CfNandWorkEntrySrc* s = (CfNandWorkEntrySrc*)(blob + 0x41F0);
    CfNandWorkEntryDst* d = work->entry;
    for (int i = 0; i < 14; i++) {
        // Clamp bounds stay live across all the field copies below.
        CfNandClampView* clamp = s->sub.vf224();
        d->head.f000 = s->head.f000;
        d->head.f002 = s->head.f002;
        d->head.f006 = s->head.f006;
        d->head.f00A = s->head.f00A;
        d->head.f00E = s->head.f00E;
        d->head.f012 = s->head.f012;
        d->head.f016 = s->head.f016;
        d->head.f01C = s->head.f01C;
        d->head.f020 = s->head.f020;
        d->head.f024 = s->head.f024;
        for (u32 k = 0; k < n1; k++) {
            d->arr028[k] = s->arr028[k];
        }
        for (u32 k = 0; k < n2; k++) {
            d->arr0E8[k] = s->arr0E8[k];
        }
        d->f178 = s->f178;
        d->f17C = s->f17CC;
        d->f180 = s->f17D0;
        d->f184 = s->f17D4;
        d->f188 = s->f17D8;
        d->f18C = s->f17DC;
        d->f190 = s->f17E0;
        d->f194 = s->f17E4;
        d->f198 = s->f17E8;
        d->f19A = s->f17EA;
        d->f19C = s->f17EC;
        d->f1A0 = s->f17F0;
        d->f1A4 = s->f17F4;
        d->f1A8 = s->f17F8;
        d->f1AA = s->f17FA;
        d->f1AC = s->f17FC;
        d->f1AE = s->f17FE;
        d->f1B0 = s->f1800;
        d->f1B2 = s->f1802;
        d->f1B4 = s->f1804;
        d->f1B6 = s->f1806;
        d->f1B8 = s->f1808;
        d->f1BC = s->f180C;
        d->f1C0 = s->f1810;
        d->f1C4 = s->f1814;
        d->f1C8 = s->f1818;
        d->f1CC = s->f181C;
        d->f1D0 = s->f1820;
        d->f1D1 = s->f1821;
        d->f1D2 = s->f1822;
        d->f1D3 = s->f1823;
        d->f1D4 = s->f1824;
        d->f1D8 = s->f1828;
        d->f1DC = s->f182C;
        d->f1E0 = s->f1830;
        d->f1E4 = s->f1834;
        d->f1E8 = s->f1838;
        d->f1EC = s->f183C;
        d->f1F0 = s->f1840;
        // Clamp against the virtual-call bounds (min of both sides).
        d->f180 = clamp->f04 <= d->f18C ? clamp->f04 : d->f18C;
        d->f188 = clamp->f0C <= d->f194 ? clamp->f0C : d->f194;
        d->f184 = clamp->f08 <= d->f190 ? clamp->f08 : d->f190;

        CfNandSub14 sub14;
        func_80174658(&sub14);
        u32 qA = s->f177C;
        u32 qB = s->f1780;
        u32 qC = s->f1784;
        sub14.f00 = s->f34D4;
        sub14.f02 = s->f34D6;
        sub14.f04 = s->f34D8;
        sub14.f08 = s->f34DC;
        sub14.f0C = s->f34E0;
        sub14.f10 = s->f34E4;
        d->f2A0 = qA;
        d->f2A4 = qB;
        d->f2A8 = qC;
        d->f2AC = sub14.f00;
        d->f2AE = sub14.f02;
        d->f2B0 = sub14.f04;
        d->f2B4 = sub14.f08;
        d->f2B8 = sub14.f0C;
        d->f2BC = sub14.f10;
        d->f2C0 = (u8)flagBit;        memset(d->tail, 0, sizeof(d->tail));
        func_8025EC0C(d->kizuna, s->kizunaSrc);
        s++;
        d++;
    }
    // Drain the party-slot queue into the buffer tail, aligned up to even.
    u8* q = (u8*)dst + 0x44B1;
    if ((u32)q & 1) {
        q = (u8*)dst + 0x44B2;
    }
    ((cf::CfGameManager*)q)->drainEventQueue();
    return OSCalcCRC16((const u8*)dst, 0x6C28);
}

// Target us-8023e7bc: fill a texture descriptor block and copy/memset its
// payload region (32 bytes aligned up from the 0x38-byte header) with the
// caller buffer, the shared NAND capture buffer, or zeros; then flush it.
extern "C" void func_8023C68C(CfNandTexBlock* self, void* src) {
    u32 size = GXGetTexBufferSize(0xa4, 0x74, 4, (GXBool)0, 0);
    // Payload starts at the 32-byte boundary past the 0x38-byte header.
    u32 off = (u32)((u8*)&self->sub.field24 - (u8*)self);
    u32 rem = off & 0x1F;
    u32 subOff = (u32)((u8*)&self->sub - (u8*)self);
    if (rem != 0) {
        off += 0x20 - rem;
    }
    self->field00 = 0x20AF30;
    self->field04 = 1;
    self->field08 = 0xC;
    self->field0C = subOff;
    self->field10 = 0;
    self->sub.width = 0xa4;
    self->sub.height = 0x74;
    self->sub.format = 4;
    self->sub.offset = off;
    u8* dst = (u8*)self + off;
    self->sub.field20 = 0;
    self->sub.field24 = 0;
    self->sub.field28 = 1;
    self->sub.field2C = 1;
    self->sub.field30 = lbl_eu_806686E0;
    self->sub.field34[0] = 0;
    self->sub.field34[1] = 0;
    self->sub.field34[2] = 0;
    self->sub.field34[3] = 0;
    // Both copy paths share one memcpy call site (retail layout).
    u8* copySrc = (u8*)src;
    if (copySrc == 0) {
        copySrc = lbl_eu_8066477C;
    }
    if (copySrc == 0) {
        memset(dst, 0, size);
    } else {
        memcpy(dst, copySrc, size);
    }
    DCStoreRangeNoSync(dst, size);
}

// Target us-8023e8f4: lay out one save-buffer slot: clear the payload,
// stamp scenario/date/progress metadata, copy the two party records and the
// bdat character name, build the embedded texture block, then CRC16 it.
// (Returns the CRC so callers can stamp their descriptors.)
extern "C" u32 func_8023C7C4(CfNandSaveSource* src, CfNandSaveBuf* dst, u32 id) {
    // Size expressions kept structurally distinct so MWCC does not CSE them
    // into one preserved-register constant (retail computes each separately).
    memset(dst, 0, 0x9C80);
    dst->field00 = (s16)cf::CfGameManager::getQueuedFileEventCount();
    OSCalendarTime cal;
    OSTicksToCalendarTime(OSGetTime(), &cal);
    dst->field04 = (u16)cal.yday;
    dst->field06 = (u16)(cal.wday + 1);
    dst->field09 = (u8)cal.year;
    dst->field03 = (u8)cal.hour;
    dst->field02 = (u8)cal.mday;
    dst->field08 = (u8)cal.month;
    dst->field67 = (u8)func_8009CF8C(0x3508);
    dst->field10 &= (u8)~0xFF;
    u32 progress = func_8006A80C();
    dst->field0A = (u16)((progress >> 4) & 0xFFFF);
    dst->field0C = (u16)((progress >> 20) & 0x3F);
    dst->field0E = src->fieldB262;
    dst->field11 |= (u8)src->fieldB260;
    CfNandRecord* recSrc = src->mRecord;
    CfNandRecord* recDst = dst->mRecord;
    for (int i = 0; i < 2; i++) {
        *recDst++ = *recSrc++;
    }
    dst->field64 = (u16)func_800A082C(func_8009EC9C((u16)dst->mRecord[0].w[0]));
    dst->field66 = (u8)id;
    memcpy(dst->mName,
           (const void*)getBdatStringColumnValue(&lbl_eu_80664090, lbl_eu_8050B470 + 0x12,
                                                 (int)(u16)dst->mRecord[0].w[0]),
           0x20);
    func_8023C68C(&dst->tex, (void*)id);
    return OSCalcCRC16((const u8*)dst, 0x10000 - 0x6380);
}

// Target us-8023ea80: assemble the full 0x248F0 on-media save image (version
// tag 0x70002) into the caller-provided buffer: header, per-region tag/CRC
// descriptors, save slot, work buffer, party snapshot, item blob, and the
// compacted save-name directory. Returns 0 when no buffer was given.
int func_8023C93C(CfNandSaveImage* img, u32 id) {
    if (img == nullptr) {
        return 0;
    }
    // Shared descriptor word 0x10 (MWCC CSEs the constant into a preserved
    // register and even folds the slot-tag length arithmetic around it).
    img->magic = 0x55535244;   // 'USRD'
    img->unk08 = 0x10;
    img->totalSize = 0x248F0;
    img->version = 0x70002;

    // FLAG region: copy the shared capture data and stamp its descriptor.
    u32 len = func_8009CF84();
    memcpy(img->flagData, func_8009CF0C(), len);
    u32 crc = OSCalcCRC16(img->flagData, len);
    img->flagTag.tag = 0x464C4147;
    img->flagTag.unk08 = 0x10;
    img->flagTag.length = 0x1230;
    img->flagTag.crc = crc;

    // GAME/work-buffer region built by func_8023C2E4 (returns its CRC).
    crc = func_8023C2E4(&img->work);
    img->gameTag.tag = 0x47414D45;
    img->gameTag.unk08 = 0x10;
    img->gameTag.length = 0x6C50;
    img->gameTag.crc = crc;

    // TIME region: progress word + three scalar settings.
    img->progress.field00 = func_8006A80C();
    img->progress.f04 = lbl_eu_80663D94;
    img->progress.f08 = lbl_eu_80661AF4;
    img->progress.f0A = lbl_eu_80661AF6;
    crc = OSCalcCRC16((const u8*)&img->progress, 0xC);
    img->timeTag.tag = 0x54494D45;
    img->timeTag.unk08 = 0x10;
    img->timeTag.length = 0x30;
    img->timeTag.crc = crc;

    // PCPM region: party snapshot from func_8023C1F0.
    crc = func_8023C1F0(&img->snapshot);
    img->partyTag.tag = 0x5043504D;
    img->partyTag.unk08 = 0x10;
    img->partyTag.length = 0x50;
    img->partyTag.crc = crc;

    // CAMD region: camera block filled through its +0xC subrecord.
    func_8006CBD8((u8*)&img->camBlock, (u8*)&img->camBlock.f0C);
    crc = OSCalcCRC16((const u8*)&img->camBlock, 0x10);
    img->camTag.tag = 0x43414D44;
    img->camTag.unk08 = 0x10;
    img->camTag.length = 0x30;
    img->camTag.crc = crc;

    // ITEM region: bulk copy of the shared item blob.
    memcpy(img->itemBlob, lbl_eu_806641B8, 0x12120);
    crc = OSCalcCRC16(img->itemBlob, 0x12120);
    img->itemTag.tag = 0x4954454D;
    img->itemTag.unk08 = 0x10;
    img->itemTag.length = 0x12120;
    img->itemTag.crc = crc;

    // WTHR region: message count with a game-manager fallback when zero.
    func_8016E09C(&img->wthrBlock);
    u16 cnt = (u16)func_8009CF8C(0x3F);
    img->wthrBlock.f0E = cnt;
    if (cnt == 0) {
        img->wthrBlock.f0E = (u16)getEventCounterA__Q22cf13CfGameManagerFv();
    }
    crc = OSCalcCRC16((const u8*)&img->wthrBlock, 0x10);
    img->wthrTag.tag = 0x57544852;
    img->wthrTag.unk08 = 0x10;
    img->wthrTag.length = 0x30;
    img->wthrTag.crc = crc;

    // SNDS region: three audio-query floats plus a zero word.
    img->sndArea.f00 = getMasterVolume();
    img->sndArea.f04 = func_801896A0();
    img->sndArea.f08 = func_801895EC();
    img->sndArea.f0C = 0;
    crc = OSCalcCRC16((const u8*)&img->sndArea, 0x10);
    img->sndTag.tag = 0x534E4453;
    img->sndTag.unk08 = 0x10;
    img->sndTag.length = 0x30;
    img->sndTag.crc = crc;

    // MINE region CRC (contents are left as produced upstream).
    // MINE region is filled by func_80207C94, then CRC'd.
    func_80207C94(img->mineRegion);
    crc = OSCalcCRC16(img->mineRegion, 0x384);
    img->mineTag.tag = 0x4D494E45;
    img->mineTag.unk08 = 0x10;
    img->mineTag.length = 0x3B0;
    img->mineTag.crc = crc;

    // Save-slot payload built over the whole image as its data source.
    crc = func_8023C7C4((CfNandSaveSource*)img, &img->slot, id);
    img->slotTag.tag = 0x5448554D;
    img->slotTag.length = 0xA010;
    img->slotTag.unk08 = 0x10;
    img->slotTag.crc = crc;

    // TBOX region: compact the circular save-name directory into flat
    // 0x1C-byte records (the counter lives at table+0).
    CfNandNameRoot* cont = func_800B6CA0();
    memset(&img->names, 0, sizeof(img->names));
    img->names.count = 0;
    // Root pointer is re-read from the container every iteration (retail
    // keeps only the container in a preserved register).
    for (CfNandNameNode* n = cont->mNext->mNext; n != cont->mNext;
         n = n->mNext) {
        CfNandSaveNameEntry* e = &img->names.entries[img->names.count];
        e->f04 = n->f08;
        e->f08 = n->f0C;
        e->f0C = n->f10;
        e->f10 = n->f14;
        e->f14 = n->f18;
        e->f18 = n->f1C;
        e->f1C = n->f20;
        e->f1E = n->f22;
        e->f1F = n->f23;
        e->f04 = 0;
        img->names.count++;
    }
    crc = OSCalcCRC16((const u8*)&img->names, 0x234);
    img->tboxTag.tag = 0x54424F58;
    img->tboxTag.unk08 = 0x10;
    img->tboxTag.length = 0x410;
    img->tboxTag.crc = crc;

    // OPTD tail: snapshot of the option singleton.
    memcpy(img->optdBlob, getInstance__14Class_80296898Fv(), 0x40);
    crc = OSCalcCRC16(img->optdBlob, 0x40);
    img->optdTag.tag = 0x4F505444;
    img->optdTag.unk08 = 0x10;
    img->optdTag.length = 0x40;
    img->optdTag.crc = crc;
    return 1;
}

// Validate the CRC chain of a save image. Returns 1 when every region
// checksum matches. Retail emits three FULLY UNROLLED branch bodies (no
// sharing): 0x70002 -> OSCalcCRC16 over the 0x70002 tail, 0x70001 ->
// MemManager::calculateCrc over the larger 0x70001 tail, and 0x60002 ->
// calculateCrc with the FIRST region check omitted entirely.
// Validate the CRC chain of a save image. Returns 1 when every region
// checksum matches. Every checksum word is the .crc of the tag block that
// precedes its region in CfNandSaveImage. Retail emits three FULLY UNROLLED
// branch bodies (no sharing): 0x70002 -> OSCalcCRC16, 0x70001 ->
// MemManager::calculateCrc, and 0x60002 -> calculateCrc with the FIRST
// region check omitted entirely.
extern "C" int func_8023CD9C(CfNandSaveBlock* block) {
    if (block == nullptr) {
        return 0;
    }
    int result = 1;
    if (!(block->field_0C == 0x70002 || block->field_0C == 0x70001 ||
          block->field_0C == 0x60002)) {
        return 0;
    }
    // Magic/version checks read through the parameter (retail keeps them
    // in volatile regs; MWCC CSEs each load across both compares).
    if (block->magic != 0x444D4D59 && block->magic != 0x55535244) {
        return 0;
    }
    CfNandSaveImage* img = (CfNandSaveImage*)block;
    if (block->field_0C == 0x70002) {
        if ((OSCalcCRC16(block->data, 0x9C80) & 0xFFFF) != img->slotTag.crc) {
            result = 0;
        }
        if ((OSCalcCRC16(img->flagData, func_8009CF84()) & 0xFFFF) != img->flagTag.crc) {
            result = 0;
        }
        if ((OSCalcCRC16((const u8*)&img->work, 0x6C28) & 0xFFFF) != img->gameTag.crc) {
            result = 0;
        }
        if ((OSCalcCRC16((const u8*)&img->progress, 0xC) & 0xFFFF) != img->timeTag.crc) {
            result = 0;
        }
        if ((OSCalcCRC16((const u8*)&img->snapshot, 0x34) & 0xFFFF) != img->partyTag.crc) {
            result = 0;
        }
        if ((OSCalcCRC16((const u8*)&img->camBlock, 0x10) & 0xFFFF) != img->camTag.crc) {
            result = 0;
        }
        if ((OSCalcCRC16(img->itemBlob, 0x12120) & 0xFFFF) != img->itemTag.crc) {
            result = 0;
        }
        if ((OSCalcCRC16((const u8*)&img->wthrBlock, 0x10) & 0xFFFF) != img->wthrTag.crc) {
            result = 0;
        }
        if ((OSCalcCRC16((const u8*)&img->sndArea, 0x10) & 0xFFFF) != img->sndTag.crc) {
            result = 0;
        }
        if ((OSCalcCRC16(img->mineRegion, 0x384) & 0xFFFF) != img->mineTag.crc) {
            result = 0;
        }
        if ((OSCalcCRC16((const u8*)&img->names, 0x234) & 0xFFFF) != img->tboxTag.crc) {
            result = 0;
        }
        if ((OSCalcCRC16(img->optdBlob, 0x40) & 0xFFFF) != img->optdTag.crc) {
            result = 0;
        }
    } else if (block->field_0C == 0x70001) {
        if ((mtl::MemManager::calculateCrc(block->data, 0x9C80) & 0xFFFF) != img->slotTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc(img->flagData, func_8009CF84()) & 0xFFFF) != img->flagTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->work, 0x6C28) & 0xFFFF) != img->gameTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->progress, 0xC) & 0xFFFF) != img->timeTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->snapshot, 0x34) & 0xFFFF) != img->partyTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->camBlock, 0x10) & 0xFFFF) != img->camTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc(img->itemBlob, 0x157D0) & 0xFFFF) != img->itemTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->wthrBlock, 0x10) & 0xFFFF) != img->wthrTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->sndArea, 0x10) & 0xFFFF) != img->sndTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc(img->mineRegion, 0x384) & 0xFFFF) != img->mineTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->names, 0x234) & 0xFFFF) != img->tboxTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc(img->optdBlob, 0x40) & 0xFFFF) != img->optdTag.crc) {
            result = 0;
        }
    } else {
        // Version 0x60002: same chain minus the leading slot-region check.
        if ((mtl::MemManager::calculateCrc(img->flagData, func_8009CF84()) & 0xFFFF) != img->flagTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->work, 0x6C28) & 0xFFFF) != img->gameTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->progress, 0xC) & 0xFFFF) != img->timeTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->snapshot, 0x34) & 0xFFFF) != img->partyTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->camBlock, 0x10) & 0xFFFF) != img->camTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc(img->itemBlob, 0x157D0) & 0xFFFF) != img->itemTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->wthrBlock, 0x10) & 0xFFFF) != img->wthrTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->sndArea, 0x10) & 0xFFFF) != img->sndTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc(img->mineRegion, 0x384) & 0xFFFF) != img->mineTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc((const u8*)&img->names, 0x234) & 0xFFFF) != img->tboxTag.crc) {
            result = 0;
        }
        if ((mtl::MemManager::calculateCrc(img->optdBlob, 0x40) & 0xFFFF) != img->optdTag.crc) {
            result = 0;
        }
    }
    return result;
}

// properly-typed import (retail returns the teardown result in r3)
extern "C" void* __dt__8023E448();
// opaque callee for the teardown path (real body is target us-8023f51c)
extern "C" void* func_8023D3D8();

// NAND teardown: guards on the singleton's heap buffer. The `buf ? buf : 0`
// self-assignment reproduces the retail materialized-bool diamond (the buffer
// word is re-tested with its own cr0 before the main path). `zero` stays live
// across the __dl__ call, forcing the retail r31 save/reuse, and the
// func_8023D3D8 result is held in r30 until the final return.
void* __dt__8023E448() {
    // The `p ? p : 0` normalizer reproduces the retail materialized-bool
    // diamond (load; cmpi; beq->li 0; re-test). Every formulation tried lands
    // the diamond value in r0; retail holds it in r3 - the residual 4-insn
    // reg-swap is documented in the session notes.
    void* raw = lbl_eu_80664768->field_0x5C;
    raw = raw != 0 ? raw : 0;
    if (raw == 0)
        return (void*)0;
    lbl_eu_80664772 = 1;
    void* result = func_8023D3D8();
    lbl_eu_80664772 = 0;
    if (lbl_eu_80664768->field_0x5C != 0) {
        __dl__FPv(lbl_eu_80664768->field_0x5C);
        lbl_eu_80664768->field_0x5C = 0;
    }
    return result;
}

// Placeholder for target us-8023f51c (retail body is large). noinline keeps
// the call site real; the store through a loaded pointer makes the call
// clobber-unknown, forcing callers to reload globals across it.
extern "C" __declspec(noinline) void* func_8023D3D8() {
    *lbl_eu_8066477C = 0;
    return nullptr;
}

// OnFileEvent body (retail Fv symbol carries a hidden CEventFile* in r4: the
// IWorkEvent dispatcher calls the OnFileEvent vtable slot with this_sub in r3
// and the file event in r4; the OnFileEvent adjustor thunk rewrites r3, so the
// file is the only live argument here). The dummy first parameter keeps the
// file in r4 without touching the verbatim retail symbol name.
extern "C" u32 handleFileEvent__Q22cf13CfNandManagerFv(void* unused, CEventFile* file) {
    lbl_eu_8066476C = 0;
    // Event types: 1 = load finished (kick the game manager), 4 = teardown.
    if (file->unk0 == 1 && file->field_14 != 0) {
        resetBattlePresentation__Q22cf13CfGameManagerFv();
    } else if (file->unk0 == 4) {
        if (lbl_eu_80664768->field_0x5C != 0) {
            __dl__FPv(lbl_eu_80664768->field_0x5C);
            lbl_eu_80664768->field_0x5C = 0;
        }
    }
    return 1;
}

// Target us-80240688: validate the save block at CfNandManager+0x178. A zero
// flag short-circuits; the 'DMMY' magic means an empty slot (clear flag+out);
// version 0x70002 is CRC16-checked via OSCalcCRC16, 0x70001 via
// MemManager::calculateCrc; a mismatch reports status 3. Any other version
// just clears the flag and out.
extern "C" void func_8023E544(cf::CfNandManager* self, u32 type, u8* flag,
                              u32* out) {
    (void)type;
    if (*flag == 0) {
        return;
    }
    CfNandSaveBlock* block = (CfNandSaveBlock*)self->mPending.mPayload;
    if (block->magic == 0x444D4D59) {   // 'DMMY' dummy slot
        *flag = 0;
        *out = 0;
        return;
    }
    u32 version = block->field_0C;
    if (version == 0x70002) {
        u32 crc = OSCalcCRC16(block->data, 0x9C80);
        if ((crc & 0xFFFF) != block->field_1C) {
            *flag = 0;
            *out = 3;
        }
    } else if (version == 0x70001) {
        u32 crc = mtl::MemManager::calculateCrc(block->data, 0x9C80);
        if ((crc & 0xFFFF) != block->field_1C) {
            *flag = 0;
            *out = 3;
        }
    } else {
        *flag = 0;
        *out = 0;
    }
}

__declspec(noinline) void __dt__8023E63C(cf::CfNandManager* self) {
    u16 type = self->mPending.mType;
    if (type == 0) {
        return;
    }

    // can-act flag: NAND queue initialized and no manager operation in flight.
    int canAct = 0;
    if (lbl_eu_8065FD00.mStatus != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        u32 flags;
        if (mgr != 0) {
            flags = mgr->field_180;
        } else {
            flags = 0;
        }
        if (flags == 0) {
            canAct = 1;
        }
    }

    u8 status = (u8)canAct; // result byte reported to the callback
    u32 out = 0;            // result word reported to the callback

    // Phase 1: validate / refresh state for this event type. Written as a
    // compare chain (not a switch): retail emits sequential cmplwi tests.
    // Cases 3/0x12 re-read the field (retail reloads lhz into a scratch reg
    // after the entry check), cases 2/0x11 reuse the entry-time value.
    if (self->mPending.mType == 3) {
        func_8023E544(self, type, &status, &out);
    } else if (self->mPending.mType == 0x12) {
        if (canAct != 0) {
            CfNandSaveBlock* block = (CfNandSaveBlock*)self->mPending.mPayload;
            if (block->magic != 0x444D4D59) { // 'DMMY' = empty slot
                self->field_184++;
            }
        } else {
            out = 5;
        }
    } else if (type == 2) {
        if (canAct != 0) {
            self->mPending.mCb = 0;
        } else {
            self->mEventQueue.mTail = 0;
            self->mEventQueue.mHead = 0;
        }
    } else if (type == 0x11) {
        lbl_eu_8066476E = 0;
        if (canAct == 0) {
            out = 5;
        } else {
            // Build the save-file name either by formatting or from the
            // static suffix of the bdat blob, then scan the directory list.
            const char* entry = (const char*)self->mPending.mPayload;
            s32 count = lbl_eu_80664778;
            ml::FixStr<32> name(true);
            if (self->mPending.mSubtype != 0) {
                name.format(lbl_eu_8050B470, lbl_eu_806628C0);
            } else {
                name = (const char*)(lbl_eu_8050B470 + 7);
            }
            s32 found = 0;
            s32 i;
            for (i = 0; i < count; i++) {
                if (strcmp(name.c_str(), entry) == 0) {
                    found = 1;
                    break;
                }
                entry += strlen(entry) + 1;
            }
            status = (u8)found;
            if (found != 0) {
                lbl_eu_8066476E |= (u16)(1 << self->mPending.mSubtype);
            } else {
                out = 0;
            }
        }
    }

    // Phase 2: post-process based on the validation result.
    if (type == 0xB) {
        if (status != 0) {
            if (self->mPending.mSubtype != 3) {
                self->mPending.mCb = 0;
            }
        } else {
            self->mEventQueue.mTail = 0;
            self->mEventQueue.mHead = 0;
        }
    } else if (type == 0x11) {
        if (status != 0) {
            s32 size = self->mEventQueue.mSize;
            s32 next = self->mEventQueue.mHead + 1;
            self->field_180 = 0;
            self->mEventQueue.mTail--;
            self->mEventQueue.mHead = next % size;
        }
    } else if (type == 0x12) {
        if (self->mPending.mSubtype == 3) {
            status = (u8)(self->field_184 != 0);
        } else {
            self->mPending.mCb = 0;
            self->field_180 = 0;
        }
    } else if (type == 0xA) {
        if (status != 0) {
            // Advance the ring head by two slots and rewind the tail by three
            // (the load pipeline reserves one extra event slot).
            s32 size = self->mEventQueue.mSize;
            s32 m = (self->mEventQueue.mHead + 1) % size;
            self->mEventQueue.mTail -= 3;
            m = (m + 1) % size;
            self->mEventQueue.mHead = (m + 1) % size;
        }
    }

    // Phase 3: re-validate the payload when a write/erase completed, then
    // dispatch the pending callback with the results.
    if (self->mPending.mCb != 0) {
        CfNandSaveBlock* block = (CfNandSaveBlock*)self->mPending.mPayload;
        if (status != 0) {
            u32 t = type - 1;
            if (t <= 1) {
                if (func_8023CD9C(block) == 0) {
                    status = 0;
                    out = 3;
                }
            } else if (type == 8) {
                if (func_8023CD9C(block) == 0) {
                    out = 3;
                    status = 0;
                    self->mEventQueue.mTail = 0;
                    self->mEventQueue.mHead = 0;
                } else {
                    lbl_eu_8066476E |= (u16)(1 << self->mPending.mSubtype);
                    if (self->mPending.mSubtype != 3) {
                        self->mPending.mCb = 0;
                    }
                }
            }
        } else {
            func_8023EABC(self, type, &status, &out);
            if (type == 8 && self->mPending.mSubtype != 3) {
                self->mPending.mCb = 0;
            }
        }
        if (self->mPending.mCb != 0) {
            ((void (*)(u16, u8, u8, u32, void*))self->mPending.mCb)(
                self->mPending.mType, self->mPending.mSubtype, status, out,
                (void*)self->mPending.mPayload);
        }
    }

    // Consume the handled event.
    self->mPending.mType = 0;
    self->mPending.mPayload = 0;
    self->mPending.mFlag = 0;
}

// Target us-80240c00: event-status dispatch for the NAND manager. Maps the
// event type to a status word written through *out (the r5 status slot is
// unused here). field_180 bit0/bit2 report 1, else 2. Types 1/2 report 3,
// 6/7 report 4, everything else in 3..0x26 falls into the dense switch that
// MWCC lowers to the retail jump table (jumptable_eu_80536B2C).
extern "C" void func_8023EABC(cf::CfNandManager* self, u32 type, u8* status, u32* out) {
    // Leading equality chains, then the dense switch over 3..0x26.
    if (type == 0x10 || type == 0x21) {
        u32 flags = self->field_180;
        if ((flags & 1) != 0 || (flags & 4) != 0) {
            *out = 1;
        } else {
            *out = 2;
        }
    } else if (type == 1 || type == 2) {
        *out = 3;
    } else {
        switch (type) {
        // Dense case set over 3..0x26 (two groups) so MWCC emits the retail
        // jump table (jumptable_eu_80536B2C).
        case 0x6:
        case 0x7:
            *out = 4;
            break;
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x8:
        case 0x9:
        case 0xC:
        case 0xD:
        // Dead but keeps three switch target groups, which is what makes
        // MWCC lower this to the retail jump-table form.
        case 0x10:
        case 0x21:
            break;
        case 0xE:
        case 0xF:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x20:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26: {
            // Same status computation as above, kept as a distinct copy so
            // the jump-table body is emitted separately.
            u32 flags = self->field_180;
            if ((flags & 1) != 0 || (flags & 4) != 0) {
                *out = 1;
            } else {
                *out = 2;
            }
            break;
        }
        default:
            break;
        }
    }
}

// NAND completion pump for the manager: drains the embedded event ring,
// builds the save-file name for the popped event, and issues the matching
// request against the shared CNand queue. Event types not requesting I/O
// report through func_8023EABC and invoke the pending callback.
void func_8023EB78(cf::CfNandManager* self) {
    while (self->mEventQueue.mTail != 0) {
        // Pop slot (head) and advance the ring.
        s32 size = self->mEventQueue.mSize;
        s32 head = self->mEventQueue.mHead;
        s32 next = head + 1;
        self->mEventQueue.mTail--;
        CfNandEvent* ev =
            &self->mEventQueue.mRingBase[head];
        self->mEventQueue.mHead = next % size;

        // Publish the popped event into the pending-event scratch.
        self->mPending.mFlag = ev->mFieldF;
        self->mPending.mCb = ev->mWord;
        self->mPending.mTag = ev->mTag;
        self->mPending.mPayload = ev->mField8;
        self->mPending.mType = ev->mFieldC;
        self->mPending.mSubtype = ev->mFieldE;

        // Keep a stack copy and build the save-file name.
        CfNandEvent evCopy = *ev;
        (void)evCopy;
        ml::FixStr<32> name(true);
        if (self->mPending.mSubtype != 0) {
            name.format(lbl_eu_8050B470, lbl_eu_806628C0);
        } else {
            name = (const char*)(lbl_eu_8050B470 + 7);
        }

        switch (self->mPending.mType) {
        case 1:
        case 8:
        case 2: {
            // Load request: lazily allocate the shared capture buffer, then
            // enqueue a read into it.
            cf::CfNandManager* mgr = lbl_eu_80664768;
            if (mgr->field_0x5C == 0) {
                mgr->field_0x5C = (u8*)mtl::MemManager::allocate_ex(
                    0x40000, mtl::MemManager::getHandleMEM2(), 0x40);
            }
            u8* buf = mgr->field_0x5C;
            if (buf == 0) {
                buf = 0;
            }
            self->mPending.mPayload = (u32)buf;
            func_804DA34C(&lbl_eu_8065FD00, name.c_str(), (u32)buf,
                          self->mPending.mTag);
            return;
        }
        case 9:
        case 0xA: {
            // Banner/shared-path load.
            cf::CfNandManager* mgr = lbl_eu_80664768;
            if (mgr->field_0x5C == 0) {
                mgr->field_0x5C = (u8*)mtl::MemManager::allocate_ex(
                    0x40000, mtl::MemManager::getHandleMEM2(), 0x40);
            }
            u8* buf = mgr->field_0x5C;
            if (buf == 0) {
                buf = 0;
            }
            self->mPending.mPayload = (u32)buf;
            func_eu_804DE660(&lbl_eu_8065FD00, (u32)buf, self->mPending.mTag);
            return;
        }
        case 0x11:
            // Directory listing into the entry-count scratch.
            lbl_eu_80664778 = 0;
            self->mPending.mPayload = (u32)lbl_eu_80576AC0;
            func_804DA3E4(&lbl_eu_8065FD00, lbl_eu_80576AC0, 0x200,
                          &lbl_eu_80664778);
            return;
        case 3:
        case 0x12:
            // Plain read into the pending buffer.
            func_804DA34C(&lbl_eu_8065FD00, name.c_str(),
                          self->mPending.mPayload, self->mPending.mTag);
            return;
        case 0xB:
        case 0xC:
            if (self->mPending.mType == 0xC &&
                (lbl_eu_80663E28 & 0x01000000) != 0) {
                func_804DA29C(&lbl_eu_8065FD00, name.c_str(),
                              self->mPending.mPayload, self->mPending.mTag, 0,
                              0x34);
            } else {
                // Flush request; the mode-flag gate picks the fixed table.
                func_804DA29C(&lbl_eu_8065FD00, name.c_str(),
                              self->mPending.mPayload, self->mPending.mTag,
                              lbl_eu_80577358, 0x34);
            }
            return;
        case 6:
        case 7:
            // Remove request.
            func_804DA3A0(&lbl_eu_8065FD00, (u32)name.c_str());
            return;
        case 0xE:
            // Nothing to do; keep draining.
            break;
        case 0x20:
            // Retry: put the current pending event back into the ring while
            // its retry counter lasts.
            if (self->mPending.mTag > 0) {
                self->mPending.mTag--;
                cf::CfNandManager* mgr = lbl_eu_80664768;
                mgr->mEventQueue.mHead--;
                if (mgr->mEventQueue.mHead < 0) {
                    mgr->mEventQueue.mHead += mgr->mEventQueue.mSize;
                }
                CfNandEvent* slot =
                    &mgr->mEventQueue.mRingBase[mgr->mEventQueue.mHead];
                slot->mWord = self->mPending.mCb;
                slot->mTag = self->mPending.mTag;
                slot->mField8 = self->mPending.mPayload;
                slot->mFieldC = self->mPending.mType;
                slot->mFieldE = self->mPending.mSubtype;
                slot->mFieldF = self->mPending.mFlag;
                mgr->mEventQueue.mTail++;
            }
            break;
        default: {
            // Terminal event: report status and dispatch the callback.
            u8 cbStatus = 0;
            u32 cbOut = self->field_180;
            func_8023EABC(self, self->mPending.mType, &cbStatus, &cbOut);
            if (self->mPending.mCb != 0) {
                ((void (*)(u16, u8, u8, u32, void*))self->mPending.mCb)(
                    self->mPending.mType, self->mPending.mSubtype, cbStatus,
                    cbOut, (void*)self->mPending.mPayload);
            }
            self->mPending.mType = 0;
            self->mPending.mPayload = 0;
            self->mPending.mFlag = 0;
            self->mEventQueue.mTail = 0;
            self->mEventQueue.mHead = 0;
            self->field_180 = 0;
            return;
        }
        }
    }
}

void func_8023F288(u8* self) {
    // Retail materializes `busy = (head != tail) || (flag != 0)` into r5
    // (li r5,0; conditional li r5,1; cmpi r5,0) and skips the teardown when
    // busy — the ||-materialized inverse of the guard, not inline &&.
    bool busy = (lbl_eu_8065FD00.mHead != lbl_eu_8065FD00.mTail) ||
                (lbl_eu_8066476C != 0);
    if (!busy) {
        __dt__8023E63C((cf::CfNandManager*)self);
        func_8023EB78((cf::CfNandManager*)self);
    }
}

// Target us-802414a4: push two events {0, 2, 0, 0x20, 0, 0} and
// {cb, 0x28000, 0, 6, value, 0} onto the manager ring. Always returns 1.
u32 func_8023F2F4(u32 value, void (*cb)(u32, u32, u32, u32)) {
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = 0;
        entry->mTag = 2;
        entry->mField8 = 0;
        entry->mFieldC = 0x20;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = (u32)cb;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 6;
        entry->mFieldE = (u8)value;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    return 1;
}

// Target us-80241570: when no presentation-flag bits are set, push five
// events onto the manager ring: {0,tag 2,0x20}, then callback-tagged events
// {0x80000000} with status codes 0x22/0x11/0x21/0x4 carrying the arguments.
// Returns 1 on success.
u32 func_8023F3C0(u32 a, u32 b, u32 c) {
    if (lbl_eu_80663E24 & 0xafa40000) {
        return 0;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = 0;
        entry->mTag = 2;
        entry->mField8 = 0;
        entry->mFieldC = 0x20;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = 0;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 0x22;
        entry->mFieldE = (u8)a;
        entry->mFieldF = (u8)c;
        mgr->mEventQueue.mTail++;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = 0;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 0x11;
        entry->mFieldE = (u8)a;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = b;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 0x21;
        entry->mFieldE = (u8)a;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = b;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 4;
        entry->mFieldE = (u8)a;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    return 1;
}

// Target us-8024177c: push two {word, tag, 0, fieldC, 0, 0} events onto the
// manager ring (tag 0x10000/fieldC 0x21, then tag 0/fieldC 0xd), each gated
// on the singleton being live. Always returns 1.
u32 func_8023F5CC(u32 word) {
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0x10000;
        entry->mField8 = 0;
        entry->mFieldC = 0x21;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0;
        entry->mField8 = 0;
        entry->mFieldC = 0xd;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    return 1;
}

// Push an event {word, tag, 0, fieldC, byte, 0} onto the manager ring when
// the singleton is live. (Param order chosen so MWCC's right-to-left argument
// evaluation materializes constants in the retail sequence.)
static void CfNandPushEvent(u32 tag, u32 fieldC, u8 byte, u32 word) {
    cf::CfNandManager* mgr = lbl_eu_80664768;
    if (mgr == 0) {
        return;
    }
    CfNandEvent* entry =
        &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                    mgr->mEventQueue.mSize];
    entry->mWord = word;
    entry->mTag = tag;
    entry->mField8 = 0;
    entry->mFieldC = fieldC;
    entry->mFieldE = byte;
    entry->mFieldF = 0;
    mgr->mEventQueue.mTail++;
}

// Target us-80241840: three rounds of four events each: {0, tag 2, 0x20},
// then callback-tagged {0x28000} events with status codes 0x23/0x21/0xB and
// the round index. Returns 1 on success.
extern "C" int func_8023F690(void (*cb)(u32, u32, u32, u32)) {
    if (lbl_eu_80664768 == 0) {
        return 0;
    }
    u32 cbWord = (u32)cb;
    for (int round = 1; round <= 3; round++) {
        CfNandPushEvent(2, 0x20, 0, 0);
        CfNandPushEvent(0x28000, 0x23, (u8)round, 0);
        CfNandPushEvent(0x28000, 0x21, (u8)round, 0);
        CfNandPushEvent(0x28000, 0xB, (u8)round, cbWord);
    }
    return 1;
}

// Target us-80241a10: push five events: {0, 2, 0x20}, then callback-tagged
// {0x28000} events with status 0x23/0x11/0x21 carrying the slot byte, and
// finally {word, 0x28000, 0xC}. Returns 1 on success.
extern "C" int func_8023F860(int slot, void* word) {
    if (lbl_eu_80664768 == 0) {
        return 0;
    }
    CfNandPushEvent(2, 0x20, 0, 0);
    CfNandPushEvent(0x28000, 0x23, (u8)slot, 0);
    CfNandPushEvent(0x28000, 0x11, (u8)slot, 0);
    CfNandPushEvent(0x28000, 0x21, (u8)slot, 0);
    CfNandPushEvent(0x28000, 0xC, (u8)slot, (u32)word);
    return 1;
}

// Target us-80241c14: zero a 0xA000-byte caller buffer, then push
// {word, 0xA000, buf, 3, byte, 0} onto the manager ring. Returns 1 when
// the singleton is live, else 0.
u32 func_8023FA64(u8* buf, u8 byte, u32 word) {
    u32 result;
    memset(buf, 0, 0xA000);
    if (lbl_eu_80664768 == 0) {
        result = 0;
    } else {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0xA000;
        entry->mField8 = (u32)buf;
        entry->mFieldC = 3;
        entry->mFieldE = byte;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
        result = 1;
    }
    return result;
}

// Push an event {word, 0x28000, 0, 1, byte, 0} onto the manager's ring and
// report success through an int result held in a single live-across-body
// variable (retail li r11,0 / li r11,1 / mr r3,r11 shape).
// Push an event {word, 0x28000, 0, 1, byte, 0} onto the manager's ring.
// Written as a returning inline helper so MWCC folds the singleton check into
// the caller's result variable (retail li r11,0 / li r11,1 / mr r3,r11).
static int CfNandPushEventRead(int byte, u32 word) {
    cf::CfNandManager* mgr = lbl_eu_80664768;
    if (mgr == 0) {
        return 0;
    }
    CfNandEvent* entry =
        &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                    mgr->mEventQueue.mSize];
    entry->mWord = word;
    entry->mTag = 0x28000;
    entry->mField8 = 0;
    entry->mFieldC = 1;
    entry->mFieldE = (u8)byte;
    entry->mFieldF = 0;
    mgr->mEventQueue.mTail++;
    return 1;
}

int func_8023FB28(int byte, u32 word) {
    return CfNandPushEventRead(byte, word);
}

// Push an event {word, 0x28000, 0, 0x10, 0, 0} onto the manager's ring.
// Returns 1 on success, 0 when the singleton is not set up.
u32 func_8023FBA0(u32 word) {
    if (lbl_eu_80664768 == 0) {
        return 0;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry =
            &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                        mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 0x10;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    return 1;
}

// Target us-80241dc8: push three {word, 0x28000, 0, 8, counter, 0} events
// onto the manager ring (counter = 1..3), after clearing the u16 event-status
// word. Returns whether the last push saw a live singleton.
// Push one {word, 0x28000, 0, 8, counter, 0} event; IPA-inlined so the body is
// colored in the caller's register-allocation context (retail shape).
static inline int CfNandPushEventMode8(u32 word, u32 counter) {
    cf::CfNandManager* mgr = lbl_eu_80664768;
    if (mgr == 0) {
        return 0;
    }
    CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                     mgr->mEventQueue.mSize];
    entry->mWord = (u32)word;
    entry->mTag = 0x28000;
    entry->mField8 = 0;
    entry->mFieldC = 8;
    entry->mFieldE = counter;
    entry->mFieldF = 0;
    mgr->mEventQueue.mTail++;
    return 1;
}

int func_8023FC18(void (*word)(u32, u32, u32, u32)) {
    if (lbl_eu_80664768 == 0) {
        return 0;
    }
    lbl_eu_8066476E = 0;
    u32 result;
    for (u32 counter = 1; counter <= 3; counter++) {
        result = CfNandPushEventMode8((u32)word, counter);
    }
    return result;
}

// Push an event {word, 0x10000, 0, 9, 0, 0} onto the manager's ring.
// Written as an IPA-inlined helper so the body gets colored in the caller's
// register-allocation context (retail shape).
static inline int CfNandPushEventMode9(u32 word) {
    u32 result;
    cf::CfNandManager* mgr = lbl_eu_80664768;
    if (mgr == 0) {
        result = 0;
    } else {
        CfNandEvent* entry =
            &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                        mgr->mEventQueue.mSize];
        entry->mWord = (u32)word;
        entry->mTag = 0x10000;
        entry->mField8 = 0;
        entry->mFieldC = 9;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
        result = 1;
    }
    return result;
}

int func_8023FCCC(void (*word)(u32, u32, u32, u32)) {
    if (lbl_eu_80664768 == 0) {
        return 0;
    }
    return CfNandPushEventMode9((u32)word);
}

// Target us-80241efc: push the mode-tagged event {mode, 0x10000, .., 0xA} then
// three {0, 0, .., 7, i} events for i = 1..3. Returns 1 on success.
extern "C" int func_8023FD4C(int mode) {
    if (lbl_eu_80664768 == 0) {
        return 0;
    }
    CfNandPushEvent(0x10000, 0xA, 0, (u32)mode);
    for (int i = 1; i <= 3; i++) {
        CfNandPushEvent(0, 7, (u8)i, 0);
    }
    return 1;
}

// Target us-8024208c ...
// Ctor-less mirror of CfNandEvent used as the retail sp+8..sp+0x17 scratch
// home (no implicit-initialization stores to hoist).
struct CfNandSaveScratch {
    u32 mWord;   // 0x00
    u32 mTag;    // 0x04
    u32 mField8; // 0x08
    u16 mFieldC; // 0x0C
    u8 mFieldE;  // 0x0E
    u8 mFieldF;  // 0x0F
};

s32 func_8023FEDC(u32 word) {
    cf::CfNandManager* mgr = lbl_eu_80664768;
    if (mgr == 0) {
        return 0;
    }
    s32 result = 0;
    mgr->field_184 = 0;
    u8* table = lbl_eu_80576AC0;
    lbl_eu_80664768->field_186 = 0;
    CfNandSaveScratch ev;
    // Volatile-qualified builds keep the scratch-home stores loop-resident.
    volatile CfNandSaveScratch* evp = &ev;
    for (s32 counter = 1; counter <= 3; counter++) {
        mgr = lbl_eu_80664768;
        if (mgr == 0) {
            result = 0;
            break;
        }
        evp->mWord = word;
        evp->mFieldC = 0x12;
        evp->mFieldE = (u8)counter;
        evp->mTag = 0x80;
        evp->mField8 = (u32)(table + (counter - 1) * 0x80);
        evp->mFieldF = 0;
        result = 1;
        CfNandEventQueue* q = &mgr->mEventQueue;
        CfNandEvent* slot =
            &q->mRingBase[(q->mHead + q->mTail) % q->mSize];
        slot->mWord = ev.mWord;
        slot->mTag = ev.mTag;
        slot->mField8 = ev.mField8;
        slot->mFieldC = ev.mFieldC;
        slot->mFieldE = ev.mFieldE;
        slot->mFieldF = ev.mFieldF;
        q->mTail++;
    }
    return result;
}

// Target us-80242188: CfNandManager factory (retail flat symbol
// create__Q22cf13CfNandManagerFv). Returns 0 while the singleton exists;
// otherwise allocates the 0x194-byte manager from work memory, runs the
// flat-name ctor, and registers it under pParent.
cf::CfNandManager* create__Q22cf13CfNandManagerFv(CProcess* pParent, CScn* pScene) {
    if (lbl_eu_80664768 != 0) {
        return 0;
    }
    cf::CfNandManager* mgr =
        (cf::CfNandManager*)mtl::MemManager::allocate(0x194, CWorkThreadSystem::getWorkMem());
    if (mgr != 0) {
        mgr = (cf::CfNandManager*)__ct__cf_CfNandManager(mgr, pScene);
    }
    mgr->Regist(pParent, false);
    return mgr;
}

// Returns whether a NAND file operation is in flight: reads the heap-buffer
// pointer at +0x5C of the CfNandManager singleton (null singleton -> 0).
u32 func_8024005C() {
    if (lbl_eu_80664768 != nullptr) {
        return lbl_eu_80664768->field_0x5C != 0;
    }
    return 0;
}

// Downscale the screen-sized RGBA8 EFB capture in lbl_eu_80664780 into the
// 164x116 tiled RGB565 texture at lbl_eu_8066477C (nearest-neighbour via
// per-axis screen/tile scale factors), flush it, and arm the counter to 3.
//
// The int->double conversions are spelled manually through the shared
// .sdata2 correction doubles (lbl_eu_806686F8 = 2^52 for unsigned sources,
// lbl_eu_80668700 = 2^52+2^31 for signed) so the pool relocs land on the
// retail labels instead of TU-local @N pool entries.
void func_80240084() {
    union { double d; u32 w[2]; } cu, cs;
    cu.w[0] = 0x43300000;
    cs.w[0] = 0x43300000;
    u32 size = GXGetTexBufferSize(0xa4, 0x74, 4, (GXBool)0, 0);
    cu.w[1] = (u32)CDeviceVI::getRenderModeObj()->fbWidth;
    cs.w[1] = 164u ^ 0x80000000;
    double scaleX = (cu.d - lbl_eu_806686F8) / (cs.d - lbl_eu_80668700);
    cu.w[1] = (u32)CDeviceVI::getRenderModeObj()->efbHeight;
    cs.w[1] = 116u ^ 0x80000000;
    double scaleY = (cu.d - lbl_eu_806686F8) / (cs.d - lbl_eu_80668700);
    // Cached per-loop constants: the clamp threshold stays as a float so the
    // compare widens it via lfs, and the signed correction double is hoisted
    // into an FPR for the whole pixel loop.
    const f32 th = lbl_eu_806686E0;
    const double cB = lbl_eu_80668700;
    for (int i = 0; i < 0x74; i++) {
        for (int j = 0; j < 0xa4; j++) {
            cs.w[1] = (u32)j ^ 0x80000000;
            double vx = (cs.d - cB) * scaleX;
            cs.w[1] = (u32)i ^ 0x80000000;
            double vy = (cs.d - cB) * scaleY;
            int x = vx > th ? vx + lbl_eu_806686E8 : vx + lbl_eu_806686F0;
            int y = vy > th ? vy + lbl_eu_806686E8 : vy + lbl_eu_806686F0;
            // Source: RGBA8 tile group - AR plane at tile*0x40, GB at +0x20.
            const u8* s =
                lbl_eu_80664780 +
                (((y >> 2) * (CDeviceVI::getRenderModeObj()->fbWidth >> 2) +
                  (x >> 2))
                 << 6) +
                (((y & 3) << 2) + (x & 3)) * 2;
            u32 px = (s[0] << 24) | (s[1] << 16) | (s[0x20] << 8) | s[0x21];
            // Pack RGBA8888 -> RGB565.
            u16 c = (u16)(((px >> 8) & 0xF800) | ((px >> 5) & 0x7E0) |
                          ((px >> 3) & 0x1F));
            // Dest: 32-byte RGB565 tiles, 164/4 tiles per row.
            u16* d = (u16*)lbl_eu_8066477C +
                     ((j >> 2) + (i >> 2) * 41) * 8 + (i & 3) * 2 + (j & 3);
            *d = c;
        }
    }
    lbl_eu_80664770 = 3;
    DCFlushRange(lbl_eu_8066477C, size);
}

// Target us-80242518: (re)build the NAND resource buffers: two GX texture
// buffer sizes (a fixed 0xa4x0x74 tile and a VI-size buffer) merged into one
// tail allocation, halves stored in lbl_eu_8066477C / lbl_eu_80664780. Then
// re-arm the u16 counter, pull the D80 scene, and (re)register the manager's
// IScnRender (+0x58 subobject) with the scene.
void func_80240360() {
    if (lbl_eu_8066477C == 0) {
        u32 size1 = GXGetTexBufferSize(0xa4, 0x74, 4, 0, 0);
        u16 efbH = CDeviceVI::getRenderModeObj()->efbHeight;
        u32 size2 = GXGetTexBufferSize(CDeviceVI::getRenderModeObj()->fbWidth, efbH, 6, 0, 0);
        u8* buf = (u8*)mtl::MemManager::allocate_tail(mtl::MemManager::getHandleMEM2(), size1 + size2, 0x20);
        lbl_eu_8066477C = buf;
        lbl_eu_80664780 = buf + size1;
    }
    lbl_eu_80664770 = 1;
    CScn* scene = (CScn*)CfRes_getD80Flag();
    scene->addRenderCB(static_cast<IScnRender*>(lbl_eu_80664768), 4, 0);
}

void func_80240420() {
    // Teardown: drop the NAND manager's render callback from the D80 scene,
    // free the heap buffer, and clear the u16 counter.
    CScn* scene = (CScn*)CfRes_getD80Flag();
    if (scene != nullptr) {
        scene->removeRenderCB(static_cast<IScnRender*>(lbl_eu_80664768));
    }
    if (lbl_eu_8066477C != nullptr) {
        mtl::MemManager::deallocate(lbl_eu_8066477C);
        lbl_eu_8066477C = nullptr;
    }
    lbl_eu_80664770 = 0;
}

void cf::CfNandManager::cbRenderBefore() {
    // While the counter is armed (1), copy the EFB into the shared texture
    // buffer and flush it, then disarm.
    if (lbl_eu_80664770 == 1) {
        // Rule A (decl order) + liveness: size1 declared before the block-3
        // height so the pair colors {size1=r30, height=r31}.
        u16 h1;
        u16 h2;
        u32 size1;
        h1 = CDeviceVI::getRenderModeObj()->efbHeight;
        GXSetTexCopySrc(0, 0, CDeviceVI::getRenderModeObj()->fbWidth, h1);
        h2 = CDeviceVI::getRenderModeObj()->efbHeight;
        GXSetTexCopyDst(CDeviceVI::getRenderModeObj()->fbWidth, h2, (GXTexFmt)6,
                        (GXBool)0);
        GXCopyTex(lbl_eu_80664780, (GXBool)0);
        GXPixModeSync();
        GXInvalidateTexAll();
        size1 = GXGetTexBufferSize(0xa4, 0x74, 4, (GXBool)0, 0);
        h1 = CDeviceVI::getRenderModeObj()->efbHeight;
        u32 size2 =
            GXGetTexBufferSize(CDeviceVI::getRenderModeObj()->fbWidth, h1, (GXTexFmt)6,
                               (GXBool)0, 0);
        DCFlushRange(lbl_eu_8066477C, size1 + size2);
        lbl_eu_80664770 = 2;
    }
}

void OnFileEvent__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))handleFileEvent__Q22cf13CfNandManagerFv)((char*)self - 0x54); }

void destroyViaWorkEvent__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))__dt__Q22cf13CfNandManagerFv)((char*)self - 0x54); }

void handleRenderCallback__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))cbRenderBefore__Q22cf13CfNandManagerFv)((char*)self - 0x58); }

extern "C" void destroyViaRender__Q22cf13CfNandManagerFv(cf::CfNandManager* self) { ((void(*)(void*))__dt__Q22cf13CfNandManagerFv)((char*)self - 0x58); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CTTask<T> is declared in kyoshin/CTaskGameEff.hpp (via harness_catalog.hpp);
// specializations below emit the retail Move/Draw/dtor symbols.
namespace cf { class CfNandManager; }
template<> CTTask<cf::CfNandManager>::~CTTask() {}

// Dispatch the +0x3C move callback through __ptmf_test/__ptmf_scall when set.
template<>
void CTTask<cf::CfNandManager>::Move() {
    if (__ptmf_test(&mMoveFunc)) {
        (static_cast<cf::CfNandManager*>(this)->*mMoveFunc)();
    }
}

// Dispatch the +0x48 draw callback through __ptmf_test/__ptmf_scall when set.
template<>
void CTTask<cf::CfNandManager>::Draw() {
    if (__ptmf_test(&mDrawFunc)) {
        (static_cast<cf::CfNandManager*>(this)->*mDrawFunc)();
    }
}
extern "C" void sinit_802405F0(void) {}
