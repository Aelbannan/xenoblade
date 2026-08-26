// CfMapMineManager - mine-point management (field collection points)
#include "kyoshin/cf/CfMapMineManager.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
// CfGameManager.hpp now declares the unified pointer-arg form; this TU's
// retail sites reproduce without materializing r3, so keep the TU-local
// no-arg decl from CfMapMineManager.hpp and rename the shared copy away.
#define CItem_initItemImplInstances mineMgrCfGameManagerCopyUnused
#include "kyoshin/cf/CfGameManager.hpp"
#undef CItem_initItemImplInstances
// High-level C++ reconstruction from retail ASM (US, 0x8020785C..0x80209D20).
// All 20 TU functions carry placeholder retail symbol names; they are emitted
// with C linkage so the object exports the exact retail symbols.

#include <types.h>
#include <decomp.h>
#include <string.h>
#include <math.h>

class BdatFilePointer;
struct CfMapMineManager;

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

// Mine point payload (0x24 bytes). Lives inside each reslist node at +0x08.
struct MinePoint {
    void* mObj0;      // 0x00  scene object A
    void* mObj4;      // 0x04  scene object B
    f32 mPosX;        // 0x08
    f32 mPosY;        // 0x0C
    f32 mPosZ;        // 0x10
    f32 mTimer14;     // 0x14
    u16 mId18;        // 0x18
    s16 mCounter1A;   // 0x1A
    u16 mPointId1C;   // 0x1C
    u8 mArea1E;       // 0x1E
    u8 mAreaSub1F;    // 0x1F
    union {
        u32 mFlags;   // 0x20  raw
        struct {
            u32 mFlagId : 10;    // value bits 22-31
            u32 mFlagKind : 6;   // value bits 16-21
            u32 mFlagCount : 8;  // value bits 8-15
            u32 mFlagLow : 8;    // value bits 0-7
        };
    };
};

// Intrusive reslist node: {next, prev, item}; stride 0x2C.
struct MineNode {
    MineNode* mNext;   // 0x00
    MineNode* mPrev;   // 0x04
    MinePoint mItem;   // 0x08
};

// Standalone reslist<MinePoint> layout (0x40 bytes).
struct MineListBase {
    void* mVtable;         // 0x00
    MineNode* mStartPtr;   // 0x04  (= &mStartNode)
    MineNode mStartNode;   // 0x08  (0x2C bytes -> ends at 0x34)
    MineNode* mList;       // 0x34
    u32 mCapacity;         // 0x38
    u8 mUnk03C;            // 0x3C
    u8 mPad03D[3];         // 0x3D
};

// One on-screen mine message (ring-buffer slot, 0x48 bytes).
struct MineMsg {
    char mText[0x40];  // 0x00
    u32 mLen;          // 0x40
    f32 mTime;         // 0x44
};

// Sound-effect timeout entry.
struct MineSoundTimer {
    u32 mId;           // 0x0
    f32 mTime;         // 0x4
};

// Ring buffer of 16 messages, embedded at +0x44 of the manager.
struct MineMsgRing {
    MineMsg mSlots[16];    // 0x000..0x480
    MineMsg* mBase;        // 0x480  (= &mSlots[0])
    u32 mReadIdx;          // 0x484
    u32 mCount;            // 0x488
    u32 mCapacity;         // 0x48C  (= 16)
};

struct CfMapMineManager {
    f32 mTime;             // 0x000
    MineListBase mPoints;  // 0x004  (0x40 bytes -> ends at 0x44)
    MineMsgRing mMsgs;     // 0x044  (0x490 bytes -> ends at 0x4D4)
    MineSoundTimer mSnd[16]; // 0x4D4..0x554
};

// Virtual interface view of the spawned scene object.
// setPosition sits at retail vtable offset 0x9C; unk158 at 0x158.
struct MineSceneObjectIf {
    virtual void v008();
    virtual void v00C();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01C();
    virtual void v020();
    virtual void v024();
    virtual void v028();
    virtual void v02C();
    virtual void v030();
    virtual void v034();
    virtual void v038();
    virtual void v03C();
    virtual void v040();
    virtual void v044();
    virtual void v048();
    virtual void v04C();
    virtual void v050();
    virtual void v054();
    virtual void v058();
    virtual void v05C();
    virtual void v060();
    virtual void v064();
    virtual void v068();
    virtual void v06C();
    virtual void v070();
    virtual void v074();
    virtual void v078();
    virtual void v07C();
    virtual void v080();
    virtual void v084();
    virtual void v088();
    virtual void v08C();
    virtual void v090();
    virtual void v094();
    virtual void v098();
    virtual void setPosition(void* position);
    virtual void unk0A0();
    virtual void unk0A4();
    virtual void unk0A8();
    virtual void* unk0AC();
    virtual void unk0B0();
    virtual void unk0B4();
    virtual void unk0B8();
    virtual void unk0BC();
    virtual void unk0C0();
    virtual void unk0C4();
    virtual void unk0C8();
    virtual void unk0CC();
    virtual void unk0D0();
    virtual void unk0D4();
    virtual void unk0D8();
    virtual void unk0DC();
    virtual void unk0E0();
    virtual void unk0E4();
    virtual void unk0E8();
    virtual void unk0EC();
    virtual void unk0F0();
    virtual void unk0F4();
    virtual void unk0F8();
    virtual void unk0FC();
    virtual void unk100();
    virtual void unk104();
    virtual void unk108();
    virtual void unk10C();
    virtual void unk110();
    virtual void unk114();
    virtual void unk118();
    virtual void unk11C();
    virtual void unk120();
    virtual void unk124();
    virtual void unk128();
    virtual void unk12C();
    virtual void unk130();
    virtual void unk134();
    virtual void unk138();
    virtual void unk13C();
    virtual void unk140();
    virtual void unk144();
    virtual void unk148();
    virtual void unk14C();
    virtual void unk150();
    virtual void unk154();
    virtual void unk158(int flag);
};

// ---------------------------------------------------------------------------
// Small helpers (high-level; inlined at -O4)
// ---------------------------------------------------------------------------

// Data-member view of the scene object (byte flag at 0x90).
struct MineSceneObjData {
    u8 mPad04[0x8C];
    u8 unk90;
};

static inline MineNode* ListHead(CfMapMineManager* m) { return m->mPoints.mStartPtr; }

// _reslist_base::clearList() - mark every slot free, reset the sentinel.
static inline void ListClearBase(MineListBase* L) {
    MineNode* start = L->mStartPtr;
    MineNode* n = start->mNext;
    while (n != start) {
        MineNode* cur = n;
        n = n->mNext;
        cur->mNext = 0;
    }
    start->mNext = start;
    start->mPrev = start;
}

static inline void ListClear(CfMapMineManager* m) { ListClearBase(&m->mPoints); }

// First free slot index (mNext == NULL); appends at mCapacity when full.
static inline u32 ListFindFree(CfMapMineManager* m) {
    u32 i = 0;
    while (i < m->mPoints.mCapacity) {
        if (((MineNode*)((u8*)m->mPoints.mList + i * 0x2C))->mNext == 0) break;
        i++;
    }
    return i;
}

// ---------------------------------------------------------------------------
// func_80205F78 - default constructor of a reslist array element (node).
// Zeroes the item's runtime fields, preserves mFlags id bits partially.
// ---------------------------------------------------------------------------
extern "C" void func_80205F78(MineNode* node) {
    node->mItem.mObj0 = 0;
    node->mItem.mObj4 = 0;
    node->mItem.mTimer14 = lbl_eu_806682B0;
    node->mItem.mId18 = 0;
    node->mItem.mPointId1C = 0;
    node->mItem.mFlags &= 0x003FFFFF;
    node->mItem.mFlags &= 0xFFFF00FF;
}

