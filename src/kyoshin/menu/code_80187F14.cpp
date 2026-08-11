// Auto-scaffolded catalog TU for kyoshin/menu/code_80187F14
// Sound effect management system for menu UI

#include <string.h>
#include "kyoshin/harness_catalog.hpp"
#include "monolib/util/FixStr.hpp"
#include "monolib/util/MemManager.hpp"

extern "C" {
    extern s32 lbl_eu_80575798[];
    extern s32 lbl_eu_805757BC[];
    extern float lbl_eu_80662490;
    extern float lbl_eu_80662494;
    extern float lbl_eu_80662498;
    extern float lbl_eu_8066249C;
    extern float lbl_eu_806624A0;
    extern u16 lbl_eu_806642E0;
    extern float lbl_eu_806642E4;
    extern s32* lbl_eu_80663E60;
    extern u32 lbl_eu_80663E24;
    extern float lbl_eu_80667A08;
    extern float lbl_eu_80667A0C;
    extern float lbl_eu_80667A10;
    extern float lbl_eu_80667A14;
    extern float lbl_eu_80667A18;
    extern char lbl_eu_80503AB0[];
}

// CLibCri wrappers: retail reloc names are unmangled (C ABI), so C linkage
// is required for the call-site reloc names to match.
extern "C" s32 func_80459A78__7CLibCriFv(s32);
extern "C" s32 func_80459A7C__7CLibCriFv(s32);
extern "C" void func_80459A84__7CLibCriFv(s32, s32);
extern "C" void func_80459A88__7CLibCriFv(s32, float, float, s32);
extern "C" void func_80459A8C__7CLibCriFv(s32);
extern "C" void func_80459A90__7CLibCriFv(s32);
extern "C" void func_80459A94__7CLibCriFv(s32, float);
extern "C" s32 func_80459A9C__7CLibCriFv(s32, void*, s32, float, float, float);
extern "C" s32 func_8045997C__7CLibCriFv(const char*, s32, s32);
extern "C" void getInstance__7CLibCriFv();
extern "C" s32 func_8008585C__Q22cf13CfGameManagerFv();

struct SoundSlot {
    s32 handle;              // 0x00
    char name[0x40];         // 0x04
    s32 nameLen;             // 0x44
    float field_0x48;        // 0x48
    float x;                 // 0x4C
    float y;                 // 0x50
    u16 field_0x54;          // 0x54
    u16 field_0x56;          // 0x56
    u16 status;              // 0x58
    s32 backupHandle;        // 0x5C
    char backupName[0x40];   // 0x60
    s32 backupNameLen;       // 0xA0
    float backupF1;          // 0xA4
    float backupF2;          // 0xA8
    float backupF3;          // 0xAC
    u16 backupU16_1;         // 0xB0
    u16 backupU16_2;         // 0xB2
    u16 backupU16_3;         // 0xB4
};

// --- SIMPLE HELPERS ---
static inline SoundSlot* sptr(s32 i) {
    s32* b = lbl_eu_80663E60;
    SoundSlot* s;
    if (b != nullptr) s = (SoundSlot*)((u8*)b + i * 0xB8);
    else s = nullptr;
    return s;
}

static inline s32 isValid(s32 h) { return h != -1; }

// Param block passed to func_80187F14 (the sound-play dispatcher): the
// formatted sound name, a MEM2 alloc handle, three volumes and two flags.
struct SoundPlayParams {
    ml::FixStr<64>* name;   // 0x00
    u32 handle;             // 0x04
    float vol1;             // 0x08
    float vol2;             // 0x0C
    float vol3;             // 0x10
    u16 field_0x14;         // 0x14
    u16 field_0x16;         // 0x16
};

// --- FORWARD DECLARATIONS ---
// func_801882AC has an unmangled retail name -> C linkage so call sites
// emit the same reloc name as retail.
extern "C" void func_801882AC(SoundSlot*, float, u32);
extern "C" void func_80188488(SoundSlot*, u32, float, float, float);
extern "C" s32 func_801887C8(u32, s32, s32);
extern "C" s32 func_80188B80(s32, s32, float, float, s32);
void func_801889D0(SoundSlot* base);
void func_8018986C(const char* name, float vol);
extern "C" s32 func_80189A04(s32);
extern "C" s32 func_80187F14(SoundSlot* slot, SoundPlayParams* params, s32 flag);

