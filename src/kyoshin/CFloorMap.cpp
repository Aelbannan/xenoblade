// Auto-scaffolded catalog TU for kyoshin/CFloorMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CFloorMap.hpp"
#include <cstdio>

extern u32 lbl_eu_8066479C;
extern u32 func_8003B1EC(u32);
extern u32 lbl_eu_8050BDF8[];
extern u8 lbl_eu_80664798;

extern int CSysWin_getUnk34(void*);
extern void func_80246200(void*);
extern u32 func_80248558(void*);
extern void func_8024577C(void*, u16);
extern void func_801F3850(void*, u16);
extern void func_801375A0(float*, void*);
extern void func_80137C1C(void*, void*);
extern void* func_80136190(const char*, const char*, const char*);
extern void* func_801355F4();
extern void* createPicture__10CLibLayoutFv();
extern void SetName__Q34nw4r3lyt4PaneFPCc(void*, const char*);
extern void* func_80137E7C(void*, const char*, const char*);
extern u32 func_8009CF8C(u32);
extern void func_80138078(u32);

// Draw helpers used by CFloorMap::Draw and related functions
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void func_801F35B0(void* scrollBar, void* drawInfo);
void func_8022B7C8(void* sysWin, void* drawInfo);
void func_801D20B0(void* cursor, void* drawInfo);

extern float lbl_eu_80668764;
extern float lbl_eu_80668794;
extern float lbl_eu_80668798;
extern float lbl_eu_806687A4;
extern float lbl_eu_806687A8;
extern float lbl_eu_806687AC;
extern float lbl_eu_806687B0;
extern float lbl_eu_806687B4;
extern float lbl_eu_806687B8;

typedef void* (*VFuncPtr)(void*, const char*, u32);

u8 func_8024CE60(void* self) { return static_cast<CFloorMapFull*>(self)->field_40; }


void func_80245450(void* self) {
    extern void func_80246200(void*);
    extern void func_80138078(u32);
    u8* p = (u8*)self;
    s8 idx0 = (s8)p[0x09];
    u32 base = idx0 * 0x30C;
    u8 count = *(u8*)(p + base + 0x318);
    s8 idxA = (s8)p[0x0A];
    s8 idxB = (s8)p[0x0B];
    if (count >= 5) {
        idxA++;
        if (idxA >= 5) {
            idxA = 4;
            idxB++;
            if (idxB > (s8)(count - 5)) {
                idxA = 0;
                idxB = 0;
            }
        }
    } else {
        idxA++;
        if (idxA >= (s8)count) {
            idxA = 0;
            idxB = 0;
        }
    }
    p[0x0A] = idxA;
    p[0x0B] = idxB;
    func_80246200(self);
}

void func_802455F0(void* self) {
    extern void func_80246200(void*);
    u8* p = (u8*)self;
    s8 idx0 = (s8)p[0x09];
    u32 base = idx0 * 0x30C;
    u8 count = *(u8*)(p + base + 0x318);
    if (count >= 5) {
        p[0x0B] += 5;
        if ((s8)p[0x0B] > (s8)count) {
            p[0x0A] = p[0x0B] - count;
            p[0x0B] = count;
            if ((s8)p[0x0A] >= 5) {
                p[0x0A] = 0;
            }
        }
    } else {
        p[0x0A] = count - 1;
        p[0x0B] = 0;
        if ((s8)p[0x0A] < 0) p[0x0A] = 0;
    }
    func_80246200(self);
}

void func_8024577C(void* self, u16 val) {
    u8* p = (u8*)self;
    if (!val) {
        p[0x0A] = -1;
        p[0x0B] = 0;
        return;
    }
    s8 idx0 = (s8)p[0x09];
    s8 idxA = (s8)p[0x0A];
    s8 idxB = (s8)p[0x0B];
    u32 base = idx0 * 0x30C;
    u8 count = *(u8*)(p + base + 0x318);
    for (u8 i = 0; i < count; i++) {
        if (*(u16*)(p + base + i * 0x18 + 0x18) == val) {
            if (i >= 5) {
                p[0x0A] = 4;
                p[0x0B] = i - 4;
            } else {
                p[0x0A] = i;
                p[0x0B] = 0;
            }
            func_80246200(self);
            if (idxA != (s8)p[0x0A] || idxB != (s8)p[0x0B]) {
                func_80138078(1);
            }
            return;
        }
    }
}

void func_80245950(){}

void func_80245DF8(){}

void func_80246200(void* self){}

