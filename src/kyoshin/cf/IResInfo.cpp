// Auto-scaffolded catalog TU for kyoshin/cf/IResInfo
// Replace stubs with high-level C/C++ during decomp.

#include <string.h>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceVI.hpp"

// ============================================================
// External globals
// ============================================================
extern "C" {
    extern u32 lbl_eu_80663E24;
    extern u32 lbl_eu_80663E28;
    extern u32 lbl_eu_80663E30;
    extern u32 lbl_eu_8065FC18[];
    extern float lbl_eu_80666200;
    extern float lbl_eu_80666204;
    extern float lbl_eu_80666208;
    extern float lbl_eu_8066620C;
    extern char lbl_eu_804FB380[]; // "KYP" string (incomplete array -> lis/addi, not sda21)
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
    u32 func_80062430(u32*, int, u32*);
    int func_800AA600(void*);
    bool func_800A9068(void*);
    bool func_800A8BD8(void*);
    void func_800A8C68(void*);
    bool func_800A9024(void*);
    void func_800A9344(void*, int);
    void func_800B79A4(void*);
    void func_800B7A18(void);
    void func_804CC1BC(void*, void*);
    void func_804CC1D8(void*, void*);
    s32 func_800B1C40();
    u32 func_80061FFC(void);
    bool func_800865E8__Q22cf13CfGameManagerFv(void);
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
    void* func_80066E7C(ResInfoEntry*, u32);
    void* func_80066CF8(ResInfoEntry*);
    void* func_80066DAC(ResInfoEntry*, u32*);
    bool func_80066788(void*, bool, bool, bool);
}

// Named wrappers (forward decls)
extern "C" bool testResInfoFlag(u32 flags);
extern "C" bool testWordFlag(const u32* p, u32 mask);

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
extern "C" int func_8006846C(void* self, int a) { return a * 11 + 13; }
extern "C" int func_800689AC() { return 0x10004; }

// Accessors on the 0x3C-byte resource record. The leading unused int is the
// retail hidden argument: the object arrives in r4 (MWCC free-function ABI),
// leaving r3 free for the return value.
extern "C" void func_800665F4(int, ResInfoEntry* self) {
    u8 b = self->field_0x32;
    if (b < 8) func_801BFA64(b + 5);
}

extern "C" u32 func_80066160(int, ResInfoEntry* self) {
    u8 t = self->field_0x33;
    u32 ret = 0;
    s16 v = self->field_0x34;
    if (t == 10 && v < 3) ret = self->field_0x18;
    return ret;
}

extern "C" u32 func_80066184(int, ResInfoEntry* self) {
    u8 t = self->field_0x33;
    u32 ret = 0;
    s16 v = self->field_0x34;
    if (t == 0 && v < 3) ret = self->field_0x18;
    return ret;
}

extern "C" int func_8006626C(int, ResInfoEntry* self) {
    u8 t = self->field_0x33;
    int v = self->field_0x34;
    if (t == 9 && v >= 0) return func_801BFA64(v + 2);
    return v;
}

extern "C" int func_8006611C(int, ResInfoEntry* self) {
    int ret = 0;
    u8 t = self->field_0x33;
    s16 v = self->field_0x34;
    if (t == 10 && v < 3) ret = func_800A8AAC(v);
    return ret;
}

// ============================================================
// func_80062430 wrapper functions
// ============================================================

extern "C" u32 func_8006638C(int unused, ResInfoEntry* self) {
    u32 result = 0;
    if (self != 0 && self->data != 0) {
        return func_80062430(self->data, 0, &result);
    }
    return 0;
}

extern "C" u32 func_800663D8(int unused, ResInfoEntry* self) {
    u32 result = 0;
    if (self != 0 && self->data != 0) {
        return func_80062430(self->data, 1, &result);
    }
    return 0;
}

extern "C" u32 func_80066424(int unused, ResInfoEntry* self) {
    u32 result = 0;
    if (self != 0) {
        u32* r = self->data;
        if (r != 0) {
            func_80062430(r, 1, &result);
        }
    }
    return result;
}

