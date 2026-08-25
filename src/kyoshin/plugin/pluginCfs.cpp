// Translation unit for kyoshin/plugin/pluginCfs
// Plugin script functions for the CFS (Common File System / script) subsystem.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/plugin/pluginCfs.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

struct UnkClass_8009ECB0;

// Planted s32->f32 conversion magic (0x4330000080000000): naming one pooled
// constant makes MWCC unify the literal pool so the builtin conversion's
// anonymous @N label resolves to this retail sdata2 address
// (MWCC_CASES: ocUnit::turn pool-unification idiom).
const f64 lbl_eu_80665E40 = 4503601774854144.0;

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
    void func_800BE12C(int, int, int, int, int);
    void func_800BE28C(int, bool);
    bool func_8009CF8C(int);
    // func_8009D018: declared (u32,u32) by CfGameManager.hpp:767 - local
    // (int,int) form conflicts (10197).
    int* func_8009EC9C(int);
    int* func_8009ECB0();
    void func_8009E0A8(int*, int);
    int func_8009E284(int*, int);
    int* func_8009D790(int*, int);
    void func_800B6800(float, void*, int);    void* func_800B07E8__Fv();
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
    void func_800AB8CC(float, float*, float*);
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
int setMapJumpArea(VMThread* pThread) {
    int args[15];
    int v22, v20, v17, v21, v16, v14_int;
    
    // Read args 1-15: vmArgIntGet(2, ptr) for arg1, vmArgFixedGet for args 3-15
    // Arg 1: int
    {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        args[0] = vmArgIntGet(2, arg);
    }
    // Args 2-15: each is vmArgFixedGet via vmArgPtrGet
    for (int i = 1; i < 15; i++) {
        VMArg* arg = vmArgPtrGet(pThread, i + 1);
        if (i < 13) {
            args[i] = vmArgFixedGet(i + 2, arg);
        } else {
            args[i] = vmArgFixedGet(i + 2, arg);
        }
    }
    
    // Arg 16: optional int
    if (vmArgOmitChk(pThread, 16)) {
        v22 = 0x168;
        v20 = 17;
    } else {
        v20 = 17;
        VMArg* arg = vmArgPtrGet(pThread, 16);
        v22 = vmArgIntGet(17, arg);
    }
    
    // Arg 17: optional string
    v17 = 0;
    if (!vmArgOmitChk(pThread, v20)) {
        VMArg* arg = vmArgPtrGet(pThread, v20);
        v20++;
        v17 = (int)vmArgStringGet(v20, arg);
    } else {
        v20++;
    }
    
    // Arg 18: optional int
    v21 = 0;
    if (!vmArgOmitChk(pThread, v20)) {
        VMArg* arg = vmArgPtrGet(pThread, v20);
        v20++;
        v21 = vmArgIntGet(v20, arg);
    } else {
        v20++;
    }
    
    // Arg 19: optional int
    v16 = 0;
    if (!vmArgOmitChk(pThread, v20)) {
        VMArg* arg = vmArgPtrGet(pThread, v20);
        v20++;
        v16 = vmArgIntGet(v20, arg);
    } else {
        v20++;
    }
    
    // Arg 20: optional int
    v14_int = 0;
    if (!vmArgOmitChk(pThread, v20)) {
        VMArg* arg = vmArgPtrGet(pThread, v20);
        v20++;
        v14_int = vmArgIntGet(v20, arg);
    }
    
    // Convert fixed-point args (3-15) to floats
    float v3 = (float)(s32)args[2] / 4096.0f;
    float v4 = (float)(s32)args[3] / 4096.0f;
    float v5 = (float)(s32)args[4] / 4096.0f;
    float v6 = (float)(s32)args[5] / 4096.0f;
    float v7 = (float)(s32)args[6] / 4096.0f;
    float v8 = (float)(s32)args[7] / 4096.0f;
    float v9 = (float)(s32)args[8] / 4096.0f;
    float v10 = (float)(s32)args[9] / 4096.0f;
    float v11 = (float)(s32)args[10] / 4096.0f;
    float v12 = (float)(s32)args[11] / 4096.0f;
    float v13 = (float)(s32)args[12] / 4096.0f;
    float v14f = (float)(s32)args[13] / 4096.0f;
    float v15f = (float)(s32)args[14] / 4096.0f;
    
    float t1[3] = {v3, v4, v5};
    float t2[3] = {v6, v7, v8};
    float t3[3] = {v9, v10, v11};
    float t4[3] = {v12, v13, v14f};
    
    void* gm = func_80081CB8__Q22cf13CfGameManagerFv();
    if (gm) {
        func_800AB978(gm, v15f, lbl_eu_80665E34, t4, t3);
        func_800AB8CC(v15f, t1, t2);
        
        float sum[3];
        sum[0] = t1[0] + t2[0];
        sum[1] = t1[1] + t2[1];
        sum[2] = t1[2] + t2[2];
        
        float scaled[3];
        scaled[0] = sum[0] * lbl_80665E38__Q23mtl10MemManager;
        scaled[1] = sum[1] * lbl_80665E38__Q23mtl10MemManager;
        scaled[2] = sum[2] * lbl_80665E38__Q23mtl10MemManager;
        
        func_800AC30C(gm, args[0], args[1], (const char*)v17);
        
        int neg = -v21;
        int orVal = neg | v21;
        int boolVal = (unsigned int)orVal >> 31;
        func_8007C360__Q22cf13CfGameManagerFv((float)(s32)v22, boolVal, v16);
    }
    
    func_8007C360__Q22cf13CfGameManagerFv(1.0f, v14_int, v16);
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
int setMapJumpAreaBox(VMThread* pThread) {
    int args[12];
    int v28, v15, v27, v26, v25;
    
    // Args 1-2: ints
    {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        args[0] = vmArgIntGet(2, arg);
    }
    {
        VMArg* arg = vmArgPtrGet(pThread, 2);
        args[1] = vmArgIntGet(3, arg);
    }
    // Args 3-11: fixed-point
    for (int i = 2; i < 11; i++) {
        VMArg* arg = vmArgPtrGet(pThread, i + 1);
        args[i] = vmArgFixedGet(i + 2, arg);
    }
    
    // Arg 12: optional int
    if (vmArgOmitChk(pThread, 12)) {
        v28 = 0x168;
        v15 = 13;
    } else {
        v15 = 13;
        VMArg* arg = vmArgPtrGet(pThread, 12);
        v28 = vmArgIntGet(13, arg);
    }
    
    // Arg 13: optional string
    v27 = 0;
    if (!vmArgOmitChk(pThread, v15)) {
        VMArg* arg = vmArgPtrGet(pThread, v15);
        v15++;
        v27 = (int)vmArgStringGet(v15, arg);
    } else {
        v15++;
    }
    
    // Arg 14: optional int
    v26 = 0;
    if (!vmArgOmitChk(pThread, v15)) {
        VMArg* arg = vmArgPtrGet(pThread, v15);
        v15++;
        v26 = vmArgIntGet(v15, arg);
    } else {
        v15++;
    }
    
    // Arg 15: optional int
    v25 = 0;
    if (!vmArgOmitChk(pThread, v15)) {
        VMArg* arg = vmArgPtrGet(pThread, v15);
        v15++;
        v25 = vmArgIntGet(v15, arg);
    } else {
        v15++;
    }
    
    // Arg 16: optional int
    int v16 = 0;
    if (!vmArgOmitChk(pThread, v15)) {
        VMArg* arg = vmArgPtrGet(pThread, v15);
        v15++;
        v16 = vmArgIntGet(v15, arg);
    }
    
    // Convert fixed-point to floats
    float f1 = (float)(s32)args[2] / 4096.0f;
    float f2 = (float)(s32)args[3] / 4096.0f;
    float f3 = (float)(s32)args[4] / 4096.0f;
    float f4 = (float)(s32)args[5] / 4096.0f;
    float f5 = (float)(s32)args[6] / 4096.0f;
    float f6 = (float)(s32)args[7] / 4096.0f;
    float f7 = (float)(s32)args[8] / 4096.0f;
    float f8 = (float)(s32)args[9] / 4096.0f;
    float f9 = (float)(s32)args[10] / 4096.0f;
    
    float tA[3] = {f1, f2, f3};
    float tB[3] = {f4, f5, f6};
    float tC[3] = {f7, f8, f9};
    
    void* gm = func_80081CB8__Q22cf13CfGameManagerFv();
    if (gm) {
        func_800AB978(gm, lbl_eu_80665E48, lbl_eu_80665E34, tB, tB);
        func_800ABA18(0.0f, tA, tC);
        func_800AC30C(gm, args[0], args[1], (const char*)v27);
        
        int neg = -v26;
        int orVal = neg | v26;
        int boolVal = (unsigned int)orVal >> 31;
        func_8007C360__Q22cf13CfGameManagerFv((float)(s32)v28, boolVal, v25);
    }
    
    func_8007C360__Q22cf13CfGameManagerFv(1.0f, v25, v16);
    return 0;
}

// --- setWarpArea (us-80048238) ---
// Builds a warp region from a base point plus two y-offset corners and a
// mirrored corner pair, hands it to the manager, then stores the optional
// fade timer and notifies through vtable slot 0x158.
int setWarpArea(VMThread* pThread) {
    int id0 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int id1 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    int fx = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    int fy = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    int fz = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    int fw = vmArgFixedGet(7, vmArgPtrGet(pThread, 6));
    int hTop = vmArgFixedGet(8, vmArgPtrGet(pThread, 7));
    vmArgFixedGet(9, vmArgPtrGet(pThread, 8)); // fetched but unused
    int bx = vmArgFixedGet(10, vmArgPtrGet(pThread, 9));
    int by = vmArgFixedGet(11, vmArgPtrGet(pThread, 10));
    int bz = vmArgFixedGet(12, vmArgPtrGet(pThread, 11));
    int g0 = vmArgIntGet(13, vmArgPtrGet(pThread, 12));
    int g1 = vmArgIntGet(14, vmArgPtrGet(pThread, 13));
    int g2 = vmArgIntGet(15, vmArgPtrGet(pThread, 14));
    int g3 = vmArgIntGet(16, vmArgPtrGet(pThread, 15));

    int idx = 16;
    int fade;
    if (vmArgOmitChk(pThread, idx)) {
        fade = 0x168;
        idx = 17;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx);
        idx = 17;
        fade = vmArgIntGet(idx, arg);
    }

    const char* name;
    if (vmArgOmitChk(pThread, idx)) {
        name = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx);
        idx++;
        name = vmArgStringGet(idx, arg);
    }

    int flag;
    if (vmArgOmitChk(pThread, idx)) {
        flag = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx);
        idx++;
        flag = vmArgIntGet(idx, arg);
    }

    int optC;
    if (vmArgOmitChk(pThread, idx)) {
        optC = 0;
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
    cornerB.z = (float)(s32)bz / lbl_eu_80665E30;
    cornerMin.z = (float)(s32)fz / lbl_eu_80665E30;
    cornerMax.x = (float)(s32)fx / lbl_eu_80665E30;
    cornerB.x = (float)(s32)bx / lbl_eu_80665E30;
    cornerMax.z = (float)(s32)fz / lbl_eu_80665E30;
    cornerA = cornerB;
    cornerMax.y = (float)(s32)fy / lbl_eu_80665E30 +
                  (float)(s32)hTop / lbl_eu_80665E30;
    cornerB.y = (float)(s32)by / lbl_eu_80665E30;
    cornerMin.y = (float)(s32)fy / lbl_eu_80665E30 -
                  (float)(s32)optD / lbl_eu_80665E30;
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

    // Fixed-point -> float area bounds (MWCC 0x4330/xoris conversion idiom)
    float boxMin[3];
    float boxMax[3];
    boxMin[0] = (float)(s32)fx0 / lbl_eu_80665E30;
    boxMin[1] = (float)(s32)fy0 / lbl_eu_80665E30;
    boxMin[2] = (float)(s32)fz0 / lbl_eu_80665E30;
    boxMax[0] = (float)(s32)fx1 / lbl_eu_80665E30;
    boxMax[1] = (float)(s32)fy1 / lbl_eu_80665E30;
    boxMax[2] = (float)(s32)fz1 / lbl_eu_80665E30;

    void* gm = func_80081CB8__Q22cf13CfGameManagerFv();
    if (gm != NULL) {
        func_800ABD44(gm, boxMax, boxMin, (float)(s32)fw / lbl_eu_80665E30);
        func_800AC30C(gm, argA, argB, 0);

        // Register the preload request via the manager's virtual notifier
        cf::CfGameManager* mgr = (cf::CfGameManager*)gm;
        typedef void (cf::CfGameManager::*Notifier)(bool);
        (mgr->**(Notifier*)((void**)(*(void**)mgr) + 0x158 / 4))(flag != 0);
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
int setEventArea(VMThread* pThread) {
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
int setTownArea(VMThread* pThread) {
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
    int v31, v30;
    
    if (vmArgOmitChk(pThread, 1)) {
        v31 = 0;
        v30 = 2;
    } else {
        v30 = 2;
        VMArg* arg = vmArgPtrGet(pThread, 1);
        v31 = vmArgIntGet(2, arg);
    }
    
    int val;
    if (vmArgOmitChk(pThread, v30)) {
        val = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, v30);
        v30++;
        val = vmArgIntGet(v30, arg);
    }
    
    func_80085E58__Q22cf13CfGameManagerFv(v31, val);
    void* bm = getInstance__Q22cf14CBattleManagerFv();
    func_800F4004(bm);
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
    u32 isEq = (u32)__cntlzw(out[0] - 2) >> 5;
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

    int id1;
    if (vmArgOmitChk(pThread, 2)) {
        id1 = 0;
    } else {
        id1 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    }

    // Rolling optional-index walk over args 3..7
    int idx = 3;
    int id2, id3, id4, id5, id6;
    if (vmArgOmitChk(pThread, idx)) { id2 = 0; idx++; }
    else { id2 = vmArgIntGet(idx + 1, vmArgPtrGet(pThread, idx)); idx++; }
    if (vmArgOmitChk(pThread, idx)) { id3 = 0; idx++; }
    else { id3 = vmArgIntGet(idx + 1, vmArgPtrGet(pThread, idx)); idx++; }
    if (vmArgOmitChk(pThread, idx)) { id4 = 0; idx++; }
    else { id4 = vmArgIntGet(idx + 1, vmArgPtrGet(pThread, idx)); idx++; }
    if (vmArgOmitChk(pThread, idx)) { id5 = 0; idx++; }
    else { id5 = vmArgIntGet(idx + 1, vmArgPtrGet(pThread, idx)); idx++; }
    if (vmArgOmitChk(pThread, idx)) { id6 = 0; idx++; }
    else { id6 = vmArgIntGet(idx + 1, vmArgPtrGet(pThread, idx)); idx++; }

    // Final optional bool defaults to true when omitted
    int warpFlag;
    if (vmArgOmitChk(pThread, idx)) {
        warpFlag = 1;
        idx++;
    } else {
        warpFlag = vmArgBoolGet(idx + 1, vmArgPtrGet(pThread, idx));
        idx++;
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
    float zeroF = lbl_eu_80665E4C;
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
int makeGuestParty(VMThread* pThread) {
    int v30, v29, v31;
    
    if (vmArgOmitChk(pThread, 1)) { v30 = 0; v31 = 2; }
    else { v31 = 2; VMArg* arg = vmArgPtrGet(pThread, 1); v30 = vmArgIntGet(2, arg); }
    
    if (vmArgOmitChk(pThread, v31)) { v29 = 0; v31++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v31); v31++; v29 = vmArgIntGet(v31, arg); }
    
    if (vmArgOmitChk(pThread, v31)) { v31 = 0; }
    else { VMArg* arg = vmArgPtrGet(pThread, v31); v31++; v31 = vmArgIntGet(v31, arg); }
    
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
int equipItem(VMThread* vmThread) {
    int charId;
    int slotIds[5] = {-1, -1, -1, -1, -1};
    int slotIdx = 1;

    // Get charId from arg 1
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        charId = vmArgIntGet(slotIdx, arg);
        slotIdx++;
    }

    // Read optional slot args (up to 5)
    for (int i = 0; i < 5; i++) {
        if (vmArgOmitChk(vmThread, slotIdx)) {
            slotIdx++;
        } else {
            VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
            slotIdx++;
            slotIds[i] = vmArgIntGet(slotIdx, arg);
            slotIdx++;
        }
    }

    // Early return if item limit reached
    if (func_8009CF8C(0x3508)) {
        return 0;
    }

    // Align charId and get character data
    int* charData = func_8009EC9C(charId & 0xFFFF);
    int* partyData = func_8009ECB0();
    int isNewItem = func_8009E284(partyData, charId & 0xFFFF);
    bool anyChanged = false;

    for (int slot = 0; slot <= 4; slot++) {
        int* slotData = func_8009D790(&charData[7], slot);
        int itemId = slotIds[slot];
        bool doAction = false;

        if (slotData != NULL) {
            int slotType = (*slotData) >> 20;
            if (itemId < 0) {
                doAction = true;
            } else if (itemId == slotType) {
                goto skip_slot;
            } else {
                CItem_initItemImplInstances();
                doAction = true;
                anyChanged = true;
            }
        } else {
            if (itemId < 0) {
                charData[slot + 1] = (charData[slot + 1] & 0xFFFF0000) | (itemId & 0xFFFF);
            }
            doAction = true;
            anyChanged = true;
        }

        if (!doAction) goto skip_slot;

        if (itemId) {
            int slotType;
            switch (slot) {
                case 0: slotType = 4; break;
                case 1: slotType = 5; break;
                case 2: slotType = 6; break;
                case 3: slotType = 7; break;
                case 4: slotType = 8; break;
            }

            if (charId >= 1 && charId <= 11) {
                void* invSlot = func_8015783C(slotType, charId, 0);
                if (itemId != 0) {
                    func_80158118(invSlot, itemId & 0xFFFF, 1);
                } else {
                    CItem_initItemImplInstances();
                }

                int newSlotIdx = slot + 1;
                for (int k = 0; k < 8; k++) {
                    void* slotRef = func_80157948(charId, k);
                    if ((signed char)(*(u8*)slotRef) == newSlotIdx) {
                        func_80159B40(charId, k, &slotIds[0]);
                    }
                }
            }
        }
        skip_slot:;
    }

    if (anyChanged) {
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
        // Swapped-operand guards block MWCC's range-check fusion so the two
        // signed cmpi/branch pairs stay byte-identical to retail.
        do {
            if (0 >= charId) break;
            if (11 < charId) break;
            int* invSlot = (int*)func_8015783C(2, charId, 0);
            if (((*invSlot) >> 12 & 0xF) == 2) {
                CItemImplInstance* inst = (CItemImplInstance*)CItem_initItemImplInstances();
                itemId = inst->getWeaponSlot(invSlot) & 0xFFFF;
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
            if (0 >= charId) break;
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
        void* box = func_800B07E8__Fv();
        float fAreaId = areaId;
        func_800B6800(fAreaId, obj, 1);
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
            func_800BE12C((int)mapObj, state, 0, -1, 1);
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