void* __dt__802462F0(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void func_80246330(){}

void* __dt__802468C8(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void func_80246908(){}

void func_80247490(){}

void func_8024808C(void* self, void* arg2) {
    extern void func_8003AA34();
    extern void* getFP__FPCc(const char*);
    extern u32 func_801361E8(const char*, const char*, u32);
    extern u32 func_8009CF8C(u32);
    extern void func_80141DC4(f32*);
    extern s16 func_80136330(u32, const char*, u32);
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern u32 func_8003B1EC(u32);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_80668778;
    extern f64 lbl_eu_80668788;
    u8* p = (u8*)self;
    func_8003AA34();
    u32 fp = (u32)getFP__FPCc(&lbl_eu_8050BEA8[0x17F]);
    u8 map = func_801361E8((const char*)fp, &lbl_eu_8050BEA8[0x18C], *(u32*)((u8*)arg2 + 0x10));
    if (map != lbl_eu_80664798) return;
    if (!func_8009CF8C(0x20C8)) return;
    f32 buf[3];
    func_80141DC4(buf);
    u8 count = func_8003B1EC(fp);
    u8 r26 = 0;
    for (u8 i = 1; i <= count; i++) {
        s16 val = func_80136330(fp, &lbl_eu_8050BEA8[0x15A], i);
        if ((f32)(s16)val > buf[1]) {
            if (i == p[0x0C]) { r26 = 1; break; }
        }
    }
    if (r26) {
        p[0x0A]++;
    }
}

void func_8024830C(void* self, void* arg2) {
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern s16 func_80136330(u32, const char*, u32);
    extern u32 func_8009CF8C(u32);
    extern void func_80141DC4(f32*);
    extern u32 func_801361E8(const char*, const char*, u32);
    extern u32 lbl_eu_80664184;
    extern u32 lbl_eu_806640A8;
    extern u16 lbl_eu_8050B798;
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_80668778;
    extern f32 lbl_eu_8066877C;
    extern f64 lbl_eu_80668788;
    u8* p = (u8*)self;
    f32* result = (f32*)p;
    result[0] = result[1] = result[2] = lbl_eu_80668764;
    void* slot = *(void**)((u8*)arg2 + 0x00);
    if (!slot) return;
    if ((lbl_eu_80664184 & 0xFF) == lbl_eu_80664798) {
        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
        if (!player) return;
        void** vt = *(void***)player;
        f32* pos = (f32*)((void*(*)(void*))vt[0xAC])(player);
        result[0] = pos[0]; result[1] = pos[1]; result[2] = pos[2];
    } else {
        u16 idx = *(u16*)lbl_eu_8050B798;
        if (!idx) return;
        f32 buf[3];
        func_80141DC4(buf);
        result[0] = buf[0]; result[1] = buf[1]; result[2] = buf[2];
    }
    s16 val1 = func_80136330(*(u32*)lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
    s16 val2 = func_80136330(*(u32*)lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);
    u8 region = func_801361E8(&lbl_eu_8050BEA8[0x1FE], &lbl_eu_8050BEA8[0x1F], lbl_eu_80664798);
    result[0] += (f32)(s16)val1 / ((f32)(s32)region * lbl_eu_80668778);
    result[1] -= (f32)(s16)val2 / ((f32)(s32)region * lbl_eu_80668778);
    result[2] = lbl_eu_80668764;
    if (*(void**)(p + 0x3108)) {
        void* obj = *(void**)((u8*)*(void**)(p + 0x3108) + 0x10);
        *(f32*)((u8*)obj + 0x2C) = result[0];
        *(f32*)((u8*)obj + 0x30) = result[1];
        *(f32*)((u8*)obj + 0x34) = result[2];
    }
    func_801F3850(*(void**)(p + 0x3134), (u16)(s16)p[0x0B]);
}

u32 func_80248558(void* self) {
    extern void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, const char*);
    u8* p = (u8*)self;
    void* data = *(void**)(p + 0x08);
    if (!data) return 0;
    void* obj = *(void**)((u8*)data + 0x10);
    if (!obj) Panic__Q24nw4r2dbFPCciPCce(&lbl_eu_8050BEA8[0x26C], 0x23D, "", "");
    void** vtable = *(void***)obj;
    void* result = ((void*(*)(void*, const char*, u32))vtable[15])(obj, &lbl_eu_8050BEA8[0x26C], 1);
    if (!result) return 0;
    void* target = *(void**)((u8*)result + 0x14);
    void* current = result;
    while (current) {
        void* cur_target = *(void**)((u8*)current + 0x10);
        if (cur_target == target) break;
        void* next = *(void**)((u8*)current + 0x0C);
        if (!next) break;
        current = next;
    }
    return current ? *(u32*)((u8*)current + 0x14) : 0;
}

void* func_80248920(void* self, const char* name, float x, float y, void* arg5, const char* paneName) {
    if (!name) return NULL;
    if (!paneName) return NULL;

    void* result = (char*)func_80136190(&lbl_eu_8050BEA8[0x2f6], &lbl_eu_8050BEA8[0x303], name);

    char buf[48];
    sprintf(buf, &lbl_eu_8050BEA8[0x30e], result);

    void* accessor = func_801355F4();
    typedef void* (*VFuncPtr4)(void*, u32, void*, u32);
    VFuncPtr4* vt = *(VFuncPtr4**)accessor;
    void* picture = vt[3](accessor, 0x74696d67, buf, 0);

    if (!picture) return NULL;

    void* pic = createPicture__10CLibLayoutFv();
    SetName__Q34nw4r3lyt4PaneFPCc(pic, paneName);

    *(float*)((u8*)pic + 0x2C) = x;
    *(float*)((u8*)pic + 0x30) = y;
    *(float*)((u8*)pic + 0x34) = lbl_eu_80668764;

    func_80137C1C(pic, arg5);

    u8* byte = (u8*)pic + 0xBB;
    *byte = (*byte & 0x7F) | 0x01;

    *(float*)((u8*)pic + 0x44) = lbl_eu_80668794;
    *(float*)((u8*)pic + 0x48) = lbl_eu_80668794;

    return pic;
}

void func_80248A6C(){}

void func_80248ED8(){}

void func_80249344(){}

void func_802497B0(){}

void func_80249C1C(){}

void func_8024A448(){}

void func_8024A748(){}

void func_8024AEEC(){}

void func_8024B234(){}

void func_8024B4CC(void* result, void* data, void* node) {
    extern f32 lbl_eu_80668764;
    f32* r = (f32*)result;
    r[0] = r[1] = r[2] = lbl_eu_80668764;
    void* target = *(void**)((u8*)data + 0x10);
    if (target == node || !node) return;
    r[0] = *(f32*)((u8*)node + 0x2C);
    r[1] = *(f32*)((u8*)node + 0x30);
    r[2] = *(f32*)((u8*)node + 0x34);
    void* next = *(void**)((u8*)node + 0x0C);
    f32 temp[3] = {lbl_eu_80668764, lbl_eu_80668764, lbl_eu_80668764};
    if (target != next && next) {
        temp[0] = *(f32*)((u8*)next + 0x2C);
        temp[1] = *(f32*)((u8*)next + 0x30);
        temp[2] = *(f32*)((u8*)next + 0x34);
    }
    void* next2 = next ? *(void**)((u8*)next + 0x0C) : 0;
    if (target != next2 && next2) {
        f32 local[3] = {lbl_eu_80668764, lbl_eu_80668764, lbl_eu_80668764};
        local[0] = *(f32*)((u8*)next2 + 0x2C);
        local[1] = *(f32*)((u8*)next2 + 0x30);
        local[2] = *(f32*)((u8*)next2 + 0x34);
        void* next3 = *(void**)((u8*)next2 + 0x0C);
        if (target != next3 && next3) {
            f32 rec[3];
            func_8024B4CC(rec, data, next3);
            local[0] += rec[0]; local[1] += rec[1]; local[2] += rec[2];
        }
        temp[0] += local[0]; temp[1] += local[1]; temp[2] += local[2];
    }
    r[0] += temp[0]; r[1] += temp[1]; r[2] += temp[2];
}

void* __dt__8024B6B8(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void func_8024B6F8(void* self, void* arg2, u32 arg3, u32 arg4) {
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern u32 func_8009CF8C(u32);
    extern s16 func_80136330(u32, const char*, u32);
    extern s16 func_80137E7C(void*, void*, const char*, ...);
    u8* p = (u8*)self;
    if (!arg2 || !*(void**)p) return;
    void* player = getPlayer__Q22cf13CfGameManagerFi(0);
    if (!player) return;
    void* data = *(void**)p;
    void* obj = *(void**)((u8*)data + 0x10);
    void** vtable = *(void***)obj;
    void* result = ((void*(*)(void*, void*, const char*, ...))vtable[1])(data, arg2, &lbl_eu_8050BEA8[0x47F]);
    if (!result) return;
    for (u32 i = 1; i <= arg3; i++) {
        s16 val = func_80136330(*(u32*)lbl_eu_8066479C, &lbl_eu_8050BEA8[0x487], i);
        if (val) {
            u8* pBB = (u8*)result + 0xBB;
            u8 bit = (i == arg4) ? 1 : 0;
            *pBB = (*pBB & 0x7F) | bit;
        }
    }
    if (arg4 == 0xC) {
        if (result) {
            u32 val = func_8009CF8C(0x20);
            u8* pBB = (u8*)result + 0xBB;
            *pBB = (*pBB & 0x7F) | ((__cntlzw(val ^ 0x166) >> 5) & 1);
        }
    } else if (arg4 == 5) {
        if (result) {
            u32 val = func_8009CF8C(0x20);
            u8* pBB = (u8*)result + 0xBB;
            u8 bit = ((val - 0x171) | (val ^ 0x171)) >> 31;
            *pBB = (*pBB & 0x7F) | (bit & 1);
        }
    }
}

void* __dt__8024B894(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void __ct__CFloorMap(){}

CFloorMap::~CFloorMap() {}

void func_8024BE1C(){}


void func_8024C1FC(){}

void func_8024C8F8(void* self, void* drawInfo) {
    CFloorMapFull* p = (CFloorMapFull*)self;
    if (!p->field_40) return;
    if (!p->field_41) return;
    if (p->mLayout130)
        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout130), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    if (p->mLayout138)
        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout138), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    if (p->mLayout140)
        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout140), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    if (*(void**)((u8*)p + 0x150)) {
        u8 i = 0;
        do {
            void* s = *(void**)((u8*)p + 0x150 + i * 8);
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(s), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
            i++;
        } while (i < p->field_1F0);
    }
    if (p->field_208) {
        if (p->mLayout1FC)
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout1FC), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
        if (p->mLayout32D4 && p->field_32E5)
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout32D4), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
        if (p->field_204 && p->mLayout32EC && p->field_32FD)
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout32EC), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
        if ((s8)p->field_206 >= 0) {
            u8 cond;
            if (p->mLayout32D4) {
                void** vtable = *(void***)(*(void**)((u8*)p->mLayout32D4 + 0x10));
                void* result = ((void*(*)(void*, const char*, u32))vtable[15])((void*)p->mLayout32D4, &lbl_eu_8050BEA8[0xEE], 1);
                cond = (*(u8*)((u8*)result + 0xBB)) & 1;
            } else {
                cond = 0;
            }
            if (!cond) {
                s8 idx = (s8)p->field_205;
                if (*(u8*)((u8*)p + idx * 0x30C + 0x514)) {
                    if (p->mLayout3304 && p->field_3315)
                        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout3304), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
                }
            }
        }
        if (p->mLayout331C && p->field_332D)
            func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout331C), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    }
    if (p->field_208)
        func_801F35B0((void*)p->mScrollBar, drawInfo);
    if (p->field_333C && p->mLayout3334)
        func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(p->mLayout3334), reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    func_8022B7C8((void*)p->mSysWinB8, drawInfo);
    func_8022B7C8((void*)p->mSysWinF4, drawInfo);
    if ((s8)p->field_5C >= 0)
        func_801D20B0((void*)p->mCursor, drawInfo);
}