// ============================================================
// MATCHED FUNCTIONS
// ============================================================

float func_801895EC() { return lbl_eu_80662490; }
float func_801896A0() { return lbl_eu_80662498; }

s32* func_80189C34() { return lbl_eu_805757BC; }

void func_80189C70() { lbl_eu_806642E4 = lbl_eu_80667A14; }
void func_80189C7C() { lbl_eu_806642E4 = lbl_eu_80667A18; }

s32 func_80189BF4(s32 index) {
    s32* b = lbl_eu_80663E60;
    SoundSlot* s;
    if (b != nullptr) s = (SoundSlot*)((u8*)b + index * 0xB8);
    else s = nullptr;
    if (s != nullptr) {
        // Boolean-ize (handle == -1) so MWCC emits the addi/cntlzw/srwi idiom.
        return (s->handle == -1) ? 1 : 0;
    }
    return 1;
}

s32 func_8018892C(s32 index) {
    s32* b = lbl_eu_80663E60;
    SoundSlot* s;
    if (b != nullptr) s = (SoundSlot*)((u8*)b + index * 0xB8);
    else s = nullptr;
    s32 result = 0;
    // (u32) compare: 0xFFFFFFFF cannot fit a signed 16-bit immediate, so MWCC
    // emits the addis+cmplwi test the retail uses.
    if (s != nullptr && (u32)s->handle != 0xFFFFFFFF) result = 1;
    return result;
}

// Callers pass a position pointer (r4), a value (r5) and three floats; they
// are forwarded unchanged to func_80459A9C, so the extra params stay live in
// r4/r5/f1-f3 and the slot base lands in r6 (retail allocation).
s32 func_80189C40(s32 index, f32* pos, s32 val, float f1, float f2, float f3) {
    SoundSlot* s;
    s32* b = lbl_eu_80663E60;
    if (b != nullptr) s = (SoundSlot*)((u8*)b + index * 0xB8);
    else s = nullptr;
    if (s == nullptr) return (s32)s;  // 0 - reuse the register so MWCC folds beq+{blr} into beqlr
    return func_80459A9C__7CLibCriFv(s->handle, pos, val, f1, f2, f3);
}

// vol arrives in f1 from the caller and is passed straight through to
// func_801882AC - retail never reloads it (no lfs in the body).
void func_80189424(float vol) {
    s32* b = lbl_eu_80663E60;
    SoundSlot* s;
    if (b != nullptr) s = (SoundSlot*)((u8*)b + 0xB8);
    else s = nullptr;
    if (s != nullptr) func_801882AC(s, vol, 2);
}

void func_80188774(SoundSlot* slot) {
    SoundSlot* s = slot;  // local copy declared before the counter -> r31, counter -> r30 (retail order)
    for (s32 i = 0; i < 5u; i++) {
        // Volume is the global (reloaded every iteration - the call may
        // clobber it, which is why retail keeps the lfs inside the loop).
        func_801882AC(s, lbl_eu_80667A08, 2);
        s = (SoundSlot*)((u8*)s + 0xB8);
    }
}

void func_80188890(s32 active) {
    SoundSlot* s;
    s32 off;
    s32 i;
    i = 0;
    off = 0;
    while (i < 5) {
        s32* b = lbl_eu_80663E60;
        if (b != nullptr) s = (SoundSlot*)((u8*)b + off);
        else s = nullptr;
        if (s != nullptr && (u32)s->handle != 0xFFFFFFFF) {
            getInstance__7CLibCriFv();
            func_80459A84__7CLibCriFv(s->handle, active);
            if (active != 0) s->status = 5;
            else s->status = 2;
        }
        i++;
        off += 0xB8;
    }
}

// vol arrives in f1 from the caller and is passed straight through to
// func_801882AC - retail never reloads it (no lfs before the call).
void func_80189318(s32 clearName, float vol) {
    s32* b = lbl_eu_80663E60;
    SoundSlot* s;
    if (b != nullptr) s = (SoundSlot*)b;
    else s = nullptr;
    if (s != nullptr) func_801882AC(s, vol, 2);
    if (clearName != 0) {
        if (lbl_eu_80575798[8] != 0) {
            ((char*)lbl_eu_80575798)[0] = '\0';
            lbl_eu_80575798[8] = 0;
        }
    }
}

