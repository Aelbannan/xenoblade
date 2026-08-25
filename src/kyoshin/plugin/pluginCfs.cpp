// Translation unit for kyoshin/plugin/pluginCfs
// Plugin script functions for the CFS (Common File System / script) subsystem.

// Retail calls func_8015783C with a full-width character id (no truncation
// mask at call sites); CfGameManager.hpp declares its dataId param as u16,
// which forces a rlwinm mask on every call and trips the illegal-overload
// rule if redeclared alongside. Rename the header form out of the way for
// this TU and declare the retail (int,int,int) shape instead.
#define func_8015783C func_8015783C_u16hdr
#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/plugin/pluginCfs.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#undef func_8015783C
extern "C" void* func_8015783C(int mappedIndex, int dataId, int slot);

// Retail passes the character id to func_8015783C untruncated; the
// CfGameManager.hpp declaration takes u16, which would force a rlwinm mask
// at every call. This int-param extern "C" overload resolves instead for
// integer arguments (approved reloc-name fix, PLAN.md 17.6).
extern "C" void* func_8015783C(int mappedIndex, int dataId, int slot);

struct UnkClass_8009ECB0;
struct UnkClass_805764CC;

// Planted s32->f32 conversion magic (0x4330000080000000 = 2^52 + 2^31),
// imported from retail sdata2: MWCC pools the builtin cast's magic as a
// TU-local label that resolves to this symbol (MWCC_CASES 7i).
extern const f64 lbl_eu_80665E40;

// ============================================================================
// External declarations (called functions not declared in included headers)
// ============================================================================
extern "C" {
    extern unsigned long lbl_eu_80663E24;  // game state flags
    extern unsigned long lbl_eu_80663E28;  // game state flags 2
    extern float lbl_eu_80663EDC;          // wait pop timer
    extern unsigned long lbl_eu_80663E14;  // ignore PAL flag
    extern float lbl_eu_80665E30;          // fixed-point divisor (4096.0f)
    extern float lbl_eu_80665E34;          // 0.5f
    extern float lbl_80665E38__Q23mtl10MemManager;          // scale factor
    extern float lbl_eu_80665E3C;          // 1.0f
    extern float lbl_eu_80665E48;          // various constants
    extern float lbl_eu_80665E4C;          // 0.0f

    void func_8004302C(int, int);
    void func_80043628();
    void func_800A3304();
    void func_801579A4();
    void func_8012FAA8();
    int func_8015796C(int);
    void func_80157184(int);
    void func_801571A8(int);
    // func_8015730C / func_8015783C / func_80157948 / CItem_initItemImplInstances /
    // func_80199678: declared by kyoshin/cf/CfGameManager.hpp above - local
    // re-declarations with different types trip MWCC 10197.
    void func_80159B40(int, int, void*);
    void func_80158118(void*, int, int);
    void func_80155A00(void*);
    void func_8016DF34(int);
    void func_8016DF4C(int);

    void func_801F4AD4(int, int);
    void func_801F4B68(int, int);
    void func_801F4BFC(int, int);
    void func_801F4C90(int, int, int);
    void func_eu_8049AB50(int, int);
    int getUnk80664658();
    int isTvFormatPal__9CDeviceVIFv();
    // CItem_initItemImplInstances: CfGameManager.hpp declares it returning
    // CItemImplInstances* - local void* form conflicts (10197).
    void* getInstance__14Class_80296898Fv();
    void func_800B70FC(int, int);
    int func_800B8D5C();
    void func_800BE28C(int, bool);
    bool func_8009CF8C(int);
    // func_8009D018: declared (u32,u32) by CfGameManager.hpp:767 - local
    // (int,int) form conflicts (10197).
    int* func_8009EC9C(int);
    int* func_8009ECB0();
    void func_8009E0A8(int*, int);
    int func_8009E284(int*, int);
    int* func_8009D790(int*, int);
    void func_800B6800(UnkClass_805764CC*, void*, int, float);    void* func_800B07E8__Fv();
    void func_800B1AF4(void*);
    void func_8007C360__Q22cf13CfGameManagerFv(float, int, int);
    void func_8007F830__Q22cf13CfGameManagerFv(int, int);
    void func_800823A4__Q22cf13CfGameManagerFv(int, int);
    void func_80083EA4__Q22cf13CfGameManagerFv();
    bool func_80084B68__Q22cf13CfGameManagerFv();
    void func_8008566C__Q22cf13CfGameManagerFv(int, float*, int);
    int func_80086B1C__Q22cf13CfGameManagerFv();
    void* func_80081CB8__Q22cf13CfGameManagerFv();
    void func_800AB978(void*, float, float, void*, void*);
    void func_800AB7F8(void*, float, void*, void*, int, int, int, int);
    void func_800ABD44(void*, float*, float*, float);
    void func_800ABDE4(void*, float*, float*, float);
    void func_800AC3F4(void*, const char*, const char*);
    void func_800ABE84(void*, float*, float*, float);
    void func_800AC378(void*, const char*, int);
    void func_800AB8CC(void*, float, void*, void*);
    void func_800ABA18(float, float*, float*);
    void func_800AC30C(void*, int, int, const char*);
    void func_80082254__Q22cf13CfGameManagerFv();
    void func_80086DAC__Q22cf13CfGameManagerFv();
    void func_80082258__Q22cf13CfGameManagerFv();
    void func_80080888__Q22cf13CfGameManagerFv(int, int);
    void func_80085E58__Q22cf13CfGameManagerFv(int, int);
    void func_80085FB8__Q22cf13CfGameManagerFv(int);
    void func_800B946C();
    void func_800B98C8(int);
    void func_80083888__Q22cf13CfGameManagerFv(const char*);
    bool func_80084BF4__Q22cf13CfGameManagerFv();
    bool func_80087250__Q22cf13CfGameManagerFv();
    void func_800B94A0(const char*);
    bool func_8009E344(void*, int, int*, int*);
    void func_8007C374__Q22cf13CfGameManagerFv(u32, u32, int, float);
    void func_8009E3C0();
    int  func_801586D4(int, int);
    void func_80159C04(int, int);
    int  func_80158068(int);
    void func_800F4004(void*);
    void func_80462D04__8CTaskLODFv(int);
    void func_80462D5C__8CTaskLODFv(int);
    void* func_804BC9EC__Fv();
    void func_804BCC30(void*, int);
    void func_804BCC3C(void*, int);
    void func_8047BD8C__17UnkClass_8047BB54Fv(void*, int);
    void func_8047BD94__17UnkClass_8047BB54Fv(void*, int);
}

// ============================================================================
// Batch 1: Plugin functions (pluginCfs.cpp)
// ============================================================================