extern "C" u32 func_8006646C(int unused, ResInfoEntry* self) {
    u32 result = 0;
    if (self != 0 && self->data != 0) {
        return func_80062430(self->data, 4, &result);
    }
    return 0;
}

extern "C" u32 func_800664B8(int unused, ResInfoEntry* self) {
    u32 result = 0;
    if (self != 0 && self->data != 0) {
        func_80062430(self->data, 2, &result);
    }
    return self->field_0x10;
}

extern "C" u32 func_8006650C(int unused, ResInfoEntry* self) {
    u32 result = 0;
    if (self != 0) {
        u32* r = self->data;
        if (r != 0) {
            func_80062430(r, 2, &result);
        }
    }
    return result;
}

extern "C" u32 func_80066554(int unused, ResInfoEntry* self) {
    u32 result = 0;
    if (self != 0 && self->data != 0) {
        return func_80062430(self->data, 3, &result);
    }
    return 0;
}

extern "C" void func_800665A0(int unused, ResInfoEntry* self) {
    CFileHandle* handle = self->field_0x28;
    if (handle != 0) {
        CDeviceFile::cancel(handle);
        self->field_0x04 = 0;
        self->data = 0;
        self->field_0x28 = 0;
        self->field_0x00 = 0;
        self->field_0x24 = 0;
        self->field_0x20 = 0;
    }
}

// ============================================================
// func_80065F24 (0x90)
// ============================================================
extern "C" void func_80065F24(int unused, ResInfoEntry* param) {
    if (param->field_0x10 == 0) {
        u16 a, b;
        func_800832BC__Q22cf13CfGameManagerFv(&a, &b);
        u32 result = (u32)func_80066E7C(param, (b << 10) | ((a << 20) | 0x60000000));
        if (result != 0) {
            u32* fc18 = lbl_eu_8065FC18;
            if (fc18 != 0) {
                param->field_0x10 = result;
                func_804CC1BC(fc18, (void*)result);
            }
        }
    }
    lbl_eu_80663E24 |= 0x1000;
}

// ============================================================
// func_eu_80066940 (0x4C)
// ============================================================
extern "C" void func_eu_80066940(int unused, ResInfoEntry* self) {
    void* result = self->field_0x2C->getResourceBase(self, 0);
    CBdat::func_8003AA78(1, result);
    lbl_eu_80663E28 |= 0x2000;
}