void func_8024CB94(){}

u8 func_8024CE1C(void* self) {
    extern int CScrollBar_isVisible(void*);
    if (CScrollBar_isVisible((u8*)self + 0x60)) {
        return *(u8*)((u8*)self + 0x42);
    }
    return 0;
}


void func_8024CE68(){}

void func_8024D23C(){}

void func_8024D614(){}

void func_8024DA0C(){}

void func_8024DE08(){}

void func_8024E2BC(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern void func_802455F0(void*);
    extern int sprintf(char*, const char*, ...);
    extern void func_8024B4CC(void*, void*, void*);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_806687BC;
    u8* p = (u8*)self;
    if (*(u32*)(p + 0x2C)) return;
    if (!p[0x208]) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    if (p[0x58]) return;
    s8 idx0 = (s8)p[0x205];
    u32 base = idx0 * 0x30C;
    if (!*(u8*)(p + base + 0x514)) return;
    func_802455F0(p + 0x1FC);
    s8 idx1 = (s8)p[0x206];
    u16 val = 0;
    if (idx1 >= 0) {
        s8 idx2 = (s8)p[0x207];
        val = *(u16*)(p + base + (idx2 + idx1) * 0x18 + 0x214);
    }
    if (!val) return;
    f32 pos[3] = {lbl_eu_80668764, lbl_eu_80668764, lbl_eu_80668764};
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB]);
    void* data = *(void**)(p + 0x140);
    void* obj = *(void**)((u8*)data + 0x10);
    VFuncPtr* vtable = *(VFuncPtr**)obj;
    void* result = vtable[15](obj, buf, 1);
    if (result) {
        void* target = *(void**)((u8*)data + 0x10);
        void* node = result;
        if (node && *(void**)((u8*)node + 0x10) != target) {
            void* next = *(void**)((u8*)node + 0x0C);
            if (next && *(void**)((u8*)next + 0x10) != target) {
                void* next2 = *(void**)((u8*)next + 0x0C);
                if (next2 && *(void**)((u8*)next2 + 0x10) != target) {
                    f32 p1[3];
                    func_8024B4CC(p1, data, next2);
                    pos[0] += p1[0]; pos[1] += p1[1]; pos[2] += p1[2];
                }
                pos[0] += *(f32*)((u8*)next2 + 0x2C);
                pos[1] += *(f32*)((u8*)next2 + 0x30);
                pos[2] += *(f32*)((u8*)next2 + 0x34);
            }
            pos[0] += *(f32*)((u8*)next + 0x2C);
            pos[1] += *(f32*)((u8*)next + 0x30);
            pos[2] += *(f32*)((u8*)next + 0x34);
        }
        void* result2 = vtable[15](obj, &lbl_eu_8050BEA8[0x136], 1);
        if (result2) {
            f32 scale = *(f32*)((u8*)result2 + 0x44);
            pos[0] *= scale;
            pos[0] += *(f32*)((u8*)data + 0x2C);
            pos[1] += *(f32*)((u8*)data + 0x30);
            pos[2] += *(f32*)((u8*)data + 0x34);
        }
    }
    *(f32*)(p + 0x54) = lbl_eu_806687BC;
    p[0x41] = 2;
    *(f32*)(p + 0x4C) = pos[0] / lbl_eu_806687BC;
    *(f32*)(p + 0x50) = pos[1] / lbl_eu_806687BC;
}

