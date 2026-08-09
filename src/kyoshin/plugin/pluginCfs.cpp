// Translation unit for kyoshin/plugin/pluginCfs
// Plugin script functions for the CFS (Common File System / script) subsystem.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/plugin/pluginCfs.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

struct UnkClass_8009ECB0;

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
    extern float lbl_eu_80665E40;          // 4503601774854144.0f (for xoris)
    extern float lbl_eu_80665E48;          // various constants
    extern float lbl_eu_80665E4C;          // 0.0f
    extern float lbl_eu_80665E30;          // fixed-point divisor
    extern float lbl_eu_80665E34;          // 0.5f
    extern float lbl_80665E38__Q23mtl10MemManager;          // scale factor
    extern float lbl_eu_80665E3C;          // 1.0f
    extern float lbl_eu_80665E40;          // 4503601774854144.0f
    extern float lbl_eu_80665E48;          // misc constant
    extern float lbl_eu_80665E4C;          // 0.0f

    void func_8004302C(int, int);
    void func_80043628();
    void func_800A3304();
    void func_801579A4();
    void func_8012FAA8();
    int func_8015796C(int);
    void func_80157184(int);
    void func_801571A8(int);
    void func_8015730C(int, int, int);
    void* func_8015783C(int, int, int);
    void* func_80157948(int, int);
    void func_80159B40(int, int, void*);
    void func_80158118(void*, int, int);
    void func_80155A00(void*);
    void func_80158420(int, int*, int, int*);
    void func_8016DF34(int);
    void func_8016DF4C(int);
    void func_8018C8F4(void*, int);
    void func_80199678(void*, int);
    void func_801F4AD4(int, int);
    void func_801F4B68(int, int);
    void func_801F4BFC(int, int);
    void func_801F4C90(int, int, int);
    int func_80291BF8();
    void func_eu_8049AB50(int, int);
    int getUnk80664658();
    int isTvFormatPal__9CDeviceVIFv();
    void* CItem_initItemImplInstances();
    void* getInstance__Q22cf14CBattleManagerFv();
    void* getInstance__14Class_80296898Fv();
    void func_800B70FC(int, int);
    int func_800B8D5C();
    void func_800BE12C(int, int, int, int, int);
    void func_800BE28C(int, bool);
    bool func_8009CF8C(int);
    void func_8009D018(int, int);
    int* func_8009EC9C(int);
    int* func_8009ECB0();
    void func_8009E0C4(int*, int, int);
    void func_8009E0A8(int*, int);
    int func_8009E284(int*, int);
    int* func_8009D790(int*, int);
    void func_8009DBF4(int*, int, int);
    void func_800B6800(float, void*, int);
    void* func_800B07E8__Fv();
    void func_800B1AF4(void*);
    void func_8007C360__Q22cf13CfGameManagerFv(float, int, int);
    void func_8007F830__Q22cf13CfGameManagerFv(int, int);
    void func_800823A4__Q22cf13CfGameManagerFv(int, int);
    void func_80083EA4__Q22cf13CfGameManagerFv();
    bool func_80084B68__Q22cf13CfGameManagerFv();
    void func_8008566C__Q22cf13CfGameManagerFv(float*, int, int);
    int func_80086B1C__Q22cf13CfGameManagerFv();
    void* func_80081CB8__Q22cf13CfGameManagerFv();
    void func_800AB978(float, float*, float*);
    void func_800AB8CC(float, float*, float*);
    void func_800ABA18(float, float*, float*);
    void func_800AC30C(int, int, const char*);
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
    void func_8009E740(void*, int);
    int func_8009E56C(void*, int, int);
    bool func_8009E344(void*, int, int*, int*);
    void func_80083D50__Q22cf13CfGameManagerFv(u32, u32, u32, u32, float);
    void func_8007C374__Q22cf13CfGameManagerFv(u32, u32, float, u8);
    void func_8009E3C0();
    int  func_801586D4(int, int);
    void func_80159C04(int, int);
    int  func_80158068(int);
    void func_8007F1FC__Q22cf13CfGameManagerFv(void*, int);
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
        func_800AB978(v15f, t4, t3);
        func_800AB8CC(v15f, t1, t2);
        
        float sum[3];
        sum[0] = t1[0] + t2[0];
        sum[1] = t1[1] + t2[1];
        sum[2] = t1[2] + t2[2];
        
        float scaled[3];
        scaled[0] = sum[0] * lbl_80665E38__Q23mtl10MemManager;
        scaled[1] = sum[1] * lbl_80665E38__Q23mtl10MemManager;
        scaled[2] = sum[2] * lbl_80665E38__Q23mtl10MemManager;
        
        func_800AC30C(args[0], args[1], (const char*)v17);
        
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
        func_800AB978(lbl_eu_80665E48, tB, tB);
        func_800ABA18(0.0f, tA, tC);
        func_800AC30C(args[0], args[1], (const char*)v27);
        
        int neg = -v26;
        int orVal = neg | v26;
        int boolVal = (unsigned int)orVal >> 31;
        func_8007C360__Q22cf13CfGameManagerFv((float)(s32)v28, boolVal, v25);
    }
    
    func_8007C360__Q22cf13CfGameManagerFv(1.0f, v25, v16);
    return 0;
}

