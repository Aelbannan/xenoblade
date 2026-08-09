// Auto-scaffolded catalog TU for kyoshin/cf/IResInfo
// Replace stubs with high-level C/C++ during decomp.

#include <string.h>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/IResInfo.hpp"

// ============================================================
// External globals
// ============================================================
extern "C" {
    extern u32 lbl_eu_80663E24;
    extern u32 lbl_eu_80663E28;
    extern u32 lbl_eu_80663E30;
    extern u32 lbl_eu_8065FC18;
    extern float lbl_eu_80666200;
    extern float lbl_eu_80666204;
    extern float lbl_eu_80666208;
    extern float lbl_eu_8066620C;
    extern u32 lbl_eu_804FB380;
    extern u32 lbl_eu_80526920[];
    extern u32 lbl_eu_80526938[];
    extern u32 lbl_eu_80526944[];
    extern u32 lbl_eu_805269AC[];
    extern u32 lbl_eu_80526A10[];
    extern u32 lbl_eu_80526A70[];
    extern u32 lbl_eu_80526AD0[];
    extern u32 lbl_eu_80526B30[];
    extern u32 lbl_eu_80526B90[];
    extern u32 lbl_eu_80526BF0[];
    extern u32 lbl_eu_80526C50[];
    extern u32 lbl_eu_80526CB0[];
    extern u32 lbl_eu_80526D10[];
    extern u32 lbl_eu_80526D70[];
    extern u32 lbl_eu_80526920[];
}

// ============================================================
// External function declarations
// ============================================================
extern "C" {
    void func_800832BC__Q22cf13CfGameManagerFv(u16*, u16*);
    int func_800A86D8(int, int);
    int func_800A85D8(u8);
    int func_800A84FC(u8);
    int func_800A87E0(int);
    int func_800A8704(int);
    int func_800A88C8(int);
    int func_800A89C0(int);
    int func_800A8AAC(int);
    int func_800A86AC(int, int, int);
    void func_80062430(u32*, int, u32*);
    int func_800AA600(void*);
    void func_800AA33C(void*, u32, u32, u32);
    bool func_800A9068(void*);
    bool func_800A8BD8(void*);
    void func_800A8C68(void*);
    bool func_800A9024(void*);
    void func_800A9344(void*, int);
    void func_800B79A4(void*);
    void func_800B7A18(void);
    void func_804CC1BC(void*);
    void func_804CC1D8(void*, void*);
    bool func_800B1C40(void*);
    u32 func_80061FFC(void);
    bool func_800865E8__Q22cf13CfGameManagerFv(void);
    void func_801BFA64(int);
    void func_801BFA08(int, void*, int, int);
    void func_801BFA88(int, int, int, int);
    void func_801BFE8C(int, int, int);
    void cancel__11CDeviceFileFP11CFileHandle(void*);
    void waitForDrawDone__9CDeviceVIFv(void);
    void* allocate_array__Q23mtl10MemManagerFUlUl(u32, u32);
    void deallocate__Q23mtl10MemManagerFPv(void*);
    void __dla__FPv(void*);
    void __dl__FPv(void*);
    void CfRes_orBits_649B4(void*, int);
    void func_800A9CD0(void);
    void func_800A8C90(void);
    void* func_80066E7C(void*, u32);
    void* func_80066CF8(void*);
    void* func_80066DAC(void*, u32*);
    bool func_80066788(void*, bool, bool, bool);
}

// Named wrappers (forward decls)
bool testResInfoFlag(u32 flags);
bool testWordFlag(const u32* p, u32 mask);

// ============================================================
// Tiny functions
// ============================================================

extern "C" void func_eu_80066938() {
    func_800A9CD0();
}

extern "C" bool func_80068998() { return true; }
extern "C" int func_800689B8() { return 4; }
extern "C" int func_800689C8() { return 2; }
extern "C" int func_800689E0() { return 0x200; }
extern "C" int func_800689E8() { return 0x200; }
extern "C" int func_800689F0() { return 0x200; }
extern "C" int func_800689F8() { return 0x20; }
extern "C" int func_80068A00() { return 0x200; }
extern "C" int func_80068A08() { return 0x200; }
extern "C" int func_80068A10() { return 0x200; }
extern "C" int func_eu_800693E8() { return 0x200; }
extern "C" int func_8006846C(int a) { return a * 11 + 13; }
extern "C" int func_800689AC() { return 0x10004; }

extern "C" void func_800665F4(u8* self) {
    u8 b = *(u8*)((char*)self + 0x32);
    if (b < 8) func_801BFA64(b + 5);
}