void func_8024E650(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern u32 func_80248558(void*);
    extern void func_8024577C(void*, u16);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_806687A4;
    extern f32 lbl_eu_806687A8;
    u8* p = (u8*)self;
    if (p[0x58]) goto done;
    if (CSysWin_getUnk34(p + 0xB8)) goto done;
    if (CSysWin_getUnk34(p + 0xF4)) goto done;
    f32* pos = (f32*)(p + 0x44);
    pos[1] -= lbl_eu_806687A8;
    if (pos[1] < lbl_eu_806687A4) pos[1] = lbl_eu_806687A4;
    for (int i = 0; i < 3; i++) {
        void* slot = *(void**)(p + 0x130 + i * 8);
        if (slot) {
            void* obj = *(void**)((u8*)slot + 0x10);
            *(f32*)((u8*)obj + 0x2C) = pos[0];
            *(f32*)((u8*)obj + 0x30) = pos[1];
            *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
        }
    }
    void* slot = *(void**)(p + 0x150);
    if (slot) {
        void* obj = *(void**)((u8*)slot + 0x10);
        *(f32*)((u8*)obj + 0x2C) = pos[0];
        *(f32*)((u8*)obj + 0x30) = pos[1];
        *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
    }
    for (u8 i = 0; i < p[0x1F0]; i++) {
        void* s = *(void**)(p + 0x150 + i * 8);
        if (s) {
            void* obj = *(void**)((u8*)s + 0x10);
            *(f32*)((u8*)obj + 0x2C) = pos[0];
            *(f32*)((u8*)obj + 0x30) = pos[1];
            *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
        }
    }
    func_80248558(p + 0x140);
    func_8024577C(p + 0x1FC, *(u16*)(p + 0x5A));
    p[0x5D] = 1;
done:;
}