// --- setMapJumpArea (us-8004785c) ---
// Two ids, four fixed-point corner triples plus an extra radius fixed, an
// optional fade (default 0x168 frames), a name string, a flag int, one
// fetched-but-discarded int and a final optional int. Retail passes the
// final call's first int argument from a register its own code never writes
// (stale r16), so optC below is deliberately left uninitialized.
int setMapJumpArea(VMThread* pThread) {
    // Scalar declarations up front: MWCC reserves callee-saved registers for
    // the whole block at entry in declaration order, and retail's pool runs
    // out exactly at aX (the 10th corner value).
    // Scalar declarations up front: MWCC colors this long-lived set in
    // descending declaration order from the top of the free callee-saved
    // range; retail's layout (fade r22 .. name/optC low) fixes this order.
    int fade;
    int flag;
    int idx;
    int idA; // first id read
    int idB; // second id read
    const char* name;
    int optC;
    int dX, dY, dZ, cX, cY, cZ, bX, bY, bZ, aX, aY, aZ, radius;

    idA = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    idB = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    dX = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    dY = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    dZ = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    cX = vmArgFixedGet(7, vmArgPtrGet(pThread, 6));
    cY = vmArgFixedGet(8, vmArgPtrGet(pThread, 7));
    cZ = vmArgFixedGet(9, vmArgPtrGet(pThread, 8));
    bX = vmArgFixedGet(10, vmArgPtrGet(pThread, 9));
    bY = vmArgFixedGet(11, vmArgPtrGet(pThread, 10));
    bZ = vmArgFixedGet(12, vmArgPtrGet(pThread, 11));
    aX = vmArgFixedGet(13, vmArgPtrGet(pThread, 12));
    aY = vmArgFixedGet(14, vmArgPtrGet(pThread, 13));
    aZ = vmArgFixedGet(15, vmArgPtrGet(pThread, 14));
    radius = vmArgFixedGet(16, vmArgPtrGet(pThread, 15));

    // Optional fade frames (default 0x168)
    idx = 16;
    if (vmArgOmitChk(pThread, idx)) {
        fade = 0x168;
        idx = 17;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx);
        idx = 17;
        fade = vmArgIntGet(idx, arg);
    }

    if (vmArgOmitChk(pThread, idx)) {
        name = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx);
        idx++;
        name = vmArgStringGet(idx, arg);
    }

    if (vmArgOmitChk(pThread, idx)) {
        flag = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx);
        idx++;
        flag = vmArgIntGet(idx, arg);
    }

    // Fourth optional: its present-path value reaches the final call.
    // optC is deliberately never written on the omitted path (retail reads
    // a stale register there).
    if (vmArgOmitChk(pThread, idx)) {
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx);
        idx++;
        optC = vmArgIntGet(idx, arg);
    }

    // Last optional: the omitted branch does not bump the rolling index.
    int optD;
    if (vmArgOmitChk(pThread, idx)) {
        optD = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx);
        idx++;
        optD = vmArgIntGet(idx, arg);
    }

    // All vectors declared up front so the stack layout matches retail
    // (scaled/center/copy chain low, corner triples high).
    ml::CVec3 scaledV;
    ml::CVec3 center;
    ml::CVec3 centerCopy;
    ml::CVec3 scaledCopy;
    ml::CVec3 cornerA;
    ml::CVec3 cornerB;
    ml::CVec3 cornerMax;
    ml::CVec3 cornerMin;

    // Fixed-point -> float conversions in retail store order: min triple,
    // max triple, B triple, A triple, radius last. Radius stays live across
    // the manager fetch so MWCC colors it into callee-saved f31.
    cornerMin.x = (float)(s32)dX / lbl_eu_80665E30;
    cornerMin.y = (float)(s32)dY / lbl_eu_80665E30;
    cornerMin.z = (float)(s32)dZ / lbl_eu_80665E30;
    cornerMax.x = (float)(s32)cX / lbl_eu_80665E30;
    cornerMax.y = (float)(s32)cY / lbl_eu_80665E30;
    cornerMax.z = (float)(s32)cZ / lbl_eu_80665E30;
    cornerB.x = (float)(s32)bX / lbl_eu_80665E30;
    cornerB.y = (float)(s32)bY / lbl_eu_80665E30;
    cornerB.z = (float)(s32)bZ / lbl_eu_80665E30;
    cornerA.x = (float)(s32)aX / lbl_eu_80665E30;
    cornerA.y = (float)(s32)aY / lbl_eu_80665E30;
    cornerA.z = (float)(s32)aZ / lbl_eu_80665E30;
    float warpH = (float)(s32)radius / lbl_eu_80665E30;

    cf::CfGameManager* mgr =
        (cf::CfGameManager*)func_80081CB8__Q22cf13CfGameManagerFv();
    if (mgr != NULL) {
        func_800AB978(mgr, warpH, lbl_eu_80665E34, &cornerB, &cornerA);
        func_800AB8CC(mgr, warpH, &cornerMin, &cornerMax);

        // Center of the min/max pair, duplicated, scaled, duplicated again.
        center.x = cornerMin.x + cornerMax.x;
        center.y = cornerMin.y + cornerMax.y;
        center.z = cornerMin.z + cornerMax.z;
        centerCopy = center;
        scaledV.x = centerCopy.x * lbl_80665E38__Q23mtl10MemManager;
        scaledV.y = centerCopy.y * lbl_80665E38__Q23mtl10MemManager;
        scaledV.z = centerCopy.z * lbl_80665E38__Q23mtl10MemManager;
        scaledCopy = scaledV;

        // Party-position dispatch through CfGameManager vtable slot 0x9C.
        ((cf::CfGameManagerVt158*)mgr)->setPos(&scaledCopy);

        func_800AC30C(mgr, idA, idB, name);

        // Preload notifier dispatch through CfGameManager vtable slot 0x158.
        ((cf::CfGameManagerVt158*)mgr)->notifyPreload(flag != 0);

        ((CfGameManagerWarpView*)mgr)->warpFade = (float)(s32)fade;
    }

    // Called unconditionally.
    func_8007C360__Q22cf13CfGameManagerFv(lbl_eu_80665E3C, optC, optD);
    return 0;
}

void func_80047814__Q22cf13CfObjectPointFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

// --- setMapJumpAreaBox (us-80047e18) ---
// Nine fixed-point corners plus two ids, an optional fade (default 0x168
// frames), a name string and two trailing optional ints. Retail reuses the
// first corner-x register for the final optional int (both fold into r16),
// so the overwrite below is intentional.
int setMapJumpAreaBox(VMThread* pThread) {
    // Three constant-indexed int triples: MWCC promotes fully-enregistered
    // small arrays as blocks, matching retail's contiguous callee-saved
    // groups r16-r18 (low), r19-r21 (high), r29-r31 (mid).
    int idx;
    int lowV[3];
    int highV[3];
    VMThread* th;
    int ids[2];
    int opt2;
    int flag;
    const char* name;
    int fade;
    int midV[3];

    // One contiguous 9-float block (retail sp+0x8..0x28): [0..2] low
    // corner triple, [3..5] mid, [6..8] high.
    float corners[9];

    th = pThread;
    ids[1] = vmArgIntGet(2, vmArgPtrGet(th, 1));
    ids[0] = vmArgIntGet(3, vmArgPtrGet(th, 2));
    lowV[0] = vmArgFixedGet(4, vmArgPtrGet(th, 3));
    lowV[1] = vmArgFixedGet(5, vmArgPtrGet(th, 4));
    lowV[2] = vmArgFixedGet(6, vmArgPtrGet(th, 5));
    midV[0] = vmArgFixedGet(7, vmArgPtrGet(th, 6));
    midV[1] = vmArgFixedGet(8, vmArgPtrGet(th, 7));
    midV[2] = vmArgFixedGet(9, vmArgPtrGet(th, 8));
    highV[0] = vmArgFixedGet(10, vmArgPtrGet(th, 9));
    highV[1] = vmArgFixedGet(11, vmArgPtrGet(th, 10));
    highV[2] = vmArgFixedGet(12, vmArgPtrGet(th, 11));

    // Optional fade frames (default 0x168)
    if (vmArgOmitChk(th, 12)) {
        fade = 0x168;
        idx = 13;
    } else {
        idx = 13;
        fade = vmArgIntGet(idx, vmArgPtrGet(th, 12));
    }

    // Optional name string
    if (vmArgOmitChk(th, idx)) {
        name = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(th, idx++);
        name = vmArgStringGet(idx, arg);
    }

    // Two more optional ints sharing the rolling index
    if (vmArgOmitChk(th, idx)) {
        flag = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(th, idx++);
        flag = vmArgIntGet(idx, arg);
    }

    if (vmArgOmitChk(th, idx)) {
        opt2 = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(th, idx++);
        opt2 = vmArgIntGet(idx, arg);
    }

    // Last optional int overwrites the first corner-x value (retail folds
    // both into r16); its omitted branch leaves the rolling index untouched.
    {
        int val = 0;
        if (!vmArgOmitChk(th, idx)) {
            VMArg* arg = vmArgPtrGet(th, idx++);
            val = vmArgIntGet(idx, arg);
        }
        lowV[0] = val;
    }

    // Fixed-point -> float corners in retail store order: high triple,
    // mid triple, low triple.
    corners[6] = (float)(s32)lowV[0] / lbl_eu_80665E30;
    corners[7] = (float)(s32)lowV[1] / lbl_eu_80665E30;
    corners[8] = (float)(s32)lowV[2] / lbl_eu_80665E30;
    corners[3] = (float)(s32)midV[0] / lbl_eu_80665E30;
    corners[4] = (float)(s32)midV[1] / lbl_eu_80665E30;
    corners[5] = (float)(s32)midV[2] / lbl_eu_80665E30;
    corners[0] = (float)(s32)highV[0] / lbl_eu_80665E30;
    corners[1] = (float)(s32)highV[1] / lbl_eu_80665E30;
    corners[2] = (float)(s32)highV[2] / lbl_eu_80665E30;

    cf::CfGameManager* mgr =
        (cf::CfGameManager*)func_80081CB8__Q22cf13CfGameManagerFv();
    if (mgr != NULL) {
        func_800AB978(mgr, lbl_eu_80665E48, lbl_eu_80665E34, &corners[3], &corners[3]);
        func_800ABA18(lbl_eu_80665E4C, &corners[6], &corners[0]);
        func_800AC30C(mgr, ids[1], ids[0], name);

        // Preload notifier dispatch through CfGameManager vtable slot 0x158.
        ((cf::CfGameManagerVt158*)mgr)->notifyPreload(flag != 0);

        ((CfGameManagerWarpView*)mgr)->warpFade = (float)(s32)fade;
    }

    // Called unconditionally, even without a manager.
    func_8007C360__Q22cf13CfGameManagerFv(lbl_eu_80665E3C, opt2, lowV[0]);
    return 0;
}