// ============================================================
// func_80065FB4 (0x168) - switch dispatch
// ============================================================
// The entry record arrives in r4 (leading unused int keeps the retail
// free-function ABI, leaving r3 free for the default return 0). Dispatches
// on the entry type: types 0-4 sum an audio-manager slot value with a
// subtype-indexed offset, type 5 reads the bank base, and types 6-10 map
// the entry index (guarded < 3) through per-type accessors.
extern "C" int func_80065FB4(int unused, ResInfoEntry* self, int param) {
    int ret = 0;
    u8 type = self->field_0x33;
    u8 subtype = self->field_0x32;
    s16 v34 = self->field_0x34;

    switch (type) {
    case 0: ret = func_800A85D8(subtype) + func_800A86D8(param, 0); break;
    case 1: ret = func_800A85D8(subtype) + func_800A86D8(param, 1); break;
    case 2: ret = func_800A85D8(subtype) + func_800A86D8(param, 2); break;
    case 3: ret = func_800A85D8(subtype) + func_800A86D8(param, 3); break;
    case 4: ret = func_800A85D8(subtype) + func_800A86D8(param, 4); break;
    case 5: ret = func_800A84FC(subtype); break;
    case 6: if (v34 < 3) ret = func_800A87E0(v34); break;
    case 7: if (v34 < 3) ret = func_800A8704(v34); break;
    case 8: if (v34 < 3) ret = func_800A88C8(v34); break;
    case 9: if (v34 < 3) ret = func_800A89C0(v34); break;
    case 10: if (v34 < 3) ret = func_800A8AAC(v34); break;
    }
    return ret;
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
                u32* fc18 = lbl_eu_8065FC18;
                if (fc18 != 0) {
                    *(u32**)((char*)param + 0x10) = (u32*)r;
                    func_804CC1BC(fc18, r);
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
// Object arrives in r4 (retail hidden first arg in r3). Resolves the base
// via func_80066CF8, caches it in field_0x10 (registered in the FC18 list),
// then publishes the entry to the sound manager (subtype -> slot idx+5) and
// raises bit 11 (0x800).
extern "C" void func_8006660C(int unused, ResInfoEntry* self) {
    void* r = func_80066CF8(self);
    if (self->field_0x10 == 0 && r != 0 && lbl_eu_8065FC18 != 0) {
        self->field_0x10 = (u32)r;
        func_804CC1BC(lbl_eu_8065FC18, r);
    }
    u32 out;
    void* entry = func_80066DAC(self, &out);
    if (!(self->field_0x00 & 0x800) && entry != 0 && self->field_0x32 < 8) {
        int idx = self->field_0x32 + 5;
        func_801BFA64(idx);
        func_801BFA08(idx, entry, out, out);
        void* base = self->field_0x2C->getResourceBase(self, 0);
        func_801BFA88(idx, 5, (int)((char*)base + self->field_0x18),
                      self->field_0x1C - self->field_0x18);
        self->field_0x00 |= 0x800;
    }
}

// ============================================================
// func_80066714 (0x74)
// ============================================================
void func_80066714(ResInfoEntry* self, bool cleanup) {
    u32* fc18 = lbl_eu_8065FC18;
    if (fc18 != 0) {
        if (self->field_0x10 != 0) {
            func_800B79A4((u32*)self->field_0x10);
            if (cleanup) {
                func_804CC1D8(fc18, (u32*)self->field_0x10);
                self->field_0x10 = 0;
            }
        }
    }
}

// ============================================================
// func_80066C74 (0x84)
// ============================================================
void func_80066C74(ResInfoEntry* self, u32* data, u32 size) {
    if (self->field_0x2C->getFlags() & 0x18E) {
        func_80066788(self, 0, 0, 0);
        self->data = data;
        self->field_0x1C = size;
        self->field_0x00 |= 0x80;
    }
}

// ============================================================
// func_80066CF8 (0xB4) - find entry by name
// ============================================================
extern "C" __declspec(noinline) void* func_80066CF8(ResInfoEntry* self) {
    void* base;
    u8* p;
    void* result = 0;
    base = self->field_0x2C->getResourceBase(self, 0);
    if (base != 0) {
        if (strcmp(lbl_eu_804FB380, (const char*)base) == 0) {
            u8* eb = (u8*)base + 0x10;
            p = eb;
            for (u32 i = 0; i < *(u32*)((u8*)base + 0x08); p += 16, i++) {
                // Entry stride is 16 bytes: word 0 is the offset, word 2 (+8) a tag
                // whose value 0xC..0x11 marks a match (unsigned compare after -0xC).
                int val = func_800AA600(p + 8) - 0xC;
                if ((u32)val <= 5) {
                    result = (u8*)base + *(u32*)(eb + i * 16);
                    break;
                }
            }
        }
    }
    return result;
}

// ============================================================
// func_80066DAC (0xD0) - find entry by name, return index
// ============================================================
extern "C" __declspec(noinline) void* func_80066DAC(ResInfoEntry* self, u32* outIdx) {
    *outIdx = 0;
    void* base;
    u8* p;
    void* result = 0;
    base = self->field_0x2C->getResourceBase(self, 0);
    if (base != 0 && strcmp(lbl_eu_804FB380, (const char*)base) == 0) {
        u32* eb = (u32*)((char*)base + 0x10);
        p = (u8*)eb;
        for (u32 i = 0; i < *(u32*)((char*)base + 0x08); p += 0x10, i++) {
            // Entry stride is 16 bytes; word 2 (+8) is a tag whose value
            // 0x12..0x16 marks a match (unsigned compare after -0x12).
            if ((u32)(func_800AA600(p + 8) - 0x12) <= 4) {
                u32 off = eb[i * 4];
                *outIdx = eb[i * 4 + 1];
                result = (char*)base + off;
                break;
            }
        }
    }
    return result;
}

// ============================================================
// func_80066E7C (0x120) - find entry by ID
// ============================================================
// Resolves the resource base through the object's lookup and returns the
// entry whose 8-byte name tag (at base+0x10 + i*0x10 + 8) matches the packed
// token formatted by func_800AA33C. Categories 14/15 short-circuit on the
// vtable+0x40 probe instead of the name table.
extern "C" void* func_80066E7C(ResInfoEntry* self, u32 id) {
    void* base;
    char* p;
    u32 cnt;
    void* result;
    u32* eb;
    u32 cat;
    result = 0;
    base = self->field_0x2C->getResourceBase(self, 0);
    cat = id >> 27;
    if (cat >= 0x0E && cat <= 0x0F) {
        if (self->field_0x2C->vfunc0C(self) != 0) return base;
        return 0;
    }
    if (base != 0 && strcmp(lbl_eu_804FB380, (const char*)base) == 0) {
        ml::FixStr<64> buf(true);
        eb = (u32*)((char*)base + 0x10);
        func_800AA33C(buf, id, 0, 0);
        cnt = *(u32*)((char*)base + 0x08);
        p = (char*)eb;
        for (u32 i = 0; i < cnt; p += 0x10, i++) {
            if (strncmp(buf.mString, p + 8, 8) == 0) {
                u32 off = eb[i * 4];
                result = (char*)base + off;
                break;
            }
        }
    }
    return result;
}

// ============================================================
// func_80066788 (0x4D4) - main update
// ============================================================
extern "C" bool func_80066788(void* self, bool r4, bool r5, bool r6) {
    if (self == 0 && r6 == 0) { r4 = true; r5 = true; }
    
    bool hasClear = false;
    if (func_800B1C40()) {
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
    if (id != 0) {
        ResGridEntry* p;
        ResGridEntry* startEntry = (ResGridEntry*)((u8*)self + start * 0x3C);
        p = startEntry;
        for (int i = start; i < end; p++, i++) {
            if (id == p->id) {
                *outIdx = i;
                *outVal = ((ResGridEntry*)((u8*)self + i * 0x3C))->value;
                ((ResGridEntry*)((u8*)self + i * 0x3C))->field_0x10 = 0;
                ((ResGridEntry*)((u8*)self + i * 0x3C))->field_0x3A = 0;
                return (u8*)self + i * 0x3C + 4;
            }
        }
        p = startEntry;
        for (int i = start; i < end; p++, i++) {
            if (p->id == 0) {
                *outIdx = i;
                *outVal = ((ResGridEntry*)((u8*)self + i * 0x3C))->value;
                ((ResGridEntry*)((u8*)self + i * 0x3C))->field_0x10 = 0;
                ((ResGridEntry*)((u8*)self + i * 0x3C))->field_0x3A = 0;
                return (u8*)self + i * 0x3C + 4;
            }
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
    if (id != 0) {
        // 0x4D entries at self+0x2D0 (logical indices 0xC..0x58). Keep a
        // separate pointer IV so the id load uses +8(r6) (retail shape).
        ResEntry* entries = (ResEntry*)((char*)self + 0x2D0);
        for (int i = 0xC; i < 0x4D + 0xC; entries++, i++) {
            if (id == entries->id) {
                *outIdx = i;
                return (char*)self + i * 0x3C + 4;
            }
        }
    }
    return 0;
}

// ============================================================
// func_8006861C (0x64) - search entries at 0x16BC
// ============================================================
u8* func_8006861C(u8* self, u32 id, u32* outIdx, u32* outVal) {
    *outIdx = 0;
    *outVal = -1;
    if (id != 0) {
        // 0x20-entry table at self+0x16BC (logical indices 0x61..0x80).
        // Pointer IV walks the table; the found path re-derives self+i*0x3C
        // from the index (retail shape: mulli + add).
        ResEntry* entries = (ResEntry*)(self + 0x16BC);
        for (int i = 0x61; i < 0x81; entries++, i++) {
            if (id == entries->id) {
                *outIdx = i;
                *outVal = ((ResEntry*)self)[i].value;
                return ((ResEntry*)self)[i].data;
            }
        }
    }
    return 0;
}

// ============================================================
// func_800686E4 (0x244) - update/resolve
// ============================================================
// Frame tick for the 0x59..0x80 resource grids. Depending on global flags it
// either resets the 0x1ED0 counter (param ? 3.0 : 120.0) or decrements it
// toward 0; then walks both grids, unloading/resetting entries whose state
// flags say they are done, and clearing the resolution probe for the rest.
extern "C" bool func_800686E4(ResInfoContainer* self, bool param) {
    bool any = false;
    bool cond;
    if ((lbl_eu_80663E24 & 0x20) != 0
     || (lbl_eu_80663E24 & 0x2000000) != 0
     || (lbl_eu_80663E24 & 0x400) != 0) {
        cond = true;
    } else {
        cond = false;
    }
    bool flag25 = (lbl_eu_80663E24 & 0x40000) != 0;

    if (func_800865E8__Q22cf13CfGameManagerFv()) return false;
    if (flag25) return false;

    if (cond) {
        self->field_0x1ED0 = param ? lbl_eu_80666204 : lbl_eu_80666208;
    } else if (flag25 == 0) {
        // flag25 is provably 0 here (early-returned above); the redundant
        // test mirrors the retail `beq cr1` reusing the earlier compare flag.
        self->field_0x1ED0 -= lbl_eu_8066620C;
        if (self->field_0x1ED0 < lbl_eu_80666200) {
            self->field_0x1ED0 = lbl_eu_80666200;
        }
    }

    float zero = lbl_eu_80666200;
    ResGridEntry* base = self->grid;        // 0x14DC
    ResGridEntry* target = self->grid + 1;  // 0x14E0
    for (int i = 0x59; i < 0x81; i++) {
        void* r = base->lookup->getResourceBase(target, 0);
        if (r != 0 && func_800A8BD8(r)) {
            bool cond2;
            if (i >= 0x61) {
                cond2 = self->field_0x1ED0 <= zero
                     && ((ResGridEntryEx*)base)->field_0x3C == 0;
            } else {
                cond2 = (param ? ((ResGridEntryEx*)base)->field_0x3E
                               : ((ResGridEntryEx*)base)->field_0x3C) == 0;
            }
            if (cond2) {
                if (cond) {
                    if ((base->field_0x04 & 0x200) != 0
                     && base->handle == (CFileHandle*)(u32)lbl_eu_80663E30) {
                        continue;
                    }
                }
                if (flag25) {
                    if ((base->field_0x04 & 0x400) != 0
                     && base->handle == (CFileHandle*)(u32)lbl_eu_80663E30) {
                        continue;
                    }
                }
                if (r != 0) {
                    if (func_80066788(target, true, param, false)) {
                        any = true;
                    }
                }
            } else {
                if (base->lookup->vfunc0C(target) != 0) {
                    if (base->field_0x10 != 0) {
                        base->field_0x10 = 0;
                    }
                }
            }
        }
        base++;
        target++;
    }
    if (any) {
        CDeviceVI::waitForDrawDone();
    }
    return any;
}

// ============================================================
// func_80068928 (0x68) - search id in [start, end)
// ============================================================
void* func_80068928(u8* self, u32 id, int start, int end) {
    ResEntry* entries = (ResEntry*)((u8*)self + start * 0x3C);
    int i = start;
    while (i < end) {
        if (id == entries->id) goto found;
        i++;
        entries++;
    }
    i = -1;
found:
    // -1 (not found) fails the unsigned <= 0x81 test; a found index must be
    // within the 0x59..0x81 grid to be addressable.
    bool ok = (u32)i <= 0x81;
    return ok ? (u8*)self + i * 0x3C + 4 : 0;
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
void func_80067D38(ResInfoEntry* entry) {
    ResInfoEntry* e = entry;
    for (int i = 0; i < 0x82; i++, e++) {
        if (e->field_0x2C != 0) {
            // Retail passes the record's 0x2C slot to CDeviceFile::cancel
            // (treats it as a file handle here).
            CDeviceFile::cancel((CFileHandle*)e->field_0x2C);
            e->data = 0;
            e->field_0x0C = 0;
            e->field_0x2C = 0;
            e->field_0x04 = 0;
            e->field_0x28 = 0;
            e->field_0x24 = 0;
        }
        e->field_0x18 = 0;
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
// Walks the 0x16BC grid (indices 0x61..0x80): for each entry whose resource
// object resolves a base for the target record, run the main update.
extern "C" void func_80067E78(u8* self, bool param) {
    ResGridEntry* target = (ResGridEntry*)(self + 0x16C0);
    ResGridEntry* base = (ResGridEntry*)(self + 0x16BC);
    for (int i = 0x61; i < 0x81; i++) {
        if (base->lookup->getResourceBase(target, 0) != 0) {
            func_80066788(target, 0, 0, param);
        }
        target++;
        base++;
    }
}

// func_80067F10 (0xD0)
extern "C" void func_80067F10(void* self, bool param) {
    if (func_800A9068(self)) return;
    ResGridEntry* target = (ResGridEntry*)((u8*)self + 0x16C0);
    ResGridEntry* base = (ResGridEntry*)((u8*)self + 0x16BC);
    for (int i = 0x61; i < 0x81; i++, target++, base++) {
        void* r = base->lookup->getResourceBase(target, 0);
        if (r != 0) {
            u32 cat = base->id >> 27;
            bool skip = (cat == 8) || (cat == 11);
            if (skip) continue;
            func_80066788(target, 0, 0, param);
        }
    }
}

// func_80067FE0 (0x98)
// Same grid walk; only entries whose data word carries the 0x30000 flag bits
// are updated.
extern "C" void func_80067FE0(u8* self) {
    ResGridEntry* target = (ResGridEntry*)(self + 0x16C0);
    ResGridEntry* base = (ResGridEntry*)(self + 0x16BC);
    for (int i = 0x61; i < 0x81; i++) {
        if (base->lookup->getResourceBase(target, 0) != 0) {
            if (base->field_0x04 & 0x3000) {
                func_80066788(target, 0, 0, true);
            }
        }
        target++;
        base++;
    }
}

// func_80068078 (0x98)
// Walks the 0x14DC grid (indices 0x59..0x80); entries whose resolved base is
// a loaded MCA are force-reloaded.
extern "C" void func_80068078(u8* self) {
    ResGridEntry* target = (ResGridEntry*)(self + 0x14E0);
    ResGridEntry* base = (ResGridEntry*)(self + 0x14DC);
    for (int i = 0x59; i < 0x81; i++) {
        void* r = base->lookup->getResourceBase(target, 0);
        if (r != 0) {
            if (func_800A8BD8(r)) {
                func_80066788(target, true, true, false);
            }
        }
        target++;
        base++;
    }
}

// func_80068110 (0x144)
// Grid walk over 0x14DC (indices 0x59..0x60). With `param` the update runs
// for entries carrying the 0x200/0x400 bits; without it, entries lacking
// those bits are updated unless the global flagResult (0x40000/0x8000)
// excludes the entry's category (8/11).
extern "C" void func_80068110(u8* self, bool param) {
    CDeviceVI::waitForDrawDone();
    u32 flags24 = lbl_eu_80663E24;
    bool flagResult = (flags24 & 0x40000) | (flags24 & 0x8000);
    ResGridEntry* target = (ResGridEntry*)(self + 0x14E0);
    ResGridEntry* base = (ResGridEntry*)(self + 0x14DC);
    for (int i = 0x59; i < 0x61; i++, target++, base++) {
        void* r = base->lookup->getResourceBase(target, 0);
        if (r != 0 && func_800A8BD8(r)) {
            if (*(s16*)((u8*)base + 0x3E) == 0) {
                if (base->lookup->vfunc0C(target) != 0) {
                    u32 fl = base->field_0x04;
                    bool hasFlag2 = (fl & 0x200) || (fl & 0x400);
                    if (param && hasFlag2) {
                        func_80066788(target, 0, 0, 1);
                    } else if (!hasFlag2) {
                        u32 cat = base->id >> 27;
                        if (!(flagResult && (cat == 8 || cat == 11))) {
                            func_80066788(target, 0, 0, 1);
                        }
                    }
                }
            }
        }
    }
}

// func_80068254 (0x104)
// Grid walk over 0x14DC (indices 0x59..0x80): run the main update for each
// entry whose resolved base is a loaded MCA, not dirty (s16 at +0x3E), and
// resolvable via the vtable+0x40 probe, carrying the 0x200/0x400 bits.
// waitForDrawDone precedes the first update only.
extern "C" void func_80068254(u8* self) {
    ResGridEntry* target = (ResGridEntry*)(self + 0x14E0);
    ResGridEntry* base = (ResGridEntry*)(self + 0x14DC);
    bool waited = false;
    for (int i = 0x59; i < 0x81; i++, target++, base++) {
        void* r = base->lookup->getResourceBase(target, 0);
        if (r != 0 && func_800A8BD8(r)) {
            if (*(s16*)((u8*)base + 0x3E) == 0) {
                if (base->lookup->vfunc0C(target) != 0) {
                    bool hasFlag2 = true;
                    u32 fl = base->field_0x04;
                    if ((fl & 0x200) == 0 && (fl & 0x400) == 0) hasFlag2 = false;
                    if (hasFlag2) {
                        if (!waited) CDeviceVI::waitForDrawDone();
                        waited = true;
                        func_80066788(target, 0, 0, 1);
                    }
                }
            }
        }
    }
}

// ============================================================
// func_80068358 (0xD4) - flag handling
// ============================================================
extern "C" void func_80068358(void* self) {
    bool f40, f10, flag2;
    flag2 = testWordFlag((const u32*)self, 2) != 0;
    f10 = testResInfoFlag(0x10) != 0;
    f40 = testResInfoFlag(0x40) != 0;

    if (func_800B1C40() || (f10 && f40)) {
        flag2 = true;
        CfRes_orBits_649B4(self, 1);
    }

    if (testWordFlag((const u32*)self, 1)) {
        func_800686E4((ResInfoContainer*)self, flag2);
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
// Destructors (reslist<unsigned short> / _reslist_base<unsigned short>)
// ============================================================

// _reslist_base<unsigned short> deleting destructor (retail
// __dt___reslist_base_unsigned_short). Old flat template mangling -> plain
// global function over the ResListUS layout mirror (same shape as the matched
// CUIBattleManager reslist dtors). u16 is trivially destructible, so the ring
// clear only zeroes the node links and the array delete is a bare __dla__.
void* __dt___reslist_base_unsigned_short(ResListUS* self, int mode) {
    if (self != NULL) {
        self->vtable = lbl_eu_80526938;
        {
            ResListUSNode* cur = self->mHead->next;
            while (cur != self->mHead) {
                ResListUSNode* prev = cur;
                cur = cur->next;
                prev->next = NULL;
            }
            self->mHead->next = self->mHead;
            self->mHead->prev = self->mHead;
        }
        if (self->mOwnsList == 0 && self->mList != NULL) {
            delete[] self->mList;
            self->mList = NULL;
        }
    }
    if (mode > 0) {
        delete self;
    }
    return self;
}

// reslist<unsigned short> deleting destructor (retail __dt__reslist_unsigned_short).
void* __dt__reslist_unsigned_short(ResListUS* self, int mode) {
    // Doubled null check mirrors retail: MWCC keeps the dead second beq.
    if (self != NULL) {
        if (self != NULL) {
            self->vtable = lbl_eu_80526938;
            {
                ResListUSNode* cur = self->mHead->next;
                while (cur != self->mHead) {
                    ResListUSNode* prev = cur;
                    cur = cur->next;
                    prev->next = NULL;
                }
                self->mHead->next = self->mHead;
                self->mHead->prev = self->mHead;
            }
            if (self->mOwnsList == 0 && self->mList != NULL) {
                delete[] self->mList;
                self->mList = NULL;
            }
        }
    }
    if (mode > 0) {
        delete self;
    }
    return self;
}

// Deleting destructor of the resource container: tears down the embedded
// reslist<u32> ring at 0x1EB4 (nodes relinked to the sentinel, slot array
// freed unless ownership was transferred), zeroes the capacity, then runs
// the same teardown for the reslist<unsigned short> view at +0x1EB0 (vtable
// set to lbl_eu_80526938). Frees the container itself when mode > 0.
extern "C" void* __dt__8006754C(u8* self, int mode) {
    ResInfoStorage* obj = (ResInfoStorage*)self;
    if (obj != 0) {
        for (ResInfoListNode* cur = obj->mStartNodePtr->next; cur != obj->mStartNodePtr;) {
            ResInfoListNode* node = cur;
            cur = cur->next;
            node->next = 0;
        }
        obj->mStartNodePtr->next = obj->mStartNodePtr;
        obj->mStartNodePtr->prev = obj->mStartNodePtr;
        if (obj->mOwnsList == 0 && obj->mList != 0) {
            __dla__FPv(obj->mList);
            obj->mList = 0;
        }

        ResListUS* rl = (ResListUS*)((u8*)obj + 0x1EB0);
        obj->mCapacity = 0;
        if (rl != 0) {
            if (rl != 0) {
                rl->vtable = lbl_eu_80526938;
                ResListUSNode* cur = rl->mHead->next;
                while (cur != rl->mHead) {
                    ResListUSNode* node = cur;
                    cur = cur->next;
                    node->next = 0;
                }
                rl->mHead->next = rl->mHead;
                rl->mHead->prev = rl->mHead;
                if (rl->mOwnsList == 0 && rl->mList != 0) {
                    __dla__FPv(rl->mList);
                    rl->mList = 0;
                }
            }
        }
        if (mode > 0) {
            __dl__FPv(obj);
        }
    }
    return obj;
}

extern "C" void __dt__80067670(u8* self) {
    ResInfoStorage* obj = (ResInfoStorage*)self;

    // clearList: walk the ring from the sentinel, zeroing each node's mNext
    // link (T is trivially destructible, so there is no per-item teardown),
    // then reset the sentinel to an empty ring.
    ResInfoListNode* cur = obj->mStartNodePtr->next;
    while (cur != obj->mStartNodePtr) {
        ResInfoListNode* node = cur;
        cur = cur->next;
        node->next = 0;
    }
    obj->mStartNodePtr->next = obj->mStartNodePtr;
    obj->mStartNodePtr->prev = obj->mStartNodePtr;

    // destroyList tail: free the slot array unless ownership was transferred.
    if (obj->mOwnsList == 0 && obj->mList != 0) {
        __dla__FPv(obj->mList);
        obj->mList = 0;
    }
    obj->mCapacity = 0;
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

extern "C" __declspec(noinline) bool testResInfoFlag(u32 flags) {
    return (lbl_eu_80663E28 & flags) != 0;
}

extern "C" __declspec(noinline) bool testWordFlag(const u32* p, u32 mask) {
    return (*p & mask) != 0;
}

extern "C" __declspec(noinline) void clearWordFlag(u32* self, u32 mask) {
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