void func_8024E828(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern u32 func_80248558(void*);
    extern void func_8024577C(void*, u16);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_806687A8;
    extern f32 lbl_eu_806687AC;
    u8* p = (u8*)self;
    if (p[0x58]) goto done;
    if (CSysWin_getUnk34(p + 0xB8)) goto done;
    if (CSysWin_getUnk34(p + 0xF4)) goto done;
    f32* pos = (f32*)(p + 0x44);
    pos[1] += lbl_eu_806687A8;
    if (pos[1] > lbl_eu_806687AC) pos[1] = lbl_eu_806687AC;
    for (int i = 0; i < 3; i++) {
        void* slot = *(void**)(p + 0x130 + i * 8);
        if (slot) {
            void* obj = *(void**)((u8*)slot + 0x10);
            *(f32*)((u8*)obj + 0x2C) = pos[0];
            *(f32*)((u8*)obj + 0x30) = pos[1];
            *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
        }
    }
    void* slot = *(void**)(p + 0x150);
    if (slot) {
        void* obj = *(void**)((u8*)slot + 0x10);
        *(f32*)((u8*)obj + 0x2C) = pos[0];
        *(f32*)((u8*)obj + 0x30) = pos[1];
        *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
    }
    for (u8 i = 0; i < p[0x1F0]; i++) {
        void* s = *(void**)(p + 0x150 + i * 8);
        if (s) {
            void* obj = *(void**)((u8*)s + 0x10);
            *(f32*)((u8*)obj + 0x2C) = pos[0];
            *(f32*)((u8*)obj + 0x30) = pos[1];
            *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
        }
    }
    func_80248558(p + 0x140);
    func_8024577C(p + 0x1FC, *(u16*)(p + 0x5A));
    p[0x5D] = 1;
done:;
}

