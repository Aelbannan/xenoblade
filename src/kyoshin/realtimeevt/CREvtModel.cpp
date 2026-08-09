// CREvtModel - Real-time event model base class
// High-level C/C++ reconstruction

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/realtimeevt/CREvtModel.hpp"
#include <nw4r/g3d/res/g3d_resfile.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include <nw4r/g3d/res/g3d_resdict.h>

// External declarations (C++ linkage for non-mangled symbols)
extern "C" {
extern void __ct__cf_CREvtObj(void* self, int arg);
extern void* __dt__Q22cf8CREvtObjFv(void* self, int dealloc);
extern void __dt__80185754(void* self);
extern void func_80185700(void* self);
extern void* func_80185748(u32 size);
extern u32 func_80495E60(void* ptr);
extern void* func_80495EAC(void* global, void* data, int param);
extern void func_8016AF4C(void* data, const char* name, int* outValue);
extern void func_8016B384(void* data, const char* name, void* outValue);
extern int  func_8016B788(void* data);
extern u32  func_80180954(void);
extern u32  func_80180960(void);
extern int func_8016A3A8(void);
extern void func_804C0254(void* ptr, int flag);
extern void* func_8048315C(void* model);
extern void func_804838DC(void* model, int flag);
extern void func_80482DF4(void* model, int flag);
extern void func_804839D4(void* model, u32 r4, void* r5, u32 r6, u32 r7, u32 r8, u32 r9);
extern void func_80484E04(void* model, int value);
extern void func_80482B3C(void* model, int flag);
extern void* GetResAnmChr__Q34nw4r3g3d7ResFileCFi(void* resFile, int index);
extern void* GetResUserData__Q34nw4r3g3d9ResAnmChrFv(void* self);
extern void* __vc__Q34nw4r3g3d6ResDicCFPCc(void* self, const char* key);
extern void Panic__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);
extern void* __dynamic_cast(void* obj, long offset, const void* srcType, const void* dstType, void* src2dst);
extern void __ct__CREvtModelMap(void* self, void* parent);
extern void __ct__CREvtModelObj(void* self, void* parent);
extern void __ct__CREvtModelPc(void* self, void* parent);
extern void func_8049E708(void* data, int index);
extern const void* lbl_eu_8053167C[];
extern const char lbl_eu_80503344[];
extern void* lbl_eu_80663E14;
extern const float lbl_eu_80667770;
extern const float lbl_eu_80667774;
extern const float lbl_eu_80667778;
extern const float lbl_eu_8066777C;
extern const void* lbl_eu_806623F8;
extern const void* lbl_eu_80662400;
extern const void* lbl_eu_80662418;
extern const void* lbl_eu_8066241C;
extern const char lbl_eu_80530D18[];
extern const char lbl_eu_80530CFC[];
extern const char lbl_eu_80530CF0[];
extern const char lbl_eu_80530F08[];
extern const char lbl_eu_80530EE0[];
extern const char lbl_eu_80530D54[];
extern const char lbl_eu_80530D2C[];
extern const char lbl_eu_80530DC4[];
extern const char lbl_eu_80530DA8[];
extern const char lbl_eu_80530D68[];
extern const char lbl_eu_80530ECC[];
extern const char lbl_eu_80530E88[];
}

#define FLD(type, base, off) (*(type*)((char*)(base) + (off)))