void func_80189390(const char* name) {
    s32* b = lbl_eu_80663E60;
    SoundSlot* s;
    if (b != nullptr) s = (SoundSlot*)b;
    else s = nullptr;
    if (s != nullptr) func_801882AC(s, lbl_eu_80667A08, 2);
    if (name != nullptr) {
        lbl_eu_80575798[8] = strlen(name);
        strcpy((char*)lbl_eu_80575798, name);
    } else {
        ((char*)lbl_eu_80575798)[0] = '\0';
        lbl_eu_80575798[8] = 0;
    }
}

extern "C" void func_80188488(SoundSlot* slot, u32 type, float f1, float f2, float f3) {
    if (!isValid(slot->handle)) return;
    u16 st = slot->status;
    if (st == 4) return;
    if (f3 <= 0.0f || st == 5) {
        func_80459A90__7CLibCriFv(slot->handle);
        slot->x = f1;
        slot->y = f2;
        return;
    }
    float prod = f1 * f2;
    func_80459A88__7CLibCriFv(slot->handle, prod, f3, type);
    slot->x = f1;
    slot->y = f2;
    if (type == 2) {
        slot->handle = -1;
        slot->field_0x54 = 0;
        slot->field_0x56 = 0;
        slot->status = 0;
        slot->field_0x48 = 0.0f;
    }
}

void func_801896A8(s32 index, float f1, float f2) {
    lbl_eu_80662494 = f1;
    if (index != 0) return;
    for (s32 i = 0; i <= 1; i++) {
        float vol;
        if (i >= 0 && i <= 1) vol = lbl_eu_80662490 * lbl_eu_80662494;
        else vol = lbl_eu_80662498;
        SoundSlot* s = sptr(i);
        if (s != nullptr && isValid(s->handle)) func_80188488(s, 0, 0.0f, vol, f2);
    }
}

void func_80189510(float f1) {
    lbl_eu_80662490 = f1;
    for (s32 i = 0; i <= 1; i++) {
        SoundSlot* s = sptr(i);
        if (s != nullptr && (u32)s->handle != 0xFFFFFFFF) {
            float vol;
            volatile s32 inRange = 0;
            if (i >= 0 && i <= 1) {
                inRange = 1;
            }
            if (inRange) vol = lbl_eu_80662490 * lbl_eu_80662494;
            else vol = lbl_eu_80662498;
            SoundSlot* s2 = sptr(i);
            if (s2 != nullptr) {
                func_80188488(s2, 0, s->x, vol, lbl_eu_80667A08);
            }
        }
    }
}

void func_801895F4(float f1) {
    lbl_eu_80662498 = f1;
    for (s32 i = 2; i <= 4; i++) {
        float vol;
        s32 inRange = 0;
        if (i >= 0 && i <= 1) inRange = 1;
        if (inRange) vol = lbl_eu_80662490 * lbl_eu_80662494;
        else vol = lbl_eu_80662498;
        s32* b = lbl_eu_80663E60;
        SoundSlot* s;
        if (b != nullptr) s = (SoundSlot*)((u8*)b + i * 0xB8);
        else s = nullptr;
        if (s != nullptr) func_80188488(s, 0, lbl_eu_80667A0C, vol, lbl_eu_80667A08);
    }
}