extern "C" void* func_80066160(u8* self) {
    u8 type = *(u8*)((char*)self + 0x33);
    s16 val = *(s16*)((char*)self + 0x34);
    if (type == 10 && val < 3) return *(void**)((char*)self + 0x18);
    return 0;
}

extern "C" void* func_80066184(u8* self) {
    u8 type = *(u8*)((char*)self + 0x33);
    s16 val = *(s16*)((char*)self + 0x34);
    if (type == 0 && val < 3) return *(void**)((char*)self + 0x18);
    return 0;
}

extern "C" int func_8006626C(u8* self) {
    u8 type = *(u8*)((char*)self + 0x33);
    s16 val = *(s16*)((char*)self + 0x34);
    if (type == 9 && val >= 0) return val + 2;
    return val;
}

extern "C" int func_8006611C(u8* self) {
    u8 type = *(u8*)((char*)self + 0x33);
    s16 val = *(s16*)((char*)self + 0x34);
    if (type == 10 && val < 3) return func_800A8AAC(val);
    return 0;
}

// ============================================================
// func_80062430 wrapper functions
// ============================================================

extern "C" u32 func_8006638C(u8* self) {
    if (self == 0) return 0;
    u32* r = *(u32**)((char*)self + 0x08);
    if (r == 0) return 0;
    u32 result = 0;
    func_80062430(r, 0, &result);
    return result;
}

extern "C" u32 func_800663D8(u8* self) {
    if (self == 0) return 0;
    u32* r = *(u32**)((char*)self + 0x08);
    if (r == 0) return 0;
    u32 result = 0;
    func_80062430(r, 1, &result);
    return result;
}

extern "C" u32 func_80066424(u8* self) {
    if (self == 0) return 0;
    u32* r = *(u32**)((char*)self + 0x08);
    if (r == 0) return 0;
    u32 result = 0;
    func_80062430(r, 1, &result);
    return result;
}

extern "C" u32 func_8006646C(u8* self) {
    if (self == 0) return 0;
    u32* r = *(u32**)((char*)self + 0x08);
    if (r == 0) return 0;
    u32 result = 0;
    func_80062430(r, 4, &result);
    return result;
}

extern "C" u32 func_800664B8(u8* self) {
    if (self == 0) return 0;
    u32* r = *(u32**)((char*)self + 0x08);
    if (r != 0) {
        u32 result = 0;
        func_80062430(r, 2, &result);
    }
    return *(u32*)((char*)self + 0x10);
}

extern "C" u32 func_8006650C(u8* self) {
    if (self == 0) return 0;
    u32* r = *(u32**)((char*)self + 0x08);
    if (r == 0) return 0;
    u32 result = 0;
    func_80062430(r, 2, &result);
    return result;
}

extern "C" u32 func_80066554(u8* self) {
    if (self == 0) return 0;
    u32* r = *(u32**)((char*)self + 0x08);
    if (r == 0) return 0;
    u32 result = 0;
    func_80062430(r, 3, &result);
    return result;
}

extern "C" void func_800665A0(u8* self) {
    void* f = *(void**)((char*)self + 0x28);
    if (f != 0) {
        cancel__11CDeviceFileFP11CFileHandle(f);
        *(u32*)((char*)self + 0x04) = 0;
        *(u32*)((char*)self + 0x08) = 0;
        *(u32*)((char*)self + 0x28) = 0;
        *(u32*)((char*)self + 0x00) = 0;
        *(u32*)((char*)self + 0x24) = 0;
        *(u32*)((char*)self + 0x20) = 0;
    }
}

// ============================================================
// func_80065F24 (0x90)
// ============================================================
extern "C" void func_80065F24(u8* self, void* param) {
    u32* ptr10 = *(u32**)((char*)param + 0x10);
    if (ptr10 == 0) {
        u16 a, b;
        func_800832BC__Q22cf13CfGameManagerFv(&a, &b);
        u32 val = (a << 20) | 0x60000000 | (b << 10);
        void* result = func_80066E7C(param, val);
        if (result != 0) {
            u32* fc18 = &lbl_eu_8065FC18;
            if (fc18 != 0) {
                *(u32**)((char*)param + 0x10) = (u32*)result;
                func_804CC1BC(fc18);
            }
        }
    }
    lbl_eu_80663E24 |= 0x1000;
}

// ============================================================
// func_eu_80066940 (0x4C)
// ============================================================
extern "C" void func_eu_80066940(u8* self, void* param) {
    void* obj = *(void**)((char*)param + 0x2C);
    void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
    void* result = fn(obj, 0);
    CBdat::func_8003AA78(1, result);
    lbl_eu_80663E28 |= 0x2000;
}