// --- setWarpArea (us-80048238) ---
int setWarpArea(VMThread* pThread) {
    return 0;
}

// --- setMapPreloadArea (us-800487dc) ---
int setMapPreloadArea(VMThread* pThread) {
    return 0;
}

// --- mapJump (us-80048a5c) ---
int mapJump(VMThread* pThread) {
    int r25 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int r26 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    int r27 = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    int r28 = vmArgFixedGet(5, vmArgPtrGet(pThread, 4));
    int r29 = vmArgFixedGet(6, vmArgPtrGet(pThread, 5));
    int r30 = vmArgIntGet(7, vmArgPtrGet(pThread, 6));

    int r31 = 8;
    const char* r24;
    if (vmArgOmitChk(pThread, 7) == 0) {
        r24 = vmArgStringGet(8, vmArgPtrGet(pThread, 7));
    } else {
        r24 = 0;
    }

    int r23;
    if (vmArgOmitChk(pThread, r31) == 0) {
        r23 = vmArgIntGet(r31 + 1, vmArgPtrGet(pThread, r31));
    } else {
        r23 = 0;
    }
    r31++;

    if (vmArgOmitChk(pThread, r31) == 0) {
        r31 = vmArgIntGet(r31 + 1, vmArgPtrGet(pThread, r31));
    } else {
        r31 = 0;
    }

    float pt[3] = {
        (float)(s32)r27 / 4096.0f,
        (float)(s32)r28 / 4096.0f,
        (float)(s32)r29 / 4096.0f,
    };

    func_80083D50__Q22cf13CfGameManagerFv((u32)(r25 & 0xFFFF), (u32)(r26 & 0xFFFF),
                                         (u32)(u8*)&pt[0], (u32)r24, (float)(s32)r30);
    func_8007C374__Q22cf13CfGameManagerFv((u32)r23, (u32)r31, 1.0f, 1);
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
    
    int firstOut = -1;
    int secondOut = 0;
    int* data = func_8009ECB0();
    func_8009E344((void*)data, val, &secondOut, &firstOut);
    
    int result = secondOut;
    int orVal = (1 - result) | (result - 1);
    
    VMArg ret;
    *(u8*)&ret.type = ((unsigned int)orVal >> 31) + 1;
    vmRetValSet(pThread, &ret);
    return 1;
}

// --- isResvParty (us-80049a28) ---
int isResvParty(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgIntGet(2, arg);
    
    int firstOut = -1;
    int secondOut = 0;
    int* data = func_8009ECB0();
    func_8009E344((void*)data, val, &secondOut, &firstOut);
    
    int result = secondOut;
    int orVal = (2 - result) | (result - 2);
    
    VMArg ret;
    *(u8*)&ret.type = ((unsigned int)orVal >> 31) + 1;
    vmRetValSet(pThread, &ret);
    return 1;
}

// --- addParty (us-80049ac0) ---
int addParty(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int v31 = vmArgIntGet(2, arg);
    
    int firstOut = 0;
    int secondOut = -1;
    int* data = func_8009ECB0();
    func_8009E344((void*)data, v31, &firstOut, &secondOut);
    
    int count = firstOut;
    int sub = count - 2;
    int clz = __cntlzw(sub);
    int isEq = (unsigned int)clz >> 5;
    
    if (isEq != 1 && isEq != 2) {
        UnkClass_8009ECB0* data2 = (UnkClass_8009ECB0*)func_8009ECB0();
        int group = 0;
        
        if (v31 == 9)       group = 6;
        else if (v31 == 10) group = 7;
        else if (v31 == 6)  group = 9;
        else if (v31 == 7)  group = 10;
        else if (v31 == 12 || v31 == 13) group = 4;
        
        if (group) func_8009E740(data2, group);
        
        if (!func_8009E56C(data2, v31, 1)) {
            func_8009E56C(data2, v31, 2);
        }
    }
    
    func_800823A4__Q22cf13CfGameManagerFv(v31, 1);
    return 0;
}

