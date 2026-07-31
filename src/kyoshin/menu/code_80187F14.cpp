// Auto-scaffolded catalog TU for kyoshin/menu/code_80187F14
// Sound effect management system for menu UI

#include <string.h>
#include "kyoshin/harness_catalog.hpp"

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
    extern float lbl_eu_80667A08;
    extern float lbl_eu_80667A0C;
    extern float lbl_eu_80667A10;
    extern float lbl_eu_80667A14;
    extern float lbl_eu_80667A18;
    extern char lbl_eu_80503AB0[];
}

extern s32 func_80459A78__7CLibCriFv(s32);
extern s32 func_80459A7C__7CLibCriFv(s32);
extern void func_80459A84__7CLibCriFv(s32, s32);
extern void func_80459A88__7CLibCriFv(s32, float, float, s32);
extern void func_80459A8C__7CLibCriFv(s32);
extern void func_80459A90__7CLibCriFv(s32);
extern void func_80459A94__7CLibCriFv(s32, float);
extern s32 func_80459A9C__7CLibCriFv(s32);
extern s32 func_8045997C__7CLibCriFv(const char*, s32, s32);
extern void getInstance__7CLibCriFv();

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
    if (b == nullptr) return nullptr;
    return (SoundSlot*)((u8*)b + i * 0xB8);
}

static inline s32 isValid(s32 h) { return h != -1; }

// --- FORWARD DECLARATIONS ---
void func_801882AC(SoundSlot*, float, u32);
void func_80188488(SoundSlot*, u32, float, float, float);
s32 func_801887C8(u32, u32, u32);
s32 func_80188B80(s32, const char*, float, float, s32);
void func_801889D0();
void func_8018986C(s32, float);
s32 func_80189A04(s32);

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
    if (s == nullptr) return 1;
    s32 h = s->handle;
    if (h == -1) return 1;
    return 0;
}

s32 func_8018892C(s32 index) {
    s32* b = lbl_eu_80663E60;
    SoundSlot* s;
    if (b != nullptr) s = (SoundSlot*)((u8*)b + index * 0xB8);
    else s = nullptr;
    if (s == nullptr) return 0;
    s32 h = s->handle;
    if (h == -1) return 0;
    return 1;
}

s32 func_80189C40(s32 index) {
    s32* b = lbl_eu_80663E60;
    SoundSlot* s;
    if (b != nullptr) s = (SoundSlot*)((u8*)b + index * 0xB8);
    else s = nullptr;
    if (s == nullptr) return 0;
    return func_80459A9C__7CLibCriFv(s->handle);
}

void func_80189424() {
    s32* b = lbl_eu_80663E60;
    SoundSlot* s;
    if (b != nullptr) s = (SoundSlot*)((u8*)b + 1 * 0xB8);
    else s = nullptr;
    if (s != nullptr) func_801882AC(s, 0.0f, 2);
}

void func_80188774(SoundSlot* slot) {
    for (s32 i = 0; i < 5; i++) {
        func_801882AC(slot, 0.0f, 2);
        slot = (SoundSlot*)((u8*)slot + 0xB8);
    }
}

void func_80188890(s32 active) {
    for (s32 i = 0; i < 5; i++) {
        SoundSlot* s = sptr(i);
        if (s != nullptr && isValid(s->handle)) {
            func_80459A84__7CLibCriFv(s->handle, active);
            if (active != 0) s->status = 5;
            else s->status = 2;
        }
    }
}

void func_80189318(s32 clearName) {
    SoundSlot* s0 = sptr(0);
    if (s0 != nullptr) func_801882AC(s0, 0.0f, 2);
    if (clearName != 0) {
        if (lbl_eu_80575798[8] != 0) {
            ((char*)lbl_eu_80575798)[0] = '\0';
            lbl_eu_80575798[8] = 0;
        }
    }
}

void func_80189390(const char* name) {
    SoundSlot* s0 = sptr(0);
    if (s0 != nullptr) func_801882AC(s0, 0.0f, 2);
    if (name != nullptr) {
        lbl_eu_80575798[8] = strlen(name);
        strcpy((char*)lbl_eu_80575798, name);
    } else {
        ((char*)lbl_eu_80575798)[0] = '\0';
        lbl_eu_80575798[8] = 0;
    }
}

void func_80188488(SoundSlot* slot, u32 type, float f1, float f2, float f3) {
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
        float vol;
        if (i >= 0 && i <= 1) vol = lbl_eu_80662490 * lbl_eu_80662494;
        else vol = lbl_eu_80662498;
        SoundSlot* s = sptr(i);
        if (s != nullptr && isValid(s->handle)) func_80188488(s, 0, 0.0f, vol, s->x);
    }
}