// ============================================================
// func_80065FB4 (0x168) - switch dispatch
// ============================================================
extern "C" int func_80065FB4(u8* self, int param) {
    u8 type = *(u8*)((char*)self + 0x33);
    u8 subtype = *(u8*)((char*)self + 0x32);
    s16 v34 = *(s16*)((char*)self + 0x34);
    
    switch (type) {
    case 0: func_800A86D8(param, 0); return func_800A85D8(subtype) + func_800A86D8(param, 0);
    case 1: func_800A86D8(param, 1); return func_800A85D8(subtype) + func_800A86D8(param, 1);
    case 2: func_800A86D8(param, 2); return func_800A85D8(subtype) + func_800A86D8(param, 2);
    case 3: func_800A86D8(param, 3); return func_800A85D8(subtype) + func_800A86D8(param, 3);
    case 4: func_800A86D8(param, 4); return func_800A85D8(subtype) + func_800A86D8(param, 4);
    case 5: return func_800A84FC(subtype);
    case 6: if (v34 < 3) return func_800A87E0(v34); break;
    case 7: if (v34 < 3) return func_800A8704(v34); break;
    case 8: if (v34 < 3) return func_800A88C8(v34); break;
    case 9: if (v34 < 3) return func_800A89C0(v34); break;
    case 10: if (v34 < 3) return func_800A8AAC(v34); break;
    }
    return 0;
}

// ============================================================
// func_800661A8 (0xC4) - bit test
// ============================================================
extern "C" bool func_800661A8(u8* self, int r5, int r6) {
    u8 type = *(u8*)((char*)self + 0x33);
    int mask = 0;
    switch (type) {
    case 0: mask = func_800A86AC(r5, 0, 0); break;
    case 1: mask = 0x10000; break;
    case 2: mask = 0x32000; break;
    case 3: mask = -0x28000; break;
    case 4: mask = 0x1C800; break;
    case 5: mask = 0x62800; break;
    case 6: mask = -0x57000; break;
    }
    if (mask == 0) return false;
    return (r6 ^ mask) == 0;
}

// ============================================================
// func_80066290 (0xE4)
// ============================================================
extern "C" void func_80066290(u8* self, void* param) {
    u8 type = *(u8*)((char*)param + 0x33);
    s16 v34 = *(s16*)((char*)param + 0x34);
    if (type == 10) {
        u32* p10 = *(u32**)((char*)param + 0x10);
        if (p10 == 0) {
            void* obj = *(void**)((char*)param + 0x2C);
            void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
            void* r = fn(obj, 0);
            if (r != 0) {
                u32* fc18 = &lbl_eu_8065FC18;
                if (fc18 != 0) {
                    *(u32**)((char*)param + 0x10) = (u32*)r;
                    func_804CC1BC(fc18);
                }
            }
        }
    } else if (type == 9 && v34 >= 0) {
        void* obj = *(void**)((char*)param + 0x2C);
        void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
        void* r = fn(obj, 0);
        func_801BFA08(v34 + 2, r, 0x62800, *(u32*)((char*)param + 0x18));
        func_801BFA88(v34 + 2, 3, 0, 0);
    }
}

// ============================================================
// func_8006660C (0x108)
// ============================================================
extern "C" void func_8006660C(u8* self) {
    void* r = func_80066CF8(self);
    u32* p10 = *(u32**)((char*)self + 0x10);
    if (p10 == 0 && r != 0) {
        u32* fc18 = &lbl_eu_8065FC18;
        if (fc18 != 0) {
            *(u32**)((char*)self + 0x10) = (u32*)r;
            func_804CC1BC(fc18);
        }
    }
    u32 out;
    void* entry = func_80066DAC(self, &out);
    u32 flags = *(u32*)((char*)self + 0x00);
    if (!(flags & 0x800) && entry != 0) {
        u8 st = *(u8*)((char*)self + 0x32);
        if (st < 8) {
            int idx = st + 5;
            func_801BFA64(idx);
            func_801BFA08(idx, entry, 0x62800, out);
            void* obj = *(void**)((char*)self + 0x2C);
            void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
            void* r2 = fn(obj, 0);
            u32 p18 = *(u32*)((char*)self + 0x18);
            u32 p1C = *(u32*)((char*)self + 0x1C);
            func_801BFA88(idx, 5, (u32)r2 + p18, p18 - p1C);
            *(u32*)((char*)self + 0x00) |= 0x800;
        }
    }
}