void func_8024EA00(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern u32 func_80248558(void*);
    extern void func_8024577C(void*, u16);
    extern f32 lbl_eu_80668798;
    extern f32 lbl_eu_806687A8;
    extern f32 lbl_eu_806687B0;
    extern f32 lbl_eu_806687B4;
    extern f32 lbl_eu_80668764;
    u8* p = (u8*)self;
    if (p[0x58] || CSysWin_getUnk34(p + 0xB8) || CSysWin_getUnk34(p + 0xF4)) return;
    void* slot = *(void**)(p + 0x130);
    if (!slot) slot = 0;
    void* obj = *(void**)((u8*)slot + 0x10);
    VFuncPtr* vt = *(VFuncPtr**)obj;
    void* result = vt[15](obj, &lbl_eu_8050BEA8[0x136], 1);
    f32 f3 = lbl_eu_806687B0 * *(f32*)((u8*)result + 0x44) - lbl_eu_806687B4;
    f32* pos = (f32*)(p + 0x44);
    *pos += lbl_eu_806687A8;
    f32 limit = lbl_eu_80668798 * f3;
    if (*pos > limit) *pos = limit;
    for (int i = 0; i < 3; i++) {
        void* s = *(void**)(p + 0x130 + i * 8);
        if (s) {
            void* o = *(void**)((u8*)s + 0x10);
            *(f32*)((u8*)o + 0x2C) = pos[0];
            *(f32*)((u8*)o + 0x30) = pos[1];
            *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
        }
    }
    void* s = *(void**)(p + 0x150);
    if (s) {
        void* o = *(void**)((u8*)s + 0x10);
        *(f32*)((u8*)o + 0x2C) = pos[0];
        *(f32*)((u8*)o + 0x30) = pos[1];
        *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
    }
    for (u8 i = 0; i < p[0x1F0]; i++) {
        void* s2 = *(void**)(p + 0x150 + i * 8);
        if (s2) {
            void* o = *(void**)((u8*)s2 + 0x10);
            *(f32*)((u8*)o + 0x2C) = pos[0];
            *(f32*)((u8*)o + 0x30) = pos[1];
            *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
        }
    }
    func_80248558(p + 0x140);
    func_8024577C(p + 0x1FC, *(u16*)(p + 0x5A));
    p[0x5D] = 1;
}

void func_8024EC24(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern u32 func_80248558(void*);
    extern void func_8024577C(void*, u16);
    extern f32 lbl_eu_80668798;
    extern f32 lbl_eu_806687A8;
    extern f32 lbl_eu_806687B0;
    extern f32 lbl_eu_806687B4;
    extern f32 lbl_eu_806687B8;
    extern f32 lbl_eu_80668764;
    u8* p = (u8*)self;
    if (p[0x58] || CSysWin_getUnk34(p + 0xB8) || CSysWin_getUnk34(p + 0xF4)) return;
    void* slot = *(void**)(p + 0x130);
    if (!slot) slot = 0;
    void* obj = *(void**)((u8*)slot + 0x10);
    VFuncPtr* vt = *(VFuncPtr**)obj;
    void* result = vt[15](obj, &lbl_eu_8050BEA8[0x136], 1);
    f32 f4 = lbl_eu_806687B0 * *(f32*)((u8*)result + 0x44) - lbl_eu_806687B4;
    f32 f3 = lbl_eu_80668798 * f4;
    f32* pos = (f32*)(p + 0x44);
    *pos -= lbl_eu_806687A8;
    f32 limit = lbl_eu_806687B8 * f3;
    if (*pos < limit) *pos = limit;
    for (int i = 0; i < 3; i++) {
        void* s = *(void**)(p + 0x130 + i * 8);
        if (s) {
            void* o = *(void**)((u8*)s + 0x10);
            *(f32*)((u8*)o + 0x2C) = pos[0];
            *(f32*)((u8*)o + 0x30) = pos[1];
            *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
        }
    }
    void* s = *(void**)(p + 0x150);
    if (s) {
        void* o = *(void**)((u8*)s + 0x10);
        *(f32*)((u8*)o + 0x2C) = pos[0];
        *(f32*)((u8*)o + 0x30) = pos[1];
        *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
    }
    for (u8 i = 0; i < p[0x1F0]; i++) {
        void* s2 = *(void**)(p + 0x150 + i * 8);
        if (s2) {
            void* o = *(void**)((u8*)s2 + 0x10);
            *(f32*)((u8*)o + 0x2C) = pos[0];
            *(f32*)((u8*)o + 0x30) = pos[1];
            *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
        }
    }
    func_80248558(p + 0x140);
    func_8024577C(p + 0x1FC, *(u16*)(p + 0x5A));
    p[0x5D] = 1;
}

void func_8024EE50(){}

void func_8024F1FC(void* self, u32 arg2) {
    extern void func_8003AA34();
    extern void* getFP__FPCc(const char*);
    extern u8 lbl_eu_80664798;
    lbl_eu_80664798 = (u8)arg2;
    if (arg2 > 0x1C) return;
    func_8003AA34();
    u32 strs[] = {
        0x524, 0x534, 0x544, 0x554, 0x564, 0x574, 0x584, 0x594,
        0x5A4, 0x5B4, 0x5C4, 0x5D4, 0x5E4, 0x5F4, 0x604, 0x614,
        0x624, 0x634, 0x634, 0x644, 0x654, 0x664, 0x674, 0x684,
        0x634, 0x644, 0x654, 0x664
    };
    lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[strs[arg2]]);
}

extern "C" u32 func_8024F538(u8* self) {
    u8 val = *(u8*)(self + 0x41);
    u32 result = __cntlzw(val - 2);
    return result >> 5;
}

extern "C" u16 func_8024F54C(u8* self) { return *(u16*)(self + 0x5A); }

extern "C" u8 func_8024F554(CFloorMapFull* self) { return self->field_58; }