void func_801895F4(float f1) {
    lbl_eu_80662498 = f1;
    for (s32 i = 2; i <= 4; i++) {
        float vol;
        if (i >= 0 && i <= 1) vol = lbl_eu_80662490 * lbl_eu_80662494;
        else vol = lbl_eu_80662498;
        SoundSlot* s = sptr(i);
        if (s != nullptr) func_80188488(s, 0, 1.0f, vol, 0.0f);
    }
}

void func_801882AC(SoundSlot* slot, float vol, u32 type) {
    getInstance__7CLibCriFv();
    if (vol == 0.0f) {
        if (isValid(slot->backupHandle)) {
            func_80459A7C__7CLibCriFv(slot->backupHandle);
            slot->backupHandle = -1;
            slot->backupU16_1 = 0;
            slot->backupU16_2 = 0;
            slot->backupU16_3 = 0;
            slot->backupF1 = 0.0f;
        }
    }
    if (isValid(slot->handle)) {
        slot->backupHandle = -1;
        slot->backupU16_1 = 0;
        slot->backupU16_2 = 0;
        slot->backupU16_3 = 0;
        slot->backupF1 = 0.0f;
        slot->x = 0.0f;
        if (vol <= 0.0f && type != 1) {
            func_80459A7C__7CLibCriFv(slot->handle);
            slot->handle = -1;
            slot->field_0x54 = 0;
            slot->field_0x56 = 0;
            slot->status = 0;
            slot->field_0x48 = 0.0f;
            return;
        }
        func_80459A88__7CLibCriFv(slot->handle, 0.0f, vol, type);
        if (type == 2) {
            if (isValid(slot->backupHandle)) func_80459A7C__7CLibCriFv(slot->backupHandle);
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
            slot->field_0x48 = 0.0f;
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

void func_80188584(SoundSlot* slot) {
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

void func_801889D0() {
    for (s32 i = 0; i < 5; i++) {
        SoundSlot* s = sptr(i);
        if (s == nullptr) continue;
        if (isValid(s->backupHandle)) {
            if (func_80459A78__7CLibCriFv(s->backupHandle) != 0) {
                s->backupHandle = -1;
                s->backupU16_1 = 0;
                s->backupU16_2 = 0;
                s->backupU16_3 = 0;
                s->backupF1 = 0.0f;
            }
        }
        if (isValid(s->handle)) {
            if (func_80459A78__7CLibCriFv(s->handle) != 0) {
                s->handle = -1;
                s->field_0x54 = 0;
                s->field_0x56 = 0;
                s->status = 0;
                s->field_0x48 = 0.0f;
            }
        }
    }
    float cur = lbl_eu_806642E4;
    if (cur != 0.0f) {
        lbl_eu_8066249C = lbl_eu_8066249C + cur;
        float max = lbl_eu_806624A0;
        if (lbl_eu_8066249C >= max) {
            lbl_eu_8066249C = max;
            lbl_eu_806642E4 = 0.0f;
        } else if (lbl_eu_8066249C >= 1.0f) {
            lbl_eu_8066249C = 1.0f;
            lbl_eu_806642E4 = 0.0f;
        }
        SoundSlot* s1 = sptr(1);
        if (s1 != nullptr && isValid(s1->handle)) func_80459A94__7CLibCriFv(s1->handle, lbl_eu_8066249C);
        SoundSlot* s0 = sptr(0);
        if (s0 != nullptr && isValid(s0->handle)) func_80459A94__7CLibCriFv(s0->handle, lbl_eu_8066249C);
    }
}

s32 func_80189A04(s32 index) {
    if (index == 0) {
        SoundSlot* s2 = sptr(2);
        if (s2 != nullptr && isValid(s2->handle)) return 0;
        SoundSlot* s3 = sptr(3);
        if (s3 != nullptr && isValid(s3->handle)) return 0;
        SoundSlot* s4 = sptr(4);
        if (s4 != nullptr && isValid(s4->handle)) return 0;
        return 1;
    }
    return 0;
}

// STUBS for complex functions
void func_80187F14() {}
s32 func_80188B80(s32 index, const char* name, float f1, float f2, s32 flag) { return 0; }
void func_80188D34() {}
void func_80189034() {}
void func_8018986C(s32 index, float f1) {}
s32 func_801887C8(u32 wantId, u32 startIdx, u32 endIdx) { return -1; }
void func_8018896C() {}
void func_801897A0() {}
void func_80189F20() {}
void func_80189F84() {}
void func_8018A134() {}
void func_8018A2E8() {}
void func_8018A5E8() {}
void func_8018A8CC() {}
void func_8018A944() {}
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
void func_8018B31C() {}