// ============================================================
// func_80066714 (0x74)
// ============================================================
extern "C" void func_80066714(u8* self, bool cleanup) {
    u32* fc18 = &lbl_eu_8065FC18;
    if (fc18 != 0) {
        u32* p10 = *(u32**)((char*)self + 0x10);
        if (p10 != 0) {
            func_800B79A4(p10);
            if (cleanup) {
                func_804CC1D8(fc18, p10);
                *(u32*)((char*)self + 0x10) = 0;
            }
        }
    }
}

// ============================================================
// func_80066C74 (0x84)
// ============================================================
extern "C" void func_80066C74(u8* self, void* data, void* size) {
    void* obj = *(void**)((char*)self + 0x2C);
    u32 (*fn)(void*) = (u32 (*)(void*))*(void**)(*(u32*)obj + 0xC);
    u32 f = fn(obj);
    if (f & 0x18E) {
        func_80066788(self, 0, 0, 0);
        *(u32*)((char*)self + 0x08) = (u32)data;
        *(u32*)((char*)self + 0x00) |= 0x80;
        *(u32*)((char*)self + 0x1C) = (u32)size;
    }
}

// ============================================================
// func_80066CF8 (0xB4) - find entry by name
// ============================================================
extern "C" void* func_80066CF8(void* self) {
    void* obj = *(void**)((char*)self + 0x2C);
    void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
    void* base = fn(obj, 0);
    if (base == 0) return 0;
    if (strcmp((const char*)&lbl_eu_804FB380, (const char*)base) != 0) return 0;
    
    u32* eb = (u32*)((char*)base + 0x10);
    u32 cnt = *(u32*)((char*)base + 0x08);
    for (u32 i = 0; i < cnt; i++) {
        int val = func_800AA600((char*)eb + i * 0x10 + 0x08);
        val -= 0xC;
        if (val >= 0 && val <= 5) {
            u32 off = eb[i * 4];
            return (char*)base + off;
        }
    }
    return 0;
}

// ============================================================
// func_80066DAC (0xD0) - find entry by name, return index
// ============================================================
extern "C" void* func_80066DAC(void* self, u32* outIdx) {
    void* obj = *(void**)((char*)self + 0x2C);
    void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
    void* base = fn(obj, 0);
    *outIdx = 0;
    if (base == 0) return 0;
    if (strcmp((const char*)&lbl_eu_804FB380, (const char*)base) != 0) return 0;
    
    u32* eb = (u32*)((char*)base + 0x10);
    u32 cnt = *(u32*)((char*)base + 0x08);
    for (u32 i = 0; i < cnt; i++) {
        int val = func_800AA600((char*)eb + i * 0x10 + 0x08);
        val -= 0x12;
        if (val >= 0 && val <= 4) {
            *outIdx = eb[i * 4 + 1];
            u32 off = eb[i * 4];
            return (char*)base + off;
        }
    }
    return 0;
}

// ============================================================
// func_80066E7C (0x120) - find entry by ID
// ============================================================
extern "C" void* func_80066E7C(void* self, u32 id) {
    void* obj = *(void**)((char*)self + 0x2C);
    void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
    void* base = fn(obj, 0);
    
    u32 cat = id >> 27;
    if (cat >= 0x0E && cat <= 0x0F) {
        void* (*cfn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 0x40);
        if (cfn(obj, 0) != 0) return base;
        return 0;
    }
    if (base == 0) return 0;
    if (strcmp((const char*)&lbl_eu_804FB380, (const char*)base) != 0) return 0;
    
    char nameBuf[8] = {0};
    func_800AA33C(nameBuf, id, 0, 0);
    
    u32 cnt = *(u32*)((char*)base + 0x08);
    u32* eb = (u32*)((char*)base + 0x10);
    for (u32 i = 0; i < cnt; i++) {
        if (strncmp(nameBuf, (const char*)((char*)eb + i * 0x10 + 0x08), 8) == 0) {
            u32 off = eb[i * 4];
            return (char*)base + off;
        }
    }
    return 0;
}