// ---------------------------------------------------------------------------
// __dt___reslist_base_cf_CfMapMineManager_MinePoint - base list destructor.
// ---------------------------------------------------------------------------
extern "C" MineListBase*
__dt___reslist_base_cf_CfMapMineManager_MinePoint(MineListBase* self,
                                                  s32 flags) {
    if (self != 0) {
        self->mVtable = lbl_eu_80535744;
        {
            MineNode* cur = self->mStartPtr->mNext;
            while (cur != self->mStartPtr) {
                MineNode* prev = cur;
                cur = cur->mNext;
                prev->mNext = 0;
            }
            self->mStartPtr->mNext = self->mStartPtr;
            self->mStartPtr->mPrev = self->mStartPtr;
        }
        if (self->mUnk03C == 0 && self->mList != 0) {
            // Doubled null check mirrors retail: MWCC keeps the dead second
            // beq (same shape as the derived-list dtor below).
            if (self->mList != 0) {
                __dla__FPv((u8*)self->mList - 0x10);
            }
            self->mList = 0;
        }
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// __dt__reslist_cf_CfMapMineManager_MinePoint - derived list destructor.
// ---------------------------------------------------------------------------
extern "C" MineListBase*
__dt__reslist_cf_CfMapMineManager_MinePoint(MineListBase* self, s32 flags) {
    MineNode* prev;
    MineNode* cur;
    // Doubled null check mirrors retail: MWCC keeps the dead second beq
    // (same shape as the matched base-list dtor above).
    if (self != 0) {
        if (self != 0) {
            self->mVtable = lbl_eu_80535744;
            cur = self->mStartPtr->mNext;
            while (cur != self->mStartPtr) {
                prev = cur;
                cur = cur->mNext;
                prev->mNext = 0;
            }
            self->mStartPtr->mNext = self->mStartPtr;
            self->mStartPtr->mPrev = self->mStartPtr;
            if (self->mUnk03C == 0 && self->mList != 0) {
                // Doubled null check mirrors retail: MWCC keeps the dead
                // second beq before the array-free call.
                if (self->mList != 0) {
                    __dla__FPv((u8*)self->mList - 0x10);
                }
                self->mList = 0;
            }
        }
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// __dt__80206124 - MineMsgRing destructor: resets the read cursor.
// ---------------------------------------------------------------------------
extern "C" MineMsgRing* __dt__80206124(MineMsgRing* self, s32 flags) {
    if (self != 0) {
        self->mCount = 0;
        self->mReadIdx = 0;
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// __dt__80206170 - full CfMapMineManager destructor.
// ---------------------------------------------------------------------------
extern "C" CfMapMineManager* __dt__80206170(CfMapMineManager* self, s32 flags) {
    if (self != 0) {
        // Inlined derived reslist<MinePoint> teardown on the embedded list.
        // The sentinel is re-loaded through the manager on every test (the
        // node stores may alias it), matching retail's reload-per-iteration.
        {
            MineNode* n = self->mPoints.mStartPtr->mNext;
            while (n != self->mPoints.mStartPtr) {
                MineNode* cur = n;
                n = n->mNext;
                cur->mNext = 0;
            }
            self->mPoints.mStartPtr->mNext = self->mPoints.mStartPtr;
            self->mPoints.mStartPtr->mPrev = self->mPoints.mStartPtr;
        }
        if (self->mPoints.mUnk03C == false) {
            if (self->mPoints.mList != 0) {
                // Doubled null check mirrors retail: MWCC keeps the dead
                // second beq before the array-free call.
                if (self->mPoints.mList != 0) {
                    __dla__FPv((u8*)self->mPoints.mList - 0x10);
                }
                self->mPoints.mList = 0;
            }
        }
        self->mPoints.mCapacity = 0;
        lbl_eu_806646A0 = 0;
        // Inlined ~MineMsgRing(&self->mMsgs, 0): MWCC retains the inlined
        // destructor's null-this branch even though the address is constant,
        // and keeps the member stores relative to the manager itself.
        if (&self->mMsgs != 0) {
            self->mMsgs.mCount = 0;
            self->mMsgs.mReadIdx = 0;
        }
        // Inlined ~_reslist_base on the +0x04 base subobject.
        MineListBase* pts = &self->mPoints;
        if (pts != 0) {
            if (pts != 0) {
                pts->mVtable = lbl_eu_80535744;
                MineNode* n = pts->mStartPtr->mNext;
                while (n != pts->mStartPtr) {
                    MineNode* cur = n;
                    n = n->mNext;
                    cur->mNext = 0;
                }
                pts->mStartPtr->mNext = pts->mStartPtr;
                pts->mStartPtr->mPrev = pts->mStartPtr;
                if (pts->mUnk03C == false) {
                    if (pts->mList != 0) {
                        if (pts->mList != 0) {
                            __dla__FPv((u8*)pts->mList - 0x10);
                        }
                        pts->mList = 0;
                    }
                }
            }
        }
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// Address-taken zero-timer init; taking the address keeps the aggregate's
// memory home slots (retail stores dead home copies at sp+8/sp+0xC).
// ---------------------------------------------------------------------------
static inline void InitTimerZero(MineSoundTimer* t) {
    t->mId = 0;
    t->mTime = lbl_eu_806682B0;
}

// ---------------------------------------------------------------------------
// Inlined slot/timer clearing helpers. Taking the bounds as parameters keeps
// MWCC from folding the trip count, which is what retail's div-based
// countdown loops require.
// ---------------------------------------------------------------------------
static inline void ClearMsgSlots(MineMsg* p, MineMsg* end) {
    for (; p < end; ++p) {
        p->mText[0] = 0;
        p->mLen = 0;
    }
}

static inline void ClearTimers(MineSoundTimer* p, MineSoundTimer* end, u32 id,
                               f32 time) {
    for (; p < end; ++p) {
        p->mId = id;
        p->mTime = time;
    }
}

static inline void ClearNodes(MineNode* p, MineNode* end) {
    for (; p < end; ++p) {
        p->mNext = 0;
    }
}

// ---------------------------------------------------------------------------
// __ct__80205A7C - CfMapMineManager constructor.
// Store order mirrors retail exactly (scheduler-sensitive).
// ---------------------------------------------------------------------------
extern "C" CfMapMineManager* __ct__80205A7C(CfMapMineManager* self) {
    // Retail reads the sentinel item's flags first and applies the two
    // inlined element-ctor masks as separate operations.
    u32 flags = self->mPoints.mStartNode.mItem.mFlags;
    flags &= 0x003FFFFF;

    f32 zero = lbl_eu_806682B0;
    self->mPoints.mVtable = lbl_eu_80535744;
    self->mTime = zero;

    self->mPoints.mStartNode.mItem.mObj0 = 0;
    self->mPoints.mStartNode.mItem.mObj4 = 0;
    self->mPoints.mStartNode.mItem.mTimer14 = zero;
    self->mPoints.mStartNode.mItem.mId18 = 0;
    self->mPoints.mStartNode.mItem.mPointId1C = 0;
    flags &= 0xFFFF00FF;
    self->mPoints.mStartNode.mItem.mFlags = flags;

    self->mPoints.mList = 0;
    self->mPoints.mCapacity = 0;
    self->mPoints.mUnk03C = false;
    self->mPoints.mStartPtr = &self->mPoints.mStartNode;
    self->mPoints.mStartNode.mNext = &self->mPoints.mStartNode;
    self->mPoints.mStartNode.mPrev = &self->mPoints.mStartNode;
    self->mPoints.mVtable = lbl_eu_8053572C;

    // Message slots: slot 0 is left untouched here; slots 1..15 get their
    // text head and length cleared.
    ClearMsgSlots(&self->mMsgs.mSlots[1], &self->mMsgs.mSlots[16]);

    // MineMsgRing header.
    self->mMsgs.mCapacity = 16;
    self->mMsgs.mBase = self->mMsgs.mSlots;
    self->mMsgs.mCount = 0;
    self->mMsgs.mReadIdx = 0;

    // Sound timers: timer 0 scalar, timers 1..15 via loop.
    self->mSnd[0].mId = 0;
    self->mSnd[0].mTime = zero;
    ClearTimers(&self->mSnd[1], &self->mSnd[16], 0, zero);

    lbl_eu_806646A0 = self;

    // Allocate the node array (150 nodes x 0x2C).
    u32 handle = func_80061FFC();
    void* mem = allocate_array__Q23mtl10MemManagerFUlUl(0x19D8, handle);
    self->mPoints.mList = (MineNode*)__construct_new_array(mem, (void*)func_80205F78, 0,
                                                           0x2C, 0x96);
    MineNode* nodes = self->mPoints.mList;
    ClearNodes(nodes, &nodes[150]);
    self->mPoints.mCapacity = 0x96;

    // Reset the message ring and sound timers through the singleton global
    // (retail re-reads lbl_eu_806646A0 between every store). The zeroed
    // timer pair lives in stack homes like the InitTimerZero helper shape.
    CfMapMineManager* mgr = lbl_eu_806646A0;
    if (mgr != 0) {
        MineNode* start = mgr->mPoints.mStartPtr;
        MineNode* n = start->mNext;
        while (n != start) {
            MineNode* cur = n;
            n = n->mNext;
            cur->mNext = 0;
        }
        start->mNext = start;
        start->mPrev = start;

        MineSoundTimer init;
        InitTimerZero(&init);

        lbl_eu_806646A0->mMsgs.mCount = init.mId;
        lbl_eu_806646A0->mMsgs.mReadIdx = init.mId;
        lbl_eu_806646A0->mTime = init.mTime;
        ClearTimers(&mgr->mSnd[0], &mgr->mSnd[16], init.mId, init.mTime);
    }
    return self;
}

// ---------------------------------------------------------------------------
// func_802062BC - global reset via the singleton.
// ---------------------------------------------------------------------------
extern "C" void func_802062BC() {
    MineSoundTimer init;
    CfMapMineManager* mgr = lbl_eu_806646A0;
    if (mgr == 0) return;

    // Inlined _reslist_base::clearList(): the sentinel pointer is re-read
    // through the manager on every iteration (the node stores may alias it).
    MineNode* n = mgr->mPoints.mStartPtr->mNext;
    while (n != mgr->mPoints.mStartPtr) {
        MineNode* cur = n;
        n = n->mNext;
        cur->mNext = 0;
    }
    mgr->mPoints.mStartPtr->mNext = mgr->mPoints.mStartPtr;
    mgr->mPoints.mStartPtr->mPrev = mgr->mPoints.mStartPtr;

    InitTimerZero(&init);

    // Retail re-reads the singleton global between every store (the list-node
    // stores above may alias it), so keep the explicit global references.
    lbl_eu_806646A0->mMsgs.mCount = init.mId;
    lbl_eu_806646A0->mMsgs.mReadIdx = init.mId;
    lbl_eu_806646A0->mTime = init.mTime;

    MineSoundTimer* snd = lbl_eu_806646A0->mSnd;
    for (int i = 0; i < 16; i++) {
        snd[i] = init;
    }
}

// ---------------------------------------------------------------------------
// func_80206388 - release every point's scene objects and reset state.
// ---------------------------------------------------------------------------
extern "C" void func_80206388(CfMapMineManager* self) {
    MineNode* n = self->mPoints.mStartPtr->mNext;
    while (n != self->mPoints.mStartPtr) {
        if (n->mItem.mObj0 != 0) {
            if (func_800B8920(n->mItem.mObj0) != 0) {
                func_800B9404(n->mItem.mObj0);
            }
            n->mItem.mObj0 = 0;
        }
        if (n->mItem.mObj4 != 0) {
            if (func_800B8920(n->mItem.mObj4) != 0) {
                func_800B9404(n->mItem.mObj4);
            }
            n->mItem.mObj4 = 0;
        }
        n->mItem.mObj0 = 0;
        n->mItem.mObj4 = 0;
        n = n->mNext;
    }

    // Address-taken temp keeps the dead home slots at sp+8/sp+0xC (retail).
    MineSoundTimer t;
    InitTimerZero(&t);

    self->mMsgs.mCount = 0;
    self->mMsgs.mReadIdx = 0;
    MineSoundTimer* snd = self->mSnd;
    int i = 16;
    do {
        snd[16 - i] = t;
        i--;
    } while (i != 0);
    self->mTime = lbl_eu_806682B0;
}

// ---------------------------------------------------------------------------
// func_802064A8 - populate one MinePoint from the mine-list bdat row.
// Returns (rowValue18 != 0).
// ---------------------------------------------------------------------------

extern "C" int func_802064A8(CfMapMineManager* self, u32 rowId, MinePoint* pt,
                             int isNew) {
    BdatFilePointer* file = lbl_eu_806640C8;
    const char* cols = lbl_eu_80508424;

    // Row scratch: retail keeps every bdat-derived value memory-resident.
    struct MineRow {
        f32 posX;
        f32 posY;
        f32 posZ;
        u16 val26;
        u16 val2D;
        u8 val34;
    } row;

    // Coordinates are stored as scaled bdat ints: value * 1.0e-4. The signed
    // int->double conversion is what retail emits as the 0x4330/xoris pair
    // with the shared 2^52+2^31 fixup double.
    // Coordinates are stored as scaled bdat ints: value * 1.0e-4. The signed
    // int->double conversion is emitted by MWCC as the 0x4330/xoris pair with
    // the shared 2^52+2^31 fixup double.
    row.posX =
        (f32)(s32)getBdatStringColumnValue(file, cols + 0x10, rowId) *
        lbl_eu_806682B4;
    row.posY =
        (f32)(s32)getBdatStringColumnValue(file, cols + 0x15, rowId) *
        lbl_eu_806682B4;
    row.posZ =
        (f32)(s32)getBdatStringColumnValue(file, cols + 0x1A, rowId) *
        lbl_eu_806682B4;
    row.val26 = (u16)getBdatStringColumnValue(file, cols + 0x1F, rowId);
    row.val2D = (u16)getBdatStringColumnValue(file, cols + 0x26, rowId);
    row.val34 = (u8)getBdatStringColumnValue(file, cols + 0x2D, rowId);

    if (row.val26 != 0) {
        // Flags: bits 22-31 = row id, bits 12-14 cleared,
        // bit 15 = "kind byte != 2".
        u32 fl = pt->mFlags;
        fl = (fl & ~(0x3FFu << 22)) | ((rowId & 0x3FF) << 22);
        fl &= ~(0x7u << 12);
        fl = (fl & ~(1u << 15)) | ((u32)(row.val34 != 2) << 15);
        pt->mPosX = row.posX;
        pt->mPosY = row.posY;
        pt->mPosZ = row.posZ;
        pt->mId18 = row.val26;
        pt->mPointId1C = row.val2D;
        pt->mObj0 = 0;
        pt->mObj4 = 0;
        pt->mFlags = fl;
        pt->mArea1E = (u8)lbl_eu_80663E42;
        pt->mAreaSub1F = (u8)lbl_eu_80663E44;
        pt->mCounter1A = 0;
        if (isNew != 0) {
            // Retail re-reads the bdat handle global into a fresh local here.
            BdatFilePointer* file2 = lbl_eu_806640C8;
            u8 lo = (u8)getBdatStringColumnValue(file2, cols + 0x00, rowId);
            u8 hi = (u8)getBdatStringColumnValue(file2, cols + 0x08, rowId);
            int r = mtRand__Q22ml4mathFi((hi - lo) + 1);
            pt->mTimer14 = lbl_eu_806682B0;
            u32 fl2 = pt->mFlags;
            fl2 = (fl2 & ~(0xFFu << 8)) | (((lo + r) & 0xFF) << 8);
            pt->mFlags = fl2;
        }
    }
    return row.val26 != 0;
}

// ---------------------------------------------------------------------------
// func_802066A8 - (re)spawn the scene object for a mine point.
// ---------------------------------------------------------------------------
struct MineNameBuf {
    char mText[0x40];
    u32 mLen;
};

extern "C" int func_802066A8(CfMapMineManager* self, MinePoint* pt) {
    if (pt->mArea1E != (u8)lbl_eu_80663E42 ||
        pt->mAreaSub1F != (u8)lbl_eu_80663E44) {
        return 0;
    }
    if (pt->mObj4 != 0) {
        return 1;
    }

    MineNameBuf name;
    void* obj = func_800B20B4(func_800B07E8__Fv(), 0x4000, 0, 0);
    if (obj != 0) {
        pt->mObj4 = obj;

        const char* cols = lbl_eu_80508424;
        void* fp = getFP__FPCc(cols + 0x37);
        u32 id = pt->mFlags >> 22;
        u32 resId = getBdatStringColumnValue(
            lbl_eu_806640C8, cols + 0x45, id);
        name.mText[0] = 0;
        const char* nm = (const char*)getBdatStringColumnValue(
            (BdatFilePointer*)fp, cols + 0x4E, (u16)resId);
        name.mLen = strlen(nm);
        strcpy(name.mText, nm);
        func_800C13FC(obj, &name, 0xC);
        ((MineSceneObjectIf*)obj)->unk158(1);
        ((MineSceneObjectIf*)obj)->setPosition(&pt->mPosX);
        ((MineSceneObjData*)obj)->unk90 = 0;
    }

    return pt->mObj4 != 0;
}

// ---------------------------------------------------------------------------
// func_80207C08 - true when no active point matches (id, area, sub).
// ---------------------------------------------------------------------------
extern "C" int func_80207C08(u32 pointId, int area, int sub) {
    if (lbl_eu_806646A0 == 0 || pointId == 0) {
        return 1;
    }
    MineNode* head = lbl_eu_806646A0->mPoints.mStartPtr;
    MineNode* n = head->mNext;
    while (n != head) {
        if (n->mItem.mId18 != 0 && (n->mItem.mFlagKind & 1) != 0 &&
            area == n->mItem.mArea1E && sub == n->mItem.mAreaSub1F &&
            pointId == n->mItem.mPointId1C &&
            (*(volatile u32*)&n->mItem.mFlags >> 8 & 0xFF) != 0) {
            return 0;
        }
        n = n->mNext;
    }
    return 1;
}

// ---------------------------------------------------------------------------
// func_80207C94 - snapshot active points into a 6-byte record array.
// ---------------------------------------------------------------------------
struct MineSnapshot {
    s16 mTimer;
    u8 mCount;
    u8 mKind;
    u8 mArea;
    u8 mSub;
};

extern "C" void func_80207C94(u8* out) {
    memset(out, 0, 0x384);
    MineNode* n = lbl_eu_806646A0->mPoints.mStartPtr->mNext;
    while (n != lbl_eu_806646A0->mPoints.mStartPtr) {
        MineSnapshot* snapshot = (MineSnapshot*)out;
        snapshot->mTimer = (s16)n->mItem.mTimer14;
        snapshot->mCount = (u8)((n->mItem.mFlags >> 8) & 0xFF);
        snapshot->mKind = (u8)DECOMP_PPC_RLWINM(n->mItem.mFlags, 10, 24, 31);
        snapshot->mArea = n->mItem.mArea1E;
        snapshot->mSub = n->mItem.mAreaSub1F;
        out += 6;
        n = n->mNext;
    }
}

// ---------------------------------------------------------------------------
// func_80207B24 - play a collection sound effect based on drop kind.
// ---------------------------------------------------------------------------
extern "C" void func_80207B24(CfMapMineManager* self, u32 kind, void* pos) {
    int sfx = 0;
    if (kind == 4) sfx = 0xB;
    else if (kind == 5) sfx = 0xC;
    else if (kind == 6) sfx = 0xD;
    else if (kind == 7) sfx = 0xE;
    else if (kind == 8) sfx = 0xF;
    else if (kind == 9) sfx = 0x10;

    void* player = cf::CfGameManager::getPlayer(0);
    if (player == 0) return;
    void* obj = func_8008187C__Q22cf13CfGameManagerFv(sfx);
    if (obj == 0) return;
    func_800ACF78(obj, player, 0);
    ((MineSceneObjectIf*)obj)->setPosition(pos);
}

// ---------------------------------------------------------------------------
// func_80206FA8 - per-point update: timers, respawn, LOD registration.
// ---------------------------------------------------------------------------
extern "C" void func_80206FA8(CfMapMineManager* self, MinePoint* pt) {
    // Declaration order mirrors retail register assignment:
    // r27=area, r30=self, r28=sub, r31=pt, f31=dt.
    u32 area = lbl_eu_80663E42;
    u32 sub = lbl_eu_80663E44;
    f32 dt = func_80496288((void*)CfRes_getD80Flag());

    if (pt->mTimer14 > lbl_eu_806682B0) {
        // Duplicate active point: let time advance (retail branches out
        // with beq when the scan returns zero).
        if (func_80207C08(pt->mPointId1C, pt->mArea1E, pt->mAreaSub1F) != 0) {
            pt->mTimer14 = pt->mTimer14 - dt;
        }
        return;
    }

    u32 fl = pt->mFlags;
    pt->mTimer14 = lbl_eu_806682B0;
    if (((fl >> 8) & 0xFF) != 0) return;
    if (area != pt->mArea1E) return;
    if (sub != pt->mAreaSub1F) return;

    BdatFilePointer* file = lbl_eu_806640C8;
    const char* cols = lbl_eu_80508424;
    u32 id = fl >> 22;
    u8 lo = (u8)getBdatStringColumnValue(file, cols, id);
    u8 hi = (u8)getBdatStringColumnValue(file, cols + 8, id);
    int r = mtRand__Q22ml4mathFi((hi - lo) + 1);
    u32 f2 = pt->mFlags;
    f2 = (f2 & 0xFFFF00FF) | ((lo + r) << 8);
    pt->mFlags = f2;

    if (((f2 >> 8) & 0xFF) != 0) {
        if (((f2 >> 15) & 1) != 0) {
            // (double)(160 - flagBit): 0x4330 int->double sequence minus
            // the 160.0 sdata double literal, passed raw in fp1.
            func_80462E58__8CTaskLODFv(
                pt->mPointId1C, 1,
                (f64)(0xA0 - ((lbl_eu_80663E24 >> 20) & 1)) -
                    lbl_eu_806682C0);
        } else {
            void* v = func_80186BC8(pt->mPointId1C);
            if (v != 0) {
                // Retail reuses the func_80186BC8 result register as the
                // id argument (r3 flows straight through).
                func_800BFBF4((u16)(u32)v, 1);
            }
        }
    }
    func_802066A8(self, pt);
}

// ---------------------------------------------------------------------------
// func_802073CC - advance message-ring timers, pop the oldest when expired.
// ---------------------------------------------------------------------------
extern "C" void func_802073CC(CfMapMineManager* self) {
    f32 dt = func_80496288((void*)CfRes_getD80Flag());

    int changed = 0;
    u32 cnt = self->mMsgs.mCount;
    for (u32 i = 0; i < cnt; i++) {
        // Slots are addressed through the stored base pointer.
        MineMsg* slot =
            &self->mMsgs.mBase[(self->mMsgs.mReadIdx + i) % self->mMsgs.mCapacity];
        if (slot->mTime >= lbl_eu_806682B0) {
            f32 t = slot->mTime - dt;
            slot->mTime = t;
            if (t < lbl_eu_806682D8) {
                slot->mTime = lbl_eu_806682B0;
                changed = 1;
            }
        }
    }

    if (changed != 0) {
        // Pop the oldest entry: the cursor walks backwards through the
        // ring, so both the slot address and the updated cursor use a
        // NEGATED remainder (retail computes base - rem*0x48 and stores
        // -(next % cap) into mReadIdx).
        u32 rd = self->mMsgs.mReadIdx;
        MineMsg* oldSlot =
            &self->mMsgs.mBase[-(rd % self->mMsgs.mCapacity)];
        func_801352A4(oldSlot);
        s32 next = rd + 1;
        self->mMsgs.mCount = self->mMsgs.mCount - 1;
        self->mMsgs.mReadIdx =
            -(next % (s32)self->mMsgs.mCapacity);
        // Second, un-modulo'd address of the expiring slot (retail computes
        // this separately after updating the ring cursors).
        MineMsg* msg = &self->mMsgs.mBase[rd];
        char buf[0x40];
        u32 len = strlen(msg->mText);
        strcpy(buf, msg->mText);
        f32 t = msg->mTime;
        (void)len;
        (void)t;
        (void)buf;
    }
}

// ---------------------------------------------------------------------------
// func_8020712C - find the nearest activatable point to a position.
// ---------------------------------------------------------------------------
// For each point: refresh timers via func_80206FA8, gate on the player-id
// bdat range, then compare horizontal distance (X/Z only; Y is only used as
// an absolute threshold of 50).
extern "C" void func_8020712C(MineNode** out, CfMapMineManager* mgr,
                              CfMapMineManager* list, f32* pos) {
    BdatFilePointer* file = lbl_eu_806640C8;
    u16 area = lbl_eu_80663E42;
    u16 sub = lbl_eu_80663E44;
    u32 playerId = cf::CfGameManager::func_800822F4();
    *out = 0;
    playerId &= 0xFFFF;

    u32 marker;
    if (func_800FE68C() != 0) {
        marker = *(u32*)((u8*)func_800FE68C() + 0x90E4);
    } else {
        marker = 0;
    }

    const char* cols = lbl_eu_80508424;
    void* released = 0;
    MineNode* n = list->mPoints.mStartPtr->mNext;
    // Retail re-reads the sentinel from the list object on every iteration.
    while (n != list->mPoints.mStartPtr) {
        n->mItem.mFlags &= ~0x00060000u;
        if (n->mItem.mTimer14 > lbl_eu_806682B0 ||
            ((n->mItem.mFlags >> 8) & 0xFF) == 0) {
            func_80206FA8(mgr, &n->mItem);
        } else if (area != n->mItem.mArea1E || sub != n->mItem.mAreaSub1F) {
            ;
        } else {
            u16 lo = (u16)getBdatStringColumnValue(file, cols + 0x60,
                                                   n->mItem.mFlags >> 22);
            u16 hi = (u16)getBdatStringColumnValue(file, cols + 0x66,
                                                   n->mItem.mFlags >> 22);
            if (playerId < lo || playerId > hi) {
                if (n->mItem.mObj0 != 0) {
                    func_800ACC14(n->mItem.mObj0, 1);
                    n->mItem.mObj0 = released;
                }
            } else {
                n->mItem.mFlags |= 0x00040000;
                f32 diff[3];
                diff[0] = n->mItem.mPosX - pos[0];
                diff[1] = n->mItem.mPosY - pos[1];
                diff[2] = n->mItem.mPosZ - pos[2];
                f32 dx = diff[0];
                f32 dy = diff[1];
                f32 dz = diff[2];
                f32 ady = (f32)__fabs((f64)dy);
                f32 dist2 = dx * dx + dz * dz;
                if (ady > lbl_eu_806682C8 || dist2 > lbl_eu_806682CC) {
                    if (n->mItem.mObj0 != 0) {
                        *(u32*)((u8*)n->mItem.mObj0 + 0x68) |= 0x40;
                        n->mItem.mObj0 = released;
                    }
                } else {
                    if (n->mItem.mObj0 != 0 && n->mItem.mCounter1A > 0) {
                        n->mItem.mCounter1A -= 1;
                    }
                    if (n->mItem.mCounter1A <= 0 && ady < lbl_eu_806682D0 &&
                        dist2 < lbl_eu_806682D4 && n->mItem.mObj4 != 0) {
                        n->mItem.mFlags |= 0x00020000;
                        if (*(u32*)((u8*)n->mItem.mObj4 + 0x74) == marker) {
                            *out = n;
                        }
                    }
                }
            }
        }
        n = n->mNext;
    }
}

// ---------------------------------------------------------------------------
// func_80206BD4 - main per-frame point scan: spawn/despawn and insert new.
// ---------------------------------------------------------------------------
extern "C" void func_80206BD4(CfMapMineManager* self) {
    BdatFilePointer* file = lbl_eu_806640C8;
    u32 rowBegin = func_8003B41C(file);
    u32 rowEnd = rowBegin + func_8003B1EC(file);
    u16 area = lbl_eu_80663E42;
    u16 sub = lbl_eu_80663E44;

    MineNode* start = self->mPoints.mStartPtr;
    MineNode* n = start->mNext;
    while (n != start) {
        if (n->mItem.mObj0 != 0) {
            if (func_800B8920(n->mItem.mObj0) != 0) {
                func_800B9404(n->mItem.mObj0);
            }
            n->mItem.mObj0 = 0;
        }
        if (n->mItem.mObj4 != 0) {
            if (func_800B8920(n->mItem.mObj4) != 0) {
                func_800B9404(n->mItem.mObj4);
            }
            n->mItem.mObj4 = 0;
        }
        n->mItem.mObj0 = 0;
        n->mItem.mObj4 = 0;
        n = n->mNext;
    }

    self->mMsgs.mCount = 0;
    self->mMsgs.mReadIdx = 0;
    for (int i = 0; i < 16; i++) {
        self->mSnd[i].mId = 0;
        self->mSnd[i].mTime = 0.0f;
    }
    self->mTime = 0.0f;

    MinePoint tmp;
    for (u32 row = rowBegin; row < rowEnd; row++) {
        MineNode* found = 0;
        MineNode* s2 = self->mPoints.mStartPtr;
        MineNode* m = s2->mNext;
        while (m != s2) {
            if ((m->mItem.mFlags >> 22) == (row & 0xFFFF) &&
                (u8)area == m->mItem.mArea1E &&
                (u8)sub == m->mItem.mAreaSub1F) {
                found = m;
                break;
            }
            m = m->mNext;
        }

        if (found != 0) {
            func_802064A8(self, row, &found->mItem, 0);
            if (found->mItem.mTimer14 < lbl_eu_8066A208) {
                func_802066A8(self, &found->mItem);
                if ((found->mItem.mFlags & 0x00010000) != 0) {
                    u32 g = lbl_eu_80663E24;
                    func_80462E58__8CTaskLODFv(
                        found->mItem.mPointId1C, 1,
                        (f64)(0xA0 - ((g >> 20) & 1)) - lbl_eu_806682C0);
                } else {
                    if (func_80186BC8(found->mItem.mPointId1C) != 0) {
                        func_800BFBF4(found->mItem.mPointId1C, 1);
                    }
                }
            } else if (found->mItem.mArea1E == (u8)area &&
                       found->mItem.mAreaSub1F == (u8)sub) {
                if (func_80207C08(found->mItem.mPointId1C,
                                  found->mItem.mArea1E,
                                  found->mItem.mAreaSub1F) != 0) {
                    if ((found->mItem.mFlags & 0x00010000) != 0) {
                        u32 g = lbl_eu_80663E24;
                        func_80462E58__8CTaskLODFv(
                            found->mItem.mPointId1C, 0,
                            (f64)(0xA0 - ((g >> 20) & 1)) - lbl_eu_806682C0);
                    } else {
                        if (func_80186BC8(found->mItem.mPointId1C) != 0) {
                            func_800BFBF4(found->mItem.mPointId1C, 2);
                        }
                    }
                }
            }
        } else {
            if (func_802064A8(self, row, &tmp, 1) != 0) {
                func_802066A8(self, &tmp);
                u32 idx = ListFindFree(self);
                MineNode* slot = (MineNode*)((u8*)self->mPoints.mList + idx * 0x2C);
                MinePoint* dst = &slot->mItem;
                *(u32*)((u8*)dst + 0x00) = *(u32*)((u8*)&tmp + 0x00);
                *(u32*)((u8*)dst + 0x04) = *(u32*)((u8*)&tmp + 0x04);
                *(f32*)((u8*)dst + 0x08) = *(f32*)((u8*)&tmp + 0x08);
                *(f32*)((u8*)dst + 0x0C) = *(f32*)((u8*)&tmp + 0x0C);
                *(f32*)((u8*)dst + 0x10) = *(f32*)((u8*)&tmp + 0x10);
                *(f32*)((u8*)dst + 0x14) = *(f32*)((u8*)&tmp + 0x14);
                *(u16*)((u8*)dst + 0x18) = *(u16*)((u8*)&tmp + 0x18);
                *(s16*)((u8*)dst + 0x1A) = *(s16*)((u8*)&tmp + 0x1A);
                *(u16*)((u8*)dst + 0x1C) = *(u16*)((u8*)&tmp + 0x1C);
                *(u8*)((u8*)dst + 0x1E) = *(u8*)((u8*)&tmp + 0x1E);
                *(u8*)((u8*)dst + 0x1F) = *(u8*)((u8*)&tmp + 0x1F);
                *(u32*)((u8*)dst + 0x20) = *(u32*)((u8*)&tmp + 0x20);
                // push_back into the intrusive list
                MineNode* head = self->mPoints.mStartPtr;
                slot->mNext = head;
                slot->mPrev = head->mPrev;
                head->mPrev->mNext = slot;
                head->mPrev = slot;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// func_802067E4 - generate a collectible item drop for a mine point.
// ---------------------------------------------------------------------------
// Drop payload built into the caller's buffer (0x34 bytes initialized).
struct MineDrop {
    u32 mHead00;           // 0x00
    u16 mField04;          // 0x04
    u8 mByte06;            // 0x06
    u8 mKind07;            // 0x07 (low 6 bits = drop sfx kind)
    u8 mPad08[0x16 - 0x08];
    u8 mState16;           // 1 = normal drop, 2 = rare drop
    u8 mPad17[0x34 - 0x17];
};

extern "C" int func_802067E4(CfMapMineManager* self, MinePoint* pt,
                             MineDrop* item, u8* outFlag) {
    if (lbl_eu_806646A8 == 0) {
        lbl_eu_806646A4 = strlen(lbl_eu_80662758) - 1;
        lbl_eu_806646A8 = 1;
    }

    BdatFilePointer* file = lbl_eu_806640C8;
    *outFlag = 0;
    memset(item, 0, 0x34);
    const char* cols = lbl_eu_80508424;

    // Resolve the bdat row id for this mine point's flag id.
    u16 rowVal = getBdatStringColumnValue(file, cols + 0x1F, pt->mFlags >> 22);
    if (rowVal == 0) {
        return 0;
    }

    func_801583E0(item);
    CItemImplInstances* impl = CItem_initItemImplInstances();
    ((CItemInstVt1C*)impl)->_v1C(item);

    u8 colA;
    u8 colB;
    func_80157F04(rowVal, &colA, &colB);
    u8 kind = getBdatStringColumnValue(file, cols + 0x53, colB);

    // Sum lottery weights over the active inventory categories (1-8).
    u32* entry = (u32*)((u8*)func_8009ECB0() + 0x4);
    u32 total = 0;
    for (int i = 0; i < 9; i++, entry++) {
        u32 v = *entry;
        if (v >= 1 && v <= 8) {
            u8* rec = (u8*)func_8009EC9C((u16)v);
            u8* data = rec + 0x3534;
            if (data != NULL && func_8026178C(data, 0x91) != 0) {
                total += func_8025FB10(data, 0x91);
            }
        }
    }

    // Rare-drop gate: (weighted ^ rnd) sign trick on the top bit.
    int rnd = mtRand__Q22ml4mathFi(10000);
    u32 weighted = (total + lbl_eu_80662760[(pt->mFlags >> 16) & 1]) * 100;
    u32 x = weighted ^ rnd;
    u32 q = ((s32)x >> 1) - (x & weighted);
    *outFlag = q >> 31;
    if (*outFlag != 0) {
        item->mState16 = 2;
    } else {
        item->mState16 = 1;
    }

    // Area 4 raises the drop kind to a global cap.
    u16 cap = func_8009CF8C(0x800);
    if (cap > kind && lbl_eu_80663E42 == 4) {
        kind = cap;
    }
    func_801570A0(item, kind);

    int rangeIdx = ((pt->mFlags >> 16) & 1) * 2 + *outFlag;
    u8 lo = lbl_eu_80662750[rangeIdx * 2];
    u8 hi = lbl_eu_80662750[rangeIdx * 2 + 1];

    for (int k = 0; k < 4; k++) {
        CItemImplInstances* im = CItem_initItemImplInstances();
        ((CItemInstVt50*)im)->_v50(item, k, 0);
        CItemImplInstances* im2 = CItem_initItemImplInstances();
        ((CItemInstVt68*)im2)->_v68(item, k, 0);
    }

    // Rare drops allow up to 4 items, normal drops only 2 (need at least 1).
    int need, limit;
    if ((pt->mFlags & 0x00010000) != 0) {
        need = 2;
        limit = 4;
    } else {
        need = 1;
        limit = 2;
    }

    u16 picked[4];
    memset(picked, 0, sizeof(picked));
    u16* dst = picked;
    int attempts = 0;
    int got = 0;
    int nameLen = lbl_eu_806646A4;

    // Lottery: per-attempt column names are formed by patching one character
    // of the scratch name buffers ('1'..'4'); roll against the rate column.
    for (;;) {
        lbl_eu_80662758[nameLen] = attempts + '1';
        lbl_eu_80535720[nameLen] = attempts + '1';
        u16 itemId = getBdatStringColumnValue(file, lbl_eu_80662758,
                                              pt->mFlags >> 22);
        u8 rateRaw = getBdatStringColumnValue(file, lbl_eu_80535720,
                                              pt->mFlags >> 22);
        u16 effRate = rateRaw;
        if (*outFlag != 0 && rateRaw != 0) {
            effRate = rateRaw + 100;
        }
        int roll = mtRand__Q22ml4mathFi(100);
        if (itemId != 0 && roll < effRate) {
            int dup = 0;
            for (int j = 0; j < got; j++) {
                if (itemId == picked[j]) {
                    dup = 1;
                    break;
                }
            }
            if (dup == 0) {
                u16 qty = mtRand__Q22ml4mathFii(lo, hi);
                CItemImplInstances* im = CItem_initItemImplInstances();
                ((CItemInstVt50*)im)->_v50(item, got, itemId);
                CItemImplInstances* im2 = CItem_initItemImplInstances();
                ((CItemInstVt68*)im2)->_v68(item, got, qty);
                *dst++ = itemId;
                got++;
                if (got >= limit) {
                    return 1;
                }
            }
        }
        attempts++;
        if (attempts < limit) continue;
        attempts %= limit;
        if (got < need) continue;
        return 1;
    }
}

// ---------------------------------------------------------------------------
// func_802074F0 - top-level update: timers, nearest point, collection input.
// ---------------------------------------------------------------------------
extern "C" void func_802074F0(CfMapMineManager* self) {
    MineNode* nearest;
    CfMapMineManager* mgr = self;
    void* player = cf::CfGameManager::getPlayer(0);
    if (player == 0) return;

    func_802073CC(mgr);
    f32 dt = func_80496288((void*)CfRes_getD80Flag());
    const f32 zero = lbl_eu_806682B0;
    const f64 zeroD = lbl_eu_806682D8;

    // Sound-timer countdown over the 16 fixed slots.
    MineSoundTimer* snd = mgr->mSnd;
    MineSoundTimer* sndEnd = &mgr->mSnd[16];
    for (; snd != sndEnd; ++snd) {
        if (snd->mId != 0) {
            snd->mTime -= dt;
            if (snd->mTime <= zeroD) {
                func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, snd->mId, 0, 0,
                                                           lbl_eu_806682E4);
                snd->mId = 0;
                snd->mTime = zero;
            }
        }
    }

    // Player position (virtual getter at vtable+0xAC); Y gets a small offset.
    f32 pos[3];
    {
        void* pv = ((MineSceneObjectIf*)player)->unk0AC();
        pos[0] = ((f32*)pv)[0];
        pos[1] = ((f32*)pv)[1] + lbl_eu_806682E8;
        pos[2] = ((f32*)pv)[2];
    }

    if (mgr->mTime > zero) {
        mgr->mTime -= func_80496288((void*)CfRes_getD80Flag());
        if (mgr->mTime < zero) {
            mgr->mTime = zero;
        }
        return;
    }

    // Scan window open: clear the collected flag and find the nearest point.
    lbl_eu_80663E24 &= ~0x80000000u;
    func_8020712C(&nearest, mgr, (CfMapMineManager*)((u8*)mgr + 0x4), pos);
    if (nearest == 0) return;
    if (func_8007F91C__Q22cf13CfGameManagerFv() != 0) return;

    u32 fl = nearest->mItem.mFlags;
    int ready = 0;
    if ((fl & 0x00020000) != 0) {
        if (((fl >> 8) & 0xFF) != 0) {
            ready = 1;
        }
    }
    if (ready == 0) return;
    if (func_8013EB90(1) != 0) return;
    if (func_800FF738() != 0) return;
    if ((lbl_eu_80663E24 & 0xAFA40000) != 0) return;
    if (func_80084BF4__Q22cf13CfGameManagerFv() != 0) return;
    UnkClass_800821F8* unk = func_800821F8__Q22cf13CfGameManagerFv();
    if (unk != 0) {
        if ((*(u32*)((u8*)unk + 0x4) & 0x01000000) != 0) return;
    }
    if (func_80085840__Q22cf13CfGameManagerFv() == 0) return;
    if ((lbl_eu_80663E24 & 0x00002000) != 0) return;
    void* pad = cf::CfGameManager::getCurrentPad();
    if ((*(u32*)((u8*)pad + 0x4) & 0x00000010) == 0) return;

    // Generate the drop payload for this mine point.
    MineDrop drop;
    u8 dropFlag = 0;
    drop.mField04 = 0;
    drop.mHead00 = 0;
    if (func_802067E4(mgr, &nearest->mItem, &drop, &dropFlag) == 0) return;

    if (func_801599D4(&drop, 0) == 0) {
        const char* cols = lbl_eu_80508424;
        u32 msg = getBdatStringColumnValue(
            (BdatFilePointer*)getFP__FPCc(cols + 0x77), cols + 0x4E, 0x1B);
        func_8013D55C((void*)msg, 0, 0);
        return;
    }

    // Collection accepted: consume one unit from the point's counter.
    u32 fl2 = nearest->mItem.mFlags;
    u32 cnt = (fl2 >> 8) & 0xFF;
    if (cnt != 0) {
        cnt--;
        fl2 = (fl2 & 0xFFFF00FF) | (cnt << 8);
        nearest->mItem.mFlags = fl2;
        if (cnt == 0) {
            if (nearest->mItem.mObj4 != 0) {
                if (func_800B8920(nearest->mItem.mObj4) != 0) {
                    func_800B9404(nearest->mItem.mObj4);
                    nearest->mItem.mObj4 = 0;
                }
                nearest->mItem.mObj4 = 0;
            }
            // Rare points only despawn when no duplicate remains active.
            if ((fl2 & 0x00010000) == 0 ||
                func_80207C08(nearest->mItem.mPointId1C, nearest->mItem.mArea1E,
                              nearest->mItem.mAreaSub1F) != 0) {
                // Dispatcher re-reads the flags fresh from memory.
                fl2 = nearest->mItem.mFlags;
                if ((fl2 & 0x00010000) != 0) {
                    u32 bit = (lbl_eu_80663E24 >> 20) & 1;
                    func_80462E58__8CTaskLODFv(
                        nearest->mItem.mPointId1C, 0,
                        (f64)(0xA0 & ~(u32)-(s32)bit) - lbl_eu_806682C0);
                } else {
                    void* v = func_80186BC8(nearest->mItem.mPointId1C);
                    if (v != 0) {
                        func_800BFBF4((u16)(u32)v, 2);
                    }
                }
            }
            // Respawn delay derived from the bdat kind byte.
            u32 id3 = nearest->mItem.mFlags >> 22;
            const char* colsb = lbl_eu_80508424;
            u8 kindCh = (u8)getBdatStringColumnValue(lbl_eu_806640C8,
                                                     colsb + 0x6C, id3);
            f32 delay = lbl_eu_806682EC *
                        (lbl_eu_806682F0 *
                         ((f32)((f64)kindCh - lbl_eu_806682C0)));
            nearest->mItem.mTimer14 = delay;
        }
    }

    mgr->mTime = lbl_eu_806682EC;
    func_80207B24(mgr, drop.mKind07 & 0x3F, &nearest->mItem.mPosX);
    func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x3C, 0, 0, lbl_eu_806682E4);

    // Register a one-shot sound timer in the first free slot.
    u16 sndId = 0x46 + (u16)(dropFlag != 0 ? 1 : 0);
    for (MineSoundTimer* p = mgr->mSnd; p != &mgr->mSnd[16]; ++p) {
        if (p->mId == 0) {
            p->mId = sndId;
            p->mTime = lbl_eu_806682E0;
            break;
        }
    }

    // Push a message into the ring buffer (evicting the oldest when full).
    char msgText[0x40];
    msgText[0] = 0;
    CItemImplInstances* im = CItem_initItemImplInstances();
    const char* nm = ((CItemInstVt50*)im)->_v20(&drop);
    u32 msgLen = strlen(nm);
    strcpy(msgText, nm);
    f32 msgTime = lbl_eu_806682F4;

    MineMsgRing* ring = &mgr->mMsgs;
    if (ring->mCount == 16) {
        u32 old = ring->mReadIdx;
        MineMsg* oldest = &ring->mBase[old];
        u32 oldLen = strlen(oldest->mText);
        char oldBuf[0x40];
        strcpy(oldBuf, oldest->mText);
        f32 oldTime = oldest->mTime;
        ring->mCount -= 1;
        ring->mReadIdx = (old + 1) % ring->mCapacity;
        (void)oldLen;
        (void)oldBuf;
        (void)oldTime;
    }
    u32 widx = (ring->mReadIdx + ring->mCount) % ring->mCapacity;
    MineMsg* slot = &ring->mBase[widx];
    slot->mLen = strlen(msgText);
    strcpy(slot->mText, msgText);
    slot->mTime = msgTime;
    ring->mCount += 1;

    u32 cid = func_80082694__Q22cf13CfGameManagerFv(0x70);
    u32 cval = cid + 1;
    func_8008269C__Q22cf13CfGameManagerFv(0x70, cval);
    if (cval == 1) func_800826F0__Q22cf13CfGameManagerFv(0x70);
    else if (cval == 0x32) func_800826F0__Q22cf13CfGameManagerFv(0x71);
    else if (cval == 0x1F4) func_800826F0__Q22cf13CfGameManagerFv(0x72);

    CItemImplInstances* im2 = CItem_initItemImplInstances();
    u32 rarity = ((CItemInstVt50*)im2)->_v08(&drop);
    if ((rarity & 0xFFFF) >= 5) {
        func_800826F0__Q22cf13CfGameManagerFv(0x76);
    }

    if (dropFlag != 0) {
        u32 sid = func_80082694__Q22cf13CfGameManagerFv(0x73);
        u32 sval = sid + 1;
        func_8008269C__Q22cf13CfGameManagerFv(0x73, sval);
        if (sval == 1) func_800826F0__Q22cf13CfGameManagerFv(0x73);
        else if (sval == 7) func_800826F0__Q22cf13CfGameManagerFv(0x74);
        else if (sval == 0x4D) func_800826F0__Q22cf13CfGameManagerFv(0x75);
    }

    lbl_eu_80663E24 |= 0x80000000;
}

// ---------------------------------------------------------------------------
// func_80207D2C - rebuild point list from a 150-entry save record.
// Each 6-byte record: u16 id, u8 count, u8 hi(id)/stop-flag, u8 area, u8 sub.
// A record with byte 3 == 0 terminates the stream.  The rebuilt point's
// timer is (float)id - 160.0 and X/Y/Z are left uninitialized (retail does
// not touch them either).
// ---------------------------------------------------------------------------
extern "C" void func_80207D2C(u8* rec) {
    CfMapMineManager* mgr = lbl_eu_806646A0;
    if (mgr != 0) {
        // Inlined reslist clear; the sentinel is re-read through the manager
        // on every test (the node stores may alias it).
        MineNode* n = mgr->mPoints.mStartPtr->mNext;
        while (n != mgr->mPoints.mStartPtr) {
            MineNode* cur = n;
            n = n->mNext;
            cur->mNext = 0;
        }
        mgr->mPoints.mStartPtr->mNext = mgr->mPoints.mStartPtr;
        mgr->mPoints.mStartPtr->mPrev = mgr->mPoints.mStartPtr;

        // Sound-timer reset goes through an address-taken local pair
        // (retail keeps the zero id / zero time in stack homes r31+8/0xC
        // and re-reads the singleton global between every store).
        MineSoundTimer init;
        InitTimerZero(&init);

        lbl_eu_806646A0->mMsgs.mCount = init.mId;
        lbl_eu_806646A0->mMsgs.mReadIdx = init.mId;
        lbl_eu_806646A0->mTime = init.mTime;

        for (int i = 0; i < 16; i++) {
            mgr->mSnd[i].mId = init.mId;
            mgr->mSnd[i].mTime = init.mTime;
        }
    }

    // Scratch point template; position/timer fields are overwritten per
    // record below (timer) or left as garbage (position), matching retail.
    // Flag masks mirror the inlined reslist element ctor (func_80205F78)
    // plus one extra bits-10..12 clear.
    MinePoint tmp;
    tmp.mObj0 = 0;
    tmp.mObj4 = 0;
    tmp.mTimer14 = lbl_eu_806682B0;
    tmp.mId18 = 0;
    tmp.mPointId1C = 0;
    tmp.mCounter1A = 0;
    u32 fl0 = tmp.mFlags;
    fl0 &= 0x003FFFFF;
    fl0 &= 0xFFFF00FF;
    fl0 &= 0xFFFFE3FF;
    tmp.mFlags = fl0;

    for (int i = 0; i < 150; i++) {
        if (rec[3] == 0) break;
        u16 pid = *(u16*)(rec + 0);
        u8 area = rec[4];
        u8 sub = rec[5];
        u8 count = rec[2];
        u8 pidHi = rec[3];

        tmp.mTimer14 = (f32)((f64)(u32)pid - lbl_eu_806682C0);
        u32 fl = tmp.mFlags;
        fl = (fl & 0xFF00FFFF) | ((u32)count << 8);
        fl = (fl & 0xFFC00FFF) | ((u32)pidHi << 22);
        tmp.mFlags = fl;

        // Singleton is re-read from the global every iteration.
        CfMapMineManager* m = lbl_eu_806646A0;
        MineNode* head = m->mPoints.mStartPtr;
        tmp.mArea1E = area;
        tmp.mAreaSub1F = sub;

        // First free slot (mNext == NULL), appends at mCapacity.
        u32 cap = m->mPoints.mCapacity;
        u32 idx = 0;
        while ((int)idx < (int)cap) {
            if (m->mPoints.mList[idx].mNext == 0) break;
            idx++;
        }
        MineNode* node = &m->mPoints.mList[idx];
        MinePoint* dst = &node->mItem;
        if (dst != 0) {
            dst->mObj0 = tmp.mObj0;
            dst->mObj4 = tmp.mObj4;
            dst->mPosX = tmp.mPosX;
            dst->mPosY = tmp.mPosY;
            dst->mPosZ = tmp.mPosZ;
            dst->mTimer14 = tmp.mTimer14;
            dst->mId18 = tmp.mId18;
            dst->mCounter1A = tmp.mCounter1A;
            dst->mPointId1C = tmp.mPointId1C;
            dst->mArea1E = tmp.mArea1E;
            dst->mAreaSub1F = tmp.mAreaSub1F;
            dst->mFlags = tmp.mFlags;
        }
        // push_back onto the intrusive list.
        node->mNext = head;
        node->mPrev = head->mPrev;
        head->mPrev->mNext = node;
        head->mPrev = node;
        rec += 6;
    }
}