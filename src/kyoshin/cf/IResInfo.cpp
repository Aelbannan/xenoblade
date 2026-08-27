// Auto-scaffolded catalog TU for kyoshin/cf/IResInfo
// Replace stubs with high-level C/C++ during decomp.

#include <string.h>
#include <new>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// ============================================================
// External globals
// ============================================================
extern "C" {
    extern u32 lbl_eu_80663E28;
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
    void getEventHalfwordPair__Q22cf13CfGameManagerFv(u16*, u16*);
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
    void func_801BFE8C(u32, u32, u32);
    void func_804CC1BC(void*, void*);
    void func_804CC1D8(void*, void*);
    s32 func_800B1C40();
    u32 func_80061FFC(void);
    bool processEventQueueB__Q22cf13CfGameManagerFv(void);
    void cancel__11CDeviceFileFP11CFileHandle(void*);
    void waitForDrawDone__9CDeviceVIFv(void);
    void* allocate_array__Q23mtl10MemManagerFUlUl(u32, u32);
    void deallocate__Q23mtl10MemManagerFPv(void*);
    void __dla__FPv(void*);
    void __dl__FPv(void*);
    void CfRes_orBits_649B4(void*, int);
    void func_800A9CD0(void);
    void* func_80066E7C(ResInfoEntry*, u32);
    void* func_80066CF8(ResInfoEntry*);
    void* func_80066DAC(ResInfoEntry*, u32*);
    int func_800A8C90(void);
}

// Main per-entry update tick (defined below).
bool func_80066788(ResInfoEntry* self, bool paramLoad, bool paramFade, bool paramCancel);

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
        getEventHalfwordPair__Q22cf13CfGameManagerFv(&a, &b);
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
// func_800661A8 (0xC4) - range check: builds a per-type limit mask, then
// returns value > mask (unsigned). The retail xor/cntlzw/slw/srwi tail is
// MWCC's branchless expansion of that unsigned comparison.
// Leading unused int keeps the retail free-function ABI (entry record in r4).
// Types 7..10 have a zero limit, so they fall through to the same false
// result; keeping them as labeled cases plus a real `default: return false`
// preserves the dense 0..10 switch range MWCC needs to emit the retail jump
// table (Wii/1.1 lowers cheaper switches to a compare chain).
extern "C" bool func_800661A8(int unused, ResInfoEntry* entry, u32 value, u32 param) {
    u32 slot = (param >> 20) & 0x7F;
    u32 mask = 0;
    u8 type = entry->field_0x33;
    switch (type) {
    case 0: mask = func_800A86AC(slot, type - 1, 0); break;
    case 1: mask = 0x10000; break;
    case 2: mask = 0x32000; break;
    case 3: mask = 0xC8000; break;
    case 4: mask = 0xC800; break;
    case 5: mask = 0x62800; break;
    case 6: mask = 0x99000; break;
    default: return false;
    case 7: case 8: case 9: case 10: break;
    }
    if (mask == 0) return false;
    return value > mask;
}

