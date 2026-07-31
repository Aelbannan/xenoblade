// CfMapMineManager - mine-point management (field collection points)
// High-level C++ reconstruction from retail ASM (US, 0x8020785C..0x80209D20).
// All 20 TU functions carry placeholder retail symbol names; they are emitted
// with C linkage so the object exports the exact retail symbols.

#include <types.h>
#include <decomp.h>
#include <string.h>

// ---------------------------------------------------------------------------
// External runtime / game functions (retail-unmangled symbols)
// ---------------------------------------------------------------------------

class BdatFilePointer;
extern "C" const char* getBdatStringColumnValue(BdatFilePointer* file,
                                                const char* column, u16 index);
extern "C" u32 func_8003B1EC(BdatFilePointer* file);
extern "C" u32 func_8003B41C(BdatFilePointer* file);
extern "C" u32 CfRes_getD80Flag();
extern "C" f32 func_80496288();
extern "C" u32 func_80061FFC();
extern "C" const f32 lbl_eu_806682B0;
extern "C" void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);
extern "C" void* __construct_new_array(void* block, void* ctor, void* dtor,
                                       u32 size, u32 n);
extern "C" void __dl__FPv(void* ptr);
extern "C" void __dla__FPv(void* ptr);
extern "C" int mtRand__Q22ml4mathFi(int n);
extern "C" int mtRand__Q22ml4mathFii(int lo, int hi);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int idx);
extern "C" u32 func_800822F4__Q22cf13CfGameManagerFv();
extern "C" void* func_800B8920(void* obj);
extern "C" void func_800B9404(void* obj);
extern "C" int func_80186BC8(u16 id);
extern "C" void func_800BFBF4(u16 id, int mode);
extern "C" void func_80462E58__8CTaskLODFv(u32 a, u32 b, f32 c);
extern "C" void func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(u32 a, u32 b, u32 c,
                                                           u32 d, f32 e);
extern "C" void* CItem_initItemImplInstances();
extern "C" void func_801583E0(void* item);
extern "C" void func_801570A0(void* item, u16 v);
extern "C" void func_80157F04(u16 v, void* outA, void* outB);
extern "C" void* func_8009ECB0();
extern "C" void* func_8009EC9C(u16 index);
extern "C" int func_8009CF8C(int v);
extern "C" int func_8026178C(void* data, u32 flag);
extern "C" u32 func_8025FB10(void* data, u32 flag);
extern "C" void* func_801599D4(void* item, int v);
extern "C" void func_801352A4(void* mgr);
extern "C" void* func_800B07E8__Fv();
extern "C" void* func_800B20B4(void* a, u32 b, u32 c, u32 d);
extern "C" void func_800C13FC(void* obj, void* name, u32 len);
extern "C" void func_800ACC14(void* obj, int mode);
extern "C" void func_800ACF78(void* obj, void* player, int v);
extern "C" int func_8013EB90(int v);
extern "C" int func_800FF738();
extern "C" void func_8013D55C(void* a, u32 b, u32 c);
extern "C" u32 func_800FE68C();
extern "C" void* func_8008187C__Q22cf13CfGameManagerFv(u32 value);
extern "C" void* func_8007F91C__Q22cf13CfGameManagerFv();
extern "C" int func_80084BF4__Q22cf13CfGameManagerFv();
extern "C" void* func_800821F8__Q22cf13CfGameManagerFv();
extern "C" int func_80085840__Q22cf13CfGameManagerFv();
extern "C" u32 func_80082694__Q22cf13CfGameManagerFv(u32 id);
extern "C" void func_8008269C__Q22cf13CfGameManagerFv(u32 id, u32 value);
extern "C" void func_800826F0__Q22cf13CfGameManagerFv(u32 id);
extern "C" void* getCurrentPad__Q22cf13CfGameManagerFv();
extern "C" void* getFP__FPCc(const char* name);

// ---------------------------------------------------------------------------
// External data (retail linker symbols)
// ---------------------------------------------------------------------------

struct CfMapMineManager;
extern "C" CfMapMineManager* lbl_eu_806646A0;   // sbss: singleton instance
extern "C" u32 lbl_eu_80663E24;                 // sbss: game state flags
extern "C" u16 lbl_eu_80663E42;                 // sbss: current area id
extern "C" u16 lbl_eu_80663E44;                 // sbss: current area sub-id
extern "C" BdatFilePointer* lbl_eu_806640C8;    // sbss: bdat file handle
extern "C" s32 lbl_eu_806646A4;                 // sbss: cached name length
extern "C" s8 lbl_eu_806646A8;                  // sbss: one-time init flag
extern "C" char lbl_eu_80662758[];              // sdata: scratch name buffer
extern "C" u8 lbl_eu_80662750[];                // sdata: range-pair table
extern "C" u32 lbl_eu_80662760[];               // sdata: lottery bias table
extern "C" char lbl_eu_80508424[];              // rodata: bdat column names
extern "C" char lbl_eu_80535720[];              // data: scratch name buffer 2
extern "C" u8 lbl_eu_8053572C[];                // data: reslist vtable
extern "C" u8 lbl_eu_80535744[];                // data: _reslist_base vtable

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
    MineMsgRing* mBase;    // 0x480  (= this)
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