// ============================================================
// func_80066788 (0x4D4) - main update
// ============================================================
extern "C" bool func_80066788(void* self, bool r4, bool r5, bool r6) {
    if (self == 0 && r6 == 0) { r4 = true; r5 = true; }
    
    bool hasClear = false;
    if (func_800B1C40(self)) {
        if (lbl_eu_80663E28 & (1 << 4)) hasClear = true;
    }
    bool flag25 = (lbl_eu_80663E28 >> 6) & 1;
    bool didWork = false;
    
    u32* data = *(u32**)((char*)self + 0x08);
    if (data != 0 && (*(u32*)((char*)self + 0x00) & 0x100)) {
        void* obj = *(void**)((char*)self + 0x2C);
        u32 (*ofn)(void*) = (u32 (*)(void*))*(void**)(*(u32*)obj + 0xC);
        u32 of = ofn(obj);
        
        if (of & 0x10000) {
            void* (*cfn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 0x3C);
            if (cfn(obj, 0) != 0) {
                if (r6) { cancel__11CDeviceFileFP11CFileHandle(*(void**)((char*)self + 0x28)); *(u32*)((char*)self + 0x28) = 0; }
                else return false;
            }
            waitForDrawDone__9CDeviceVIFv();
            u32* rd = *(u32**)((char*)self + 0x08);
            if (func_800A8BD8(rd)) func_800A8C68(rd);
            else if (func_800A9024(rd)) func_800A9344(rd, 1);
            else if (rd != 0) { deallocate__Q23mtl10MemManagerFPv(rd); *(u32*)((char*)self + 0x08) = 0; }
            didWork = true;
        } else {
            void* (*cfn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 0x3C);
            if (cfn(obj, 0) != 0) {
                cancel__11CDeviceFileFP11CFileHandle(*(void**)((char*)self + 0x28));
                didWork = false;
                *(u32*)((char*)self + 0x28) = 0;
                u32* rd = *(u32**)((char*)self + 0x08);
                if (func_800A8BD8(rd)) func_800A8C68(rd);
                else if (func_800A9024(rd)) func_800A9344(rd, 1);
                else if (rd != 0) { deallocate__Q23mtl10MemManagerFPv(rd); *(u32*)((char*)self + 0x08) = 0; }
                didWork = true;
            }
            // ... rest of the function is very complex
        }
    }
    return didWork;
}

// ============================================================
// func_80068494 (0xD0) - search entry
// ============================================================
extern "C" void* func_80068494(void* self, u32 id, u32* outIdx, u32* outVal, int start, int end) {
    *outIdx = 0;
    *outVal = -1;
    if (id == 0) return 0;
    
    for (int i = start; i < end; i++) {
        u32* entry = (u32*)((char*)self + i * 0x3C);
        if (entry[2] == id) {
            *outIdx = i;
            *outVal = *(u8*)((char*)entry + 0x36);
            entry[4] = 0;
            *(u16*)((char*)entry + 0x3A) = 0;
            return (char*)entry + 4;
        }
    }
    for (int i = start; i < end; i++) {
        u32* entry = (u32*)((char*)self + i * 0x3C);
        if (entry[2] == 0) {
            *outIdx = i;
            *outVal = *(u8*)((char*)entry + 0x36);
            entry[4] = 0;
            *(u16*)((char*)entry + 0x3A) = 0;
            return (char*)entry + 4;
        }
    }
    return 0;
}

// ============================================================
// func_80068564 (0x58)
// ============================================================
extern "C" void* func_80068564(u8* self, u32 id, u32* outIdx, u32* outVal) {
    void* r = func_80068494(self, id, outIdx, outVal, 0x59, 0x61);
    if (r == 0) CfRes_orBits_649B4(self, 2);
    return r;
}

// ============================================================
// func_800685C8 (0x54) - search entries at 0x2D0
// ============================================================
extern "C" void* func_800685C8(u8* self, u32 id, u32* outIdx) {
    *outIdx = 0;
    if (id == 0) return 0;
    for (int i = 0xC; i < 0x4D + 0xC; i++) {
        u32* entry = (u32*)((char*)self + 0x2D0 + i * 0x3C);
        if (entry[2] == id) {
            *outIdx = i;
            return (char*)entry + 4;
        }
    }
    return 0;
}

// ============================================================
// func_8006861C (0x64) - search entries at 0x16BC
// ============================================================
extern "C" void* func_8006861C(u8* self, u32 id, u32* outIdx, u32* outVal) {
    *outIdx = 0;
    *outVal = -1;
    if (id == 0) return 0;
    for (int i = 0x61; i < 0x61 + 0x20; i++) {
        u32* entry = (u32*)((char*)self + 0x16BC + (i - 0x61) * 0x3C);
        if (entry[2] == id) {
            *outIdx = i;
            *outVal = *(u8*)((char*)entry + 0x36);
            return (char*)entry + 4;
        }
    }
    return 0;
}

