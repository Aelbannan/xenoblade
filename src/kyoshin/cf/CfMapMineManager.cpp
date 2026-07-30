// CfMapMineManager - mine-point management (field collection points)
// High-level C++ reconstruction from retail ASM (US, 0x8020785C..0x80209D20).
// All 20 TU functions carry placeholder retail symbol names; they are emitted
// with C linkage so the object exports the exact retail symbols.

#include <types.h>
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
    u32 mFlags;       // 0x20  [id:10][kind:6][count:8][low:8] (MSB-first)
};

// Intrusive reslist node: {next, prev, item}; stride 0x2C.
struct MineNode {
    MineNode* mNext;   // 0x00
    MineNode* mPrev;   // 0x04
    MinePoint mItem;   // 0x08
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
    // reslist<MinePoint> mPoints - laid out explicitly for symbol control:
    void* mVtable;         // 0x004
    MineNode* mStartPtr;   // 0x008  (= &mStartNode)
    MineNode mStartNode;   // 0x00C  (0x2C bytes)
    MineNode* mList;       // 0x038
    u32 mCapacity;         // 0x03C
    u8 mUnk040;            // 0x040
    u8 mPad041[3];         // 0x041
    MineMsgRing mMsgs;     // 0x044  (0x490 bytes -> ends at 0x4D4)
    MineSoundTimer mSnd[16]; // 0x4D4..0x554
};

// ---------------------------------------------------------------------------
// Small helpers (high-level; inlined at -O4)
// ---------------------------------------------------------------------------

static inline MineNode* ListHead(CfMapMineManager* m) { return m->mStartPtr; }

// _reslist_base::clearList() - mark every slot free, reset the sentinel.
static inline void ListClear(CfMapMineManager* m) {
    MineNode* start = m->mStartPtr;
    MineNode* n = start->mNext;
    while (n != start) {
        MineNode* cur = n;
        n = n->mNext;
        cur->mNext = 0;
    }
    start->mNext = start;
    start->mPrev = start;
}

// First free slot index (mNext == NULL); appends at mCapacity when full.
static inline u32 ListFindFree(CfMapMineManager* m) {
    u32 i = 0;
    while (i < m->mCapacity) {
        if (((MineNode*)((u8*)m->mList + i * 0x2C))->mNext == 0) break;
        i++;
    }
    return i;
}

// ---------------------------------------------------------------------------
// func_80205F78 - default constructor of a reslist array element (node).
// Zeroes the item's runtime fields, preserves mFlags id bits partially.
// ---------------------------------------------------------------------------
extern "C" void func_80205F78(MineNode* node) {
    u32 flags = node->mItem.mFlags;
    flags &= 0x003FFFFF;          // clrlwi r0, r0, 10
    flags &= 0xFFFF00FF;          // rlwinm r0, r0, 0, 24, 15
    node->mItem.mObj0 = 0;
    node->mItem.mObj4 = 0;
    node->mItem.mTimer14 = 0.0f;
    node->mItem.mId18 = 0;
    node->mItem.mPointId1C = 0;
    node->mItem.mFlags = flags;
}