// All functions with C linkage
extern "C" {

void __ct__CREvtModel(void* self, void* pData, void* pArg) {
    __ct__cf_CREvtObj(self, 3);
    u32* base = (u32*)self;
    u32* vtable = (u32*)(void*)&lbl_eu_8053167C;
    u32 zero = 0;
    base[0] = (u32)vtable;
    base[0x14/4] = (u32)pArg;
    base[0x1C/4] = (u32)pData;
    base[0x20/4] = zero;
    base[0x24/4] = zero;
    base[0x28/4] = zero;
    base[0x2C/4] = 0;
    base[0x30/4] = 0;
    base[0x34/4] = 0;
    base[0x18/4] = zero | 0x8;
}

void* __ct__80172668(void* self, int deallocFlag) {
    if (self != 0) {
        FLD(const void**, self, 0) = lbl_eu_8053167C;
        ((void (*)(void*))lbl_eu_8053167C[0x3C / 4])(self);
        __dt__Q22cf8CREvtObjFv(self, 0);
        if (deallocFlag > 0) { __dt__80185754(self); }
    }
    return self;
}

void func_80172768(void* self) {
    (*(void (**)(void*))((char*)*(void**)self + 0x10))(self);
    void* p24 = FLD(void*, self, 0x24);
    if (p24 != 0) { func_80495E60(p24); FLD(void*, self, 0x24) = 0; }
    void* p20 = FLD(void*, self, 0x20);
    if (p20 != 0) { func_80495E60(p20); FLD(void*, self, 0x20) = 0; }
}

int func_801726DC(void* self) {
    u32 flags = FLD(u32, self, 0x18);
    if (flags & 0x80) { return 1; }
    (*(void (**)(void*))((char*)*(void**)self + 0x10))(self);
    void* p24 = FLD(void*, self, 0x24);
    if (p24 != 0) { func_80495E60(p24); FLD(void*, self, 0x24) = 0; }
    void* p20 = FLD(void*, self, 0x20);
    if (p20 != 0) { func_80495E60(p20); FLD(void*, self, 0x20) = 0; }
    FLD(u32, self, 0x18) |= 0x80;
    return 1;
}

void* func_801727D0(void* self) {
    return (char*)FLD(void*, self, 0x1C) + 0x10;
}

void func_801727DC(void* self) {
    func_80185700(self);
    s32 counter = FLD(s32, self, 0x34);
    if (counter < 4) {
        u16 val = *(u16*)((char*)FLD(void*, self, 0x1C) + counter * 2 + 0x6C);
        if (val != 0) {
            int result = func_8016A3A8();
            if (result == (int)val) {
                u32 flags = FLD(u32, self, 0x18);
                if (flags & 0x400) { flags &= ~0x400; }
                else { flags |= 0x400; }
                FLD(u32, self, 0x18) = flags;
                FLD(s32, self, 0x34) = counter + 1;
            }
        }
    }
    void* model = FLD(void*, self, 0x20);
    if (model != 0) {
        u32 flags = FLD(u32, self, 0x18);
        int bit = (flags >> 21) & 1;
        func_804C0254((char*)model + 0x31C, bit);
        u32 check = func_80180960();
        s32 field2C = FLD(s32, self, 0x2C);
        void** vtable = *(void***)self;
        if (check == 0) {
            ((void (*)(void*, int))vtable[0x30 / 4])(self, (field2C - 1) == 0 ? 1 : 0);
        } else {
            ((void (*)(void*, int))vtable[0x30 / 4])(self, (field2C != 2) ? 1 : 0);
        }
    }
}

void func_801728F8(void* self) {
    void* model = FLD(void*, self, 0x20);
    if (model != 0) {
        float zero = 0.0f;
        float pos[3] = { zero, zero, zero };
        void* p = func_8048315C(model);
        if (p != 0) {
            FLD(u32, p, 0) = FLD(u32, pos, 0);
            FLD(u32, p, 4) = FLD(u32, pos, 4);
            FLD(u32, p, 8) = FLD(u32, pos, 8);
        }
        float rot[3] = { zero, zero, zero };
        void* r = func_8048315C(model);
        if (r != 0) {
            FLD(u32, r, 0xC) = FLD(u32, rot, 0);
            FLD(u32, r, 0x10) = FLD(u32, rot, 4);
            FLD(u32, r, 0x14) = FLD(u32, rot, 8);
        }
    }
}

void func_8017298C(void* self) {
    void* p = FLD(void*, self, 0x28);
    if (p != 0) { func_80495E60(p); FLD(void*, self, 0x28) = 0; }
}

int func_801729D0(void* self) {
    void* p = FLD(void*, self, 0x1C);
    if (p == 0) { return 0; }
    return (FLD(u32, p, 0x58) >> 4) & 1;
}

void func_801729F0(void* self, void* pData, void* pModelData) {
    u32 flags = FLD(u32, pModelData, 4);
    void** vtable = *(void***)self;
    ((void (*)(void*, int))vtable[0x38 / 4])(self, flags & 1);
    void* model = FLD(void*, self, 0x20);
    if (model == 0) return;
    func_804838DC(model, FLD(u32, pModelData, 4) & 1);
    void* p24 = FLD(void*, self, 0x24);
    if (p24 != 0) {
        ((void (*)(void*))((char*)*(void**)self + 0x10))(self);
        if (func_80180954() == 0) { func_80172EA4(self, p24, model, 1); }
        FLD(void*, self, 0x28) = p24;
        FLD(void*, self, 0x24) = 0;
    }
    if (pData == 0) {
        void* p1C = FLD(void*, self, 0x1C);
        if (!(FLD(u32, p1C, 0x58) & 0x8)) {
            void* m = FLD(void*, self, 0x20);
            ((void (*)(void*, int))(*(void***)m)[0x84 / 4])(m, 1);
        }
        return;
    }
    void* newData = func_80495EAC(&lbl_eu_80663E14, pData, 0);
    FLD(void*, self, 0x24) = newData;
    func_80482DF4(model, func_80180954() == 0 ? 1 : 0);
    func_804839D4(model, (u32)newData, 0, 0, 0, 1, -1);
    func_80172CE4(self, model);
    ((void (*)(void*, int))(*(void***)model)[0x84 / 4])(model, 0);
    func_80484E04(model, 1);
    FLD(u32, model, 0x7A4) |= 0x20000000;
    FLD(s32, self, 0x2C) = 0;
    FLD(s32, self, 0x30) = 0;
    { int tmp = 0; func_8016AF4C(newData, &lbl_eu_80503344[0], &tmp); FLD(s32, self, 0x2C) = tmp; }
    { int tmp = 0; func_8016AF4C(newData, &lbl_eu_80503344[0x0B], &tmp); FLD(s32, self, 0x30) = tmp; }
    u32 check = func_80180960();
    void** vt = *(void***)self;
    if (check == 0) {
        ((void (*)(void*, int))vt[0x30 / 4])(self, (FLD(s32, self, 0x2C) - 1) == 0 ? 1 : 0);
    } else {
        ((void (*)(void*, int))vt[0x30 / 4])(self, (FLD(s32, self, 0x2C) != 2) ? 1 : 0);
    }
    int sv = 0; func_8016AF4C(newData, &lbl_eu_80503344[0x13], &sv);
    if (sv >= 1) { func_80482B3C(model, 1); }
    int sv2 = 1; func_8016AF4C(newData, &lbl_eu_80503344[0x1E], &sv2);
    if (sv2 != 0) { FLD(u32, model, 0x7A0) |= 0x8; }
    else { FLD(u32, model, 0x7A0) &= ~0x8; }
    if (func_80180954() == 0) { func_80172EA4(self, newData, model, 0); }
    int result = func_8016B788(newData);
    ((void (*)(void*, int))(*(void***)model)[0x84 / 4])(model, result);
}

void func_80172CC0(void) {}

void func_80172CC4(void* self) {
    void* model = FLD(void*, self, 0x20);
    if (model != 0) { ((void (*)(void*))(*(void***)model)[0xB4 / 4])(model); }
}

void func_80172CE4(void* self, void* model) {
    const char* strBase = lbl_eu_80503344;
    int stackVal = 0;
    float stackF1 = 1.0f;
    float stackF0 = 0.0f;
    func_8016AF4C(FLD(void*, self, 0x24), &strBase[0x24], &stackVal);
    if (stackVal > 0) {
        stackVal--;
        func_8016B384(FLD(void*, self, 0x24), &strBase[0x2C], &stackF1);
        func_8016B384(FLD(void*, self, 0x24), &strBase[0x36], &stackF0);
    } else { stackVal = 1; }
    ((void (*)(void*, int))(*(void***)model)[0x50 / 4])(model, stackVal == 0 ? 0 : 1);
    void* shadow = __dynamic_cast(FLD(void*, model, 0x7EC), 0, (void*)&lbl_eu_806623F8, (void*)&lbl_eu_80662400, 0);
    if (shadow != 0) {
        if (stackVal == 1) { FLD(u8, shadow, 0x2A) = 1; }
        else if (stackVal == 2) { FLD(u8, shadow, 0x2A) = 0; }
        float f255 = lbl_eu_8066777C;
        int a = (int)(f255 * stackF0);
        FLD(u8, shadow, 4) = 0; FLD(u8, shadow, 5) = 0;
        FLD(u8, shadow, 6) = 0; FLD(u8, shadow, 7) = (u8)a;
        FLD(f32, shadow, 8) = stackF1;
    }
}

void func_80172EA4(void* self, void* animData, void* model, int flag) {
    nw4r::g3d::ResFile resFile((void*)((char*)animData + 0xC));
    int numAnmChr = resFile.GetResAnmChrNumEntries();
    for (int i = 0; i < numAnmChr; i++) {
        void* anmChrData = GetResAnmChr__Q34nw4r3g3d7ResFileCFi((char*)animData + 0xC, i);
        nw4r::g3d::ResAnmChr anmChr(anmChrData);
        void* userData = GetResUserData__Q34nw4r3g3d9ResAnmChrFv(&anmChr);
        if (userData == 0) continue;
        void* dic = (char*)userData + 4;
        if ((u32)dic & 3) { Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0); }
        void* result = __vc__Q34nw4r3g3d6ResDicCFPCc(dic, &lbl_eu_80503344[0x3D]);
        if ((u32)result & 3) { Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C); }
        if (result == 0) continue;
        if (FLD(u32, result, 0xC) != 2) { Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530ECC, 0x44, lbl_eu_80530E88); }
        void* dataPtr = 0;
        if (FLD(u32, result, 4) != 0) { dataPtr = (char*)result + FLD(u32, result, 4); }
        if (dataPtr != 0) {
            ((void (*)(void*, void*, int))(*(void***)model)[0x28 / 4])(model, dataPtr, flag);
        }
    }
    if (flag == 0) {
        void* p1C = FLD(void*, self, 0x1C);
        if (FLD(u32, p1C, 0x58) & 0x100) {
            ((void (*)(void*, const char*, int))(*(void***)model)[0x28 / 4])(model, &lbl_eu_80503344[0x44], 0);
        }
    }
}

void* func_801730D0(void* self) {
    u32 type = FLD(u32, self, 0x28);
    if (type == 1) {
        void* obj = func_80185748(0x1F0);
        if (obj != 0) { __ct__CREvtModelMap(obj, self); }
        return obj;
    } else if (type == 2) {
        u8 b16 = FLD(u8, self, 0x16);
        u8 b17 = FLD(u8, self, 0x17);
        if (b16 != 0x30 || b17 != 0x30) {
            void* obj = func_80185748(0xB4);
            if (obj != 0) { __ct__CREvtModelPc(obj, self); }
            return obj;
        }
    }
    void* obj = func_80185748(0x88);
    if (obj != 0) { __ct__CREvtModelObj(obj, self); }
    return obj;
}

int func_80173194(void* self) {
    return FLD(u32, self, 0x18) & 1;
}

u32 func_801731A0(void* self) {
    return (FLD(u32, self, 0x18) >> 4) & 1;
}

int func_801731AC(void* self) {
    (void)self; return 0;
}

} // extern "C"