// ============================================================
// func_800686E4 (0x244) - update/resolve
// ============================================================
extern "C" bool func_800686E4(void* self, bool param) {
    u32 flags24 = lbl_eu_80663E24;
    bool cond = (flags24 & (1 << 5)) || ((flags24 >> 25) & 1) || ((flags24 >> 10) & 1);
    bool flag25 = (lbl_eu_80663E28 >> 6) & 1;
    
    if (func_800865E8__Q22cf13CfGameManagerFv()) return false;
    if (flag25) return false;
    
    if (cond) {
        lbl_eu_80666204 = 0.0f;
        lbl_eu_80666208 = 0.0f;
        *(float*)((char*)self + 0x1ED0) = param ? 0.0f : 0.0f;
    } else {
        float t = *(float*)((char*)self + 0x1ED0);
        lbl_eu_8066620C = 0.0f;
        t -= 0.0f;
        *(float*)((char*)self + 0x1ED0) = t;
        if (t < 0.0f) *(float*)((char*)self + 0x1ED0) = 0.0f;
    }
    
    float zero = 0.0f;
    bool any = false;
    int idx = 0x59;
    for (int i = 0x59; i < 0x81; i++) {
        // ... complex per-entry logic
    }
    if (any) waitForDrawDone__9CDeviceVIFv();
    return any;
}

// ============================================================
// func_80068928 (0x68) - search
// ============================================================
extern "C" void* func_80068928(u8* self, u32 id, int start, int end) {
    for (int i = start; i < end; i++) {
        u32* entry = (u32*)((char*)self + i * 0x3C);
        if (entry[2] == id) {
            if (i <= 0x81) return (char*)entry + 4;
        }
    }
    return 0;
}

// ============================================================
// func_800685BC (0xC) - wrapper
// ============================================================
extern "C" void* func_800685BC(u8* self, u32 id, u32* outIdx, u32* outVal) {
    return func_80068494(self, id, outIdx, outVal, 0x61, 0x81);
}

// ============================================================
// func_80067D38 (0x7C) - reset entries
// ============================================================
extern "C" void func_80067D38(void* self) {
    for (int i = 0; i < 0x82; i++) {
        void* f = *(void**)((char*)self + 0x2C);
        if (f != 0) {
            cancel__11CDeviceFileFP11CFileHandle(f);
            *(u32*)((char*)self + 0x08) = 0;
            *(u32*)((char*)self + 0x0C) = 0;
            *(u32*)((char*)self + 0x2C) = 0;
            *(u32*)((char*)self + 0x04) = 0;
            *(u32*)((char*)self + 0x28) = 0;
            *(u32*)((char*)self + 0x24) = 0;
        }
        *(u32*)((char*)self + 0x18) = 0;
        self = (char*)self + 0x3C;
    }
}

// ============================================================
// func_80067DB4, 80067E78, 80067F10, 80067FE0, 80068078, 80068110, 80068254 - loop over entries
// ============================================================

// func_80067DB4 (0xC4) - init entries
extern "C" void func_80067DB4(u8* self) {
    memset((char*)self + 0x1518, 0, 8);
    memset((char*)self + 0x1554, 0, 8);
    memset((char*)self + 0x1590, 0, 8);
    memset((char*)self + 0x15CC, 0, 8);
    memset((char*)self + 0x1608, 0, 8);
    memset((char*)self + 0x1644, 0, 8);
    memset((char*)self + 0x1680, 0, 8);
    memset((char*)self + 0x16BC, 0, 8);
    
    void* base = (char*)self + 0x16BC;
    void* target = (char*)self + 0x16C0;
    for (int i = 0x61; i < 0x81; i++) {
        void* obj = *(void**)((char*)base + 0x30);
        void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
        fn(obj, 0);
        *(u16*)((char*)base + 0x3C) = 0;
        *(u16*)((char*)base + 0x3E) = 0;
        base = (char*)base + 0x3C;
        target = (char*)target + 0x3C;
    }
}

// func_80067E78 (0x98) - load entries
extern "C" void func_80067E78(u8* self, bool param) {
    void* base = (char*)self + 0x16BC;
    void* target = (char*)self + 0x16C0;
    for (int i = 0x61; i < 0x81; i++) {
        void* obj = *(void**)((char*)base + 0x30);
        void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
        void* r = fn(obj, 0);
        if (r != 0) func_80066788(target, 0, 0, param);
        base = (char*)base + 0x3C;
        target = (char*)target + 0x3C;
    }
}