// --- setWarpArea (us-80048238) ---
// Builds a warp region from a base point plus two y-offset corners and a
// mirrored corner pair, hands it to the manager, then stores the optional
// fade timer and notifies through vtable slot 0x158.
// Declaration order drives MWCC's callee-saved coloring (descending from
// r31): retail wants hTop=r31 .. fx=r27, fade=r26, flag=r25, idx=r24,
// id0=r23, id1=r22, g0..g3=r21..r18, name=r17, optC=r16 and mid=r14.
int setWarpArea(VMThread* pThread) {
    int hTop;
    int fw;
    int fz;
    int fy;
    int fx;
    int fade;
    int flag;
    int idx;
    int id0;
    int id1;
    int g0;
    int g1;
    int g2;
    int g3;
    const char* name;
    int optC;
    // Arg 9 feeds only cornerMin.y; its register is recycled by the final
    // optional int (both land in retail r14).
    int mid;
    int optD;

    id0 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    id1 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    fx = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    fy = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    fz = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    fw = vmArgFixedGet(7, vmArgPtrGet(pThread, 6));
    hTop = vmArgFixedGet(8, vmArgPtrGet(pThread, 7));
    mid = vmArgFixedGet(9, vmArgPtrGet(pThread, 8));
    // Retail spills these three contiguously (sp+0x78..0x80); the array
    // gives them deterministic stack homes out of the coloring pool.
    int b[3];
    b[0] = vmArgFixedGet(10, vmArgPtrGet(pThread, 9));
    b[1] = vmArgFixedGet(11, vmArgPtrGet(pThread, 10));
    b[2] = vmArgFixedGet(12, vmArgPtrGet(pThread, 11));
    g0 = vmArgIntGet(13, vmArgPtrGet(pThread, 12));
    g1 = vmArgIntGet(14, vmArgPtrGet(pThread, 13));
    g2 = vmArgIntGet(15, vmArgPtrGet(pThread, 14));
    g3 = vmArgIntGet(16, vmArgPtrGet(pThread, 15));

    idx = 16;
    if (vmArgOmitChk(pThread, idx)) {
        fade = 0x168;
        idx++;
    } else {
        fade = vmArgIntGet(idx + 1, vmArgPtrGet(pThread, idx));
        idx++;
    }

    if (vmArgOmitChk(pThread, idx)) {
        name = 0;
        idx++;
    } else {
        name = vmArgStringGet(idx, vmArgPtrGet(pThread, idx++));
    }

    if (vmArgOmitChk(pThread, idx)) {
        flag = 0;
        idx++;
    } else {
        flag = vmArgIntGet(idx, vmArgPtrGet(pThread, idx++));
    }

    if (vmArgOmitChk(pThread, idx)) {
        optC = 0;
        idx++;
    } else {
        optC = vmArgIntGet(idx, vmArgPtrGet(pThread, idx++));
    }

    // Last optional: the omitted branch does not bump the rolling index.
    if (vmArgOmitChk(pThread, idx)) {
        optD = 0;
    } else {
        optD = vmArgIntGet(idx, vmArgPtrGet(pThread, idx++));
    }

    // Fixed-point -> float corners. Each conversion is written as an
    // independent expression in retail's store order (min.x, B.z, min.z,
    // max.x, B.x, max.z, max.y, B.y, min.y, warp height last); duplicating
    // the fx/fz/fy expressions keeps MWCC from CSE-ing the divisions.
    // All eight 12-byte vectors are declared up front so the stack layout
    // matches retail (scaled/center/copy chain low, corners high).
    ml::CVec3 scaledV;
    ml::CVec3 center;
    ml::CVec3 centerCopy;
    ml::CVec3 scaledCopy;
    ml::CVec3 cornerA;
    ml::CVec3 cornerB;
    ml::CVec3 cornerMax;
    ml::CVec3 cornerMin;

    cornerMin.x = (float)(s32)fx / lbl_eu_80665E30;
    cornerB.z = (float)(s32)b[2] / lbl_eu_80665E30;
    cornerMin.z = (float)(s32)fz / lbl_eu_80665E30;
    cornerMax.x = (float)(s32)fx / lbl_eu_80665E30;
    cornerB.x = (float)(s32)b[0] / lbl_eu_80665E30;
    cornerMax.z = (float)(s32)fz / lbl_eu_80665E30;
    cornerA = cornerB;
    cornerMax.y = (float)(s32)fy / lbl_eu_80665E30 +
                  (float)(s32)hTop / lbl_eu_80665E30;
    cornerB.y = (float)(s32)b[1] / lbl_eu_80665E30;
    cornerMin.y = (float)(s32)fy / lbl_eu_80665E30 -
                  (float)(s32)mid / lbl_eu_80665E30;
    float warpH = (float)(s32)fw / lbl_eu_80665E30;

    cf::CfGameManager* mgr =
        (cf::CfGameManager*)func_80081CB8__Q22cf13CfGameManagerFv();
    if (mgr != NULL) {
        func_800AB7F8(mgr, warpH, &cornerMin, &cornerMax, g0, g1, g2, g3);
        func_800AB978(mgr, warpH, lbl_eu_80665E34, &cornerB, &cornerA);

        // Center of the min/max pair, duplicated, scaled, duplicated again.
        center.x = cornerMin.x + cornerMax.x;
        center.y = cornerMin.y + cornerMax.y;
        center.z = cornerMin.z + cornerMax.z;
        centerCopy = center;
        scaledV.x = centerCopy.x * lbl_80665E38__Q23mtl10MemManager;
        scaledV.y = centerCopy.y * lbl_80665E38__Q23mtl10MemManager;
        scaledV.z = centerCopy.z * lbl_80665E38__Q23mtl10MemManager;
        scaledCopy = scaledV;

        func_800AC30C(mgr, id0, id1, name);

        // Preload notifier dispatch through CfGameManager vtable slot 0x158.
        ((cf::CfGameManagerVt158*)mgr)->notifyPreload(flag != 0);

        ((CfGameManagerWarpView*)mgr)->warpFade = (float)(s32)fade;
    }

    func_8007C360__Q22cf13CfGameManagerFv(lbl_eu_80665E3C, optC, optD);
    return 0;
}

// --- setMapPreloadArea (us-800487dc) ---
// All fixed-point -> float conversions happen up front; the warp height stays
// live in a local across the manager fetch so MWCC colors it into the
// callee-saved f31 (retail saves/restores f31 around the whole body).
int setMapPreloadArea(VMThread* pThread) {
    int argA = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int argB = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    int fx0 = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    int fy0 = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    int fz0 = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    int fx1 = vmArgFixedGet(7, vmArgPtrGet(pThread, 6));
    int fy1 = vmArgFixedGet(8, vmArgPtrGet(pThread, 7));
    int fz1 = vmArgFixedGet(9, vmArgPtrGet(pThread, 8));
    int fw = vmArgFixedGet(10, vmArgPtrGet(pThread, 9));

    int flag;
    if (vmArgOmitChk(pThread, 10)) {
        flag = 0;
    } else {
        flag = vmArgIntGet(11, vmArgPtrGet(pThread, 10));
    }

    // Stack-slot order matches retail: min block low (0x8, declared first),
    // max block high (0x14); max converts first.
    float boxMax[3];
    float boxMin[3];

    // Builtin biased int->float casts emit MWCC's 0x4330/xoris idiom; the
    // pooled magic resolves to the shared retail blob lbl_eu_80665E40.
    boxMax[0] = (float)(s32)fx0 / lbl_eu_80665E30;
    boxMax[1] = (float)(s32)fy0 / lbl_eu_80665E30;
    boxMax[2] = (float)(s32)fz0 / lbl_eu_80665E30;
    boxMin[0] = (float)(s32)fx1 / lbl_eu_80665E30;
    boxMin[1] = (float)(s32)fy1 / lbl_eu_80665E30;
    boxMin[2] = (float)(s32)fz1 / lbl_eu_80665E30;
    float scale = (float)(s32)fw / lbl_eu_80665E30;

    cf::CfGameManager* mgr =
        (cf::CfGameManager*)func_80081CB8__Q22cf13CfGameManagerFv();
    if (mgr != NULL) {
        func_800ABD44(mgr, boxMax, boxMin, scale);
        func_800AC30C(mgr, argA, argB, 0);

        // Preload notifier dispatch through CfGameManager vtable slot 0x158.
        ((cf::CfGameManagerVt158*)mgr)->notifyPreload(flag != 0);
    }

    return 0;
}