// ---------------------------------------------------------------------------
// __dt___reslist_base_cf_CfMapMineManager_MinePoint - base list destructor.
// ---------------------------------------------------------------------------
extern "C" CfMapMineManager*
__dt___reslist_base_cf_CfMapMineManager_MinePoint(CfMapMineManager* self,
                                                  s32 flags) {
    if (self != 0) {
        self->mVtable = lbl_eu_80535744;
        ListClear(self);
        if (self->mUnk040 == false) {
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
extern "C" CfMapMineManager*
__dt__reslist_cf_CfMapMineManager_MinePoint(CfMapMineManager* self, s32 flags) {
    if (self != 0) {
        if (self != 0) {
            self->mVtable = lbl_eu_80535744;
            ListClear(self);
            if (self->mUnk040 == false) {
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
        if (self->mUnk040 == false) {
            if (self->mList != 0) {
                __dla__FPv((u8*)self->mList - 0x10);
            }
            self->mList = 0;
        }
        self->mCapacity = 0;
        lbl_eu_806646A0 = 0;
        if ((u32)self + 0x44 != 0) {
            self->mMsgs.mCount = 0;
            self->mMsgs.mReadIdx = 0;
        }
        // Inlined ~_reslist_base for the embedded list subobject.
        if ((u32)self + 0x04 != 0) {
            self->mVtable = lbl_eu_80535744;
            ListClear(self);
            if (self->mUnk040 == false) {
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
// __ct__80205A7C - CfMapMineManager constructor.
// ---------------------------------------------------------------------------
extern "C" CfMapMineManager* __ct__80205A7C(CfMapMineManager* self) {
    u32 flags = self->mStartNode.mItem.mFlags;
    flags &= 0x003FFFFF;
    flags &= 0xFFFF00FF;

    self->mTime = 0.0f;
    self->mVtable = lbl_eu_80535744;
    self->mList = 0;
    self->mCapacity = 0;
    self->mStartNode.mItem.mTimer14 = 0.0f;
    self->mStartNode.mItem.mId18 = 0;
    self->mStartNode.mItem.mPointId1C = 0;
    self->mStartNode.mItem.mFlags = flags;
    self->mStartNode.mItem.mObj0 = 0;
    self->mStartNode.mItem.mObj4 = 0;
    self->mUnk040 = false;
    self->mStartPtr = &self->mStartNode;
    self->mStartNode.mNext = &self->mStartNode;
    self->mStartNode.mPrev = &self->mStartNode;
    self->mVtable = lbl_eu_8053572C;
    self->mMsgs.mSlots[0].mText[0] = 0;
    self->mMsgs.mSlots[0].mLen = 0;

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
    self->mList = (MineNode*)__construct_new_array(mem, (void*)func_80205F78, 0,
                                                   0x2C, 0x96);
    for (int i = 0; i < 150; i++) {
        ((MineNode*)((u8*)self->mList + i * 0x2C))->mNext = 0;
    }
    self->mCapacity = 0x96;

    // Reset the message ring and sound timers on the singleton.
    CfMapMineManager* mgr = lbl_eu_806646A0;
    if (mgr != 0) {
        MineNode* start = mgr->mStartPtr;
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

    MineNode* start = mgr->mStartPtr;
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
    MineNode* start = self->mStartPtr;
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
    void* vt2 = *(void**)obj;
    ((void (*)(void*, void*))(*(u32*)((u8*)vt2 + 0x9C)))(obj, &pt->mPosX);
    *(u8*)((u8*)obj + 0x90) = 0;

    return pt->mObj4 != 0;
}

// ---------------------------------------------------------------------------
// func_80207C08 - true when no active point matches (id, area, sub).
// ---------------------------------------------------------------------------
extern "C" int func_80207C08(u32 pointId, u32 area, u32 sub) {
    CfMapMineManager* mgr = lbl_eu_806646A0;
    if (mgr == 0 || pointId == 0) {
        return 1;
    }
    MineNode* start = mgr->mStartPtr;
    MineNode* n = start->mNext;
    while (n != start) {
        if (n->mItem.mId18 != 0 && (n->mItem.mFlags & 0x00010000) != 0 &&
            area == n->mItem.mArea1E && sub == n->mItem.mAreaSub1F &&
            pointId == n->mItem.mPointId1C &&
            (n->mItem.mFlags & 0x0000FF00) != 0) {
            return 0;
        }
        n = n->mNext;
    }
    return 1;
}

// ---------------------------------------------------------------------------
// func_80207C94 - snapshot active points into a 6-byte record array.
// ---------------------------------------------------------------------------
extern "C" void func_80207C94(u8* out) {
    memset(out, 0, 0x384);
    MineNode* start = lbl_eu_806646A0->mStartPtr;
    MineNode* n = start->mNext;
    while (n != start) {
        *(s16*)(out + 0x0) = (s16)n->mItem.mTimer14;
        *(out + 0x2) = (u8)((n->mItem.mFlags >> 8) & 0xFF);
        *(out + 0x3) = (u8)((n->mItem.mFlags >> 2) & 0xFF);
        *(out + 0x4) = n->mItem.mArea1E;
        *(out + 0x5) = n->mItem.mAreaSub1F;
        out += 6;
        n = n->mNext;
    }
}