// func_80067F10 (0xD0)
extern "C" void func_80067F10(u8* self, bool param) {
    func_800A9068(self);
    if (!func_800A9068(self)) {
        void* base = (char*)self + 0x16BC;
        void* target = (char*)self + 0x16C0;
        for (int i = 0x61; i < 0x81; i++) {
            void* obj = *(void**)((char*)base + 0x30);
            void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
            void* r = fn(obj, 0);
            if (r != 0) {
                u32 flags = *(u32*)((char*)base + 0x08);
                u32 cat = flags >> 27;
                if (cat != 8 && cat != 11) {
                    func_80066788(target, 0, 0, param);
                }
            }
            base = (char*)base + 0x3C;
            target = (char*)target + 0x3C;
        }
    }
}

// func_80067FE0 (0x98)
extern "C" void func_80067FE0(u8* self) {
    void* base = (char*)self + 0x16BC;
    void* target = (char*)self + 0x16C0;
    for (int i = 0x61; i < 0x81; i++) {
        void* obj = *(void**)((char*)base + 0x30);
        void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
        void* r = fn(obj, 0);
        if (r != 0) {
            u32 fl = *(u32*)((char*)base + 0x04);
            if (fl & 0x30000) {
                func_80066788(target, 0, 0, true);
            }
        }
        base = (char*)base + 0x3C;
        target = (char*)target + 0x3C;
    }
}

// func_80068078 (0x98)
extern "C" void func_80068078(u8* self) {
    void* base = (char*)self + 0x14DC;
    void* target = (char*)self + 0x14E0;
    for (int i = 0x59; i < 0x81; i++) {
        void* obj = *(void**)((char*)base + 0x30);
        void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
        void* r = fn(obj, 0);
        if (r != 0) {
            if (func_800A8BD8(r)) {
                func_80066788(target, true, true, false);
            }
        }
        base = (char*)base + 0x3C;
        target = (char*)target + 0x3C;
    }
}

// func_80068110 (0x144)
extern "C" void func_80068110(u8* self, bool param) {
    waitForDrawDone__9CDeviceVIFv();
    u32 flags24 = lbl_eu_80663E24;
    bool hasFlag = (flags24 >> 18) & 1;
    bool flagVal = ((flags24 >> 25) | (flags24 >> 10)) & 1;
    bool flagResult = flagVal;
    
    void* base = (char*)self + 0x14DC;
    void* target = (char*)self + 0x14E0;
    for (int i = 0x59; i < 0x61; i++) {
        void* obj = *(void**)((char*)base + 0x30);
        void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
        void* r = fn(obj, 0);
        if (r != 0) {
            if (func_800A8BD8(r)) {
                s16 t = *(s16*)((char*)base + 0x3E);
                if (t == 0) {
                    void* (*cfn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 0x40);
                    if (cfn(obj, 0) != 0) {
                        u32 fl = *(u32*)((char*)base + 0x04);
                        bool hasFlag2 = (fl & 0x400) || (fl & 0x200);
                        if (param) {
                            if (hasFlag2) func_80066788(target, 0, 0, true);
                        } else {
                            if (!hasFlag2) {
                                u32 cat = *(u32*)((char*)base + 0x08) >> 27;
                                if (!(flagResult && (cat == 8 || cat == 11))) {
                                    func_80066788(target, 0, 0, true);
                                }
                            }
                        }
                    }
                }
            }
        }
        base = (char*)base + 0x3C;
        target = (char*)target + 0x3C;
    }
}

// func_80068254 (0x104)
extern "C" void func_80068254(u8* self) {
    bool first = true;
    void* base = (char*)self + 0x14DC;
    void* target = (char*)self + 0x14E0;
    for (int i = 0x59; i < 0x81; i++) {
        void* obj = *(void**)((char*)base + 0x30);
        void* (*fn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 8);
        void* r = fn(obj, 0);
        if (r != 0) {
            if (func_800A8BD8(r)) {
                s16 t = *(s16*)((char*)base + 0x3E);
                if (t == 0) {
                    void* (*cfn)(void*, int) = (void* (*)(void*, int))*(void**)(*(u32*)obj + 0x40);
                    if (cfn(obj, 0) != 0) {
                        u32 fl = *(u32*)((char*)base + 0x04);
                        bool hasFlag2 = (fl & 0x400) || (fl & 0x200);
                        if (hasFlag2) {
                            if (first) { waitForDrawDone__9CDeviceVIFv(); first = false; }
                            func_80066788(target, 0, 0, true);
                        }
                    }
                }
            }
        }
        base = (char*)base + 0x3C;
        target = (char*)target + 0x3C;
    }
}