// --- mapJump (us-80048a5c) ---
int mapJump(VMThread* pThread) {
    int areaId = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int subId = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    int fx = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    int fy = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    int fz = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    int rot = vmArgIntGet(7, vmArgPtrGet(pThread, 6));

    int optIdx = 8;
    const char* name;
    if (vmArgOmitChk(pThread, 7)) {
        name = 0;
    } else {
        name = vmArgStringGet(8, vmArgPtrGet(pThread, 7));
    }

    // Optional warp-flag then fade-frames args share the rolling index
    int flag;
    if (vmArgOmitChk(pThread, optIdx)) {
        flag = 0;
    } else {
        flag = vmArgIntGet(optIdx + 1, vmArgPtrGet(pThread, optIdx));
    }
    optIdx++;

    int frames;
    if (vmArgOmitChk(pThread, optIdx)) {
        frames = 0;
    } else {
        frames = vmArgIntGet(optIdx + 1, vmArgPtrGet(pThread, optIdx));
        optIdx++;
    }

    // Fixed-point -> float position (MWCC 0x4330/xoris conversion idiom);
    // separate assignments let each result die immediately (no FPR saves)
    float pt[3];
    pt[0] = (float)(s32)fx / lbl_eu_80665E30;
    pt[1] = (float)(s32)fy / lbl_eu_80665E30;
    pt[2] = (float)(s32)fz / lbl_eu_80665E30;

    func_80083D50__Q22cf13CfGameManagerFv(areaId & 0xFFFF, subId & 0xFFFF,
                                           (u32)&pt[0], (u32)name,
                                           (float)(s32)rot / lbl_eu_80665E30);
    func_8007C374__Q22cf13CfGameManagerFv(flag, frames, lbl_eu_80665E3C, 1);
    return 0;
}

// --- setMapPreloadArea2 (us-80048c84) ---
int setMapPreloadArea2(VMThread* pThread) {
    return 0;
}

// --- setEventArea (us-80048ecc) ---
// Builds an event trigger area from a fixed-point base point plus top/bottom
// y extents, hands it to the manager, tags it with two strings, then fires
// the preload notifier (vtable slot 0x158) gated by an optional flag.
int setEventArea(VMThread* pThread) {
    // Declaration order drives MWCC's callee-saved coloring; retail wants
    // flag=r31, the five fixed-point values as a block r26-30, tag=r25,
    // name=r24 - consistent with the originals being an args array.
    int flag;
    int args[5];
    const char* tag;
    const char* eventName;

    eventName = vmArgStringGet(2, vmArgPtrGet(pThread, 1));
    args[0] = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    args[1] = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    args[2] = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    args[3] = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    args[4] = vmArgFixedGet(7, vmArgPtrGet(pThread, 6));

    // Optional tag string (arg 7): literals only, like retail.
    // One variable serves as the rolling optional-index (8) and is then
    // overwritten with the final flag value, matching retail's r31 reuse.
    if (vmArgOmitChk(pThread, 7)) {
        tag = 0;
        flag = 8;
    } else {
        flag = 8;
        VMArg* arg = vmArgPtrGet(pThread, 7);
        tag = vmArgStringGet(flag, arg);
    }

    // Optional flag (arg 8)
    if (vmArgOmitChk(pThread, flag)) {
        flag = 0;
    } else {
        int next = flag + 1;
        VMArg* arg = vmArgPtrGet(pThread, flag);
        flag = vmArgIntGet(next, arg);
    }

    // Fixed-point -> float conversions in retail store order; the three base
    // coordinates stay contiguous so the array can be passed by pointer.
    float box[3];
    box[0] = (float)(s32)args[0] / lbl_eu_80665E30;
    box[1] = (float)(s32)args[1] / lbl_eu_80665E30;
    box[2] = (float)(s32)args[2] / lbl_eu_80665E30;
    float top = (float)(s32)args[3] / lbl_eu_80665E30;
    float bottom = (float)(s32)args[4] / lbl_eu_80665E30;

    void* mgr = func_80081CB8__Q22cf13CfGameManagerFv();
    if (mgr != NULL) {
        ml::CVec3 pt;
        pt.x = box[0];
        pt.y = box[1] + bottom;
        pt.z = box[2];
        func_800ABDE4(mgr, (float*)&pt, box, top);
        func_800AC3F4(mgr, eventName, tag);

        // Preload notifier dispatch through CfGameManager vtable slot 0x158.
        ((cf::CfGameManagerVt158*)mgr)->notifyPreload(flag != 0);
    }

    return 0;
}

// --- delEventArea (us-80049130) ---
int delEventArea(VMThread* pThread) {
    const char* str;
    if (vmArgOmitChk(pThread, 1)) {
        str = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        str = vmArgStringGet(2, arg);
    }
    func_800B94A0(str);
    return 0;
}

// --- setTownArea (us-80049190) ---
// Reads five fixed-point coords plus an optional name/flag pair, converts
// the coords around a manager fetch (height/top-offset stay in callee-saved
// f31/f30 across it), builds "<name><ext>" tags for two script-string slots
// (extension literal advanced by 3 for the second tag), then fires the
// preload notifier (vtable slot 0x158).
// Tag scratch: each record is a 0x20-char buffer followed by its u32 length.
struct TownTag {
    char buf[0x20];
    u32 len;
};

extern char lbl_eu_804FB040[];  // pooled ".xxx" extension literal

int setTownArea(VMThread* pThread) {
    // Declaration order drives MWCC's callee-saved coloring: flag=r31,
    // the five fixed-point values as a block r26-r30, name=r24 (see
    // setEventArea); the rolling optional index lands in r25 and the
    // manager pointer reuses it once the index dies.
    int flag;
    int idx;
    const char* name;

    // Five fixed-point reads as one multi-declarator group.
    int a0 = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    int a1 = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    int a2 = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    int fh = vmArgFixedGet(5, vmArgPtrGet(pThread, 4)),
        ft = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    name = vmArgStringGet(7, vmArgPtrGet(pThread, 6));

    if (vmArgOmitChk(pThread, 7)) {
        flag = 0;
        idx = 8;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, 7);
        idx = 8;
        flag = vmArgIntGet(8, arg);
    }

    // Two trailing optional ints fetched and discarded. The rolling-index
    // bump sits between argument setup and the pointer fetch
    // (vmArgPtrGet(pThread, idx++)); the last block's read arm is inline.
    if (vmArgOmitChk(pThread, idx)) {
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        vmArgIntGet(idx, arg);
    }
    if (!vmArgOmitChk(pThread, idx)) {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        vmArgIntGet(idx, arg);
    }

    // Fixed-point -> float conversions in retail store order; the height
    // param and top offset survive the manager fetch in callee-saved FPRs.
    ml::CVec3 box;
    float height;
    float topOff;
    box.x = (float)(s32)a0 / lbl_eu_80665E30;
    box.y = (float)(s32)a1 / lbl_eu_80665E30;
    box.z = (float)(s32)a2 / lbl_eu_80665E30;
    height = (float)(s32)fh / lbl_eu_80665E30;
    topOff = (float)(s32)ft / lbl_eu_80665E30;

    void* mgr = func_80081CB8__Q22cf13CfGameManagerFv();
    if (mgr != NULL) {
        // Struct copy from box then y-patch: reproduces retail's integer
        // lwz/stw bit-copy triple plus the lfs/fadds/stfs y update.
        ml::CVec3 pt = box;
        pt.y = pt.y + topOff;
        func_800ABE84(mgr, (float*)&box, (float*)&pt, height);

        if (name != NULL) {
        // Declared highest-first: MWCC puts the first local at the top of
        // the frame, retail order is d:0x8c, c:0x68, b:0x44, a:0x20.
        const char* ext = lbl_eu_804FB040;
        TownTag d;
        TownTag c;
        TownTag b;
        TownTag a;

            d.buf[0] = '\0';
            d.len = 0;
            c.len = strlen(name);
            strcpy(c.buf, name);

            // First tag: name + ext
            b.len = strlen(c.buf);
            strcpy(b.buf, c.buf);
            u32 extLen = strlen(ext);
            strcat(b.buf, ext);
            b.len += extLen;
            d.len = strlen(b.buf);
            strcpy(d.buf, b.buf);
            func_800AC378(mgr, d.buf, 0);

            // Second tag: name + (ext + 3)
            ext += 3;
            a.len = strlen(c.buf);
            strcpy(a.buf, c.buf);
            extLen = strlen(ext);
            strcat(a.buf, ext);
            a.len += extLen;
            d.len = strlen(a.buf);
            strcpy(d.buf, a.buf);
            func_800AC378(mgr, d.buf, 1);
        } else {
            func_800AC378(mgr, name, 0);
            func_800AC378(mgr, name, 1);
        }

        // Preload notifier dispatch through CfGameManager vtable slot 0x158.
        ((cf::CfGameManagerVt158*)mgr)->notifyPreload(flag != 0);
    }

    return 0;
}

// --- addPopID (us-8004953c) ---
int addPopID(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    vmArgIntGet(2, arg);
    func_80082254__Q22cf13CfGameManagerFv();
    return 0;
}

// --- setTimeSpeed (us-80049574) ---
int setTimeSpeed(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    vmArgIntGet(2, arg);
    func_80086DAC__Q22cf13CfGameManagerFv();
    return 0;
}