void func_8024F55C(void* self) {
    extern int CSysWin_isActive(void*);
    extern void func_801D216C(void*, int);
    extern void func_8022B8E4(void*);
    extern void func_80138078(unsigned long);
    u8* p = (u8*)self;
    if (p[0x58] && CSysWin_isActive(p + 0xB8)) {
        func_801D216C(p + 0xA0, 0);
        func_8022B8E4(p + 0xB8);
        p[0x58] = 0;
        func_80138078(6);
    }
}

typedef void* (*VFuncPtr)(void*, const char*, u32);
u32 getHandleMEM2__Q23mtl10MemManagerFv();
void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, char const*, void*, int, int);
void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, char const*, void*, int, int);
int func_800A9D90();
void func_801F34F4(void*);

void func_8024C104(void* self) {
    u8* p = (u8*)self;
    u32 handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
    *(void**)(p + 0x24) = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, &lbl_eu_8050BEA8[0x4e7], self, 0, 0);
    u32 handle2 = func_800A9D90();
    *(void**)(p + 0x30) = readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(handle2, &lbl_eu_8050BEA8[0x4fc], self, 0, 0);
    u32 buffer[29];
    u32* dst = &buffer[1];
    u32* src = lbl_eu_8050BDF8;
    buffer[0] = 0;
    int count = 14;
    do {
        *dst++ = *src++;
        *dst++ = *src++;
    } while (--count);
    handle = getHandleMEM2__Q23mtl10MemManagerFv();
    u8 idx = lbl_eu_80664798;
    *(void**)(p + 0x28) = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, (char*)buffer[idx], self, 0, 0);
    func_801F34F4(p + 0x60);
    typedef void (*VoidVFuncPtr)(void*);
    VoidVFuncPtr* vt = *(VoidVFuncPtr**)(p + 0xB8);
    vt[0x20](p + 0xB8);
    vt = *(VoidVFuncPtr**)(p + 0xF4);
    vt[0x20](p + 0xF4);
}


void func_8024F5C4(void* self, u32 arg2) {
    void* ptr = *(void**)((u8*)self + 0x32D4);
    if (!ptr) return;
    void* obj = *(void**)((u8*)ptr + 0x10);
    VFuncPtr* vt = *(VFuncPtr**)obj;
    void* result = vt[15](obj, (char*)&lbl_eu_8050BEA8 + 0xEE, 1);
    *(u8*)((u8*)result + 0xBB) = (*(u8*)((u8*)result + 0xBB) & 0x7F) | (u8)arg2;
}

unsigned char func_8024F630(void) {
    return (unsigned char)func_8003B1EC(lbl_eu_8066479C);
}

void func_8024F658(void* self) {
    extern int CSysWin_getUnk34(void*);
    u8* p = (u8*)self;
    if (p[0x58]) return;
    if (CSysWin_getUnk34(p + 0xB8)) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    u8 val = p[0x208];
    u32 result = __cntlzw(val);
    p[0x208] = result >> 5;
}

u8 func_8024F6BC(void* self) {
    CFloorMapFull* full = static_cast<CFloorMapFull*>(self);
    if (full->field_58) return 0;
    return full->field_208;
}

extern "C" u8 func_8024F6D8(CFloorMapFull* self) { return self->field_208; }

u32 func_8024F6E0(void* self) {
    s8 idx1 = *(s8*)((u8*)self + 0x206);
    u16 val;
    if (idx1 < 0) {
        val = 0;
    } else {
        s8 idx0 = *(s8*)((u8*)self + 0x205);
        s8 idx2 = *(s8*)((u8*)self + 0x207);
        u32 offset = idx0 * 0x30C + (idx2 + idx1) * 0x18;
        val = *(u16*)((u8*)self + offset + 0x214);
    }
    return val != 0 ? 1 : 0;
}

void func_8024F72C(void* self) {
    extern int CSysWin_getUnk34(void*);
    u8* p = (u8*)self;
    if (CSysWin_getUnk34(p + 0xB8)) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    p[0x333C] = (u32)__cntlzw(p[0x333C]) >> 5;
}

u32 func_8024F784(void* self) {
    extern int CSysWin_getUnk34(void*);
    u8* p = (u8*)self;
    if (CSysWin_getUnk34(p + 0xB8)) return 1;
    return CSysWin_getUnk34(p + 0xF4);
}