extern "C" void func_801882AC(SoundSlot* slot, float vol, u32 type) {
    getInstance__7CLibCriFv();
    if (lbl_eu_80667A08 == vol) {
        getInstance__7CLibCriFv();
        if ((u32)slot->backupHandle != 0xFFFFFFFF) {
            func_80459A7C__7CLibCriFv(slot->backupHandle);
            slot->backupHandle = -1;
            slot->backupU16_1 = 0;
            slot->backupU16_2 = 0;
            slot->backupU16_3 = 0;
            slot->backupF1 = lbl_eu_80667A08;
        }
    }
    if ((u32)slot->handle != 0xFFFFFFFF) {
        slot->backupHandle = -1;
        slot->backupU16_1 = 0;
        slot->backupU16_2 = 0;
        slot->backupU16_3 = 0;
        slot->backupF1 = lbl_eu_80667A08;
        slot->x = lbl_eu_80667A08;
        if (vol <= lbl_eu_80667A08 && type != 1) {
            func_80459A7C__7CLibCriFv(slot->handle);
            slot->handle = -1;
            slot->field_0x54 = 0;
            slot->field_0x56 = 0;
            slot->status = 0;
            slot->field_0x48 = lbl_eu_80667A08;
            return;
        }
        func_80459A88__7CLibCriFv(slot->handle, lbl_eu_80667A08, vol, type);
        if (type == 2) {
            if ((u32)slot->backupHandle != 0xFFFFFFFF) func_80459A7C__7CLibCriFv(slot->backupHandle);
            slot->backupHandle = slot->handle;
            slot->backupNameLen = strlen(slot->name);
            strcpy(slot->backupName, slot->name);
            slot->backupF1 = slot->field_0x48;
            slot->backupF2 = slot->x;
            slot->backupF3 = slot->y;
            slot->backupU16_1 = slot->field_0x54;
            slot->backupU16_2 = slot->field_0x56;
            slot->backupU16_3 = slot->status;
            slot->handle = -1;
            slot->field_0x54 = 0;
            slot->field_0x56 = 0;
            slot->status = 0;
            slot->field_0x48 = lbl_eu_80667A08;
        } else if (type == 1) {
            slot->status = 4;
        }
    }
}

void func_80189C88() {
    for (s32 i = 0; i <= 4; i++) {
        SoundSlot* s = sptr(i);
        if (s != nullptr && isValid(s->handle)) {
            if (i <= 1) {
                if (isValid(s->backupHandle)) {
                    func_80459A7C__7CLibCriFv(s->backupHandle);
                    s->backupHandle = -1;
                    s->backupU16_1 = 0;
                    s->backupU16_2 = 0;
                    s->backupU16_3 = 0;
                    s->backupF1 = 0.0f;
                }
            } else {
                func_801882AC(s, 0.0f, 2);
            }
        }
    }
}

void sinit_80189D68() {
    const char* str = &lbl_eu_80503AB0[0x04];
    s32 len = strlen(str);
    lbl_eu_80575798[8] = len;
    strcpy((char*)lbl_eu_80575798, str);
    len = strlen(str);
    lbl_eu_805757BC[8] = len;
    strcpy((char*)lbl_eu_805757BC, str);
    lbl_eu_806642E0 = 0;
}

extern "C" void func_80188584(SoundSlot* slot) {
    u8* start = (u8*)slot + 0xB8;
    u8* end = (u8*)slot + 0x398;
    for (u8* cur = start; cur < end; cur += 0xB8) {
        cur[0x04] = '\0';
        *(s32*)(cur + 0x44) = 0;
        cur[0x60] = '\0';
        *(s32*)(cur + 0xA0) = 0;
    }
    ((char*)lbl_eu_80575798)[0] = '\0';
    lbl_eu_80575798[8] = 0;
    ((char*)lbl_eu_805757BC)[0] = '\0';
    lbl_eu_805757BC[8] = 0;
    lbl_eu_806642E0 = 0;
    lbl_eu_8066249C = 1.0f;
    lbl_eu_806642E4 = 0.0f;
    lbl_eu_806624A0 = lbl_eu_80667A10;
    slot->handle = -1;
    slot->field_0x54 = 0;
    slot->field_0x56 = 0;
    slot->status = 0;
    slot->field_0x48 = 0.0f;
    slot->backupHandle = -1;
    slot->backupU16_1 = 0;
    slot->backupU16_2 = 0;
    slot->backupU16_3 = 0;
    slot->backupF1 = 0.0f;
    for (s32 i = 1; i <= 4; i++) {
        SoundSlot* s = (SoundSlot*)((u8*)slot + i * 0xB8);
        s->handle = -1;
        s->field_0x54 = 0;
        s->field_0x56 = 0;
        s->status = 0;
        s->field_0x48 = 0.0f;
        s->backupHandle = -1;
        s->backupU16_1 = 0;
        s->backupU16_2 = 0;
        s->backupU16_3 = 0;
        s->backupF1 = 0.0f;
    }
}