// --- setPopSheet (us-800495ac) ---
int setPopSheet(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    vmArgStringGet(2, arg);
    func_80082258__Q22cf13CfGameManagerFv();
    return 0;
}

// --- changeWalker (us-800495e4) ---
int changeWalker(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgIntGet(2, arg);
    func_80080888__Q22cf13CfGameManagerFv(val & 0xFFFF, 1);
    return 0;
}

// --- eventStart (us-80049624) ---
int eventStart(VMThread* pThread) {
    int eventId;
    int idx;
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        idx = 2;
        eventId = vmArgIntGet(2, arg);
    } else {
        eventId = 0;
        idx = 2;
    }

    // Fused ternary keeps the priority value in the return-value register
    // across both arms, matching retail's register allocation.
    func_80085E58__Q22cf13CfGameManagerFv(
        eventId,
        vmArgOmitChk(pThread, idx)
            ? 0
            : vmArgIntGet(idx, vmArgPtrGet(pThread, idx++)));
    func_800F4004(getInstance__Q22cf14CBattleManagerFv());
    return 0;
}

unsigned long battleEventStart() {
    lbl_eu_80663E24 |= 0x40000;
    return 0;
}

// --- eventEnd (us-800496fc) ---
int eventEnd(VMThread* pThread) {
    int val;
    if (vmArgOmitChk(pThread, 1)) {
        val = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        val = vmArgIntGet(2, arg);
    }
    func_80085FB8__Q22cf13CfGameManagerFv(val);
    return 0;
}

// --- delAttr (us-8004975c) ---
int delAttr(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    vmArgIntGet(2, arg);
    func_800B946C();
    return 0;
}

// --- setMono (us-80049794) ---
int setMono(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgBoolGet(2, arg);
    int neg = -val;
    int orVal = neg | val;
    func_800B98C8((unsigned int)orVal >> 31);
    return 0;
}

// --- setMapDispID (us-800497d8) ---
int setMapDispID(VMThread* pThread) {
    int v26, v27, v28, v29, v30;
    
    {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        v26 = vmArgIntGet(2, arg);
    }
    {
        VMArg* arg = vmArgPtrGet(pThread, 2);
        v27 = vmArgBoolGet(3, arg);
    }
    {
        VMArg* arg = vmArgPtrGet(pThread, 3);
        v28 = vmArgBoolGet(4, arg);
    }
    {
        VMArg* arg = vmArgPtrGet(pThread, 4);
        v29 = vmArgBoolGet(5, arg);
    }
    {
        VMArg* arg = vmArgPtrGet(pThread, 5);
        v30 = vmArgBoolGet(6, arg);
    }
    
    void* gm = cf::CfGameManager::func_80083298();
    if (gm) {
        if (v27) {
            if (v28) func_80462D04__8CTaskLODFv(v26);
            if (v29) { func_804BCC30(func_804BC9EC__Fv(), v26); }
            if (v30) func_8047BD8C__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, v26);
        } else {
            if (v28) func_80462D5C__8CTaskLODFv(v26);
            if (v29) { func_804BCC3C(func_804BC9EC__Fv(), v26); }
            if (v30) func_8047BD94__17UnkClass_8047BB54Fv((u8*)gm + 0xF0, v26);
        }
    }
    return 0;
}

// --- loadCfEvent (us-80049918) ---
int loadCfEvent(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    const char* str = vmArgStringGet(2, arg);
    func_80083888__Q22cf13CfGameManagerFv(str);
    return 0;
}

// --- waitCfEvent (us-80049950) ---
int waitCfEvent(VMThread* pThread) {
    if (func_80084BF4__Q22cf13CfGameManagerFv()) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

// --- isMainParty (us-80049990) ---
int isMainParty(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgIntGet(2, arg);
    
    VMArg ret;
    int out[2];
    out[0] = 0;
    out[1] = -1;
    int* data = func_8009ECB0();
    func_8009E344((void*)data, val, &out[0], &out[1]);
    
    int result = out[0];
    int orVal = (1 - result) | (result - 1);
    
    *(u8*)&ret.type = ((unsigned int)orVal >> 31) + 1;
    vmRetValSet(pThread, &ret);
    return 1;
}

// --- isResvParty (us-80049a28) ---
int isResvParty(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgIntGet(2, arg);
    
    VMArg ret;
    int out[2];
    out[0] = 0;
    out[1] = -1;
    int* data = func_8009ECB0();
    func_8009E344((void*)data, val, &out[0], &out[1]);
    
    int result = out[0];
    int orVal = (2 - result) | (result - 2);
    
    *(u8*)&ret.type = ((unsigned int)orVal >> 31) + 1;
    vmRetValSet(pThread, &ret);
    return 1;
}

// --- addParty (us-80049ac0) ---
int addParty(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int v31 = vmArgIntGet(2, arg);

    // out slot pair probed from the party manager (count / reserved)
    int out[2];
    out[0] = 0;
    out[1] = -1;
    func_8009E344(func_8009ECB0(), v31, &out[0], &out[1]);

    // count==2 (or the folded duplicate test) skips the group bookkeeping
    int isEq = ((unsigned)__cntlzw(out[0] - 2)) >> 5;
    if (isEq != 1 && isEq != 2) {
        UnkClass_8009ECB0* data2 = (UnkClass_8009ECB0*)func_8009ECB0();
        int group = 0;

        if (v31 == 9)       group = 6;
        else if (v31 == 10) group = 7;
        else if (v31 == 6)  group = 9;
        else if (v31 == 7)  group = 10;
        else if (v31 == 12) group = 4;
        else if (v31 == 13) group = 4;

        if (group) func_8009E740((cf::CtrlObjectParamSlots*)data2, group);

        if (!func_8009E56C((cf::CtrlObjectParamSlots*)data2, v31, 1)) {
            func_8009E56C((cf::CtrlObjectParamSlots*)data2, v31, 2);
        }
    }

    func_800823A4__Q22cf13CfGameManagerFv(v31, 1);
    return 0;
}

// --- makeParty (us-80049bf8) ---
// Reads a leader id plus six optional party ids and a warp flag, fills an
// 18-byte u16 party record, snapshots each player's position/scale, applies
// the party, then restores positions/scales through the player vtable.
int makeParty(VMThread* pThread) {
        VMArg* arg = vmArgPtrGet(pThread, 1);
    int id0 = vmArgIntGet(2, arg);
    int id1, id2, id3, id4, id5, id6;
    int warpFlag;
    int idx = 2;

    // Optional args 2..8 share a rolling index; each omitted arg defaults
    // to 0 except the final bool which defaults to true.
    if (vmArgOmitChk(pThread, 2)) {
        id1 = 0;
        idx = 3;
    } else {
        VMArg* a = vmArgPtrGet(pThread, 2);
        idx = 3;
        id1 = vmArgIntGet(3, a);
    }

    if (vmArgOmitChk(pThread, idx)) {
        id2 = 0;
        idx++;
    } else {
        int cur = idx;
        idx++;
        VMArg* a = vmArgPtrGet(pThread, cur);
        id2 = vmArgIntGet(idx, a);
    }
    if (vmArgOmitChk(pThread, idx)) {
        id3 = 0;
        idx++;
    } else {
        int cur = idx;
        idx++;
        VMArg* a = vmArgPtrGet(pThread, cur);
        id3 = vmArgIntGet(idx, a);
    }
    if (vmArgOmitChk(pThread, idx)) {
        id4 = 0;
        idx++;
    } else {
        int cur = idx;
        idx++;
        VMArg* a = vmArgPtrGet(pThread, cur);
        id4 = vmArgIntGet(idx, a);
    }
    if (vmArgOmitChk(pThread, idx)) {
        id5 = 0;
        idx++;
    } else {
        int cur = idx;
        idx++;
        VMArg* a = vmArgPtrGet(pThread, cur);
        id5 = vmArgIntGet(idx, a);
    }
    if (vmArgOmitChk(pThread, idx)) {
        id6 = 0;
        idx++;
    } else {
        int cur = idx;
        idx++;
        VMArg* a = vmArgPtrGet(pThread, cur);
        id6 = vmArgIntGet(idx, a);
    }

    // Final optional bool defaults to true when omitted.
    if (vmArgOmitChk(pThread, idx)) {
        warpFlag = 1;
    } else {
        int cur = idx;
        idx++;
        VMArg* a = vmArgPtrGet(pThread, cur);
        warpFlag = vmArgBoolGet(idx, a);
    }

    // 18-byte party record: seven u16 ids then zero tail
    u16 party[9];
    memset(party, 0, 0x12);
    party[0] = id0;
    party[1] = id1;
    party[2] = id2;
    party[3] = id3;
    party[4] = id4;
    party[5] = id5;
    party[6] = id6;

    ml::CVec3 pos[3];
    float scale[3];
    // Zero scale held in a local across the loop so MWCC colors it into
    // the callee-saved f31 (retail saves/restores f31).
    const float zeroF = lbl_eu_80665E4C;
    for (int i = 0; i < 3; i++) {
        pos[i] = ml::CVec3::zero;
        scale[i] = zeroF;
        cf::unkPartyPlayerDispatch* player =
            (cf::unkPartyPlayerDispatch*)cf::CfGameManager::getPlayer(i);
        if (player != NULL) {
            pos[i] = *player->getPosPtr();
            scale[i] = player->getScale();
        }
    }

    func_8007F1FC__Q22cf13CfGameManagerFv(party, 1);

    if (warpFlag != 0) {
        int* d = func_8009ECB0();
        func_80080888__Q22cf13CfGameManagerFv(d[1] & 0xFFFF, 1);
    }

    if (cf::CfGameManager::func_800822F4() == 1 &&
        func_80087250__Q22cf13CfGameManagerFv()) {
        func_8012FAA8();
    }

    for (int i = 0; i < 3; i++) {
        cf::unkPartyPlayerDispatch* player =
            (cf::unkPartyPlayerDispatch*)cf::CfGameManager::getPlayer(i);
        if (player != NULL) {
            player->setPos(&pos[i]);
            player->setScale(scale[i]);
        }
    }

    return 0;
}

float cf::CfObject::CfObject_UnkVirtualFunc31() {
    return *(float*)((u8*)this + 0x4C);
}

// --- makeGuestParty (us-80049f84) ---
// Rolling optional-arg walk; the index bump sits between argument setup and
// the call in retail, written here as a post-increment on vmArgPtrGet's arg.
int makeGuestParty(VMThread* pThread) {
    int v30, v29;
    int v31 = 1;
    
    if (vmArgOmitChk(pThread, 1)) { v30 = 0; v31 = 2; }
    else { VMArg* arg = vmArgPtrGet(pThread, v31++); v30 = vmArgIntGet(v31, arg); }
    
    if (vmArgOmitChk(pThread, v31)) { v29 = 0; v31++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v31++); v29 = vmArgIntGet(v31, arg); }
    
    if (vmArgOmitChk(pThread, v31)) { v31 = 0; }
    else { VMArg* arg = vmArgPtrGet(pThread, v31++); v31 = vmArgIntGet(v31, arg); }
    
    int* data = func_8009ECB0();
    data[0x28 / 4] = v30 & 0xFFFF;
    data[0x2C / 4] = v29 & 0xFFFF;
    data[0x30 / 4] = v31 & 0xFFFF;
    func_8009E3C0();
    return 0;
}