// ============================================================
// func_80066290 (0xE4)
// ============================================================
// Entry record arrives in r4 (retail hidden first arg in r3). Type 10 caches
// the resource base in field_0x10; type 9 publishes it to the sound manager.
// Computing slot (= idx+2) before the virtual call lets MWCC reuse the idx
// register (r5) for the call's 0 argument, matching retail allocation.
extern "C" void func_80066290(int unused, ResInfoEntry* self) {
    u8 type = self->field_0x33;
    s16 idx = self->field_0x34;
    if (type == 10) {
        if (self->field_0x10 == 0) {
            void* r = self->field_0x2C->getResourceBase(self, 0);
            if (r != 0 && lbl_eu_8065FC18 != 0) {
                self->field_0x10 = (u32)r;
                func_804CC1BC(lbl_eu_8065FC18, r);
            }
        }
    } else if (type == 9 && idx >= 0) {
        int slot = idx + 2;
        void* r = self->field_0x2C->getResourceBase(self, 0);
        func_801BFA08(slot, r, self->field_0x18, 0x62800);
        func_801BFA88(slot, 3, 0, 0);
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
// Per-record state machine tick. Drives the fade counter at +0x36 through
// its load/unload thresholds, cancels in-flight device I/O when the lookup
// object reports it, and releases the record's data buffer once the fade
// finishes. paramLoad/paramFade/paramCancel select which caller policy
// applies; when both load and cancel are false they default to true.
bool func_80066788(ResInfoEntry* self, bool paramLoad, bool paramFade, bool paramCancel) {
    if (paramLoad == false && paramCancel == false) {
        paramLoad = true;
        paramFade = true;
    }

    // Bit set while a system-wide clear is active (raises the unload floor).
    bool hasClear = false;
    if (func_800B1C40()) {
        if (lbl_eu_80663E28 & 0x10) {
            hasClear = true;
        }
    }

    bool didWork = false;

    u32* data = self->data;
    u32 gbit6 = (lbl_eu_80663E28 >> 6) & 1;
    if (data != 0) {
        if (self->field_0x00 & 0x80) {
            u32 flags = self->field_0x2C->getFlags();
            if (flags & 0x10000) {
                // Streaming path: probe first, optionally cancel pending IO.
                if (self->field_0x2C->vfunc0B(self) != 0) {
                    if (paramCancel) {
                        CDeviceFile::cancel(self->field_0x28);
                        self->field_0x28 = 0;
                    } else {
                        return false;
                    }
                }
                CDeviceVI::waitForDrawDone();
                if (func_800A8BD8(self->data)) {
                    func_800A8C68(self->data);
                } else {
                    if (func_800A9024(self->data)) {
                        func_800A9344(self->data, 1);
                    } else if (self->data != 0) {
                        deallocate__Q23mtl10MemManagerFPv(self->data);
                        self->data = 0;
                    }
                }
                didWork = true;
            } else {
                if (self->field_0x2C->vfunc0B(self) != 0) {
                    CDeviceFile::cancel(self->field_0x28);
                    self->field_0x28 = 0;
                    if (func_800A8BD8(self->data)) {
                        func_800A8C68(self->data);
                    } else {
                        if (func_800A9024(self->data)) {
                            func_800A9344(self->data, 1);
                        } else if (self->data != 0) {
                            deallocate__Q23mtl10MemManagerFPv(self->data);
                            self->data = 0;
                        }
                    }
                    didWork = true;
                }
                if (paramLoad) {
                    // Fade start: pick an initial counter value from the global
                    // mode bits and the entry flags.
                    int st = func_800A8C90();
                    // Global mode word is read twice (retail keeps two loads).
                    u32 f24a = *(volatile u32*)&lbl_eu_80663E24;
                    int mode = 1;
                    u32 f24b = *(volatile u32*)&lbl_eu_80663E24;
                    if ((((f24a & 0x2000000) | (f24a & 0x400)) == 0) && ((f24b & 0x40000) == 0)) {
                        mode = 0;
                    }
                    if (self->field_0x0C == 0) {
                        self->field_0x0C = (u32)self->data;
                        if (mode != 0) {
                            if (lbl_eu_8065FC18 != 0 && self->field_0x10 != 0) {
                                func_800B79A4((void*)self->field_0x10);
                            }
                            self->field_0x36 = 2;
                        } else if (self->field_0x00 & 0x200) {
                            self->field_0x36 = 5;
                        } else if (hasClear) {
                            self->field_0x36 = 3;
                        } else if (paramFade) {
                            self->field_0x36 = 0xf;
                        } else {
                            self->field_0x36 = 0x96;
                        }
                    } else {
                        // Fade running: clamp the counter by priority, then step.
                        if (hasClear) {
                            if (self->field_0x36 > 3) {
                                self->field_0x36 = 3;
                            }
                        } else if (gbit6 != 0) {
                            if (self->field_0x36 > 5) {
                                self->field_0x36 = 5;
                            }
                        }
                        if (st == 0) {
                            if (self->field_0x36 > 3) {
                                self->field_0x36 = 3;
                            }
                        } else if ((u32)st < 1) {
                            if (self->field_0x36 > 0xf) {
                                self->field_0x36 = 0xf;
                            }
                        } else if ((u32)st < 2) {
                            if (self->field_0x36 > 0x1e) {
                                self->field_0x36 = 0x1e;
                            }
                        }
                        s16 v = self->field_0x36 - 1;
                        self->field_0x36 = v;
                        if (v == 2) {
                            // Fade threshold reached: unregister from the shared
                            // list and notify the audio grid for ids 0x59..0x60.
                            if (lbl_eu_8065FC18 != 0 && self->field_0x10 != 0) {
                                func_800B79A4((void*)self->field_0x10);
                            }
                            // Keep the raw id live in r3: retail computes
                            // id+0x10000 into a second register.
                            u32 id = self->field_0x30;
                            u32 n = id + 0x10000;
                            if ((u16)(n - 0x59) <= 7) {
                                func_800B7A18();
                            }
                            if (self->field_0x00 & 0x800) {
                                func_801BFE8C(self->field_0x32 + 5, -1, 0);
                            }
                        } else if (v <= 0) {
                            // Fade finished: release the sound voice and buffer.
                            CDeviceVI::waitForDrawDone();
                            if (self->field_0x00 & 0x800) {
                                func_801BFA64(self->field_0x32 + 5);
                                self->field_0x00 &= ~0x800;
                            }
                            func_800A8C68((void*)self->field_0x0C);
                            didWork = true;
                        }
                    }
                } else {
                    CDeviceVI::waitForDrawDone();
                    if (self->field_0x00 & 0x800) {
                        func_801BFA64(self->field_0x32 + 5);
                        self->field_0x00 &= ~0x800;
                    }
                    func_800A8C68((void*)self->data);
                    didWork = true;
                }
            }
        }
    } else {
        // No data attached: just drop any cached resource registration.
        if (self->field_0x10 != 0 && lbl_eu_8065FC18 != 0) {
            func_800B79A4((void*)self->field_0x10);
            func_804CC1D8(lbl_eu_8065FC18, (void*)self->field_0x10);
            self->field_0x10 = 0;
        }
    }

    if (didWork) {
        // Work was done this tick: clear the global reload bit and fully
        // reset the record back to its empty state.
        lbl_eu_80663E28 &= ~0x40;
        if (lbl_eu_8065FC18 != 0 && self->field_0x10 != 0) {
            func_800B79A4((void*)self->field_0x10);
            func_804CC1D8(lbl_eu_8065FC18, (void*)self->field_0x10);
            self->field_0x10 = 0;
        }
        u32 id = self->field_0x30;
        u32 n = id + 0x10000;
        if ((u16)(n - 0x59) <= 7) {
            func_800B7A18();
        }
        self->data = 0;
        self->field_0x1C = 0;
        self->field_0x0C = 0;
        self->field_0x36 = 0;
        self->field_0x00 = 0;
        self->field_0x38 = 0;
        self->field_0x3A = 0;
        self->field_0x04 = 0;
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
// Retail reads the global flag word twice (bit 0x20 gate, then the rest),
// so the volatile alias keeps both loads distinct.
extern "C" bool func_800686E4(ResInfoContainer* self, bool param) {
    // Declaration order mirrors retail callee-saved coloring:
    // cond=r31, flag25=r30, res=r29, target=r28, base=r27, any=r26.
    u32 fGate = *(volatile u32*)&lbl_eu_80663E24;
    u32 flags = *(volatile u32*)&lbl_eu_80663E24;

    bool cond;
    bool flag25;
    float minVal;
    void* res;
    ResGridEntry* target;
    ResGridEntry* base;
    bool any = false;
    int i;

    cond = true;
    if (((fGate & 0x20) == 0)
     && (((flags & 0x2000000) | (flags & 0x400)) == 0)) {
        cond = false;
    }
    flag25 = (flags & 0x40000) != 0;

    if (processEventQueueB__Q22cf13CfGameManagerFv()) return false;
    if (flag25) return false;

    minVal = lbl_eu_80666200;
    if (cond || flag25) {
        // Shared-store form: MWCC emits one stfs for both arms (retail).
        self->field_0x1ED0 = param ? lbl_eu_80666204 : lbl_eu_80666208;
    } else {
        // flag25 is provably 0 here (early-returned above); the redundant
        // test mirrors the retail `beq cr1` reusing the earlier compare flag.
        float limit = lbl_eu_80666200;
        self->field_0x1ED0 -= lbl_eu_8066620C;
        if (self->field_0x1ED0 < limit) {
            self->field_0x1ED0 = limit;
        }
    }

    minVal = lbl_eu_80666200;
    base = self->grid;                                  // 0x14DC
    target = (ResGridEntry*)&self->grid[0].field_0x04;  // 0x14E0
    for (i = 0x59; i < 0x81; i++, base++, target++) {
        res = base->lookup->getResourceBase(target, 0);
        if (res != 0 && func_800A8BD8(res)) {
            bool unload;
            if (i >= 0x61) {
                unload = (self->field_0x1ED0 <= minVal)
                      && (((ResGridEntryEx*)base)->field_0x3C == 0);
            } else if (param) {
                unload = ((ResGridEntryEx*)base)->field_0x3E == 0;
            } else {
                unload = ((ResGridEntryEx*)base)->field_0x3C == 0;
            }
            if (unload) {
                if (cond) {
                    if ((base->field_0x04 & 0x200) != 0
                     && base->handle == (CFileHandle*)lbl_eu_80663E30) {
                        continue;
                    }
                }
                if (flag25) {
                    if ((base->field_0x04 & 0x400) != 0
                     && base->handle == (CFileHandle*)lbl_eu_80663E30) {
                        continue;
                    }
                }
                if (res != 0) {
                    if (func_80066788((ResInfoEntry*)target, true, param, false)) {
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
    ResEntry* entry = (ResEntry*)((u8*)self + start * 0x3C);
    int i;
    // Counted for-loop with live-out index: MWCC emits subf/mtctr guard +
    // bdnz (retail). Pointer IV advances first in the header.
    for (i = start; i < end; entry++, i++) {
        if (id == entry->id) {
            goto found;
        }
    }
    i = -1;
found:
    // -1 (not found) fails the unsigned <= 0x81 test; a found index must be
    // within the grid to be addressable.
    int flag = 0;
    if ((u32)i <= 0x81) {
        flag = 1;
    }
    if (flag != 0) {
        return (u8*)self + i * 0x3C + 4;
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
// Zeroes the overlapping s16 state words that trail every gridLow record
// (they physically overlay the following record's first bytes), then walks
// the 0x16BC grid resolving each entry's resource base and clearing its own
// trailing state words.
extern "C" void func_80067DB4(u8* self) {
    // Zero the trailing s16 state words of each gridLow record (they
    // physically overlay the following record's first bytes). Written as an
    // offset loop so MWCC unrolls to r3-relative halfword stores.
    for (int k = 0; k < 8; k++) {
        *(s16*)(self + 0x1518 + k * 0x3C) = 0;
        *(s16*)(self + 0x151A + k * 0x3C) = 0;
    }

    ResGridEntry* target = (ResGridEntry*)(self + 0x16C0);
    ResGridEntry* base = (ResGridEntry*)(self + 0x16BC);
    for (int i = 0x61; i < 0x81; i++) {
        base->lookup->getResourceBase(target, 0);
        ((ResGridEntryEx*)base)->field_0x3C = 0;
        ((ResGridEntryEx*)base)->field_0x3E = 0;
        base++;
        target++;
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
            func_80066788((ResInfoEntry*)target, 0, 0, param);
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
            func_80066788((ResInfoEntry*)target, 0, 0, param);
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
                func_80066788((ResInfoEntry*)target, 0, 0, true);
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
    // Walks the 0x14DC grid (indices 0x59..0x80); entries whose resolved
    // base is a loaded MCA are force-reloaded.
    ResGridEntry* target = (ResGridEntry*)(self + 0x14E0);
    ResGridEntry* base = (ResGridEntry*)(self + 0x14DC);
    for (int i = 0x59; i < 0x81; i++) {
        void* r = base->lookup->getResourceBase(target, 0);
        if (r != 0) {
            if (func_800A8BD8(r)) {
                func_80066788((ResInfoEntry*)target, 1, 1, 0);
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
                        func_80066788((ResInfoEntry*)target, 0, 0, 1);
                    } else if (!hasFlag2) {
                        u32 cat = base->id >> 27;
                        if (!(flagResult && (cat == 8 || cat == 11))) {
                            func_80066788((ResInfoEntry*)target, 0, 0, 1);
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
                        func_80066788((ResInfoEntry*)target, 0, 0, 1);
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
// Constructor: zeroes the header flag word and the 130-record entry
// table (0x3C stride starting at +0x04), installs the per-category
// resource vtables into the scratch slots, resets the embedded reslist
// ring at 0x1EB4, then allocates the node pool (130 x 0xC) and clears
// each node's next link.
// Record 0 is spelled out so its stores stay r3-relative (retail peels
// it); the rest runs as a pointer walk that MWCC unrolls x8.
// ============================================================
extern "C" void __ct__80066F9C(ResCtorLayout* self) {
    self->flags = 0;
    self->entries[0].field_0x00 = 0;
    self->entries[0].field_0x04 = 0;
    self->entries[0].field_0x08 = 0;
    self->entries[0].field_0x10 = 0;
    self->entries[0].field_0x18 = 0;
    self->entries[0].field_0x1C = 0;
    self->entries[0].field_0x24 = 0;
    self->entries[0].field_0x28 = 0;
    self->entries[0].field_0x34 = 0;
    self->entries[0].field_0x38 = 0;
    self->entries[0].field_0x3A = 0;

    // Scratch lookup slots: each category's resource-object vtable.
    ResCtorEntry* entry = &self->entries[1];
    ResCtorEntry* end = &self->entries[0x82];
    while (entry < end) {
        entry->field_0x00 = 0;
        entry->field_0x04 = 0;
        entry->field_0x08 = 0;
        entry->field_0x10 = 0;
        entry->field_0x18 = 0;
        entry->field_0x1C = 0;
        entry->field_0x24 = 0;
        entry->field_0x28 = 0;
        entry->field_0x34 = 0;
        entry->field_0x38 = 0;
        entry->field_0x3A = 0;
        entry++;
    }

    self->slots[0] = lbl_eu_80526D70;
    self->slots[1] = lbl_eu_80526D10;
    self->slots[2] = lbl_eu_80526CB0;
    self->slots[3] = lbl_eu_80526C50;
    self->slots[4] = lbl_eu_80526AD0;
    self->slots[5] = lbl_eu_80526A70;
    self->slots[6] = lbl_eu_80526A10;
    self->slots[7] = lbl_eu_80526A10;
    self->slots[8] = lbl_eu_805269AC;
    self->slots[9] = lbl_eu_80526944;
    self->slots[10] = lbl_eu_80526BF0;
    self->slots[11] = lbl_eu_80526B30;
    self->slots[12] = lbl_eu_80526938;

    self->mList = NULL;
    self->mCapacity = 0;
    self->mOwnsList = 0;

    // Close the embedded reslist ring on its sentinel.
    self->mStartNodePtr = &self->mStartNode;
    self->mStartNode.next = &self->mStartNode;
    self->mStartNode.prev = &self->mStartNode;

    // Retail keeps both vtable stores (reslist view, then the raw one).
    self->vtable = lbl_eu_80526938;
    self->vtable = lbl_eu_80526920;

    u32 heapId = func_80061FFC();
    self->mList = (ResListUSNode*)allocate_array__Q23mtl10MemManagerFUlUl(0x618, heapId);
    for (int i = 0; i < 0x82; i++) {
        self->mList[i].next = NULL;
    }
    self->mCapacity = 0x82;
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
        // Walk the node chain nulling each next pointer (nodes are
        // pool-allocated, not freed here), then reset the sentinel.
        ResListUSNode* node = self->mHead->next;
        while (node != self->mHead) {
            ResListUSNode* cur = node;
            node = cur->next;
            cur->next = NULL;
        }
        self->mHead->next = self->mHead;
        self->mHead->prev = self->mHead;
        if (self->mOwnsList == 0 && self->mList != NULL) {
            __dla__FPv(self->mList);
            self->mList = NULL;
        }
        if (mode > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// reslist<unsigned short> deleting destructor (retail __dt__reslist_unsigned_short).
void* __dt__reslist_unsigned_short(ResListUS* self, int mode) {
    // Doubled null check mirrors retail: MWCC keeps the dead second beq.
    // NOTE residual: retail allocates the loop-carried node pointer in the
    // register mHead was loaded into (r4); our toolchain consistently colors
    // it r5 regardless of source shape (see session notes) - 7 pure reg-swaps.
    if (self != NULL) {
        if (self != NULL) {
            self->vtable = lbl_eu_80526938;
            ResListUSNode* cur = self->mHead->next;
            while (cur != self->mHead) {
                ResListUSNode* prev = cur;
                cur = cur->next;
                prev->next = NULL;
            }
            self->mHead->next = self->mHead;
            self->mHead->prev = self->mHead;
            if (self->mOwnsList == 0 && self->mList != NULL) {
                __dla__FPv(self->mList);
                self->mList = NULL;
            }
        }
        if (mode > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Deleting destructor of the resource container: tears down the embedded
// reslist<u32> ring at 0x1EB4 (nodes relinked to the sentinel, slot array
// freed unless ownership was transferred), zeroes the capacity, then runs
// the same teardown for the reslist<unsigned short> view at +0x1EB0 (vtable
// set to lbl_eu_80526938). Frees the container itself when mode > 0.
// Shared teardown body (mirrors __dt__80067670); expanded inline in the
// deleting destructor below.
static void ResStorage_ClearFirstList(ResInfoStorage* obj) {
    ResInfoListNode* tmp;
    ResInfoListNode* head = obj->mStartNodePtr;
    ResInfoListNode* node = head->next;
    while (node != obj->mStartNodePtr) {
        tmp = node;
        node = tmp->next;
        tmp->next = 0;
    }
    obj->mStartNodePtr->next = obj->mStartNodePtr;
    obj->mStartNodePtr->prev = obj->mStartNodePtr;
    if (obj->mOwnsList == 0 && obj->mList != 0) {
        __dla__FPv(obj->mList);
        obj->mList = 0;
    }
}

void* __dt__8006754C(u8* self, int mode) {
    ResInfoStorage* obj = (ResInfoStorage*)self;
    if (obj != 0) {
        ResStorage_ClearFirstList(obj);

        ResListUS* rl = (ResListUS*)(obj->_00 + 0x1EB0);
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
// func_800676F8 (0x658) - initializer
// ============================================================
// Full initializer: bulk-clears everything after the flag word, sets bit 0,
// resets the fade counter, re-closes the embedded reslist ring at 0x1EB4,
// then fills the three 0x3C-stride record tables.
//
// Session notes (matching residual, best 362/1644B vs retail 1624B):
// - The 0x2D0 fill MUST be two ranges (n<0x48 counted + n<0x50 unrolled).
//   A single n<0x50 loop makes MWCC emit ctr=10 with NO 8-record tail block
//   (-364B); do-while forms suppress the unroll entirely (addic. shape).
// - The flags/counter/ring-clear region is schedule-sensitive: the explicit
//   `u32 flags = self->flags | 1; counter = ...; self->flags = flags;`
//   order reproduces retail's lfs/stfs placement; plain `|=` hoists the
//   head load too early.
// - Remaining residual: global GPR coloring cascade - our allocator splits
//   the zero constant across r9/r11/r27/r29/r31 where retail unifies it in
//   r0, and maps retail's per-category address regs one-to-many; plus ~5
//   insns of size drift around the unrolled 8-record tail. Removing the
//   cached `u32* slots` local (r31-relative addressing) frees one callee-
//   saved reg but shifts ALL colors (_savegpr_19) and scores worse (383).
//   Likely needs whole-function allocation alignment, not a local fix.
extern "C" void func_800676F8(ResInfoWork* self) {
    memset(&self->_04, 0, 0x1E78);
    u32 flags = self->flags | 1;
    self->counter = lbl_eu_80666200;
    self->flags = flags;

    // Reset the embedded reslist ring: unlink every node, then close the
    // sentinel back on itself.
    ResInfoListNode* cur = self->mStartNodePtr->next;
    while (cur != self->mStartNodePtr) {
        ResInfoListNode* node = cur;
        cur = cur->next;
        node->next = NULL;
    }
    self->mStartNodePtr->next = self->mStartNodePtr;
    self->mStartNodePtr->prev = self->mStartNodePtr;

    // Scratch lookup slots baked into the record tables.
    u32* slots = self->slots;

    // Small type table at 0x78: sound categories 2..11, each bound to its
    // manager slot. Statement order follows the retail store sequence.
    ResInitEntry* pre = self->preTable;
    pre[0].field_0x30 = (ResInfoListNode*)&slots[0];
    pre[0].field_0x36 = 2;
    pre[0].field_0x37 = 0;
    pre[0].field_0x34 = 2;
    pre[1].field_0x30 = (ResInfoListNode*)&slots[1];
    pre[1].field_0x36 = 3;
    pre[1].field_0x37 = 0;
    pre[1].field_0x34 = 3;
    pre[2].field_0x30 = (ResInfoListNode*)&slots[2];
    pre[2].field_0x36 = 4;
    pre[2].field_0x37 = 0;
    pre[2].field_0x34 = 4;
    pre[3].field_0x30 = (ResInfoListNode*)&slots[3];
    pre[3].field_0x36 = 5;
    pre[3].field_0x37 = 0;
    pre[3].field_0x34 = 5;
    pre[4].field_0x30 = (ResInfoListNode*)&slots[4];
    pre[4].field_0x36 = 6;
    pre[4].field_0x37 = 0;
    pre[4].field_0x34 = 6;
    pre[6].field_0x30 = (ResInfoListNode*)&slots[5];
    pre[6].field_0x36 = 8;
    pre[6].field_0x37 = 0;
    pre[6].field_0x34 = 8;
    pre[7].field_0x30 = (ResInfoListNode*)&slots[10];
    pre[7].field_0x36 = 9;
    pre[7].field_0x37 = 0;
    pre[7].field_0x34 = 9;
    pre[8].field_0x30 = (ResInfoListNode*)&slots[11];
    pre[8].field_0x36 = 10;
    pre[8].field_0x37 = 0;
    pre[8].field_0x34 = 10;
    pre[5].field_0x30 = (ResInfoListNode*)&slots[11];
    pre[5].field_0x36 = 7;
    pre[5].field_0x37 = 0;
    pre[5].field_0x34 = 7;
    pre[9].field_0x30 = (ResInfoListNode*)&slots[12];
    pre[9].field_0x36 = 11;
    pre[9].field_0x37 = 0;
    pre[9].field_0x34 = 11;

    // 0x2D0 table: records forming an 11-wide grid (logical ids 0xC..0x5B).
    // field_0x36/0x37 decompose the linear index n into row/col. Written as
    // two ranges (0..0x47 counted, 0x48..0x4F fully unrolled by MWCC); the
    // last 3 records physically overlay gridLow[0..2], which the explicit
    // block below re-initializes (retail does the same double write).
    ResInitEntry* e = self->entries;
    int n = 0;
    for (; n < 0x48; n++, e++) {
        e->field_0x30 = (ResInfoListNode*)&slots[6];
        e->field_0x36 = n / 11;
        e->field_0x37 = n % 11;
        e->field_0x34 = (s16)(0xC + n);
    }
    for (; n < 0x50; n++, e++) {
        e->field_0x30 = (ResInfoListNode*)&slots[6];
        e->field_0x36 = n / 11;
        e->field_0x37 = n % 11;
        e->field_0x34 = (s16)(0xC + n);
    }

    // Low grid: ids 0x59..0x60.
    ResInitEntry* g = self->gridLow;
    g[0].field_0x30 = (ResInfoListNode*)&slots[8];
    g[0].field_0x36 = 0;
    g[0].field_0x37 = 0;
    g[0].field_0x34 = 0x59;
    g[1].field_0x30 = (ResInfoListNode*)&slots[8];
    g[1].field_0x36 = 0;
    g[1].field_0x37 = 0;
    g[1].field_0x34 = 0x5a;
    g[2].field_0x30 = (ResInfoListNode*)&slots[8];
    g[2].field_0x36 = 0;
    g[2].field_0x37 = 0;
    g[2].field_0x34 = 0x5b;
    g[3].field_0x30 = (ResInfoListNode*)&slots[8];
    g[3].field_0x36 = 0;
    g[3].field_0x37 = 0;
    g[3].field_0x34 = 0x5c;
    g[4].field_0x30 = (ResInfoListNode*)&slots[8];
    g[4].field_0x36 = 0;
    g[4].field_0x37 = 0;
    g[4].field_0x34 = 0x5d;
    g[5].field_0x30 = (ResInfoListNode*)&slots[8];
    g[5].field_0x36 = 0;
    g[5].field_0x37 = 0;
    g[5].field_0x34 = 0x5e;
    g[6].field_0x30 = (ResInfoListNode*)&slots[8];
    g[6].field_0x36 = 0;
    g[6].field_0x37 = 0;
    g[6].field_0x34 = 0x5f;
    g[7].field_0x30 = (ResInfoListNode*)&slots[8];
    g[7].field_0x36 = 0;
    g[7].field_0x37 = 0;
    g[7].field_0x34 = 0x60;

    // High grid: ids 0x61..0x80, lookup slot B; columns continue at 8.
    e = self->gridHigh;
    int col = 8;
    for (int n = 0x61; n < 0x81; n++, col++, e++) {
        e->field_0x30 = (ResInfoListNode*)&slots[9];
        e->field_0x36 = col;
        e->field_0x37 = 0;
        e->field_0x34 = n;
    }
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