// Deleting destructor for the 5-slot sound object (retail symbol is
// address-named, so this is a plain free function rather than a member dtor).
void* __dt__801886EC(SoundSlot* _this, int flags) {
    if (_this != nullptr) {
        SoundSlot* s = _this;
        for (s32 i = 0; i < 5u; i++) {
            func_801882AC(s, lbl_eu_80667A08, 2);
            s = (SoundSlot*)((u8*)s + 0xB8);
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// Per-frame cleanup: drop finished sounds from all 5 slots (handles whose
// CRI voice finished playing), then ramp the master volume toward its
// target and push the new volume to the two BGM slots (0 and 1).
void func_801889D0(SoundSlot* base) {
    float zero = lbl_eu_80667A08;
    SoundSlot* s = base;
    u32 i = 0;
    do {
        // Clear the backup handle once its voice has finished playing.
        if ((s->backupHandle == -1)
                ? 0
                : (getInstance__7CLibCriFv(), !func_80459A78__7CLibCriFv(s->backupHandle))) {
            s->backupHandle = -1;
            s->backupU16_1 = 0;
            s->backupU16_2 = 0;
            s->backupU16_3 = 0;
            s->backupF1 = zero;
        }
        // Clear the main handle once its voice has finished playing.
        if ((s->handle == -1)
                ? 0
                : (getInstance__7CLibCriFv(), !func_80459A78__7CLibCriFv(s->handle))) {
            s->handle = -1;
            s->field_0x54 = 0;
            s->field_0x56 = 0;
            s->status = 0;
            s->field_0x48 = zero;
        }
        i++;
        s++;
    } while (i < 5);

    // Ramp the master volume by the per-frame delta, clamped to the
    // target (lbl_eu_806624A0) and to 1.0.
    float cur = lbl_eu_806642E4;
    if (cur != zero) {
        float max = lbl_eu_806624A0;
        float v = lbl_eu_8066249C + cur;
        lbl_eu_8066249C = v;
        if (v < max) {
            v = max;
            lbl_eu_8066249C = max;
            lbl_eu_806642E4 = zero;
        } else if (v >= lbl_eu_80667A0C) {
            v = lbl_eu_80667A0C;
            lbl_eu_8066249C = lbl_eu_80667A0C;
            lbl_eu_806642E4 = zero;
        }
        SoundSlot* s1 = sptr(1);
        if (s1 != nullptr && (u32)s1->handle != 0xFFFFFFFF) {
            if ((u32)s1->handle != 0xFFFFFFFF) {
                func_80459A94__7CLibCriFv(s1->handle, v);
            }
        }
        SoundSlot* s0 = sptr(0);
        if (s0 != nullptr && (u32)s0->handle != 0xFFFFFFFF) {
            if ((u32)s0->handle != 0xFFFFFFFF) {
                func_80459A94__7CLibCriFv(s0->handle, lbl_eu_8066249C);
            }
        }
    }
}

extern "C" s32 func_80189A04(s32 index) {
    if (index == 0) {
        // Busy if any SE slot (2..4) still holds a live handle.
        SoundSlot* s2 = sptr(2);
        if (s2 != nullptr && (u32)s2->handle != 0xFFFFFFFF) return 0;
        SoundSlot* s3 = sptr(3);
        if (s3 != nullptr && (u32)s3->handle != 0xFFFFFFFF) return 0;
        SoundSlot* s4 = sptr(4);
        if (s4 != nullptr && (u32)s4->handle != 0xFFFFFFFF) return 0;
    } else {
        // index is really a sound-name string here: format it into a full
        // path and search the slots for a live match.
        const char* name = (const char*)index;
        ml::FixStr<64> str;
        bool hasDot = strstr(name, &lbl_eu_80503AB0[0x00]) != nullptr;
        bool hasSlash = strstr(name, &lbl_eu_80503AB0[0x02]) != nullptr;
        const char* mid = hasDot ? &lbl_eu_80503AB0[0x04] : &lbl_eu_80503AB0[0x05];
        const char* prefix = hasSlash ? &lbl_eu_80503AB0[0x04] : &lbl_eu_80503AB0[0x11];
        str.format(&lbl_eu_80503AB0[0x0a], prefix, mid, name);
        s32 i;
        SoundSlot* cur = (SoundSlot*)lbl_eu_80663E60;
        if (cur == nullptr) goto notFound;
        for (i = 0; i <= 4; i++, cur++) {
            if ((u32)cur->handle == 0xFFFFFFFF) continue;
            bool matched = (strcmp(cur->name, str.mString) == 0);
            if (!matched) continue;
            goto found;
        }
    notFound:
        i = -1;
    found:
        if (i != -1) {
            SoundSlot* s = sptr(i);
            if ((u32)s->handle != 0xFFFFFFFF) return 0;
        }
    }
    return 1;
}

// STUBS for complex functions
s32 func_80187F14(SoundSlot* slot, SoundPlayParams* params, s32 flag) { return 0; }
s32 func_80188B80(s32 index, s32 name, float f1, float f2, s32 flag) {
    if (index == -1) return 0;
    const char* strName = (const char*)name;
    ml::FixStr<64> str;
    bool hasDot = strstr(strName, &lbl_eu_80503AB0[0x00]) != nullptr;
    bool hasSlash = strstr(strName, &lbl_eu_80503AB0[0x02]) != nullptr;
    const char* mid = hasDot ? &lbl_eu_80503AB0[0x04] : &lbl_eu_80503AB0[0x05];
    const char* prefix = hasSlash ? &lbl_eu_80503AB0[0x04] : &lbl_eu_80503AB0[0x11];
    str.format(&lbl_eu_80503AB0[0x0a], prefix, mid, strName);

    // Build the play-request param block for func_80187F14.
    SoundPlayParams params;
    params.handle = -1;
    params.vol2 = lbl_eu_80667A0C;
    params.vol3 = lbl_eu_80667A0C;
    params.vol1 = lbl_eu_80667A08;
    params.name = &str;
    params.field_0x14 = 2;
    params.field_0x16 = 1;
    params.handle = mtl::MemManager::getHandleMEM2();
    params.vol1 = f1;
    params.vol2 = f2;
    s32 inRange = 0;
    if (index >= 0) {
        if (index <= 1) {
            inRange = 1;
        }
    }
    if (inRange) params.vol3 = lbl_eu_80662490 * lbl_eu_80662494;
    else params.vol3 = lbl_eu_80662498;

    s32 result = 0;
    SoundSlot* s = sptr(index);
    if (s != nullptr) {
        result = (func_80187F14(s, &params, flag) != -1) ? 1 : 0;
    }
    return result;
}
void func_80188D34() {}
void func_80189034() {}
void func_8018986C(const char* name, float vol) {
    if (name != nullptr) {
        // Build the full sound path from the input name.
        ml::FixStr<64> str;
        bool hasDot = strstr(name, &lbl_eu_80503AB0[0x00]) != nullptr;
        bool hasSlash = strstr(name, &lbl_eu_80503AB0[0x02]) != nullptr;
        const char* mid = hasDot ? &lbl_eu_80503AB0[0x04] : &lbl_eu_80503AB0[0x05];
        const char* prefix = hasSlash ? &lbl_eu_80503AB0[0x04] : &lbl_eu_80503AB0[0x11];
        str.format(&lbl_eu_80503AB0[0x0a], prefix, mid, name);
        // Find the slot whose live name matches the formatted path.
        s32 i;
        SoundSlot* cur = (SoundSlot*)lbl_eu_80663E60;
        if (cur == nullptr) goto notFound;
        for (i = 0; i <= 4; i++, cur++) {
            if ((u32)cur->handle == 0xFFFFFFFF) continue;
            bool matched = (strcmp(cur->name, str.mString) == 0);
            if (!matched) continue;
            goto found;
        }
    notFound:
        i = -1;
    found:
        if (i != -1) {
            SoundSlot* s = sptr(i);
            if (s != nullptr) func_801882AC(s, vol, 2);
        }
    } else {
        // No name: stop/clear the three SE slots (2..4).
        for (s32 i = 2; i <= 4; i++) {
            SoundSlot* s = sptr(i);
            if (s != nullptr) func_801882AC(s, vol, 2);
        }
    }
}
extern "C" s32 func_801887C8(u32 wantId, s32 startIdx, s32 endIdx) {
    s32* b = lbl_eu_80663E60;
    if (b != nullptr) {
        // Pass 1: return the first slot in [startIdx..endIdx] whose handle
        // is free (== -1).
        for (s32 i = startIdx; i <= endIdx; i++) {
            SoundSlot* s = (SoundSlot*)((u8*)b + i * 0xB8);
            if ((u32)s->handle == 0xFFFFFFFF) return i;
        }
        // Pass 2: otherwise return the first slot whose priority
        // (field_0x54) is below wantId, stopping its sound first.
        for (s32 i = startIdx; i <= endIdx; i++) {
            SoundSlot* s = (SoundSlot*)((u8*)b + i * 0xB8);
            if (wantId > s->field_0x54) {
                func_801882AC(s, lbl_eu_80667A08, 2);
                return i;
            }
        }
    }
    return -1;
}
// Slot index 0..1 plays the master-volume product, 2..4 use the backup
// volume. type and the two floats are forwarded from the caller.
void func_8018896C(s32 index, u32 type, float f1, float f2) {
    float vol;
    // Materialize the range test into a flag via two separate signed checks.
    // A bare `index >= 0 && index <= 1` collapses into a single unsigned
    // cmpli, but retail keeps both signed cmpwi (li r0,0 default + li r0,1).
    s32 inRange = 0;
    if (index >= 0) {
        if (index <= 1) {
            inRange = 1;
        }
    }
    if (inRange) vol = lbl_eu_80662490 * lbl_eu_80662494;
    else vol = lbl_eu_80662498;
    s32* b = lbl_eu_80663E60;
    SoundSlot* s;
    if (b != nullptr) s = (SoundSlot*)((u8*)b + index * 0xB8);
    else s = nullptr;
    if (s != nullptr) func_80188488(s, type, f1, vol, f2);
}

extern "C" s32 func_801897A0(s32 wantId, s32 type, float f1) {
    if (func_8008585C__Q22cf13CfGameManagerFv()) return 0;
    if ((lbl_eu_80663E24 & 0x400000) != 0 && (lbl_eu_80663E24 & 0x20000) == 0) return 0;
    if (func_80189A04(wantId) == 0) return 0;
    s32 slot = func_801887C8(type, 2, 4);
    if (func_80188B80(slot, wantId, f1, lbl_eu_80667A0C, 0) != 0) return slot;
    return -1;
}
void func_80189F20() {}
void func_80189F84() {}
void func_8018A134() {}
void func_8018A2E8() {}
void func_8018A5E8() {}
void func_8018A9D8() {}
void func_8018AA04() {}
void func_8018AAC4() {}
void func_8018AD54() {}
void func_8018AE20() {}
void func_8018AFB8() {}
void func_8018B1A8() {}
void func_8018B1E8() {}
void func_8018B1F4() {}
void func_8018B224() {}
void func_8018B230() {}
void func_8018B23C() {}

extern "C" s32 func_80189450() {
    // When the flag bit is set, refuse (return 0) if any of the two name
    // buffers (slot 0 / slot 1) lacks the substring at 80503AB0+0x1A while
    // its slot still holds a live handle; otherwise clear the flag and allow.
    if ((lbl_eu_806642E0 & 1) != 0) {
        s32 i = 0;
        s32 off = 0;
        SoundSlot* s;
        while (i <= 1) {
            s32* b = lbl_eu_80663E60;
            if (b != nullptr) s = (SoundSlot*)((u8*)b + off);
            else s = nullptr;
            if (s != nullptr) {
                const char* name = (i != 0) ? (const char*)lbl_eu_805757BC
                                            : (const char*)lbl_eu_80575798;
                if (strstr(name, &lbl_eu_80503AB0[0x1a]) != nullptr) goto next;
                if ((u32)s->handle != 0xFFFFFFFF) return 0;
            }
        next:
            i++;
            off += 0xB8;
        }
        lbl_eu_806642E0 &= 0xFFFE;
    }
    return 1;
}