// --- delParty (us-8004a0a0) ---
int delParty(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int v31 = vmArgIntGet(2, arg);
    int* data = func_8009ECB0();
    func_8009E740((cf::CtrlObjectParamSlots*)data, v31);
    return 0;
}

// --- addItem (us-8004a0ec) ---
int addItem(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgIntGet(2, arg);
    int result = func_801586D4(val & 0xFFFF, 1);
    
    int neg = -result;
    int orVal = neg | result;
    int boolVal = (unsigned int)orVal >> 31;
    int clz = __cntlzw(boolVal);
    
    VMArg ret;
    *(u8*)&ret.type = ((unsigned int)clz >> 5) + 1;
    vmRetValSet(pThread, &ret);
    return 1;
}

// --- delItem (us-8004a160) ---
int delItem(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int v31 = vmArgIntGet(2, arg);
    
    int val;
    if (vmArgOmitChk(pThread, 2)) {
        val = 1;
    } else {
        VMArg* arg2 = vmArgPtrGet(pThread, 2);
        val = vmArgIntGet(3, arg2);
    }
    
    func_80159C04(v31 & 0xFFFF, val);
    return 0;
}

// --- totalItem (us-8004a1ec) ---
int totalItem(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgIntGet(2, arg);
    int count = func_80158068(val & 0xFFFF);
    
    VMArg ret;
    ret.type = 3;
    ret.value.intVal = count;
    vmRetValSet(pThread, &ret);
    return 1;
}

// ============================================================================
// Target functions to match (batch 2)
// ============================================================================

// --- equipItem (us-8004a24c) ---
// Reads a character id plus five optional slot ids (rolling omitted-index,
// defaults -1), then for each equip slot 0..4 syncs the character record and
// (for party characters 1-11) the inventory entry.
// The five optionals stay in scalars until after the func_8009CF8C gate;
// only then are they spilled into the stack array (retail stores the block
// at sp+0x10 right before func_8009EC9C).
int equipItem(VMThread* vmThread) {
    // Same rolling-optional idiom as the matched makeGuestParty above:
    // index starts at the first omitted arg; each fetch is
    // vmArgPtrGet(thread, idx++) / vmArgIntGet(idx, arg). The fifth value
    // lands back in the index variable itself (retail folds both into r29).
    int s0, s1, s2, s3;
    int idx;
    int changed;
    int slot;
    int outInt;
    short outShort;
    int scratch;

    int charId = vmArgIntGet(2, vmArgPtrGet(vmThread, 1));
    idx = 2;

    if (vmArgOmitChk(vmThread, idx)) { s0 = -1; idx++; }
    else { VMArg* a = vmArgPtrGet(vmThread, idx++); s0 = vmArgIntGet(idx, a); }

    if (vmArgOmitChk(vmThread, idx)) { s1 = -1; idx++; }
    else { VMArg* a = vmArgPtrGet(vmThread, idx++); s1 = vmArgIntGet(idx, a); }
    if (vmArgOmitChk(vmThread, idx)) { s2 = -1; idx++; }
    else { VMArg* a = vmArgPtrGet(vmThread, idx++); s2 = vmArgIntGet(idx, a); }
    if (vmArgOmitChk(vmThread, idx)) { s3 = -1; idx++; }
    else { VMArg* a = vmArgPtrGet(vmThread, idx++); s3 = vmArgIntGet(idx, a); }
    if (vmArgOmitChk(vmThread, idx)) { idx = -1; }
    else { VMArg* a = vmArgPtrGet(vmThread, idx++); idx = vmArgIntGet(idx, a); }

    if (func_8009CF8C(0x3508)) {
        return 0;
    }

    int slots[5];
    slots[0] = s0;
    changed = 0;
    slots[1] = s1;
    slots[2] = s2;
    slots[3] = s3;
    slots[4] = idx;

    int* charData = func_8009EC9C(charId & 0xFFFF);
    int* mgrData = func_8009ECB0();
    // Note: full (unmasked) char id passed here, unlike EC9C above.
    // The E284 result reuses the rolling-index variable (retail folds both
    // into r29).
    idx = func_8009E284(mgrData, charId);

    for (slot = 0; slot <= 4; slot++) {
        int* slotRec = func_8009D790(&charData[7], slot);
        int itemId = slots[slot];
        int act = 0;

        if (slotRec != NULL) {
            // Same item type already equipped: nothing to do
            if (itemId >= 0 && itemId != (*slotRec >> 20)) {
                // Item-type probe via the singleton (result discarded)
                ((CItemImplInstance*)CItem_initItemImplInstances())
                    ->getItemId(slotRec);
                changed = 1;
                act = 1;
            }
        } else {
            if (itemId < 0) {
                // Negative id: read back the currently equipped id
                itemId = *(u16*)((char*)charData + slot * 2 + 2);
            } else {
                act = 1;
            }
            changed = 1;
        }

        if (act) {
            if (idx != 0) {
                func_80158420(itemId & 0xFFFF, &outShort, 1, &outInt);
                func_8009DBF4(charData, slot, (void*)(s32)outShort);
            } else {
                func_8009E0C4(charData, slot & 0xFFFF, itemId & 0xFFFF);
            }

            // Equip-slot index -> item-type id
            int type = 0;
            if (slot == 0) type = 4;
            else if (slot == 1) type = 5;
            else if (slot == 2) type = 6;
            else if (slot == 3) type = 7;
            else if (slot == 4) type = 8;
            else if (slot == 5) type = 2;

            if (charId >= 1 && charId <= 11) {
                void* invSlot = func_8015783C(type, charId, 0);
                if (itemId == 0) {
                    // Empty equip: refresh the inventory entry's item view
                    ((CItemImplInstance*)CItem_initItemImplInstances())
                        ->getItemId(invSlot);
                } else {
                    func_80158118(invSlot, itemId & 0xFFFF, 1);
                }

                for (int k = 0; k < 8; k++) {
                    s8* ref = (s8*)func_80157948(charId, k);
                    if (*ref == slot + 1) {
                        func_80159B40(charId, k, &scratch);
                    }
                }
            }
        }
    }

    if (changed) {
        func_800B70FC(charId, 0);
    }

    return 0;
}