// Cast-only view of the scene object.  RTTI contributes the two hidden
// entries before the first declared virtual, so this declaration places the
// position callback at the retail vtable offset 0x9C.
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
};

// ---------------------------------------------------------------------------
// Small helpers (high-level; inlined at -O4)
// ---------------------------------------------------------------------------

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
        ListClearBase(self);
        if (self->mUnk03C == false) {
            if (self->mList != 0) {
                __dla__FPv((u8*)self->mList - 0x10);
            }
            self->mList = 0;
        }
    }
    if (flags > 0) {
        __dl__FPv(self);
    }
    return self;
}

// ---------------------------------------------------------------------------
// __dt__reslist_cf_CfMapMineManager_MinePoint - derived list destructor.
// ---------------------------------------------------------------------------
extern "C" MineListBase*
__dt__reslist_cf_CfMapMineManager_MinePoint(MineListBase* self, s32 flags) {
    if (self != 0) {
        if (self != 0) {
            self->mVtable = lbl_eu_80535744;
            ListClearBase(self);
            if (self->mUnk03C == false) {
                if (self->mList != 0) {
                    __dla__FPv((u8*)self->mList - 0x10);
                }
                self->mList = 0;
            }
        }
    }
    if (flags > 0) {
        __dl__FPv(self);
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
        ListClear(self);
        if (self->mPoints.mUnk03C == false) {
            if (self->mPoints.mList != 0) {
                __dla__FPv((u8*)self->mPoints.mList - 0x10);
            }
            self->mPoints.mList = 0;
        }
        self->mPoints.mCapacity = 0;
        lbl_eu_806646A0 = 0;
        if ((u32)self + 0x44 != 0) {
            self->mMsgs.mCount = 0;
            self->mMsgs.mReadIdx = 0;
        }
        // Inlined ~_reslist_base for the embedded list subobject.
        if ((u32)self + 0x04 != 0) {
            self->mPoints.mVtable = lbl_eu_80535744;
            ListClear(self);
            if (self->mPoints.mUnk03C == false) {
                if (self->mPoints.mList != 0) {
                    __dla__FPv((u8*)self->mPoints.mList - 0x10);
                }
                self->mPoints.mList = 0;
            }
        }
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// __ct__80205A7C - CfMapMineManager constructor.
// ---------------------------------------------------------------------------
extern "C" CfMapMineManager* __ct__80205A7C(CfMapMineManager* self) {
    u32 flags = self->mPoints.mStartNode.mItem.mFlags;
    flags &= 0x003FFFFF;
    flags &= 0xFFFF00FF;

    self->mTime = 0.0f;
    self->mPoints.mVtable = lbl_eu_80535744;
    self->mPoints.mList = 0;
    self->mPoints.mCapacity = 0;
    self->mPoints.mStartNode.mItem.mTimer14 = 0.0f;
    self->mPoints.mStartNode.mItem.mId18 = 0;
    self->mPoints.mStartNode.mItem.mPointId1C = 0;
    self->mPoints.mStartNode.mItem.mFlags = flags;
    self->mPoints.mStartNode.mItem.mObj0 = 0;
    self->mPoints.mStartNode.mItem.mObj4 = 0;
    self->mPoints.mUnk03C = false;
    self->mPoints.mStartPtr = &self->mPoints.mStartNode;
    self->mPoints.mStartNode.mNext = &self->mPoints.mStartNode;
    self->mPoints.mStartNode.mPrev = &self->mPoints.mStartNode;
    self->mPoints.mVtable = lbl_eu_8053572C;
    self->mMsgs.mSlots[0].mText[0] = 0;
    self->mMsgs.mSlots[0].mLen = 0;
    for (int i = 1; i < 16; i++) {
        self->mMsgs.mSlots[i].mText[0] = 0;
        self->mMsgs.mSlots[i].mLen = 0;
    }

    // MineMsgRing header.
    self->mMsgs.mCapacity = 16;
    self->mMsgs.mBase = &self->mMsgs;
    self->mMsgs.mCount = 0;
    self->mMsgs.mReadIdx = 0;

    // Sound timers.
    for (int i = 0; i < 16; i++) {
        self->mSnd[i].mId = 0;
        self->mSnd[i].mTime = 0.0f;
    }

    lbl_eu_806646A0 = self;

    // Allocate the node array (150 nodes x 0x2C).
    u32 handle = func_80061FFC();
    void* mem = allocate_array__Q23mtl10MemManagerFUlUl(0x19D8, handle);
    self->mPoints.mList = (MineNode*)__construct_new_array(mem, (void*)func_80205F78, 0,
                                                           0x2C, 0x96);
    for (int i = 0; i < 150; i++) {
        ((MineNode*)((u8*)self->mPoints.mList + i * 0x2C))->mNext = 0;
    }
    self->mPoints.mCapacity = 0x96;

    // Reset the message ring and sound timers on the singleton.
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
        mgr->mMsgs.mCount = 0;
        mgr->mMsgs.mReadIdx = 0;
        mgr->mTime = 0.0f;
        for (int i = 0; i < 16; i++) {
            mgr->mSnd[i].mId = 0;
            mgr->mSnd[i].mTime = 0.0f;
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// func_802062BC - global reset via the singleton.
// ---------------------------------------------------------------------------
extern "C" void func_802062BC() {
    CfMapMineManager* mgr = lbl_eu_806646A0;
    if (mgr == 0) return;

    MineNode* start = mgr->mPoints.mStartPtr;
    MineNode* n = start->mNext;
    while (n != start) {
        MineNode* cur = n;
        n = n->mNext;
        cur->mNext = 0;
    }
    start->mNext = start;
    start->mPrev = start;

    lbl_eu_806646A0->mMsgs.mCount = 0;
    lbl_eu_806646A0->mMsgs.mReadIdx = 0;
    lbl_eu_806646A0->mTime = 0.0f;
    for (int i = 0; i < 16; i++) {
        lbl_eu_806646A0->mSnd[i].mId = 0;
        lbl_eu_806646A0->mSnd[i].mTime = 0.0f;
    }
}

// ---------------------------------------------------------------------------
// func_80206388 - release every point's scene objects and reset state.
// ---------------------------------------------------------------------------
extern "C" void func_80206388(CfMapMineManager* self) {
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
}

// ---------------------------------------------------------------------------
// func_802064A8 - populate one MinePoint from the mine-list bdat row.
// Returns (rowValue18 != 0).
// ---------------------------------------------------------------------------
extern "C" int func_802064A8(CfMapMineManager* self, u32 rowId, MinePoint* pt,
                             int isNew) {
    BdatFilePointer* file = lbl_eu_806640C8;
    const char* cols = lbl_eu_80508424;

    f32 x = (f32)(s32)getBdatStringColumnValue(file, cols + 0x10, rowId) *
            1.0e-4f;
    f32 y = (f32)(s32)getBdatStringColumnValue(file, cols + 0x15, rowId) *
            1.0e-4f;
    f32 z = (f32)(s32)getBdatStringColumnValue(file, cols + 0x1A, rowId) *
            1.0e-4f;
    u16 val26 = (u16)getBdatStringColumnValue(file, cols + 0x1F, rowId);
    u16 val2D = (u16)getBdatStringColumnValue(file, cols + 0x26, rowId);
    u8 val34 = (u8)getBdatStringColumnValue(file, cols + 0x2D, rowId);

    if (val26 != 0) {
        u32 fl = pt->mFlags;
        fl = (fl & 0x003FFFFF) | ((rowId & 0x3FF) << 22);
        u32 bit = ((u32)(val34 - 2) < 1u);
        fl = (fl & 0xFFFE0FFF) | (bit << 16);
        pt->mPosX = x;
        pt->mPosY = y;
        pt->mPosZ = z;
        pt->mId18 = val26;
        pt->mPointId1C = val2D;
        pt->mObj0 = 0;
        pt->mObj4 = 0;
        pt->mFlags = fl;
        pt->mArea1E = (u8)lbl_eu_80663E42;
        pt->mAreaSub1F = (u8)lbl_eu_80663E44;
        pt->mCounter1A = 0;
        if (isNew != 0) {
            u8 lo = (u8)getBdatStringColumnValue(file, cols + 0x00, rowId);
            u8 hi = (u8)getBdatStringColumnValue(file, cols + 0x08, rowId);
            int r = mtRand__Q22ml4mathFi((hi - lo) + 1);
            u32 fl2 = pt->mFlags;
            fl2 = (fl2 & 0xFFFF00FF) | (((lo + r) & 0xFF) << 8);
            pt->mFlags = fl2;
            pt->mTimer14 = 0.0f;
        }
    }
    return val26 != 0;
}

// ---------------------------------------------------------------------------
// func_802066A8 - (re)spawn the scene object for a mine point.
// ---------------------------------------------------------------------------
extern "C" int func_802066A8(CfMapMineManager* self, MinePoint* pt) {
    if (pt->mArea1E != (u8)lbl_eu_80663E42 ||
        pt->mAreaSub1F != (u8)lbl_eu_80663E44) {
        return 0;
    }
    if (pt->mObj4 != 0) {
        return 1;
    }

    void* a = func_800B07E8__Fv();
    void* obj = func_800B20B4(a, 0x4000, 0, 0);
    if (obj == 0) {
        return pt->mObj4 != 0;
    }
    pt->mObj4 = obj;

    const char* cols = lbl_eu_80508424;
    void* fp = getFP__FPCc(cols + 0x37);
    u16 resId = (u16)getBdatStringColumnValue(lbl_eu_806640C8, cols + 0x45,
                                              pt->mFlags >> 22);
    char nameBuf[0x40];
    u8 nameLenByte = 0;
    const char* nm = getBdatStringColumnValue((BdatFilePointer*)fp, cols + 0x4E, resId);
    u32 len = strlen(nm);
    strcpy(nameBuf, nm);
    func_800C13FC(obj, nameBuf, 0xC);

    void* vt = *(void**)obj;
    ((void (*)(void*, u32))(*(u32*)((u8*)vt + 0x158)))(obj, 1);
    ((MineSceneObjectIf*)obj)->setPosition(&pt->mPosX);
    *(u8*)((u8*)obj + 0x90) = 0;

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
        if (n->mItem.mId18 != 0 && ((n->mItem.mFlags >> 16) & 1) != 0 &&
            area == n->mItem.mArea1E && sub == n->mItem.mAreaSub1F &&
            pointId == n->mItem.mPointId1C &&
            ((n->mItem.mFlags >> 8) & 0xFF) != 0) {
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
    MineNode* start = lbl_eu_806646A0->mPoints.mStartPtr;
    MineNode* n = start->mNext;
    (void)start;
    while (n != start) {
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

    void* player = getPlayer__Q22cf13CfGameManagerFi(0);
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
    u16 area = lbl_eu_80663E42;
    u16 sub = lbl_eu_80663E44;
    CfRes_getD80Flag();
    f32 dt = func_80496288();

    if (pt->mTimer14 > 0.0f) {
        if (func_80207C08(pt->mPointId1C, pt->mArea1E, pt->mAreaSub1F) == 0) {
            pt->mTimer14 = pt->mTimer14 - dt;
        }
        return;
    }

    pt->mTimer14 = 0.0f;
    if (((pt->mFlags >> 8) & 0xFF) != 0) return;
    if (area != pt->mArea1E) return;
    if (sub != pt->mAreaSub1F) return;

    BdatFilePointer* file = lbl_eu_806640C8;
    u32 id = pt->mFlags >> 22;
    const char* cols = lbl_eu_80508424;
    u8 lo = (u8)getBdatStringColumnValue(file, cols + 0x00, id);
    u8 hi = (u8)getBdatStringColumnValue(file, cols + 0x08, id);
    int r = mtRand__Q22ml4mathFi((hi - lo) + 1);
    u32 fl = pt->mFlags;
    fl = (fl & 0xFFFF00FF) | (((lo + r) & 0xFF) << 8);
    pt->mFlags = fl;

    if (((fl >> 8) & 0xFF) != 0) {
        if ((fl & 0x00010000) != 0) {
            u32 g = lbl_eu_80663E24;
            f32 a = 160.0f - (f32)((g >> 20) & 1);
            func_80462E58__8CTaskLODFv(pt->mPointId1C, 1, a);
        } else {
            if (func_80186BC8(pt->mPointId1C) != 0) {
                func_800BFBF4(pt->mPointId1C, 1);
            }
        }
        func_802066A8(self, pt);
    }
}

// ---------------------------------------------------------------------------
// func_802073CC - advance message-ring timers, pop the oldest when expired.
// ---------------------------------------------------------------------------
extern "C" void func_802073CC(CfMapMineManager* self) {
    CfRes_getD80Flag();
    f32 dt = func_80496288();

    MineMsgRing* ring = &self->mMsgs;
    int changed = 0;
    for (u32 i = 0; i < ring->mCount; i++) {
        MineMsg* slot =
            &ring->mSlots[(ring->mReadIdx + i) % ring->mCapacity];
        if (slot->mTime <= 0.0f) {
            slot->mTime = slot->mTime - dt;
            if (slot->mTime < 0.0) {
                slot->mTime = 0.0f;
                changed = 1;
            }
        }
    }

    if (changed != 0) {
        MineMsg* slot = &ring->mSlots[ring->mReadIdx % ring->mCapacity];
        func_801352A4(self);
        u32 next = ring->mReadIdx + 1;
        ring->mCount = ring->mCount - 1;
        ring->mReadIdx = next % ring->mCapacity;
        char buf[0x40];
        u32 len = strlen(slot->mText);
        strcpy(buf, slot->mText);
        f32 t = slot->mTime;
        (void)len;
        (void)t;
        (void)buf;
    }
}

// ---------------------------------------------------------------------------
// func_8020712C - find the nearest activatable point to a position.
// ---------------------------------------------------------------------------
extern "C" void func_8020712C(MineNode** out, CfMapMineManager* mgr,
                              CfMapMineManager* list, f32* pos) {
    BdatFilePointer* file = lbl_eu_806640C8;
    u16 area = lbl_eu_80663E42;
    u16 sub = lbl_eu_80663E44;
    u32 playerId = func_800822F4__Q22cf13CfGameManagerFv();
    *out = 0;
    playerId &= 0xFFFF;

    u32 marker;
    if (func_800FE68C() != 0) {
        u32 base = func_800FE68C() + 0x10000;
        marker = *(u32*)(base - 0x6F1C);
    } else {
        marker = 0;
    }

    const char* cols = lbl_eu_80508424;
    MineNode* start = list->mPoints.mStartPtr;
    MineNode* n = start->mNext;
    while (n != start) {
        n->mItem.mFlags &= ~0x00060000;
        if (n->mItem.mTimer14 > 0.0f || ((n->mItem.mFlags >> 8) & 0xFF) == 0) {
            func_80206FA8(mgr, &n->mItem);
            n = n->mNext;
            continue;
        }
        if (area != n->mItem.mArea1E || sub != n->mItem.mAreaSub1F) {
            n = n->mNext;
            continue;
        }
        u32 id = n->mItem.mFlags >> 22;
        u16 a = (u16)getBdatStringColumnValue(file, cols + 0x60, id);
        u16 b = (u16)getBdatStringColumnValue(file, cols + 0x66,
                                              n->mItem.mFlags >> 22);
        if (playerId < a || playerId > b) {
            if (n->mItem.mObj0 != 0) {
                func_800ACC14(n->mItem.mObj0, 1);
                n->mItem.mObj0 = 0;
            }
            n = n->mNext;
            continue;
        }
        n->mItem.mFlags |= 0x00040000;
        f32 dx = n->mItem.mPosX - pos[0];
        f32 dy = n->mItem.mPosY - pos[1];
        f32 dz = n->mItem.mPosZ - pos[2];
        f32 ady = dy < 0 ? -dy : dy;
        f32 dist2 = dx * dx + dy * dy + dz * dz;
        if (ady > 50.0f || dist2 > 40000.0f) {
            if (n->mItem.mObj0 != 0) {
                *(u32*)((u8*)n->mItem.mObj0 + 0x68) |= 0x40;
                n->mItem.mObj0 = 0;
            }
            n = n->mNext;
            continue;
        }
        if (n->mItem.mObj0 != 0 && n->mItem.mCounter1A > 0) {
            n->mItem.mCounter1A = n->mItem.mCounter1A - 1;
        }
        if (n->mItem.mCounter1A > 0) {
            n = n->mNext;
            continue;
        }
        if (ady >= 2.5f || dist2 >= 6.25f) {
            n = n->mNext;
            continue;
        }
        if (n->mItem.mObj4 == 0) {
            n = n->mNext;
            continue;
        }
        n->mItem.mFlags |= 0x00020000;
        if (*(u32*)((u8*)n->mItem.mObj4 + 0x74) == marker) {
            *out = n;
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
            if (found->mItem.mTimer14 < 1.0e-6f) {
                func_802066A8(self, &found->mItem);
                if ((found->mItem.mFlags & 0x00010000) != 0) {
                    u32 g = lbl_eu_80663E24;
                    f32 a = 160.0f - (f32)((g >> 20) & 1);
                    func_80462E58__8CTaskLODFv(found->mItem.mPointId1C, 1, a);
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
                        f32 a = 160.0f - (f32)((g >> 20) & 1);
                        func_80462E58__8CTaskLODFv(found->mItem.mPointId1C, 0,
                                                   a);
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
extern "C" int func_802067E4(CfMapMineManager* self, MinePoint* pt,
                             void* item, u8* outFlag) {
    if (lbl_eu_806646A8 == 0) {
        lbl_eu_806646A4 = strlen(lbl_eu_80662758) - 1;
        lbl_eu_806646A8 = 1;
    }

    BdatFilePointer* file = lbl_eu_806640C8;
    *outFlag = 0;
    memset(item, 0, 0x34);
    const char* cols = lbl_eu_80508424;
    u16 rowVal = (u16)getBdatStringColumnValue(file, cols + 0x1F,
                                               pt->mFlags >> 22);
    if (rowVal == 0) {
        return 0;
    }

    func_801583E0(item);
    void* impl = CItem_initItemImplInstances();
    ((void (*)(void*, void*))(*(u32*)((u8*)*(void**)impl + 0x1C)))(impl, item);

    u8 spA, spB;
    func_80157F04(rowVal, &spA, &spB);
    u8 kind = (u8)getBdatStringColumnValue(file, cols + 0x53, spB);

    void* mgr = func_8009ECB0();
    u32* arr = (u32*)((u8*)mgr + 0x4);
    u32 total = 0;
    for (int i = 0; i < 9; i++) {
        u32 v = arr[i];
        if (v >= 1 && v <= 8) {
            void* rec = func_8009EC9C((u16)v);
            void* data = (u8*)rec + 0x3534;
            if (data != 0 && func_8026178C(data, 0x91) != 0) {
                total += func_8025FB10(data, 0x91);
            }
        }
    }

    int rnd = mtRand__Q22ml4mathFi(10000);
    u32 bias = lbl_eu_80662760[(pt->mFlags >> 11) & 1];
    u32 scaled = (total + bias) * 100;
    u32 x = scaled ^ rnd;
    u32 q = (x & scaled) - (s32)x / 2;
    u8 flagBit = (u8)(q >> 31);
    *outFlag = flagBit;
    *(u8*)((u8*)item + 0x16) = flagBit ? 2 : 1;

    u16 cap = (u16)func_8009CF8C(0x800);
    if (cap > kind && lbl_eu_80663E42 == 4) {
        kind = cap;
    }
    func_801570A0(item, kind);

    u32 tblIdx = ((pt->mFlags >> 16) & 1) ? 2 : 0;
    if (*outFlag) tblIdx += 1;
    u8 lo = lbl_eu_80662750[tblIdx * 2];
    u8 hi = lbl_eu_80662750[tblIdx * 2 + 1];

    for (int k = 0; k < 4; k++) {
        void* im = CItem_initItemImplInstances();
        ((void (*)(void*, void*, u32, u32))(*(u32*)((u8*)*(void**)im + 0x50)))(
            im, item, k, 0);
        void* im2 = CItem_initItemImplInstances();
        ((void (*)(void*, void*, u32, u32))(*(u32*)((u8*)*(void**)im2 + 0x68)))(
            im2, item, k, 0);
    }

    int need, limit;
    if ((pt->mFlags & 0x00010000) != 0) {
        need = 2;
        limit = 4;
    } else {
        need = 1;
        limit = 2;
    }

    u16 picked[4];
    memset(picked, 0, 8);
    int attempts = 0;
    int npicked = 0;
    s32 nameLen = lbl_eu_806646A4;

    for (;;) {
        lbl_eu_80662758[nameLen] = (char)(attempts + 0x31);
        lbl_eu_80535720[nameLen] = (char)(attempts + 0x31);
        u16 A = (u16)getBdatStringColumnValue(file, lbl_eu_80662758,
                                              pt->mFlags >> 22);
        u8 B = (u8)getBdatStringColumnValue(file, lbl_eu_80535720,
                                            pt->mFlags >> 22);
        if (*outFlag != 0 && B != 0) {
            B = (u8)(B + 100);
        }
        int roll = mtRand__Q22ml4mathFi(100);
        if (A != 0 && roll < B) {
            int dup = 0;
            for (int j = 0; j < npicked; j++) {
                if (A == picked[j]) {
                    dup = 1;
                    break;
                }
            }
            if (dup == 0) {
                u16 qty = (u16)mtRand__Q22ml4mathFii(lo, hi);
                void* im = CItem_initItemImplInstances();
                ((void (*)(void*, void*, u32, u32))(*(u32*)((u8*)*(void**)im +
                                                             0x50)))(im, item,
                                                                     npicked, A);
                void* im2 = CItem_initItemImplInstances();
                ((void (*)(void*, void*, u32, u32))(*(u32*)((u8*)*(void**)im2 +
                                                             0x68)))(im2, item,
                                                                     npicked,
                                                                     qty);
                picked[npicked] = A;
                npicked++;
                if (npicked >= limit) {
                    return 1;
                }
            }
        }
        attempts++;
        if (attempts >= limit) {
            attempts -= (attempts / limit) * limit;
        }
        if (npicked >= need) {
            return 1;
        }
    }
}

// ---------------------------------------------------------------------------
// func_802074F0 - top-level update: timers, nearest point, collection input.
// ---------------------------------------------------------------------------
extern "C" void func_802074F0(CfMapMineManager* self) {
    void* player = getPlayer__Q22cf13CfGameManagerFi(0);
    if (player == 0) return;

    func_802073CC(self);
    CfRes_getD80Flag();
    f32 dt = func_80496288();

    for (int i = 0; i < 16; i++) {
        if (self->mSnd[i].mId != 0) {
            self->mSnd[i].mTime -= dt;
            if (self->mSnd[i].mTime <= 0.0) {
                func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, self->mSnd[i].mId,
                                                           0, 0, 0.6f);
                self->mSnd[i].mId = 0;
                self->mSnd[i].mTime = 0.0f;
            }
        }
    }

    void* p2 = getPlayer__Q22cf13CfGameManagerFi(0);
    void* vt = *(void**)p2;
    f32 px = ((f32 (*)(void*))(*(u32*)((u8*)vt + 0xAC)))(p2);
    // (position fetch elided in this reconstruction stub)

    if (self->mTime > 0.0f) {
        CfRes_getD80Flag();
        self->mTime -= func_80496288();
        if (self->mTime < 0.0f) self->mTime = 0.0f;
        return;
    }

    MineNode* nearest;
    f32 pos[3];
    lbl_eu_80663E24 &= ~1u;
    func_8020712C(&nearest, self, (CfMapMineManager*)((u8*)self + 0x4), pos);
    if (nearest == 0) return;
    if (func_8007F91C__Q22cf13CfGameManagerFv() != 0) return;

    int ready = 0;
    u32 fl = nearest->mItem.mFlags;
    if ((fl & 0x00020000) != 0 && ((fl >> 8) & 0xFF) != 0) {
        ready = 1;
    }
    if (ready == 0) return;
    if (func_8013EB90(1) != 0) return;
    if (func_800FF738() != 0) return;
    if ((lbl_eu_80663E24 & 0xAFA40000) != 0) return;
    if (func_80084BF4__Q22cf13CfGameManagerFv() != 0) return;
    void* pad2 = func_800821F8__Q22cf13CfGameManagerFv();
    if (pad2 != 0 && (*(u32*)((u8*)pad2 + 0x4) & 0x01000000) != 0) return;
    if (func_80085840__Q22cf13CfGameManagerFv() == 0) return;
    if ((lbl_eu_80663E24 & 0x00002000) != 0) return;
    void* pad = getCurrentPad__Q22cf13CfGameManagerFv();
    if ((*(u32*)((u8*)pad + 0x4) & 0x00000010) == 0) return;

    u8 itemBuf[0x80];
    u8 dropFlag = 0;
    u16 sp24 = 0;
    u32 sp20 = 0;
    if (func_802067E4(self, &nearest->mItem, itemBuf, &dropFlag) == 0) return;

    if (func_801599D4(itemBuf, 0) == 0) {
        const char* cols = lbl_eu_80508424;
        void* fp = getFP__FPCc(cols + 0x77);
        getBdatStringColumnValue((BdatFilePointer*)fp, cols + 0x4E, 0x1B);
        func_8013D55C(0, 0, 0);
        return;
    }

    // Collection succeeded: decrement count, spawn message, play fanfare.
    u32 fl2 = nearest->mItem.mFlags;
    u32 cnt = (fl2 >> 8) & 0xFF;
    fl2 = (fl2 & 0xFFFF00FF) | (((cnt - 1) & 0xFF) << 8);
    nearest->mItem.mFlags = fl2;
    if (((fl2 >> 8) & 0xFF) == 0) {
        if (nearest->mItem.mObj4 != 0) {
            if (func_800B8920(nearest->mItem.mObj4) != 0) {
                func_800B9404(nearest->mItem.mObj4);
            }
            nearest->mItem.mObj4 = 0;
        }
        nearest->mItem.mObj4 = 0;
        if ((nearest->mItem.mFlags & 0x00010000) == 0 ||
            func_80207C08(nearest->mItem.mPointId1C, nearest->mItem.mArea1E,
                          nearest->mItem.mAreaSub1F) == 0) {
            if ((nearest->mItem.mFlags & 0x00010000) != 0) {
                u32 g = lbl_eu_80663E24;
                f32 a = 160.0f - (f32)((g >> 20) & 1);
                func_80462E58__8CTaskLODFv(nearest->mItem.mPointId1C, 0, a);
            } else {
                if (func_80186BC8(nearest->mItem.mPointId1C) != 0) {
                    func_800BFBF4(nearest->mItem.mPointId1C, 2);
                }
            }
        }
    }

    u32 id = nearest->mItem.mFlags >> 22;
    u8 msgKind = (u8)getBdatStringColumnValue(lbl_eu_806640C8,
                                              lbl_eu_80508424 + 0x6C, id);
    f32 msgTime = 30.0f * (1.0f / 60.0f) *
                  (176.0f - (f32)((msgKind & 0xFF)));
    nearest->mItem.mTimer14 = msgTime;
    self->mTime = 60.0f;

    func_80207B24(self, (dropFlag >> 6) & 3, (u8*)itemBuf + 0x10);
    func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x3C, 0, 0, 0.6f);

    // Register a sound timer.
    u16 sndId = (dropFlag != 0) ? 0x47 : 0x46;
    for (int i = 0; i < 16; i++) {
        if (self->mSnd[i].mId == 0) {
            self->mSnd[i].mId = sndId;
            self->mSnd[i].mTime = 25.0f;
            break;
        }
    }

    // Push a message into the ring buffer.
    MineMsgRing* ring = &self->mMsgs;
    char msgText[0x48];
    msgText[0] = 0;
    void* im = CItem_initItemImplInstances();
    const char* nm = ((const char* (*)(void*, void*))(*(u32*)((u8*)*(void**)im +
                                                              0x20)))(im,
                                                                      itemBuf);
    u32 mlen = strlen(nm);
    strcpy(msgText, nm);
    f32 mtime = 22.0f;

    if (ring->mCount == 16) {
        MineMsg* old = &ring->mSlots[ring->mReadIdx % ring->mCapacity];
        u32 nxt = ring->mReadIdx + 1;
        ring->mCount -= 1;
        ring->mReadIdx = nxt % ring->mCapacity;
        (void)old;
    }
    {
        u32 widx = (ring->mReadIdx + ring->mCount) % ring->mCapacity;
        MineMsg* slot = &ring->mSlots[widx];
        slot->mLen = strlen(msgText);
        strcpy(slot->mText, msgText);
        slot->mTime = mtime;
        ring->mCount += 1;
    }

    u32 cid = func_80082694__Q22cf13CfGameManagerFv(0x70);
    u32 cval = cid + 1;
    func_8008269C__Q22cf13CfGameManagerFv(0x70, cval);
    if (cval == 1) func_800826F0__Q22cf13CfGameManagerFv(0x70);
    else if (cval == 0x32) func_800826F0__Q22cf13CfGameManagerFv(0x71);
    else if (cval == 0x1F4) func_800826F0__Q22cf13CfGameManagerFv(0x72);

    void* im2 = CItem_initItemImplInstances();
    u32 rarity = ((u32(*)(void*, void*))(*(u32*)((u8*)*(void**)im2 + 0x08)))(
        im2, itemBuf);
    if ((rarity & 0xFFFF) >= 5) {
        func_800826F0__Q22cf13CfGameManagerFv(0x76);
    }

    if (msgText[0] != 0) {
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
// ---------------------------------------------------------------------------
extern "C" void func_80207D2C(u8* rec) {
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
        mgr->mMsgs.mCount = 0;
        mgr->mMsgs.mReadIdx = 0;
        mgr->mTime = 0.0f;
        for (int i = 0; i < 16; i++) {
            mgr->mSnd[i].mId = 0;
            mgr->mSnd[i].mTime = 0.0f;
        }
    }

    MinePoint tmp;
    tmp.mObj0 = 0;
    tmp.mObj4 = 0;
    tmp.mPosX = 0.0f;
    tmp.mPosY = 0.0f;
    tmp.mPosZ = 0.0f;
    tmp.mTimer14 = 0.0f;
    tmp.mId18 = 0;
    tmp.mCounter1A = 0;
    tmp.mPointId1C = 0;
    tmp.mArea1E = 0;
    tmp.mAreaSub1F = 0;
    u32 fl = tmp.mFlags;
    fl &= 0x003FFFFF;
    fl &= 0xFFFF00FF;
    fl &= 0xFF1FFFFF;
    tmp.mFlags = fl;

    for (int i = 0; i < 150; i++) {
        u8 cnt = rec[3];
        if (cnt == 0) break;
        u16 pid = *(u16*)(rec + 0);
        u8 area = rec[4];
        u8 sub = rec[5];
        u8 count = rec[2];
        tmp.mTimer14 = (f32)(176.0 - (f32)sub);
        u32 f2 = tmp.mFlags;
        f2 = (f2 & 0xFFFF00FF) | ((count & 0xFF) << 8);
        f2 = (f2 & 0x003FFFFF) | ((cnt & 0x3FF) << 22);
        tmp.mFlags = f2;
        tmp.mArea1E = area;
        tmp.mAreaSub1F = (u8)(176.0 - (f32)sub);

        CfMapMineManager* m2 = lbl_eu_806646A0;
        u32 idx = 0;
        while (idx < m2->mPoints.mCapacity) {
            if (((MineNode*)((u8*)m2->mPoints.mList + idx * 0x2C))->mNext == 0) break;
            idx++;
        }
        MineNode* slot = (MineNode*)((u8*)m2->mPoints.mList + idx * 0x2C);
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

        MineNode* head = m2->mPoints.mStartPtr;
        slot->mNext = head;
        slot->mPrev = head->mPrev;
        head->mPrev->mNext = slot;
        head->mPrev = slot;
        rec += 6;
    }
}