// --- makeParty (us-80049bf8) ---
int makeParty(VMThread* pThread) {
    int v29, v28, v27, v26, v25, v24, v23, v30;
    
    {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        v29 = vmArgIntGet(2, arg);
    }
    
    // Arg 2: optional
    if (vmArgOmitChk(pThread, 2)) { v28 = 0; v30 = 3; }
    else { v30 = 3; VMArg* arg = vmArgPtrGet(pThread, 2); v28 = vmArgIntGet(3, arg); }
    
    // Arg 3: optional
    if (vmArgOmitChk(pThread, v30)) { v27 = 0; v30++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; v27 = vmArgIntGet(v30, arg); }
    
    // Arg 4: optional
    if (vmArgOmitChk(pThread, v30)) { v26 = 0; v30++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; v26 = vmArgIntGet(v30, arg); }
    
    // Arg 5: optional
    if (vmArgOmitChk(pThread, v30)) { v25 = 0; v30++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; v25 = vmArgIntGet(v30, arg); }
    
    // Arg 6: optional
    if (vmArgOmitChk(pThread, v30)) { v24 = 0; v30++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; v24 = vmArgIntGet(v30, arg); }
    
    // Arg 7: optional
    if (vmArgOmitChk(pThread, v30)) { v23 = 0; v30++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; v23 = vmArgIntGet(v30, arg); }
    
    // Arg 8: optional bool
    int r30_bool;
    if (vmArgOmitChk(pThread, v30)) { r30_bool = 1; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; r30_bool = vmArgBoolGet(v30, arg); }
    
    // Build party data
    u16 pdata[9];
    memset(pdata, 0, sizeof(pdata));
    pdata[0] = v29 & 0xFFFF;
    pdata[1] = v28 & 0xFFFF;
    pdata[2] = v27 & 0xFFFF;
    pdata[3] = v26 & 0xFFFF;
    pdata[4] = v25 & 0xFFFF;
    pdata[5] = v24 & 0xFFFF;
    
    // Player positions
    float pos[3][3];
    float scale[3];
    for (int i = 0; i < 3; i++) {
        pos[i][0] = pos[i][1] = pos[i][2] = 0.0f;
        scale[i] = 0.0f;
        void* player = cf::CfGameManager::getPlayer(i);
        if (player) {
            void* ppos = (*(void*(*)(void*))(*(void**)player))(player);
            pos[i][0] = *(float*)((u8*)ppos + 0);
            pos[i][1] = *(float*)((u8*)ppos + 4);
            pos[i][2] = *(float*)((u8*)ppos + 8);
            scale[i] = (*(float(*)(void*))(*(void**)player))(player);
        }
    }
    
    func_8007F1FC__Q22cf13CfGameManagerFv(pdata, 1);
    
    if (r30_bool) {
        int* d = func_8009ECB0();
        func_80080888__Q22cf13CfGameManagerFv(d[1] & 0xFFFF, 1);
    }
    
    if (func_8009CF8C(0x20) == 1 && func_80087250__Q22cf13CfGameManagerFv()) {
        func_8012FAA8();
    }
    
    for (int i = 0; i < 3; i++) {
        void* player = cf::CfGameManager::getPlayer(i);
        if (player) {
            (*(void(*)(void*, float*))(*(void**)player))(player, pos[i]);
            (*(void(*)(void*, float))(*(void**)player))(player, scale[i]);
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
    func_8009E740(data, v31);
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
int equipWeapon(VMThread* vmThread) {
    int charId;
    int weaponId;
    int slotIdx = 1;

    // Get charId
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        charId = vmArgIntGet(slotIdx, arg);
        slotIdx++;
    }

    // Get weaponId
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        weaponId = vmArgIntGet(slotIdx, arg);
        slotIdx++;
    }

    if (func_8009CF8C(0x3508)) {
        return 0;
    }

    int* charData = func_8009EC9C(charId & 0xFFFF);
    int* weaponSlot = func_8009D790(&charData[7], 5);

    if (weaponSlot != NULL) {
        int slotType = (*weaponSlot) >> 20;
        if (weaponId == slotType) {
            return 0;
        }

        CItem_initItemImplInstances();

        int newWeaponId = weaponId & 0xFFFF;
        int slotInfo;
        func_80158420(newWeaponId, &slotInfo, 1, &slotInfo);
        func_8009E0A8(charData, slotInfo);

        func_800B70FC(charId, 0);
        lbl_eu_80663E28 |= 0x08000000;
    } else {
        int* cd = func_8009EC9C(charId & 0xFFFF);
        func_8009E0C4(cd, 5, weaponId & 0xFFFF);

        if (charId >= 1 && charId <= 11) {
            void* invSlot = func_8015783C(2, charId, 0);
            if (weaponId != 0) {
                func_80158118(invSlot, weaponId & 0xFFFF, 1);
                func_80155A00(invSlot);
            } else {
                CItem_initItemImplInstances();
            }

            for (int k = 0; k < 8; k++) {
                void* slotRef = func_80157948(charId, k);
                if (*(u8*)slotRef == 6) {
                    func_80159B40(charId, k, &weaponId);
                }
            }
        }
    }

    return 0;
}

// --- getWeaponSlot (us-8004a7c8) ---
int getWeaponSlot(VMThread* vmThread) {
    int charId;
    int itemId = 0;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        charId = vmArgIntGet(2, arg);
    }

    int* charData = func_8009EC9C(charId & 0xFFFF);
    int* slotData = func_8009D790(&charData[7], 5);

    if (slotData != NULL) {
        CItemImplInstance* inst = (CItemImplInstance*)CItem_initItemImplInstances();
        itemId = inst->getWeaponSlot(slotData) & 0xFFFF;
    } else if (charId >= 1 && charId <= 11) {
        void* invSlot = func_8015783C(2, charId, 0);
        int slotType = (*(int*)invSlot) >> 12 & 0xF;
        if (slotType == 2) {
            CItemImplInstance* inst = (CItemImplInstance*)CItem_initItemImplInstances();
            itemId = inst->getWeaponSlot(invSlot) & 0xFFFF;
        }
    }

    VMArg ret;
    ret.type = 3;
    ret.value.intVal = itemId;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- setWeaponSlot (us-8004a8d8) ---
int setWeaponSlot(VMThread* vmThread) {
    int charId;
    int itemId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        charId = vmArgIntGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        itemId = vmArgIntGet(3, arg);
    }

    int* charData = func_8009EC9C(charId & 0xFFFF);
    int* slotData = func_8009D790(&charData[7], 5);

    if (slotData != NULL) {
        CItem_initItemImplInstances();
    } else if (charId >= 1 && charId <= 11) {
        void* invSlot = func_8015783C(2, charId, 0);
        CItem_initItemImplInstances();
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
int setFade(VMThread* vmThread) {
    int fixedR, fixedG, fixedB, fixedA;
    int frames = 0;
    int slotIdx = 1;

    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        fixedR = vmArgFixedGet(slotIdx, arg);
        slotIdx++;
    }
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        fixedG = vmArgFixedGet(slotIdx, arg);
        slotIdx++;
    }
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        fixedB = vmArgFixedGet(slotIdx, arg);
        slotIdx++;
    }
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        fixedA = vmArgFixedGet(slotIdx, arg);
        slotIdx++;
    }

    if (vmArgOmitChk(vmThread, slotIdx)) {
        frames = 0;
    } else {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        frames = vmArgIntGet(slotIdx + 1, arg);
    }

    float fR = (float)(s32)fixedR / 4096.0f;
    float fG = (float)(s32)fixedG / 4096.0f;
    float fB = (float)(s32)fixedB / 4096.0f;
    float fA = (float)(s32)fixedA / 4096.0f;

    float color[4] = {fR, fG, fB, fA};
    func_8008566C__Q22cf13CfGameManagerFv(color, frames & 0xFFFF, 1);

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
int setDispOffArea(VMThread* vmThread) {
    void* ocObj;
    int areaId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        ocObj = vmArgOCGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        areaId = vmArgFixedGet(3, arg);
    }

    void* obj = func_801864DC(func_801862C0(), *(int*)((u8*)ocObj + 4));

    if (obj != NULL) {
        void* box = func_800B07E8__Fv();
        float fAreaId = (float)(s16)areaId;
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
int setWeather(VMThread* vmThread) {
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

    void* mapObj = func_80186BC8((void*)(mapObjId & 0xFFFF));
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