// --- equipWeapon (us-8004a608) ---
int equipWeapon(VMThread* pThread) {
    int charId;
    int weaponId;
    int* charData;
    int* weaponSlot;

    {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        charId = vmArgIntGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(pThread, 2);
        weaponId = vmArgIntGet(3, arg);
    }

    if (func_8009CF8C(0x3508)) {
        return 0;
    }

    charData = func_8009EC9C(charId & 0xFFFF);
    weaponSlot = func_8009D790(&charData[7], 5);

    if (weaponSlot != NULL) {
        // Already equipped with this type: nothing to do
        if (weaponId == (int)((u32)*weaponSlot >> 20)) {
            return 0;
        }
        // Item-type probe via the item implementation singleton (result unused)
        ((CItemImplInstance*)CItem_initItemImplInstances())->getItemId(weaponSlot);
        short slotVal;
        int unkVal;
        func_80158420(weaponId & 0xFFFF, &slotVal, 1, &unkVal);
        func_8009E0A8(charData, slotVal);
        func_800B70FC(charId, 0);
        lbl_eu_80663E28 |= 0x08000000;
    } else {
        func_8009E0C4(func_8009EC9C(charId & 0xFFFF), 5, weaponId & 0xFFFF);

        if (charId >= 1 && charId <= 11) {
            int syncId;
            void* invSlot = func_8015783C(2, charId, 0);
            if (weaponId == 0) {
                // Empty equip: refresh the inventory entry's item view (discarded)
                ((CItemImplInstance*)CItem_initItemImplInstances())->getItemId(invSlot);
            } else {
                func_80158118(invSlot, weaponId & 0xFFFF, 1);
                func_80155A00(invSlot);
            }

            for (int k = 0; k < 8; k++) {
                u8* slotRef = (u8*)func_80157948(charId, k);
                if (*slotRef == 6) {
                    func_80159B40(charId, k, &syncId);
                }
            }
        }
    }

    return 0;
}

// --- getWeaponSlot (us-8004a7c8) ---
// Reads the equipped weapon id of a character. Prefers the character's
// weapon slot record (+0x1C list, index 5); if absent, falls back to the
// inventory slot 2 entry when its item-type nibble is 2 (weapon).
int getWeaponSlot(VMThread* vmThread) {
    int* slotData;
    int charId;
    int itemId;

    VMArg* arg = vmArgPtrGet(vmThread, 1);
    charId = vmArgIntGet(2, arg);
    int* charData = func_8009EC9C(charId & 0xFFFF);
    itemId = 0;
    slotData = func_8009D790(&charData[7], 5);

    if (slotData != NULL) {
        CItemImplInstance* inst = (CItemImplInstance*)CItem_initItemImplInstances();
        itemId = inst->getWeaponSlot(slotData) & 0xFFFF;
    } else {
        // Swapped first guard blocks MWCC's unsigned-range fusion so the
        // two signed cmpi/branch pairs stay byte-identical to retail.
        do {
            if (1 > charId) break;
            if (charId > 11) break;
            u32* invSlot = (u32*)func_8015783C(2, charId, 0);
            // Item-type nibble sits at bits 16-19; reading it unsigned lets
            // MWCC fold shift+mask into a single rotate and emit cmpli.
            if ((*invSlot >> 16 & 0xF) == 2) {
                CItemImplInstance* inst = (CItemImplInstance*)CItem_initItemImplInstances();
                itemId = inst->getWeaponSlot((void*)invSlot) & 0xFFFF;
            }
        } while (0);
    }

    VMArg ret;
    ret.type = 3;
    ret.value.intVal = itemId;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- setWeaponSlot (us-8004a8d8) ---
// Writes a weapon id into the character's weapon slot (or the inventory
// fallback for guest characters 1-11 without a slot record).
int setWeaponSlot(VMThread* vmThread) {
    int* slotData;
    int charId;
    int itemId;

    VMArg* arg = vmArgPtrGet(vmThread, 1);
    charId = vmArgIntGet(2, arg);
    arg = vmArgPtrGet(vmThread, 2);
    itemId = vmArgIntGet(3, arg);

    int* charData = func_8009EC9C(charId & 0xFFFF);
    slotData = func_8009D790(&charData[7], 5);

    if (slotData != NULL) {
        CItemImplInstance* inst = (CItemImplInstance*)CItem_initItemImplInstances();
        inst->setWeaponSlot(slotData, itemId);
    } else {
        // Swapped-operand guards block MWCC's range-check fusion (see getWeaponSlot).
        do {
            if (1 > charId) break;
            if (11 < charId) break;
            int* invSlot = (int*)func_8015783C(2, charId, 0);
            CItemImplInstance* inst = (CItemImplInstance*)CItem_initItemImplInstances();
            inst->setWeaponSlot(invSlot, itemId);
        } while (0);
    }

    VMArg ret;
    ret.type = 3;
    ret.value.intVal = itemId;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- waitEventRes (us-8004a9ec) ---
int waitEventRes(VMThread* vmThread) {
    int waitMode = 0;

    if (!func_80084B68__Q22cf13CfGameManagerFv()) {
        waitMode = 1;
    }

    if (waitMode) {
        vmWaitModeSet(vmThread);
    }

    return 0;
}

// --- setFade (us-8004aa44) ---
// Converts four fixed-point color args (and an optional frame count) to a
// float RGBA array; the (f32)(s32) casts emit MWCC's 0x4330/xoris/lfd/fsubs
// idiom with the magic and the 4096.0 divisor pooled/named like retail.
int setFade(VMThread* vmThread) {
    VMArg* arg = vmArgPtrGet(vmThread, 1);
    int fixedR = vmArgFixedGet(2, arg);
    arg = vmArgPtrGet(vmThread, 2);
    int fixedG = vmArgFixedGet(3, arg);
    arg = vmArgPtrGet(vmThread, 3);
    int fixedB = vmArgFixedGet(4, arg);
    arg = vmArgPtrGet(vmThread, 4);
    int fixedA = vmArgFixedGet(5, arg);

    int frames;
    if (vmArgOmitChk(vmThread, 5)) {
        frames = 0;
    } else {
        frames = vmArgIntGet(6, vmArgPtrGet(vmThread, 5));
    }

    float color[4];
    color[0] = (float)(s32)fixedR / lbl_eu_80665E30;
    color[1] = (float)(s32)fixedG / lbl_eu_80665E30;
    color[2] = (float)(s32)fixedB / lbl_eu_80665E30;
    color[3] = (float)(s32)fixedA / lbl_eu_80665E30;

    func_8008566C__Q22cf13CfGameManagerFv(frames & 0xFFFF, color, 1);
    return 0;
}

// --- applyPcPrm (us-8004ab9c) ---
int applyPcPrm(VMThread* vmThread) {
    int charId;
    int param;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        charId = vmArgIntGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        param = vmArgIntGet(3, arg);
    }

    if (func_8009CF8C(0x3508)) {
        return 0;
    }

    func_8007F830__Q22cf13CfGameManagerFv(charId, param);
    func_800823A4__Q22cf13CfGameManagerFv(param, 0);
    return 0;
}

// --- setDispOffArea (us-8004ac34) ---
// Looks up the map object referenced by the script OC argument and pushes a
// display-offset area id onto it. The int->float conversion uses MWCC's
// 0x4330/xoris double-magic idiom pooled at lbl_eu_80665E40.
int setDispOffArea(VMThread* vmThread) {
    VMArg* arg = vmArgPtrGet(vmThread, 1);
    void* ocObj = vmArgOCGet(2, arg);
    arg = vmArgPtrGet(vmThread, 2);
    int areaId = vmArgFixedGet(3, arg);

    void* obj = func_801864DC(func_801862C0(), *(int*)((u8*)ocObj + 4));

    if (obj != NULL) {
        // The singleton from func_800B07E8__Fv is passed as the callee's self.
        // The int->float arg uses MWCC's builtin biased-conversion idiom
        // (xoris/lis 0x4330/stw/stw/lfd/fsubs); its magic pools as a TU-local
        // label that resolves to lbl_eu_80665E40 (MWCC_CASES 7i/fsub-rule).
        UnkClass_805764CC* box = (UnkClass_805764CC*)func_800B07E8__Fv();
        func_800B6800(box, obj, 1, areaId);
    }

    return 0;
}

// --- setScheduleType (us-8004ace0) ---
int setScheduleType(VMThread* vmThread) {
    void* ocObj;
    int enable;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        ocObj = vmArgOCGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        enable = vmArgBoolGet(3, arg);
    }

    void* obj = func_801864DC(func_801862C0(), *(int*)((u8*)ocObj + 4));

    if (obj != NULL) {
        int flags = *(int*)((u8*)obj + 0x64);
        if (flags & 0x08) {
            func_800BE28C((int)obj, enable);
        }
    }

    return 0;
}

// --- setWeather (us-8004ad78) ---
extern "C" int setWeather(VMThread* vmThread) {
    int weatherId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        weatherId = vmArgIntGet(2, arg);
    }

    func_8016DF34(weatherId);
    return 0;
}