void func_8024F7CC(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern int sprintf(char*, const char*, ...);
    extern void func_8024B4CC(void*, void*, void*);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_806687BC;
    extern u32 func_80248558(void*);
    extern void func_8024577C(void*, u16);
    u8* p = (u8*)self;
    if (!p[0x5D]) return;
    p[0x5D] = 0;
    if (*(u32*)(p + 0x2C)) return;
    if (p[0x41] != 1) return;
    if (!p[0x208]) return;
    if (CSysWin_getUnk34(p + 0xB8)) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    if (p[0x58]) return;
    s8 idx0 = (s8)p[0x205];
    u32 base = idx0 * 0x30C;
    if (!*(u8*)(p + base + 0x514)) return;
    s8 idx1 = (s8)p[0x206];
    u16 val = 0;
    if (idx1 >= 0) {
        s8 idx2 = (s8)p[0x207];
        val = *(u16*)(p + base + (idx2 + idx1) * 0x18 + 0x214);
    }
    if (!val) return;
    f32 pos[3] = {lbl_eu_80668764, lbl_eu_80668764, lbl_eu_80668764};
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB]);
    void* data = *(void**)(p + 0x140);
    void* obj = *(void**)((u8*)data + 0x10);
    VFuncPtr* vtable = *(VFuncPtr**)obj;
    void* result = vtable[15](obj, buf, 1);
    if (result) {
        void* target = *(void**)((u8*)data + 0x10);
        void* node = result;
        if (node && *(void**)((u8*)node + 0x10) != target) {
            void* next = *(void**)((u8*)node + 0x0C);
            if (next && *(void**)((u8*)next + 0x10) != target) {
                void* next2 = *(void**)((u8*)next + 0x0C);
                if (next2 && *(void**)((u8*)next2 + 0x10) != target) {
                    f32 p1[3];
                    func_8024B4CC(p1, data, next2);
                    pos[0] += p1[0]; pos[1] += p1[1]; pos[2] += p1[2];
                }
                pos[0] += *(f32*)((u8*)next2 + 0x2C);
                pos[1] += *(f32*)((u8*)next2 + 0x30);
                pos[2] += *(f32*)((u8*)next2 + 0x34);
            }
            pos[0] += *(f32*)((u8*)next + 0x2C);
            pos[1] += *(f32*)((u8*)next + 0x30);
            pos[2] += *(f32*)((u8*)next + 0x34);
        }
        void* result2 = vtable[15](obj, &lbl_eu_8050BEA8[0x136], 1);
        if (result2) {
            f32 scale = *(f32*)((u8*)result2 + 0x44);
            pos[0] *= scale;
            pos[0] += *(f32*)((u8*)data + 0x2C);
            pos[1] += *(f32*)((u8*)data + 0x30);
            pos[2] += *(f32*)((u8*)data + 0x34);
        }
    }
    *(f32*)(p + 0x54) = lbl_eu_806687BC;
    p[0x41] = 2;
    *(f32*)(p + 0x4C) = pos[0] / lbl_eu_806687BC;
    *(f32*)(p + 0x50) = pos[1] / lbl_eu_806687BC;
}

u32 func_8024FB78() {
    extern u32 lbl_eu_80664184;
    extern u8 lbl_eu_80664798;
    extern u32 lbl_eu_8066479C;
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern u32 func_8003B1EC(u32);
    extern s16 func_80136330(u32, const char*, u32);
    extern void func_80141DC4(f32*);
    extern f64 lbl_eu_80668770;
    u32 result = 0;
    if ((lbl_eu_80664184 & 0xFF) == lbl_eu_80664798) {
        if (!lbl_eu_8066479C) return 0;
        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
        if (!player) return 0;
        void** vt = *(void***)player;
        f32* pos = (f32*)((void*(*)(void*))vt[0xAC])(player);
        f32 y = pos[1];
        u32 count = func_8003B1EC(lbl_eu_8066479C);
        for (u32 i = 1; i <= count; i++) {
            s16 val = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], i);
            if ((f32)(s16)val > y) return i;
        }
    } else {
        if (!lbl_eu_8066479C) return 0;
        f32 buf[3];
        func_80141DC4(buf);
        u32 count = func_8003B1EC(lbl_eu_8066479C);
        for (u32 i = 1; i <= count; i++) {
            s16 val = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], i);
            if ((f32)(s16)val > buf[1]) return i;
        }
    }
    return 0;
}

void CFloorMap::OnFileEvent() const {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_80250CB4() {
    extern u16 lbl_eu_806647A0[];
    extern u16 lbl_eu_806647A8[];
    extern u16 lbl_eu_806647B0[];
    extern u16 lbl_eu_806647B8[];
    for (int i = 0; i < 4; i++) {
        lbl_eu_806647A0[i] = 0xFFFF;
        lbl_eu_806647A8[i] = 0xFFFF;
    }
    lbl_eu_806647B0[0] = 0xA0;
    lbl_eu_806647B0[1] = 0x8C;
    lbl_eu_806647B0[2] = 0x23;
    lbl_eu_806647B0[3] = 0xFFFF;
    lbl_eu_806647B8[0] = 0xD9;
    lbl_eu_806647B8[1] = 0xC0;
    lbl_eu_806647B8[2] = 0x43;
    lbl_eu_806647B8[3] = 0xFFFF;
}

extern "C" void func_80244764() {}
extern "C" void func_80244944() {}
extern "C" void func_80244AE8() {}
extern "C" void func_80244C60() {}
extern "C" void func_80244DD8() {}
extern "C" void func_802452C4() {}