// ============================================================
// func_80068358 (0xD4) - flag handling
// ============================================================
extern "C" void func_80068358(u8* self) {
    bool flag2 = false;
    if (lbl_eu_80663E24 & 2) flag2 = true;
    bool f10 = testResInfoFlag(0x10);
    bool f40 = testResInfoFlag(0x40);
    
    if (func_800B1C40(self) || (f10 && f40)) {
        flag2 = true;
        CfRes_orBits_649B4(self, 1);
    }
    
    if (testWordFlag((const u32*)self, 1)) {
        func_800686E4(self, flag2);
        clearWordFlag((u32*)self, 2);
    }
}

// ============================================================
// Constructor (stub)
// ============================================================
extern "C" void __ct__80066F9C(u8* self) {
    // Large constructor - stub for now
    memset(self, 0, 0x1E78);
}

// ============================================================
// Destructors (stubs)
// ============================================================
extern "C" void __dt___reslist_base_unsigned_short(u8* self, int mode) {
    if (self == 0) return;
    *(u32*)((char*)self + 0x00) = (u32)&lbl_eu_80526938;
    if (mode > 0) __dl__FPv(self);
}

extern "C" void __dt__reslist_unsigned_short(u8* self, int mode) {
    if (self == 0) return;
    *(u32*)((char*)self + 0x00) = (u32)&lbl_eu_80526938;
    u8 owns = *(u8*)((char*)self + 0x1C);
    if (owns == 0) {
        void* buf = *(void**)((char*)self + 0x14);
        if (buf != 0) { __dla__FPv(buf); *(u32*)((char*)self + 0x14) = 0; }
    }
    if (mode > 0) __dl__FPv(self);
}

extern "C" void __dt__8006754C(u8* self, int mode) {
    if (self == 0) return;
    *(u32*)((char*)self + 0x00) = (u32)&lbl_eu_80526938;
    u8 owns = *(u8*)((char*)self + 0x1ECC);
    if (owns == 0) {
        void* buf = *(void**)((char*)self + 0x1EC4);
        if (buf != 0) { __dla__FPv(buf); *(u32*)((char*)self + 0x1EC4) = 0; }
    }
    if (mode > 0) __dl__FPv(self);
}

extern "C" void __dt__80067670(u8* self) {
    u8 owns = *(u8*)((char*)self + 0x1ECC);
    if (owns == 0) {
        void* buf = *(void**)((char*)self + 0x1EC4);
        if (buf != 0) { __dla__FPv(buf); *(u32*)((char*)self + 0x1EC4) = 0; }
    }
    *(u32*)((char*)self + 0x1EC8) = 0;
}

// ============================================================
// func_800676F8 (0x658) - initializer (stub)
// ============================================================
extern "C" void func_800676F8(u8* self) {
    memset((char*)self + 4, 0, 0x1E78);
    *(u32*)self |= 1;
    *(float*)((char*)self + 0x1ED0) = 0.0f;
}

// ============================================================
// Named wrappers for -2 targets
// ============================================================

bool testResInfoFlag(u32 flags) {
    return (lbl_eu_80663E28 & flags) != 0;
}

bool testWordFlag(const u32* p, u32 mask) {
    return (*p & mask) != 0;
}

extern "C" void clearWordFlag(u32* self, u32 mask) {
    *self &= ~mask;
}

extern "C" char* getEntryPtr(char* base, int a, int b) {
    return base + (a + b + 0x59) * 0x3C + 4;
}

extern "C" char* getEntryPtrGrid(char* self, int a, int b) {
    return self + (b + a * 11 + 12) * 60 + 4;
}

extern "C" u32 getResEntryId(int unused, const ResEntry* p) {
    return (p != 0) ? p->id : 0;
}

extern "C" int getResMaxCount(u8* self) { return 0x200; }

extern "C" bool isResFlag0() { return false; }
extern "C" bool isResFlag1() { return false; }
extern "C" bool isResFlag2() { return false; }
extern "C" bool isResFlag3() { return false; }
extern "C" bool isResFlag4() { return false; }
extern "C" void resNoop() {}

extern "C" u8* ResContainer::findResEntry(u32 id, u32* outIndex, u32* outValue) {
    ResEntry* entry = table;
    u32 index = 0x59;
    *outIndex = 0;
    *outValue = (u32)-1;
    if (id == 0) return 0;
    for (u32 i = 0; i < 0x28; i++, index++, entry++) {
        if (entry->id == id) {
            *outIndex = index;
            *outValue = entry->value;
            return entry->data;
        }
    }
    return 0;
}