// --- setWeatherArea (us-8004adb0) ---
int setWeatherArea(VMThread* vmThread) {
    int areaId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        areaId = vmArgIntGet(2, arg);
    }

    func_8016DF4C(areaId);
    return 0;
}

// --- setGimmick (us-8004ade8) ---
int setGimmick(VMThread* vmThread) {
    int gimmickId;
    int state;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        gimmickId = vmArgIntGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        state = vmArgIntGet(3, arg);
    }

    int unk = getUnk80664658();
    if (unk) {
        if (state == 0)
            func_801F4AD4(unk, gimmickId);
        else if (state == 1)
            func_801F4B68(unk, gimmickId);
        else if (state == 2)
            func_801F4BFC(unk, gimmickId);
    }

    return 0;
}

// --- setElvGim (us-8004ae94) ---
int setElvGim(VMThread* vmThread) {
    int gimmickId;
    int value;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        gimmickId = vmArgIntGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        value = vmArgIntGet(3, arg);
    }

    int unk = getUnk80664658();
    if (unk) {
        func_801F4C90(unk, gimmickId, value);
    }

    return 0;
}

// --- setActMapObj (us-8004af14) ---
int setActMapObj(VMThread* vmThread) {
    int mapObjId;
    int state;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        mapObjId = vmArgIntGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        state = vmArgIntGet(3, arg);
    }

    void* mapObj = func_80186BC8(mapObjId & 0xFFFF);
    if (mapObj != NULL) {
        int* objPtr = (int*)((u8*)mapObj + 0xC4);
        if (*objPtr != 0) {
            func_800BE12C((u8*)mapObj, state, 0, -1, 1);
        }
    }

    return 0;
}

// --- getMapID (us-8004afac) ---
int getMapID(VMThread* vmThread) {
    VMArg ret;
    ret.type = 3;
    ret.value.intVal = func_80086B1C__Q22cf13CfGameManagerFv();
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- clearGimmickJump (us-8004aff4) ---
int clearGimmickJump(VMThread* vmThread) {
    unsigned long flags = lbl_eu_80663E24;
    lbl_eu_80663E24 = flags & ~0x80000;

    if (flags & 0x80) {
        func_80083EA4__Q22cf13CfGameManagerFv();
    }

    return 0;
}

// --- partyMember (us-8004b02c) ---
int partyMember(VMThread* vmThread) {
    int memberIdx;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        memberIdx = vmArgIntGet(2, arg);
    }

    int* partyData = func_8009ECB0();
    int offset = (memberIdx - 1) * 4;
    int memberVal = *(int*)((u8*)partyData + 4 + offset);
    int clz = __cntlzw(memberVal);

    VMArg ret;
    *(u8*)&ret.type = ((unsigned int)clz >> 5) + 1;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- clearPartyGauge (us-8004b0a8) ---
int clearPartyGauge(VMThread* vmThread) {
    void* battleMgr = getInstance__Q22cf14CBattleManagerFv();
    if (battleMgr != NULL) {
        void* gauge = getInstance__Q22cf14CBattleManagerFv();
        func_8018C8F4((u8*)gauge + 0x194, 0);
    }
    return 0;
}

// --- waitPop (us-8004b0e4) ---
int waitPop(VMThread* vmThread) {
    if (!func_800B8D5C()) {
        if (!(lbl_eu_80663E28 & 0x10)) {
            lbl_eu_80663EDC = lbl_eu_80665E4C;
        }
        lbl_eu_80663E28 |= 0x10;
        vmWaitModeSet(vmThread);
    } else {
        lbl_eu_80663E28 &= ~0x10;
    }

    return 0;
}

// --- partyWarp (us-8004b154) ---
int partyWarp(VMThread* vmThread) {
    for (int i = 1; i < 3; i++) {
        cf::unkPartyPlayerDispatch* player =
            (cf::unkPartyPlayerDispatch*)cf::CfGameManager::getPlayer(i);
        if (player != NULL) {
            void* task = player->getTask();
            if (task != NULL) {
                func_80199678((u8*)task + 0x8C, 1);
            }
        }
    }
    return 0;
}

// --- setMoney (us-8004b1dc) ---
int setMoney(VMThread* vmThread) {
    int amount;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        amount = vmArgIntGet(2, arg);
    }

    func_80157184(amount);
    return 0;
}

// --- addMoney (us-8004b214) ---
int addMoney(VMThread* vmThread) {
    int amount;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        amount = vmArgIntGet(2, arg);
    }

    func_801571A8(amount);
    return 0;
}

// --- isTimeSkip (us-8004b24c) ---
int isTimeSkip(VMThread* vmThread) {
    unsigned long flags = lbl_eu_80663E28;
    int bit = (flags >> 26) & 1;
    int result = (bit == 0) ? 2 : 1;

    VMArg ret;
    ret.type = result;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- delHoldBox (us-8004b288) ---
int delHoldBox(VMThread* vmThread) {
    int charId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        charId = vmArgIntGet(2, arg);
    }

    for (int slot = 4; slot <= 8; slot++) {
        func_8015730C(charId, 0, slot);
    }

    func_8015730C(charId, 0, 2);

    int buf;
    for (int k = 0; k < 8; k++) {
        void* slotRef = func_80157948(charId, k);
        if ((signed char)(*(u8*)slotRef) != 0) {
            func_80159B40(charId, k, &buf);
        }
    }

    return 0;
}

// --- getWeaponID (us-8004b338) ---
int getWeaponID(VMThread* vmThread) {
    int charId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        charId = vmArgIntGet(2, arg);
    }

    int* charData = func_8009EC9C(charId & 0xFFFF);

    VMArg ret;
    ret.type = 3;
    ret.value.intVal = *(u16*)((u8*)charData + 0x0C);
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- addItemLimit (us-8004b41c) ---
int addItemLimit(VMThread* vmThread) {
    int limit;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        limit = vmArgIntGet(2, arg);
    }

    func_8015796C(limit);
    return 0;
}

// --- setPcCtrl (us-8004b49c) ---
int setPcCtrl(VMThread* vmThread) {
    int enable;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        enable = vmArgBoolGet(2, arg);
    }

    lbl_eu_80663E24 = enable ? (lbl_eu_80663E24 & ~0x2000) : (lbl_eu_80663E24 | 0x2000);
    return 0;
}

// --- saveNamedCount (us-8004b4fc) ---
int saveNamedCount(VMThread* vmThread) {
    int count = func_80291BF8();
    int total = count + func_8009CF8C(0x10A);
    func_8009D018(0x10A, total < 0 ? 0 : (total > 0x3E7 ? 0x3E7 : total));
    return 0;
}

// --- isPal (us-8004b55c) ---
int isPal(VMThread* vmThread) {
    int isPal = isTvFormatPal__9CDeviceVIFv();
    int result = (isPal == 0) ? 2 : 1;

    VMArg ret;
    ret.type = result;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- setIgnorePal (us-8004b5a8) ---
int setIgnorePal(VMThread* vmThread) {
    VMArg* arg = vmArgPtrGet(vmThread, 1);
    int val = vmArgBoolGet(2, arg);
    int neg = -val;
    int orVal = neg | val;
    func_eu_8049AB50(lbl_eu_80663E14, (unsigned int)orVal >> 31);
    return 0;
}

// --- isVoiceJP (us-8004b5f4) ---
int isVoiceJP(VMThread* vmThread) {
    void* instance = getInstance__14Class_80296898Fv();
    int lang = *(u8*)((u8*)instance + 0x1C);
    int orVal = (1 - lang) | (lang - 1);

    VMArg ret;
    ret.type = ((unsigned int)orVal >> 31) + 1;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// ============================================================================
// Already-matched functions (kept as-is)
// ============================================================================

bool func_8004A2E8() {
    return false;
}

void func_8004A400() {
    // Intentionally empty
}

void* CfObject_UnkVirtualFunc48__Q22cf12CfObjectMoveFv(void* self) {
    void* ptr = *(void**)((u8*)self + 0xC4);
    if (ptr == NULL) {
        return *(void**)((u8*)self + 0x6C0);
    }
    return *(void**)((u8*)ptr + 8);
}

int clearTbox() {
    void* ptr = func_800B07E8__Fv();
    if (ptr) func_800B1AF4(ptr);
    return 0;
}

int returnTitle() {
    func_80043628();
    return 0;
}

int dispLoading() {
    func_8004302C(1, 0);
    return 0;
}

int clearItemLimit() {
    func_801579A4();
    return 0;
}

int clearEquipGem() {
    func_800A3304();
    return 0;
}

unsigned long setFieldVision() {
    lbl_eu_80663E28 |= 0x20000000;
    return 0;
}

void pluginCfsRegist(){
    extern char lbl_eu_804FB040[];
    extern PluginFuncData lbl_eu_80525F98[];
    vmPluginRegist(&lbl_eu_804FB040[6], lbl_eu_80525F98);
}